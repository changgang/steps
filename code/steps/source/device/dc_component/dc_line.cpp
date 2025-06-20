#include "header/device/dc_component/dc_line.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;
DC_LINE::DC_LINE(STEPS& toolkit) : virtual_bus(toolkit)
{
    set_toolkit(toolkit);
    clear();
}

DC_LINE::~DC_LINE()
{
    ;
}

void DC_LINE::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}
STEPS& DC_LINE::get_toolkit()
{
    return *toolkit;
}


bool DC_LINE::is_connected_to_bus(unsigned int bus) const
{
    return false;
}



bool DC_LINE::is_valid() const
{
    if(get_sending_side_bus_number()==0)
        return false;
    if(get_receiving_side_bus_number()==0)
        return false;
    return true;
}

void DC_LINE::check()
{
    ;
}

void DC_LINE::clear()
{
    sending_side_bus = 0;
    receiving_side_bus = 0;
    status = false;
    line_R_in_ohm = 0.0;
    line_L_in_H = 0.0;
    line_C_in_uF = 0.0;
    Udc_ibus = 0.0;
    Udc_jbus = 0.0;
    idc = 0.0;
    P_send = 0.0;
    P_rec = 0.0;

    i_RL_sending_side = 0.0;
    i_RL_receiving_side = 0.0;
    ic_sending_side = 0.0;
    ic_reciving_side = 0.0;
    history_Ui = 0.0;
    history_Uj = 0.0;
    history_Uv = 0.0;
    virtual_bus.clear();
    virtual_bus.set_dc_bus_type(DC_INVALID_BUS);

    fault_location = 0.0;
    fault_R = INFINITE_THRESHOLD;
    fault_L = INFINITE_THRESHOLD;
    fault_C = INFINITE_THRESHOLD;
}

void DC_LINE::report() const
{
    ;
}

void DC_LINE::save() const
{
    ;
}

DEVICE_ID DC_LINE::get_device_id() const
{
    ;
}

void DC_LINE::set_model(MODEL* model)
{
    ;
}

MODEL* DC_LINE::get_model_of_type(string model_type, unsigned int index)
{
    ;
}

void DC_LINE::set_sending_side_bus(const unsigned int bus)
{
    this->sending_side_bus = bus;
}

void DC_LINE::set_receiving_side_bus(const unsigned int bus)
{
    this->receiving_side_bus = bus;
}

void DC_LINE::set_dc_line_name(const string name)
{
    this->name = name;
}

void DC_LINE::set_status(const bool status)
{
    this->status = status;
}

void DC_LINE::set_dc_network_simulation_time_step(const double t_dc)
{
    this->dc_simulation_time_step = t_dc;
}

void DC_LINE::set_line_R_in_ohm(const double r)
{
    this->line_R_in_ohm = r;
}

void DC_LINE::set_line_L_in_H(const double l)
{
    this->line_L_in_H = l;
}

void DC_LINE::set_line_C_in_uF(const double c)
{
    this->line_C_in_uF = c;
}

void DC_LINE::set_sending_side_power(const double psend)
{
    this->P_send = psend;
}

void DC_LINE::set_Udc_ibus_in_kV(const double udc)
{
    this->Udc_ibus = udc;
}
void DC_LINE::set_Udc_jbus_in_kV(const double udc)
{
    this->Udc_jbus = udc;
}

void DC_LINE::set_receiving_side_power(const double prec)
{
    this->P_rec = prec;
}

void DC_LINE::set_dc_line_current(const double idc)
{
    this->idc = idc;
}

void DC_LINE::set_i_RL_in_kA_on_sending_side(const double i1)
{
    this->i_RL_sending_side = i1;
}

void DC_LINE::set_i_RL_in_kA_on_receiving_side(const double i2)
{
    this->i_RL_receiving_side = i2;
}

void DC_LINE::set_ic_in_kA_on_sending_side(const double ic1)
{
    this->ic_sending_side = ic1;
}

void DC_LINE::set_ic_in_kA_on_receiving_side(const double ic2)
{
    this->ic_reciving_side = ic2;
}

void DC_LINE::set_dc_line_history_Ui_in_kV(const double Ui)
{
    this->history_Ui = Ui;
}

void DC_LINE::set_dc_line_history_Uj_in_kV(const double Uj)
{
    this->history_Uj = Uj;
}

void DC_LINE::set_dc_line_history_Uv_in_kV(const double Uv)
{
    this->history_Uv = Uv;
}

unsigned int DC_LINE::get_sending_side_bus_number() const
{
    return sending_side_bus;
}

unsigned int DC_LINE::get_receiving_side_bus_number() const
{
    return receiving_side_bus;
}

string DC_LINE::get_dc_line_name() const
{
    return name;
}

bool DC_LINE::get_status() const
{
    return status;
}

