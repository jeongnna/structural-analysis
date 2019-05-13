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


#endif
