#include "header/model/common_model/elec_model/elec0.h"

ELEC0::ELEC0(STEPS& toolkit) : ELEC_MODEL(toolkit),
                               voltage_sensor(toolkit),
                               voltage_regulator_first_order_block(toolkit),
                               voltage_regulator_integrator(toolkit),
                               voltage_regulator_filter(toolkit),
                               active_power_sensor(toolkit),
                               Q_error_integrator(toolkit),
                               V_error_integrator(toolkit),
                               power_order_integrator(toolkit),
                               frequency_controller(toolkit)
{
    clear();
}

ELEC0::ELEC0(const ELEC0& model) : ELEC_MODEL(model.get_toolkit()),
                                   voltage_sensor(model.get_toolkit()),
                                   voltage_regulator_first_order_block(model.get_toolkit()),
                                   voltage_regulator_integrator(model.get_toolkit()),
                                   voltage_regulator_filter(model.get_toolkit()),
                                   active_power_sensor(model.get_toolkit()),
                                   Q_error_integrator(model.get_toolkit()),
                                   V_error_integrator(model.get_toolkit()),
                                   power_order_integrator(model.get_toolkit()),
                                   frequency_controller(model.get_toolkit())
{
    copy_from_const_model(model);
}

ELEC0::~ELEC0()
{
    ;
}

ELEC0& ELEC0::operator=(const ELEC0& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void ELEC0::clear()
{
    set_voltage_flag(0);

    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    power_order_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    frequency_controller.clear();

}

void ELEC0::copy_from_const_model(const ELEC0& model)
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

string ELEC0::get_model_name() const
{
    return "ELEC0";
}

void ELEC0::set_var_control_mode(ELEC_REACTIVE_POWER_CONTROL_MODE mode)
{
    this->pe_var_control_mode = mode;
}

ELEC_REACTIVE_POWER_CONTROL_MODE ELEC0::get_var_control_mode() const
{
    return pe_var_control_mode;
}

void ELEC0::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void ELEC0::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void ELEC0::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void ELEC0::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void ELEC0::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void ELEC0::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void ELEC0::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void ELEC0::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void ELEC0::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void ELEC0::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void ELEC0::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void ELEC0::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void ELEC0::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void ELEC0::set_voltage_flag(unsigned int flag)
{
    Voltage_Flag = flag;
}

void ELEC0::set_KQV(double K)
{
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void ELEC0::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void ELEC0::set_EQmin_in_pu(double I)
{
    EQmin = I;
}

void ELEC0::set_Kvi(double K)
{
    frequency_controller.set_Kvi(K);
}

void ELEC0::set_Tvi_in_s(double T)
{
    frequency_controller.set_Tvi_in_s(T);
}

void ELEC0::set_Kdroop(double K)
{
    frequency_controller.set_Kdroop(K);
}

void ELEC0::set_Tdroop_in_s(double T)
{
    frequency_controller.set_Tdroop_in_s(T);
}

void ELEC0::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    frequency_controller.set_frequency_deviation_upper_deadband_in_pu(f);
}

void ELEC0::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    frequency_controller.set_frequency_deviation_lower_deadband_in_pu(f);
}

void ELEC0::set_Kfint(double K)
{
    frequency_controller.set_Kfint(K);
}

void ELEC0::set_rPmax_in_pu(double r)
{
    max_Pcmd_rate = r;
}

void ELEC0::set_rPmin_in_pu(double r)
{
    min_Pcmd_rate = r;
}

void ELEC0::set_TFP_in_s(double T)
{
    power_order_integrator.set_T_in_s(T);
}

void ELEC0::set_Pmax_in_pu(double p)
{
    power_order_integrator.set_upper_limit(p);
}

void ELEC0::set_Pmin_in_pu(double p)
{
    power_order_integrator.set_lower_limit(p);
}

void ELEC0::set_IPmax_in_pu(double I)
{
    IPmax = I;
}

double ELEC0::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double ELEC0::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double ELEC0::get_Fn() const
{
    return Fn;
}

double ELEC0::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double ELEC0::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double ELEC0::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double ELEC0::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double ELEC0::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double ELEC0::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double ELEC0::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double ELEC0::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double ELEC0::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double ELEC0::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

unsigned int ELEC0::get_voltage_flag() const
{
    return Voltage_Flag;
}

double ELEC0::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double ELEC0::get_EQmax_in_pu() const
{
    return EQmax;
}

double ELEC0::get_EQmin_in_pu() const
{
    return EQmin;
}

double ELEC0::get_Kvi() const
{
    return frequency_controller.get_Kvi();
}

double ELEC0::get_Tvi_in_s() const
{
    return frequency_controller.get_Tvi_in_s();
}

