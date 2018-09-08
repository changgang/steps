#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

int api_get_transformer_integer_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE != "TRANSFORMER" and SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return 0;
        }

        if(SIDE == "TRANSFORMER")
        {
            if(PARAMETER_NAME == "BUS" or PARAMETER_NAME == "BUS_NMETER" or PARAMETER_NAME == "BUS_METER" or PARAMETER_NAME == "NON METERED END BUS")
                return transptr->get_non_metered_end_bus();

            if(PARAMETER_NAME=="OWNER1" or PARAMETER_NAME=="OWNER2" or PARAMETER_NAME=="OWNER3" or PARAMETER_NAME=="OWNER4")
                return get_owner_of_device(transptr, PARAMETER_NAME);
        }
        else
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="BUS" or PARAMETER_NAME == "BUS NUMBER")
                return transptr->get_winding_bus(winding);

            if(PARAMETER_NAME == "TYPE" or PARAMETER_NAME == "WINDING CONNECTION TYPE")
            {
                TRANSFORMER_WINDING_CONNECTION_TYPE connection_type = transptr->get_winding_connection_type(winding);
                if(connection_type == WYE_CONNECTION)
                    return 1;
                else
                {
                    if(connection_type == DELTA_CONNECTION)
                        return 2;
                    else
                        return 3;
                }
            }

            if(PARAMETER_NAME=="NTAPS" or PARAMETER_NAME == "NUMBER OF TAPS")
                return transptr->get_winding_number_of_taps(winding);

            if(PARAMETER_NAME=="BUS_CTRL" or PARAMETER_NAME == "CONTROLLED BUS")
                return transptr->get_winding_controlled_bus(winding);
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

void api_set_transformer_integer_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name, int value)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);
        string COMBINED_PARAMETER_NAME = SIDE+":"+PARAMETER_NAME;

        if(SIDE != "TRANSFORMER" and SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(SIDE == "TRANSFORMER")
        {
            if(PARAMETER_NAME == "BUS_NMETER" or PARAMETER_NAME == "NON METERED END BUS")
                return transptr->set_non_metered_end_bus(value);
            if(PARAMETER_NAME=="OWNER1" or PARAMETER_NAME=="OWNER2" or PARAMETER_NAME=="OWNER3" or PARAMETER_NAME=="OWNER4")
                return set_owner_of_device(transptr, PARAMETER_NAME, value);
        }
        else
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="BUS" or PARAMETER_NAME == "BUS NUMBER")
                return transptr->set_winding_bus(winding, value);

            if(PARAMETER_NAME == "TYPE" or PARAMETER_NAME == "WINDING CONNECTION TYPE")
            {
                TRANSFORMER_WINDING_CONNECTION_TYPE connection_type;
                if(value==1)
                    connection_type = WYE_CONNECTION;
                else
                {
                    if(value==2)
                        connection_type = DELTA_CONNECTION;
                    else
                        connection_type = WYE_N_CONNECTION;
                }
                return transptr->set_winding_connection_type(winding, connection_type);
            }

            if(PARAMETER_NAME=="NTAPS" or PARAMETER_NAME == "NUMBER OF TAPS")
                return transptr->set_winding_number_of_taps(winding, value);

            if(PARAMETER_NAME=="BUS_CTRL" or PARAMETER_NAME == "CONTROLLED BUS")
                return transptr->set_winding_controlled_bus(winding, value);
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}


