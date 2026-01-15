#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

const char* api_get_mt_hvdc_related_model_name(char* mtdc_name, char* model_type, char* device_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    MT_HVDC* mtdc = psdb.get_pointer_to_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="MTDC" or MODEL_TYPE=="MT HVDC")
    {
        MT_DC_GRID_MODEL* model = mtdc->get_mtdc_network_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="LCC" or MODEL_TYPE=="LCC CONVERTER")
    {
        HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
        HLCC_MODEL* model = lcc->get_hlcc_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;;
    }
    if(MODEL_TYPE=="VSC" or MODEL_TYPE=="VSC CONVERTER")
    {
        HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
        HVSC_MODEL* model = vsc->get_hvsc_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

double api_get_mt_hvdc_related_model_float_parameter(char* mtdc_name, char* model_type, char* device_name, char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    MT_HVDC* mtdc = psdb.get_pointer_to_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="MTDC" or MODEL_TYPE=="MT HVDC")
    {
        MT_DC_GRID_MODEL* model = mtdc->get_mtdc_network_model();
        if(model!=NULL)
			return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    if(MODEL_TYPE=="LCC" or MODEL_TYPE=="LCC CONVERTER")
    {
        HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
        HLCC_MODEL* model = lcc->get_hlcc_model();
        if(model!=NULL)
			return model->get_model_data_with_name(PARAMETER_NAME);
		return 0.0;
    }
    if(MODEL_TYPE=="VSC" or MODEL_TYPE=="VSC CONVERTER")
    {
        HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
        HVSC_MODEL* model = vsc->get_hvsc_model();
        if(model!=NULL)
			return model->get_model_data_with_name(PARAMETER_NAME);;
		return 0.0;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return 0.0;
}

void api_set_mt_hvdc_related_model_float_parameter(char* mtdc_name, char* model_type, char* device_name, char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    MT_HVDC* mtdc = psdb.get_pointer_to_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="MTDC" or MODEL_TYPE=="MT HVDC")
    {
        MT_DC_GRID_MODEL* model = mtdc->get_mtdc_network_model();
        if(model!=NULL)
			return model->set_model_data_with_name(PARAMETER_NAME,value);
        else
            return;
    }
    if(MODEL_TYPE=="LCC" or MODEL_TYPE=="LCC CONVERTER")
    {
        HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
        HLCC_MODEL* model = lcc->get_hlcc_model();
        if(model!=NULL)
			return model->set_model_data_with_name(PARAMETER_NAME,value);
		return;
    }
    if(MODEL_TYPE=="VSC" or MODEL_TYPE=="VSC CONVERTER")
    {
        HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
        HVSC_MODEL* model = vsc->get_hvsc_model();
        if(model!=NULL)
			return model->set_model_data_with_name(PARAMETER_NAME,value);
		return;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return;
}

unsigned int api_get_mt_hvdc_related_model_float_parameter_count(char* mtdc_name, char* model_type, char* device_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    MT_HVDC* mtdc = psdb.get_pointer_to_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0;
    }
    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="MTDC" or MODEL_TYPE=="MT HVDC")
    {
        MT_DC_GRID_MODEL* model = mtdc->get_mtdc_network_model();
        if(model!=NULL)
			return model->get_model_float_parameter_count();
        else
            return 0;
    }
    if(MODEL_TYPE=="LCC" or MODEL_TYPE=="LCC CONVERTER")
    {
        HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
        HLCC_MODEL* model = lcc->get_hlcc_model();
        if(model!=NULL)
			return model->get_model_float_parameter_count();
		return 0;
    }
    if(MODEL_TYPE=="VSC" or MODEL_TYPE=="VSC CONVERTER")
    {
        HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
        HVSC_MODEL* model = vsc->get_hvsc_model();
        if(model!=NULL)
			return model->get_model_float_parameter_count();
		return 0;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return 0;
}

const char* api_get_mt_hvdc_related_model_float_parameter_name(char* mtdc_name, char* model_type, unsigned int parameter_index, char* device_name, unsigned int toolkit_index)
{
    string par_name = "";
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    MT_HVDC* mtdc = psdb.get_pointer_to_mt_hvdc(mtdc_name);
    DEVICE_ID did = mtdc->get_device_id();
    if(mtdc==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", par_name.c_str());
        return toolkit.steps_char_buffer;
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="MTDC" or MODEL_TYPE=="MT HVDC")
    {
        MT_DC_GRID_MODEL* model = mtdc->get_mtdc_network_model();
        if(model!=NULL)
			par_name = model->get_model_data_name(parameter_index);
        snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", par_name.c_str());
        return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="LCC" or MODEL_TYPE=="LCC CONVERTER")
    {
        HLCC* lcc = mtdc->get_lcc_converter_with_name(device_name);
        HLCC_MODEL* model = lcc->get_hlcc_model();
        if(model!=NULL)
			par_name = model->get_model_data_name(parameter_index);
		snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", par_name.c_str());
        return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="VSC" or MODEL_TYPE=="VSC CONVERTER")
    {
        HVSC* vsc = mtdc->get_vsc_converter_with_name(device_name);
        HVSC_MODEL* model = vsc->get_hvsc_model();
        if(model!=NULL)
			par_name = model->get_model_data_name(parameter_index);
		snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", par_name.c_str());
        return toolkit.steps_char_buffer;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", par_name.c_str());
    return toolkit.steps_char_buffer;
}

