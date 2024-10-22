#include "header/model/common_model/vrt_model/vrt_relay_model.h"

VRT_RELAY_MODEL::VRT_RELAY_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{
    vrt_trip_timer.set_toolkit(toolkit);
    clear();
}

VRT_RELAY_MODEL::~VRT_RELAY_MODEL()
{
    ;
}

void VRT_RELAY_MODEL::copy_from_const_model(const VRT_RELAY_MODEL& model)
{
    clear();
    MULTI_POINT_LINE m = model.get_vrt_line();
    vrt_line = m;
    set_vrt_trip_scale(model.get_vrt_trip_scale());
}

void VRT_RELAY_MODEL::clear()
{
    vrt_line.clear();
    vrt_trip_timer.clear();
}

void VRT_RELAY_MODEL::set_vrt_trip_time_delay_in_s(double t)
{
    vrt_trip_timer.set_timer_interval_in_s(t);
}

void VRT_RELAY_MODEL::add_vrt_time_volt_threshold_pair(double t, double v)
{
    vrt_line.add_x_and_y_pair(t, v);
}

void VRT_RELAY_MODEL::set_vrt_trip_scale(double scale)
{
    vrt_trip_scale = scale;
}

MULTI_POINT_LINE VRT_RELAY_MODEL::get_vrt_line() const
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
    return vrt_line.get_valid_point_size();
}

double VRT_RELAY_MODEL::get_vrt_time_threshold(unsigned int index) const
{
    return vrt_line.get_point_x_with_index(index);
}

double VRT_RELAY_MODEL::get_vrt_volt_threshold(unsigned int index) const
{
    return vrt_line.get_point_y_with_index(index);
}

double VRT_RELAY_MODEL::get_vrt_trip_scale() const
{
    return vrt_trip_scale;
}

double VRT_RELAY_MODEL::get_time_when_vrt_enter_during_status() const
{
    return t0_vrt_activated;
}

void VRT_RELAY_MODEL::set_time_when_vrt_enter_during_status(double t)
{
    t0_vrt_activated = t;
}

void VRT_RELAY_MODEL::set_point_time_with_index(unsigned int index, double value)
{
    vrt_line.set_point_x_with_index(index, value);
}

void VRT_RELAY_MODEL::set_point_voltage_with_index(unsigned int index, double value)
{
    vrt_line.set_point_y_with_index(index, value);
}
