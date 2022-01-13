#include "header/data_imexporter/steps_imexporter.h"
#include "header/basic/utility.h"
#include "header/device/bus.h"
#include "header/device/load.h"
#include "header/device/fixed_shunt.h"
#include "header/device/generator.h"
#include "header/device/wt_generator.h"
#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

STEPS_IMEXPORTER::STEPS_IMEXPORTER(STEPS& toolkit) : DATA_IMEXPORTER(toolkit)
{
    splitted_sraw_data_in_ram.clear();
    splitted_sdyr_data_in_ram.clear();
}

STEPS_IMEXPORTER::~STEPS_IMEXPORTER()
{
}

void STEPS_IMEXPORTER::load_powerflow_data(string file)
{
    ostringstream osstream;
    osstream<<"Loading powerflow data from STEPS file: "<<file;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    load_powerflow_data_into_ram(file);
    if(splitted_sraw_data_in_ram.size()==0)
    {
        osstream<<"No data in the given STEPS file: "<<file<<endl
          <<"Please check if the file exist or not.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        return;
    }
    load_all_devices();

    osstream<<"Done loading powerflow data.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void STEPS_IMEXPORTER::load_all_devices()
{
    load_case_data();
    load_bus_data();
    load_load_data();
    load_fixed_shunt_data();
    load_source_data();
    load_line_data();
    load_transformer_data();
    load_area_data();
    load_hvdc_data();
    load_lcc_hvdc_data();
    load_vsc_hvdc_data();
    load_transformer_impedance_correction_table_data();
    load_multi_terminal_hvdc_data();
    load_multi_section_line_data();
    load_zone_data();
    load_interarea_transfer_data();
    load_owner_data();
    load_facts_data();
    load_switched_shunt_data();
}

void STEPS_IMEXPORTER::load_powerflow_result(string file)
{
    ostringstream osstream;
    osstream<<"Loading powerflow result from STEPS file: "<<file;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    vector<string> data_in_ram = load_powerflow_result_into_ram(file);
    if(data_in_ram.size()==0)
    {
        osstream<<"No data in the given PSS/E file: "<<file<<endl
          <<"Please check if the file exist or not.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    load_bus_powerflow_result(data_in_ram);
    load_generator_powerflow_result(data_in_ram);
    load_wt_generator_powerflow_result(data_in_ram);
    load_pv_unit_powerflow_result(data_in_ram);
    load_energy_storage_powerflow_result(data_in_ram);
    load_transformer_powerflow_result(data_in_ram);
    load_hvdc_powerflow_result(data_in_ram);

    osstream<<"Done loading powerflow result.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void STEPS_IMEXPORTER::load_sequence_data(string sq_source)
{
    sq_source = string2upper(sq_source);
}


void STEPS_IMEXPORTER::load_powerflow_data_from_steps_vector(vector<vector<vector<string> > >& data)
{
    splitted_sraw_data_in_ram = data;
    if(splitted_sraw_data_in_ram.size()==0)
    {
        ostringstream osstream;
        osstream<<"No data in the given STEPS powerflow vector <splitted_sraw_data_in_ram>."<<endl
                <<"Please check if the vector contents exist or not.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);

        return;
    }
    load_all_devices();
}

void STEPS_IMEXPORTER::load_powerflow_data_into_ram(string file)
{
    ostringstream osstream;

    splitted_sraw_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"STEPS sraw file '"<<file<<"' is not accessible. Loading STEPS sraw data is failed.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    char buffer[10240];
    string sbuffer;

    vector<string> splitted_buffer;
    vector< vector<string> > splitted_data_of_one_type;
    splitted_data_of_one_type.clear();

    if(fgets(buffer, 1024, fid)==NULL)
    {
        fclose(fid);
        return;
    }

    sbuffer = trim_steps_comment(buffer);
    sbuffer = trim_string(sbuffer);
    splitted_buffer = split_string(sbuffer, ",");
    splitted_data_of_one_type.push_back(splitted_buffer);

    splitted_buffer.clear();
    for(unsigned int i=0; i!=2; ++i)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            fclose(fid);
            return;
        }
        sbuffer = trim_steps_comment(buffer);
        sbuffer = trim_string(sbuffer);
        splitted_buffer.push_back(sbuffer);
    }
    splitted_data_of_one_type.push_back(splitted_buffer);
    splitted_sraw_data_in_ram.push_back(splitted_data_of_one_type);

    splitted_buffer.clear();
    splitted_data_of_one_type.clear();

    while(true)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            if(splitted_data_of_one_type.size()!=0)
            {
                splitted_sraw_data_in_ram.push_back(splitted_data_of_one_type);
                splitted_buffer.clear();
                splitted_data_of_one_type.clear();
            }
            break;
        }
        sbuffer = trim_steps_comment(buffer);
        sbuffer = trim_string(sbuffer);
        if(sbuffer.size()!=0)
        {
            if(sbuffer != "0")
            {
                splitted_buffer = split_string(sbuffer, ",");
                splitted_data_of_one_type.push_back(splitted_buffer);
            }
            else
            {
                splitted_sraw_data_in_ram.push_back(splitted_data_of_one_type);
                splitted_buffer.clear();
                splitted_data_of_one_type.clear();
            }
        }
        else
            break;
    }
    fclose(fid);
}

string STEPS_IMEXPORTER::trim_steps_comment(string str)
{
    if(str.size()==0)
        return str;

    size_t found = str.find_first_of('/');
    if(found!=string::npos) str.erase(found, str.size());
    return str;
}

