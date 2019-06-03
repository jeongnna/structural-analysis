#ifndef __FRAME_H_
#define __FRAME_H_

#include <vector>
#include <string>
#include <fstream>
#include "Matrix.h"
#include "Material.h"
#include "Load.h"
#include "Node.h"
#include "Element.h"


class Frame {
private:
    std::vector<Material> m_materials;
    std::vector<Node> m_nodes;
    std::vector<Element> m_elements;

    Material& find_material(int id);
    Node& find_node(int id);
    Element& find_element(int id);
    void read_material_info(std::ifstream &infile);
    void read_node_info(std::ifstream &infile);
    void read_element_info(std::ifstream &infile);
    void read_load_info(std::ifstream &infile);
    void read_elementload_info(std::ifstream &infile);

public:
    Frame();

    std::vector<Material>& get_materials();
    std::vector<Node>& get_nodes();
    std::vector<Element>& get_elements();

    void print();
    void construct(std::string inputfile);

    Matrix stiffness_matrix();
    Matrix nodal_load_vector();
    Matrix fixed_end_moment();
    void compute_displacement();
};


#endif
