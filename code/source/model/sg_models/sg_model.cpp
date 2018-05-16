#include "header/model/sg_models/sg_model.h"

#include "header/power_system_database.h"
SG_MODEL::SG_MODEL()
{
    ;
}

SG_MODEL::~SG_MODEL()
{
    ;
}

double SG_MODEL::get_mbase_in_MVA() const
{
    GENERATOR* gen = (GENERATOR*) get_device_pointer();
    if(gen!=NULL)
        return gen->get_mbase_in_MVA();
    else
        return 0.0;
}

complex<double> SG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    GENERATOR* gen = (GENERATOR*) get_device_pointer();
    if(gen==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return 0.0;

    size_t bus = gen->get_generator_bus();
    complex<double> Vxy = psdb->get_bus_complex_voltage_in_pu(bus);
    return Vxy;
}
