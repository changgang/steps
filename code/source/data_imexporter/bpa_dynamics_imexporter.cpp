#include "header/data_imexporter/bpa_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/all_supported_models.h"
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;

void BPA_IMEXPORTER::load_dynamic_data(string file)
{
    if(not is_power_system_database_set())
        return;

    ostringstream osstream;
    osstream<<"Loading dynamic data from BPA file: "<<file;
    show_information_with_leading_time_stamp(osstream);

    load_dynamic_data_into_ram(file);

    if(dat_data_in_ram.size()==0)
    {
        osstream<<"No data in the given BPA file: "<<file<<endl
          <<"Please check if the file exist or not.";
        show_information_with_leading_time_stamp(osstream);

        return;
    }
    classify_dynamic_cards();


    osstream<<"Done loading dynamic data.";
    show_information_with_leading_time_stamp(osstream);
}

void BPA_IMEXPORTER::load_dynamic_data_into_ram(string file)
{
    ostringstream osstream;
    swi_data_in_ram.clear();

    ifstream swi_file(file);

    if(!swi_file)
    {
        ostringstream osstream;
        osstream<<"BPA swi file '"<<file<<"' is not accessible. Loading BPA swi data is failed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string buffer;
    while(getline(swi_file,buffer))
    {
        if(not is_comment_line(buffer))
            swi_data_in_ram.push_back(buffer);
    }

    swi_file.close();

    size_t n = swi_data_in_ram.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<<swi_data_in_ram[i]<<endl;
    }
    show_information_with_leading_time_stamp(osstream);
}


void BPA_IMEXPORTER::format_all_dynamic_cards()
{

}

