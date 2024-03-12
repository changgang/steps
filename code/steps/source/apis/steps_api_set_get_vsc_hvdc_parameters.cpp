#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_vsc_hvdc_integer_data(char* vsc_name, char* device_type, char* parameter_name, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "VSC NUMBER" or PARAMETER_NAME == "CONVERTER COUNT")
                return vsc_hvdc->get_converter_count();
            if(PARAMETER_NAME == "DC BUS NUMBER" or PARAMETER_NAME == "DC BUS COUNT")
                return vsc_hvdc->get_dc_bus_count();
            if(PARAMETER_NAME == "DC LINE NUMBER" or PARAMETER_NAME == "DC LINE COUNT")
                return vsc_hvdc->get_dc_line_count();
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "IBUS" or PARAMETER_NAME == "CONVERTER BUS NUMBER")
            {
                return vsc_hvdc->get_converter_ac_bus(index);
            }
            if(PARAMETER_NAME == "P MODE" or PARAMETER_NAME == "ACTIVE POWER MODE")
            {
                VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE p_mode = vsc_hvdc->get_converter_active_power_operation_mode(index);
                switch(p_mode)
                {
                    case VSC_DC_VOLTAGE_CONTORL:
                        return 1;
                    case VSC_AC_ACTIVE_POWER_CONTORL:
                        return 2;
                    case VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                        return 3;
                    case VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL:
                        return 4;
                    case VSC_AC_VOLTAGE_ANGLE_CONTROL:
                        return 5;
                    case VSC_INVALID_DC_CONTORL:
                        return 999;
                    default:
                        return 999;
                }
            }
            if(PARAMETER_NAME == "FLAG" or PARAMETER_NAME == "DC VOLTAGE CONTROL PRIORITY")
            {
                return vsc_hvdc->get_converter_dc_voltage_control_priority(index);
            }
            if(PARAMETER_NAME == "Q MODE" or PARAMETER_NAME == "REACTIVE POWER MODE")
            {
                VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE q_mode = vsc_hvdc->get_converter_reactive_power_operation_mode(index);
                switch(q_mode)
                {
                    case VSC_AC_VOLTAGE_CONTROL:
                      return 1;
                    case VSC_AC_REACTIVE_POWER_CONTROL:
                      return 2;
                    case VSC_INVALID_AC_CONTORL:
                      return 999;
                    default:
                      return 999;
                }
            }
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "DC BUS" or PARAMETER_NAME == "DC BUS NUMBER")
            {
                return vsc_hvdc->get_dc_bus_number(index);
            }
            if(PARAMETER_NAME == "VSC INDEX" or PARAMETER_NAME == "CONVERTER INDEX")
            {
                return vsc_hvdc->get_dc_bus_converter_index_with_dc_bus_index(index);
            }
            if(PARAMETER_NAME == "AREA" or PARAMETER_NAME == "DC BUS AREA")
            {
                return vsc_hvdc->get_dc_bus_area(index);
            }
            if(PARAMETER_NAME == "ZONE" or PARAMETER_NAME == "DC BUS ZONE")
            {
                return vsc_hvdc->get_dc_bus_zone(index);
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "BUS SEND" or PARAMETER_NAME == "SENDING SIDE BUS NUMBER")
                return vsc_hvdc->get_dc_line_sending_side_bus(index);
            if(PARAMETER_NAME == "BUS RECV" or PARAMETER_NAME == "RECEIVING SIDE BUS NUMBER")
                return vsc_hvdc->get_dc_line_receiving_side_bus(index);
            if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
                return vsc_hvdc->get_dc_line_meter_end(index);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_vsc_hvdc_integer_data(char* vsc_name, char* device_type, char* parameter_name, int value, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "VSC NUMBER" or PARAMETER_NAME == "CONVERTER COUNT")
                return vsc_hvdc->set_converter_count(value);
            if(PARAMETER_NAME == "DC BUS NUMBER" or PARAMETER_NAME == "DC BUS COUNT")
                return vsc_hvdc->set_dc_bus_count(value);
            if(PARAMETER_NAME == "DC LINE NUMBER" or PARAMETER_NAME == "DC LINE COUNT")
                return vsc_hvdc->set_dc_line_count(value);
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "IBUS" or PARAMETER_NAME == "CONVERTER BUS NUMBER")
            {
                return vsc_hvdc->set_converter_ac_bus(index,value);
            }
            if(PARAMETER_NAME == "P MODE" or PARAMETER_NAME == "ACTIVE POWER MODE")
            {
                switch(value)
                {
                    case 1:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_DC_VOLTAGE_CONTORL);
                            break;
                        }
                    case 2:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_AC_ACTIVE_POWER_CONTORL);
                            break;
                        }
                    case 3:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL);
                            break;
                        }
                    case 4:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL);
                            break;
                        }
                    case 5:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_AC_VOLTAGE_ANGLE_CONTROL);
                            break;
                        }
                    case 999:
                        {
                            vsc_hvdc->set_converter_active_power_operation_mode(index, VSC_INVALID_DC_CONTORL);
                            break;
                        }
                    default:
                        break;
                }
                return;
            }
            if(PARAMETER_NAME == "FLAG" or PARAMETER_NAME == "DC VOLTAGE CONTROL PRIORITY")
            {
                return vsc_hvdc->set_converter_dc_voltage_control_priority(index, value);
            }
            if(PARAMETER_NAME == "Q MODE" or PARAMETER_NAME == "REACTIVE POWER MODE")
            {
                switch(value)
                {
                    case 1:
                        {
                          vsc_hvdc->set_converter_reactive_power_operation_mode(index, VSC_AC_VOLTAGE_CONTROL);
                          break;
                        }
                    case 2:
                        {
                          vsc_hvdc->set_converter_reactive_power_operation_mode(index, VSC_AC_REACTIVE_POWER_CONTROL);
                          break;
                        }
                    case 999:
                        {
                          vsc_hvdc->set_converter_reactive_power_operation_mode(index,VSC_INVALID_AC_CONTORL);
                          break;
                        }
                    default:
                      break;
                }
                return;
            }
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "DC BUS" or PARAMETER_NAME == "DC BUS NUMBER")
            {
                return vsc_hvdc->set_dc_bus_number(index,value);
            }
            if(PARAMETER_NAME == "VSC INDEX" or PARAMETER_NAME == "CONVERTER INDEX")
            {
                return vsc_hvdc->set_dc_bus_converter_index_with_dc_bus_index(index,value);
            }
            if(PARAMETER_NAME == "AREA" or PARAMETER_NAME == "DC BUS AREA")
            {
                return vsc_hvdc->set_dc_bus_area(index,value);
            }
            if(PARAMETER_NAME == "ZONE" or PARAMETER_NAME == "DC BUS ZONE")
            {
                return vsc_hvdc->set_dc_bus_zone(index,value);
            }
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "BUS SEND" or PARAMETER_NAME == "SENDING SIDE BUS NUMBER")
                return vsc_hvdc->set_dc_line_sending_side_bus(index,value);
            if(PARAMETER_NAME == "BUS RECV" or PARAMETER_NAME == "RECEIVING SIDE BUS NUMBER")
                return vsc_hvdc->set_dc_line_receiving_side_bus(index,value);
            if(PARAMETER_NAME=="BUS_METER" or PARAMETER_NAME=="METER END BUS NUMBER")
                return vsc_hvdc->set_dc_line_meter_end(index,value);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
    }

}

