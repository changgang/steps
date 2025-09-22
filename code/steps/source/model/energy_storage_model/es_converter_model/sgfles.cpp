#include "header/model/energy_storage_model/es_converter_model/sgfles.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

SGFLES::SGFLES(STEPS& toolkit) : ES_CONVERTER_MODEL(toolkit),
                               csconv3(toolkit),
                               frequency_controller(toolkit),
                               active_power_regulator_filter(toolkit),
                               reactive_current_regulator_filter(toolkit),
                               voltage_regulator_integrator(toolkit)
{
    clear();
}

SGFLES::~SGFLES()
{
}

void SGFLES::clear()
{
    set_model_float_parameter_count(16);
    set_current_source_flag(true);
    csconv3.clear();

    frequency_controller.clear();
    frequency_controller.set_Kfint(0.0);

    unbypass_model();
}

void SGFLES::copy_from_const_model(const SGFLES& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    csconv3.set_toolkit(toolkit);
    frequency_controller.set_toolkit(toolkit);
    active_power_regulator_filter.set_toolkit(toolkit);
    reactive_current_regulator_filter.set_toolkit(toolkit);
    voltage_regulator_integrator.set_toolkit(toolkit);

    clear();

    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

SGFLES::SGFLES(const SGFLES& model):ES_CONVERTER_MODEL(model.get_toolkit()),
                                                           csconv3(model.get_toolkit()),
                                                           frequency_controller(model.get_toolkit()),
                                                           active_power_regulator_filter(model.get_toolkit()),
                                                           reactive_current_regulator_filter(model.get_toolkit()),
                                                           voltage_regulator_integrator(model.get_toolkit())
{
    copy_from_const_model(model);
}

SGFLES& SGFLES::operator=(const SGFLES& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void SGFLES::set_KPLL(double K)
{
    csconv3.set_KPLL(K);
}

void SGFLES::set_KIPLL(double K)
{
    csconv3.set_KIPLL(K);
}

void SGFLES::set_PLLmax(double pmax)
{
    csconv3.set_PLLmax(pmax);
}

void SGFLES::set_Imax_in_pu_based_on_mbase(double Imax)
{
    this->Imax = Imax;
}

void SGFLES::set_Kvi(double K)
{
    frequency_controller.set_Kvi(K);
}

void SGFLES::set_Tvi_in_s(double T)
{
    frequency_controller.set_Tvi_in_s(T);
}

void SGFLES::set_Kdroop(double K)
{
    frequency_controller.set_Kdroop(K);
}

void SGFLES::set_Tdroop_in_s(double T)
{
    frequency_controller.set_Tdroop_in_s(T);
}

void SGFLES::set_Tpcmd(double T)
{
    active_power_regulator_filter.set_T_in_s(T);
}

void SGFLES::set_Tv(double Tv)
{
    voltage_regulator_integrator.set_T_in_s(Tv);
}

void SGFLES::set_Tqcmd(double Tqcmd)
{
    reactive_current_regulator_filter.set_T_in_s(Tqcmd);
}

void SGFLES::set_lvrt_voltage_threshold(double v)
{
    this->lvrt_voltage_threshold = v;
}

void SGFLES::set_k_Ip_lvrt_during(double k)
{
    this->k_Ip_lvrt_during = k;
}

void SGFLES::set_Ipset_lvrt_during(double I)
{
    this->Ipset_lvrt_during = I;
}

void SGFLES::set_slope_Ip_lvrt_recover(double slope)
{
    this->slope_Ip_lvrt_recover = slope;
}

void SGFLES::set_k_Iq_lvrt_during(double k)
{
    this->k_Iq_lvrt_during = k;
}

void SGFLES::set_Iqset_lvrt_during(double I)
{
    this->Iqset_lvrt_during = I;
}

void SGFLES::set_slope_Iq_lvrt_recover(double slope)
{
    this->slope_Iq_lvrt_recover = slope;
}

double SGFLES::get_KPLL() const
{
    return csconv3.get_KPLL();
}

double SGFLES::get_KIPLL() const
{
    return csconv3.get_KIPLL();
}

double SGFLES::get_PLLmax() const
{
    return csconv3.get_PLLmax();
}

double SGFLES::get_Imax_in_pu_based_on_mbase() const
{
    return Imax;
}

double SGFLES::get_Kvi() const
{
    return frequency_controller.get_Kvi();
}

double SGFLES::get_Tvi_in_s() const
{
    return frequency_controller.get_Tvi_in_s();
}

double SGFLES::get_Kdroop() const
{
    return frequency_controller.get_Kdroop();
}

double SGFLES::get_Tdroop_in_s() const
{
    return frequency_controller.get_Tdroop_in_s();
}

double SGFLES::get_Tpcmd() const
{
    return active_power_regulator_filter.get_T_in_s();
}

double SGFLES::get_Tv() const
{
    return voltage_regulator_integrator.get_T_in_s();
}

double SGFLES::get_Tqcmd() const
{
    return reactive_current_regulator_filter.get_T_in_s();
}

double SGFLES::get_lvrt_voltage_threshold() const
{
    return lvrt_voltage_threshold;
}

double SGFLES::get_k_Ip_lvrt_during() const
{
    return k_Ip_lvrt_during;
}

double SGFLES::get_Ipset_lvrt_during() const
{
    return Ipset_lvrt_during;
}

double SGFLES::get_slope_Ip_lvrt_recover() const
{
    return slope_Ip_lvrt_recover;
}

double SGFLES::get_k_Iq_lvrt_during() const
{
    return k_Iq_lvrt_during;
}

double SGFLES::get_Iqset_lvrt_during() const
{
    return Iqset_lvrt_during;
}

double SGFLES::get_slope_Iq_lvrt_recover() const
{
    return slope_Iq_lvrt_recover;
}

double SGFLES::get_Ipcmd_in_pu_based_on_mbase()
{
    double Ipcmd;

    if(not is_elec_control_bypassed())
    {
        Ipcmd = active_power_regulator_filter.get_output() / get_bus_voltage_in_pu();
        if (Ipcmd > get_Imax_in_pu_based_on_mbase())
            Ipcmd = get_Imax_in_pu_based_on_mbase();
        else
        {
            if(Ipcmd < 0.0)
                Ipcmd = 0.0;
        }
    }
    else
    {
        Ipcmd = get_lvrt_IP_command();
        double value = pow(get_Imax_in_pu_based_on_mbase(),2) - pow(get_Iqcmd_in_pu_based_on_mbase(),2);
        double Ipmax = 0.0;
        if(value < 0)
            Ipmax = 0;
        else
            Ipmax = steps_sqrt(value);
        if(Ipcmd > Ipmax)
            Ipcmd = Ipmax;
        if(Ipcmd < 0)
            Ipcmd = 0;
    }
    return Ipcmd;
}

double SGFLES::get_Iqcmd_in_pu_based_on_mbase()
{


    double Iqcmd;
    if(not is_elec_control_bypassed())
    {
        Iqcmd =  - reactive_current_regulator_filter.get_output();
        double value = pow(get_Imax_in_pu_based_on_mbase(),2) - pow(get_Ipcmd_in_pu_based_on_mbase(),2);
        double Iqmax = 0.0;
        if(value < 0)
            Iqmax = 0;
        else
            Iqmax = steps_sqrt(value);
        if(Iqcmd > Iqmax)
            Iqcmd = Iqmax;
        if(Iqcmd < -Iqmax)
            Iqcmd = -Iqmax;
    }
    else
    {
        Iqcmd = get_lvrt_IQ_command();
        if (Iqcmd > get_Imax_in_pu_based_on_mbase())
            Iqcmd = get_Imax_in_pu_based_on_mbase();
        else
        {
            if(Iqcmd < -get_Imax_in_pu_based_on_mbase())
                Iqcmd = -get_Imax_in_pu_based_on_mbase();
        }
    }
    return Iqcmd;
}

double SGFLES::get_bus_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

void SGFLES::set_voltage_reference_in_pu(double Vref)
{
    this->Vref = Vref;
}

double SGFLES::get_voltage_reference_in_pu() const
{
    return Vref;
}

void SGFLES::unbypass_model()
{
    elec_control_bypassed = false;
}

void SGFLES::bypass_model()
{
    elec_control_bypassed = true;
}

bool SGFLES::is_elec_control_bypassed() const
{
    return elec_control_bypassed;
}

void SGFLES::check_lvrt_status()
{
    VRT_STATUS current_status = get_lvrt_status();
    switch(current_status)
    {
        case VRT_NORMAL_STATUS:
        {
            double v = get_bus_voltage_in_pu();
            double vth = get_lvrt_voltage_threshold();

            if(v<vth)
            {
                STEPS & toolkit = get_toolkit();
                double time = toolkit.get_dynamic_simulation_time_in_s();
                set_lvrt_status(VRT_DURING_STATUS);
                bypass_model();
                ostringstream osstream;
                osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                        <<" VRT_NORMAL_STATUS to VRT_DURING_STATUS at time "<<time<<" s since V is "<<v<<"(<"<<vth<<").";;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            double Ip0 = active_power_regulator_filter.get_output() / get_bus_voltage_in_pu();;
            double Iq0 = - reactive_current_regulator_filter.get_output();
            update_IPQ0_in_lvrt_during_status(Ip0, Iq0);
            break;
        }
        case VRT_DURING_STATUS:
        {
            double v = get_bus_voltage_in_pu();
            double vth = get_lvrt_voltage_threshold();
            if(v>vth)
            {
                ostringstream osstream;
                STEPS & toolkit = get_toolkit();
                double time = toolkit.get_dynamic_simulation_time_in_s();
                set_lvrt_status(VRT_RECOVER_STATUS);
                P_recovered = false;
                Q_recovered = false;
                set_time_when_lvrt_enter_recover_status(time);
                osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                        <<" VRT_DURING_STATUS to VRT_RECOVER_STATUS at time "<<time<<" s since V is "<<v<<"(>"<<vth<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            double Ip0 = get_lvrt_IP_command();
            double Iq0 = get_lvrt_IQ_command();
            update_IPQ0_in_lvrt_recover_status(Ip0, Iq0);
            break;
        }
        case VRT_RECOVER_STATUS:
        default:
        {
            ostringstream osstream;
            STEPS & toolkit = get_toolkit();
            double time = toolkit.get_dynamic_simulation_time_in_s();

            double v = get_bus_voltage_in_pu();
            double vth = get_lvrt_voltage_threshold();
            if(v<vth)
            {
                set_lvrt_status(VRT_DURING_STATUS);
                set_time_when_lvrt_enter_recover_status(INFINITE_THRESHOLD);

                osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                        <<" VRT_RECOVER_STATUS to VRT_DURING_STATUS at time "<<time<<" s since V is "<<v<<"(<"<<vth<<").";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(is_active_power_recovered() and is_reactive_power_recovered())
                {
                    unbypass_model();
                    set_lvrt_status(VRT_NORMAL_STATUS);
                    set_time_when_lvrt_enter_recover_status(INFINITE_THRESHOLD);

                    osstream<<"LVRT status of "<<get_compound_device_name()<<" is changed from "
                            <<" VRT_RECOVER_STATUS to VRT_NORMAL_STATUS at time "<<time<<" s since both IP and IQ command recovered.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            break;
        }
    }
}

void SGFLES::set_lvrt_status(VRT_STATUS status)
{
    this->lvrt_status = status;
}

VRT_STATUS SGFLES::get_lvrt_status() const
{
    return lvrt_status;
}

bool SGFLES::is_active_power_recovered() const
{
    return P_recovered;
}

bool SGFLES::is_reactive_power_recovered() const
{
    return Q_recovered;
}

double SGFLES::get_lvrt_IP_command()
{
    STEPS& toolkit = get_toolkit();
    VRT_STATUS current_status = get_lvrt_status();
    double order = 0.0;
    double time = 0.0, t0 = 0.0;
    double Istart = 0.0, Iend  = 0.0;

    switch(current_status)
    {
        case VRT_DURING_STATUS:
        {
            //return get_k_Ip_lvrt_during()*get_bus_voltage_in_pu() + get_Ipset_lvrt_during();
            return - get_k_Ip_lvrt_during()*(get_lvrt_voltage_threshold() - get_bus_voltage_in_pu()) + get_Ip0_lvrt_activated_in_pu();
        }
        case VRT_RECOVER_STATUS:
        {
            time = toolkit.get_dynamic_simulation_time_in_s();
            t0 = get_time_when_lvrt_enter_recover_status();
            Istart = get_Ip0_lvrt_recover_activated_in_pu();
            Iend = get_Ip0_lvrt_activated_in_pu();
            if(Istart<Iend)
            {
                order = Istart+fabs(get_slope_Ip_lvrt_recover())*(time-t0);
                if(order>Iend or fabs(order-Iend)<FLOAT_EPSILON)
                {
                    order = Iend;
                    P_recovered = true;
                }

            }
            else //Istart>=Iend
            {
                order = Istart-fabs(get_slope_Ip_lvrt_recover())*(time-t0);
                if(order<Iend or fabs(order-Iend)<FLOAT_EPSILON)
                {
                    order = Iend;
                    P_recovered = true;
                }
            }
            return order;
        }
        case VRT_NORMAL_STATUS:
        default:
            return 0.0;
    }
}

double SGFLES::get_lvrt_IQ_command()
{
    STEPS& toolkit = get_toolkit();
    VRT_STATUS current_status = get_lvrt_status();
    double order = 0.0;
    double time = 0.0, t0 = 0.0;
    double Istart = 0.0, Iend  = 0.0;

    switch(current_status)
    {
        case VRT_DURING_STATUS:
        {
            //return - (get_k_Iq_lvrt_during()*(get_lvrt_voltage_threshold() - get_bus_voltage_in_pu()) + get_Iqset_lvrt_during());
            return - (get_k_Iq_lvrt_during()*(get_lvrt_voltage_threshold() - get_bus_voltage_in_pu()) - get_Iq0_lvrt_activated_in_pu());
        }
        case VRT_RECOVER_STATUS:
        {
            time = toolkit.get_dynamic_simulation_time_in_s();
            t0 = get_time_when_lvrt_enter_recover_status();
            Istart = - get_Iq0_lvrt_recover_activated_in_pu();
            Iend = - get_Iq0_lvrt_activated_in_pu();
            if(Istart<Iend)
            {
                order = Istart+fabs(get_slope_Iq_lvrt_recover())*(time-t0);
                if(order>Iend or fabs(order-Iend)<FLOAT_EPSILON)
                {
                    order = Iend;
                    Q_recovered = true;
                }
            }
            else //Istart>=Iend
            {
                order = Istart-fabs(get_slope_Iq_lvrt_recover())*(time-t0);
                if(order<Iend or fabs(order-Iend)<FLOAT_EPSILON)
                {
                    order = Iend;
                    Q_recovered = true;
                }
            }
            return -order;
        }
        case VRT_NORMAL_STATUS:
        default:
            return 0.0;
    }
}

void SGFLES::update_IPQ0_in_lvrt_during_status(double Ip0, double Iq0)
{
    set_Ip0_lvrt_activated_in_pu(Ip0);
    set_Iq0_lvrt_activated_in_pu(Iq0);
}

void SGFLES::update_IPQ0_in_lvrt_recover_status(double Ip0, double Iq0)
{
    set_Ip0_lvrt_recover_activated_in_pu(Ip0);
    set_Iq0_lvrt_recover_activated_in_pu(Iq0);
}

void SGFLES::set_time_when_lvrt_enter_recover_status(double t)
{
    this->t0_lvrt_recover_activated = t;
}

void SGFLES::set_Ip0_lvrt_activated_in_pu(double I)
{
    this->Ip0_lvrt_activated = I;
}

void SGFLES::set_Iq0_lvrt_activated_in_pu(double I)
{
    this->Iq0_lvrt_activated = I;
}

void SGFLES::set_Ip0_lvrt_recover_activated_in_pu(double I)
{
    this->Ip0_lvrt_recover_activated = I;
}

void SGFLES::set_Iq0_lvrt_recover_activated_in_pu(double I)
{
    this->Iq0_lvrt_recover_activated = I;
}

double SGFLES::get_time_when_lvrt_enter_recover_status() const
{
    return t0_lvrt_recover_activated;
}

double SGFLES::get_Ip0_lvrt_activated_in_pu() const
{
    return Ip0_lvrt_activated;
}

double SGFLES::get_Iq0_lvrt_activated_in_pu() const
{
    return Iq0_lvrt_activated;
}

double SGFLES::get_Ip0_lvrt_recover_activated_in_pu() const
{
    return Ip0_lvrt_recover_activated;
}

double SGFLES::get_Iq0_lvrt_recover_activated_in_pu() const
{
    return Iq0_lvrt_recover_activated;
}

string SGFLES::get_model_name() const
{
    return "SGFLES";
}

bool SGFLES::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;
    if(data.size()>=19)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double kpll, kipll, pllmax, Imax, Kvi, Tvi, Kdroop, Tdroop, Tpcmd, Tv, Tqcmd;
            double lvrt_voltage_threshold, k_Ip_lvrt_during, slope_Ip_lvrt_recover;
            double k_Iq_lvrt_during, slope_Iq_lvrt_recover;//, Ipset_lvrt_during, Iqset_lvrt_during;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            kpll = get_double_data(data[i],"0.0"); i++;
            kipll = get_double_data(data[i],"0.0"); i++;
            pllmax = get_double_data(data[i],"0.0"); i++;
            Imax = get_double_data(data[i],"0.0"); i++;
            Kvi = get_double_data(data[i],"0.0"); i++;
            Tvi = get_double_data(data[i],"0.0"); i++;
            Kdroop = get_double_data(data[i],"0.0"); i++;
            Tdroop = get_double_data(data[i],"0.0"); i++;
            Tpcmd = get_double_data(data[i],"0.0"); i++;
            Tv = get_double_data(data[i],"0.0"); i++;
            Tqcmd = get_double_data(data[i],"0.0"); i++;
            lvrt_voltage_threshold = get_double_data(data[i],"0.0"); i++;
            k_Ip_lvrt_during = get_double_data(data[i],"0.0"); i++;
            //Ipset_lvrt_during = get_double_data(data[i],"0.0"); i++;
            slope_Ip_lvrt_recover = get_double_data(data[i],"0.0"); i++;
            k_Iq_lvrt_during = get_double_data(data[i],"0.0"); i++;
            //Iqset_lvrt_during = get_double_data(data[i],"0.0"); i++;
            slope_Iq_lvrt_recover = get_double_data(data[i],"0.0");

            set_KPLL(kpll);
            set_KIPLL(kipll);
            set_PLLmax(pllmax);
            set_Imax_in_pu_based_on_mbase(Imax);
            set_Kvi(Kvi);
            set_Tvi_in_s(Tvi);
            set_Kdroop(Kdroop);
            set_Tdroop_in_s(Tdroop);
            set_Tpcmd(Tpcmd);
            set_Tv(Tv);
            set_Tqcmd(Tqcmd);
            set_lvrt_voltage_threshold(lvrt_voltage_threshold);
            set_k_Ip_lvrt_during(k_Ip_lvrt_during);
            //set_Ipset_lvrt_during(Ipset_lvrt_during);
            set_slope_Ip_lvrt_recover(slope_Ip_lvrt_recover);
            set_k_Iq_lvrt_during(k_Iq_lvrt_during);
            //set_Iqset_lvrt_during(Iqset_lvrt_during);
            set_slope_Iq_lvrt_recover(slope_Iq_lvrt_recover);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool SGFLES::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool SGFLES::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void SGFLES::setup_block_toolkit_and_parameters()
{
    csconv3.set_bus_pointer(get_bus_pointer());
    csconv3.set_device_pointer(get_device_pointer());
    frequency_controller.set_bus_pointer(get_bus_pointer());
    frequency_controller.set_device_pointer(get_device_pointer());
}

void SGFLES::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        ENERGY_STORAGE* es = get_energy_storage_pointer();
        setup_block_toolkit_and_parameters();

        double mbase = get_mbase_in_MVA();
        double P = es->get_p_generation_in_MW();
        double Q = es->get_q_generation_in_MVar();
        P /= mbase;
        Q /= mbase;

        complex<double> Z = es->get_source_impedance_in_pu();
        double Xeq = Z.imag();

        complex<double> Zsource(0.0, Xeq);

        double vterm = get_bus_voltage_in_pu();
        double vref = vterm;
        set_voltage_reference_in_pu(vref);

        csconv3.set_initial_P_generation_in_pu(P);
        csconv3.set_initial_Q_generation_in_pu(Q);
        csconv3.set_source_impedance_in_pu_based_on_mbase(Zsource);
        csconv3.initialize();

        double Iq = Q / vterm;
        frequency_controller.initialize();
        active_power_regulator_filter.set_output(P);
        active_power_regulator_filter.initialize();

        reactive_current_regulator_filter.set_output(Iq);
        reactive_current_regulator_filter.initialize();

        voltage_regulator_integrator.set_output(Iq);
        voltage_regulator_integrator.initialize();

        set_lvrt_status(VRT_NORMAL_STATUS);

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(3) States of blocks"<<endl
                    <<"    active_current_commander block state: "<<csconv3.get_active_current_commander_state()<<endl
                    <<"    reactive_current_commander block state: "<<csconv3.get_reactive_current_commander_state()<<endl
                    <<"    PLL_frequency_integrator block state: "<<csconv3.get_PLL_frequency_integrator_state()<<endl
                    <<"    PLL_angle_integrator block state: "<<csconv3.get_PLL_angle_integrator_state()<<endl
                    <<"    virtual inertia regulator state: "<<frequency_controller.get_virtual_inertia_controller_state()<<endl
                    <<"    frequency droop regulator state: "<<frequency_controller.get_frequency_droop_controller_state()<<endl
                    <<"    active_power_regulator_filter block state: "<<active_power_regulator_filter.get_state()<<endl
                    <<"    reactive_current_regulator_filter block state: "<<reactive_current_regulator_filter.get_state()<<endl
                    <<"    voltage_regulator_integrator block state: "<<voltage_regulator_integrator.get_output()<<endl
                    <<"(4) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(5) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(6) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu"<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void SGFLES::run(DYNAMIC_MODE mode)
{
    check_lvrt_status();

    if(not is_elec_control_bypassed())
    {
        frequency_controller.run(mode);
        double input = get_initial_active_power_command_in_pu_based_on_mbase() + frequency_controller.get_frequency_regulation_power_deviation_in_pu();
        active_power_regulator_filter.set_input(input);
        active_power_regulator_filter.run(mode);

        input = get_voltage_reference_in_pu() - get_bus_voltage_in_pu();
        voltage_regulator_integrator.set_input(input);
        voltage_regulator_integrator.run(mode);
        reactive_current_regulator_filter.set_input(voltage_regulator_integrator.get_output());
        reactive_current_regulator_filter.run(mode);
    }

    double Ipcmd = get_Ipcmd_in_pu_based_on_mbase();
    double Iqcmd = get_Iqcmd_in_pu_based_on_mbase();
    csconv3.set_Ipcmd_in_pu_based_on_mbase(Ipcmd);
    csconv3.set_Iqcmd_in_pu_based_on_mbase(Iqcmd);
    csconv3.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> SGFLES::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv3.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double SGFLES::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return csconv3.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> SGFLES::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv3.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> SGFLES::get_internal_voltage_in_pu_in_xy_axis()
{
    return csconv3.get_internal_voltage_in_pu_in_xy_axis();
}

double SGFLES::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return csconv3.get_initial_active_current_command_in_pu_based_on_mbase();
}

double SGFLES::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return csconv3.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double SGFLES::get_initial_reactive_voltage_command_in_pu() const
{
    return csconv3.get_initial_reactive_voltage_command_in_pu();
}

double SGFLES::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return csconv3.get_initial_active_power_command_in_pu_based_on_mbase();
}

double SGFLES::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return csconv3.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void SGFLES::check()
{
    ;
}

void SGFLES::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void SGFLES::save()
{
    ;
}

string SGFLES::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_KPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_KIPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_PLLmax()<<", "
            <<setw(8)<<setprecision(6)<<get_Imax_in_pu_based_on_mbase()<<", "
            <<setw(8)<<setprecision(6)<<get_Kvi()<<", "
            <<setw(8)<<setprecision(6)<<get_Tvi_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Kdroop()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Tdroop_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Tpcmd()<<", "
            <<setw(8)<<setprecision(6)<<get_Tv()<<", "
            <<setw(8)<<setprecision(6)<<get_Tqcmd()<<", "
            <<setw(8)<<setprecision(6)<<get_lvrt_voltage_threshold()<<", "
            <<setw(8)<<setprecision(6)<<get_k_Ip_lvrt_during()<<", "
            //<<setw(8)<<setprecision(6)<<get_Ipset_lvrt_during()<<", "
            <<setw(8)<<setprecision(6)<<get_slope_Ip_lvrt_recover()<<", "
            <<setw(8)<<setprecision(6)<<get_k_Iq_lvrt_during()<<", "
            //<<setw(8)<<setprecision(6)<<get_Iqset_lvrt_during()<<", "
            <<setw(8)<<setprecision(6)<<get_slope_Iq_lvrt_recover()<<" /";
    return osstream.str();
}

void SGFLES::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KPLL", i); i++;
    add_model_data_name_and_index_pair("KIPLL", i); i++;
    add_model_data_name_and_index_pair("PLLMAX", i); i++;
    add_model_data_name_and_index_pair("IMAX", i); i++;
    add_model_data_name_and_index_pair("KVI", i); i++;
    add_model_data_name_and_index_pair("TVI", i); i++;
    add_model_data_name_and_index_pair("KDROOP", i); i++;
    add_model_data_name_and_index_pair("TDROOP", i); i++;
    add_model_data_name_and_index_pair("TPCMD", i); i++;
    add_model_data_name_and_index_pair("TV", i); i++;
    add_model_data_name_and_index_pair("TQCMD", i); i++;
    add_model_data_name_and_index_pair("VTHRES_LVRT ", i); i++;
    add_model_data_name_and_index_pair("KIP_LVRT", i); i++;
    //add_model_data_name_and_index_pair("IPSET_LVRT", i); i++;
    add_model_data_name_and_index_pair("IPSLOPE_LVRT", i); i++;
    add_model_data_name_and_index_pair("KIQ_LVRT", i); i++;
    //add_model_data_name_and_index_pair("IQSET_LVRT", i); i++;
    add_model_data_name_and_index_pair("IQSLOPE_LVRT", i); i++;
}

double SGFLES::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="KPLL")
        return get_KPLL();
    if(par_name=="KIPLL")
        return get_KIPLL();
    if(par_name=="PLLMAX")
        return get_PLLmax();
    if(par_name=="IMAX")
        return get_Imax_in_pu_based_on_mbase();
    if(par_name=="KVI")
        return get_Kvi();
    if(par_name=="TVI")
        return get_Tvi_in_s();
    if(par_name=="KDROOP")
        return get_Kdroop();
    if(par_name=="TDROOP")
        return get_Tdroop_in_s();
    if(par_name=="TPCMD")
        return get_Tpcmd();
    if(par_name=="TV")
        return get_Tv();
    if(par_name=="TQCMD")
        return get_Tqcmd();
    if(par_name=="VTHRES_LVRT")
        return get_lvrt_voltage_threshold();
    if(par_name=="KIP_LVRT")
        return get_k_Ip_lvrt_during();
    //if(par_name=="IPSET_LVRT")
    //    return get_Ipset_lvrt_during();
    if(par_name=="IPSLOPE_LVRT")
        return get_slope_Ip_lvrt_recover();
    if(par_name=="KIQ_LVRT")
        return get_k_Iq_lvrt_during();
    //if(par_name=="IQSET_LVRT")
    //    return get_Iqset_lvrt_during();
    if(par_name=="IQSLOPE_LVRT")
        return get_slope_Iq_lvrt_recover();
    return 0.0;
}

void SGFLES::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KPLL")
        return set_KPLL(value);
    if(par_name=="KIPLL")
        return set_KIPLL(value);
    if(par_name=="PLLMAX")
        return set_PLLmax(value);
    if(par_name=="IMAX")
        return set_Imax_in_pu_based_on_mbase(value);
    if(par_name=="KVI")
        return set_Kvi(value);
    if(par_name=="TVI")
        return set_Tvi_in_s(value);
    if(par_name=="KDROOP")
        return set_Kdroop(value);
    if(par_name=="TDROOP")
        return set_Tdroop_in_s(value);
    if(par_name=="TPCMD")
        return set_Tpcmd(value);
    if(par_name=="TV")
        return set_Tv(value);
    if(par_name=="TQCMD")
        return set_Tqcmd(value);
    if(par_name=="VTHRES_LVRT")
        return set_lvrt_voltage_threshold(value);
    if(par_name=="KIP_LVRT")
        return set_k_Ip_lvrt_during(value);
    //if(par_name=="IPSET_LVRT")
    //    return set_Ipset_lvrt_during(value);
    if(par_name=="IPSLOPE_LVRT")
        return set_slope_Ip_lvrt_recover(value);
    if(par_name=="KIQ_LVRT")
        return set_k_Iq_lvrt_during(value);
    //if(par_name=="IQSET_LVRT")
    //    return set_Iqset_lvrt_during(value);
    if(par_name=="IQSLOPE_LVRT")
        return set_slope_Iq_lvrt_recover(value);
    return;
}

