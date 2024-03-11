#include "header/model/energy_storage_model/es_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
ES_MODEL::ES_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_ENERGY_STORAGE);
}

ES_MODEL::~ES_MODEL()
{
    ;
}

ENERGY_STORAGE* ES_MODEL::get_energy_storage_pointer() const
{
    return (ENERGY_STORAGE*) get_device_pointer();
}

double ES_MODEL::get_mbase_in_MVA() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    return energy_storage->get_mbase_in_MVA();
}

double ES_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    return energy_storage->get_one_over_mbase_in_one_over_MVA();
}

double ES_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> ES_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double ES_MODEL::get_terminal_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double ES_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

complex<double> ES_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    return energy_storage->get_source_impedance_in_pu();
}
