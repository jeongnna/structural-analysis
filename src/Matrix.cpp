#include <iostream>
#include <vector>
#include "Matrix.h"


// Construct with initial value
Matrix::Matrix(int row_size, int col_size, double init) {
    m_row_size = row_size;
    m_col_size = col_size;
    m_matrix.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_matrix[i].resize(m_col_size, init);
}

// Construct with 2d-vector
Matrix::Matrix(std::vector<std::vector<double> > mat) : m_matrix(mat) {
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
    m_matrix.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_matrix[i].resize(m_col_size, vec[i]);
}

int& Matrix::get_row_size() {return m_row_size;}
int& Matrix::get_col_size() {return m_col_size;}

// Print elements
// TODO: 자릿수 맞춰서 출력
void Matrix::print() {
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            std::cout << m_matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

// Get value of given location
double& Matrix::operator()(int row_id, int col_id) {
    return m_matrix[row_id][col_id];
}

// Get transposed matrix
Matrix Matrix::transpose() {
    Matrix res(m_col_size, m_row_size, 0.0);
    for (int i = 0; i < m_col_size; i++) {
        for (int j = 0; j < m_row_size; j++)
            res(i, j) = m_matrix[j][i];
    }
    return res;
}

// Scalar operator +
Matrix Matrix::operator+(double scalar) {
    Matrix res(m_row_size, m_col_size, 0.0);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] + scalar;
    }
    return res;
}

// Scalar operator -
Matrix Matrix::operator-(double scalar) {
    Matrix res(m_row_size, m_col_size, 0.0);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] - scalar;
    }
    return res;
}

// Scalar operator *
Matrix Matrix::operator*(double scalar) {
    Matrix res(m_row_size, m_col_size, 0.0);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] * scalar;
    }
    return res;
}

// Scalar operator /
Matrix Matrix::operator/(double scalar) {
    Matrix res(m_row_size, m_col_size, 0.0);
    int i, j;
    for (i = 0; i < m_row_size; i++) {
        for (j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] / scalar;
    }
    return res;
}

// Matrix operator +
Matrix Matrix::operator+(Matrix &B) {
    Matrix res(m_col_size, m_row_size, 0.0);
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] + B(i, j);
    }
    return res;
}

// Matrix operator -
Matrix Matrix::operator-(Matrix &B) {
    Matrix res(m_col_size, m_row_size, 0.0);
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            res(i, j) = m_matrix[i][j] - B(i, j);
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
                    product += m_matrix[i][k] * B(k, j);
            }
        }
    }
    else
        throw "Error: Two matrices are not conformative.";
    return res;
}
