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
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    //file = string2upper(file);
    load_dynamic_data_into_ram(file);
    load_all_models();

    osstream<<"Done loading dynamic data.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void STEPS_IMEXPORTER::load_dynamic_data_from_steps_vector(vector<vector<string> >& data)
{
    splitted_sdyr_data_in_ram = data;
    load_all_models();
}

void STEPS_IMEXPORTER::load_dynamic_data_into_ram(string file)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    splitted_sdyr_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"STEPS dyr file '"<<file<<"' is not accessible. Loading STEPS dyr data is failed.";
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

    //for(unsigned int i=0; i<sdyr_data_in_ram.size(); ++i)
    //    cout<<sdyr_data_in_ram[i]<<endl;
}

void STEPS_IMEXPORTER::load_all_models()
{
    unsigned int n = splitted_sdyr_data_in_ram.size();
    for(unsigned int i=0; i!=n; ++i)
        load_one_model(splitted_sdyr_data_in_ram[i]);
}

void STEPS_IMEXPORTER::load_one_model(vector<string>& data)
{
    ostringstream osstream;
    //osstream<<"Now go parsing dynamic data: "<<data;
    //show_information_with_leading_time_stamp(osstream);

    string model_name = get_dynamic_model_name(data);
    if(model_name=="")
        return;
    if(model_name=="GENCLS") { add_GENCLS_model(data); return;}
    if(model_name=="GENROU") { add_GENROU_model(data); return;}
    if(model_name=="GENSAL") { add_GENSAL_model(data); return;}

    if(model_name=="COMP") { add_COMP_model(data); return;}
    if(model_name=="IEEEVC") { add_IEEEVC_model(data); return;}
    if(model_name=="PSASPVC") { add_PSASPVC_model(data); return;}

    if(model_name=="IEE2ST") { add_IEE2ST_model(data); return;}
    if(model_name=="PSASPS1") { add_PSASPS1_model(data); return;}
    if(model_name=="PSASPS2") { add_PSASPS2_model(data); return;}
    if(model_name=="PSASPS3") { add_PSASPS3_model(data); return;}
    if(model_name=="PSASPS4") { add_PSASPS4_model(data); return;}
    if(model_name=="PSASPS5") { add_PSASPS5_model(data); return;}
    if(model_name=="PSASPS6") { add_PSASPS6_model(data); return;}
    if(model_name=="PSASPS8") { add_PSASPS8_model(data); return;}

    if(model_name=="SEXS") { add_SEXS_model(data); return;}
    if(model_name=="IEEET1") { add_IEEET1_model(data); return;}
    if(model_name=="PSASPE1") { add_PSASPE1_model(data); return;}
    if(model_name=="PSASPE2") { add_PSASPE2_model(data); return;}
    if(model_name=="CSEET1") { add_CSEET1_model(data); return;}
    if(model_name=="CSEET2") { add_CSEET2_model(data); return;}
    if(model_name=="PSASPE13") { add_PSASPE13_model(data); return;}
    if(model_name=="PSASPE14") { add_PSASPE14_model(data); return;}

    if(model_name=="TGOV1") { add_TGOV1_model(data); return;}
    if(model_name=="IEEEG1") { add_IEEEG1_model(data); return;}
    if(model_name=="IEEEG2") { add_IEEEG2_model(data); return;}
    if(model_name=="IEEEG3") { add_IEEEG3_model(data); return;}
    if(model_name=="IEESGO") { add_IEESGO_model(data); return;}
    if(model_name=="IEEEG1SB") { add_IEEEG1SB_model(data); return;}
    if(model_name=="GAST2A") { add_GAST2A_model(data); return;}
    if(model_name=="URCSCT") { add_URCSCT_model(data); return;}

    if(model_name=="LCFB1") { add_LCFB1_model(data); return;}


    if(model_name=="IEELAL" or model_name=="IEELAR" or model_name=="IEELZN" or
       model_name=="IEELOW" or model_name=="IEELBL") { add_IEEL_model(data); return;}

    if(model_name=="CIM6AL" or model_name=="CIM6AR" or model_name=="CIM6ZN" or
       model_name=="CIM6OW" or model_name=="CIM6BL") { add_CIM6_model(data); return;}

    if(model_name=="UVLSAL" or model_name=="UVLSAR" or model_name=="UVLSZN" or
       model_name=="UVLSOW" or model_name=="UVLSBL") { add_UVLS_model(data); return;}

    if(model_name=="UFLSAL" or model_name=="UFLSAR" or model_name=="UFLSZN" or
       model_name=="UFLSOW" or model_name=="UFLSBL") { add_UFLS_model(data); return;}
    if(model_name=="PUFLSAL" or model_name=="PUFLSAR" or model_name=="PUFLSZN" or
       model_name=="PUFLSOW" or model_name=="PUFLSBL") { add_PUFLS_model(data); return;}

    if(model_name=="CDC4T") { add_CDC4T_model(data); return;}
    if(model_name=="CDC6T") { add_CDC6T_model(data); return;}

    if(model_name=="VSCHVDCP0") { add_VSCHVDCP0_model(data); return;}
    if(model_name=="VSCHVDCC0") { add_VSCHVDCC0_model(data); return;}
    if(model_name=="VSG0") { add_VSG0_model(data); return;}

    if(model_name=="WT3G1") { add_WT3G1_model(data); return;}
    if(model_name=="WT3G0") { add_WT3G0_model(data); return;}
    if(model_name=="WT3G2") { add_WT3G2_model(data); return;}
    if(model_name=="AERD0") { add_AERD0_model(data); return;}
    if(model_name=="WT3T0") { add_WT3T0_model(data); return;}
    if(model_name=="WT3E0") { add_WT3E0_model(data); return;}
    if(model_name=="WT3E1") { add_WT3E1_model(data); return;}
    if(model_name=="WT3P0") { add_WT3P0_model(data); return;}
    if(model_name=="FILEWIND") { add_FILEWIND_model(data); return;}
    if(model_name=="WTRLY0") { add_WTRLY0_model(data); return;}

    if(model_name=="PVGU1") { add_PVGU1_model(data); return;}


    osstream<<"Warning. Dynamic model '"<<model_name<<"' is not supported. Check line "<<__LINE__<<" in file "<<__FILE__;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

}

