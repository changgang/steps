#include "header/model/vsg_model/vsgcontrol0.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
VSGCONTROL0::VSGCONTROL0(STEPS& toolkit) : VSG_MODEL(toolkit),
                             virtual_speed_deviation_block(toolkit),
                             virtual_angle_block(toolkit),
                             virtual_voltage_block(toolkit)
{
    clear();
}

VSGCONTROL0::VSGCONTROL0(const VSGCONTROL0& model) : VSG_MODEL(model.get_toolkit()),
                                virtual_speed_deviation_block(model.get_toolkit()),
                                virtual_angle_block(model.get_toolkit()),
                                virtual_voltage_block(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSGCONTROL0& VSGCONTROL0::operator=(const VSGCONTROL0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

VSGCONTROL0::~VSGCONTROL0()
{
    ;
}

void VSGCONTROL0::clear()
{
    set_Tj_in_s(INFINITE_THRESHOLD);
    set_D(0.0);
    set_Ku(0.0);
    set_Te_in_s(INFINITE_THRESHOLD);
}

void VSGCONTROL0::copy_from_const_model(const VSGCONTROL0& model)
{
    clear();
    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());
}

void VSGCONTROL0::set_Tj_in_s(double Tj)
{
    if(fabs(Tj)>FLOAT_EPSILON)
        virtual_speed_deviation_block.set_T_in_s(Tj);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Tj ("<<Tj<<") is not allowed since it is too small when setting Tj in "
                <<get_model_name()<<" model for "<<get_device_id().get_compound_device_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void VSGCONTROL0::set_D(double D)
{
    this->D = D;
}

void VSGCONTROL0::set_Ku(double Ku)
{
    this->Ku = Ku;
}

void VSGCONTROL0::set_Te_in_s(double Ke)
{
    virtual_voltage_block.set_T_in_s(Ke);
}

double VSGCONTROL0::get_Tj_in_s() const
{
    return virtual_speed_deviation_block.get_T_in_s();
}

double VSGCONTROL0::get_D() const
{
    return D;
}

double VSGCONTROL0::get_Ku() const
{
    return Ku;
}

double VSGCONTROL0::get_Te_in_s() const
{
    return virtual_voltage_block.get_T_in_s();
}

string VSGCONTROL0::get_model_name() const
{
    return "VSGCONTROL0";
}

bool VSGCONTROL0::setup_model_with_steps_string_vector(vector<string>& data)
{
    /* 'VSGCONTROL0', ibus, ickt, Tj, D, Ku, Ke */
    bool is_successful = false;
    if(data.size()>=7)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double Tj, D, Ku, Te;

            unsigned int i=3;
            Tj = get_double_data(data[i],"0.0"); i++;
            D  = get_double_data(data[i],"0.0"); i++;
            Ku = get_double_data(data[i],"0.0"); i++;
            Te = get_double_data(data[i],"0.0");

            set_Tj_in_s(Tj);
            set_D(D);
            set_Ku(Ku);
            set_Te_in_s(Te);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;

}

bool VSGCONTROL0::setup_model_with_psse_string(string data)
{
    /* ibus, 'VSGCONTROL0', ickt, Tj, D, Ku, Ke */
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSGCONTROL0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSGCONTROL0::setup_block_toolkit_and_parameters()
{
    virtual_angle_block.set_T_in_s(1.0/get_bus_base_angle_speed_in_radps());
}

void VSGCONTROL0::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        SOURCE* source = get_source_pointer();
        double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

        complex<double> Zsource(source->get_source_impedance_in_pu());

        double P = source->get_p_generation_in_MW();
        double Q = source->get_q_generation_in_MVar();
        complex<double> S(P*one_over_mbase,Q*one_over_mbase);

        complex<double> Vxy = get_terminal_complex_voltage_in_pu();
        complex<double> Ixy = conj(S/Vxy);
        complex<double> Exy = Vxy+Ixy*Zsource;

        double angle = steps_fast_complex_arg(Exy);

        virtual_angle_block.set_output(angle);
        virtual_angle_block.initialize();

        virtual_speed_deviation_block.set_output(0.0);
        virtual_speed_deviation_block.initialize();

        double E = steps_fast_complex_abs(Exy);
        virtual_voltage_block.set_output(E);
        virtual_voltage_block.initialize();

        set_Pref_in_pu_based_on_mbase(P);
        set_Qref_in_pu_based_on_mbase(Q);
        set_Vref_in_pu(steps_fast_complex_abs(Vxy));

        set_flag_model_initialized_as_true();
    }
}

void VSGCONTROL0::run(DYNAMIC_MODE mode)
{
    complex<double> S = get_terminal_complex_power_in_pu_based_on_mbase();
    double P = S.real();
    double Q = S.imag();
    double V = get_terminal_voltage_in_pu();

    double input;

    input = get_Pref_in_pu_based_on_mbase() - P;
    input /= get_virtual_speed_in_pu();
    input -= (get_D()*get_virtual_speed_deviation_in_pu());

    virtual_speed_deviation_block.set_input(input);
    virtual_speed_deviation_block.run(mode);

    input = virtual_speed_deviation_block.get_output();

    virtual_angle_block.set_input(input);
    virtual_angle_block.run(mode);

    input = get_Qref_in_pu_based_on_mbase() - Q;
    input += get_Ku()*(get_Vref_in_pu() - V);

    virtual_voltage_block.set_input(input);
    virtual_voltage_block.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void VSGCONTROL0::check()
{

}

void VSGCONTROL0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSGCONTROL0::save()
{
    ;
}

string VSGCONTROL0::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_Tj_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Ku()<<", "
            <<setw(8)<<setprecision(6)<<get_Te_in_s()<<" /";

    return osstream.str();
}

void VSGCONTROL0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("Tj", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
    add_model_data_name_and_index_pair("KU", i); i++;
    add_model_data_name_and_index_pair("TE", i);
}

double VSGCONTROL0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="TJ") return get_Tj_in_s();
        if(par_name=="D")  return get_D();
        if(par_name=="KU") return get_Ku();
        if(par_name=="TE") return get_Te_in_s();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void VSGCONTROL0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="TJ") return set_Tj_in_s(value);
        if(par_name=="D")  return set_D(value);
        if(par_name=="KU") return set_Ku(value);
        if(par_name=="KE") return set_Te_in_s(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double VSGCONTROL0::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tj_in_s()!=0.0 and mint>get_Tj_in_s())
        mint = get_Tj_in_s();
    if(get_Te_in_s()!=0.0 and mint>get_Te_in_s())
        mint = get_Te_in_s();
    return mint;
}

