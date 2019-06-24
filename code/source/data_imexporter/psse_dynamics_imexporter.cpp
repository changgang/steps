#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/steps_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/all_supported_models.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

void PSSE_IMEXPORTER::load_dynamic_data(string file)
{
    ostringstream osstream;
    osstream<<"Loading dynamic data from PSS/E file: "<<file;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    //file = string2upper(file);
    load_dynamic_data_into_ram(file);
    load_all_models();

    osstream<<"Done loading dynamic data.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSSE_IMEXPORTER::load_dynamic_data_into_ram(string file)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    dyr_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"PSS/E dyr file '"<<file<<"' is not accessible. Loading PSS/E dyr data is failed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    char buffer[10240];
    string sbuffer;

    string data_of_one_type;
    data_of_one_type.clear();
    while(true)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            if(data_of_one_type.size()!=0)
            {
                data_of_one_type = string2csv(data_of_one_type);
                dyr_data_in_ram.push_back(data_of_one_type);
                data_of_one_type.clear();
            }
            break;
        }
        sbuffer = buffer;
        size_t index_of_slash = sbuffer.find('/');
        if(index_of_slash==string::npos)
        {
            sbuffer = trim_string(sbuffer);
            data_of_one_type = data_of_one_type +" "+sbuffer;
        }
        else
        {
            sbuffer = trim_psse_comment(buffer);
            sbuffer = trim_string(sbuffer);
            if(sbuffer=="")
                continue;
            data_of_one_type = data_of_one_type +" "+sbuffer;

            data_of_one_type = string2csv(data_of_one_type);
            dyr_data_in_ram.push_back(data_of_one_type);
            data_of_one_type.clear();
        }
    }
    fclose(fid);

    //for(size_t i=0; i<dyr_data_in_ram.size(); ++i)
    //    cout<<dyr_data_in_ram[i]<<endl;
}

void PSSE_IMEXPORTER::load_all_models()
{
    size_t n = dyr_data_in_ram.size();
    for(size_t i=0; i!=n; ++i)
        load_one_model(dyr_data_in_ram[i]);
}

void PSSE_IMEXPORTER::load_one_model(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    //for(size_t i=0; i<record.size();++i)
    //    cout<<record[i]<<endl;
    STEPS_IMEXPORTER importer;
    importer.set_toolkit(get_toolkit(__PRETTY_FUNCTION__));
    importer.load_one_model(record);
    //cout<<"done. Now go to next one"<<endl;
}

void PSSE_IMEXPORTER::export_dynamic_data(string file)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. PSS/E dyr file "<<file<<" cannot be opened for exporting dynamic data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    MODEL* model=NULL;
    size_t n=0;
    vector<GENERATOR*> gens = psdb.get_all_generators();
    GENERATOR* gen = NULL;
    n = gens.size();
    for(size_t i=0; i!=n; ++i)
    {
        gen = gens[i];
        model = gen->get_sync_generator_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = gen->get_exciter_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = gen->get_stabilizer_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = gen->get_turbine_governor_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
    }
    vector<WT_GENERATOR*> wt_gens = psdb.get_all_wt_generators();
    WT_GENERATOR* wt_gen = NULL;
    n = wt_gens.size();
    for(size_t i=0; i!=n; ++i)
    {
        wt_gen = wt_gens[i];
        model = wt_gen->get_wt_generator_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wt_aerodynamic_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wt_turbine_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wt_electrical_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wt_pitch_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wind_speed_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = wt_gen->get_wt_relay_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
    }
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    HVDC* hvdc = NULL;
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];
        model = hvdc->get_hvdc_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = hvdc->get_auxiliary_signal_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
    }
    vector<LOAD*> loads = psdb.get_all_loads();
    LOAD* load = NULL;
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        model = load->get_load_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = load->get_load_frequency_relay_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
        model = load->get_load_voltage_relay_model();
        if(model!=NULL)
            ofs<<model->get_standard_model_string()<<"\n";
    }
    ofs.close();
}
