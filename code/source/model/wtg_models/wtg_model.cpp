#include "header/model/wtg_models/wtg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
WTG_MODEL::WTG_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
}

WTG_MODEL::~WTG_MODEL()
{
    ;
}

WT_GENERATOR* WTG_MODEL::get_wt_generator_pointer() const
{
    return (WT_GENERATOR*) get_device_pointer();
}

double WTG_MODEL::get_mbase_in_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_mbase_in_MVA();
    else
        return 0.0;
}

double WTG_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_one_over_mbase_in_one_over_MVA();
    else
        return 0.0;
}

unsigned int WTG_MODEL::get_number_of_lumped_wt_generators() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_number_of_lumped_wt_generators();
    else
        return 0;
}

double WTG_MODEL::get_rated_power_per_wt_generator_in_MW() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_rated_power_per_wt_generator_in_MW();
    else
        return 0.0;
}

double WTG_MODEL::get_terminal_voltage_in_pu() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator!=NULL)
    {
        BUS* bus = generator->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_positive_sequence_voltage_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}

complex<double> WTG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator!=NULL)
    {
        BUS* bus = generator->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_positive_sequence_complex_voltage_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WTG_MODEL::get_bus_base_frequency_in_Hz() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator!=NULL)
    {
        BUS* bus = generator->get_bus_pointer();
        if(bus!=NULL)
            return bus->get_base_frequency_in_Hz();
        else
            return 0.0;
    }
    else
        return 0.0;
}

complex<double> WTG_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator!=NULL)
        return wt_generator->get_source_impedance_in_pu();
    else
        return 0.0;
}