void STEPS_IMEXPORTER::load_case_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<1)
        return;
    vector< vector<string> > DATA = splitted_sraw_data_in_ram[0];
    vector<string> data = DATA[0];

    if(data.size()>0)
    {
        int appendCode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(appendCode==0) toolkit.clear(); // if not in appending mode, clear the toolkit
    }
    if(data.size()>0)
    {
        psdb.set_system_base_power_in_MVA(get_double_data(data.front(),"100.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        set_data_version(get_integer_data(data.front(),"31"));
        data.erase(data.begin());
    }
    data.erase(data.begin()); // transformer rating
    data.erase(data.begin()); // line rating
    if(data.size()>0)
    {
        double f = get_double_data(data.front(),"50.0");
        set_base_frequency_in_Hz(f);
        data.erase(data.begin());
    }
    data = DATA[1];

    psdb.set_case_information(data[0]);
    psdb.set_case_additional_information(data[1]);
}


void STEPS_IMEXPORTER::set_data_version(unsigned int version)
{
    data_version = version;
}

unsigned int STEPS_IMEXPORTER::get_data_version() const
{
    return data_version;
}

void STEPS_IMEXPORTER::load_bus_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<2)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[1];
    vector<string> data;

    unsigned int ndata = DATA.size();
    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];
        BUS bus(toolkit);
        bus.set_base_frequency_in_Hz(get_base_frequency_in_Hz());

        if(data.size()>0)
        {
            bus.set_bus_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            string busname = get_string_data(data.front(),"");
            bus.set_bus_name(trim_string(busname));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_base_voltage_in_kV(get_double_data(data.front(),"100.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int bt = get_integer_data(data.front(),"1");
            if(bt==1) bus.set_bus_type(PQ_TYPE);
            if(abs(bt)==2) bus.set_bus_type(PV_TYPE);
            if(bt==3) bus.set_bus_type(SLACK_TYPE);
            if(bt==4) bus.set_bus_type(OUT_OF_SERVICE);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_area_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_zone_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_owner_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_positive_sequence_voltage_in_pu(get_double_data(data.front(),"1.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_positive_sequence_angle_in_deg(get_double_data(data.front(),"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_normal_voltage_upper_limit_in_pu(get_double_data(data.front(),"1.1"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_normal_voltage_lower_limit_in_pu(get_double_data(data.front(),"0.9"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_emergency_voltage_upper_limit_in_pu(get_double_data(data.front(),"1.1"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_emergency_voltage_lower_limit_in_pu(get_double_data(data.front(),"0.9"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_base_frequency_in_Hz(get_double_data(data.front(),"0.0"));
            data.erase(data.begin());
        }
        psdb.append_bus(bus);
    }
}
void STEPS_IMEXPORTER::load_load_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<3)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[2];
    vector<string> data;

    unsigned int ndata = DATA.size();
    double p = 0.0, q = 0.0;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];
        LOAD load(toolkit);

        if(data.size()>0)
        {
            load.set_load_bus(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_identifier(get_string_data(data.front(),""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int status = get_integer_data(data.front(),"1");
            if(status==1) load.set_status(true);
            else          load.set_status(false);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_area_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_zone_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_power_load_in_MVA(complex<double>(p,q));
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_current_load_in_MVA(complex<double>(p,q));
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_impedance_load_in_MVA(complex<double>(p,-q));
        if(data.size()>0)
        {
            load.set_owner_number(get_integer_data(data.front(),"1"));
            data.erase(data.begin());
        }

        psdb.append_load(load);
    }
}
void STEPS_IMEXPORTER::load_fixed_shunt_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<4)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[3];
    vector<string> data;

    unsigned int ndata = DATA.size();
    unsigned int n=0;
    double p = 0.0, q = 0.0;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];
        FIXED_SHUNT shunt(toolkit);

        if(data.size()>0)
        {
            shunt.set_shunt_bus(get_integer_data(data[n],"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            shunt.set_identifier(get_string_data(data[n],""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int status = get_integer_data(data[n],"1");
            if(status==1) shunt.set_status(true);
            else          shunt.set_status(false);
            data.erase(data.begin());
        }
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        shunt.set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double>(p,-q));

        psdb.append_fixed_shunt(shunt);
    }
}
void STEPS_IMEXPORTER::load_source_data()
{
    if(splitted_sraw_data_in_ram.size()<5)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[4];
    vector<string> data;

    unsigned int ndata = DATA.size();

    unsigned int SOURCE_TYPE_INDEX = 28;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];

        unsigned int n = data.size();

        SOURCE_TYPE source_type = SYNC_GENERATOR_SOURCE;
        if(n>SOURCE_TYPE_INDEX)
        {
            int type = get_integer_data(data[SOURCE_TYPE_INDEX],"0");
            switch(type)
            {
                case 0:
                    source_type = SYNC_GENERATOR_SOURCE;
                    break;
                case 1:
                    source_type = WT_GENERATOR_SOURCE;
                    break;
                case 2:
                    source_type = PV_UNIT_SOURCE;
                    break;
                case 3:
                    source_type = ENERGY_STORAGE_SOURCE;
                    break;
                default:
                    source_type = SYNC_GENERATOR_SOURCE;
                    break;
            }
        }
        switch(source_type)
        {
            case SYNC_GENERATOR_SOURCE:
            {
                load_generator_data(data);
                break;
            }
            case WT_GENERATOR_SOURCE:
            {
                load_wt_generator_data(data);
                break;
            }
            case PV_UNIT_SOURCE:
            {
                load_pv_unit_data(data);
                break;
            }
            case ENERGY_STORAGE_SOURCE:
            {
                load_energy_storage_data(data);
                break;
            }
            default:
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Invalid source type is detected in STEPS sraw file of line:\n%s",string_vector2csv(data).c_str());
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(buffer);
                break;
            }
        }
    }
    /*
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<SOURCE*> sources = psdb.get_all_sources();
    for(unsigned int i=0; i<sources.size(); ++i)
        sources[i]->report();
    */
}

void STEPS_IMEXPORTER::load_generator_data(vector<string>& data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    GENERATOR generator(toolkit);

    load_source_common_data(data, &generator);

    psdb.append_generator(generator);
}

void STEPS_IMEXPORTER::load_wt_generator_data(vector<string>& data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    WT_GENERATOR wt_generator(toolkit);

    load_source_common_data(data, &wt_generator);
    load_source_var_control_data(data, &wt_generator);

    psdb.append_wt_generator(wt_generator);
}


void STEPS_IMEXPORTER::load_pv_unit_data(vector<string>& data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    PV_UNIT pv_unit(toolkit);

    load_source_common_data(data, &pv_unit);
    load_source_var_control_data(data, &pv_unit);

    psdb.append_pv_unit(pv_unit);
}


void STEPS_IMEXPORTER::load_energy_storage_data(vector<string>& data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ENERGY_STORAGE estorage(toolkit);

    load_source_common_data(data, &estorage);
    load_source_var_control_data(data, &estorage);

    psdb.append_energy_storage(estorage);
}

void STEPS_IMEXPORTER::load_source_common_data(vector<string>& data, SOURCE* source)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double rs = 0.0, xs = 0.0;
    //double rt = 0.0, xt = 0.0, tt = 1.0;
    int status;
    if(data.size()>0)
    {
        source->set_source_bus(get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_identifier(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_generation_in_MW(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_generation_in_MVar(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_max_in_MVar(get_double_data(data.front(),"999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_min_in_MVar(get_double_data(data.front(),"-999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_voltage_to_regulate_in_pu(get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_bus_to_regulate(get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        char mbase[100];
        snprintf(mbase, 100, "%lf",psdb.get_system_base_power_in_MVA());
        source->set_mbase_in_MVA(get_double_data(data.front(),mbase));
        data.erase(data.begin());
    }
    rs = 0.0; xs = 0.0;
    if(data.size()>0)
    {
        rs = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        xs = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    source->set_source_impedance_in_pu(complex<double>(rs, xs));
    //rt = 0.0; xt = 0.0;
    if(data.size()>0)
    {
        //rt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        //xt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        //tt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    status = 0;
    if(data.size()>0)
    {
        status = get_integer_data(data.front(),"1");
        if(status == 1) source->set_status(true);
        else            source->set_status(false);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        get_double_data(data.front(),"100.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_max_in_MW(get_double_data(data.front(),"999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_min_in_MW(get_double_data(data.front(),"-999.0"));
        data.erase(data.begin());
    }
    OWNERSHIP os;
    int owner=0; double frac=0.0;
    for(int i=0; i!=4; ++i)
    {
        owner = 0;
        frac = 0.0;
        if(data.size()>0)
        {
            owner = get_integer_data(data.front(),"0");
            data.erase(data.begin());
        }
        else
            break;

        if(data.size()>0)
        {
            frac = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        else
            break;

        os.append_owner_and_its_fraction(owner, frac);
    }
    os.normalize();
    source->set_ownership(os);
}

void STEPS_IMEXPORTER::load_source_var_control_data(vector<string>& data, SOURCE* source)
{
    double pgen = source->get_p_generation_in_MW();

    int var_control_mode = 0;
    double pf = 1.0;
    if(data.size()>0)
    {
        var_control_mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(data.size()>0)
        {
            pf = get_double_data(data.front(),"1.0");
            data.erase(data.begin());
        }
        if(pf==0.0)
            pf = 0.01;
        if(pf<0.0)
            pf = -pf;
        if(pf>1.0)
            pf = 1.0;;

        switch(var_control_mode)
        {
            case 0:
            case 1: // qmax and qmin to hold voltage
            {
                break;
            }
            case 2: // constant power factor to hold voltage
            {
                double qmax = 0.0;
                if(pf==1.0)
                    qmax = 0.0;
                else
                    qmax = pgen/pf*steps_sqrt(1.0-pf*pf);
                source->set_q_max_in_MVar(qmax);
                source->set_q_min_in_MVar(-qmax);
                break;
            }
            case 3: // constant power
            default:
            {
                double qgen = source->get_q_generation_in_MVar();
                source->set_q_max_in_MVar(qgen);
                source->set_q_min_in_MVar(qgen);
                break;
            }
        }
    }
}

void STEPS_IMEXPORTER::load_line_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<6)
        return;
    vector<vector<string> >DATA = splitted_sraw_data_in_ram[5];
    vector<string> data;

    unsigned int ndata = DATA.size();
    unsigned int n=0;
    double r = 0.0, x = 0.0, g = 0.0, b = 0.0;
    int status;
    RATING rating;
    int meterend;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];
        LINE line(toolkit);

        if(data.size()>0)
        {
            line.set_sending_side_bus(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            line.set_receiving_side_bus(get_integer_data(data[n],"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            line.set_identifier(get_string_data(data[n],""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            r=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_line_positive_sequence_z_in_pu(complex<double>(r,x));
        if(data.size()>0)
        {
            b=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_line_positive_sequence_y_in_pu(complex<double>(0.0,b));
        rating.clear();
        if(data.size()>0)
        {
            rating.set_rating_A_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            rating.set_rating_B_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            rating.set_rating_C_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        line.set_rating(rating);
        g=0.0; b=0.0;
        if(data.size()>0)
        {
            g = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            b = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(g,b));
        g=0.0; b=0.0;
        if(data.size()>0)
        {
            g = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            b = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(g,b));
        status  = 0;
        if(data.size()>0)
        {
            status = get_integer_data(data[n],"0");
            data.erase(data.begin());
        }
        if(status == 1)
        {
            line.set_sending_side_breaker_status(true);
            line.set_receiving_side_breaker_status(true);
        }
        else
        {
            line.set_sending_side_breaker_status(false);
            line.set_receiving_side_breaker_status(false);
        }
        meterend = 1;
        if(data.size()>0)
        {
            meterend = get_integer_data(data[n],"1");
            data.erase(data.begin());
        }
        if(meterend<=1) line.set_meter_end_bus(line.get_sending_side_bus());
        else            line.set_meter_end_bus(line.get_receiving_side_bus());
        if(data.size()>0)
        {
            line.set_length(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        OWNERSHIP os;
        int owner=0; double frac=0.0;
        for(unsigned int j=0; j!=4; ++j)
        {
            owner = 0;
            frac = 0.0;
            if(data.size()>0)
            {
                owner = get_integer_data(data.front(),"0");
                data.erase(data.begin());
            }
            else
                break;

            if(data.size()>0)
            {
                frac = get_double_data(data.front(),"0.0");
                data.erase(data.begin());
            }
            else
                break;

            os.append_owner_and_its_fraction(owner, frac);
        }
        os.normalize();
        line.set_ownership(os);

        psdb.append_line(line);
    }
}
void STEPS_IMEXPORTER::load_transformer_data()
{
    if(splitted_sraw_data_in_ram.size()<7)
        return;
    vector<vector<string> >DATA = splitted_sraw_data_in_ram[6];

    vector<string> data;

    unsigned int ndata = DATA.size();

    vector<vector<string> > trans_data;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        trans_data.clear();

        data = DATA[i];
        string kbus = data[2];
        trans_data.push_back(data);
        ++i;
        if(i>=ndata)
            break;
        data = DATA[i];
        trans_data.push_back(data);
        ++i;
        if(i>=ndata)
            break;
        data = DATA[i];
        trans_data.push_back(data);
        ++i;
        if(i>=ndata)
            break;
        data = DATA[i];
        trans_data.push_back(data);

        if(kbus=="" or kbus=="0")
        {
            data.clear();
            trans_data.push_back(data);
        }
        else
        {
            ++i;
            if(i>=ndata)
                break;
            data = DATA[i];
            trans_data.push_back(data);
        }
        add_transformer_with_data(trans_data);
    }
}

void STEPS_IMEXPORTER::add_transformer_with_data(vector<vector<string> > trans_data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    TRANSFORMER trans(toolkit);

    vector<string> data_trans = trans_data[0];
    vector<string> data_z = trans_data[1];
    vector<string> data_winding_p = trans_data[2];
    vector<string> data_winding_s = trans_data[3];
    vector<string> data_winding_t = trans_data[4];

    //string str, strval;
    //vector<string> data;

    TRANSFORMER_WINDING_TAP_CODE winding_code = TAP_WINDING_VOLTAGE_IN_KV;
    TRANSFORMER_IMPEDANCE_CODE impedance_code = IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
    TRANSFORMER_ADMITTANCE_CODE magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;

    switch(get_integer_data(data_trans[4],"1"))
    {
        case 1:
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE;
            break;
        case 2:
            winding_code = TAP_WINDING_VOLTAGE_IN_KV;
            break;
        case 3:
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE;
            break;
        default: // default is 1
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE;
            break;
    };

    switch(get_integer_data(data_trans[5],"1"))
    {
        case 1:
            impedance_code = IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        case 2:
            impedance_code = IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        case 3:
            impedance_code = IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        default: // default is 1
            impedance_code = IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
    }
    switch(get_integer_data(data_trans[6],"1"))
    {
        case 1:
            magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;
            break;
        case 2:
            magnetizing_code = ADMITTANCE_LOSS_IN_WATT_AND_CURRENT_IN_PU_ON_PRIMARY_SECONDARY_WINDINGS_POWER_AND_PRIMARY_WINDING_NOMINAL_VOLTAGE;
            break;
        default: // default is 1
            magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;
            break;
    }

    // first get the transformer information
    add_transformer_basic_data(trans, data_trans);
    add_transformer_winding_data(trans, PRIMARY_SIDE, data_winding_p, winding_code);
    add_transformer_winding_data(trans, SECONDARY_SIDE, data_winding_s, winding_code);
    add_transformer_winding_data(trans, TERTIARY_SIDE, data_winding_t, winding_code);
    add_transformer_impedance_admittance_data(trans, data_z, impedance_code, magnetizing_code);

    psdb.append_transformer(trans);
}


void STEPS_IMEXPORTER::add_transformer_basic_data(TRANSFORMER& trans, vector<string> data)
{
    double gm = 0.0, bm = 0.0;
    if(data.size()>0)
    {
        trans.set_winding_bus(PRIMARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_winding_bus(SECONDARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_winding_bus(TERTIARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_identifier(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // winding_code is already set in its father function
        //winding_code = get_integer_data(data.front(),"0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // impedance_code is already set in its father function
        //impedance_code = get_integer_data(data.front(),"0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // magnetizing_code code is already set in its father function
        //magnetizing_code = get_integer_data(data.front(),"1");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        gm = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        bm = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(gm,bm));
    if(data.size()>0)
    {
        int nonmeterend = get_integer_data(data.front(),"2");
        data.erase(data.begin());
        switch(nonmeterend)
        {
            case 1:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(PRIMARY_SIDE));
                break;
            }
            case 2:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(SECONDARY_SIDE));
                break;
            }
            case 3:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(TERTIARY_SIDE));
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        trans.set_name(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        int status = get_integer_data(data.front(),"1");
        data.erase(data.begin());

        switch(status)
        {
            case 1:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 2:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, false);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 3:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, false);
                break;
            }
            case 4:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, false);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 0:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, false);
                trans.set_winding_breaker_status(SECONDARY_SIDE, false);
                trans.set_winding_breaker_status(TERTIARY_SIDE, false);
                break;
            }
            default:
                break;
        }
    }

    OWNERSHIP os;
    int owner=0; double frac=0.0;
    for(unsigned int j=0; j!=4; ++j)
    {
        owner = 0;
        frac = 0.0;
        if(data.size()>0)
        {
            owner = get_integer_data(data.front(),"0");
            data.erase(data.begin());
        }
        else
            break;

        if(data.size()>0)
        {
            frac = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        else
            break;

        os.append_owner_and_its_fraction(owner, frac);
    }
    os.normalize();
    trans.set_ownership(os);
}
void STEPS_IMEXPORTER::add_transformer_winding_data(TRANSFORMER&trans, TRANSFORMER_WINDING_SIDE winding, vector<string> data, TRANSFORMER_WINDING_TAP_CODE winding_code)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(data.size()<2)
        return;
    trans.set_winding_nominal_voltage_in_kV(winding, get_double_data(data[1],"0.0"));
    data.erase(data.begin()+1);
    if(data.size()>0)
    {
        double t = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(winding_code)
        {
            case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
            {
                t *= psdb.get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                t /= trans.get_winding_nominal_voltage_in_kV(winding);
                break;
            }
            case TAP_WINDING_VOLTAGE_IN_KV:
            {
                t /= trans.get_winding_nominal_voltage_in_kV(winding);
                break;
            }
            default: // TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                break;
        }
        trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, t);
    }
    if(data.size()>0)
    {
        trans.set_winding_angle_shift_in_deg(winding, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    RATING rate;
    if(data.size()>0)
    {
        rate.set_rating_A_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        rate.set_rating_B_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        rate.set_rating_C_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    trans.set_winding_rating_in_MVA(winding, rate);
    if(data.size()>0)
    {
        int mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        switch(mode)
        {
            case 0:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_NO_CONTROL);
                break;
            }
            case 1:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_VOLTAGE_CONTROL);
                break;
            }
            case 2:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_REACTIVE_POWER_CONTROL);
                break;
            }
            case 3:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_ACTIVE_POWER_CONTROL);
                break;
            }
            case 4:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_HVDC_LINE_CONTROL);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        trans.set_winding_controlled_bus(winding, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double tamax = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_NO_CONTROL:
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            {
                switch(winding_code)
                {
                    case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
                    {
                        tamax *= psdb.get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                        tamax /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    case TAP_WINDING_VOLTAGE_IN_KV:
                    {
                        tamax /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    default: // TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                        break;
                }
                trans.set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, tamax);
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_winding_max_angle_shift_in_deg(winding, tamax);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double tamin = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_NO_CONTROL:
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            {
                switch(winding_code)
                {
                    case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
                    {
                        tamin *= psdb.get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                        tamin /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    case TAP_WINDING_VOLTAGE_IN_KV:
                    {
                        tamin /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    default: //TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                        break;
                }
                trans.set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, tamin);
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_winding_min_angle_shift_in_deg(winding, tamin);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double vpqmax = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            {
                trans.set_winding_controlled_max_voltage_in_pu(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_max_reactive_power_into_winding_in_MVar(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_max_active_power_into_winding_in_MW(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            case TRANSFORMER_TAP_NO_CONTROL:
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double vpqmin = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            {
                trans.set_winding_controlled_min_voltage_in_pu(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_min_reactive_power_into_winding_in_MVar(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_min_active_power_into_winding_in_MW(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            case TRANSFORMER_TAP_NO_CONTROL:
            default:
                break;
        }
    }

    if(data.size()>0)
    {
        trans.set_winding_number_of_taps(winding, get_integer_data(data.front(),"33"));
        data.erase(data.begin());
    }
}

void STEPS_IMEXPORTER::add_transformer_impedance_admittance_data(TRANSFORMER& trans, vector<string> data, TRANSFORMER_IMPEDANCE_CODE impedance_code, TRANSFORMER_ADMITTANCE_CODE magnetizing_code)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double g = trans.get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().real();
    double b = trans.get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().imag();

    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double vbase, sbase;

    double r = 0.0, x = 0.0, s = 0.0;

    if(data.size()>0)
    {
        r = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        x = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        s = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    switch(impedance_code)
    {
        case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
        {
            r = r*s*one_over_sbase;
            x = x*s*one_over_sbase;
            break;
        }
        case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
        {
            r = r/s;
            x = steps_sqrt(x*x-r*r);
            break;
        }
        default: // IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
            break;
    }
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, complex<double>(r,x));
    trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, s);

    if(trans.is_three_winding_transformer())
    {
        r = 0.0; x = 0.0; s = 0.0;
        if(data.size()>0)
        {
            r = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            s = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        switch(impedance_code)
        {
            case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r*s*one_over_sbase;
                x = x*s*one_over_sbase;
                break;
            }
            case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r/s;
                x = steps_sqrt(x*x-r*r);
                break;
            }
            default:// IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
                break;
        }
        trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, complex<double>(r,x));
        trans.set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE, s);

        r = 0.0; x = 0.0; s = 0.0;
        if(data.size()>0)
        {
            r = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            s = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        switch(impedance_code)
        {
            case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r*s*one_over_sbase;
                x = x*s*one_over_sbase;
                break;
            }
            case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r/s;
                x = steps_sqrt(x*x-r*r);
                break;
            }
            default: // IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
                break;
        }
        trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE, complex<double>(r,x));
        trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE, s);
    }

    switch(magnetizing_code)
    {
        case ADMITTANCE_LOSS_IN_WATT_AND_CURRENT_IN_PU_ON_PRIMARY_SECONDARY_WINDINGS_POWER_AND_PRIMARY_WINDING_NOMINAL_VOLTAGE:
            g = g*1e-6;
            vbase = trans.get_winding_nominal_voltage_in_kV(PRIMARY_SIDE);
            sbase = trans.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);
            g = g/(vbase*vbase);
            b = b*sbase/(vbase*vbase);
            sbase = psdb.get_system_base_power_in_MVA();
            vbase = psdb.get_bus_base_voltage_in_kV(trans.get_winding_bus(PRIMARY_SIDE));
            g = g/(sbase/(vbase*vbase));
            b = b/(sbase/(vbase*vbase));
            b = steps_sqrt(b*b-g*g);
            b = -b;
            trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(g,b));
            break;
        default: //ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE is default of steps
            break;
    }
}

void STEPS_IMEXPORTER::load_area_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<8)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[7];

    vector<string> data;

    unsigned int ndata = DATA.size();

    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];

        AREA area(toolkit);

        area.set_area_number(get_integer_data(data[0],"0"));
        area.set_area_swing_bus(get_integer_data(data[1],"0"));
        area.set_expected_power_leaving_area_in_MW(get_double_data(data[2],"0.0"));
        area.set_area_power_mismatch_tolerance_in_MW(get_double_data(data[3],"10.0"));
        area.set_area_name(get_string_data(data[4],""));

        psdb.append_area(area);
    }
}

void STEPS_IMEXPORTER::load_hvdc_data()
{
    if(splitted_sraw_data_in_ram.size()<9)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[8];

    vector<string> data;

    unsigned int ndata = DATA.size();
    RATING rating;

    vector<vector<string> > hvdc_data;
    for(unsigned int i=0; i!=ndata; ++i)
    {
        hvdc_data.clear();

        data = DATA[i];
        hvdc_data.push_back(data);
        i++;
        if(i>=ndata)
            break;
        data = DATA[i];
        hvdc_data.push_back(data);
        i++;
        if(i>=ndata)
            break;
        data = DATA[i];
        hvdc_data.push_back(data);

        add_hvdc_with_data(hvdc_data);
    }
}

void STEPS_IMEXPORTER::add_hvdc_with_data(vector<vector<string> > hvdc_data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVDC hvdc(toolkit);

    vector<string> data_hvdc = hvdc_data[0];
    vector<string> data_rec = hvdc_data[1];
    vector<string> data_inv = hvdc_data[2];

    add_hvdc_basic_data(hvdc, data_hvdc);
    add_hvdc_converter_data(hvdc, RECTIFIER, data_rec);
    add_hvdc_converter_data(hvdc, INVERTER, data_inv);

    while(psdb.is_hvdc_exist(hvdc.get_device_id()))
        hvdc.set_identifier(hvdc.get_identifier()+"#");

    psdb.append_hvdc(hvdc);
}
void STEPS_IMEXPORTER::add_hvdc_basic_data(HVDC& hvdc, vector<string> data)
{
    if(data.size()>0)
    {
        string name = get_string_data(data.front(),"");
        hvdc.set_name(name);
        hvdc.set_identifier(name);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        int mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(mode==0)
            hvdc.set_status(false);
        else
        {
            hvdc.set_status(true);
            if(mode==1)
                hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
            else
                hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
        }
    }
    if(data.size()>0)
    {
        hvdc.set_line_resistance_in_ohm(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double value = get_double_data(data.front(), "0.0");
        data.erase(data.begin());
        switch(hvdc.get_converter_operation_mode(RECTIFIER))
        {
            case RECTIFIER_CONSTANT_CURRENT:
                hvdc.set_nominal_dc_current_in_kA(value*1e-3);
                break;
            default:// constant power + blocked
                hvdc.set_nominal_dc_power_in_MW(fabs(value));
                if(value>0.0)
                    hvdc.set_side_to_hold_power(RECTIFIER);
                else
                    hvdc.set_side_to_hold_power(INVERTER);
                break;
        }
    }
    if(data.size()>0)
    {
        hvdc.set_nominal_dc_voltage_in_kV(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(get_double_data(data.front(),"0.15"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_current_power_margin(get_double_data(data.front(),"0.15"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        string meterend = get_string_data(data.front(),"I");
        if(meterend=="R")
            hvdc.set_meter_end(RECTIFIER);
        else
            hvdc.set_meter_end(INVERTER);
        data.erase(data.begin());
    }
}

void STEPS_IMEXPORTER::add_hvdc_converter_data(HVDC& hvdc, CONVERTER_SIDE converter, vector<string> data)
{
    if(data.size()>0)
    {
        hvdc.set_converter_bus(converter, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_number_of_bridge(converter, get_integer_data(data.front(),"1"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_max_alpha_or_gamma_in_deg(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_min_alpha_or_gamma_in_deg(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    double r=0.0, x=0.0;
    if(data.size()>0)
    {
        r = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        x = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    hvdc.set_converter_transformer_impedance_in_ohm(converter, complex<double>(r,x));
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double turn_ratio = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        double v = hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(converter)*turn_ratio;
        hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(converter, v);
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_max_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_min_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double step = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        double tap = hvdc.get_converter_transformer_max_tap_in_pu(converter)-hvdc.get_converter_transformer_min_tap_in_pu(converter);
        int n = int(fabs(round(tap/step)))+1;
        hvdc.set_converter_transformer_number_of_taps(converter, n);
    }
    if(data.size()>0)
    {
        data.erase(data.begin());
    }
}


void STEPS_IMEXPORTER::load_lcc_hvdc_data()
{

}

void STEPS_IMEXPORTER::add_lcc_hvdc_with_data(vector<vector<string> > hvdc_data)
{

}

void STEPS_IMEXPORTER::add_lcc_hvdc_basic_data(LCC_HVDC& hvdc, vector<string> data)
{

}

void STEPS_IMEXPORTER::add_lcc_hvdc_converter_data(LCC_HVDC& hvdc, CONVERTER_SIDE side, unsigned int converter_index, vector<string> data)
{

}

void STEPS_IMEXPORTER::load_vsc_hvdc_data()
{
    if(splitted_sraw_data_in_ram.size()<10)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[9];
    vector<string> data;
    unsigned int ndata=DATA.size();
    vector<vector<string> > vsc_hvdc_data;
    for(unsigned int i=0;i<ndata;)
    {
        vsc_hvdc_data.clear();
        data=DATA[i];
        vsc_hvdc_data.push_back(data);
        ++i;
        unsigned int ncon=get_integer_data(data[1],"0");
        unsigned int nbus=get_integer_data(data[2],"0");
        unsigned int ndc_line=get_integer_data(data[3],"0");
        unsigned int nlines_to_read= ncon+nbus+ndc_line;
        for(unsigned int j=0; j<nlines_to_read;++j)
        {
            if(i>=ndata) break;
            data=DATA[i];
            vsc_hvdc_data.push_back(data);
            ++i;
        }
        add_vsc_hvdc_with_data(vsc_hvdc_data);
    }
}

void STEPS_IMEXPORTER::add_vsc_hvdc_with_data(vector<vector<string> > vsc_hvdc_data)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC vsc_hvdc(toolkit);
    unsigned int i = 0;

    vector<string> basic_data=vsc_hvdc_data[i];
    ++i;

    unsigned int ncon=get_integer_data(basic_data[1],"0");
    unsigned int nbus=get_integer_data(basic_data[2],"0");
    unsigned int ndc_line=get_integer_data(basic_data[3],"0");

    vector<vector<string> > vsc_hvdc_converter_data;
    vector<vector<string> > vsc_hvdc_dc_bus_data;
    vector<vector<string> > vsc_hvdc_dc_line_data;
    vector<string> data;
    vsc_hvdc_converter_data.clear();
    vsc_hvdc_dc_bus_data.clear();
    vsc_hvdc_dc_line_data.clear();
    data.clear();

    for(unsigned int j=0;j<ncon;++j)
    {
        data=vsc_hvdc_data[i];
        vsc_hvdc_converter_data.push_back(data);
        ++i;
    }

    for(unsigned int j=0;j<nbus;++j)
    {
        data=vsc_hvdc_data[i];
        vsc_hvdc_dc_bus_data.push_back(data);
        ++i;
    }

    for(unsigned int j=0;j<ndc_line;++j)
    {
        data=vsc_hvdc_data[i];
        vsc_hvdc_dc_line_data.push_back(data);
        ++i;
    }

    add_vsc_hvdc_basic_data(vsc_hvdc, basic_data);
    add_vsc_hvdc_converter_data(vsc_hvdc, vsc_hvdc_converter_data);
    add_vsc_hvdc_dc_bus_data(vsc_hvdc, vsc_hvdc_dc_bus_data);
    add_vsc_hvdc_dc_line_data(vsc_hvdc, vsc_hvdc_dc_line_data);

    while(psdb.is_vsc_hvdc_exist(vsc_hvdc.get_device_id()))
        vsc_hvdc.set_identifier(vsc_hvdc.get_identifier()+"#");
    psdb.append_vsc_hvdc(vsc_hvdc);
}

void STEPS_IMEXPORTER::add_vsc_hvdc_basic_data(VSC_HVDC& vsc_hvdc, vector<string> data)
{
    if(data.size()>0)
    {
        string name = get_string_data(data.front(), "");
        vsc_hvdc.set_name(name);
        vsc_hvdc.set_identifier(name);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        unsigned int n_converter = get_integer_data(data.front(),"0");
        vsc_hvdc.set_converter_count(n_converter);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        unsigned int n_dc_bus=get_integer_data(data.front(),"0");
        vsc_hvdc.set_dc_bus_count(n_dc_bus);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        unsigned int n_dc_line=get_integer_data(data.front(),"0");
        vsc_hvdc.set_dc_line_count(n_dc_line);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        int mode=get_integer_data(data.front(),"0");
        if(mode==0)
            vsc_hvdc.set_status(false);
        else
            vsc_hvdc.set_status(true);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        unsigned int bus_vcon=get_integer_data(data.front(),"0");
        //vsc_hvdc.set_ac_converter_bus_with_dc_voltage_control(bus_vcon);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        unsigned int dc_base_voltage=get_double_data(data.front(),"0.0");
        vsc_hvdc.set_dc_network_base_voltage_in_kV(dc_base_voltage);
        data.erase(data.begin());
    }

}

void STEPS_IMEXPORTER::add_vsc_hvdc_converter_data(VSC_HVDC& vsc_hvdc, vector<vector<string> > vsc_hvdc_converter_data)
{
    vector<string> data;
    data.clear();

    unsigned int n_converter = vsc_hvdc_converter_data.size();

    for(unsigned int i=0;i!=n_converter;++i)
    {
        data=vsc_hvdc_converter_data[i];
        if(data.size()>0)
        {
            int ibus=get_integer_data(data.front(),"0");
            vsc_hvdc.set_converter_ac_bus(i, ibus);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int type=get_integer_data(data.front(),"0");
            data.erase(data.begin());
            if(type==1)
                vsc_hvdc.set_converter_dc_operation_mode(i, VSC_DC_VOLTAGE_CONTORL);
            else
                vsc_hvdc.set_converter_dc_operation_mode(i, VSC_AC_ACTIVE_POWER_CONTORL);
        }
        if(data.size()>0)
        {
            int mode=get_integer_data(data.front(),"0");
            data.erase(data.begin());
            if(mode==1)
                vsc_hvdc.set_converter_ac_operation_mode(i,VSC_AC_VOLTAGE_CONTROL);
            else
                vsc_hvdc.set_converter_ac_operation_mode(i,VSC_AC_REACTIVE_POWER_CONTROL);
        }
        if(data.size()>0)
        {
            double dcset=get_double_data(data.front(),"0.0");
            data.erase(data.begin());
            if(vsc_hvdc.get_converter_dc_operation_mode(i)==VSC_DC_VOLTAGE_CONTORL)
            {
                vsc_hvdc.set_converter_nominal_dc_voltage_command_in_kV(i,dcset);
            }

            if(vsc_hvdc.get_converter_dc_operation_mode(i)==VSC_AC_ACTIVE_POWER_CONTORL)
            {
                vsc_hvdc.set_converter_nominal_ac_active_power_command_in_MW(i,dcset);
            }

        }
        if(data.size()>0)
        {
            double acset=get_double_data(data.front(),"0.0");
            data.erase(data.begin());
            if(vsc_hvdc.get_converter_ac_operation_mode(i)==VSC_AC_VOLTAGE_CONTROL)
            {
                vsc_hvdc.set_converter_nominal_ac_voltage_command_in_pu(i,acset);
            }

            if(vsc_hvdc.get_converter_ac_operation_mode(i)==VSC_AC_REACTIVE_POWER_CONTROL)
            {
                vsc_hvdc.set_converter_nominal_ac_reactive_power_command_in_Mvar(i,acset);
            }

        }
        if(data.size()>0)
        {
            double Aloss=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_loss_factor_A_in_kW(i,Aloss);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double Bloss=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_loss_factor_B_in_kW_per_amp(i,Bloss);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double minloss=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_minimum_loss_in_kW(i,minloss);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double smax=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_rated_capacity_in_MVA(i,smax);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double imax=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_rated_current_in_A(i,imax);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double pwf=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_power_weighting_factor(i,pwf);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double maxq=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_Qmax_in_MVar(i,maxq);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double minq=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_Qmin_in_MVar(i,minq);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int remot=get_integer_data(data.front(),"0");
            vsc_hvdc.set_converter_remote_bus_to_regulate(i,remot);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double rmpct=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_converter_remote_regulation_percent(i,rmpct);
            data.erase(data.begin());
        }
    }
}

void STEPS_IMEXPORTER::add_vsc_hvdc_dc_bus_data(VSC_HVDC& vsc_hvdc, vector<vector<string> > vsc_hvdc_dc_bus_data)
{
    vector<string> data;
    data.clear();

    unsigned int nbus = vsc_hvdc_dc_bus_data.size();

    for(unsigned int i=0;i!=nbus;++i)
    {
        data=vsc_hvdc_dc_bus_data[i];
        if(data.size()>0)
        {
            int idc=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_number(i,idc);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int ib=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_ac_bus_number(i,ib);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int area=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_area(i,area);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int zone=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_zone(i,zone);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            string dcname=get_string_data(data.front(),"");
            vsc_hvdc.set_dc_bus_name(i,dcname);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double rgrnd=get_double_data(data.front(),"");
            vsc_hvdc.set_dc_bus_ground_resistance_in_ohm(i,rgrnd);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int owner=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_owner_number(i,owner);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double supply_power=get_double_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_generation_power_in_MW(i,supply_power);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double cons_power=get_double_data(data.front(),"0");
            vsc_hvdc.set_dc_bus_load_power_in_MW(i,cons_power);
            data.erase(data.begin());
        }
    }
}

void STEPS_IMEXPORTER::add_vsc_hvdc_dc_line_data(VSC_HVDC& vsc_hvdc, vector<vector<string> > vsc_hvdc_dc_line_data)
{
    vector<string> data;
    data.clear();

    unsigned int ndc_line = vsc_hvdc_dc_line_data.size();

    for(unsigned int i=0;i!=ndc_line;++i)
    {
        data=vsc_hvdc_dc_line_data[i];
        if(data.size()>0)
        {
            int idc=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_line_sending_side_bus(i,idc);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int jdc=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_line_receiving_side_bus(i,jdc);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            string identifier=get_string_data(data.front(),"");
            vsc_hvdc.set_dc_line_identifier(i,identifier);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int met=get_integer_data(data.front(),"0");
            vsc_hvdc.set_dc_line_meter_end_bus(i,met);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double Rdc=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_dc_line_resistance_in_ohm(i,Rdc);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            double Ldc=get_double_data(data.front(),"0.0");
            vsc_hvdc.set_dc_line_inductance_in_mH(i,Ldc);
            data.erase(data.begin());
        }
    }
}

void STEPS_IMEXPORTER::load_transformer_impedance_correction_table_data()
{
    ;
}

void STEPS_IMEXPORTER::load_multi_terminal_hvdc_data()
{
    ;
}

void STEPS_IMEXPORTER::load_multi_section_line_data()
{
    ;
}

void STEPS_IMEXPORTER::load_zone_data()
{
    if(splitted_sraw_data_in_ram.size()<14)
        return;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<vector<string> > DATA = splitted_sraw_data_in_ram[13];

    vector<string> data;

    unsigned int ndata = DATA.size();

    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];

        ZONE zone(toolkit);

        zone.set_zone_number(get_integer_data(data[0],"0"));
        zone.set_zone_name(get_string_data(data[1],""));

        psdb.append_zone(zone);
    }
}

void STEPS_IMEXPORTER::load_interarea_transfer_data()
{
    ;
}

void STEPS_IMEXPORTER::load_owner_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(splitted_sraw_data_in_ram.size()<16)
        return;
    vector<vector<string> > DATA = splitted_sraw_data_in_ram[15];

    vector<string> data;

    unsigned int ndata = DATA.size();

    for(unsigned int i=0; i!=ndata; ++i)
    {
        data = DATA[i];

        OWNER owner(toolkit);

        owner.set_owner_number(get_integer_data(data[0],"0"));
        owner.set_owner_name(get_string_data(data[1],""));

        psdb.append_owner(owner);
    }
}

void STEPS_IMEXPORTER::load_facts_data()
{
    ;
}

void STEPS_IMEXPORTER::load_switched_shunt_data()
{
    ;
}


void STEPS_IMEXPORTER::export_powerflow_data(string file, bool export_zero_impedance_line, POWERFLOW_DATA_SAVE_MODE save_mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. STEPS sraw file "<<file<<" cannot be opened for exporting powerflow data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    set_export_zero_impedance_line_logic(export_zero_impedance_line);
    set_powerflow_data_save_mode(save_mode);

    if(export_zero_impedance_line==false)
        psdb.update_overshadowed_bus_count();
    else
        psdb.set_all_buses_un_overshadowed();

    ofs<<export_case_data();
    ofs<<export_bus_data();
    ofs<<"0 / END OF BUS DATA, BEGIN LOAD DATA"<<endl;
    ofs<<export_load_data();
    ofs<<"0 / END OF LOAD DATA, BEGIN FIXED SHUNT DATA"<<endl;
    ofs<<export_fixed_shunt_data();
    ofs<<"0 / END OF FIXED SHUNT DATA, BEGIN GENERATOR DATA"<<endl;
    ofs<<export_source_data();
    ofs<<"0 / END OF GENERATOR DATA, BEGIN TRANSMISSION LINE DATA"<<endl;
    ofs<<export_line_data();
    ofs<<"0 / END OF TRANSMISSION LINE DATA, BEGIN TRANSFORMER DATA"<<endl;
    ofs<<export_transformer_data();
    ofs<<"0 / END OF TRANSFORMER DATA, BEGIN AREA DATA"<<endl;
    ofs<<export_area_data();
    ofs<<"0 / END OF AREA DATA, BEGIN TWO-TERMINAL HVDC DATA"<<endl;
    ofs<<export_hvdc_data();
    ofs<<"'0' / END OF TWO-TERMINAL HVDC DATA, BEGIN VSC HVDC LINE DATA"<<endl;
    ofs<<export_vsc_hvdc_data();
    ofs<<"'0' / END OF VSC HVDC LINE DATA, BEGIN IMPEDANCE CORRECTION DATA"<<endl;
    ofs<<export_transformer_impedance_correction_table_data();
    ofs<<"0 / END OF IMPEDANCE CORRECTION DATA, BEGIN MULTI-TERMINAL HVDC DATA"<<endl;
    ofs<<export_multi_terminal_hvdc_data();
    ofs<<"0 / END OF MULTI-TERMINAL HVDC DATA, BEGIN MULTI-SECTION LINE DATA"<<endl;
    ofs<<export_multi_section_line_data();
    ofs<<"0 / END OF MULTI-SECTION LINE DATA, BEGIN ZONE DATA"<<endl;
    ofs<<export_zone_data();
    ofs<<"0 / END OF ZONE DATA, BEGIN INTER-AREA TRANSFER DATA"<<endl;
    ofs<<export_interarea_transfer_data();
    ofs<<"0 / END OF INTER-AREA TRANSFER DATA, BEGIN OWNER DATA"<<endl;
    ofs<<export_owner_data();
    ofs<<"0 / END OF OWNER DATA, BEGIN FACTS DEVICE DATA"<<endl;
    ofs<<export_facts_data();
    ofs<<"0 / END OF FACTS DEVICE DATA, BEGIN SWITCHED SHUNT DATA"<<endl;
    ofs<<export_switched_shunt_data();
    ofs<<"0 / END OF SWITCHED SHUNT DATA, BEGIN GNE DATA"<<endl;
    ofs<<"0 / END OF GNE DATA, BEGIN INDUCTION MACHINE DATA"<<endl;
    ofs<<"0 / END OF INDUCTION MACHINE DATA"<<endl;
    ofs<<"Q";

    ofs.close();

    if(export_zero_impedance_line==false)
        psdb.set_all_buses_un_overshadowed();//recover
}



string STEPS_IMEXPORTER::export_case_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    char buffer[1000];
    vector<unsigned int> buses = psdb.get_all_buses_number();
    double fbase = 50.0;
    if(buses.size()!=0)
        fbase = psdb.get_bus_base_frequency_in_Hz(buses[0]);
    snprintf(buffer, 1000, "0, %f, 33, 0, 0, %f", psdb.get_system_base_power_in_MVA(), fbase);
    osstream<<buffer<<endl;
    snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
    osstream<<buffer<<endl;
    snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
    osstream<<buffer<<endl;
    return osstream.str();
}

string STEPS_IMEXPORTER::export_bus_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<BUS*> buses = psdb.get_all_buses();
    unsigned int n = buses.size();
    char buffer[1000];
    for(unsigned int i=0; i!=n; ++i)
    {
        BUS* bus = buses[i];
        if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus->get_bus_number())!=0)
            continue;

        BUS_TYPE bus_type = bus->get_bus_type();
        if(bus_type == OUT_OF_SERVICE and get_export_out_of_service_bus_logic()==false)
            continue;

        int type = 4;
        if(bus_type == PQ_TYPE) type = 1;
        if(bus_type == PV_TYPE) type = 2;
        if(bus_type == PV_TO_PQ_TYPE_1 or bus_type == PV_TO_PQ_TYPE_2 or
           bus_type == PV_TO_PQ_TYPE_3 or bus_type == PV_TO_PQ_TYPE_4) type = -2;
        if(bus_type == SLACK_TYPE) type = 3;
        if(bus_type == OUT_OF_SERVICE) type = 4;

        snprintf(buffer, 1000, "%8u, \"%-16s\", %8.2f, %2d, %4u, %4u, %4u, %10.6f, %10.6f, %6.4f, %6.4f, %6.4f, %6.4f, %4.1f",
                 bus->get_bus_number(), (bus->get_bus_name()).c_str(), bus->get_base_voltage_in_kV(), type,
                 bus->get_area_number(), bus->get_zone_number(), bus->get_owner_number(),
                 bus->get_positive_sequence_voltage_in_pu(), bus->get_positive_sequence_angle_in_deg(),
                 bus->get_normal_voltage_upper_limit_in_pu(), bus->get_normal_voltage_lower_limit_in_pu(),
                 bus->get_emergency_voltage_upper_limit_in_pu(), bus->get_emergency_voltage_lower_limit_in_pu(),
                 bus->get_base_frequency_in_Hz());
        osstream<<buffer<<endl;
    }

    return osstream.str();
}

string STEPS_IMEXPORTER::export_load_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<LOAD*> loads = psdb.get_all_loads();
    unsigned int n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        LOAD* load = loads[i];

        unsigned int bus = load->get_load_bus();
        string ickt = load->get_identifier();
        if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus)!=0)
        {
            bus = psdb.get_equivalent_bus_of_bus(bus);
            ickt = ickt + toolkit.get_next_alphabeta();
        }
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(bus)==OUT_OF_SERVICE)
            continue;

        osstream<<right
                <<setw(8)<<bus<<", "
                <<"\""
                <<left
                <<setw(2)<<ickt
                <<"\""<<", "
                <<right
                <<load->get_status()<<", "
                <<setw(8)<<load->get_area_number()<<", "
                <<setw(8)<<load->get_zone_number()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_power_load_in_MVA().real()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_power_load_in_MVA().imag()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_current_load_in_MVA().real()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_current_load_in_MVA().imag()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_impedance_load_in_MVA().real()<<", "
                <<setw(8)<<setprecision(6)<<load->get_nominal_constant_impedance_load_in_MVA().imag()<<", "
                <<setw(8)<<load->get_owner_number()<<", "
                <<setw(8)<<load->get_flag_interruptable()
                <<endl;
    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_fixed_shunt_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<FIXED_SHUNT*> fshunts = psdb.get_all_fixed_shunts();
    unsigned int n = fshunts.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        FIXED_SHUNT* shunt = fshunts[i];

        unsigned int bus = shunt->get_shunt_bus();
        string ickt = shunt->get_identifier();
        if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus)!=0)
        {
            bus = psdb.get_equivalent_bus_of_bus(bus);
            ickt = ickt + toolkit.get_next_alphabeta();
        }
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(bus)==OUT_OF_SERVICE)
            continue;

        osstream<<right
                <<setw(8)<<bus<<", "
                <<"\""
                <<left
                <<setw(2)<<ickt
                <<"\""<<", "
                <<right
                <<shunt->get_status()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<shunt->get_nominal_positive_sequence_impedance_shunt_in_MVA().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<-shunt->get_nominal_positive_sequence_impedance_shunt_in_MVA().imag()<<endl;
    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_source_data() const
{
    ostringstream osstream;
    osstream<<export_generator_data();
    osstream<<export_wt_generator_data();
    osstream<<export_pv_unit_data();
    osstream<<export_energy_storage_data();

    return osstream.str();
}

string STEPS_IMEXPORTER::export_generator_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<GENERATOR*> generators = psdb.get_all_generators();
    unsigned int n = generators.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        GENERATOR* generator = generators[i];
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(generator->get_source_bus())==OUT_OF_SERVICE)
            continue;

        osstream<<export_source_common_data(generator);
        osstream<<"0, 0.00, 0"<<endl;
    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_wt_generator_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    unsigned int n = wt_generators.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        WT_GENERATOR* wt_generator = wt_generators[i];
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(wt_generator->get_source_bus())==OUT_OF_SERVICE)
            continue;

        osstream<<export_source_common_data(wt_generator);
        osstream<<export_source_var_control_data(wt_generator);
        osstream<<", 1"<<endl;
    }
    return osstream.str();
}


