#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_load_powerflow_data_from_file(char* file, char* file_type)
{
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER importer;
        importer.load_powerflow_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER importer;
            importer.load_powerflow_data(file);
        }
    }
}

void api_save_powerflow_data_to_file(char* file, char* file_type)
{
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER exporter;
        exporter.export_powerflow_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER exporter;
            exporter.export_powerflow_data(file);
        }
    }
}


void api_load_dynamic_data_from_file(char* file, char* file_type)
{
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER importer;
        importer.load_dynamic_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER importer;
            importer.load_dynamic_data(file);
        }
    }
}

void api_save_dynamic_data_to_file(char* file, char* file_type)
{
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER exporter;
        exporter.export_dynamic_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER exporter;
            exporter.export_dynamic_data(file);
        }
    }
}

