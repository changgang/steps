#include "header/model/sg_models/turbine_governor_model/IEESGO.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
IEESGO::IEESGO(STEPS& toolkit) : TURBINE_GOVERNOR_MODEL(toolkit),
                                 governor_tuner(toolkit),
                                 governor(toolkit),
                                 high_pressure_turbine(toolkit),
                                 medium_pressure_turbine(toolkit),
                                 low_pressure_turbine(toolkit)
{
    clear();
}

IEESGO::~IEESGO()
{
}

void IEESGO::clear()
{
    set_model_float_parameter_count(11);

    governor_tuner.set_limiter_type(NO_LIMITER);
    governor.set_limiter_type(NO_LIMITER);
    high_pressure_turbine.set_limiter_type(NO_LIMITER);
    medium_pressure_turbine.set_limiter_type(NO_LIMITER);
    low_pressure_turbine.set_limiter_type(NO_LIMITER);
    set_Pmax_in_pu(1.0);
    set_Pmin_in_pu(0.0);
}

void IEESGO::copy_from_const_model(const IEESGO& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    governor_tuner.set_toolkit(toolkit);
    governor.set_toolkit(toolkit);
    high_pressure_turbine.set_toolkit(toolkit);
    medium_pressure_turbine.set_toolkit(toolkit);
    low_pressure_turbine.set_toolkit(toolkit);

    clear();

    this->set_K1(model.get_K1());
    this->set_K2(model.get_K2());
    this->set_K3(model.get_K3());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Pmin_in_pu(model.get_Pmin_in_pu());
}

IEESGO::IEESGO(const IEESGO&model) : TURBINE_GOVERNOR_MODEL(model.get_toolkit()),
                                     governor_tuner(model.get_toolkit()),
                                     governor(model.get_toolkit()),
                                     high_pressure_turbine(model.get_toolkit()),
                                     medium_pressure_turbine(model.get_toolkit()),
                                     low_pressure_turbine(model.get_toolkit())
{
    copy_from_const_model(model);
}

