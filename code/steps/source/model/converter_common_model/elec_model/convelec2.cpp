#include "header/model/converter_common_model/elec_model/convelec2.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

CONVELEC2::CONVELEC2(STEPS& toolkit) : CONVERTER_ELEC_MODEL(toolkit),
                               voltage_sensor(toolkit),
                               voltage_regulator_first_order_block(toolkit),
                               voltage_regulator_integrator(toolkit),
                               voltage_regulator_filter(toolkit),
                               active_power_sensor(toolkit),
                               Q_error_integrator(toolkit),
                               V_error_integrator(toolkit),
                               virtual_inertia_emulator(toolkit),
                               frequency_droop_controller(toolkit),
                               frequency_integral_controller(toolkit),
                               Ipcmd_controller(toolkit)
{
    clear();
}

CONVELEC2::CONVELEC2(const CONVELEC2& model) : CONVERTER_ELEC_MODEL(model.get_toolkit()),
                                   voltage_sensor(model.get_toolkit()),
                                   voltage_regulator_first_order_block(model.get_toolkit()),
                                   voltage_regulator_integrator(model.get_toolkit()),
                                   voltage_regulator_filter(model.get_toolkit()),
                                   active_power_sensor(model.get_toolkit()),
                                   Q_error_integrator(model.get_toolkit()),
                                   V_error_integrator(model.get_toolkit()),
                                   virtual_inertia_emulator(model.get_toolkit()),
                                   frequency_droop_controller(model.get_toolkit()),
                                   frequency_integral_controller(model.get_toolkit()),
                                   Ipcmd_controller(model.get_toolkit())
{
    copy_from_const_model(model);
}

CONVELEC2::~CONVELEC2()
{
    ;
}

CONVELEC2& CONVELEC2::operator=(const CONVELEC2& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void CONVELEC2::clear()
{
    set_voltage_flag(0);

    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    Ipcmd_controller.set_limiter_type(NON_WINDUP_LIMITER);
    frequency_integral_controller.set_limiter_type(NO_LIMITER);

}

void CONVELEC2::copy_from_const_model(const CONVELEC2& model)
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

    virtual_inertia_emulator.set_toolkit(toolkit);
    frequency_droop_controller.set_toolkit(toolkit);

    frequency_integral_controller.set_toolkit(toolkit);
    Ipcmd_controller.set_toolkit(toolkit);

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
    set_KPF(model.get_KPF());
    set_KIF(model.get_KIF());
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
    set_Ipmax_in_pu(model.get_Ipmax_in_pu());
    set_Ipmin_in_pu(model.get_Ipmin_in_pu());
    set_Kvi(model.get_Kvi());
    set_Tvi_in_s(model.get_Tvi_in_s());
    set_Kdroop(model.get_Kdroop());
    set_Tdroop_in_s(model.get_Tdroop_in_s());
    set_frequency_deviation_upper_deadband_in_pu(model.get_frequency_deviation_upper_deadband_in_pu());
    set_frequency_deviation_lower_deadband_in_pu(model.get_frequency_deviation_lower_deadband_in_pu());
    set_Kfint(model.get_Kfint());
}

void CONVELEC2::set_var_control_mode(PE_VAR_CONTROL_MODE mode)
{
    this->pe_var_control_mode = mode;
}

PE_VAR_CONTROL_MODE CONVELEC2::get_var_control_mode() const
{
    return pe_var_control_mode;
}

void CONVELEC2::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void CONVELEC2::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void CONVELEC2::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void CONVELEC2::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void CONVELEC2::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void CONVELEC2::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void CONVELEC2::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void CONVELEC2::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void CONVELEC2::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void CONVELEC2::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void CONVELEC2::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void CONVELEC2::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void CONVELEC2::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void CONVELEC2::set_voltage_flag(unsigned int flag)
{
    Voltage_Flag = flag;
}