double api_get_vsc_hvdc_float_data(char* vsc_name, char* device_type, char* parameter_name, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "VDCBASE_KV" or PARAMETER_NAME == "DC BASE VOLTAGE IN KV")
                return vsc_hvdc->get_dc_network_base_voltage_in_kV();
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "PREF_MW" or PARAMETER_NAME == "NOMINAL AC ACTIVE POWER IN MW")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_AC_ACTIVE_POWER_CONTORL)
                {
                    return vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_AC_ACTIVE_POWER_CONTORL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "UDCREF_KV" or PARAMETER_NAME == "NOMINAL DC VOLTAGE IN KV")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_VOLTAGE_CONTORL)
                {
                    return vsc_hvdc->get_converter_nominal_dc_voltage_command_in_kV(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_VOLTAGE_CONTORL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "UDC_KV" or PARAMETER_NAME == "INITIAL DC VOLTAGE IN KV")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
                   vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->get_converter_initial_dc_voltage_reference_in_kV(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "IDC_KA" or PARAMETER_NAME == "INITIAL DC CURRENT IN KA")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->get_converter_initial_dc_current_reference_in_kA(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "PDC_MW" or PARAMETER_NAME == "INITIAL DC ACTIVE POWER IN MW")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->get_converter_initial_dc_active_power_reference_in_MW(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "KD" or PARAMETER_NAME == "DROOP COEFFICIENT")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
                   vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->get_converter_initial_droop_coefficient_for_droop_control(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "UACREF_PU" or PARAMETER_NAME == "NOMINAL AC VOLTAGE IN PU")
            {
                if(vsc_hvdc->get_converter_reactive_power_operation_mode(index)==VSC_AC_VOLTAGE_CONTROL)
                {
                    return vsc_hvdc->get_converter_nominal_ac_voltage_command_in_pu(index);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Reactive power control mode of converter index  %u  is not VSC_AC_VOLTAGE_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "QACREF_MVAR" or PARAMETER_NAME == "NOMINAL AC REACTIVE POWER IN MVAR")
            {
                if(vsc_hvdc->get_converter_reactive_power_operation_mode(index)==VSC_AC_REACTIVE_POWER_CONTROL)
                    return vsc_hvdc->get_converter_nominal_ac_reactive_power_command_in_Mvar(index);
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Reactive power control mode of converter index  %u  is not VSC_AC_REACTIVE_POWER_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return 0.0;
                }
            }
            if(PARAMETER_NAME == "ALOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT A IN KW")
            {
                return vsc_hvdc->get_converter_loss_factor_A_in_kW(index);
            }
            if(PARAMETER_NAME == "BLOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT B IN KW PER AMP")
                return vsc_hvdc->get_converter_loss_factor_B_in_kW_per_amp(index);
            if(PARAMETER_NAME == "CLOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT C IN KW PER AMP SQUARD")
                return vsc_hvdc->get_converter_loss_factor_C_in_kW_per_amp_squard(index);
            if(PARAMETER_NAME == "MINLOSS_KW" or PARAMETER_NAME == "MINIMUM CONVERTER LOSS IN KW")
                return vsc_hvdc->get_converter_minimum_loss_in_kW(index);
            if(PARAMETER_NAME == "SMAX_MVA" or PARAMETER_NAME == "CONVERTER RATED CAPACITY IN MVA")
                return vsc_hvdc->get_converter_rated_capacity_in_MVA(index);
            if(PARAMETER_NAME == "IMAX_A" or PARAMETER_NAME == "CONVERTER RATED CURRENT IN AMP")
                return vsc_hvdc->get_converter_rated_current_in_A(index);
            if(PARAMETER_NAME == "RC_OHM" or PARAMETER_NAME == "CONVERTER COMMUTATING RESISTANCE IN OHM")
                return vsc_hvdc->get_converter_commutating_impedance_in_ohm(index).real();
            if(PARAMETER_NAME == "XC_OHM" or PARAMETER_NAME == "CONVERTER COMMUTATING REACTANCE IN OHM")
                return vsc_hvdc->get_converter_commutating_impedance_in_ohm(index).imag();
            if(PARAMETER_NAME == "PMAX_MW" or PARAMETER_NAME == "MAXIMUM ACTIVE POWER IN MW")
                return vsc_hvdc->get_converter_Pmax_in_MW(index);
            if(PARAMETER_NAME == "PMIN_MW" or PARAMETER_NAME == "MINIMUM ACTIVE POWER IN MW")
                return vsc_hvdc->get_converter_Pmin_in_MW(index);
            if(PARAMETER_NAME == "QMAX_MVAR" or PARAMETER_NAME == "MAXIMUM REACTIVE POWER IN MVAR")
                return vsc_hvdc->get_converter_Qmax_in_MVar(index);
            if(PARAMETER_NAME == "QMIN_MVAR" or PARAMETER_NAME == "MINIMUM REACTIVE POWER IN MVAR")
                return vsc_hvdc->get_converter_Qmin_in_MVar(index);
            if(PARAMETER_NAME == "UDMAX_KV" or PARAMETER_NAME == "MAXIMUM DC VOLTAGE IN KV")
                return vsc_hvdc->get_converter_Udmax_in_kV(index);
            if(PARAMETER_NAME == "UDMIN_KV" or PARAMETER_NAME == "MINIMUM DC VOLTAGE IN KV")
                return vsc_hvdc->get_converter_Udmin_in_kV(index);
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "VDC_KV" or PARAMETER_NAME == "DC VOLTAGE IN KV")
                return vsc_hvdc->get_dc_bus_Vdc_in_kV(index);
            if(PARAMETER_NAME == "RG_OHM" or PARAMETER_NAME == "GROUND RESISTANCE IN OHM")
                return vsc_hvdc->get_dc_bus_ground_resistance_in_ohm(index);
            if(PARAMETER_NAME == "GEN_MW" or PARAMETER_NAME == "DC GENERATION POWER IN MW")
                return vsc_hvdc->get_dc_bus_generation_power_in_MW(index);
            if(PARAMETER_NAME == "LOAD_MW" or PARAMETER_NAME == "DC LOAD POWER IN MW")
                return vsc_hvdc->get_dc_bus_load_power_in_MW(index);
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "RL_OHM" or PARAMETER_NAME == "LINE R IN OHM")
                return vsc_hvdc->get_dc_line_resistance_in_ohm(index);
            if(PARAMETER_NAME == "LL_MH" or PARAMETER_NAME == "LINE L IN MH")
                return vsc_hvdc->get_dc_line_inductance_in_mH(index);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return 0.0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_vsc_hvdc_float_data(char* vsc_name, char* device_type, char* parameter_name, double value, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "VDCBASE_KV" or PARAMETER_NAME == "DC BASE VOLTAGE IN KV")
                return vsc_hvdc->set_dc_network_base_voltage_in_kV(value);
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "PREF_MW" or PARAMETER_NAME == "NOMINAL AC ACTIVE POWER IN MW")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_AC_ACTIVE_POWER_CONTORL)
                {
                    return vsc_hvdc->set_converter_nominal_ac_active_power_command_in_MW(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_AC_ACTIVE_POWER_CONTORL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "UDCREF_KV" or PARAMETER_NAME == "NOMINAL DC VOLTAGE IN KV")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_VOLTAGE_CONTORL)
                {
                    return vsc_hvdc->set_converter_nominal_dc_voltage_command_in_kV(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_VOLTAGE_CONTORL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "UDC_KV" or PARAMETER_NAME == "INITIAL DC VOLTAGE IN KV")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
                   vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->set_converter_initial_dc_voltage_reference_in_kV(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "IDC_KA" or PARAMETER_NAME == "INITIAL DC CURRENT IN KA")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->set_converter_initial_dc_current_reference_in_kA(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "PDC_MW" or PARAMETER_NAME == "INITIAL DC ACTIVE POWER IN MW")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->set_converter_initial_dc_active_power_reference_in_MW(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "KD" or PARAMETER_NAME == "DROOP COEFFICIENT")
            {
                if(vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL or
                   vsc_hvdc->get_converter_active_power_operation_mode(index)==VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL)
                {
                    return vsc_hvdc->set_converter_initial_droop_coefficient_for_droop_control(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Active power control mode of converter index  %u  is not VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL and VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "UACREF_PU" or PARAMETER_NAME == "NOMINAL AC VOLTAGE IN PU")
            {
                if(vsc_hvdc->get_converter_reactive_power_operation_mode(index)==VSC_AC_VOLTAGE_CONTROL)
                {
                    return vsc_hvdc->set_converter_nominal_ac_voltage_command_in_pu(index,value);
                }
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Reactive power control mode of converter index  %u  is not VSC_AC_VOLTAGE_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "QACREF_MVAR" or PARAMETER_NAME == "NOMINAL AC REACTIVE POWER IN MVAR")
            {
                if(vsc_hvdc->get_converter_reactive_power_operation_mode(index)==VSC_AC_REACTIVE_POWER_CONTROL)
                    return vsc_hvdc->set_converter_nominal_ac_reactive_power_command_in_Mvar(index,value);
                else
                {
                    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Reactive power control mode of converter index  %u  is not VSC_AC_REACTIVE_POWER_CONTROL, please check control mode!", index);
                    toolkit.show_information_with_leading_time_stamp(buffer);
                    return;
                }
            }
            if(PARAMETER_NAME == "ALOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT A IN KW")
            {
                return vsc_hvdc->set_converter_loss_factor_A_in_kW(index,value);
            }
            if(PARAMETER_NAME == "BLOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT B IN KW PER AMP")
                return vsc_hvdc->set_converter_loss_factor_B_in_kW_per_amp(index,value);
            if(PARAMETER_NAME == "CLOSS" or PARAMETER_NAME == "CONVERTER LOSS COEFFICIENT C IN KW PER AMP SQUARD")
                return vsc_hvdc->set_converter_loss_factor_C_in_KW_per_amp_squared(index,value);
            if(PARAMETER_NAME == "MINLOSS_KW" or PARAMETER_NAME == "MINIMUM CONVERTER LOSS IN KW")
                return vsc_hvdc->set_converter_minimum_loss_in_kW(index,value);
            if(PARAMETER_NAME == "SMAX_MVA" or PARAMETER_NAME == "CONVERTER RATED CAPACITY IN MVA")
                return vsc_hvdc->set_converter_rated_capacity_in_MVA(index,value);
            if(PARAMETER_NAME == "IMAX_A" or PARAMETER_NAME == "CONVERTER RATED CURRENT IN AMP")
                return vsc_hvdc->set_converter_rated_current_in_A(index,value);
            if(PARAMETER_NAME == "RC_OHM" or PARAMETER_NAME == "CONVERTER COMMUTATING RESISTANCE IN OHM")
            {
                double xc = vsc_hvdc->get_converter_commutating_impedance_in_ohm(index).imag();
                return vsc_hvdc->set_converter_commutating_impedance_in_ohm(index, complex<double>(value,xc));
            }
            if(PARAMETER_NAME == "XC_OHM" or PARAMETER_NAME == "CONVERTER COMMUTATING REACTANCE IN OHM")
            {
                double rc = vsc_hvdc->get_converter_commutating_impedance_in_ohm(index).real();
                return vsc_hvdc->set_converter_commutating_impedance_in_ohm(index,complex<double>(rc,value));
            }
            if(PARAMETER_NAME == "PMAX_MW" or PARAMETER_NAME == "MAXIMUM ACTIVE POWER IN MW")
                return vsc_hvdc->set_converter_Pmax_in_MW(index,value);
            if(PARAMETER_NAME == "PMIN_MW" or PARAMETER_NAME == "MINIMUM ACTIVE POWER IN MW")
                return vsc_hvdc->set_converter_Pmin_in_MW(index,value);
            if(PARAMETER_NAME == "QMAX_MVAR" or PARAMETER_NAME == "MAXIMUM REACTIVE POWER IN MVAR")
                return vsc_hvdc->set_converter_Qmax_in_MVar(index,value);
            if(PARAMETER_NAME == "QMIN_MVAR" or PARAMETER_NAME == "MINIMUM REACTIVE POWER IN MVAR")
                return vsc_hvdc->set_converter_Qmin_in_MVar(index,value);
            if(PARAMETER_NAME == "UDMAX_KV" or PARAMETER_NAME == "MAXIMUM DC VOLTAGE IN KV")
                return vsc_hvdc->set_converter_Udmax_in_kV(index,value);
            if(PARAMETER_NAME == "UDMIN_KV" or PARAMETER_NAME == "MINIMUM DC VOLTAGE IN KV")
                return vsc_hvdc->set_converter_Udmin_in_kV(index,value);
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "VDC_KV" or PARAMETER_NAME == "DC VOLTAGE IN KV")
                return vsc_hvdc->set_dc_bus_Vdc_in_kV(index,value);
            if(PARAMETER_NAME == "RG_OHM" or PARAMETER_NAME == "GROUND RESISTANCE IN OHM")
                return vsc_hvdc->set_dc_bus_ground_resistance_in_ohm(index,value);
            if(PARAMETER_NAME == "GEN_MW" or PARAMETER_NAME == "DC GENERATION POWER IN MW")
                return vsc_hvdc->set_dc_bus_generation_power_in_MW(index,value);
            if(PARAMETER_NAME == "LOAD_MW" or PARAMETER_NAME == "DC LOAD POWER IN MW")
                return vsc_hvdc->set_dc_bus_load_power_in_MW(index,value);
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "RL_OHM" or PARAMETER_NAME == "LINE R IN OHM")
                return vsc_hvdc->set_dc_line_resistance_in_ohm(index,value);
            if(PARAMETER_NAME == "LL_MH" or PARAMETER_NAME == "LINE L IN MH")
                return vsc_hvdc->set_dc_line_inductance_in_mH(index,value);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
    }
}

