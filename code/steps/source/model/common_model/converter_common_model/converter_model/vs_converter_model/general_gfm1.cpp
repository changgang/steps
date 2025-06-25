#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/general_gfm1.h"

GENERAL_GFM1::GENERAL_GFM1(STEPS& toolkit) : VS_CONVERTER_MODEL(toolkit),
                             active_power_sensor(toolkit),
                             active_power_regulator_block(toolkit),
                             virtual_angle_block(toolkit),
                             reactive_power_sensor(toolkit),
                             terminal_voltage_sensor(toolkit),
                             reactive_power_regulator_block(toolkit),
                             convcol1(toolkit),
                             pll0(toolkit)
{
    clear();
}

GENERAL_GFM1::GENERAL_GFM1(const GENERAL_GFM1& model) : VS_CONVERTER_MODEL(model.get_toolkit()),
                             active_power_sensor(model.get_toolkit()),
                             active_power_regulator_block(model.get_toolkit()),
                             virtual_angle_block(model.get_toolkit()),
                             reactive_power_sensor(model.get_toolkit()),
                             terminal_voltage_sensor(model.get_toolkit()),
                             reactive_power_regulator_block(model.get_toolkit()),
                             convcol1(model.get_toolkit()),
                             pll0(model.get_toolkit())
{
    copy_from_const_model(model);
}

GENERAL_GFM1& GENERAL_GFM1::operator=(const GENERAL_GFM1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

GENERAL_GFM1::~GENERAL_GFM1()
{
    ;
}

void GENERAL_GFM1::clear()
{
    set_voltage_flag(1);
    set_Kp(0.0);
    set_Tp_in_s(INFINITE_THRESHOLD);
    set_Tpf_in_s(0.02);
    set_Tqf_in_s(0.02);
    set_Tvf_in_s(0.02);
    set_mq(0.05);
    set_mv(1.0);
    set_Kpv(0.0);
    set_Kiv(0.0);
    set_Emax(1.2);
    set_Emin(0.0);
    set_current_limit_mode(SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED);
    set_dq_oriented_flag(ACL_ORIENTED);
    set_phi_i_in_deg(0.0);
    set_Imax(INFINITE_THRESHOLD);
    reactive_power_regulator_block.set_limiter_type(NON_WINDUP_LIMITER);
    pll0.clear();
}

void GENERAL_GFM1::copy_from_const_model(const GENERAL_GFM1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_power_sensor.set_toolkit(toolkit);
    active_power_regulator_block.set_toolkit(toolkit);
    virtual_angle_block.set_toolkit(toolkit);
    reactive_power_sensor.set_toolkit(toolkit);
    terminal_voltage_sensor.set_toolkit(toolkit);
    reactive_power_regulator_block.set_toolkit(toolkit);
    convcol1.set_toolkit(toolkit);
    pll0.set_toolkit(toolkit);

    clear();

    set_voltage_flag(model.get_voltage_flag());
    set_Kp(model.get_Kp());
    set_Tp_in_s(model.get_Tpf_in_s());
    set_Tpf_in_s(model.get_Tpf_in_s());
    set_Tqf_in_s(model.get_Tqf_in_s());
    set_Tvf_in_s(model.get_Tvf_in_s());
    set_mq(model.get_mq());
    set_mv(model.get_mv());
    set_Kpv(model.get_Kpv());
    set_Kiv(model.get_Kiv());
    set_Emax(model.get_Emax());
    set_Emin(model.get_Emin());
    set_current_limit_mode(model.get_current_limit_mode());
    set_dq_oriented_flag(model.get_dq_oriented_flag());
    set_phi_i_in_deg(model.get_phi_i_in_deg());
    set_Imax(model.get_Imax());
    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
    set_PLLmin(model.get_PLLmin());
}

string GENERAL_GFM1::get_model_name() const
{
    return "GENERAL_GFM1";
}

void GENERAL_GFM1::set_Eref_in_pu(double E)
{
    this->Eref = E;
}

void GENERAL_GFM1::set_voltage_flag(unsigned int flag)
{
    this->Voltage_Flag = flag;
}

void GENERAL_GFM1::set_Kp(double Kp)
{
    active_power_regulator_block.set_K(Kp);
}

void GENERAL_GFM1::set_Tp_in_s(double Tp)
{
    active_power_regulator_block.set_T_in_s(Tp);
}

void GENERAL_GFM1::set_Tpf_in_s(double Tpf)
{
    active_power_sensor.set_T_in_s(Tpf);
}

void GENERAL_GFM1::set_Tqf_in_s(double Tqf)
{
    reactive_power_sensor.set_T_in_s(Tqf);
}

void GENERAL_GFM1::set_Tvf_in_s(double Tvf)
{
    terminal_voltage_sensor.set_T_in_s(Tvf);
}

void GENERAL_GFM1::set_mq(double mq)
{
    this->mq = mq;
}

void GENERAL_GFM1::set_mv(double mv)
{
    this->mv = mv;
}

void GENERAL_GFM1::set_Kpv(double Kpv)
{
    reactive_power_regulator_block.set_Kp(Kpv);
}

void GENERAL_GFM1::set_Kiv(double Kiv)
{
    reactive_power_regulator_block.set_Ki(Kiv);
}

void GENERAL_GFM1::set_Emax(double Emax)
{
    this->Emax = Emax;
    reactive_power_regulator_block.set_upper_limit(Emax);
}

void GENERAL_GFM1::set_Emin(double Emin)
{
    this->Emin = Emin;
    reactive_power_regulator_block.set_lower_limit(Emin);
}

void GENERAL_GFM1::set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode)
{
    convcol1.set_current_limit_mode(current_limit_mode);
}

