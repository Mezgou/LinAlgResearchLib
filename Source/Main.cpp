#include "Test.h"

int main() {
    int resultTests = 0;
#ifndef NDEBUG
    resultTests = RunTests();
#endif
    std::cout << "Linear Algebra Research Library!\n";
    return resultTests;
}