void STEPS_IMEXPORTER::load_one_model(string& data)
{
    vector<string> vecstr = split_string(data,",");
    load_one_model(vecstr);
}

void STEPS_IMEXPORTER::load_dynamic_data_from_psse_string(vector<string>& data)
{
    unsigned int n = data.size();
    for(unsigned int i=0; i<n; ++i)
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
    did.set_device_type(STEPS_GENERATOR);

    if(data.size()<3)
        return did;

    unsigned int bus;
    string identifier;

    unsigned int i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_wt_generator_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);

    if(data.size()<3)
        return did;

    unsigned int bus;
    string identifier;

    unsigned int i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_pv_unit_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_PV_UNIT);

    if(data.size()<3)
        return did;

    unsigned int bus;
    string identifier;

    unsigned int i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_load_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);

    if(data.size()<3)
        return did;

    unsigned int bus;
    string identifier;

    unsigned int i=1;
    bus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_line_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LINE);

    if(data.size()<4)
        return did;

    unsigned int ibus, jbus;
    string identifier;

    unsigned int i=1;
    ibus = get_integer_data(data[i], "0"); ++i;
    jbus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}
DEVICE_ID STEPS_IMEXPORTER::get_hvdc_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_HVDC);

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(data.size()<2)
        return did;

    string hvdc_name;

    unsigned int i=1;
    hvdc_name = get_string_data(data[i], "0");

    unsigned int n = psdb.get_hvdc_count();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(hvdc_name == hvdcs[i]->get_name())
        {
            did = hvdcs[i]->get_device_id();
            break;
        }
    }

    return did;
}

string STEPS_IMEXPORTER::get_vsc_hvdc_name_from_string_vector(vector<string>& data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(data.size()<2)
        return "";

    string vsc_hvdc_name;

    unsigned int i=1;
    vsc_hvdc_name = get_string_data(data[i], "0");

    return vsc_hvdc_name;
}

DEVICE_ID STEPS_IMEXPORTER::get_vsc_hvdc_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_VSC_HVDC);

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(data.size()<2)
        return did;

    string vsc_hvdc_name = get_vsc_hvdc_name_from_string_vector(data);

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_hvdc_name);
    if(vsc_hvdc!=NULL)
        return vsc_hvdc->get_device_id();
    else
        return did;
}

DEVICE_ID STEPS_IMEXPORTER::get_transformer_device_id_from_string_vector(vector<string>& data)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_TRANSFORMER);

    if(data.size()<4)
        return did;

    unsigned int ibus, jbus, kbus;
    string identifier;

    unsigned int i=0;
    ibus = get_integer_data(data[i], "0"); ++i; ++i;
    jbus = get_integer_data(data[i], "0"); ++i;
    kbus = get_integer_data(data[i], "0"); ++i;
    identifier = get_string_data(data[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

void STEPS_IMEXPORTER::add_GENCLS_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENCLS")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            GENCLS model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid GENCLS model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_GENROU_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENROU")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            GENROU model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid GENROU model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_GENSAL_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GENSAL")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            GENSAL model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid GENSAL model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_COMP_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "COMP")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            COMP model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid COMP model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEVC_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEVC")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEEEVC model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEEEVC model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}


