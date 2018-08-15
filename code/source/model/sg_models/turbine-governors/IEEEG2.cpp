#include "header/model/sg_models/turbine_governor_model/IEEEG2.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
using namespace std;
static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@DROOP",     //4
                                            "STATE@TUNER",       //5
                                            "STATE@WATER HAMMER"       //6
                                            };//8

IEEEG2::IEEEG2()
{
    clear();
}

IEEEG2::~IEEEG2()
{
    clear();
}
void IEEEG2::clear()
{
    droop.set_limiter_type(NO_LIMITER);
    tuner.set_limiter_type(NO_LIMITER);
    tuner.set_K(1.0);
    water_hammer.set_limiter_type(NO_LIMITER);
    water_hammer.set_K(1.0);

}
void IEEEG2::copy_from_const_model(const IEEEG2& model)
{
    clear();

    this->set_K(model.get_K());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Pmin_in_pu(model.get_Pmin_in_pu());
    this->set_T4_in_s(model.get_T4_in_s());
}

IEEEG2::IEEEG2(const IEEEG2&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

IEEEG2& IEEEG2::operator=(const IEEEG2& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEG2::get_model_name() const
{
    return "IEEEG2";
}

double IEEEG2::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double IEEEG2::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();

    return 0.0;
}

void IEEEG2::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void IEEEG2::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
}

void IEEEG2::set_K(double K)
{
    droop.set_K(K);
}

void IEEEG2::set_T1_in_s(double T)
{
    droop.set_T_in_s(T);
}

void IEEEG2::set_T2_in_s(double T)
{
    tuner.set_T1_in_s(T);
}

void IEEEG2::set_T3_in_s(double T)
{
    tuner.set_T2_in_s(T);
}

void IEEEG2::set_Pmax_in_pu(double P)
{
    this->Pmax = P;
}

void IEEEG2::set_Pmin_in_pu(double P)
{
    this->Pmin = P;
}

void IEEEG2::set_T4_in_s(double T)
{
    water_hammer.set_T1_in_s(-T);
    water_hammer.set_T2_in_s(0.5*T);
}

double IEEEG2::get_K() const
{
    return droop.get_K();
}

double IEEEG2::get_T1_in_s() const
{
    return droop.get_T_in_s();
}

double IEEEG2::get_T2_in_s() const
{
    return tuner.get_T1_in_s();
}

double IEEEG2::get_T3_in_s() const
{
    return tuner.get_T2_in_s();
}

double IEEEG2::get_Pmax_in_pu() const
{
    return Pmax;
}

double IEEEG2::get_Pmin_in_pu() const
{
    return Pmin;
}

double IEEEG2::get_T4_in_s() const
{
    return -water_hammer.get_T1_in_s();
}

bool IEEEG2::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool IEEEG2::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<10)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double k, t1, t2, t3, pmax, pmin, t4;

    size_t i=3;
    k = get_double_data(dyrdata[i],"0.0"); i++;
    t1 = get_double_data(dyrdata[i],"0.0"); i++;
    t2 = get_double_data(dyrdata[i],"0.0"); i++;
    t3 = get_double_data(dyrdata[i],"0.0"); i++;
    pmax = get_double_data(dyrdata[i],"0.0"); i++;
    pmin = get_double_data(dyrdata[i],"0.0"); i++;
    t4 = get_double_data(dyrdata[i],"0.0"); i++;

    set_K(k);
    set_T1_in_s(t1);
    set_T2_in_s(t2);
    set_T3_in_s(t3);
    set_Pmax_in_pu(pmax);
    set_Pmin_in_pu(pmin);
    set_T4_in_s(t4);

    is_successful = true;

    return is_successful;
}

bool IEEEG2::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEEEG2::initialize()
{
    ostringstream osstream;

    if(is_model_initialized())
        return;

    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return;

    if(not gen_model->is_model_initialized())
        gen_model->initialize();

    double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

    droop.set_output(0.0);
    droop.initialize();

    tuner.set_output(0.0);
    tuner.initialize();

    if(pmech0>get_Pmax_in_pu())
    {
        osstream<<"Initialization error. Pmech of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Pmech is "<<pmech0<<", and Pmax is "<<get_Pmax_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    if(pmech0<get_Pmin_in_pu())
    {
        osstream<<"Initialization error. Pmech of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Pmech is "<<pmech0<<", and Pmin is "<<get_Pmin_in_pu()<<".";
        show_information_with_leading_time_stamp(osstream);
    }

    water_hammer.set_output(pmech0);
    water_hammer.initialize();

    set_mechanical_power_reference_in_pu_based_on_mbase(pmech0);

    set_flag_model_initialized_as_true();
}

void IEEEG2::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return;

    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    droop.set_input(speed);
    droop.run(mode);

    double input = droop.get_output();

    tuner.set_input(input);
    tuner.run(mode);

    input = Pref - tuner.get_output();

    double Pmax = get_Pmax_in_pu();
    double Pmin = get_Pmin_in_pu();
    if(input>Pmax)
        input = Pmax;
    if(input<Pmin)
        input = Pmin;

    water_hammer.set_input(input);
    water_hammer.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEG2::get_mechanical_power_in_pu_based_on_mbase() const
{
    double Pmech = water_hammer.get_output();

    return Pmech;
}

double IEEEG2::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEEEG2::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEEEG2::check()
{
    ;
}

void IEEEG2::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}
void IEEEG2::save()
{
    ;
}
string IEEEG2::get_standard_model_string() const
{
    ostringstream osstream;

    double K = get_K();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double Pmax = get_Pmax_in_pu();
    double Pmin = get_Pmin_in_pu();
    double T4 = get_T4_in_s();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<K<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<Pmax<<", "
      <<setw(8)<<setprecision(6)<<Pmin<<", "
      <<setw(8)<<setprecision(6)<<T4<<"  /";

    return osstream.str();
}

size_t IEEEG2::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string IEEEG2::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double IEEEG2::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double IEEEG2::get_variable_with_name(string var_name)
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

    if(var_name == "STATE@DROOP")
        return droop.get_state();

    if(var_name == "STATE@TUNER")
        return tuner.get_state();

    if(var_name == "STATE@WATER HAMMER")
        return water_hammer.get_state();

    return 0.0;
}

string IEEEG2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEG2::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEG2::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
