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

bool PVP0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<string_vector2csv(data);
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}

bool PVP0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PVP0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}
void PVP0::setup_block_toolkit_and_parameters()
{
    ;
}

void PVP0::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        PV_UNIT* pv_unit = get_pv_unit_pointer();
        if(pv_unit!=NULL)
        {
            setup_block_toolkit_and_parameters();
        }
    }
}

void PVP0::run(DYNAMIC_MODE mode)
{
    ;
}


void PVP0::check()
{
    ostringstream osstream;
}

void PVP0::clear()
{
}

void PVP0::report()
{
    ;
}

void PVP0::save()
{
    ;
}

string PVP0::get_standard_psse_string() const
{
    return "";
}

void PVP0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double PVP0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void PVP0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void PVP0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("", i); i++;
}

double PVP0::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
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
