#include "header/apis/steps_api_common.h"
#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

void show_side_not_supported_for_device_with_api(string side, DEVICE_ID did, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Side '%s' is not supported for %s with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             side.c_str(), (did.get_compound_device_name()).c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_parameter_not_supported_for_device_with_api(string par_name, DEVICE_ID did, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for %s with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), (did.get_compound_device_name()).c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_parameter_not_supported_with_api(string par_name, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_device_not_exist_with_api(DEVICE_ID did, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Device %s does not exist in database with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             (did.get_compound_device_name()).c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_vsc_hvdc_not_exist_with_api(string name, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "VSC HVDC %s does not exist in database with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             name.c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_model_type_not_supported_for_device_with_api(string model_type, DEVICE_ID did, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Model type %s is not supported for device %s with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             model_type.c_str(), (did.get_compound_device_name()).c_str(), api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}


void show_parameter_not_supported_for_area_zone_owner_with_api(string par_name, unsigned int no, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for Area/Zone/Owner %u with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             par_name.c_str(), no, api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void show_area_zone_owner_not_exist_with_api(unsigned int no, string api_func, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Area/Zone/Owner %u does not exist in database with api %s.\n"
             "Nothing will be changed.\n"
             "If return value is expected, 0/0.0/False/EMPTY STRING will be returned",
             no, api_func.c_str());
    toolkit.show_information_with_leading_time_stamp(buffer);
}

unsigned int get_owner_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, unsigned int toolkit_index)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="OWNER1")
    {
        return device->get_owner_of_index(0);
    }

    if(PARAMETER_NAME=="OWNER2")
    {
        return device->get_owner_of_index(1);
    }

    if(PARAMETER_NAME=="OWNER3")
    {
        return device->get_owner_of_index(2);
    }

    if(PARAMETER_NAME=="OWNER4")
    {
        return device->get_owner_of_index(3);
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return 0;
}

void set_owner_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string PARAMETER_NAME = string2upper(parameter_name);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    if(PARAMETER_NAME=="OWNER1")
    {
        unsigned int n = device->get_owner_count();
        if(n>=1)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 1 already set and will not be set again for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os;
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER2")
    {
        unsigned int n = device->get_owner_count();
        if(n>=2)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 2 already set and will not be set again for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER3")
    {
        unsigned int n = device->get_owner_count();
        if(n>=3)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 3 already set and will not be set again for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="OWNER4")
    {
        unsigned int n = device->get_owner_count();
        if(n>=4)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 4 already set and will not be set again for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        os.append_owner_and_its_fraction(value, 0.0);
        device->set_ownership(os);
        return;
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return;
}

double get_owner_fraction_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, unsigned int toolkit_index)
{
    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FRAC1")
    {
        return device->get_fraction_of_owner_of_index(0);
    }

    if(PARAMETER_NAME=="FRAC2")
    {
        return device->get_fraction_of_owner_of_index(1);
    }

    if(PARAMETER_NAME=="FRAC3")
    {
        return device->get_fraction_of_owner_of_index(2);
    }

    if(PARAMETER_NAME=="FRAC4")
    {
        return device->get_fraction_of_owner_of_index(3);
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
    return 0;
}

void set_owner_fraction_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string PARAMETER_NAME = string2upper(parameter_name);
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    if(PARAMETER_NAME=="FRAC1")
    {
        unsigned int n = device->get_owner_count();
        if(n<1)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 1 does not exist and owner fraction will not be set for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        unsigned int owner = device->get_owner_of_index(0);
        double frac = device->get_fraction_of_owner_of_index(0);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC2")
    {
        unsigned int n = device->get_owner_count();
        if(n<2)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 2 does not exist and owner fraction will not be set for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        unsigned int owner = device->get_owner_of_index(1);
        double frac = device->get_fraction_of_owner_of_index(1);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC3")
    {
        unsigned int n = device->get_owner_count();
        if(n<3)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 3 does not exist and owner fraction will not be set for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        unsigned int owner = device->get_owner_of_index(2);
        double frac = device->get_fraction_of_owner_of_index(2);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }

    if(PARAMETER_NAME=="FRAC4")
    {
        unsigned int n = device->get_owner_count();
        if(n<4)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Owner 4 does not exist and owner fraction will not be set for %s.",
                     (device->get_compound_device_name()).c_str());
            toolkit.show_information_with_leading_time_stamp(buffer);
            return;
        }
        OWNERSHIP os = device->get_ownership();
        unsigned int owner = device->get_owner_of_index(3);
        double frac = device->get_fraction_of_owner_of_index(3);
        os.append_owner_and_its_fraction(owner, -frac+value);
        device->set_ownership(os);
        return;
    }
    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, device->get_device_id(), __FUNCTION__);
}


