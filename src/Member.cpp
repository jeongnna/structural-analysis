#include <vector>
#include <cmath>
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"
#include "Member.h"


Member::Member(int id, Node &node1, Node &node2, Material &material)
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


int& Member::get_id() {return m_id;}
Node& Member::get_node1() {return m_node1;}
Node& Member::get_node2() {return m_node2;}
Material& Member::get_material() {return m_material;}
double& Member::get_length() {return m_length;}
double& Member::get_lx() {return m_lx;}
double& Member::get_ly() {return m_ly;}

void Member::add_load(MemberLoad &load) {
    m_loads.push_back(load);
}


Matrix Member::rotation_matrix() {
    std::vector<std::vector<double> > rot =
        {{m_lx, -m_ly,   0.0,     0.0,     0.0,   0.0},
         {m_ly,  m_lx,   0.0,     0.0,     0.0,   0.0},
         { 0.0,   0.0,   1.0,     0.0,     0.0,   0.0},
         { 0.0,   0.0,   0.0,    m_lx,   -m_ly,   0.0},
         { 0.0,   0.0,   0.0,    m_ly,    m_lx,   0.0},
         { 0.0,   0.0,   0.0,     0.0,     0.0,   1.0}};
    return Matrix(rot);
}

Matrix Member::local_stiffness_matrix() {
    double &A = m_material.get_A();
    double &E = m_material.get_E();
    double &I = m_material.get_I();
    double &L = m_length;

    double _AEL_ = A * E / L;
    double _12EIL3_ = 12 * E * I / pow(L, 3);
    double _6EIL2_ = 6 * E * I / pow(L, 2);
    double _4EIL_ = 4 * E * I / L;
    double _2EIL_ = 2 * E * I / L;

    std::vector<std::vector<double> > ke =
        {{  _AEL_,          0,         0,    -_AEL_,          0,         0},
         {      0,   _12EIL3_,   _6EIL2_,         0,  -_12EIL3_,   _6EIL2_},
         {      0,    _6EIL2_,    _4EIL_,         0,   -_6EIL2_,    _2EIL_},
         { -_AEL_,          0,         0,     _AEL_,          0,         0},
         {      0,  -_12EIL3_,  -_6EIL2_,         0,   _12EIL3_,  -_6EIL2_},
         {      0,    _6EIL2_,    _2EIL_,         0,   -_6EIL2_,    _4EIL_}};
    return Matrix(ke);
}

Matrix Member::global_stiffness_matrix() {
    Matrix rot = rotation_matrix();
    Matrix rot_t = rot.transpose();
    Matrix ke = local_stiffness_matrix();
    return rot.dot(ke).dot(rot_t);
}

Matrix Member::local_fem() {
    Matrix loadvec(6, 1, 0);
    // m_loads 에 저장된 load 정보로부터
    // nodal load substitution 6 x 1 Matrix 를 만들어야 함.
    // [px1, py1, m1, px2, py2, m2]
    return loadvec;
}

Matrix Member::global_fem() {
    Matrix rot = rotation_matrix();
    Matrix loadvec = local_fem();
    return rot.dot(loadvec);
}
