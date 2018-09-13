#include "header/model/pvu_models/pv_irradiance_model/fileirrd.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@GOVERNOR",     //4
                                            "STATE@TURBINE",       //5
                                            };//8

FILEIRRAD::FILEIRRAD()
{
    clear();
}

FILEIRRAD::~FILEIRRAD()
{
    clear();
}

void FILEIRRAD::clear()
{
}

void FILEIRRAD::copy_from_const_model(const FILEIRRAD& model)
{
    clear();
}

FILEIRRAD::FILEIRRAD(const FILEIRRAD& model)
{
    copy_from_const_model(model);
}

FILEIRRAD& FILEIRRAD::operator=(const FILEIRRAD& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string FILEIRRAD::get_model_name() const
{
    return "FILEIRRAD";
}


void FILEIRRAD::set_solar_irradiance_serial_file(string file)
{
    ;
}

string FILEIRRAD::get_solar_irradiance_serial_file() const
{
    return "";
}


double FILEIRRAD::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double FILEIRRAD::get_model_data_with_name(string par_name) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input par_name is provided: "<<par_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

void FILEIRRAD::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void FILEIRRAD::set_model_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

bool FILEIRRAD::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool FILEIRRAD::setup_model_with_psse_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool FILEIRRAD::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}


void FILEIRRAD::initialize()
{

}


void FILEIRRAD::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    show_information_with_leading_time_stamp(osstream);
}

double FILEIRRAD::get_solar_irradiance_in_pu()
{
    return 0.0;
}

double FILEIRRAD::get_solar_irradiance_direction_in_deg()
{
    return 0.0;
}


void FILEIRRAD::check()
{
    ;
}


void FILEIRRAD::report()
{
    show_information_with_leading_time_stamp(get_standard_model_string());
}

void FILEIRRAD::save()
{
    ;
}

string FILEIRRAD::get_standard_model_string() const
{
    return "";
}


size_t FILEIRRAD::get_variable_index_from_variable_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0;
}

string FILEIRRAD::get_variable_name_from_variable_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return "";
}

double FILEIRRAD::get_variable_with_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double FILEIRRAD::get_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string FILEIRRAD::get_dynamic_data_in_psse_format() const
{
    return get_standard_model_string();
}

string FILEIRRAD::get_dynamic_data_in_bpa_format() const
{
    return get_standard_model_string();
}

string FILEIRRAD::get_dynamic_data_in_steps_format() const
{
    return get_standard_model_string();
}