string BPA_IMEXPORTER::get_dynamic_card_type(string data) const
{
    string unsupported_card_type = "UNSUPPORTED BPA CARD";
    string card_type;

    card_type = data.substr(0, 1);
    card_type = string2upper(card_type);
    if(card_type == "B")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "BE")
            return card_type;
        if(card_type == "BC")
        {
            card_type = data.substr(0, 3);
            card_type = string2upper(card_type);
            if(card_type == "BC ")
                return "BC";

            if(card_type == "BC+")
                return card_type;
            else
                return unsupported_card_type;
        }
        return unsupported_card_type;
    }

    if(card_type == "C")
    {
        card_type = data.substr(0,4);
        card_type = string2upper(card_type);
        if(card_type == "CASE")
            return card_type;
        else
            return unsupported_card_type;
    }

    if(card_type == "D")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "D ")
            return "D";

        if(card_type == "DT" or card_type == "DF" or card_type == "DS" or
           card_type == "DZ" or card_type == "DK" or card_type == "DL" or
           card_type == "D+")
            return card_type;

        if(card_type == "DM")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "DM ")
                return "DM";

            if(card_type == "DM#")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "DN")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "DN ")
                return "DN";

            if(card_type == "DN#")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "DA")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "DA ")
                return "DA";

            if(card_type == "DA#")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "DG")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "DG ")
                return "DG";

            if(card_type == "DG+")
                return card_type;
            else
                return unsupported_card_type;
        }

        return unsupported_card_type;
    }

    if(card_type == "E")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);

        if(card_type=="EA" or card_type == "EB" or card_type == "EC" or
           card_type=="ED" or card_type == "EE" or card_type == "EF" or
           card_type=="EG" or card_type == "EJ" or card_type == "EK" or
           card_type=="EM" or card_type == "E*" or
           card_type=="EP" or card_type == "EZ" or card_type == "ES" or
           card_type=="ER" or card_type == "EV" or card_type == "EU")
            return card_type;

        if(card_type == "EL")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "EL ")
                return "EL";

            if(card_type == "EL+")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "EN")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "EN ")
                return "EN";

            if(card_type == "EN+")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "EQ")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "EQC")
                return card_type;
            else
                return unsupported_card_type;
        }
        return unsupported_card_type;
    }

    if(card_type == "F")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);

        if(card_type=="FA" or card_type == "FB" or card_type == "FC" or
           card_type=="FD" or
           card_type=="FG" or card_type == "FH" or card_type == "FJ" or
           card_type=="FK" or card_type == "FL" or card_type == "FZ" or
           card_type=="FM" or card_type == "FN" or card_type == "FO" or
           card_type=="FP" or card_type == "FQ" or card_type == "FR" or
           card_type=="FS" or card_type == "FT" or card_type == "FU" or
           card_type=="FV" or card_type == "F+" or card_type == "F#")
            return card_type;

        if(card_type == "FE")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "FE ")
                return "FE";

            if(card_type == "FEX")
                return card_type;
            else
                return unsupported_card_type;
        }
        if(card_type == "FF")
        {
            // how to distinguish FF control card and FF excitation card
            return card_type;
        }
        if(card_type=="FX")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "FX ")
                return "FX";

            if(card_type=="FX+")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type=="FY")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "FY ")
                return "FY";

            if(card_type=="FY+" or card_type=="FY#")
                return card_type;
            else
                return unsupported_card_type;
        }

        return unsupported_card_type;
    }

    if(card_type == "G")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "GG" or card_type == "GH" or card_type == "GS" or
           card_type == "GL" or card_type == "GW" or card_type == "GA" or
           card_type == "GJ" or card_type == "GK" or card_type == "GZ" or
           card_type == "GD" or card_type == "GX")
            return card_type;

        if(card_type == "GI")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "GI ")
                return "GI";

            if(card_type == "GI+")
                return card_type;
            else
                return unsupported_card_type;
        }

        return unsupported_card_type;
    }

    if(card_type == "L")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type=="LA" or card_type == "LB" or card_type == "L+" or
           card_type=="LP" or card_type == "LQ")
            return card_type;
        else
            return unsupported_card_type;
    }

    if(card_type == "M")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type=="M ")
            return "M";

        if(card_type=="MC" or card_type == "MF" or card_type == "MG" or
           card_type=="ML" or card_type == "MJ" or card_type == "MK" or
           card_type=="MW" or card_type == "ME" or card_type == "MM" or
           card_type=="MR" or card_type == "MY" )
            return card_type;
        else
            return unsupported_card_type;
    }

    if(card_type == "P")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);

        if(card_type=="PV")
            return card_type;
        else
            return unsupported_card_type;
    }

    if(card_type == "S")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "SF" or card_type == "SP" or card_type == "SS" or
           card_type == "ST" or card_type == "SA" or card_type == "SD" or
           card_type == "SE" or card_type == "SJ" or card_type == "SK" or
           card_type == "SL" or card_type == "SX" or card_type == "SY")
            return card_type;

        if(card_type == "SH")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "SH ")
                return "SH";
            if(card_type == "SH+")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "SI")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "SI ")
                return "SI";
            if(card_type == "SI+")
                return card_type;
            else
                return unsupported_card_type;
        }

        if(card_type == "SB")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "SB ")
                return "SB";
            if(card_type == "SB+")
                return card_type;
            else
                return unsupported_card_type;
        }
        return unsupported_card_type;
    }


    if(card_type == "T")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "TA" or card_type == "TB" or card_type == "TC" or
           card_type == "TW" or card_type == "TV")
            return card_type;
        else
            return unsupported_card_type;
    }

    return unsupported_card_type;

}


void BPA_IMEXPORTER::classify_dynamic_cards()
{

}

void BPA_IMEXPORTER::load_all_models()
{
    size_t n = swi_data_in_ram.size();
    for(size_t i=0; i!=n; ++i)
        load_one_model(swi_data_in_ram[i]);

}

