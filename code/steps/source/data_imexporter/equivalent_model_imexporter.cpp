#include "header/data_imexporter/equivalent_model_imexporter.h"
#include "header/model/equivalent_model/equivalent_models.h"
#include "header/basic/utility.h"
#include "header/meter/meter_setter.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

EQUIVALENT_MODEL_IMEXPORTER::EQUIVALENT_MODEL_IMEXPORTER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
    data_in_ram.clear();
}
EQUIVALENT_MODEL_IMEXPORTER::~EQUIVALENT_MODEL_IMEXPORTER()
{
    ;
}
STEPS& EQUIVALENT_MODEL_IMEXPORTER::get_toolkit() const
{
    return *toolkit;
}
void EQUIVALENT_MODEL_IMEXPORTER::load_equivalent_model(string file)
{
    load_data_into_ram(file);

    unsigned int n = data_in_ram.size();

    for(unsigned int i=0; i!=n; ++i)
    {
        vector< vector<string> > model = data_in_ram[i];
        add_equivalent_device(model);
    }

    for(unsigned int i=0; i!=n; ++i)
    {
        vector< vector<string> > model = data_in_ram[i];

        vector<string> first_line = model[0];

        string model_name = first_line[0];

        if(model_name=="ARXL")
        {
            load_ARXL_model(model);
            continue;
        }

    }
}

