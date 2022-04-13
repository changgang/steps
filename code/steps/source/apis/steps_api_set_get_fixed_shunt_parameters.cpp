#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_fixed_shunt_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_fixed_shunt_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

double api_get_fixed_shunt_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P0_MW" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN MW")
            return shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA().real();

        if(PARAMETER_NAME=="Q0_MVAR" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN MVAR")
            return shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA().imag();

        if(PARAMETER_NAME=="R_PU" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN PU")
            return shuntptr->get_nominal_impedance_shunt_in_pu().real();

        if(PARAMETER_NAME=="X_PU" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN PU")
            return shuntptr->get_nominal_impedance_shunt_in_pu().imag();

        if(PARAMETER_NAME=="G_PU" or PARAMETER_NAME=="NOMINAL CONDUCTANCE SHUNT IN PU")
            return shuntptr->get_nominal_admittance_shunt_in_pu().real();

        if(PARAMETER_NAME=="B_PU" or PARAMETER_NAME=="NOMINAL SUSCEPTANCE SHUNT IN PU")
            return shuntptr->get_nominal_admittance_shunt_in_pu().imag();

        if(PARAMETER_NAME=="G0_PU" or PARAMETER_NAME=="ZERO SEQUENCE CONDUCTANCE SHUNT IN PU")
            return shuntptr->get_nominal_zero_sequence_admittance_shunt_in_pu().real();
        if(PARAMETER_NAME=="B0_PU" or PARAMETER_NAME=="ZERO SEQUENCE SUSCEPTANCE SHUNT IN PU")
            return shuntptr->get_nominal_zero_sequence_admittance_shunt_in_pu().imag();

        if(PARAMETER_NAME=="I1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN PU")
            return abs(shuntptr->get_positive_sequence_complex_current_in_pu());
        if(PARAMETER_NAME=="I2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN PU")
            return abs(shuntptr->get_positive_sequence_complex_current_in_pu());
        if(PARAMETER_NAME=="I0_PU" or PARAMETER_NAME=="ZERo SEQUENCE CURRENT IN PU")
            return abs(shuntptr->get_positive_sequence_complex_current_in_pu());
        if(PARAMETER_NAME=="I1_KA" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN KA")
            return abs(shuntptr->get_positive_sequence_complex_current_in_kA());
        if(PARAMETER_NAME=="I2_KA" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN KA")
            return abs(shuntptr->get_positive_sequence_complex_current_in_kA());
        if(PARAMETER_NAME=="I0_KA" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN KA")
            return abs(shuntptr->get_positive_sequence_complex_current_in_kA());

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_fixed_shunt_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="P0_MW" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN MW")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(value, S.imag()));
        }

        if(PARAMETER_NAME=="Q0_MVAR" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN MVAR")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(S.real(), value));
        }

        if(PARAMETER_NAME=="R_PU" or PARAMETER_NAME=="NOMINAL RESISTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            if(S==0.0)
            {
                if(value==0.0)
                    return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(0.0, 0.0));
                else
                    return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(1.0/value*sbase, 0.0));
            }
            else
            {
                complex<double> Y, Z;
                Y = S/sbase;
                Z = 1.0/Y;
                Z = complex<double>(value, Z.imag());
                Y = 1.0/Z;
                S = Y*sbase;
                return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(S);
            }
        }

        if(PARAMETER_NAME=="X_PU" or PARAMETER_NAME=="NOMINAL REACTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            if(S==0.0)
            {
                if(value==0.0)
                    return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(0.0, 0.0));
                else
                    return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(0.0, 1.0/value*sbase));
            }
            else
            {
                complex<double> Y, Z;
                Y = S/sbase;
                Z = 1.0/Y;
                Z = complex<double>(Z.real(),value);
                Y = 1.0/Z;
                S = Y*sbase;
                return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(S);
            }
        }

        if(PARAMETER_NAME=="G_PU" or PARAMETER_NAME=="NOMINAL CONDUCTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(value*sbase, S.imag()));
        }

        if(PARAMETER_NAME=="B_PU" or PARAMETER_NAME=="NOMINAL SUSCEPTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_positive_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(S.real(), -value*sbase));
        }

        if(PARAMETER_NAME=="G0_PU" or PARAMETER_NAME=="ZERO SEQUENCE CONDUCTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_zero_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_zero_sequence_impedance_shunt_in_MVA(complex<double>(value*sbase, S.imag()));
        }

        if(PARAMETER_NAME=="B0_PU" or PARAMETER_NAME=="ZERO SEQUENCE SUSCEPTANCE SHUNT IN PU")
        {
            complex<double> S = shuntptr->get_nominal_zero_sequence_impedance_shunt_in_MVA();
            return shuntptr->set_nominal_zero_sequence_impedance_shunt_in_MVA(complex<double>(S.real(), -value*sbase));
        }

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_fixed_shunt_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (shuntptr->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}
		if (PARAMETER_NAME == "NAME")
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (shuntptr->get_name()).c_str());
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

void api_set_fixed_shunt_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
    if(shuntptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="ID" or PARAMETER_NAME=="IDENTIFIER")
            return shuntptr->set_identifier(value);
        if(PARAMETER_NAME=="NAME")
            return shuntptr->set_name(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_fixed_shunt_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
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

void api_set_fixed_shunt_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    FIXED_SHUNT* shuntptr = psdb.get_fixed_shunt(did);
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
