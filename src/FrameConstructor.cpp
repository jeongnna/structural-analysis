#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include "structure.h"
#include "FrameConstructor.h"


// TODO: wrap `comment` in a namespace.
std::regex comment("^//.*");

FrameConstructor::FrameConstructor()
{
    // empty
}

void FrameConstructor::set_inputfile(std::string inputfile) {
    m_inputfile = inputfile;
}

Material& FrameConstructor::find_material(int id) {
    int n_materials = m_materials.size();

    for (int i = 0; i < n_materials; i++) {
        Material &mtrl = m_materials[i];
        if (mtrl.get_id() == id)
            return mtrl;
    }
    throw "Error: The material matching id does not exist.";
}

Node& FrameConstructor::find_node(int id) {
    int n_nodes = m_nodes.size();

    for (int i = 0; i < n_nodes; i++) {
        Node &node = m_nodes[i];
        if (node.get_id() == id)
            return node;
    }
    throw "Error: The node matching id does not exist.";
}

Member& FrameConstructor::find_member(int id) {
    int n_members = m_members.size();

    for (int i = 0; i < n_members; i++) {
        Member &mb = m_members[i];
        if (mb.get_id() == id)
            return mb;
    }
    throw "Error: The member matching id does not exist.";
}


void FrameConstructor::read_material_info(std::ifstream &infile) {
    std::string line;
    int id;
    double A, E, I;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> A >> E >> I;
        Material material(id, A, E, I);
        m_materials.push_back(material);
    }
}

void FrameConstructor::read_node_info(std::ifstream &infile) {
    std::string line;
    int id;
    double x, y;
    bool f1, f2, f3;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> x >> y >> f1 >> f2 >> f3;
        Node nd(id, x, y, f1, f2, f3);
        m_nodes.push_back(nd);
    }
}

void FrameConstructor::read_member_info(std::ifstream &infile) {
    std::string line;
    int id, id_node1, id_node2, id_material;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> id_node1 >> id_node2 >> id_material;
        Member mb(id, find_node(id_node1), find_node(id_node2), find_material(id_material));
        m_members.push_back(mb);
    }
}

void FrameConstructor::read_load_info(std::ifstream &infile) {
    std::string line;
    int id, object_id;
    double px, py, pr;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> object_id >> px >> py >> pr;
        Load ld(px, py, pr);
        Node &nd = find_node(object_id);
        nd.add_load(ld);
    }
}

void FrameConstructor::read_memberload_info(std::ifstream &infile) {
    std::string line;
    int id, object_id, loadtype;
    double magnitude, a, b;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> object_id >> loadtype >> magnitude >> a >> b;
        MemberLoad ld(loadtype, magnitude, a, b);
        Member &mb = find_member(object_id);
        mb.add_load(ld);
    }
}

Frame FrameConstructor::construct(std::string inputfile) {
    std::ifstream infile;
    infile.open(inputfile);
    std::string line;

    while (getline(infile, line)) {
        if (line == "@Material")
            read_material_info(infile);

        else if (line == "@Node")
            read_node_info(infile);

        else if (line == "@Member")
            read_member_info(infile);

        else if (line == "@Load")
            read_load_info(infile);

        else if (line == "@MemberLoad")
            read_memberload_info(infile);
    }

    infile.close();
    return Frame(m_nodes, m_members);
}
