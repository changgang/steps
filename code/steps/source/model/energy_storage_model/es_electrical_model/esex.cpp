#include "header/model/energy_storage_model/es_electrical_model/esex.h"
#include "header/device/energy_storage.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

ESEX::ESEX(STEPS& toolkit) : ES_ELECTRICAL_MODEL(toolkit),
                               elecx(toolkit)

{
    clear();
}

ESEX::ESEX(const ESEX& model) : ES_ELECTRICAL_MODEL(model.get_toolkit()),
                                   elecx(model.get_toolkit())

{
    copy_from_const_model(model);
}

ESEX::~ESEX()
{
    ;
}

ESEX& ESEX::operator=(const ESEX& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void ESEX::clear()
{
    set_model_float_parameter_count(41);
    elecx.clear();
}

void ESEX::copy_from_const_model(const ESEX& model)
{
    if(model.is_model_bypassed()) bypass_model();

    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    elecx.set_toolkit(toolkit);

    clear();


    set_bus_to_regulate(model.get_bus_to_regulate());
    set_active_power_control_mode(model.get_active_power_control_mode());
    set_var_control_mode(model.get_var_control_mode());
    set_Xcomp_in_pu(model.get_Xcomp_in_pu());
    set_TRV_in_s(model.get_TRV_in_s());
    set_Fn(model.get_Fn());
    set_KIV(model.get_KIV());
    set_Qmax_in_pu(model.get_Qmax_in_pu());
    set_Qmin_in_pu(model.get_Qmin_in_pu());
    set_KPV(model.get_KPV());
    set_TV_in_s(model.get_TV_in_s());
    set_TFV_in_s(model.get_TFV_in_s());
    set_TP_in_s(model.get_TP_in_s());
    set_KQI(model.get_KQI());
    set_Vmax_in_pu(model.get_Vmax_in_pu());
    set_Vmin_in_pu(model.get_Vmin_in_pu());
    set_voltage_flag(model.get_voltage_flag());
    set_KQV(model.get_KQV());
    set_EQmax_in_pu(model.get_EQmax_in_pu());
    set_EQmin_in_pu(model.get_EQmin_in_pu());
    set_Tspeed_in_s(model.get_Tspeed_in_s());
    set_KPP(model.get_KPP());
    set_KIP(model.get_KIP());
    set_Kvi(model.get_Kvi());
    set_Tvi_in_s(model.get_Tvi_in_s());
    set_Kdroop(model.get_Kdroop());
    set_Tdroop_in_s(model.get_Tdroop_in_s());
    set_frequency_deviation_upper_deadband_in_pu(model.get_frequency_deviation_upper_deadband_in_pu());
    set_frequency_deviation_lower_deadband_in_pu(model.get_frequency_deviation_lower_deadband_in_pu());
    set_Kfint(model.get_Kfint());
    set_rPmax_in_pu(model.get_rPmax_in_pu());
    set_rPmin_in_pu(model.get_rPmin_in_pu());
    set_TFP_in_s(model.get_TFP_in_s());
    set_Pmax_in_pu(model.get_Pmax_in_pu());
    set_Pmin_in_pu(model.get_Pmin_in_pu());
    set_IPmax_in_pu(model.get_IPmax_in_pu());
}

string ESEX::get_model_name() const
{
    return "ESEX";
}

void ESEX::set_var_control_mode(ELEC_REACTIVE_POWER_CONTROL_MODE mode)
{
    elecx.set_var_control_mode(mode);
}

ELEC_REACTIVE_POWER_CONTROL_MODE ESEX::get_var_control_mode() const
{
    return elecx.get_var_control_mode();
}

void ESEX::set_active_power_control_mode(ELEC_ACTIVE_POWER_CONTROL_MODE mode)
{
    elecx.set_active_power_control_mode(mode);
}

ELEC_ACTIVE_POWER_CONTROL_MODE ESEX::get_active_power_control_mode() const
{
    return elecx.get_active_power_control_mode();
}

void ESEX::enable_active_power_closed_loop_control()
{
    elecx.enable_active_power_closed_loop_control();
}

void ESEX::enable_active_power_open_loop_control()
{
    elecx.enable_active_power_open_loop_control();
}

bool ESEX::get_active_power_closed_control_flag() const
{
    return elecx.get_active_power_closed_control_flag();
}

void ESEX::set_bus_to_regulate(unsigned int bus)
{
    elecx.set_bus_to_regulate(bus);
}

unsigned int ESEX::get_bus_to_regulate() const
{
    return elecx.get_bus_to_regulate();
}

void ESEX::set_Xcomp_in_pu(double Xc)
{
    elecx.set_Xcomp_in_pu(Xc);
}

void ESEX::set_TRV_in_s(double T)
{
    elecx.set_TRV_in_s(T);
}

void ESEX::set_Fn(double Fn)
{
    elecx.set_Fn(Fn);
}

void ESEX::set_KIV(double K)
{
    elecx.set_KIV(K);
}

void ESEX::set_Qmax_in_pu(double q)
{
    elecx.set_Qmax_in_pu(q);
}

void ESEX::set_Qmin_in_pu(double q)
{
    elecx.set_Qmin_in_pu(q);
}

void ESEX::set_KPV(double K)
{
    elecx.set_KPV(K);
}

void ESEX::set_TV_in_s(double T)
{
    elecx.set_TV_in_s(T);
}

void ESEX::set_TFV_in_s(double T)
{
    elecx.set_TFV_in_s(T);
}

void ESEX::set_TP_in_s(double T)
{
    elecx.set_TP_in_s(T);
}

void ESEX::set_KQI(double K)
{
    elecx.set_KQI(K);
}

void ESEX::set_Vmax_in_pu(double v)
{
    elecx.set_Vmax_in_pu(v);
}

void ESEX::set_Vmin_in_pu(double v)
{
    elecx.set_Vmin_in_pu(v);
}

void ESEX::set_voltage_flag(unsigned int flag)
{
    elecx.set_voltage_flag(flag);
}

void ESEX::set_KQV(double K)
{
    elecx.set_KQV(K);
}

void ESEX::set_EQmax_in_pu(double I)
{
    elecx.set_EQmax_in_pu(I);
}

void ESEX::set_EQmin_in_pu(double I)
{
    elecx.set_EQmin_in_pu(I);
}

double ESEX::get_Xcomp_in_pu() const
{
    return elecx.get_Xcomp_in_pu();
}

double ESEX::get_TRV_in_s() const
{
    return elecx.get_TRV_in_s();
}

double ESEX::get_Fn() const
{
    return elecx.get_Fn();
}

double ESEX::get_KIV() const
{
    return elecx.get_KIV();
}

double ESEX::get_Qmax_in_pu() const
{
    return elecx.get_Qmax_in_pu();
}

double ESEX::get_Qmin_in_pu() const
{
    return elecx.get_Qmin_in_pu();
}

double ESEX::get_KPV() const
{
    return elecx.get_KPV();
}

double ESEX::get_TV_in_s() const
{
    return elecx.get_TV_in_s();
}

double ESEX::get_TFV_in_s() const
{
    return elecx.get_TFV_in_s();
}

double ESEX::get_TP_in_s() const
{
    return elecx.get_TP_in_s();
}

double ESEX::get_KQI() const
{
    return elecx.get_KQI();
}

double ESEX::get_Vmax_in_pu() const
{
    return elecx.get_Vmax_in_pu();
}

double ESEX::get_Vmin_in_pu() const
{
    return elecx.get_Vmin_in_pu();
}

unsigned int ESEX::get_voltage_flag() const
{
    return elecx.get_voltage_flag();
}

double ESEX::get_KQV() const
{
    return elecx.get_KQV();
}

double ESEX::get_EQmax_in_pu() const
{
    return elecx.get_EQmax_in_pu();
}

double ESEX::get_EQmin_in_pu() const
{
    return elecx.get_EQmin_in_pu();
}

void ESEX::set_Kvi(double K)
{
    elecx.set_Kvi(K);
}

void ESEX::set_Tvi_in_s(double T)
{
    elecx.set_Tvi_in_s(T);
}

void ESEX::set_Kdroop(double K)
{
    elecx.set_Kdroop(K);
}

void ESEX::set_Tdroop_in_s(double T)
{
    elecx.set_Tdroop_in_s(T);
}

void ESEX::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    elecx.set_frequency_deviation_upper_deadband_in_pu(f);
}