double api_get_transformer_float_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "TRANSFORMER" and SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return 0.0;
        }

         if(SIDE == "TRANSFORMER")
        {
            if(PARAMETER_NAME == "GM_PU" or PARAMETER_NAME == "MAGNETIZING G BASED ON PRI WINDING BUS BASE V AND SYSTEM BASE S IN PU")
                return transptr->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().real();
            if(PARAMETER_NAME == "BM_PU" or PARAMETER_NAME == "MAGNETIZING B BASED ON PRI WINDING BUS BASE V AND SYSTEM BASE S IN PU")
                return transptr->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().imag();
            if(PARAMETER_NAME == "SBASE_PS_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN PRI AND SEC IN MVA")
                return transptr->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);
            if(PARAMETER_NAME == "SBASE_ST_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN SEC AND TER IN MVA")
                return transptr->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE);
            if(PARAMETER_NAME == "SBASE_TP_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN TER AND PRI IN MVA")
                return transptr->get_winding_nominal_capacity_in_MVA(TERTIARY_SIDE, PRIMARY_SIDE);
            if(PARAMETER_NAME == "R_PS_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN PRI AND SEC BASED ON WINDING ")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real();
            if(PARAMETER_NAME == "X_PS_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN PRI AND SEC BASED ON WINDING NOMINALS IN PU")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag();
            if(PARAMETER_NAME == "R_ST_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN SEC AND TER BASED ON WINDING NOMINALS IN PU")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).real();
            if(PARAMETER_NAME == "X_ST_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN SEC AND TER BASED ON WINDING NOMINALS IN PU")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).imag();
            if(PARAMETER_NAME == "R_TP_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN TER AND PRI BASED ON WINDING NOMINALS IN PU")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE).real();
            if(PARAMETER_NAME == "X_TP_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN TER AND PRI BASED ON WINDING NOMINALS IN PU")
                return transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE).imag();
            if(PARAMETER_NAME == "VSTAR_PU" or PARAMETER_NAME == "STAR BUS VOLTAGE IN PU")
                return steps_fast_complex_abs(transptr->get_star_bus_complex_voltage_in_pu());
            if(PARAMETER_NAME == "ASTAR_DEG" or PARAMETER_NAME == "STAR BUS ANGLE IN DEG")
                return rad2deg(steps_fast_complex_arg(transptr->get_star_bus_complex_voltage_in_pu()));
            if(PARAMETER_NAME=="FRAC1" or PARAMETER_NAME=="FRAC2" or PARAMETER_NAME=="FRAC3" or PARAMETER_NAME=="FRAC4")
                return get_owner_fraction_of_device(transptr, parameter_name);
        }
        else
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="VN_KV" or PARAMETER_NAME=="NOMINAL VOLTAGE IN KV")
                return transptr->get_winding_nominal_voltage_in_kV(winding);
            if(PARAMETER_NAME=="TAP_PU" or PARAMETER_NAME=="TURN RATIO IN PU")
                return transptr->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding);
            if(PARAMETER_NAME=="ANGLE_DEG" or PARAMETER_NAME=="ANGLE SHIFT IN DEG")
                return transptr->get_winding_angle_shift_in_deg(winding);
            if(PARAMETER_NAME=="RATE_A_MVA" or PARAMETER_NAME=="RATING A IN MVA")
                return transptr->get_winding_rating_in_MVA(winding).get_rating_A_MVA();
            if(PARAMETER_NAME=="RATE_B_MVA" or PARAMETER_NAME=="RATING B IN MVA")
                return transptr->get_winding_rating_in_MVA(winding).get_rating_B_MVA();
            if(PARAMETER_NAME=="RATE_C_MVA" or PARAMETER_NAME=="RATING C IN MVA")
                return transptr->get_winding_rating_in_MVA(winding).get_rating_C_MVA();
            if(PARAMETER_NAME=="TAP_MAX_PU" or PARAMETER_NAME=="MAX TURN RATIO BASED ON  NOMINAL V IN PU")
                return transptr->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding);
            if(PARAMETER_NAME=="TAP_MIN_PU" or PARAMETER_NAME=="MIN TURN RATIO BASED ON  NOMINAL V IN PU")
                return transptr->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding);
            if(PARAMETER_NAME=="ANGLE_MAX_DEG" or PARAMETER_NAME=="MAX ANGLE SHIFT IN DEG")
                return transptr->get_winding_max_angle_shift_in_deg(winding);
            if(PARAMETER_NAME=="ANGLE_MIN_DEG" or PARAMETER_NAME=="MIN ANGLE SHIFT IN DEG")
                return transptr->get_winding_min_angle_shift_in_deg(winding);
            if(PARAMETER_NAME=="VMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX V IN PU")
                return transptr->get_winding_controlled_max_voltage_in_pu(winding);
            if(PARAMETER_NAME=="VMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN V IN PU")
                return transptr->get_winding_controlled_min_voltage_in_pu(winding);
            if(PARAMETER_NAME=="QMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX Q INTO WINDING IN MVAR ")
                return transptr->get_controlled_max_reactive_power_into_winding_in_MVar(winding);
            if(PARAMETER_NAME=="QMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN Q INTO WINDING IN MVAR")
                return transptr->get_controlled_min_reactive_power_into_winding_in_MVar(winding);
            if(PARAMETER_NAME=="PMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX P INTO WINDING IN MW")
                return transptr->get_controlled_max_active_power_into_winding_in_MW(winding);
            if(PARAMETER_NAME=="PMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN P INTO WINDING IN MW")
                return transptr->get_controlled_min_active_power_into_winding_in_MW(winding);
            if(PARAMETER_NAME=="K_PU" or PARAMETER_NAME=="OFF NOMINAL TURN RATIO IN PU")
                return transptr->get_winding_off_nominal_turn_ratio_in_pu(winding);

            if(PARAMETER_NAME=="VSTAR_KV" or PARAMETER_NAME=="STAR BUS VOLTAGE IN KV")
                return steps_fast_complex_abs(transptr->get_star_bus_complex_voltage_in_kV_based_on_winding_nominal_voltage(winding));

            if(PARAMETER_NAME=="I_PU" or PARAMETER_NAME=="CURRENT IN PU")
                return steps_fast_complex_abs(transptr->get_winding_complex_current_in_pu(winding));

            if(PARAMETER_NAME=="I_KA" or PARAMETER_NAME=="CURRENT IN KA")
                return steps_fast_complex_abs(transptr->get_winding_complex_current_in_kA(winding));

            if(PARAMETER_NAME=="P_PU" or PARAMETER_NAME=="ACTIVE POWER IN PU")
                return transptr->get_winding_complex_power_in_pu(winding).real();

            if(PARAMETER_NAME=="Q_PU" or PARAMETER_NAME=="REACTIVE POWER IN PU")
                return transptr->get_winding_complex_power_in_pu(winding).imag();

            if(PARAMETER_NAME=="P_MW" or PARAMETER_NAME=="ACTIVE POWER IN MW")
                return transptr->get_winding_complex_power_in_MVA(winding).real();

            if(PARAMETER_NAME=="Q_MVAR" or PARAMETER_NAME=="REACTIVE POWER IN MVAR")
                return transptr->get_winding_complex_power_in_MVA(winding).imag();

            if(PARAMETER_NAME=="I_KA" or PARAMETER_NAME=="CURRENT IN KA")
                return abs(transptr->get_winding_complex_current_in_kA(winding));
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