string STEPS_IMEXPORTER::export_pv_unit_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
    unsigned int n = pv_units.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        PV_UNIT* pv_unit = pv_units[i];
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(pv_unit->get_source_bus())==OUT_OF_SERVICE)
            continue;

        osstream<<export_source_common_data(pv_unit);
        osstream<<export_source_var_control_data(pv_unit);

        osstream<<", 2"<<endl;
    }
    return osstream.str();
}


string STEPS_IMEXPORTER::export_energy_storage_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<ENERGY_STORAGE*> estorages = psdb.get_all_energy_storages();
    unsigned int n = estorages.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        ENERGY_STORAGE* estorage = estorages[i];
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(estorage->get_source_bus())==OUT_OF_SERVICE)
            continue;

        osstream<<export_source_common_data(estorage);
        osstream<<export_source_var_control_data(estorage);

        osstream<<", 3"<<endl;
    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_source_common_data(SOURCE* source) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int bus = source->get_source_bus();
    unsigned int bus_to_regulate = source->get_bus_to_regulate();
    string ickt = source->get_identifier();
    if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus)!=0)
    {
        bus = psdb.get_equivalent_bus_of_bus(bus);
        ickt = ickt + toolkit.get_next_alphabeta();
        if(bus_to_regulate!=0)
            bus_to_regulate = psdb.get_equivalent_bus_of_bus(bus_to_regulate);
    }

    osstream<<right
           <<setw(8)<<bus<<", "
           <<"\""<<left
           <<setw(2)<<ickt<<"\""<<", "
           <<right
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_generation_in_MW()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_generation_in_MVar()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_max_in_MVar()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_min_in_MVar()<<", "
           <<setw(8)<<setprecision(6)<<fixed<<source->get_voltage_to_regulate_in_pu()<<", ";
    if(source->get_bus_to_regulate()==source->get_source_bus())
        osstream<<setw(8)<<0<<", ";
    else
        osstream<<setw(8)<<bus_to_regulate<<", ";
    osstream<<setw(10)<<setprecision(4)<<fixed<<source->get_mbase_in_MVA()<<", "
           <<setw(9)<<setprecision(6)<<fixed<<source->get_source_impedance_in_pu().real()<<", "
           <<setw(9)<<setprecision(6)<<fixed<<source->get_source_impedance_in_pu().imag()<<", "
           <<setprecision(2)<<0.0<<", "<<0.0<<", "<<1.0<<", "
           <<source->get_status()<<","
           <<setprecision(2)<<100.0<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_max_in_MW()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_min_in_MW()<<", ";
    if(source->get_owner_count()==0)
        osstream<<"1, 1.0, 0, 0.0, 0, 0.0, 0, 0.0, ";
    else
    {
        osstream<< setprecision(0) << source->get_owner_of_index(0) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(0) << ", "
                << setprecision(0) << source->get_owner_of_index(1) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(1) << ", "
                << setprecision(0) << source->get_owner_of_index(2) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(2) << ", "
                << setprecision(0) << source->get_owner_of_index(3) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(3) << ", ";
    }

    return osstream.str();
}

