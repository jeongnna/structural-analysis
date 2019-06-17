#include <vector>
#include <string>
#include <regex>  // to distinguish comments in inputfile
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  // std::setw (when writing json file)
#include "../lib/json.hpp"
#include "Matrix.h"
#include "LinearSolver.h"
#include "Load.h"
#include "Node.h"
#include "Element.h"
#include "Frame.h"

using json = nlohmann::json;


Frame::Frame(std::string name)
: m_name(name)
{
    // empty
}


std::vector<Material>& Frame::get_materials() {return m_materials;}
std::vector<Node>& Frame::get_nodes() {return m_nodes;}
std::vector<Element>& Frame::get_elements() {return m_elements;}


Matrix Frame::stiffness_matrix() {
    int mat_size = 3 * m_nodes.size();
    Matrix kmat_frame(mat_size, mat_size, 0.0);

    for (int k = 0; k < m_elements.size(); k++) {
        Element &elm = m_elements[k];
        Matrix kmat = elm.global_stiffness_matrix();
        int p1 = 3 * elm.get_node1().get_index();
        int p2 = 3 * elm.get_node2().get_index();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                kmat_frame[p1 + i][p1 + j] += kmat[i][j];
                kmat_frame[p1 + i][p2 + j] += kmat[i][3 + j];
                kmat_frame[p2 + i][p1 + j] += kmat[3 + i][j];
                kmat_frame[p2 + i][p2 + j] += kmat[3 + i][3 + j];
            }
        }
    }

    return kmat_frame;
}

Matrix Frame::nodal_load_vector() {
    Matrix loadvec(3 * m_nodes.size(), 1, 0.0);

    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nde = m_nodes[k];
        std::vector<NodeLoad> &loads = nde.get_loads();

        for (int i = 0; i < loads.size(); i++) {
            NodeLoad &ndld = loads[i];
            loadvec[3 * k + 0][0] += ndld.get_px();
            loadvec[3 * k + 1][0] += ndld.get_py();
            loadvec[3 * k + 2][0] += ndld.get_pr();
        }
    }

    return loadvec;
}

Matrix Frame::fixed_end_moment() {
    Matrix fem(3 * m_nodes.size(), 1, 0.0);

    for (int k = 0; k < m_elements.size(); k++) {
        Element &elm = m_elements[k];
        Matrix elm_fem = elm.global_fixed_end_moment();
        int p1 = 3 * elm.get_node1().get_index();
        int p2 = 3 * elm.get_node2().get_index();

        for (int i = 0; i < 3; i++) {
            fem[p1 + i][0] += elm_fem[i][0];
            fem[p2 + i][0] += elm_fem[3 + i][0];
        }
    }

    return fem;
}

void Frame::compute_displacement() {
    std::vector<int> free_idx;
    for (int k = 0; k < m_nodes.size(); k++) {
        std::vector<bool> fixed = m_nodes[k].get_fixed();

        if (!fixed[0])  // x
            free_idx.push_back(3 * k);
        if (!fixed[1])  // y
            free_idx.push_back(3 * k + 1);
        if (!fixed[2])  // rotation
            free_idx.push_back(3 * k + 2);
    }

    Matrix kmat = stiffness_matrix().to_csv("outputs/" + m_name + "/stiffness_matrix.csv").get(free_idx, free_idx);
    Matrix loadvec = nodal_load_vector().to_csv("outputs/" + m_name + "/nodal_load_vector.csv").get(free_idx, 0);
    Matrix fem = fixed_end_moment().to_csv("outputs/" + m_name + "/fixed_end_moment.csv").get(free_idx, 0);
    Matrix disp_tmp = LinearSolver::solve(kmat, loadvec - fem);

    std::vector<double> disp;
    for (int k = 0; k < m_nodes.size(); k++) {
        static int f = 0;
        int p = 3 * k;
        disp.assign(3, 0);

        for (int i = 0; i < 3; i++) {
            if (p + i == free_idx[f])
                disp[i] = disp_tmp[f++][0];
        }

        m_nodes[k].set_displacement(disp[0], disp[1], disp[2]);
    }
}

