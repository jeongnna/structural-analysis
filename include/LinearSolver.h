#ifndef __LINEARSOLVE_H_
#define __LINEARSOLVE_H_

#include "Matrix.h"


class LinearSolver {
private:
    static Matrix forward(Matrix &A, Matrix &b);
    static Matrix backward(Matrix &A, Matrix &b);

public:
    LinearSolver();

    static void lu_decomposition(Matrix &A);
    static Matrix solve(Matrix A, Matrix b);
};


#endif
