#ifndef __LOAD_H_
#define __LOAD_H_

#include <cstddef>


enum load_t {concentrated, distributed, moment};

class Load {
private:
    bool m_applied;
    load_t m_type;  // 0: concentrated, 1: distributed, 2: moment
    double m_magnitude;
    double m_lx;  // for concentrated load
    double m_ly;  // for concentrated load

public:
    Load();
    Load(int type, double magnitude, double lx, double ly);

    bool& get_applied();
    load_t& get_type();
    double& get_magnitude();
    double& get_lx();
    double& get_ly();
};


#endif
