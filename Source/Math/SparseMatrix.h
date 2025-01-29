#pragma once

#include "Matrix.h"

template<typename T>
class SparseMatrix {
public:
    explicit SparseMatrix(const Matrix<T>& matrix)
        : m_IsSquare(matrix.IsSquare()), m_Rows(matrix.GetRows()), m_Cols(matrix.GetCols()) {
        ConvertToCSR(matrix);
    }

    explicit SparseMatrix(const std::vector<T>& values, const std::vector<size_t>& colsIdx,
        const std::vector<size_t>& rowPtr, const size_t rows, const size_t cols)
        : m_Values(values), m_ColsIdx(colsIdx), m_RowPtr(rowPtr),
          m_IsSquare(rows == cols), m_Rows(rows), m_Cols(cols) {}

    ~SparseMatrix() = default;

    T operator()(size_t row, size_t col) const {
        if (row >= m_Rows || col >= m_Cols) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t cIdx = m_RowPtr[row]; cIdx < m_RowPtr[row + 1]; cIdx++) {
            if (m_ColsIdx[cIdx] == col) {
                return m_Values[cIdx];
            }
        }
        return 0;
    }

    Matrix<T> GetStandardMatrix() const {
        Matrix<T> matrix(m_Rows, m_Cols, 0);
        for (size_t cRow = 0; cRow < m_Rows; cRow++) {
            for (size_t cIdx = m_RowPtr[cRow]; cIdx < m_RowPtr[cRow + 1]; cIdx++) {
                matrix(cRow, m_ColsIdx[cIdx]) = m_Values[cIdx];
            }
        }
        return matrix;
    }

    T GetTrace() const {
        T trace = 0;
        if (!m_IsSquare) {
            throw std::invalid_argument("It is not a square matrix!");
        }
        for (size_t cRow = 0; cRow < m_Rows; cRow++) {
            for (size_t cIdx = m_RowPtr[cRow]; cIdx < m_RowPtr[cRow + 1]; cIdx++) {
                if (m_ColsIdx[cIdx] == cRow) {
                    trace += m_Values[cIdx];
                    break;
                }
            }
        }
        return trace;
    }

    T GetDeterminant() const {
        if (!m_IsSquare) {
            throw std::invalid_argument("It is not a square matrix!");
        }
        return DeterminantRecursive(GetStandardMatrix());
    }

    void operator*=(const T scalar) {
        for (size_t cIdx = 0; cIdx < m_Values.size(); cIdx++) {
            m_Values[cIdx] *= scalar;
        }
    }

    SparseMatrix<T> operator+(const SparseMatrix<T>& other) const {
        if (m_Cols != other.GetCols() || m_Rows != other.GetRows()) {
            throw std::invalid_argument("Matrices cannot be added: inconsistent sizes!");
        }

        std::vector<T> rValues;
        std::vector<size_t> rColsIdx;
        std::vector<size_t> rRowPtr(m_Rows + 1, 0);

        for (size_t cRow = 0; cRow < m_Rows; cRow++) {
            size_t startIdx = m_RowPtr[cRow];
            size_t endIdx = m_RowPtr[cRow + 1];
            size_t otherStartIdx = other.GetRowPtr()[cRow];
            size_t otherEndIdx = other.GetRowPtr()[cRow + 1];

            size_t currentIdx = startIdx;
            size_t otherCurrentIdx = otherStartIdx;

            while (currentIdx < endIdx || otherCurrentIdx < otherEndIdx) {
                if (currentIdx < endIdx && (otherCurrentIdx >= otherEndIdx || m_ColsIdx[currentIdx] < other.GetColsIdx()[otherCurrentIdx])) {
                    rValues.push_back(m_Values[currentIdx]);
                    rColsIdx.push_back(m_ColsIdx[currentIdx]);
                    currentIdx++;
                } else if (otherCurrentIdx < otherEndIdx && (currentIdx >= endIdx || m_ColsIdx[currentIdx] > other.GetColsIdx()[otherCurrentIdx])) {
                    rValues.push_back(other.GetValues()[otherCurrentIdx]);
                    rColsIdx.push_back(other.GetColsIdx()[otherCurrentIdx]);
                    ++otherCurrentIdx;
                } else {
                    rValues.push_back(m_Values[currentIdx] + other.GetValues()[otherCurrentIdx]);
                    rColsIdx.push_back(m_ColsIdx[currentIdx]);
                    currentIdx++;
                    ++otherCurrentIdx;
                }
            }
            rRowPtr[cRow + 1] = rValues.size();
        }

        return SparseMatrix<T>(rValues, rColsIdx, rRowPtr, GetRows(), GetCols());
    }

    SparseMatrix<T> operator*(const SparseMatrix<T>& other) const {
        if (m_Cols != other.GetRows()) {
            throw std::invalid_argument("Matrices cannot be multiplied: inconsistent sizes!");
        }

        std::vector<T> rValues;
        std::vector<size_t> rColsIdx;
        std::vector<size_t> rRowPtr(m_Rows + 1, 0);

        for (size_t cRow = 0; cRow < m_Rows; cRow++) {
            std::vector<T> rowResult(m_Cols, 0);
            size_t startIdx = m_RowPtr[cRow];
            size_t endIdx = m_RowPtr[cRow + 1];

            for (size_t cIdx = startIdx; cIdx < endIdx; cIdx++) {
                T value = m_Values[cIdx];
                size_t col = m_ColsIdx[cIdx];
                size_t otherStartIdx = other.GetRowPtr()[col];
                size_t otherEndIdx = other.GetRowPtr()[col + 1];

                for (size_t otherIdx = otherStartIdx; otherIdx < otherEndIdx; ++otherIdx) {
                    size_t otherCol = other.GetColsIdx()[otherIdx];
                    rowResult[otherCol] += value * other.GetValues()[otherIdx];
                }
            }

            for (size_t cCol = 0; cCol < m_Cols; cCol++) {
                if (rowResult[cCol] != 0) {
                    rValues.push_back(rowResult[cCol]);
                    rColsIdx.push_back(cCol);
                }
            }
            rRowPtr[cRow + 1] = rValues.size();
        }

        return SparseMatrix<T>(rValues, rColsIdx, rRowPtr, GetRows(), other.GetCols());
    }

    size_t GetRows() const { return m_Rows; }
    size_t GetCols() const { return m_Cols; }
    const std::vector<T>& GetValues() const { return m_Values; }
    const std::vector<size_t>& GetColsIdx() const { return m_ColsIdx; }
    const std::vector<size_t>& GetRowPtr() const { return m_RowPtr; }

