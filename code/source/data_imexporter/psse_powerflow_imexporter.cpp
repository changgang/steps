#include "header/data_imexporter/psse_imexporter.h"
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

PSSE_IMEXPORTER::PSSE_IMEXPORTER()
{
    raw_data_in_ram.clear();
    dyr_data_in_ram.clear();
}

PSSE_IMEXPORTER::~PSSE_IMEXPORTER()
{
    raw_data_in_ram.clear();
    dyr_data_in_ram.clear();
}

void PSSE_IMEXPORTER::load_powerflow_data(string file)
{
    ostringstream osstream;
    osstream<<"Loading powerflow data from PSS/E file: "<<file;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    load_powerflow_data_into_ram(file);
    if(raw_data_in_ram.size()==0)
    {
        osstream<<"No data in the given PSS/E file: "<<file<<endl
          <<"Please check if the file exist or not.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    STEPS_IMEXPORTER steps_importer;
    steps_importer.set_toolkit(toolkit);

    vector<vector<vector<string> > > data = convert_psse_raw_data2steps_vector();
    steps_importer.load_powerflow_data_from_steps_vector(data);

    osstream<<"Done loading powerflow data.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSSE_IMEXPORTER::load_sequence_data(string sq_source)
{
    sq_source = string2upper(sq_source);
}


void PSSE_IMEXPORTER::load_powerflow_data_into_ram(string file)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    raw_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"PSS/E raw file '"<<file<<"' is not accessible. Loading PSS/E raw data is failed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    char buffer[10240];
    string sbuffer;

    vector<string> data_of_one_type;
    data_of_one_type.clear();

    if(fgets(buffer, 1024, fid)==NULL)
    {
        fclose(fid);
        return;
    }
    sbuffer = trim_psse_comment(buffer);
    sbuffer = trim_string(sbuffer);
    data_of_one_type.push_back(sbuffer);

    raw_data_in_ram.push_back(data_of_one_type);

    data_of_one_type.clear();

    for(size_t i=0; i!=2; ++i)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            fclose(fid);
            return;
        }
        sbuffer = trim_psse_comment(buffer);
        sbuffer = trim_string(sbuffer);
        data_of_one_type.push_back(sbuffer);
    }
    raw_data_in_ram.push_back(data_of_one_type);

    data_of_one_type.clear();

    while(true)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            if(data_of_one_type.size()!=0)
            {
                raw_data_in_ram.push_back(data_of_one_type);
                data_of_one_type.clear();
            }
            break;
        }
        sbuffer = trim_psse_comment(buffer);
        sbuffer = trim_string(sbuffer);
        if(sbuffer.size()!=0)
        {
            if(sbuffer != "0")
            {
                data_of_one_type.push_back(sbuffer);
            }
            else
            {
                raw_data_in_ram.push_back(data_of_one_type);
                data_of_one_type.clear();
            }
        }
        else
            break;
    }
    fclose(fid);
}

string PSSE_IMEXPORTER::trim_psse_comment(string str)
{
    if(str.size()==0)
        return str;

    size_t found = str.find_first_of('/');
    if(found!=string::npos) str.erase(found, str.size());
    return str;
}




vector<vector<vector<string> > >  PSSE_IMEXPORTER::convert_psse_raw_data2steps_vector() const
{
    vector<vector<vector<string> > > data;
    data.push_back(convert_case_data2steps_vector());
    data.push_back(convert_bus_data2steps_vector());
    data.push_back(convert_load_data2steps_vector());
    data.push_back(convert_fixed_shunt_data2steps_vector());
    data.push_back(convert_source_data2steps_vector());
    data.push_back(convert_line_data2steps_vector());
    data.push_back(convert_transformer_data2steps_vector());
    data.push_back(convert_hvdc_data2steps_vector());
    data.push_back(convert_area_data2steps_vector());
    data.push_back(convert_vsc_hvdc_data2steps_vector());
    data.push_back(convert_transformer_inpedance_correction_table_data2steps_vector());
    data.push_back(convert_multi_terminal_hvdc_data2steps_vector());
    data.push_back(convert_multi_section_line_data2steps_vector());
    data.push_back(convert_zone_data2steps_vector());
    data.push_back(convert_interarea_transfer_data2steps_vector());
    data.push_back(convert_owner_data2steps_vector());
    data.push_back(convert_facts_data2steps_vector());
    data.push_back(convert_switched_shunt_data2steps_vector());

    return data;
}

