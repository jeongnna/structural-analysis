#ifndef __LOAD_H_
#define __LOAD_H_

#include "Matrix.h"


class NodeLoad {
private:
    int m_id;
    double m_px;
    double m_py;
    double m_pr;

public:
    NodeLoad(int id, double px, double py, double pr);

    int& get_id();
    double& get_px();
    double& get_py();
    double& get_pr();
};


class ElementLoad {
private:
    int m_id;
    int m_loadtype;
    double m_magnitude;
    double m_a;
    double m_b;

public:
    ElementLoad(int id, int loadtype, double magnitude, double a, double b);

    int& get_id();
    int& get_loadtype();
    double& get_magnitude();
    double& get_a();
    double& get_b();

    Matrix fixed_end_moment();
};


#endif