void STEPS_IMEXPORTER::add_PSASPVC_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPVC")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPVC model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPVC model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEE2ST_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEE2ST")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEE2ST model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEE2ST model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPS1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS2")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS2 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS2 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS3_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS3")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS3 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS3 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS4_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS4")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS4 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS4 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}
void STEPS_IMEXPORTER::add_PSASPS5_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS5")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS5 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS5 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPS6_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS6")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS6 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS6 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPS8_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPS8")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPS8 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPS8 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_SEXS_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "SEXS")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            SEXS model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid SEXS model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEEET1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEET1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEEET1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEEET1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPE1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPE1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE2")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPE2 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPE2 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_CSEET1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CSEET1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            CSEET1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid CSEET1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_CSEET2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CSEET2")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            CSEET2 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid CSEET2 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE13_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE13")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPE13 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPE13 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_PSASPE14_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PSASPE14")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            PSASPE14 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid PSASPE14 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_TGOV1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "TGOV1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            TGOV1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid TGOV1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG1")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEEEG1 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEEEG1 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG2")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEEEG2 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEEEG2 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG3_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG3")
        return;

    if(data.size()>=3)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
        DEVICE_ID did = get_generator_device_id_from_string_vector(data);

        GENERATOR* generator = psdb.get_generator(did);
        if(generator != NULL)
        {
            IEEEG3 model(toolkit);
            model.set_device_id(did);
            bool successful = model.setup_model_with_steps_string_vector(data);
            if(successful)
                dmdb.add_model(&model);
            else
            {
                ostringstream osstream;
                osstream<<"Warning. Invalid IEEEG3 model is built, but will not be set for "<<generator->get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}


void STEPS_IMEXPORTER::add_IEESGO_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEESGO")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEESGO model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEESGO model is built, but will not be set for "<<generator->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_IEEEG1SB_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "IEEEG1SB")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        IEEEG1SB model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid IEEEG1SB model is built, but will not be set for "<<generator->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_GAST2A_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "GAST2A")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        GAST2A model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid GAST2A model is built, but will not be set for "<<generator->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_URCSCT_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "URCSCT")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        URCSCT model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid URCSCT model is built, but will not be set for "<<generator->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_LCFB1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "LCFB1")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_generator_device_id_from_string_vector(data);

    GENERATOR* generator = psdb.get_generator(did);
    if(generator != NULL)
    {
        LCFB1 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid LCFB1 model is built, but will not be set for "<<generator->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
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
       model_name!="CIM6AL" and model_name!="CIM6AR" and model_name!="CIM6ZN" and
       model_name!="CIM6BL" and
       model_name!="UVLSAL" and model_name!="UVLSAR" and model_name!="UVLSZN" and
       model_name!="UVLSBL" and
       model_name!="UFLSAL" and model_name!="UFLSAR" and model_name!="UFLSZN" and
       model_name!="UFLSBL" and
       model_name!="PUFLSAL" and model_name!="PUFLSAR" and model_name!="PUFLSZN" and
       model_name!="PUFLSBL"
       )
        return loads;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(data.size()<3)
        return loads;

    unsigned int subsystem_number;
    string identifier;

    unsigned int i=0;
    i = 1; subsystem_number = get_integer_data(data[i], "0");
    i = 2; identifier = get_string_data(data[i],"");

    unsigned int model_name_size = model_name.size();
    string model_subsystem_type = model_name.substr(model_name_size-2,2);

    if(model_subsystem_type=="AL")
    {
        loads = psdb.get_all_loads();
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="AR")
    {
        unsigned int area_no = subsystem_number;
        loads = psdb.get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            --iter;
            unsigned int area = (*iter)->get_area_number();
            if(area!=area_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="ZN")
    {
        unsigned int zone_no = subsystem_number;
        loads = psdb.get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            --iter;
            unsigned int zone = (*iter)->get_zone_number();
            if(zone!=zone_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_subsystem_type=="BL")
    {
        unsigned int bus_no = subsystem_number;
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

    STEPS& toolkit = get_toolkit();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    IEEL model(toolkit);
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            model.set_device_id(loads[i]->get_device_id());
            dmdb.add_model(&model);
        }
    }
}

void STEPS_IMEXPORTER::add_CIM6_model(vector<string>& data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="CIM6AL" and model_name!="CIM6AR" and model_name!="CIM6ZN" and
       model_name!="CIM6BL")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    CIM6 model(toolkit);
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            model.set_device_id(loads[i]->get_device_id());
            dmdb.add_model(&model);
        }
    }
}

