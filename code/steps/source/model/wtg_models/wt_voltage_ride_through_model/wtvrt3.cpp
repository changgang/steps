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
    set_model_float_parameter_count(35);

}

void WTVRT3::copy_from_const_model(const WTVRT3& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

string WTVRT3::get_model_name() const
{
    return "WTVRT3";
}

void WTVRT3::set_vrt_voltage_threshold(char HL_flag, double v)
{
    if(HL_flag=='L') return lvrt.set_vrt_voltage_threshold();
    else             return hvrt.set_vrt_voltage_threshold();
}

void WTVRT3::set_vrt_prepare2recover_delay_time_in_s(char HL_flag, double t)
{
    if(HL_flag=='L') return lvrt.set_vrt_prepare2recover_delay_time_in_s();
    else             return hvrt.set_vrt_prepare2recover_delay_time_in_s();
}


void WTVRT3::set_P_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_P_vrt_during_mode();
    else             return hvrt.set_P_vrt_during_mode();
}

void WTVRT3::set_kP(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_kP();
    else             return hvrt.set_kP();
}

void WTVRT3::set_P_set_in_pu(char HL_flag, double p)
{
    if(HL_flag=='L') return lvrt.set_P_set_in_pu();
    else             return hvrt.set_P_set_in_pu();
}

void WTVRT3::set_k1_Ip(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k1_Ip();
    else             return hvrt.set_k1_Ip();
}

void WTVRT3::set_k2_Ip(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.set_k2_Ip();
    else             return hvrt.set_k2_Ip();
}

void WTVRT3::set_Ip_set_in_pu(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.set_Ip_set_in_pu();
    else             return hvrt.set_Ip_set_in_pu();
}

void WTVRT3::set_LVPL_low_voltage_in_pu(double v)
{
    lvrt.set_LVPL_low_voltage_in_pu();
}

void WTVRT3::set_LVPL_high_voltage_in_pu(double v)
{
    lvrt.set_LVPL_high_voltage_in_pu();
}

void WTVRT3::set_LVPL_gain_at_high_voltage(double g)
{
    lvrt.set_LVPL_gain_at_high_voltage();
}


void WTVRT3::set_P_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.set_P_vrt_prepare2recover_mode();
    else             return hvrt.set_P_vrt_prepare2recover_mode();
}

