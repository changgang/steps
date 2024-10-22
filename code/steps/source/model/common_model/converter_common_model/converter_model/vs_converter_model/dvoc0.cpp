#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/dvoc0.h"

DVOC0::DVOC0(STEPS& toolkit) : VS_CONVERTER_MODEL(toolkit),
                             virtual_angle_block(toolkit),
                             virtual_voltage_block(toolkit),
                             convcol(toolkit)
{
    clear();
}

DVOC0::DVOC0(const DVOC0& model) : VS_CONVERTER_MODEL(model.get_toolkit()),
                             virtual_angle_block(model.get_toolkit()),
                             virtual_voltage_block(model.get_toolkit()),
                             convcol(model.get_toolkit())
{
    copy_from_const_model(model);
}

DVOC0& DVOC0::operator=(const DVOC0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

DVOC0::~DVOC0()
{
    ;
}

void DVOC0::clear()
{
    virtual_angle_block.set_limiter_type(NO_LIMITER);
    virtual_voltage_block.set_limiter_type(NO_LIMITER);
    set_eta(0.0);
    set_alpha(0.001);
    set_deltafmax_in_pu(0.0);
    set_deltafmin_in_pu(0.0);
    set_pq_priority_flag(Q_FIRST);
    set_Imax(INFINITE_THRESHOLD);
}

void DVOC0::copy_from_const_model(const DVOC0& model)
{
    clear();
    set_eta(model.get_eta());
    set_alpha(model.get_alpha());
    set_deltafmax_in_pu(model.get_deltafmax_in_pu());
    set_deltafmin_in_pu(model.get_deltafmin_in_pu());
    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(model.get_Imax());
}

string DVOC0::get_model_name() const
{
    return "DVOC0";
}

void DVOC0::set_eta(double eta)
{
    this->eta = eta;
}

void DVOC0::set_alpha(double alpha)
{
    this->alpha = alpha;
}

void DVOC0::set_deltafmax_in_pu(double deltafmax)
{
    this->deltafmax_pu = deltafmax;
}

void DVOC0::set_deltafmin_in_pu(double deltafmin)
{
    this->deltafmin_pu = deltafmin;
}

void DVOC0::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    convcol.set_PQ_priority_flag(flag);
}

void DVOC0::set_Imax(double Imax)
{
    convcol.set_Imax(Imax);
}

double DVOC0::get_eta() const
{
    return eta;
}

double DVOC0::get_alpha() const
{
    return alpha;
}

double DVOC0::get_deltafmax_in_pu() const
{
    return deltafmax_pu;
}

double DVOC0::get_deltafmin_in_pu() const
{
    return deltafmin_pu;
}

COL_PQ_PRIORITY DVOC0::get_pq_priority_flag() const
{
    return convcol.get_PQ_priority_flag();
}

double DVOC0::get_Imax() const
{
    return convcol.get_Imax();
}

double DVOC0::get_virtual_angle_block_state() const
{
    return virtual_angle_block.get_state();
}

double DVOC0::get_virtual_voltage_block_state() const
{
    return virtual_voltage_block.get_state();
}

complex<double> DVOC0::get_virtual_complex_voltage_in_pu_in_xy_axis()
{
    double E = get_virtual_voltage_in_pu();
    double theta = get_virtual_angle_in_rad();
    double Ex = E * steps_cos(theta);
    double Ey = E * steps_sin(theta);
    complex<double> Exy(Ex, Ey);
    return Exy;
}

void DVOC0::setup_block_toolkit_and_parameters()
{
    virtual_angle_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());
    virtual_voltage_block.set_T_in_s(1.0);
    convcol.set_device_pointer(get_device_pointer());
}

void DVOC0::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real();
    double Iy = Ixy.imag();

    double angle_in_rad = get_bus_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);

    double Ip = Ix*cosine + Iy*sine;
    double Iq =-Ix*sine + Iy*cosine;

    complex<double> Esource = Vxy + Ixy * Zsource;
    double E = steps_fast_complex_abs(Esource);
    double angle = steps_fast_complex_arg(Esource);

    set_Pref_in_pu_based_on_mbase(P);
    set_Qref_in_pu_based_on_mbase(Q);
    set_Vref_in_pu(E);

    virtual_angle_block.set_output(angle);
    virtual_angle_block.initialize();

    virtual_voltage_block.set_output(E);
    virtual_voltage_block.initialize();

    set_initial_active_current_command_in_pu_based_on_mbase(Ip);
    set_initial_reactive_current_command_in_pu_based_on_mbase(Iq);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(E);
}

void DVOC0::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real();
    double Q = S.imag();

    double input;

    double Pref = get_Pref_in_pu_based_on_mbase();
    double Qref = get_Qref_in_pu_based_on_mbase();
    double Vref = get_initial_reactive_voltage_command_in_pu();

    double E = virtual_voltage_block.get_output();
    input = eta * (Pref / (Vref * Vref) - P / (E * E));
    if(input >= deltafmin_pu and input <= deltafmax_pu)
        ;
    else
    {
        if(input > deltafmax_pu)
            input = deltafmax_pu;
        else
            input = deltafmin_pu;
    }
    virtual_angle_block.set_input(input);
    virtual_angle_block.run(mode);

    double input1 = eta * (Qref / (Vref * Vref) - Q / (E * E));
    double input2 = (1 - (E * E) / (Vref * Vref)) * eta * alpha;
    input = (input1 + input2) * E;
    virtual_voltage_block.set_input(input);
    virtual_voltage_block.run(mode);
}

complex<double> DVOC0::get_limited_PQ_current_order()
{
    complex<double> Exy = get_virtual_complex_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy_guess = (Exy - Vxy) / Zsource;

    double Ix_guess = Ixy_guess.real();
    double Iy_guess = Ixy_guess.imag();
    double angle_in_rad = get_bus_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Ip_guess = Ix_guess*cosine + Iy_guess*sine;
    double Iq_guess =-Ix_guess*sine + Iy_guess*cosine;
    complex<double> I_guess (Ip_guess,Iq_guess);
    return convcol.get_limited_PQ_current_order(I_guess);
}

complex<double> DVOC0::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    return Exy/Zsource;
}

complex<double> DVOC0::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = (Exy - Vxy) / Zsource;
    return Ixy;
}

complex<double> DVOC0::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    double angle_in_rad = get_bus_voltage_angle_in_rad();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> I = get_limited_PQ_current_order();
    double Ip = I.real();
    double Iq = I.imag();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Ix = Ip*cosine - Iq*sine;
    double Iy = Ip*sine + Iq*cosine;
    complex<double> Ixy(Ix,Iy);
    complex<double> Exy = Vxy + Ixy * Zsource;
    return Exy;
}

double DVOC0::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double DVOC0::get_virtual_frequency_deviation_in_pu() const
{
    return virtual_angle_block.get_input();

}

double DVOC0::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double DVOC0::get_virtual_voltage_in_pu() const
{
    return virtual_voltage_block.get_output();
}