double ELEC0::get_Kdroop() const
{
    return frequency_controller.get_Kdroop();
}

double ELEC0::get_Tdroop_in_s() const
{
    return frequency_controller.get_Tdroop_in_s();
}

double ELEC0::get_frequency_deviation_upper_deadband_in_pu() const
{
    return frequency_controller.get_frequency_deviation_upper_deadband_in_pu();
}

double ELEC0::get_frequency_deviation_lower_deadband_in_pu() const
{
    return frequency_controller.get_frequency_deviation_lower_deadband_in_pu();
}

double ELEC0::get_Kfint() const
{
    return frequency_controller.get_Kfint();
}

double ELEC0::get_rPmax_in_pu() const
{
    return max_Pcmd_rate;
}

double ELEC0::get_rPmin_in_pu() const
{
    return min_Pcmd_rate;
}

double ELEC0::get_TFP_in_s() const
{
    return power_order_integrator.get_T_in_s();
}

double ELEC0::get_Pmax_in_pu() const
{
    return power_order_integrator.get_upper_limit();
}

double ELEC0::get_Pmin_in_pu() const
{
    return power_order_integrator.get_lower_limit();
}

double ELEC0::get_IPmax_in_pu() const
{
    return IPmax;
}

double ELEC0::get_voltage_sensor_state() const
{
    return voltage_sensor.get_state();
}

double ELEC0::get_voltage_regulator_first_order_block_state() const
{
    return voltage_regulator_first_order_block.get_state();
}

double ELEC0::get_voltage_regulator_integrator_state() const
{
    return voltage_regulator_integrator.get_state();
}

double ELEC0::get_voltage_regulator_filter_state() const
{
    return voltage_regulator_filter.get_state();
}

double ELEC0::get_active_power_sensor_state() const
{
    return active_power_sensor.get_state();
}

double ELEC0::get_Q_error_integrator_state() const
{
    return Q_error_integrator.get_state();
}

double ELEC0::get_V_error_integrator_state() const
{
    return V_error_integrator.get_state();
}

double ELEC0::get_virtual_inertia_controller_state() const
{
    return frequency_controller.get_virtual_inertia_controller_state();
}

double ELEC0::get_frequency_droop_controller_state() const
{
    return frequency_controller.get_frequency_droop_controller_state();
}

double ELEC0::get_frequency_integral_controller_state() const
{
    return frequency_controller.get_frequency_integral_controller_state();
}

double ELEC0::get_power_order_integrator_state() const
{
    return power_order_integrator.get_state();
}

void ELEC0::setup_block_toolkit_and_parameters()
{
    frequency_controller.set_bus_pointer(get_bus_pointer());
    frequency_controller.set_device_pointer(get_device_pointer());
}

void ELEC0::initialize()
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
    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
    {
        osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
          <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    double pcmd = get_initial_active_power_command_in_pu_based_on_mbase();
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


    frequency_controller.initialize();
}

void ELEC0::initialize_active_power_control_as_zero()
{

}

void ELEC0::run(DYNAMIC_MODE mode)
{
    double vterm = get_bus_voltage_in_pu();
    double iterm = get_terminal_current_in_pu_based_on_mbase();

    double pelec = get_active_power_generation_in_pu_based_on_mbase();
    double qelec = get_reactive_power_generation_in_pu_based_on_mbase();
    complex<double> selec(pelec, qelec);

    double Pref = get_active_power_reference_in_pu_based_on_mbase();

    frequency_controller.run(mode);

    double input = Pref + frequency_controller.get_frequency_regulation_power_deviation_in_pu();

    input = input - power_order_integrator.get_output();

    if(input>get_rPmax_in_pu())
        input = get_rPmax_in_pu();
    if(input<get_rPmin_in_pu())
        input = get_rPmin_in_pu();

    power_order_integrator.set_input(input);
    power_order_integrator.run(mode);

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


double ELEC0::get_active_current_command_in_pu_based_on_mbase()
{
    double vterm = get_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;

    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    double ipcmd = pcmd/vterm;
    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
        ipcmd = ipmax;
    return ipcmd;
}

double ELEC0::get_active_power_command_in_pu_based_on_mbase() const
{
    return power_order_integrator.get_output();
}

double ELEC0::get_reactive_current_command_in_pu_based_on_mbase()
{
    double xeq = get_Xsource();

    double eqcmd = get_reactive_voltage_command_in_pu();

    return eqcmd/(-xeq);
}

double ELEC0::get_reactive_power_command_in_pu_based_on_mbase()
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

double ELEC0::get_reactive_voltage_command_in_pu() const
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
