#include "header/model/converter_common_model/converter_model/vs_converter_model/general_gfm.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
GENERAL_GFM::GENERAL_GFM(STEPS& toolkit) : VS_CONVERTER_MODEL(toolkit),
                             active_power_regulator_block(toolkit),
                             reactive_power_regulator_block(toolkit),
                             virtual_angle_block(toolkit),
                             convcol(toolkit)
{
    clear();
}

GENERAL_GFM::GENERAL_GFM(const GENERAL_GFM& model) : VS_CONVERTER_MODEL(model.get_toolkit()),
                                active_power_regulator_block(model.get_toolkit()),
                                reactive_power_regulator_block(model.get_toolkit()),
                                virtual_angle_block(model.get_toolkit()),
                                convcol(model.get_toolkit())
{
    copy_from_const_model(model);
}

GENERAL_GFM& GENERAL_GFM::operator=(const GENERAL_GFM& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

GENERAL_GFM::~GENERAL_GFM()
{
    ;
}

void GENERAL_GFM::clear()
{
    set_Kp(0.0);
    set_Tp_in_s(INFINITE_THRESHOLD);
    set_Kq(0.0);
    set_Tq_in_s(INFINITE_THRESHOLD);
    set_pq_priority_flag(Q_FIRST);
    set_Imax(INFINITE_THRESHOLD);
}

void GENERAL_GFM::copy_from_const_model(const GENERAL_GFM& model)
{
    clear();
    set_Kp(model.get_Kp());
    set_Tp_in_s(model.get_Tp_in_s());
    set_Kq(model.get_Kq());
    set_Tq_in_s(model.get_Tq_in_s());
    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(get_Imax());
}


void GENERAL_GFM::set_Kp(double Kp)
{
    active_power_regulator_block.set_K(Kp);
}

void GENERAL_GFM::set_Tp_in_s(double Tp)
{
    active_power_regulator_block.set_T_in_s(Tp);
}

void GENERAL_GFM::set_Kq(double Kq)
{
    reactive_power_regulator_block.set_K(Kq);
}

void GENERAL_GFM::set_Tq_in_s(double Tq)
{
    reactive_power_regulator_block.set_T_in_s(Tq);
}

void GENERAL_GFM::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    convcol.set_PQ_priority_flag(flag);
}

void GENERAL_GFM::set_Imax(double Imax)
{
    convcol.set_Imax(Imax);
}

double GENERAL_GFM::get_Kp() const
{
    return active_power_regulator_block.get_K();
}

double GENERAL_GFM::get_Tp_in_s() const
{
    return active_power_regulator_block.get_T_in_s();
}

double GENERAL_GFM::get_Kq() const
{
    return reactive_power_regulator_block.get_K();
}

double GENERAL_GFM::get_Tq_in_s() const
{
    return reactive_power_regulator_block.get_T_in_s();
}

COL_PQ_PRIORITY GENERAL_GFM::get_pq_priority_flag() const
{
    return convcol.get_PQ_priority_flag();
}

double GENERAL_GFM::get_Imax() const
{
    return convcol.get_Imax();
}

double GENERAL_GFM::get_active_power_regulator_block_state() const
{
    return active_power_regulator_block.get_state();
}

double GENERAL_GFM::get_reactive_power_regulator_block_state() const
{
    return reactive_power_regulator_block.get_state();
}

double GENERAL_GFM::get_virtual_angle_block_state() const
{
    return virtual_angle_block.get_state();
}

complex<double> GENERAL_GFM::get_virtual_complex_voltage_in_pu_in_xy_axis()
{
    double E = get_virtual_voltage_in_pu();
    double theta = get_virtual_angle_in_rad();
    double Ex = E * steps_cos(theta);
    double Ey = E * steps_sin(theta);
    complex<double> Exy(Ex, Ey);
    return Exy;
}

void GENERAL_GFM::setup_block_toolkit_and_parameters()
{
    virtual_angle_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());
    convcol.set_device_pointer(get_device_pointer());
}

void GENERAL_GFM::initialize()
{
    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();
    complex<double> S(P, Q);

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real();
    double Iy = Ixy.imag();

    double angle_in_rad = get_terminal_voltage_angle_in_rad();
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

    active_power_regulator_block.set_output(0.0);
    active_power_regulator_block.initialize();

    reactive_power_regulator_block.set_output(0.0);
    reactive_power_regulator_block.initialize();

    set_initial_active_current_command_in_pu_based_on_mbase(Ip);
    set_initial_reactive_current_command_in_pu_based_on_mbase(Iq);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(E);
}

void GENERAL_GFM::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real(), Q = S.imag();

    double input;

    input = get_Pref_in_pu_based_on_mbase() - P;
    active_power_regulator_block.set_input(input);
    active_power_regulator_block.run(mode);

    input = active_power_regulator_block.get_output();
    virtual_angle_block.set_input(input);
    virtual_angle_block.run(mode);

    input = get_Qref_in_pu_based_on_mbase() - Q;
    reactive_power_regulator_block.set_input(input);
    reactive_power_regulator_block.run(mode);
}

complex<double> GENERAL_GFM::get_limited_PQ_current_order()
{
    complex<double> Exy = get_virtual_complex_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy_guess = (Exy - Vxy) / Zsource;

    double Ix_guess = Ixy_guess.real();
    double Iy_guess = Ixy_guess.imag();
    double angle_in_rad = get_terminal_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Ip_guess = Ix_guess*cosine + Iy_guess*sine;
    double Iq_guess =-Ix_guess*sine + Iy_guess*cosine;
    complex<double> I_guess (Ip_guess,Iq_guess);
    return convcol.get_limited_PQ_current_order(I_guess);
}

complex<double> GENERAL_GFM::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    return Exy/Zsource;
}

complex<double> GENERAL_GFM::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = (Exy - Vxy) / Zsource;
    return Ixy;
}

complex<double> GENERAL_GFM::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    double angle_in_rad = get_terminal_voltage_angle_in_rad();
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

double GENERAL_GFM::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double GENERAL_GFM::get_virtual_frequency_deviation_in_pu() const
{
    return active_power_regulator_block.get_output();
}

double GENERAL_GFM::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double GENERAL_GFM::get_virtual_voltage_in_pu() const
{
    return reactive_power_regulator_block.get_output() + get_initial_reactive_voltage_command_in_pu();
}
