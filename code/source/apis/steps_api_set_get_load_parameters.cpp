#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_load_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return loadptr->get_area_number();

        if(PARAMETER_NAME=="ZONE" or PARAMETER_NAME=="ZONE NUMBER")
            return loadptr->get_zone_number();

        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return loadptr->get_owner_number();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_load_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="AREA" or PARAMETER_NAME=="AREA NUMBER")
            return loadptr->set_area_number(value);

        if(PARAMETER_NAME=="ZONE" or PARAMETER_NAME=="ZONE NUMBER")
            return loadptr->set_zone_number(value);

        if(PARAMETER_NAME=="OWNER" or PARAMETER_NAME=="OWNER NUMBER")
            return loadptr->set_owner_number(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

double api_get_load_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="PP0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT POWER ACTIVE POWER IN MW")
            return loadptr->get_nominal_constant_power_load_in_MVA().real();

        if(PARAMETER_NAME=="QP0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT POWER REACTIVE POWER IN MVAR")
            return loadptr->get_nominal_constant_power_load_in_MVA().imag();

        if(PARAMETER_NAME=="PI0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT ACTIVE POWER IN MW")
            return loadptr->get_nominal_constant_current_load_in_MVA().real();

        if(PARAMETER_NAME=="QI0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
            return loadptr->get_nominal_constant_current_load_in_MVA().imag();

        if(PARAMETER_NAME=="PZ0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT IMPEDANCE ACTIVE POWER IN MW")
            return loadptr->get_nominal_constant_impedance_load_in_MVA().real();

        if(PARAMETER_NAME=="QZ0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
            return loadptr->get_nominal_constant_impedance_load_in_MVA().imag();

        if(PARAMETER_NAME=="P0_MW" or PARAMETER_NAME=="TOTAL NOMINAL ACTIVE POWER IN MW")
            return loadptr->get_nominal_total_load_in_MVA().real();

        if(PARAMETER_NAME=="Q0_MVAR" or PARAMETER_NAME=="TOTAL NOMINAL REACTIVE POWER IN MVAR")
            return loadptr->get_nominal_total_load_in_MVA().imag();

        if(PARAMETER_NAME=="PP_MW" or PARAMETER_NAME=="ACTUAL CONSTANT POWER ACTIVE POWER IN MW")
            return loadptr->get_actual_constant_power_load_in_MVA().real();

        if(PARAMETER_NAME=="QP_MVAR" or PARAMETER_NAME=="ACTUAL CONSTANT POWER REACTIVE POWER IN MVAR")
            return loadptr->get_actual_constant_power_load_in_MVA().imag();

        if(PARAMETER_NAME=="PI_MW" or PARAMETER_NAME=="ACTUAL CONSTANT CURRENT ACTIVE POWER IN MW")
            return loadptr->get_actual_constant_current_load_in_MVA().real();

        if(PARAMETER_NAME=="QI_MVAR" or PARAMETER_NAME=="ACTUAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
            return loadptr->get_actual_constant_current_load_in_MVA().imag();

        if(PARAMETER_NAME=="PZ_MW" or PARAMETER_NAME=="ACTUAL CONSTANT IMPEDANCE ACTIVE POWER IN MW")
            return loadptr->get_actual_constant_impedance_load_in_MVA().real();

        if(PARAMETER_NAME=="QZ_MVAR" or PARAMETER_NAME=="ACTUAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
            return loadptr->get_actual_constant_impedance_load_in_MVA().imag();

        if(PARAMETER_NAME=="P_MW" or PARAMETER_NAME=="TOTAL ACTUAL ACTIVE POWER IN MW")
            return loadptr->get_actual_total_load_in_MVA().real();

        if(PARAMETER_NAME=="Q_MVAR" or PARAMETER_NAME=="TOTAL ACTUAL REACTIVE POWER IN MVAR")
            return loadptr->get_actual_total_load_in_MVA().imag();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_load_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="PP0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT POWER ACTIVE POWER IN MW")
        {
            double q = loadptr->get_nominal_constant_power_load_in_MVA().imag();
            return loadptr->set_nominal_constant_power_load_in_MVA(complex<double>(value, q));
        }

        if(PARAMETER_NAME=="QP0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT POWER REACTIVE POWER IN MVAR")
        {
            double p = loadptr->get_nominal_constant_power_load_in_MVA().real();
            return loadptr->set_nominal_constant_power_load_in_MVA(complex<double>(p, value));
        }

        if(PARAMETER_NAME=="PI0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT ACTIVE POWER IN MW")
        {
            double q = loadptr->get_nominal_constant_current_load_in_MVA().imag();
            return loadptr->set_nominal_constant_current_load_in_MVA(complex<double>(value, q));
        }

        if(PARAMETER_NAME=="QI0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
        {
            double p = loadptr->get_nominal_constant_current_load_in_MVA().real();
            return loadptr->set_nominal_constant_current_load_in_MVA(complex<double>(p, value));
        }

        if(PARAMETER_NAME=="PZ0_MW" or PARAMETER_NAME=="NOMINAL CONSTANT IMPEDANCE ACTIVE POWER IN MW")
        {
            double q = loadptr->get_nominal_constant_impedance_load_in_MVA().imag();
            return loadptr->set_nominal_constant_impedance_load_in_MVA(complex<double>(value, q));
        }

        if(PARAMETER_NAME=="QZ0_MVAR" or PARAMETER_NAME=="NOMINAL CONSTANT CURRENT REACTIVE POWER IN MVAR")
        {
            double p = loadptr->get_nominal_constant_impedance_load_in_MVA().real();
            return loadptr->set_nominal_constant_impedance_load_in_MVA(complex<double>(p, value));
        }

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_load_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
}

void api_set_load_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_load_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS")
            return loadptr->get_status();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_load_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_load_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LOAD* loadptr = psdb.get_load(did);
    if(loadptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS")
            return loadptr->set_status(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}