void CONVELEC2::set_KQV(double K)
{
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void CONVELEC2::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void CONVELEC2::set_EQmin_in_pu(double I)
{
    EQmin = I;
}

void CONVELEC2::set_Kvi(double K)
{
    virtual_inertia_emulator.set_K(K);
}

void CONVELEC2::set_Tvi_in_s(double T)
{
    virtual_inertia_emulator.set_T_in_s(T);
}

void CONVELEC2::set_Kdroop(double K)
{
    frequency_droop_controller.set_K(K);
}

void CONVELEC2::set_Tdroop_in_s(double T)
{
    frequency_droop_controller.set_T_in_s(T);
}

void CONVELEC2::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    f_upper_pu = f;
}

void CONVELEC2::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    f_lower_pu = f;
}

void CONVELEC2::set_Kfint(double K)
{
    double t = 0.0;
    if(fabs(K)<DOUBLE_EPSILON)
        t = INFINITE_THRESHOLD;
    else
        t = 1.0/K;
    frequency_integral_controller.set_T_in_s(t);
}

void CONVELEC2::set_KIF(double K)
{
    Ipcmd_controller.set_Ki(K);
}

void CONVELEC2::set_KPF(double K)
{
    Ipcmd_controller.set_Kp(K);
}

void CONVELEC2::set_Ipmax_in_pu(double I)
{
    Ipcmd_controller.set_upper_limit(I);
}

void CONVELEC2::set_Ipmin_in_pu(double I)
{
    Ipcmd_controller.set_lower_limit(I);
}

double CONVELEC2::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double CONVELEC2::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double CONVELEC2::get_Fn() const
{
    return Fn;
}

double CONVELEC2::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double CONVELEC2::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double CONVELEC2::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double CONVELEC2::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double CONVELEC2::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double CONVELEC2::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double CONVELEC2::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double CONVELEC2::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double CONVELEC2::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double CONVELEC2::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

unsigned int CONVELEC2::get_voltage_flag() const
{
    return Voltage_Flag;
}

double CONVELEC2::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double CONVELEC2::get_EQmax_in_pu() const
{
    return EQmax;
}

double CONVELEC2::get_EQmin_in_pu() const
{
    return EQmin;
}

double CONVELEC2::get_Kvi() const
{
    return virtual_inertia_emulator.get_K();
}

double CONVELEC2::get_Tvi_in_s() const
{
    return virtual_inertia_emulator.get_T_in_s();
}

double CONVELEC2::get_Kdroop() const
{
    return frequency_droop_controller.get_K();
}

double CONVELEC2::get_Tdroop_in_s() const
{
    return frequency_droop_controller.get_T_in_s();
}

double CONVELEC2::get_frequency_deviation_upper_deadband_in_pu() const
{
    return f_upper_pu;
}

double CONVELEC2::get_frequency_deviation_lower_deadband_in_pu() const
{
    return f_lower_pu;
}

double CONVELEC2::get_Kfint() const
{
    double t = frequency_integral_controller.get_T_in_s();
    double k = 0.0;
    if(fabs(t-INFINITE_THRESHOLD)<DOUBLE_EPSILON)
        k = 0.0;
    else
        k = 1.0/t;
    return k;
}

double CONVELEC2::get_KIF() const
{
    return Ipcmd_controller.get_Ki();
}

double CONVELEC2::get_KPF() const
{
    return Ipcmd_controller.get_Kp();
}

double CONVELEC2::get_Ipmax_in_pu() const
{
    return Ipcmd_controller.get_upper_limit();
}

double CONVELEC2::get_Ipmin_in_pu() const
{
    return Ipcmd_controller.get_lower_limit();
}

double CONVELEC2::get_voltage_sensor_state() const
{
    return voltage_sensor.get_state();
}

double CONVELEC2::get_voltage_regulator_first_order_block_state() const
{
    return voltage_regulator_first_order_block.get_state();
}

double CONVELEC2::get_voltage_regulator_integrator_state() const
{
    return voltage_regulator_integrator.get_state();
}

double CONVELEC2::get_voltage_regulator_filter_state() const
{
    return voltage_regulator_filter.get_state();
}

