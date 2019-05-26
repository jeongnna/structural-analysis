#include <iostream>
#include <string>
#include "Matrix.h"
#include "LinearSolver.h"
#include "structure.h"
#include "FrameConstructor.h"

using namespace std;


int main() {
    FrameConstructor fc;
    Frame frame = fc.construct("data/test_frame.txt");
    cout << endl;

    frame.print();
    cout << endl;

    cout << "<Stiffness matrix>" << endl;
    Matrix ke = frame.stiffness_matrix();
    ke.print();
    cout << endl;

    cout << "<Load vector>" << endl;
    Matrix loadvec = frame.load_vector();
    loadvec.print();
    cout << endl;

    return 0;
}
