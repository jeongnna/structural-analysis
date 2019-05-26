#ifndef __LINEARSOLVE_H_
#define __LINEARSOLVE_H_

#include "Matrix.h"


class LinearSolver {
private:
    Matrix forward(Matrix &A, Matrix &b);
    Matrix backward(Matrix &A, Matrix &b);

public:
    LinearSolver();

    void lu_decomposition(Matrix &A);
    Matrix solve(Matrix A, Matrix b);
};


#endif
