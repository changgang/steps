#include "header/model/model.h"
#include <cstdio>
#include "header/basic/utility.h"

MODEL::MODEL()
{
    set_power_system_database(NULL);
    device_pointer = NULL;

    allowed_device_type = "";

    set_flag_model_initialized_as_false();

    set_flag_model_updated_as_false();

    activate_model();
}

MODEL::~MODEL()
{
    ;
}


double MODEL::get_dynamic_simulator_time_in_s() const
{
    DEVICE* device = get_device_pointer();
    if(device!=NULL)
        return device->get_dynamic_simulator_time_in_s();
    else
        return 0.0;
}

void MODEL::set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(string device_type)
{
    device_type = string2upper(device_type);
    if(device_type=="BUS" or device_type=="GENERATOR" or device_type=="WT GENERATOR" or
       device_type=="LOAD" or device_type=="LINE" or device_type=="TRANSFORMER" or
       device_type=="HVDC" or device_type=="FIXED SHUNT" or device_type=="SWITCHED SHUNT" or
       device_type=="EQUIVALENT DEVICE")
        allowed_device_type = device_type;
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Device type '"<<device_type<<"' is not supported when setting up dynamic model.";
        show_information_with_leading_time_stamp(sstream);
    }
}
string MODEL::get_allowed_device_type() const
{
    return allowed_device_type;
}

void MODEL::set_power_system_database_and_attached_device(POWER_SYSTEM_DATABASE* psdb, DEVICE_ID did)
{
    set_power_system_database(psdb);
    set_device_id(did);
}

void MODEL::set_device_id(DEVICE_ID did)
{
    ostringstream sstream;
    if(not did.is_valid())
    {
        sstream<<"Warning. Invalid device id (possible of "<<did.get_device_type()<<") is given to build model. "
          <<"Model device id will not be updated.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(did.get_device_type()!=get_allowed_device_type())
    {
        sstream<<"Warning. Invalid device type ("<<did.get_device_type()<<") is given to build model for which "<<get_allowed_device_type()<<" is expected."
          <<"Model device id will not be updated.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
        return;

    if(get_device_pointer()!=NULL)
    {
        sstream<<"Warning. Valid device ("<<get_device_name()<<") has already been set for "<<get_model_type()<<" model '"<<get_model_name()<<"'."<<endl
          <<"New device ("<<did.get_device_name()<<") will be updated.";
        show_information_with_leading_time_stamp(sstream);
    }


    device_pointer = psdb->get_device(did);

    if(device_pointer==NULL)
    {
        sstream<<"Warning. No valid device can be found for dynamic model.";
        show_information_with_leading_time_stamp(sstream);
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


size_t MODEL::get_variable_index_from_variable_name(string var_name, vector<string>& MODEL_VARIABLE_TABLE) const
{
    var_name = string2upper(var_name);

    size_t n = MODEL_VARIABLE_TABLE.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(MODEL_VARIABLE_TABLE[i]==var_name)
            return i;
    }
    return INDEX_NOT_EXIST;
}
string MODEL::get_variable_name_from_variable_index(size_t var_index, vector<string>& MODEL_VARIABLE_TABLE) const
{
    size_t n = MODEL_VARIABLE_TABLE.size();
    if(var_index<n)
        return MODEL_VARIABLE_TABLE[var_index];
    else
        return "";
}

bool MODEL::is_valid() const
{
    // should never be called
    return false;

    /*POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_device_id();
    if(psdb==NULL or (not did.is_valid()))
        return false;
    else
        return true;*/
}
