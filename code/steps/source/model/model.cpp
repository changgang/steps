#include "header/model/model.h"
#include <cstdio>
#include "header/basic/utility.h"
#include "header/STEPS.h"

#include <iostream>
#include <cstring>
using namespace std;

MODEL::MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);

    device_pointer = NULL;

    bus_pointer = NULL;
    //inverter_bus_pointer = NULL;

    for(unsigned int i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
        allowed_device_types[i][0]='\0';

    model_data_table = NULL;
    model_internal_variable_table = NULL;

    set_flag_model_initialized_as_false();

    set_flag_model_updated_as_false();

    activate_model();

    set_model_float_parameter_count(0);

    user_input_time_series_file = NULL;
    user_input_time_series = NULL;
}

STEPS& MODEL::get_toolkit() const
{
    return *toolkit;
}

void MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

MODEL::~MODEL()
{
    /*if(allowed_device_types!=NULL)
        delete allowed_device_types;*/

    destroy_manually_allocated_storage();
}

void MODEL::allocate_model_variables()
{
    /*if(allowed_device_types==NULL)
        allowed_device_types = new vector<string>;*/

    if(model_data_table==NULL)
        model_data_table =  new MODEL_VAR_TABLE;
    if(model_internal_variable_table==NULL)
        model_internal_variable_table =  new MODEL_VAR_TABLE;

    if(user_input_time_series_file==NULL)
        user_input_time_series_file = new string;
    if(user_input_time_series==NULL)
        user_input_time_series = new TIME_SERIES;
}