string STEPS_IMEXPORTER::export_source_var_control_data(SOURCE* source) const
{
    ostringstream osstream;
    double p = source->get_p_generation_in_MW();

    double qmax = source->get_q_max_in_MVar();
    double qmin = source->get_q_min_in_MVar();
    if(fabs(qmax+qmin)>DOUBLE_EPSILON)
    {
        if(fabs(qmax-qmin)>DOUBLE_EPSILON)
            osstream<<"0, 0.00";
        else
            osstream<<"3, 0.00";
    }
    else
    {
        double pf = p/steps_sqrt(p*p+qmax*qmax);
        osstream<<"2, "<<setw(4)<<setiosflags(ios::fixed)<<setprecision(2)<<pf;
    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_line_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<LINE*> lines = psdb.get_all_lines();
    unsigned int n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        LINE* line = lines[i];

        unsigned int ibus = line->get_sending_side_bus();
        unsigned int jbus = line->get_receiving_side_bus();
        string ickt = line->get_identifier();
        if(get_export_zero_impedance_line_logic()==false and (psdb.get_equivalent_bus_of_bus(ibus)!=0 or psdb.get_equivalent_bus_of_bus(jbus)!=0))
        {
            if(psdb.get_equivalent_bus_of_bus(ibus)!=0)
                ibus = psdb.get_equivalent_bus_of_bus(ibus);
            if(psdb.get_equivalent_bus_of_bus(jbus)!=0)
                jbus = psdb.get_equivalent_bus_of_bus(jbus);
            ickt = ickt + toolkit.get_next_alphabeta();
        }
        if(get_export_out_of_service_bus_logic()==false and (psdb.get_bus_type(ibus)==OUT_OF_SERVICE or psdb.get_bus_type(jbus)==OUT_OF_SERVICE))
            continue;

        if(ibus==jbus)
            continue;

        unsigned int meterend = 1;
        if(line->get_meter_end_bus()==line->get_receiving_side_bus())
            meterend = 2;

        osstream<<right
                <<setw(8)<<ibus<<", "
                <<setw(8)<<jbus<<", "
                <<"\""<<left
                <<setw(2)<<ickt<<"\""<<", "
                <<right
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_z_in_pu().real()<<","
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_z_in_pu().imag()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_y_in_pu().imag()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_A_MVA()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_B_MVA()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_C_MVA()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_sending_side_in_pu().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_sending_side_in_pu().imag()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_receiving_side_in_pu().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_receiving_side_in_pu().imag()<<", "
                <<(line->get_sending_side_breaker_status() and line->get_receiving_side_breaker_status())<<", "
                <<meterend<<", "
                <<setw(6)<<setprecision(2)<<fixed<<line->get_length()<<", "
                <<setw(4)<<line->get_owner_of_index(0)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(0)<<", "
                <<setw(4)<<line->get_owner_of_index(1)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(1)<<", "
                <<setw(4)<<line->get_owner_of_index(2)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(2)<<", "
                <<setw(4)<<line->get_owner_of_index(3)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(3)<<endl;

    }
    return osstream.str();
}
string STEPS_IMEXPORTER::export_transformer_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
    unsigned int n = transformers.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        TRANSFORMER* trans = transformers[i];


        unsigned int ibus = trans->get_winding_bus(PRIMARY_SIDE);
        unsigned int jbus = trans->get_winding_bus(SECONDARY_SIDE);
        unsigned int kbus = trans->get_winding_bus(TERTIARY_SIDE);
        string ickt = trans->get_identifier();
        if(get_export_zero_impedance_line_logic()==false and (psdb.get_equivalent_bus_of_bus(ibus)!=0 or psdb.get_equivalent_bus_of_bus(jbus)!=0 or psdb.get_equivalent_bus_of_bus(kbus)!=0))
        {
            if(psdb.get_equivalent_bus_of_bus(ibus)!=0)
                ibus = psdb.get_equivalent_bus_of_bus(ibus);
            if(psdb.get_equivalent_bus_of_bus(jbus)!=0)
                jbus = psdb.get_equivalent_bus_of_bus(jbus);
            if(psdb.get_equivalent_bus_of_bus(kbus)!=0)
                kbus = psdb.get_equivalent_bus_of_bus(kbus);
            ickt = ickt + toolkit.get_next_alphabeta();
        }
        if(trans->is_two_winding_transformer())
        {
            if(get_export_out_of_service_bus_logic()==false and (psdb.get_bus_type(ibus)==OUT_OF_SERVICE or psdb.get_bus_type(jbus)==OUT_OF_SERVICE))
                continue;
        }
        else
        {
            if(get_export_out_of_service_bus_logic()==false and (psdb.get_bus_type(ibus)==OUT_OF_SERVICE or psdb.get_bus_type(jbus)==OUT_OF_SERVICE or psdb.get_bus_type(kbus)==OUT_OF_SERVICE))
                continue;
        }

        unsigned int nonmeterend = 2;
        if(trans->is_two_winding_transformer())
        {
            if(trans->get_non_metered_end_bus() == trans->get_winding_bus(PRIMARY_SIDE))
                nonmeterend = 1;
            else
                nonmeterend = 2;
        }
        else
        {
            if(trans->get_non_metered_end_bus() == trans->get_winding_bus(PRIMARY_SIDE))
                nonmeterend = 1;
            else
            {
                if(trans->get_non_metered_end_bus() == trans->get_winding_bus(SECONDARY_SIDE))
                    nonmeterend = 2;
                else
                {
                    if(trans->get_non_metered_end_bus() == trans->get_winding_bus(TERTIARY_SIDE))
                        nonmeterend = 3;
                    else
                        nonmeterend = 2;
                }
            }
        }

        unsigned int status = 1;
        if(trans->is_two_winding_transformer())
        {
            if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false or trans->get_winding_breaker_status(SECONDARY_SIDE)==false)
                status = 0;
        }
        else
        {
            if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false and trans->get_winding_breaker_status(SECONDARY_SIDE)==false
               and trans->get_winding_breaker_status(TERTIARY_SIDE)==false)
                status = 0;
            else
            {
                if(trans->get_winding_breaker_status(SECONDARY_SIDE)==false)
                    status = 2;
                if(trans->get_winding_breaker_status(TERTIARY_SIDE)==false)
                    status = 2;
                if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false)
                    status = 2;
            }
        }
        osstream<<right
              <<setw(8)<<ibus<<", "
              <<setw(8)<<jbus<<", "
              <<setw(8)<<kbus<<", "
              <<"\""<<left
              <<setw(2)<<ickt<<"\", "
              <<right
              <<TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE<<", "
              <<IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE<<", "
              <<ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE<<", "
              <<setprecision(6)<<fixed<<trans->get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu().real()<<", "
              <<setprecision(6)<<fixed<<trans->get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu().imag()<<", "
              <<nonmeterend<<", "
              <<"\""<<left
              <<setw(16)<<trans->get_name()<<"\", "
              <<right
              <<status<<", "
              <<trans->get_owner_of_index(0)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(0)<<", "
              <<trans->get_owner_of_index(1)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(1)<<", "
              <<trans->get_owner_of_index(2)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(2)<<", "
              <<trans->get_owner_of_index(3)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(3)<<", "
              <<"\"\""<<endl;
        if(trans->is_two_winding_transformer())
        {
            osstream<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real()<<", "
              <<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag()<<", "
              <<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<endl;

            TRANSFORMER_WINDING_SIDE winding = PRIMARY_SIDE;

            unsigned int control_mode;
            switch(trans->get_winding_control_mode(winding))
            {
                case TRANSFORMER_TAP_NO_CONTROL:
                    control_mode = 0;
                    break;
                case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                    control_mode = 1;
                    break;
                case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                    control_mode = 2;
                    break;
                case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                    control_mode = 3;
                    break;
                case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                    control_mode = 4;
                    break;
                case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                    control_mode = 5;
                    break;
                default:
                    control_mode = 0;
                    break;
            }

            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
              <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<", "
              <<setw(6)<<setprecision(2)<<fixed<<trans->get_winding_angle_shift_in_deg(winding)<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_A_MVA()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_B_MVA()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_C_MVA()<<", "
              <<control_mode<<", ";
            switch(trans->get_winding_control_mode(winding))
            {
                case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_angle_shift_in_deg(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_angle_shift_in_deg(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_active_power_into_winding_in_MW(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_active_power_into_winding_in_MW(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_NO_CONTROL:
                case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                default:
                    osstream<<0<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                    break;
            }

            osstream<<setw(3)<<setprecision(0)<<trans->get_winding_number_of_taps(winding)<<", "
              <<setw(3)<<setprecision(0)<<0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<endl;

            winding = SECONDARY_SIDE;
            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
              <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<endl;
        }
        else
        {
            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)<<", "
              <<"1.0, 0.0"<<endl;

            for(unsigned int j=1; j!=4; ++j)
            {
                TRANSFORMER_WINDING_SIDE winding=PRIMARY_SIDE;
                if(j==1) winding = PRIMARY_SIDE;
                if(j==2) winding = SECONDARY_SIDE;
                if(j==3) winding = TERTIARY_SIDE;

                unsigned int control_mode;
                switch(trans->get_winding_control_mode(winding))
                {
                    case TRANSFORMER_TAP_NO_CONTROL:
                        control_mode = 0;
                        break;
                    case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                        control_mode = 1;
                        break;
                    case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                        control_mode = 2;
                        break;
                    case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                        control_mode = 3;
                        break;
                    case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                        control_mode = 4;
                        break;
                    case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                        control_mode = 5;
                        break;
                    default:
                        control_mode = 0;
                        break;
                }

                osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                  <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<", "
                  <<setw(6)<<setprecision(2)<<fixed<<trans->get_winding_angle_shift_in_deg(winding)<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_A_MVA()<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_B_MVA()<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_C_MVA()<<", "
                  <<control_mode<<", ";
                switch(trans->get_winding_control_mode(winding))
                {
                    case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                    case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_angle_shift_in_deg(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_angle_shift_in_deg(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_active_power_into_winding_in_MW(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_active_power_into_winding_in_MW(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_NO_CONTROL:
                    case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                    default:
                        osstream<<0<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                        break;
                }

                osstream<<setw(3)<<setprecision(0)<<trans->get_winding_number_of_taps(winding)<<", "
                  <<setw(3)<<setprecision(0)<<0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<endl;
            }
        }
    }
    return osstream.str();
}
string STEPS_IMEXPORTER::export_area_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<AREA*> areas = psdb.get_all_areas();
    unsigned int n = areas.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        AREA* area = areas[i];

        unsigned int bus = area->get_area_swing_bus();
        if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus)!=0)
        {
            bus = psdb.get_equivalent_bus_of_bus(bus);
        }

        osstream<<setw(8)<<area->get_area_number()<<", "
          <<bus<<", "
          <<setprecision(3)<<area->get_expected_power_leaving_area_in_MW()<<", "
          <<setprecision(3)<<area->get_area_power_mismatch_tolerance_in_MW()<<", "
          <<"\""<<area->get_area_name()<<"\""<<endl;
    }

    return osstream.str();
}
string STEPS_IMEXPORTER::export_hvdc_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    unsigned int n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        HVDC* hvdc = hvdcs[i];

        unsigned int rbus = hvdc->get_converter_bus(RECTIFIER);
        unsigned int ibus = hvdc->get_converter_bus(INVERTER);
        string ickt = hvdc->get_identifier();
        if(get_export_zero_impedance_line_logic()==false and (psdb.get_equivalent_bus_of_bus(rbus)!=0 or psdb.get_equivalent_bus_of_bus(ibus)!=0))
        {
            if(psdb.get_equivalent_bus_of_bus(rbus)!=0)
                rbus = psdb.get_equivalent_bus_of_bus(rbus);
            if(psdb.get_equivalent_bus_of_bus(ibus)!=0)
                ibus = psdb.get_equivalent_bus_of_bus(ibus);
            ickt = ickt + toolkit.get_next_alphabeta();
        }
        if(get_export_out_of_service_bus_logic()==false and (psdb.get_bus_type(rbus)==OUT_OF_SERVICE or psdb.get_bus_type(ibus)==OUT_OF_SERVICE))
            continue;

        osstream<<"\""<<left
               <<setw(16)<<hvdc->get_name()<<"\""<<", ";
        osstream<<right;

        bool status = hvdc->get_status();
        HVDC_OPERATION_MODE mode = hvdc->get_converter_operation_mode(RECTIFIER);
        if(status == false)
            osstream<<0<<", ";
        else
        {
            if(mode==RECTIFIER_CONSTANT_CURRENT)
                osstream<<2<<", ";
            else
                osstream<<1<<", ";
        }
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_line_resistance_in_ohm()<<", ";
        if(mode==RECTIFIER_CONSTANT_CURRENT)
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_current_in_kA()*100.0<<", ";
        else
        {
            CONVERTER_SIDE side = hvdc->get_side_to_hold_dc_power();
            if(side==RECTIFIER)
                osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_power_in_MW()<<", ";
            else
                osstream<<setw(6)<<setprecision(2)<<fixed<<-hvdc->get_nominal_dc_power_in_MW()<<", ";
        }

        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_voltage_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_current_power_margin()<<", ";

        osstream<<"\""<<(hvdc->get_meter_end()==RECTIFIER?"R":"I")<<"\", ";
        osstream<<"0.0, 20, 1.0"<<endl;

        for(unsigned int j=0; j!=2; ++j)
        {
            CONVERTER_SIDE converter=RECTIFIER;
            unsigned int bus = rbus;
            if(j==0) converter = RECTIFIER;
            if(j==1){converter = INVERTER; bus = ibus;}


            osstream<<setw(8)<<bus<<", ";
            osstream<<hvdc->get_converter_number_of_bridge(converter)<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_max_alpha_or_gamma_in_deg(converter)<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_min_alpha_or_gamma_in_deg(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_impedance_in_ohm(converter).real()<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_impedance_in_ohm(converter).imag()<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter)<<", ";
            double turn_ratio = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(converter)/
                       hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter);
            osstream<<setw(6)<<setprecision(4)<<fixed<<turn_ratio<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_tap_in_pu(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_max_tap_in_pu(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_min_tap_in_pu(converter)<<", ";
            double tap_step = (hvdc->get_converter_transformer_max_tap_in_pu(converter)-
                               hvdc->get_converter_transformer_min_tap_in_pu(converter))/
                               (hvdc->get_converter_transformer_number_of_taps(converter)-1);
            osstream<<setw(6)<<setprecision(5)<<fixed<<tap_step<<", ";
            osstream<<"0, 0, 0, \"1\", 0.0"<<endl;
        }
    }

    return osstream.str();
}

