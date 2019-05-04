#ifndef __BEAM_H_
#define __BEAM_H_

#include <cstddef>
#include <vector>
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"


class Beam {
private:
    int m_id;
    Node &m_node1;
    Node &m_node2;
    Material &m_material;
    double m_length;
    double m_lx;
    double m_ly;
    Load m_load;

public:
    Beam(int id, Node &node1, Node &node2, Material &material);

    int& get_id();
    Node& get_node1();
    Node& get_node2();
    Material& get_material();
    double& get_length();
    double& get_lx();
    double& get_ly();

    void set_load(Load &load);

    Matrix rotation_matrix();
    Matrix local_stiffness_matrix();
    Matrix global_stiffness_matrix();
    Matrix local_load_vector();
    Matrix global_load_vector();
};


#endif