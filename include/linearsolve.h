#ifndef __LOAD_H_
#define __LOAD_H_

#include <cstddef>
#include "Matrix.h"


Matrix forward(Matrix &A, Matrix &b) {
    // All diagonal elements of A are assumed to be 1.
    int &n = A.get_row_size();
    Matrix x(n, 1, 0);

    x(0, 0) = b(0, 0);
    for (int i = 1; i < n; i++) {
        x(i, 0) = b(i, 0);
        for (int j = 0; j < i; j++)
            x(i, 0) -= A(i, j) * x(j, 0);
    }
    return x;
}

Matrix backward(Matrix &A, Matrix &b) {
    int &n = A.get_row_size();
    Matrix x(n, 1, 0);

    x(n - 1, 0) = b(n - 1, 0) / A(n - 1, n - 1);
    for (int i = n - 2; i >= 0; i--) {
        for (int j = i + 1; j < n; j++)
            b(i, 0) -= A(i, j) * x(j, 0);
        x(i, 0) = b(i, 0) / A(i, i);
    }
    return x;
}

void lu_decomposition(Matrix &A) {
    int &n = A.get_row_size();

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++)
            A(i, k) /= A(k, k);

        for (int j = k + 1; j < n; j++) {
            for (int i = k + 1; i < n; i++)
                A(i, j) -= A(i, k) * A(k, j);
        }
    }
}

Matrix solve(Matrix A, Matrix b) {
    lu_decomposition(A);
    Matrix y = forward(A, b);
    return backward(A, y);
}


#endif