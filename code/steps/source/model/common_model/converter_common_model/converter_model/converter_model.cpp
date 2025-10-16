#include "header/model/common_model/converter_common_model/converter_model/converter_model.h"

CONVERTER_MODEL::CONVERTER_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{

}

CONVERTER_MODEL::~CONVERTER_MODEL()
{
    ;
}

void CONVERTER_MODEL::set_initial_P_generation_in_pu(double P)
{
    initial_P_generation_in_pu = P;
}

void CONVERTER_MODEL::set_initial_Q_generation_in_pu(double Q)
{
    initial_Q_generation_in_pu = Q;
}

double CONVERTER_MODEL::get_initial_P_generation_in_pu() const
{
    return initial_P_generation_in_pu;
}

double CONVERTER_MODEL::get_initial_Q_generation_in_pu() const
{
    return initial_Q_generation_in_pu;
}

void CONVERTER_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd)
{
    this->initial_active_current_command_in_pu_based_on_mbase = Ipcmd;
}

void CONVERTER_MODEL::set_initial_active_power_command_in_pu_based_on_mbase(double pcmd)
{
    this->initial_active_power_command_in_pu_based_on_mbase = pcmd;
}

void CONVERTER_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd)
{
    this->initial_reactive_current_command_in_pu_based_on_mbase = Iqcmd;
}

void CONVERTER_MODEL::set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd)
{
    this->initial_reactive_power_command_in_pu_based_on_mbase = qcmd;
}

void CONVERTER_MODEL::set_initial_reactive_voltage_command_in_pu(double eqcmd)
{
    this->initial_reactive_voltage_command_in_pu = eqcmd;
}

double CONVERTER_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return initial_active_current_command_in_pu_based_on_mbase;
}

double CONVERTER_MODEL::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return initial_active_power_command_in_pu_based_on_mbase;
}

double CONVERTER_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return initial_reactive_current_command_in_pu_based_on_mbase;
}

double CONVERTER_MODEL::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return initial_reactive_power_command_in_pu_based_on_mbase;
}

double CONVERTER_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return initial_reactive_voltage_command_in_pu;
}

void CONVERTER_MODEL::set_source_impedance_in_pu_based_on_mbase(complex<double> Z)
{
    this->Zsource = Z;
}

complex<double> CONVERTER_MODEL::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> S = Vxy*conj(Ixy);
    return S;
}

double CONVERTER_MODEL::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double CONVERTER_MODEL::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double CONVERTER_MODEL::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}
