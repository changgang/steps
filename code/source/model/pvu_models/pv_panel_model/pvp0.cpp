#include "header/model/pvu_models/pv_panel_model/pvp0.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PVP0::PVP0()
{
    clear();
}

PVP0::PVP0(const PVP0& model):PV_PANEL_MODEL()
{
    copy_from_const_model(model);
}

PVP0::~PVP0()
{
    ;
}

PVP0& PVP0::operator=(const PVP0& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void PVP0::copy_from_const_model(const PVP0& model)
{
    clear();
    PV_PANEL_MODEL::copy_from_const_model(model);
}

string PVP0::get_model_name() const
{
    return "PVP0";
}

double PVP0::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double PVP0::get_model_data_with_name(string par_name) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input par_name is provided: "<<par_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

void PVP0::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void PVP0::set_model_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

bool PVP0::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}

bool PVP0::setup_model_with_psse_string(string data)
{
    return false;
}

bool PVP0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}


void PVP0::check()
{
    ostringstream oosstream;
}

void PVP0::clear()
{
    prepare_model_internal_variable_table();
}

void PVP0::report()
{
    ;
}

void PVP0::save()
{
    ;
}

string PVP0::get_standard_model_string() const
{
    return "";
}

void PVP0::prepare_model_internal_variable_table()
{
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("", i); i++;
}

double PVP0::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string PVP0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PVP0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string PVP0::get_dynamic_data_in_steps_format() const
{
    return "";
}
