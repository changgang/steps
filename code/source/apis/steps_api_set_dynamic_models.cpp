#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_set_dynamic_model(char* model_string, char* file_type)
{
    string data(model_string);
    string FileType(file_type);
    FileType = string2upper(FileType);
    if(FileType=="PSSE" or FileType=="PSS/E")
	{
	    PSSE_IMEXPORTER importer;
        importer.load_one_model(data);
	}
	else
    {
        if(FileType=="BPA")
        {
            BPA_IMEXPORTER importer;
            importer.load_one_model(data);
        }
        else
        {
            ostringstream stream;
            stream<<"File Type '"<<file_type<<"' is not supported for loading dynamic model with "<<__FUNCTION__<<"()";
            show_information_with_leading_time_stamp(stream);
        }
    }
}
