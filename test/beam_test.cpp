#include <iostream>
#include "Beam.h"

using namespace std;


int main() {
    Material mtrl(0, 100, 200, 300.0);
    Node node1(0, 100, 0, 1, 1, 0);
    Node node2(1, 200, 50, 0, 1, 0);
    Beam beam(0, node1, node2, mtrl);
    
    cout << "length: " << beam.get_length() << endl;
    cout << "lx: " << beam.get_lx() << endl;
    cout << "ly: " << beam.get_ly() << endl;
    cout << endl;
    
    Load load(0, 1000, 0, -1);
    beam.set_load(load);
    
    Matrix ke = beam.local_stiffness_matrix();
    Matrix ke_global = beam.global_stiffness_matrix();
    cout << "stiffness matrix" << endl;
    cout << endl;
    ke.print();
    cout << endl;
    ke_global.print();
    cout << endl;
    
    Matrix loadvec = beam.local_load_vector();
    Matrix loadvec_global = beam.global_load_vector();
    cout << "load vector" << endl;
    cout << endl;
    loadvec.print();
    cout << endl;
    loadvec_global.print();
    cout << endl;
    
    return 0;
}
