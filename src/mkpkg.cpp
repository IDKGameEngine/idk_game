#include "idk_core/file.hpp"
#include "idk_core/metric.hpp"
#include "idk_core/IdkPkg.hpp"

int main(int argc, char **argv)
{
    static idk::core::PkgWriter<512*idk::MEGA> pkgWt;
    std::string outpath = "out.pkg";

    for (int i=1; i<argc; i++)
    {
        if ((i+1 < argc) && (std::string(argv[i]) == "-o"))
        {
            outpath = std::string(argv[i+1]);
            i += 1;
            continue;
        }

        const char *filepath = argv[i];
        idk::FileReader fileRd(filepath);

        if (!pkgWt.writeFile(filepath, fileRd.getData(), fileRd.getSize()))
        {
            fprintf(stderr, "Error packaging file \"%s\"\n", filepath);
            exit(1);
        }
    }

    idk::FileWriter fileWt(outpath.c_str());
    fileWt.write(pkgWt.getData(), pkgWt.getSize());

    return 0;
}
