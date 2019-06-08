#include <iostream>
#include "Element.h"

using namespace std;


int main() {
    Material mtrl(0, 100, 200, 300.0);
    Node node1(0, 100, 0, 1, 1, 0);
    Node node2(1, 200, 50, 0, 1, 0);
    Element elm(0, node1, node2, mtrl);

    cout << "length: " << elm.get_length() << endl;
    cout << "lx: " << elm.get_lx() << endl;
    cout << "ly: " << elm.get_ly() << endl;
    cout << endl;

    ElementLoad ld(0, 1000, 0, 0);
    elm.add_load(ld);

    Matrix kmat = elm.local_stiffness_matrix();
    Matrix kmat_global = elm.global_stiffness_matrix();
    cout << "stiffness matrix" << endl << endl;
    kmat.print();
    cout << endl;
    kmat_global.print();
    cout << endl;

    Matrix fem = elm.local_fixed_end_moment();
    Matrix fem_global = elm.global_fixed_end_moment();
    cout << "load vector" << endl << endl;
    fem.print();
    cout << endl;
    fem_global.print();
    cout << endl;

    return 0;
}
