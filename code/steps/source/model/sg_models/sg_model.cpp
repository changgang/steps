#include "header/model/sg_models/sg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
SG_MODEL::SG_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_GENERATOR);
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
    return gen->get_mbase_in_MVA();
}

double SG_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    GENERATOR* gen = get_generator_pointer();
    return gen->get_one_over_mbase_in_one_over_MVA();
}

double SG_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

complex<double> SG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double SG_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_system_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_system_A();
        case 'B':
            return get_linearized_system_B();
        case 'C':
            return get_linearized_system_C();
        case 'D':
            return get_linearized_system_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}
