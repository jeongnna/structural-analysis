#include <iostream>
#include "Load.h"

using namespace std;


int main() {
    Load load(0, 1000, 0, -1);
    
    cout << "applied: " << load.get_applied() << endl;
    cout << "type: " << load.get_type() << endl;
    cout << "magnitude: " << load.get_magnitude() << endl;
    cout << "lx: " << load.get_lx() << endl;
    cout << "ly: " << load.get_ly() << endl;
    
    return 0;
}