private:
    void ConvertToCSR(const Matrix<T>& matrix) {
        m_RowPtr.reserve(m_Rows + 1);
        m_RowPtr.push_back(0);

        for (size_t cRow = 0; cRow < m_Rows; cRow++) {
            for (size_t cCol = 0; cCol < m_Cols; cCol++) {
                if (matrix(cRow, cCol) != 0) {
                    m_Values.push_back(matrix(cRow, cCol));
                    m_ColsIdx.push_back(cCol);
                }
            }
            m_RowPtr.push_back(m_Values.size());
        }
    }

    T DeterminantRecursive(const Matrix<T>& matrix) const {
        size_t n = matrix.GetCols();
        if (n == 1) {
            return matrix(0, 0);
        }
        if (n == 2) {
            return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
        }

        T det = 0;
        for (size_t cCol = 0; cCol < n; cCol++) {
            std::vector<std::vector<T>> minorMatrix(n - 1, std::vector<T>(n - 1));

            for (size_t i = 1; i < n; i++) {
                size_t minorCol = 0;
                for (size_t j = 0; j < n; j++) {
                    if (j != cCol) {
                        minorMatrix[i - 1][minorCol++] = matrix(i, j);
                    }
                }
            }

            det += (cCol % 2 == 0 ? 1 : -1) * matrix(0, cCol) * DeterminantRecursive(Matrix(minorMatrix));
        }

        return det;
    }

private:
    std::vector<T> m_Values;
    std::vector<size_t> m_ColsIdx;
    std::vector<size_t> m_RowPtr;

    bool m_IsSquare;
    size_t m_Rows, m_Cols;
};