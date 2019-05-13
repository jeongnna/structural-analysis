#include <vector>
#include <string>
#include <regex>
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

Beam& FrameConstructor::find_beam(int id) {
    int n_beams = m_beams.size();

    for (int i = 0; i < n_beams; i++) {
        Beam &beam = m_beams[i];
        if (beam.get_id() == id)
            return beam;
    }
    throw "Error: The beam matching id does not exist.";
}


void FrameConstructor::read_material_information(std::ifstream &infile) {
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

void FrameConstructor::read_node_information(std::ifstream &infile) {
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
        Node node(id, x, y, f1, f2, f3);
        m_nodes.push_back(node);
    }
}

void FrameConstructor::read_beam_information(std::ifstream &infile) {
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
        Beam beam(id, find_node(id_node1), find_node(id_node2), find_material(id_material));
        m_beams.push_back(beam);
    }
}

void FrameConstructor::read_load_information(std::ifstream &infile) {
    std::string line;
    int id, is_nodal, object_id;
    double px, py, pr;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;
        std::stringstream ss;
        ss.str(line);
        ss >> id >> is_nodal >> object_id >> px >> py >> pr;
        if (is_nodal) {
            Load load(px, py, pr);
            Node &node = find_node(object_id);
            node.add_load(load);
        }
        else {
            // TODO: create beam load
            Load load(px, py, pr);  // remove
            Beam &beam = find_beam(object_id);
            beam.add_load(load);
        }
    }
}

Frame FrameConstructor::construct() {
    std::ifstream infile;
    infile.open(m_inputfile);
    std::string line;

    while (getline(infile, line)) {

        if (line == "@Material")
            read_material_information(infile);

        else if (line == "@Node")
            read_node_information(infile);

        else if (line == "@Beam")
            read_beam_information(infile);

        else if (line == "@Load")
            read_load_information(infile);
    }

    infile.close();
    return Frame(m_nodes, m_beams);
}
