#include "header/device/source_var.h"
SOURCE_VAR::SOURCE_VAR()
{
    p_generation_MW = 0.0;
    q_generation_MVar = 0.0;

    bus_to_regulate = 0;

    source_Z_pu = 0.0;

    p_max_MW = 0.0;
    p_min_MW = 0.0;
    q_max_MVar = 0.0;
    q_min_MVar = 0.0;
    voltage_to_regulate_pu = 1.0;
}
