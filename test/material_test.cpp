#include <iostream>
#include "Material.h"

using namespace std;


int main() {
    Material mtrl(0, 100, 200, 300.0);
    
    cout << "id: " << mtrl.get_id() << endl;
    cout << "A: " << mtrl.get_A() << endl;
    cout << "E: " << mtrl.get_E() << endl;
    cout << "I: " << mtrl.get_I() << endl;
    
    return 0;
}
