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
        Member &mb = m_members[k];
        Matrix ke = mb.global_stiffness_matrix();
        int p1 = 3 * mb.get_node1().get_id();
        int p2 = 3 * mb.get_node2().get_id();

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

Matrix Frame::load_vector() {
    Matrix loadvec_frame(3 * m_nodes.size(), 1, 0);

    // load on members
    for (int k = 0; k < m_members.size(); k++) {
        Member &mb = m_members[k];
        Matrix mb_fem = mb.global_fem();
        int p1 = 3 * mb.get_node1().get_id();
        int p2 = 3 * mb.get_node2().get_id();

        for (int i = 0; i < 3; i++) {
            loadvec_frame(p1 + i, 0) -= mb_fem(i, 0);
            loadvec_frame(p2 + i, 0) -= mb_fem(3 + i, 0);
        }
    }

    // load on nodes
    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nd = m_nodes[k];
        std::vector<Load> &loads = nd.get_load();
        int p = 3 * nd.get_id();

        for (int i = 0; i < loads.size(); i++) {
            Load &load = loads[i];
            loadvec_frame(p + 0, 0) += load.get_px();
            loadvec_frame(p + 1, 0) += load.get_py();
            loadvec_frame(p + 2, 0) += load.get_pr();
        }
    }

    return loadvec_frame;
}

Matrix Frame::displacement() {
    std::vector<int> free;
    for (int k = 0; k < m_nodes.size(); k++) {
        Node &nd = m_nodes[k];
        int p = 3 * k;
        if (!nd.get_fx())
            free.push_back(p);
        if (!nd.get_fy())
            free.push_back(p + 1);
        if (!nd.get_fr())
            free.push_back(p + 2);
    }

    LinearSolver linsolver;
    Matrix ke = stiffness_matrix();
    Matrix loadvec = load_vector();
    Matrix disp_tmp = linsolver.solve(ke(free, free), loadvec(free, 0));

    Matrix disp = Matrix(3 * m_nodes.size(), 1, 0);
    for (int p = 0; p < 3 * m_nodes.size(); p++) {
        int count = 0;
        if (p == free[count])
            disp(p, 0) = disp_tmp(count++, 0);
        else
            disp(p, 0) = 0;
        if (count != free.size())
            throw "Error";
    }

    return disp;
}
