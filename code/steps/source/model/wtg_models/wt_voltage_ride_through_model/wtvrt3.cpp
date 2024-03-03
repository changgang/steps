#include "header/model/wtg_models/wt_voltage_ride_through_model/wtvrt3.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

WTVRT3::WTVRT3(STEPS& toolkit) : WT_VOLTAGE_RIDE_THROUGH_MODEL(toolkit),
                                 lvrt(toolkit), hvrt(toolkit)
{
    clear();
}

WTVRT3::WTVRT3(const WTVRT3& model) : WT_VOLTAGE_RIDE_THROUGH_MODEL(model.get_toolkit()),
                                      lvrt(model.get_toolkit()), hvrt(model.get_toolkit())
{
    copy_from_const_model(model);
}

WTVRT3::~WTVRT3()
{
    ;
}

WTVRT3& WTVRT3::operator=(const WTVRT3& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void WTVRT3::clear()
{
    set_model_float_parameter_count(58);
}

void WTVRT3::copy_from_const_model(const WTVRT3& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();
    set_vrt_voltage_threshold('L',model.get_vrt_voltage_threshold('L'));

    set_P_vrt_during_mode('L', model.get_P_vrt_during_mode('L'));
    set_Q_vrt_during_mode('L', model.get_Q_vrt_during_mode('L'));
    set_P_vrt_prepare2recover_mode('L', model.get_P_vrt_prepare2recover_mode('L'));
    set_Q_vrt_prepare2recover_mode('L', model.get_Q_vrt_prepare2recover_mode('L'));
    set_P_vrt_recover_mode('L', model.get_P_vrt_recover_mode('L'));
    set_Q_vrt_recover_mode('L', model.get_Q_vrt_recover_mode('L'));

    set_kP_vrt_during('L', model.get_kP_vrt_during('L'));
    set_P_set_vrt_during_in_pu('L', model.get_P_set_vrt_during_in_pu('L'));
    set_k1_Ip_vrt_during('L', model.get_k1_Ip_vrt_during('L'));
    set_k2_Ip_vrt_during('L', model.get_k2_Ip_vrt_during('L'));
    set_Ip_set_vrt_during_in_pu('L', model.get_Ip_set_vrt_during_in_pu('L'));
    set_LVPL_low_voltage_in_pu(model.get_LVPL_low_voltage_in_pu());
    set_LVPL_high_voltage_in_pu(model.get_LVPL_high_voltage_in_pu());
    set_LVPL_gain_at_high_voltage(model.get_LVPL_gain_at_high_voltage());

    set_k_Ip_prepare2recover('L', model.get_k_Ip_prepare2recover('L'));
    set_Ip_set_prepare2recover('L', model.get_Ip_set_prepare2recover('L'));

    set_P_recover_slope_in_pu_per_s('L', model.get_P_recover_slope_in_pu_per_s('L'));
    set_P_recover_inertia_in_s('L', model.get_P_recover_inertia_in_s('L'));

    set_kQ_vrt_during('L', model.get_kQ_vrt_during('L'));
    set_Q_set_vrt_during_in_pu('L', model.get_Q_set_vrt_during_in_pu('L'));
    set_k1_Iq_vrt_during('L', model.get_k1_Iq_vrt_during('L'));
    set_k2_Iq_vrt_during('L', model.get_k2_Iq_vrt_during('L'));
    set_Iq_set_vrt_during_in_pu('L', model.get_Iq_set_vrt_during_in_pu('L'));

    set_k_Iq_prepare2recover('L', model.get_k_Iq_prepare2recover('L'));
    set_Iq_set_prepare2recover('L', model.get_Iq_set_prepare2recover('L'));

    set_Q_recover_slope_in_pu_per_s('L', model.get_Q_recover_slope_in_pu_per_s('L'));
    set_Q_recover_inertia_in_s('L', model.get_Q_recover_inertia_in_s('L'));

    set_P_vrt_during_mode('H', model.get_P_vrt_during_mode('H'));
    set_Q_vrt_during_mode('H', model.get_Q_vrt_during_mode('H'));
    set_P_vrt_prepare2recover_mode('H', model.get_P_vrt_prepare2recover_mode('H'));
    set_Q_vrt_prepare2recover_mode('H', model.get_Q_vrt_prepare2recover_mode('H'));
    set_P_vrt_recover_mode('H', model.get_P_vrt_recover_mode('H'));
    set_Q_vrt_recover_mode('H', model.get_Q_vrt_recover_mode('H'));

    set_kP_vrt_during('H', model.get_kP_vrt_during('H'));
    set_P_set_vrt_during_in_pu('H', model.get_P_set_vrt_during_in_pu('H'));
    set_k1_Ip_vrt_during('H', model.get_k1_Ip_vrt_during('H'));
    set_k2_Ip_vrt_during('H', model.get_k2_Ip_vrt_during('H'));
    set_Ip_set_vrt_during_in_pu('H', model.get_Ip_set_vrt_during_in_pu('H'));

    set_k_Ip_prepare2recover('H', model.get_k_Ip_prepare2recover('H'));
    set_Ip_set_prepare2recover('H', model.get_Ip_set_prepare2recover('H'));

    set_P_recover_slope_in_pu_per_s('H', model.get_P_recover_slope_in_pu_per_s('H'));
    set_P_recover_inertia_in_s('H', model.get_P_recover_inertia_in_s('H'));

    set_kQ_vrt_during('H', model.get_kQ_vrt_during('H'));
    set_Q_set_vrt_during_in_pu('H', model.get_Q_set_vrt_during_in_pu('H'));
    set_k1_Iq_vrt_during('H', model.get_k1_Iq_vrt_during('H'));
    set_k2_Iq_vrt_during('H', model.get_k2_Iq_vrt_during('H'));
    set_Iq_set_vrt_during_in_pu('H', model.get_Iq_set_vrt_during_in_pu('H'));

    set_k_Iq_prepare2recover('H', model.get_k_Iq_prepare2recover('H'));
    set_Iq_set_prepare2recover('H', model.get_Iq_set_prepare2recover('H'));

    set_Q_recover_slope_in_pu_per_s('H', model.get_Q_recover_slope_in_pu_per_s('H'));
    set_Q_recover_inertia_in_s('H', model.get_Q_recover_inertia_in_s('H'));

}

string WTVRT3::get_model_name() const
{
    return "WTVRT3";
}

void WTVRT3::set_vrt_voltage_threshold(char HL_flag, double v)
{
    if(HL_flag=='L') return lvrt.set_vrt_voltage_threshold(v);
    else             return hvrt.set_vrt_voltage_threshold(v);
}

void WTVRT3::set_vrt_prepare2recover_delay_time_in_s(char HL_flag, double t)
{
    if(HL_flag=='L') return lvrt.set_vrt_prepare2recover_delay_time_in_s(t);
    else             return hvrt.set_vrt_prepare2recover_delay_time_in_s(t);
}


void WTVRT3::set_P_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_P_vrt_during_mode(m);
    else             return hvrt.set_P_vrt_during_mode(m);
}

void WTVRT3::set_kP_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_kP_vrt_during(k);
    else             return hvrt.set_kP_vrt_during(k);
}

void WTVRT3::set_P_set_vrt_during_in_pu(char HL_flag, double p)
{
    if(HL_flag=='L') return lvrt.set_P_set_vrt_during_in_pu(p);
    else             return hvrt.set_P_set_vrt_during_in_pu(p);
}

void WTVRT3::set_k1_Ip_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k1_Ip_vrt_during(k);
    else             return hvrt.set_k1_Ip_vrt_during(k);
}

void WTVRT3::set_k2_Ip_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k2_Ip_vrt_during(k);
    else             return hvrt.set_k2_Ip_vrt_during(k);
}

void WTVRT3::set_Ip_set_vrt_during_in_pu(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.set_Ip_set_vrt_during_in_pu(I);
    else             return hvrt.set_Ip_set_vrt_during_in_pu(I);
}

void WTVRT3::set_LVPL_low_voltage_in_pu(double v)
{
    lvrt.set_LVPL_low_voltage_in_pu(v);
}

void WTVRT3::set_LVPL_high_voltage_in_pu(double v)
{
    lvrt.set_LVPL_high_voltage_in_pu(v);
}

void WTVRT3::set_LVPL_gain_at_high_voltage(double g)
{
    lvrt.set_LVPL_gain_at_high_voltage(g);
}


