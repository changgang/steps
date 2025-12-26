#ifndef SV_SOURCE_METER_GETTER_H
#define SV_SOURCE_METER_GETTER_H

#include "header/meter/meter_parameter_struct.h"
#include <string>
using namespace std;

namespace sv_source_meter_interface
{
    double get_terminal_current_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para);
    double get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para);
    double get_svs_control_model_internal_variable(const METER_PARAMETER& para);
}


#endif // SV_SOURCE_METER_GETTER_H
