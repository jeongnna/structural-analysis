#include <iostream>
#include <vector>
#include "Load.h"
#include "Node.h"


Node::Node(int index, int id, double x, double y, bool fx, bool fy, bool fr)
: m_index(index), m_id(id), m_x(x), m_y(y)
{
    m_fixed = {fx, fy, fr};
    m_displacement = {0, 0, 0};
}

int& Node::get_index() {return m_index;}
int& Node::get_id() {return m_id;}
double& Node::get_x() {return m_x;}
double& Node::get_y() {return m_y;}
std::vector<bool>& Node::get_fixed() {return m_fixed;}
std::vector<NodeLoad>& Node::get_loads() {return m_loads;}
std::vector<double>& Node::get_displacement() {return m_displacement;}

void Node::add_load(NodeLoad &load) {
    m_loads.push_back(load);
}

void Node::set_displacement(double dx, double dy, double dr) {
    m_displacement[0] = dx;
    m_displacement[1] = dy;
    m_displacement[2] = dr;
}