void BPA_IMEXPORTER::load_one_model(string data)
{
    ostringstream osstream;
    osstream<<"Now go parsing dynamic data: "<<data;
    show_information_with_leading_time_stamp(osstream);

    string model_name = get_dynamic_model_name(data);
    if(model_name=="")
        return;
    if(model_name=="GENCLS") { add_GENCLS_model(data); return;}
    if(model_name=="GENROU") { add_GENROU_model(data); return;}
    if(model_name=="GENSAL") { add_GENSAL_model(data); return;}

    if(model_name=="COMP") { add_COMP_model(data); return;}

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


    if(model_name=="IEELAL" or model_name=="IEELAR" or model_name=="IEELZN" or
       model_name=="IEELOW" or model_name=="IEELBL") { add_IEEL_model(data); return;}

    osstream<<"Warning. Dynamic model '"<<model_name<<"' is not supported. Check line "<<__LINE__<<" in file "<<__FILE__<<".";
    show_information_with_leading_time_stamp(osstream);

}
string BPA_IMEXPORTER::get_dynamic_model_name(string data)
{
    vector<string> splitted_data = split_string(data, ",");
    if(splitted_data.size()>=2)
    {
        string model_name = splitted_data[1];
        model_name = get_string_data(model_name, "");
        return model_name;
    }
    else
        return "";
}

