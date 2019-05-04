#include "Material.h"


Material::Material(int id, double A, double E, double I)
: m_id(id), m_A(A), m_E(E), m_I(I)
{
    // empty
}

int& Material::get_id() {return m_id;}
double& Material::get_A() {return m_A;}
double& Material::get_E() {return m_E;}
double& Material::get_I() {return m_I;}