void WTVRT3::set_P_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_P_vrt_prepare2recover_mode(m);
    else             return hvrt.set_P_vrt_prepare2recover_mode(m);
}

void WTVRT3::set_k_Ip_prepare2recover(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k_Ip_prepare2recover(k);
    else             return hvrt.set_k_Ip_prepare2recover(k);
}

void WTVRT3::set_Ip_set_prepare2recover(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.set_Ip_set_prepare2recover(I);
    else             return hvrt.set_Ip_set_prepare2recover(I);
}


void WTVRT3::set_P_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_P_vrt_recover_mode(m);
    else             return hvrt.set_P_vrt_recover_mode(m);
}

void WTVRT3::set_P_recover_slope_in_pu_per_s(char HL_flag, double s)
{
    if(HL_flag=='L') return lvrt.set_P_recover_slope_in_pu_per_s(s);
    else             return hvrt.set_P_recover_slope_in_pu_per_s(s);
}

void WTVRT3::set_P_recover_inertia_in_s(char HL_flag, double T)
{
    if(HL_flag=='L') return lvrt.set_P_recover_inertia_in_s(T);
    else             return hvrt.set_P_recover_inertia_in_s(T);
}


void WTVRT3::set_Q_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_Q_vrt_during_mode(m);
    else             return hvrt.set_Q_vrt_during_mode(m);
}

void WTVRT3::set_kQ_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_kQ_vrt_during(k);
    else             return hvrt.set_kQ_vrt_during(k);
}

void WTVRT3::set_Q_set_vrt_during_in_pu(char HL_flag, double q)
{
    if(HL_flag=='L') return lvrt.set_Q_set_vrt_during_in_pu(q);
    else             return hvrt.set_Q_set_vrt_during_in_pu(q);
}

void WTVRT3::set_k1_Iq_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k1_Iq_vrt_during(k);
    else             return hvrt.set_k1_Iq_vrt_during(k);
}

void WTVRT3::set_k2_Iq_vrt_during(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k2_Iq_vrt_during(k);
    else             return hvrt.set_k2_Iq_vrt_during(k);
}

void WTVRT3::set_Iq_set_vrt_during_in_pu(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.set_Iq_set_vrt_during_in_pu(I);
    else             return hvrt.set_Iq_set_vrt_during_in_pu(I);
}

void WTVRT3::set_Q_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_Q_vrt_prepare2recover_mode(m);
    else             return hvrt.set_Q_vrt_prepare2recover_mode(m);
}

void WTVRT3::set_k_Iq_prepare2recover(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k_Iq_prepare2recover(k);
    else             return hvrt.set_k_Iq_prepare2recover(k);
}

void WTVRT3::set_Iq_set_prepare2recover(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.set_Iq_set_prepare2recover(I);
    else             return hvrt.set_Iq_set_prepare2recover(I);
}


void WTVRT3::set_Q_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_Q_vrt_recover_mode(m);
    else             return hvrt.set_Q_vrt_recover_mode(m);
}

void WTVRT3::set_Q_recover_slope_in_pu_per_s(char HL_flag, double s)
{
    if(HL_flag=='L') return lvrt.set_Q_recover_slope_in_pu_per_s(s);
    else             return hvrt.set_Q_recover_slope_in_pu_per_s(s);
}

void WTVRT3::set_Q_recover_inertia_in_s(char HL_flag, double T)
{
    if(HL_flag=='L') return lvrt.set_Q_recover_inertia_in_s(T);
    else             return hvrt.set_Q_recover_inertia_in_s(T);
}


double WTVRT3::get_vrt_voltage_threshold(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_vrt_voltage_threshold();
    else             return hvrt.get_vrt_voltage_threshold();
}

double WTVRT3::get_vrt_prepare2recover_delay_time_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_vrt_prepare2recover_delay_time_in_s();
    else             return hvrt.get_vrt_prepare2recover_delay_time_in_s();
}


VRT_DURING_CONTROL_MODE WTVRT3::get_P_vrt_during_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_vrt_during_mode();
    else             return hvrt.get_P_vrt_during_mode();
}

double WTVRT3::get_kP_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_kP_vrt_during();
    else             return hvrt.get_kP_vrt_during();
}

double WTVRT3::get_P_set_vrt_during_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_set_vrt_during_in_pu();
    else             return hvrt.get_P_set_vrt_during_in_pu();
}

double WTVRT3::get_k1_Ip_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k1_Ip_vrt_during();
    else             return hvrt.get_k1_Ip_vrt_during();
}

double WTVRT3::get_k2_Ip_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k2_Ip_vrt_during();
    else             return hvrt.get_k2_Ip_vrt_during();
}

double WTVRT3::get_Ip_set_vrt_during_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Ip_set_vrt_during_in_pu();
    else             return hvrt.get_Ip_set_vrt_during_in_pu();
}

double WTVRT3::get_LVPL_low_voltage_in_pu() const
{
    return lvrt.get_LVPL_low_voltage_in_pu();
}

double WTVRT3::get_LVPL_high_voltage_in_pu() const
{
    return lvrt.get_LVPL_high_voltage_in_pu();
}

double WTVRT3::get_LVPL_gain_at_high_voltage() const
{
    return lvrt.get_LVPL_gain_at_high_voltage();
}


VRT_PREPARE2RECOVER_CONTROL_MODE WTVRT3::get_P_vrt_prepare2recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_vrt_prepare2recover_mode();
    else             return hvrt.get_P_vrt_prepare2recover_mode();
}

double WTVRT3::get_k_Ip_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k_Ip_prepare2recover();
    else             return hvrt.get_k_Ip_prepare2recover();
}

double WTVRT3::get_Ip_set_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Ip_set_prepare2recover();
    else             return hvrt.get_Ip_set_prepare2recover();
}


VRT_RECOVER_CONTROL_MODE WTVRT3::get_P_vrt_recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_vrt_recover_mode();
    else             return hvrt.get_P_vrt_recover_mode();
}

double WTVRT3::get_P_recover_slope_in_pu_per_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_recover_slope_in_pu_per_s();
    else             return hvrt.get_P_recover_slope_in_pu_per_s();
}

double WTVRT3::get_P_recover_inertia_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_P_recover_inertia_in_s();
    else             return hvrt.get_P_recover_inertia_in_s();
}


VRT_DURING_CONTROL_MODE WTVRT3::get_Q_vrt_during_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_vrt_during_mode();
    else             return hvrt.get_Q_vrt_during_mode();
}

double WTVRT3::get_kQ_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_kQ_vrt_during();
    else             return hvrt.get_kQ_vrt_during();
}

double WTVRT3::get_Q_set_vrt_during_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_set_vrt_during_in_pu();
    else             return hvrt.get_Q_set_vrt_during_in_pu();
}

double WTVRT3::get_k1_Iq_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k1_Iq_vrt_during();
    else             return hvrt.get_k1_Iq_vrt_during();
}

double WTVRT3::get_k2_Iq_vrt_during(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k2_Iq_vrt_during();
    else             return hvrt.get_k2_Iq_vrt_during();
}

double WTVRT3::get_Iq_set_vrt_during_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Iq_set_vrt_during_in_pu();
    else             return hvrt.get_Iq_set_vrt_during_in_pu();
}


VRT_PREPARE2RECOVER_CONTROL_MODE WTVRT3::get_Q_vrt_prepare2recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_vrt_prepare2recover_mode();
    else             return hvrt.get_Q_vrt_prepare2recover_mode();
}

double WTVRT3::get_k_Iq_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_k_Iq_prepare2recover();
    else             return hvrt.get_k_Iq_prepare2recover();
}

double WTVRT3::get_Iq_set_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Iq_set_prepare2recover();
    else             return hvrt.get_Iq_set_prepare2recover();
}


VRT_RECOVER_CONTROL_MODE WTVRT3::get_Q_vrt_recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_vrt_recover_mode();
    else             return hvrt.get_Q_vrt_recover_mode();
}

double WTVRT3::get_Q_recover_slope_in_pu_per_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_recover_slope_in_pu_per_s();
    else             return hvrt.get_Q_recover_slope_in_pu_per_s();
}

double WTVRT3::get_Q_recover_inertia_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.get_Q_recover_inertia_in_s();
    else             return hvrt.get_Q_recover_inertia_in_s();
}

