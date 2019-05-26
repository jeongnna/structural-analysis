#include <iostream>
#include <vector>
#include "Matrix.h"
#include "LinearSolver.h"
#include "Load.h"
#include "Node.h"
#include "Member.h"
#include "Frame.h"


Frame::Frame(std::vector<Node> &nodes, std::vector<Member> &members)
: m_nodes(nodes), m_members(members)
{
    // empty
}

void Frame::print() {
    std::cout << "Number of nodes: " << m_nodes.size() << std::endl;
    std::cout << "Number of members: " << m_members.size() << std::endl;
}

Matrix Frame::stiffness_matrix() {
    int mat_size = 3 * m_nodes.size();
    Matrix ke_frame(mat_size, mat_size, 0);

    for (int k = 0; k < m_members.size(); k++) {
        Member &mbr = m_members[k];
        Matrix ke = mbr.global_stiffness_matrix();
        int p1 = 3 * mbr.get_node1().get_id();
        int p2 = 3 * mbr.get_node2().get_id();

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
        std::vector<Load> &loads = nde.get_load();
        int p = 3 * nde.get_id();

        for (int i = 0; i < loads.size(); i++) {
            Load &load = loads[i];
            loadvec(p + 0, 0) += load.get_px();
            loadvec(p + 1, 0) += load.get_py();
            loadvec(p + 2, 0) += load.get_pr();
        }
    }

    return loadvec;
}

Matrix Frame::fixed_end_moment() {
    Matrix fem(3 * m_nodes.size(), 1, 0);

    for (int k = 0; k < m_members.size(); k++) {
        Member &mbr = m_members[k];
        Matrix mbr_fem = mbr.global_fixed_end_moment();
        int p1 = 3 * mbr.get_node1().get_id();
        int p2 = 3 * mbr.get_node2().get_id();

        for (int i = 0; i < 3; i++) {
            fem(p1 + i, 0) += mbr_fem(i, 0);
            fem(p2 + i, 0) += mbr_fem(3 + i, 0);
        }
    }
}

void Frame::compute_displacement() {
    std::vector<int> free_idx;
    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nde = m_nodes[k];
        std::vector<bool> fixed = nde.get_fixed();
        int p = 3 * nde.get_id();
        if (!fixed[0])  // x
            free_idx.push_back(p);
        if (!fixed[1])  // y
            free_idx.push_back(p + 1);
        if (!fixed[2])  // rotation
            free_idx.push_back(p + 2);
    }

    LinearSolver linearsolver;
    Matrix ke = stiffness_matrix()(free_idx, free_idx);
    Matrix loadvec = nodal_load_vector()(free_idx, 0);
    Matrix fem = fixed_end_moment()(free_idx, 0);
    Matrix disp_tmp = linearsolver.solve(ke, loadvec - fem);

//    Matrix disp = Matrix(3 * m_nodes.size(), 1, 0);
//    int f = 0;
//    for (int p = 0; p < 3 * m_nodes.size(); p++) {
//        if (p == free_idx[f])
//            disp(p, 0) = disp_tmp(f++, 0);
//        else
//            disp(p, 0) = 0;
//    }
//
//    for (int k = 0; k < m_nodes.size(); k++) {
//        Node &nde = m_nodes[k];
//        int p = 3 * nde.get_id();
//        nde.set_displacement(disp(p, 0), disp(p + 1, 0), disp(p + 2, 0));
//    }
    std::vector<double> disp;

    for (int k = 0; k < m_nodes.size(); k++) {
        static int f = 0;
        Node &nde = m_nodes[k];
        int p = 3 * nde.get_id();
        disp.assign(3, 0);

        for (int i = 0; i < 3; i++) {
            if (p + i == free_idx[f])
                disp[i] = disp_tmp(f++, 0);
        }

        nde.set_displacement(disp[0], disp[1], disp[2]);
    }
}
