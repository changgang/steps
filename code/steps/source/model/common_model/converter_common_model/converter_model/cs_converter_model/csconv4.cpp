#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/csconv4.h"

CSCONV4::CSCONV4(STEPS& toolkit) : CS_CONVERTER_MODEL(toolkit),
                               active_current_commander(toolkit),
                               reactive_voltage_commander(toolkit),
                               convcol(toolkit),
                               pll0(toolkit)

{
    clear();
}

CSCONV4::CSCONV4(const CSCONV4& model) : CS_CONVERTER_MODEL(model.get_toolkit()),
                               active_current_commander(model.get_toolkit()),
                               reactive_voltage_commander(model.get_toolkit()),
                               convcol(model.get_toolkit()),
                               pll0(model.get_toolkit())
{
    copy_from_const_model(model);
}

CSCONV4& CSCONV4::operator=(const CSCONV4& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

CSCONV4::~CSCONV4()
{
    ;
}

void CSCONV4::clear()
{
    active_current_commander.set_limiter_type(NO_LIMITER);
    active_current_commander.set_K(1.0);

    reactive_voltage_commander.set_limiter_type(NO_LIMITER);
    reactive_voltage_commander.set_K(1.0);

    pll0.clear();
}

void CSCONV4::copy_from_const_model(const CSCONV4& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_current_commander.set_toolkit(toolkit);
    reactive_voltage_commander.set_toolkit(toolkit);
    convcol.set_toolkit(toolkit);
    pll0.set_toolkit(toolkit);

    clear();
    set_converter_activer_current_command_T_in_s(model.get_converter_activer_current_command_T_in_s());
    set_converter_reactiver_voltage_command_T_in_s(model.get_converter_reactiver_voltage_command_T_in_s());

    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

string CSCONV4::get_model_name() const
{
    return "CSCONV4";
}

void CSCONV4::set_converter_activer_current_command_T_in_s(double t)
{
    active_current_commander.set_T_in_s(t);
}

void CSCONV4::set_converter_reactiver_voltage_command_T_in_s(double t)
{
    reactive_voltage_commander.set_T_in_s(t);
}

void CSCONV4::set_KPLL(double K)
{
    pll0.set_Kp(K);
}

void CSCONV4::set_KIPLL(double K)
{
    pll0.set_Ki(K);
}

void CSCONV4::set_PLLmax(double pmax)
{
    pll0.set_Pllmax(pmax);
}

void CSCONV4::set_PLLmin(double pmin)
{
    pll0.set_Pllmin(pmin);
}

void CSCONV4::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    convcol.set_PQ_priority_flag(flag);
}

void CSCONV4::set_Imax(double Imax)
{
    convcol.set_Imax(Imax);
}

double CSCONV4::get_converter_activer_current_command_T_in_s() const
{
    return active_current_commander.get_T_in_s();
}

double CSCONV4::get_converter_reactiver_voltage_command_T_in_s() const
{
    return reactive_voltage_commander.get_T_in_s();
}

double CSCONV4::get_KPLL() const
{
    return pll0.get_Kp();
}

double CSCONV4::get_KIPLL() const
{
    return pll0.get_Ki();
}

double CSCONV4::get_PLLmax() const
{
    return pll0.get_Pllmax();
}

double CSCONV4::get_PLLmin() const
{
    return pll0.get_Pllmin();
}

COL_PQ_PRIORITY CSCONV4::get_pq_priority_flag() const
{
    return convcol.get_PQ_priority_flag();
}

double CSCONV4::get_Imax() const
{
    return convcol.get_Imax();
}

double CSCONV4::get_active_current_commander_state() const
{
    return active_current_commander.get_state();
}

double CSCONV4::get_reactive_voltage_commander_state() const
{
    return reactive_voltage_commander.get_state();
}

double CSCONV4::get_PLL_frequency_integrator_state() const
{
    return pll0.get_frquency_deviation_block_state();
}

double CSCONV4::get_PLL_angle_integrator_state() const
{
    return pll0.get_angle_block_state();
}

void CSCONV4::setup_block_toolkit_and_parameters()
{
    pll0.set_bus_pointer(get_bus_pointer());
    convcol.set_device_pointer(get_device_pointer());
}

void CSCONV4::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    double angle_in_rad = get_bus_voltage_angle_in_rad();

    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real();
    double Iy = Ixy.imag();

    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double IP = Ix*cosine + Iy*sine;
    double IQ =-Ix*sine + Iy*cosine;

    double xeq = get_source_impedance_in_pu_based_on_mbase().imag();
    double EQ = IQ*(-xeq);
    set_initial_active_current_command_in_pu_based_on_mbase(IP);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_current_command_in_pu_based_on_mbase(IQ);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(EQ);
    active_current_commander.set_output(IP);
    active_current_commander.initialize();
    reactive_voltage_commander.set_output(EQ);
    reactive_voltage_commander.initialize();
    pll0.initialize();

}

void CSCONV4::run(DYNAMIC_MODE mode)
{
    double Ipcmd = get_Ipcmd_in_pu_based_on_mbase();

    active_current_commander.set_input(Ipcmd);
    active_current_commander.run(mode);

    double Eqcmd = get_Eqcmd_in_pu();

    reactive_voltage_commander.set_input(Eqcmd);
    reactive_voltage_commander.run(mode);

    pll0.run(mode);

}

complex<double> CSCONV4::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    double Ip = get_Ip_in_pu();
    double Iq = get_Iq_in_pu();

    double pll_angle = get_pll_angle_in_rad();

    double sine = steps_sin(pll_angle), cosine = steps_cos(pll_angle);
    double Ix = Ip*cosine - Iq*sine;
    double Iy = Ip*sine + Iq*cosine;

    complex<double> Ixy(Ix, Iy);
    return Ixy;
}

complex<double> CSCONV4::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> CSCONV4::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Ixy = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> Z = get_source_impedance_in_pu_based_on_mbase();
    return Ixy*Z;
}

double CSCONV4::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double CSCONV4::get_pll_angle_in_rad()
{
    return pll0.get_pll_angle_in_rad();
}

double CSCONV4::get_pll_frequency_deviation_in_pu()
{
    return pll0.get_pll_frequency_deviation_in_pu();
}

complex<double> CSCONV4::get_Ipq_in_pu()
{
    double Xeq = get_source_impedance_in_pu_based_on_mbase().imag();
    double Ip_guess = active_current_commander.get_output();
    double Iq_guess = -reactive_voltage_commander.get_output()/Xeq;

    complex<double> I_guess(Ip_guess, Iq_guess);
    complex<double> Ipq = convcol.get_limited_PQ_current_order(I_guess);
    return Ipq;
}

double CSCONV4::get_Ip_in_pu()
{
    return get_Ipq_in_pu().real();
}

double CSCONV4::get_Iq_in_pu()
{
    return get_Ipq_in_pu().imag();
}
