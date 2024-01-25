#include "header/model/vsg_model/vsgcontrol2.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
VSGCONTROL2::VSGCONTROL2(STEPS& toolkit) : VSG_MODEL2(toolkit),
                                           virtual_frequency_deviation_block(toolkit),
                                           virtual_angle_block(toolkit),
                                           virtual_voltage_block(toolkit)
{
    clear();
}

VSGCONTROL2::VSGCONTROL2(const VSGCONTROL2& model) : VSG_MODEL2(model.get_toolkit()),
                                                     virtual_frequency_deviation_block(model.get_toolkit()),
                                                     virtual_angle_block(model.get_toolkit()),
                                                     virtual_voltage_block(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSGCONTROL2& VSGCONTROL2::operator=(const VSGCONTROL2& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

VSGCONTROL2::~VSGCONTROL2()
{
    ;
}

void VSGCONTROL2::clear()
{
    set_Tj_in_s(INFINITE_THRESHOLD);
    set_D(0.0);
    set_Ku(0.0);
    set_Te_in_s(INFINITE_THRESHOLD);
}

void VSGCONTROL2::copy_from_const_model(const VSGCONTROL2& model)
{
    clear();
    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());
}

void VSGCONTROL2::set_Tj_in_s(double Tj)
{
    if(fabs(Tj)>FLOAT_EPSILON)
        virtual_frequency_deviation_block.set_T_in_s(Tj);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Tj ("<<Tj<<") is not allowed since it is too small when setting Tj in "
                <<get_model_name()<<" model";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void VSGCONTROL2::set_D(double D)
{
    this->D = D;
}

void VSGCONTROL2::set_Ku(double Ku)
{
    this->Ku = Ku;
}

void VSGCONTROL2::set_Te_in_s(double Ke)
{
    virtual_voltage_block.set_T_in_s(Ke);
}

double VSGCONTROL2::get_Tj_in_s() const
{
    return virtual_frequency_deviation_block.get_T_in_s();
}

double VSGCONTROL2::get_D() const
{
    return D;
}

double VSGCONTROL2::get_Ku() const
{
    return Ku;
}

double VSGCONTROL2::get_Te_in_s() const
{
    return virtual_voltage_block.get_T_in_s();
}

string VSGCONTROL2::get_model_name() const
{
    return "VSGCONTROL2";
}

void VSGCONTROL2::initialize()
{
    initialize_base_frequency_and_base_angle_speed();

    virtual_angle_block.set_T_in_s(1.0/get_base_angle_speed_in_radps());

    double angle = get_initial_virtual_angle_in_rad();

    virtual_angle_block.set_output(angle);
    virtual_angle_block.initialize();

    virtual_frequency_deviation_block.set_output(0.0);
    virtual_frequency_deviation_block.initialize();

    double E = get_initial_virtual_voltage_in_pu();
    virtual_voltage_block.set_output(E);
    virtual_voltage_block.initialize();
}

void VSGCONTROL2::run(DYNAMIC_MODE mode)
{
    double P = get_P_in_pu_based_on_mbase();
    double Q = get_Q_in_pu_based_on_mbase();
    double V = get_V_in_pu();

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
    input += get_Ku()*(get_Vref_in_pu() - V);

    virtual_voltage_block.set_input(input);
    virtual_voltage_block.run(mode);
}

double VSGCONTROL2::get_virtual_frequency_deviation_in_Hz() const
{
    return virtual_frequency_deviation_block.get_output();
}

double VSGCONTROL2::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double VSGCONTROL2::get_virtual_voltage_in_pu() const
{
    return virtual_voltage_block.get_output();
}

string VSGCONTROL2::get_standard_psse_string() const
{
    ostringstream osstream;

    string model_name = "'"+get_model_name()+"'";
    osstream<<setw(10)<<model_name<<", "
            <<setw(8)<<setprecision(6)<<get_Tj_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Ku()<<", "
            <<setw(8)<<setprecision(6)<<get_Te_in_s()<<" /";

    return osstream.str();
}
