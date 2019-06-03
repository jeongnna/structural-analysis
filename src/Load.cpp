#include "Load.h"


NodeLoad::NodeLoad(int id, double px, double py, double pr)
: m_id(id), m_px(px), m_py(py), m_pr(pr)
{
    // empty
}

int& NodeLoad::get_id() {return m_id;}
double& NodeLoad::get_px() {return m_px;}
double& NodeLoad::get_py() {return m_py;}
double& NodeLoad::get_pr() {return m_pr;}


ElementLoad::ElementLoad(int id, int loadtype, double magnitude, double a, double b)
: m_id(id), m_loadtype(loadtype), m_magnitude(magnitude), m_a(a), m_b(b)
{
    // empty
}

int& ElementLoad::get_id() {return m_id;}
int& ElementLoad::get_loadtype(){return m_loadtype;}
double& ElementLoad::get_magnitude(){return m_magnitude;}
double& ElementLoad::get_a(){return m_a;}
double& ElementLoad::get_b(){return m_b;}
