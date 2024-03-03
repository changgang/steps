#include "header/model/converter_common_models/vrt_model/hvrt_control.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cmath>

using namespace std;

HVRT_CONTROL::HVRT_CONTROL(STEPS& toolkit) : VRT_CONTROL_MODEL(toolkit)
{
    clear();
}

HVRT_CONTROL::~HVRT_CONTROL()
{
    ;
}

HVRT_CONTROL::HVRT_CONTROL(const HVRT_CONTROL& model) : VRT_CONTROL_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

HVRT_CONTROL& HVRT_CONTROL::operator=(const HVRT_CONTROL& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void HVRT_CONTROL::clear()
{
    set_vrt_status(VRT_NORMAL_STATUS);
    set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
    set_time_when_vrt_enter_prepare2recover_status(INFINITE_THRESHOLD);
    set_time_when_vrt_enter_recover_status(INFINITE_THRESHOLD);
    reset_vrt_prepare2recover_timer();


    set_P_vrt_during_mode(VRT_DURING_POWER_CONTROL_MODE);
    set_Q_vrt_during_mode(VRT_DURING_POWER_CONTROL_MODE);
    set_P_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE);
    set_Q_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE);
    set_P_vrt_recover_mode(VRT_RECOVER_LINEAR_CONTROL_MODE);
    set_Q_vrt_recover_mode(VRT_RECOVER_LINEAR_CONTROL_MODE);

    set_kP_vrt_during(0.0);
    set_P_set_vrt_during_in_pu(0.0);
    set_k1_Ip_vrt_during(0.0);
    set_k2_Ip_vrt_during(0.0);
    set_Ip_set_vrt_during_in_pu(0.0);

    set_k_Ip_prepare2recover(0.0);
    set_Ip_set_prepare2recover(0.0);

    set_P_recover_slope_in_pu_per_s(0.0);
    set_P_recover_inertia_in_s(0.0);

    set_kQ_vrt_during(0.0);
    set_Q_set_vrt_during_in_pu(0.0);
    set_k1_Iq_vrt_during(0.0);
    set_k2_Iq_vrt_during(0.0);
    set_Iq_set_vrt_during_in_pu(0.0);

    set_k_Iq_prepare2recover(0.0);
    set_Iq_set_prepare2recover(0.0);

    set_Q_recover_slope_in_pu_per_s(0.0);
    set_Q_recover_inertia_in_s(0.0);
}

void HVRT_CONTROL::copy_from_const_model(const HVRT_CONTROL& model)
{
    clear();
    VRT_CONTROL_MODEL::copy_from_const_model(model);

    set_P_vrt_during_mode(model.get_P_vrt_during_mode());
    set_Q_vrt_during_mode(model.get_Q_vrt_during_mode());
    set_P_vrt_prepare2recover_mode(model.get_P_vrt_prepare2recover_mode());
    set_Q_vrt_prepare2recover_mode(model.get_Q_vrt_prepare2recover_mode());
    set_P_vrt_recover_mode(model.get_P_vrt_recover_mode());
    set_Q_vrt_recover_mode(model.get_Q_vrt_recover_mode());

    set_kP_vrt_during(model.get_kP_vrt_during());
    set_P_set_vrt_during_in_pu(model.get_P_set_vrt_during_in_pu());
    set_k1_Ip_vrt_during(model.get_k1_Ip_vrt_during());
    set_k2_Ip_vrt_during(model.get_k2_Ip_vrt_during());
    set_Ip_set_vrt_during_in_pu(model.get_Ip_set_vrt_during_in_pu());

    set_k_Ip_prepare2recover(model.get_k_Ip_prepare2recover());
    set_Ip_set_prepare2recover(model.get_Ip_set_prepare2recover());

    set_P_recover_slope_in_pu_per_s(model.get_P_recover_slope_in_pu_per_s());
    set_P_recover_inertia_in_s(model.get_P_recover_inertia_in_s());

    set_kQ_vrt_during(model.get_kQ_vrt_during());
    set_Q_set_vrt_during_in_pu(model.get_Q_set_vrt_during_in_pu());
    set_k1_Iq_vrt_during(model.get_k1_Iq_vrt_during());
    set_k2_Iq_vrt_during(model.get_k2_Iq_vrt_during());
    set_Iq_set_vrt_during_in_pu(model.get_Iq_set_vrt_during_in_pu());

    set_k_Iq_prepare2recover(model.get_k_Iq_prepare2recover());
    set_Iq_set_prepare2recover(model.get_Iq_set_prepare2recover());

    set_Q_recover_slope_in_pu_per_s(model.get_Q_recover_slope_in_pu_per_s());
    set_Q_recover_inertia_in_s(model.get_Q_recover_inertia_in_s());
}

