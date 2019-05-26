#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <vector>


class Matrix {
private:
    int m_row_size;
    int m_col_size;
    std::vector<std::vector<double> > m_matrix;

public:
    Matrix(int row_size, int col_size, double init);
    Matrix(std::vector<std::vector<double> > mat);
    Matrix(std::vector<double> vec);

    int& get_row_size();
    int& get_col_size();

    void print();
    double& operator()(int row_idx, int col_idx);
    Matrix operator()(std::vector<int> &row_idx, std::vector<int> &col_idx);
    Matrix operator()(std::vector<int> &row_idx, int col_idx);
    Matrix operator()(int row_idx, std::vector<int> &col_idx);

    Matrix transpose();

    Matrix operator+(double scalar);
    Matrix operator-(double scalar);
    Matrix operator*(double scalar);
    Matrix operator/(double scalar);

    Matrix operator+(Matrix &B);
    Matrix operator-(Matrix &B);
    Matrix dot(Matrix &B);
};


#endif