void api_set_transformer_float_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name, double value)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "TRANSFORMER" and SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(SIDE == "TRANSFORMER")
        {
            if(PARAMETER_NAME == "GM_PU" or PARAMETER_NAME == "MAGNETIZING G BASED ON PRI WINDING BUS BASE V AND SYSTEM BASE S IN PU")
            {
                double b = transptr->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().imag();
                transptr->set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(value,b));
                return;
            }
            if(PARAMETER_NAME == "BM_PU" or PARAMETER_NAME == "MAGNETIZING B BASED ON PRI WINDING BUS BASE V AND SYSTEM BASE S IN PU")
            {
                double g = transptr->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().real();
                transptr->set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(g,value));
                return;
            }
            if(PARAMETER_NAME == "SBASE_PS_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN PRI AND SEC IN MVA")
            {
                transptr->set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE,value);
                return;
            }

            if(PARAMETER_NAME == "SBASE_ST_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN SEC AND TER IN MVA")
            {
                transptr->set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE,value);
                return;
            }
            if(PARAMETER_NAME == "SBASE_TP_MVA" or PARAMETER_NAME == "NOMINAL CAPACITY BETWEEN TER AND PRI IN MVA")
            {
                transptr->set_winding_nominal_capacity_in_MVA(TERTIARY_SIDE, PRIMARY_SIDE,value);
                return;
            }
            if(PARAMETER_NAME == "R_PS_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN PRI AND SEC BASED ON WINDING ")
            {
                double x = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE,complex<double>(value,x));
                return;
            }
            if(PARAMETER_NAME == "X_PS_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN PRI AND SEC BASED ON WINDING NOMINALS IN PU")
            {
                double r = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE,complex<double>(r,value));
                return;
            }
            if(PARAMETER_NAME == "R_ST_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN SEC AND TER BASED ON WINDING NOMINALS IN PU")
            {
                double x = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).imag();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE,complex<double>(value,x));
                return;
            }
            if(PARAMETER_NAME == "X_ST_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN SEC AND TER BASED ON WINDING NOMINALS IN PU")
            {
                double r = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).real();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE,complex<double>(r,value));
                return;
            }
            if(PARAMETER_NAME == "R_TP_PU" or PARAMETER_NAME == "LEAKAGE R BETWEEN TER AND PRI BASED ON WINDING NOMINALS IN PU")
            {
                double x = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE).imag();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE,complex<double>(value,x));
                return;
            }
            if(PARAMETER_NAME == "X_TP_PU" or PARAMETER_NAME == "LEAKAGE X BETWEEN TER AND PRI BASED ON WINDING NOMINALS IN PU")
            {
                double r = transptr->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE).real();
                transptr->set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE,complex<double>(r,value));
                return;
            }
            if(PARAMETER_NAME=="FRAC1" or PARAMETER_NAME=="FRAC2" or PARAMETER_NAME=="FRAC3" or PARAMETER_NAME=="FRAC4")
                return set_owner_fraction_of_device(transptr, PARAMETER_NAME, value);
        }
        else
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="VN_KV" or PARAMETER_NAME=="NOMINAL VOLTAGE IN KV")
            {
                transptr->set_winding_nominal_voltage_in_kV(winding,value);
                return;
            }
            if(PARAMETER_NAME=="TAP_PU" or PARAMETER_NAME=="TURN RATIO IN PU")
            {
                transptr->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding,value);
                return;
            }
            if(PARAMETER_NAME=="ANGLE_DEG" or PARAMETER_NAME=="ANGLE SHIFT IN DEG")
            {
                transptr->set_winding_angle_shift_in_deg(winding,value);
                return;
            }
            if(PARAMETER_NAME=="RATE_A_MVA" or PARAMETER_NAME=="RATING A IN MVA")
            {
                RATING rating = transptr->get_winding_rating_in_MVA(winding);
                rating.set_rating_A_MVA(value);
                transptr->set_winding_rating_in_MVA(winding,rating);
                return;
            }

            if(PARAMETER_NAME=="RATE_B_MVA" or PARAMETER_NAME=="RATING B IN MVA")
            {
                RATING rating = transptr->get_winding_rating_in_MVA(winding);
                rating.set_rating_B_MVA(value);
                transptr->set_winding_rating_in_MVA(winding,rating);
                return;
            }

            if(PARAMETER_NAME=="RATE_C_MVA" or PARAMETER_NAME=="RATING C IN MVA")
            {
                RATING rating = transptr->get_winding_rating_in_MVA(winding);
                rating.set_rating_C_MVA(value);
                transptr->set_winding_rating_in_MVA(winding,rating);
                return;
            }

            if(PARAMETER_NAME=="TAP_MAX_PU" or PARAMETER_NAME=="MAX TURN RATIO BASED ON  NOMINAL V IN PU")
                transptr->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding,value);
            if(PARAMETER_NAME=="TAP_MIN_PU" or PARAMETER_NAME=="MIN TURN RATIO BASED ON  NOMINAL V IN PU")
                return transptr->set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding,value);
            if(PARAMETER_NAME=="ANGLE_MAX_DEG" or PARAMETER_NAME=="MAX ANGLE SHIFT IN DEG")
                return transptr->set_winding_max_angle_shift_in_deg(winding,value);
            if(PARAMETER_NAME=="ANGLE_MIN_DEG" or PARAMETER_NAME=="MIN ANGLE SHIFT IN DEG")
                return transptr->set_winding_min_angle_shift_in_deg(winding,value);
            if(PARAMETER_NAME=="VMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX V IN PU")
                return transptr->set_winding_controlled_max_voltage_in_pu(winding,value);
            if(PARAMETER_NAME=="VMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN V IN PU")
                return transptr->set_winding_controlled_min_voltage_in_pu(winding,value);
            if(PARAMETER_NAME=="QMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX Q INTO WINDING IN MVAR ")
                return transptr->set_controlled_max_reactive_power_into_winding_in_MVar(winding,value);
            if(PARAMETER_NAME=="QMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN Q INTO WINDING IN MVAR")
                return transptr->set_controlled_min_reactive_power_into_winding_in_MVar(winding,value);
            if(PARAMETER_NAME=="PMAX_PU" or PARAMETER_NAME=="CONTROLLED MAX P INTO WINDING IN MW")
                return transptr->set_controlled_max_active_power_into_winding_in_MW(winding,value);
            if(PARAMETER_NAME=="PMIN_PU" or PARAMETER_NAME=="CONTROLLED MIN P INTO WINDING IN MW")
                return transptr->set_controlled_min_active_power_into_winding_in_MW(winding,value);

        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

