#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

const char* api_get_generator_related_model_name(size_t bus, char* identifier, char* model_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
	snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return toolkit.steps_char_buffer;
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        SYNC_GENERATOR_MODEL* model = generator->get_sync_generator_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;;
    }
    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
		if (model != NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
		return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
        return toolkit.steps_char_buffer;
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        if(model!=NULL)
			snprintf(toolkit.steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (model->get_model_name()).c_str());
        return toolkit.steps_char_buffer;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return toolkit.steps_char_buffer;
}

double api_get_generator_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return 0.0;
    }
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        SYNC_GENERATOR_MODEL* model = generator->get_sync_generator_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }

    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        if(model!=NULL)
            return model->get_model_data_with_name(PARAMETER_NAME);
        else
            return 0.0;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return 0.0;
}

void api_set_generator_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name, double value, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return;
    }
    string MODEL_TYPE = string2upper(model_type);
    string PARAMETER_NAME = string2upper(parameter_name);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        SYNC_GENERATOR_MODEL* model = generator->get_sync_generator_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }

    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        if(model!=NULL)
            return model->set_model_data_with_name(PARAMETER_NAME, value);
        else
            return;
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
}


