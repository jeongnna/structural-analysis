#include <iostream>
#include "Matrix.h"
using namespace std;


int main(void) {
    std::vector<std::vector<double> > mat = {{1.0, 2.0, 3.0},
                                             {4.0, 5.0, 6.0},
                                             {7.0, 8.0, 9.0}};
    std::vector<double> vec = {1.0, 2.0, 3.0};

    // Test constructors
    cout << "Test constructors" << endl;
    // - initial value
    cout << "initial value" << endl;
    Matrix A1(3, 3, 2.0);
    A1.print();
    cout << endl;
    // - 2d vector
    cout << "2d vector" << endl;
    Matrix A2(mat);
    A2.print();
    cout << endl;
    // - 1d vector
    cout << "1d vector" << endl;
    Matrix b1(vec);
    b1.print();
    cout << endl;

    // Test accessing value
    // access by scalar
    cout << "A2[2, 0]" << endl;
    //cout << A2(2, 0) << endl;
    cout << A2[2][0] << endl;
    // access by vector
    vector<int> row_idx({1, 2});
    vector<int> col_idx({0, 1});
    cout << "A2[1:3, 0:2]" << endl;
    A2.get(row_idx, col_idx).print();
    cout << endl;

    // Test scalar operators
    cout << "Test scalar operators" << endl;
    cout << "+" << endl;
    (A2 + 1).print();
    cout << endl;
    cout << "*" << endl;
    (A2 * 3).print();
    cout << endl;

    // Test matrix operators
    cout << "Test matrix operators" << endl;
    // - transpose
    cout << "transpose" << endl;
    A2.transpose().print();
    cout << endl;
    // +
    cout << "+" << endl;
    (A1 + A2).print();
    cout << endl;
    // *
    cout << "dot with matrix" << endl;
    A1.dot(A2).print();
    cout << endl;
    cout << "dot with vector" << endl;
    A1.dot(b1).print();
    cout << endl;

    // Revise values
    A2[0][1] += A2[0][2];
    A2.print();

    return 0;
}
