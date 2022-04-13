#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"


double api_get_pv_unit_sequence_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_pv_unit_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv_unit = psdb.get_pv_unit(did);

    if(pv_unit==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME == "RPOS_PU" or "POSITIVE SEQUENCE RESISTANCE IN PU")
        return pv_unit->get_positive_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XPOS_PU" or "POSITIVE SEQUENCE REACTANCE IN PU")
        return pv_unit->get_positive_sequence_reactance_in_pu();
    if(PARAMETER_NAME == "RNEG_PU" or "NEGATIVE SEQUENCE RESISTANCE IN PU")
        return pv_unit->get_negative_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XNEG_PU" or "NEGATIVE SEQUENCE REACTANCE IN PU")
        return pv_unit->get_negative_sequence_reactance_in_pu();
    if(PARAMETER_NAME == "RZERO_PU" or "ZERO SEQUENCE RESISTANCE IN PU")
        return pv_unit->get_zero_sequence_resistance_in_pu();
    if(PARAMETER_NAME == "XZERO_PU" or "ZERO SEQUENCE REACTANCE IN PU")
        return pv_unit->get_zero_sequence_reactance_in_pu();
    if(PARAMETER_NAME == "RG_PU" or "GROUNDING RESISTANCE IN PU")
        return pv_unit->get_grounding_resistance_in_pu();
    if(PARAMETER_NAME == "XG_PU" or "GROUNDING REACTANCE IN PU")
        return pv_unit->get_grounding_reactance_in_pu();


    if(PARAMETER_NAME=="I1_PU" or "POSITIVE SEQUENCE CURRENT IN PU")
        return abs(pv_unit->get_positive_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I2_PU" or "NEGATIVE SEQUENCE CURRENT IN PU")
        return abs(pv_unit->get_negative_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I0_PU" or "ZERO SEQUENCE CURRENT IN PU")
        return abs(pv_unit->get_zero_sequence_complex_current_in_pu());
    if(PARAMETER_NAME=="I1_KA" or "POSITIVE SEQUENCE CURRENT IN KA")
        return abs(pv_unit->get_positive_sequence_complex_current_in_kA());
    if(PARAMETER_NAME=="I2_KA" or "NEGATIVE SEQUENCE CURRENT IN KA")
        return abs(pv_unit->get_negative_sequence_complex_current_in_kA());
    if(PARAMETER_NAME=="I0_KA" or "ZERO SEQUENCE CURRENT IN KA")
        return abs(pv_unit->get_zero_sequence_complex_current_in_kA());

    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
}
void api_set_pv_unit_sequence_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_pv_unit_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv_unit = psdb.get_pv_unit(did);

    if(pv_unit==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME == "RPOS_PU" or "POSITIVE SEQUENCE RESISTANCE IN PU")
        return pv_unit->set_positive_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XSDV_PU" or "POSITIVE SEQUENCE REACTANCE IN PU")
        return pv_unit->set_positive_sequence_reactance_in_pu(value);
    if(PARAMETER_NAME == "RNEG_PU" or "NEGATIVE SEQUENCE RESISTANCE IN PU")
        return pv_unit->set_negative_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XNEG_PU" or "NEGATIVE SEQUENCE REACTANCE IN PU")
        return pv_unit->set_negative_sequence_reactance_in_pu(value);
    if(PARAMETER_NAME == "RZERO_PU" or "ZERO SEQUENCE RESISTANCE IN PU")
        return pv_unit->set_zero_sequence_resistance_in_pu(value);
    if(PARAMETER_NAME == "XZERO_PU" or "ZERO SEQUENCE REACTANCE IN PU")
        return pv_unit->set_zero_sequence_reactance_in_pu(value);
    if(PARAMETER_NAME == "RG_PU" or "GROUNDING RESISTANCE IN PU")
        return pv_unit->set_grounding_resistance_in_pu(value);
    if(PARAMETER_NAME == "XG_PU" or "GROUNDING REACTANCE IN PU")
        return pv_unit->set_grounding_reactance_in_pu(value);

    show_parameter_not_supported_for_device_with_api(PARAMETER_NAME, did, __FUNCTION__);
}