void MODEL::destroy_manually_allocated_storage()
{
    if(model_data_table!=NULL)
        delete model_data_table;
    if(model_internal_variable_table!=NULL)
        delete model_internal_variable_table;

    if(user_input_time_series_file!=NULL)
        delete user_input_time_series_file;
    if(user_input_time_series!=NULL)
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
            for(unsigned int i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
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
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}
vector<string> MODEL::get_allowed_device_types() const
{
    vector<string> temp;
    for(unsigned int i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
    {
        if(allowed_device_types[i][0]!='\0')
            temp.push_back(allowed_device_types[i]);
    }
    return temp;
}

bool MODEL::has_allowed_device_type(string device_type) const
{
    device_type = string2upper(device_type);
    for(unsigned int i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
    {
        if(allowed_device_types[i][0]!='\0')
        {
            if(allowed_device_types[i]==device_type)
                return true;
        }
    }
    return false;
}

void MODEL::set_model_float_parameter_count(unsigned int n)
{
    n_parameters = n;
}

unsigned int MODEL::get_model_float_parameter_count() const
{
    return n_parameters;
}

void MODEL::clear_model_data_table()
{
    if(model_data_table!=NULL)
        model_data_table->clear();
}

void MODEL::add_model_data_name_and_index_pair(string var_name, unsigned int var_index)
{
    if(model_data_table!=NULL)
        model_data_table->add_variable_name_index_pair(var_name, var_index);
}

unsigned int MODEL::get_model_data_index(string var_name) const
{
    if(model_data_table!=NULL)
        return (*model_data_table)[var_name];
    else
        return INDEX_NOT_EXIST;
}

string MODEL::get_model_data_name(unsigned int var_index) const
{
    if(model_data_table!=NULL)
        return (*model_data_table)[var_index];
    else
        return "";
}

bool MODEL::is_model_data_exist(string var_name) const
{
    if(model_data_table!=NULL)
        return (*model_data_table)[var_name]!=INDEX_NOT_EXIST;
    else
        return false;
}

bool MODEL::is_model_data_exist(unsigned int var_index) const
{
    if(model_data_table!=NULL)
        return (*model_data_table)[var_index]!="";
    else
        return false;
}

void MODEL::set_model_data_with_index(unsigned int index, double value)
{
    string var_name = get_model_data_name(index);
    if(var_name!="")
        set_model_data_with_name(var_name, value);
    else
        toolkit->show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
}

double MODEL::get_model_data_with_index(unsigned int index)
{
    string var_name = get_model_data_name(index);
    if(var_name!="")
        return get_model_data_with_name(var_name);
    else
    {
        toolkit->show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
        return 0.0;
    }
}

void MODEL::clear_model_internal_variable_table()
{
    if(model_internal_variable_table!=NULL)
        model_internal_variable_table->clear();
}

void MODEL::add_model_inernal_variable_name_and_index_pair(string var_name, unsigned int var_index)
{
    if(model_internal_variable_table!=NULL)
        model_internal_variable_table->add_variable_name_index_pair(var_name, var_index);
}

unsigned int MODEL::get_model_inernal_variable_index(string var_name) const
{
    if(model_internal_variable_table!=NULL)
        return (*model_internal_variable_table)[var_name];
    else
        return INDEX_NOT_EXIST;
}

string MODEL::get_model_inernal_variable_name(unsigned int var_index) const
{
    if(model_internal_variable_table!=NULL)
        return (*model_internal_variable_table)[var_index];
    else
        return "";
}

bool MODEL::is_model_inernal_variable_exist(string var_name) const
{
    if(model_internal_variable_table!=NULL)
        return (*model_internal_variable_table)[var_name]!=INDEX_NOT_EXIST;
    else
        return false;
}

bool MODEL::is_model_inernal_variable_exist(unsigned int var_index) const
{
    if(model_internal_variable_table!=NULL)
        return (*model_internal_variable_table)[var_index]!="";
    else
        return false;
}

double MODEL::get_model_internal_variable_with_index(unsigned int index)
{
    string var_name = get_model_inernal_variable_name(index);
    if(var_name!="") return get_model_internal_variable_with_name(var_name);
    else             return 0.0;
}

void MODEL::set_device_id(DEVICE_ID did)
{
    ostringstream osstream;
    if(not did.is_valid())
    {
        osstream<<"Warning. Invalid device id (possible of "<<did.get_device_type()<<") is given to build model. "
          <<"Model device id will not be updated.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(not has_allowed_device_type(did.get_device_type()))
    {
        osstream<<"Warning. Invalid device type ("<<did.get_device_type()<<") is given to build model for which the following types of devices is expected:\n";
        for(unsigned int i=0; i<STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT; ++i)
        {
            if(allowed_device_types[i][0]!='\0')
                osstream<<allowed_device_types[i]<<"\n";
        }
        osstream<<"Model device id will not be updated.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    /*if(get_device_pointer()!=NULL)
    {
        osstream<<"Warning. Valid device ("<<get_device_name()<<") has already been set for "<<get_model_type()<<" model '"<<get_model_name()<<"'."<<endl
          <<"New device ("<<did.get_device_name()<<") will be updated.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/

    device_pointer = psdb.get_nonbus_device(did);

    if(device_pointer!=NULL)
    {
        string device_type = did.get_device_type();
        if(device_type=="GENERATOR")
        {
            bus_pointer = ((GENERATOR*) device_pointer)->get_bus_pointer();
            return;
        }
        if(device_type=="WT GENERATOR")
        {
            bus_pointer = ((WT_GENERATOR*) device_pointer)->get_bus_pointer();
            return;
        }
        if(device_type=="PV UNIT")
        {
            bus_pointer = ((PV_UNIT*) device_pointer)->get_bus_pointer();
            return;
        }
        if(device_type=="ENERGY STORAGE")
        {
            bus_pointer = ((ENERGY_STORAGE*) device_pointer)->get_bus_pointer();
            return;
        }
        if(device_type=="LOAD")
        {
            bus_pointer = ((LOAD*) device_pointer)->get_bus_pointer();
            return;
        }
        /*if(device_type=="LINE")
        {
            sending_bus_pointer = ((LINE*) device_pointer)->get_sending_side_bus_pointer();
            receiving_bus_pointer = ((LINE*) device_pointer)->get_receiving_side_bus_pointer();
            return;
        }
        if(device_type=="HVDC")
        {
            rectifier_bus_pointer = ((HVDC*) device_pointer)->get_bus_pointer(RECTIFIER);
            inverter_bus_pointer = ((HVDC*) device_pointer)->get_bus_pointer(INVERTER);
            return;
        }
        if(device_type=="TRANSFORMER")
        {
            primary_winding_bus_pointer = ((TRANSFORMER*) device_pointer)->get_winding_bus_pointer(PRIMARY_SIDE);
            secondary_winding_bus_pointer = ((TRANSFORMER*) device_pointer)->get_winding_bus_pointer(SECONDARY_SIDE);
            //tertiary_winding_bus_pointer = ((TRANSFORMER*) device_pointer)->get_winding_bus_pointer(TERTIARY_SIDE);
            return;
        }*/
    }
    else
    {
        osstream<<"Warning. No valid device can be found for dynamic model.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

NONBUS_DEVICE* MODEL::get_device_pointer() const
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

BUS* MODEL::get_bus_pointer() const
{
    return bus_pointer;
}


/*BUS* MODEL::get_bus_pointer(HVDC_CONVERTER_SIDE converter) const
{
    if(converter==RECTIFIER)
        return rectifier_bus_pointer;
    else
        return inverter_bus_pointer;
}

BUS* MODEL::get_bus_pointer(LINE_SIDE side) const
{
    if(side==SENDING_SIDE)
        return sending_bus_pointer;
    else
        return receiving_bus_pointer;
}

BUS* MODEL::get_bus_pointer(TRANSFORMER_WINDING_SIDE winding) const
{
    switch(winding)
    {
        case PRIMARY_SIDE:
            return primary_winding_bus_pointer;
        case SECONDARY_SIDE:
            return secondary_winding_bus_pointer;
        default:
            return primary_winding_bus_pointer;
            //return tertiary_winding_bus_pointer;
    }
}*/

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

