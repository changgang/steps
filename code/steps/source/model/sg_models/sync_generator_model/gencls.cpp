#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;
GENCLS::GENCLS(STEPS& toolkit) : SYNC_GENERATOR_MODEL(toolkit)
{
    clear();
}

GENCLS::~GENCLS()
{
}

void GENCLS::clear()
{
    set_model_float_parameter_count(2);
}
void GENCLS::copy_from_const_model(const GENCLS& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    set_blocks_toolkit(toolkit);

    clear();

    this->set_Tj_in_s(model.get_Tj_in_s());
    this->set_D(model.get_D());
    this->set_Rs(model.get_Rs());
    this->set_Xdp(model.get_Xdp());

}
GENCLS::GENCLS(const GENCLS& model):SYNC_GENERATOR_MODEL(model.get_toolkit())
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

void GENCLS::update_source_impedance()
{
    GENERATOR* generator = get_generator_pointer();
    complex<double> Zs = generator->get_source_impedance_in_pu();
    double rs = Zs.real();
    double xs = Zs.imag();
    set_Rs(rs);
    set_Xdp(xs);
}


bool GENCLS::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=3+get_model_float_parameter_count())
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double H, D;

            unsigned int i=3;
            H = get_double_data(data[i],"0.0"); i++;
            D = get_double_data(data[i],"0.0");

            set_H_in_s(H);
            set_D(D);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool GENCLS::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool GENCLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void GENCLS::initialize()
{
    ostringstream osstream;

    setup_block_toolkit_and_parameters();

    update_source_impedance();

    // rotor angle and speed
    initialize_rotor_angle();

    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();

    double speed = 0.0;
    rotor_speed_block->set_output(speed);
    rotor_speed_block->initialize();

    GENERATOR* generator = get_generator_pointer();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double rs = get_Rs();
    double xdp = get_Xdp();
    complex<double> Zsource(rs, xdp);

    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    complex<double> S(P*one_over_mbase,Q*one_over_mbase);

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
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    complex<double> S(P*one_over_mbase,Q*one_over_mbase);

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
    //delta_torque = (tmech-telec)*omega-damping*speed;
    rotor_speed_block->set_input(delta_torque);
    rotor_speed_block->run(mode);

    speed = get_rotor_speed_deviation_in_pu();

    speed = DOUBLE_PI*fbase*speed;
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
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return (Exy/Z)*(mbase*one_over_sbase);
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

    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return Ixy*(mbase*one_over_sbase);
}

double GENCLS::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double GENCLS::get_terminal_current_in_pu_based_on_sbase()
{
    double I = get_terminal_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return I*(mbase*one_over_sbase);
}


void GENCLS::check()
{
    ;
}

void GENCLS::report()
{
    ostringstream osstream;
    osstream<< get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void GENCLS::save()
{
    ;
}

string GENCLS::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_H_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<" /";
    return osstream.str();
}

void GENCLS::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("H", i); i++;
    add_model_data_name_and_index_pair("D", i);
}

double GENCLS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "H")  return get_H_in_s();
        if(par_name == "D")  return get_D();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void GENCLS::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "H") return set_H_in_s(value);
        if(par_name == "D") return set_D(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double GENCLS::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_H_in_s()!=0.0 and mint>get_H_in_s())
        mint = get_H_in_s();
    return mint;
}

void GENCLS::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@ROTOR ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ROTOR SPEED BLOCK", i); i++;
}

double GENCLS::get_model_internal_variable_with_name(string var_name)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    var_name = string2upper(var_name);
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

void GENCLS::linearize()
{
    STEPS_SPARSE_MATRIX* gen_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN", gen_matrix);
    // do linearization
    STEPS_SPARSE_MATRIX* gen_avr_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN-AVR", gen_avr_matrix);
    // do linearization
    STEPS_SPARSE_MATRIX* gen_gov_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN-GOV", gen_gov_matrix);
    // do linearization
}

void GENCLS::build_linearized_matrix_ABCD()
{
    return;
}
