#include "header/apis/steps_api.h"
#include "header/apis/steps_api_common.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"
#include "header/STEPS.h"

void api_set_dynamic_model(char* model_string, char* file_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string data(model_string);
    string FileType(file_type);
    FileType = string2upper(FileType);
    if(FileType=="PSSE" or FileType=="PSS/E")
	{
	    PSSE_IMEXPORTER importer(toolkit);
        importer.load_one_model(data);
	}
	else
    {
        if(FileType=="BPA")
        {
            BPA_IMEXPORTER importer(toolkit);
            importer.load_one_model(data);
        }
        else
        {
            ostringstream stream;
            stream<<"File Type '"<<file_type<<"' is not supported for loading dynamic model with "<<__FUNCTION__<<"()";
            toolkit.show_information_with_leading_time_stamp(stream);
        }
    }
}


void api_disable_generator_related_model(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__, toolkit_index);
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        generator->disable_mode_of_type("SYNC GENERATOR");
		return;
    }
    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        generator->disable_mode_of_type("EXCITER");
		return;
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        generator->disable_mode_of_type("STABILIZER");
		return;
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        generator->disable_mode_of_type("TURBINE GOVERNOR");
		return;
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        generator->disable_mode_of_type("COMPENSATOR");
		return;
    }
    if(MODEL_TYPE=="TLC" or MODEL_TYPE=="TURBINE LOAD CONTROLLER")
    {
        generator->disable_mode_of_type("TURBINE LOAD CONTROLLER");
		return;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
}

void api_enable_generator_related_model(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
	snprintf(toolkit.steps_char_buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    DEVICE_ID did = get_generator_device_id(bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(did);
    if(generator==NULL)
    {
        show_device_not_exist_with_api(did, __FUNCTION__, toolkit_index);
    }

    string MODEL_TYPE = string2upper(model_type);
    if(MODEL_TYPE=="GEN" or MODEL_TYPE=="SYNC GENERATOR")
    {
        generator->enable_mode_of_type("SYNC GENERATOR");
		return;
    }
    if(MODEL_TYPE=="AVR" or MODEL_TYPE=="EXCITER")
    {
        generator->enable_mode_of_type("EXCITER");
		return;
    }
    if(MODEL_TYPE=="PSS" or MODEL_TYPE=="STABILIZER")
    {
        generator->enable_mode_of_type("STABILIZER");
		return;
    }
    if(MODEL_TYPE=="GOV" or MODEL_TYPE=="TURBINE GOVERNOR")
    {
        generator->enable_mode_of_type("TURBINE GOVERNOR");
		return;
    }
    if(MODEL_TYPE=="COMP" or MODEL_TYPE=="COMPENSATOR")
    {
        generator->enable_mode_of_type("COMPENSATOR");
		return;
    }
    if(MODEL_TYPE=="TLC" or MODEL_TYPE=="TURBINE LOAD CONTROLLER")
    {
        generator->enable_mode_of_type("TURBINE LOAD CONTROLLER");
		return;
    }
    show_model_type_not_supported_for_device_with_api(MODEL_TYPE, did, __FUNCTION__);
}

