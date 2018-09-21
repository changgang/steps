#include "header/model/sg_models/turbine_governor_model/TGOV1.h"
#include "header/basic/utility.h"
TGOV1::TGOV1()
{
    clear();
}

TGOV1::~TGOV1()
{
    clear();
}

void TGOV1::clear()
{
    prepare_model_variable_table();
    governor.set_limiter_type(WINDUP_LIMITER);
    turbine.set_K(1.0);
}

void TGOV1::copy_from_const_model(const TGOV1& model)
{
    clear();

    //this->set_power_system_database(model.get_power_system_database());
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

double TGOV1::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double TGOV1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="VMAX")
        return get_Valvemax_in_pu();
    if(par_name=="VMIN")
        return get_Valvemin_in_pu();

    return 0.0;
}

void TGOV1::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void TGOV1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="VMAX")
        return set_Valvemax_in_pu(value);
    if(par_name=="VMIN")
        return set_Valvemin_in_pu(value);
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


bool TGOV1::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool TGOV1::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<9)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double r, t1, vmax, vmin, t2, t3, d;

    size_t i=3;
    r = get_double_data(dyrdata[i],"0.0"); i++;
    t1 = get_double_data(dyrdata[i],"0.0"); i++;
    vmax = get_double_data(dyrdata[i],"0.0"); i++;
    vmin = get_double_data(dyrdata[i],"0.0"); i++;
    t2 = get_double_data(dyrdata[i],"0.0"); i++;
    t3 = get_double_data(dyrdata[i],"0.0"); i++;
    d = get_double_data(dyrdata[i],"0.0"); i++;

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

bool TGOV1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}


void TGOV1::initialize()
{
    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();
    turbine.set_output(pmech0);
    turbine.initialize();

    governor.set_output(pmech0);
    governor.initialize();

    set_mechanical_power_reference_in_pu_based_on_mbase(pmech0*get_R());

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
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}
void TGOV1::save()
{
    ;
}
string TGOV1::get_standard_model_string() const
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
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<R<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<Vmax<<", "
      <<setw(8)<<setprecision(6)<<Vmin<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<D<<"  /";
    return osstream.str();
}

void TGOV1::prepare_model_variable_table()
{
    size_t i=0;
    add_model_variable_name_and_index_pair("GENERATOR MECHANICAL POWER IN PU", i); i++;
    add_model_variable_name_and_index_pair("GENERATOR MECHANICAL POWER IN MW", i); i++;
    add_model_variable_name_and_index_pair("MECHANICAL POWER REFERENCE IN PU", i); i++;
    add_model_variable_name_and_index_pair("GENERATOR ROTOR SPEED DEVIATION IN PU", i); i++;
    add_model_variable_name_and_index_pair("STATE@GOVERNOR", i); i++;
    add_model_variable_name_and_index_pair("STATE@TURBINE", i); i++;
}

double TGOV1::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "GENERATOR MECHANICAL POWER IN PU")
        return get_mechanical_power_in_pu_based_on_mbase();

    if(var_name == "GENERATOR MECHANICAL POWER IN MW")
    {
        GENERATOR* generator = get_generator_pointer();
        return get_mechanical_power_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
    }

    if(var_name == "MECHANICAL POWER REFERENCE IN PU")
        return get_mechanical_power_reference_in_pu_based_on_mbase();

    if(var_name == "GENERATOR ROTOR SPEED DEVIATION IN PU")
        return get_rotor_speed_deviation_in_pu_from_sync_generator_model();

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