void ESEX::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    elecx.set_frequency_deviation_lower_deadband_in_pu(f);
}

void ESEX::set_Kfint(double K)
{
    elecx.set_Kfint(K);
}

double ESEX::get_Kvi() const
{
    return elecx.get_Kvi();
}

double ESEX::get_Tvi_in_s() const
{
    return elecx.get_Tvi_in_s();
}

double ESEX::get_Kdroop() const
{
    return elecx.get_Kdroop();
}

double ESEX::get_Tdroop_in_s() const
{
    return elecx.get_Tdroop_in_s();
}

double ESEX::get_frequency_deviation_upper_deadband_in_pu() const
{
    return elecx.get_frequency_deviation_upper_deadband_in_pu();
}

double ESEX::get_frequency_deviation_lower_deadband_in_pu() const
{
    return elecx.get_frequency_deviation_lower_deadband_in_pu();
}

double ESEX::get_Kfint() const
{
    return elecx.get_Kfint();
}

void ESEX::set_Tspeed_in_s(double T)
{
    elecx.set_Tspeed_in_s(T);
}

void ESEX::set_KPP(double K)
{
    elecx.set_KPP(K);
}

void ESEX::set_KIP(double K)
{
    elecx.set_KIP(K);
}

double ESEX::get_Tspeed_in_s() const
{
    return elecx.get_Tspeed_in_s();
}

