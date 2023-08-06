#include "header/model/wtg_models/wt_electrical_model/wt3e0.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

WT3E0::WT3E0(STEPS& toolkit) : WT_ELECTRICAL_MODEL(toolkit),
                               voltage_sensor(toolkit),
                               voltage_regulator_first_order_block(toolkit),
                               voltage_regulator_integrator(toolkit),
                               voltage_regulator_filter(toolkit),
                               active_power_sensor(toolkit),
                               Q_error_integrator(toolkit),
                               V_error_integrator(toolkit),
                               wind_turbine_speed_reference_sensor(toolkit),
                               torque_PI_regulator(toolkit),
                               virtual_inertia_emulator(toolkit),
                               frequency_droop_controller(toolkit),
                               frequency_integral_controller(toolkit),
                               power_order_integrator(toolkit)
{
    clear();
}

WT3E0::WT3E0(const WT3E0& model) : WT_ELECTRICAL_MODEL(model.get_toolkit()),
                                   voltage_sensor(model.get_toolkit()),
                                   voltage_regulator_first_order_block(model.get_toolkit()),
                                   voltage_regulator_integrator(model.get_toolkit()),
                                   voltage_regulator_filter(model.get_toolkit()),
                                   active_power_sensor(model.get_toolkit()),
                                   Q_error_integrator(model.get_toolkit()),
                                   V_error_integrator(model.get_toolkit()),
                                   wind_turbine_speed_reference_sensor(model.get_toolkit()),
                                   torque_PI_regulator(model.get_toolkit()),
                                   virtual_inertia_emulator(model.get_toolkit()),
                                   frequency_droop_controller(model.get_toolkit()),
                                   frequency_integral_controller(model.get_toolkit()),
                                   power_order_integrator(model.get_toolkit())
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

    set_voltage_flag(0);

    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    power_order_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    frequency_integral_controller.set_limiter_type(NO_LIMITER);

    frequency_regulation_enabled = true;
}

void WT3E0::copy_from_const_model(const WT3E0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    voltage_sensor.set_toolkit(toolkit);
    voltage_regulator_first_order_block.set_toolkit(toolkit);
    voltage_regulator_integrator.set_toolkit(toolkit);
    voltage_regulator_filter.set_toolkit(toolkit);
    active_power_sensor.set_toolkit(toolkit);
    Q_error_integrator.set_toolkit(toolkit);
    V_error_integrator.set_toolkit(toolkit);

    wind_turbine_speed_reference_sensor.set_toolkit(toolkit);
    torque_PI_regulator.set_toolkit(toolkit);
    virtual_inertia_emulator.set_toolkit(toolkit);
    frequency_droop_controller.set_toolkit(toolkit);

    frequency_integral_controller.set_toolkit(toolkit);
    power_order_integrator.set_toolkit(toolkit);

    clear();

    set_bus_to_regulate(model.get_bus_to_regulate());
    set_var_control_mode(model.get_var_control_mode());
    set_wind_turbine_power_speed_lookup_table(model.get_wind_turbine_power_speed_lookup_table());

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

void WT3E0::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void WT3E0::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void WT3E0::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void WT3E0::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void WT3E0::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void WT3E0::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void WT3E0::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void WT3E0::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void WT3E0::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void WT3E0::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void WT3E0::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void WT3E0::set_voltage_flag(unsigned int flag)
{
    if(flag<3)
        Voltage_Flag = flag;
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
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void WT3E0::set_EQmin_in_pu(double I)
{
    EQmin = I;
}

void WT3E0::set_Tspeed_in_s(double T)
{
    wind_turbine_speed_reference_sensor.set_T_in_s(T);
}

void WT3E0::set_KPP(double K)
{
    torque_PI_regulator.set_Kp(K);
}

void WT3E0::set_KIP(double K)
{
    torque_PI_regulator.set_Ki(K);
}

void WT3E0::set_Kvi(double K)
{
    virtual_inertia_emulator.set_K(K);
}

void WT3E0::set_Tvi_in_s(double T)
{
    virtual_inertia_emulator.set_T_in_s(T);
}

void WT3E0::set_Kdroop(double K)
{
    frequency_droop_controller.set_K(K);
}

void WT3E0::set_Tdroop_in_s(double T)
{
    frequency_droop_controller.set_T_in_s(T);
}

void WT3E0::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    f_upper_pu = f;
}

void WT3E0::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    f_lower_pu = f;
}

