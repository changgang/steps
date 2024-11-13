#include "header/model/common_model/elec_model/elecx.h"

ELECX::ELECX(STEPS& toolkit) : ELEC_MODEL(toolkit),
                               voltage_sensor(toolkit),
                               voltage_regulator_first_order_block(toolkit),
                               voltage_regulator_integrator(toolkit),
                               voltage_regulator_filter(toolkit),
                               active_power_sensor(toolkit),
                               Q_error_integrator(toolkit),
                               V_error_integrator(toolkit),
                               frequency_controller(toolkit),
                               speed_reference_sensor(toolkit),
                               speed_controller(toolkit),
                               torque_controller(toolkit),
                               Vdc_controller(toolkit),
                               power_closed_loop_controller(toolkit),
                               power_order_integrator(toolkit)
{
    clear();
}

ELECX::ELECX(const ELECX& model) : ELEC_MODEL(model.get_toolkit()),
                                   voltage_sensor(model.get_toolkit()),
                                   voltage_regulator_first_order_block(model.get_toolkit()),
                                   voltage_regulator_integrator(model.get_toolkit()),
                                   voltage_regulator_filter(model.get_toolkit()),
                                   active_power_sensor(model.get_toolkit()),
                                   Q_error_integrator(model.get_toolkit()),
                                   V_error_integrator(model.get_toolkit()),
                                   frequency_controller(model.get_toolkit()),
                                   speed_reference_sensor(model.get_toolkit()),
                                   speed_controller(model.get_toolkit()),
                                   torque_controller(model.get_toolkit()),
                                   Vdc_controller(model.get_toolkit()),
                                   power_closed_loop_controller(model.get_toolkit()),
                                   power_order_integrator(model.get_toolkit())
{
    copy_from_const_model(model);
}

ELECX::~ELECX()
{
    ;
}

ELECX& ELECX::operator=(const ELECX& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void ELECX::clear()
{
    set_voltage_flag(0);

    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    power_order_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    speed_reference_sensor.set_limiter_type(NO_LIMITER);
    speed_controller.set_limiter_type(NO_LIMITER);
    torque_controller.set_limiter_type(NO_LIMITER);
    Vdc_controller.set_limiter_type(NO_LIMITER);
    power_closed_loop_controller.set_limiter_type(NO_LIMITER);

    frequency_controller.clear();
    enable_active_power_open_loop_control();

}

void ELECX::copy_from_const_model(const ELECX& model)
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
    power_order_integrator.set_toolkit(toolkit);
    frequency_controller.set_toolkit(toolkit);

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

    set_rPmax_in_pu(model.get_rPmax_in_pu());
    set_rPmin_in_pu(model.get_rPmin_in_pu());
    set_TFP_in_s(model.get_TFP_in_s());
    set_Pmax_in_pu(model.get_Pmax_in_pu());
    set_Pmin_in_pu(model.get_Pmin_in_pu());
    set_IPmax_in_pu(model.get_IPmax_in_pu());

    set_Kvi(model.get_Kvi());
    set_Tvi_in_s(model.get_Tvi_in_s());
    set_Kdroop(model.get_Kdroop());
    set_Tdroop_in_s(model.get_Tdroop_in_s());
    set_Kfint(model.get_Kfint());
    set_frequency_deviation_lower_deadband_in_pu(model.get_frequency_deviation_lower_deadband_in_pu());
    set_frequency_deviation_upper_deadband_in_pu(model.get_frequency_deviation_upper_deadband_in_pu());


}

string ELECX::get_model_name() const
{
    return "ELECX";
}

void ELECX::set_var_control_mode(ELEC_REACTIVE_POWER_CONTROL_MODE mode)
{
    this->pe_var_control_mode = mode;
}

ELEC_REACTIVE_POWER_CONTROL_MODE ELECX::get_var_control_mode() const
{
    return this->pe_var_control_mode;
}

void ELECX::set_active_power_control_mode(ELEC_ACTIVE_POWER_CONTROL_MODE mode)
{
    this->pe_control_mode = mode;
}