const char* api_get_vsc_hvdc_string_data(char* vsc_name, char* device_type, char* parameter_name, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "PROJECT NAME")
            {
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc_hvdc->get_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            if(PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
            {
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc_hvdc->get_identifier()).c_str());
                return toolkit.steps_char_buffer;
            }
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "CONVERTER NAME")
            {
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc_hvdc->get_converter_name(index)).c_str());
                return toolkit.steps_char_buffer;
            }
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "DC BUS NAME")
            {
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc_hvdc->get_dc_bus_name(index)).c_str());
                return toolkit.steps_char_buffer;
            }
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
            {
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc_hvdc->get_dc_line_identifier(index)).c_str());
                return toolkit.steps_char_buffer;
            }
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

void api_set_vsc_hvdc_string_data(char* vsc_name, char* device_type, char* parameter_name, char* value, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "PROJECT NAME")
                return vsc_hvdc->set_name(value);
            if(PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
                return vsc_hvdc->set_identifier(value);
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "CONVERTER NAME")
                return vsc_hvdc->set_converter_name(index,value);
        }
        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "VSC DC BUS")
        {
            if(PARAMETER_NAME == "NAME" or PARAMETER_NAME == "DC BUS NAME")
                return vsc_hvdc->set_dc_bus_name(index,value);
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
                return vsc_hvdc->set_dc_line_identifier(index,value);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
    }
}

bool api_get_vsc_hvdc_boolean_data(char* vsc_name, char* device_type, char* parameter_name, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->get_status();
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->get_converter_status(index);
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->get_dc_line_status(index);
        }
         show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_vsc_hvdc_boolean_data(char* vsc_name, char* device_type, char* parameter_name, bool value, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();
    if(vsc_hvdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "PROJECT" or DEVICE_TYPE == "VSC LCC_HVDC2T PROJECT")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->set_status(value);
        }
        if(DEVICE_TYPE == "CONVERTER" or DEVICE_TYPE == "VSC")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->set_converter_status(index,value);
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "VSC DC LINE")
        {
            if(PARAMETER_NAME == "STATUS")
                return vsc_hvdc->set_dc_line_status(index,value);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
    }
}
