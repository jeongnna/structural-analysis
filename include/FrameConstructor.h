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
    std::vector<Member> m_members;

    Material& find_material(int id);
    Node& find_node(int id);
    Member& find_member(int id);

    void read_material_info(std::ifstream &infile);
    void read_node_info(std::ifstream &infile);
    void read_member_info(std::ifstream &infile);
    void read_load_info(std::ifstream &infile);
    void read_memberload_info(std::ifstream &infile);

public:
    FrameConstructor();

    void set_inputfile(std::string inputfile);

    //Frame construct();
    Frame construct(std::string inputfile);
};


#endif
