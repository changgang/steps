#include "header/model/converter_common_models/source_converter_model/gfrmc_model/vsg_model/vsg0.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
VSG0::VSG0(STEPS& toolkit) : VSG_MODEL(toolkit),
                             virtual_frequency_deviation_block(toolkit),
                             virtual_angle_block(toolkit),
                             virtual_voltage_block(toolkit)
{
    clear();
}

VSG0::VSG0(const VSG0& model) : VSG_MODEL(model.get_toolkit()),
                                virtual_frequency_deviation_block(model.get_toolkit()),
                                virtual_angle_block(model.get_toolkit()),
                                virtual_voltage_block(model.get_toolkit())
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
}

void VSG0::copy_from_const_model(const VSG0& model)
{
    clear();
    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());
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

string VSG0::get_model_name() const
{
    return "VSG0";
}

void VSG0::initialize()
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

void VSG0::run(DYNAMIC_MODE mode)
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

double VSG0::get_virtual_frequency_deviation_in_Hz() const
{
    return virtual_frequency_deviation_block.get_output() * get_base_frequency_in_Hz();
}

double VSG0::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double VSG0::get_virtual_voltage_in_pu() const
{
    return virtual_voltage_block.get_output();
}

string VSG0::get_standard_psse_string() const
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
