#pragma once

#include "idk/level/Level.hpp"
#include "idk/network/Network.hpp"

namespace mygame
{
    inline constexpr idk::net::ResourceId DemoModel = 1, PlayerModel = 2, Completed = 3, SceneBase = 100;
    inline constexpr uint32_t Jump = 1, Sprint = 2, Restart = 4;

    // A bounded, deterministic list of the initial level and its exit chain.
    // Clients resolve scene IDs locally; paths never come from the network.
    class Content
    {
    public:
        static constexpr size_t MaxLevels = 16;
        bool initialize(const char *initial, const char *root, const char *demo, idk::level::Error &);
        bool load(size_t index, idk::Arena &, idk::level::Level &, idk::level::Error &) const;
        bool isLevel() const { return mCount != 0; }
        uint64_t manifest() const { return mManifest; }
        size_t count() const { return mCount; }
        int next(size_t index) const { return index < mCount ? mEntries[index].next : -1; }
        const char *demoPath() const { return mDemo; }

    private:
        struct Entry { char path[4096]{}; uint64_t hash = 0; int next = -1; };
        Entry mEntries[MaxLevels]{};
        size_t mCount = 0;
        uint64_t mManifest = 0;
        char mDemo[4096]{};
    };
}