double ESEX::get_KPP() const
{
    return elecx.get_KPP();
}

double ESEX::get_KIP() const
{
    return elecx.get_KIP();
}

void ESEX::set_Kpspeed(double K)
{
    elecx.set_Kpspeed(K);
}

void ESEX::set_Kispeed(double K)
{
    elecx.set_Kispeed(K);
}

double ESEX::get_Kpspeed() const
{
    return elecx.get_Kpspeed();
}

double ESEX::get_Kispeed() const
{
    return elecx.get_Kispeed();
}

void ESEX::set_rPmax_in_pu(double r)
{
    elecx.set_rPmax_in_pu(r);
}

void ESEX::set_rPmin_in_pu(double r)
{
    elecx.set_rPmin_in_pu(r);
}

void ESEX::set_TFP_in_s(double T)
{
    elecx.set_TFP_in_s(T);
}

void ESEX::set_Pmax_in_pu(double p)
{
    elecx.set_Pmax_in_pu(p);
}

void ESEX::set_Pmin_in_pu(double p)
{
    elecx.set_Pmin_in_pu(p);
}

void ESEX::set_IPmax_in_pu(double I)
{
    elecx.set_IPmax_in_pu(I);
}

void ESEX::set_Kpip(double K)
{
    elecx.set_Kpip(K);
}

void ESEX::set_Kiip(double K)
{
    elecx.set_Kiip(K);
}

double ESEX::get_rPmax_in_pu() const
{
    return elecx.get_rPmax_in_pu();
}

double ESEX::get_rPmin_in_pu() const
{
    return elecx.get_rPmin_in_pu();
}

double ESEX::get_TFP_in_s() const
{
    return elecx.get_TFP_in_s();
}

double ESEX::get_Pmax_in_pu() const
{
    return elecx.get_Pmax_in_pu();
}

double ESEX::get_Pmin_in_pu() const
{
    return elecx.get_Pmin_in_pu();
}

double ESEX::get_IPmax_in_pu() const
{
    return elecx.get_IPmax_in_pu();
}

double ESEX::get_Kpip() const
{
    return elecx.get_Kpip();
}

double ESEX::get_Kiip() const
{
    return elecx.get_Kiip();
}

double ESEX::get_voltage_sensor_state() const
{
    return elecx.get_voltage_sensor_state();
}

double ESEX::get_voltage_regulator_first_order_block_state() const
{
    return elecx.get_voltage_regulator_first_order_block_state();
}

double ESEX::get_voltage_regulator_integrator_state() const
{
    return elecx.get_voltage_regulator_integrator_state();
}

double ESEX::get_voltage_regulator_filter_state() const
{
    return elecx.get_voltage_regulator_filter_state();
}

double ESEX::get_active_power_sensor_state() const
{
    return elecx.get_active_power_sensor_state();
}

double ESEX::get_Q_error_integrator_state() const
{
    return elecx.get_Q_error_integrator_state();
}

double ESEX::get_V_error_integrator_state() const
{
    return elecx.get_V_error_integrator_state();
}

double ESEX::get_virtual_inertia_controller_state() const
{
    return elecx.get_virtual_inertia_controller_state();
}

double ESEX::get_frequency_droop_controller_state() const
{
    return elecx.get_frequency_droop_controller_state();
}

double ESEX::get_frequency_integral_controller_state() const
{
    return elecx.get_frequency_integral_controller_state();
}

double ESEX::get_power_order_integrator_state() const
{
    return elecx.get_power_order_integrator_state();
}

double ESEX::get_speed_reference_sensor_state() const
{
    return elecx.get_speed_reference_sensor_state();
}

double ESEX::get_torque_controller_state() const
{
    return elecx.get_torque_controller_state();
}

double ESEX::get_virtual_inertia_power_deviation_in_pu() const
{
    return elecx.get_virtual_inertia_power_deviation_in_pu();
}

double ESEX::get_frequency_droop_power_deviation_in_pu() const
{
    return elecx.get_frequency_droop_power_deviation_in_pu();
}

double ESEX::get_frequency_integral_power_deviation_in_pu() const
{
    return elecx.get_frequency_integral_power_deviation_in_pu();
}

double ESEX::get_frequency_regulation_power_deviation_in_pu() const
{
    return elecx.get_frequency_regulation_power_deviation_in_pu();
}

