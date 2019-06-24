#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

const char* api_get_pv_unit_related_model_name(size_t bus, char* identifier, char* model_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    DEVICE_ID did = get_pv_unit_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv_unit = psdb.get_pv_unit(did);
    if(pv_unit==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="PVC" or MODEL_TYPE=="PV CONVERTER")
    {
        PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="PVP" or MODEL_TYPE=="PV PANEL")
    {
        PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="PVELEC" or MODEL_TYPE=="PV ELECTRICAL")
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="PVIRRD" or MODEL_TYPE=="PV IRRADIANCE")
    {
        PV_IRRADIANCE_MODEL* model = pv_unit->get_pv_irradiance_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
	return toolkit.steps_char_buffer;
}

double api_get_pv_unit_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name, size_t toolkit_index)
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
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="PVC" or MODEL_TYPE=="PV CONVERTER")
    {
        PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }

    if(MODEL_TYPE=="PVP" or MODEL_TYPE=="PV PANEL")
    {
        PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }

    if(MODEL_TYPE=="PVELEC" or MODEL_TYPE=="PV ELECTRICAL")
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    if(MODEL_TYPE=="PVIRRD" or MODEL_TYPE=="PV IRRADIANCE")
    {
        PV_IRRADIANCE_MODEL* model = pv_unit->get_pv_irradiance_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return 0.0;
}

void api_set_pv_unit_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name, double value, size_t toolkit_index)
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
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="PVC" or MODEL_TYPE=="PV CONVERTER")
    {
        PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }

    if(MODEL_TYPE=="PVP" or MODEL_TYPE=="PV PANEL")
    {
        PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    if(MODEL_TYPE=="PVELEC" or MODEL_TYPE=="PV ELECTRICAL")
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    if(MODEL_TYPE=="PVIRRD" or MODEL_TYPE=="PV IRRADIANCE")
    {
        PV_IRRADIANCE_MODEL* model = pv_unit->get_pv_irradiance_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
}