IEESGO& IEESGO::operator=(const IEESGO& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEESGO::get_model_name() const
{
    return "IEESGO";
}

void IEESGO::set_K1(double K)
{
    governor.set_K(K);
}

void IEESGO::set_K2(double K)
{
    medium_pressure_turbine.set_K(K);
}

void IEESGO::set_K3(double K)
{
    low_pressure_turbine.set_K(K);
}

void IEESGO::set_T1_in_s(double T)
{
    governor_tuner.set_T2_in_s(T);
}

void IEESGO::set_T2_in_s(double T)
{
    governor_tuner.set_T1_in_s(T);
}

void IEESGO::set_T3_in_s(double T)
{
    governor.set_T_in_s(T);
}

void IEESGO::set_T4_in_s(double T)
{
    high_pressure_turbine.set_T_in_s(T);
}

void IEESGO::set_T5_in_s(double T)
{
    medium_pressure_turbine.set_T_in_s(T);
}

void IEESGO::set_T6_in_s(double T)
{
    low_pressure_turbine.set_T_in_s(T);
}

void IEESGO::set_Pmax_in_pu(double P)
{
    pmax = P;
}

void IEESGO::set_Pmin_in_pu(double P)
{
    pmin = P;
}


double IEESGO::get_K1() const
{
    return governor.get_K();
}

double IEESGO::get_K2() const
{
    return medium_pressure_turbine.get_K();
}

double IEESGO::get_K3() const
{
    return low_pressure_turbine.get_K();
}

double IEESGO::get_T1_in_s() const
{
    return governor_tuner.get_T2_in_s();
}

double IEESGO::get_T2_in_s() const
{
    return governor_tuner.get_T1_in_s();
}

double IEESGO::get_T3_in_s() const
{
    return governor.get_T_in_s();
}

double IEESGO::get_T4_in_s() const
{
    return high_pressure_turbine.get_T_in_s();
}

double IEESGO::get_T5_in_s() const
{
    return medium_pressure_turbine.get_T_in_s();
}

double IEESGO::get_T6_in_s() const
{
    return low_pressure_turbine.get_T_in_s();
}

double IEESGO::get_Pmax_in_pu() const
{
    return pmax;
}

double IEESGO::get_Pmin_in_pu() const
{
    return pmin;
}

bool IEESGO::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=14)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double t1, t2, t3, t4, t5, t6, k1, k2, k3, pmax, pmin;
            unsigned int i=3;
            t1 = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            t4 = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            t6 = get_double_data(data[i],"0.0"); i++;
            k1 = get_double_data(data[i],"0.0"); i++;
            k2 = get_double_data(data[i],"0.0"); i++;
            k3 = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;

            set_K1(k1);
            set_K2(k2);
            set_K3(k3);
            set_T1_in_s(t1);
            set_T2_in_s(t2);
            set_T3_in_s(t3);
            set_T4_in_s(t4);
            set_T5_in_s(t5);
            set_T6_in_s(t6);
            set_Pmax_in_pu(pmax);
            set_Pmin_in_pu(pmin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEESGO::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEESGO::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEESGO::setup_block_toolkit_and_parameters()
{
}

void IEESGO::initialize()
{
    setup_block_toolkit_and_parameters();

    ostringstream osstream;

    STEPS& toolkit = get_toolkit();

    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

    high_pressure_turbine.set_output(pmech0);
    high_pressure_turbine.initialize();

    medium_pressure_turbine.set_output(pmech0*get_K2());
    medium_pressure_turbine.initialize();

    low_pressure_turbine.set_output(pmech0*get_K2()*get_K3());
    low_pressure_turbine.initialize();

    governor.set_output(0.0);
    governor.initialize();

    governor_tuner.set_output(0.0);
    governor_tuner.initialize();

    set_initial_mechanical_power_reference_in_pu_based_on_mbase(pmech0);

    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    if(pmech0>pmax)
    {
        osstream<<"Initialization Error. Initial mechanical power exceeds upper limit when initializing "
               <<get_model_type()<<" model "<<get_model_name()<<" of "<<get_compound_device_name()<<endl
               <<"Initial mechanical power is "<<pmech0<<" p.u., upper limit is "<<pmax;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(pmech0<pmin)
    {
        osstream<<"Initialization Error. Initial mechanical power exceeds lower limit when initializing "
               <<get_model_type()<<" model "<<get_model_name()<<" of "<<get_compound_device_name()<<endl
               <<"Initial mechanical power is "<<pmech0<<" p.u., lower limit is "<<pmin;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    set_flag_model_initialized_as_true();
}

void IEESGO::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = speed;
    governor_tuner.set_input(input);
    governor_tuner.run(mode);

    input = governor_tuner.get_output();
    governor.set_input(input);
    governor.run(mode);

    input = Pref - governor.get_output();
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    if(input>pmax)
        input = pmax;
    if(input<pmin)
        input = pmin;

    high_pressure_turbine.set_input(input);
    high_pressure_turbine.run(mode);

    input = high_pressure_turbine.get_output();
    medium_pressure_turbine.set_input(input);
    medium_pressure_turbine.run(mode);

    input = medium_pressure_turbine.get_output();
    low_pressure_turbine.set_input(input);
    low_pressure_turbine.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}
double IEESGO::get_mechanical_power_in_pu_based_on_mbase() const
{
    double output_high = high_pressure_turbine.get_output();
    double output_medium = medium_pressure_turbine.get_output();
    double output_low = low_pressure_turbine.get_output();

    double k2 = get_K2();
    double k3 = get_K3();

    return (1.0-k2)*output_high+(1.0-k3)*output_medium+output_low;
}

double IEESGO::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEESGO::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEESGO::check()
{
    ;
}

void IEESGO::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEESGO::save()
{
    ;
}
string IEESGO::get_standard_psse_string() const
{
    ostringstream osstream;

    double k1 = get_K1();
    double k2 = get_K2();
    double k3 = get_K3();
    double t1 = get_T1_in_s();
    double t2 = get_T2_in_s();
    double t3 = get_T3_in_s();
    double t4 = get_T4_in_s();
    double t5 = get_T5_in_s();
    double t6 = get_T6_in_s();
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<t1<<", "
            <<setw(8)<<setprecision(6)<<t2<<", "
            <<setw(8)<<setprecision(6)<<t3<<", "
            <<setw(8)<<setprecision(6)<<t4<<", "
            <<setw(8)<<setprecision(6)<<t5<<", "
            <<setw(8)<<setprecision(6)<<t6<<", "
            <<setw(8)<<setprecision(6)<<k1<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<k2<<", "
            <<setw(8)<<setprecision(6)<<k3<<", "
            <<setw(8)<<setprecision(6)<<pmax<<", "
            <<setw(8)<<setprecision(6)<<pmin<<" /";
    return osstream.str();
}

void IEESGO::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("T4", i); i++;
    add_model_data_name_and_index_pair("T5", i); i++;
    add_model_data_name_and_index_pair("T6", i); i++;
    add_model_data_name_and_index_pair("K1", i); i++;
    add_model_data_name_and_index_pair("K2", i); i++;
    add_model_data_name_and_index_pair("K3", i); i++;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("PMIN", i); i++;
}

double IEESGO::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="T1")
        return get_T1_in_s();
    if(par_name=="T2")
        return get_T2_in_s();
    if(par_name=="T3")
        return get_T3_in_s();
    if(par_name=="T4")
        return get_T4_in_s();
    if(par_name=="T5")
        return get_T5_in_s();
    if(par_name=="T6")
        return get_T6_in_s();
    if(par_name=="K1")
        return get_K1();
    if(par_name=="K2")
        return get_K2();
    if(par_name=="K3")
        return get_K3();
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();

    return 0.0;
}

void IEESGO::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="T1")
        return set_T1_in_s(value);
    if(par_name=="T2")
        return set_T2_in_s(value);
    if(par_name=="T3")
        return set_T3_in_s(value);
    if(par_name=="T4")
        return set_T4_in_s(value);
    if(par_name=="T5")
        return set_T5_in_s(value);
    if(par_name=="T6")
        return set_T6_in_s(value);
    if(par_name=="K1")
        return set_K1(value);
    if(par_name=="K2")
        return set_K2(value);
    if(par_name=="K3")
        return set_K3(value);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
}

double IEESGO::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void IEESGO::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@GOVERNOR TUNER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@GOVERNOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@HIGH TURBINE", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@MEDIUM TURBINE", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@LOW TURBINE", i); i++;
}

double IEESGO::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@GOVERNOR TUNER")
        return governor_tuner.get_state();

    if(var_name == "STATE@GOVERNOR")
        return governor.get_state();

    if(var_name == "STATE@HIGH TURBINE")
        return high_pressure_turbine.get_state();

    if(var_name == "STATE@MEDIUM TURBINE")
        return medium_pressure_turbine.get_state();

    if(var_name == "STATE@LOW TURBINE")
        return low_pressure_turbine.get_state();

    return 0.0;
}

string IEESGO::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEESGO::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEESGO::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