ELEC_ACTIVE_POWER_CONTROL_MODE ELECX::get_active_power_control_mode() const
{
    return this->pe_control_mode;
}

void ELECX::enable_active_power_closed_loop_control()
{
    this->active_power_closed_control_flag = true;
}

void ELECX::enable_active_power_open_loop_control()
{
    this->active_power_closed_control_flag = false;
}

bool ELECX::get_active_power_closed_control_flag() const
{
    return active_power_closed_control_flag;
}

void ELECX::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void ELECX::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void ELECX::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void ELECX::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void ELECX::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void ELECX::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void ELECX::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void ELECX::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void ELECX::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void ELECX::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void ELECX::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void ELECX::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void ELECX::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void ELECX::set_voltage_flag(unsigned int flag)
{
    Voltage_Flag = flag;
}

void ELECX::set_KQV(double K)
{
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void ELECX::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void ELECX::set_EQmin_in_pu(double I)
{
    EQmin = I;
}

double ELECX::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double ELECX::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double ELECX::get_Fn() const
{
    return Fn;
}

double ELECX::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double ELECX::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double ELECX::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double ELECX::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double ELECX::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double ELECX::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double ELECX::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double ELECX::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double ELECX::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double ELECX::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

unsigned int ELECX::get_voltage_flag() const
{
    return Voltage_Flag;
}

double ELECX::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double ELECX::get_EQmax_in_pu() const
{
    return EQmax;
}

double ELECX::get_EQmin_in_pu() const
{
    return EQmin;
}

void ELECX::set_Kvi(double K)
{
    frequency_controller.set_Kvi(K);
}

void ELECX::set_Tvi_in_s(double T)
{
    frequency_controller.set_Tvi_in_s(T);
}

void ELECX::set_Kdroop(double K)
{
    frequency_controller.set_Kdroop(K);
}

void ELECX::set_Tdroop_in_s(double T)
{
    frequency_controller.set_Tdroop_in_s(T);
}

void ELECX::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    frequency_controller.set_frequency_deviation_upper_deadband_in_pu(f);
}

void ELECX::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    frequency_controller.set_frequency_deviation_lower_deadband_in_pu(f);
}

void ELECX::set_Kfint(double K)
{
    frequency_controller.set_Kfint(K);
}

double ELECX::get_Kvi() const
{
    return frequency_controller.get_Kvi();
}

double ELECX::get_Tvi_in_s() const
{
    return frequency_controller.get_Tvi_in_s();
}

double ELECX::get_Kdroop() const
{
    return frequency_controller.get_Kdroop();
}

double ELECX::get_Tdroop_in_s() const
{
    return frequency_controller.get_Tdroop_in_s();
}

double ELECX::get_frequency_deviation_upper_deadband_in_pu() const
{
    return frequency_controller.get_frequency_deviation_upper_deadband_in_pu();
}

double ELECX::get_frequency_deviation_lower_deadband_in_pu() const
{
    return frequency_controller.get_frequency_deviation_lower_deadband_in_pu();
}

double ELECX::get_Kfint() const
{
    return frequency_controller.get_Kfint();
}

void ELECX::set_Tspeed_in_s(double T)
{
    speed_reference_sensor.set_T_in_s(T);
}

void ELECX::set_KPP(double K)
{
    torque_controller.set_Kp(K);
}

void ELECX::set_KIP(double K)
{
    torque_controller.set_Ki(K);
}

double ELECX::get_Tspeed_in_s() const
{
    return speed_reference_sensor.get_T_in_s();
}

double ELECX::get_KPP() const
{
    return torque_controller.get_Kp();
}

double ELECX::get_KIP() const
{
    return torque_controller.get_Ki();
}

void ELECX::set_Kpspeed(double K)
{
    speed_controller.set_Kp(K);
}

void ELECX::set_Kispeed(double K)
{
    speed_controller.set_Ki(K);
}

double ELECX::get_Kpspeed() const
{
    return speed_controller.get_Kp();
}

double ELECX::get_Kispeed() const
{
    return speed_controller.get_Ki();
}

void ELECX::set_Kpdc(double K)
{
    Vdc_controller.set_Kp(K);
}

