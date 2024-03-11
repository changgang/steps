#include "header/model/energy_storage_model/es_battery_model/esbx.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

ESBX::ESBX(STEPS& toolkit) : ES_BATTERY_MODEL(toolkit)
{
    clear();
}

ESBX::ESBX(const ESBX& model):ES_BATTERY_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESBX::~ESBX()
{
    ;
}

ESBX& ESBX::operator=(const ESBX& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void ESBX::copy_from_const_model(const ESBX& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    //ES_BATTERY_MODEL::copy_from_const_model(model);
}

string ESBX::get_model_name() const
{
    return "ESBX";
}

bool ESBX::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<string_vector2csv(data);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}

bool ESBX::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESBX::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}
void ESBX::setup_block_toolkit_and_parameters()
{
    ;
}

void ESBX::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();
    }
}

void ESBX::run(DYNAMIC_MODE mode)
{
    ;
}


void ESBX::check()
{
    ostringstream osstream;
}

void ESBX::clear()
{
}

void ESBX::report()
{
    ;
}

void ESBX::save()
{
    ;
}

string ESBX::get_standard_psse_string(bool export_internal_bus_number) const
{
    return "";
}

void ESBX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double ESBX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void ESBX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double ESBX::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void ESBX::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("", i); i++;
}

double ESBX::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string ESBX::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESBX::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string ESBX::get_dynamic_data_in_steps_format() const
{
    return "";
}