bool ESEX::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=44)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int bus;
            int var_control_flag;
            unsigned int voltage_flag;
            int p_control_mode;
            int loop_flag;
            double xcomp, trv, fn, kiv, qmax, qmin, kpv, tv, tfv, tp, kqi, vmax,
                    vmin, kqv, eqmax, eqmin, kvi, tvi, kdroop, tdroop, fupper, flower,
                    kfint, tspeed, kpp, kip, kpspeed, kispeed, rpmax, rpmin, tfp,
                    pmax, pmin, ipmax, kpip, kiip;

            unsigned int i=3;
            bus = (unsigned int)(get_integer_data(data[i],"0")); i++;
            var_control_flag = get_integer_data(data[i],"0"); i++;
            voltage_flag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            p_control_mode = get_integer_data(data[i],"0"); i++;
            loop_flag = get_integer_data(data[i],"0"); i++;
            xcomp = get_double_data(data[i],"0.0"); i++;
            trv = get_double_data(data[i],"0.0"); i++;
            fn = get_double_data(data[i],"0.0"); i++;
            kiv = get_double_data(data[i],"0.0"); i++;
            qmax = get_double_data(data[i],"0.0"); i++;
            qmin = get_double_data(data[i],"0.0"); i++;
            kpv = get_double_data(data[i],"0.0"); i++;
            tv = get_double_data(data[i],"0.0"); i++;
            tfv = get_double_data(data[i],"0.0"); i++;
            tp = get_double_data(data[i],"0.0"); i++;
            kqi = get_double_data(data[i],"0.0"); i++;
            vmax = get_double_data(data[i],"0.0"); i++;
            vmin = get_double_data(data[i],"0.0"); i++;
            kqv = get_double_data(data[i],"0.0"); i++;
            eqmax = get_double_data(data[i],"0.0"); i++;
            eqmin = get_double_data(data[i],"0.0"); i++;
            kvi = get_double_data(data[i],"0.0"); i++;
            tvi = get_double_data(data[i],"0.0"); i++;
            kdroop = get_double_data(data[i],"0.0"); i++;
            tdroop = get_double_data(data[i],"0.0"); i++;
            fupper = get_double_data(data[i],"0.0"); i++;
            flower = get_double_data(data[i],"0.0"); i++;
            kfint = get_double_data(data[i],"0.0"); i++;
            tspeed = get_double_data(data[i],"0.0"); i++;
            kpp = get_double_data(data[i],"0.0"); i++;
            kip = get_double_data(data[i],"0.0"); i++;
            kpspeed = get_double_data(data[i],"0.0"); i++;
            kispeed = get_double_data(data[i],"0.0"); i++;
            rpmax = get_double_data(data[i],"0.0"); i++;
            rpmin = get_double_data(data[i],"0.0"); i++;
            tfp = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            ipmax = get_double_data(data[i],"0.0"); i++;
            kpip = get_double_data(data[i],"0.0"); i++;
            kiip = get_double_data(data[i],"0.0"); i++;


            set_bus_to_regulate(bus);
            ELEC_REACTIVE_POWER_CONTROL_MODE qmode;
            switch(var_control_flag)
            {
                case 0:
                {
                    qmode = CONSTANT_VAR_MODE;
                    break;
                }
                case 1:
                {
                    qmode = CONSTANT_VOLTAGE_MODE;
                    break;
                }
                case -1:
                {
                    qmode = CONSTANT_POWER_FACTOR_MODE;
                    break;
                }
                default:
                {
                    qmode = CONSTANT_VAR_MODE;
                    break;
                }
            }

            set_var_control_mode(qmode);

            if(voltage_flag>2)
                voltage_flag = 2;
            set_voltage_flag(voltage_flag);


            ELEC_ACTIVE_POWER_CONTROL_MODE pmode;
            switch(p_control_mode)
            {
                case 0:
                {
                    pmode = SPEED_CONTROL_MODE;
                    break;
                }
                case 1:
                {
                    pmode = ACTIVE_POWER_MODE;
                    break;
                }
                case 2:
                {
                    pmode = CONSTANT_TORQUE_MODE;
                    break;
                }
                default:
                {
                    pmode = SPEED_CONTROL_MODE;
                    break;
                }
            }

            set_active_power_control_mode(pmode);

            if(loop_flag==1)
                enable_active_power_closed_loop_control();
            else
                enable_active_power_open_loop_control();


            set_Xcomp_in_pu(xcomp);
            set_TRV_in_s(trv);
            set_Fn(fn);
            set_KIV(kiv);
            set_Qmax_in_pu(qmax);
            set_Qmin_in_pu(qmin);
            set_KPV(kpv);
            set_TV_in_s(tv);
            set_TFV_in_s(tfv);
            set_TP_in_s(tp);
            set_KQI(kqi);
            set_Vmax_in_pu(vmax);
            set_Vmin_in_pu(vmin);
            set_KQV(kqv);
            set_EQmax_in_pu(eqmax);
            set_EQmin_in_pu(eqmin);
            set_Kvi(kvi);
            set_Tvi_in_s(tvi);
            set_Kdroop(kdroop);
            set_Tdroop_in_s(tdroop);
            set_frequency_deviation_lower_deadband_in_pu(flower);
            set_frequency_deviation_upper_deadband_in_pu(fupper);
            set_Kfint(kfint);
            set_Tspeed_in_s(tspeed);
            set_KPP(kpp);
            set_KIP(kip);
            set_Kpspeed(kpspeed);
            set_Kispeed(kispeed);
            set_rPmax_in_pu(rpmax);
            set_rPmin_in_pu(rpmin);
            set_TFP_in_s(tfp);
            set_Pmax_in_pu(pmax);
            set_Pmin_in_pu(pmin);
            set_IPmax_in_pu(ipmax);
            set_Kpip(kpip);
            set_Kiip(kiip);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool ESEX::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESEX::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESEX::setup_block_toolkit_and_parameters()
{
    elecx.set_bus_pointer(get_bus_pointer());
    elecx.set_device_pointer(get_device_pointer());
}

