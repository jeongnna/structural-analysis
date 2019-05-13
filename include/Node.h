#ifndef __STRUCTURES_H_
#define __STRUCTURES_H_

#include <vector>
#include "Load.h"


class Node {
private:
    int m_id;
    double m_x;
    double m_y;
    bool m_fx;
    bool m_fy;
    bool m_fr;
    std::vector<Load> m_loads;

public:
    Node(int id, double x, double y, bool fx, bool fy, bool fr);

    int& get_id();
    double& get_x();
    double& get_y();
    bool& get_fx();
    bool& get_fy();
    bool& get_fr();
    std::vector<Load>& get_load();

    void add_load(Load &load);
};


#endif
