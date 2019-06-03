#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Matrix.h"
#include "LinearSolver.h"
#include "Load.h"
#include "Node.h"
#include "Element.h"
#include "Frame.h"


// TODO: wrap `comment` in a namespace.
std::regex comment("^//.*");


Frame::Frame() {
    // empty
}


std::vector<Material>& Frame::get_materials() {return m_materials;}
std::vector<Node>& Frame::get_nodes() {return m_nodes;}
std::vector<Element>& Frame::get_elements() {return m_elements;}


void Frame::print() {
    std::cout << "# Frame information" << std::endl;
    std::cout << std::endl;

    // print node information
    std::cout << "@Node" << std::endl;
    std::cout << "Number of nodes: " << m_nodes.size() << std::endl;
    for (int k = 0; k < m_nodes.size(); k++)
        m_nodes[k].print();
    std::cout << std::endl;

    // print element information
    std::cout << "@Element" << std::endl;
    std::cout << "Number of elements: " << m_elements.size() << std::endl;
    for (int k = 0; k < m_elements.size(); k++)
        m_elements[k].print();
    std::cout << std::endl;
}

Material& Frame::find_material(int id) {
    int n_materials = m_materials.size();

    for (int k = 0; k < n_materials; k++) {
        Material &mtrl = m_materials[k];
        if (mtrl.get_id() == id)
            return mtrl;
    }
    throw "Error: The material matching id does not exist.";
}

Node& Frame::find_node(int id) {
    int n_nodes = m_nodes.size();

    for (int k = 0; k < n_nodes; k++) {
        Node &nde = m_nodes[k];
        if (nde.get_id() == id)
            return nde;
    }
    throw "Error: The node matching id does not exist.";
}

Element& Frame::find_element(int id) {
    int n_elements = m_elements.size();

    for (int k = 0; k < n_elements; k++) {
        Element &mb = m_elements[k];
        if (mb.get_id() == id)
            return mb;
    }
    throw "Error: The element matching id does not exist.";
}

void Frame::read_material_info(std::ifstream &infile) {
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

void Frame::read_node_info(std::ifstream &infile) {
    std::string line;
    int id;
    double x, y;
    bool f1, f2, f3;

    while (getline(infile, line)) {
        static int index = 0;

        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;

        std::stringstream ss;
        ss.str(line);
        ss >> id >> x >> y >> f1 >> f2 >> f3;

        Node nde(index, id, x, y, f1, f2, f3);
        m_nodes.push_back(nde);

        index++;
    }
}

void Frame::read_element_info(std::ifstream &infile) {
    std::string line;
    int id, node1_id, node2_id, material_id;

    while (getline(infile, line)) {
        if (line.empty())
            break;
        else if (std::regex_match(line, comment))
            continue;

        std::stringstream ss;
        ss.str(line);
        ss >> id >> node1_id >> node2_id >> material_id;

        Element elm(id, find_node(node1_id), find_node(node2_id), find_material(material_id));
        m_elements.push_back(elm);
    }
}

void Frame::read_load_info(std::ifstream &infile) {
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

        NodeLoad ndld(id, px, py, pr);
        Node &nde = find_node(object_id);
        nde.add_load(ndld);
    }
}

void Frame::read_elementload_info(std::ifstream &infile) {
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

        ElementLoad emld(id, loadtype, magnitude, a, b);
        Element &elm = find_element(object_id);
        elm.add_load(emld);
    }
}

void Frame::construct(std::string inputfile) {
    std::ifstream infile;
    infile.open(inputfile);
    std::string line;

    while (getline(infile, line)) {
        if (line == "@Material")
            read_material_info(infile);

        else if (line == "@Node")
            read_node_info(infile);

        else if (line == "@Element")
            read_element_info(infile);

        else if (line == "@NodeLoad")
            read_load_info(infile);

        else if (line == "@ElementLoad")
            read_elementload_info(infile);
    }

    infile.close();
}


Matrix Frame::stiffness_matrix() {
    int mat_size = 3 * m_nodes.size();
    Matrix ke_frame(mat_size, mat_size, 0);

    for (int k = 0; k < m_elements.size(); k++) {
        Element &elm = m_elements[k];
        Matrix ke = elm.global_stiffness_matrix();
        int p1 = 3 * elm.get_node1().get_index();
        int p2 = 3 * elm.get_node2().get_index();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ke_frame(p1 + i, p1 + j) += ke(i, j);
                ke_frame(p1 + i, p2 + j) += ke(i, 3 + j);
                ke_frame(p2 + i, p1 + j) += ke(3 + i, j);
                ke_frame(p2 + i, p2 + j) += ke(3 + i, 3 + j);
            }
        }
    }

    return ke_frame;
}

Matrix Frame::nodal_load_vector() {
    Matrix loadvec(3 * m_nodes.size(), 1, 0);

    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nde = m_nodes[k];
        std::vector<NodeLoad> &loads = nde.get_loads();

        for (int i = 0; i < loads.size(); i++) {
            NodeLoad &ndld = loads[i];
            loadvec(3 * k + 0, 0) += ndld.get_px();
            loadvec(3 * k + 1, 0) += ndld.get_py();
            loadvec(3 * k + 2, 0) += ndld.get_pr();
        }
    }

    return loadvec;
}

Matrix Frame::fixed_end_moment() {
    Matrix fem(3 * m_nodes.size(), 1, 0);

    for (int k = 0; k < m_elements.size(); k++) {
        Element &elm = m_elements[k];
        Matrix elm_fem = elm.global_fixed_end_moment();
        int p1 = 3 * elm.get_node1().get_index();
        int p2 = 3 * elm.get_node2().get_index();

        for (int i = 0; i < 3; i++) {
            fem(p1 + i, 0) += elm_fem(i, 0);
            fem(p2 + i, 0) += elm_fem(3 + i, 0);
        }
    }

    return fem;
}

void Frame::compute_displacement() {
    std::vector<int> free_idx;
    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nde = m_nodes[k];
        std::vector<bool> fixed = nde.get_fixed();

        if (!fixed[0])  // x
            free_idx.push_back(3 * k);
        if (!fixed[1])  // y
            free_idx.push_back(3 * k + 1);
        if (!fixed[2])  // rotation
            free_idx.push_back(3 * k + 2);
    }

    LinearSolver linearsolver;
    Matrix ke = stiffness_matrix()(free_idx, free_idx);
    Matrix loadvec = nodal_load_vector()(free_idx, 0);
    Matrix fem = fixed_end_moment()(free_idx, 0);
    Matrix disp_tmp = linearsolver.solve(ke, loadvec - fem);

    std::vector<double> disp;
    for (int k = 0; k < m_nodes.size(); k++) {
        static int f = 0;
        Node &nde = m_nodes[k];
        int p = 3 * k;
        disp.assign(3, 0);

        for (int i = 0; i < 3; i++) {
            if (p + i == free_idx[f])
                disp[i] = disp_tmp(f++, 0);
        }

        nde.set_displacement(disp[0], disp[1], disp[2]);
    }
}
