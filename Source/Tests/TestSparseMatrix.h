#pragma once
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "SparseMatrix.h"

static void TestConvertToCSR() {
    std::vector<std::vector<double>> inputMatrix = {
            {5, 0, 0},
            {0, 8, 0},
            {3, 0, 6}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    std::vector<double> expectedValues = {5, 8, 3, 6};
    std::vector<size_t> expectedColumnIdx = {0, 1, 0, 2};
    std::vector<size_t> expectedRowPtr = {0, 1, 2, 4};
    CU_ASSERT_EQUAL(matrix.GetValues().size(), expectedValues.size());
    for (size_t i = 0; i < expectedValues.size(); i++) {
        CU_ASSERT_DOUBLE_EQUAL(matrix.GetValues()[i], expectedValues[i], 1e-9);
        CU_ASSERT_EQUAL(matrix.GetColsIdx()[i], expectedColumnIdx[i]);
    }
    CU_ASSERT_EQUAL(matrix.GetRowPtr().size(), expectedRowPtr.size());
    for (size_t i = 0; i < expectedRowPtr.size(); i++) {
        CU_ASSERT_EQUAL(matrix.GetRowPtr()[i], expectedRowPtr[i]);
    }
}

static void TestGetTrace() {
    std::vector<std::vector<double>> inputMatrix = {
            {5, 0, 0},
            {0, 8, 0},
            {3, 0, 6}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    double trace = matrix.GetTrace();
    CU_ASSERT_DOUBLE_EQUAL(trace, 19.0, 1e-9);
}

static void TestGetElement() {
    std::vector<std::vector<double>> inputMatrix = {
            {5, 0, 0},
            {0, 8, 0},
            {3, 0, 6}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    CU_ASSERT_DOUBLE_EQUAL(matrix(0, 0), 5.0, 1e-9);
    CU_ASSERT_DOUBLE_EQUAL(matrix(1, 1), 8.0, 1e-9);
    CU_ASSERT_DOUBLE_EQUAL(matrix(2, 2), 6.0, 1e-9);
    CU_ASSERT_DOUBLE_EQUAL(matrix(0, 1), 0.0, 1e-9);
}

static void TestGetStandardMatrix() {
    std::vector<std::vector<double>> inputMatrix = {
            {5, 0, 0},
            {0, 8, 0},
            {3, 0, 6}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    Matrix<double> basicMatrix = matrix.GetStandardMatrix();
    for (size_t i = 0; i < inputMatrix.size(); i++) {
        for (size_t j = 0; j < inputMatrix[0].size(); j++) {
            CU_ASSERT_DOUBLE_EQUAL(basicMatrix(i, j), inputMatrix[i][j], 1e-9);
        }
    }
}

static void TestMatrixAddition() {
    std::vector<std::vector<double>> firstMatrix = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };
    std::vector<std::vector<double>> secondMatrix = {
            {9, 8, 7},
            {6, 5, 4},
            {3, 2, 1}
    };
    SparseMatrix<double> mat1 = SparseMatrix<double>(Matrix<double>(firstMatrix));
    SparseMatrix<double> mat2 = SparseMatrix<double>(Matrix<double>(secondMatrix));
    SparseMatrix<double> result = mat1 + mat2;
    std::vector<std::vector<double>> expected = {
            {10, 10, 10},
            {10, 10, 10},
            {10, 10, 10}
    };
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(result(i, j), expected[i][j], 1e-9);
        }
    }
}

static void TestScalarMultiplication() {
    std::vector<std::vector<double>> inputMatrix = {
            {1, 0, 0},
            {0, 2, 0},
            {0, 0, 3}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    matrix *= 2.0;
    CU_ASSERT_DOUBLE_EQUAL(matrix(0, 0), 2.0, 1e-9);
    CU_ASSERT_DOUBLE_EQUAL(matrix(1, 1), 4.0, 1e-9);
    CU_ASSERT_DOUBLE_EQUAL(matrix(2, 2), 6.0, 1e-9);
}

static void TestMatrixMultiplication() {
    std::vector<std::vector<double>> firstMatrix = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
    };
    std::vector<std::vector<double>> secondMatrix = {
            {9, 8, 7},
            {6, 5, 4},
            {3, 2, 1}
    };
    SparseMatrix<double> mat1 = SparseMatrix<double>(Matrix<double>(firstMatrix));
    SparseMatrix<double> mat2 = SparseMatrix<double>(Matrix<double>(secondMatrix));
    SparseMatrix<double> result = mat1 * mat2;
    std::vector<std::vector<double>> expected = {
            {30, 24, 18},
            {84, 69, 54},
            {138, 114, 90}
    };
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(result(i, j), expected[i][j], 1e-9);
        }
    }
}

static void TestGetDeterminant() {
    std::vector<std::vector<double>> inputMatrix = {
            {2, 3, 1},
            {4, 1, 3},
            {3, 2, 4}
    };
    SparseMatrix<double> matrix = SparseMatrix<double>(Matrix<double>(inputMatrix));
    const double determinant = matrix.GetDeterminant();
    CU_ASSERT_DOUBLE_EQUAL(determinant, -20.0, 1e-9);
}