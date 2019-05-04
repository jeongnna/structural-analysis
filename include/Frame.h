#ifndef __FRAME_H_
#define __FRAME_H_

#include <vector>
#include "Matrix.h"
#include "Load.h"
#include "Node.h"
#include "Beam.h"


class Frame {
private:
    std::vector<Node> &m_nodes;
    std::vector<Beam> &m_beams;
    int m_num_nodes;
    int m_num_beams;

public:
    Frame(std::vector<Node> &nodes, std::vector<Beam> &beams);

    void print();

    Matrix stiffness_matrix();
    Matrix load_vector();
};


#endif