void WTVRT3::set_k_Ip_prepare2recover(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Ip_set_prepare2recover(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


void WTVRT3::set_P_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_P_recover_slope_in_pu_per_s(char HL_flag, double s)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_P_recover_inertia_in_s(char HL_flag, double T)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


void WTVRT3::set_Q_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_kQ(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Q_set_in_pu(char HL_flag, double q)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_k1_Iq(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_k2_Iq(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Iq_set_in_pu(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


void WTVRT3::set_Q_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_k_Iq_prepare2recover(char HL_flag, double k)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Iq_set_prepare2recover(char HL_flag, double I)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


void WTVRT3::set_Q_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Q_recover_slope_in_pu_per_s(char HL_flag, double s)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

void WTVRT3::set_Q_recover_inertia_in_s(char HL_flag, double T)
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


double WTVRT3::get_vrt_voltage_threshold(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_vrt_prepare2recover_delay_time_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


VRT_DURING_CONTROL_MODE WTVRT3::get_P_vrt_during_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_kP(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_P_set_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k1_Ip(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k2_Ip(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Ip_set_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_LVPL_low_voltage_in_pu() const
{
    return lvrt.();
}

double WTVRT3::get_LVPL_high_voltage_in_pu() const
{
    return lvrt.();
}

double WTVRT3::get_LVPL_gain_at_high_voltage() const
{
    return lvrt.();
}


VRT_PREPARE2RECOVER_CONTROL_MODE WTVRT3::get_P_vrt_prepare2recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k_Ip_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Ip_set_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


VRT_RECOVER_CONTROL_MODE WTVRT3::get_P_vrt_recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_P_recover_slope_in_pu_per_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_P_recover_inertia_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


VRT_DURING_CONTROL_MODE WTVRT3::get_Q_vrt_during_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_kQ(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Q_set_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k1_Iq(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k2_Iq(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Iq_set_in_pu(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


VRT_PREPARE2RECOVER_CONTROL_MODE WTVRT3::get_Q_vrt_prepare2recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_k_Iq_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Iq_set_prepare2recover(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}


VRT_RECOVER_CONTROL_MODE WTVRT3::get_Q_vrt_recover_mode(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Q_recover_slope_in_pu_per_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

double WTVRT3::get_Q_recover_inertia_in_s(char HL_flag) const
{
    if(HL_flag=='L') return lvrt.();
    else             return hvrt.();
}

bool WTVRT3::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=38)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int bus, voltage_flag;
            int var_control_flag;
            double tfv, kpv, kiv, xc, tfp, kpp, kip, pmax, pmin, qmax, qmin,
                   ipmax, trv, rpmax, rpmin, tspeed, kqi, vmax, vmin,
                   kqv, eqmax, eqmin, tv, tp, fn,
                   kvi, tvi, kdroop, tdroop, fupper, flower, kint;

            unsigned int i=3;
            bus = get_integer_data(data[i],"0"); i++;
            var_control_flag = get_integer_data(data[i],"0"); i++;
            voltage_flag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            xc = get_double_data(data[i],"0.0"); i++;
            trv = get_double_data(data[i],"0.0"); i++;
            fn = get_double_data(data[i],"0.0"); i++;
            kpv = get_double_data(data[i],"0.0"); i++;
            tv = get_double_data(data[i],"0.0"); i++;
            kiv = get_double_data(data[i],"0.0"); i++;
            qmin = get_double_data(data[i],"0.0"); i++;
            qmax = get_double_data(data[i],"0.0"); i++;
            tfv = get_double_data(data[i],"0.0"); i++;
            tp = get_double_data(data[i],"0.0"); i++;
            kqi = get_double_data(data[i],"0.0"); i++;
            vmin = get_double_data(data[i],"0.0"); i++;
            vmax = get_double_data(data[i],"0.0"); i++;
            kqv = get_double_data(data[i],"0.0"); i++;
            eqmin = get_double_data(data[i],"0.0"); i++;
            eqmax = get_double_data(data[i],"0.0"); i++;
            tspeed = get_double_data(data[i],"0.0"); i++;
            kpp = get_double_data(data[i],"0.0"); i++;
            kip = get_double_data(data[i],"0.0"); i++;
            kvi = get_double_data(data[i],"0.0"); i++;
            tvi = get_double_data(data[i],"0.0"); i++;
            kdroop = get_double_data(data[i],"0.0"); i++;
            tdroop = get_double_data(data[i],"0.0"); i++;
            flower = get_double_data(data[i],"0.0"); i++;
            fupper = get_double_data(data[i],"0.0"); i++;
            kint = get_double_data(data[i],"0.0"); i++;
            rpmin = get_double_data(data[i],"0.0"); i++;
            rpmax = get_double_data(data[i],"0.0"); i++;
            tfp = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            ipmax = get_double_data(data[i],"0.0");

            set_bus_to_regulate(bus);
            PE_VAR_CONTROL_MODE mode;
            switch(var_control_flag)
            {
                case 0:
                {
                    mode = CONSTANT_VAR_MODE;
                    break;
                }
                case 1:
                {
                    mode = CONSTANT_VOLTAGE_MODE;
                    break;
                }
                case -1:
                {
                    mode = CONSTANT_POWER_FACTOR_MODE;
                    break;
                }
                default:
                {
                    mode = CONSTANT_VAR_MODE;
                    break;
                }
            }

            set_var_control_mode(mode);
            set_Xcomp_in_pu(xc);
            set_TRV_in_s(trv);
            set_Fn(fn);
            set_KPV(kpv);
            set_TV_in_s(tv);
            set_KIV(kiv);
            set_Qmin_in_pu(qmin);
            set_Qmax_in_pu(qmax);
            set_TFV_in_s(tfv);
            set_TP_in_s(tp);
            set_KQI(kqi);
            set_Vmin_in_pu(vmin);
            set_Vmax_in_pu(vmax);

            if(voltage_flag>2)
                voltage_flag = 2;
            set_voltage_flag(voltage_flag);
            set_KQV(kqv);
            set_EQmin_in_pu(eqmin);
            set_EQmax_in_pu(eqmax);
            set_Tspeed_in_s(tspeed);
            set_KPP(kpp);
            set_KIP(kip);
            set_Kvi(kvi);
            set_Tvi_in_s(tvi);
            set_Kdroop(kdroop);
            set_Tdroop_in_s(tdroop);
            set_frequency_deviation_lower_deadband_in_pu(flower);
            set_frequency_deviation_upper_deadband_in_pu(fupper);
            set_Kfint(kint);
            set_rPmin_in_pu(rpmin);
            set_rPmax_in_pu(rpmax);
            set_TFP_in_s(tfp);
            set_Pmin_in_pu(pmin);
            set_Pmax_in_pu(pmax);
            set_IPmax_in_pu(ipmax);

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
        lvrt.update_PQI0_in_vrt_normal_status(P0, Q0, Ip0, Iq0);

        hvrt.set_vrt_status(VRT_NORMAL_STATUS);
        hvrt.update_PQI0_in_vrt_normal_status(P0, Q0, Ip0, Iq0);

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
        lvrt.check_vrt_status();
        hvrt.check_vrt_status();

        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        WT_ELECTRICAL_MODEL* wtelecmodel = wt_generator->get_wt_electrical_model();

        double P0, Q0, Ip0, Iq0;

        switch(lvrt.get_vrt_status()
        {
            case VRT_NORMAL_STATUS:
                P0 = wtelecmodel->get_active_power_command_in_pu_based_on_mbase();
                Q0 = wtelecmodel->get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = wtelecmodel->get_active_current_command_in_pu_based_on_mbase();
                Iq0 = wtelecmodel->get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_normal_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_DURING_STATUS:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_PREPARE2RECOVER_STATUS:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_prepare2recover_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_RECOVER_STATUS:
            default:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                lvrt.update_PQI0_in_vrt_recover_status(P0, Q0, Ip0, Iq0);
                break;
        }
        switch(hvrt.get_vrt_status()
        {
            case VRT_NORMAL_STATUS:
                P0 = wtelecmodel->get_active_power_command_in_pu_based_on_mbase();
                Q0 = wtelecmodel->get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = wtelecmodel->get_active_current_command_in_pu_based_on_mbase();
                Iq0 = wtelecmodel->get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_normal_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_DURING_STATUS:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_during_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_PREPARE2RECOVER_STATUS:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_prepare2recover_status(P0, Q0, Ip0, Iq0);
                break;
            case VRT_RECOVER_STATUS:
            default:
                P0 = get_active_power_command_in_pu_based_on_mbase();
                Q0 = get_reactive_power_command_in_pu_based_on_mbase();
                Ip0 = get_active_current_command_in_pu_based_on_mbase();
                Iq0 = get_reactive_current_command_in_pu_based_on_mbase();
                hvrt.update_PQI0_in_vrt_recover_status(P0, Q0, Ip0, Iq0);
                break;
        }
        set_flag_model_updated_as_true();
    }
}


double WTVRT3::get_active_current_command_in_pu_based_on_mbase()
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;

    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    return pcmd/vterm;
}

double WTVRT3::get_active_power_command_in_pu_based_on_mbase() const
{
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_P_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_P_command();
}

double WTVRT3::get_reactive_current_command_in_pu_based_on_mbase()
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;

    double qcmd = get_reactive_power_command_in_pu_based_on_mbase();
    return qcmd/vterm;
}

double WTVRT3::get_reactive_power_command_in_pu_based_on_mbase()
{
    if(lvrt.get_vrt_status()==VRT_NORMAL_STATUS and hvrt.get_vrt_status()==VRT_NORMAL_STATUS)
        return 0.0;
    if(lvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return lvrt.get_vrt_Q_command();
    if(hvrt.get_vrt_status()!=VRT_NORMAL_STATUS)
        return hvrt.get_vrt_Q_command();
}

double WTVRT3::get_reactive_voltage_command_in_pu() const
{
    return 0.0;
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

    unsigned int bus_reg = get_bus_to_regulate();
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
    int var_mode = (mode==CONSTANT_VAR_MODE)? 0: (mode==CONSTANT_POWER_FACTOR_MODE? -1 : 1);
    unsigned int voltage_flag = get_voltage_flag();
    double xc = get_Xcomp_in_pu();
    double trv = get_TRV_in_s();
    double fn = get_Fn();
    double kpv = get_KPV();
    double tv = get_TV_in_s();
    double kiv = get_KIV();
    double qmax = get_Qmax_in_pu();
    double qmin = get_Qmin_in_pu();
    double tfv = get_TFV_in_s();
    double tp = get_TP_in_s();
    double kqi = get_KQI();
    double vmin = get_Vmin_in_pu();
    double vmax = get_Vmax_in_pu();
    double kqv = get_KQV();
    double eqmin = get_EQmin_in_pu();
    double eqmax = get_EQmax_in_pu();
    double tspeed = get_Tspeed_in_s();
    double kpp = get_KPP();
    double kip = get_KIP();
    double kvi = get_Kvi();
    double tvi = get_Tvi_in_s();
    double kdroop = get_Kdroop();
    double tdroop = get_Tdroop_in_s();
    double flower = get_frequency_deviation_lower_deadband_in_pu();
    double fupper = get_frequency_deviation_upper_deadband_in_pu();
    double kint = get_Kfint();
    double rpmin = get_rPmin_in_pu();
    double rpmax = get_rPmax_in_pu();
    double tfp = get_TFP_in_s();
    double pmin = get_Pmin_in_pu();
    double pmax = get_Pmax_in_pu();
    double ipmax = get_IPmax_in_pu();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
    {
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;
        if(bus_reg!=0) bus_reg = network.get_internal_bus_number_of_physical_bus(bus_reg)+1;
    }

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<bus_reg<<", "
            <<setw(8)<<var_mode<<", "
            <<setw(8)<<voltage_flag<<", "
            <<setw(8)<<setprecision(6)<<xc<<", "
            <<setw(8)<<setprecision(6)<<trv<<", "
            <<setw(8)<<setprecision(6)<<fn<<", "
            <<setw(8)<<setprecision(6)<<kpv<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<tv<<", "
            <<setw(8)<<setprecision(6)<<kiv<<", "
            <<setw(8)<<setprecision(6)<<qmin<<", "
            <<setw(8)<<setprecision(6)<<qmax<<", "
            <<setw(8)<<setprecision(6)<<tfv<<", "
            <<setw(8)<<setprecision(6)<<tp<<", "
            <<setw(8)<<setprecision(6)<<kqi<<", "
            <<setw(8)<<setprecision(6)<<vmin<<", "
            <<setw(8)<<setprecision(6)<<vmax<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kqv<<", "
            <<setw(8)<<setprecision(6)<<eqmin<<", "
            <<setw(8)<<setprecision(6)<<eqmax<<", "
            <<setw(8)<<setprecision(6)<<tspeed<<", "
            <<setw(8)<<setprecision(6)<<kpp<<", "
            <<setw(8)<<setprecision(6)<<kip<<", "
            <<setw(8)<<setprecision(6)<<kvi<<", "
            <<setw(8)<<setprecision(6)<<tvi<<", "
            <<setw(8)<<setprecision(6)<<kdroop<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<tdroop<<", "
            <<setw(8)<<setprecision(6)<<flower<<", "
            <<setw(8)<<setprecision(6)<<fupper<<", "
            <<setw(8)<<setprecision(6)<<kint<<", "
            <<setw(8)<<setprecision(6)<<rpmin<<", "
            <<setw(8)<<setprecision(6)<<rpmax<<", "
            <<setw(8)<<setprecision(6)<<tfp<<", "
            <<setw(8)<<setprecision(6)<<pmin<<", "
            <<setw(8)<<setprecision(6)<<pmax<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<ipmax<<" / ";

    return osstream.str();
}

void WTVRT3::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("BUS TO REGULATE", i); i++;
    add_model_data_name_and_index_pair("VAR CONTROL FLAG", i); i++;
    add_model_data_name_and_index_pair("VOLTAGE FLAG", i); i++;
    add_model_data_name_and_index_pair("XCOMP IN PU", i); i++;
    add_model_data_name_and_index_pair("T VOLTAGE SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("FN", i); i++;
    add_model_data_name_and_index_pair("KP VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("TP VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("KI VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("QMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("QMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("T REACTIVE POWER FILTER IN S", i); i++;
    add_model_data_name_and_index_pair("T ACTIVE POWER SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("KI REACTIVE POWER ERROR", i); i++;
    add_model_data_name_and_index_pair("VMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("VMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("KI VOLTAGE COMMAND", i); i++;
    add_model_data_name_and_index_pair("EQMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("EQMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("T SPEED SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("KP SPEED", i); i++;
    add_model_data_name_and_index_pair("KI SPEED", i); i++;
    add_model_data_name_and_index_pair("K VIRTUAL INERTIA", i); i++;
    add_model_data_name_and_index_pair("T VIRTUAL INERTIA IN S", i); i++;
    add_model_data_name_and_index_pair("K FREQUENCY DROOP", i); i++;
    add_model_data_name_and_index_pair("T FREQUENCY DROOP IN S", i); i++;
    add_model_data_name_and_index_pair("F LOWER IN PU", i); i++;
    add_model_data_name_and_index_pair("F UPPER IN PU", i); i++;
    add_model_data_name_and_index_pair("K SECONDARY FREQUENCY REGULATION", i); i++;
    add_model_data_name_and_index_pair("P RATE MIN IN PU/S", i); i++;
    add_model_data_name_and_index_pair("P RATE MAX IN PU/S", i); i++;
    add_model_data_name_and_index_pair("T ACTIVE POWER COMMAND IN S", i); i++;
    add_model_data_name_and_index_pair("PMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("PMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("ACTIVE CURRENT MAX IN PU", i); i++;
}

double WTVRT3::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "BUS TO REGULATE")  return get_bus_to_regulate();
    if(par_name == "VAR CONTROL FLAG") return get_var_control_mode();
    if(par_name == "VOLTAGE FLAG")     return get_voltage_flag();
    if(par_name == "XCOMP IN PU")           return get_Xcomp_in_pu();
    if(par_name == "T VOLTAGE SENSOR IN S") return get_TRV_in_s();
    if(par_name == "FN")                    return get_Fn();
    if(par_name == "KP VOLTAGE ERROR")      return get_KPV();
    if(par_name == "TP VOLTAGE ERROR")      return get_TV_in_s();
    if(par_name == "KI VOLTAGE ERROR")      return get_KIV();
    if(par_name == "QMIN IN PU")            return get_Qmin_in_pu();
    if(par_name == "QMAX IN PU")            return get_Qmax_in_pu();
    if(par_name == "T REACTIVE POWER FILTER IN S")  return get_TFV_in_s();
    if(par_name == "T ACTIVE POWER SENSOR IN S")    return get_TP_in_s();
    if(par_name == "KI REACTIVE POWER ERROR")       return get_KQI();
    if(par_name == "VMIN IN PU")                    return get_Vmin_in_pu();
    if(par_name == "VMAX IN PU")                    return get_Vmax_in_pu();
    if(par_name == "KI VOLTAGE COMMAND")            return get_KQV();
    if(par_name == "EQMIN IN PU")                   return get_EQmin_in_pu();
    if(par_name == "EQMAX IN PU")                   return get_EQmax_in_pu();
    if(par_name == "T SPEED SENSOR IN S")           return get_Tspeed_in_s();
    if(par_name == "KP SPEED")                      return get_KPP();
    if(par_name == "KI SPEED")                      return get_KIP();
    if(par_name == "K VIRTUAL INERTIA")             return get_Kvi();
    if(par_name == "T VIRTUAL INERTIA IN S")        return get_Tvi_in_s();
    if(par_name == "K FREQUENCY DROOP")             return get_Kdroop();
    if(par_name == "T FREQUENCY DROOP IN S")        return get_Tdroop_in_s();
    if(par_name == "F LOWER IN PU")                 return get_frequency_deviation_lower_deadband_in_pu();
    if(par_name == "F UPPER IN PU")                 return get_frequency_deviation_upper_deadband_in_pu();
    if(par_name == "K SECONDARY FREQUENCY REGULATION") return get_Kfint();
    if(par_name == "P RATE MIN IN PU/S")               return get_rPmin_in_pu();
    if(par_name == "P RATE MAX IN PU/S")               return get_rPmax_in_pu();
    if(par_name == "T ACTIVE POWER COMMAND IN S")      return get_TFP_in_s();
    if(par_name == "PMIN IN PU")                       return get_Pmin_in_pu();
    if(par_name == "PMAX IN PU")                       return get_Pmax_in_pu();
    if(par_name == "ACTIVE CURRENT MAX IN PU")         return get_IPmax_in_pu();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void WTVRT3::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "BUS TO REGULATE") return set_bus_to_regulate((unsigned int)(value));

    if(par_name == "VAR CONTROL FLAG")
    {
        int ivalue = int(value);
        PE_VAR_CONTROL_MODE mode = CONSTANT_VAR_MODE;
        switch(ivalue)
        {
            case 0:
                mode = CONSTANT_VAR_MODE;
                break;
            case 1:
                mode = CONSTANT_VOLTAGE_MODE;
                break;
            case -1:
                mode = CONSTANT_POWER_FACTOR_MODE;
                break;
            default:
                mode = CONSTANT_VAR_MODE;
                break;
        }
        return set_var_control_mode(mode);
    }

    if(par_name == "VOLTAGE FLAG")
    {
        unsigned int flag = (unsigned int)(value);
        return set_voltage_flag(flag);
    }
    if(par_name == "XCOMP IN PU")           return set_Xcomp_in_pu(value);
    if(par_name == "T VOLTAGE SENSOR IN S") return set_TRV_in_s(value);
    if(par_name == "FN")                    return set_Fn(value);
    if(par_name == "KP VOLTAGE ERROR")      return set_KPV(value);
    if(par_name == "TP VOLTAGE ERROR")      return set_TV_in_s(value);
    if(par_name == "KI VOLTAGE ERROR")      return set_KIV(value);
    if(par_name == "QMIN IN PU")            return set_Qmin_in_pu(value);
    if(par_name == "QMAX IN PU")            return set_Qmax_in_pu(value);
    if(par_name == "T REACTIVE POWER FILTER IN S")  return set_TFV_in_s(value);
    if(par_name == "T ACTIVE POWER SENSOR IN S")    return set_TP_in_s(value);
    if(par_name == "KI REACTIVE POWER ERROR")       return set_KQI(value);
    if(par_name == "VMIN IN PU")                    return set_Vmin_in_pu(value);
    if(par_name == "VMAX IN PU")                    return set_Vmax_in_pu(value);
    if(par_name == "KI VOLTAGE COMMAND")            return set_KQV(value);
    if(par_name == "EQMIN IN PU")                   return set_EQmin_in_pu(value);
    if(par_name == "EQMAX IN PU")                   return set_EQmax_in_pu(value);
    if(par_name == "T SPEED SENSOR IN S")           return set_Tspeed_in_s(value);
    if(par_name == "KP SPEED")                      return set_KPP(value);
    if(par_name == "KI SPEED")                      return set_KIP(value);
    if(par_name == "K VIRTUAL INERTIA")             return set_Kvi(value);
    if(par_name == "T VIRTUAL INERTIA IN S")        return set_Tvi_in_s(value);
    if(par_name == "K FREQUENCY DROOP")             return set_Kdroop(value);
    if(par_name == "T FREQUENCY DROOP IN S")        return set_Tdroop_in_s(value);
    if(par_name == "F LOWER IN PU")                 return set_frequency_deviation_lower_deadband_in_pu(value);
    if(par_name == "F UPPER IN PU")                 return set_frequency_deviation_upper_deadband_in_pu(value);
    if(par_name == "K SECONDARY FREQUENCY REGULATION") return set_Kfint(value);
    if(par_name == "P RATE MIN IN PU/S")               return set_rPmin_in_pu(value);
    if(par_name == "P RATE MAX IN PU/S")               return set_rPmax_in_pu(value);
    if(par_name == "T ACTIVE POWER COMMAND IN S")      return set_TFP_in_s(value);
    if(par_name == "PMIN IN PU")                       return set_Pmin_in_pu(value);
    if(par_name == "PMAX IN PU")                       return set_Pmax_in_pu(value);
    if(par_name == "ACTIVE CURRENT MAX IN PU")         return set_IPmax_in_pu(value);

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
    add_model_internal_variable_name_and_index_pair("STATE@SPEED REFERENCE SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@TORQUE REGULATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL INERTIA CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PRIMARY FREQUENCY CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SECONDARY FREQUENCY CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL INERTIA CONTROL COMMAND", i); i++;
    add_model_internal_variable_name_and_index_pair("PRIMARY FREQUENCY CONTROL COMMAND", i); i++;
    add_model_internal_variable_name_and_index_pair("SECONDARY FREQUENCY CONTROL COMMAND", i); i++;
}

double WTVRT3::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "STATE@SPEED REFERENCE SENSOR") return wind_turbine_speed_reference_sensor.get_state();
	if(var_name == "STATE@TORQUE REGULATOR") return torque_PI_regulator.get_state();
	if(var_name == "STATE@VIRTUAL INERTIA CONTROL") return virtual_inertia_emulator.get_state();
	if(var_name == "STATE@PRIMARY FREQUENCY CONTROL") return frequency_droop_controller.get_state();
	if(var_name == "STATE@SECONDARY FREQUENCY CONTROL") return frequency_integral_controller.get_state();
	if(var_name == "VIRTUAL INERTIA CONTROL COMMAND") return virtual_inertia_emulator.get_output();
	if(var_name == "PRIMARY FREQUENCY CONTROL COMMAND") return frequency_droop_controller.get_output();
	if(var_name == "SECONDARY FREQUENCY CONTROL COMMAND") return frequency_integral_controller.get_output();
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
