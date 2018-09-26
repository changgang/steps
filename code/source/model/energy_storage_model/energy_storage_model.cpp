#include "header/model/energy_storage_model/energy_storage_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
ENERGY_STORAGE_MODEL::ENERGY_STORAGE_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("ENERGY STORAGE");
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
    if(estorage!=NULL)
        return estorage->get_mbase_in_MVA();
    else
        return 0.0;
}

string ENERGY_STORAGE_MODEL::get_model_type() const
{
    return "ENERGY STORAGE";
}

complex<double> ENERGY_STORAGE_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    if(estorage==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = estorage->get_energy_storage_bus();
    return psdb.get_bus_complex_voltage_in_pu(bus);
}

double ENERGY_STORAGE_MODEL::get_terminal_bus_voltage_in_pu() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    if(estorage==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = estorage->get_energy_storage_bus();
    return psdb.get_bus_voltage_in_pu(bus);
}

double ENERGY_STORAGE_MODEL::get_terminal_bus_angle_in_rad() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    if(estorage==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = estorage->get_energy_storage_bus();
    return psdb.get_bus_angle_in_rad(bus);
}


double ENERGY_STORAGE_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    ENERGY_STORAGE* estorage = get_energy_storage_pointer();
    if(estorage==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t bus = estorage->get_energy_storage_bus();
    return psdb.get_bus_frequency_deviation_in_pu(bus);
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
