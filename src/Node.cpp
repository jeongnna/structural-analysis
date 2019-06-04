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

void Node::print() {
    std::cout << "{" << std::endl;
    std::cout << "    Node ID: " << m_id << std::endl;
    std::cout << "    Coordinates: [" << m_x << ", " << m_y << "]" << std::endl;
    std::cout << "    Fixed: [" << m_fixed[0] << ", " << m_fixed[1] << ", " << m_fixed[2] << "]" << std::endl;
    std::cout << "    Loads: [";
    for (int l = 0; l < m_loads.size(); l++) {
        std::cout << m_loads[l].get_id();
        if (l < m_loads.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << "    Displacement: [";
    for (int l = 0; l < 3; l++) {
        std::cout << m_displacement[l];
        if (l < 2)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    std::cout << "}" << std::endl;
}

void Node::add_load(NodeLoad &load) {
    m_loads.push_back(load);
}

void Node::set_displacement(double dx, double dy, double dr) {
    m_displacement[0] = dx;
    m_displacement[1] = dy;
    m_displacement[2] = dr;
}
