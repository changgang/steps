#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

const char* api_get_generator_related_model_name(size_t bus, char* identifier, char* model_type)
{
    string BLANK = "";
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    GENERATOR* generator = psdb->get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__);
        return BLANK.c_str();
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        SYNC_GENERATOR_MODEL* model = generator->get_sync_generator_model();
        return model->get_model_name().c_str();
    }
    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
        return model->get_model_name().c_str();
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        return model->get_model_name().c_str();
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        return model->get_model_name().c_str();
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        return model->get_model_name().c_str();
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return BLANK.c_str();
}

double api_get_generator_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name)
{
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    GENERATOR* generator = psdb->get_generator(did);
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
        return model->get_double_data_with_name(PARAMETER_NAME);
    }

    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
        return model->get_double_data_with_name(PARAMETER_NAME);
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        return model->get_double_data_with_name(PARAMETER_NAME);
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        return model->get_double_data_with_name(PARAMETER_NAME);
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        return model->get_double_data_with_name(PARAMETER_NAME);
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
    return 0.0;
}

void api_set_generator_related_model_float_parameter(size_t bus, char* identifier, char* model_type, char* parameter_name, double value)
{
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    GENERATOR* generator = psdb->get_generator(did);
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
        return model->set_double_data_with_name(PARAMETER_NAME, value);
    }

    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        EXCITER_MODEL* model = generator->get_exciter_model();
        return model->set_double_data_with_name(PARAMETER_NAME, value);
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        STABILIZER_MODEL* model = generator->get_stabilizer_model();
        return model->set_double_data_with_name(PARAMETER_NAME, value);
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        return model->set_double_data_with_name(PARAMETER_NAME, value);
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* model = generator->get_compensator_model();
        return model->set_double_data_with_name(PARAMETER_NAME, value);
    }
    show_parameter_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
}


