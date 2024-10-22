#include "header/model/common_model/vrt_model/vrt_control_model.h"

VRT_CONTROL_MODEL::VRT_CONTROL_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{
    set_vrt_status(VRT_NORMAL_STATUS);
    set_vrt_voltage_threshold(0.0);
    prepare2recover_timer.set_toolkit(toolkit);
}

VRT_CONTROL_MODEL::~VRT_CONTROL_MODEL()
{
    ;
}

void VRT_CONTROL_MODEL::copy_from_const_model(const VRT_CONTROL_MODEL& model)
{
    set_vrt_status(model.get_vrt_status());
    set_vrt_voltage_threshold(model.get_vrt_voltage_threshold());
}

void VRT_CONTROL_MODEL::set_vrt_voltage_threshold(double v)
{
    vrt_voltage_threshold = v;
}

void VRT_CONTROL_MODEL::set_vrt_prepare2recover_delay_time_in_s(double t)
{
    prepare2recover_timer.set_timer_interval_in_s(t);
}

double VRT_CONTROL_MODEL::get_vrt_voltage_threshold() const
{
    return vrt_voltage_threshold;
}

double VRT_CONTROL_MODEL::get_vrt_prepare2recover_delay_time_in_s() const
{
    return prepare2recover_timer.get_timer_interval_in_s();
}

void VRT_CONTROL_MODEL::reset_vrt_prepare2recover_timer()
{
    prepare2recover_timer.reset();
}

void VRT_CONTROL_MODEL::start_vrt_prepare2recover_timer()
{
    prepare2recover_timer.start();
}

bool VRT_CONTROL_MODEL::is_vrt_prepare2recover_timer_timed_out()
{
    return prepare2recover_timer.is_timed_out();
}

void VRT_CONTROL_MODEL::set_vrt_status(VRT_STATUS status)
{
    this->status = status;
}

VRT_STATUS VRT_CONTROL_MODEL::get_vrt_status() const
{
    return status;
}

void VRT_CONTROL_MODEL::set_current_Ip_command(double Ip)
{
    Ipcmd_now = Ip;
}

void VRT_CONTROL_MODEL::set_current_Iq_command(double Iq)
{
    Iqcmd_now = Iq;
}

void VRT_CONTROL_MODEL::set_current_P_command(double P)
{
    Pcmd_now = P;
}

void VRT_CONTROL_MODEL::set_current_Q_command(double Q)
{
    Qcmd_now = Q;
}

double VRT_CONTROL_MODEL::get_current_Ip_command() const
{
    return Ipcmd_now;
}

double VRT_CONTROL_MODEL::get_current_Iq_command() const
{
    return Iqcmd_now;
}

double VRT_CONTROL_MODEL::get_current_P_command() const
{
    return Pcmd_now;
}

double VRT_CONTROL_MODEL::get_current_Q_command() const

{
    return Qcmd_now;
}

void VRT_CONTROL_MODEL::set_time_when_vrt_enter_during_status(double t)
{
    t0_vrt_activated = t;
}

void VRT_CONTROL_MODEL::set_time_when_vrt_enter_prepare2recover_status(double t)
{
    t0_vrt_prepare2recover_activated = t;
}

void VRT_CONTROL_MODEL::set_time_when_vrt_enter_recover_status(double t)
{
    t0_vrt_recover_activated = t;
}

void VRT_CONTROL_MODEL::set_Ip0_vrt_activated_in_pu(double I)
{
    Ip0_vrt_activated = I;
}

void VRT_CONTROL_MODEL::set_Ip0_vrt_prepare2recover_activated_in_pu(double I)
{
    Ip0_vrt_prepare2recover_activated = I;
}

void VRT_CONTROL_MODEL::set_Ip0_vrt_recover_activated_in_pu(double I)
{
    Ip0_vrt_recover_activated = I;
}

void VRT_CONTROL_MODEL::set_Iq0_vrt_activated_in_pu(double I)
{
    Iq0_vrt_activated = I;
}

void VRT_CONTROL_MODEL::set_Iq0_vrt_prepare2recover_activated_in_pu(double I)
{
    Iq0_vrt_prepare2recover_activated = I;
}

void VRT_CONTROL_MODEL::set_Iq0_vrt_recover_activated_in_pu(double I)
{
    Iq0_vrt_recover_activated = I;
}

void VRT_CONTROL_MODEL::set_P0_vrt_activated_in_pu_based_on_mbase(double P)
{
    P0_vrt_activated = P;
}