double CONVELEC2::get_active_power_sensor_state() const
{
    return active_power_sensor.get_state();
}

double CONVELEC2::get_Q_error_integrator_state() const
{
    return Q_error_integrator.get_state();
}

double CONVELEC2::get_V_error_integrator_state() const
{
    return V_error_integrator.get_state();
}

double CONVELEC2::get_virtual_inertia_emulator_state() const
{
    return virtual_inertia_emulator.get_state();
}

double CONVELEC2::get_frequency_droop_controller_state() const
{
    return frequency_droop_controller.get_state();
}

double CONVELEC2::get_frequency_integral_controller_state() const
{
    return frequency_integral_controller.get_state();
}

double CONVELEC2::get_Ipcmd_controller_state() const
{
    return Ipcmd_controller.get_state();
}

void CONVELEC2::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double vterm = get_terminal_voltage_in_pu();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    double pelec = get_active_power_generation_in_pu_based_on_mbase();
    double qelec = get_reactive_power_generation_in_pu_based_on_mbase();
    complex<double> selec(pelec, qelec);

    double ipcmd = get_initial_active_current_command_in_pu_based_on_mbase();
    double ipmax = get_Ipmax_in_pu();
    double ipmin = get_Ipmin_in_pu();
    if(ipcmd>ipmax)
    {
        osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
          <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(ipcmd<ipmin)
    {
        osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
          <<"IPcmd is "<<ipcmd<<", and IPmin is "<<ipmin<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    Ipcmd_controller.set_output(ipcmd);
    Ipcmd_controller.initialize();

    virtual_inertia_emulator.set_input(0.0);
    virtual_inertia_emulator.initialize();

    frequency_droop_controller.set_output(0.0);
    frequency_droop_controller.initialize();

    frequency_integral_controller.set_output(0.0);
    frequency_integral_controller.initialize();


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

void CONVELEC2::run(DYNAMIC_MODE mode)
{
    double vterm = get_terminal_voltage_in_pu();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    double freq = get_terminal_bus_frequency_deviation_in_pu();

    double pelec = get_active_power_generation_in_pu_based_on_mbase();
    double qelec = get_reactive_power_generation_in_pu_based_on_mbase();
    complex<double> selec(pelec, qelec);

    double Pref = get_active_power_reference_in_pu_based_on_mbase();
    double input = 0.0;

    virtual_inertia_emulator.set_input(freq);
    virtual_inertia_emulator.run(mode);

    frequency_droop_controller.set_input(freq);
    frequency_droop_controller.run(mode);

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


    input = Pref -(virtual_inertia_emulator.get_output()
                   +frequency_droop_controller.get_output()
                   +frequency_integral_controller.get_output());

    active_power_sensor.set_input(pelec);
    active_power_sensor.run(mode);

    input -= active_power_sensor.get_output();

    Ipcmd_controller.set_input(input);
    Ipcmd_controller.run(mode);

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


    PE_VAR_CONTROL_MODE var_mode = get_var_control_mode();
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

double CONVELEC2::get_active_current_command_in_pu_based_on_mbase()
{
    return Ipcmd_controller.get_output();
}

double CONVELEC2::get_active_power_command_in_pu_based_on_mbase() const
{
    return Ipcmd_controller.get_output()*get_terminal_voltage_in_pu();
}

double CONVELEC2::get_reactive_current_command_in_pu_based_on_mbase()
{
    double xeq = get_Xsource();

    double eqcmd = get_reactive_voltage_command_in_pu();

    return eqcmd/(-xeq);
}

double CONVELEC2::get_reactive_power_command_in_pu_based_on_mbase()
{
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
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

double CONVELEC2::get_reactive_voltage_command_in_pu() const
{
    unsigned int voltage_flag = get_voltage_flag();
    if(voltage_flag==0)
    {
        double vterm = get_terminal_voltage_in_pu();
        return Q_error_integrator.get_output()-vterm;
    }
    else
    {
        return V_error_integrator.get_output();
    }
}
