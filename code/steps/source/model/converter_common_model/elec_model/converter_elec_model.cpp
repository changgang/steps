#include "header/model/converter_common_model/elec_model/converter_elec_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

CONVERTER_ELEC_MODEL::CONVERTER_ELEC_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
    bus_ptr = NULL;
}

CONVERTER_ELEC_MODEL::~CONVERTER_ELEC_MODEL()
{
    ;
}

void CONVERTER_ELEC_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& CONVERTER_ELEC_MODEL::get_toolkit() const
{
    return *toolkit;
}

void CONVERTER_ELEC_MODEL::set_bus_pointer(BUS* bus)
{
    bus_ptr = bus;
}

BUS* CONVERTER_ELEC_MODEL::get_bus_pointer() const
{
    return bus_ptr;
}

unsigned int CONVERTER_ELEC_MODEL::get_bus() const
{
    return bus_ptr->get_bus_number();
}

void CONVERTER_ELEC_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

NONBUS_DEVICE* CONVERTER_ELEC_MODEL::get_device_pointer() const
{
    return device;
}

DEVICE_ID CONVERTER_ELEC_MODEL::get_device_id() const
{
    return device->get_device_id();
}

string CONVERTER_ELEC_MODEL::get_compound_device_name() const
{
    return get_device_id().get_compound_device_name();
}

void CONVERTER_ELEC_MODEL::set_model_name(string name)
{
    this->model_name = name;
}

string CONVERTER_ELEC_MODEL::get_model_name() const
{
    return this->model_name;
}

void CONVERTER_ELEC_MODEL::set_active_power_reference_in_pu_based_on_mbase(double Pref)
{
    this->active_power_reference_in_pu_based_on_mbase = Pref;
}


void CONVERTER_ELEC_MODEL::set_terminal_current_in_pu_based_on_mbase(double Iterm)
{
    this->terminal_current_in_pu_based_on_mbase = Iterm;
}

void CONVERTER_ELEC_MODEL::set_active_power_generation_in_pu_based_on_mbase(double Pgen)
{
    this->active_power_generation_in_pu_based_on_mbase = Pgen;
}

void CONVERTER_ELEC_MODEL::set_reactive_power_generation_in_pu_based_on_mbase(double Qgen)
{
    this->reactive_power_generation_in_pu_based_on_mbase = Qgen;
}

void CONVERTER_ELEC_MODEL::set_voltage_reference_in_pu(double Vref)
{
    this->voltage_reference_in_pu = Vref;
}

void CONVERTER_ELEC_MODEL::set_voltage_reference_in_pu_with_bus_to_regulate()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int bus = get_bus_to_regulate();
    if(bus==0)
        set_voltage_reference_in_pu(get_terminal_voltage_in_pu());
    else
        set_voltage_reference_in_pu(psdb.get_bus_positive_sequence_voltage_in_pu(bus));
}

void CONVERTER_ELEC_MODEL::set_power_factor_reference_in_pu(double pf)
{
    this->power_factor_reference_in_pu = pf;
}

void CONVERTER_ELEC_MODEL::set_reactive_power_reference_in_pu_based_on_mbase(double Qref)
{
    this->reactive_power_reference_in_pu_based_on_mbase = Qref;
}

double CONVERTER_ELEC_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

double CONVERTER_ELEC_MODEL::get_active_power_reference_in_pu_based_on_mbase() const
{
    return active_power_reference_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    complex<double> V= bus->get_positive_sequence_complex_voltage_in_pu();
    return steps_fast_complex_abs(V);
}

double CONVERTER_ELEC_MODEL::get_terminal_current_in_pu_based_on_mbase() const
{
    return terminal_current_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_active_power_generation_in_pu_based_on_mbase() const
{
    return active_power_generation_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_reactive_power_generation_in_pu_based_on_mbase() const
{
    return reactive_power_generation_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

double CONVERTER_ELEC_MODEL::get_power_factor_reference_in_pu() const
{
    return power_factor_reference_in_pu;
}

double CONVERTER_ELEC_MODEL::get_reactive_power_reference_in_pu_based_on_mbase() const
{
    return reactive_power_reference_in_pu_based_on_mbase;
}

void CONVERTER_ELEC_MODEL::set_bus_to_regulate(unsigned int bus)
{
    bus_to_regulate = bus;
}

unsigned int CONVERTER_ELEC_MODEL::get_bus_to_regulate()  const
{
    return bus_to_regulate;
}

void CONVERTER_ELEC_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd)
{
    this->initial_active_current_command_in_pu_based_on_mbase = Ipcmd;
}

void CONVERTER_ELEC_MODEL::set_initial_active_power_command_in_pu_based_on_mbase(double pcmd)
{
    this->initial_active_power_command_in_pu_based_on_mbase = pcmd;
}

void CONVERTER_ELEC_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd)
{
    this->initial_reactive_current_command_in_pu_based_on_mbase = Iqcmd;
}

void CONVERTER_ELEC_MODEL::set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd)
{
    this->initial_reactive_power_command_in_pu_based_on_mbase = qcmd;
}

void CONVERTER_ELEC_MODEL::set_initial_reactive_voltage_command_in_pu(double eqcmd)
{
    this->initial_reactive_voltage_command_in_pu = eqcmd;
}

void CONVERTER_ELEC_MODEL::set_Xsource(double X)
{
    this->Xsource = X;
}

double CONVERTER_ELEC_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return initial_active_current_command_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return initial_active_power_command_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return initial_reactive_current_command_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return initial_reactive_power_command_in_pu_based_on_mbase;
}

double CONVERTER_ELEC_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return initial_reactive_voltage_command_in_pu;
}

double CONVERTER_ELEC_MODEL::get_Xsource() const
{
    return Xsource;
}