string STEPS_IMEXPORTER::export_lcc_hvdc_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return osstream.str();
}

string STEPS_IMEXPORTER::export_vsc_hvdc_data() const
{
    ostringstream osstream;
    STEPS& toolkit=get_toolkit();
    POWER_SYSTEM_DATABASE& psdb=toolkit.get_power_system_database();

    vector<VSC_HVDC*> vsc_hvdcs=psdb.get_all_vsc_hvdcs();
    unsigned int n=vsc_hvdcs.size();
    for(unsigned int i=0;i!=n;++i)
    {
        VSC_HVDC* vsc_hvdc=vsc_hvdcs[i];
        osstream<<"\""<<left
                <<setw(8)<<vsc_hvdc->get_name()<<"\""<<", "
                <<right
                <<setw(8)<<vsc_hvdc->get_converter_count()<<", "
                <<setw(8)<<vsc_hvdc->get_dc_bus_count()<<", "
                <<setw(8)<<vsc_hvdc->get_dc_line_count()<<", ";
        bool status=vsc_hvdc->get_status();
        if (status==false)
            osstream<<0<<", ";
        else
            osstream<<1<<", ";
        osstream<<setw(8)<<vsc_hvdc->get_ac_converter_bus_with_dc_voltage_control()<<","
                <<setw(8)<<vsc_hvdc->get_dc_network_base_voltage_in_kV()<<endl;
        unsigned int ncon=vsc_hvdc->get_converter_count();
        unsigned int nbus=vsc_hvdc->get_dc_bus_count();
        unsigned int ndc_line=vsc_hvdc->get_dc_line_count();
        for(unsigned int i=0;i!=ncon;++i)
        {
            osstream<<setw(4)<<vsc_hvdc->get_converter_ac_bus(i)<<", "
                    <<vsc_hvdc->get_converter_dc_operation_mode(i)<<", "
                    <<vsc_hvdc->get_converter_ac_operation_mode(i)<<", ";
            double dcset;
            if(vsc_hvdc->get_converter_dc_operation_mode(i)==1)
                dcset=vsc_hvdc->get_converter_nominal_dc_voltage_command_in_kV(i);
            else
                dcset=vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(i);
            double acset;
            if(vsc_hvdc->get_converter_ac_operation_mode(i)==1)
                acset=vsc_hvdc->get_converter_nominal_ac_voltage_command_in_pu(i);
            else
                acset=vsc_hvdc->get_converter_nominal_ac_reactive_power_command_in_Mvar(i);
            osstream<<setw(8)<<setprecision(2)<<fixed<<dcset<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<acset<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_loss_factor_A_in_kW(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_loss_factor_B_in_kW_per_amp(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_minimum_loss_in_kW(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_rated_capacity_in_MVA(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_current_rating_in_amp(i)<<", "
                    <<setw(8)<<setprecision(2)<<fixed<<vsc_hvdc->get_converter_power_weighting_factor(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_Qmax_in_MVar(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_converter_Qmin_in_MVar(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_converter_remote_bus_to_regulate(i)<<", "
                    <<setw(8)<<setprecision(2)<<fixed<<vsc_hvdc->get_converter_remote_regulation_percent(i)<<endl;
        }
        for(unsigned int i=0;i!=nbus;++i)
        {
            osstream<<setw(8)<<vsc_hvdc->get_dc_bus_number(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_ac_bus_number(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_area(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_zone(i)<<", "
                    <<"\""<<left
                    <<setw(8)<<vsc_hvdc->get_dc_bus_name(i)<<"\""<<", "
                    <<right
                    <<setw(8)<<setprecision(4)<<vsc_hvdc->get_dc_bus_ground_resistance_in_ohm(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_owner_number(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_generation_power_in_MW(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_bus_load_power_in_MW(i)<<endl;
        }
        for(unsigned int i=0;i!=ndc_line;++i)
        {
            osstream<<setw(8)<<vsc_hvdc->get_dc_line_sending_side_bus(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_line_receiving_side_bus(i)<<", "
                    <<"\""
                    <<left
                    <<setw(8)<<vsc_hvdc->get_dc_line_identifier(i)
                    <<"\""<<", "
                    <<right
                    <<setw(8)<<vsc_hvdc->get_dc_line_meter_end_bus(i)<<", "
                    <<setw(8)<<vsc_hvdc->get_dc_line_meter_end_bus(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_dc_line_resistance_in_ohm(i)<<", "
                    <<setw(8)<<setprecision(4)<<fixed<<vsc_hvdc->get_dc_line_inductance_in_mH(i)<<endl;
        }

    }
    return osstream.str();
}

string STEPS_IMEXPORTER::export_transformer_impedance_correction_table_data() const
{
    return "";
}

string STEPS_IMEXPORTER::export_multi_terminal_hvdc_data() const
{
    return "";
}

string STEPS_IMEXPORTER::export_multi_section_line_data() const
{
    return "";
}

string STEPS_IMEXPORTER::export_zone_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<ZONE*> zones = psdb.get_all_zones();
    unsigned int n = zones.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        ZONE* zone = zones[i];

        osstream<<setw(8)<<zone->get_zone_number()<<", "
          <<"\""<<zone->get_zone_name()<<"\""<<endl;
    }

    return osstream.str();
}

string STEPS_IMEXPORTER::export_interarea_transfer_data() const
{
    return "";
}

string STEPS_IMEXPORTER::export_owner_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<OWNER*> owners = psdb.get_all_owners();
    unsigned int n = owners.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        OWNER* owner = owners[i];

        osstream<<setw(8)<<owner->get_owner_number()<<", "
          <<"\""<<owner->get_owner_name()<<"\""<<endl;
    }

    return osstream.str();
}
string STEPS_IMEXPORTER::export_facts_data() const
{
    return "";
}

string STEPS_IMEXPORTER::export_switched_shunt_data() const
{
    return "";
}

unsigned int STEPS_IMEXPORTER::get_starting_index_of_device_powerflow_result(const vector<string>& data, const string& device_type)
{
    unsigned int n_data = data.size();
    unsigned int starting_index = INDEX_NOT_EXIST;
    for(unsigned int i=0; i<n_data; ++i)
    {
        if(data[i]==device_type)
        {
            starting_index = i;
            break;
        }
    }
    return starting_index;
}

unsigned int STEPS_IMEXPORTER::get_data_column_index_in_powerflow_result(const vector<string>& data, unsigned int starting_index, const string& data_name)
{
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");
    unsigned int n_header = header_contents.size();
    unsigned int index=INDEX_NOT_EXIST;
    for(unsigned int i=0; i<n_header; ++i)
    {
        if(header_contents[i]==data_name)
        {
            index = i;
            break;
        }
    }
    return index;
}


vector<string> STEPS_IMEXPORTER::load_powerflow_result_into_ram(string file)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    vector<string> result_in_ram;

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"STEPS powerflow result file '"<<file<<"' is not accessible. Loading STEPS powerflow result is failed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return result_in_ram;
    }

    char buffer[10240];
    string sbuffer;

    while(true)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            fclose(fid);
            break;
        }
        sbuffer = buffer;
        sbuffer = trim_string(sbuffer);
        result_in_ram.push_back(sbuffer);
    }
    return result_in_ram;
}

void STEPS_IMEXPORTER::load_bus_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% Bus");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No bus powerflow result is found in the powerflow result file. No bus voltage and angle is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int bus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "BUS");
    unsigned int voltage_index = get_data_column_index_in_powerflow_result(data, starting_index, "VOLTAGE/PU");
    unsigned int angle_index = get_data_column_index_in_powerflow_result(data, starting_index, "ANGLE/DEG");
    if(bus_number_index==INDEX_NOT_EXIST or voltage_index==INDEX_NOT_EXIST or angle_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of bus contents not found. "<<bus_number_index<<", "<<voltage_index<<", "<<angle_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    unsigned int n_data = data.size();
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int bus_number = str2int(contents[bus_number_index]);
        double voltage = str2double(contents[voltage_index]);
        double angle = str2double(contents[angle_index]);
        BUS* bus = psdb.get_bus(bus_number);
        if(bus!=NULL)
        {
            bus->set_positive_sequence_voltage_in_pu(voltage);
            bus->set_positive_sequence_angle_in_deg(angle);
        }
    }
}

void STEPS_IMEXPORTER::load_generator_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% Generator");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No generator powerflow result is found in the powerflow result file. No generator P and Q is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int bus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "BUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int P_index = get_data_column_index_in_powerflow_result(data, starting_index, "P/MW");
    unsigned int Q_index = get_data_column_index_in_powerflow_result(data, starting_index, "Q/MVAR");
    if(bus_number_index==INDEX_NOT_EXIST or identifier_index==INDEX_NOT_EXIST or P_index==INDEX_NOT_EXIST or Q_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of generator contents not found. "<<bus_number_index<<", "<<identifier_index<<", "<<P_index<<", "<<Q_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int bus_number = str2int(contents[bus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double P = str2double(contents[P_index]);
        double Q = str2double(contents[Q_index]);
        DEVICE_ID did = get_generator_device_id(bus_number,identifier);
        GENERATOR* gen = psdb.get_generator(did);
        if(gen!=NULL)
        {
            gen->set_p_generation_in_MW(P);
            gen->set_q_generation_in_MVar(Q);
        }
    }
}

void STEPS_IMEXPORTER::load_wt_generator_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% WT generator");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No WT generator powerflow result is found in the powerflow result file. No WT generator P and Q is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int bus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "BUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int P_index = get_data_column_index_in_powerflow_result(data, starting_index, "P/MW");
    unsigned int Q_index = get_data_column_index_in_powerflow_result(data, starting_index, "Q/MVAR");
    if(bus_number_index==INDEX_NOT_EXIST or identifier_index==INDEX_NOT_EXIST or P_index==INDEX_NOT_EXIST or Q_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of WT generator contents not found. "<<bus_number_index<<", "<<identifier_index<<", "<<P_index<<", "<<Q_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int bus_number = str2int(contents[bus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double P = str2double(contents[P_index]);
        double Q = str2double(contents[Q_index]);
        DEVICE_ID did = get_wt_generator_device_id(bus_number,identifier);
        WT_GENERATOR* gen = psdb.get_wt_generator(did);
        if(gen!=NULL)
        {
            gen->set_p_generation_in_MW(P);
            gen->set_q_generation_in_MVar(Q);
        }
    }
}

void STEPS_IMEXPORTER::load_pv_unit_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% PV unit");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No PV unit powerflow result is found in the powerflow result file. No PV unit P and Q is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int bus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "BUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int P_index = get_data_column_index_in_powerflow_result(data, starting_index, "P/MW");
    unsigned int Q_index = get_data_column_index_in_powerflow_result(data, starting_index, "Q/MVAR");
    if(bus_number_index==INDEX_NOT_EXIST or identifier_index==INDEX_NOT_EXIST or P_index==INDEX_NOT_EXIST or Q_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of PV unit contents not found. "<<bus_number_index<<", "<<identifier_index<<", "<<P_index<<", "<<Q_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int bus_number = str2int(contents[bus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double P = str2double(contents[P_index]);
        double Q = str2double(contents[Q_index]);
        DEVICE_ID did = get_pv_unit_device_id(bus_number,identifier);
        PV_UNIT* pv = psdb.get_pv_unit(did);
        if(pv!=NULL)
        {
            pv->set_p_generation_in_MW(P);
            pv->set_q_generation_in_MVar(Q);
        }
    }
}

void STEPS_IMEXPORTER::load_energy_storage_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% Energy storage");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No energy storage powerflow result is found in the powerflow result file. No energy storage P and Q is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int bus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "BUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int P_index = get_data_column_index_in_powerflow_result(data, starting_index, "P/MW");
    unsigned int Q_index = get_data_column_index_in_powerflow_result(data, starting_index, "Q/MVAR");
    if(bus_number_index==INDEX_NOT_EXIST or identifier_index==INDEX_NOT_EXIST or P_index==INDEX_NOT_EXIST or Q_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of energy storage contents not found. "<<bus_number_index<<", "<<identifier_index<<", "<<P_index<<", "<<Q_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int bus_number = str2int(contents[bus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double P = str2double(contents[P_index]);
        double Q = str2double(contents[Q_index]);
        DEVICE_ID did = get_energy_storage_device_id(bus_number,identifier);
        ENERGY_STORAGE* es = psdb.get_energy_storage(did);
        if(es!=NULL)
        {
            es->set_p_generation_in_MW(P);
            es->set_q_generation_in_MVar(Q);
        }
    }
}

void STEPS_IMEXPORTER::load_transformer_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% Transformer");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No transformer powerflow result is found in the powerflow result file. No transformer off-nominal tap is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int ibus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "IBUS");
    unsigned int jbus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "JBUS");
    unsigned int kbus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "KBUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int itap_index = get_data_column_index_in_powerflow_result(data, starting_index, "KI/PU");
    unsigned int jtap_index = get_data_column_index_in_powerflow_result(data, starting_index, "KJ/PU");
    unsigned int ktap_index = get_data_column_index_in_powerflow_result(data, starting_index, "KK/PU");
    if(ibus_number_index==INDEX_NOT_EXIST or jbus_number_index==INDEX_NOT_EXIST or kbus_number_index==INDEX_NOT_EXIST or
       identifier_index==INDEX_NOT_EXIST or
       itap_index==INDEX_NOT_EXIST or jtap_index==INDEX_NOT_EXIST or ktap_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of transformer contents not found. "<<ibus_number_index<<", "<<jbus_number_index<<", "<<kbus_number_index<<", "
                <<identifier_index<<", "<<itap_index<<", "<<jtap_index<<", "<<ktap_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int ibus_number = str2int(contents[ibus_number_index]);
        unsigned int jbus_number = str2int(contents[jbus_number_index]);
        unsigned int kbus_number = str2int(contents[kbus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double itap = str2double(contents[itap_index]);
        double jtap = str2double(contents[jtap_index]);
        double ktap = str2double(contents[ktap_index]);
        DEVICE_ID did = get_transformer_device_id(ibus_number,jbus_number,kbus_number,identifier);
        TRANSFORMER* trans = psdb.get_transformer(did);
        if(trans!=NULL)
        {
            trans->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, itap);
            trans->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, jtap);
            if(trans->is_three_winding_transformer())
                trans->set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, ktap);
        }
    }
}

