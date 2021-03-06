#include <iostream>
#include <vector>
#include <cmath>
#include "../lib/json.hpp"
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"
#include "Element.h"

using json = nlohmann::json;


Element::Element(int id, Node &node1, Node &node2, Material &material)
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


int& Element::get_id() {return m_id;}
Node& Element::get_node1() {return m_node1;}
Node& Element::get_node2() {return m_node2;}
Material& Element::get_material() {return m_material;}
double& Element::get_length() {return m_length;}
double& Element::get_lx() {return m_lx;}
double& Element::get_ly() {return m_ly;}
std::vector<ElementLoad>& Element::get_loads() {return m_loads;}

void Element::add_load(ElementLoad &load) {
    m_loads.push_back(load);
}


Matrix Element::rotation_matrix() {
    std::vector<std::vector<double> > rot =
        {{m_lx, -m_ly,   0.0,     0.0,     0.0,   0.0},
         {m_ly,  m_lx,   0.0,     0.0,     0.0,   0.0},
         { 0.0,   0.0,   1.0,     0.0,     0.0,   0.0},
         { 0.0,   0.0,   0.0,    m_lx,   -m_ly,   0.0},
         { 0.0,   0.0,   0.0,    m_ly,    m_lx,   0.0},
         { 0.0,   0.0,   0.0,     0.0,     0.0,   1.0}};
    return Matrix(rot);
}

Matrix Element::local_stiffness_matrix() {
    double &A = m_material.get_A();
    double &E = m_material.get_E();
    double &I = m_material.get_I();
    double &L = m_length;

    double _AEL_ = A * E / L;
    double _12EIL3_ = 12 * E * I / pow(L, 3);
    double _6EIL2_ = 6 * E * I / pow(L, 2);
    double _4EIL_ = 4 * E * I / L;
    double _2EIL_ = 2 * E * I / L;

    std::vector<std::vector<double> > kmat =
        {{  _AEL_,          0,         0,    -_AEL_,          0,         0},
         {      0,   _12EIL3_,   _6EIL2_,         0,  -_12EIL3_,   _6EIL2_},
         {      0,    _6EIL2_,    _4EIL_,         0,   -_6EIL2_,    _2EIL_},
         { -_AEL_,          0,         0,     _AEL_,          0,         0},
         {      0,  -_12EIL3_,  -_6EIL2_,         0,   _12EIL3_,  -_6EIL2_},
         {      0,    _6EIL2_,    _2EIL_,         0,   -_6EIL2_,    _4EIL_}};
    return Matrix(kmat);
}

Matrix Element::global_stiffness_matrix() {
    Matrix rot = rotation_matrix();
    Matrix rot_t = rot.transpose();
    Matrix kmat = local_stiffness_matrix();
    return rot.dot(kmat).dot(rot_t);
}

Matrix Element::local_fixed_end_moment() {
    Matrix fem(6, 1, 0.0);
    for (int l = 0; l < m_loads.size(); l++) {
        Matrix tmp = m_loads[l].fixed_end_moment();
        fem += tmp;
    }
    return fem;
}

Matrix Element::global_fixed_end_moment() {
    Matrix rot = rotation_matrix();
    Matrix fem = local_fixed_end_moment();
    return rot.dot(fem);
}

Matrix Element::reaction() {
    std::vector<double> &node1_disp = m_node1.get_displacement();
    std::vector<double> &node2_disp = m_node2.get_displacement();
    Matrix disp(6, 1, 0.0);
    for (int i = 0; i < 3; i++) {
        disp[0 + i][0] = node1_disp[i];
        disp[3 + i][0] = node2_disp[i];
    }

    Matrix kmat = local_stiffness_matrix();
    Matrix rot_t = rotation_matrix().transpose();
    Matrix fem = local_fixed_end_moment();
    return kmat.dot(rot_t).dot(disp) + fem;
}


json Element::json_object() {
    json jsn;
    jsn["node1_x"] = m_node1.get_x();
    jsn["node1_y"] = m_node1.get_y();
    jsn["node2_x"] = m_node2.get_x();
    jsn["node2_y"] = m_node2.get_y();
    jsn["reaction"] = reaction().to_1dvector();

    jsn["load"] = json::array();
    for (int l = 0; l < m_loads.size(); l++) {
        ElementLoad &emld = m_loads[l];
        json load_jsn;
        load_jsn["loadtype"] = emld.get_loadtype();
        load_jsn["magnitude"] = emld.get_magnitude();
        load_jsn["a"] = emld.get_a();
        load_jsn["b"] = emld.get_b();
        jsn["load"].push_back(load_jsn);
    }

    return jsn;
}
