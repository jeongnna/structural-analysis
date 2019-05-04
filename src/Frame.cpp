#include <iostream>
#include <vector>
#include "Matrix.h"
#include "Load.h"
#include "Node.h"
#include "Beam.h"
#include "Frame.h"


Frame::Frame(std::vector<Node> &nodes, std::vector<Beam> &beams)
: m_nodes(nodes), m_beams(beams)
{
    m_num_nodes = m_nodes.size();
    m_num_beams = m_beams.size();
}

void Frame::print() {
    std::cout << "Number of nodes: " << m_num_nodes << std::endl;
    std::cout << "Number of beams: " << m_num_beams << std::endl;
}

Matrix Frame::stiffness_matrix() {
    Matrix ke_frame(3 * m_num_nodes, 3 * m_num_nodes, 0);

    for (int k = 0; k < m_num_beams; k++) {
        Beam &bm = m_beams[k];
        Matrix ke = bm.global_stiffness_matrix();
        int p1 = 3 * bm.get_node1().get_id();
        int p2 = 3 * bm.get_node2().get_id();

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
    Matrix loadvec_frame(3 * m_num_nodes, 1, 0);

    // load on beams
    for (int k = 0; k < m_num_beams; k++) {
        Beam &bm = m_beams[k];
        Matrix loadvec = bm.global_load_vector();
        int p1 = 3 * bm.get_node1().get_id();
        int p2 = 3 * bm.get_node2().get_id();

        for (int i = 0; i < 3; i++) {
            loadvec_frame(p1 + i, 0) += loadvec(i, 0);
            loadvec_frame(p2 + i, 0) += loadvec(3 + i, 0);
        }
    }

    // load on nodes
    for (int k = 0; k < m_num_nodes; k++) {
        Node &nd = m_nodes[k];
        Load &nodal_load = nd.get_load();
        if (!nodal_load.get_applied())
            continue;
        load_t loadtype = nodal_load.get_type();
        int p = 3 * nd.get_id();

        if (loadtype == concentrated) {
            double &magnitude = nodal_load.get_magnitude();
            double &lx = nodal_load.get_lx();
            double &ly = nodal_load.get_ly();
            loadvec_frame(p + 0, 0) += (magnitude * lx);
            loadvec_frame(p + 1, 0) += (magnitude * ly);
        }
        else if (loadtype == distributed) {

        }
        else if (loadtype == moment) {
            loadvec_frame(p + 2, 0) += nodal_load.get_magnitude();
        }
    }

    return loadvec_frame;
}
