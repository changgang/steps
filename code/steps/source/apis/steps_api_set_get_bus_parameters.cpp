#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_bus_integer_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="TYPE" or PARAMETER_NAME=="BUS TYPE")
        {
            BUS_TYPE bus_type = busptr->get_bus_type();
            switch(bus_type)
            {
                case PQ_TYPE:
                    return 1;
                case PV_TYPE:
                case PV_TO_PQ_TYPE_1:
                case PV_TO_PQ_TYPE_2:
                case PV_TO_PQ_TYPE_3:
                case PV_TO_PQ_TYPE_4:
                    return 2;
                case SLACK_TYPE:
                    return 3;
                case OUT_OF_SERVICE:
                    return 0;
                default:
                    return 0;
            }
        }

        if(PARAMETER_NAME=="EQUIVALENT BUS" or PARAMETER_NAME=="EQUIVALENT BUS NUMBER")
            return busptr->get_equivalent_bus_number();

        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return busptr->get_area_number();

        if(PARAMETER_NAME=="ZONE" or PARAMETER_NAME=="ZONE NUMBER")
            return busptr->get_zone_number();

        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return busptr->get_owner_number();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_bus_integer_data(unsigned int bus, char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="TYPE" or PARAMETER_NAME=="BUS TYPE")
        {
            switch(value)
            {
                case 1:
                {
                    busptr->set_bus_type(PQ_TYPE);
                    break;
                }
                case 2:
                case -2:
                {
                    busptr->set_bus_type(PV_TYPE);
                    break;
                }
                case 3:
                {
                    busptr->set_bus_type(SLACK_TYPE);
                    break;
                }
                case 0:
                {
                    busptr->set_bus_type(OUT_OF_SERVICE);
                    break;
                }
                default:
                {
                    break;
                }
            }
            return;
        }
        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return busptr->set_area_number(value);

        if(PARAMETER_NAME=="ZONE" or PARAMETER_NAME=="ZONE NUMBER")
            return busptr->set_zone_number(value);

        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return busptr->set_owner_number(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

double api_get_bus_float_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="VBASE_KV" or PARAMETER_NAME=="BASE VOLTAGE IN KV")
            return busptr->get_base_voltage_in_kV();

        if(PARAMETER_NAME=="V_PU" or PARAMETER_NAME=="VOLTAGE IN PU")
            return busptr->get_positive_sequence_voltage_in_pu();

        if(PARAMETER_NAME=="V_KV" or PARAMETER_NAME=="VOLTAGE IN KV")
            return busptr->get_positive_sequence_voltage_in_kV();

        if(PARAMETER_NAME=="ANGLE_RAD" or PARAMETER_NAME=="ANGLE IN RAD")
            return busptr->get_positive_sequence_angle_in_rad();

        if(PARAMETER_NAME=="ANGLE_DEG" or PARAMETER_NAME=="ANGLE IN DEG")
            return busptr->get_positive_sequence_angle_in_deg();

        if(PARAMETER_NAME=="VMAX_PU" or PARAMETER_NAME=="NORMAL VMAX IN PU")
            return busptr->get_normal_voltage_upper_limit_in_pu();

        if(PARAMETER_NAME=="VMIN_PU" or PARAMETER_NAME=="NORMAL VMIN IN PU")
            return busptr->get_normal_voltage_lower_limit_in_pu();

        if(PARAMETER_NAME=="EMAX_PU" or PARAMETER_NAME=="EMERGENCY VMAX IN PU")
            return busptr->get_emergency_voltage_upper_limit_in_pu();

        if(PARAMETER_NAME=="EMIN_PU" or PARAMETER_NAME=="EMERGENCY VMIN IN PU")
            return busptr->get_emergency_voltage_lower_limit_in_pu();

        if(PARAMETER_NAME=="FBASE_HZ" or PARAMETER_NAME=="BASE FREQUENCY IN HZ")
            return busptr->get_base_frequency_in_Hz();

        if(PARAMETER_NAME=="V2_PU" or PARAMETER_NAME=="NEGATIVE VOLTAGE IN PU")
            return busptr->get_negative_sequence_voltage_in_pu();

        if(PARAMETER_NAME=="V2_KV" or PARAMETER_NAME=="NEGATIVE VOLTAGE IN KV")
            return busptr->get_negative_sequence_voltage_in_kV();

        if(PARAMETER_NAME=="NEGATIVE_ANGLE_RAD" or PARAMETER_NAME=="NEGATIVE ANGLE IN RAD")
            return busptr->get_negative_sequence_angle_in_rad();

        if(PARAMETER_NAME=="NEGATIVE_ANGLE_DEG" or PARAMETER_NAME=="NEGATIVE ANGLE IN DEG")
            return busptr->get_negative_sequence_angle_in_deg();

        if(PARAMETER_NAME=="V0_PU" or PARAMETER_NAME=="ZERO VOLTAGE IN PU")
            return busptr->get_zero_sequence_voltage_in_pu();

        if(PARAMETER_NAME=="V0_KV" or PARAMETER_NAME=="ZERO VOLTAGE IN KV")
            return busptr->get_zero_sequence_voltage_in_kV();

        if(PARAMETER_NAME=="ZERO_ANGLE_RAD" or PARAMETER_NAME=="ZERO ANGLE IN RAD")
            return busptr->get_zero_sequence_angle_in_rad();

        if(PARAMETER_NAME=="ZERO_ANGLE_DEG" or PARAMETER_NAME=="ZERO ANGLE IN DEG")
            return busptr->get_zero_sequence_angle_in_deg();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_bus_float_data(unsigned int bus, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="VBASE_KV" or PARAMETER_NAME=="BASE VOLTAGE IN KV")
            return busptr->set_base_voltage_in_kV(value);

        if(PARAMETER_NAME=="V_PU" or PARAMETER_NAME=="VOLTAGE IN PU")
            return busptr->set_positive_sequence_voltage_in_pu(value);

        if(PARAMETER_NAME=="V_KV" or PARAMETER_NAME=="VOLTAGE IN KV")
            return busptr->set_positive_sequence_voltage_in_kV(value);

        if(PARAMETER_NAME=="ANGLE_RAD" or PARAMETER_NAME=="ANGLE IN RAD")
            return busptr->set_positive_sequence_angle_in_rad(value);

        if(PARAMETER_NAME=="ANGLE_DEG" or PARAMETER_NAME=="ANGLE IN DEG")
            return busptr->set_positive_sequence_angle_in_deg(value);

        if(PARAMETER_NAME=="VMAX_PU" or PARAMETER_NAME=="NORMAL VMAX IN PU")
            return busptr->set_normal_voltage_upper_limit_in_pu(value);

        if(PARAMETER_NAME=="VMIN_PU" or PARAMETER_NAME=="NORMAL VMIN IN PU")
            return busptr->set_normal_voltage_lower_limit_in_pu(value);

        if(PARAMETER_NAME=="EMAX_PU" or PARAMETER_NAME=="EMERGENCY VMAX IN PU")
            return busptr->set_emergency_voltage_upper_limit_in_pu(value);

        if(PARAMETER_NAME=="EMIN_PU" or PARAMETER_NAME=="EMERGENCY VMIN IN PU")
            return busptr->set_emergency_voltage_lower_limit_in_pu(value);

        if(PARAMETER_NAME=="FBASE_HZ" or PARAMETER_NAME=="BASE FREQUENCY IN HZ")
            return busptr->set_base_frequency_in_Hz(value);

        if(PARAMETER_NAME=="V2_PU" or PARAMETER_NAME=="NEGATIVE VOLTAGE IN PU")
            return busptr->set_negative_sequence_voltage_in_pu(value);

        if(PARAMETER_NAME=="V2_KV" or PARAMETER_NAME=="NEGATIVE VOLTAGE IN KV")
            return busptr->set_negative_sequence_voltage_in_kV(value);

        if(PARAMETER_NAME=="NEGATIVE_ANGLE_RAD" or PARAMETER_NAME=="NEGATIVE ANGLE IN RAD")
            return busptr->set_negative_sequence_angle_in_rad(value);

        if(PARAMETER_NAME=="NEGATIVE_ANGLE_DEG" or PARAMETER_NAME=="NEGATIVE ANGLE IN DEG")
            return busptr->set_negative_sequence_angle_in_deg(value);

        if(PARAMETER_NAME=="V0_PU" or PARAMETER_NAME=="ZERO VOLTAGE IN PU")
            return busptr->set_zero_sequence_voltage_in_pu(value);

        if(PARAMETER_NAME=="V0_KV" or PARAMETER_NAME=="ZERO VOLTAGE IN KV")
            return busptr->set_zero_sequence_voltage_in_kV(value);

        if(PARAMETER_NAME=="ZERO_ANGLE_RAD" or PARAMETER_NAME=="ZERO ANGLE IN RAD")
            return busptr->set_zero_sequence_angle_in_rad(value);

        if(PARAMETER_NAME=="ZERO_ANGLE_DEG" or PARAMETER_NAME=="ZERO ANGLE IN DEG")
            return busptr->set_zero_sequence_angle_in_deg(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_bus_string_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "NAME" or PARAMETER_NAME == "BUS NAME")
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (busptr->get_bus_name()).c_str());
			return toolkit.steps_char_buffer;
		}

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
}

void api_set_bus_string_data(unsigned int bus, char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="NAME" or PARAMETER_NAME=="BUS NAME")
            return busptr->set_bus_name(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_bus_boolean_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_bus_device_id(bus);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_bus_boolean_data(unsigned int bus, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s() has not been implemented. Input parameters are provided: %u, %s, %s.",
             __FUNCTION__, bus, parameter_name, (value==true?"True":"False"));
    toolkit.show_information_with_leading_time_stamp(buffer);
    return;
}
