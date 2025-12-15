#include "header/meter/bus_meter_getter.h"
#include "header/device/bus.h"
#include "header/basic/steps_enum.h"
#include "header/steps_namespace.h"

double bus_meter_getter::get_voltage_pu(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_positive_sequence_voltage_in_pu();
    else
        return 0.0;
}

double bus_meter_getter::get_voltage_kV(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_positive_sequence_voltage_in_kV();
    else
        return 0.0;
}

double bus_meter_getter::get_angle_deg(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_positive_sequence_angle_in_deg();
    else
        return 0.0;
}

double bus_meter_getter::get_angle_rad(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_positive_sequence_angle_in_rad();
    else
        return 0.0;
}

double bus_meter_getter::get_frequency_deviation_pu(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_frequency_deviation_in_pu();
    else
        return 0.0;
}

double bus_meter_getter::get_frequency_deviation_Hz(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_frequency_deviation_in_Hz();
    else
        return 0.0;
}

double bus_meter_getter::get_frequency_pu(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return 1.0 + bus->get_frequency_deviation_in_pu();
    else
        return 0.0;
}

double bus_meter_getter::get_frequency_Hz(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_base_frequency_in_Hz() + bus->get_frequency_deviation_in_Hz();
    else
        return 0.0;
}

double bus_meter_getter::get_rocof_Hz_per_second(const METER_PARAMETER& para)
{
    BUS* bus = (BUS*) para.device_pointer;
    if(bus && bus->get_bus_type() != OUT_OF_SERVICE)
        return bus->get_ROCOF_in_Hz_per_second();
    else
        return 0.0;
}

double bus_meter_getter::get_rocof_pu_per_second(const METER_PARAMETER& para)
{
    return 0.0;
}

double bus_meter_getter::get_rocov_kV_per_second(const METER_PARAMETER& para)
{
    return 0.0;
}

double bus_meter_getter::get_rocov_pu_per_second(const METER_PARAMETER& para)
{
    return 0.0;
}