vector<vector<string> > PSSE_IMEXPORTER::convert_i_th_type_data2steps_vector(size_t i) const
{
    vector<vector<string> > data;
    size_t n = raw_data_in_ram.size();
    if(i<n)
    {
        vector<string> DATA = raw_data_in_ram[i];
        size_t m = DATA.size();
        for(size_t j=0; j<m; ++j)
            data.push_back(split_string(DATA[j],","));
    }
    return data;
}

vector<vector<string> > PSSE_IMEXPORTER::convert_case_data2steps_vector() const
{
    vector<vector<string> > data;
    data.push_back(split_string(raw_data_in_ram[0][0],","));//case line
    data.push_back(raw_data_in_ram[1]);// two lines
    return data;
}
vector<vector<string> > PSSE_IMEXPORTER::convert_bus_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(2);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_load_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(3);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_fixed_shunt_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(4);
}

vector<vector<string> > PSSE_IMEXPORTER::convert_source_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(5);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_line_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(6);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_transformer_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(7);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_hvdc_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(8);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_area_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(9);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_vsc_hvdc_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(10);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_transformer_inpedance_correction_table_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(11);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_multi_terminal_hvdc_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(12);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_multi_section_line_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(13);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_zone_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(14);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_interarea_transfer_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(15);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_owner_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(16);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_facts_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(17);
}
vector<vector<string> > PSSE_IMEXPORTER::convert_switched_shunt_data2steps_vector() const
{
    return convert_i_th_type_data2steps_vector(18);
}

