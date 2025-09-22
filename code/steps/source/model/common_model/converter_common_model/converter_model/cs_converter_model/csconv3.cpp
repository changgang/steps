#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/csconv3.h"

CSCONV3::CSCONV3(STEPS& toolkit) : CS_CONVERTER_MODEL(toolkit),
                               active_current_commander(toolkit),
                               reactive_current_commander(toolkit),
                               pll0(toolkit)

{
    clear();
}

CSCONV3::CSCONV3(const CSCONV3& model) : CS_CONVERTER_MODEL(model.get_toolkit()),
                               active_current_commander(model.get_toolkit()),
                               reactive_current_commander(model.get_toolkit()),
                               pll0(model.get_toolkit())
{
    copy_from_const_model(model);
}

CSCONV3& CSCONV3::operator=(const CSCONV3& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

CSCONV3::~CSCONV3()
{
    ;
}

void CSCONV3::clear()
{
    complex<double> Z(0.0, 0.0);
    set_source_impedance_in_pu_based_on_mbase(Z);
    active_current_commander.set_limiter_type(NO_LIMITER);
    active_current_commander.set_K(1.0);
    active_current_commander.set_T_in_s(0.02);

    reactive_current_commander.set_limiter_type(NO_LIMITER);
    reactive_current_commander.set_K(1.0);
    reactive_current_commander.set_T_in_s(0.02);

    pll0.clear();
}

void CSCONV3::copy_from_const_model(const CSCONV3& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_current_commander.set_toolkit(toolkit);
    reactive_current_commander.set_toolkit(toolkit);
    pll0.set_toolkit(toolkit);

    clear();

    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

string CSCONV3::get_model_name() const
{
    return "CSCONV3";
}

void CSCONV3::set_KPLL(double K)
{
    pll0.set_Kp(K);
}

void CSCONV3::set_KIPLL(double K)
{
    pll0.set_Ki(K);
}

void CSCONV3::set_PLLmax(double pmax)
{
    pll0.set_Pllmax(pmax);
    pll0.set_Pllmin(-pmax);
}

double CSCONV3::get_KPLL() const
{
    return pll0.get_Kp();
}

double CSCONV3::get_KIPLL() const
{
    return pll0.get_Ki();
}

double CSCONV3::get_PLLmax() const
{
    return pll0.get_Pllmax();
}

double CSCONV3::get_active_current_commander_state() const
{
    return active_current_commander.get_state();
}

double CSCONV3::get_reactive_current_commander_state() const
{
    return reactive_current_commander.get_state();
}

double CSCONV3::get_PLL_frequency_integrator_state() const
{
    return pll0.get_frquency_deviation_block_state();
}

double CSCONV3::get_PLL_angle_integrator_state() const
{
    return pll0.get_angle_block_state();
}

void CSCONV3::setup_block_toolkit_and_parameters()
{
    pll0.set_bus_pointer(get_bus_pointer());
}

void CSCONV3::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    double angle_in_rad = get_bus_voltage_angle_in_rad();

    double xeq = get_source_impedance_in_pu_based_on_mbase().imag();
    complex<double> Zsource(0.0, xeq);
    complex<double> Ixy = conj(S/Vxy);
    complex<double> Isource = Ixy;

    double Ix = Isource.real();
    double Iy = Isource.imag();

    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double IP = Ix*cosine + Iy*sine;
    double IQ =-Ix*sine + Iy*cosine;


    double EQ = IQ*(-xeq);

    set_initial_active_current_command_in_pu_based_on_mbase(IP);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_current_command_in_pu_based_on_mbase(IQ);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(EQ);

    active_current_commander.set_output(IP);
    active_current_commander.initialize();

    reactive_current_commander.set_output(IQ);
    reactive_current_commander.initialize();

    pll0.initialize();

}

void CSCONV3::run(DYNAMIC_MODE mode)
{
    double IP = get_Ipcmd_in_pu_based_on_mbase();
    active_current_commander.set_input(IP);
    active_current_commander.run(mode);

    double IQ = get_Iqcmd_in_pu_based_on_mbase();
    reactive_current_commander.set_input(IQ);
    reactive_current_commander.run(mode);

    pll0.run(mode);

}

complex<double> CSCONV3::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
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

complex<double> CSCONV3::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> CSCONV3::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Ixy = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> Z = get_source_impedance_in_pu_based_on_mbase();
    return Ixy*Z;
}

double CSCONV3::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double CSCONV3::get_pll_angle_in_rad()
{
    return pll0.get_pll_angle_in_rad();
}

double CSCONV3::get_pll_frequency_deviation_in_pu()
{
    return pll0.get_pll_frequency_deviation_in_pu();
}

double CSCONV3::get_Ip_in_pu()
{
    return active_current_commander.get_output();
}

double CSCONV3::get_Iq_in_pu()
{
    return reactive_current_commander.get_output();
}
