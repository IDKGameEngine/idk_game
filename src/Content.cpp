#include "Content.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{
    constexpr uint64_t Seed = 14695981039346656037ULL;
    void hashBytes(uint64_t &hash, const void *data, size_t count)
    {
        auto *bytes = static_cast<const unsigned char *>(data);
        for (size_t i = 0; i < count; ++i) { hash ^= bytes[i]; hash *= 1099511628211ULL; }
    }
    bool fileHash(const char *path, uint64_t &hash, idk::level::Error &error)
    {
        auto *file = std::fopen(path, "rb");
        if (!file) return error.set("Could not open game content file.");
        hash = Seed;
        unsigned char bytes[4096];
        size_t count;
        while ((count = std::fread(bytes, 1, sizeof(bytes), file))) hashBytes(hash, bytes, count);
        bool good = !std::ferror(file);
        good = std::fclose(file) == 0 && good;
        return good || error.set("Could not read game content file.");
    }
}

bool mygame::Content::initialize(const char *initial, const char *root, const char *demo, idk::level::Error &error)
{
    mCount = 0;
    mManifest = Seed;
    // Includes gameplay/input semantics. Change this when those become incompatible.
    const char schema[] = "idk-game-authority-1";
    hashBytes(mManifest, schema, sizeof(schema));
    if (!initial)
    {
        uint64_t hash;
        if (!demo || !realpath(demo, mDemo) || !fileHash(mDemo, hash, error))
            return error.set("Could not read the demo model. Install the game content first.");
        // Feed numbers in a defined order rather than hashing native layouts.
        for (unsigned i = 0; i < 8; ++i) { uint8_t b = uint8_t(hash >> (i * 8)); hashBytes(mManifest, &b, 1); }
        return mManifest != 0;
    }
    char base[4096];
    if (!root || !realpath(root, base) || !realpath(initial, mEntries[0].path))
        return error.set("Could not resolve initial level or level root.");
    mCount = 1;
    alignas(16) static std::byte storage[4 * 1024 * 1024];
    for (size_t i = 0; i < mCount; ++i)
    {
        auto &entry = mEntries[i];
        entry.next = -1;
        if (!fileHash(entry.path, entry.hash, error)) return false;
        idk::Arena arena(storage);
        idk::level::Level level;
        if (!load(i, arena, level, error)) return false;
        if (level.destination.path[0])
        {
            char path[4096], absolute[4096];
            int size = std::snprintf(path, sizeof(path), "%s/%s", base, level.destination.path);
            if (size < 0 || size_t(size) >= sizeof(path) || !realpath(path, absolute))
                return error.set("A linked destination level is missing. Build the entire level chain before hosting.");
            size_t length = std::strlen(base);
            if (std::strcmp(base, "/") && (std::strncmp(absolute, base, length) || absolute[length] != '/'))
                return error.set("A destination level resolves outside the level root.");
            for (size_t j = 0; j < mCount; ++j)
                if (!std::strcmp(absolute, mEntries[j].path)) entry.next = int(j);
            if (entry.next < 0)
            {
                if (mCount == MaxLevels) return error.set("The level chain exceeds 16 unique levels.");
                entry.next = int(mCount);
                std::snprintf(mEntries[mCount++].path, sizeof(Entry::path), "%s", absolute);
            }
        }
        for (unsigned byte = 0; byte < 8; ++byte)
        {
            uint8_t value = uint8_t(entry.hash >> (byte * 8));
            hashBytes(mManifest, &value, 1);
        }
        uint8_t destination = uint8_t(entry.next + 1);
        hashBytes(mManifest, &destination, 1);
    }
    return mManifest != 0;
}

bool mygame::Content::load(size_t index, idk::Arena &arena, idk::level::Level &level, idk::level::Error &error) const
{
    if (index >= mCount) return error.set("Unknown scene resource ID.");
    const auto &entry = mEntries[index];
    uint64_t before = 0, after = 0;
    if (!fileHash(entry.path, before, error) || before != entry.hash)
        return error.set("Level content changed since session startup. Restart with matching content.");
    if (!idk::level::Load(entry.path, arena, level, error)) return false;
    if (!fileHash(entry.path, after, error) || after != entry.hash)
        return error.set("Level content changed during loading.");
    return true;
}
