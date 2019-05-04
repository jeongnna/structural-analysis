#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include <cstddef>


class Material {
private:
    int m_id;
    double m_A;
    double m_E;
    double m_I;

public:
    Material(int id, double A, double E, double I);

    int& get_id();
    double& get_A();
    double& get_E();
    double& get_I();
};


#endif
