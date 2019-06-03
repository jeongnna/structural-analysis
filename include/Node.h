#ifndef __NODE_H_
#define __NODE_H_

#include <vector>
#include "Load.h"


class Node {
private:
    int m_index, m_id;
    double m_x, m_y;
    std::vector<bool> m_fixed;
    std::vector<NodeLoad> m_loads;
    std::vector<double> m_displacement;

public:
    Node(int index, int id, double x, double y, bool fx, bool fy, bool fr);

    int& get_index();
    int& get_id();
    double& get_x();
    double& get_y();
    std::vector<bool>& get_fixed();
    std::vector<NodeLoad>& get_loads();
    std::vector<double>& get_displacement();

    void print();

    void add_load(NodeLoad &load);
    void set_displacement(double &dx, double &dy, double &dr);
};


#endif