void WT3E0::set_Kfint(double K)
{
    double t = 0.0;
    if(fabs(K)<DOUBLE_EPSILON)
        t = INFINITE_THRESHOLD;
    else
        t = 1.0/K;
    frequency_integral_controller.set_T_in_s(t);
}
void WT3E0::set_rPmax_in_pu(double r)
{
    max_torque_rate = r;
}

void WT3E0::set_rPmin_in_pu(double r)
{
    min_torque_rate = r;
}

void WT3E0::set_TFP_in_s(double T)
{
    power_order_integrator.set_T_in_s(T);
}

void WT3E0::set_Pmax_in_pu(double p)
{
    power_order_integrator.set_upper_limit(p);
}

void WT3E0::set_Pmin_in_pu(double p)
{
    power_order_integrator.set_lower_limit(p);
}

void WT3E0::set_IPmax_in_pu(double I)
{
    IPmax = I;
}

double WT3E0::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double WT3E0::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double WT3E0::get_Fn() const
{
    return Fn;
}

double WT3E0::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double WT3E0::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double WT3E0::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double WT3E0::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double WT3E0::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double WT3E0::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double WT3E0::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double WT3E0::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double WT3E0::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double WT3E0::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

unsigned int WT3E0::get_voltage_flag() const
{
    return Voltage_Flag;
}

double WT3E0::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double WT3E0::get_EQmax_in_pu() const
{
    return EQmax;
}

double WT3E0::get_EQmin_in_pu() const
{
    return EQmin;
}

double WT3E0::get_Tspeed_in_s() const
{
    return wind_turbine_speed_reference_sensor.get_T_in_s();
}

double WT3E0::get_KPP() const
{
    return torque_PI_regulator.get_Kp();
}

double WT3E0::get_KIP() const
{
    return torque_PI_regulator.get_Ki();
}

double WT3E0::get_Kvi() const
{
    return virtual_inertia_emulator.get_K();
}

double WT3E0::get_Tvi_in_s() const
{
    return virtual_inertia_emulator.get_T_in_s();
}

double WT3E0::get_Kdroop() const
{
    return frequency_droop_controller.get_K();
}

double WT3E0::get_Tdroop_in_s() const
{
    return frequency_droop_controller.get_T_in_s();
}

double WT3E0::get_frequency_deviation_upper_deadband_in_pu() const
{
    return f_upper_pu;
}

double WT3E0::get_frequency_deviation_lower_deadband_in_pu() const
{
    return f_lower_pu;
}

double WT3E0::get_Kfint() const
{
    double t = frequency_integral_controller.get_T_in_s();
    double k = 0.0;
    if(fabs(t-INFINITE_THRESHOLD)<DOUBLE_EPSILON)
        k = 0.0;
    else
        k = 1.0/t;
    return k;
}
double WT3E0::get_rPmax_in_pu() const
{
    return max_torque_rate;
}

double WT3E0::get_rPmin_in_pu() const
{
    return min_torque_rate;
}

double WT3E0::get_TFP_in_s() const
{
    return power_order_integrator.get_T_in_s();
}

double WT3E0::get_Pmax_in_pu() const
{
    return power_order_integrator.get_upper_limit();
}

double WT3E0::get_Pmin_in_pu() const
{
    return power_order_integrator.get_lower_limit();
}

double WT3E0::get_IPmax_in_pu() const
{
    return IPmax;
}

bool WT3E0::is_frequency_regulation_enabled() const
{
    return frequency_regulation_enabled;
}
void WT3E0::trip_frequency_regulation()
{
    if(frequency_regulation_enabled==true)
        frequency_regulation_enabled = false;
}

void WT3E0::set_speed_reference_bias_in_pu(double bias)
{
    speedref_bias =  bias;
}