void PSSE_IMEXPORTER::export_powerflow_data(string file, bool export_zero_impedance_line, bool export_out_of_service_bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    set_export_out_of_service_bus_logic(export_out_of_service_bus);

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. PSS/E raw file "<<file<<" cannot be opened for exporting powerflow data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    set_export_zero_impedance_line_logic(export_zero_impedance_line);

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

string PSSE_IMEXPORTER::export_case_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    char buffer[1000];
    vector<size_t> buses = psdb.get_all_buses_number();
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

string PSSE_IMEXPORTER::export_bus_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<BUS*> buses = psdb.get_all_buses();
    size_t n = buses.size();
    char buffer[1000];
    for(size_t i=0; i!=n; ++i)
    {
        BUS* bus = buses[i];
        if(get_export_zero_impedance_line_logic()==false and psdb.get_equivalent_bus_of_bus(bus->get_bus_number())!=0)
            continue;
        if(get_export_out_of_service_bus_logic()==false)
            continue;

        BUS_TYPE bus_type = bus->get_bus_type();
        int type = 4;
        if(bus_type == PQ_TYPE) type = 1;
        if(bus_type == PV_TYPE) type = 2;
        if(bus_type == PV_TO_PQ_TYPE_1 or bus_type == PV_TO_PQ_TYPE_2 or
           bus_type == PV_TO_PQ_TYPE_3 or bus_type == PV_TO_PQ_TYPE_4) type = -2;
        if(bus_type == SLACK_TYPE) type = 3;
        if(bus_type == OUT_OF_SERVICE) type = 4;
        //if(type == 4) continue;

        snprintf(buffer, 1000, "%8lu, \"%-16s\", %8.2f, %2d, %4lu, %4lu, %4lu, %10.6f, %10.6f, %6.4f, %6.4f, %6.4f, %6.4f, %4.1f",
                 bus->get_bus_number(), (bus->get_bus_name()).c_str(), bus->get_base_voltage_in_kV(), type,
                 bus->get_area_number(), bus->get_zone_number(), bus->get_owner_number(),
                 bus->get_voltage_in_pu(), bus->get_angle_in_deg(),
                 bus->get_normal_voltage_upper_limit_in_pu(), bus->get_normal_voltage_lower_limit_in_pu(),
                 bus->get_emergency_voltage_upper_limit_in_pu(), bus->get_emergency_voltage_lower_limit_in_pu(),
                 bus->get_base_frequency_in_Hz());
        osstream<<buffer<<endl;
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_load_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<LOAD*> loads = psdb.get_all_loads();
    size_t n = loads.size();
    char buffer[1000];
    for(size_t i=0; i!=n; ++i)
    {
        LOAD* load = loads[i];

        /*osstream<<right
          <<setw(8)
          <<load->get_load_bus()<<", "
          <<"\""<<left
          <<load->get_identifier()<<"\""<<", "
          <<right
          <<load->get_status()<<", "
          <<setw(4)<<load->get_area_number()<<", "
          <<setw(4)<<load->get_zone_number()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_power_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_power_load_in_MVA().imag()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_current_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_current_load_in_MVA().imag()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_impedance_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<-load->get_nominal_constant_impedance_load_in_MVA().imag()<<", "
          <<setw(4)<<load->get_owner_number()<<", "
          <<setw(2)<<fixed<<1<<", "
          <<setw(2)<<load->get_flag_interruptable()<<endl;*/

        size_t bus = load->get_load_bus();
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

string PSSE_IMEXPORTER::export_fixed_shunt_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<FIXED_SHUNT*> fshunts = psdb.get_all_fixed_shunts();
    size_t n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
    {
        FIXED_SHUNT* shunt = fshunts[i];

        size_t bus = shunt->get_shunt_bus();
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
                <<setw(12)<<setprecision(6)<<fixed<<shunt->get_nominal_impedance_shunt_in_MVA().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<-shunt->get_nominal_impedance_shunt_in_MVA().imag()<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_source_data() const
{
    ostringstream osstream;
    osstream<<export_generator_data();
    osstream<<export_wt_generator_data();
    osstream<<export_pv_unit_data();
    osstream<<export_energy_storage_data();

    return osstream.str();
}

string PSSE_IMEXPORTER::export_generator_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<GENERATOR*> generators = psdb.get_all_generators();
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
    {
        GENERATOR* generator = generators[i];
        if(get_export_out_of_service_bus_logic()==false and psdb.get_bus_type(generator->get_source_bus())==OUT_OF_SERVICE)
            continue;

        osstream<<export_source_common_data(generator);
        osstream<<"0, 0.0, 0"<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_wt_generator_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
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


string PSSE_IMEXPORTER::export_pv_unit_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
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


string PSSE_IMEXPORTER::export_energy_storage_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<ENERGY_STORAGE*> estorages = psdb.get_all_energy_storages();
    size_t n = estorages.size();
    for(size_t i=0; i!=n; ++i)
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

string PSSE_IMEXPORTER::export_source_common_data(SOURCE* source) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t bus = source->get_source_bus();
    size_t bus_to_regulate = source->get_bus_to_regulate();
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

string PSSE_IMEXPORTER::export_source_var_control_data(SOURCE* source) const
{
    ostringstream osstream;
    double p = source->get_p_generation_in_MW();

    double qmax = source->get_q_max_in_MVar();
    double qmin = source->get_q_min_in_MVar();
    if(fabs(qmax+qmin)>FLOAT_EPSILON)
    {
        if(fabs(qmax-qmin)>FLOAT_EPSILON)
            osstream<<"0, 0.0";
        else
            osstream<<"3, 0.0";
    }
    else
    {
        double pf = p/sqrt(p*p+qmax*qmax);
        osstream<<"2, "<<setprecision(6)<<pf;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_line_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<LINE*> lines = psdb.get_all_lines();
    size_t n = lines.size();
    for(size_t i=0; i!=n; ++i)
    {
        LINE* line = lines[i];

        size_t ibus = line->get_sending_side_bus();
        size_t jbus = line->get_receiving_side_bus();
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

        size_t meterend = 1;
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
string PSSE_IMEXPORTER::export_transformer_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
    size_t n = transformers.size();
    for(size_t i=0; i!=n; ++i)
    {
        TRANSFORMER* trans = transformers[i];


        size_t ibus = trans->get_winding_bus(PRIMARY_SIDE);
        size_t jbus = trans->get_winding_bus(SECONDARY_SIDE);
        size_t kbus = trans->get_winding_bus(TERTIARY_SIDE);
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

        size_t nonmeterend = 2;
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

        size_t status = 1;
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
              <<setw(16)<<trans->get_transformer_name()<<"\", "
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

            size_t control_mode;
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

            for(size_t j=1; j!=4; ++j)
            {
                TRANSFORMER_WINDING_SIDE winding=PRIMARY_SIDE;
                if(j==1) winding = PRIMARY_SIDE;
                if(j==2) winding = SECONDARY_SIDE;
                if(j==3) winding = TERTIARY_SIDE;

                size_t control_mode;
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
string PSSE_IMEXPORTER::export_area_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<AREA*> areas = psdb.get_all_areas();
    size_t n = areas.size();
    for(size_t i=0; i!=n; ++i)
    {
        AREA* area = areas[i];

        size_t bus = area->get_area_swing_bus();
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
string PSSE_IMEXPORTER::export_hvdc_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        HVDC* hvdc = hvdcs[i];

        size_t rbus = hvdc->get_converter_bus(RECTIFIER);
        size_t ibus = hvdc->get_converter_bus(INVERTER);
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
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_current_per_pole_in_kA()*100.0<<", ";
        else
        {
            HVDC_CONVERTER_SIDE side = hvdc->get_side_to_hold_dc_power();
            if(side==RECTIFIER)
                osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_power_per_pole_in_MW()<<", ";
            else
                osstream<<setw(6)<<setprecision(2)<<fixed<<-hvdc->get_nominal_dc_power_per_pole_in_MW()<<", ";
        }

        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_voltage_per_pole_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_current_power_margin()<<", ";

        osstream<<"\""<<(hvdc->get_meter_end()==RECTIFIER?"R":"I")<<"\", ";
        osstream<<"0.0, 20, 1.0"<<endl;

        for(size_t j=0; j!=2; ++j)
        {
            HVDC_CONVERTER_SIDE converter=RECTIFIER;
            size_t bus = rbus;
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
                               hvdc->get_converter_transformer_number_of_taps(converter);
            osstream<<setw(6)<<setprecision(5)<<fixed<<tap_step<<", ";
            osstream<<"0, 0, 0, \"1\", 0.0"<<endl;
        }
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_vsc_hvdc_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_transformer_impedance_correction_table_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_multi_terminal_hvdc_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_multi_section_line_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_zone_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<ZONE*> zones = psdb.get_all_zones();
    size_t n = zones.size();
    for(size_t i=0; i!=n; ++i)
    {
        ZONE* zone = zones[i];

        osstream<<setw(8)<<zone->get_zone_number()<<", "
          <<"\""<<zone->get_zone_name()<<"\""<<endl;
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_interarea_transfer_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_owner_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<OWNER*> owners = psdb.get_all_owners();
    size_t n = owners.size();
    for(size_t i=0; i!=n; ++i)
    {
        OWNER* owner = owners[i];

        osstream<<setw(8)<<owner->get_owner_number()<<", "
          <<"\""<<owner->get_owner_name()<<"\""<<endl;
    }

    return osstream.str();
}
string PSSE_IMEXPORTER::export_facts_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_switched_shunt_data() const
{
    return "";
}


void PSSE_IMEXPORTER::export_sequence_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}
