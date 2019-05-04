#ifndef __STRUCTURES_H_
#define __STRUCTURES_H_

#include "Load.h"


class Node {
private:
    int m_id;
    double m_x;
    double m_y;
    bool m_fx;
    bool m_fy;
    bool m_fr;
    Load m_load;

public:
    Node(int id, double x, double y, bool fx, bool fy, bool fr);

    int& get_id();
    double& get_x();
    double& get_y();
    bool& get_fx();
    bool& get_fy();
    bool& get_fr();
    Load& get_load();

    void set_load(Load &load);
};


#endif