double WT3E0::get_speed_reference_bias_in_pu() const
{
    return speedref_bias;
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

                double vterm = get_terminal_bus_voltage_in_pu();
                double iterm = get_wt_generator_terminal_current_in_pu();
                //double mbase = get_mbase_in_MVA();
                complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
                double pelec = selec.real();
                double qelec = selec.imag();
                double speed = get_wt_generator_speed_in_pu();

                double ipcmd = wtgenmodel->get_initial_active_current_command_in_pu_based_on_mbase();
                double ipmax = get_IPmax_in_pu();
                if(ipcmd>ipmax)
                {
                    osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                      <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                double porder = ipcmd*vterm;
                double pmax = get_Pmax_in_pu();
                double pmin = get_Pmin_in_pu();
                power_order_integrator.set_output(porder);
                if(porder>pmax)
                {
                    osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                      <<"Porder is "<<porder<<", and Pmax is "<<pmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(porder<pmin)
                {
                    osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                      <<"Porder is "<<porder<<", and Pmin is "<<pmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                power_order_integrator.initialize();

                double torque =  porder/speed;
                torque_PI_regulator.set_output(torque);
                torque_PI_regulator.initialize();

                virtual_inertia_emulator.set_input(0.0);
                virtual_inertia_emulator.initialize();

                frequency_droop_controller.set_output(0.0);
                frequency_droop_controller.initialize();

                frequency_integral_controller.set_output(0.0);
                frequency_integral_controller.initialize();

                double speedref = get_wt_generator_speed_referance_in_pu();
                //set_speed_reference_bias_in_pu(speedref-speed);
                wind_turbine_speed_reference_sensor.set_output(speedref);
                wind_turbine_speed_reference_sensor.initialize();


                double iqcmd = wtgenmodel->get_initial_reactive_current_command_in_pu_based_on_mbase();
                double xsource = get_source_impedance_in_pu_based_on_mbase().imag();
                double eqcmd = iqcmd*(-xsource);

                double verror = 0.0;
                unsigned int vflag = get_voltage_flag();
                if(vflag == 0)
                {
                    V_error_integrator.set_output(0.0);
                    V_error_integrator.initialize();

                    verror = eqcmd;
                }
                else
                {
                    if(vflag == 1)
                    {
                        V_error_integrator.set_upper_limit(vterm+get_EQmax_in_pu());
                        V_error_integrator.set_lower_limit(vterm+get_EQmin_in_pu());
                    }
                    else
                    {
                        V_error_integrator.set_upper_limit(get_EQmax_in_pu());
                        V_error_integrator.set_lower_limit(get_EQmin_in_pu());
                    }
                    double vmax = V_error_integrator.get_upper_limit();
                    double vmin = V_error_integrator.get_lower_limit();

                    V_error_integrator.set_output(eqcmd);
                    if(eqcmd>vmax)
                    {
                        osstream<<"Initialization error. Eqcmd (reactive voltage command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                          <<"Eqcmd is "<<eqcmd<<", and Vmax is "<<vmax<<" for voltage flag = "<<vflag;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    if(eqcmd<vmin)
                    {
                        osstream<<"Initialization error. Eqcmd (reactive voltage command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                          <<"Eqcmd is "<<eqcmd<<", and Vmin is "<<vmin<<" for voltage flag = "<<vflag;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    V_error_integrator.initialize();
                    verror = 0.0;
                }

                double vcmd = verror+vterm;
                double vmax = get_Vmax_in_pu();
                double vmin = get_Vmin_in_pu();
                Q_error_integrator.set_output(vcmd);
                if(vcmd>vmax)
                {
                    osstream<<"Initialization error. Vcmd (voltage command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                      <<"Vcmd is "<<vcmd<<", and Vmax is "<<vmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(vcmd<vmin)
                {
                    osstream<<"Initialization error. Vcmd (voltage command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                      <<"Vcmd is "<<vcmd<<", and Vmin is "<<vmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                Q_error_integrator.initialize();

                double qcmd = qelec;
                double qmax = get_Qmax_in_pu();
                double qmin = get_Qmin_in_pu();
                if(qcmd>qmax)
                {
                    osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                            <<"Qcmd is "<<qcmd<<", and Qmax is "<<qmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(qcmd<qmin)
                {
                    osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                            <<"Qcmd is "<<qcmd<<", and Qmin is "<<qmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                set_reactive_power_reference_in_pu(qcmd);

                active_power_sensor.set_output(pelec);
                active_power_sensor.initialize();

                double smag = steps_fast_complex_abs(selec);
                double pf = fabs(pelec) / smag;
                pf = (qelec/pelec>0? pf:-pf);
                set_power_factor_reference_in_pu(pf);

                voltage_regulator_filter.set_output(qcmd);
                voltage_regulator_filter.initialize();

                voltage_regulator_integrator.set_output(qcmd);
                voltage_regulator_integrator.initialize();

                voltage_regulator_first_order_block.set_output(0.0);
                voltage_regulator_first_order_block.initialize();

                double xcomp = get_Xcomp_in_pu();
                double vref = vterm+iterm*xcomp;

                voltage_sensor.set_output(vref);
                voltage_sensor.initialize();

                set_voltage_reference_in_pu(vref);
                //show_information_with_leading_time_stamp(osstream);

                set_flag_model_initialized_as_true();
                if(toolkit.is_detailed_log_enabled())
                {
                    osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                            <<"(1) voltage sensor state: "<<voltage_sensor.get_state()<<endl
                            <<"(2) voltage reference: "<< get_voltage_reference_in_pu()<<endl
                            <<"(3) voltage regulator integrator state: "<<voltage_regulator_integrator.get_state()<<endl
                            <<"(4) voltage regulator first order block state: "<<voltage_regulator_first_order_block.get_state()<<endl
                            <<"(5) voltage regulator filter state: "<<voltage_regulator_filter.get_state()<<endl
                            <<"(6) active power sensor state: "<<active_power_sensor.get_state()<<endl
                            <<"(7) reactive power error integrator state: "<<Q_error_integrator.get_state()<<endl
                            <<"(8) voltage error integrator state: "<<V_error_integrator.get_state()<<endl
                            <<"(9) reference speed sensor state: "<<wind_turbine_speed_reference_sensor.get_state()<<endl
                            <<"(10) torque PI regulator state: "<<torque_PI_regulator.get_state()<<endl
                            <<"(11) virtual inertia regulator state: "<<virtual_inertia_emulator.get_state()<<endl
                            <<"(12) frequency droop regulator state: "<<frequency_droop_controller.get_state()<<endl
                            <<"(13) reactive voltage command: "<<get_reactive_voltage_command_in_pu()<<endl
                            <<"(14) active current command: "<<get_active_current_command_in_pu_based_on_mbase()<<endl
                            <<"(15) reactive current command: "<<get_reactive_current_command_in_pu_based_on_mbase();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
}

void WT3E0::run(DYNAMIC_MODE mode)
{
    double vterm = get_terminal_bus_voltage_in_pu();
    double iterm = get_wt_generator_terminal_current_in_pu();
    double freq = get_terminal_bus_frequency_deviation_in_pu();
    //double mbase = get_mbase_in_MVA();
    complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();

    double speed     = get_wt_generator_speed_in_pu();
    double speed_ref = get_wt_generator_speed_referance_in_pu();
    //double speedref_bias = get_speed_reference_bias_in_pu();

    wind_turbine_speed_reference_sensor.set_input(speed_ref);
    wind_turbine_speed_reference_sensor.run(mode);
    //osstream<<"wind_turbine_speed_reference_sensor input = "<<speed_ref<<", output = "<<wind_turbine_speed_reference_sensor.get_output()<<endl;

    //double input = speed + speedref_bias - wind_turbine_speed_reference_sensor.get_output();
    double input = speed - wind_turbine_speed_reference_sensor.get_output();
    //osstream << "at time " << toolkit.get_dynamic_simulation_time_in_s()<< "s, speed error is: " << input << endl;
    //show_information_with_leading_time_stamp(osstream);

    torque_PI_regulator.set_input(input);
    torque_PI_regulator.run(mode);
    //osstream << "at time " << toolkit.get_dynamic_simulation_time_in_s()<< "s, speed error PI regulator is: " << torque_PI_regulator.get_output() << endl;
    //show_information_with_leading_time_stamp(osstream);
    //osstream<<"torque_PI_regulator input = "<<input<<", output = "<<torque_PI_regulator.get_output()<<endl;

    virtual_inertia_emulator.set_input(freq);
    virtual_inertia_emulator.run(mode);
    //osstream<<"virtual_inertia_emulator input = "<<-freq<<", output = "<<virtual_inertia_emulator.get_output()<<endl;

    frequency_droop_controller.set_input(freq);
    frequency_droop_controller.run(mode);
    //osstream<<"frequency_droop_controller input = "<<-freq<<", output = "<<frequency_droop_controller.get_output()<<endl;

    double fupper = get_frequency_deviation_upper_deadband_in_pu();
    double flower = get_frequency_deviation_lower_deadband_in_pu();
    double f_int =  0;
    if(freq>=flower and freq<=fupper)
        ;
    else
    {
        if(freq>fupper)
            f_int = freq - fupper;
        else
            f_int = freq - flower;
    }
    frequency_integral_controller.set_input(f_int);
    frequency_integral_controller.run(mode);

    //osstream<<"speed = "<<speed<<endl;
/*    if(mode==UPDATE_MODE and is_frequency_regulation_enabled())
    {
        WT_AERODYNAMIC_MODEL* aerd = wt_generator->get_wt_aerodynamic_model();
        double wmin = aerd->get_min_steady_state_turbine_speed_in_pu();
        double wmax = aerd->get_max_steady_state_turbine_speed_in_pu();
        WT_TURBINE_MODEL* turb = wt_generator->get_wt_turbine_model();
        double w = turb->get_generator_speed_in_pu();
        if(w<wmin or w>wmax*1.1)
        {
            osstream<<"Frequency regulation logic of "<<get_compound_device_name()<<" is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<"s due to ";
            if(w<wmin)
                osstream<<"rotor w<wmin: "<<w<<"<"<<wmin<<" pu";
            else
                osstream<<"rotor w>wmax*1.1: "<<w<<">"<<wmax*1.1<<" pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
            trip_frequency_regulation();
        }
    }*/
    if(is_frequency_regulation_enabled())
    {
        input = torque_PI_regulator.get_output()*speed
                -power_order_integrator.get_output()
                -(virtual_inertia_emulator.get_output()
                  +frequency_droop_controller.get_output()
                  +frequency_integral_controller.get_output());
    }
    else
    {
        input = torque_PI_regulator.get_output()*speed
                -power_order_integrator.get_output();
    }
    //osstream << "at time " << toolkit.get_dynamic_simulation_time_in_s()<< "s, active power rate is: " << input << endl;
    //show_information_with_leading_time_stamp(osstream);
    if(input>get_rPmax_in_pu())
        input = get_rPmax_in_pu();
    if(input<get_rPmin_in_pu())
        input = get_rPmin_in_pu();

    //osstream<<"At time "<<toolkit.get_dynamic_simulation_time_in_s()<<", WT3E0 power_order_integrator input is "<<input;
    //show_information_with_leading_time_stamp(osstream);
    power_order_integrator.set_input(input);
    power_order_integrator.run(mode);
    //osstream << "at time " << toolkit.get_dynamic_simulation_time_in_s()<< "s, active power order is: " << power_order_integrator.get_output() << endl;
    //show_information_with_leading_time_stamp(osstream);
    //osstream<<"power_order_integrator input = "<<input<<", output = "<<power_order_integrator.get_output()<<endl;

    active_power_sensor.set_input(pelec);
    active_power_sensor.run(mode);
    //osstream<<"active_power_sensor input = "<<pelec<<", output = "<<active_power_sensor.get_output()<<endl;

    double xcomp = get_Xcomp_in_pu();
    input = vterm+iterm*xcomp;
    voltage_sensor.set_input(input);
    voltage_sensor.run(mode);
    //osstream<<"At time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
    //osstream<<"voltage_sensor input = "<<setw(20)<<setprecision(19)<<fixed<<input<<", output = "<<voltage_sensor.get_output()<<endl;

    input = get_voltage_reference_in_pu()-voltage_sensor.get_output();
    //osstream<<"input = "<<input<<endl;
    input /= get_Fn();

    voltage_regulator_integrator.set_input(input);
    voltage_regulator_integrator.run(mode);
    //osstream<<"voltage regulator integrator input = "<<input<<", output = "<<voltage_regulator_integrator.get_output()<<endl;

    voltage_regulator_first_order_block.set_input(input);
    voltage_regulator_first_order_block.run(mode);
    //osstream<<"voltage regulator first order block input = "<<input<<", output = "<<voltage_regulator_first_order_block.get_output()<<endl;

    input = voltage_regulator_integrator.get_output() + voltage_regulator_first_order_block.get_output();
    //osstream<<"voltage_regulator_filter input = "<<input<<", output = "<<voltage_regulator_filter.get_output()<<endl;
    voltage_regulator_filter.set_input(input);
    voltage_regulator_filter.run(mode);
    //osstream<<"voltage_regulator_filter input = "<<input<<", output = "<<voltage_regulator_filter.get_output()<<endl;

    PE_VAR_CONTROL_MODE var_mode = get_var_control_mode();
    double qcmd = 0.0;
    if(var_mode==CONSTANT_VAR_MODE)
        qcmd = get_reactive_power_reference_in_pu();
    else
    {
        if(var_mode==CONSTANT_POWER_FACTOR_MODE)
        {
            double pf = get_power_factor_reference_in_pu();
            qcmd = steps_sqrt(1.0-pf*pf)/pf*active_power_sensor.get_output();
        }
        else
            qcmd = voltage_regulator_filter.get_output();
    }
    if(qcmd>get_Qmax_in_pu())
        qcmd = get_Qmax_in_pu();
    if(qcmd<get_Qmin_in_pu())
        qcmd = get_Qmin_in_pu();
    //osstream<<"Q command = "<<qcmd<<endl;

    input = qcmd - qelec;
    Q_error_integrator.set_input(input);
    Q_error_integrator.run(mode);
    //osstream<<"Q_error_integrator input = "<<input<<", output = "<<Q_error_integrator.get_output()<<endl;

    unsigned int vflag = get_voltage_flag();
    if(vflag == 1 or vflag == 2)
    {
        if(vflag == 1)
        {
            V_error_integrator.set_upper_limit(vterm+get_EQmax_in_pu());
            V_error_integrator.set_lower_limit(vterm+get_EQmin_in_pu());
        }
        else //vflag == 2
        {
            V_error_integrator.set_upper_limit(get_EQmax_in_pu());
            V_error_integrator.set_lower_limit(get_EQmin_in_pu());
        }
        input = Q_error_integrator.get_output()-vterm;
        V_error_integrator.set_input(input);
        V_error_integrator.run(mode);
        //osstream<<"V_error_integrator input = "<<input<<", output = "<<V_error_integrator.get_output()<<endl;
    }
    //show_information_with_leading_time_stamp(osstream);

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}


double WT3E0::get_active_current_command_in_pu_based_on_mbase()
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;

    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    double ipcmd = pcmd/vterm;
    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
        ipcmd = ipmax;
    return ipcmd;
}

double WT3E0::get_active_power_command_in_pu_based_on_mbase() const
{
    return power_order_integrator.get_output();
}

double WT3E0::get_reactive_current_command_in_pu_based_on_mbase()
{
    return 0.0;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    double xeq = gen->get_source_impedance_in_pu().imag();

    double eqcmd = get_reactive_voltage_command_in_pu();

    return eqcmd/(-xeq);
}

double WT3E0::get_reactive_power_command_in_pu_based_on_mbase()
{
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
    switch(mode)
    {
        case CONSTANT_VAR_MODE:
        {
            return get_reactive_power_reference_in_pu();
        }
        case CONSTANT_POWER_FACTOR_MODE:
        {
            double pf = get_power_factor_reference_in_pu();
            double p = active_power_sensor.get_output();

            return p*steps_sqrt(1.0-pf*pf)/pf;
        }
        case CONSTANT_VOLTAGE_MODE:
        {
            return voltage_regulator_filter.get_output();
        }
        default:
            return 0.0;
    }
}

double WT3E0::get_reactive_voltage_command_in_pu() const
{
    unsigned int voltage_flag = get_voltage_flag();
    if(voltage_flag==0)
    {
        double vterm = get_terminal_bus_voltage_in_pu();
        return Q_error_integrator.get_output()-vterm;
    }
    else
    {
        return V_error_integrator.get_output();
    }
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
