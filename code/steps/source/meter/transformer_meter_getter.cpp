#include "header/meter/transformer_meter_getter.h"
#include "header/device/transformer.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

double transformer_meter_getter::get_current_kA_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
}

double transformer_meter_getter::get_current_kA_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
}

double transformer_meter_getter::get_current_kA_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(TERTIARY_SIDE));
}

double transformer_meter_getter::get_current_pu_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(PRIMARY_SIDE));
}

double transformer_meter_getter::get_current_pu_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(SECONDARY_SIDE));
}

double transformer_meter_getter::get_current_pu_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(TERTIARY_SIDE));
}

double transformer_meter_getter::get_active_power_MW_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
}

double transformer_meter_getter::get_active_power_MW_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
}

double transformer_meter_getter::get_active_power_MW_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).real();
}

double transformer_meter_getter::get_active_power_pu_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).real();
}

double transformer_meter_getter::get_active_power_pu_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).real();
}

double transformer_meter_getter::get_active_power_pu_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).real();
}

double transformer_meter_getter::get_reactive_power_MVar_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
}

double transformer_meter_getter::get_reactive_power_MVar_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
}

double transformer_meter_getter::get_reactive_power_MVar_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).imag();
}

double transformer_meter_getter::get_reactive_power_pu_primary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).imag();
}

double transformer_meter_getter::get_reactive_power_pu_secondary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).imag();
}

double transformer_meter_getter::get_reactive_power_pu_tertiary(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).imag();
}

double transformer_meter_getter::get_current_kA_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
        else
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
            else
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(TERTIARY_SIDE));
        }
    }
}

double transformer_meter_getter::get_current_pu_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(PRIMARY_SIDE));
        else
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(SECONDARY_SIDE));
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(PRIMARY_SIDE));
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(SECONDARY_SIDE));
            else
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(TERTIARY_SIDE));
        }
    }
}

double transformer_meter_getter::get_active_power_MW_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
        else
            return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
            else
                return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).real();
        }
    }
}

double transformer_meter_getter::get_active_power_pu_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).real();
        else
            return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).real();
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).real();
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).real();
            else
                return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).real();
        }
    }
}

double transformer_meter_getter::get_reactive_power_MVar_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
        else
            return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
            else
                return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).imag();
        }
    }
}

double transformer_meter_getter::get_reactive_power_pu_bus_side(const METER_PARAMETER& para)
{
    TRANSFORMER* trans = (TRANSFORMER*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(trans->is_two_winding_transformer())
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE))
            return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).imag();
        else
            return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).imag();
    }
    else
    {
        if(metered_bus != trans->get_winding_bus(SECONDARY_SIDE) &&
           metered_bus != trans->get_winding_bus(TERTIARY_SIDE))
            return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).imag();
        else
        {
            if(metered_bus == trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).imag();
            else
                return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).imag();
        }
    }
}