void GENERAL_GFM1::set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag)
{
    convcol1.set_dq_oriented_flag(dq_oriented_flag);
}

void GENERAL_GFM1::set_phi_i_in_deg(double phi_i_in_deg)
{
    convcol1.set_phi_i_in_deg(phi_i_in_deg);
}

void GENERAL_GFM1::set_Imax(double Imax)
{
    convcol1.set_Imax(Imax);
}

void GENERAL_GFM1::set_KPLL(double Kp)
{
    pll0.set_Kp(Kp);
}

void GENERAL_GFM1::set_KIPLL(double Ki)
{
    pll0.set_Ki(Ki);
}

void GENERAL_GFM1::set_PLLmax(double pmax)
{
    pll0.set_Pllmax(pmax);
}

void GENERAL_GFM1::set_PLLmin(double pmin)
{
    pll0.set_Pllmin(pmin);
}

double GENERAL_GFM1::get_Eref_in_pu() const
{
    return Eref;
}

unsigned int GENERAL_GFM1::get_voltage_flag() const
{
    return Voltage_Flag;
}

double GENERAL_GFM1::get_Kp() const
{
    return active_power_regulator_block.get_K();
}

double GENERAL_GFM1::get_Tp_in_s() const
{
    return active_power_regulator_block.get_T_in_s();
}

double GENERAL_GFM1::get_Tpf_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double GENERAL_GFM1::get_Tqf_in_s() const
{
    return reactive_power_sensor.get_T_in_s();
}

double GENERAL_GFM1::get_Tvf_in_s() const
{
    return terminal_voltage_sensor.get_T_in_s();
}

double GENERAL_GFM1::get_mq() const
{
    return mq;
}

double GENERAL_GFM1::get_mv() const
{
    return mv;
}

double GENERAL_GFM1::get_Kpv() const
{
    return reactive_power_regulator_block.get_Kp();
}

double GENERAL_GFM1::get_Kiv() const
{
    return reactive_power_regulator_block.get_Ki();
}

double GENERAL_GFM1::get_Emax() const
{
    return Emax;
}

double GENERAL_GFM1::get_Emin() const
{
    return Emin;
}

CURRENT_LIMIT_MODE GENERAL_GFM1::get_current_limit_mode() const
{
    return convcol1.get_current_limit_mode();
}

DQ_ORIENTED_FLAG GENERAL_GFM1::get_dq_oriented_flag() const
{
    return convcol1.get_dq_oriented_flag();
}

double GENERAL_GFM1::get_phi_i_in_deg() const
{
    return convcol1.get_phi_i_in_deg();
}

double GENERAL_GFM1::get_phi_i_in_rad() const
{
    return convcol1.get_phi_i_in_rad();
}

double GENERAL_GFM1::get_Imax() const
{
    return convcol1.get_Imax();
}

double GENERAL_GFM1::get_KPLL() const
{
    return pll0.get_Kp();
}

double GENERAL_GFM1::get_KIPLL() const
{
    return pll0.get_Ki();
}

double GENERAL_GFM1::get_PLLmax() const
{
    return pll0.get_Pllmax();
}

double GENERAL_GFM1::get_PLLmin() const
{
    return pll0.get_Pllmin();
}

double GENERAL_GFM1::get_active_power_sensor_state() const
{
    return active_power_sensor.get_state();
}

double GENERAL_GFM1::get_active_power_regulator_block_state() const
{
    return active_power_regulator_block.get_state();
}

double GENERAL_GFM1::get_virtual_angle_block_state() const
{
    return virtual_angle_block.get_state();
}

double GENERAL_GFM1::get_reactive_power_sensor_state() const
{
    return reactive_power_sensor.get_state();
}

double GENERAL_GFM1::get_terminal_voltage_sensor_state() const
{
    return terminal_voltage_sensor.get_state();
}

double GENERAL_GFM1::get_reactive_power_regulator_block_state() const
{
    return reactive_power_regulator_block.get_state();
}

double GENERAL_GFM1::get_PLL_frequency_integrator_state() const
{
    return pll0.get_frquency_deviation_block_state();
}

double GENERAL_GFM1::get_PLL_angle_integrator_state() const
{
    return pll0.get_angle_block_state();
}

