#include "ifs.h"
#include <cassert>
#include <cstring>

int main(int argc, char *argv[])
{
    assert(argc == 11);
    assert(std::strcmp(argv[1], "-input") == 0);
    assert(std::strcmp(argv[3], "-points") == 0);
    assert(std::strcmp(argv[5], "-iters") == 0);
    assert(std::strcmp(argv[7], "-size") == 0);
    assert(std::strcmp(argv[9], "-output") == 0);
    IFS ifs;
    ifs.Initialize(argv[2]);
    Image img(std::atoi(argv[8]), std::atoi(argv[8]));
    ifs.Render(img, std::atoi(argv[4]), std::atoi(argv[6]));
    img.SaveTGA(argv[10]);
}