void VSGCONTROL0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL SPEED DEVIATION BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL VOLTAGE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("PREF", i); i++;
    add_model_internal_variable_name_and_index_pair("QREF", i); i++;
    add_model_internal_variable_name_and_index_pair("VREF", i); i++;
}

double VSGCONTROL0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(is_model_internal_variable_exist(var_name))
    {
        if(var_name == "STATE@VIRTUAL SPEED DEVIATION BLOCK") return virtual_speed_deviation_block.get_state();
        if(var_name == "STATE@VIRTUAL ANGLE BLOCK") return virtual_angle_block.get_state();
        if(var_name == "STATE@VIRTUAL VOLTAGE BLOCK") return virtual_voltage_block.get_state();
        if(var_name == "PREF") return get_Pref_in_pu_based_on_mbase();
        if(var_name == "QREF") return get_Qref_in_pu_based_on_mbase();
        if(var_name == "VREF") return get_Vref_in_pu();
    }
    return 0.0;
}

string VSGCONTROL0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSGCONTROL0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string VSGCONTROL0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}


double VSGCONTROL0::get_virtual_speed_deviation_in_pu() const
{
    return virtual_speed_deviation_block.get_output();
}

double VSGCONTROL0::get_virtual_angle_in_rad() const
{
    return virtual_angle_block.get_output();
}

double VSGCONTROL0::get_virtual_voltage_in_pu() const
{
    return virtual_voltage_block.get_output();
}

