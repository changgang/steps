#include "header/meter/equivalent_device_meter_getter.h"
#include "header/device/equivalent_device.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double equivalent_device_meter_getter::get_voltage_source_voltage_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_voltage_source_status()==true)
            return steps_fast_complex_abs(edevice->get_equivalent_voltage_source_voltage_in_pu());
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_voltage_source_voltage_angle_in_deg(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_voltage_source_status()==true)
            return rad2deg(steps_fast_complex_arg(edevice->get_equivalent_voltage_source_voltage_in_pu()));
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_voltage_source_resistance_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_voltage_source_status()==true)
            return edevice->get_equivalent_voltage_source_impedance_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_voltage_source_reactance_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_voltage_source_status()==true)
            return edevice->get_equivalent_voltage_source_impedance_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_power_load_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_power_load_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_power_load_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_power_load_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_current_load_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_current_load_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_current_load_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_current_load_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_impedance_load_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_impedance_load_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_generation_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_generation_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_generation_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_generation_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_load_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_load_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_load_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_load_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_net_load_in_mw(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_total_equivalent_power_as_load_in_MVA().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_net_load_in_mvar(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_total_equivalent_power_as_load_in_MVA().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_power_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_power_load_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_power_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_power_load_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_current_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_current_load_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_current_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_current_load_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_constant_impedance_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_constant_impedance_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_generation_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_generation_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_generation_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_generation_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_load_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_equivalent_load_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_active_power_net_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_total_equivalent_power_as_load_in_pu().real();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double equivalent_device_meter_getter::get_reactive_power_net_load_in_pu(const METER_PARAMETER& para)
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) para.device_pointer;
    if(edevice != NULL)
    {
        if(edevice->get_equivalent_load_status()==true)
            return edevice->get_total_equivalent_power_as_load_in_pu().imag();
        else
            return 0.0;
    }
    else
        return 0.0;
}
