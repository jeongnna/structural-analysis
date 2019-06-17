#include <iostream>
#include <cmath>
#include "Matrix.h"
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
int& ElementLoad::get_loadtype() {return m_loadtype;}
double& ElementLoad::get_magnitude() {return m_magnitude;}
double& ElementLoad::get_a() {return m_a;}
double& ElementLoad::get_b() {return m_b;}

Matrix ElementLoad::fixed_end_moment() {
    Matrix fem(6, 1, 0.0);
    double length = m_a + m_b;
    double m1, m2, v1, v2;

    switch (m_loadtype) {
        // concentrate load
        case 0:
            m1 = m_magnitude * m_a * pow(m_b, 2) / pow(length, 2);
            m2 = - m_magnitude * pow(m_a, 2) * m_b / pow(length, 2);
            v1 = (m_magnitude * m_b + m1 + m2) / length;
            v2 = (m_magnitude * m_a - m1 - m2) / length;
            fem[1][0] += v1;
            fem[2][0] += m1;
            fem[4][0] += v2;
            fem[5][0] += m2;
            break;

        // moment
        case 1:
            m1 = m_magnitude * m_b / length * (2 - 3 * m_b / length);
            m2 = - m_magnitude * m_a / length * (2 - 3 * m_a / length);
            v1 = (m_magnitude + m1 + m2) / length;
            v2 = - (m_magnitude + m1 + m2) / length;
            fem[1][0] += v1;
            fem[2][0] += m1;
            fem[4][0] += v2;
            fem[5][0] += m2;
            break;

        // distributed load (from 0 to a)
        case 2:
            double w = m_magnitude * pow(m_a, 2) / 12;
            double r = m_a / length;
            m1 = w * (6 - 8 * r + 3 * pow(r, 2));
            m2 = - w * (4 * r - 3 * pow(r, 2));
            v1 = (m_magnitude * m_a * (m_a / 2 + m_b) + m1 + m2) / length;
            v2 = (m_magnitude * m_a * m_a / 2 - m1 - m2) / length;
            fem[1][0] += v1;
            fem[2][0] += m1;
            fem[4][0] += v2;
            fem[5][0] += m2;
            break;
    }

    return fem;
}