void HVRT_CONTROL::set_P_vrt_during_mode(VRT_DURING_CONTROL_MODE m)
{
    P_vrt_during_mode = m;
}
void HVRT_CONTROL::set_kP_vrt_during(double k)
{
    kP_during = k;
}
void HVRT_CONTROL::set_P_set_vrt_during_in_pu(double p)
{
    P_set_during = p;
}

void HVRT_CONTROL::set_k1_Ip_vrt_during(double k)
{
    k1_Ip_during = k;
}

void HVRT_CONTROL::set_k2_Ip_vrt_during(double k)
{
    k2_Ip_during = k;
}

void HVRT_CONTROL::set_Ip_set_vrt_during_in_pu(double I)
{
    Ip_set_during = I;
}

void HVRT_CONTROL::set_P_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    P_vrt_prepare2recover_mode = m;
}
void HVRT_CONTROL::set_k_Ip_prepare2recover(double k)
{
    k_Ip_prepare2recover = k;
}
void HVRT_CONTROL::set_Ip_set_prepare2recover(double I)
{
    Ip_set_prepare2recover = I;
}

void HVRT_CONTROL::set_P_vrt_recover_mode(VRT_RECOVER_CONTROL_MODE m)
{
    P_vrt_recover_mode = m;
}
void HVRT_CONTROL::set_P_recover_slope_in_pu_per_s(double s)
{
    kP_recover = s;
}
void HVRT_CONTROL::set_P_recover_inertia_in_s(double T)
{
    TP_recover = T;
}

void HVRT_CONTROL::set_Q_vrt_during_mode(VRT_DURING_CONTROL_MODE m)
{
    Q_vrt_during_mode = m;
}
void HVRT_CONTROL::set_kQ_vrt_during(double k)
{
    kQ_during = k;
}
void HVRT_CONTROL::set_Q_set_vrt_during_in_pu(double q)
{
    Q_set_during = q;
}
void HVRT_CONTROL::set_k1_Iq_vrt_during(double k)
{
    k1_Iq_during = k;
}
void HVRT_CONTROL::set_k2_Iq_vrt_during(double k)
{
    k2_Iq_during = k;
}
void HVRT_CONTROL::set_Iq_set_vrt_during_in_pu(double I)
{
    Iq_set_during = I;
}

void HVRT_CONTROL::set_Q_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    Q_vrt_prepare2recover_mode = m;
}
void HVRT_CONTROL::set_k_Iq_prepare2recover(double k)
{
    k_Iq_prepare2recover = k;
}
void HVRT_CONTROL::set_Iq_set_prepare2recover(double I)
{
    Iq_set_prepare2recover = I;
}

void HVRT_CONTROL::set_Q_vrt_recover_mode(VRT_RECOVER_CONTROL_MODE m)
{
    Q_vrt_recover_mode = m;
}
void HVRT_CONTROL::set_Q_recover_slope_in_pu_per_s(double s)
{
    kQ_recover = s;
}
void HVRT_CONTROL::set_Q_recover_inertia_in_s(double T)
{
    TQ_recover = T;
}

VRT_DURING_CONTROL_MODE HVRT_CONTROL::get_P_vrt_during_mode() const
{
    return P_vrt_during_mode;
}
double HVRT_CONTROL::get_kP_vrt_during() const
{
    return kP_during;
}
double HVRT_CONTROL::get_P_set_vrt_during_in_pu() const
{
    return P_set_during;
}
double HVRT_CONTROL::get_k1_Ip_vrt_during() const
{
    return k1_Ip_during;
}
double HVRT_CONTROL::get_k2_Ip_vrt_during() const
{
    return k2_Ip_during;
}
double HVRT_CONTROL::get_Ip_set_vrt_during_in_pu() const
{
    return Ip_set_during;
}

VRT_PREPARE2RECOVER_CONTROL_MODE HVRT_CONTROL::get_P_vrt_prepare2recover_mode() const
{
    return P_vrt_prepare2recover_mode;
}

double HVRT_CONTROL::get_k_Ip_prepare2recover() const
{
    return k_Ip_prepare2recover;
}