void ESEX::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();
        ENERGY_STORAGE* es = get_energy_storage_pointer();
        ES_CONVERTER_MODEL* es_converter_model = es->get_es_converter_model();
        ES_MODE mode = get_energy_storage_mode();

        setup_block_toolkit_and_parameters();

        STEPS& toolkit = get_toolkit();

        double iterm = get_energy_storage_terminal_current_in_pu();
        complex<double> selec = get_energy_storage_terminal_generation_in_pu_based_on_mbase();
        double pelec = selec.real();
        double qelec = selec.imag();
        double ipcmd = es_converter_model->get_initial_active_current_command_in_pu_based_on_mbase();
        double iqcmd = es_converter_model->get_initial_reactive_current_command_in_pu_based_on_mbase();
        double pcmd = es_converter_model->get_initial_active_power_command_in_pu_based_on_mbase();
        double qcmd = es_converter_model->get_initial_reactive_power_command_in_pu_based_on_mbase();
        double eqcmd = es_converter_model->get_initial_reactive_voltage_command_in_pu();
        double xsource = get_source_impedance_in_pu_based_on_mbase().imag();

        double speed = get_rotor_speed_in_pu();
        double speedref = get_rotor_speed_reference_in_pu();

        if(mode==ES_OUT_SERVICE_MODE)
            elecx.deactivate_active_power_control();
        else
            elecx.activate_active_power_control();

        if(mode==ES_CHARGE_MODE)
            elecx.set_energy_storage_in_charge_mode();
        else
            elecx.set_energy_storage_in_discharge_mode();

        elecx.set_initial_speed(speed);
        elecx.set_speed_reference_in_pu(speedref);
        elecx.set_active_power_reference_in_pu_based_on_mbase(pcmd);
        elecx.set_terminal_current_in_pu_based_on_mbase(iterm);
        elecx.set_active_power_generation_in_pu_based_on_mbase(pelec);
        elecx.set_reactive_power_generation_in_pu_based_on_mbase(qelec);
        elecx.set_initial_active_current_command_in_pu_based_on_mbase(ipcmd);
        elecx.set_initial_active_power_command_in_pu_based_on_mbase(pcmd);
        elecx.set_initial_reactive_current_command_in_pu_based_on_mbase(iqcmd);
        elecx.set_initial_reactive_power_command_in_pu_based_on_mbase(qcmd);
        elecx.set_initial_reactive_voltage_command_in_pu(eqcmd);
        elecx.set_Xsource(xsource);
        elecx.initialize();

        set_flag_model_initialized_as_true();
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) voltage sensor state: "<<elecx.get_voltage_sensor_state()<<endl
                    <<"(2) voltage reference: "<<elecx.get_voltage_reference_in_pu()<<endl
                    <<"(3) voltage regulator integrator state: "<<elecx.get_voltage_regulator_integrator_state()<<endl
                    <<"(4) voltage regulator first order block state: "<<elecx.get_voltage_regulator_first_order_block_state()<<endl
                    <<"(5) voltage regulator filter state: "<<elecx.get_voltage_regulator_filter_state()<<endl
                    <<"(6) active power sensor state: "<<elecx.get_active_power_sensor_state()<<endl
                    <<"(7) reactive power error integrator state: "<<elecx.get_Q_error_integrator_state()<<endl
                    <<"(8) voltage error integrator state: "<<elecx.get_V_error_integrator_state()<<endl
                    <<"(9) reference speed sensor state: "<<elecx.get_speed_reference_sensor_state()<<endl
                    <<"(10) torque PI regulator state: "<<elecx.get_torque_controller_state()<<endl
                    <<"(11) virtual inertia regulator state: "<<elecx.get_virtual_inertia_controller_state()<<endl
                    <<"(12) frequency droop regulator state: "<<elecx.get_frequency_droop_controller_state()<<endl
                    <<"(13) active power PI controller state: "<<elecx.get_power_pi_controller_state()<<endl
                    <<"(14) reactive voltage command: "<<get_reactive_voltage_command_in_pu()<<endl
                    <<"(15) active current command: "<<get_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(16) reactive current command: "<<get_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(17) active power command: "<<elecx.get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(18) reactive power command: "<<elecx.get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(19) initial torque: "<<elecx.get_initial_Torque()<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESEX::initialize_active_power_control_as_zero()
{
    elecx.initialize_active_power_control_as_zero();
}