void Frame::compute_reaction() {
    std::ofstream outfile;
    outfile.open("outputs/" + m_name + "/reaction.json");

    json jsn;
    jsn["element"] = json::array();

    for (int k = 0; k < m_elements.size(); k++) {
        //json elm_jsn = m_elements[k].json_object();
        jsn["element"].push_back(m_elements[k].json_object());
    }

    outfile << std::setw(4) << jsn << std::endl;
}



Material& FrameUtil::find_material(int id, Frame& frame) {
    std::vector<Material> &materials = frame.get_materials();
    for (int k = 0; k < materials.size(); k++) {
        Material &mtrl = materials[k];
        if (mtrl.get_id() == id)
            return mtrl;
    }
    throw "Error: The material matching id does not exist.";
}

Node& FrameUtil::find_node(int id, Frame& frame) {
    std::vector<Node> &nodes = frame.get_nodes();
    for (int k = 0; k < nodes.size(); k++) {
        Node &nde = nodes[k];
        if (nde.get_id() == id)
            return nde;
    }
    throw "Error: The node matching id does not exist.";
}

Element& FrameUtil::find_element(int id, Frame& frame) {
    std::vector<Element> &elements = frame.get_elements();
    for (int k = 0; k < elements.size(); k++) {
        Element &mb = elements[k];
        if (mb.get_id() == id)
            return mb;
    }
    throw "Error: The element matching id does not exist.";
}

void FrameUtil::read_material_info(std::ifstream &infile, Frame& frame) {
    std::string line;
    std::regex comment("^//.*");
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

        frame.get_materials().push_back(Material(id, A, E, I));
    }
}

void FrameUtil::read_node_info(std::ifstream &infile, Frame& frame) {
    std::string line;
    std::regex comment("^//.*");
    int id;
    double x, y;
    bool fx, fy, fr;

    while (getline(infile, line)) {
        static int index = 0;

        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;

        std::stringstream ss;
        ss.str(line);
        ss >> id >> x >> y >> fx >> fy >> fr;
        frame.get_nodes().push_back(Node(index++, id, x, y, fx, fy, fr));
    }
}

void FrameUtil::read_element_info(std::ifstream &infile, Frame& frame) {
    std::string line;
    std::regex comment("^//.*");
    int id, node1_id, node2_id, material_id;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;

        std::stringstream ss;
        ss.str(line);
        ss >> id >> node1_id >> node2_id >> material_id;
        frame.get_elements().push_back(Element(id, find_node(node1_id, frame), find_node(node2_id, frame), find_material(material_id, frame)));
    }
}

void FrameUtil::read_nodeload_info(std::ifstream &infile, Frame& frame) {
    std::string line;
    std::regex comment("^//.*");
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
        NodeLoad ndld(id, px, py, pr);
        find_node(object_id, frame).add_load(ndld);
    }
}

void FrameUtil::read_elementload_info(std::ifstream &infile, Frame& frame) {
    std::string line;
    std::regex comment("^//.*");
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
        ElementLoad emld(id, loadtype, magnitude, a, b);
        find_element(object_id, frame).add_load(emld);
    }
}

Frame FrameUtil::construct(std::string frame_name) {
    std::ifstream infile;
    infile.open("data/" + frame_name + ".txt");
    std::string line;
    Frame frame(frame_name);

    while (getline(infile, line)) {
        if (line == "@Material")
            read_material_info(infile, frame);

        else if (line == "@Node")
            read_node_info(infile, frame);

        else if (line == "@Element")
            read_element_info(infile, frame);

        else if (line == "@NodeLoad")
            read_nodeload_info(infile, frame);

        else if (line == "@ElementLoad")
            read_elementload_info(infile, frame);
    }

    infile.close();
    return frame;
}
