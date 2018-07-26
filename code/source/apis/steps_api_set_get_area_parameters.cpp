#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"


int api_get_area_integer_data(size_t area, char* parameter_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    AREA* areaptr = psdb->get_area(area);
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

void api_set_area_integer_data(size_t area, char* parameter_name, int value)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    AREA* areaptr = psdb->get_area(area);
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


double api_get_area_float_data(size_t area, char* parameter_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    AREA* areaptr = psdb->get_area(area);
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

void api_set_area_float_data(size_t area, char* parameter_name, double value)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    AREA* areaptr = psdb->get_area(area);
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

const char* api_get_area_string_data(size_t area, char* parameter_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    string BLANK = "";
    AREA* areaptr = psdb->get_area(area);
    if(areaptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="NAME" or PARAMETER_NAME=="AREA NAME")
            return (areaptr->get_area_name()).c_str();

        show_parameter_not_supported_for_area_zone_owner_with_api(PARAMETER_NAME, area, __FUNCTION__);
        return BLANK.c_str();
    }
    else
    {
        show_area_zone_owner_not_exist_with_api(area, __FUNCTION__);
        return BLANK.c_str();
    }
}


void api_set_area_string_data(size_t area, char* parameter_name, char* value)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    string BLANK = "";
    AREA* areaptr = psdb->get_area(area);
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

bool api_get_area_boolean_data(size_t area, char* parameter_name)
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    AREA* areaptr = psdb->get_area(area);
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


void api_set_area_boolean_data(size_t area, char* parameter_name, bool value)
{
    ostringstream osstream;
    osstream<<__FUNCTION__<<"() has not been implemented. Input parameters are provided: "<<area<<", "<<parameter_name<<", "<<value;
    show_information_with_leading_time_stamp(osstream);
    return;
}
