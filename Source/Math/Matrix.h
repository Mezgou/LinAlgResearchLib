#pragma once
#include <stdexcept>
#include <vector>
#include <iostream>

template<typename T>
class Matrix {
public:
    explicit Matrix(size_t rows, size_t cols, const T& initialValue)
        : m_Data(rows, std::vector<T>(cols, initialValue)), m_IsSquare(rows == cols), m_Rows(rows), m_Cols(cols) {};
    explicit Matrix(const std::vector<std::vector<T>>& data)
        : m_Data(data), m_IsSquare(data.size() == data[0].size()), m_Rows(data.size()), m_Cols(data[0].size()) {};
    ~Matrix() = default;

    T& operator()(size_t row, size_t col) {
        if (row >= m_Rows || col >= m_Cols) {
            throw std::out_of_range("Index out of range");
        }
        return m_Data[row][col];
    }

    const T& operator()(size_t row, size_t col) const {
        if (row >= m_Rows || col >= m_Cols) {
            throw std::out_of_range("Index out of range");
        }
        return m_Data[row][col];
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        if (m_Rows != other.GetRows() || m_Cols != other.GetCols()) {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }
        Matrix<T> result(m_Rows, m_Cols, 0);
        for (size_t rIdx = 0; rIdx < m_Rows; rIdx++) {
            for (size_t cIdx = 0; cIdx < m_Cols; cIdx++) {
                result(rIdx, cIdx) = m_Data[rIdx][cIdx] + other(rIdx, cIdx);
            }
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        if (m_Cols != other.GetRows()) {
            throw std::invalid_argument("Matrix dimensions must match for multiplication");
        }
        Matrix<T> result(m_Rows, other.GetCols(), 0);
        for (size_t rIdx = 0; rIdx < m_Rows; rIdx++) {
            for (size_t cIdx = 0; cIdx < other.GetCols(); cIdx++) {
                for (size_t vIdx = 0; vIdx < m_Cols; vIdx++) {
                    result(rIdx, cIdx) += m_Data[rIdx][vIdx] * other(vIdx, cIdx);
                }
            }
        }
        return result;
    }

    Matrix<T> Transpose() const {
        Matrix<T> transposed(m_Cols, m_Rows, 0);
        for (size_t rIdx = 0; rIdx < m_Rows; rIdx++) {
            for (size_t cIdx = 0; cIdx < m_Cols; cIdx++) {
                transposed(cIdx, rIdx) = m_Data[rIdx][cIdx];
            }
        }
        return transposed;
    }

    bool IsSquare() const { return m_IsSquare; }
    size_t GetRows() const { return m_Rows; }
    size_t GetCols() const { return m_Cols; }

    void Print() const {
        for (const auto& row : m_Data) {
            for (const auto& elem : row) {
                std::cout << elem << " ";
            }
            std::cout << "\n";
        }
    }

private:
    std::vector<std::vector<T>> m_Data;
    bool m_IsSquare;
    size_t m_Rows, m_Cols;
};