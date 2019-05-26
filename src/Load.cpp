#include "Load.h"


Load::Load(double px, double py, double pr)
: m_px(px), m_py(py), m_pr(pr)
{
    // empty
}

double& Load::get_px() {return m_px;}
double& Load::get_py() {return m_py;}
double& Load::get_pr() {return m_pr;}


MemberLoad::MemberLoad(int loadtype, double magnitude, double a, double b)
: m_loadtype(loadtype), m_magnitude(magnitude), m_a(a), m_b(b)
{
    // empty
}

int& MemberLoad::get_loadtype(){return m_loadtype;}
double& MemberLoad::get_magnitude(){return m_magnitude;}
double& MemberLoad::get_a(){return m_a;}
double& MemberLoad::get_b(){return m_b;}
