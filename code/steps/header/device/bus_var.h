#ifndef BUS_VAR_H
#define BUS_VAR_H

#include "header/basic/fault.h"

#include <string>
#include <complex>

using namespace std;

struct BUS_VAR
{
    string bus_name;
    double base_voltage_in_kV;

    BUS_TYPE bus_type;
    unsigned int area_number;
    unsigned int zone_number;
    unsigned int owner_number;

    double negative_sequence_voltage_in_pu;
    double negative_sequence_angle_in_rad;
    double zero_sequence_voltage_in_pu;
    double zero_sequence_angle_in_rad;

    complex<double> negative_sequence_Euler_complex_number;
    complex<double> zero_sequence_Euler_complex_number;

    double normal_voltage_upper_limit_in_pu;
    double normal_voltage_lower_limit_in_pu;
    double emergency_voltage_upper_limit_in_pu;
    double emergency_voltage_lower_limit_in_pu;

    double voltage_to_regulate_in_pu;

    FAULT fault;
};

#endif // BUS_VAR_H
