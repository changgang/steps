#include "header/model/converter_common_models/vrt_model/vrt_control_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

VRT_CONTROL_MODEL::VRT_CONTROL_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
    bus_ptr = NULL;
    set_vrt_status(VRT_NORMAL_MODE);
}

VRT_CONTROL_MODEL::~VRT_CONTROL_MODEL()
{
    ;
}

void VRT_CONTROL_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& VRT_CONTROL_MODEL::get_toolkit() const
{
    return *toolkit;
}

void VRT_CONTROL_MODEL::set_bus_pointer(BUS* bus)
{
    bus_ptr = bus;
}

BUS* VRT_CONTROL_MODEL::get_bus_pointer() const
{
    return bus_ptr;
}

unsigned int VRT_CONTROL_MODEL::get_bus() const
{
    return bus_ptr->get_bus_number();
}

double VRT_CONTROL_MODEL::get_bus_voltage()
{
    return bus_ptr->get_positive_sequence_voltage_in_pu();
}


void VRT_CONTROL_MODEL::set_vrt_voltage_threshold(double v)
{
    vrt_voltage_threshold = v;
}

double VRT_CONTROL_MODEL::get_vrt_voltage_threshold() const
{
    return vrt_voltage_threshold;
}

void VRT_CONTROL_MODEL::set_vrt_status(VRT_STATUS status)
{
    this->status = status;
    if(status==LVRT_DRURING_MODE or status==HVRT_DRURING_MODE)
    {
        STEPS& toolkit = get_toolkit();
        t0_vrt_activated = toolkit.get_dynamic_simulation_time_in_s();
    }
    else
    {
        if(status==LVRT_RECOVER_MODE or status==HVRT_RECOVER_MODE)
        {
            STEPS& toolkit = get_toolkit();
            t0_vrt_start2recover = toolkit.get_dynamic_simulation_time_in_s();
        }
        else
            t0_vrt_activated = INFINITE_THRESHOLD;
    }
}

double VRT_CONTROL_MODEL::get_vrt_activation_time() const
{
    return t0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_vrt_start2recover_time() const
{
    return t0_vrt_start2recover;
}

VRT_STATUS VRT_CONTROL_MODEL::get_vrt_status() const
{
    return status;
}
void VRT_CONTROL_MODEL::copy_from_const_model(const VRT_CONTROL_MODEL& model)
{
    set_vrt_status(model.get_vrt_status());
    set_vrt_voltage_threshold(model.get_vrt_voltage_threshold());
}