double HVRT_CONTROL::get_Ip_set_prepare2recover() const
{
    return Ip_set_prepare2recover;
}

VRT_RECOVER_CONTROL_MODE HVRT_CONTROL::get_P_vrt_recover_mode() const
{
    return P_vrt_recover_mode;
}

double HVRT_CONTROL::get_P_recover_slope_in_pu_per_s() const
{
    return kP_recover;
}

double HVRT_CONTROL::get_P_recover_inertia_in_s() const
{
    return TP_recover;
}

VRT_DURING_CONTROL_MODE HVRT_CONTROL::get_Q_vrt_during_mode() const
{
    return Q_vrt_during_mode;
}

double HVRT_CONTROL::get_kQ_vrt_during() const
{
    return kQ_during;
}

double HVRT_CONTROL::get_Q_set_vrt_during_in_pu() const
{
    return Q_set_during;
}

double HVRT_CONTROL::get_k1_Iq_vrt_during() const
{
    return k1_Iq_during;
}

double HVRT_CONTROL::get_k2_Iq_vrt_during() const
{
    return k2_Iq_during;
}

double HVRT_CONTROL::get_Iq_set_vrt_during_in_pu() const
{
    return Iq_set_during;
}

VRT_PREPARE2RECOVER_CONTROL_MODE HVRT_CONTROL::get_Q_vrt_prepare2recover_mode() const
{
    return Q_vrt_prepare2recover_mode;
}

double HVRT_CONTROL::get_k_Iq_prepare2recover() const
{
    return k_Iq_prepare2recover;
}

double HVRT_CONTROL::get_Iq_set_prepare2recover() const
{
    return Iq_set_prepare2recover;
}

VRT_RECOVER_CONTROL_MODE HVRT_CONTROL::get_Q_vrt_recover_mode() const
{
    return Q_vrt_recover_mode;
}

double HVRT_CONTROL::get_Q_recover_slope_in_pu_per_s() const
{
    return kQ_recover;
}

double HVRT_CONTROL::get_Q_recover_inertia_in_s() const
{
    return TQ_recover;
}

void HVRT_CONTROL::check_vrt_status()
{
    VRT_STATUS current_status = get_vrt_status();
    switch(current_status)
    {
        case VRT_NORMAL_STATUS:
        {
            check_when_vrt_in_normal_status();
            break;
        }
        case VRT_DURING_STATUS:
        {
            check_when_vrt_in_during_status();
            break;
        }
        case VRT_PREPARE2RECOVER_STATUS:
        {
            check_when_vrt_in_prepare2recover_status();
            break;
        }
        case VRT_RECOVER_STATUS:
        default:
        {
            check_when_vrt_in_recover_status();
            break;
        }
    }
}

