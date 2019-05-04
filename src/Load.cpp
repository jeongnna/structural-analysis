#include "Load.h"


Load::Load() {
    m_applied = false;
}

Load::Load(int type, double magnitude, double lx, double ly)
: m_magnitude(magnitude), m_lx(lx), m_ly(ly)
{
    m_applied = true;
    switch (type) {
        case 0:
            m_type = concentrated;
            break;
        case 1:
            m_type = distributed;
            break;
        case 2:
            m_type = moment;
            break;
    }
}

bool& Load::get_applied() {return m_applied;}
load_t& Load::get_type() {return m_type;}
double& Load::get_magnitude() {return m_magnitude;}
double& Load::get_lx() {return m_lx;}
double& Load::get_ly() {return m_ly;}
