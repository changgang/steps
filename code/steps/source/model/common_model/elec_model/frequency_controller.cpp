#include "header/model/common_model/elec_model/frequency_controller.h"

FREQUENCY_CONTROLLER::FREQUENCY_CONTROLLER(STEPS& toolkit) : COMMON_MODEL(toolkit),
                                                             virtual_inertia_controller(toolkit),
                                                             frequency_droop_controller(toolkit),
                                                             frequency_integral_controller(toolkit)
{
    clear();
}

FREQUENCY_CONTROLLER::FREQUENCY_CONTROLLER(const FREQUENCY_CONTROLLER& model) : COMMON_MODEL(model.get_toolkit()),
                                                                                 virtual_inertia_controller(model.get_toolkit()),
                                                                                 frequency_droop_controller(model.get_toolkit()),
                                                                                 frequency_integral_controller(model.get_toolkit())
{
    copy_from_const_model(model);
}

FREQUENCY_CONTROLLER::~FREQUENCY_CONTROLLER()
{
    ;
}

FREQUENCY_CONTROLLER& FREQUENCY_CONTROLLER::operator=(const FREQUENCY_CONTROLLER& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void FREQUENCY_CONTROLLER::clear()
{
    set_frequency_deviation_upper_deadband_in_pu(0.0);
    set_frequency_deviation_lower_deadband_in_pu(0.0);
    virtual_inertia_controller.set_limiter_type(NO_LIMITER);
    frequency_droop_controller.set_limiter_type(NO_LIMITER);
    frequency_integral_controller.set_limiter_type(NO_LIMITER);
}

void FREQUENCY_CONTROLLER::copy_from_const_model(const FREQUENCY_CONTROLLER& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    virtual_inertia_controller.set_toolkit(toolkit);
    frequency_droop_controller.set_toolkit(toolkit);
    frequency_integral_controller.set_toolkit(toolkit);

    clear();

    set_frequency_deviation_upper_deadband_in_pu(model.get_frequency_deviation_upper_deadband_in_pu());
    set_frequency_deviation_lower_deadband_in_pu(model.get_frequency_deviation_lower_deadband_in_pu());
    set_Kfint(model.get_Kfint());
    set_Kvi(model.get_Kvi());
    set_Tvi_in_s(model.get_Tvi_in_s());
    set_Kdroop(model.get_Kdroop());
    set_Tdroop_in_s(model.get_Tdroop_in_s());
}

void FREQUENCY_CONTROLLER::set_Kvi(double K)
{
    virtual_inertia_controller.set_K(K);
}

void FREQUENCY_CONTROLLER::set_Tvi_in_s(double T)
{
    virtual_inertia_controller.set_T_in_s(T);
}

void FREQUENCY_CONTROLLER::set_Kdroop(double K)
{
    frequency_droop_controller.set_K(K);
}

void FREQUENCY_CONTROLLER::set_Tdroop_in_s(double T)
{
    frequency_droop_controller.set_T_in_s(T);
}

void FREQUENCY_CONTROLLER::set_frequency_deviation_upper_deadband_in_pu(double f)
{
    f_upper_pu = f;
}

void FREQUENCY_CONTROLLER::set_frequency_deviation_lower_deadband_in_pu(double f)
{
    f_lower_pu = f;
}

void FREQUENCY_CONTROLLER::set_Kfint(double K)
{
    double t = 0.0;
    if(fabs(K)<DOUBLE_EPSILON)
        t = INFINITE_THRESHOLD;
    else
        t = 1.0/K;
    frequency_integral_controller.set_T_in_s(t);
}

double FREQUENCY_CONTROLLER::get_Kvi() const
{
    return virtual_inertia_controller.get_K();
}

double FREQUENCY_CONTROLLER::get_Tvi_in_s() const
{
    return virtual_inertia_controller.get_T_in_s();
}

double FREQUENCY_CONTROLLER::get_Kdroop() const
{
    return frequency_droop_controller.get_K();
}

double FREQUENCY_CONTROLLER::get_Tdroop_in_s() const
{
    return frequency_droop_controller.get_T_in_s();
}

double FREQUENCY_CONTROLLER::get_Kfint() const
{
    double t = frequency_integral_controller.get_T_in_s();
    double k = 0.0;
    if(fabs(t-INFINITE_THRESHOLD)<DOUBLE_EPSILON)
        k = 0.0;
    else
        k = 1.0/t;
    return k;
}

double FREQUENCY_CONTROLLER::get_virtual_inertia_controller_state() const
{
    return virtual_inertia_controller.get_state();
}

double FREQUENCY_CONTROLLER::get_frequency_droop_controller_state() const
{
    return frequency_droop_controller.get_state();
}

double FREQUENCY_CONTROLLER::get_frequency_integral_controller_state() const
{
    return frequency_integral_controller.get_state();
}

void FREQUENCY_CONTROLLER::initialize()
{
    virtual_inertia_controller.set_input(0.0);
    virtual_inertia_controller.initialize();

    frequency_droop_controller.set_output(0.0);
    frequency_droop_controller.initialize();

    frequency_integral_controller.set_output(0.0);
    frequency_integral_controller.initialize();

}

void FREQUENCY_CONTROLLER::run(DYNAMIC_MODE mode)
{

    double freq = get_bus_frequency_deviation_in_pu();

    virtual_inertia_controller.set_input(freq);
    virtual_inertia_controller.run(mode);

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

}

double FREQUENCY_CONTROLLER::get_frequency_regulation_power_deviation_in_pu() const
{
    return get_virtual_inertia_power_deviation_in_pu()
            +get_frequency_droop_power_deviation_in_pu()
            +get_frequency_integral_power_deviation_in_pu();
}

double FREQUENCY_CONTROLLER::get_virtual_inertia_power_deviation_in_pu() const
{
    return -virtual_inertia_controller.get_output();
}

double FREQUENCY_CONTROLLER::get_frequency_droop_power_deviation_in_pu() const
{
    return -frequency_droop_controller.get_output();
}

double FREQUENCY_CONTROLLER::get_frequency_integral_power_deviation_in_pu() const
{
    return -frequency_integral_controller.get_output();
}
