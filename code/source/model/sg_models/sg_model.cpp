#include "header/model/sg_models/sg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
SG_MODEL::SG_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
}

SG_MODEL::~SG_MODEL()
{
    ;
}

GENERATOR* SG_MODEL::get_generator_pointer() const
{
    return (GENERATOR*) get_device_pointer();
}

double SG_MODEL::get_mbase_in_MVA() const
{
    GENERATOR* gen = get_generator_pointer();
    if(gen!=NULL)
        return gen->get_mbase_in_MVA();
    else
        return 0.0;
}

double SG_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    GENERATOR* gen = get_generator_pointer();
    if(gen!=NULL)
        return gen->get_one_over_mbase_in_one_over_MVA();
    else
        return 0.0;
}

double SG_MODEL::get_bus_base_frequency_in_Hz() const
{
    GENERATOR* generator = get_generator_pointer();
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

complex<double> SG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
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

double SG_MODEL::get_terminal_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
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
