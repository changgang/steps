#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_hvdc_integer_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC" and SIDE!="RECTIFIER" and SIDE!="INVERTER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return 0;
        }

        if(SIDE=="HVDC")
        {
            if(PARAMETER_NAME=="NPOLE" or PARAMETER_NAME=="POLE NUMBER")
            {
                HVDC_POLE poles = hvdcptr->get_number_of_poles();
                if(poles==SINGLE_POLE)
                    return 1;
                else
                    return 2;
            }

            if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
                return hvdcptr->get_converter_bus(hvdcptr->get_meter_end());

			if (PARAMETER_NAME == "PSIDE" or PARAMETER_NAME == "SIDE TO HOLD POWER")
				return (hvdcptr->get_side_to_hold_dc_power() == RECTIFIER) ? 0 : 1;

			if (PARAMETER_NAME == "RMODE" or PARAMETER_NAME == "RECTIFIER CONTROL MODE")
				return (hvdcptr->get_converter_operation_mode(RECTIFIER)== RECTIFIER_CONSTANT_POWER) ? 1 : 2;
        }
        else
        {
            HVDC_CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

            if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
                return hvdcptr->get_converter_bus(con_side);

			if (PARAMETER_NAME == "NBRIDGE" or PARAMETER_NAME == "NUMBER OF BRIDGE")
				return hvdcptr->get_converter_number_of_bridge(con_side);

            if(PARAMETER_NAME=="NTAP" or PARAMETER_NAME=="NUMBER OF TAPS")
                return hvdcptr->get_converter_transformer_number_of_taps(con_side);
        }

        show_parameter_not_supported_for_device_with_api(COMBINED_PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_hvdc_integer_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, int value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC" and SIDE!="RECTIFIER" and SIDE!="INVERTER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(SIDE=="HVDC")
        {
            if(PARAMETER_NAME=="NPOLE" or PARAMETER_NAME=="POLE NUMBER")
            {
                HVDC_POLE poles = (value==1?SINGLE_POLE:DOUBLE_POLE);
                return hvdcptr->set_number_of_poles(poles);
            }

            if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
                return hvdcptr->set_meter_end((size_t(value)==hvdcptr->get_converter_bus(RECTIFIER)?RECTIFIER:INVERTER));

            if(PARAMETER_NAME=="PSIDE" or PARAMETER_NAME=="SIDE TO HOLD POWER")
                return hvdcptr->set_side_to_hold_power(value==0?RECTIFIER:INVERTER);

			if (PARAMETER_NAME == "RMODE" or PARAMETER_NAME == "RECTIFIER CONTROL MODE")
				return (hvdcptr->set_converter_operation_mode(RECTIFIER, (value == 1 ? RECTIFIER_CONSTANT_POWER : RECTIFIER_CONSTANT_CURRENT)));
        }
        else
        {
            HVDC_CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

            if(PARAMETER_NAME=="BUS" or PARAMETER_NAME=="BUS NUMBER")
                return hvdcptr->set_converter_bus(con_side, value);

            if(PARAMETER_NAME=="NBRIDGE" or PARAMETER_NAME=="NUMBER OF BRIDGE")
                return hvdcptr->set_converter_number_of_bridge(con_side, value);

            if(PARAMETER_NAME=="NTAP" or PARAMETER_NAME=="NUMBER OF TAPS")
                return hvdcptr->set_converter_transformer_number_of_taps(con_side, value);
        }

        show_parameter_not_supported_for_device_with_api(COMBINED_PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}

double api_get_hvdc_float_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC" and SIDE!="RECTIFIER" and SIDE!="INVERTER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return 0.0;
        }

        if(SIDE=="HVDC")
        {
            if(PARAMETER_NAME=="R_OHM" or PARAMETER_NAME=="LINE R IN OHM")
                return hvdcptr->get_line_resistance_in_ohm();

            if(PARAMETER_NAME=="L_MH" or PARAMETER_NAME=="LINE L IN MH")
                return hvdcptr->get_line_inductance_in_mH();

            if(PARAMETER_NAME=="C_UF" or PARAMETER_NAME=="LINE C IN UF")
                return hvdcptr->get_line_capacitance_in_uF();

            if(PARAMETER_NAME=="PDCN_MW" or PARAMETER_NAME=="NOMINAL DC POWER PER POLE IN MW")
                return hvdcptr->get_nominal_dc_power_per_pole_in_MW();

            if(PARAMETER_NAME=="IDCN_KA" or PARAMETER_NAME=="NOMINAL DC CURRENT PER POLE IN KA")
                return hvdcptr->get_nominal_dc_current_per_pole_in_kA();

            if(PARAMETER_NAME=="VDCN_KV" or PARAMETER_NAME=="NOMINAL DC VOLTAGE PER POLE IN KV")
                return hvdcptr->get_nominal_dc_voltage_per_pole_in_kV();

            if(PARAMETER_NAME=="RCOMP_OHM" or PARAMETER_NAME=="COMPENSATING RESISTANCE TO HOLD DC VOLTAGE IN OHM")
                return hvdcptr->get_compensating_resistance_to_hold_dc_voltage_in_ohm();

            if(PARAMETER_NAME=="VTHRESHOLD_KV" or PARAMETER_NAME=="THRESHOLD DC VOLTAGE FOR CONSTANT POWER AND CONSTANT CURRENT MODE IN KV")
                return hvdcptr->get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV();

            if(PARAMETER_NAME=="MARGIN" or PARAMETER_NAME=="CURRENT POWER MARGIN")
                return hvdcptr->get_current_power_margin();
        }
        else
        {

            if(SIDE=="RECTIFIER")
            {
                if(PARAMETER_NAME=="PDC_CMD_MW" or PARAMETER_NAME=="NOMINAL DC POWER COMMAND IN MW")
                    return hvdcptr->get_rectifier_nominal_dc_power_command_in_MW();

                if(PARAMETER_NAME=="IDC_CMD_KA" or PARAMETER_NAME=="NOMINAL DC CURRENT COMMAND IN KA")
                    return hvdcptr->get_rectifier_nominal_dc_current_command_in_kA();

                if(PARAMETER_NAME=="ALPHA_MAX_DEG" or PARAMETER_NAME=="MAX ALPHA IN DEG")
                    return hvdcptr->get_converter_max_alpha_or_gamma_in_deg(RECTIFIER);

                if(PARAMETER_NAME=="ALPHA_MIN_DEG" or PARAMETER_NAME=="MIN ALPHA IN DEG")
                    return hvdcptr->get_converter_min_alpha_or_gamma_in_deg(RECTIFIER);

                if(PARAMETER_NAME=="ALPHA_DEG" or PARAMETER_NAME=="ALPHA IN DEG")
                    return hvdcptr->get_converter_alpha_or_gamma_in_deg(RECTIFIER);
            }
            if(SIDE=="INVERTER")
            {
                if(PARAMETER_NAME=="VDC_CMD_KV" or PARAMETER_NAME=="NOMINAL DC VOLTAGE COMMAND IN KV")
                    return hvdcptr->get_inverter_nominal_dc_voltage_command_in_kV();

                if(PARAMETER_NAME=="GAMMA_MAX_DEG" or PARAMETER_NAME=="MAX GAMMA IN DEG")
                    return hvdcptr->get_converter_max_alpha_or_gamma_in_deg(INVERTER);

                if(PARAMETER_NAME=="GAMMA_MIN_DEG" or PARAMETER_NAME=="MIN GAMMA IN DEG")
                    return hvdcptr->get_converter_min_alpha_or_gamma_in_deg(INVERTER);

                if(PARAMETER_NAME=="GAMMA_DEG" or PARAMETER_NAME=="GAMMA IN DEG")
                    return hvdcptr->get_converter_alpha_or_gamma_in_deg(INVERTER);
            }

            HVDC_CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

            if(PARAMETER_NAME=="RS_OHM" or PARAMETER_NAME=="LINE SMOOTING R IN OHM")
                return hvdcptr->get_line_smooting_resistance_in_ohm(con_side);

            if(PARAMETER_NAME=="LS_MH" or PARAMETER_NAME=="LINE SMOOTING L IN MH")
                return hvdcptr->get_line_smooting_inductance_in_mH(con_side);

            if(PARAMETER_NAME=="VDROP_KV" or PARAMETER_NAME=="VOLTAGE DROP PER BRIDGE IN KV")
                return hvdcptr->get_converter_voltage_drop_per_bridge_in_kV(con_side);

            if(PARAMETER_NAME=="VBASE_GRID_KV" or PARAMETER_NAME=="TRANSFORMER GRID SIDE BASE V IN KV")
                return hvdcptr->get_converter_transformer_grid_side_base_voltage_in_kV(con_side);

            if(PARAMETER_NAME=="VBASE_CONV_KV" or PARAMETER_NAME=="TRANSFORMER CONVERTER SIDE BASE V IN KV")
                return hvdcptr->get_converter_transformer_converter_side_base_voltage_in_kV(con_side);

            if(PARAMETER_NAME=="RT_OHM" or PARAMETER_NAME=="TRANSFORMER R IN OHM")
                return hvdcptr->get_converter_transformer_impedance_in_ohm(con_side).real();

            if(PARAMETER_NAME=="XT_OHM" or PARAMETER_NAME=="TRANSFORMER X IN OHM")
                return hvdcptr->get_converter_transformer_impedance_in_ohm(con_side).imag();

            if(PARAMETER_NAME=="GT_S" or PARAMETER_NAME=="TRANSFORMER G IN SIEMENS")
                return hvdcptr->get_converter_transformer_admittance_in_siemens(con_side).real();

            if(PARAMETER_NAME=="BT_S" or PARAMETER_NAME=="TRANSFORMER B IN SIEMENS")
                return hvdcptr->get_converter_transformer_admittance_in_siemens(con_side).imag();

            if(PARAMETER_NAME=="TAP_MAX_PU" or PARAMETER_NAME=="TRANSFORMER MAX TAP IN PU")
                return hvdcptr->get_converter_transformer_max_tap_in_pu(con_side);

            if(PARAMETER_NAME=="TAP_MIN_PU" or PARAMETER_NAME=="TRANSFORMER MIN TAP IN PU")
                return hvdcptr->get_converter_transformer_min_tap_in_pu(con_side);

            if(PARAMETER_NAME=="TAP_PU" or PARAMETER_NAME=="TRANSFORMER TAP IN PU")
                return hvdcptr->get_converter_transformer_tap_in_pu(con_side);

            if(PARAMETER_NAME=="MU_DEG" or PARAMETER_NAME=="COMMUTATING OVERLAP ANGLE IN DEG")
                return hvdcptr->get_converter_commutating_overlap_angle_in_deg(con_side);

            if(PARAMETER_NAME=="VDC_KV" or PARAMETER_NAME=="DC VOLTAGE IN KV")
                return hvdcptr->get_converter_dc_voltage_in_kV(con_side);

            if(PARAMETER_NAME=="IDC_KA" or PARAMETER_NAME=="DC CURRENT IN KA")
                return hvdcptr->get_converter_dc_current_in_kA(con_side);

            if(PARAMETER_NAME=="PDC_MW" or PARAMETER_NAME=="DC POWER IN MW")
                return hvdcptr->get_converter_dc_power_in_MW(con_side);

            if(PARAMETER_NAME=="PAC_MW" or PARAMETER_NAME=="AC ACTIVE POWER IN MW")
                return hvdcptr->get_converter_ac_active_power_in_MW(con_side);

            if(PARAMETER_NAME=="QAC_MVAR" or PARAMETER_NAME=="AC REACTIVE POWER IN MVAR")
                return hvdcptr->get_converter_ac_reactive_power_in_MVar(con_side);

            if(PARAMETER_NAME=="SAC_MVA" or PARAMETER_NAME=="AC APPARENT POWER IN MVA")
                return hvdcptr->get_converter_ac_apparent_power_in_MVA(con_side);

            if(PARAMETER_NAME=="PF_AC" or PARAMETER_NAME=="AC POWER FACTOR")
                return hvdcptr->get_converter_ac_power_factor(con_side);

            if(PARAMETER_NAME=="IAC_KA" or PARAMETER_NAME=="AC CURRENT IN KA")
                return hvdcptr->get_converter_ac_current_in_kA(con_side);

        }

        show_parameter_not_supported_for_device_with_api(COMBINED_PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_hvdc_float_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, double value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC" and SIDE!="RECTIFIER" and SIDE!="INVERTER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(SIDE=="HVDC")
        {
            if(PARAMETER_NAME=="R_OHM" or PARAMETER_NAME=="LINE R IN OHM")
                return hvdcptr->set_line_resistance_in_ohm(value);

            if(PARAMETER_NAME=="L_MH" or PARAMETER_NAME=="LINE L IN MH")
                return hvdcptr->set_line_inductance_in_mH(value);

            if(PARAMETER_NAME=="C_UF" or PARAMETER_NAME=="LINE C IN UF")
                return hvdcptr->set_line_capacitance_in_uF(value);

            if(PARAMETER_NAME=="PDCN_MW" or PARAMETER_NAME=="NOMINAL DC POWER PER POLE IN MW")
                return hvdcptr->set_nominal_dc_power_per_pole_in_MW(value);

            if(PARAMETER_NAME=="IDCN_KA" or PARAMETER_NAME=="NOMINAL DC CURRENT PER POLE IN KA")
                return hvdcptr->set_nominal_dc_current_per_pole_in_kA(value);

            if(PARAMETER_NAME=="VDCN_KV" or PARAMETER_NAME=="NOMINAL DC VOLTAGE PER POLE IN KV")
                return hvdcptr->set_nominal_dc_voltage_per_pole_in_kV(value);

            if(PARAMETER_NAME=="RCOMP_OHM" or PARAMETER_NAME=="COMPENSATING RESISTANCE TO HOLD DC VOLTAGE IN OHM")
                return hvdcptr->set_compensating_resistance_to_hold_dc_voltage_in_ohm(value);

            if(PARAMETER_NAME=="VTHRESHOLD_KV" or PARAMETER_NAME=="THRESHOLD DC VOLTAGE FOR CONSTANT POWER AND CONSTANT CURRENT MODE IN KV")
                return hvdcptr->set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(value);

            if(PARAMETER_NAME=="MARGIN" or PARAMETER_NAME=="CURRENT POWER MARGIN")
                return hvdcptr->set_current_power_margin(value);
        }
        else
        {

            if(SIDE=="RECTIFIER")
            {
                if(PARAMETER_NAME=="ALPHA_MAX_DEG" or PARAMETER_NAME=="MAX ALPHA IN DEG")
                    return hvdcptr->set_converter_max_alpha_or_gamma_in_deg(RECTIFIER,value);

                if(PARAMETER_NAME=="ALPHA_MIN_DEG" or PARAMETER_NAME=="MIN ALPHA IN DEG")
                    return hvdcptr->set_converter_min_alpha_or_gamma_in_deg(RECTIFIER,value);

                if(PARAMETER_NAME=="ALPHA_DEG" or PARAMETER_NAME=="ALPHA IN DEG")
                    return hvdcptr->set_converter_alpha_or_gamma_in_deg(RECTIFIER,value);
            }
            if(SIDE=="INVERTER")
            {
                if(PARAMETER_NAME=="GAMMA_MAX_DEG" or PARAMETER_NAME=="MAX GAMMA IN DEG")
                    return hvdcptr->set_converter_max_alpha_or_gamma_in_deg(INVERTER,value);

                if(PARAMETER_NAME=="GAMMA_MIN_DEG" or PARAMETER_NAME=="MIN GAMMA IN DEG")
                    return hvdcptr->set_converter_min_alpha_or_gamma_in_deg(INVERTER,value);

                if(PARAMETER_NAME=="GAMMA_DEG" or PARAMETER_NAME=="GAMMA IN DEG")
                    return hvdcptr->set_converter_alpha_or_gamma_in_deg(INVERTER,value);
            }

            HVDC_CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

            if(PARAMETER_NAME=="RS_OHM" or PARAMETER_NAME=="LINE SMOOTING R IN OHM")
                return hvdcptr->set_line_smooting_resistance_in_ohm(con_side,value);

            if(PARAMETER_NAME=="LS_MH" or PARAMETER_NAME=="LINE SMOOTING L IN MH")
                return hvdcptr->set_line_smooting_inductance_in_mH(con_side,value);

            if(PARAMETER_NAME=="VDROP_KV" or PARAMETER_NAME=="VOLTAGE DROP PER BRIDGE IN KV")
                return hvdcptr->set_converter_voltage_drop_per_bridge_in_kV(con_side,value);

            if(PARAMETER_NAME=="VBASE_GRID_KV" or PARAMETER_NAME=="TRANSFORMER GRID SIDE BASE V IN KV")
                return hvdcptr->set_converter_transformer_grid_side_base_voltage_in_kV(con_side,value);

            if(PARAMETER_NAME=="VBASE_CONV_KV" or PARAMETER_NAME=="TRANSFORMER CONVERTER SIDE BASE V IN KV")
                return hvdcptr->set_converter_transformer_converter_side_base_voltage_in_kV(con_side,value);

            if(PARAMETER_NAME=="RT_OHM" or PARAMETER_NAME=="TRANSFORMER R IN OHM")
            {
                double x = hvdcptr->get_converter_transformer_impedance_in_ohm(con_side).imag();
                return hvdcptr->set_converter_transformer_impedance_in_ohm(con_side, complex<double>(value,x));
            }

            if(PARAMETER_NAME=="XT_OHM" or PARAMETER_NAME=="TRANSFORMER X IN OHM")
            {
                double r = hvdcptr->get_converter_transformer_impedance_in_ohm(con_side).real();
                return hvdcptr->set_converter_transformer_impedance_in_ohm(con_side,complex<double>(r,value));
            }

            if(PARAMETER_NAME=="GT_S" or PARAMETER_NAME=="TRANSFORMER G IN SIEMENS")
            {
                double b = hvdcptr->get_converter_transformer_admittance_in_siemens(con_side).imag();
                return hvdcptr->set_converter_transformer_admittance_in_siemens(con_side,complex<double>(value,b));
            }

            if(PARAMETER_NAME=="BT_S" or PARAMETER_NAME=="TRANSFORMER B IN SIEMENS")
            {
                double g = hvdcptr->get_converter_transformer_admittance_in_siemens(con_side).real();
                return hvdcptr->set_converter_transformer_admittance_in_siemens(con_side,complex<double>(g,value));
            }

            if(PARAMETER_NAME=="TAP_MAX_PU" or PARAMETER_NAME=="TRANSFORMER MAX TAP IN PU")
                return hvdcptr->set_converter_transformer_max_tap_in_pu(con_side,value);

            if(PARAMETER_NAME=="TAP_MIN_PU" or PARAMETER_NAME=="TRANSFORMER MIN TAP IN PU")
                return hvdcptr->set_converter_transformer_min_tap_in_pu(con_side,value);

            if(PARAMETER_NAME=="TAP_PU" or PARAMETER_NAME=="TRANSFORMER TAP IN PU")
                return hvdcptr->set_converter_transformer_tap_in_pu(con_side,value);

            if(PARAMETER_NAME=="VDC_KV" or PARAMETER_NAME=="DC VOLTAGE IN KV")
                return hvdcptr->set_converter_dc_voltage_in_kV(con_side,value);

            if(PARAMETER_NAME=="IDC_KA" or PARAMETER_NAME=="DC CURRENT IN KA")
                return hvdcptr->set_converter_dc_current_in_kA(con_side,value);
        }
        show_parameter_not_supported_for_device_with_api(COMBINED_PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_hvdc_string_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

	snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return toolkit.steps_char_buffer;
        }

		if (PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (hvdcptr->get_identifier()).c_str());
			return toolkit.steps_char_buffer;
		}

		if (PARAMETER_NAME == "NAME")
		{
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (hvdcptr->get_name()).c_str());
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

void api_set_hvdc_string_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, char* value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE!="HVDC")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(PARAMETER_NAME=="ID" or PARAMETER_NAME=="IDENTIFIER")
            return hvdcptr->set_identifier(value);

        if(PARAMETER_NAME=="NAME")
            return hvdcptr->set_name(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_hvdc_boolean_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE!="HVDC")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return false;
        }

        if(PARAMETER_NAME=="STATUS")
            return hvdcptr->get_status();

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_hvdc_boolean_data(size_t ibus, size_t jbus, char* identifier, char* side, char* parameter_name, bool value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC* hvdcptr = psdb.get_hvdc(did);
    if(hvdcptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE!="HVDC")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(PARAMETER_NAME=="STATUS")
            return hvdcptr->set_status(value);

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}
