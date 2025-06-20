#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_mt_hvdc_integer_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string DEVICE_NAME = device_name;
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "NTERMINAL" or PARAMETER_NAME == "TERMINAL COUNT")
                return mtdc->get_dc_terminal_count();
            if(PARAMETER_NAME == "NSTATION" or PARAMETER_NAME == "STATION COUNT")
                return mtdc->get_dc_station_count();
            if(PARAMETER_NAME == "NLCC" or PARAMETER_NAME == "LCC COUNT")
                return mtdc->get_lcc_converter_count();
            if(PARAMETER_NAME == "NVSC" or PARAMETER_NAME == "VSC COUNT")
                return mtdc->get_vsc_converter_count();
            if(PARAMETER_NAME == "NDCBUS" or PARAMETER_NAME == "DC BUS COUNT")
                return mtdc->get_dc_bus_count();
            if(PARAMETER_NAME == "NDCLINE" or PARAMETER_NAME == "DC LINE COUNT")
                return mtdc->get_dc_line_count();
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return 0;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                if(PARAMETER_NAME == "TERMINAL NUMBER" or PARAMETER_NAME == "DC TERMINAL NUMBER")
                    return terminal->get_terminal_number();
                if(PARAMETER_NAME == "TERMIANL DCBUS" or PARAMETER_NAME == "DC TERMINAL DCBUS NUMBER")
                    return terminal->get_terminal_dc_bus_number();
                if(PARAMETER_NAME == "TERMINAL MODE" or PARAMETER_NAME == "DC TERMINAL MODE")
                {
                    DC_TERMINAL_MODE tmode = terminal->get_terminal_mode();
                    switch(tmode)
                    {
                        case DC_P_TERMIANL:
                            return 1;
                        case DC_I_TERMINAL:
                            return 2;
                        case DC_U_TERMINAL:
                            return 3;
                        case DC_ANGLE_TERMINAL:
                            return 4;
                        case DC_INVALID_TERMINAL:
                            return 999;
                        default:
                            return 999;
                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return 0.0;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE=="DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "DC STATION NUMBER")
                    return station->get_station_number();
                if(PARAMETER_NAME == "STATION TERMINAL NUMBER" or PARAMETER_NAME == "DC STATION TERMINAL NUMBER")
                    return station->get_terminal_number();
                if(PARAMETER_NAME == "STATION TYPE" or PARAMETER_NAME == "DC STATION TYPE")
                {
                    DC_STATION_STRUCTURE station_type = station->get_station_structure();
                    switch(station_type)
                    {
                        case LCC_CASCADE:
                            return 1;
                        case VSC_PARALLEL:
                            return 2;
                        case LCC_HCM:
                            return 3;
                        case INVALID_DC_STATION_STRUCTURE:
                            return 999;
                        default:
                            return 999;
                    }
                }
                if(PARAMETER_NAME == "STATION LCC" or PARAMETER_NAME == "DC STATION LCC COUNT")
                    return station->get_station_cascade_lcc_number();
                if(PARAMETER_NAME == "STATION VSC" or PARAMETER_NAME == "DC STATION VSC COUNT")
                    return station->get_station_parallel_vsc_number();
                if(PARAMETER_NAME == "STATION MODE" or PARAMETER_NAME == "DC STATION MODE")
                {
                    DC_STATION_MODE smode = station->get_station_mode();
                    switch(smode)
                    {
                        case DC_P_STATION:
                            return 1;
                        case DC_I_STATION:
                            return 2;
                        case DC_U_STATION:
                            return 3;
                        case DC_ANGLE_STATION:
                            return 4;
                        case DC_INVALID_STATION:
                            return 999;
                        default:
                            return 999;
                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE=="LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "AC BUS" or PARAMETER_NAME == "LCC AC BUS")
                    return lcc->get_converter_ac_bus();
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "LCC STATION NUMBER")
                    return lcc->get_converter_station_number();
                if(PARAMETER_NAME == "OPERATION MODE" or PARAMETER_NAME == "LCC OPERATION MODE")
                {
                    HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();
                    switch(omode)
                    {
                        case LCC_RECTIFIER:
                            return 1;
                        case LCC_INVERTER:
                            return 2;
                        case INVALID_HLCC_OPERATING_MODE:
                            return 999;
                        default:
                            return 999;
                    }
                }
                if(PARAMETER_NAME == "CONTROL MODE" or PARAMETER_NAME == "LCC CONTROL MODE")
                {
                    HLCC_CONTROL_MODE cmode = lcc->get_control_mode();
                    switch(cmode)
                    {
                        case LCC_CONSTANT_POWER_MODE:
                            return 1;
                        case LCC_CONSTANT_VOLTAGE_MODE:
                            return 2;
                        case LCC_CONSTANT_CURRENT_MODE:
                            return 3;
                        case LCC_CONSTANR_FIRING_ANGLE_MODE:
                            return 4;
                        case LCC_INVALID_CONTROL_MODE:
                            return 999;
                        default:
                            return 999;
                    }
                }
                if(PARAMETER_NAME == "BRIDEG NUMBER" or PARAMETER_NAME == "LCC BRIDEG NUMBER")
                    return lcc->get_brideg_number();
                if(PARAMETER_NAME == "TAP NUMBER" or PARAMETER_NAME == "LCC TAP NUMBER")
                    return lcc->get_converter_transformer_number_of_taps();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE=="VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                if(PARAMETER_NAME == "AC BUS" or PARAMETER_NAME == "VSC AC BUS")
                    return vsc->get_converter_ac_bus();
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "VSC STATION NUMBER")
                    return vsc->get_converter_station_number();
                if(PARAMETER_NAME == "PRIORITY" or PARAMETER_NAME == "VSC PRIORITY")
                    return vsc->get_priority();
                if(PARAMETER_NAME == "D CONTROL MODE" or PARAMETER_NAME == "VSC D CONTROL MODE")
                {
                    HVSC_D_CONTROL_MODE dmode = vsc->get_converter_d_axis_control_mode();
                    switch(dmode)
                    {
                        case VSC_CONSTANT_AC_ACTIVE_POWER_MODE:
                            return 1;
                        case VSC_CONSTANT_DC_VOLTAGE_MODE:
                            return 2;
                        case VSC_INVALID_D_CONTROL_MODE:
                            return 999;
                        default:
                            return 999;
                    }
                }
                if(PARAMETER_NAME == "Q CONTROL MODE" or PARAMETER_NAME == "VSC Q CONTROL MODE")
                {
                    HVSC_Q_CONTROL_MODE qmode = vsc->get_converter_q_axis_control_mode();
                    switch(qmode)
                    {
                        case VSC_CONSTANT_AC_REACTIVE_POWER_MODE:
                            return 1;
                        case VSC_CONSTANT_AC_VOLTAGE_MODE:
                            return 2;
                        case VSC_INVALID_Q_CONTROL_MODE:
                            return 999;
                        default:
                            return 999;
                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                if(PARAMETER_NAME == "BUS NUMBER" or PARAMETER_NAME == "DC BUS NUMBER")
                    return dcbus->get_dc_bus_number();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE=="DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "IBUS" or PARAMETER_NAME == "SENDING SIDE BUS")
                    return dcline->get_sending_side_bus_number();
                if(PARAMETER_NAME == "JBUS" or PARAMETER_NAME == "RECEICING SIDE BUS")
                    return dcline->get_receiving_side_bus_number();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }
        }

        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
}

void api_set_mt_hvdc_integer_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, int value, int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "NTERMINAL" or PARAMETER_NAME == "TERMINAL COUNT")
                return mtdc->set_dc_terminal_count(value);
            if(PARAMETER_NAME == "NSTATION" or PARAMETER_NAME == "STATION COUNT")
                return mtdc->set_dc_station_count(value);
            if(PARAMETER_NAME == "NLCC" or PARAMETER_NAME == "LCC COUNT")
                return mtdc->set_lcc_converter_count(value);
            if(PARAMETER_NAME == "NVSC" or PARAMETER_NAME == "VSC COUNT")
                return mtdc->set_vsc_converter_count(value);
            if(PARAMETER_NAME == "NDCBUS" or PARAMETER_NAME == "DC BUS COUNT")
                return mtdc->set_dc_bus_count(value);
            if(PARAMETER_NAME == "NDCLINE" or PARAMETER_NAME == "DC LINE COUNT")
                return mtdc->set_dc_line_count(value);
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                if(PARAMETER_NAME == "TERMINAL NUMBER" or PARAMETER_NAME == "DC TERMINAL NUMBER")
                    return terminal->set_terminal_number(value);
                if(PARAMETER_NAME == "TERMIANL DCBUS" or PARAMETER_NAME == "DC TERMINAL DCBUS NUMBER")
                    return terminal->set_terminal_dc_bus_number(value);
                if(PARAMETER_NAME == "TERMINAL MODE" or PARAMETER_NAME == "DC TERMINAL MODE")
                {
                    switch(value)
                    {
                        case 1:
                            return terminal->set_terminal_mode(DC_P_TERMIANL);
                        case 2:
                            return terminal->set_terminal_mode(DC_I_TERMINAL);
                        case 3:
                            return terminal->set_terminal_mode(DC_U_TERMINAL);
                        case 4:
                            return terminal->set_terminal_mode(DC_ANGLE_TERMINAL);
                        default:
                            return terminal->set_terminal_mode(DC_INVALID_TERMINAL);
                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE=="DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "DC STATION NUMBER")
                    return station->set_station_number(value);
                if(PARAMETER_NAME == "STATION TERMINAL NUMBER" or PARAMETER_NAME == "DC STATION TERMINAL NUMBER")
                    return station->set_terminal_number(value);
                if(PARAMETER_NAME == "STATION TYPE" or PARAMETER_NAME == "DC STATION TYPE")
                {
                    switch(value)
                    {
                        case 1:
                            return station->set_station_structure(LCC_CASCADE);
                        case 2:
                            return station->set_station_structure(VSC_PARALLEL);
                        case 3:
                            return station->set_station_structure(LCC_HCM);
                        default:
                            return station->set_station_structure(INVALID_DC_STATION_STRUCTURE);
                    }
                }
                if(PARAMETER_NAME == "STATION LCC" or PARAMETER_NAME == "DC STATION LCC COUNT")
                    return station->set_station_cascade_lcc_number(value);
                if(PARAMETER_NAME == "STATION VSC" or PARAMETER_NAME == "DC STATION VSC COUNT")
                    return station->set_station_parallel_vsc_number(value);
                if(PARAMETER_NAME == "STATION MODE" or PARAMETER_NAME == "DC STATION MODE")
                {
                    switch(value)
                    {
                        case 1:
                            return station->set_station_mode(DC_P_STATION);
                        case 2:
                            return station->set_station_mode(DC_I_STATION);
                        case 3:
                            return station->set_station_mode(DC_U_STATION);
                        case 4:
                            return station->set_station_mode(DC_ANGLE_STATION);
                        default:
                            return station->set_station_mode(DC_INVALID_STATION);
                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "AC BUS" or PARAMETER_NAME == "LCC AC BUS")
                    return lcc->set_converter_ac_bus(value);
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "LCC STATION NUMBER")
                    return lcc->set_converter_station_number(value);
                if(PARAMETER_NAME == "OPERATION MODE" or PARAMETER_NAME == "LCC OPERATION MODE")
                {
                    switch(value)
                    {
                        case 1:
                            return lcc->set_operating_mode(LCC_RECTIFIER);
                        case 2:
                            return lcc->set_operating_mode(LCC_INVERTER);
                        default:
                            return lcc->set_operating_mode(INVALID_HLCC_OPERATING_MODE);
                    }
                }
                if(PARAMETER_NAME == "CONTROL MODE" or PARAMETER_NAME == "LCC CONTROL MODE")
                {
                    switch(value)
                    {
                        case 1:
                            return lcc->set_control_mode(LCC_CONSTANT_POWER_MODE);
                        case 2:
                            return lcc->set_control_mode(LCC_CONSTANT_VOLTAGE_MODE);
                        case 3:
                            return lcc->set_control_mode(LCC_CONSTANT_CURRENT_MODE);
                        case 4:
                            return lcc->set_control_mode(LCC_CONSTANR_FIRING_ANGLE_MODE);
                        default:
                            return lcc->set_control_mode(LCC_INVALID_CONTROL_MODE);
                    }
                }
                if(PARAMETER_NAME == "BRIDEG NUMBER" or PARAMETER_NAME == "LCC BRIDEG NUMBER")
                    return lcc->set_brideg_number(value);
                if(PARAMETER_NAME == "TAP NUMBER" or PARAMETER_NAME == "LCC TAP NUMBER")
                    return lcc->set_converter_transformer_number_of_taps(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                if(PARAMETER_NAME == "AC BUS" or PARAMETER_NAME == "VSC AC BUS")
                    return vsc->set_converter_ac_bus(value);
                if(PARAMETER_NAME == "STATION NUMBER" or PARAMETER_NAME == "VSC STATION NUMBER")
                    return vsc->set_converter_station_number(value);
                if(PARAMETER_NAME == "PRIORITY" or PARAMETER_NAME == "VSC PRIORITY")
                    return vsc->set_priority(value);
                if(PARAMETER_NAME == "D CONTROL MODE" or PARAMETER_NAME == "VSC D CONTROL MODE")
                {
                    switch(value)
                    {
                    case 1:
                        return vsc->set_converter_d_axis_control_mode(VSC_CONSTANT_AC_ACTIVE_POWER_MODE);
                    case 2:
                        return vsc->set_converter_d_axis_control_mode(VSC_CONSTANT_DC_VOLTAGE_MODE);
                    default:
                        return vsc->set_converter_d_axis_control_mode(VSC_INVALID_D_CONTROL_MODE);
                    }
                }
                if(PARAMETER_NAME == "Q CONTROL MODE" or PARAMETER_NAME == "VSC Q CONTROL MODE")
                {
                    switch(value)
                    {
                    case 1:
                        return vsc->set_converter_q_axis_control_mode(VSC_CONSTANT_AC_REACTIVE_POWER_MODE);
                    case 2:
                        return vsc->set_converter_q_axis_control_mode(VSC_CONSTANT_AC_VOLTAGE_MODE);
                    default:
                        return vsc->set_converter_q_axis_control_mode(VSC_INVALID_Q_CONTROL_MODE);

                    }
                }
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                if(PARAMETER_NAME == "BUS NUMBER" or PARAMETER_NAME == "DC BUS NUMBER")
                    return dcbus->set_dc_bus_number(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "IBUS" or PARAMETER_NAME == "SENDING SIDE BUS")
                    return dcline->set_sending_side_bus(value);
                if(PARAMETER_NAME == "JBUS" or PARAMETER_NAME == "RECEICING SIDE BUS")
                    return dcline->set_receiving_side_bus(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}


double api_get_mt_hvdc_float_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        //string DEVICE_NAME = string2upper(device_name);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "UDC BASE" or PARAMETER_NAME == "BASE DC VOLTAGE")
                return mtdc->get_dc_base_voltage();
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return 0.0;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return 0.0;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0.0;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "PDC COMMAND IN MW" or PARAMETER_NAME == "DC POWER COMMAND VALUE IN MW")
                    return lcc->get_command_pdc_in_MW();
                if(PARAMETER_NAME == "IDC COMMAND IN KA" or PARAMETER_NAME == "DC CURRENT COMMAND VALUE IN KA")
                    return lcc->get_command_idc_in_kA();
                if(PARAMETER_NAME == "UDC COMMAND IN KV" or PARAMETER_NAME == "DC VOLTAGE COMMAND VALUE IN KV")
                    return lcc->get_command_udc_in_kV();
                if(PARAMETER_NAME == "ANGLE COMMAND IN DEG" or PARAMETER_NAME == "FIRING ANGLE COMMAND VALUE IN DEG")
                    return lcc->get_command_firing_angle();
                if(PARAMETER_NAME == "VDROP IN KV" or PARAMETER_NAME == "VOLTAGE DROP PER BRIDGE IN KV")
                    return lcc->get_voltage_drop_per_bridge_in_kV();
                if(PARAMETER_NAME == "LSMOOTH IN H" or PARAMETER_NAME == "LINE SOMMTH L IN H")
                    return lcc->get_line_smoothing_L_in_H();
                if(PARAMETER_NAME == "RSMOOTH IN OHM" or PARAMETER_NAME == "LINE SOMMTH R IN OHM")
                    return lcc->get_line_smoothing_R_in_ohm();
                if(PARAMETER_NAME == "VBASE GRID IN KV" or PARAMETER_NAME == "GRID SIDE BASE VOLTAGE IN KV")
                    return lcc->get_converter_transformer_grid_side_base_voltage_in_kV();
                if(PARAMETER_NAME == "VBASE CONVERTER IN KV" or PARAMETER_NAME == "CONVERTER SIDE BASE VOLTAGE IN KV")
                    return lcc->get_converter_transformer_converter_side_base_voltage_in_kV();
                if(PARAMETER_NAME == "RTRNAS IN OHM" or PARAMETER_NAME == "CONVERTER TRANSFORMER R IN OHM")
                    return lcc->get_converter_transformer_R_in_ohm();
                if(PARAMETER_NAME == "XTRNAS IN OHM" or PARAMETER_NAME == "CONVERTER TRANSFORMER X IN OHM")
                    return lcc->get_converter_transformer_X_in_ohm();
                if(PARAMETER_NAME == "TAP MAX IN PU" or PARAMETER_NAME == "CONVERTER TRANSFORMER TAP MAX IN PU")
                    return lcc->get_converter_transformer_max_tap_in_pu();
                 if(PARAMETER_NAME == "TAP MIN IN PU" or PARAMETER_NAME == "CONVERTER TRANSFORMER TAP MIN IN PU")
                    return lcc->get_converter_transformer_min_tap_in_pu();
                if(PARAMETER_NAME == "ANGLE MAX IN PU" or PARAMETER_NAME == "CONVERTER FIRING ANGLE MAX IN DEG")
                    return lcc->get_max_firing_angle_in_deg();
                if(PARAMETER_NAME == "ANGLE MIN IN PU" or PARAMETER_NAME == "CONVERTER FIRING ANGLE MIN IN DEG")
                    return lcc->get_min_firing_angle_in_deg();
                if(PARAMETER_NAME == "SBASE IN KVA" or PARAMETER_NAME == "RATED CAPACITY IN MVA")
                    return lcc->get_converter_rated_capacity_in_MVA();
                if(PARAMETER_NAME == "VBASE IN KV" or PARAMETER_NAME == "RATED VOLTAGE IN KV")
                    return lcc->get_converter_base_voltage_in_kV();
                if(PARAMETER_NAME == "MARGINE IN PU" or PARAMETER_NAME == "CURRENT MARGINE IN PU")
                    return lcc->get_converter_current_margin_in_pu();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0.0;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
               if(PARAMETER_NAME == "PAC COMMAND IN MW" or PARAMETER_NAME == "AC ACTIVE POWER COMMAND VALUE IN MW")
                    return vsc->get_converter_Pac_command();
                if(PARAMETER_NAME == "QAC COMMAND IN MVAR" or PARAMETER_NAME == "AC REACTIVE POWER COMMAND VALUE IN MVAR")
                    return vsc->get_converter_Qac_command();
                if(PARAMETER_NAME == "UDC COMMAND IN KV" or PARAMETER_NAME == "DC VOLTAGE COMMAND VALUE IN KV")
                    return vsc->get_converter_Udc_command();
                if(PARAMETER_NAME == "UAC COMMAND IN KV" or PARAMETER_NAME == "AC VOLTAGE COMMAND VALUE IN KV")
                    return vsc->get_converter_Us_command();
                if(PARAMETER_NAME == "ALOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT A IN KW")
                    return vsc->get_converter_loss_coefficient_A_in_kW();
                if(PARAMETER_NAME == "BLOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT B IN KW PWE AMP")
                    return vsc->get_converter_loss_coefficient_B_in_kW_per_amp();
                if(PARAMETER_NAME == "CLOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT C IN KW PWER AMP SQUARD")
                    return vsc->get_converter_loss_coefficient_C_in_kW_per_amp_squard();
                if(PARAMETER_NAME == "MIN LOSS IN MW" or PARAMETER_NAME == "VSC MIN LOSS IN MW")
                    return vsc->get_converter_min_loss_in_kW();
                if(PARAMETER_NAME == "RC IN OHM" or PARAMETER_NAME == "VSC COMMUTATING RC IN OHM")
                    return vsc->get_converter_commutating_impedance_RC();
                if(PARAMETER_NAME == "XC IN OHM" or PARAMETER_NAME == "VSC COMMUTATING XC IN OHM")
                    return vsc->get_converter_commutating_impedance_XC();
                if(PARAMETER_NAME == "SBASE IN KVA" or PARAMETER_NAME == "RATED CAPACITY IN MVA")
                    return vsc->get_converter_rated_capacity_in_MVA();
                if(PARAMETER_NAME == "VBASE IN KV" or PARAMETER_NAME == "RATED VOLTAGE IN KV")
                    return vsc->get_converter_base_voltage_in_kV();

                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0.0;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0.0;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE =="DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "R IN OHM" or PARAMETER_NAME == "LINE R IN OHM")
                    return dcline->get_line_R_in_ohm();
                if(PARAMETER_NAME == "L IN H" or PARAMETER_NAME == "LINE L IN H")
                    return dcline->get_line_L_in_H();
                if(PARAMETER_NAME == "C IN UF" or PARAMETER_NAME == "LINE C IN UF")
                    return dcline->get_line_C_in_uF();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return 0.0;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return 0;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return 0;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
}

void api_set_mt_hvdc_float_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "UDC BASE" or PARAMETER_NAME == "BASE DC VOLTAGE")
                return mtdc->set_dc_base_voltage(value);
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "PDC COMMAND IN MW" or PARAMETER_NAME == "DC POWER COMMAND VALUE IN MW")
                    return lcc->set_command_pdc_in_MW(value);
                if(PARAMETER_NAME == "IDC COMMAND IN KA" or PARAMETER_NAME == "DC CURRENT COMMAND VALUE IN KA")
                    return lcc->set_command_idc_in_kA(value);
                if(PARAMETER_NAME == "UDC COMMAND IN KV" or PARAMETER_NAME == "DC VOLTAGE COMMAND VALUE IN KV")
                    return lcc->set_command_udc_in_kV(value);
                if(PARAMETER_NAME == "ANGLE COMMAND IN DEG" or PARAMETER_NAME == "FIRING ANGLE COMMAND VALUE IN DEG")
                    return lcc->set_command_firing_angle(value);
                if(PARAMETER_NAME == "VDROP IN KV" or PARAMETER_NAME == "VOLTAGE DROP PER BRIDGE IN KV")
                    return lcc->set_voltage_drop_per_bridge_in_kV(value);
                if(PARAMETER_NAME == "LSMOOTH IN H" or PARAMETER_NAME == "LINE SOMMTH L IN H")
                    return lcc->set_line_smoothing_L_in_H(value);
                if(PARAMETER_NAME == "RSMOOTH IN OHM" or PARAMETER_NAME == "LINE SOMMTH R IN OHM")
                    return lcc->set_line_smoothing_R_in_ohm(value);
                if(PARAMETER_NAME == "VBASE GRID IN KV" or PARAMETER_NAME == "GRID SIDE BASE VOLTAGE IN KV")
                    return lcc->set_converter_transformer_grid_side_base_voltage_in_kV(value);
                if(PARAMETER_NAME == "VBASE CONVERTER IN KV" or PARAMETER_NAME == "CONVERTER SIDE BASE VOLTAGE IN KV")
                    return lcc->set_converter_transformer_converter_side_base_voltage_in_kV(value);
                if(PARAMETER_NAME == "RTRNAS IN OHM" or PARAMETER_NAME == "CONVERTER TRANSFORMER R IN OHM")
                    return lcc->set_converter_transformer_R_in_ohm(value);
                if(PARAMETER_NAME == "XTRNAS IN OHM" or PARAMETER_NAME == "CONVERTER TRANSFORMER X IN OHM")
                    return lcc->set_converter_transformer_X_in_ohm(value);
                if(PARAMETER_NAME == "TAP MAX IN PU" or PARAMETER_NAME == "CONVERTER TRANSFORMER TAP MAX IN PU")
                    return lcc->set_converter_transformer_max_tap_in_pu(value);
                if(PARAMETER_NAME == "TAP MIN IN PU" or PARAMETER_NAME == "CONVERTER TRANSFORMER TAP MIN IN PU")
                    return lcc->set_converter_transformer_min_tap_in_pu(value);
                if(PARAMETER_NAME == "ANGLE MAX IN PU" or PARAMETER_NAME == "CONVERTER FIRING ANGLE MAX IN DEG")
                    return lcc->set_max_firing_angle_in_deg(value);
                if(PARAMETER_NAME == "ANGLE MIN IN PU" or PARAMETER_NAME == "CONVERTER FIRING ANGLE MIN IN DEG")
                    return lcc->set_min_firing_angle_in_deg(value);
                if(PARAMETER_NAME == "SBASE IN KVA" or PARAMETER_NAME == "RATED CAPACITY IN MVA")
                    return lcc->set_converter_rated_capacity_in_MVA(value);
                if(PARAMETER_NAME == "VBASE IN KV" or PARAMETER_NAME == "RATED VOLTAGE IN KV")
                    return lcc->set_converter_base_voltage_in_kV(value);
                if(PARAMETER_NAME == "MARGINE IN PU" or PARAMETER_NAME == "CURRENT MARGINE IN PU")
                    return lcc->set_converter_current_margin_in_pu(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE =="VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
               if(PARAMETER_NAME == "PAC COMMAND IN MW" or PARAMETER_NAME == "AC ACTIVE POWER COMMAND VALUE IN MW")
                    return vsc->set_converter_Pac_command(value);
                if(PARAMETER_NAME == "QAC COMMAND IN MVAR" or PARAMETER_NAME == "AC REACTIVE POWER COMMAND VALUE IN MVAR")
                    return vsc->set_converter_Qac_command(value);
                if(PARAMETER_NAME == "UDC COMMAND IN KV" or PARAMETER_NAME == "DC VOLTAGE COMMAND VALUE IN KV")
                    return vsc->set_converter_Udc_command(value);
                if(PARAMETER_NAME == "UAC COMMAND IN KV" or PARAMETER_NAME == "AC VOLTAGE COMMAND VALUE IN KV")
                    return vsc->set_converter_Us_command(value);
                if(PARAMETER_NAME == "ALOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT A IN KW")
                    return vsc->set_converter_loss_coefficient_A_in_kW(value);
                if(PARAMETER_NAME == "BLOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT B IN KW PWE AMP")
                    return vsc->set_converter_loss_coefficient_B_in_kW_per_amp(value);
                if(PARAMETER_NAME == "CLOSS" or PARAMETER_NAME == "VSC LOSS COEFFICIENT C IN KW PWER AMP SQUARD")
                    return vsc->set_converter_loss_coefficient_C_in_kW_per_amp_squard(value);
                if(PARAMETER_NAME == "MIN LOSS IN MW" or PARAMETER_NAME == "VSC MIN LOSS IN MW")
                    return vsc->set_converter_min_loss_in_kW(value);
                if(PARAMETER_NAME == "RC IN OHM" or PARAMETER_NAME == "VSC COMMUTATING RC IN OHM")
                    return vsc->set_converter_commutating_impedance_RC(value);
                if(PARAMETER_NAME == "XC IN OHM" or PARAMETER_NAME == "VSC COMMUTATING XC IN OHM")
                    return vsc->set_converter_commutating_impedance_XC(value);
                if(PARAMETER_NAME == "SBASE IN KVA" or PARAMETER_NAME == "RATED CAPACITY IN MVA")
                    return vsc->set_converter_rated_capacity_in_MVA(value);
                if(PARAMETER_NAME == "VBASE IN KV" or PARAMETER_NAME == "RATED VOLTAGE IN KV")
                    return vsc->set_converter_base_voltage_in_kV(value);

                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "R IN OHM" or PARAMETER_NAME == "LINE R IN OHM")
                    return dcline->set_line_R_in_ohm(value);
                if(PARAMETER_NAME == "L IN H" or PARAMETER_NAME == "LINE L IN H")
                    return dcline->set_line_L_in_H(value);
                if(PARAMETER_NAME == "C IN UF" or PARAMETER_NAME == "LINE C IN UF")
                    return dcline->set_line_C_in_uF(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return ;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}


