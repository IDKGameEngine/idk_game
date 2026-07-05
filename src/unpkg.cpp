#include "idk_core/file.hpp"
#include "idk_core/metric.hpp"
#include "idk_core/IdkPkg.hpp"

#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    std::string inpath = "";
    bool inpathGood = false;

    for (int i=1; i<argc; i++)
    {
        if ((i+1 < argc) && (std::string(argv[i]) == "-i"))
        {
            inpath = std::string(argv[i+1]);
            inpathGood = true;
        }
    }

    if (inpathGood == false)
    {
        fprintf(stderr, "Must provide input (-i INPUT_FILE)\n");
        exit(1);
    }

    idk::FileReader fileRd(inpath.c_str());
    idk::core::PkgReader pkgRd(fileRd.getData(), fileRd.getSize());

    for (auto [name, data, size]: pkgRd)
    {
        fs::create_directories(fs::path(name).parent_path());
        idk::FileWriter fileWt(name);
        fileWt.write(data, size);
        // printf("FILE %s, data, size: 0x%lx, %lu\n", name, (uint64_t)data, size);
    }

    return 0;
}
