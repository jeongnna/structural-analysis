#include "Load.h"


Load::Load(double px, double py, double pr)
: m_px(px), m_py(py), m_pr(pr)
{
    // empty
}

double& Load::get_px() {return m_px;}
double& Load::get_py() {return m_py;}
double& Load::get_pr() {return m_pr;}
