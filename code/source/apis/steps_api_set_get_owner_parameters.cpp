#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_owner_integer_data(size_t owner, char* parameter_name)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return ownerptr->get_owner_number();

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
        return 0;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
        return 0;
    }
}

void api_set_owner_integer_data(size_t owner, char* parameter_name, int value)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return ownerptr->set_owner_number(value);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
    }
    else
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
}


double api_get_owner_float_data(size_t owner, char* parameter_name)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
        return 0.0;
    }
}

void api_set_owner_float_data(size_t owner, char* parameter_name, double value)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s() has not been implemented. Input parameters are provided: %lu, %s, %f.",
             __FUNCTION__,owner, parameter_name, value);
    show_information_with_leading_time_stamp(buffer);
    return;
}

const char* api_get_owner_string_data(size_t owner, char* parameter_name)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

	snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "NAME" or PARAMETER_NAME == "OWNER NAME")
		{
			snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (ownerptr->get_owner_name()).c_str());
			return STEPS::steps_char_buffer;
		}

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
        return STEPS::steps_char_buffer;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
        return STEPS::steps_char_buffer;
    }
}


void api_set_owner_string_data(size_t owner, char* parameter_name, char* value)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string BLANK = "";
    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="NAME" or PARAMETER_NAME=="OWNER NAME")
            return ownerptr->set_owner_name(value);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
    }
    else
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
}

bool api_get_owner_boolean_data(size_t owner, char* parameter_name)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    OWNER* ownerptr = psdb.get_owner(owner);
    if(ownerptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, owner, __FUNCTION__);
        return false;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(owner, __FUNCTION__);
        return false;
    }
}


void api_set_owner_boolean_data(size_t owner, char* parameter_name, bool value)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s() has not been implemented. Input parameters are provided: %lu, %s, %s.",
             __FUNCTION__, owner, parameter_name, (value==true?"True":"False"));
    show_information_with_leading_time_stamp(buffer);
    return;
}
