#include "header/meter/ac_line_meter_getter.h"
#include "header/device/ac_line.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter.h"

double ac_line_meter_getter::get_current_in_ka(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_kA());
            else
                return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_kA());
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_current_in_pu(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_pu());
            else
                return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_pu());
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_active_power_in_mw(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return (line->get_line_complex_power_at_sending_side_in_MVA()).real();
            else
                return (line->get_line_complex_power_at_receiving_side_in_MVA()).real();
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_active_power_in_pu(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return (line->get_line_complex_power_at_sending_side_in_pu()).real();
            else
                return (line->get_line_complex_power_at_receiving_side_in_pu()).real();
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_reactive_power_in_mvar(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return (line->get_line_complex_power_at_sending_side_in_MVA()).imag();
            else
                return (line->get_line_complex_power_at_receiving_side_in_MVA()).imag();
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_reactive_power_in_pu(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return (line->get_line_complex_power_at_sending_side_in_pu()).imag();
            else
                return (line->get_line_complex_power_at_receiving_side_in_pu()).imag();
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_in_ohm(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
            else
                return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_in_pu(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_pu());
            else
                return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_pu());
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_angle_in_deg(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm()));
            else
                return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm()));
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_angle_in_rad(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
    {
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
        {
            unsigned int metered_bus = para.metered_side_bus;
            if(metered_bus != line->get_receiving_side_bus())
                return steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
            else
                return steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
        }
    }
    return 0.0;
}

double ac_line_meter_getter::get_current_at_sending_side_in_ka(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_kA());
    return 0.0;
}

double ac_line_meter_getter::get_active_power_at_sending_side_in_mw(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return (line->get_line_complex_power_at_sending_side_in_MVA()).real();
    return 0.0;
}

double ac_line_meter_getter::get_reactive_power_at_sending_side_in_mvar(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return (line->get_line_complex_power_at_sending_side_in_MVA()).imag();
    return 0.0;
}

double ac_line_meter_getter::get_current_at_receiving_side_in_ka(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_kA());
    return 0.0;
}

double ac_line_meter_getter::get_active_power_at_receiving_side_in_mw(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return (line->get_line_complex_power_at_receiving_side_in_MVA()).real();
    return 0.0;
}

double ac_line_meter_getter::get_reactive_power_at_receiving_side_in_mvar(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return (line->get_line_complex_power_at_receiving_side_in_MVA()).imag();
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_at_sending_side_in_ohm(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_angle_at_sending_side_in_deg(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm()));
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_at_receiving_side_in_ohm(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
    return 0.0;
}

double ac_line_meter_getter::get_apparent_impedance_angle_at_receiving_side_in_deg(const METER_PARAMETER& para)
{
    AC_LINE* line = (AC_LINE*) para.device_pointer;
    if(line != NULL)
        if(line->get_sending_side_breaker_status() == true || line->get_receiving_side_breaker_status() == true)
            return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm()));
    return 0.0;
}
