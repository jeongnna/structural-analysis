#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <vector>
#include <string>


class Matrix {
private:
    int m_row_size;
    int m_col_size;
    std::vector<std::vector<double> > m_data;

public:
    Matrix();
    Matrix(int row_size, int col_size, double init);
    Matrix(std::vector<std::vector<double> > mat);
    Matrix(std::vector<double> vec);

    int& get_row_size();
    int& get_col_size();
    std::vector<std::vector<double> >& get_data();

    Matrix copy();
    Matrix print(std::string sep=" ");
    std::string to_string(std::string sep=" ");
    Matrix to_csv(std::string filename, std::string sep=",");

    std::vector<double> to_1dvector();

    std::vector<double>& operator [](int row_idx);
    Matrix get(std::vector<int> &row_idx, std::vector<int> &col_idx);
    Matrix get(std::vector<int> &row_idx, int col_idx);
    Matrix get(int row_idx, std::vector<int> &col_idx);

    Matrix transpose();

    Matrix operator +(double scalar);
    Matrix operator -(double scalar);
    Matrix operator *(double scalar);
    Matrix operator /(double scalar);

    void operator +=(double scalar);
    void operator -=(double scalar);
    void operator *=(double scalar);
    void operator /=(double scalar);

    Matrix operator +(Matrix &B);
    Matrix operator -(Matrix &B);
    Matrix operator *(Matrix &B);
    Matrix operator /(Matrix &B);
    Matrix dot(Matrix &B);

    void operator +=(Matrix &B);
    void operator -=(Matrix &B);
    void operator *=(Matrix &B);
    void operator /=(Matrix &B);
};


#endif
