#include <iostream>
#include <vector>
#include "Matrix.h"


// Default constructor
Matrix::Matrix() {
    // empty
}

// Construct with initial value
Matrix::Matrix(int row_size, int col_size, double init) {
    m_row_size = row_size;
    m_col_size = col_size;
    m_data.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_data[i].resize(m_col_size, init);
}

// Construct with 2d-vector
Matrix::Matrix(std::vector<std::vector<double> > mat)
: m_data(mat)
{
    m_row_size = mat.size();
    m_col_size = mat[0].size();
    for (int i = 1; i < m_row_size; i++) {
        if (mat[i].size() != m_col_size)
            throw "Error: Every rows must have the same length.";
    }
}

// Construct with 1d-vector
Matrix::Matrix(std::vector<double> vec) {
    m_row_size = vec.size();
    m_col_size = 1;
    m_data.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_data[i].resize(m_col_size, vec[i]);
}

int& Matrix::get_row_size() {return m_row_size;}
int& Matrix::get_col_size() {return m_col_size;}
std::vector<std::vector<double> >& Matrix::get_data() {return m_data;}

std::vector<double> Matrix::to_1dvector() {
    if (m_data[0].size() != 1)
        throw "Error: Matrix with more than one column can't be converted to 1d vector.";
    std::vector<double> vec(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        vec[i] = m_data[i][0];
    return vec;
}

// Print elements
// TODO: 자릿수 맞춰서 출력
void Matrix::print() {
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            std::cout << m_data[i][j] << " ";
        std::cout << std::endl;
    }
}

// Get value of given location
double& Matrix::operator ()(int row_idx, int col_idx) {
    return m_data[row_idx][col_idx];
}

Matrix Matrix::operator ()(std::vector<int> &row_idx, std::vector<int> &col_idx) {
    int row_size = row_idx.size();
    int col_size = col_idx.size();
    Matrix res(row_size, col_size, 0);
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++)
            res(i, j) = m_data[row_idx[i]][col_idx[j]];
    }
    return res;
}

Matrix Matrix::operator ()(std::vector<int> &row_idx, int col_idx) {
    int row_size = row_idx.size();
    Matrix res(row_size, 1, 0);
    for (int i = 0; i < row_size; i++)
        res(i, 0) = m_data[row_idx[i]][col_idx];
    return res;
}

Matrix Matrix::operator ()(int row_idx, std::vector<int> &col_idx) {
    int col_size = col_idx.size();
    Matrix res(1, col_size, 0);
    for (int j = 0; j < col_size; j++)
        res(j, 0) = m_data[row_idx][col_idx[j]];
    return res;
}

// Get transposed matrix
Matrix Matrix::transpose() {
    Matrix res(m_col_size, m_row_size, 0.0);
    for (int i = 0; i < m_col_size; i++) {
        for (int j = 0; j < m_row_size; j++)
            res(i, j) = m_data[j][i];
    }
    return res;
}

// Scalar operator +
Matrix Matrix::operator +(double scalar) {
    Matrix res(m_data);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) += scalar;
    }
    return res;
}

// Scalar operator -
Matrix Matrix::operator -(double scalar) {
    Matrix res(m_data);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) -= scalar;
    }
    return res;
}

// Scalar operator *
Matrix Matrix::operator *(double scalar) {
    Matrix res(m_data);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) *= scalar;
    }
    return res;
}

// Scalar operator /
Matrix Matrix::operator /(double scalar) {
    Matrix res(m_data);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) /= scalar;
    }
    return res;
}

// Matrix operator +
Matrix Matrix::operator +(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            res(i, j) += B(i, j);
    }
    return res;
}

// Matrix operator -
Matrix Matrix::operator -(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            res(i, j) -= B(i, j);
    }
    return res;
}

// Matrix operator dot
Matrix Matrix::dot(Matrix &B) {
    Matrix res(m_row_size, B.get_col_size(), 0.0);
    if (m_col_size == B.get_row_size()) {
        for (int i = 0; i < m_row_size; i++) {
            for (int j = 0; j < B.get_col_size(); j++) {
                double &product = res(i, j);
                for (int k = 0; k < m_col_size; k++)
                    product += m_data[i][k] * B(k, j);
            }
        }
    }
    else
        throw "Error: Two matrices are not conformative.";
    return res;
}
