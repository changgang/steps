#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_fixed_shunt_integer_data(size_t bus, char* identifier, char* parameter_name)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
            return shuntptr->get_shunt_bus();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_fixed_shunt_integer_data(size_t bus, char* identifier, char* parameter_name, int value)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
            return shuntptr->set_shunt_bus(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}


double api_get_fixed_shunt_float_data(size_t bus, char* identifier, char* parameter_name)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P0_MW" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN MW")
            return shuntptr->get_nominal_impedance_shunt_in_MVA().real();

        if(PARAMETER_NAME=="Q0_MVAR" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN MVAR")
            return shuntptr->get_nominal_impedance_shunt_in_MVA().imag();

        if(PARAMETER_NAME=="R_PU" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN PU")
            return shuntptr->get_nominal_impedance_shunt_in_pu().real();

        if(PARAMETER_NAME=="X_PU" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN PU")
            return shuntptr->get_nominal_impedance_shunt_in_pu().imag();

        if(PARAMETER_NAME=="G_PU" or PARAMETER_NAME=="NOMINAL CONDUCTANCE SHUNT IN PU")
            return shuntptr->get_nominal_admittance_shunt_in_pu().real();

        if(PARAMETER_NAME=="B_PU" or PARAMETER_NAME=="NOMINAL SUSCEPTANCE SHUNT IN PU")
            return shuntptr->get_nominal_admittance_shunt_in_pu().imag();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_fixed_shunt_float_data(size_t bus, char* identifier, char* parameter_name, double value)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    double sbase = psdb->get_system_base_power_in_MVA();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P0_MW" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN MW")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(value, S.imag()));
        }

        if(PARAMETER_NAME=="Q0_MVAR" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN MVAR")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(S.real(), value));
        }

        if(PARAMETER_NAME=="R_PU" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            if(value==0.0)
                return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(INFINITE_THRESHOLD, S.imag()));
            else
                return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(1.0/value*sbase, S.imag()));
        }

        if(PARAMETER_NAME=="X_PU" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            if(value==0.0)
                return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(S.real(), INFINITE_THRESHOLD));
            else
                return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(S.real(), 1.0/value*sbase));
        }

        if(PARAMETER_NAME=="G_PU" or PARAMETER_NAME=="NOMINAL CONDUCTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(value*sbase, S.imag()));
        }

        if(PARAMETER_NAME=="B_PU" or PARAMETER_NAME=="NOMINAL SUSCEPTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_impedance_shunt_in_MVA(complex<double>(S.real(), -value*sbase));
        }

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_fixed_shunt_string_data(size_t bus, char* identifier, char* parameter_name)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

	snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
		{
			snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (shuntptr->get_identifier()).c_str());
			return STEPS::steps_char_buffer;
		}

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return STEPS::steps_char_buffer;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return STEPS::steps_char_buffer;
    }
}


void api_set_fixed_shunt_string_data(size_t bus, char* identifier, char* parameter_name, char* value)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="ID" or PARAMETER_NAME=="IDENTIFIER")
            return shuntptr->set_identifier(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_fixed_shunt_boolean_data(size_t bus, char* identifier, char* parameter_name)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS")
            return shuntptr->get_status();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}


void api_set_fixed_shunt_boolean_data(size_t bus, char* identifier, char* parameter_name, bool value)
{
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    FIXED_SHUNT* shuntptr = psdb->get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS")
            return shuntptr->set_status(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}