void ESEX::run(DYNAMIC_MODE mode)
{
    double iterm = get_energy_storage_terminal_current_in_pu();
    complex<double> selec = get_energy_storage_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();

    double speed     = get_rotor_speed_in_pu();
    double speed_ref = get_rotor_speed_reference_in_pu();

    if(is_active_power_control_activated())
        elecx.activate_active_power_control();
    else
        elecx.deactivate_active_power_control();

    ES_MODE es_mode = get_energy_storage_mode();
    if(es_mode==ES_START_CHARGE_MODE or es_mode == ES_STOP_CHARGE_MODE or es_mode==ES_CHARGE_MODE)
        elecx.set_energy_storage_in_charge_mode();
    else
        elecx.set_energy_storage_in_discharge_mode();

    ENERGY_STORAGE* es= get_energy_storage_pointer();
    ES_AUXILIARY_MODEL*au_model = es->get_es_auxiliary_model();
    if(au_model!=NULL)
    {
        double pref = au_model->get_active_power_reference_in_pu();
        elecx.set_active_power_reference_in_pu_based_on_mbase(pref);
    }

    elecx.set_speed_in_pu(speed);
    elecx.set_speed_reference_in_pu(speed_ref);
    elecx.set_terminal_current_in_pu_based_on_mbase(iterm);
    elecx.set_active_power_generation_in_pu_based_on_mbase(pelec);
    elecx.set_reactive_power_generation_in_pu_based_on_mbase(qelec);
    elecx.run(mode);

    if(mode == DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}


double ESEX::get_frequency_regulation_power_in_pu() const
{
    return elecx.get_frequency_regulation_power_in_pu();
}

double ESEX::get_active_current_command_in_pu_based_on_mbase()
{
    return elecx.get_active_current_command_in_pu_based_on_mbase();
}

double ESEX::get_active_power_command_in_pu_based_on_mbase() const
{
    return elecx.get_active_power_command_in_pu_based_on_mbase();
}

double ESEX::get_reactive_current_command_in_pu_based_on_mbase()
{
    return elecx.get_reactive_current_command_in_pu_based_on_mbase();
}

double ESEX::get_reactive_power_command_in_pu_based_on_mbase()
{
    return elecx.get_reactive_power_command_in_pu_based_on_mbase();
}

double ESEX::get_reactive_voltage_command_in_pu() const
{
    return elecx.get_reactive_voltage_command_in_pu();
}

void ESEX::check()
{
    ;
}

void ESEX::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESEX::save()
{
    ;
}

string ESEX::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    ENERGY_STORAGE* source = get_energy_storage_pointer();
    unsigned int bus = source->get_source_bus();
    string identifier = "'"+source->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    unsigned int bus_reg = get_bus_to_regulate();
    ELEC_REACTIVE_POWER_CONTROL_MODE qmode = get_var_control_mode();
    int var_mode = (qmode==CONSTANT_VAR_MODE)? 0: (qmode==CONSTANT_POWER_FACTOR_MODE? -1 : 1);

    unsigned int voltage_flag = get_voltage_flag();

    ELEC_ACTIVE_POWER_CONTROL_MODE p_mode = get_active_power_control_mode();
    int pe_control = 0;
    if(p_mode==SPEED_CONTROL_MODE) pe_control = 0;
    if(p_mode==ACTIVE_POWER_MODE) pe_control = 1;
    if(p_mode==CONSTANT_TORQUE_MODE) pe_control = 2;

    int loop_flag = (get_active_power_closed_control_flag()==true)? 1: 0;

    double xcomp = get_Xcomp_in_pu();
    double trv = get_TRV_in_s();
    double fn = get_Fn();
    double kiv = get_KIV();
    double qmax = get_Qmax_in_pu();
    double qmin = get_Qmin_in_pu();
    double kpv = get_KPV();
    double tv = get_TV_in_s();
    double tfv = get_TFV_in_s();
    double tp = get_TP_in_s();
    double kqi = get_KQI();
    double vmax = get_Vmax_in_pu();
    double vmin = get_Vmin_in_pu();
    double kqv = get_KQV();
    double eqmax = get_EQmax_in_pu();
    double eqmin = get_EQmin_in_pu();
    double kvi = get_Kvi();
    double tvi = get_Tvi_in_s();
    double kdroop = get_Kdroop();
    double tdroop = get_Tdroop_in_s();
    double fupper = get_frequency_deviation_upper_deadband_in_pu();
    double flower = get_frequency_deviation_lower_deadband_in_pu();
    double kfint = get_Kfint();
    double tspeed = get_Tspeed_in_s();
    double kpp = get_KPP();
    double kip = get_KIP();
    double kpspeed = get_Kpspeed();
    double kispeed= get_Kispeed();
    double rpmax = get_rPmax_in_pu();
    double rpmin = get_rPmin_in_pu();
    double tfp = get_TFP_in_s();
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    double ipmax = get_IPmax_in_pu();
    double kpip = get_Kpip();
    double kiip = get_Kiip();

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
            <<setw(8)<<pe_control<<", "
            <<setw(8)<<loop_flag<<", "
            <<setw(8)<<setprecision(6)<<xcomp<<", "
            <<setw(8)<<setprecision(6)<<trv<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<fn<<", "
            <<setw(8)<<setprecision(6)<<kiv<<", "
            <<setw(8)<<setprecision(6)<<qmax<<", "
            <<setw(8)<<setprecision(6)<<qmin<<", "
            <<setw(8)<<setprecision(6)<<kpv<<", "
            <<setw(8)<<setprecision(6)<<tv<<", "
            <<setw(8)<<setprecision(6)<<tfv<<", "
            <<setw(8)<<setprecision(6)<<tp<<", "
            <<setw(8)<<setprecision(6)<<kqi<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<vmax<<", "
            <<setw(8)<<setprecision(6)<<vmin<<", "
            <<setw(8)<<setprecision(6)<<kqv<<", "
            <<setw(8)<<setprecision(6)<<eqmax<<", "
            <<setw(8)<<setprecision(6)<<eqmin<<", "
            <<setw(8)<<setprecision(6)<<kvi<<", "
            <<setw(8)<<setprecision(6)<<tvi<<", "
            <<setw(8)<<setprecision(6)<<kdroop<<", "
            <<setw(8)<<setprecision(6)<<tdroop<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<fupper<<", "
            <<setw(8)<<setprecision(6)<<flower<<", "
            <<setw(8)<<setprecision(6)<<kfint<<", "
            <<setw(8)<<setprecision(6)<<tspeed<<", "
            <<setw(8)<<setprecision(6)<<kpp<<", "
            <<setw(8)<<setprecision(6)<<kip<<", "
            <<setw(8)<<setprecision(6)<<kpspeed<<", "
            <<setw(8)<<setprecision(6)<<kispeed<<", "
            <<setw(8)<<setprecision(6)<<rpmax<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<rpmin<<", "
            <<setw(8)<<setprecision(6)<<tfp<<", "
            <<setw(8)<<setprecision(6)<<pmax<<", "
            <<setw(8)<<setprecision(6)<<pmin<<", "
            <<setw(8)<<setprecision(6)<<ipmax<<", "
            <<setw(8)<<setprecision(6)<<kpip<<", "
            <<setw(8)<<setprecision(6)<<kiip<<" / ";

    return osstream.str();
}