void STEPS_IMEXPORTER::add_UVLS_model(vector<string>& data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="UVLSAL" and model_name!="UVLSAR" and model_name!="UVLSZN" and
       model_name!="UVLSBL")
        return;

    if(data.size()<3)
        return;
    STEPS& toolkit = get_toolkit();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    UVLS model(toolkit);
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            model.set_device_id(loads[i]->get_device_id());
            dmdb.add_model(&model);
        }
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

    STEPS& toolkit = get_toolkit();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    UFLS model(toolkit);
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            model.set_device_id(loads[i]->get_device_id());
            dmdb.add_model(&model);
        }
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

    STEPS& toolkit = get_toolkit();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    PUFLS model(toolkit);
    bool successful = model.setup_model_with_steps_string_vector(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            model.set_device_id(loads[i]->get_device_id());
            dmdb.add_model(&model);
        }
    }
}

void STEPS_IMEXPORTER::add_CDC4T_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CDC4T")
        return;

    if(data.size()<2)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_hvdc_device_id_from_string_vector(data);

    HVDC* hvdc = psdb.get_hvdc(did);
    if(hvdc != NULL)
    {
        CDC4T model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CDC4T model is built, but will not be set for "<<hvdc->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_CDC6T_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "CDC6T")
        return;

    if(data.size()<2)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_hvdc_device_id_from_string_vector(data);

    HVDC* hvdc = psdb.get_hvdc(did);
    if(hvdc != NULL)
    {
        CDC6T model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid CDC6T model is built, but will not be set for "<<hvdc->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_VSCHVDCP0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "VSCHVDCP0")
        return;

    if(data.size()<2)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_vsc_hvdc_device_id_from_string_vector(data);

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(did);
    if(vsc_hvdc != NULL)
    {
        VSCHVDCP0* model = new VSCHVDCP0(toolkit);
        model->set_device_id(did);
        bool successful = model->setup_model_with_steps_string_vector(data);
        if(successful)
            vsc_hvdc->set_model(model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid VSCHVDCP0 model is built, but will not be set for "<<vsc_hvdc->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_VSCHVDCC0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "VSCHVDCC0")
        return;

    if(data.size()<18)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_vsc_hvdc_device_id_from_string_vector(data);

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(did);
    if(vsc_hvdc != NULL)
    {
        VSCHVDCC0* model = new VSCHVDCC0(toolkit);
        model->set_device_id(did);
        bool successful = model->setup_model_with_steps_string_vector(data);
        if(successful)
            vsc_hvdc->set_model(model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid VSCHVDCC0 model is built, but will not be set for "<<vsc_hvdc->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_VSG0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "VSG0")
        return;

    if(data.size()<22)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = get_vsc_hvdc_device_id_from_string_vector(data);

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(did);
    if(vsc_hvdc != NULL)
    {
        VSG0* model = new VSG0(toolkit);
        model->set_device_id(did);
        bool successful = model->setup_model_with_steps_string_vector(data);
        if(successful)
            vsc_hvdc->set_model(model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid VSG0 model is built, but will not be set for "<<vsc_hvdc->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3G1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G1")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G1 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G1 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_WT3G0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_WT3G2_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3G2")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3G2 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3G2 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_AERD0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "AERD0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        AERD0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid AERD0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3T0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3T0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3T0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3T0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3E0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3E0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3E0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3E0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3E1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3E1")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3E1 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3E1 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WT3P0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WT3P0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WT3P0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WT3P0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_FILEWIND_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "FILEWIND")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        FILEWIND model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid FILEWIND model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::add_WTRLY0_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "WTRLY0")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_wt_generator_device_id_from_string_vector(data);

    WT_GENERATOR* gen = psdb.get_wt_generator(did);
    if(gen != NULL)
    {
        WTRLY0 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid WTRLY0 model is built, but will not be set for "<<gen->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


void STEPS_IMEXPORTER::add_PVGU1_model(vector<string>& data)
{
    if(get_dynamic_model_name(data) != "PVGU1")
        return;

    if(data.size()<3)
        return;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    DEVICE_ID did = get_pv_unit_device_id_from_string_vector(data);

    PV_UNIT* pvu = psdb.get_pv_unit(did);
    if(pvu != NULL)
    {
        PVGU1 model(toolkit);
        model.set_device_id(did);
        bool successful = model.setup_model_with_steps_string_vector(data);
        if(successful)
            dmdb.add_model(&model);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid PVGU1 model is built, but will not be set for "<<pvu->get_compound_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void STEPS_IMEXPORTER::export_dynamic_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}
