#include <iostream>
#include "Node.h"

using namespace std;


int main() {
    Node node(0, 100, 50.5, 1, 1, 0);
    
    cout << "id: " << node.get_id() << endl;
    cout << "x: " << node.get_x() << endl;
    cout << "y: " << node.get_y() << endl;
    cout << "x_fixed: " << node.get_fx() << endl;
    cout << "y_fixed: " << node.get_fy() << endl;
    cout << "r_fixed: " << node.get_fr() << endl;
    
    Load load(0, 1000, 0, -1);
    node.set_load(load);
    
    return 0;
}