bool WTVRT3::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=58)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name == get_model_name())
        {
            //lvrt
            double lvrt_voltage_threshold;
            double lvrt_prepare2recover_delay_time;
            unsigned int P_lvrt_during_mode, Q_lvrt_during_mode;
            unsigned int P_lvrt_prepare2recover_mode, Q_lvrt_prepare2recover_mode;
            unsigned int P_lvrt_recover_mode, Q_lvrt_recover_mode;

            double kP_lvrt_during, P_set_lvrt_during; // P power control
            double k1_Ip_lvrt_during, k2_Ip_lvrt_during, Ip_set_lvrt_during; // P current control
            double LVPL_low_vlotage, LVPL_high_voltage, LVPL_gain_at_high_voltage; //LVPL control

            double kQ_lvrt_during, Q_set_lvrt_during; // Q power control
            double k1_Iq_lvrt_during, k2_Iq_lvrt_during, Iq_set_lvrt_during; // Q current control

            double k_Ip_lvrt_prepare2recover, Ip_set_lvrt_prepare2recover;  // prepare to recover
            double k_Iq_lvrt_prepare2recover, Iq_set_lvrt_prepare2recover;

            double kP_lvrt_recover, TP_lvrt_recover; // recover
            double kQ_lvrt_recover, TQ_lvrt_recover;


            //hvrt
            double hvrt_voltage_threshold;
            double hvrt_prepare2recover_delay_time;

            unsigned int P_hvrt_during_mode, Q_hvrt_during_mode;
            unsigned int P_hvrt_prepare2recover_mode, Q_hvrt_prepare2recover_mode;
            unsigned int P_hvrt_recover_mode, Q_hvrt_recover_mode;

            double kP_hvrt_during, P_set_hvrt_during; // P power control
            double k1_Ip_hvrt_during, k2_Ip_hvrt_during, Ip_set_hvrt_during; // P current control

            double kQ_hvrt_during, Q_set_hvrt_during; // Q power control
            double k1_Iq_hvrt_during, k2_Iq_hvrt_during, Iq_set_hvrt_during; // Q current control

            double k_Ip_hvrt_prepare2recover, Ip_set_hvrt_prepare2recover;  // prepare to recover
            double k_Iq_hvrt_prepare2recover, Iq_set_hvrt_prepare2recover;

            double kP_hvrt_recover, TP_hvrt_recover; // recover
            double kQ_hvrt_recover, TQ_hvrt_recover;

            unsigned int i=3;
            //lvrt
            lvrt_voltage_threshold = get_double_data(data[i],"0.0"); i++;
            lvrt_prepare2recover_delay_time = get_double_data(data[i],"0.0"); i++;
            P_lvrt_during_mode = get_integer_data(data[i],"0"); i++;
            Q_lvrt_during_mode = get_integer_data(data[i],"0"); i++;
            P_lvrt_prepare2recover_mode = get_integer_data(data[i],"0"); i++;
            Q_lvrt_prepare2recover_mode = get_integer_data(data[i],"0"); i++;
            P_lvrt_recover_mode = get_integer_data(data[i],"0"); i++;
            Q_lvrt_recover_mode = get_integer_data(data[i],"0"); i++;

            kP_lvrt_during = get_double_data(data[i],"0.0"); i++;
            P_set_lvrt_during = get_double_data(data[i],"0.0"); i++;
            k1_Ip_lvrt_during = get_double_data(data[i],"0.0"); i++;
            k2_Ip_lvrt_during = get_double_data(data[i],"0.0"); i++;
            Ip_set_lvrt_during = get_double_data(data[i],"0.0"); i++;
            LVPL_low_vlotage = get_double_data(data[i],"0.0"); i++;
            LVPL_high_voltage = get_double_data(data[i],"0.0"); i++;
            LVPL_gain_at_high_voltage = get_double_data(data[i],"0.0"); i++;
            kQ_lvrt_during = get_double_data(data[i],"0.0"); i++;
            Q_set_lvrt_during = get_double_data(data[i],"0.0"); i++;
            k1_Iq_lvrt_during = get_double_data(data[i],"0.0"); i++;
            k2_Iq_lvrt_during = get_double_data(data[i],"0.0"); i++;
            Iq_set_lvrt_during = get_double_data(data[i],"0.0"); i++;
            k_Ip_lvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            Ip_set_lvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            k_Iq_lvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            Iq_set_lvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            kP_lvrt_recover = get_double_data(data[i],"0.0"); i++;
            TP_lvrt_recover = get_double_data(data[i],"0.0"); i++;
            kQ_lvrt_recover = get_double_data(data[i],"0.0"); i++;
            TQ_lvrt_recover = get_double_data(data[i],"0.0"); i++;

            //hvrt
            hvrt_voltage_threshold = get_double_data(data[i],"0.0"); i++;
            hvrt_prepare2recover_delay_time = get_double_data(data[i],"0.0"); i++;
            P_hvrt_during_mode = get_integer_data(data[i],"0"); i++;
            Q_hvrt_during_mode = get_integer_data(data[i],"0"); i++;
            P_hvrt_prepare2recover_mode = get_integer_data(data[i],"0"); i++;
            Q_hvrt_prepare2recover_mode = get_integer_data(data[i],"0"); i++;
            P_hvrt_recover_mode = get_integer_data(data[i],"0"); i++;
            Q_hvrt_recover_mode = get_integer_data(data[i],"0"); i++;

            kP_hvrt_during = get_double_data(data[i],"0.0"); i++;
            P_set_hvrt_during = get_double_data(data[i],"0.0"); i++;
            k1_Ip_hvrt_during = get_double_data(data[i],"0.0"); i++;
            k2_Ip_hvrt_during = get_double_data(data[i],"0.0"); i++;
            Ip_set_hvrt_during = get_double_data(data[i],"0.0"); i++;
            kQ_hvrt_during = get_double_data(data[i],"0.0"); i++;
            Q_set_hvrt_during = get_double_data(data[i],"0.0"); i++;
            k1_Iq_hvrt_during = get_double_data(data[i],"0.0"); i++;
            k2_Iq_hvrt_during = get_double_data(data[i],"0.0"); i++;
            Iq_set_hvrt_during = get_double_data(data[i],"0.0"); i++;
            k_Ip_hvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            Ip_set_hvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            k_Iq_hvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            Iq_set_hvrt_prepare2recover = get_double_data(data[i],"0.0"); i++;
            kP_hvrt_recover = get_double_data(data[i],"0.0"); i++;
            TP_hvrt_recover = get_double_data(data[i],"0.0"); i++;
            kQ_hvrt_recover = get_double_data(data[i],"0.0"); i++;
            TQ_hvrt_recover = get_double_data(data[i],"0.0"); i++;


            //set lvrt
            set_vrt_voltage_threshold('L', lvrt_voltage_threshold);
            set_vrt_prepare2recover_delay_time_in_s('L', lvrt_prepare2recover_delay_time);

            VRT_DURING_CONTROL_MODE vrt_during_mode;
            switch(P_lvrt_during_mode)
            {
                case 0:
                {
                    vrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                    break;
                }
                case 2:
                {
                    vrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_during_mode = VRT_DURING_LVPL_CURRENT_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_during_mode('L', vrt_during_mode);

            switch(Q_lvrt_during_mode)
            {
                case 0:
                {
                    vrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                    break;
                }
                case 2:
                {
                    vrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_during_mode = VRT_DURING_LVPL_CURRENT_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_during_mode('L', vrt_during_mode);

            VRT_PREPARE2RECOVER_CONTROL_MODE vrt_prepare2recover_mode;
            switch(P_lvrt_prepare2recover_mode)
            {
                case 0:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_prepare2recover_mode('L',vrt_prepare2recover_mode);

            switch(Q_lvrt_prepare2recover_mode)
            {
                case 0:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_prepare2recover_mode('L',vrt_prepare2recover_mode);

            VRT_RECOVER_CONTROL_MODE vrt_recover_mode;
            switch(P_lvrt_recover_mode)
            {
                case 0:
                {
                    vrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_recover_mode('L', vrt_recover_mode);

            switch(Q_lvrt_recover_mode)
            {
                case 0:
                {
                    vrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_recover_mode('L', vrt_recover_mode);

            set_kP_vrt_during('L', kP_lvrt_during);
            set_P_set_vrt_during_in_pu('L',P_set_lvrt_during);
            set_k1_Ip_vrt_during('L',k1_Ip_lvrt_during);
            set_k2_Ip_vrt_during('L',k2_Ip_lvrt_during);
            set_Ip_set_vrt_during_in_pu('L',Ip_set_lvrt_during);
            set_LVPL_low_voltage_in_pu(LVPL_low_vlotage);
            set_LVPL_high_voltage_in_pu(LVPL_high_voltage);
            set_LVPL_gain_at_high_voltage(LVPL_gain_at_high_voltage);

            set_kQ_vrt_during('L',kQ_lvrt_during);
            set_Q_set_vrt_during_in_pu('L',Q_set_lvrt_during);
            set_k1_Iq_vrt_during('L',k1_Iq_lvrt_during);
            set_k2_Iq_vrt_during('L',k2_Iq_lvrt_during);
            set_Iq_set_vrt_during_in_pu('L',Iq_set_lvrt_during);

            set_k_Ip_prepare2recover('L',k_Ip_lvrt_prepare2recover);
            set_Ip_set_prepare2recover('L',Ip_set_lvrt_prepare2recover);
            set_k_Iq_prepare2recover('L',k_Iq_lvrt_prepare2recover);
            set_Iq_set_prepare2recover('L',Iq_set_lvrt_prepare2recover);
            set_P_recover_slope_in_pu_per_s('L',kP_lvrt_recover);
            set_P_recover_inertia_in_s('L',TP_lvrt_recover);
            set_Q_recover_slope_in_pu_per_s('L',kQ_lvrt_recover);
            set_Q_recover_inertia_in_s('L',TQ_lvrt_recover);

            //set hvrt

            set_vrt_voltage_threshold('H', hvrt_voltage_threshold);
            set_vrt_prepare2recover_delay_time_in_s('H', hvrt_prepare2recover_delay_time);

            switch(P_hvrt_during_mode)
            {
                case 0:
                {
                    vrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_during_mode('H', vrt_during_mode);

            switch(Q_hvrt_during_mode)
            {
                case 0:
                {
                    vrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                    break;
                }
                case 2:
                {
                    vrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_during_mode = VRT_DURING_LVPL_CURRENT_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_during_mode('H', vrt_during_mode);

            switch(P_hvrt_prepare2recover_mode)
            {
                case 0:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_prepare2recover_mode('H', vrt_prepare2recover_mode);

            switch(Q_hvrt_prepare2recover_mode)
            {
                case 0:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_prepare2recover_mode('H', vrt_prepare2recover_mode);


            switch(P_hvrt_recover_mode)
            {
                case 0:
                {
                    vrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                    break;
                }
            }
            set_P_vrt_recover_mode('H', vrt_recover_mode);

            switch(Q_hvrt_recover_mode)
            {
                case 0:
                {
                    vrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                    break;
                }
                default:
                {
                    vrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                    break;
                }
            }
            set_Q_vrt_recover_mode('H', vrt_recover_mode);

            set_kP_vrt_during('H',kP_hvrt_during);
            set_P_set_vrt_during_in_pu('H',P_set_hvrt_during);
            set_k1_Ip_vrt_during('H',k1_Ip_hvrt_during);
            set_k2_Ip_vrt_during('H',k2_Ip_hvrt_during);
            set_Ip_set_vrt_during_in_pu('H',Ip_set_hvrt_during);

            set_kQ_vrt_during('H',kQ_hvrt_during);
            set_Q_set_vrt_during_in_pu('H',Q_set_hvrt_during);
            set_k1_Iq_vrt_during('H',k1_Iq_hvrt_during);
            set_k2_Iq_vrt_during('H',k2_Iq_hvrt_during);
            set_Iq_set_vrt_during_in_pu('H',Iq_set_hvrt_during);

            set_k_Ip_prepare2recover('H',k_Ip_hvrt_prepare2recover);
            set_Ip_set_prepare2recover('H',Ip_set_hvrt_prepare2recover);
            set_k_Iq_prepare2recover('H',k_Iq_hvrt_prepare2recover);
            set_Iq_set_prepare2recover('H',Iq_set_hvrt_prepare2recover);
            set_P_recover_slope_in_pu_per_s('H',kP_hvrt_recover);
            set_P_recover_inertia_in_s('H',TP_hvrt_recover);
            set_Q_recover_slope_in_pu_per_s('H',kQ_hvrt_recover);
            set_Q_recover_inertia_in_s('H',TQ_hvrt_recover);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool WTVRT3::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WTVRT3::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WTVRT3::setup_block_toolkit_and_parameters()
{
    lvrt.set_bus_pointer(get_bus_pointer());
    lvrt.set_device_pointer(get_device_pointer());

    hvrt.set_bus_pointer(get_bus_pointer());
    hvrt.set_device_pointer(get_device_pointer());
}

void WTVRT3::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        WT_ELECTRICAL_MODEL* wtelecmodel = wt_generator->get_wt_electrical_model();

        if(not wtelecmodel->is_model_initialized())
            wtelecmodel->initialize();

        setup_block_toolkit_and_parameters();

        STEPS& toolkit = get_toolkit();

        double P0, Q0, Ip0, Iq0;
        P0 = wtelecmodel->get_active_power_command_in_pu_based_on_mbase();
        Q0 = wtelecmodel->get_reactive_power_command_in_pu_based_on_mbase();
        Ip0 = wtelecmodel->get_active_current_command_in_pu_based_on_mbase();
        Iq0 = wtelecmodel->get_reactive_current_command_in_pu_based_on_mbase();

        lvrt.set_vrt_status(VRT_NORMAL_STATUS);
        lvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);

        hvrt.set_vrt_status(VRT_NORMAL_STATUS);
        hvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);

        set_flag_model_initialized_as_true();
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) VRT initial P command: "<<P0<<endl
                    <<"(2) VRT initial Q command: "<<Q0<<endl
                    <<"(3) VRT initial IP command: "<<Ip0<<endl
                    <<"(4) VRT initial IQ command: "<<Iq0;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WTVRT3::run(DYNAMIC_MODE mode)
{
    if(mode == UPDATE_MODE)
    {
        if(hvrt.get_vrt_status() == VRT_NORMAL_STATUS)
            lvrt.check_vrt_status();
        if(lvrt.get_vrt_status() == VRT_NORMAL_STATUS)
            hvrt.check_vrt_status();


        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        WT_ELECTRICAL_MODEL* wtelecmodel = wt_generator->get_wt_electrical_model();

        double P0, Q0, Ip0, Iq0;

        switch(lvrt.get_vrt_status())
        {
            case VRT_NORMAL_STATUS:
            {
                P0 = wtelecmodel->get_active_power_command_in_pu_based_on_mbase();
                Q0 = wtelecmodel->get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = wtelecmodel->get_active_current_command_in_pu_based_on_mbase();
                Iq0 = wtelecmodel->get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_DURING_STATUS:
            {
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_prepare2recover_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_PREPARE2RECOVER_STATUS:
            {
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_recover_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_RECOVER_STATUS:
            default:
                break;
        }
        switch(hvrt.get_vrt_status())
        {
            case VRT_NORMAL_STATUS:
            {
                P0 = wtelecmodel->get_active_power_command_in_pu_based_on_mbase();
                Q0 = wtelecmodel->get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = wtelecmodel->get_active_current_command_in_pu_based_on_mbase();
                Iq0 = wtelecmodel->get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_DURING_STATUS:
            {
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_prepare2recover_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_PREPARE2RECOVER_STATUS:
            {
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_recover_status(P0, Q0, Ip0, Iq0);
                break;
            }
            case VRT_RECOVER_STATUS:
            default:
                break;
        }
        set_flag_model_updated_as_true();
    }
}

VRT_STATUS WTVRT3::get_lvrt_status() const
{
    return lvrt.get_vrt_status();
}

VRT_STATUS WTVRT3::get_hvrt_status() const
{
    return hvrt.get_vrt_status();
}

double WTVRT3::get_active_current_command_in_pu_based_on_mbase()
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_Ip_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_Ip_command();
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    return 0.0;
}

double WTVRT3::get_active_power_command_in_pu_based_on_mbase()
{
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_P_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_P_command();
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    return 0.0;
}

double WTVRT3::get_reactive_current_command_in_pu_based_on_mbase()
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_Iq_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_Iq_command();
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    return 0.0;
}

double WTVRT3::get_reactive_power_command_in_pu_based_on_mbase()
{
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_Q_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_Q_command();
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    return 0.0;
}

double WTVRT3::get_reactive_voltage_command_in_pu()
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    double xeq = gen->get_source_impedance_in_pu().imag();

    double Iqcmd = get_reactive_current_command_in_pu_based_on_mbase();

    return Iqcmd*(-xeq);
}

void WTVRT3::check()
{
    ;
}

void WTVRT3::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WTVRT3::save()
{
    ;
}

string WTVRT3::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    WT_GENERATOR* source = get_wt_generator_pointer();
    unsigned int bus = source->get_source_bus();
    string identifier = "'"+source->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    double lvrt_voltage_threshold = get_vrt_voltage_threshold('L');
    double lvrt_prepare2recover_delay_time = get_vrt_prepare2recover_delay_time_in_s('L');
    unsigned int P_lvrt_during_mode, Q_lvrt_during_mode;
    switch(get_P_vrt_during_mode('L'))
    {
        case VRT_DURING_POWER_CONTROL_MODE:
        {
            P_lvrt_during_mode = 0;
            break;
        }
        case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
        {
            P_lvrt_during_mode = 1;
            break;
        }
        case VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE:
        {
            P_lvrt_during_mode = 2;
            break;
        }
        default:
        {
            P_lvrt_during_mode = 3;
            break;
        }
    }

    switch(get_Q_vrt_during_mode('L'))
    {
        case VRT_DURING_POWER_CONTROL_MODE:
        {
            Q_lvrt_during_mode = 0;
            break;
        }
        case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
        {
            Q_lvrt_during_mode = 1;
            break;
        }
        case VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE:
        {
            Q_lvrt_during_mode = 2;
            break;
        }
        default:
        {
            Q_lvrt_during_mode = 3;
            break;
        }
    }

    unsigned int P_lvrt_prepare2recover_mode, Q_lvrt_prepare2recover_mode;
    switch(get_P_vrt_prepare2recover_mode('L'))
    {
        case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
        {
            P_lvrt_prepare2recover_mode = 0;
            break;
        }
        case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
        {
            P_lvrt_prepare2recover_mode = 1;
            break;
        }
        default:
        {
            P_lvrt_prepare2recover_mode = 2;
            break;
        }
    }

    switch(get_Q_vrt_prepare2recover_mode('L'))
    {
        case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
        {
            Q_lvrt_prepare2recover_mode = 0;
            break;
        }
        case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
        {
            Q_lvrt_prepare2recover_mode = 1;
            break;
        }
        default:
        {
            Q_lvrt_prepare2recover_mode = 2;
            break;
        }
    }
    unsigned int P_lvrt_recover_mode, Q_lvrt_recover_mode;

    switch(get_P_vrt_recover_mode('L'))
    {
        case VRT_RECOVER_LINEAR_CONTROL_MODE:
        {
            P_lvrt_recover_mode = 0;
            break;
        }
        default:
        {
            P_lvrt_recover_mode = 1;
            break;
        }
    }

    switch(get_Q_vrt_recover_mode('L'))
    {
        case VRT_RECOVER_LINEAR_CONTROL_MODE:
        {
            Q_lvrt_recover_mode = 0;
            break;
        }
        default:
        {
            Q_lvrt_recover_mode = 1;
            break;
        }
    }

    double kP_lvrt_during = get_kP_vrt_during('L');
    double P_set_lvrt_during = get_P_set_vrt_during_in_pu('L');
    double k1_Ip_lvrt_during = get_k1_Ip_vrt_during('L');
    double k2_Ip_lvrt_during = get_k2_Ip_vrt_during('L');
    double Ip_set_lvrt_during = get_Ip_set_vrt_during_in_pu('L');
    double LVPL_low_voltage = get_LVPL_low_voltage_in_pu();
    double LVPL_high_voltage = get_LVPL_high_voltage_in_pu();
    double LVPL_gain_at_high_voltage = get_LVPL_gain_at_high_voltage();

    double kQ_lvrt_during = get_kQ_vrt_during('L');
    double Q_set_lvrt_during = get_Q_set_vrt_during_in_pu('L');
    double k1_Iq_lvrt_during = get_k1_Iq_vrt_during('L');
    double k2_Iq_lvrt_during = get_k2_Iq_vrt_during('L');
    double Iq_set_lvrt_during = get_Iq_set_vrt_during_in_pu('L');

    double k_Ip_lvrt_prepare2recover = get_k_Ip_prepare2recover('L');
    double Ip_set_lvrt_prepare2recover = get_Ip_set_prepare2recover('L');
    double k_Iq_lvrt_prepare2recover = get_k_Iq_prepare2recover('L');
    double Iq_set_lvrt_prepare2recover = get_Iq_set_prepare2recover('L');
    double kP_lvrt_recover = get_P_recover_slope_in_pu_per_s('L');
    double TP_lvrt_recover = get_P_recover_inertia_in_s('L');
    double kQ_lvrt_recover= get_Q_recover_slope_in_pu_per_s('L');
    double TQ_lvrt_recover = get_Q_recover_inertia_in_s('L');

    //hvrt
    double hvrt_voltage_threshold = get_vrt_voltage_threshold('H');
    double hvrt_prepare2recover_delay_time = get_vrt_prepare2recover_delay_time_in_s('H');
    unsigned int P_hvrt_during_mode, Q_hvrt_during_mode;
    switch(get_P_vrt_during_mode('H'))
    {
        case VRT_DURING_POWER_CONTROL_MODE:
        {
            P_hvrt_during_mode = 0;
            break;
        }
        case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
        {
            P_hvrt_during_mode = 1;
            break;
        }
        default:
        {
            P_hvrt_during_mode = 2;
            break;
        }
    }

    switch(get_Q_vrt_during_mode('H'))
    {
        case VRT_DURING_POWER_CONTROL_MODE:
        {
            Q_hvrt_during_mode = 0;
            break;
        }
        case VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE:
        {
            Q_hvrt_during_mode = 1;
            break;
        }
        case VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE:
        {
            Q_hvrt_during_mode = 2;
            break;
        }
        default:
        {
            Q_hvrt_during_mode = 3;
            break;
        }
    }

    unsigned int P_hvrt_prepare2recover_mode, Q_hvrt_prepare2recover_mode;
    switch(get_P_vrt_prepare2recover_mode('H'))
    {
        case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
        {
            P_hvrt_prepare2recover_mode = 0;
            break;
        }
        case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
        {
            P_hvrt_prepare2recover_mode = 1;
            break;
        }
        default:
        {
            P_hvrt_prepare2recover_mode = 2;
            break;
        }
    }

    switch(get_Q_vrt_prepare2recover_mode('H'))
    {
        case VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE:
        {
            Q_hvrt_prepare2recover_mode = 0;
            break;
        }
        case VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE:
        {
            Q_hvrt_prepare2recover_mode = 1;
            break;
        }
        default:
        {
            Q_hvrt_prepare2recover_mode = 2;
            break;
        }
    }
    unsigned int P_hvrt_recover_mode, Q_hvrt_recover_mode;

    switch(get_P_vrt_recover_mode('H'))
    {
        case VRT_RECOVER_LINEAR_CONTROL_MODE:
        {
            P_hvrt_recover_mode = 0;
            break;
        }
        default:
        {
            P_hvrt_recover_mode = 1;
            break;
        }
    }

    switch(get_Q_vrt_recover_mode('H'))
    {
        case VRT_RECOVER_LINEAR_CONTROL_MODE:
        {
            Q_hvrt_recover_mode = 0;
            break;
        }
        default:
        {
            Q_hvrt_recover_mode = 1;
            break;
        }
    }

    double kP_hvrt_during = get_kP_vrt_during('H');
    double P_set_hvrt_during = get_P_set_vrt_during_in_pu('H');
    double k1_Ip_hvrt_during = get_k1_Ip_vrt_during('H');
    double k2_Ip_hvrt_during = get_k2_Ip_vrt_during('H');
    double Ip_set_hvrt_during = get_Ip_set_vrt_during_in_pu('H');

    double kQ_hvrt_during = get_kQ_vrt_during('H');
    double Q_set_hvrt_during = get_Q_set_vrt_during_in_pu('H');
    double k1_Iq_hvrt_during = get_k1_Iq_vrt_during('H');
    double k2_Iq_hvrt_during = get_k2_Iq_vrt_during('H');
    double Iq_set_hvrt_during = get_Iq_set_vrt_during_in_pu('H');

    double k_Ip_hvrt_prepare2recover = get_k_Ip_prepare2recover('H');
    double Ip_set_hvrt_prepare2recover = get_Ip_set_prepare2recover('H');
    double k_Iq_hvrt_prepare2recover = get_k_Iq_prepare2recover('H');
    double Iq_set_hvrt_prepare2recover = get_Iq_set_prepare2recover('H');
    double kP_hvrt_recover = get_P_recover_slope_in_pu_per_s('H');
    double TP_hvrt_recover = get_P_recover_inertia_in_s('H');
    double kQ_hvrt_recover= get_Q_recover_slope_in_pu_per_s('H');
    double TQ_hvrt_recover = get_Q_recover_inertia_in_s('H');


    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
    {
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;
    }

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<lvrt_voltage_threshold<<", "
            <<setw(8)<<setprecision(6)<<lvrt_prepare2recover_delay_time<<", "
            <<setw(8)<<P_lvrt_during_mode<<", "
            <<setw(8)<<Q_lvrt_during_mode<<", "
            <<setw(8)<<P_lvrt_prepare2recover_mode<<", "
            <<setw(8)<<Q_lvrt_prepare2recover_mode<<", "
            <<setw(8)<<P_lvrt_recover_mode<<", "
            <<setw(8)<<Q_lvrt_recover_mode<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kP_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<P_set_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k1_Ip_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k2_Ip_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Ip_set_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<LVPL_low_voltage<<", "
            <<setw(8)<<setprecision(6)<<LVPL_high_voltage<<", "
            <<setw(8)<<setprecision(6)<<LVPL_gain_at_high_voltage<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kQ_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Q_set_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k1_Iq_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k2_Iq_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Iq_set_lvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k_Ip_lvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<Ip_set_lvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<k_Iq_lvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<Iq_set_lvrt_prepare2recover<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kP_lvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<TP_lvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<kQ_lvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<TQ_lvrt_recover<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<hvrt_voltage_threshold<<", "
            <<setw(8)<<setprecision(6)<<hvrt_prepare2recover_delay_time<<", "
            <<setw(8)<<P_hvrt_during_mode<<", "
            <<setw(8)<<Q_hvrt_during_mode<<", "
            <<setw(8)<<P_hvrt_prepare2recover_mode<<", "
            <<setw(8)<<Q_hvrt_prepare2recover_mode<<", "
            <<setw(8)<<P_hvrt_recover_mode<<", "
            <<setw(8)<<Q_hvrt_recover_mode<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kP_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<P_set_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k1_Ip_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k2_Ip_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Ip_set_hvrt_during<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kQ_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Q_set_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k1_Iq_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k2_Iq_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<Iq_set_hvrt_during<<", "
            <<setw(8)<<setprecision(6)<<k_Ip_hvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<Ip_set_hvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<k_Iq_hvrt_prepare2recover<<", "
            <<setw(8)<<setprecision(6)<<Iq_set_hvrt_prepare2recover<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kP_hvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<TP_hvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<kQ_hvrt_recover<<", "
            <<setw(8)<<setprecision(6)<<TQ_hvrt_recover<<" /";

    return osstream.str();
}

void WTVRT3::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    //lvrt
    add_model_data_name_and_index_pair("LVRT VOLTAGE THRESHOLD", i); i++;
    add_model_data_name_and_index_pair("LVRT PREPARE2RECOVER DELAY TIME", i); i++;
    add_model_data_name_and_index_pair("P LVRT DURING MODE", i); i++;
    add_model_data_name_and_index_pair("Q LVRT DURING MODE", i); i++;
    add_model_data_name_and_index_pair("P LVRT PREPARE2RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("Q LVRT PREPARE2RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("P LVRT RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("Q LVRT RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("KP LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("P SET LVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("K1 IP LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K2 IP LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("IP SET LVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("LVPL LOW VOLTAGE", i); i++;
    add_model_data_name_and_index_pair("LVPL HIGH VOLTAGE", i); i++;
    add_model_data_name_and_index_pair("LVPL GAIN AT HIGH VOLTAGE", i); i++;
    add_model_data_name_and_index_pair("KQ LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("Q SET LVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("K1 IQ LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K2 IQ LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("IQ SET LVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K IP LVRT PREPARE2RECOVER", i); i++;
    add_model_data_name_and_index_pair("IP SET LVRT PREPARE2RECOVER IN PU", i); i++;
    add_model_data_name_and_index_pair("K IQ LVRT PREPARE2RECOVER", i); i++;
    add_model_data_name_and_index_pair("IQ SET LVRT PREPARE2RECOVER IN PU", i); i++;
    add_model_data_name_and_index_pair("KP LVRT RECOVER IN PU PER S", i); i++;
    add_model_data_name_and_index_pair("TP LVRT RECOVER IN S", i); i++;
    add_model_data_name_and_index_pair("KQ LVRT RECOVER IN PU PER S", i); i++;
    add_model_data_name_and_index_pair("TQ LVRT RECOVER IN S", i); i++;
    //hvrt
    add_model_data_name_and_index_pair("HVRT VOLTAGE THRESHOLD", i); i++;
    add_model_data_name_and_index_pair("HVRT PREPARE2RECOVER DELAY TIME", i); i++;
    add_model_data_name_and_index_pair("P HVRT DURING MODE", i); i++;
    add_model_data_name_and_index_pair("Q HVRT DURING MODE", i); i++;
    add_model_data_name_and_index_pair("P HVRT PREPARE2RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("Q HVRT PREPARE2RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("P HVRT RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("Q HVRT RECOVER MODE", i); i++;
    add_model_data_name_and_index_pair("KP HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("P SET HVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("K1 IP HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K2 IP HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("IP SET HVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("KQ HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("Q SET HVRT DURING IN PU", i); i++;
    add_model_data_name_and_index_pair("K1 IQ HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K2 IQ HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("IQ SET HVRT DURING", i); i++;
    add_model_data_name_and_index_pair("K IP HVRT PREPARE2RECOVER", i); i++;
    add_model_data_name_and_index_pair("IP SET HVRT PREPARE2RECOVER IN PU", i); i++;
    add_model_data_name_and_index_pair("K IQ HVRT PREPARE2RECOVER", i); i++;
    add_model_data_name_and_index_pair("IQ SET HVRT PREPARE2RECOVER IN PU", i); i++;
    add_model_data_name_and_index_pair("KP HVRT RECOVER IN PU PER S", i); i++;
    add_model_data_name_and_index_pair("TP HVRT RECOVER IN S", i); i++;
    add_model_data_name_and_index_pair("KQ HVRT RECOVER IN PU PER S", i); i++;
    add_model_data_name_and_index_pair("TQ LVRT RECOVER IN S", i); i++;
}

double WTVRT3::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    //lvrt
    if(par_name == "LVRT VOLTAGE THRESHOLD") return get_vrt_voltage_threshold('L');
    if(par_name == "LVRT PREPARE2RECOVER DELAY TIME") return get_vrt_prepare2recover_delay_time_in_s('L');
    if(par_name == "P LVRT DURING MODE") return get_P_vrt_during_mode('L');
    if(par_name == "Q LVRT DURING MODE") return get_Q_vrt_during_mode('L');
    if(par_name == "P LVRT PREPARE2RECOVER MODE") return get_P_vrt_prepare2recover_mode('L');
    if(par_name == "Q LVRT PREPARE2RECOVER MODE") return get_Q_vrt_prepare2recover_mode('L');
    if(par_name == "P LVRT RECOVER MODE") return get_P_vrt_recover_mode('L');
    if(par_name == "Q LVRT RECOVER MODE") return get_Q_vrt_recover_mode('L');
    if(par_name == "KP LVRT DURING") return  get_kP_vrt_during('L');
    if(par_name == "P SET LVRT DURING IN PU") return get_P_set_vrt_during_in_pu('L');
    if(par_name == "K1 IP LVRT DURING") return get_k1_Ip_vrt_during('L');
    if(par_name == "K2 IP LVRT DURING") return get_k2_Ip_vrt_during('L');
    if(par_name == "IP SET LVRT DURING IN PU") return get_Ip_set_vrt_during_in_pu('L');
    if(par_name == "LVPL LOW VOLTAGE") return get_LVPL_low_voltage_in_pu();
    if(par_name == "LVPL HIGH VOLTAGE") return get_LVPL_high_voltage_in_pu();
    if(par_name == "LVPL GAIN AT HIGH VOLTAGE") return get_LVPL_gain_at_high_voltage();
    if(par_name == "KQ LVRT DURING") return get_kQ_vrt_during('L');
    if(par_name == "Q SET LVRT DURING IN PU") return get_Q_set_vrt_during_in_pu('L');
    if(par_name == "K1 IQ LVRT DURING") return get_k1_Iq_vrt_during('L');
    if(par_name == "K2 IQ LVRT DURING") return get_k2_Iq_vrt_during('L');
    if(par_name == "IQ SET LVRT DURING") return get_Iq_set_vrt_during_in_pu('L');
    if(par_name == "K IP LVRT PREPARE2RECOVER") return get_k_Ip_prepare2recover('L');
    if(par_name == "IP SET LVRT PREPARE2RECOVER IN PU") return get_Ip_set_prepare2recover('L');
    if(par_name == "K IQ LVRT PREPARE2RECOVER") return get_k_Iq_prepare2recover('L');
    if(par_name == "IQ SET LVRT PREPARE2RECOVER IN PU") return get_Iq_set_prepare2recover('L');
    if(par_name == "KP LVRT RECOVER IN PU PER S") return get_P_recover_slope_in_pu_per_s('L');
    if(par_name == "TP LVRT RECOVER IN S") return get_P_recover_inertia_in_s('L');
    if(par_name == "KQ LVRT RECOVER IN PU PER S") return get_Q_recover_slope_in_pu_per_s('L');
    if(par_name == "TQ LVRT RECOVER IN S") return get_Q_recover_inertia_in_s('L');

    //hvrt
    if(par_name == "HVRT VOLTAGE THRESHOLD") return get_vrt_voltage_threshold('H');
    if(par_name == "HVRT PREPARE2RECOVER DELAY TIME") return get_vrt_prepare2recover_delay_time_in_s('H');
    if(par_name == "P HVRT DURING MODE") return get_P_vrt_during_mode('H');
    if(par_name == "Q HVRT DURING MODE") return get_Q_vrt_during_mode('H');
    if(par_name == "P HVRT PREPARE2RECOVER MODE") return get_P_vrt_prepare2recover_mode('H');
    if(par_name == "Q HVRT PREPARE2RECOVER MODE") return get_Q_vrt_prepare2recover_mode('H');
    if(par_name == "P HVRT RECOVER MODE") return get_P_vrt_recover_mode('H');
    if(par_name == "Q HVRT RECOVER MODE") return get_Q_vrt_recover_mode('H');
    if(par_name == "KP HVRT DURING") return get_kP_vrt_during('H');
    if(par_name == "P SET HVRT DURING IN PU") return get_P_set_vrt_during_in_pu('H');
    if(par_name == "K1 IP HVRT DURING") return get_k1_Ip_vrt_during('H');
    if(par_name == "K2 IP HVRT DURING") return get_k2_Ip_vrt_during('H');
    if(par_name == "IP SET HVRT DURING IN PU") return get_Ip_set_vrt_during_in_pu('H');
    if(par_name == "KQ HVRT DURING") return get_kQ_vrt_during('H');
    if(par_name == "Q SET HVRT DURING IN PU") return get_Q_set_vrt_during_in_pu('H');
    if(par_name == "K1 IQ HVRT DURING") return get_k1_Iq_vrt_during('H');
    if(par_name == "K2 IQ HVRT DURING") return get_k2_Iq_vrt_during('H');
    if(par_name == "IQ SET HVRT DURING") return get_Iq_set_vrt_during_in_pu('H');
    if(par_name == "K IP HVRT PREPARE2RECOVER") return get_k_Ip_prepare2recover('H');
    if(par_name == "IP SET HVRT PREPARE2RECOVER IN PU") return get_Ip_set_prepare2recover('H');
    if(par_name == "K IQ HVRT PREPARE2RECOVER") return get_k_Iq_prepare2recover('H');
    if(par_name == "IQ SET HVRT PREPARE2RECOVER IN PU") return get_Iq_set_prepare2recover('H');
    if(par_name == "KP HVRT RECOVER IN PU PER S") return get_P_recover_slope_in_pu_per_s('H');
    if(par_name == "TP HVRT RECOVER IN S") return get_P_recover_inertia_in_s('H');
    if(par_name == "KQ HVRT RECOVER IN PU PER S") return get_Q_recover_slope_in_pu_per_s('H');
    if(par_name == "TQ LVRT RECOVER IN S") return get_Q_recover_inertia_in_s('H');

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void WTVRT3::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    //lvrt
    if(par_name == "LVRT VOLTAGE THRESHOLD") return set_vrt_voltage_threshold('L', value);
    if(par_name == "LVRT PREPARE2RECOVER DELAY TIME") return set_vrt_prepare2recover_delay_time_in_s('L', value);
    if(par_name == "P LVRT DURING MODE")
    {
        VRT_DURING_CONTROL_MODE p_lvrt_during_mode;
        switch(int(value))
        {
            case 0:
                p_lvrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                break;
            case 1:
                p_lvrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                break;
            case 2:
                p_lvrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                break;
            default:
                p_lvrt_during_mode = VRT_DURING_LVPL_CURRENT_CONTROL_MODE;
                break;
        }
        return set_P_vrt_during_mode('L', p_lvrt_during_mode);
    }
    if(par_name == "Q LVRT DURING MODE")
    {
        VRT_DURING_CONTROL_MODE q_lvrt_during_mode;
        switch(int(value))
        {
            case 0:
                q_lvrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                break;
            case 1:
                q_lvrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                break;
            default:
                q_lvrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_during_mode('L', q_lvrt_during_mode);
    }
    if(par_name == "P LVRT PREPARE2RECOVER MODE")
    {
        VRT_PREPARE2RECOVER_CONTROL_MODE p_lvrt_prepare2recover_mode;
        switch(int(value))
        {
            case 0:
                p_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                break;
            case 1:
                p_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                break;
            default:
                p_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                break;
        }
        return set_P_vrt_prepare2recover_mode('L', p_lvrt_prepare2recover_mode);
    }

    if(par_name == "Q LVRT PREPARE2RECOVER MODE")
    {
        VRT_PREPARE2RECOVER_CONTROL_MODE q_lvrt_prepare2recover_mode;
        switch(int(value))
        {
            case 0:
                q_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                break;
            case 1:
                q_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                break;
            default:
                q_lvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_prepare2recover_mode('L', q_lvrt_prepare2recover_mode);
    }
    if(par_name == "P LVRT RECOVER MODE")
    {
        VRT_RECOVER_CONTROL_MODE p_lvrt_recover_mode;
        switch(int(value))
        {
            case 0:
                p_lvrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                break;
            default:
                p_lvrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                break;
        }
        return set_P_vrt_recover_mode('L', p_lvrt_recover_mode);
    }
    if(par_name == "Q LVRT RECOVER MODE")
    {
        VRT_RECOVER_CONTROL_MODE q_lvrt_recover_mode;
        switch(int(value))
        {
            case 0:
                q_lvrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                break;
            default:
                q_lvrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_recover_mode('L', q_lvrt_recover_mode);
    }

    if(par_name == "KP LVRT DURING") return  set_kP_vrt_during('L', value);
    if(par_name == "P SET LVRT DURING IN PU") return set_P_set_vrt_during_in_pu('L', value);
    if(par_name == "K1 IP LVRT DURING") return set_k1_Ip_vrt_during('L', value);
    if(par_name == "K2 IP LVRT DURING") return set_k2_Ip_vrt_during('L', value);
    if(par_name == "IP SET LVRT DURING IN PU") return set_Ip_set_vrt_during_in_pu('L', value);
    if(par_name == "LVPL LOW VOLTAGE") return set_LVPL_low_voltage_in_pu(value);
    if(par_name == "LVPL HIGH VOLTAGE") return set_LVPL_high_voltage_in_pu(value);
    if(par_name == "LVPL GAIN AT HIGH VOLTAGE") return set_LVPL_gain_at_high_voltage(value);
    if(par_name == "KQ LVRT DURING") return set_kQ_vrt_during('L', value);
    if(par_name == "Q SET LVRT DURING IN PU") return set_Q_set_vrt_during_in_pu('L', value);
    if(par_name == "K1 IQ LVRT DURING") return set_k1_Iq_vrt_during('L', value);
    if(par_name == "K2 IQ LVRT DURING") return set_k2_Iq_vrt_during('L', value);
    if(par_name == "IQ SET LVRT DURING") return set_Iq_set_vrt_during_in_pu('L', value);
    if(par_name == "K IP LVRT PREPARE2RECOVER") return set_k_Ip_prepare2recover('L', value);
    if(par_name == "IP SET LVRT PREPARE2RECOVER IN PU") return set_Ip_set_prepare2recover('L', value);
    if(par_name == "K IQ LVRT PREPARE2RECOVER") return set_k_Iq_prepare2recover('L', value);
    if(par_name == "IQ SET LVRT PREPARE2RECOVER IN PU") return set_Iq_set_prepare2recover('L', value);
    if(par_name == "KP LVRT RECOVER IN PU PER S") return set_P_recover_slope_in_pu_per_s('L', value);
    if(par_name == "TP LVRT RECOVER IN S") return set_P_recover_inertia_in_s('L', value);
    if(par_name == "KQ LVRT RECOVER IN PU PER S") return set_Q_recover_slope_in_pu_per_s('L', value);
    if(par_name == "TQ LVRT RECOVER IN S") return set_Q_recover_inertia_in_s('L', value);

    //hvrt
    if(par_name == "HVRT VOLTAGE THRESHOLD") return set_vrt_voltage_threshold('H', value);
    if(par_name == "HVRT PREPARE2RECOVER DELAY TIME") return set_vrt_prepare2recover_delay_time_in_s('H', value);

    if(par_name == "P HVRT DURING MODE")
    {
        VRT_DURING_CONTROL_MODE p_hvrt_during_mode;
        switch(int(value))
        {
            case 0:
                p_hvrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                break;
            case 1:
                p_hvrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                break;
            case 2:
                p_hvrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                break;
            default:
                p_hvrt_during_mode = VRT_DURING_LVPL_CURRENT_CONTROL_MODE;
                break;
        }
        return set_P_vrt_during_mode('H', p_hvrt_during_mode);
    }
    if(par_name == "Q HVRT DURING MODE")
    {
        VRT_DURING_CONTROL_MODE q_hvrt_during_mode;
        switch(int(value))
        {
            case 0:
                q_hvrt_during_mode = VRT_DURING_POWER_CONTROL_MODE;
                break;
            case 1:
                q_hvrt_during_mode = VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE;
                break;
            default:
                q_hvrt_during_mode = VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_during_mode('H', q_hvrt_during_mode);
    }
    if(par_name == "P HVRT PREPARE2RECOVER MODE")
    {
        VRT_PREPARE2RECOVER_CONTROL_MODE p_hvrt_prepare2recover_mode;
        switch(int(value))
        {
            case 0:
                p_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                break;
            case 1:
                p_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                break;
            default:
                p_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                break;
        }
        return set_P_vrt_prepare2recover_mode('H', p_hvrt_prepare2recover_mode);
    }

    if(par_name == "Q HVRT PREPARE2RECOVER MODE")
    {
        VRT_PREPARE2RECOVER_CONTROL_MODE q_hvrt_prepare2recover_mode;
        switch(int(value))
        {
            case 0:
                q_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE;
                break;
            case 1:
                q_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE;
                break;
            default:
                q_hvrt_prepare2recover_mode = VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_prepare2recover_mode('H', q_hvrt_prepare2recover_mode);
    }
    if(par_name == "P HVRT RECOVER MODE")
    {
        VRT_RECOVER_CONTROL_MODE p_hvrt_recover_mode;
        switch(int(value))
        {
            case 0:
                p_hvrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                break;
            default:
                p_hvrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                break;
        }
        return set_P_vrt_recover_mode('H', p_hvrt_recover_mode);
    }
    if(par_name == "Q HVRT RECOVER MODE")
    {
        VRT_RECOVER_CONTROL_MODE q_hvrt_recover_mode;
        switch(int(value))
        {
            case 0:
                q_hvrt_recover_mode = VRT_RECOVER_LINEAR_CONTROL_MODE;
                break;
            default:
                q_hvrt_recover_mode = VRT_RECOVER_EXPOENTIAL_CONTROL_MODE;
                break;
        }
        return set_Q_vrt_recover_mode('H', q_hvrt_recover_mode);
    }

    if(par_name == "KP HVRT DURING") return set_kP_vrt_during('H', value);
    if(par_name == "P SET HVRT DURING IN PU") return set_P_set_vrt_during_in_pu('H', value);
    if(par_name == "K1 IP HVRT DURING") return set_k1_Ip_vrt_during('H', value);
    if(par_name == "K2 IP HVRT DURING") return set_k2_Ip_vrt_during('H', value);
    if(par_name == "IP SET HVRT DURING IN PU") return set_Ip_set_vrt_during_in_pu('H', value);
    if(par_name == "KQ HVRT DURING") return set_kQ_vrt_during('H', value);
    if(par_name == "Q SET HVRT DURING IN PU") return set_Q_set_vrt_during_in_pu('H', value);
    if(par_name == "K1 IQ HVRT DURING") return set_k1_Iq_vrt_during('H', value);
    if(par_name == "K2 IQ HVRT DURING") return set_k2_Iq_vrt_during('H', value);
    if(par_name == "IQ SET HVRT DURING") return set_Iq_set_vrt_during_in_pu('H', value);
    if(par_name == "K IP HVRT PREPARE2RECOVER") return set_k_Ip_prepare2recover('H', value);
    if(par_name == "IP SET HVRT PREPARE2RECOVER IN PU") return set_Ip_set_prepare2recover('H', value);
    if(par_name == "K IQ HVRT PREPARE2RECOVER") return set_k_Iq_prepare2recover('H', value);
    if(par_name == "IQ SET HVRT PREPARE2RECOVER IN PU") return set_Iq_set_prepare2recover('H', value);
    if(par_name == "KP HVRT RECOVER IN PU PER S") return set_P_recover_slope_in_pu_per_s('H', value);
    if(par_name == "TP HVRT RECOVER IN S") return set_P_recover_inertia_in_s('H', value);
    if(par_name == "KQ HVRT RECOVER IN PU PER S") return set_Q_recover_slope_in_pu_per_s('H', value);
    if(par_name == "TQ LVRT RECOVER IN S") return set_Q_recover_inertia_in_s('H', value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double WTVRT3::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void WTVRT3::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@LVRT STATUS", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@HVRT STATUS", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@VRT ACTIVE POWER COMMAND IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@VRT REACTIVE POWER COMMAND IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@VRT ACTIVE CURRENT COMMAND IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@VRT REACTIVE CURRENT COMMAND IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@VRT REACTIVE VOLTAGE COMMAND IN PU", i); i++;
}

double WTVRT3::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "STATE@LVRT STATUS")
    {
        VRT_STATUS lvrt_status = lvrt.get_vrt_status();
        switch(lvrt_status)
        {
            case VRT_NORMAL_STATUS:
                return double(0);
            case VRT_DURING_STATUS:
                return double(1);
            case VRT_PREPARE2RECOVER_STATUS:
                return double(2);
            default:
                return double(3);
        }
    }
	if(var_name == "STATE@HVRT STATUS")
    {
        VRT_STATUS hvrt_status = hvrt.get_vrt_status();
        switch(hvrt_status)
        {
            case VRT_NORMAL_STATUS:
                return double(0);
            case VRT_DURING_STATUS:
                return double(1);
            case VRT_PREPARE2RECOVER_STATUS:
                return double(2);
            default:
                return double(3);
        }
    }
	if(var_name == "OUTPUT@VRT ACTIVE POWER COMMAND IN PU") return get_active_power_command_in_pu_based_on_mbase();
	if(var_name == "OUTPUT@VRT REACTIVE POWER COMMAND IN PU") return get_reactive_power_command_in_pu_based_on_mbase();
	if(var_name == "OUTPUT@VRT ACTIVE CURRENT COMMAND IN PU") return get_active_current_command_in_pu_based_on_mbase();
	if(var_name == "OUTPUT@VRT REACTIVE CURRENT COMMAND IN PU") return get_reactive_current_command_in_pu_based_on_mbase();
	if(var_name == "OUTPUT@VRT REACTIVE VOLTAGE COMMAND IN PU") return get_reactive_voltage_command_in_pu();
    return 0.0;
}

string WTVRT3::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string WTVRT3::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WTVRT3::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