const char* api_get_mt_hvdc_string_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);

    DEVICE_ID did = mtdc->get_device_id();
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        //string DEVICE_NAME = string2upper(device_name);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return toolkit.steps_char_buffer;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return toolkit.steps_char_buffer;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return toolkit.steps_char_buffer;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                if(PARAMETER_NAME=="NAME")
                {
                    snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc->get_converter_name()).c_str());
                    return toolkit.steps_char_buffer;
                }

                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return toolkit.steps_char_buffer;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return toolkit.steps_char_buffer;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return toolkit.steps_char_buffer;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
}

void api_set_mt_hvdc_string_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);

    DEVICE_ID did = mtdc->get_device_id();
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        //string DEVICE_NAME = string2upper(device_name);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE =="DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}


bool api_get_mt_hvdc_boolean_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);

    DEVICE_ID did = mtdc->get_device_id();
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "STATUS")
                return mtdc->get_status();
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return false;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return terminal->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return station->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return lcc->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE =="VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return vsc->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return dcbus->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return dcline->get_status();
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return false;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return false;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return false;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return false;
    }
}

void api_set_mt_hvdc_boolean_data(char* mtdc_name, char* device_type, char* device_name, char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);

    DEVICE_ID did = mtdc->get_device_id();
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    if(mtdc != NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        string PARAMETER_NAME = string2upper(parameter_name);
        if(DEVICE_TYPE == "MTDC" or DEVICE_TYPE == "MT HVDC PROJECT")
        {
            if(PARAMETER_NAME == "STATUS")
                return mtdc->set_status(value);
            show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
            return;
        }
        if(DEVICE_TYPE == "DC TERMINAL" or DEVICE_TYPE == "TERMINAL")
        {
            DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_name(device_name);
            if(terminal!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return terminal->set_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name, __FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            DC_STATION* station = mtdc->get_dc_station_with_name(device_name);
            if(station!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return station->set_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;

            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
            if(lcc!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return lcc->set_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }

        }
        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
            if(vsc!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return vsc->set_converter_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_name(device_name);
            if(dcbus!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return dcbus->set_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }

        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            DC_LINE* dcline = mtdc->get_dc_line_with_line_name(device_name);
            if(dcline!=NULL)
            {
                if(PARAMETER_NAME == "STATUS")
                    return dcline->set_status(value);
                show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
                return;
            }
            else
            {
                show_mtdc_device_not_exist_with_api(device_name,__FUNCTION__);
                return;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}

void api_set_mt_hvdc_device_name_with_index(char* mtdc_name, char* device_type, unsigned int index, char* name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc!=NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        if(DEVICE_TYPE == "TERMINAL" or DEVICE_TYPE =="DC TERMINAL")
        {
            unsigned int n_terminal = mtdc->get_dc_terminal_count();
            if(index<n_terminal)
            {
                return mtdc->set_terminal_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Terminal index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }

        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            unsigned int n_station = mtdc->get_dc_station_count();
            if(index<n_station)
            {
                return mtdc->set_station_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Station index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }

        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            unsigned int n_lcc = mtdc->get_lcc_converter_count();
            if(index<n_lcc)
            {
                return mtdc->set_lcc_converter_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "LCC converter index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }

        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            unsigned int n_vsc = mtdc->get_vsc_converter_count();
            if(index<n_vsc)
            {
                return mtdc->set_vsc_converter_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "VSC converter index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            unsigned int n_dcbus = mtdc->get_dc_bus_count();
            if(index<n_dcbus)
            {
                return mtdc->set_dc_bus_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "DC bus index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            unsigned int n_dcline = mtdc->get_dc_line_count();
            if(index<n_dcline)
            {
                return mtdc->set_dc_line_name(index,name);
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "DC line index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
}

const char* api_get_mt_hvdc_device_name_with_index(char* mtdc_name, char* device_type, unsigned int index, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc!=NULL)
    {
        string DEVICE_TYPE = string2upper(device_type);
        if(DEVICE_TYPE == "TERMINAL" or DEVICE_TYPE =="DC TERMINAL")
        {
            unsigned int n_terminal = mtdc->get_dc_terminal_count();
            if(index<n_terminal)
            {
                DC_TERMINAL* terminal = mtdc->get_dc_terminal_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (terminal->get_terminal_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Terminal index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "STATION" or DEVICE_TYPE == "DC STATION")
        {
            unsigned int n_station = mtdc->get_dc_station_count();
            if(index<n_station)
            {
                DC_STATION* station = mtdc->get_dc_station_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (station->get_station_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Station index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "LCC" or DEVICE_TYPE == "LCC CONVERTER")
        {
            unsigned int n_lcc = mtdc->get_lcc_converter_count();
            if(index<n_lcc)
            {
                HLCC* lcc = mtdc->get_lcc_converter_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (lcc->get_converter_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "LCC converter index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "VSC" or DEVICE_TYPE == "VSC CONVERTER")
        {
            unsigned int n_vsc = mtdc->get_vsc_converter_count();
            if(index<n_vsc)
            {
                HVSC* vsc = mtdc->get_vsc_converter_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (vsc->get_converter_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "VSC converter index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }

        if(DEVICE_TYPE == "DC BUS" or DEVICE_TYPE == "DCBUS")
        {
            unsigned int n_dcbus = mtdc->get_dc_bus_count();
            if(index<n_dcbus)
            {
                DC_BUS* dcbus = mtdc->get_dc_bus_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (dcbus->get_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "DC bus index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }
        if(DEVICE_TYPE == "DC LINE" or DEVICE_TYPE == "DCLINE")
        {
            unsigned int n_dcline = mtdc->get_dc_line_count();
            if(index<n_dcline)
            {
                DC_LINE* dcline = mtdc->get_dc_line_with_index(index);
                snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (dcline->get_dc_line_name()).c_str());
                return toolkit.steps_char_buffer;
            }
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "DC line index  %u  is out of range in MTDC %s !", index, mtdc_name);
                toolkit.show_information_with_leading_time_stamp(buffer);
                return toolkit.steps_char_buffer;
            }
        }
        show_mtdc_device_type_not_supported_with_api(DEVICE_TYPE,__FUNCTION__);
        return toolkit.steps_char_buffer;
    }
    else
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }
}
