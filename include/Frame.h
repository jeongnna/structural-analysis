#ifndef __FRAME_H_
#define __FRAME_H_

#include <vector>
#include "Matrix.h"
#include "Load.h"
#include "Node.h"
#include "Member.h"


class Frame {
private:
    std::vector<Node> &m_nodes;
    std::vector<Member> &m_members;

public:
    Frame(std::vector<Node> &nodes, std::vector<Member> &members);

    void print();

    Matrix stiffness_matrix();
    Matrix load_vector();
    Matrix displacement();
};


#endif
