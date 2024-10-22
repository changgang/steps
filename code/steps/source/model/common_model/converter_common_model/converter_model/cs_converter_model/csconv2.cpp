#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/csconv2.h"

CSCONV2::CSCONV2(STEPS& toolkit) : CS_CONVERTER_MODEL(toolkit),
                               active_current_commander(toolkit),
                               LVPL_voltage_sensor(toolkit),
                               reactive_voltage_commander(toolkit),
                               pll0(toolkit)

{
    clear();
}

CSCONV2::CSCONV2(const CSCONV2& model) : CS_CONVERTER_MODEL(model.get_toolkit()),
                               active_current_commander(model.get_toolkit()),
                               LVPL_voltage_sensor(model.get_toolkit()),
                               reactive_voltage_commander(model.get_toolkit()),
                               pll0(model.get_toolkit())
{
    copy_from_const_model(model);
}

CSCONV2& CSCONV2::operator=(const CSCONV2& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

CSCONV2::~CSCONV2()
{
    ;
}

void CSCONV2::clear()
{
    active_current_commander.set_limiter_type(NO_LIMITER);

    reactive_voltage_commander.set_limiter_type(NO_LIMITER);
    reactive_voltage_commander.set_K(1.0);

    LVPL_voltage_sensor.set_limiter_type(NO_LIMITER);
    LVPL_voltage_sensor.set_K(1.0);
    pll0.clear();
}

void CSCONV2::copy_from_const_model(const CSCONV2& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    active_current_commander.set_toolkit(toolkit);
    LVPL_voltage_sensor.set_toolkit(toolkit);
    reactive_voltage_commander.set_toolkit(toolkit);
    pll0.set_toolkit(toolkit);

    clear();
    set_converter_activer_current_command_T_in_s(model.get_converter_activer_current_command_T_in_s());
    set_LVPL_max_rate_of_active_current_change(model.get_LVPL_max_rate_of_active_current_change());
    set_LVPL_voltage_sensor_T_in_s(model.get_LVPL_voltage_sensor_T_in_s());

    set_converter_reactiver_voltage_command_T_in_s(model.get_converter_reactiver_voltage_command_T_in_s());
    set_HVRC_voltage_in_pu(model.get_HVRC_voltage_in_pu());
    set_HVRC_current_in_pu(model.get_HVRC_current_in_pu());

    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

string CSCONV2::get_model_name() const
{
    return "CSCONV2";
}

void CSCONV2::set_converter_activer_current_command_T_in_s(double t)
{
    active_current_commander.set_T_in_s(t);
}

void CSCONV2::set_converter_reactiver_voltage_command_T_in_s(double t)
{
    reactive_voltage_commander.set_T_in_s(t);
}

void CSCONV2::set_KPLL(double K)
{
    pll0.set_Kp(K);
}

void CSCONV2::set_KIPLL(double K)
{
    pll0.set_Ki(K);
}

void CSCONV2::set_PLLmax(double pmax)
{
    pll0.set_Pllmax(pmax);
}

void CSCONV2::set_PLLmin(double pmin)
{
    pll0.set_Pllmin(pmin);
}

void CSCONV2::set_HVRC_voltage_in_pu(double v)
{
    HVRCR_voltage = v;
}

void CSCONV2::set_HVRC_current_in_pu(double i)
{
    HVRCR_current = i;
}

void CSCONV2::set_LVPL_max_rate_of_active_current_change(double rate)
{
    LVPL_active_power_change_rate = rate;
}

void CSCONV2::set_LVPL_voltage_sensor_T_in_s(double t)
{
    LVPL_voltage_sensor.set_T_in_s(t);
}

void CSCONV2::set_LVPL_low_voltage_in_pu(double v)
{
    lvpl.set_low_voltage_in_pu(v);
}
void CSCONV2::set_LVPL_high_voltage_in_pu(double v)
{
    lvpl.set_high_voltage_in_pu(v);
}
void CSCONV2::set_LVPL_gain_at_high_voltage(double k)
{
    lvpl.set_gain_at_high_voltage(k);
}

double CSCONV2::get_converter_activer_current_command_T_in_s() const
{
    return active_current_commander.get_T_in_s();
}

double CSCONV2::get_converter_reactiver_voltage_command_T_in_s() const
{
    return reactive_voltage_commander.get_T_in_s();
}

double CSCONV2::get_KPLL() const
{
    return pll0.get_Kp();
}

double CSCONV2::get_KIPLL() const
{
    return pll0.get_Ki();
}

double CSCONV2::get_PLLmax() const
{
    return pll0.get_Pllmax();
}

double CSCONV2::get_PLLmin() const
{
    return pll0.get_Pllmin();
}

double CSCONV2::get_HVRC_voltage_in_pu() const
{
    return HVRCR_voltage;
}

double CSCONV2::get_HVRC_current_in_pu() const
{
    return HVRCR_current;
}

double CSCONV2::get_LVPL_max_rate_of_active_current_change() const
{
    return LVPL_active_power_change_rate;
}

double CSCONV2::get_LVPL_voltage_sensor_T_in_s() const
{
    return LVPL_voltage_sensor.get_T_in_s();
}

double CSCONV2::get_LVPL_low_voltage_in_pu() const
{
    return lvpl.get_low_voltage_in_pu();
}

double CSCONV2::get_LVPL_high_voltage_in_pu() const
{
    return lvpl.get_high_voltage_in_pu();
}

double CSCONV2::get_LVPL_gain_at_high_voltage() const
{
    return lvpl.get_gain_at_hig_voltage();
}

double CSCONV2::get_active_current_commander_state() const
{
    return active_current_commander.get_state();
}

double CSCONV2::get_reactive_voltage_commander_state() const
{
    return reactive_voltage_commander.get_state();
}

double CSCONV2::get_PLL_frequency_integrator_state() const
{
    return pll0.get_frquency_deviation_block_state();
}

double CSCONV2::get_PLL_angle_integrator_state() const
{
    return pll0.get_angle_block_state();
}

double CSCONV2::get_LVPL_voltage_sensor_state() const
{
    return LVPL_voltage_sensor.get_state();
}

void CSCONV2::setup_block_toolkit_and_parameters()
{
    pll0.set_bus_pointer(get_bus_pointer());
}

void CSCONV2::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    double V = get_bus_voltage_in_pu();
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

    LVPL_voltage_sensor.set_output(V);
    LVPL_voltage_sensor.initialize();


    pll0.initialize();

}

void CSCONV2::run(DYNAMIC_MODE mode)
{
    double V = get_bus_voltage_in_pu();

    LVPL_voltage_sensor.set_input(V);
    LVPL_voltage_sensor.run(mode);

    double lvpl_order = lvpl.get_LVPL_order(LVPL_voltage_sensor.get_output());

    double IP = get_Ipcmd_in_pu_based_on_mbase();

    double input = active_current_commander.get_output();
    if(input>lvpl_order)
        input = lvpl_order;

    input = IP - input;
    double lvpl_rate_max = get_LVPL_max_rate_of_active_current_change();
    if(input>lvpl_rate_max)
        input = lvpl_rate_max;

    active_current_commander.set_input(input);
    active_current_commander.run(mode);

    double EQ = get_Eqcmd_in_pu();

    reactive_voltage_commander.set_input(EQ);
    reactive_voltage_commander.run(mode);

    pll0.run(mode);

}

complex<double> CSCONV2::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    double V = get_bus_voltage_in_pu();

    double Xeq = get_source_impedance_in_pu_based_on_mbase().imag();

    // low voltage active current logic
    double V_LVACR_high = V;
    double V_LVACR_low = 0.4;
    if(V<=V_LVACR_low)
        V_LVACR_low = V;
    if(V<=0.8)
        V_LVACR_high = 0.8;

    double Ip = active_current_commander.get_output();

    double lvpl_order = lvpl.get_LVPL_order(LVPL_voltage_sensor.get_output());
    if(Ip>lvpl_order)
        Ip =lvpl_order;
    Ip = Ip*(V-V_LVACR_low)/(V_LVACR_high-V_LVACR_low);

    // high voltage reactive current logic
    /*double v_hvrc = get_HVRC_voltage_in_pu();
    double overvoltage_correction_factor = 0.0;
    if(V>=v_hvrc)
        overvoltage_correction_factor=0.7*(V-v_hvrc);


    double Iq = -reactive_voltage_commander.get_output()/Xeq + overvoltage_correction_factor/V;*/
    double Iq = -reactive_voltage_commander.get_output()/Xeq;
    double hvrc_i = get_HVRC_current_in_pu();
    if(Iq<-hvrc_i)
        Iq = -hvrc_i;

    double pll_angle = get_pll_angle_in_rad();

    double sine = steps_sin(pll_angle), cosine = steps_cos(pll_angle);
    double Ix = Ip*cosine - Iq*sine;
    double Iy = Ip*sine + Iq*cosine;

    complex<double> Ixy(Ix, Iy);

    return Ixy;

}

complex<double> CSCONV2::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> CSCONV2::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Ixy = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> Z = get_source_impedance_in_pu_based_on_mbase();
    return Ixy*Z;
}

double CSCONV2::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double CSCONV2::get_pll_angle_in_rad()
{
    return pll0.get_pll_angle_in_rad();
}

double CSCONV2::get_pll_frequency_deviation_in_pu()
{
    return pll0.get_pll_frequency_deviation_in_pu();
}
