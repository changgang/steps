#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"


double api_get_generator_sequence_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);

    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME == "RPOS_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE RESISTANCE IN PU")
        return generator->get_positive_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XPPDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE SUBTRANSIENT REACTANCE IN PU")
        return generator->get_positive_sequence_subtransient_reactance_in_pu();
    if(PARAMETER_NAME == "XPDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE TRANSIENT REACTANCE IN PU")
        return generator->get_positive_sequence_transient_reactance_in_pu();
    if(PARAMETER_NAME == "XSDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE SYNCHRONOUS REACTANCE IN PU")
        return generator->get_positive_sequence_syncronous_reactance_in_pu();
    if(PARAMETER_NAME == "RNEG_PU" or PARAMETER_NAME == "NEGATIVE SEQUENCE RESISTANCE IN PU")
        return generator->get_negative_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XNEG_PU" or PARAMETER_NAME == "NEGATIVE SEQUENCE REACTANCE IN PU")
        return generator->get_negative_sequence_reactance_in_pu();
    if(PARAMETER_NAME == "RZERO_PU" or PARAMETER_NAME == "ZERO SEQUENCE RESISTANCE IN PU")
        return generator->get_zero_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XZERO_PU" or PARAMETER_NAME == "ZERO SEQUENCE REACTANCE IN PU")
        return generator->get_zero_sequence_reactance_in_pu();
    if(PARAMETER_NAME == "RG_PU" or PARAMETER_NAME == "GROUNDING RESISTANCE IN PU")
        return generator->get_grounding_resistance_in_pu();
    if(PARAMETER_NAME == "XG_PU" or PARAMETER_NAME == "GROUNDING REACTANCE IN PU")
        return generator->get_grounding_reactance_in_pu();

    if(PARAMETER_NAME == "I1_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE CURRENT IN PU")
        return abs(generator->get_positive_sequence_complex_current_in_pu());
    if(PARAMETER_NAME == "I2_PU" or PARAMETER_NAME == "NEGATIVE SEQUENCE CURRENT IN PU")
        return abs(generator->get_negative_sequence_complex_current_in_pu());
    if(PARAMETER_NAME == "I0_PU" or PARAMETER_NAME == "ZERO SEQUENCE CURRENT IN PU")
        return abs(generator->get_zero_sequence_complex_current_in_pu());
    if(PARAMETER_NAME == "I1_KA" or PARAMETER_NAME == "POSITIVE SEQUENCE CURRENT IN KA")
        return abs(generator->get_positive_sequence_complex_current_in_kA());
    if(PARAMETER_NAME == "I2_KA" or PARAMETER_NAME == "NEGATIVE SEQUENCE CURRENT IN KA")
        return abs(generator->get_negative_sequence_complex_current_in_kA());
    if(PARAMETER_NAME == "I0_KA" or PARAMETER_NAME == "ZERO SEQUENCE CURRENT IN KA")
        return abs(generator->get_zero_sequence_complex_current_in_kA());

    if(PARAMETER_NAME=="I1_ANG_RAD" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT ANGLE IN RAD")
        return arg(generator->get_positive_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I1_ANG_DEG" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(generator->get_positive_sequence_complex_current_in_pu()));
    if(PARAMETER_NAME=="I2_ANG_RAD" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT ANGLE IN RAD")
        return arg(generator->get_negative_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I2_ANG_DEG" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(generator->get_negative_sequence_complex_current_in_pu()));
    if(PARAMETER_NAME=="I0_ANG_RAD" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT ANGLE IN RAD")
        return arg(generator->get_zero_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I0_ANG_DEG" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(generator->get_zero_sequence_complex_current_in_pu()));

    if(PARAMETER_NAME=="E_PU" or PARAMETER_NAME=="INTERNAL VOLTAGE IN PU")
        return abs(generator->get_complex_internal_voltage_for_short_circuit_solver_in_pu());
    if(PARAMETER_NAME=="E_ANG_RAD" or PARAMETER_NAME=="INTERNAL VOLTAGE ANGLE IN RAD")
        return arg(generator->get_complex_internal_voltage_for_short_circuit_solver_in_pu());
    if(PARAMETER_NAME=="E_ANG_DEG" or PARAMETER_NAME=="INTERNAL VOLTAGE ANGLE IN DEG")
        return rad2deg(arg(generator->get_complex_internal_voltage_for_short_circuit_solver_in_pu()));

    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
    return 0.0;

}
void api_set_generator_sequence_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);

    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME == "RPOS_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE RESISTANCE IN PU")
        return generator->set_positive_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XPPDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE SUBTRANSIENT REACTANCE IN PU")
        return generator->set_positive_sequence_subtransient_reactance_in_pu(value);
    if(PARAMETER_NAME == "XPDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE TRANSIENT REACTANCE IN PU")
        return generator->set_positive_sequence_transient_reactance_in_pu(value);
    if(PARAMETER_NAME == "XSDV_PU" or PARAMETER_NAME == "POSITIVE SEQUENCE SYNCHRONOUS REACTANCE IN PU")
        return generator->set_positive_sequence_syncronous_reactance_in_pu(value);
    if(PARAMETER_NAME == "RNEG_PU" or PARAMETER_NAME == "NEGATIVE SEQUENCE RESISTANCE IN PU")
        return generator->set_negative_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XNEG_PU" or PARAMETER_NAME == "NEGATIVE SEQUENCE REACTANCE IN PU")
        return generator->set_negative_sequence_reactance_in_pu(value);
    if(PARAMETER_NAME == "RZERO_PU" or PARAMETER_NAME == "ZERO SEQUENCE RESISTANCE IN PU")
        return generator->set_zero_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XZERO_PU" or PARAMETER_NAME == "ZERO SEQUENCE REACTANCE IN PU")
        return generator->set_zero_sequence_reactance_in_pu(value);
    if(PARAMETER_NAME == "RG_PU" or PARAMETER_NAME == "GROUNDING RESISTANCE IN PU")
        return generator->set_grounding_resistance_in_pu(value);
    if(PARAMETER_NAME == "XG_PU" or PARAMETER_NAME == "GROUNDING REACTANCE IN PU")
        return generator->set_grounding_reactance_in_pu(value);

    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
}
