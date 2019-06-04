#include <iostream>
#include <vector>
#include "Matrix.h"
#include "LinearSolver.h"

using namespace std;


int main() {
    vector<vector<double> > A_values = {{2.0, -4.0, 2.0},
                                        {4.0, -9.0, 7.0},
                                        {2.0,  1.0, 3.0}};
    Matrix A(A_values);

    vector<vector<double> > b_values = {{ 6.0},
                                        {20.0},
                                        {14.0}};
    Matrix b(b_values);

    Matrix x = LinearSolver::solve(A, b);
    x.print();  // Expected value: [2, 1, 3]

    return 0;
}
