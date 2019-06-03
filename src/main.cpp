#include <iostream>
#include <string>
#include "structure.h"

using namespace std;


int main() {
    Frame frame;
    frame.construct("data/test_frame.txt");

    cout << "<Stiffness matrix>" << endl;
    Matrix ke = frame.stiffness_matrix();
    ke.print();
    cout << endl;

    cout << "<Load vector>" << endl;
    Matrix loadvec = frame.nodal_load_vector();
    loadvec.print();
    cout << endl;

    frame.compute_displacement();
    frame.print();

    return 0;
}