void EQUIVALENT_MODEL_IMEXPORTER::load_data_into_ram(string file)
{
    ostringstream osstream;

    data_in_ram.clear();

    FILE* fid = fopen(file.c_str(), "rt");
    if(fid==NULL)
    {
        osstream<<"equivalent model file cannot be open"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    char buffer[1024];
    string data;
    vector< vector<string> > one_model;
    vector<string> one_line_in_a_model;

    one_model.clear();

    while(true)
    {
        if(fgets(buffer, 1024, fid)!=NULL)
        {
            data = string2csv(buffer);
            one_line_in_a_model = split_string(data,",");
            if(one_line_in_a_model.size()==0)
                continue;

            if(one_line_in_a_model[0]=="END")
            {
                data_in_ram.push_back(one_model);
                one_model.clear();
            }
            else
                one_model.push_back(one_line_in_a_model);
        }
        else
            break;
    }
    fclose(fid);
}

void EQUIVALENT_MODEL_IMEXPORTER::add_equivalent_device(vector< vector<string> >& model_data)
{
    unsigned int n = model_data.size();
    if(n==0)
        return;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ostringstream osstream;

    vector<string> data_line = model_data[0];

    // new ARX model found
    unsigned int side_bus = get_integer_data(data_line[4],"0");

    EQUIVALENT_DEVICE edevice(*toolkit);
    edevice.set_equivalent_device_bus(side_bus);
    if(data_line[0]=="ARXL")
        edevice.set_identifier("ARXL");

    psdb.append_equivalent_device(edevice);

    osstream<<"Equivalent device added: "<<edevice.get_compound_device_name();
    toolkit->show_information_with_leading_time_stamp(osstream);
}

METER EQUIVALENT_MODEL_IMEXPORTER::get_meter_from_data(const vector<string> & data_line, unsigned int& delay, double& coefficient)
{
    ostringstream osstream;

    METER_SETTER setter(*toolkit);

    METER meter(*toolkit);

    if(data_line.size()==0)
    {
        osstream<<"FATAL error. No data is found in the data line. Cannot generate meter for equivalent model.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return meter;
    }

    if(data_line[1]=="BUS")
    {
        unsigned int bus = get_integer_data(data_line[3],"0");
        delay = get_integer_data(data_line[4],"0");
        coefficient = get_double_data(data_line[5],"0.0");

        if(data_line[2].find("VOLTAGE_PU")!=string::npos)
            meter = setter.prepare_bus_voltage_in_pu_meter(bus);
        else
            if(data_line[2].find("VOLTAGE_KV")!=string::npos)
                meter = setter.prepare_bus_voltage_in_kV_meter(bus);
            else
                if(data_line[2].find("ANGLE_DEG")!=string::npos)
                    meter = setter.prepare_bus_angle_in_deg_meter(bus);
                else
                    if(data_line[2].find("ANGLE_RAD")!=string::npos)
                        meter = setter.prepare_bus_angle_in_rad_meter(bus);
                    else
                        if(data_line[2].find("FREQUENY_DEVIATION_PU")!=string::npos)
                            meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus);
                        else
                            if(data_line[2].find("FREQUENY_DEVIATION_HZ")!=string::npos)
                                meter = setter.prepare_bus_frequency_deviation_in_Hz_meter(bus);
                            else
                                if(data_line[2].find("FREQUENY_PU")!=string::npos)
                                    meter = setter.prepare_bus_frequency_in_pu_meter(bus);
                                else
                                    if(data_line[2].find("FREQUENY_HZ")!=string::npos)
                                        meter = setter.prepare_bus_frequency_in_Hz_meter(bus);
        return meter;
    }


    if(data_line[1]=="LINE")
    {
        DEVICE_ID did;
        did.set_device_type(STEPS_AC_LINE);
        TERMINAL terminal;
        terminal.clear();
        terminal.append_bus(get_integer_data(data_line[3],"0"));
        terminal.append_bus(get_integer_data(data_line[4],"0"));
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(get_string_data(data_line[5],"")));

        unsigned int side_bus = get_integer_data(data_line[6],"0");

        delay = get_integer_data(data_line[7],"0");
        coefficient = get_double_data(data_line[8],"0.0");

        if(data_line[2].find("ACTIVE_POWER_MW")!=string::npos)
            meter = setter.prepare_line_active_power_in_MW_meter(did, side_bus);
        else
            if(data_line[2].find("ACTIVE_POWER_PU")!=string::npos)
                meter = setter.prepare_line_active_power_in_pu_meter(did, side_bus);
            else
                if(data_line[2].find("REACTIVE_POWER_MVAR")!=string::npos)
                    meter = setter.prepare_line_reactive_power_in_MVar_meter(did, side_bus);
                else
                    if(data_line[2].find("REACTIVE_POWER_PU")!=string::npos)
                        meter = setter.prepare_line_reactive_power_in_pu_meter(did, side_bus);
                    else
                        if(data_line[2].find("CURRENT_KA")!=string::npos)
                            meter = setter.prepare_line_current_in_kA_meter(did, side_bus);
                        else
                            if(data_line[2].find("CURRENT_PU")!=string::npos)
                                meter = setter.prepare_line_current_in_pu_meter(did, side_bus);
        return meter;
    }

    if(data_line[1]=="TRANSFORMER")
    {
        DEVICE_ID did;
        did.set_device_type(STEPS_TRANSFORMER);
        TERMINAL terminal;
        terminal.clear();
        terminal.append_bus(get_integer_data(data_line[3],"0"));
        terminal.append_bus(get_integer_data(data_line[4],"0"));
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(get_string_data(data_line[5],"")));

        unsigned int side_bus = get_integer_data(data_line[6],"0");

        delay = get_integer_data(data_line[7],"0");
        coefficient = get_double_data(data_line[8],"0.0");

        if(data_line[2].find("ACTIVE_POWER_MW")!=string::npos)
            meter = setter.prepare_transformer_active_power_in_MW_meter(did, side_bus);
        else
            if(data_line[2].find("ACTIVE_POWER_PU")!=string::npos)
                meter = setter.prepare_transformer_active_power_in_pu_meter(did, side_bus);
            else
                if(data_line[2].find("REACTIVE_POWER_MVAR")!=string::npos)
                    meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did, side_bus);
                else
                    if(data_line[2].find("REACTIVE_POWER_PU")!=string::npos)
                        meter = setter.prepare_transformer_reactive_power_in_pu_meter(did, side_bus);
                    else
                        if(data_line[2].find("CURRENT_KA")!=string::npos)
                            meter = setter.prepare_transformer_current_in_kA_meter(did, side_bus);
                        else
                            if(data_line[2].find("CURRENT_PU")!=string::npos)
                                meter = setter.prepare_transformer_current_in_pu_meter(did, side_bus);
        return meter;
    }


    if(data_line[1]=="GENERATOR")
    {
        unsigned int bus = get_integer_data(data_line[3],"0");
        string identifier = get_string_data(data_line[4],"");
        delay = get_integer_data(data_line[5],"0");
        coefficient = get_double_data(data_line[6],"0.0");

        DEVICE_ID did;
        did.set_device_type(STEPS_GENERATOR);
        TERMINAL terminal;
        terminal.clear();
        terminal.append_bus(bus);
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(identifier));

        if(data_line[2].find("ACTIVE_POWER_MW")!=string::npos)
            meter = setter.prepare_generator_terminal_active_power_in_MW_meter(did);
        else
            if(data_line[2].find("ACTIVE_POWER_PU")!=string::npos)
                meter = setter.prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
            else
                if(data_line[2].find("REACTIVE_POWER_MVAR")!=string::npos)
                    meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(did);
                else
                    if(data_line[2].find("REACTIVE_POWER_PU")!=string::npos)
                        meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
                    else
                        if(data_line[2].find("EXCITATION_VOLTAGE_PU")!=string::npos)
                            meter = setter.prepare_generator_excitation_voltage_in_pu_meter(did);
                        else
                            if(data_line[2].find("MECHANICAL_POWER_MW")!=string::npos)
                                meter = setter.prepare_generator_mechanical_power_in_MW_meter(did);
                            else
                                if(data_line[2].find("MECHANICAL_POWER_PU")!=string::npos)
                                    meter = setter.prepare_generator_mechanical_power_in_pu_on_sbase_meter(did);
        return meter;
    }


    if(data_line[1]=="LOAD")
    {
        unsigned int bus = get_integer_data(data_line[3],"0");
        string identifier = get_string_data(data_line[4],"");
        delay = get_integer_data(data_line[5],"0");
        coefficient = get_double_data(data_line[6],"0.0");

        DEVICE_ID did;
        did.set_device_type(STEPS_LOAD);
        TERMINAL terminal;
        terminal.clear();
        terminal.append_bus(bus);
        did.set_device_terminal(terminal);
        did.set_device_identifier_index(get_index_of_string(identifier));

        if(data_line[2].find("ACTIVE_POWER_MW")!=string::npos)
            meter = setter.prepare_load_active_power_in_MW_meter(did);
        else
            if(data_line[2].find("ACTIVE_POWER_PU")!=string::npos)
                meter = setter.prepare_load_active_power_in_pu_meter(did);
            else
                if(data_line[2].find("REACTIVE_POWER_MVAR")!=string::npos)
                    meter = setter.prepare_load_reactive_power_in_MVar_meter(did);
                else
                    if(data_line[2].find("REACTIVE_POWER_PU")!=string::npos)
                        meter = setter.prepare_load_reactive_power_in_pu_meter(did);
        return meter;
    }


    return meter;
}

