#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_area_integer_data(unsigned int area, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return areaptr->get_area_number();

        if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
            return areaptr->get_area_swing_bus();

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
        return 0;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
        return 0;
    }
}

void api_set_area_integer_data(unsigned int area, char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return areaptr->set_area_number(value);

        if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
            return areaptr->set_area_swing_bus(value);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
    }
    else
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
}

double api_get_area_float_data(unsigned int area, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P_LEAVING_MW" or PARAMETER_NAME=="EXPECTED POWER LEAVING AREA IN MW")
            return areaptr->get_expected_power_leaving_area_in_MW();

        if(PARAMETER_NAME=="P_TOLERANCE_MW" or PARAMETER_NAME=="POWER MISMATCH TOLERANCE IN MW")
            return areaptr->get_area_power_mismatch_tolerance_in_MW();

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
        return 0.0;
    }
}

void api_set_area_float_data(unsigned int area, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P_LEAVING_MW" or PARAMETER_NAME=="EXPECTED POWER LEAVING AREA IN MW")
            return areaptr->set_expected_power_leaving_area_in_MW(value);

        if(PARAMETER_NAME=="P_TOLERANCE_MW" or PARAMETER_NAME=="POWER MISMATCH TOLERANCE IN MW")
            return areaptr->set_area_power_mismatch_tolerance_in_MW(value);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
    }
    else
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
}

const char* api_get_area_string_data(unsigned int area, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "NAME" or PARAMETER_NAME == "AREA NAME")
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (areaptr->get_area_name()).c_str());
			return toolkit.steps_char_buffer;
		}

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
}


void api_set_area_string_data(unsigned int area, char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string BLANK = "";
    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="NAME" or PARAMETER_NAME=="AREA NAME")
            return areaptr->set_area_name(value);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
    }
    else
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
}

bool api_get_area_boolean_data(unsigned int area, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    AREA* areaptr = psdb.get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
        return false;
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
        return false;
    }
}

void api_set_area_boolean_data(unsigned int area, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s() has not been implemented. Input parameters are provided: %u, %s, %s.",
             __FUNCTION__, area, parameter_name, (value==true?"True":"False"));
    toolkit.show_information_with_leading_time_stamp(buffer);
    return;
}