double SGFLES::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void SGFLES::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("PLL FREQUENCY IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("PLL FREQUENCY IN HZ", i); i++;
    add_model_internal_variable_name_and_index_pair("ACTIVE CURRENT IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("REACTIVE CURRENT IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("ACTIVE CURRENT COMMAND IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("REACTIVE CURRENT COMMAND IN PU ON MBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL FREQUENCY BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL INERTIA CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PRIMARY FREQUENCY CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE POWER REGULATOR FILTER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE CURRENT REGULATOR FILTER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VOLTAGE REGULATOR INTEGRATOR", i); i++;
}

double SGFLES::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "PLL ANGLE IN RAD")
        return get_pll_angle_in_rad();
    if(var_name == "PLL FREQUENCY IN PU")
        return get_pll_frequency_in_pu();
    if(var_name == "PLL FREQUENCY IN HZ")
        return get_pll_frequency_in_Hz();
    if(var_name == "ACTIVE CURRENT IN PU ON MBASE")
        return csconv3.get_Ip_in_pu();
    if(var_name == "REACTIVE CURRENT IN PU ON MBASE")
        return csconv3.get_Iq_in_pu();
    if(var_name == "ACTIVE CURRENT COMMAND IN PU ON MBASE")
        return get_Ipcmd_in_pu_based_on_mbase();
    if(var_name == "REACTIVE CURRENT COMMAND IN PU ON MBASE")
        return get_Iqcmd_in_pu_based_on_mbase();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return csconv3.get_active_current_commander_state();
    if(var_name == "STATE@REACTIVE CURRENT COMMAND BLOCK")
        return csconv3.get_reactive_current_commander_state();
    if(var_name == "STATE@PLL FREQUENCY BLOCK")
        return csconv3.get_PLL_frequency_integrator_state();
    if(var_name == "STATE@PLL ANGLE BLOCK")
        return csconv3.get_PLL_angle_integrator_state();
    if(var_name == "STATE@VIRTUAL INERTIA CONTROL")
        return frequency_controller.get_virtual_inertia_controller_state();
    if(var_name == "STATE@PRIMARY FREQUENCY CONTROL")
        return frequency_controller.get_frequency_droop_controller_state();
    if(var_name == "STATE@ACTIVE POWER REGULATOR FILTER")
        return active_power_regulator_filter.get_state();
    if(var_name == "STATE@REACTIVE CURRENT REGULATOR FILTER")
        return reactive_current_regulator_filter.get_state();
    if(var_name == "STATE@VOLTAGE REGULATOR INTEGRATOR")
        return voltage_regulator_integrator.get_state();
    return 0.0;
}

double SGFLES::get_pll_angle_in_rad()
{
    return csconv3.get_pll_angle_in_rad();
}

double SGFLES::get_pll_angle_in_deg()
{
    return csconv3.get_pll_angle_in_deg();
}

double SGFLES::get_pll_frequency_deviation_in_pu()
{
    return csconv3.get_pll_frequency_deviation_in_pu();
}

double SGFLES::get_pll_frequency_deviation_in_Hz()
{
    return csconv3.get_pll_frequency_deviation_in_Hz();
}

double SGFLES::get_pll_frequency_in_pu()
{
    return csconv3.get_pll_frequency_deviation_in_pu();
}

double SGFLES::get_pll_frequency_in_Hz()
{
    return csconv3.get_pll_frequency_in_Hz();
}


string SGFLES::get_dynamic_data_in_psse_format() const
{
    return "";
}

string SGFLES::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string SGFLES::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
