#include "header/device/bus_var.h"
BUS_VAR::BUS_VAR()
{
    bus_name = "";
    base_voltage_in_kV = 0.0;

    bus_type = OUT_OF_SERVICE;
    area_number = 0;
    zone_number = 0;
    owner_number = 0;

    negative_sequence_voltage_in_pu = 1.0;
    negative_sequence_angle_in_rad = 0.0;
    zero_sequence_voltage_in_pu = 1.0;
    zero_sequence_angle_in_rad = 0.0;

    negative_sequence_Euler_complex_number = 1.0;
    zero_sequence_Euler_complex_number = 1.0;

    normal_voltage_upper_limit_in_pu = 1.1;
    normal_voltage_lower_limit_in_pu = 0.9;
    emergency_voltage_upper_limit_in_pu = 1.1;
    emergency_voltage_lower_limit_in_pu = 0.9;

    voltage_to_regulate_in_pu = 0.0;
}

BUS_VAR::~BUS_VAR()
{

}
