#include "header/model/wtg_models/wt_electrical_model/wt3e0.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

WT3E0::WT3E0(STEPS& toolkit) : WT_ELECTRICAL_MODEL(toolkit),
                               elecx(toolkit)

{
    clear();
}

WT3E0::WT3E0(const WT3E0& model) : WT_ELECTRICAL_MODEL(model.get_toolkit()),
                                   elecx(model.get_toolkit())

{
    copy_from_const_model(model);
}

WT3E0::~WT3E0()
{
    ;
}

WT3E0& WT3E0::operator=(const WT3E0& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void WT3E0::clear()
{
    set_model_float_parameter_count(35);
    elecx.clear();
}

void WT3E0::copy_from_const_model(const WT3E0& model)
{
    if(model.is_model_bypassed()) bypass_model();

    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    elecx.set_toolkit(toolkit);

    clear();


    set_bus_to_regulate(model.get_bus_to_regulate());
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

string WT3E0::get_model_name() const
{
    return "WT3E0";
}

void WT3E0::set_var_control_mode(ELEC_REACTIVE_POWER_CONTROL_MODE mode)
{
    elecx.set_var_control_mode(mode);
}

ELEC_REACTIVE_POWER_CONTROL_MODE WT3E0::get_var_control_mode() const
{
    return elecx.get_var_control_mode();
}

void WT3E0::set_bus_to_regulate(unsigned int bus)
{
    elecx.set_bus_to_regulate(bus);
}

unsigned int WT3E0::get_bus_to_regulate() const
{
    return elecx.get_bus_to_regulate();
}

void WT3E0::set_Xcomp_in_pu(double Xc)
{
    elecx.set_Xcomp_in_pu(Xc);
}

void WT3E0::set_TRV_in_s(double T)
{
    elecx.set_TRV_in_s(T);
}

void WT3E0::set_Fn(double Fn)
{
    elecx.set_Fn(Fn);
}

void WT3E0::set_KIV(double K)
{
    elecx.set_KIV(K);
}

void WT3E0::set_Qmax_in_pu(double q)
{
    elecx.set_Qmax_in_pu(q);
}

void WT3E0::set_Qmin_in_pu(double q)
{
    elecx.set_Qmin_in_pu(q);
}

void WT3E0::set_KPV(double K)
{
    elecx.set_KPV(K);
}

void WT3E0::set_TV_in_s(double T)
{
    elecx.set_TV_in_s(T);
}

void WT3E0::set_TFV_in_s(double T)
{
    elecx.set_TFV_in_s(T);
}

void WT3E0::set_TP_in_s(double T)
{
    elecx.set_TP_in_s(T);
}

void WT3E0::set_KQI(double K)
{
    elecx.set_KQI(K);
}

void WT3E0::set_Vmax_in_pu(double v)
{
    elecx.set_Vmax_in_pu(v);
}

void WT3E0::set_Vmin_in_pu(double v)
{
    elecx.set_Vmin_in_pu(v);
}

void WT3E0::set_voltage_flag(unsigned int flag)
{
    if(flag<3)
        elecx.set_voltage_flag(flag);
    else
    {
        ostringstream osstream;
        osstream<<"Error. "<<flag<<" is not allowed to set up voltage flag for "<<get_model_name()<<" model. 0, 1, or 2 is allowed.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void WT3E0::set_KQV(double K)
{
    elecx.set_KQV(K);
}

void WT3E0::set_EQmax_in_pu(double I)
{
    elecx.set_EQmax_in_pu(I);
}

void WT3E0::set_EQmin_in_pu(double I)
{
    elecx.set_EQmin_in_pu(I);
}

void WT3E0::set_Tspeed_in_s(double T)
{
    elecx.set_Tspeed_in_s(T);
}

void WT3E0::set_KPP(double K)
{
    elecx.set_KPP(K);
}

void WT3E0::set_KIP(double K)
{
    elecx.set_KIP(K);
}

void WT3E0::set_Kvi(double K)
{
    elecx.set_Kvi(K);
}

void WT3E0::set_Tvi_in_s(double T)
{
    elecx.set_Tvi_in_s(T);
}

void WT3E0::set_Kdroop(double K)
{
    elecx.set_Kdroop(K);
}

void WT3E0::set_Tdroop_in_s(double T)
{
    elecx.set_Tdroop_in_s(T);
}

void WT3E0::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    elecx.set_frequency_deviation_upper_deadband_in_pu(f);
}

void WT3E0::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    elecx.set_frequency_deviation_lower_deadband_in_pu(f);
}

void WT3E0::set_Kfint(double K)
{
    elecx.set_Kfint(K);
}

void WT3E0::set_rPmax_in_pu(double r)
{
    elecx.set_rPmax_in_pu(r);
}

void WT3E0::set_rPmin_in_pu(double r)
{
    elecx.set_rPmin_in_pu(r);
}

void WT3E0::set_TFP_in_s(double T)
{
    elecx.set_TFP_in_s(T);
}

void WT3E0::set_Pmax_in_pu(double p)
{
    elecx.set_Pmax_in_pu(p);
}

void WT3E0::set_Pmin_in_pu(double p)
{
    elecx.set_Pmin_in_pu(p);
}

void WT3E0::set_IPmax_in_pu(double I)
{
    elecx.set_IPmax_in_pu(I);
}

double WT3E0::get_Xcomp_in_pu() const
{
    return elecx.get_Xcomp_in_pu();
}

double WT3E0::get_TRV_in_s() const
{
    return elecx.get_TRV_in_s();
}

double WT3E0::get_Fn() const
{
    return elecx.get_Fn();
}

double WT3E0::get_KIV() const
{
    return elecx.get_KIV();
}

double WT3E0::get_Qmax_in_pu() const
{
    return elecx.get_Qmax_in_pu();
}

double WT3E0::get_Qmin_in_pu() const
{
    return elecx.get_Qmin_in_pu();
}

double WT3E0::get_KPV() const
{
    return elecx.get_KPV();
}

double WT3E0::get_TV_in_s() const
{
    return elecx.get_TV_in_s();
}

double WT3E0::get_TFV_in_s() const
{
    return elecx.get_TFV_in_s();
}

double WT3E0::get_TP_in_s() const
{
    return elecx.get_TP_in_s();
}

double WT3E0::get_KQI() const
{
    return elecx.get_KQI();
}

double WT3E0::get_Vmax_in_pu() const
{
    return elecx.get_Vmax_in_pu();
}

double WT3E0::get_Vmin_in_pu() const
{
    return elecx.get_Vmin_in_pu();
}

unsigned int WT3E0::get_voltage_flag() const
{
    return elecx.get_voltage_flag();
}

double WT3E0::get_KQV() const
{
    return elecx.get_KQV();
}

double WT3E0::get_EQmax_in_pu() const
{
    return elecx.get_EQmax_in_pu();
}

double WT3E0::get_EQmin_in_pu() const
{
    return elecx.get_EQmin_in_pu();
}

double WT3E0::get_Tspeed_in_s() const
{
    return elecx.get_Tspeed_in_s();
}

double WT3E0::get_KPP() const
{
    return elecx.get_KPP();
}

double WT3E0::get_KIP() const
{
    return elecx.get_KIP();
}

double WT3E0::get_Kvi() const
{
    return elecx.get_Kvi();
}

double WT3E0::get_Tvi_in_s() const
{
    return elecx.get_Tvi_in_s();
}

double WT3E0::get_Kdroop() const
{
    return elecx.get_Kdroop();
}

double WT3E0::get_Tdroop_in_s() const
{
    return elecx.get_Tdroop_in_s();
}

double WT3E0::get_frequency_deviation_upper_deadband_in_pu() const
{
    return elecx.get_frequency_deviation_upper_deadband_in_pu();
}

double WT3E0::get_frequency_deviation_lower_deadband_in_pu() const
{
    return elecx.get_frequency_deviation_lower_deadband_in_pu();
}

double WT3E0::get_Kfint() const
{
    return elecx.get_Kfint();
}
double WT3E0::get_rPmax_in_pu() const
{
    return elecx.get_rPmax_in_pu();
}

double WT3E0::get_rPmin_in_pu() const
{
    return elecx.get_rPmin_in_pu();
}

double WT3E0::get_TFP_in_s() const
{
    return elecx.get_TFP_in_s();
}

double WT3E0::get_Pmax_in_pu() const
{
    return elecx.get_Pmax_in_pu();
}

double WT3E0::get_Pmin_in_pu() const
{
    return elecx.get_Pmin_in_pu();
}

double WT3E0::get_IPmax_in_pu() const
{
    return elecx.get_IPmax_in_pu();
}

bool WT3E0::setup_model_with_steps_string_vector(vector<string>& data)
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
            ELEC_REACTIVE_POWER_CONTROL_MODE mode;
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

bool WT3E0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3E0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3E0::setup_block_toolkit_and_parameters()
{
    elecx.set_bus_pointer(get_bus_pointer());
    elecx.set_device_pointer(get_device_pointer());
    elecx.set_active_power_control_mode(SPEED_CONTROL_MODE_WITH_TORQUE);
    elecx.enable_active_power_open_loop_control();
}

void WT3E0::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        WT_GENERATOR_MODEL* wtgenmodel = wt_generator->get_wt_generator_model();

        if(not wtgenmodel->is_model_initialized())
            wtgenmodel->initialize();

        WT_AERODYNAMIC_MODEL* aerdmodel = wt_generator->get_wt_aerodynamic_model();
        if(aerdmodel!=NULL)
        {
            if(not aerdmodel->is_model_initialized())
                aerdmodel->initialize();

            WT_TURBINE_MODEL* turbine_model = wt_generator->get_wt_turbine_model();
            if(turbine_model!=NULL)
            {
                if(not turbine_model->is_model_initialized())
                    turbine_model->initialize();


                setup_block_toolkit_and_parameters();

                STEPS& toolkit = get_toolkit();

                double iterm = get_wt_generator_terminal_current_in_pu();
                complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
                double pelec = selec.real();
                double qelec = selec.imag();
                double ipcmd = wtgenmodel->get_initial_active_current_command_in_pu_based_on_mbase();
                double iqcmd = wtgenmodel->get_initial_reactive_current_command_in_pu_based_on_mbase();
                double pcmd = wtgenmodel->get_initial_active_power_command_in_pu_based_on_mbase();
                double qcmd = wtgenmodel->get_initial_reactive_power_command_in_pu_based_on_mbase();
                double eqcmd = wtgenmodel->get_initial_reactive_voltage_command_in_pu();
                double xsource = get_source_impedance_in_pu_based_on_mbase().imag();
                double speed = get_wt_generator_speed_in_pu();
                double speedref = get_wt_generator_speed_reference_in_pu();

                elecx.set_initial_speed(speed);
                elecx.set_speed_reference_in_pu(speedref);
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
                            <<"(10) torque controller state: "<<elecx.get_torque_controller_state()<<endl
                            <<"(11) virtual inertia regulator state: "<<elecx.get_virtual_inertia_controller_state()<<endl
                            <<"(12) frequency droop regulator state: "<<elecx.get_frequency_droop_controller_state()<<endl
                            <<"(13) reactive voltage command: "<<get_reactive_voltage_command_in_pu()<<endl
                            <<"(14) active current command: "<<get_active_current_command_in_pu_based_on_mbase()<<endl
                            <<"(15) reactive current command: "<<get_reactive_current_command_in_pu_based_on_mbase();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
}

void WT3E0::check_limits_in_initialization()
{
    return;
}

void WT3E0::run(DYNAMIC_MODE mode)
{

    double iterm = get_wt_generator_terminal_current_in_pu();

    complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();

    double speed     = get_wt_generator_speed_in_pu();
    double speed_ref = get_wt_generator_speed_reference_in_pu();

    elecx.set_speed_in_pu(speed);
    elecx.set_speed_reference_in_pu(speed_ref);
    elecx.set_terminal_current_in_pu_based_on_mbase(iterm);
    elecx.set_active_power_generation_in_pu_based_on_mbase(pelec);
    elecx.set_reactive_power_generation_in_pu_based_on_mbase(qelec);
    elecx.run(mode);

    if(mode == DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}


double WT3E0::get_active_current_command_in_pu_based_on_mbase()
{
    return elecx.get_active_current_command_in_pu_based_on_mbase();
}

double WT3E0::get_active_power_command_in_pu_based_on_mbase() const
{
    return elecx.get_active_power_command_in_pu_based_on_mbase();
}

double WT3E0::get_reactive_current_command_in_pu_based_on_mbase()
{
    return elecx.get_reactive_current_command_in_pu_based_on_mbase();
}

double WT3E0::get_reactive_power_command_in_pu_based_on_mbase()
{
    return elecx.get_reactive_power_command_in_pu_based_on_mbase();
}

double WT3E0::get_reactive_voltage_command_in_pu() const
{
    return elecx.get_reactive_voltage_command_in_pu();
}

void WT3E0::check()
{
    ;
}

void WT3E0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3E0::save()
{
    ;
}

string WT3E0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    WT_GENERATOR* source = get_wt_generator_pointer();
    unsigned int bus = source->get_source_bus();
    string identifier = "'"+source->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    unsigned int bus_reg = get_bus_to_regulate();
    ELEC_REACTIVE_POWER_CONTROL_MODE mode = get_var_control_mode();
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

void WT3E0::prepare_model_data_table()
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

double WT3E0::get_model_data_with_name(string par_name) const
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

void WT3E0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "BUS TO REGULATE") return set_bus_to_regulate((unsigned int)(value));

    if(par_name == "VAR CONTROL FLAG")
    {
        int ivalue = int(value);
        ELEC_REACTIVE_POWER_CONTROL_MODE mode = CONSTANT_VAR_MODE;
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

double WT3E0::get_minimum_nonzero_time_constant_in_s()
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

void WT3E0::prepare_model_internal_variable_table()
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

double WT3E0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "STATE@SPEED REFERENCE SENSOR") return elecx.get_speed_reference_sensor_state();
	if(var_name == "STATE@TORQUE REGULATOR") return elecx.get_torque_controller_state();
	if(var_name == "STATE@VIRTUAL INERTIA CONTROL") return elecx.get_virtual_inertia_controller_state();
	if(var_name == "STATE@PRIMARY FREQUENCY CONTROL") return elecx.get_frequency_droop_controller_state();
	if(var_name == "STATE@SECONDARY FREQUENCY CONTROL") return elecx.get_frequency_integral_controller_state();
	if(var_name == "VIRTUAL INERTIA CONTROL COMMAND") return elecx.get_virtual_inertia_controller_state();
	if(var_name == "PRIMARY FREQUENCY CONTROL COMMAND") return elecx.get_frequency_droop_controller_state();
	if(var_name == "SECONDARY FREQUENCY CONTROL COMMAND") return elecx.get_frequency_integral_controller_state();
    return 0.0;
}

string WT3E0::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string WT3E0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3E0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