void ELECX::set_Kidc(double K)
{
    Vdc_controller.set_Ki(K);
}

double ELECX::get_Kpdc() const
{
    return Vdc_controller.get_Kp();
}

double ELECX::get_Kidc() const
{
    return Vdc_controller.get_Ki();
}

void ELECX::set_rPmax_in_pu(double r)
{
    max_Pcmd_rate = r;
}

void ELECX::set_rPmin_in_pu(double r)
{
    min_Pcmd_rate = r;
}

void ELECX::set_TFP_in_s(double T)
{
    power_order_integrator.set_T_in_s(T);
}

void ELECX::set_Pmax_in_pu(double p)
{
    power_order_integrator.set_upper_limit(p);
}

void ELECX::set_Pmin_in_pu(double p)
{
    power_order_integrator.set_lower_limit(p);
}

void ELECX::set_IPmax_in_pu(double I)
{
    IPmax = I;
}

void ELECX::set_Kpip(double K)
{
    power_closed_loop_controller.set_Kp(K);
}

void ELECX::set_Kiip(double K)
{
    power_closed_loop_controller.set_Ki(K);
}

double ELECX::get_rPmax_in_pu() const
{
    return max_Pcmd_rate;
}

double ELECX::get_rPmin_in_pu() const
{
    return min_Pcmd_rate;
}

double ELECX::get_TFP_in_s() const
{
    return power_order_integrator.get_T_in_s();
}

double ELECX::get_Pmax_in_pu() const
{
    return power_order_integrator.get_upper_limit();
}

double ELECX::get_Pmin_in_pu() const
{
    return power_order_integrator.get_lower_limit();
}

double ELECX::get_IPmax_in_pu() const
{
    return IPmax;
}

double ELECX::get_Kpip() const
{
    return power_closed_loop_controller.get_Kp();
}

double ELECX::get_Kiip() const
{
    return power_closed_loop_controller.get_Ki();
}

double ELECX::get_voltage_sensor_state() const
{
    return voltage_sensor.get_state();
}

double ELECX::get_voltage_regulator_first_order_block_state() const
{
    return voltage_regulator_first_order_block.get_state();
}

double ELECX::get_voltage_regulator_integrator_state() const
{
    return voltage_regulator_integrator.get_state();
}

double ELECX::get_voltage_regulator_filter_state() const
{
    return voltage_regulator_filter.get_state();
}

double ELECX::get_active_power_sensor_state() const
{
    return active_power_sensor.get_state();
}

double ELECX::get_Q_error_integrator_state() const
{
    return Q_error_integrator.get_state();
}

double ELECX::get_V_error_integrator_state() const
{
    return V_error_integrator.get_state();
}

double ELECX::get_virtual_inertia_controller_state() const
{
    return frequency_controller.get_virtual_inertia_controller_state();
}

double ELECX::get_frequency_droop_controller_state() const
{
    return frequency_controller.get_frequency_droop_controller_state();
}

double ELECX::get_frequency_integral_controller_state() const
{
    return frequency_controller.get_frequency_integral_controller_state();
}

double ELECX::get_power_order_integrator_state() const
{
    return power_order_integrator.get_state();
}

double ELECX::get_speed_reference_sensor_state() const
{
    return speed_reference_sensor.get_state();
}

double ELECX::get_torque_controller_state() const
{
    return torque_controller.get_state();
}

double ELECX::get_virtual_inertia_power_deviation_in_pu() const
{
    return frequency_controller.get_virtual_inertia_power_deviation_in_pu();
}

double ELECX::get_frequency_droop_power_deviation_in_pu() const
{
    return frequency_controller.get_frequency_droop_power_deviation_in_pu();
}

double ELECX::get_frequency_integral_power_deviation_in_pu() const
{
    return frequency_controller.get_frequency_integral_power_deviation_in_pu();
}

double ELECX::get_frequency_regulation_power_deviation_in_pu() const
{
    return frequency_controller.get_frequency_regulation_power_deviation_in_pu();
}

void ELECX::setup_block_toolkit_and_parameters()
{
    frequency_controller.set_bus_pointer(get_bus_pointer());
    frequency_controller.set_device_pointer(get_device_pointer());
}

