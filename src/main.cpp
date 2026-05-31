#include "core/FWECore.h"
#include <iostream>

/**
 * FWE — Flow Works Engine
 * Entry point
 */

int main(int argc, char* argv[]) {
    fwe::FWECore core;

    if (!core.initialize()) {
        std::cerr << "[FWE] Core initialization failed.\n";
        return 1;
    }

    // Qt outer shell + Dear ImGui viewport launch
    // TODO: initialize UI layer

    core.shutdown();
    return 0;
}
