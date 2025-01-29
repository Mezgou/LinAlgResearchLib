#pragma once

#include <iostream>

#include "TestSparseMatrix.h"
#include "Utils.h"

static int RunTests() {
#ifdef _WIN32
    EnableAnsiSupport();
#endif
    if (CU_initialize_registry() != CUE_SUCCESS) {
        std::cerr << RED << "Failed to initialize the test registry!" << RESET << std::endl;
        return CU_get_error();
    }
    CU_pSuite suite = CU_add_suite("Matrix and SparseMatrix Tests", nullptr, nullptr);
    if (!suite) {
        CU_cleanup_registry();
        std::cerr << RED << "Error adding the test set!" << RESET << std::endl;
        return CU_get_error();
    }

    if (!CU_add_test(suite, "TestGetElement", TestGetElement) ||
        !CU_add_test(suite, "TestGetTrace", TestGetTrace) ||
        !CU_add_test(suite, "TestGetStd", TestGetStandardMatrix) ||
        !CU_add_test(suite, "TestConvertToCSR", TestConvertToCSR) ||
        !CU_add_test(suite, "TestMatrixAddition", TestMatrixAddition) ||
        !CU_add_test(suite, "TestScalarMultiplication", TestScalarMultiplication) ||
        !CU_add_test(suite, "TestMatrixMultiplication", TestMatrixMultiplication) ||
        !CU_add_test(suite, "TestGetDeterminant", TestGetDeterminant)) {
        CU_cleanup_registry();
        std::cerr << RED << "Error adding tests!" << RESET << std::endl;
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_NORMAL);
    std::cout << CYAN << "Run tests..." << RESET << std::endl;
    CU_basic_run_tests();

    PrintTestResult(suite);
    CU_cleanup_registry();
    return 0;
}