#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;
GENCLS::GENCLS()
{
    clear();
}

GENCLS::~GENCLS()
{
    clear();
}

void GENCLS::clear()
{
    prepare_model_variable_table();
}
void GENCLS::copy_from_const_model(const GENCLS& model)
{
    clear();

    this->set_Tj_in_s(model.get_Tj_in_s());
    this->set_D(model.get_D());
    this->set_Rs(model.get_Rs());
    this->set_Xdp(model.get_Xdp());

}
GENCLS::GENCLS(const GENCLS& model):SYNC_GENERATOR_MODEL()
{
    copy_from_const_model(model);
}

GENCLS& GENCLS::operator=(const GENCLS& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string GENCLS::get_model_name() const
{
    return "GENCLS";
}

double GENCLS::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double GENCLS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void GENCLS::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void GENCLS::set_model_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void GENCLS::update_source_impedance()
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        complex<double> Zs = generator->get_source_impedance_in_pu();
        double rs = Zs.real();
        double xs = Zs.imag();
        set_Rs(rs);
        set_Xdp(xs);
    }
}


bool GENCLS::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool GENCLS::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<5)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double H, D;

    size_t i=3;
    H = get_double_data(dyrdata[i],"0.0"); i++;
    D = get_double_data(dyrdata[i],"0.0");

    set_H_in_s(H);
    set_D(D);

    is_successful = true;

    return is_successful;
}

bool GENCLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void GENCLS::initialize()
{
    update_source_impedance();

    // rotor angle and speed
    initialize_rotor_angle();

    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();

    double speed = 0.0;
    rotor_speed_block->set_output(speed);
    rotor_speed_block->initialize();

    GENERATOR* generator = get_generator_pointer();
    double mbase = get_mbase_in_MVA();

    double rs = get_Rs();
    double xdp = get_Xdp();
    complex<double> Zsource(rs, xdp);

    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    complex<double> S(P/mbase,Q/mbase);

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();

    // mechanical power
    complex<double> Ixy = conj(S/Vxy);
    double I = steps_fast_complex_abs(Ixy);
    double pmech0 = S.real()+I*I*rs;
    set_initial_mechanical_power_in_pu_based_on_mbase(pmech0);
    // excitation voltage
    complex<double> Exy = Vxy+Ixy*Zsource;
    double efd0 = steps_fast_complex_abs(Exy);
    set_initial_excitation_voltage_in_pu(efd0);
}

void GENCLS::initialize_rotor_angle()
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    double rs = get_Rs();
    double xdp = get_Xdp();
    complex<double> Z(rs, xdp);

    GENERATOR* generator = get_generator_pointer();
    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    double mbase = get_mbase_in_MVA();

    complex<double> S(P/mbase,Q/mbase);

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);

    complex<double> Exy = Vxy+Ixy*Z;

    double rotor_angle = steps_fast_complex_arg(Exy/Vxy)+ steps_fast_complex_arg(Vxy);

    rotor_angle_block->set_output(rotor_angle);
    rotor_angle_block->initialize();

    set_flag_model_initialized_as_true();
}

void GENCLS::run(DYNAMIC_MODE mode)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    double fbase = get_bus_base_frequency_in_Hz();

    double damping = get_D();

    double pmech = get_mechanical_power_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu();
    double omega = get_rotor_speed_in_pu();
    double tmech = pmech/omega;

    //double pelec = get_air_gap_power_in_pu_based_on_mbase();
    double telec = get_air_gap_torque_in_pu_based_on_mbase();

    double delta_torque = tmech-telec-damping*speed;
    //double input = pmech-pelec - D*speed;
    rotor_speed_block->set_input(delta_torque);
    rotor_speed_block->run(mode);

    speed = get_rotor_speed_deviation_in_pu();

    speed = 2.0*PI*fbase*speed;
    rotor_angle_block->set_input(speed);
    rotor_angle_block->run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> GENCLS::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Z(get_Rs(), get_Xdp());
    double mbase = get_mbase_in_MVA();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    return Exy/Z*mbase/sbase;
}

complex<double> GENCLS::get_terminal_complex_current_in_pu_in_dq_axis_based_on_mbase()
{
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    double angle = get_rotor_angle_in_rad();

    return xy2dq_with_angle_in_rad(Ixy, angle);
}

complex<double> GENCLS::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Zsource(get_Rs(), get_Xdp());
    complex<double> Ixy = (Exy-Vxy)/Zsource;

    return Ixy;
}

complex<double> GENCLS::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    return Ixy*mbase/sbase;
}

double GENCLS::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double GENCLS::get_terminal_current_in_pu_based_on_sbase()
{
    double I = get_terminal_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    return I*mbase/sbase;
}


void GENCLS::check()
{
    ;
}

void GENCLS::report()
{
    ostringstream osstream;
    osstream<< get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void GENCLS::save()
{
    ;
}

string GENCLS::get_standard_model_string() const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<get_H_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_D()<<"  /";
    return osstream.str();
}

