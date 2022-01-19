#ifndef VSC_STATION_H_INCLUDED
#define VSC_STATION_H_INCLUDED
#include "header/basic/steps_enum.h"
#include "header/block/block.h"
#include "header/block/pi_block.h"
#include <complex>
#include <string>
using namespace std;

class BUS;

class VSC_MODEL_VAR
{
    public:
    VSC_MODEL_VAR(STEPS& toolkit);
    ~VSC_MODEL_VAR();
    VSC_MODEL_VAR(const VSC_MODEL_VAR& var);

    unsigned int converter_bus;
    BUS* converter_busptr;
    unsigned int active_power_control_mode;
    unsigned int reactive_power_control_mode;
    PI_BLOCK active_power_control_block;
    PI_BLOCK reactive_power_control_block;
    PI_BLOCK ac_voltage_control_block;
    PI_BLOCK ud_voltage_control_block;
    INTEGRAL_BLOCK dc_voltage_block;

};
#endif // VSC_STATION_H_INCLUDED