void ESEX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("BUS TO REGULATE", i); i++;
    add_model_data_name_and_index_pair("ACTIVE POWER CONTROL FLAG", i); i++;
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
    add_model_data_name_and_index_pair("KPIP", i); i++;
    add_model_data_name_and_index_pair("KIIP", i); i++;
    add_model_data_name_and_index_pair("KPSPEED", i); i++;
    add_model_data_name_and_index_pair("KISPEED", i); i++;
}

double ESEX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "BUS TO REGULATE")  return get_bus_to_regulate();
    if(par_name == "ACTIVE POWER CONTROL FLAG")
    {
        ELEC_ACTIVE_POWER_CONTROL_MODE p_mode = get_active_power_control_mode();
        int pe_control = 0;
        if(p_mode==SPEED_CONTROL_MODE_WITH_TORQUE) pe_control = 0;
        if(p_mode==SPEED_CONTROL_MODE) pe_control = 1;
        if(p_mode==ACTIVE_POWER_MODE) pe_control = 2;
        if(p_mode==CONSTANT_TORQUE_MODE) pe_control = 3;
        return pe_control;
    }

    if(par_name == "LOOP CONTROL FLAG")
    {
        int loop_flag = (get_active_power_closed_control_flag()==true)? 1: 0;
        return loop_flag;
    }

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
    if(par_name == "KPIP")                             return get_Kpip();
    if(par_name == "KIIP")                             return get_Kiip();
    if(par_name == "KPSPEED")                          return get_Kpspeed();
    if(par_name == "KISPEED")                          return get_Kispeed();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void ESEX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "BUS TO REGULATE") return set_bus_to_regulate((unsigned int)(value));

    if(par_name == "ACTIVE POWER CONTROL FLAG")
    {
        int ivalue = int(value);
        ELEC_ACTIVE_POWER_CONTROL_MODE pmode = SPEED_CONTROL_MODE_WITH_TORQUE;
        switch(ivalue)
        {
            case 0:
                pmode = SPEED_CONTROL_MODE;
                break;
            case 1:
                pmode = CONSTANT_TORQUE_MODE;
                break;
            case 2:
                pmode = ACTIVE_POWER_MODE;
                break;
            default:
                pmode = ACTIVE_POWER_MODE;
                break;
        }
        return set_active_power_control_mode(pmode);
    }

    if(par_name == "VAR CONTROL FLAG")
    {
        int ivalue1 = int(value);
        ELEC_REACTIVE_POWER_CONTROL_MODE qmode = CONSTANT_VAR_MODE;
        switch(ivalue1)
        {
            case 0:
                qmode = CONSTANT_VAR_MODE;
                break;
            case 1:
                qmode = CONSTANT_VOLTAGE_MODE;
                break;
            case -1:
                qmode = CONSTANT_POWER_FACTOR_MODE;
                break;
            default:
                qmode = CONSTANT_VAR_MODE;
                break;
        }
        return set_var_control_mode(qmode);
    }

    if(par_name == "VOLTAGE FLAG")
    {
        unsigned int flag = (unsigned int)(value);
        return set_voltage_flag(flag);
    }
    if(par_name == "LOOP CONTROL FLAG")
    {
        int flag = (int)(value);
        if(flag==1) return enable_active_power_closed_loop_control();
        if(flag==0) return enable_active_power_open_loop_control();
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
    if(par_name == "KPIP")                             return set_Kpip(value);
    if(par_name == "KIIP")                             return set_Kiip(value);
    if(par_name == "KPSPEED")                          return set_Kpspeed(value);
    if(par_name == "KISPEED")                          return set_Kispeed(value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double ESEX::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_TRV_in_s()!=0.0 and mint>get_TRV_in_s())
        mint = get_TRV_in_s();
    if(get_TV_in_s()!=0.0 and mint>get_TV_in_s())
        mint = get_TV_in_s();
    if(get_TFV_in_s()!=0.0 and mint>get_TFV_in_s())
        mint = get_TFV_in_s();
    if(get_TP_in_s()!=0.0 and mint>get_TP_in_s())
        mint = get_TP_in_s();
    if(get_Tspeed_in_s()!=0.0 and mint>get_Tspeed_in_s())
        mint = get_Tspeed_in_s();
    if(get_Tvi_in_s()!=0.0 and mint>get_Tvi_in_s())
        mint = get_Tvi_in_s();
    if(get_Tdroop_in_s()!=0.0 and mint>get_Tdroop_in_s())
        mint = get_Tdroop_in_s();
    if(get_TFP_in_s()!=0.0 and mint>get_TFP_in_s())
        mint = get_TFP_in_s();
    return mint;
}

