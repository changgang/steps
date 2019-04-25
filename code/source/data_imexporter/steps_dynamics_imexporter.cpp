#include "header/data_imexporter/steps_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/all_supported_models.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

void STEPS_IMEXPORTER::load_dynamic_data(string file)
{
    ostringstream osstream;
    osstream<<"Loading dynamic data from STEPS file: "<<file;
    show_information_with_leading_time_stamp(osstream);

    //file = string2upper(file);
    load_dynamic_data_into_ram(file);
    load_all_models();

    osstream<<"Done loading dynamic data.";
    show_information_with_leading_time_stamp(osstream);
}

void STEPS_IMEXPORTER::load_dynamic_data_from_steps_vector(vector<vector<string> >& data)
{
    splitted_sdyr_data_in_ram = data;
    load_all_models();
}

void STEPS_IMEXPORTER::load_dynamic_data_into_ram(string file)
{
    ostringstream osstream;

    splitted_sdyr_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"STEPS dyr file '"<<file<<"' is not accessible. Loading STEPS dyr data is failed.";
        show_information_with_leading_time_stamp(osstream);
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
                splitted_sdyr_data_in_ram.push_back(split_string(data_of_one_type,","));
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
            sbuffer = trim_steps_comment(buffer);
            sbuffer = trim_string(sbuffer);
            data_of_one_type = data_of_one_type +" "+sbuffer;

            data_of_one_type = string2csv(data_of_one_type);
            splitted_sdyr_data_in_ram.push_back(split_string(data_of_one_type,","));
            data_of_one_type.clear();
        }
    }
    fclose(fid);

    //for(size_t i=0; i<sdyr_data_in_ram.size(); ++i)
    //    cout<<sdyr_data_in_ram[i]<<endl;
}

void STEPS_IMEXPORTER::load_all_models()
{
    size_t n = splitted_sdyr_data_in_ram.size();
    for(size_t i=0; i!=n; ++i)
        load_one_model(splitted_sdyr_data_in_ram[i]);
}

void STEPS_IMEXPORTER::load_one_model(vector<string>& data)
{
    ostringstream osstream;
    //osstream<< "Now go parsing dynamic data: "<<data;
    //show_information_with_leading_time_stamp(osstream);

    string model_name = get_dynamic_model_name(data);
    if(model_name=="")
        return;
    if(model_name=="GENCLS") { add_GENCLS_model(data); return;}
    if(model_name=="GENROU") { add_GENROU_model(data); return;}
    if(model_name=="GENSAL") { add_GENSAL_model(data); return;}

    if(model_name=="COMP") { add_COMP_model(data); return;}

    if(model_name=="IEE2ST") { add_IEE2ST_model(data); return;}
    if(model_name=="PSASPS1") { add_PSASPS1_model(data); return;}
    if(model_name=="PSASPS2") { add_PSASPS2_model(data); return;}
    if(model_name=="PSASPS3") { add_PSASPS3_model(data); return;}
    if(model_name=="PSASPS4") { add_PSASPS4_model(data); return;}
    if(model_name=="PSASPS5") { add_PSASPS5_model(data); return;}

    if(model_name=="SEXS") { add_SEXS_model(data); return;}
    if(model_name=="IEEET1") { add_IEEET1_model(data); return;}
    if(model_name=="PSASPE1") { add_PSASPE1_model(data); return;}
    if(model_name=="PSASPE2") { add_PSASPE2_model(data); return;}
    if(model_name=="CSEET1") { add_CSEET1_model(data); return;}
    if(model_name=="CSEET2") { add_CSEET2_model(data); return;}
    if(model_name=="PSASPE13") { add_PSASPE13_model(data); return;}

    if(model_name=="TGOV1") { add_TGOV1_model(data); return;}
    if(model_name=="IEEEG1") { add_IEEEG1_model(data); return;}
    if(model_name=="IEEEG2") { add_IEEEG2_model(data); return;}
    if(model_name=="IEEEG3") { add_IEEEG3_model(data); return;}
    if(model_name=="IEESGO") { add_IEESGO_model(data); return;}
    if(model_name=="IEEEG1SB") { add_IEEEG1SB_model(data); return;}

    if(model_name=="LCFB1") { add_LCFB1_model(data); return;}


    if(model_name=="IEELAL" or model_name=="IEELAR" or model_name=="IEELZN" or
       model_name=="IEELOW" or model_name=="IEELBL") { add_IEEL_model(data); return;}

    if(model_name=="UFLSAL" or model_name=="UFLSAR" or model_name=="UFLSZN" or
       model_name=="UFLSOW" or model_name=="UFLSBL") { add_UFLS_model(data); return;}
    if(model_name=="PUFLSAL" or model_name=="PUFLSAR" or model_name=="PUFLSZN" or
       model_name=="PUFLSOW" or model_name=="PUFLSBL") { add_PUFLS_model(data); return;}

    if(model_name=="CDC4T") { add_CDC4T_model(data); return;}
    if(model_name=="CDC6T") { add_CDC6T_model(data); return;}

    if(model_name=="WT3G1") { add_WT3G1_model(data); return;}
    if(model_name=="WT3G0") { add_WT3G0_model(data); return;}
    if(model_name=="WT3G2") { add_WT3G2_model(data); return;}
    if(model_name=="AERD0") { add_AERD0_model(data); return;}
    if(model_name=="WT3T0") { add_WT3T0_model(data); return;}
    if(model_name=="WT3E0") { add_WT3E0_model(data); return;}
    if(model_name=="WT3P0") { add_WT3P0_model(data); return;}
    if(model_name=="FILEWIND") { add_FILEWIND_model(data); return;}
    if(model_name=="WTRLY0") { add_WTRLY0_model(data); return;}


    osstream<<"Warning. Dynamic model '"<<model_name<<"' is not supported. Check line "<<__LINE__<<" in file "<<__FILE__;
    show_information_with_leading_time_stamp(osstream);

}

