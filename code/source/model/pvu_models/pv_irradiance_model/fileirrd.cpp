#include "header/model/pvu_models/pv_irradiance_model/fileirrd.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;
FILEIRRAD::FILEIRRAD()
{
    clear();
}

FILEIRRAD::~FILEIRRAD()
{
}

void FILEIRRAD::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
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

bool FILEIRRAD::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<string_vector2csv(data);
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

bool FILEIRRAD::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool FILEIRRAD::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


void FILEIRRAD::setup_block_toolkit_and_parameters()
{
    ;
}

void FILEIRRAD::initialize()
{
    setup_block_toolkit_and_parameters();
}


void FILEIRRAD::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(get_standard_psse_string());
}

void FILEIRRAD::save()
{
    ;
}

string FILEIRRAD::get_standard_psse_string() const
{
    return "";
}

void FILEIRRAD::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double FILEIRRAD::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void FILEIRRAD::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void FILEIRRAD::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("", i); i++;
}

double FILEIRRAD::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string FILEIRRAD::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string FILEIRRAD::get_dynamic_data_in_bpa_format() const
{
    return get_standard_psse_string();
}

string FILEIRRAD::get_dynamic_data_in_steps_format() const
{
    return get_standard_psse_string();
}
