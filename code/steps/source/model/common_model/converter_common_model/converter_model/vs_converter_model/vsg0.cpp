#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/vsg0.h"

VSG0::VSG0(STEPS& toolkit) : VS_CONVERTER_MODEL(toolkit),
                             virtual_frequency_deviation_block(toolkit),
                             virtual_angle_block(toolkit),
                             virtual_voltage_block(toolkit),
                             convcol(toolkit)
{
    clear();
}

VSG0::VSG0(const VSG0& model) : VS_CONVERTER_MODEL(model.get_toolkit()),
                                virtual_frequency_deviation_block(model.get_toolkit()),
                                virtual_angle_block(model.get_toolkit()),
                                virtual_voltage_block(model.get_toolkit()),
                                convcol(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSG0& VSG0::operator=(const VSG0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

VSG0::~VSG0()
{
    ;
}

void VSG0::clear()
{
    set_Tj_in_s(INFINITE_THRESHOLD);
    set_D(0.0);
    set_Ku(0.0);
    set_Te_in_s(INFINITE_THRESHOLD);
    set_pq_priority_flag(Q_FIRST);
    set_Imax(INFINITE_THRESHOLD);
}

void VSG0::copy_from_const_model(const VSG0& model)
{
    clear();
    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());
    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(get_Imax());
}

string VSG0::get_model_name() const
{
    return "VSG0";
}

void VSG0::set_Tj_in_s(double Tj)
{
    if(fabs(Tj)>FLOAT_EPSILON)
        virtual_frequency_deviation_block.set_T_in_s(Tj);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Tj ("<<Tj<<") is not allowed since it is too small when setting Tj in "
                <<get_model_name()<<" model of "<<get_compound_device_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void VSG0::set_D(double D)
{
    this->D = D;
}

void VSG0::set_Ku(double Ku)
{
    this->Ku = Ku;
}

void VSG0::set_Te_in_s(double Ke)
{
    virtual_voltage_block.set_T_in_s(Ke);
}

void VSG0::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    convcol.set_PQ_priority_flag(flag);
}

void VSG0::set_Imax(double Imax)
{
    convcol.set_Imax(Imax);
}

double VSG0::get_Tj_in_s() const
{
    return virtual_frequency_deviation_block.get_T_in_s();
}

double VSG0::get_D() const
{
    return D;
}

double VSG0::get_Ku() const
{
    return Ku;
}

double VSG0::get_Te_in_s() const
{
    return virtual_voltage_block.get_T_in_s();
}

COL_PQ_PRIORITY VSG0::get_pq_priority_flag() const
{
    return convcol.get_PQ_priority_flag();
}

double VSG0::get_Imax() const
{
    return convcol.get_Imax();
}

double VSG0::get_virtual_frequency_deviation_block_state() const
{
    return virtual_frequency_deviation_block.get_state();
}

double VSG0::get_virtual_angle_block_state() const
{
    return virtual_angle_block.get_state();
}

double VSG0::get_virtual_voltage_block_state() const
{
    return virtual_voltage_block.get_state();
}

complex<double> VSG0::get_virtual_complex_voltage_in_pu_in_xy_axis()
{
    double E = get_virtual_voltage_in_pu();
    double theta = get_virtual_angle_in_rad();
    double Ex = E * steps_cos(theta);
    double Ey = E * steps_sin(theta);
    complex<double> Exy(Ex, Ey);
    return Exy;
}

void VSG0::setup_block_toolkit_and_parameters()
{
    virtual_angle_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());
    convcol.set_device_pointer(get_device_pointer());
}

void VSG0::initialize()
{

    setup_block_toolkit_and_parameters();
    double P = get_initial_P_generation_in_pu();
    double Q = get_initial_Q_generation_in_pu();

    complex<double> S(P, Q);

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

    double Xeq = Zsource.imag();

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real();
    double Iy = Ixy.imag();

    double angle_in_rad = get_bus_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);

    double Ip = Ix*cosine + Iy*sine;
    double Iq =-Ix*sine + Iy*cosine;


    double Eqcmd = Iq*(-Xeq);

    set_Pref_in_pu_based_on_mbase(P);
    set_Qref_in_pu_based_on_mbase(Q);
    set_Vref_in_pu(Eqcmd);

    complex<double> Esource = Vxy + Ixy * Zsource;
    double E = steps_fast_complex_abs(Esource);
    double angle = steps_fast_complex_arg(Esource);


    virtual_angle_block.set_output(angle);
    virtual_angle_block.initialize();

    virtual_frequency_deviation_block.set_output(0.0);
    virtual_frequency_deviation_block.initialize();

    virtual_voltage_block.set_output(E);
    virtual_voltage_block.initialize();


    set_initial_active_current_command_in_pu_based_on_mbase(Ip);
    set_initial_reactive_current_command_in_pu_based_on_mbase(Iq);
    set_initial_active_power_command_in_pu_based_on_mbase(P);
    set_initial_reactive_power_command_in_pu_based_on_mbase(Q);
    set_initial_reactive_voltage_command_in_pu(Eqcmd);
}

void VSG0::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real(), Q = S.imag();

    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    double Ix = Ixy.real(), Iy = Ixy.imag();

    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    double Xeq = Zsource.imag();

    double angle_in_rad = get_bus_voltage_angle_in_rad();
    double sine = steps_sin(angle_in_rad), cosine = steps_cos(angle_in_rad);
    double Iq = -Ix*sine + Iy*cosine;
    double Eq = Iq*(-Xeq);

    double input;

    input = get_Pref_in_pu_based_on_mbase() - P;
    input /= get_virtual_frequency_in_pu();
    input -= (get_D()*get_virtual_frequency_deviation_in_pu());

    virtual_frequency_deviation_block.set_input(input);
    virtual_frequency_deviation_block.run(mode);

    input = virtual_frequency_deviation_block.get_output();

    virtual_angle_block.set_input(input);
    virtual_angle_block.run(mode);

    input = get_Qref_in_pu_based_on_mbase() - Q;
    input += get_Ku()*(get_Vref_in_pu() - Eq);

    virtual_voltage_block.set_input(input);
    virtual_voltage_block.run(mode);
}

complex<double> VSG0::get_limited_PQ_current_order()
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

complex<double> VSG0::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    return Exy/Zsource;
}

complex<double> VSG0::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_bus_complex_voltage_in_pu();
    complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();
    complex<double> Ixy = (Exy - Vxy) / Zsource;
    return Ixy;
}

complex<double> VSG0::get_internal_voltage_in_pu_in_xy_axis()
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

double VSG0::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double pterm = get_terminal_active_power_in_pu_based_on_mbase();
    double rsource = get_source_impedance_in_pu_based_on_mbase().real();
    double iterm = get_terminal_current_in_pu_based_on_mbase();
    return pterm+rsource*iterm*iterm;
}

double VSG0::get_virtual_frequency_deviation_in_pu() const
{
    return virtual_frequency_deviation_block.get_output();
}

double VSG0::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double VSG0::get_virtual_voltage_in_pu() const
{
    return virtual_voltage_block.get_output();
}