void ESEX::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@SPEED REFERENCE SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@TORQUE REGULATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL INERTIA CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PRIMARY FREQUENCY CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SECONDARY FREQUENCY CONTROL", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE POWER PI CONTROLLER", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL INERTIA CONTROL COMMAND", i); i++;
    add_model_internal_variable_name_and_index_pair("PRIMARY FREQUENCY CONTROL COMMAND", i); i++;
    add_model_internal_variable_name_and_index_pair("SECONDARY FREQUENCY CONTROL COMMAND", i); i++;
}

double ESEX::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "STATE@SPEED REFERENCE SENSOR") return elecx.get_speed_reference_sensor_state();
	if(var_name == "STATE@TORQUE REGULATOR") return elecx.get_torque_controller_state();
	if(var_name == "STATE@VIRTUAL INERTIA CONTROL") return elecx.get_virtual_inertia_controller_state();
	if(var_name == "STATE@PRIMARY FREQUENCY CONTROL") return elecx.get_frequency_droop_controller_state();
	if(var_name == "STATE@SECONDARY FREQUENCY CONTROL") return elecx.get_frequency_integral_controller_state();
	if(var_name == "STATE@ACTIVE POWER PI CONTROLLER") return elecx.get_power_pi_controller_state();
	if(var_name == "VIRTUAL INERTIA CONTROL COMMAND") return elecx.get_virtual_inertia_power_deviation_in_pu();
	if(var_name == "PRIMARY FREQUENCY CONTROL COMMAND") return elecx.get_frequency_droop_power_deviation_in_pu();
	if(var_name == "SECONDARY FREQUENCY CONTROL COMMAND") return elecx.get_frequency_integral_power_deviation_in_pu();
    return 0.0;
}

string ESEX::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string ESEX::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESEX::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
