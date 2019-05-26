#ifndef __LOAD_H_
#define __LOAD_H_


class Load {
private:
    double m_px;
    double m_py;
    double m_pr;

public:
    Load(double m_px, double m_py, double m_pr);

    double& get_px();
    double& get_py();
    double& get_pr();
};


class MemberLoad {
private:
    int m_loadtype;
    double m_magnitude;
    double m_a;
    double m_b;

public:
    MemberLoad(int loadtype, double magnitude, double a, double b);

    int& get_loadtype();
    double& get_magnitude();
    double& get_a();
    double& get_b();
};


#endif
