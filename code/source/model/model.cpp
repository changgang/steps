#include "header/model/model.h"
#include <cstdio>
#include "header/basic/utility.h"
#include "header/STEPS.h"

#include <iostream>
#include <cstring>
using namespace std;

MODEL::MODEL()
{
    device_pointer = NULL;

    for(size_t i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
        allowed_device_types[i][0]='\0';

    model_data_table = nullptr;
    model_internal_variable_table = nullptr;

    set_flag_model_initialized_as_false();

    set_flag_model_updated_as_false();

    activate_model();

    set_model_float_parameter_count(0);

    user_input_time_series_file = nullptr;
    user_input_time_series = nullptr;
}

MODEL::~MODEL()
{
    /*if(allowed_device_types!=nullptr)
        delete allowed_device_types;*/

    destroy_manually_allocated_storage();
}

void MODEL::allocate_model_variables()
{
    /*if(allowed_device_types==nullptr)
        allowed_device_types = new vector<string>;*/

    if(model_data_table==nullptr)
        model_data_table =  new MODEL_VAR_TABLE;
    if(model_internal_variable_table==nullptr)
        model_internal_variable_table =  new MODEL_VAR_TABLE;

    if(user_input_time_series_file==nullptr)
        user_input_time_series_file = new string;
    if(user_input_time_series==nullptr)
        user_input_time_series = new TIME_SERIES;
}

void MODEL::destroy_manually_allocated_storage()
{
    if(model_data_table!=nullptr)
        delete model_data_table;
    if(model_internal_variable_table!=nullptr)
        delete model_internal_variable_table;

    if(user_input_time_series_file!=nullptr)
        delete user_input_time_series_file;
    if(user_input_time_series!=nullptr)
        delete user_input_time_series;
}

void MODEL::set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(string device_type)
{
    device_type = string2upper(device_type);
    if(device_type=="BUS" or device_type=="GENERATOR" or device_type=="WT GENERATOR" or device_type=="PV UNIT" or
       device_type=="LOAD" or device_type=="LINE" or device_type=="TRANSFORMER" or
       device_type=="HVDC" or device_type=="FIXED SHUNT" or device_type=="SWITCHED SHUNT" or
       device_type=="EQUIVALENT DEVICE" or device_type=="ENERGY STORAGE")
    {
        if(not has_allowed_device_type(device_type))
        {
            for(size_t i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
            {
                if(allowed_device_types[i][0]=='\0')
                {
                    strcpy(allowed_device_types[i],device_type.c_str());
                    break;
                }
            }
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Device type '"<<device_type<<"' is not supported when setting up dynamic model.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}
vector<string> MODEL::get_allowed_device_types() const
{
    vector<string> temp;
    for(size_t i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
    {
        if(allowed_device_types[i][0]!='\0')
            temp.push_back(allowed_device_types[i]);
    }
    return temp;
}

bool MODEL::has_allowed_device_type(string device_type) const
{
    device_type = string2upper(device_type);
    for(size_t i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
    {
        if(allowed_device_types[i][0]!='\0')
        {
            if(allowed_device_types[i]==device_type)
                return true;
        }
    }
    return false;
}

void MODEL::set_model_float_parameter_count(size_t n)
{
    n_parameters = n;
}

size_t MODEL::get_model_float_parameter_count() const
{
    return n_parameters;
}

void MODEL::clear_model_data_table()
{
    if(model_data_table!=nullptr)
        model_data_table->clear();
}

void MODEL::add_model_data_name_and_index_pair(string var_name, size_t var_index)
{
    if(model_data_table!=nullptr)
        model_data_table->add_variable_name_index_pair(var_name, var_index);
}

size_t MODEL::get_model_data_index(string var_name) const
{
    if(model_data_table!=nullptr)
        return (*model_data_table)[var_name];
    else
        return INDEX_NOT_EXIST;
}

string MODEL::get_model_data_name(size_t var_index) const
{
    if(model_data_table!=nullptr)
        return (*model_data_table)[var_index];
    else
        return "";
}

bool MODEL::is_model_data_exist(string var_name) const
{
    if(model_data_table!=nullptr)
        return (*model_data_table)[var_name]!=INDEX_NOT_EXIST;
    else
        return false;
}

bool MODEL::is_model_data_exist(size_t var_index) const
{
    if(model_data_table!=nullptr)
        return (*model_data_table)[var_index]!="";
    else
        return false;
}

void MODEL::set_model_data_with_index(size_t index, double value)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    string var_name = get_model_data_name(index);
    if(var_name!="")
        set_model_data_with_name(var_name, value);
    else
        toolkit.show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
}

double MODEL::get_model_data_with_index(size_t index)
{
    string var_name = get_model_data_name(index);
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    if(var_name!="")
        return get_model_data_with_name(var_name);
    else
    {
        toolkit.show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
        return 0.0;
    }
}

void MODEL::clear_model_internal_variable_table()
{
    if(model_internal_variable_table!=nullptr)
        model_internal_variable_table->clear();
}

void MODEL::add_model_inernal_variable_name_and_index_pair(string var_name, size_t var_index)
{
    if(model_internal_variable_table!=nullptr)
        model_internal_variable_table->add_variable_name_index_pair(var_name, var_index);
}

size_t MODEL::get_model_inernal_variable_index(string var_name) const
{
    if(model_internal_variable_table!=nullptr)
        return (*model_internal_variable_table)[var_name];
    else
        return INDEX_NOT_EXIST;
}

string MODEL::get_model_inernal_variable_name(size_t var_index) const
{
    if(model_internal_variable_table!=nullptr)
        return (*model_internal_variable_table)[var_index];
    else
        return "";
}

bool MODEL::is_model_inernal_variable_exist(string var_name) const
{
    if(model_internal_variable_table!=nullptr)
        return (*model_internal_variable_table)[var_name]!=INDEX_NOT_EXIST;
    else
        return false;
}

bool MODEL::is_model_inernal_variable_exist(size_t var_index) const
{
    if(model_internal_variable_table!=nullptr)
        return (*model_internal_variable_table)[var_index]!="";
    else
        return false;
}

double MODEL::get_model_internal_variable_with_index(size_t index)
{
    string var_name = get_model_inernal_variable_name(index);
    if(var_name!="") return get_model_internal_variable_with_name(var_name);
    else             return 0.0;
}

void MODEL::set_device_id(DEVICE_ID did)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    if(not did.is_valid())
    {
        osstream<<"Warning. Invalid device id (possible of "<<did.get_device_type()<<") is given to build model. "
          <<"Model device id will not be updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(not has_allowed_device_type(did.get_device_type()))
    {
        osstream<<"Warning. Invalid device type ("<<did.get_device_type()<<") is given to build model for which the following types of devices is expected:\n";
        for(size_t i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
        {
            if(allowed_device_types[i][0]!='\0')
                osstream<<allowed_device_types[i]<<"\n";
        }
        osstream<<"Model device id will not be updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    /*if(get_device_pointer()!=NULL)
    {
        osstream<<"Warning. Valid device ("<<get_device_name()<<") has already been set for "<<get_model_type()<<" model '"<<get_model_name()<<"'."<<endl
          <<"New device ("<<did.get_device_name()<<") will be updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }*/

    device_pointer = psdb.get_device(did);

    if(device_pointer==NULL)
    {
        osstream<<"Warning. No valid device can be found for dynamic model.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

DEVICE* MODEL::get_device_pointer() const
{
    return device_pointer;
}

DEVICE_ID MODEL::get_device_id() const
{
    DEVICE_ID did;
    DEVICE* ptr = get_device_pointer();
    if(ptr!=NULL)
        did = ptr->get_device_id();

    return did;
}

string MODEL::get_device_name() const
{
    return get_device_id().get_device_name();
}

void MODEL::set_flag_model_initialized_as_false()
{
    flag_model_initialized = false;
}

void MODEL::set_flag_model_initialized_as_true()
{
    flag_model_initialized = true;
}

bool MODEL::is_model_initialized() const
{
    return flag_model_initialized;
}

void MODEL::set_flag_model_updated_as_false()
{
    flag_model_updated = false;
}

void MODEL::set_flag_model_updated_as_true()
{
    flag_model_updated = true;
}

bool MODEL::is_model_updated() const
{
    return flag_model_updated;
}


void MODEL::activate_model()
{
    flag_model_active = true;
}
void MODEL::deactivate_model()
{
    flag_model_active = false;
}

bool MODEL::is_model_active() const
{
    return flag_model_active;
}


bool MODEL::is_valid() const
{
    // should never be called
    return false;

    /*POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_device_id();
    if(psdb==NULL or (not did.is_valid()))
        return false;
    else
        return true;*/
}