void HVRT_CONTROL::check_when_vrt_in_normal_status()
{
    double v = get_bus_voltage();
    double vth = get_vrt_voltage_threshold();

    if(v>vth)
    {
        STEPS & toolkit = get_toolkit();
        double time = toolkit.get_dynamic_simulation_time_in_s();

        set_vrt_status(VRT_DURING_STATUS);
        set_time_when_vrt_enter_during_status(time);

        ostringstream osstream;
        osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                <<" VRT_NORMAL_STATUS to VRT_DURING_STATUS at time "<<time<<" s since V is "<<v<<"(>"<<vth<<")";;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HVRT_CONTROL::check_when_vrt_in_during_status()
{
    double v = get_bus_voltage();
    double vth = get_vrt_voltage_threshold();
    if(v<vth)
    {
        ostringstream osstream;
        STEPS & toolkit = get_toolkit();
        double time = toolkit.get_dynamic_simulation_time_in_s();
        if(get_vrt_prepare2recover_delay_time_in_s()>FLOAT_EPSILON)
        {
            set_vrt_status(VRT_PREPARE2RECOVER_STATUS);
            set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
            set_time_when_vrt_enter_prepare2recover_status(time);
            start_vrt_prepare2recover_timer();

            osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                    <<" VRT_DURING_STATUS to VRT_PREPARE2RECOVER_STATUS at time "<<time<<" s since V is "<<v<<"(<"<<vth<<")";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            //set_vrt_status(VRT_PREPARE2RECOVER_STATUS);
            double P0 = get_vrt_P_command();
            double Q0 = get_vrt_Q_command();
            double Ip0 = get_vrt_Ip_command();
            double Iq0 = get_vrt_Iq_command();
            update_PQI0_in_vrt_recover_status(P0, Q0, Ip0, Iq0);

            set_vrt_status(VRT_RECOVER_STATUS);
            P_recovered = false;
            Q_recovered = false;
            set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
            set_time_when_vrt_enter_recover_status(time);

            osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                    <<" VRT_DURING_STATUS to VRT_RECOVER_STATUS at time "<<time<<" s since V is "<<v<<"(<"<<vth<<") and prepare2recover timer delay is ZERO.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void HVRT_CONTROL::check_when_vrt_in_prepare2recover_status()
{
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    double v = get_bus_voltage();
    double vth = get_vrt_voltage_threshold();
    if(v>vth)
    {
        set_vrt_status(VRT_DURING_STATUS);
        set_time_when_vrt_enter_during_status(time);
        set_time_when_vrt_enter_prepare2recover_status(INFINITE_THRESHOLD);
        reset_vrt_prepare2recover_timer();

        osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                <<" VRT_PREPARE2RECOVER_STATUS to VRT_DURING_STATUS at time "<<time<<" s since V is "<<v<<"(>"<<vth<<")";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        if(is_vrt_prepare2recover_timer_timed_out())
        {
            osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                    <<" VRT_PREPARE2RECOVER_STATUS to VRT_RECOVER_STATUS at time "<<time<<" s since prepare2recover timer is timed out.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            set_vrt_status(VRT_RECOVER_STATUS);
            P_recovered = false;
            Q_recovered = false;
            set_time_when_vrt_enter_prepare2recover_status(INFINITE_THRESHOLD);
            set_time_when_vrt_enter_recover_status(time);
            reset_vrt_prepare2recover_timer();
        }
    }

}

void HVRT_CONTROL::check_when_vrt_in_recover_status()
{
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    double v = get_bus_voltage();
    double vth = get_vrt_voltage_threshold();
    if(v>vth)
    {
        set_vrt_status(VRT_DURING_STATUS);
        set_time_when_vrt_enter_during_status(time);
        set_time_when_vrt_enter_recover_status(INFINITE_THRESHOLD);

        osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                <<" VRT_RECOVER_STATUS to VRT_DURING_STATUS at time "<<time<<" s since V is "<<v<<"(>"<<vth<<")";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        if(is_active_power_recovered() and is_reactive_power_recovered())
        {
            set_vrt_status(VRT_NORMAL_STATUS);
            set_time_when_vrt_enter_recover_status(INFINITE_THRESHOLD);

            osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                    <<" VRT_RECOVER_STATUS to VRT_NORMAL_STATUS at time "<<time<<" s since both P and Q command recovered.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

bool HVRT_CONTROL::is_active_power_recovered() const
{
    return P_recovered;
}

bool HVRT_CONTROL::is_reactive_power_recovered() const
{
    return Q_recovered;
}


double HVRT_CONTROL::get_vrt_P_command()
{
    STEPS& toolkit = get_toolkit();
    VRT_STATUS current_status = get_vrt_status();
    double order = 0.0, Ip0 = 0.0;
    double time = 0.0, t0 = 0.0;
    double Istart = 0.0, Iend  = 0.0;
    double k = 0.0;

    switch(current_status)
    {
        case VRT_DURING_STATUS:
        {
            VRT_DURING_CONTROL_MODE mode = get_P_vrt_during_mode();
            switch(mode)
            {
                case VRT_DURING_POWER_CONTROL_MODE:
                    return get_kP_vrt_during()*get_P0_vrt_activated_in_pu_based_on_mbase()+get_P_set_vrt_during_in_pu();
                case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
                    return get_bus_voltage()*(get_k1_Ip_vrt_during()*get_bus_voltage() + get_k2_Ip_vrt_during()*get_Ip0_vrt_activated_in_pu() + get_Ip_set_vrt_during_in_pu());
                case VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE:
                    return get_bus_voltage()*get_Ip0_vrt_activated_in_pu();
                case VRT_DURING_LVPL_CURRENT_CONTROL_MODE:
                default:
                    return 0.0;
            }
            break;
        }
        case VRT_PREPARE2RECOVER_STATUS:
        {
            VRT_PREPARE2RECOVER_CONTROL_MODE mode = get_P_vrt_prepare2recover_mode();
            switch(mode)
            {
                case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
                    Ip0 = get_Ip0_vrt_activated_in_pu();
                    order = get_k_Ip_prepare2recover()*Ip0+get_Ip_set_prepare2recover();
                    if(order>Ip0) order = Ip0;
                    return order*get_bus_voltage();
                case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
                    return get_Ip0_vrt_prepare2recover_activated_in_pu()*get_bus_voltage();
                case VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE:
                default:
                    return get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase();
            }
            break;
        }
        case VRT_RECOVER_STATUS:
        {
            VRT_RECOVER_CONTROL_MODE mode = get_P_vrt_recover_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            t0 = get_time_when_vrt_enter_recover_status();
            Istart = get_Ip0_vrt_recover_activated_in_pu();
            Iend = get_Ip0_vrt_activated_in_pu();
            switch(mode)
            {
                case VRT_RECOVER_LINEAR_CONTROL_MODE:
                    order = Istart+get_P_recover_slope_in_pu_per_s()*(time-t0);
                    break;
                case VRT_RECOVER_EXPOENTIAL_CONTROL_MODE:
                default:
                    k = (time-t0)/get_P_recover_inertia_in_s();
                    if(k>=5) order = Iend;
                    else     order = Istart + (Iend - Istart)*(1.0 - exp(-k));
                    break;
            }
            if(order>Iend or fabs(order-Iend)<FLOAT_EPSILON)
            {
                order = Iend;
                P_recovered = true;
            }
            return order*get_bus_voltage();
        }
        case VRT_NORMAL_STATUS:
        default:
            return 0.0;
    }
}
double HVRT_CONTROL::get_vrt_Q_command()
{
    STEPS& toolkit = get_toolkit();
    VRT_STATUS current_status = get_vrt_status();
    double order = 0.0;
    double time = 0.0, t0 = 0.0;
    double Istart = 0.0, Iend  = 0.0;
    double k = 0.0;

    switch(current_status)
    {
        case VRT_DURING_STATUS:
        {
            VRT_DURING_CONTROL_MODE mode = get_Q_vrt_during_mode();
            switch(mode)
            {
                case VRT_DURING_POWER_CONTROL_MODE:
                    return get_kQ_vrt_during()*get_Q0_vrt_activated_in_pu_based_on_mbase()+get_Q_set_vrt_during_in_pu();
                case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
                    return get_bus_voltage()*(get_k1_Iq_vrt_during()*(get_vrt_voltage_threshold() - get_bus_voltage()) + get_k2_Iq_vrt_during()*fabs(get_Iq0_vrt_activated_in_pu()) + get_Iq_set_vrt_during_in_pu());
                case VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE:
                    return get_bus_voltage()*fabs(get_Iq0_vrt_activated_in_pu());
                case VRT_DURING_LVPL_CURRENT_CONTROL_MODE:
                default:
                    return 0.0;
            }
            break;
        }
        case VRT_PREPARE2RECOVER_STATUS:
        {
            VRT_PREPARE2RECOVER_CONTROL_MODE mode = get_Q_vrt_prepare2recover_mode();
            switch(mode)
            {
                case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
                    return fabs(get_Iq0_vrt_activated_in_pu())*get_bus_voltage();
                case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
                    order = get_k_Iq_prepare2recover()* fabs(get_Iq0_vrt_prepare2recover_activated_in_pu()) + get_Iq_set_prepare2recover();
                    return order*get_bus_voltage();
                case VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE:
                default:
                    return get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase();
            }
            break;
        }
        case VRT_RECOVER_STATUS:
        {
            VRT_RECOVER_CONTROL_MODE mode = get_Q_vrt_recover_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            t0 = get_time_when_vrt_enter_recover_status();
            Istart = get_Iq0_vrt_recover_activated_in_pu();
            Iend = get_Iq0_vrt_activated_in_pu();
            switch(mode)
            {
                case VRT_RECOVER_LINEAR_CONTROL_MODE:
                    order = Istart+get_Q_recover_slope_in_pu_per_s()*(time-t0);
                    break;
                case VRT_RECOVER_EXPOENTIAL_CONTROL_MODE:
                default:
                    k = (time-t0)/get_Q_recover_inertia_in_s();
                    if(k>=5) order = Iend;
                    else     order = Istart + (Iend - Istart)*(1.0 - exp(-k));
                    break;
            }
            if(order>Iend or fabs(order-Iend)<FLOAT_EPSILON)
            {
                order = Iend;
                Q_recovered = true;
            }
            return fabs(order*get_bus_voltage());
        }
        case VRT_NORMAL_STATUS:
        default:
            return 0.0;
    }
}
