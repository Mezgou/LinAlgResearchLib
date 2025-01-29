#pragma once

#include <cstdint>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"

#define LOG(title, x) std::cout << title << ": " << x << "\n"

#ifdef _WIN32
#include <windows.h>

static void EnableAnsiSupport() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

static void PrintTestResult(const CU_pSuite& suite) {
    uint32_t failures = CU_get_number_of_failures();
    uint32_t tests = CU_get_number_of_tests_run();
    uint32_t passed = tests - failures;

    std::cout << CYAN << "----------------------------------------" << RESET << std::endl;
    std::cout << CYAN << "Recruitment Tests: " << suite->pName << RESET << std::endl;
    std::cout << "Total tests: " << tests << std::endl;
    std::cout << GREEN << "Successful: " << passed << RESET << std::endl;
    if (failures > 0) {
        std::cout << RED << "Failed: " << failures << RESET << std::endl;
    } else {
        std::cout << GREEN << "All tests have been successfully passed!" << RESET << std::endl;
    }
    std::cout << CYAN << "----------------------------------------" << RESET << std::endl;
}