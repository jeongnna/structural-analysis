#ifndef __FRAME_CONSTRUCTOR_H_
#define __FRAME_CONSTRUCTOR_H_

#include <vector>
#include <string>
#include <fstream>
#include "structure.h"


class FrameConstructor {
private:
    std::string m_inputfile;
    std::vector<Material> m_materials;
    std::vector<Node> m_nodes;
    std::vector<Beam> m_beams;

    Material& find_material(int id);
    Node& find_node(int id);
    Beam& find_beam(int id);

    void read_material_information(std::ifstream &infile);
    void read_node_information(std::ifstream &infile);
    void read_beam_information(std::ifstream &infile);
    void read_load_information(std::ifstream &infile);

public:
    FrameConstructor();

    void set_inputfile(std::string inputfile);

    Frame construct();
};


#endif
