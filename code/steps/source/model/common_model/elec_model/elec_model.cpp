#include "header/model/common_model/elec_model/elec_model.h"

ELEC_MODEL::ELEC_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{

}

ELEC_MODEL::~ELEC_MODEL()
{
    ;
}

void ELEC_MODEL::set_terminal_current_in_pu_based_on_mbase(double Iterm)
{
    this->terminal_current_in_pu_based_on_mbase = Iterm;
}

void ELEC_MODEL::set_active_power_generation_in_pu_based_on_mbase(double Pgen)
{
    this->active_power_generation_in_pu_based_on_mbase = Pgen;
}

void ELEC_MODEL::set_reactive_power_generation_in_pu_based_on_mbase(double Qgen)
{
    this->reactive_power_generation_in_pu_based_on_mbase = Qgen;
}

void ELEC_MODEL::set_speed_in_pu(double s)
{
    this->speed_in_pu = s;
}

void ELEC_MODEL::set_Vdc_in_kV(double v)
{
    this->Vdc_in_kV = v;
}

void ELEC_MODEL::set_voltage_reference_in_pu(double Vref)
{
    this->voltage_reference_in_pu = Vref;
}

void ELEC_MODEL::set_voltage_reference_in_pu_with_bus_to_regulate()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int bus = get_bus_to_regulate();
    if(bus==0)
        set_voltage_reference_in_pu(get_bus_voltage_in_pu());
    else
        set_voltage_reference_in_pu(psdb.get_bus_positive_sequence_voltage_in_pu(bus));
}

void ELEC_MODEL::set_power_factor_reference_in_pu(double pf)
{
    this->power_factor_reference_in_pu = pf;
}

void ELEC_MODEL::set_active_power_reference_in_pu_based_on_mbase(double Pref)
{
    this->active_power_reference_in_pu_based_on_mbase = Pref;
}

void ELEC_MODEL::set_reactive_power_reference_in_pu_based_on_mbase(double Qref)
{
    this->reactive_power_reference_in_pu_based_on_mbase = Qref;
}

void ELEC_MODEL::set_speed_reference_in_pu(double s)
{
    this->speed_reference_in_pu = s;
}

void ELEC_MODEL::set_Vdc_reference_in_kV(double v)
{
    this->Vdc_reference_in_kV = v;
}

void ELEC_MODEL::set_bus_to_regulate(unsigned int bus)
{
    bus_to_regulate = bus;
}

void ELEC_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd)
{
    this->initial_active_current_command_in_pu_based_on_mbase = Ipcmd;
}

void ELEC_MODEL::set_initial_active_power_command_in_pu_based_on_mbase(double pcmd)
{
    this->initial_active_power_command_in_pu_based_on_mbase = pcmd;
}

void ELEC_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd)
{
    this->initial_reactive_current_command_in_pu_based_on_mbase = Iqcmd;
}

void ELEC_MODEL::set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd)
{
    this->initial_reactive_power_command_in_pu_based_on_mbase = qcmd;
}

void ELEC_MODEL::set_initial_reactive_voltage_command_in_pu(double eqcmd)
{
    this->initial_reactive_voltage_command_in_pu = eqcmd;
}

void ELEC_MODEL::set_initial_Pref(double P)
{
    this->initial_Pref = P;
}

void ELEC_MODEL::set_initial_speed(double s)
{
    this->initial_speed = s;
}

void ELEC_MODEL::set_initial_Vdc(double v)
{
    this->initial_Vdc = v;
}

void ELEC_MODEL::set_initial_Torque(double T)
{
    this->initial_Torque = T;
}

void ELEC_MODEL::set_Xsource(double X)
{
    this->Xsource = X;
}

double ELEC_MODEL::get_terminal_current_in_pu_based_on_mbase() const
{
    return terminal_current_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_active_power_generation_in_pu_based_on_mbase() const
{
    return active_power_generation_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_reactive_power_generation_in_pu_based_on_mbase() const
{
    return reactive_power_generation_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_speed_in_pu() const
{
    return speed_in_pu;
}

double ELEC_MODEL::get_Vdc_in_kV() const
{
    return Vdc_in_kV;
}

double ELEC_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

double ELEC_MODEL::get_power_factor_reference_in_pu() const
{
    return power_factor_reference_in_pu;
}

double ELEC_MODEL::get_active_power_reference_in_pu_based_on_mbase() const
{
    return active_power_reference_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_reactive_power_reference_in_pu_based_on_mbase() const
{
    return reactive_power_reference_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_speed_reference_in_pu() const
{
    return speed_reference_in_pu;
}

double ELEC_MODEL::get_Vdc_reference_in_kV() const
{
    return Vdc_reference_in_kV;
}

unsigned int ELEC_MODEL::get_bus_to_regulate()  const
{
    return bus_to_regulate;
}

double ELEC_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return initial_active_current_command_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return initial_active_power_command_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return initial_reactive_current_command_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return initial_reactive_power_command_in_pu_based_on_mbase;
}

double ELEC_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return initial_reactive_voltage_command_in_pu;
}

double ELEC_MODEL::get_initial_Pref() const
{
    return initial_Pref;
}

double ELEC_MODEL::get_initial_speed() const
{
    return initial_speed;
}

double ELEC_MODEL::get_initial_Vdc() const
{
    return initial_Vdc;
}

double ELEC_MODEL::get_initial_Torque() const
{
    return initial_Torque;
}

double ELEC_MODEL::get_Xsource() const
{
    return Xsource;
}