void GENERAL_GFM1::setup_block_toolkit_and_parameters()
{
    virtual_angle_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());
    convcol1.set_device_pointer(get_device_pointer());
    pll0.set_bus_pointer(get_bus_pointer());
}

void GENERAL_GFM1::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Esource = Vxy + Ixy * Zsource;
    double E = steps_fast_complex_abs(Esource);
    double E_angle = steps_fast_complex_arg(Esource);
    double Vterm = get_bus_voltage_in_pu();

    double Ix = Ixy.real();
    double Iy = Ixy.imag();
    double V_angle = get_bus_voltage_angle_in_rad();
    double sine = steps_sin(V_angle), cosine = steps_cos(V_angle);
    double Ip = Ix*cosine + Iy*sine;
    double Iq =-Ix*sine + Iy*cosine;

    virtual_angle_block.set_output(E_angle);
    virtual_angle_block.initialize();

    active_power_regulator_block.set_output(0.0);
    active_power_regulator_block.initialize();

    active_power_sensor.set_output(P);
    active_power_sensor.initialize();

    reactive_power_regulator_block.set_output(E);
    reactive_power_regulator_block.initialize();

    terminal_voltage_sensor.set_output(Vterm);
    terminal_voltage_sensor.initialize();

    reactive_power_sensor.set_output(Q);
    reactive_power_sensor.initialize();

    set_Pref_in_pu_based_on_mbase(P);
    set_Qref_in_pu_based_on_mbase(Q);
    set_Vref_in_pu(Vterm);
    set_Eref_in_pu(E);

    set_initial_active_current_command_in_pu_based_on_mbase(Ip);
    set_initial_reactive_current_command_in_pu_based_on_mbase(Iq);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(Vterm);

    pll0.initialize();
}

void GENERAL_GFM1::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real(), Q = S.imag();

    double input;

    active_power_sensor.set_input(P);
    active_power_sensor.run(mode);
/*
    STEPS& toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if (time > 3.3)
        input = 0.5 - active_power_sensor.get_output();
    else if (time > 3.0)
        input = 0.5 - active_power_sensor.get_output();
    else
        input = 0.8 - active_power_sensor.get_output();*/
    input = get_Pref_in_pu_based_on_mbase() - active_power_sensor.get_output();
    active_power_regulator_block.set_input(input);
    active_power_regulator_block.run(mode);

    input = active_power_regulator_block.get_output();
    virtual_angle_block.set_input(input);
    virtual_angle_block.run(mode);

    reactive_power_sensor.set_input(Q);
    reactive_power_sensor.run(mode);

    terminal_voltage_sensor.set_input(get_bus_voltage_in_pu());
    terminal_voltage_sensor.run(mode);

    input = mq * (get_Qref_in_pu_based_on_mbase() - reactive_power_sensor.get_output())
          + mv * (get_Vref_in_pu() - terminal_voltage_sensor.get_output());
    reactive_power_regulator_block.set_input(input);
    reactive_power_regulator_block.run(mode);

    pll0.run(mode);
}

double GENERAL_GFM1::get_virtual_frequency_deviation_in_pu() const
{
    return active_power_regulator_block.get_output();
}

double GENERAL_GFM1::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double GENERAL_GFM1::get_virtual_voltage_in_pu() const
{
    switch(Voltage_Flag)
    {
        case 0://directly set internal voltage
        {
            return get_Eref_in_pu() + mq * (get_Qref_in_pu_based_on_mbase() - reactive_power_sensor.get_output());
        }
        default://use PI controller
        {
            double E = reactive_power_regulator_block.get_output();
            if(E > Emax)
                return Emax;
            else if(E < Emin)
                return Emin;
            else
                return E;
        }
    }
}

complex<double> GENERAL_GFM1::get_virtual_complex_voltage_in_pu_in_xy_axis()
{
    double E = get_virtual_voltage_in_pu();
    double theta = get_virtual_angle_in_rad();
    double Ex = E * steps_cos(theta);
    double Ey = E * steps_sin(theta);
    complex<double> Exy(Ex, Ey);
    return Exy;
}

complex<double> GENERAL_GFM1::get_unlimited_xy_current_order()
{
    complex<double> Exy = get_virtual_complex_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    return (Exy - Vxy) / Zsource;
}

complex<double> GENERAL_GFM1::get_limited_xy_current_order()
{
    complex<double> Ixy_guess = get_unlimited_xy_current_order();
    double virtual_angle_in_rad = virtual_angle_block.get_output();
    double pll_angle_in_rad = pll0.get_pll_angle_in_rad();
    return convcol1.get_limited_xy_current_order(Ixy_guess, virtual_angle_in_rad, pll_angle_in_rad);
}

complex<double> GENERAL_GFM1::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = get_limited_xy_current_order();
    complex<double> Exy = Vxy + Ixy * Zsource;
    return Exy;
}

complex<double> GENERAL_GFM1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    return Exy/Zsource;
}

complex<double> GENERAL_GFM1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return get_limited_xy_current_order();
}

double GENERAL_GFM1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}


