#include "header/model/sg_models/turbine_governor_model/TGOV1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
TGOV1::TGOV1()
{
    clear();
}

TGOV1::~TGOV1()
{
}

void TGOV1::clear()
{
    set_model_float_parameter_count(7);
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    governor.set_limiter_type(WINDUP_LIMITER);
    turbine.set_K(1.0);
}

void TGOV1::copy_from_const_model(const TGOV1& model)
{
    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->governor.set_limiter_type(WINDUP_LIMITER);
    this->set_R(model.get_R());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_Valvemax_in_pu(model.get_Valvemax_in_pu());
    this->set_Valvemin_in_pu(model.get_Valvemin_in_pu());
    this->set_D(model.get_D());
}

TGOV1::TGOV1(const TGOV1&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

TGOV1& TGOV1::operator=(const TGOV1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string TGOV1::get_model_name() const
{
    return "TGOV1";
}

void TGOV1::set_R(double R)
{
    governor.set_K(1.0/R);
}
void TGOV1::set_T1_in_s(double T)
{
    governor.set_T_in_s(T);
}

void TGOV1::set_T2_in_s(double T)
{
    turbine.set_T1_in_s(T);
}

void TGOV1::set_T3_in_s(double T)
{
    turbine.set_T2_in_s(T);
}

void TGOV1::set_Valvemax_in_pu(double V)
{
    governor.set_upper_limit(V);
}

void TGOV1::set_Valvemin_in_pu(double V)
{
    governor.set_lower_limit(V);
}

void TGOV1::set_D(double D)
{
    damping.set_K(D);
}

double TGOV1::get_R() const
{
    return 1.0/governor.get_K();
}

double TGOV1::get_T1_in_s() const
{
    return governor.get_T_in_s();
}
double TGOV1::get_T2_in_s() const
{
    return turbine.get_T1_in_s();
}

double TGOV1::get_T3_in_s() const
{
    return turbine.get_T2_in_s();
}

double TGOV1::get_Valvemax_in_pu() const
{
    return governor.get_upper_limit();
}

double TGOV1::get_Valvemin_in_pu() const
{
    return governor.get_lower_limit();
}

double TGOV1::get_D() const
{
    return damping.get_K();
}

bool TGOV1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=10)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double r, t1, vmax, vmin, t2, t3, d;

            size_t i=3;
            r = get_double_data(data[i],"0.0"); i++;
            t1 = get_double_data(data[i],"0.0"); i++;
            vmax = get_double_data(data[i],"0.0"); i++;
            vmin = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            d = get_double_data(data[i],"0.0"); i++;

            set_R(r);
            set_T1_in_s(t1);
            set_Valvemax_in_pu(vmax);
            set_Valvemin_in_pu(vmin);
            set_T2_in_s(t2);
            set_T3_in_s(t3);
            set_D(d);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool TGOV1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool TGOV1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void TGOV1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    governor.set_toolkit(toolkit);
    turbine.set_toolkit(toolkit);
    damping.set_toolkit(toolkit);
}

void TGOV1::initialize()
{
    setup_block_toolkit_and_parameters();

    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();
    turbine.set_output(pmech0);
    turbine.initialize();

    governor.set_output(pmech0);
    governor.initialize();

    set_initial_mechanical_power_reference_in_pu_based_on_mbase(pmech0*get_R());

    set_flag_model_initialized_as_true();
}

void TGOV1::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed;
    governor.set_input(input);
    governor.run(mode);

    input = governor.get_output();
    turbine.set_input(input);
    turbine.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}
double TGOV1::get_mechanical_power_in_pu_based_on_mbase() const
{
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();
    double D = get_D();

    return turbine.get_output()-D*speed;
}

double TGOV1::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Valvemax_in_pu();
}

double TGOV1::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Valvemin_in_pu();
}

void TGOV1::check()
{
    ;
}

void TGOV1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void TGOV1::save()
{
    ;
}
string TGOV1::get_standard_psse_string() const
{
    ostringstream osstream;

    double R = get_R();
    double T1 = get_T1_in_s();
    double Vmax = get_Valvemax_in_pu();
    double Vmin = get_Valvemin_in_pu();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double D = get_D();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<R<<", "
            <<setw(8)<<setprecision(6)<<T1<<", "
            <<setw(8)<<setprecision(6)<<Vmax<<", "
            <<setw(8)<<setprecision(6)<<Vmin<<", "
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<D<<" /";
    return osstream.str();
}

void TGOV1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("R", i); i++;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("VMAX", i); i++;
    add_model_data_name_and_index_pair("VMIN", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
}


double TGOV1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="R")
        return get_R();
    if(par_name=="T1")
        return get_T1_in_s();
    if(par_name=="VMAX")
        return get_Valvemax_in_pu();
    if(par_name=="VMIN")
        return get_Valvemin_in_pu();
    if(par_name=="T2")
        return get_T2_in_s();
    if(par_name=="T3")
        return get_T3_in_s();
    if(par_name=="D")
        return get_D();


    return 0.0;
}

void TGOV1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="R")
        return set_R(value);
    if(par_name=="T1")
        return set_T1_in_s(value);
    if(par_name=="VMAX")
        return set_Valvemax_in_pu(value);
    if(par_name=="VMIN")
        return set_Valvemin_in_pu(value);
    if(par_name=="T2")
        return set_T2_in_s(value);
    if(par_name=="T3")
        return set_T3_in_s(value);
    if(par_name=="D")
        return set_D(value);
}

void TGOV1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@GOVERNOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TURBINE", i); i++;
}

double TGOV1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@GOVERNOR")
        return governor.get_state();

    if(var_name == "STATE@TURBINE")
        return turbine.get_state();

    return 0.0;
}

string TGOV1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string TGOV1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string TGOV1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
