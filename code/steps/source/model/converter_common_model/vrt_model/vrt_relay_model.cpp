#include "header/model/converter_common_models/vrt_model/vrt_relay_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

VRT_RELAY_MODEL::VRT_RELAY_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
    bus_ptr = NULL;
    set_vrt_status(VRT_NORMAL_MODE);
    vrt_trip_timer.set_toolkit(toolkit);
}

VRT_RELAY_MODEL::~VRT_RELAY_MODEL()
{
    ;
}

void VRT_RELAY_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
    vrt_trip_timer.set_toolkit(toolkit);
}

STEPS& VRT_RELAY_MODEL::get_toolkit() const
{
    return *toolkit;
}

void VRT_RELAY_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

void VRT_RELAY_MODEL::set_bus_pointer(BUS* bus)
{
    bus_ptr = bus;
}

NONBUS_DEVICE* VRT_RELAY_MODEL::get_device_pointer() const
{
    return device;
}

BUS* VRT_RELAY_MODEL::get_bus_pointer() const
{
    return bus_ptr;
}

unsigned int VRT_RELAY_MODEL::get_bus() const
{
    return bus_ptr->get_bus_number();
}

double VRT_RELAY_MODEL::get_bus_voltage()
{
    return bus_ptr->get_positive_sequence_voltage_in_pu();
}

void VRT_RELAY_MODEL::set_vrt_trip_time_delay_in_s(double t)
{
    vrt_trip_timer.set_timer_interval_in_s(t);
}

void VRT_RELAY_MODEL::set_vrt_point_number(unsigned int n)
{
    vrt_line.set_point_number(n);
}

void VRT_RELAY_MODEL::set_vrt_time_volt_threshold_pair(unsigned int index, double t, double v)
{
    vrt_line.set_point(index, t, v);
}

void VRT_RELAY_MODEL::set_vrt_trip_scale(double scale)
{
    vrt_trip_scale = scale;
}

MULTI_POINT_LINE& VRT_RELAY_MODEL::get_vrt_line()
{
    return vrt_line;
}

TIMER& VRT_RELAY_MODEL::get_vrt_trip_timer()
{
    return vrt_trip_timer;
}

double VRT_RELAY_MODEL::get_vrt_trip_time_delay_in_s() const
{
    return vrt_trip_timer.get_timer_interval_in_s();
}

unsigned int VRT_RELAY_MODEL::get_vrt_point_number() const
{
    return vrt_line.get_point_number();
}

double VRT_RELAY_MODEL::get_vrt_time_threshold(unsigned int index) const
{
    return vrt_line.get_point_x(index);
}

double VRT_RELAY_MODEL::get_vrt_volt_threshold(unsigned int index) const
{
    return vrt_line.get_point_y(index);
}

double VRT_RELAY_MODEL::get_vrt_trip_scale() const
{
    return vrt_trip_scale;
}

void VRT_RELAY_MODEL::set_vrt_status(VRT_STATUS status)
{
    this->status = status;
    if(status==LVRT_DRURING_MODE or status==HVRT_DRURING_MODE)
    {
        STEPS& toolkit = get_toolkit();
        t0_vrt_activated = toolkit.get_dynamic_simulation_time_in_s();
    }
    else
        t0_vrt_activated = INFINITE_THRESHOLD;
}

double VRT_RELAY_MODEL::get_vrt_activation_time() const
{
    return t0_vrt_activated;
}

VRT_STATUS VRT_RELAY_MODEL::get_vrt_status() const
{
    return status;
}
void VRT_RELAY_MODEL::copy_from_const_model(const VRT_RELAY_MODEL& model)
{
    set_vrt_status(model.get_vrt_status());

    unsigned int n = model.get_vrt_point_number();
    set_vrt_point_number(n);
    for(unsigned int i=0; i<n; ++i)
    {
        double t = model.get_vrt_time_threshold(i);
        double v = model.get_vrt_volt_threshold(i);
        set_vrt_time_volt_threshold_pair(i, t, v);
    }
}
