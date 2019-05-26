#include <vector>
#include "Load.h"
#include "Node.h"


Node::Node(int id, double x, double y, bool fx, bool fy, bool fr)
: m_id(id), m_x(x), m_y(y)
{
    m_fixed = {fx, fy, fr};
    m_displacement = {0, 0, 0};
}

int& Node::get_id() {return m_id;}
double& Node::get_x() {return m_x;}
double& Node::get_y() {return m_y;}
//bool& Node::get_fx() {return m_fx;}
//bool& Node::get_fy() {return m_fy;}
//bool& Node::get_fr() {return m_fr;}
std::vector<bool>& Node::get_fixed() {return m_fixed;}
std::vector<Load>& Node::get_load() {return m_loads;}

void Node::add_load(Load &load) {
    m_loads.push_back(load);
}

void Node::set_displacement(double &dx, double &dy, double &dr) {
    m_displacement[0] = dx;
    m_displacement[1] = dy;
    m_displacement[2] = dr;
}
