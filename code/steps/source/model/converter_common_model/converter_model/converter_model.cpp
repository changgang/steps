#include "header/model/converter_common_model/converter_model/converter_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
CONVERTER_MODEL::CONVERTER_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

CONVERTER_MODEL::~CONVERTER_MODEL()
{
    ;
}

void CONVERTER_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& CONVERTER_MODEL::get_toolkit() const
{
    return *toolkit;
}

void CONVERTER_MODEL::set_bus_pointer(BUS* busptr)
{
    this->busptr = busptr;
}

void CONVERTER_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

BUS* CONVERTER_MODEL::get_bus_pointer() const
{
    return busptr;
}

NONBUS_DEVICE* CONVERTER_MODEL::get_device_pointer() const
{
    return device;
}

DEVICE_ID CONVERTER_MODEL::get_device_id() const
{
    return device->get_device_id();
}

string CONVERTER_MODEL::get_compound_device_name() const
{
    return get_device_id().get_compound_device_name();
}

void CONVERTER_MODEL::set_model_name(string name)
{
    this->model_name = name;
}

string CONVERTER_MODEL::get_model_name() const
{
    return this->model_name;
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

double CONVERTER_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> CONVERTER_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double CONVERTER_MODEL::get_terminal_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double CONVERTER_MODEL::get_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

double CONVERTER_MODEL::get_base_angle_speed_in_radps() const
{
    return DOUBLE_PI*get_base_frequency_in_Hz();
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

complex<double> CONVERTER_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    return this->Zsource;
}

void CONVERTER_MODEL::set_source_impedance_in_pu_based_on_mbase(complex<double> Z)
{
    this->Zsource = Z;
}

complex<double> CONVERTER_MODEL::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
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
