#ifndef __ELEMENT_H_
#define __ELEMENT_H_

#include <vector>
#include "../lib/json.hpp"
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"

using json = nlohmann::json;


class Element {
private:
    int m_id;
    Node &m_node1;
    Node &m_node2;
    Material &m_material;
    double m_length;
    double m_lx;
    double m_ly;
    std::vector<ElementLoad> m_loads;

public:
    Element(int id, Node &node1, Node &node2, Material &material);

    int& get_id();
    Node& get_node1();
    Node& get_node2();
    Material& get_material();
    double& get_length();
    double& get_lx();
    double& get_ly();
    std::vector<ElementLoad>& get_loads();

    void add_load(ElementLoad &load);

    Matrix rotation_matrix();
    Matrix local_stiffness_matrix();
    Matrix global_stiffness_matrix();
    Matrix local_fixed_end_moment();
    Matrix global_fixed_end_moment();
    Matrix reaction();

    json json_object();
};


#endif
