#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_line_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="BUS_SEND" or PARAMETER_NAME=="SENDING SIDE BUS NUMBER")
            return lineptr->get_sending_side_bus();

        if(PARAMETER_NAME=="BUS_RECV" or PARAMETER_NAME=="RECEIVING SIDE BUS NUMBER")
            return lineptr->get_receiving_side_bus();

        if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
            return lineptr->get_meter_end_bus();


        if(PARAMETER_NAME=="OWNER1" or PARAMETER_NAME=="OWNER2" or PARAMETER_NAME=="OWNER3" or PARAMETER_NAME=="OWNER4")
            return get_owner_of_device(lineptr, PARAMETER_NAME);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_line_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="BUS_SEND" or PARAMETER_NAME=="SENDING SIDE BUS NUMBER")
            return lineptr->set_sending_side_bus(value);

        if(PARAMETER_NAME=="BUS_RECV" or PARAMETER_NAME=="RECEIVING SIDE BUS NUMBER")
            return lineptr->set_receiving_side_bus(value);

        if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
            return lineptr->set_meter_end_bus(value);

        if(PARAMETER_NAME=="OWNER1" or PARAMETER_NAME=="OWNER2" or PARAMETER_NAME=="OWNER3" or PARAMETER_NAME=="OWNER4")
            return set_owner_of_device(lineptr, PARAMETER_NAME, value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

double api_get_line_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="R1_PU" or PARAMETER_NAME=="LINE R1 IN PU")
            return lineptr->get_line_positive_sequence_z_in_pu().real();

        if(PARAMETER_NAME=="X1_PU" or PARAMETER_NAME=="LINE X1 IN PU")
            return lineptr->get_line_positive_sequence_z_in_pu().imag();

        if(PARAMETER_NAME=="G1_PU" or PARAMETER_NAME=="LINE G1 IN PU")
            return lineptr->get_line_positive_sequence_y_in_pu().real();

        if(PARAMETER_NAME=="B1_PU" or PARAMETER_NAME=="LINE B1 IN PU")
            return lineptr->get_line_positive_sequence_y_in_pu().imag();

        if(PARAMETER_NAME=="R0_PU" or PARAMETER_NAME=="LINE R0 IN PU")
            return lineptr->get_line_zero_sequence_z_in_pu().real();

        if(PARAMETER_NAME=="X0_PU" or PARAMETER_NAME=="LINE X0 IN PU")
            return lineptr->get_line_zero_sequence_z_in_pu().imag();

        if(PARAMETER_NAME=="G0_PU" or PARAMETER_NAME=="LINE G0 IN PU")
            return lineptr->get_line_zero_sequence_y_in_pu().real();

        if(PARAMETER_NAME=="B0_PU" or PARAMETER_NAME=="LINE B0 IN PU")
            return lineptr->get_line_zero_sequence_y_in_pu().imag();

        if(PARAMETER_NAME=="SG1_PU" or PARAMETER_NAME=="SENDING SIDE SHUNT G1 IN PU")
            return lineptr->get_shunt_positive_sequence_y_at_sending_side_in_pu().real();

        if(PARAMETER_NAME=="SB1_PU" or PARAMETER_NAME=="SENDING SIDE B1 IN PU")
            return lineptr->get_shunt_positive_sequence_y_at_sending_side_in_pu().imag();

        if(PARAMETER_NAME=="RG1_PU" or PARAMETER_NAME=="RECEIVING SIDE G1 IN PU")
            return lineptr->get_shunt_positive_sequence_y_at_receiving_side_in_pu().real();

        if(PARAMETER_NAME=="RB1_PU" or PARAMETER_NAME=="RECEIVING SIDE B1 IN PU")
            return lineptr->get_shunt_positive_sequence_y_at_receiving_side_in_pu().imag();

        if(PARAMETER_NAME=="SG0_PU" or PARAMETER_NAME=="SENDING SIDE SHUNT G0 IN PU")
            return lineptr->get_shunt_zero_sequence_y_at_sending_side_in_pu().real();

        if(PARAMETER_NAME=="SB0_PU" or PARAMETER_NAME=="SENDING SIDE B0 IN PU")
            return lineptr->get_shunt_zero_sequence_y_at_sending_side_in_pu().imag();

        if(PARAMETER_NAME=="RG0_PU" or PARAMETER_NAME=="RECEIVING SIDE G0 IN PU")
            return lineptr->get_shunt_zero_sequence_y_at_receiving_side_in_pu().real();

        if(PARAMETER_NAME=="RB0_PU" or PARAMETER_NAME=="RECEIVING SIDE B0 IN PU")
            return lineptr->get_shunt_zero_sequence_y_at_receiving_side_in_pu().imag();

        if(PARAMETER_NAME=="RATE_A_MVA" or PARAMETER_NAME=="RATING A IN MVA")
            return lineptr->get_rating().get_rating_A_MVA();

        if(PARAMETER_NAME=="RATE_B_MVA" or PARAMETER_NAME=="RATING B IN MVA")
            return lineptr->get_rating().get_rating_B_MVA();

        if(PARAMETER_NAME=="RATE_C_MVA" or PARAMETER_NAME=="RATING C IN MVA")
            return lineptr->get_rating().get_rating_C_MVA();

        if(PARAMETER_NAME=="LENGTH" or PARAMETER_NAME=="LINE LENGTH")
            return lineptr->get_length();

        if(PARAMETER_NAME=="FRAC1" or PARAMETER_NAME=="FRAC2" or PARAMETER_NAME=="FRAC3" or PARAMETER_NAME=="FRAC4")
            return get_owner_fraction_of_device(lineptr, parameter_name);

        if(PARAMETER_NAME=="PSEND_MW")
            return lineptr->get_line_complex_power_at_sending_side_in_MVA().real();
        if(PARAMETER_NAME=="QSEND_MVAR")
            return lineptr->get_line_complex_power_at_sending_side_in_MVA().imag();

        if(PARAMETER_NAME=="PRECV_MW")
            return lineptr->get_line_complex_power_at_receiving_side_in_MVA().real();
        if(PARAMETER_NAME=="QRECV_MVAR")
            return lineptr->get_line_complex_power_at_receiving_side_in_MVA().imag();

        if(PARAMETER_NAME=="ISEND_KA")
            return abs(lineptr->get_line_complex_current_at_sending_side_in_kA());
        if(PARAMETER_NAME=="IRECV_KA")
            return abs(lineptr->get_line_complex_current_at_receiving_side_in_kA());

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_line_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="R1_PU" or PARAMETER_NAME=="LINE R1 IN PU")
        {
            complex<double> Z = lineptr->get_line_positive_sequence_z_in_pu();
            return lineptr->set_line_positive_sequence_z_in_pu(complex<double>(value, Z.imag()));
        }

        if(PARAMETER_NAME=="X1_PU" or PARAMETER_NAME=="LINE X1 IN PU")
        {
            complex<double> Z = lineptr->get_line_positive_sequence_z_in_pu();
            return lineptr->set_line_positive_sequence_z_in_pu(complex<double>(Z.real(), value));
        }

        if(PARAMETER_NAME=="G1_PU" or PARAMETER_NAME=="LINE G1 IN PU")
        {
            complex<double> Y = lineptr->get_line_positive_sequence_y_in_pu();
            return lineptr->set_line_positive_sequence_y_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="B1_PU" or PARAMETER_NAME=="LINE B1 IN PU")
        {
            complex<double> Y = lineptr->get_line_positive_sequence_y_in_pu();
            return lineptr->set_line_positive_sequence_y_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="R0_PU" or PARAMETER_NAME=="LINE R0 IN PU")
        {
            complex<double> Z = lineptr->get_line_zero_sequence_z_in_pu();
            return lineptr->set_line_zero_sequence_z_in_pu(complex<double>(value, Z.imag()));
        }

        if(PARAMETER_NAME=="X0_PU" or PARAMETER_NAME=="LINE X0 IN PU")
        {
            complex<double> Z = lineptr->get_line_zero_sequence_z_in_pu();
            return lineptr->set_line_zero_sequence_z_in_pu(complex<double>(Z.real(), value));
        }

        if(PARAMETER_NAME=="G0_PU" or PARAMETER_NAME=="LINE G0 IN PU")
        {
            complex<double> Y = lineptr->get_line_zero_sequence_y_in_pu();
            return lineptr->set_line_zero_sequence_y_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="B0_PU" or PARAMETER_NAME=="LINE B0 IN PU")
        {
            complex<double> Y = lineptr->get_line_zero_sequence_y_in_pu();
            return lineptr->set_line_zero_sequence_y_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="SG1_PU" or PARAMETER_NAME=="SENDING SIDE SHUNT G1 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_positive_sequence_y_at_sending_side_in_pu();
            return lineptr->set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="SB1_PU" or PARAMETER_NAME=="SENDING SIDE B1 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_positive_sequence_y_at_sending_side_in_pu();
            return lineptr->set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="RG1_PU" or PARAMETER_NAME=="RECEIVING SIDE G1 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_positive_sequence_y_at_receiving_side_in_pu();
            return lineptr->set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="RB1_PU" or PARAMETER_NAME=="RECEIVING SIDE B1 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_positive_sequence_y_at_receiving_side_in_pu();
            return lineptr->set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="SG0_PU" or PARAMETER_NAME=="SENDING SIDE SHUNT G0 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_zero_sequence_y_at_sending_side_in_pu();
            return lineptr->set_shunt_zero_sequence_y_at_sending_side_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="SB0_PU" or PARAMETER_NAME=="SENDING SIDE B0 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_zero_sequence_y_at_sending_side_in_pu();
            return lineptr->set_shunt_zero_sequence_y_at_sending_side_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="RG0_PU" or PARAMETER_NAME=="RECEIVING SIDE G0 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_zero_sequence_y_at_receiving_side_in_pu();
            return lineptr->set_shunt_zero_sequence_y_at_receiving_side_in_pu(complex<double>(value, Y.imag()));
        }

        if(PARAMETER_NAME=="RB0_PU" or PARAMETER_NAME=="RECEIVING SIDE B0 IN PU")
        {
            complex<double> Y = lineptr->get_shunt_zero_sequence_y_at_receiving_side_in_pu();
            return lineptr->set_shunt_zero_sequence_y_at_receiving_side_in_pu(complex<double>(Y.real(), value));
        }

        if(PARAMETER_NAME=="RATE_A_MVA" or PARAMETER_NAME=="RATING A IN MVA")
        {
            RATING rating = lineptr->get_rating();
            rating.set_rating_A_MVA(value);
            return lineptr->set_rating(rating);
        }

        if(PARAMETER_NAME=="RATE_B_MVA" or PARAMETER_NAME=="RATING B IN MVA")
        {
            RATING rating = lineptr->get_rating();
            rating.set_rating_B_MVA(value);
            return lineptr->set_rating(rating);
        }

        if(PARAMETER_NAME=="RATE_C_MVA" or PARAMETER_NAME=="RATING C IN MVA")
        {
            RATING rating = lineptr->get_rating();
            rating.set_rating_C_MVA(value);
            return lineptr->set_rating(rating);
        }

        if(PARAMETER_NAME=="LENGTH" or PARAMETER_NAME=="LINE LENGTH")
            return lineptr->set_length(value);

        if(PARAMETER_NAME=="FRAC1" or PARAMETER_NAME=="FRAC2" or PARAMETER_NAME=="FRAC3" or PARAMETER_NAME=="FRAC4")
            return set_owner_fraction_of_device(lineptr, PARAMETER_NAME, value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_line_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
		if (PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
		{
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (lineptr->get_identifier()).c_str());
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

void api_set_line_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);
        if(PARAMETER_NAME=="ID" or PARAMETER_NAME=="IDENTIFIER")
            return lineptr->set_identifier(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_line_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS_SEND" or PARAMETER_NAME=="SENDING SIDE BREAKER STATUS")
            return lineptr->get_sending_side_breaker_status();

        if(PARAMETER_NAME=="STATUS_RECV" or PARAMETER_NAME=="RECEIVING SIDE BREAKER STATUS")
            return lineptr->get_receiving_side_breaker_status();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_line_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* lineptr = psdb.get_line(did);
    if(lineptr!=NULL)
    {
        string PARAMETER_NAME = string2upper(parameter_name);

        if(PARAMETER_NAME=="STATUS_SEND" or PARAMETER_NAME=="SENDING SIDE BREAKER STATUS")
            return lineptr->set_sending_side_breaker_status(value);

        if(PARAMETER_NAME=="STATUS_RECV" or PARAMETER_NAME=="RECEIVING SIDE BREAKER STATUS")
            return lineptr->set_receiving_side_breaker_status(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}
