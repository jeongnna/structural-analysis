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

public:
    Frame();

    std::vector<Material>& get_materials();
    std::vector<Node>& get_nodes();
    std::vector<Element>& get_elements();

    void print();

    Matrix stiffness_matrix();
    Matrix nodal_load_vector();
    Matrix fixed_end_moment();
    void compute_displacement();
    void compute_reaction(std::string filename);
};


namespace FrameUtil {
    Material& find_material(int id, Frame& frame);
    Node& find_node(int id, Frame& frame);
    Element& find_element(int id, Frame& frame);

    void read_material_info(std::ifstream &infile, Frame& frame);
    void read_node_info(std::ifstream &infile, Frame& frame);
    void read_element_info(std::ifstream &infile, Frame& frame);
    void read_nodeload_info(std::ifstream &infile, Frame& frame);
    void read_elementload_info(std::ifstream &infile, Frame& frame);
    Frame construct(std::string inputfile);
}


#endif
