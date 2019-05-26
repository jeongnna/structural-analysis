#include <iostream>
#include "Member.h"

using namespace std;


int main() {
    Material mtrl(0, 100, 200, 300.0);
    Node node1(0, 100, 0, 1, 1, 0);
    Node node2(1, 200, 50, 0, 1, 0);
    Member mb(0, node1, node2, mtrl);

    cout << "length: " << mb.get_length() << endl;
    cout << "lx: " << mb.get_lx() << endl;
    cout << "ly: " << mb.get_ly() << endl;
    cout << endl;

    MemberLoad ld(0, 1000, 0, 0);
    mb.add_load(ld);

    Matrix ke = mb.local_stiffness_matrix();
    Matrix ke_global = mb.global_stiffness_matrix();
    cout << "stiffness matrix" << endl;
    cout << endl;
    ke.print();
    cout << endl;
    ke_global.print();
    cout << endl;

    Matrix fem = mb.local_fem();
    Matrix fem_global = mb.global_fem();
    cout << "load vector" << endl;
    cout << endl;
    fem.print();
    cout << endl;
    fem_global.print();
    cout << endl;

    return 0;
}