void GENCLS::prepare_model_variable_table()
{
    size_t i=0;
    add_model_variable_name_and_index_pair("ROTOR ANGLE IN DEG", i); i++;
    add_model_variable_name_and_index_pair("ROTOR SPEED DEVIATION IN PU", i); i++;
    add_model_variable_name_and_index_pair("AIR GAP POWER IN PU", i); i++;
    add_model_variable_name_and_index_pair("AIR GAP POWER IN MW", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL P IN PU", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL P IN MW", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL Q IN PU", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL Q IN MVAR", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL S IN PU", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL S IN MVA", i); i++;
    add_model_variable_name_and_index_pair("MECHANICAL POWER IN PU", i); i++;
    add_model_variable_name_and_index_pair("MECHANICAL POWER IN MW", i); i++;
    add_model_variable_name_and_index_pair("EXCITATION VOLTAGE IN PU", i); i++;
    add_model_variable_name_and_index_pair("INTERNAL VOLTAGE IN PU", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL CURRENT IN PU", i); i++;
    add_model_variable_name_and_index_pair("TERMINAL CURRENT IN KA", i); i++;
    add_model_variable_name_and_index_pair("STATE@ROTOR ANGLE BLOCK", i); i++;
    add_model_variable_name_and_index_pair("STATE@ROTOR SPEED BLOCK", i); i++;
}

double GENCLS::get_variable_with_name(string var_name)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    var_name = string2upper(var_name);
    if(var_name == "GENERATOR ROTOR ANGLE IN DEG")
        return get_rotor_angle_in_deg();
    if(var_name == "GENERATOR ROTOR SPEED DEVIATION IN PU")
        return get_rotor_speed_deviation_in_pu();
    if(var_name == "GENERATOR AIR GAP POWER IN PU")
        return get_air_gap_power_in_pu_based_on_mbase();
    if(var_name == "GENERATOR AIR GAP POWER IN MW")
        return get_air_gap_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
    if(var_name == "GENERATOR TERMINAL P IN PU")
        return get_terminal_active_power_in_pu_based_on_mbase();
    if(var_name == "GENERATOR TERMINAL P IN MW")
        return get_terminal_active_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
    if(var_name == "GENERATOR TERMINAL Q IN PU")
        return get_terminal_reactive_power_in_pu_based_on_mbase();
    if(var_name == "GENERATOR TERMINAL Q IN MW")
        return get_terminal_reactive_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
    if(var_name == "GENERATOR TERMINAL S IN PU")
        return steps_fast_complex_abs(get_terminal_complex_power_in_pu_based_on_mbase());
    if(var_name == "GENERATOR TERMINAL S IN MVA")
        return steps_fast_complex_abs(get_terminal_complex_power_in_pu_based_on_mbase())*get_mbase_in_MVA();
    if(var_name == "GENERATOR MECHANICAL POWER IN PU")
        return get_mechanical_power_in_pu_based_on_mbase();
    if(var_name == "GENERATOR MECHANICAL POWER IN MW")
        return get_mechanical_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
    if(var_name == "GENERATOR EXCITATION VOLTAGE IN PU")
        return get_excitation_voltage_in_pu();
    if(var_name == "GENERATOR INTERNAL VOLTAGE IN PU")
        return steps_fast_complex_abs(get_internal_voltage_in_pu_in_xy_axis());
    if(var_name == "GENERATOR TERMINAL CURRENT IN PU")
        return get_terminal_current_in_pu_based_on_mbase();
    if(var_name == "GENERATOR TERMINAL CURRENT IN KA")
    {
        POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
        GENERATOR* generator = get_generator_pointer();
        size_t bus = generator->get_generator_bus();
        double vbase = psdb.get_bus_base_voltage_in_kV(bus);
        double mbase = generator->get_mbase_in_MVA();
        double ibase = mbase/sqrt(3.0)/vbase;
        return get_terminal_current_in_pu_based_on_mbase()*ibase;
    }
    if(var_name == "STATE@ROTOR ANGLE BLOCK")
        return rotor_angle_block->get_state();
    if(var_name == "STATE@ROTOR SPEED BLOCK")
        return rotor_speed_block->get_state();

    return 0.0;
}

double GENCLS::get_air_gap_power_in_pu_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Exy*conj(Ixy);
    return S.real();
}

double GENCLS::get_air_gap_power_in_MW()
{
    return get_air_gap_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double GENCLS::get_air_gap_torque_in_pu_based_on_mbase()
{
    double P = get_air_gap_power_in_pu_based_on_mbase();
    double omega = get_rotor_speed_in_pu();

    return P/omega;
}


double GENCLS::get_accelerating_power_in_pu_based_on_mbase()
{
    return get_mechanical_power_in_pu_based_on_mbase()-get_air_gap_power_in_pu_based_on_mbase();
}


double GENCLS::get_accelerating_power_in_MW()
{
    return get_accelerating_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}


complex<double> GENCLS::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> GENCLS::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double GENCLS::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double GENCLS::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double GENCLS::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double GENCLS::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double GENCLS::get_field_current_in_pu_based_on_mbase()
{
    return 0.0;
}

double GENCLS::get_rotor_angle_in_rad()
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    return rotor_angle_block->get_output();
}

double GENCLS::get_rotor_angle_in_deg()
{
    return rad2deg(get_rotor_angle_in_rad());
}

double GENCLS::get_rotor_speed_deviation_in_pu()
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();

    return rotor_speed_block->get_output();
}

double GENCLS::get_rotor_speed_in_pu()
{
    return 1.0+get_rotor_speed_deviation_in_pu();
}

complex<double> GENCLS::get_internal_voltage_in_pu_in_dq_axis()
{
    double Efd = get_excitation_voltage_in_pu();

    return complex<double>(0.0, Efd);
}

complex<double> GENCLS::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Edq = get_internal_voltage_in_pu_in_dq_axis();

    double angle = get_rotor_angle_in_rad();

    return dq2xy_with_angle_in_rad(Edq, angle);
}



void GENCLS::set_rotor_angle_in_deg(double angle)
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    rotor_angle_block->set_output(deg2rad(angle));
    rotor_angle_block->initialize(); // the initialize function is used to update STORE
}

void GENCLS::set_rotor_speed_deviation_in_pu(double speed)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();

    rotor_speed_block->set_output(speed);
    rotor_speed_block->initialize();
}



string GENCLS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string GENCLS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string GENCLS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
