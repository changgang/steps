#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_load_powerflow_data_from_file(char* file, char* file_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER importer;
        importer.set_toolkit(toolkit);
        importer.load_powerflow_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER importer;
            importer.set_toolkit(toolkit);
            importer.load_powerflow_data(file);
        }
    }
}

void api_save_powerflow_data_to_file(char* file, char* file_type, bool export_zero_impedance_line, bool export_out_of_service_bus, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER exporter;
        exporter.set_toolkit(toolkit);
        exporter.set_export_out_of_service_bus_logic(export_out_of_service_bus);
        exporter.export_powerflow_data(file, export_zero_impedance_line);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER exporter;
            exporter.set_toolkit(toolkit);
            exporter.set_export_out_of_service_bus_logic(export_out_of_service_bus);
            exporter.export_powerflow_data(file, export_zero_impedance_line);
        }
    }
}


void api_load_dynamic_data_from_file(char* file, char* file_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER importer;
        importer.set_toolkit(toolkit);
        importer.load_dynamic_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER importer;
            importer.set_toolkit(toolkit);
            importer.load_dynamic_data(file);
        }
    }
}

void api_save_dynamic_data_to_file(char* file, char* file_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    string string_file_type = string2upper(file_type);
    if(string_file_type=="PSSE" or string_file_type=="PSS/E")
    {
        PSSE_IMEXPORTER exporter;
        exporter.set_toolkit(toolkit);
        exporter.export_dynamic_data(file);
    }
    else
    {
        if(string_file_type=="BPA")
        {
            BPA_IMEXPORTER exporter;
            exporter.set_toolkit(toolkit);
            exporter.export_dynamic_data(file);
        }
    }
}

