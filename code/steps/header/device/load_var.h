#ifndef LOAD_VAR_H
#define LOAD_VAR_H

#include <complex>

using namespace std;

struct LOAD_VAR
{   complex<double> s_constant_power_in_MVA,
                    s_constant_current_in_MVA,
                    s_constant_impedance_in_MVA;
    unsigned int area_number, zone_number, owner_number;
    bool interruptable;

    complex<double> s_negative_sequence_in_MVA, s_zero_sequence_in_MVA;
};
#endif // LOAD_VAR_H
