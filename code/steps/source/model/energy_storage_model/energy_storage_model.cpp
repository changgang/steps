#include "header/model/energy_storage_model/energy_storage_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
ENERGY_STORAGE_MODEL::ENERGY_STORAGE_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_ENERGY_STORAGE);
}

ENERGY_STORAGE_MODEL::~ENERGY_STORAGE_MODEL()
{
    ;
}

ENERGY_STORAGE* ENERGY_STORAGE_MODEL::get_energy_storage_pointer() const
{
    return (ENERGY_STORAGE*) get_device_pointer();
}

double ENERGY_STORAGE_MODEL::get_mbase_in_MVA() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    return estorage->get_mbase_in_MVA();
}

double ENERGY_STORAGE_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    return estorage->get_one_over_mbase_in_one_over_MVA();
}

string ENERGY_STORAGE_MODEL::get_model_type() const
{
    return "ENERGY STORAGE";
}

complex<double> ENERGY_STORAGE_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double ENERGY_STORAGE_MODEL::get_terminal_bus_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

double ENERGY_STORAGE_MODEL::get_terminal_bus_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}


double ENERGY_STORAGE_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

void ENERGY_STORAGE_MODEL::set_Pmax_in_pu(double p)
{
    this->max_power_in_pu = p;
}

void ENERGY_STORAGE_MODEL::set_E0_in_pu(double E)
{
    this->initial_energy_in_pu = E;
}

void ENERGY_STORAGE_MODEL::set_En_in_MWh(double E)
{
    this->nominal_energy_in_MWh = E;
}

void ENERGY_STORAGE_MODEL::set_Pref_in_pu(double p)
{
    this->Pref_in_pu = p;
}

void ENERGY_STORAGE_MODEL::set_Vref_in_pu(double v)
{
    this->Vref_in_pu = v;
}

double ENERGY_STORAGE_MODEL::get_Pmax_in_pu() const
{
    return max_power_in_pu;
}

double ENERGY_STORAGE_MODEL::get_E0_in_pu() const
{
    return initial_energy_in_pu;
}

double ENERGY_STORAGE_MODEL::get_En_in_MWh() const
{
    return nominal_energy_in_MWh;
}

double ENERGY_STORAGE_MODEL::get_Pref_in_pu() const
{
    return this->Pref_in_pu;
}

double ENERGY_STORAGE_MODEL::get_Vref_in_pu() const
{
    return this->Vref_in_pu;
}
