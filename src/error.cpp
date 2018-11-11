#include <iostream>
#include "error.h"

void errorPrint(const std::string& msg, const std::string& leftDisp) {
    std::cerr << leftDisp << msg << std::endl;
}