void STEPS_IMEXPORTER::load_one_model(string& data)
{
    vector<string> vecstr = split_string(data,",");
    load_one_model(vecstr);
}

void STEPS_IMEXPORTER::load_dynamic_data_from_psse_string(vector<string>& data)
{
    size_t n = data.size();
    for(size_t i=0; i<n; ++i)
    {
        vector<string> record = split_string(data[i], ",");
        string temp = record[0];
        record[0]=record[1];
        record[1]=temp;
        load_one_model(record);
    }
}

string STEPS_IMEXPORTER::get_dynamic_model_name(vector<string>& data)
{
    if(data.size()>0)
    {
        string model_name = data[0];
        model_name = get_string_data(model_name, "");
        return model_name;
    }
    else
        return "";
}

DEVICE_ID STEPS_IMEXPORTER::get_generator_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");

    if(data.size()<3)
        return did;

    size_t bus;
    string identifier;

    size_t i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_wt_generator_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");

    if(data.size()<3)
        return did;

    size_t bus;
    string identifier;

    size_t i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_load_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("LOAD");

    if(data.size()<3)
        return did;

    size_t bus;
    string identifier;

    size_t i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_line_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("LINE");

    if(data.size()<4)
        return did;

    size_t ibus, jbus;
    string identifier;

    size_t i=1;
    ibus = get_integer_data(data[i], "0"); ++i;
    jbus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}
DEVICE_ID STEPS_IMEXPORTER::get_hvdc_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("HVDC");

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(data.size()<2)
        return did;

    string hvdc_name;

    size_t i=1;
    hvdc_name = get_string_data(data[i], "0");

    size_t n = psdb.get_hvdc_count();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    for(size_t i=0; i!=n; ++i)
    {
        if(hvdc_name == hvdcs[i]->get_name())
        {
            TERMINAL terminal;
            terminal.append_bus(hvdcs[i]->get_converter_bus(RECTIFIER));
            terminal.append_bus(hvdcs[i]->get_converter_bus(INVERTER));
            did.set_device_terminal(terminal);
            did.set_device_identifier(hvdcs[i]->get_identifier());
            break;
        }
    }

    return did;
}
DEVICE_ID STEPS_IMEXPORTER::get_transformer_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");

    if(data.size()<4)
        return did;

    size_t ibus, jbus, kbus;
    string identifier;

    size_t i=0;
    ibus = get_integer_data(data[i], "0"); ++i; ++i;
    jbus = get_integer_data(data[i], "0"); ++i;
    kbus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

void STEPS_IMEXPORTER::add_GENCLS_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENCLS")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        GENCLS model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid GENCLS model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_GENROU_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENROU")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        GENROU model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid GENROU model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_GENSAL_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENSAL")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        GENSAL model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid GENSAL model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_COMP_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "COMP")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        COMP model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid COMP model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEE2ST_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEE2ST")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEE2ST model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEE2ST model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPS1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS1")
        return;

    if(data.size()<14)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPS1 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPS1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS2")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPS2 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPS2 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS3_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS3")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPS3 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPS3 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS4_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS4")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPS4 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPS4 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS5_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS5")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPS5 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPS5 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_SEXS_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "SEXS")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        SEXS model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid SEXS model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEET1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEET1")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEET1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEET1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE1")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPE1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPE1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE2")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPE2 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPE2 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_CSEET1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CSEET1")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        CSEET1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CSEET1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_CSEET2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CSEET2")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        CSEET2 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CSEET2 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE13_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE13")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        PSASPE13 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PSASPE13 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_TGOV1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "TGOV1")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        TGOV1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid TGOV1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG1")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEEG1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEEG1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG2")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEEG2 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEEG2 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG3_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG3")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEEG3 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEEG3 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_IEESGO_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEESGO")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEESGO model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEESGO model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG1SB_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG1SB")
        return;

    if(data.size()<3)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEEG1SB model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEEG1SB model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_LCFB1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "LCFB1")
        return;

    if(data.size()<12)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        LCFB1 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            generator->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid LCFB1 model is built, but will not be set for "<<generator->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