void STEPS_IMEXPORTER::load_hvdc_powerflow_result(const vector<string>& data)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_data = data.size();
    unsigned int starting_index = get_starting_index_of_device_powerflow_result(data, "% Hvdc");
    if(starting_index == INDEX_NOT_EXIST)
    {
        osstream<<"Warning! No HVDC powerflow result is found in the powerflow result file. No HVDC off-nominal tap and firing angle is updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    string header = data[starting_index+1];
    vector<string> header_contents = split_string(header, ",");

    unsigned int rbus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "IBUS");
    unsigned int ibus_number_index = get_data_column_index_in_powerflow_result(data, starting_index, "JBUS");
    unsigned int identifier_index = get_data_column_index_in_powerflow_result(data, starting_index, "ID");
    unsigned int rtap_index = get_data_column_index_in_powerflow_result(data, starting_index, "KR/PU");
    unsigned int itap_index = get_data_column_index_in_powerflow_result(data, starting_index, "KI/PU");
    unsigned int alpha_index = get_data_column_index_in_powerflow_result(data, starting_index, "ALPHA/DEG");
    unsigned int gamma_index = get_data_column_index_in_powerflow_result(data, starting_index, "GAMMA/DEG");
    if(rbus_number_index==INDEX_NOT_EXIST or ibus_number_index==INDEX_NOT_EXIST or identifier_index==INDEX_NOT_EXIST or
       rtap_index==INDEX_NOT_EXIST or itap_index==INDEX_NOT_EXIST or
       alpha_index==INDEX_NOT_EXIST or gamma_index==INDEX_NOT_EXIST)
    {
        osstream<<"Index of transformer contents not found. "<<rbus_number_index<<", "<<ibus_number_index<<", "<<identifier_index<<", "
                <<rtap_index<<", "<<itap_index<<", "<<alpha_index<<", "<<gamma_index;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    // go load powerflow result
    for(unsigned int i=starting_index+2; i<n_data; ++i)
    {
        string line = data[i];
        if(line.size()==0 or line.at(0)=='%')
            break;

        vector<string> contents = split_string(line, ",");
        unsigned int rbus_number = str2int(contents[rbus_number_index]);
        unsigned int ibus_number = str2int(contents[ibus_number_index]);
        string identifier = contents[identifier_index];
        identifier = trim_string(identifier, "\"\'");
        double rtap = str2double(contents[rtap_index]);
        double itap = str2double(contents[itap_index]);
        double alpha = str2double(contents[alpha_index]);
        double gamma = str2double(contents[gamma_index]);
        DEVICE_ID did = get_hvdc_device_id(rbus_number,ibus_number,identifier);
        HVDC* hvdc = psdb.get_hvdc(did);
        if(hvdc!=NULL)
        {
            hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, alpha);
            hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, gamma);
            hvdc->set_converter_transformer_tap_in_pu(RECTIFIER, rtap);
            hvdc->set_converter_transformer_tap_in_pu(INVERTER, itap);
        }
    }
}

void STEPS_IMEXPORTER::export_sequence_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}