void ELECX::initialize()
{
    setup_block_toolkit_and_parameters();

    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double vterm = get_bus_voltage_in_pu();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    double pelec = get_active_power_generation_in_pu_based_on_mbase();
    double qelec = get_reactive_power_generation_in_pu_based_on_mbase();
    complex<double> selec(pelec, qelec);

    double ipcmd = get_initial_active_current_command_in_pu_based_on_mbase();
    double pcmd = get_initial_active_power_command_in_pu_based_on_mbase();

    frequency_controller.initialize();

    ELEC_ACTIVE_POWER_CONTROL_MODE mode = get_active_power_control_mode();
    if(mode == SPEED_CONTROL_MODE_WITH_TORQUE)
    {
        double speed = get_initial_speed();
        double torque =  pcmd/speed;
        torque_controller.set_output(torque);
        torque_controller.initialize();

        double speedref = get_speed_reference_in_pu();

        speed_reference_sensor.set_output(speedref);
        speed_reference_sensor.initialize();
    }

    if(mode == SPEED_CONTROL_MODE)
    {
        speed_controller.set_output(pcmd);
        speed_controller.initialize();
    }

    if(mode == DC_VOLTAGE_MODE)
    {
        Vdc_controller.set_output(pcmd);
        Vdc_controller.initialize();
    }

    if(mode == CONSTANT_TORQUE_MODE)
    {
        double speed = get_initial_speed();
        double torque =  pcmd/speed;
        set_initial_Torque(torque);
    }

    set_initial_Pref(pcmd);
    set_active_power_reference_in_pu_based_on_mbase(pcmd);

    power_closed_loop_controller.set_output(ipcmd);
    power_closed_loop_controller.initialize();


    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
    {
        osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
          <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    power_order_integrator.set_output(pcmd);
    if(pcmd>pmax)
    {
        osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
          <<"Porder is "<<pcmd<<", and Pmax is "<<pmax<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(pcmd<pmin)
    {
        osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
          <<"Porder is "<<pcmd<<", and Pmin is "<<pmin<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    power_order_integrator.initialize();

    double eqcmd = get_initial_reactive_voltage_command_in_pu();

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

    double qcmd = get_initial_reactive_power_command_in_pu_based_on_mbase();
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

    set_reactive_power_reference_in_pu_based_on_mbase(qcmd);

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

}

void ELECX::run(DYNAMIC_MODE mode)
{
    double vterm = get_bus_voltage_in_pu();
    double iterm = get_terminal_current_in_pu_based_on_mbase();

    double pelec = get_active_power_generation_in_pu_based_on_mbase();
    double qelec = get_reactive_power_generation_in_pu_based_on_mbase();
    complex<double> selec(pelec, qelec);

    frequency_controller.run(mode);

    double Pref = 0.0;
    double input = 0.0;
    ELEC_ACTIVE_POWER_CONTROL_MODE active_power_mode = get_active_power_control_mode();
    if(active_power_mode == SPEED_CONTROL_MODE_WITH_TORQUE)
    {
        double speed     = get_speed_in_pu();
        double speed_ref = get_speed_reference_in_pu();

        speed_reference_sensor.set_input(speed_ref);
        speed_reference_sensor.run(mode);

        input = speed - speed_reference_sensor.get_output();


        torque_controller.set_input(input);
        torque_controller.run(mode);

        Pref = torque_controller.get_output()*speed;

    }

    if(active_power_mode == SPEED_CONTROL_MODE)
    {
        double speed     = get_speed_in_pu();
        double speed_ref = get_speed_reference_in_pu();
        input = speed - speed_ref;

        speed_controller.set_input(input);
        speed_controller.run(mode);

        Pref = speed_controller.get_output();
    }

    if(active_power_mode == ACTIVE_POWER_MODE)
    {
        Pref = get_active_power_reference_in_pu_based_on_mbase();
    }

    if(active_power_mode == DC_VOLTAGE_MODE)
    {
        double Vdc = get_Vdc_in_kV();
        double Vdc_ref = get_Vdc_reference_in_kV();
        input = Vdc - Vdc_ref;
        Vdc_controller.set_input(input);
        Vdc_controller.run(mode);

        Pref = Vdc_controller.get_output();
    }

    if(active_power_mode == CONSTANT_TORQUE_MODE)
    {
        double speed     = get_speed_in_pu();
        Pref = speed*get_initial_Torque();

    }

    input = Pref + frequency_controller.get_frequency_regulation_power_deviation_in_pu();

    input = input - power_order_integrator.get_output();

    if(input>get_rPmax_in_pu())
        input = get_rPmax_in_pu();
    if(input<get_rPmin_in_pu())
        input = get_rPmin_in_pu();

    power_order_integrator.set_input(input);
    power_order_integrator.run(mode);

    input = power_order_integrator.get_output() - pelec;
    power_closed_loop_controller.set_input(input);
    power_closed_loop_controller.run(mode);

    active_power_sensor.set_input(pelec);
    active_power_sensor.run(mode);

    double xcomp = get_Xcomp_in_pu();
    input = vterm+iterm*xcomp;
    voltage_sensor.set_input(input);
    voltage_sensor.run(mode);

    input = get_voltage_reference_in_pu()-voltage_sensor.get_output();

    input /= get_Fn();

    voltage_regulator_integrator.set_input(input);
    voltage_regulator_integrator.run(mode);

    voltage_regulator_first_order_block.set_input(input);
    voltage_regulator_first_order_block.run(mode);

    input = voltage_regulator_integrator.get_output() + voltage_regulator_first_order_block.get_output();

    voltage_regulator_filter.set_input(input);
    voltage_regulator_filter.run(mode);


    ELEC_REACTIVE_POWER_CONTROL_MODE var_mode = get_var_control_mode();
    double qcmd = 0.0;
    if(var_mode==CONSTANT_VAR_MODE)
        qcmd = get_reactive_power_reference_in_pu_based_on_mbase();
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

    input = qcmd - qelec;
    Q_error_integrator.set_input(input);
    Q_error_integrator.run(mode);

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
    }

}


double ELECX::get_active_current_command_in_pu_based_on_mbase()
{
    if(get_active_power_closed_control_flag()==false)
    {
        double vterm = get_bus_voltage_in_pu();
        if(vterm==0.0)
            return 0.0;
        double pcmd = power_order_integrator.get_output();
        double ipcmd = pcmd/vterm;
        double ipmax = get_IPmax_in_pu();
        if(ipcmd>ipmax)
            ipcmd = ipmax;
        return ipcmd;
    }
    else
    {
        double ipcmd = power_closed_loop_controller.get_output();
        double ipmax = get_IPmax_in_pu();
        if(ipcmd>ipmax)
            ipcmd = ipmax;
        return ipcmd;
    }
}

double ELECX::get_active_power_command_in_pu_based_on_mbase() const
{
    if(get_active_power_closed_control_flag()==false)
        return power_order_integrator.get_output();
    else
    {
        double ipcmd = power_closed_loop_controller.get_output();
        double ipmax = get_IPmax_in_pu();
        if(ipcmd>ipmax)
            ipcmd = ipmax;
        double vterm = get_bus_voltage_in_pu();
        return ipcmd*vterm;
    }
}

double ELECX::get_reactive_current_command_in_pu_based_on_mbase()
{
    double xeq = get_Xsource();

    double eqcmd = get_reactive_voltage_command_in_pu();

    return eqcmd/(-xeq);
}

double ELECX::get_reactive_power_command_in_pu_based_on_mbase()
{
    ELEC_REACTIVE_POWER_CONTROL_MODE mode = get_var_control_mode();
    switch(mode)
    {
        case CONSTANT_VAR_MODE:
        {
            return get_reactive_power_reference_in_pu_based_on_mbase();
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

double ELECX::get_reactive_voltage_command_in_pu() const
{
    unsigned int voltage_flag = get_voltage_flag();
    if(voltage_flag==0)
    {
        double vterm = get_bus_voltage_in_pu();
        return Q_error_integrator.get_output()-vterm;
    }
    else
    {
        return V_error_integrator.get_output();
    }
}
