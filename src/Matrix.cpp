#include <iostream>
#include <fstream>
#include <vector>
#include "Matrix.h"


// Default constructor
Matrix::Matrix() {
    // empty
}

// Construct with size and initial value
Matrix::Matrix(int row_size, int col_size, double init)
: m_row_size(row_size), m_col_size(col_size)
{
    if (row_size < 1)
        throw "Error: Row size must be a positive integer";
    if (col_size < 1)
        throw "Error: Column size must be a positive integer";
    m_data.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_data[i].resize(m_col_size, init);
}

// Construct with 2D vector
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

// Construct with 1D vector
Matrix::Matrix(std::vector<double> vec) {
    m_row_size = vec.size();
    m_col_size = 1;
    m_data.resize(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        m_data[i].resize(m_col_size, vec[i]);
}


// Accessors
int& Matrix::get_row_size() {return m_row_size;}
int& Matrix::get_col_size() {return m_col_size;}
std::vector<std::vector<double> >& Matrix::get_data() {return m_data;}


// Print elements
void Matrix::print(std::string sep) {
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            std::cout << m_data[i][j] << sep;
        std::cout << std::endl;
    }
}

// Convert to string
std::string Matrix::to_string(std::string sep) {
    std::string str;
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            str += std::to_string(m_data[i][j]) + sep;
        std::cout << std::endl;
    }
    return str;
}

// Save matrix to CSV file
void Matrix::to_csv(std::string filename, std::string sep) {
    std::ofstream outfile;
    outfile.open(filename);
    for (int i = 0; i < m_row_size; i++) {
        for (int j = 0; j < m_col_size; j++)
            outfile << m_data[i][j] << sep;
        outfile << std::endl;
    }
}


// Convert matrix to 1D vector
std::vector<double> Matrix::to_1dvector() {
    if (m_data[0].size() != 1)
        throw "Error: Matrix with more than one column can't be converted to 1D vector.";
    std::vector<double> vec(m_row_size);
    for (int i = 0; i < m_row_size; i++)
        vec[i] = m_data[i][0];
    return vec;
}


// Access a row
// You can access (i, j) value like this: `example_matrix[i][j]`
// You can also revise the value: `example_matrix[i][j] = 0.0;`
std::vector<double>& Matrix::operator [](int row_idx) {
    return m_data[row_idx];
}

// Get submatrix
// This returns a matrix which has a copy of values so that
// you can't revise values of the original matrix
Matrix Matrix::get(std::vector<int> &row_idx, std::vector<int> &col_idx) {
    int row_size = row_idx.size();
    int col_size = col_idx.size();
    Matrix res(row_size, col_size, 0);
    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            res[i][j] = m_data[row_idx[i]][col_idx[j]];
    return res;
}

Matrix Matrix::get(std::vector<int> &row_idx, int col_idx) {
    int row_size = row_idx.size();
    Matrix res(row_size, 1, 0);
    for (int i = 0; i < row_size; i++)
        res[i][0] = m_data[row_idx[i]][col_idx];
    return res;
}

Matrix Matrix::get(int row_idx, std::vector<int> &col_idx) {
    int col_size = col_idx.size();
    Matrix res(1, col_size, 0);
    for (int j = 0; j < col_size; j++)
        res[j][0] = m_data[row_idx][col_idx[j]];
    return res;
}


// Get transposed matrix
Matrix Matrix::transpose() {
    Matrix res(m_col_size, m_row_size, 0.0);
    for (int i = 0; i < m_col_size; i++)
        for (int j = 0; j < m_row_size; j++)
            res[i][j] = m_data[j][i];
    return res;
}


// Scalar operators
Matrix Matrix::operator +(double scalar) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] += scalar;
    return res;
}

Matrix Matrix::operator -(double scalar) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] -= scalar;
    return res;
}

Matrix Matrix::operator *(double scalar) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] *= scalar;
    return res;
}

Matrix Matrix::operator /(double scalar) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] /= scalar;
    return res;
}

void Matrix::operator +=(double scalar) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] += scalar;
}

void Matrix::operator -=(double scalar) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] -= scalar;
}

void Matrix::operator *=(double scalar) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] *= scalar;
}

void Matrix::operator /=(double scalar) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] /= scalar;
}


// Matrix operators
Matrix Matrix::operator +(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] += B[i][j];
    return res;
}

Matrix Matrix::operator -(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] -= B[i][j];
    return res;
}

Matrix Matrix::operator *(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] *= B[i][j];
    return res;
}

Matrix Matrix::operator /(Matrix &B) {
    Matrix res(m_data);
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            res[i][j] /= B[i][j];
    return res;
}

Matrix Matrix::dot(Matrix &B) {
    Matrix res(m_row_size, B.get_col_size(), 0.0);
    if (m_col_size == B.get_row_size()) {
        for (int i = 0; i < m_row_size; i++) {
            for (int j = 0; j < B.get_col_size(); j++) {
                double &product = res[i][j];
                for (int k = 0; k < m_col_size; k++)
                    product += m_data[i][k] * B[k][j];
            }
        }
    }
    else
        throw "Error: Column size of first matrix must be equal to row size of second matrix.";
    return res;
}

void Matrix::operator +=(Matrix &B) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] += B[i][j];
}

void Matrix::operator -=(Matrix &B) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] -= B[i][j];
}

void Matrix::operator *=(Matrix &B) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] *= B[i][j];
}

void Matrix::operator /=(Matrix &B) {
    for (int i = 0; i < m_row_size; i++)
        for (int j = 0; j < m_col_size; j++)
            m_data[i][j] /= B[i][j];
}
