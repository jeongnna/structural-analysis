#include <vector>
#include <cmath>
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"
#include "Beam.h"


Beam::Beam(int id, Node &node1, Node &node2, Material &material)
: m_id(id), m_node1(node1), m_node2(node2), m_material(material)
{
    double &x1 = m_node1.get_x();
    double &y1 = m_node1.get_y();
    double &x2 = m_node2.get_x();
    double &y2 = m_node2.get_y();
    double dx = x2 - x1;
    double dy = y2 - y1;
    m_length = sqrt(pow(dx, 2) + pow(dy, 2));
    m_lx = dx / m_length;
    m_ly = dy / m_length;
}


int& Beam::get_id() {return m_id;}
Node& Beam::get_node1() {return m_node1;}
Node& Beam::get_node2() {return m_node2;}
Material& Beam::get_material() {return m_material;}
double& Beam::get_length() {return m_length;}
double& Beam::get_lx() {return m_lx;}
double& Beam::get_ly() {return m_ly;}

void Beam::add_load(Load &load) {
    m_loads.push_back(load);
}


Matrix Beam::rotation_matrix() {
    std::vector<std::vector<double> > rot = {{m_lx, -m_ly,   0.0,     0.0,     0.0,   0.0},
                                             {m_ly,  m_lx,   0.0,     0.0,     0.0,   0.0},
                                             { 0.0,   0.0,   1.0,     0.0,     0.0,   0.0},
                                             { 0.0,   0.0,   0.0,    m_lx,   -m_ly,   0.0},
                                             { 0.0,   0.0,   0.0,    m_ly,    m_lx,   0.0},
                                             { 0.0,   0.0,   0.0,     0.0,     0.0,   1.0}};
    return Matrix(rot);
}

Matrix Beam::local_stiffness_matrix() {
    double &A = m_material.get_A();
    double &E = m_material.get_E();
    double &I = m_material.get_I();
    double &L = m_length;

    double AE = A * E;
    double EI = E * I;
    double L2 = pow(L, 2);
    double L3 = pow(L, 3);

    std::vector<std::vector<double> > ke =
//   column 1 |     column 2 |     column 3 |     column 4 |     column 5 |     column 6 |
    {{  AE / L,             0,             0,       -AE / L,             0,             0},
     {       0,  12 * EI / L3,   6 * EI / L2,             0, -12 * EI / L3,   6 * EI / L2},
     {       0,   6 * EI / L2,    4 * EI / L,             0,  -6 * EI / L2,    2 * EI / L},
     { -AE / L,             0,             0,        AE / L,             0,             0},
     {       0, -12 * EI / L3,  -6 * EI / L2,             0,  12 * EI / L3,  -6 * EI / L2},
     {       0,   6 * EI / L2,    2 * EI / L,             0,  -6 * EI / L2,    4 * EI / L}};
    return Matrix(ke);
}

Matrix Beam::global_stiffness_matrix() {
    Matrix rot = rotation_matrix();
    Matrix rot_t = rot.transpose();
    Matrix ke = local_stiffness_matrix();
    return rot.dot(ke).dot(rot_t);
}

Matrix Beam::local_load_substitute() {
    Matrix loadvec(6, 1, 0);
    // m_loads 에 저장된 load 정보로부터
    // nodal load substitution 6 x 1 Matrix 를 만들어야 함.
    // [px1, py1, m1, px2, py2, m2]
    return loadvec;
}

Matrix Beam::global_load_substitute() {
    Matrix rot = rotation_matrix();
    Matrix loadvec = local_load_substitute();
    return rot.dot(loadvec);
}