vector<LOAD*> STEPS_IMEXPORTER::get_all_loads_of(vector<string>& data)
{
    vector<LOAD*> loads;

    string model_name = get_dynamic_model_name(data);
    if(model_name!="IEELAL" and model_name!="IEELAR" and model_name!="IEELZN" and
       model_name!="IEELBL" and
       model_name!="CIM5AL" and model_name!="CIM5AR" and model_name!="CIM5ZN" and
       model_name!="CIM5BL" and
       model_name!="UFLSAL" and model_name!="UFLSAR" and model_name!="UFLSZN" and
       model_name!="UFLSBL" and
       model_name!="PUFLSAL" and model_name!="PUFLSAR" and model_name!="PUFLSZN" and
       model_name!="PUFLSBL"
       )
        return loads;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(data.size()<3)
        return loads;

    size_t subsystem_number;
    string identifier;

    size_t i=0;
    i = 1; subsystem_number = get_integer_data(data[i], "0");
    i = 2; identifier = get_string_data(data[i],"");

    size_t model_name_size = model_name.size();
    string model_subsystem_type = model_name.substr(model_name_size-2,2);

    if(model_subsystem_type=="AL")
    {
        loads = psdb.get_all_loads();
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="AR")
    {
        size_t area_no = subsystem_number;
        loads = psdb.get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            --iter;
            size_t area = (*iter)->get_area_number();
            if(area!=area_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="ZN")
    {
        size_t zone_no = subsystem_number;
        loads = psdb.get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            --iter;
            size_t zone = (*iter)->get_zone_number();
            if(zone!=zone_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="BL")
    {
        size_t bus_no = subsystem_number;
        loads = psdb.get_loads_connecting_to_bus(bus_no);
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    return loads;
}

vector<LOAD*> STEPS_IMEXPORTER::remove_loads_with_different_identifier(vector<LOAD*> loads, string identifier)
{
    if(identifier=="*")
        return loads;

    for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
    {
        --iter;
        string load_id = (*iter)->get_identifier();
        if(load_id!=identifier)
            loads.erase(iter);
    }
    return loads;
}

void STEPS_IMEXPORTER::add_IEEL_model(vector<string>& data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="IEELAL" and model_name!="IEELAR" and model_name!="IEELZN" and
       model_name!="IEELBL")
        return;

    if(data.size()<3)
        return;

    IEEL model;
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
            loads[i]->set_model(&model);
    }
}

void STEPS_IMEXPORTER::add_UFLS_model(vector<string>& data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="UFLSAL" and model_name!="UFLSAR" and model_name!="UFLSZN" and
       model_name!="UFLSBL")
        return;

    if(data.size()<3)
        return;

    UFLS model;

    bool successful = model.setup_model_with_steps_string_vector(data);

    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
            loads[i]->set_model(&model);
    }
}

void STEPS_IMEXPORTER::add_PUFLS_model(vector<string>& data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="PUFLSAL" and model_name!="PUFLSAR" and model_name!="PUFLSZN" and
       model_name!="PUFLSBL")
        return;

    if(data.size()<3)
        return;

    PUFLS model;
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
            loads[i]->set_model(&model);
    }
}

void STEPS_IMEXPORTER::add_CDC4T_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CDC4T")
        return;


    if(data.size()<24)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_hvdc_device_id_from_string_vector(data);

    HVDC* hvdc = psdb.get_hvdc(did);
    if(hvdc != NULL)
    {
        CDC4T model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            hvdc->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CDC4T model is built, but will not be set for "<<hvdc->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_CDC6T_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CDC6T")
        return;

    if(data.size()<34)
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_hvdc_device_id_from_string_vector(data);

    HVDC* hvdc = psdb.get_hvdc(did);
    if(hvdc != NULL)
    {
        CDC6T model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            hvdc->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CDC6T model is built, but will not be set for "<<hvdc->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3G1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G1")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G1 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G1 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_WT3G0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G0 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_WT3G2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G2")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G2 model;

        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G2 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_AERD0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "AERD0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        AERD0 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid AERD0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3T0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3T0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3T0 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3T0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3E0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3E0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3E0 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3E0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3P0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3P0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3P0 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3P0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_FILEWIND_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "FILEWIND")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        FILEWIND model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid FILEWIND model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WTRLY0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WTRLY0")
        return;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WTRLY0 model;
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            gen->set_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WTRLY0 model is built, but will not be set for "<<gen->get_device_name();
            show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::export_dynamic_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}
