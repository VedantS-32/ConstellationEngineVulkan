#include "CStellApplication.h"

#include <stdexcept>
#include <cstdlib>

int main()
{
    CStell::CStellApplication Instance0;

    try {
        Instance0.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}