double DC_LINE::get_dc_network_simulation_time_step() const
{
    return dc_simulation_time_step;
}

double DC_LINE::get_line_R_in_ohm() const
{
    return line_R_in_ohm;
}

double DC_LINE::get_line_L_in_H() const
{
    return line_L_in_H;
}

double DC_LINE::get_line_C_in_uF() const
{
    return line_C_in_uF;
}

double DC_LINE::get_Udc_ibus_in_kV() const
{
    return Udc_ibus;
}
double DC_LINE::get_Udc_jbus_in_kV() const
{
    return Udc_jbus;
}

double DC_LINE::get_sending_side_power() const
{
    return P_send;
}

double DC_LINE::get_receiving_side_power() const
{
    return P_rec;
}

double DC_LINE::get_dc_line_current() const
{
    return idc;
}

double DC_LINE::get_i_RL_in_kA_on_sending_side()
{
    return i_RL_sending_side;
}

double DC_LINE::get_i_RL_in_kA_on_receiving_side()
{
    return i_RL_receiving_side;
}

double DC_LINE::get_ic_in_kA_on_sending_side()
{
    return ic_sending_side;
}
double DC_LINE::get_ic_in_kA_on_receiving_side()
{
    return ic_reciving_side;
}

double DC_LINE::get_dc_line_history_Ui_in_kV()
{
    return history_Ui;
}

double DC_LINE::get_dc_line_history_Uj_in_kV()
{
    return history_Uj;
}

double DC_LINE::get_dc_line_history_Uv_in_kV()
{
    return history_Uv;
}

double DC_LINE::get_virtual_bus_voltage()
{
    return virtual_bus.get_dc_bus_voltage_in_kV();
}

double DC_LINE::get_virtual_bus_current()
{
    if(get_dc_line_fault_R_in_ohm()==INFINITE_THRESHOLD)
        return 0.0;
    else
        return virtual_bus.get_dc_bus_current_in_kA();
}

void DC_LINE::set_dc_line_fault_location(double location)
{
    this->fault_location = location;
}

void DC_LINE::set_dc_line_fault_R_in_ohm(double R)
{
    this->fault_R = R;
}

void DC_LINE::set_dc_line_fault_L_in_H(double L)
{
    this->fault_L = L;
}

void DC_LINE::set_dc_line_fault_C_in_uF(double C)
{
    this->fault_C = C;
}

double DC_LINE::get_line_dynamic_y_of_RL_in_S_in_implicit_trapezoid_method(double m)
{
    double h = get_dc_network_simulation_time_step();
    double L = get_line_L_in_H()*m;
    double R = get_line_R_in_ohm()*m;
    double Y_RL = h/(2*L+h*R);
    return Y_RL;
}

double DC_LINE::get_line_dynamic_injection_current_of_RL_in_kA_in_implicit_trapezoid_method(double m, double Ui, double Uj, double I_L)
{
    double h = get_dc_network_simulation_time_step();
    double L = get_line_L_in_H()*m;
    double R = get_line_R_in_ohm()*m;
    double I_RL = h/(2*L+h*R)*(Ui-Uj)+(2*L-h*R)/(2*L+h*R)*I_L;
    return I_RL;
}

double DC_LINE::get_line_dynamic_y_of_C_in_S_in_implicit_trapezoid_method()
{
    double h = get_dc_network_simulation_time_step();
    double C = get_line_C_in_uF()*1e-6;
    double Y_C = 2*C/h;
    return Y_C;
}

double DC_LINE::get_line_dynamic_injection_current_of_C_in_kA_in_implicit_trapezoid_method(double Ui, double Ic)
{
    double h = get_dc_network_simulation_time_step();
    double C = get_line_C_in_uF()*1e-6;
    double I_C = -2*C/h*Ui-Ic;
    return I_C;
}

double DC_LINE::get_line_dynamic_y_of_RL_in_S_in_implicit_euler_method(double m)
{
    return 0.0;
}

double DC_LINE::get_line_dynamic_injection_current_of_RL_in_kA_in_implicit_euler_method(double m, double Ui, double Uj, double I_L)
{
    return 0.0;
}

double DC_LINE::get_line_dynamic_y_of_C_in_S_in_implicit_euler_method()
{
    return 0.0;
}

double DC_LINE::get_line_dynamic_injection_current_of_C_in_kA_in_implicit_euler_method(double Ui, double Ic)
{
    return 0.0;
}

DC_BUS* DC_LINE::get_dc_line_virtual_bus()
{
    return &virtual_bus;
}

double DC_LINE::get_dc_line_fault_location()
{
    return fault_location;
}

double DC_LINE::get_dc_line_fault_R_in_ohm()
{
    return fault_R;
}

double DC_LINE::get_dc_line_fault_L_in_H()
{
    return fault_L;
}

double DC_LINE::get_dc_line_fault_C_in_uF()
{
    return fault_C;
}
