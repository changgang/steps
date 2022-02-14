#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/VSCHVDCP0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
VSCHVDCP0::VSCHVDCP0(STEPS& toolkit):VSC_HVDC_NETWORK_MODEL(toolkit)
{
    ;
}

VSCHVDCP0::VSCHVDCP0(const VSCHVDCP0& model):VSC_HVDC_NETWORK_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSCHVDCP0::~VSCHVDCP0()
{
    ;
}

VSCHVDCP0& VSCHVDCP0::operator=(const VSCHVDCP0& model)
{
    if(this != &model)
        copy_from_const_model(model);
    return *this;
}

void VSCHVDCP0::copy_from_const_model(const VSCHVDCP0& model)
{
    ;
}

string VSCHVDCP0::get_model_name() const
{
    return "VSCHVDCP0";
}

bool VSCHVDCP0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=2)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool VSCHVDCP0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSCHVDCP0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSCHVDCP0::setup_block_toolkit_and_parameters()
{

}

void VSCHVDCP0::initialize()
{

}

void VSCHVDCP0::run(DYNAMIC_MODE mode)
{

}

void VSCHVDCP0::check()
{

}

void VSCHVDCP0::clear()
{

}

void VSCHVDCP0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSCHVDCP0::save()
{

}

string VSCHVDCP0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    string vsc_name = "'"+vsc->get_name()+"'";

    string model_name = "'"+get_model_name()+"'";


    osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<" /";
    return osstream.str();
}

void VSCHVDCP0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TEST", i); i++;
}

double VSCHVDCP0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return 0.0;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void VSCHVDCP0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double VSCHVDCP0::get_minimum_nonzero_time_constant_in_s()
{
    return 0.0;
}

void VSCHVDCP0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("TEST STATE", i); i++;
}

double VSCHVDCP0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TEST STATE") return 0.0;
    return 0.0;
}

string VSCHVDCP0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSCHVDCP0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string VSCHVDCP0::get_dynamic_data_in_steps_format() const
{
    return "";
}

void VSCHVDCP0::solve_vsc_hvdc_network()
{
    Y matrix should be rebuild(line trip, line fault) in DYNAMIC_SIMULATOR
    get Udc from converters Ceq
    solve steady state network, set VSC_HVDC_CONVERTER_STRUCT::Idc_flowing_to_Ceq_in_kA
}