void EQUIVALENT_MODEL_IMEXPORTER::load_ARXL_model(vector< vector<string> >& model_data)
{
    unsigned int n = model_data.size();
    if(n==0)
        return;

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit->get_dynamic_model_database();

    ostringstream osstream;

    vector<string> data_line = model_data[0];
    if(data_line[0]!="ARXL")
        return;

    // new ARX model found
    unsigned int ibus = get_integer_data(data_line[1],"0");
    unsigned int jbus = get_integer_data(data_line[2],"0");
    string identifier = get_string_data(data_line[3],"");
    unsigned int side_bus = get_integer_data(data_line[4],"0");

    DEVICE_ID did;
    did.set_device_type(STEPS_EQUIVALENT_DEVICE);
    TERMINAL terminal;
    terminal.append_bus(side_bus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("ARXL"));

    EQUIVALENT_DEVICE* pedevice = psdb.get_equivalent_device(did);

    ARXL model(*toolkit);
    model.set_device_id(did);

    did.set_device_type(STEPS_AC_LINE);
    terminal.clear();
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string(identifier));

    model.set_output_line(did, side_bus);

    METER_SETTER setter(*toolkit);

    METER meter(*toolkit);
    unsigned int delay = 0;
    double coefficient = 0.0;
    for(unsigned int i=1; i!=n; ++i)
    {
        data_line = model_data[i];

        meter = get_meter_from_data(data_line, delay, coefficient);
        if(meter.is_valid())
        {
            if(data_line[0]=="P_INPUT")
                model.add_P_input_item(meter, delay, coefficient);
            if(data_line[0]=="Q_INPUT")
                model.add_Q_input_item(meter, delay, coefficient);
        }
        else
        {
            osstream<<"Warning. Invalid meter is detected in ARXL model data: "<<endl;
            for(unsigned int k=0; k<data_line.size(); ++k)
               osstream<<data_line[k]<<", ";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }

    vector<METER> p_meters = model.get_P_meters();
    METER p_meter = p_meters[0];
    string meter_type = p_meter.get_meter_type();

    dmdb.add_model(&model);

    EQUIVALENT_MODEL* emodel = pedevice->get_equivalent_model();

    osstream<<"ARXL model added to "<<pedevice->get_compound_device_name();
    toolkit->show_information_with_leading_time_stamp(osstream);

    osstream<<emodel->get_standard_psse_string();
    toolkit->show_information_with_leading_time_stamp(osstream);

}
