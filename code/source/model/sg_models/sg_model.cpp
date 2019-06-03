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

double SG_MODEL::get_bus_base_frequency_in_Hz() const
{
    GENERATOR* gen = get_generator_pointer();
    if(gen!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        return psdb.get_bus_base_frequency_in_Hz(gen->get_generator_bus());
    }
    else
        return 0.0;
}

complex<double> SG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    GENERATOR* gen = get_generator_pointer();
    if(gen!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        size_t bus = gen->get_generator_bus();
        complex<double> Vxy = psdb.get_bus_complex_voltage_in_pu(bus);
        return Vxy;
    }
    else
        return 0.0;
}