const char* api_get_transformer_string_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

	snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "TRANSFORMER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return STEPS::steps_char_buffer;
        }
        else
        {
			if (PARAMETER_NAME == "ID" or PARAMETER_NAME == "IDENTIFIER")
			{
				snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (transptr->get_identifier()).c_str());
				return STEPS::steps_char_buffer;
			}
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


void api_set_transformer_string_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name, char* value)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "TRANSFORMER")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        else
        {
            if(PARAMETER_NAME=="ID" or PARAMETER_NAME=="IDENTIFIER")
            {
                transptr->set_identifier(value);
                return;
            }

        }

        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}

bool api_get_transformer_boolean_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return false;
        }
         if(SIDE == "PRIMARY" or SIDE == "SECONDARY" or SIDE == "TERTIARY")
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="STATUS" or PARAMETER_NAME=="BREAKER STATUS")
                return transptr->get_winding_breaker_status(winding);
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


void api_set_transformer_boolean_data(size_t ibus, size_t jbus, size_t kbus, char* identifier, char* side, char* parameter_name, bool value)
{
    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();

    TRANSFORMER* transptr = psdb->get_transformer(did);
    if(transptr!=NULL)
    {
        string SIDE = string2upper(side);
        string PARAMETER_NAME = string2upper(parameter_name);

        if(SIDE != "PRIMARY" and SIDE != "SECONDARY" and SIDE != "TERTIARY")
        {
            show_side_not_supported_for_device_with_api(SIDE, did, __FUNCTION__);
            return;
        }

        if(SIDE == "PRIMARY" or SIDE == "SECONDARY" or SIDE == "TERTIARY")
        {
            TRANSFORMER_WINDING_SIDE winding;
            if(SIDE == "PRIMARY")
                winding = PRIMARY_SIDE;
            else
            {
                if(SIDE=="SECONDARY")
                    winding = SECONDARY_SIDE;
                else
                    winding = TERTIARY_SIDE;
            }

            if(PARAMETER_NAME=="STATUS" or PARAMETER_NAME=="BREAKER STATUS")
            {
                transptr->set_winding_breaker_status(winding,value);
                return;
            }
        }
        show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
        return;
    }
    else
        show_device_not_exist_with_api(did, __FUNCTION__);
}