DEVICE_ID BPA_IMEXPORTER::get_generator_device_id_from_string(string data)
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return did;

    size_t bus;
    string identifier;

    size_t i=0;
    bus = get_integer_data(swidata[i], "0"); i++; i++;
    identifier = get_string_data(swidata[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID BPA_IMEXPORTER::get_load_device_id_from_string(string data)
{
    DEVICE_ID did;
    did.set_device_type("LOAD");

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return did;

    size_t bus;
    string identifier;

    size_t i=0;
    bus = get_integer_data(swidata[i], "0"); i++; i++;
    identifier = get_string_data(swidata[i],"");

    TERMINAL terminal;
    terminal.append_bus(bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID BPA_IMEXPORTER::get_line_device_id_from_string(string data)
{
    DEVICE_ID did;
    did.set_device_type("LINE");

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<4)
        return did;

    size_t ibus, jbus;
    string identifier;

    size_t i=0;
    ibus = get_integer_data(swidata[i], "0"); i++; i++;
    jbus = get_integer_data(swidata[i], "0"); i++;
    identifier = get_string_data(swidata[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}
DEVICE_ID BPA_IMEXPORTER::get_hvdc_device_id_from_string(string data)
{
    DEVICE_ID did;
    did.set_device_type("HVDC");

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<4)
        return did;

    size_t ibus, jbus;
    string identifier;

    size_t i=0;
    ibus = get_integer_data(swidata[i], "0"); i++; i++;
    jbus = get_integer_data(swidata[i], "0"); i++;
    identifier = get_string_data(swidata[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}
DEVICE_ID BPA_IMEXPORTER::get_transformer_device_id_from_string(string data)
{
    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<4)
        return did;

    size_t ibus, jbus, kbus;
    string identifier;

    size_t i=0;
    ibus = get_integer_data(swidata[i], "0"); i++; i++;
    jbus = get_integer_data(swidata[i], "0"); i++;
    kbus = get_integer_data(swidata[i], "0"); i++;
    identifier = get_string_data(swidata[i],"");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier(identifier);

    return did;
}

void BPA_IMEXPORTER::add_GENCLS_model(string data)
{
    if(get_dynamic_model_name(data) != "GENCLS")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        GENCLS model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_GENROU_model(string data)
{
    if(get_dynamic_model_name(data) != "GENROU")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        GENROU model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_GENSAL_model(string data)
{
    if(get_dynamic_model_name(data) != "GENSAL")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        GENSAL model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_COMP_model(string data)
{
    if(get_dynamic_model_name(data) != "COMP")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        COMP model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_IEE2ST_model(string data)
{
    if(get_dynamic_model_name(data) != "IEEE2ST")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        IEE2ST model;
        model.set_power_system_database(psdb);
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_SEXS_model(string data)
{
    if(get_dynamic_model_name(data) != "SEXS")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        SEXS model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_IEEET1_model(string data)
{
    if(get_dynamic_model_name(data) != "IEEET1")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        IEEET1 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_PSASPE1_model(string data)
{
    if(get_dynamic_model_name(data) != "PSASPE1")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        PSASPE1 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_PSASPE2_model(string data)
{
    if(get_dynamic_model_name(data) != "PSASPE2")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        PSASPE2 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_CSEET1_model(string data)
{
    if(get_dynamic_model_name(data) != "CSEET1")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        CSEET1 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_CSEET2_model(string data)
{
    if(get_dynamic_model_name(data) != "CSEET2")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        CSEET2 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_PSASPE13_model(string data)
{
    if(get_dynamic_model_name(data) != "PSASPE13")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        PSASPE13 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_TGOV1_model(string data)
{
    if(get_dynamic_model_name(data) != "TGOV1")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        TGOV1 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_IEEEG1_model(string data)
{
    if(get_dynamic_model_name(data) != "IEEEG1")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        IEEEG1 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_IEEEG2_model(string data)
{
    if(get_dynamic_model_name(data) != "IEEEG2")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        IEEEG2 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

void BPA_IMEXPORTER::add_IEEEG3_model(string data)
{
    if(get_dynamic_model_name(data) != "IEEEG3")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = get_generator_device_id_from_string(data);

    GENERATOR* generator = psdb->get_generator(did);
    if(generator != NULL)
    {
        IEEEG3 model;
        bool successful = model.setup_model_with_psse_string(data);
        if(successful)
            generator->set_model(&model);
    }
}

vector<LOAD*> BPA_IMEXPORTER::get_all_loads_of(string data)
{
    vector<LOAD*> loads;

    string model_name = get_dynamic_model_name(data);
    if(model_name!="IEELAL" and model_name!="IEELAR" and model_name!="IEELZN" and
       model_name!="IEELBL" and
       model_name!="CIM5AL" and model_name!="CIM5AR" and model_name!="CIM5ZN" and
       model_name!="CIM5BL")
        return loads;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<9)
        return loads;

    size_t subsystem_number;
    string identifier;

    size_t i=0;
    subsystem_number = get_integer_data(swidata[i], "0");  i++; i++;
    identifier = get_string_data(swidata[i],""); i++;

    if(model_name=="IEELAL" or model_name=="CIM5AL")
    {
        loads = psdb->get_all_loads();
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_name=="IEELAR" or model_name=="CIM5AR")
    {
        size_t area_no = subsystem_number;
        loads = psdb->get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            iter--;
            size_t area = (*iter)->get_area_number();
            if(area!=area_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_name=="IEELZN" or model_name=="CIM5ZN")
    {
        size_t zone_no = subsystem_number;
        loads = psdb->get_all_loads();
        for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
        {
            iter--;
            size_t zone = (*iter)->get_zone_number();
            if(zone!=zone_no)
                loads.erase(iter);
        }
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    if(model_name=="IEELBL" or model_name=="CIM5BL")
    {
        size_t bus_no = subsystem_number;
        loads = psdb->get_loads_connecting_to_bus(bus_no);
        loads = remove_loads_with_different_identifier(loads, identifier);
        return loads;
    }
    return loads;
}

vector<LOAD*> BPA_IMEXPORTER::remove_loads_with_different_identifier(vector<LOAD*> loads, string identifier)
{
    if(identifier=="*")
        return loads;

    for(vector<LOAD*>::iterator iter=loads.end(); iter!=loads.begin(); )
    {
        iter--;
        string load_id = (*iter)->get_identifier();
        if(load_id!=identifier)
            loads.erase(iter);
    }
    return loads;
}

void BPA_IMEXPORTER::add_IEEL_model(string data)
{
    string model_name = get_dynamic_model_name(data);
    if(model_name!="IEELAL" and model_name!="IEELAR" and model_name!="IEELZN" and
       model_name!="IEELBL")
        return;

    vector<string> swidata = split_string(data,",");
    if(swidata.size()<3)
        return;

    IEEL model;
    bool successful = model.setup_model_with_psse_string(data);
    if(successful)
    {
        vector<LOAD*> loads = get_all_loads_of(data);
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
            loads[i]->set_model(&model);
    }
}


void BPA_IMEXPORTER::export_dynamic_data(string file)
{
    file;
    return;
}