void VRT_CONTROL_MODEL::set_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase(double P)
{
    P0_vrt_prepare2recover_activated = P;
}

void VRT_CONTROL_MODEL::set_P0_vrt_recover_activated_in_pu_based_on_mbase(double P)
{
    P0_vrt_recover_activated = P;
}

void VRT_CONTROL_MODEL::set_Q0_vrt_activated_in_pu_based_on_mbase(double Q)
{
    Q0_vrt_activated = Q;
}

void VRT_CONTROL_MODEL::set_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase(double Q)
{
    Q0_vrt_prepare2recover_activated = Q;
}

void VRT_CONTROL_MODEL::set_Q0_vrt_recover_activated_in_pu_based_on_mbase(double Q)
{
    Q0_vrt_recover_activated = Q;
}

double VRT_CONTROL_MODEL::get_time_when_vrt_enter_during_status() const
{
    return t0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_time_when_vrt_enter_prepare2recover_status() const
{
    return t0_vrt_prepare2recover_activated;
}

double VRT_CONTROL_MODEL::get_time_when_vrt_enter_recover_status() const
{
    return t0_vrt_recover_activated;
}

double VRT_CONTROL_MODEL::get_Ip0_vrt_activated_in_pu() const
{
    return Ip0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_Ip0_vrt_prepare2recover_activated_in_pu() const
{
    return Ip0_vrt_prepare2recover_activated;
}

double VRT_CONTROL_MODEL::get_Ip0_vrt_recover_activated_in_pu() const
{
    return Ip0_vrt_recover_activated;
}

double VRT_CONTROL_MODEL::get_Iq0_vrt_activated_in_pu() const
{
    return Iq0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_Iq0_vrt_prepare2recover_activated_in_pu() const
{
    return Iq0_vrt_prepare2recover_activated;
}

double VRT_CONTROL_MODEL::get_Iq0_vrt_recover_activated_in_pu() const
{
    return Iq0_vrt_recover_activated;
}

double VRT_CONTROL_MODEL::get_P0_vrt_activated_in_pu_based_on_mbase() const
{
    return P0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase() const
{
    return P0_vrt_prepare2recover_activated;
}

double VRT_CONTROL_MODEL::get_P0_vrt_recover_activated_in_pu_based_on_mbase() const
{
    return P0_vrt_recover_activated;
}

double VRT_CONTROL_MODEL::get_Q0_vrt_activated_in_pu_based_on_mbase() const
{
    return Q0_vrt_activated;
}

double VRT_CONTROL_MODEL::get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase() const
{
    return Q0_vrt_prepare2recover_activated;
}

double VRT_CONTROL_MODEL::get_Q0_vrt_recover_activated_in_pu_based_on_mbase() const
{
    return Q0_vrt_recover_activated;
}


void VRT_CONTROL_MODEL::update_PQI0_in_vrt_during_status(double P0, double Q0, double Ip0, double Iq0)
{
    set_P0_vrt_activated_in_pu_based_on_mbase(P0);
    set_Q0_vrt_activated_in_pu_based_on_mbase(Q0);
    set_Ip0_vrt_activated_in_pu(Ip0);
    set_Iq0_vrt_activated_in_pu(Iq0);
}

void VRT_CONTROL_MODEL::update_PQI0_in_vrt_prepare2recover_status(double P0, double Q0, double Ip0, double Iq0)
{
    set_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase(P0);
    set_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase(Q0);
    set_Ip0_vrt_prepare2recover_activated_in_pu(Ip0);
    set_Iq0_vrt_prepare2recover_activated_in_pu(Iq0);
}

void VRT_CONTROL_MODEL::update_PQI0_in_vrt_recover_status(double P0, double Q0, double Ip0, double Iq0)
{
    set_P0_vrt_recover_activated_in_pu_based_on_mbase(P0);
    set_Q0_vrt_recover_activated_in_pu_based_on_mbase(Q0);
    set_Ip0_vrt_recover_activated_in_pu(Ip0);
    set_Iq0_vrt_recover_activated_in_pu(Iq0);
}

double VRT_CONTROL_MODEL::get_vrt_Ip_command()
{
    return get_vrt_P_command()/get_bus_voltage_in_pu();
}

double VRT_CONTROL_MODEL::get_vrt_Iq_command()
{
    //Iqmcd and Qcmd are opposite numbers to each other
    return -get_vrt_Q_command()/get_bus_voltage_in_pu();
}
