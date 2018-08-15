#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR TERMINAL VOLTAGE",      //0
                                            "GENERATOR TERMINAL CURRENT",      //1
                                            "COMPENSATED VOLTAGE"      //2
                                            };

COMP::COMP()
{
    clear();
}

COMP::~COMP()
{
    clear();
}

void COMP::clear()
{
    ;
}

void COMP::copy_from_const_model(const COMP& model)
{
    clear();
    //this->set_power_system_database(model.get_power_system_database());
    //this->set_device_id(model.get_device_id());
    this->set_Xe(model.get_Xe());
}

COMP::COMP(const COMP& model) : COMPENSATOR_MODEL()
{
    copy_from_const_model(model);
}

COMP& COMP::operator=(const COMP& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string COMP::get_model_name() const
{
    return "COMP";
}

double COMP::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double COMP::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void COMP::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void COMP::set_double_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void COMP::set_Xe(double xe)
{
    this->Xe = xe;
}

double COMP::get_Xe() const
{
    return Xe;
}

bool COMP::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool COMP::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<4)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double xe;

    size_t i=3;
    xe = get_double_data(dyrdata[i],"0.0");

    set_Xe(xe);

    is_successful = true;

    return is_successful;
}

bool COMP::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}


void COMP::initialize()
{
    if(is_model_initialized())
        return;

    set_flag_model_initialized_as_false();
}

void COMP::run(DYNAMIC_MODE mode)
{
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}
double COMP::get_compensated_voltage_in_pu() const
{
    complex<double> Vt = get_generator_terminal_voltage_in_pu();
    complex<double> It = get_generator_terminal_current_in_pu();
    double xe = get_Xe();
    complex<double> j(0.0,1.0);
    complex<double> Vct = Vt-j*xe*It;
    return steps_fast_complex_abs(Vct);
}
void COMP::check()
{
    ;
}
void COMP::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}
void COMP::save()
{
    ;
}
string COMP::get_standard_model_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();
    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(8)<<get_Xe()
      <<"  /";
    return osstream.str();
}


size_t COMP::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}

string COMP::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}

double COMP::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}

double COMP::get_variable_with_name(string var_name)
{
    if(var_name == "GENERATOR TERMINAL VOLTAGE")
        return steps_fast_complex_abs(get_generator_terminal_voltage_in_pu());

    if(var_name == "GENERATOR TERMINAL CURRENT")
        return steps_fast_complex_abs(get_generator_terminal_current_in_pu());

    if(var_name == "COMPENSATED VOLTAGE")
        return get_compensated_voltage_in_pu();

    return 0.0;
}


string COMP::get_dynamic_data_in_psse_format() const
{
    return "";
}

string COMP::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string COMP::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
