#include "header/data_imexporter/bpa_imexporter.h"
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
#include <fstream>


using namespace std;

BPA_IMEXPORTER::BPA_IMEXPORTER(STEPS& toolkit) : DATA_IMEXPORTER(toolkit)
{
    dat_data_in_ram.clear();
    swi_data_in_ram.clear();

    MC_MF_MG_cards.clear();
}

BPA_IMEXPORTER::~BPA_IMEXPORTER()
{
}


string BPA_IMEXPORTER::format_bpa_data_to_readable_data(string original_data, string format)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    string data = trim_string(original_data);
    if(data.size()==0)
        return data;

    format = string2upper(format);

    if(format.size()<2)
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") is given. No BPA data will be converted.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return "";
    }

    string type = format.substr(0,1);
    if(type=="F")
    {
        if(data.find(".") != string::npos)
            return data;
        else
        {
            char tempc;
            unsigned int total_bits=0, decimal_bits=0;
            sscanf(format.c_str(),"%c%u.%u",&tempc, &total_bits, &decimal_bits);

            data = original_data.substr(0, total_bits-decimal_bits)+"."+ original_data.substr(total_bits-decimal_bits, decimal_bits);
            return data;
        }
    }
    if(type=="A")
        return data;

    if(type=="I")
        return data;

    osstream<<"Warning. Unsupported format ("<<format<<") is given to format BPA data.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    return original_data;
}

string BPA_IMEXPORTER::grow_string_to_at_least_size(string data, unsigned int least_size)
{
    unsigned int n = data.size();
    if(n>=least_size)
        return data;

    string blank(least_size-n, ' ');

    data = data + blank;

    return data;
}

void BPA_IMEXPORTER::load_powerflow_data(string file)
{
    ostringstream osstream;
    osstream<<"Loading powerflow data from BPA file: "<<file;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    load_powerflow_data_into_ram(file);

    if(dat_data_in_ram.size()==0)
    {
        osstream<<"No data in the given BPA file: "<<file<<endl
          <<"Please check if the file exists or not.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        return;
    }

    load_area_data();
    load_zone_data();
    load_owner_data();
    load_bus_data();
    load_generator_data();
    load_load_and_fixed_shunt_data();
    load_line_data();
    load_transformer_data();
    load_hvdc_data();
    /*
    load_case_data();
    */

    osstream<<"Done loading powerflow data.";
    toolkit.show_information_with_leading_time_stamp(osstream);

}

void BPA_IMEXPORTER::load_powerflow_result(string pf_source)
{
    return;
}

void BPA_IMEXPORTER::load_sequence_data(string sq_source)
{
    sq_source = string2upper(sq_source);
}


void BPA_IMEXPORTER::load_powerflow_data_into_ram(string file)
{
    STEPS& toolkit = get_toolkit();
    dat_data_in_ram.clear();

    ifstream dat_file(file);

    if(!dat_file)
    {
        ostringstream osstream;
        osstream<<"BPA dat file '"<<file<<"' is not accessible. Loading BPA dat data is failed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    string buffer;
    while(getline(dat_file,buffer))
    {
        if(not is_comment_line(buffer))
            dat_data_in_ram.push_back(buffer);
    }
    dat_file.close();

    //for(unsigned int i=0; i<dat_data_in_ram.size(); ++i)
    //    cout<<dat_data_in_ram[i]<<endl;
}

bool BPA_IMEXPORTER::is_comment_line(string str)
{
    str = trim_string(str);
    if(str.size()==0)
        return true;

    string card_type;
    card_type = str.substr(0,1);
    card_type = string2upper(card_type);

    if(card_type == ".")
        return true;
    else
    {
        if(card_type=="C")
        {
            card_type = str.substr(0,2);
            card_type = string2upper(card_type);
            if(card_type == "C ")
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

void BPA_IMEXPORTER::format_all_powerflow_cards()
{

}

string BPA_IMEXPORTER::get_powerflow_card_type(string data) const
{
    string unsupported_card_type = "UNSUPPORTED BPA CARD";
    string card_type;

    card_type = data.substr(0, 1);
    card_type = string2upper(card_type);
    if(card_type == "/")
        return card_type;

    if(card_type == "A")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type=="A ")
            card_type = "AC";
        if(card_type == "AC")
        {
            card_type = data.substr(0,3);
            card_type = string2upper(card_type);
            if(card_type == "AC+")
                return card_type;
            else
                return "AC";
        }
        else
        {
            if(card_type == "AO")
            {
                card_type = data.substr(0,3);
                card_type = string2upper(card_type);
                if(card_type == "AO+")
                    return card_type;
                else
                    return "AO";
            }
            else
                return unsupported_card_type;
        }
    }

    if(card_type == "I")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "I ")
            return "I";
        else
            return unsupported_card_type;
    }

    if(card_type == "B")
    {
        // B , B?
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "B ")
            return "B";
        else
        {
            if(card_type == "BT" or card_type == "BC" or card_type == "BV" or
               card_type == "BE" or card_type == "BQ" or card_type == "BG" or
               card_type == "BF" or card_type == "BS" or card_type == "BJ" or
               card_type == "BK" or card_type == "BL" or card_type == "BX" or
               card_type == "BD" or card_type == "BM")
                return card_type;
            else
            {
                if(card_type == "BZ")
                {
                    card_type = data.substr(0,3);
                    card_type = string2upper(card_type);
                    if(card_type == "BZ+")
                        return card_type;
                    else
                        return "BZ";
                }
                else
                    return unsupported_card_type;
            }
        }
    }

    if(card_type == "+")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "+ ")
            return "+";
        else
        {
            if(card_type == "+A" or card_type == "+C" or card_type == "+F" or
               card_type == "+I" or card_type == "+N" or card_type == "+P" or
               card_type == "+S")
                return card_type;
            else
                return unsupported_card_type;
        }
    }

    if(card_type == "X")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "X ")
            return "X";
        else
            return unsupported_card_type;
    }

    if(card_type == "L")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "L ")
            return "L";
        else
        {
            if(card_type == "L+" or card_type == "LD" or card_type == "LM" or
               card_type == "LZ")
                return card_type;
            else
                return unsupported_card_type;
        }
    }

    if(card_type == "E")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "E ")
            return "E";
        else
            return unsupported_card_type;
    }

    if(card_type == "T")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "T ")
            return "T";
        else
        {
            if(card_type == "TP")
                return card_type;
            else
                return unsupported_card_type;
        }
    }

    if(card_type == "R")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "R ")
            return "R";
        else
        {
            if(card_type == "RV" or card_type == "RQ" or card_type == "RP" or
               card_type == "RN" or card_type == "RM" or card_type == "RZ")
                return card_type;
            else
                return unsupported_card_type;
        }
    }

    if(card_type == "P")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "PA" or card_type == "PZ" or card_type == "PO" or
           card_type == "PC" or card_type == "PB")
            return card_type;
        else
            return unsupported_card_type;
    }

    if(card_type == "Z")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "Z ")
            return "Z";
        else
            return unsupported_card_type;
    }

    if(card_type == "D")
    {
        card_type = data.substr(0,2);
        card_type = string2upper(card_type);
        if(card_type == "DZ")
            return card_type;
        else
            return unsupported_card_type;
    }

    return unsupported_card_type;

}

void BPA_IMEXPORTER::load_case_data()
{
    ;
}


void BPA_IMEXPORTER::set_data_version(unsigned int version)
{
    data_version = version;
}

unsigned int BPA_IMEXPORTER::get_data_version() const
{
    return data_version;
}

void BPA_IMEXPORTER::update_bus_number_with_bus_name_and_number_pair_file(string file)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    FILE* fid = fopen(file.c_str(),"rt");
    if(fid==NULL)
    {
        osstream<<"Warning. Bus name and number pair file ("<<file<<") cannot be opened. No bus number will be updated.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    char buffer[1024];
    string sbuffer;
    unordered_map<string, unsigned int> bus_name_number_pair;
    vector<string> data;
    while(true)
    {
        if(fgets(buffer, 512, fid)==NULL)
            break;
        sbuffer = string2csv(buffer);
        data = split_string(sbuffer, ",");
        if(data.size()<2)
            continue;

        string bus_name = data[0];
        unsigned int bus_number = str2int(data[1]);
        bus_name_number_pair.insert(pair<string, unsigned int>(bus_name, bus_number));
    }
}


void BPA_IMEXPORTER::load_area_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="A" or card_type == "AC")
        {
            data = grow_string_to_at_least_size(data, 95);

            string area_name = data.substr(3,10);

            unsigned int area_number = psdb.area_name2area_number(area_name);
            if(area_number == 0)
            {
                AREA area(toolkit);
                area.set_area_name(area_name);

                unsigned int area_count = psdb.get_area_count();
                area.set_area_number(area_count+1);

                string area_schedule_str = data.substr(26, 8);
                area_schedule_str = format_bpa_data_to_readable_data(area_schedule_str, "F8.0");
                double area_schedule = get_double_data(area_schedule_str, "0.0");

                area.set_expected_power_leaving_area_in_MW(area_schedule);

                psdb.append_area(area);
            }
        }
    }
}

void BPA_IMEXPORTER::load_zone_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
           card_type == "BF" or card_type == "BJ" or card_type == "BX" or // PQ
           card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
           card_type == "BL" or // PV
           card_type=="BS" or
           card_type=="BD" or card_type == "BM" or card_type == "BZ")
        {
            data = grow_string_to_at_least_size(data, 80);

            string zone_name = data.substr(18,2);
            unsigned int zone_number = psdb.zone_name2zone_number(zone_name);
            if(zone_number == 0)
            {
                ZONE zone(toolkit);
                zone.set_zone_name(zone_name);

                unsigned int zone_count = psdb.get_zone_count();
                zone.set_zone_number(zone_count+1);
                psdb.append_zone(zone);
            }
        }
    }
}

void BPA_IMEXPORTER::load_owner_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
           card_type == "BF" or card_type == "BJ" or card_type == "BX" or // PQ
           card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
           card_type == "BL" or // PV
           card_type=="BS" or
           card_type=="BD" or card_type == "BM" or card_type=="+" or card_type == "BZ" or
           card_type=="L" or card_type == "L+" or card_type=="E" or card_type == "T" or
           card_type=="TP" or card_type == "R" or card_type=="LD" or card_type == "T")
        {
            data = grow_string_to_at_least_size(data, 80);

            string owner_name = data.substr(3,3);
            unsigned int owner_number = psdb.owner_name2owner_number(owner_name);
            if(owner_number == 0)
            {
                OWNER owner(toolkit);
                owner.set_owner_name(owner_name);

                unsigned int owner_count = psdb.get_owner_count();
                owner.set_owner_number(owner_count+1);
                psdb.append_owner(owner);
            }
        }
    }
}

void BPA_IMEXPORTER::load_bus_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
           card_type == "BF" or card_type == "BJ" or card_type == "BX" or // PQ
           card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
           card_type == "BL" or // PV
           card_type=="BS")
        {
            BUS bus(toolkit);

            unsigned int bus_count = psdb.get_bus_count();
            bus.set_bus_number(bus_count+1);

            data = grow_string_to_at_least_size(data, 80);

            string bus_name = data.substr(6, 8);
            bus.set_bus_name(bus_name);

            string base_voltage_str = data.substr(14, 4);
            base_voltage_str = format_bpa_data_to_readable_data(base_voltage_str, "F4.0");
            double base_voltage = get_double_data(base_voltage_str, "1.0");
            bus.set_base_voltage_in_kV(base_voltage);

            string owner_name = data.substr(3,3);
            unsigned int owner_number = psdb.owner_name2owner_number(owner_name);
            bus.set_owner_number(owner_number);

            string zone_name = data.substr(18,2);
            unsigned int zone_number = psdb.zone_name2zone_number(zone_name);
            bus.set_zone_number(zone_number);

            string voltage_str = data.substr(57, 4);
            voltage_str = format_bpa_data_to_readable_data(voltage_str, "F4.3");
            double voltage = get_double_data(voltage_str, "1.0");
            bus.set_positive_sequence_voltage_in_pu(voltage);

            BUS_TYPE bus_type;
            if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
               card_type == "BF" or card_type == "BJ" or card_type == "BX")// PQ
               bus_type = PQ_TYPE;
            else
            {
                if(card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
                   card_type == "BL") // PV
                    bus_type = PV_TYPE;
                else
                    bus_type = SLACK_TYPE;
            }

            bus.set_bus_type(bus_type);
            if(bus_type==PQ_TYPE or bus_type==PV_TYPE)
            {
                string vmax_str = data.substr(57, 4);
                string vmin_str = data.substr(61, 4);

                vmax_str = format_bpa_data_to_readable_data(vmax_str, "F4.3");
                vmin_str = format_bpa_data_to_readable_data(vmin_str, "F4.3");

                double vmax = get_double_data(vmax_str, "1.1");
                double vmin = get_double_data(vmin_str, "0.9");

                bus.set_voltage_upper_limit_in_pu(vmax);
                bus.set_voltage_lower_limit_in_pu(vmin);
            }
            else
            {
                string angle_str = data.substr(61, 4);
                angle_str = format_bpa_data_to_readable_data(angle_str, "F4.1");
                double angle = get_double_data(angle_str, "0.0");
                bus.set_positive_sequence_angle_in_deg(angle);
            }
            psdb.append_bus(bus);
        }
    }
    set_bus_area();
    set_area_swing_bus();
}

void BPA_IMEXPORTER::set_bus_area()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    vector<BUS*> buses = psdb.get_all_buses();

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="A" or card_type == "AC" or card_type == "AC+")
        {
            data = grow_string_to_at_least_size(data, 95);

            string area_name = data.substr(3,10);

            unsigned int area_number = psdb.area_name2area_number(area_name);
            if(area_number == 0)
                continue;

            for(unsigned int j=0; j<20; ++j)
            {
                string zone_name = data.substr(35+j*3, 2);
                unsigned int zone_number = psdb.zone_name2zone_number(zone_name);
                if(zone_number == 0)
                    continue;

                unsigned int bus_count = psdb.get_bus_count();
                for(unsigned int k=0; k<bus_count; ++k)
                {
                    if(buses[k]->get_zone_number() == zone_number)
                        buses[k]->set_area_number(area_number);
                }
            }
        }
    }
}

void BPA_IMEXPORTER::set_area_swing_bus()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="A" or card_type == "AC")
        {
            data = grow_string_to_at_least_size(data, 95);

            string area_name = data.substr(3,10);

            unsigned int area_number = psdb.area_name2area_number(area_name);
            AREA* area = psdb.get_area(area_number);
            if(area !=NULL)
            {
                string slack_bus_name = data.substr(13, 8);
                unsigned int slack_bus_number = psdb.bus_name2bus_number(slack_bus_name);

                area->set_area_swing_bus(slack_bus_number);
            }
        }
    }

    vector<BUS*> buses = psdb.get_all_buses();

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="A" or card_type == "AC" or card_type == "AC+")
        {
            data = grow_string_to_at_least_size(data, 95);

            string area_name = data.substr(3,10);

            unsigned int area_number = psdb.area_name2area_number(area_name);
            if(area_number == 0)
                continue;

            for(unsigned int j=0; j<20; ++j)
            {
                string zone_name = data.substr(35+j*3, 2);
                unsigned int zone_number = psdb.zone_name2zone_number(zone_name);
                if(zone_number == 0)
                    continue;

                unsigned int bus_count = psdb.get_bus_count();
                for(unsigned int k=0; k<bus_count; ++k)
                {
                    if(buses[k]->get_zone_number() == zone_number)
                        buses[k]->set_area_number(area_number);
                }
            }
        }
    }
}

void BPA_IMEXPORTER::load_load_and_fixed_shunt_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
           card_type == "BF" or card_type == "BJ" or card_type == "BX" or // PQ
           card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
           card_type == "BL" or // PV
           card_type=="BS")
        {
            LOAD load(toolkit);

            data = grow_string_to_at_least_size(data, 80);

            string bus_name = data.substr(6, 8);
            unsigned int bus_number = psdb.bus_name2bus_number(bus_name);
            if(bus_number==0)
                continue;
            load.set_load_bus(bus_number);

            load.set_identifier("1");
            load.set_status(true);

            string owner_name = data.substr(3,3);
            unsigned int owner_number = psdb.owner_name2owner_number(owner_name);
            load.set_owner_number(owner_number);

            BUS* bus = psdb.get_bus(bus_number);
            if(bus==NULL)
                continue;

            load.set_area_number(bus->get_area_number());
            load.set_zone_number(bus->get_zone_number());


            string P_str = data.substr(20, 5);
            P_str = format_bpa_data_to_readable_data(P_str, "F5.0");
            double P = get_double_data(P_str, "0.0");
            string Q_str = data.substr(25, 5);
            Q_str = format_bpa_data_to_readable_data(Q_str, "F5.0");
            double Q = get_double_data(Q_str, "0.0");

            load.set_nominal_constant_power_load_in_MVA(complex<double>(P, Q));

            P_str = data.substr(30, 4);
            P_str = format_bpa_data_to_readable_data(P_str, "F4.0");
            P = get_double_data(P_str, "0.0");
            Q_str = data.substr(34, 4);
            Q_str = format_bpa_data_to_readable_data(Q_str, "F4.0");
            Q = get_double_data(Q_str, "0.0");

            load.set_nominal_constant_impedance_load_in_MVA(complex<double>(P, -Q));

            psdb.append_load(load);
        }
    }

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="+")
        {
            data = grow_string_to_at_least_size(data, 80);

            string bus_name = data.substr(6, 8);
            unsigned int bus_number = psdb.bus_name2bus_number(bus_name);
            if(bus_number==0)
                continue;

            DEVICE_ID did;
            did.set_device_type(STEPS_LOAD);
            TERMINAL terminal;
            terminal.append_bus(bus_number);
            did.set_device_terminal(terminal);
            did.set_device_identifier_index(get_index_of_string("1"));

            LOAD* load = psdb.get_load(did);
            if(load==NULL)
                continue;

            string load_code = data.substr(18, 2);
            load_code = format_bpa_data_to_readable_data(load_code, "A2");
            load_code = get_string_data(load_code, "*I");

            string P_str1 = data.substr(20, 5);
            P_str1 = format_bpa_data_to_readable_data(P_str1, "F5.0");
            double P1 = get_double_data(P_str1, "0.0");
            string Q_str1 = data.substr(25, 5);
            Q_str1 = format_bpa_data_to_readable_data(Q_str1, "F5.0");
            double Q1 = get_double_data(Q_str1, "0.0");

            string P_str2 = data.substr(20, 4);
            P_str2 = format_bpa_data_to_readable_data(P_str2, "F4.0");
            double P2 = get_double_data(P_str2, "0.0");
            string Q_str2 = data.substr(25, 5);
            Q_str2 = format_bpa_data_to_readable_data(Q_str2, "F5.0");
            double Q2 = get_double_data(Q_str2, "0.0");

            if(load_code=="*I" or load_code=="01")
                load->set_nominal_constant_current_load_in_MVA(complex<double>(P1, Q1));
            else
            {
                if(load_code=="*P" or load_code=="02")
                {
                    complex<double> S = complex<double>(P1, Q1)+ load->get_nominal_constant_power_load_in_MVA();
                    load->set_nominal_constant_power_load_in_MVA(S);
                }
            }
            complex<double> S = complex<double>(P2, -Q2)+ load->get_nominal_constant_impedance_load_in_MVA();
            load->set_nominal_constant_impedance_load_in_MVA(S);
        }
    }
    while(true)
    {
        bool load_cleared = false;
        vector<LOAD*> loads = psdb.get_all_loads();
        for(unsigned int i=0; i<loads.size(); ++i)
        {
            complex<double> S_p = loads[i]->get_nominal_constant_power_load_in_MVA();
            complex<double> S_i = loads[i]->get_nominal_constant_current_load_in_MVA();
            complex<double> S_z = loads[i]->get_nominal_constant_impedance_load_in_MVA();
            if(S_p==0.0 and S_i==0.0 and S_z==0.0)
            {
                DEVICE_ID did = loads[i]->get_device_id();
                psdb.clear_load(did);
                load_cleared = true;
                break;
            }
            else
            {
                if(S_p==0.0 and S_i==0.0)
                {
                    FIXED_SHUNT fshunt(toolkit);

                    fshunt.set_shunt_bus(loads[i]->get_load_bus());
                    fshunt.set_identifier(loads[i]->get_identifier());
                    fshunt.set_status(true);
                    fshunt.set_nominal_positive_sequence_impedance_shunt_in_MVA(S_z);
                    psdb.append_fixed_shunt(fshunt);

                    DEVICE_ID did = loads[i]->get_device_id();
                    psdb.clear_load(did);
                    load_cleared = true;
                    break;
                }
            }
        }
        if(not load_cleared)
            break;
    }
}


void BPA_IMEXPORTER::load_fixed_shunt_data()
{
    ;
}

void BPA_IMEXPORTER::load_source_data()
{
    ;
}

void BPA_IMEXPORTER::load_generator_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="B" or card_type == "BT" or card_type == "BC" or card_type == "BV" or
           card_type == "BF" or card_type == "BJ" or card_type == "BX" or // PQ
           card_type=="BE" or card_type == "BQ" or card_type == "BG" or card_type == "BK" or
           card_type == "BL" or // PV
           card_type=="BS")
        {
            data = grow_string_to_at_least_size(data, 80);

            string owner_name = data.substr(3,3);
            string bus_name = data.substr(6, 8);
            unsigned int bus_number = psdb.bus_name2bus_number(bus_name);
            if(bus_number==0)
                continue;

            string pmax_str = data.substr(38, 4);
            string pgen_str = data.substr(42, 5);

            string qgen_str = data.substr(47, 5);
            string qmax_str = qgen_str;

            string qmin_str = data.substr(52, 5);
            string v_schedule_str = data.substr(57, 4);

            GENERATOR generator(toolkit);

            generator.set_generator_bus(bus_number);
            generator.set_bus_to_regulate(bus_number);
            generator.set_identifier("1");
            generator.set_status(true);
            generator.set_mbase_in_MVA(psdb.get_system_base_power_in_MVA());

            pgen_str = format_bpa_data_to_readable_data(pgen_str, "F5.0");
            double pgen = get_double_data(pgen_str, "0.0");
            generator.set_p_generation_in_MW(pgen);

            qgen_str = format_bpa_data_to_readable_data(qgen_str, "F5.0");
            double qgen = get_double_data(qgen_str, "0.0");
            generator.set_q_generation_in_MVar(qgen);

            pmax_str = format_bpa_data_to_readable_data(pmax_str, "F4.0");
            double pmax = get_double_data(pmax_str, "0.0");
            generator.set_p_max_in_MW(pmax);
            generator.set_p_min_in_MW(0.0);

            qmax_str = format_bpa_data_to_readable_data(qmax_str, "F5.0");
            double qmax = get_double_data(qmax_str, "0.0");
            generator.set_q_max_in_MVar(qmax);

            qmin_str = format_bpa_data_to_readable_data(qmin_str, "F5.0");
            double qmin = get_double_data(qmin_str, "0.0");
            generator.set_q_min_in_MVar(qmin);

            v_schedule_str = format_bpa_data_to_readable_data(v_schedule_str, "F4.3");
            double v_schedule = get_double_data(v_schedule_str, "1.0");
            generator.set_voltage_to_regulate_in_pu(v_schedule);

            psdb.append_generator(generator);
        }
    }
}

void BPA_IMEXPORTER::load_wt_generator_data()
{
    ;
}

void BPA_IMEXPORTER::load_source_common_data()
{
    ;

}
void BPA_IMEXPORTER::load_line_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    for(unsigned int i=0; i!=n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);
        if(card_type=="L")
        {
            data = grow_string_to_at_least_size(data, 80);

            string owner_name = data.substr(3,3);
            string ibus_name = data.substr(6, 8);
            string ibase_voltage_str = data.substr(14, 4);
            string meter_end = data.substr(18,1);
            string jbus_name = data.substr(19, 8);
            string jbase_voltage_str = data.substr(27, 4);
            string identifier = data.substr(31, 1);
            string rate_current_in_A = data.substr(33, 4);
            string r_str = data.substr(38, 6);
            string x_str = data.substr(44, 6);
            string g_half_str = data.substr(50, 6);
            string b_half_str = data.substr(56, 6);
            string length_str = data.substr(62, 4);

            unsigned int owner = psdb.owner_name2owner_number(owner_name);

            unsigned int ibus = psdb.bus_name2bus_number(ibus_name);
            unsigned int jbus = psdb.bus_name2bus_number(jbus_name);

            identifier = get_string_data(identifier, "");

            r_str = format_bpa_data_to_readable_data(r_str, "F6.5");
            double r = get_double_data(r_str, "0.0");

            x_str = format_bpa_data_to_readable_data(x_str, "F6.5");
            double x = get_double_data(x_str, "0.0");

            g_half_str = format_bpa_data_to_readable_data(g_half_str, "F6.5");
            double g_half = get_double_data(g_half_str, "0.0");

            b_half_str = format_bpa_data_to_readable_data(b_half_str, "F6.5");
            double b_half = get_double_data(b_half_str, "0.0");

            rate_current_in_A = format_bpa_data_to_readable_data(rate_current_in_A, "F4.0");
            double rate = get_double_data(rate_current_in_A, "0.0");
            double base_voltage = psdb.get_bus_base_voltage_in_kV(ibus);
            rate = (SQRT3*0.001)*rate*base_voltage;

            length_str = format_bpa_data_to_readable_data(length_str, "F4.1");
            double length = get_double_data(length_str,"0.0");

            LINE line(toolkit);

            line.set_sending_side_bus(ibus);
            line.set_receiving_side_bus(jbus);
            line.set_sending_side_breaker_status(true);
            line.set_receiving_side_breaker_status(true);
            line.set_identifier(identifier);

            RATING rating;
            rating.set_rating_A_MVA(rate);
            line.set_rating(rating);

            line.set_length(length);

            line.set_line_positive_sequence_z_in_pu(complex<double>(r, x));
            line.set_line_positive_sequence_y_in_pu(complex<double>(g_half, b_half)*2.0);

            if(owner != 0)
            {
                OWNERSHIP os;
                os.append_owner_and_its_fraction(owner, 1.0);
                line.set_ownership(os);
            }

            if(meter_end == "1")
                line.set_meter_end_bus(ibus);
            else
            {
                if(meter_end=="2")
                    line.set_meter_end_bus(jbus);
                else
                {
                    owner = line.get_owner_of_index(0);
                    if(owner!=0)
                    {
                        BUS* ibus_ptr = psdb.get_bus(ibus);
                        BUS* jbus_ptr = psdb.get_bus(jbus);
                        if(ibus_ptr!=NULL and jbus_ptr!=NULL)
                        {
                            unsigned int iowner = ibus_ptr->get_owner_number();
                            unsigned int jowner = jbus_ptr->get_owner_number();
                            if(iowner==jowner)
                                line.set_meter_end_bus(ibus);
                            else
                            {
                                if(iowner!=0 and iowner!=owner)
                                    line.set_meter_end_bus(ibus);
                                else
                                {
                                    if(jowner!=0 and jowner!=owner)
                                        line.set_meter_end_bus(jbus);
                                    else
                                        line.set_meter_end_bus(ibus);
                                }
                            }
                        }
                    }
                }
            }
            while(psdb.is_line_exist(line.get_device_id()))
                line.set_identifier(line.get_identifier()+"#");
            psdb.append_line(line);
        }
    }

    for(unsigned int i=0; i!=n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="E")
        {
            data = grow_string_to_at_least_size(data, 80);

            string owner_name = data.substr(3,3);
            string ibus_name = data.substr(6, 8);
            string ibase_voltage_str = data.substr(14, 4);
            string meter_end = data.substr(18,1);
            string jbus_name = data.substr(19, 8);
            string jbase_voltage_str = data.substr(27, 4);
            string identifier = data.substr(31, 1);
            string rate_current_in_A = data.substr(33, 4);
            string r_str = data.substr(38, 6);
            string x_str = data.substr(44, 6);
            string ig_str = data.substr(50, 6);
            string ib_str = data.substr(56, 6);
            string jg_str = data.substr(62, 6);
            string jb_str = data.substr(68, 6);

            unsigned int owner = psdb.owner_name2owner_number(owner_name);

            unsigned int ibus = psdb.bus_name2bus_number(ibus_name);
            unsigned int jbus = psdb.bus_name2bus_number(jbus_name);

            r_str = format_bpa_data_to_readable_data(r_str, "F6.5");
            double r = get_double_data(r_str, "0.0");

            x_str = format_bpa_data_to_readable_data(x_str, "F6.5");
            double x = get_double_data(x_str, "0.0");

            ig_str = format_bpa_data_to_readable_data(ig_str, "F6.5");
            double ig = get_double_data(ig_str, "0.0");

            ib_str = format_bpa_data_to_readable_data(ib_str, "F6.5");
            double ib = get_double_data(ib_str, "0.0");

            jg_str = format_bpa_data_to_readable_data(jg_str, "F6.5");
            double jg = get_double_data(jg_str, "0.0");

            jb_str = format_bpa_data_to_readable_data(jb_str, "F6.5");
            double jb = get_double_data(jb_str, "0.0");

            rate_current_in_A = format_bpa_data_to_readable_data(rate_current_in_A, "F4.0");
            double rate = get_double_data(rate_current_in_A, "0.0");
            double base_voltage = psdb.get_bus_base_voltage_in_kV(ibus);
            rate = SQRT3*rate*base_voltage;

            LINE line(toolkit);

            line.set_sending_side_bus(ibus);
            line.set_receiving_side_bus(jbus);
            line.set_sending_side_breaker_status(true);
            line.set_receiving_side_breaker_status(true);
            line.set_identifier(identifier);

            RATING rating;
            rating.set_rating_A_MVA(rate);
            line.set_rating(rating);

            line.set_line_positive_sequence_z_in_pu(complex<double>(r, x));
            line.set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(ig, ib));
            line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(jg, jb));

            if(owner != 0)
            {
                OWNERSHIP os;
                os.append_owner_and_its_fraction(owner, 1.0);
                line.set_ownership(os);
            }

            if(meter_end == "1")
                line.set_meter_end_bus(ibus);
            else
            {
                if(meter_end=="2")
                    line.set_meter_end_bus(jbus);
                else
                {
                    owner = line.get_owner_of_index(0);
                    if(owner!=0)
                    {
                        BUS* ibus_ptr = psdb.get_bus(ibus);
                        BUS* jbus_ptr = psdb.get_bus(jbus);
                        if(ibus_ptr!=NULL and jbus_ptr!=NULL)
                        {
                            unsigned int iowner = ibus_ptr->get_owner_number();
                            unsigned int jowner = jbus_ptr->get_owner_number();
                            if(iowner==jowner)
                                line.set_meter_end_bus(ibus);
                            else
                            {
                                if(iowner!=0 and iowner!=owner)
                                    line.set_meter_end_bus(ibus);
                                else
                                {
                                    if(jowner!=0 and jowner!=owner)
                                        line.set_meter_end_bus(jbus);
                                    else
                                        line.set_meter_end_bus(ibus);
                                }
                            }
                        }
                    }
                }
            }
            psdb.append_line(line);
        }
    }

    double sbase = psdb.get_system_base_power_in_MVA();

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="L+")
        {
            data = grow_string_to_at_least_size(data, 48);

            string ibus_name = data.substr(6, 8);
            string ibase_voltage_str = data.substr(14, 4);
            string jbus_name = data.substr(19, 8);
            string jbase_voltage_str = data.substr(27, 4);
            string identifier = data.substr(31, 1);
            string imvar_str = data.substr(33, 5);
            string jmvar_str = data.substr(43, 5);

            unsigned int ibus = psdb.bus_name2bus_number(ibus_name);
            unsigned int jbus = psdb.bus_name2bus_number(jbus_name);
            if(ibus==0 or jbus==0)
                continue;

            imvar_str = format_bpa_data_to_readable_data(imvar_str, "F5.0");
            double imvar = get_double_data(imvar_str, "0.0");

            jmvar_str = format_bpa_data_to_readable_data(jmvar_str, "F5.0");
            double jmvar = get_double_data(jmvar_str, "0.0");

            DEVICE_ID did;
            did.set_device_type(STEPS_LINE);
            TERMINAL terminal;
            terminal.append_bus(ibus);
            terminal.append_bus(jbus);
            did.set_device_terminal(terminal);
            did.set_device_identifier_index(get_index_of_string(identifier));

            LINE* line = psdb.get_line(did);
            if(line==NULL)
                continue;

            line->set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(0.0, -imvar/sbase));
            line->set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(0.0, -jmvar/sbase));
        }
    }
}

void BPA_IMEXPORTER::load_transformer_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="T" || card_type=="TP")
        {
            data = grow_string_to_at_least_size(data, 80);

            string owner_name = data.substr(3,3);
            string primary_bus_name=data.substr(6,8);
            string primary_bus_base_voltage_str=data.substr(14,4);
            string meter_end=data.substr(18,1);
            string secondary_bus_name=data.substr(19,8);
            string secondary_bus_base_voltage_str=data.substr(27,4);
            string identifier=data.substr(31,1);
            string primary_and_secondary_nominal_capacity_str=data.substr(33,4);
            string resistance_between_primary_and_secondary_str=data.substr(38,6);
            string reactance_between_primary_and_secondary_str=data.substr(44,6);
            string conductance_between_primary_and_secondary_str=data.substr(50,6);
            string susceptance_between_primary_and_secondary_str=data.substr(56,6);
            string primary_tap_location_in_kv_str=data.substr(62,5);
            string secondary_tap_location_in_kv_str=data.substr(67,5);
            string phase_shift_between_primary_and_secondary_str=data.substr(62,5);

            owner_name=format_bpa_data_to_readable_data(owner_name,"A3");
            unsigned int owner = psdb.owner_name2owner_number(owner_name);

            primary_bus_name=format_bpa_data_to_readable_data(primary_bus_name,"A8");
            unsigned int primary_bus = psdb.bus_name2bus_number(primary_bus_name);

            secondary_bus_name=format_bpa_data_to_readable_data(secondary_bus_name,"A8");
            unsigned int secondary_bus = psdb.bus_name2bus_number(secondary_bus_name);

            if(primary_bus==0 or secondary_bus==0)
                continue;

            identifier = get_string_data(identifier, "");

            primary_bus_base_voltage_str=format_bpa_data_to_readable_data(primary_bus_base_voltage_str,"F4.0");
            double primary_bus_base_voltage=get_double_data(primary_bus_base_voltage_str,"0.0");

            meter_end=format_bpa_data_to_readable_data(meter_end,"I1");


            secondary_bus_base_voltage_str=format_bpa_data_to_readable_data(secondary_bus_base_voltage_str,"F4.0");
            double secondary_bus_base_voltage=get_double_data(secondary_bus_base_voltage_str,"0.0");

            identifier=format_bpa_data_to_readable_data(identifier,"A1");

            primary_and_secondary_nominal_capacity_str=format_bpa_data_to_readable_data( primary_and_secondary_nominal_capacity_str,"F4.0");
            double primary_and_secondary_nominal_capacity=get_double_data(primary_and_secondary_nominal_capacity_str,"0.0");

            resistance_between_primary_and_secondary_str=format_bpa_data_to_readable_data(resistance_between_primary_and_secondary_str,"F6.5");
            double resistance_between_primary_and_secondary=get_double_data(resistance_between_primary_and_secondary_str,"0.0");
            reactance_between_primary_and_secondary_str=format_bpa_data_to_readable_data(reactance_between_primary_and_secondary_str,"F6.5");
            double reactance_between_primary_and_secondary=get_double_data(reactance_between_primary_and_secondary_str,"0.0");
            conductance_between_primary_and_secondary_str=format_bpa_data_to_readable_data(conductance_between_primary_and_secondary_str,"F6.5");
            double conductance_between_primary_and_secondary=get_double_data(conductance_between_primary_and_secondary_str,"0.0");
            susceptance_between_primary_and_secondary_str=format_bpa_data_to_readable_data(susceptance_between_primary_and_secondary_str,"F6.5");
            double susceptance_between_primary_and_secondary=get_double_data(susceptance_between_primary_and_secondary_str,"0.0");

            primary_tap_location_in_kv_str=format_bpa_data_to_readable_data(primary_tap_location_in_kv_str,"F5.2");
            double primary_tap_location_in_kv=get_double_data(primary_tap_location_in_kv_str,"0.0");
            secondary_tap_location_in_kv_str=format_bpa_data_to_readable_data(secondary_tap_location_in_kv_str,"F5.2");
            double secondary_tap_location_in_kv=get_double_data(secondary_tap_location_in_kv_str,"0.0");

            phase_shift_between_primary_and_secondary_str=format_bpa_data_to_readable_data(phase_shift_between_primary_and_secondary_str,"F5.2");
            double phase_shift_between_primary_and_secondary = get_double_data(phase_shift_between_primary_and_secondary_str,"0.0");

            TRANSFORMER trans(toolkit);

            trans.set_identifier(identifier);
            if(owner != 0)
            {
                OWNERSHIP os;
                os.append_owner_and_its_fraction(owner, 1.0);
                trans.set_ownership(os);
            }

            if(meter_end == "1")
                trans.set_non_metered_end_bus(secondary_bus);
            else
            {
                if(meter_end=="2")
                    trans.set_non_metered_end_bus(primary_bus);
                else
                {
                    owner = trans.get_owner_of_index(0);
                    if(owner!=0)
                    {
                        BUS* primary_bus_ptr = psdb.get_bus(primary_bus);
                        BUS* secondary_bus_ptr = psdb.get_bus(secondary_bus);
                        if(primary_bus_ptr!=NULL and secondary_bus_ptr!=NULL)
                        {
                            unsigned int primary_bus_owner = primary_bus_ptr->get_owner_number();
                            unsigned int secondary_bus_owner = secondary_bus_ptr->get_owner_number();
                            if(primary_bus_owner==secondary_bus_owner)
                                 trans.set_non_metered_end_bus(secondary_bus_owner);
                            else
                            {
                                if(primary_bus_owner!=0 and primary_bus_owner!=owner)
                                    trans.set_non_metered_end_bus(secondary_bus_owner);
                                else
                                {
                                    if(secondary_bus_owner!=0 and secondary_bus_owner!=owner)
                                        trans.set_non_metered_end_bus(primary_bus_owner);
                                    else
                                        trans.set_non_metered_end_bus(secondary_bus_owner);
                                }
                            }
                        }
                    }
                }
            }

            complex <double> magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(conductance_between_primary_and_secondary,susceptance_between_primary_and_secondary);
            trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu);

            trans.set_winding_bus(PRIMARY_SIDE,primary_bus);
            trans.set_winding_bus(SECONDARY_SIDE,secondary_bus);

            trans.set_winding_breaker_status(PRIMARY_SIDE,true);
            trans.set_winding_breaker_status(SECONDARY_SIDE,true);

            trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE,primary_bus_base_voltage);
            trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE,secondary_bus_base_voltage);

            trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,SECONDARY_SIDE,primary_and_secondary_nominal_capacity);

            double sbase_MVA = psdb.get_system_base_power_in_MVA();
            complex<double> impedance_between_primary_and_secondary_based_on_bus_base_voltage_and_system_base_power_in_pu(resistance_between_primary_and_secondary,reactance_between_primary_and_secondary);
            complex <double> impedance_between_primary_and_secondary_based_on_winding_nominals_in_pu = impedance_between_primary_and_secondary_based_on_bus_base_voltage_and_system_base_power_in_pu * (primary_and_secondary_nominal_capacity/sbase_MVA);
            trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE,SECONDARY_SIDE,impedance_between_primary_and_secondary_based_on_winding_nominals_in_pu);

            if(card_type=="T")
            {
                double primary_turn_ratio_based_on_primary_nominal_voltage_in_pu = primary_tap_location_in_kv/primary_bus_base_voltage;
                double secondary_turn_ratio_based_on_secondary_nominal_voltage_in_pu = secondary_tap_location_in_kv/secondary_bus_base_voltage;
                trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE,primary_turn_ratio_based_on_primary_nominal_voltage_in_pu);
                trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE,secondary_turn_ratio_based_on_secondary_nominal_voltage_in_pu);

            }
            if(card_type=="TP")
            {
                trans.set_winding_angle_shift_in_deg(PRIMARY_SIDE,phase_shift_between_primary_and_secondary);
                trans.set_winding_angle_shift_in_deg(SECONDARY_SIDE,0);

            }

           double rate=primary_and_secondary_nominal_capacity;
           RATING rating;
           rating.set_rating_A_MVA(rate);
           trans.set_winding_rating_in_MVA(PRIMARY_SIDE,rating);
           trans.set_winding_rating_in_MVA(SECONDARY_SIDE,rating);

           psdb.append_transformer(trans);
        }
    }

    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if( card_type=="R" || card_type=="RV" || card_type=="RQ" || card_type=="RP" || card_type=="RN" || card_type=="RM" )
        {
            string primary_bus_name=data.substr(6,8);
            string secondary_bus_name=data.substr(19,8);
            string adjustable_end_flag = data.substr(18,1);
            string controlled_bus_name=data.substr(33,8);
            string identifier = data.substr(31,1);
            string primary_bus_base_voltage_str=data.substr(14,4);
            string secondary_bus_base_voltage_str=data.substr(27,4);

            primary_bus_name = format_bpa_data_to_readable_data(primary_bus_name,"A8");
            secondary_bus_name = format_bpa_data_to_readable_data(secondary_bus_name,"A8");
            adjustable_end_flag = format_bpa_data_to_readable_data(adjustable_end_flag,"I1");
            controlled_bus_name = format_bpa_data_to_readable_data(controlled_bus_name,"A8");
            identifier = format_bpa_data_to_readable_data(identifier,"A1");

            primary_bus_base_voltage_str = format_bpa_data_to_readable_data( primary_bus_base_voltage_str,"F4.0");
            double primary_bus_base_voltage = get_double_data(primary_bus_base_voltage_str,"0.0");
            double primary_winding_nominal_voltage = primary_bus_base_voltage;

            secondary_bus_base_voltage_str = format_bpa_data_to_readable_data( secondary_bus_base_voltage_str,"F4.0");
            double secondary_bus_base_voltage = get_double_data(secondary_bus_base_voltage_str,"0.0");
            double secondary_winding_nominal_voltage = secondary_bus_base_voltage;


            unsigned int primary_bus = psdb.bus_name2bus_number(primary_bus_name);
            unsigned int secondary_bus = psdb.bus_name2bus_number(secondary_bus_name);
            if(primary_bus==0 or secondary_bus==0)
                continue;

            DEVICE_ID did;
            did.set_device_type(STEPS_TRANSFORMER);
            TERMINAL terminal;
            terminal.append_bus(primary_bus);
            terminal.append_bus(secondary_bus);
            did.set_device_terminal(terminal);
            did.set_device_identifier_index(get_index_of_string(identifier));

            TRANSFORMER* trans = psdb.get_transformer(did);
            if(trans==NULL)
                continue;

            if(adjustable_end_flag=="1")
            trans->set_winding_controlled_bus(PRIMARY_SIDE,primary_bus);
            if(adjustable_end_flag=="2")
            trans->set_winding_controlled_bus(SECONDARY_SIDE,secondary_bus);

            if(card_type=="R" || card_type=="RV" || card_type=="RQ" ||card_type=="RN")
            {
                string max_voltage_adjustable_tap_in_kv_str=data.substr(45,5);
                max_voltage_adjustable_tap_in_kv_str=format_bpa_data_to_readable_data(max_voltage_adjustable_tap_in_kv_str,"F5.2");
                double max_voltage_adjustable_tap_in_kv = get_double_data(max_voltage_adjustable_tap_in_kv_str,"0.0");

                string min_voltage_adjustable_tap_in_kv_str=data.substr(50,5);
                min_voltage_adjustable_tap_in_kv_str=format_bpa_data_to_readable_data(min_voltage_adjustable_tap_in_kv_str,"F5.2");
                //double min_voltage_adjustable_tap_in_kv = get_double_data(min_voltage_adjustable_tap_in_kv_str,"0.0");

                string number_of_taps_str=data.substr(55,2);
                number_of_taps_str=format_bpa_data_to_readable_data(number_of_taps_str,"I2");
                unsigned int number_of_taps=get_integer_data(number_of_taps_str,"0");

                TRANSFORMER_WINDING_SIDE winding =PRIMARY_SIDE;
                double winding_nominal_voltage = primary_winding_nominal_voltage;
                if(adjustable_end_flag=="1")
                {
                    winding = PRIMARY_SIDE;
                    winding_nominal_voltage=primary_winding_nominal_voltage;
                }
                if(adjustable_end_flag=="2")
                {
                    winding = SECONDARY_SIDE;
                    winding_nominal_voltage=secondary_winding_nominal_voltage;
                }
                trans->set_winding_controlled_max_voltage_in_pu(winding,max_voltage_adjustable_tap_in_kv);
                double winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu = max_voltage_adjustable_tap_in_kv/winding_nominal_voltage;
                trans->set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding,winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu);
                trans->set_winding_number_of_taps(winding,number_of_taps);
                if (card_type=="R" || card_type=="RV")
                trans->set_winding_control_mode(winding,TRANSFORMER_TAP_VOLTAGE_CONTROL);
                if (card_type=="RQ")
                {
                    string controlled_max_reactive_power_into_winding_in_MVar_str=data.substr(57,5);
                    controlled_max_reactive_power_into_winding_in_MVar_str=format_bpa_data_to_readable_data(controlled_max_reactive_power_into_winding_in_MVar_str,"F5.0");
                    double controlled_max_reactive_power_into_winding_in_MVar=get_double_data(controlled_max_reactive_power_into_winding_in_MVar_str,"0.0");
                    double controlled_min_reactive_power_into_winding_in_MVar=controlled_max_reactive_power_into_winding_in_MVar;

                    trans->set_winding_control_mode(winding,TRANSFORMER_TAP_REACTIVE_POWER_CONTROL);
                    trans->set_controlled_max_reactive_power_into_winding_in_MVar(winding,controlled_max_reactive_power_into_winding_in_MVar);
                    trans->set_controlled_min_reactive_power_into_winding_in_MVar(winding,controlled_min_reactive_power_into_winding_in_MVar);
                }
                if(card_type=="RN")
                {
                    string controlled_max_reactive_power_into_winding_in_MVar_str=data.substr(57,5);
                    controlled_max_reactive_power_into_winding_in_MVar_str=format_bpa_data_to_readable_data(controlled_max_reactive_power_into_winding_in_MVar_str,"F5.0");
                    double controlled_max_reactive_power_into_winding_in_MVar=get_double_data(controlled_max_reactive_power_into_winding_in_MVar_str,"0.0");

                    string controlled_min_reactive_power_into_winding_in_MVar_str=data.substr(62,5);
                    controlled_min_reactive_power_into_winding_in_MVar_str=format_bpa_data_to_readable_data(controlled_min_reactive_power_into_winding_in_MVar_str,"F5.0");
                    double controlled_min_reactive_power_into_winding_in_MVar=get_double_data(controlled_min_reactive_power_into_winding_in_MVar_str,"0.0");

                    trans->set_winding_control_mode(winding,TRANSFORMER_TAP_REACTIVE_POWER_CONTROL);

                    trans->set_controlled_max_reactive_power_into_winding_in_MVar(winding,controlled_max_reactive_power_into_winding_in_MVar);
                    trans->set_controlled_min_reactive_power_into_winding_in_MVar(winding,controlled_min_reactive_power_into_winding_in_MVar);
                }
            }
            if(card_type=="RP"||card_type=="RM")
            {
                string max_phase_shift_str=data.substr(45,5);
                max_phase_shift_str=format_bpa_data_to_readable_data(max_phase_shift_str,"F5.2");
                double max_phase_shift = get_double_data(max_phase_shift_str,"0.0");

                string min_phase_shift_str=data.substr(50,5);
                min_phase_shift_str=format_bpa_data_to_readable_data(min_phase_shift_str,"F5.2");
                double min_phase_shift = get_double_data(min_phase_shift_str,"0.0");

                string number_of_taps_str=data.substr(55,2);
                number_of_taps_str=format_bpa_data_to_readable_data(number_of_taps_str,"I2");
                unsigned int number_of_taps=get_integer_data(number_of_taps_str,"0");

                string max_active_power_into_winding_in_MW_str=data.substr(57,5);
                max_active_power_into_winding_in_MW_str=format_bpa_data_to_readable_data(max_active_power_into_winding_in_MW_str,"F5.0");
                double max_active_power_into_winding_in_MW=get_double_data(max_active_power_into_winding_in_MW_str,"0.0");

                double min_active_power_into_winding_in_MW = 0.0;
                if(card_type=="RP")
                    min_active_power_into_winding_in_MW=max_active_power_into_winding_in_MW;
                if(card_type=="RM")
                {
                    string min_active_power_into_winding_in_MW_str=data.substr(62,5);
                    min_active_power_into_winding_in_MW_str=format_bpa_data_to_readable_data(min_active_power_into_winding_in_MW_str,"F5.0");
                    min_active_power_into_winding_in_MW=get_double_data(max_active_power_into_winding_in_MW_str,"0.0");
                }

                TRANSFORMER_WINDING_SIDE winding = PRIMARY_SIDE;
                //double winding_nominal_voltage = primary_winding_nominal_voltage;
                if(adjustable_end_flag=="1")
                {
                    winding = PRIMARY_SIDE;
                    //winding_nominal_voltage=primary_winding_nominal_voltage;
                }
                if(adjustable_end_flag=="2")
                {
                    winding = SECONDARY_SIDE;
                    //winding_nominal_voltage=secondary_winding_nominal_voltage;
                }

                trans->set_winding_max_angle_shift_in_deg(winding,max_phase_shift);
                trans->set_winding_min_angle_shift_in_deg(winding,min_phase_shift);
                trans->set_winding_control_mode(winding,TRANSFORMER_TAP_ACTIVE_POWER_CONTROL);
                trans->set_controlled_max_active_power_into_winding_in_MW(winding,max_active_power_into_winding_in_MW);
                trans->set_controlled_min_active_power_into_winding_in_MW(winding,min_active_power_into_winding_in_MW);
                trans->set_winding_number_of_taps(winding,number_of_taps);
            }
        }
    }
}

void BPA_IMEXPORTER::load_hvdc_data()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = dat_data_in_ram.size();
    string data, card_type;

    map<string,string> from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name;
    map<string,string> from_rectifier_valve_side_bus_name_to_inverter_valve_side_bus_name;
    map<string,string> from_grid_side_bus_name_to_valve_side_bus_name;
    map<string,string> from_valve_side_bus_name_to_grid_side_bus_name;
    map<string,int>    from_converter_grid_bus_name_to_pole_number;

    // build map
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="LD")
        {
             data = grow_string_to_at_least_size(data, 80);

             string rectifier_valve_side_bus_name = data.substr(6,8);
             string inverter_valve_side_bus_name = data.substr(19,8);

             from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[inverter_valve_side_bus_name]=rectifier_valve_side_bus_name;
             from_rectifier_valve_side_bus_name_to_inverter_valve_side_bus_name[rectifier_valve_side_bus_name]=inverter_valve_side_bus_name;
        }
        if(card_type=="BD")
        {
            data = grow_string_to_at_least_size(data, 80);
            string valve_side_bus_name = data.substr(6,8);
            string grid_side_bus_name = data.substr(50,8);

            from_grid_side_bus_name_to_valve_side_bus_name[grid_side_bus_name]=valve_side_bus_name;
            from_valve_side_bus_name_to_grid_side_bus_name[valve_side_bus_name]=grid_side_bus_name;
        }
    }

    // load hvdc data
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);
        if(card_type=="LD")
        {
            string rectifier_valve_side_bus_name = data.substr(6,8);
            string inverter_valve_side_bus_name = data.substr(19,8);

            string rectifier_grid_side_bus_name = from_valve_side_bus_name_to_grid_side_bus_name[rectifier_valve_side_bus_name];
            string inverter_grid_side_bus_name = from_valve_side_bus_name_to_grid_side_bus_name[inverter_valve_side_bus_name];

            string nominal_dc_current_per_pole_in_kA_str = data.substr(33,4);
            string line_resistance_in_ohm_str = data.substr(37,6);
            string line_inductance_in_mH_str = data.substr(43,6);
            string line_capacitance_in_uF_str = data.substr(49,6);
            string power_controlled_side_flag = data.substr(55,1);
            string nominal_dc_power_per_pole_in_MW_str = data.substr(56,5);
            string nominal_dc_voltage_per_pole_in_kV_str = data.substr(61,5);

            nominal_dc_current_per_pole_in_kA_str=format_bpa_data_to_readable_data(nominal_dc_current_per_pole_in_kA_str,"F4.0");
            double  nominal_dc_current_per_pole_in_kA=get_double_data(nominal_dc_current_per_pole_in_kA_str,"0.0")/1000;
            line_resistance_in_ohm_str=format_bpa_data_to_readable_data(line_resistance_in_ohm_str,"F6.2");
            double line_resistance_in_ohm=get_double_data(line_resistance_in_ohm_str,"0.0");
            line_inductance_in_mH_str=format_bpa_data_to_readable_data(line_inductance_in_mH_str,"F6.2");
            double line_inductance_in_mH=get_double_data(line_inductance_in_mH_str,"0.0");
            line_capacitance_in_uF_str=format_bpa_data_to_readable_data(line_capacitance_in_uF_str,"F6.2");
            double line_capacitance_in_uF=get_double_data(line_capacitance_in_uF_str,"0.0");
            nominal_dc_power_per_pole_in_MW_str=format_bpa_data_to_readable_data(nominal_dc_power_per_pole_in_MW_str,"F5.1");
            double nominal_dc_power_per_pole_in_MW=get_double_data(nominal_dc_power_per_pole_in_MW_str,"0.0");
            nominal_dc_voltage_per_pole_in_kV_str=format_bpa_data_to_readable_data(nominal_dc_voltage_per_pole_in_kV_str,"F5.1");
            double nominal_dc_voltage_per_pole_in_kV=get_double_data(nominal_dc_voltage_per_pole_in_kV_str,"0.0");

            unsigned int rectifier_grid_side_bus = psdb.bus_name2bus_number(rectifier_grid_side_bus_name);
            unsigned int inverter_grid_side_bus = psdb.bus_name2bus_number(inverter_grid_side_bus_name);

            DEVICE_ID did;
            did.set_device_type(STEPS_HVDC);
            TERMINAL terminal;
            terminal.append_bus(rectifier_grid_side_bus);
            terminal.append_bus(inverter_grid_side_bus);
            did.set_device_terminal(terminal);

            HVDC* hvdc_ptr = psdb.get_hvdc(did);

            string bus_names_of_two_converters = rectifier_grid_side_bus_name+","+inverter_grid_side_bus_name;
            if( hvdc_ptr != NULL)
                from_converter_grid_bus_name_to_pole_number[bus_names_of_two_converters] += 1;
            else
                from_converter_grid_bus_name_to_pole_number[bus_names_of_two_converters] = 1;

            HVDC hvdc(toolkit);

            hvdc.set_converter_bus(RECTIFIER,rectifier_grid_side_bus);
            hvdc.set_converter_bus(INVERTER,inverter_grid_side_bus);
            hvdc.set_converter_valve_side_bus_name(RECTIFIER,rectifier_valve_side_bus_name);
            hvdc.set_converter_valve_side_bus_name(INVERTER,inverter_valve_side_bus_name);
            hvdc.set_status(true);
            hvdc.set_line_resistance_in_ohm(line_resistance_in_ohm);
            hvdc.set_line_inductance_in_mH(line_inductance_in_mH);
            hvdc.set_line_capacitance_in_uF(line_capacitance_in_uF);
            hvdc.set_nominal_dc_power_in_MW(nominal_dc_power_per_pole_in_MW);

            if(power_controlled_side_flag=="R")
                hvdc.set_side_to_hold_power(RECTIFIER);
            else if(power_controlled_side_flag=="I")
                hvdc.set_side_to_hold_power(INVERTER);

            hvdc.set_nominal_dc_current_in_kA(nominal_dc_current_per_pole_in_kA);
            hvdc.set_nominal_dc_voltage_in_kV(nominal_dc_voltage_per_pole_in_kV);


            while(psdb.is_hvdc_exist(hvdc.get_device_id()))
                hvdc.set_identifier(hvdc.get_identifier()+"#");

            string hvdc_name = "DC-"+num2str(psdb.get_hvdc_count()+1);
            hvdc.set_name(hvdc_name);
            psdb.append_hvdc(hvdc);
        }
    }

    // load HVDC angle data
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="BD")
        {
            string converter_valve_side_bus_name = data.substr(6,8);
            string converter_grid_side_bus_name = data.substr(50,8);

            string converter_number_of_bridge_str = data.substr(23,2);

            string converter_max_alpha_or_gamma_in_deg_str = data.substr(35,5);
            string converter_min_alpha_or_gamma_in_deg_str = data.substr(30,5);
            string line_smooting_inductance_in_mH_str = data.substr(25,5);

            converter_number_of_bridge_str = format_bpa_data_to_readable_data(converter_number_of_bridge_str,"I2");
            int converter_number_of_bridge = get_integer_data(converter_number_of_bridge_str,"1");
            converter_max_alpha_or_gamma_in_deg_str = format_bpa_data_to_readable_data(converter_max_alpha_or_gamma_in_deg_str,"F5.1");
            double converter_max_alpha_or_gamma_in_deg = get_double_data(converter_max_alpha_or_gamma_in_deg_str,"0.0");
            converter_min_alpha_or_gamma_in_deg_str = format_bpa_data_to_readable_data(converter_min_alpha_or_gamma_in_deg_str,"F5.1");
            double converter_min_alpha_or_gamma_in_deg = get_double_data(converter_min_alpha_or_gamma_in_deg_str,"0.0");

            line_smooting_inductance_in_mH_str=format_bpa_data_to_readable_data(line_smooting_inductance_in_mH_str,"F5.1");
            double line_smooting_inductance_in_mH=get_double_data(line_smooting_inductance_in_mH_str,"0.0");


            unsigned int converter_grid_side_bus_number = psdb.bus_name2bus_number(converter_grid_side_bus_name);
            if(converter_grid_side_bus_number==0)
                continue;

            vector<HVDC*> hvdcs = psdb.get_hvdcs_connecting_to_bus(converter_grid_side_bus_number);
            CONVERTER_SIDE converter_side_type;
            for(unsigned int j=0; j<hvdcs.size(); ++j)
            {
                HVDC* hvdc_ptr = hvdcs[j];
                if(hvdc_ptr->get_converter_bus(RECTIFIER)==converter_grid_side_bus_number)
                    converter_side_type = RECTIFIER;
                else
                    converter_side_type = INVERTER;

                hvdc_ptr->set_converter_number_of_bridge(converter_side_type,converter_number_of_bridge);
                hvdc_ptr->set_converter_max_alpha_or_gamma_in_deg(converter_side_type,converter_max_alpha_or_gamma_in_deg);
                hvdc_ptr->set_converter_min_alpha_or_gamma_in_deg(converter_side_type,converter_min_alpha_or_gamma_in_deg);
                hvdc_ptr->set_line_smooting_inductance_in_mH(converter_side_type, line_smooting_inductance_in_mH);
            }
            /*
            string inverter_grid_side_bus_name;
            string rectifier_valve_side_bus_name;
            string rectifier_grid_side_bus_name;
            string inverter_valve_side_bus_name;

            if(not from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[converter_valve_side_bus_name].empty())
            {
                converter_side_type=INVERTER;
                inverter_grid_side_bus_name=converter_grid_side_bus_name;
                rectifier_valve_side_bus_name=from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[converter_valve_side_bus_name];
                rectifier_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[rectifier_valve_side_bus_name];
            }
            else
            {
                converter_side_type=RECTIFIER;
                rectifier_grid_side_bus_name=converter_grid_side_bus_name;
                inverter_valve_side_bus_name=from_rectifier_valve_side_bus_name_to_inverter_valve_side_bus_name[converter_valve_side_bus_name];
                inverter_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[inverter_valve_side_bus_name];
            }

            unsigned int rectifier_grid_side_bus = psdb.bus_name2bus_number(rectifier_grid_side_bus_name);
            unsigned int inverter_grid_side_bus = psdb.bus_name2bus_number(inverter_grid_side_bus_name);


            DEVICE_ID did;
            did.set_device_type(STEPS_HVDC);
            TERMINAL terminal;
            terminal.append_bus(rectifier_grid_side_bus);
            terminal.append_bus(inverter_grid_side_bus);
            did.set_device_terminal(terminal);

            HVDC* hvdc_ptr = psdb.get_hvdc(did);   //

            if(hvdc_ptr == NULL)
                continue;

            hvdc_ptr->set_converter_number_of_bridge(converter_side_type,converter_number_of_bridge);
            hvdc_ptr->set_converter_max_alpha_or_gamma_in_deg(converter_side_type,converter_max_alpha_or_gamma_in_deg);
            hvdc_ptr->set_converter_min_alpha_or_gamma_in_deg(converter_side_type,converter_min_alpha_or_gamma_in_deg);
            hvdc_ptr->set_line_smooting_inductance_in_mH(line_smooting_inductance_in_mH);
            */
        }
    }

    // load transformer impedance
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type == "T")
        {
            string primary_bus_name=data.substr(6,8);
            string secondary_bus_name=data.substr(19,8);
            string primary_bus_base_voltage_str=data.substr(14,4);
            string secondary_bus_base_voltage_str=data.substr(27,4);
            string resistance_str=data.substr(38,6);
            string reactance_str=data.substr(44,6);
            string conductance_str=data.substr(50,6);
            string susceptance_str=data.substr(56,6);

            primary_bus_base_voltage_str=format_bpa_data_to_readable_data(primary_bus_base_voltage_str,"F4.0");
            double primary_bus_base_voltage=get_double_data(primary_bus_base_voltage_str,"0.0");

            secondary_bus_base_voltage_str=format_bpa_data_to_readable_data(secondary_bus_base_voltage_str,"F4.0");
            double secondary_bus_base_voltage=get_double_data(secondary_bus_base_voltage_str,"0.0");

            unsigned int primary_bus_number = psdb.bus_name2bus_number(primary_bus_name);
            unsigned int secondary_bus_number = psdb.bus_name2bus_number(secondary_bus_name);
            if((primary_bus_number==0 and secondary_bus_number==0) or
               (primary_bus_number!=0 and secondary_bus_number!=0)) // invalid trans or common trans
                continue;

            unsigned int converter_bus = (primary_bus_number!=0 ? primary_bus_number:secondary_bus_number);
            double converter_bus_base_voltage = (converter_bus==primary_bus_number ? primary_bus_base_voltage : secondary_bus_base_voltage);
			double valve_bus_base_voltage = (converter_bus==primary_bus_number ? secondary_bus_base_voltage : primary_bus_base_voltage);

            resistance_str=format_bpa_data_to_readable_data(resistance_str,"F6.5");
            double resistance=get_double_data(resistance_str,"0.0");

            reactance_str=format_bpa_data_to_readable_data(reactance_str,"F6.5");
            double reactance=get_double_data(reactance_str,"0.0");

            conductance_str=format_bpa_data_to_readable_data(conductance_str,"F6.5");
            double conductance=get_double_data(conductance_str,"0.0");

            susceptance_str=format_bpa_data_to_readable_data(susceptance_str,"F6.5");
            double susceptance=get_double_data(susceptance_str,"0.0");

            double sbase_MVA = psdb.get_system_base_power_in_MVA();

            double z_base_based_on_converter_valve_bus_nominal=valve_bus_base_voltage*valve_bus_base_voltage/sbase_MVA;

            double y_base_based_on_converter_valve_bus_nominal=sbase_MVA/(valve_bus_base_voltage*valve_bus_base_voltage);

            double resistance_in_ohm = resistance*z_base_based_on_converter_valve_bus_nominal;
            double reactance_in_ohm = reactance*z_base_based_on_converter_valve_bus_nominal;
            double conductance_in_siemens = conductance*y_base_based_on_converter_valve_bus_nominal;
            double susceptance_in_siemens = susceptance*y_base_based_on_converter_valve_bus_nominal;

            complex <double> impedance_in_ohm(resistance_in_ohm,reactance_in_ohm);
            complex <double> admittance_in_siemens(conductance_in_siemens,susceptance_in_siemens);

            vector<HVDC*> hvdcs = psdb.get_hvdcs_connecting_to_bus(converter_bus);

            CONVERTER_SIDE converter_side_type;
            for(unsigned int j=0; j<hvdcs.size(); ++j)
            {
                HVDC* hvdc_ptr = hvdcs[j];
                if(hvdc_ptr->get_converter_bus(RECTIFIER)==converter_bus)
                    converter_side_type = RECTIFIER;
                else
                    converter_side_type = INVERTER;

                hvdc_ptr->set_converter_transformer_grid_side_base_voltage_in_kV(converter_side_type,converter_bus_base_voltage);
                hvdc_ptr->set_converter_transformer_converter_side_base_voltage_in_kV(converter_side_type,valve_bus_base_voltage);
                hvdc_ptr->set_converter_transformer_impedance_in_ohm(converter_side_type,impedance_in_ohm);
                hvdc_ptr->set_converter_transformer_admittance_in_siemens(converter_side_type,admittance_in_siemens);
            }

            /*
            if(from_valve_side_bus_name_to_grid_side_bus_name[secondary_bus_name] == primary_bus_name)
            {
                CONVERTER_SIDE converter_side_type;
                string inverter_grid_side_bus_name;
                string rectifier_valve_side_bus_name;
                string rectifier_grid_side_bus_name;
                string inverter_valve_side_bus_name;

                if(not from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[secondary_bus_name].empty())
                {
                    converter_side_type=INVERTER;
                    inverter_grid_side_bus_name=primary_bus_name;
                    rectifier_valve_side_bus_name=from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[secondary_bus_name];
                    rectifier_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[rectifier_valve_side_bus_name];
                }
                else
                {
                    converter_side_type=RECTIFIER;
                    rectifier_grid_side_bus_name=primary_bus_name;
                    inverter_valve_side_bus_name=from_rectifier_valve_side_bus_name_to_inverter_valve_side_bus_name[secondary_bus_name];
                    inverter_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[inverter_valve_side_bus_name];
                }

                unsigned int rectifier_grid_side_bus = psdb.bus_name2bus_number(rectifier_grid_side_bus_name);
                unsigned int inverter_grid_side_bus = psdb.bus_name2bus_number(inverter_grid_side_bus_name);

                DEVICE_ID did;
                did.set_device_type(STEPS_HVDC);
                TERMINAL terminal;
                terminal.append_bus(rectifier_grid_side_bus);
                terminal.append_bus(inverter_grid_side_bus);
                did.set_device_terminal(terminal);

                HVDC* hvdc_ptr = psdb.get_hvdc(did);

                if( hvdc_ptr==NULL)
                    continue;

                hvdc_ptr->set_converter_transformer_grid_side_base_voltage_in_kV(converter_side_type,primary_bus_base_voltage);
                hvdc_ptr->set_converter_transformer_converter_side_base_voltage_in_kV(converter_side_type,secondary_bus_base_voltage);
                hvdc_ptr->set_converter_transformer_impedance_in_ohm(converter_side_type,impedance_in_ohm);
                hvdc_ptr->set_converter_transformer_admittance_in_siemens(converter_side_type,admittance_in_siemens);
            }
            */
        }
    }

    // load transformer taps
    for(unsigned int i=0; i<n; ++i)
    {
        data = dat_data_in_ram[i];
        card_type = get_powerflow_card_type(data);

        if(card_type=="R")
        {
            data = grow_string_to_at_least_size(data, 80);
            string primary_bus_name=data.substr(6,8);
            string secondary_bus_name=data.substr(19,8);
            string max_tap_in_kv_str=data.substr(45,5);
            string min_tap_in_kv_str=data.substr(50,5);
            string primary_bus_base_voltage_str=data.substr(14,4);
            string tap_number_str=data.substr(55,2);

            max_tap_in_kv_str=format_bpa_data_to_readable_data(max_tap_in_kv_str,"F5.2");
            double max_tap_in_kv=get_double_data(max_tap_in_kv_str,"0.0");

            min_tap_in_kv_str=format_bpa_data_to_readable_data(min_tap_in_kv_str,"F5.2");
            double min_tap_in_kv=get_double_data(min_tap_in_kv_str,"0.0");

            primary_bus_base_voltage_str=format_bpa_data_to_readable_data(primary_bus_base_voltage_str,"F4.0");
            double primary_bus_base_voltage=get_double_data(primary_bus_base_voltage_str,"0.0");

            double max_tap_in_pu=max_tap_in_kv/primary_bus_base_voltage;
            double min_tap_in_pu=min_tap_in_kv/primary_bus_base_voltage;

            tap_number_str=format_bpa_data_to_readable_data(tap_number_str,"I2");
            unsigned int tap_number=get_integer_data(tap_number_str,"0");

            unsigned int primary_bus_number = psdb.bus_name2bus_number(primary_bus_name);
            unsigned int secondary_bus_number = psdb.bus_name2bus_number(secondary_bus_name);
            if((primary_bus_number==0 and secondary_bus_number==0) or
               (primary_bus_number!=0 and secondary_bus_number!=0)) // invalid trans or common trans
                continue;

            unsigned int converter_bus = (primary_bus_number!=0 ? primary_bus_number:secondary_bus_number);

            vector<HVDC*> hvdcs = psdb.get_hvdcs_connecting_to_bus(converter_bus);

            CONVERTER_SIDE converter_side_type;
            for(unsigned int j=0; j<hvdcs.size(); ++j)
            {
                HVDC* hvdc_ptr = hvdcs[j];
                if(hvdc_ptr->get_converter_bus(RECTIFIER)==converter_bus)
                    converter_side_type = RECTIFIER;
                else
                    converter_side_type = INVERTER;

                hvdc_ptr->set_converter_transformer_max_tap_in_pu(converter_side_type,max_tap_in_pu);
                hvdc_ptr->set_converter_transformer_min_tap_in_pu(converter_side_type,min_tap_in_pu);
                hvdc_ptr->set_converter_transformer_number_of_taps(converter_side_type,tap_number);
            }

            /*
            if(from_valve_side_bus_name_to_grid_side_bus_name[secondary_bus_name]==primary_bus_name)
            {
                CONVERTER_SIDE converter_side_type;

                string inverter_grid_side_bus_name;
                string rectifier_valve_side_bus_name;
                string rectifier_grid_side_bus_name;
                string inverter_valve_side_bus_name;

                if(not from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[secondary_bus_name].empty())
                {
                    converter_side_type=INVERTER;
                    inverter_grid_side_bus_name=primary_bus_name;
                    rectifier_valve_side_bus_name=from_inverter_valve_side_bus_name_to_rectifier_valve_side_bus_name[secondary_bus_name];
                    rectifier_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[rectifier_valve_side_bus_name];
                }
                else
                {
                    converter_side_type=RECTIFIER;
                    rectifier_grid_side_bus_name=primary_bus_name;
                    inverter_valve_side_bus_name=from_rectifier_valve_side_bus_name_to_inverter_valve_side_bus_name[secondary_bus_name];
                    inverter_grid_side_bus_name=from_valve_side_bus_name_to_grid_side_bus_name[inverter_valve_side_bus_name];
                }
                unsigned int rectifier_grid_side_bus = psdb.bus_name2bus_number(rectifier_grid_side_bus_name);
                unsigned int inverter_grid_side_bus = psdb.bus_name2bus_number(inverter_grid_side_bus_name);

                DEVICE_ID did;
                did.set_device_type(STEPS_HVDC);
                TERMINAL terminal;
                terminal.append_bus(rectifier_grid_side_bus);
                terminal.append_bus(inverter_grid_side_bus);
                did.set_device_terminal(terminal);

                HVDC* hvdc_ptr = psdb.get_hvdc(did);

                if(hvdc_ptr==NULL)
                    continue;

                hvdc_ptr->set_converter_transformer_max_tap_in_pu(converter_side_type,max_tap_in_pu);
                hvdc_ptr->set_converter_transformer_min_tap_in_pu(converter_side_type,min_tap_in_pu);
                hvdc_ptr->set_converter_transformer_number_of_taps(converter_side_type,tap_number);
            }
            */
        }
    }
}

void BPA_IMEXPORTER::export_powerflow_data(string file, bool export_zero_impedance_line, POWERFLOW_DATA_SAVE_MODE save_mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. BPA dat file "<<file<<" cannot be opened for exporting powerflow data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    set_export_zero_impedance_line_logic(export_zero_impedance_line);

    if(export_zero_impedance_line==false)
        psdb.update_overshadowed_bus_count();
    else
        psdb.set_all_buses_un_overshadowed();

    ofs<<". Here goes basic case data"<<endl;
    ofs<<export_case_data();

    ofs<<". Here goes bus data"<<endl;
    ofs<<export_bus_data();

    ofs<<". Here goes transmission line data"<<endl;
    ofs<<export_line_data();

    ofs<<". Here goes transformer data"<<endl;
    ofs<<export_transformer_data();

    ofs<<". Here goes hvdc data"<<endl;
    ofs<<export_hvdc_data();

    ofs.close();

    if(export_zero_impedance_line==false)
        psdb.set_all_buses_un_overshadowed();//recover
}


string BPA_IMEXPORTER::convert_data_into_bpa_format(string original_data, string format) const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    format = string2upper(format);

    if(format.size()<2 or format.at(0)!='A')
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable string data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return original_data;
    }

    bool is_valid_format = false;
    if(format.size()==2)
    {
        if(format.at(1)>'0' and format.at(1)<'9')
            is_valid_format = true;
    }
    else
    {
        if(format.size()==3)
        {
            if(format.at(1)>'1' and format.at(1)<'9' and
               format.at(2)>='0' and format.at(2)<'9')
                is_valid_format = true;
        }
    }
    if(not is_valid_format)
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable string data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return original_data;
    }

    unsigned int n = get_integer_data(format.substr(1,format.size()-1), "1");

    string data = trim_string(original_data);
    if(data.size()<=n)
    {
        string blank(n-data.size(), ' ');
        return data+blank;
    }
    else
    {
        osstream<<"Warning. Longer string ("<<data<<") than format ("<<format<<") is detected. Information will be lost since tail will be trimmed.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        data = data.substr(0, n);
        return data;
    }
}

string BPA_IMEXPORTER::convert_data_into_bpa_format(double original_data, string format) const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    format = string2upper(format);

    if(format.size()<4 or format.at(0)!='F' or format.find(".")==string::npos)
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable double data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return num2str(original_data);
    }
    bool is_valid_format = false;
    if(format.size()==4)
    {
        is_valid_format = (format.at(1)>='1' and format.at(1)<='9' and
                           format.at(2)=='.' and
                           format.at(3)>='0' and format.at(3)<='9');
    }
    else
    {
        if(format.size()==5)
            is_valid_format = (format.at(1)>='1' and format.at(1)<='9' and
                               format.at(2)>='0' and format.at(2)<='9' and
                               format.at(3)=='.' and
                               format.at(4)>='0' and format.at(4)<='9');
    }

    if(not is_valid_format)
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable double data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return num2str(original_data);
    }

    unsigned int n, m;
    if(format.size()==4)
    {
        n = get_integer_data(format.substr(1,1),"1");
        m = get_integer_data(format.substr(3,1),"0");
    }
    else
    {
        n = get_integer_data(format.substr(1,2),"1");
        m = get_integer_data(format.substr(4,1),"0");
    }

    osstream<<setprecision(n)<<fixed<<original_data;

    string data = osstream.str();
    data.erase(data.find_last_not_of("0")+1);

    double maxvalue_with_dot, minvalue_with_dot;
    maxvalue_with_dot = 1.0*pow(10.0,n-1)-1;
    minvalue_with_dot = -(1.0*pow(10.0,n-2)-1);

    if(original_data<=maxvalue_with_dot and original_data>=minvalue_with_dot)
    {
        if(data.size()>n)
            return data.substr(0,n);
        else
        {
            string blank(n-data.size(),' ');
            return blank+data;
        }
    }
    else
    {
        double maxvalue_without_dot, minvalue_without_dot;
        if(m==0)
        {
            maxvalue_without_dot = 1.0*pow(10.0,n)-1;
            minvalue_without_dot = -(1.0*pow(10.0,n-1)-1);
        }
        else
        {
            maxvalue_without_dot = (1.0*pow(10.0,n)-1)/(1.0*pow(10.0,m));
            minvalue_without_dot = -(1.0*pow(10.0,n-1)-1)/(1.0*pow(10.0,m));
        }

        if(original_data<=maxvalue_without_dot and original_data>=minvalue_without_dot)
        {
            size_t index = data.find(".");
            string integer = data.substr(0, index);
            string decimal = data.substr(index+1, data.size());

            if(integer.size()<n-m)
            {
                string blank(n-m-integer.size(), ' ');
                integer = blank + integer;
            }

            if(decimal.size()<m)
            {
                string blank(m-decimal.size(),' ');
                decimal += blank;
            }
            else
            {
                if(decimal.size()>m)
                    decimal = decimal.substr(0, m);
            }
            return integer+decimal;
        }
        else
        {
            osstream<<"Warning. Double data ("<<original_data<<") exceeds the limit of BPA format ("<<format<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
            if(original_data>0.0)
            {
                return convert_data_into_bpa_format(maxvalue_without_dot, format);
            }
            else
            {
                return convert_data_into_bpa_format(minvalue_without_dot, format);
            }
        }
    }
}

string BPA_IMEXPORTER::convert_data_into_bpa_format(unsigned int original_data, string format) const
{
    return convert_data_into_bpa_format(int(original_data), format);
}

string BPA_IMEXPORTER::convert_data_into_bpa_format(int original_data, string format) const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    format = string2upper(format);

    if(format.size()<2 or format.at(0)!='I')
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable integer or unsigned int data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return num2str(original_data);
    }

    bool is_valid_format = false;
    if(format.size()==2)
    {
        if(format.at(1)>'0' and format.at(1)<'9')
            is_valid_format = true;
    }
    else
    {
        if(format.size()==3)
        {
            if(format.at(1)>'1' and format.at(1)<'9' and
               format.at(2)>='0' and format.at(2)<'9')
                is_valid_format = true;
        }
    }
    if(not is_valid_format)
    {
        osstream<<"Warning. Invalid BPA format ("<<format<<") when formatting readable integer or unsigned int data to bpa data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return num2str(original_data);
    }

    unsigned int n = get_integer_data(format.substr(1, string::npos), "1");

    int maxvalue = int(round(1.0*pow(10.0,n)-1));
    int minvalue = -int(round(1.0*pow(10.0,n-1)-1));

    if(original_data>maxvalue or original_data<minvalue)
    {
        osstream<<"Warning. Integer of unsigned int data ("<<original_data<<") (n="<<n<<") exceeds the limit ("<<minvalue<<", "<<maxvalue<<") of BPA format ("<<format<<").";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return num2str(original_data);
    }

    string data = num2str(original_data);
    if(data.size()<n)
    {
        string blank(n-data.size(), ' ');
        data = blank+data;
    }
    return data;
}


string BPA_IMEXPORTER::export_case_data() const
{
    return "";
}

string BPA_IMEXPORTER::export_bus_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<BUS*> buses = psdb.get_all_buses();
    unsigned int n = buses.size();
    for(unsigned int i=0; i<n; ++i)
    {
        BUS* bus = buses[i];

        unsigned int bus_number = bus->get_bus_number();
        BUS_TYPE bus_type = bus->get_bus_type();
        unsigned int owner_number = bus->get_owner_number();
        string bus_name = bus->get_bus_name();
        double base_voltage = bus->get_base_voltage_in_kV();
        unsigned int zone_number = bus->get_zone_number();
        double vmax = bus->get_normal_voltage_upper_limit_in_pu();
        double vmin = bus->get_normal_voltage_lower_limit_in_pu();
        double slack_angle = bus->get_positive_sequence_angle_in_deg();

        osstream<<"."<<endl<<". Bus "<<bus_number<<": "<<bus_name<<endl;

        vector<LOAD*> loads = psdb.get_loads_connecting_to_bus(bus_number);

        double constant_power_load_P = 0.0, constant_power_load_Q = 0.0;
        for(unsigned int j=0; j<loads.size(); ++j)
        {
            LOAD* load = loads[j];
            if(load->get_status()==false)
                continue;

            constant_power_load_P += load->get_nominal_constant_power_load_in_MVA().real();
            constant_power_load_Q += load->get_nominal_constant_power_load_in_MVA().imag();
        }
        double constant_current_load_P = 0.0, constant_current_load_Q = 0.0;
        for(unsigned int j=0; j<loads.size(); ++j)
        {
            LOAD* load = loads[j];
            if(load->get_status()==false)
                continue;

            constant_current_load_P += load->get_nominal_constant_current_load_in_MVA().real();
            constant_current_load_Q -= load->get_nominal_constant_current_load_in_MVA().imag();
        }
        double constant_impedance_load_P = 0.0, constant_impedance_load_Q = 0.0;
        for(unsigned int j=0; j<loads.size(); ++j)
        {
            LOAD* load = loads[j];
            if(load->get_status()==false)
                continue;

            constant_impedance_load_P += load->get_nominal_constant_impedance_load_in_MVA().real();
            constant_impedance_load_Q += load->get_nominal_constant_impedance_load_in_MVA().imag();
        }

        vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(bus_number);
        double pmax = 0.0, pgen = 0.0, qgen = 0.0, qmax = 0.0, qmin = 0.0, v_schedule = 1.0;

        for(unsigned int j=0; j<sources.size(); ++j)
        {
            SOURCE* source = sources[j];
            if(source->get_status()==false)
                continue;

            pmax += source->get_p_max_in_MW();
            pgen += source->get_p_generation_in_MW();
            qgen += source->get_q_generation_in_MVar();
            qmax += source->get_q_max_in_MVar();
            qmin += source->get_q_min_in_MVar();
            v_schedule = source->get_voltage_to_regulate_in_pu();
        }

        osstream<<"B";
        switch(bus_type)
        {
            case PV_TYPE:
            case PV_TO_PQ_TYPE_1:
            case PV_TO_PQ_TYPE_2:
            case PV_TO_PQ_TYPE_3:
            case PV_TO_PQ_TYPE_4:
                osstream<<"G";
                break;
            case SLACK_TYPE:
                osstream<<"S";
                break;
            default:
                osstream<<" ";
                break;
        }
        osstream<<" ";

        string owner_name = "";
        OWNER* owner = psdb.get_owner(owner_number);
        if(owner!=NULL)
            owner_name = owner->get_owner_name();

        osstream<<convert_data_into_bpa_format(owner_name,"A3");
        osstream<<convert_data_into_bpa_format(bus_name,"A8");
        osstream<<convert_data_into_bpa_format(base_voltage,"F4.0");

        string zone_name = "";
        ZONE* zone = psdb.get_zone(zone_number);
        if(zone!=NULL)
            zone_name = zone->get_zone_name();

        osstream<<convert_data_into_bpa_format(zone_name, "A2");

        if(constant_power_load_P==0.0)
            osstream<<string(5,' ');
        else
            osstream<<convert_data_into_bpa_format(constant_power_load_P,"F5.0");

        if(constant_power_load_Q==0.0)
            osstream<<string(5,' ');
        else
            osstream<<convert_data_into_bpa_format(constant_power_load_Q,"F5.0");

        if(constant_impedance_load_P==0.0)
            osstream<<string(4,' ');
        else
            osstream<<convert_data_into_bpa_format(constant_impedance_load_P,"F4.0");

        if(constant_impedance_load_Q==0.0)
            osstream<<string(4,' ');
        else
            osstream<<convert_data_into_bpa_format(-constant_impedance_load_Q,"F4.0");

        if(pmax==0.0)
            osstream<<string(4,' ');
        else
            osstream<<convert_data_into_bpa_format(pmax,"F4.0");

        if(pgen==0.0)
            osstream<<string(5,' ');
        else
            osstream<<convert_data_into_bpa_format(pgen,"F5.0");

        if(bus_type == PQ_TYPE)
        {
            if(qgen==0.0)
                osstream<<string(5,' ');
            else
                osstream<<convert_data_into_bpa_format(qgen,"F5.0");
        }
        else
        {
            if(qmax==0.0)
                osstream<<string(5,' ');
            else
                osstream<<convert_data_into_bpa_format(qmax,"F5.0");
        }
        if(qmin==0.0)
            osstream<<string(5,' ');
        else
            osstream<<convert_data_into_bpa_format(qmin,"F5.0");

        if(bus_type==PQ_TYPE)
            osstream<<convert_data_into_bpa_format(vmax,"F4.3");
        else
            osstream<<convert_data_into_bpa_format(v_schedule,"F4.3");

        if(bus_type==SLACK_TYPE)
            osstream<<convert_data_into_bpa_format(slack_angle,"F4.1");
        else
            osstream<<convert_data_into_bpa_format(vmin,"F4.3");

        if(constant_current_load_P!=0 or constant_current_load_Q!=0)
        {
            osstream<<endl;
            osstream<<"+C ";
            osstream<<convert_data_into_bpa_format(owner_name,"A3");
            osstream<<convert_data_into_bpa_format(bus_name,"A8");
            osstream<<convert_data_into_bpa_format(base_voltage,"F4.0");
            osstream<<"*I";
            if(constant_current_load_P==0.0)
                osstream<<"     ";
            else
                osstream<<convert_data_into_bpa_format(constant_current_load_P,"F5.0");
            if(constant_current_load_Q==0.0)
                osstream<<"     ";
            else
                osstream<<convert_data_into_bpa_format(constant_current_load_Q,"F5.0");
        }
        osstream<<endl;
    }

    return osstream.str();
}


string BPA_IMEXPORTER::export_line_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<LINE*> lines = psdb.get_all_lines();
    unsigned int n = lines.size();

    double sbase_MVA = psdb.get_system_base_power_in_MVA();

    for(unsigned int i=0; i<n; ++i)
    {
        LINE* line = lines[i];

        unsigned int sending_side_bus_number = line->get_sending_side_bus();
        unsigned int receiving_side_bus_number = line->get_receiving_side_bus();
        unsigned int owner_number = line->get_owner_of_index(0);
        unsigned int meter_end_bus = line->get_meter_end_bus();
        string identifier = line->get_identifier();

        RATING rate = line->get_rating();
        double rate_MVA = rate.get_rating_A_MVA();

        double r = line->get_line_positive_sequence_z_in_pu().real();
        double x = line->get_line_positive_sequence_z_in_pu().imag();

        double g_half = 0.5*line->get_line_positive_sequence_y_in_pu().real();
        double b_half = 0.5*line->get_line_positive_sequence_y_in_pu().imag();

        double length = line->get_length();

        double y_shunt_sending = line->get_shunt_positive_sequence_y_at_sending_side_in_pu().imag();
        double y_shunt_receiving = line->get_shunt_positive_sequence_y_at_receiving_side_in_pu().imag();

        string owner_name = "";
        OWNER* owner = psdb.get_owner(owner_number);
        if(owner!=NULL)
            owner_name = owner->get_owner_name();

        BUS* bus = psdb.get_bus(sending_side_bus_number);
        if(bus==NULL)
            return "";

        string sending_side_bus_name = bus->get_bus_name();
        double sending_side_base_voltage = bus->get_base_voltage_in_kV();

        unsigned int meter_end = 1;
        if(meter_end_bus==receiving_side_bus_number)
            meter_end = 2;

        bus = psdb.get_bus(receiving_side_bus_number);
        if(bus==NULL)
            return "";

        string receiving_side_bus_name = bus->get_bus_name();
        double receiving_side_base_voltage = bus->get_base_voltage_in_kV();

        double rate_A = rate_MVA/(sending_side_base_voltage*SQRT3)*1000.0;

        osstream<<"."<<endl
          <<". Line "<<sending_side_bus_number<<"("<<sending_side_bus_name<<") to "
          <<receiving_side_bus_number<<"("<<receiving_side_bus_name<<")"<<endl;

        osstream<<"L  ";
        osstream<<convert_data_into_bpa_format(owner_name, "A3");
        osstream<<convert_data_into_bpa_format(sending_side_bus_name,"A8");
        osstream<<convert_data_into_bpa_format(sending_side_base_voltage,"F4.0");
        osstream<<convert_data_into_bpa_format(meter_end,"I1");
        osstream<<convert_data_into_bpa_format(receiving_side_bus_name,"A8");
        osstream<<convert_data_into_bpa_format(receiving_side_base_voltage,"F4.0");
        osstream<<convert_data_into_bpa_format(identifier,"A1");
        osstream<<convert_data_into_bpa_format(rate_A,"F4.0");
        osstream<<" ";
        osstream<<convert_data_into_bpa_format(r,"F6.5");
        osstream<<convert_data_into_bpa_format(x,"F6.5");
        osstream<<convert_data_into_bpa_format(g_half,"F6.5");
        osstream<<convert_data_into_bpa_format(b_half,"F6.5");
        osstream<<convert_data_into_bpa_format(length,"F4.1");

        if(y_shunt_sending!=0 or y_shunt_receiving!=0)
        {
            osstream<<endl;
            osstream<<"L+"<<string(4,' ');;
            osstream<<convert_data_into_bpa_format(sending_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(sending_side_base_voltage,"F4.0");
            osstream<<" ";
            osstream<<convert_data_into_bpa_format(receiving_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(receiving_side_base_voltage,"F4.0");
            osstream<<convert_data_into_bpa_format(identifier,"A1");
            osstream<<" ";
            if(y_shunt_sending==0)
                osstream<<string(5,' ');
            else
                osstream<<convert_data_into_bpa_format(-y_shunt_sending*sbase_MVA,"F5.0");
            osstream<<string(5,' ');
            if(y_shunt_receiving==0)
                osstream<<string(5,' ');
            else
                osstream<<convert_data_into_bpa_format(-y_shunt_receiving*sbase_MVA,"F5.0");
        }
        osstream<<endl;
    }

    return osstream.str();
}
string BPA_IMEXPORTER::export_transformer_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();

    unsigned int n;
    n = transformers.size();

    for(unsigned int i=0; i<n; ++i)
    {
        TRANSFORMER* transformer = transformers[i];
        if(transformer->is_two_winding_transformer())
            osstream<<export_two_winding_transformer(transformer);
        else if (transformer->is_three_winding_transformer())
            osstream<<export_three_winding_transformer(transformer);
    }
    return osstream.str();
}


string BPA_IMEXPORTER::export_two_winding_transformer(const TRANSFORMER* trans) const
{
    if(trans==NULL)
        return "";

    if(trans->is_three_winding_transformer())
        return export_three_winding_transformer(trans);

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int primary_bus = trans->get_winding_bus(PRIMARY_SIDE);
    unsigned int secondary_bus = trans->get_winding_bus(SECONDARY_SIDE);

    string primary_bus_name = psdb.bus_number2bus_name(primary_bus);
    string secondary_bus_name = psdb.bus_number2bus_name(secondary_bus);

    double primary_base_voltage = psdb.get_bus_base_voltage_in_kV(primary_bus);
    double secondary_base_voltage = psdb.get_bus_base_voltage_in_kV(secondary_bus);

    unsigned int owner = trans->get_owner_of_index(0);
    string owner_name = psdb.owner_number2owner_name(owner);

    unsigned int non_metered_bus = trans->get_non_metered_end_bus();
    unsigned int meter_flag = 0;
    if(non_metered_bus==primary_bus)
        meter_flag = 1;
    else
        meter_flag = 2;

    string identifier = trans->get_identifier();
    double capacity  = trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);

    complex<double> Z = trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
    complex<double> Y = trans->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu();

    double zbase = trans->get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)*trans->get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)/
                   trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);
    Z = Z*zbase;

    double sbase = psdb.get_system_base_power_in_MVA();
    zbase = primary_base_voltage*primary_base_voltage/sbase;

    Z /= zbase;

    double tap_primary = trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE);
    tap_primary *= trans->get_winding_nominal_voltage_in_kV(PRIMARY_SIDE);

    double tap_secondary = trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE);
    tap_secondary *= trans->get_winding_nominal_voltage_in_kV(SECONDARY_SIDE);

    double angle_shift = trans->get_winding_angle_shift_in_deg(PRIMARY_SIDE)-trans->get_winding_angle_shift_in_deg(SECONDARY_SIDE);

    bool is_angle_shift_transformer = false;
    switch(trans->get_winding_control_mode(PRIMARY_SIDE))
    {
        case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
        case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
            is_angle_shift_transformer = true;
            break;
        default:
            is_angle_shift_transformer = false;
    }

    ostringstream osstream;

    if(trans->get_winding_breaker_status(PRIMARY_SIDE) == false or
       trans->get_winding_breaker_status(SECONDARY_SIDE) == false)
        osstream<<".";


    if(is_angle_shift_transformer)
        osstream<<"TP";
    else
        osstream<<"T ";

    osstream<<" ";
    osstream<<convert_data_into_bpa_format(owner_name,"A3");
    osstream<<convert_data_into_bpa_format(primary_bus_name,"A8")
      <<convert_data_into_bpa_format(primary_base_voltage,"F4.0");
    osstream<<meter_flag;
    osstream<<convert_data_into_bpa_format(secondary_bus_name,"A8")
      <<convert_data_into_bpa_format(secondary_base_voltage,"F4.0");
    osstream<<convert_data_into_bpa_format(identifier,"A1");
    osstream<<" ";
    osstream<<convert_data_into_bpa_format(capacity, "F4.0");
    osstream<<" ";
    osstream<<convert_data_into_bpa_format(Z.real(), "F6.5")
      <<convert_data_into_bpa_format(Z.imag(), "F6.5");
    osstream<<convert_data_into_bpa_format(Y.real(), "F6.5")
      <<convert_data_into_bpa_format(Y.imag(), "F6.5");

    if(is_angle_shift_transformer)
        osstream<<convert_data_into_bpa_format(angle_shift, "F5.2");
    else
        osstream<<convert_data_into_bpa_format(tap_primary, "F5.2")
          <<convert_data_into_bpa_format(tap_secondary, "F5.2");
    osstream<<endl;

    if (trans->get_winding_control_mode(PRIMARY_SIDE)!=TRANSFORMER_TAP_NO_CONTROL||trans->get_winding_control_mode(SECONDARY_SIDE)!=TRANSFORMER_TAP_NO_CONTROL)
    {
        for (int i=0;i<2;++i)
        {
            TRANSFORMER_WINDING_SIDE winding=PRIMARY_SIDE;
            TRANSFORMER_WINDING_SIDE winding2=SECONDARY_SIDE;
            if(i==0)
            {
                winding=PRIMARY_SIDE;
                winding2=SECONDARY_SIDE;
            }
            if(i==1)
            {
                winding=SECONDARY_SIDE;
                winding2=PRIMARY_SIDE;
            }
        if(trans->get_winding_controlled_bus(winding)!=0)
        {

            TRANSFORMER_WINDING_CONTROL_MODE winding_control_mode=trans->get_winding_control_mode(winding);
            unsigned int winding_bus=trans->get_winding_bus(winding);
            string adjustable_winding_bus_name=psdb.bus_number2bus_name(winding_bus);
            double adjustable_winding_bus_base_voltage=psdb.get_bus_base_voltage_in_kV(winding_bus);
            double adjustable_winding_nominal_voltage=trans->get_winding_nominal_voltage_in_kV(winding);

            unsigned int winding2_bus=trans->get_winding_bus(winding2);
            string controlled_winding_bus_name=psdb.bus_number2bus_name(winding2_bus);
            double controlled_winding_bus_base_voltage=psdb.get_bus_base_voltage_in_kV(winding2_bus);

            unsigned int adjustable_flag=1;

            double winding_max_adjustable_voltage=trans->get_winding_controlled_max_voltage_in_pu( winding )*adjustable_winding_nominal_voltage;
            double winding_min_adjustable_voltage=trans->get_winding_controlled_min_voltage_in_pu( winding )*adjustable_winding_nominal_voltage;
            unsigned int number_of_taps=trans->get_winding_number_of_taps( winding );
            double controlled_max_active_power_into_winding_in_MW=trans->get_controlled_max_active_power_into_winding_in_MW(winding);
            double controlled_min_active_power_into_winding_in_MW=trans->get_controlled_min_active_power_into_winding_in_MW(winding);
            double controlled_max_reactive_power_into_winding_in_MVar=trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding);
            double controlled_min_reactive_power_into_winding_in_MVar=trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding);
            double winding_max_angle_shift_in_deg=trans->get_winding_max_angle_shift_in_deg(winding);
            double winding_min_angle_shift_in_deg=trans->get_winding_min_angle_shift_in_deg(winding);

            if(trans->get_winding_breaker_status(PRIMARY_SIDE) == false or
               trans->get_winding_breaker_status(SECONDARY_SIDE) == false)
                osstream<<".";

            if(trans->get_winding_control_mode(winding)==TRANSFORMER_TAP_VOLTAGE_CONTROL)
                osstream<<"R ";
            else if( trans->get_winding_control_mode(winding)==TRANSFORMER_TAP_REACTIVE_POWER_CONTROL )
                osstream<<"RN";
            else if( trans->get_winding_control_mode(winding)==TRANSFORMER_TAP_ACTIVE_POWER_CONTROL )
                osstream<<"RM";

            osstream<<" ";
            osstream<<convert_data_into_bpa_format(owner_name,"A3");
            osstream<<convert_data_into_bpa_format(adjustable_winding_bus_name,"A8")
              <<convert_data_into_bpa_format(adjustable_winding_bus_base_voltage,"F4.0");
            osstream<<convert_data_into_bpa_format(adjustable_flag,"I1");
            osstream<<convert_data_into_bpa_format(controlled_winding_bus_name,"A8")
              <<convert_data_into_bpa_format(controlled_winding_bus_base_voltage,"F4.0");
            osstream<<"  ";
            osstream<<convert_data_into_bpa_format(controlled_winding_bus_name,"A8")
              <<convert_data_into_bpa_format(controlled_winding_bus_base_voltage,"F4.0");

            if(winding_control_mode==TRANSFORMER_TAP_VOLTAGE_CONTROL || winding_control_mode == TRANSFORMER_TAP_REACTIVE_POWER_CONTROL )
            {
                osstream<<convert_data_into_bpa_format(winding_max_adjustable_voltage,"F5.2");
                osstream<<convert_data_into_bpa_format(winding_min_adjustable_voltage,"F5.2");
                osstream<<convert_data_into_bpa_format(number_of_taps,"I2");
            }

            if(winding_control_mode==TRANSFORMER_TAP_REACTIVE_POWER_CONTROL)
            {
                osstream<<convert_data_into_bpa_format(controlled_max_reactive_power_into_winding_in_MVar,"F5.0");
                osstream<<convert_data_into_bpa_format(controlled_min_reactive_power_into_winding_in_MVar,"F5.0");
            }

            if(winding_control_mode==TRANSFORMER_TAP_ACTIVE_POWER_CONTROL)
            {
                osstream<<convert_data_into_bpa_format(winding_max_angle_shift_in_deg,"F5.2");
                osstream<<convert_data_into_bpa_format(winding_min_angle_shift_in_deg,"F5.2");
                osstream<<convert_data_into_bpa_format(number_of_taps,"I2");
                osstream<<convert_data_into_bpa_format(controlled_max_active_power_into_winding_in_MW,"F5.0");
                osstream<<convert_data_into_bpa_format(controlled_min_active_power_into_winding_in_MW,"F5.0");
            }


            osstream<<endl;
        }
      }
    }
    return osstream.str();
}

string BPA_IMEXPORTER::export_three_winding_transformer(const TRANSFORMER* trans) const
{
    if(trans==NULL)
        return "";

    if(trans->is_two_winding_transformer())
        return export_two_winding_transformer(trans);

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string transformer_name=trans->get_name();
    string neutral_point_name="&" + transformer_name;
    double neutral_voltage_in_kv=1.0;
    unsigned int owner = trans->get_owner_of_index(0);
    string owner_name = psdb.owner_number2owner_name(owner);

    complex<double> Z_between_primary_and_secondary = trans->get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
    complex<double> Z_between_primary_and_tertiary = trans->get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);
    complex<double> Z_between_secondary_and_tertiary = trans->get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
    complex<double> Y = trans->get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu();

    //double sbase = psdb.get_system_base_power_in_MVA();
    complex<double> Z_primary=(Z_between_primary_and_secondary+Z_between_primary_and_tertiary-Z_between_secondary_and_tertiary)/2.0;
    complex<double> Z_secondary=(Z_between_primary_and_secondary+Z_between_secondary_and_tertiary-Z_between_primary_and_tertiary)/2.0;
    complex<double> Z_tertiary=(Z_between_primary_and_tertiary+Z_between_secondary_and_tertiary-Z_between_primary_and_secondary)/2.0;

    double angle_shift_bwtween_primary_and_secondary = trans->get_winding_angle_shift_in_deg(PRIMARY_SIDE)-trans->get_winding_angle_shift_in_deg(SECONDARY_SIDE);
    double angle_shift_bwtween_primary_and_tertiary = trans->get_winding_angle_shift_in_deg(PRIMARY_SIDE)-trans->get_winding_angle_shift_in_deg(TERTIARY_SIDE);

    TRANSFORMER_WINDING_SIDE winding=PRIMARY_SIDE;
    ostringstream osstream;

    for(unsigned int i=0; i<3; ++i)
    {
        if(i==0) winding=PRIMARY_SIDE;
        if(i==1) winding=SECONDARY_SIDE;
        if(i==2) winding=TERTIARY_SIDE;

        if(trans->get_winding_breaker_status(winding) == false)
            osstream<<".";

        bool is_angle_shift_transformer = false;

        switch(trans->get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                is_angle_shift_transformer = true;
                break;
            default:
                is_angle_shift_transformer = false;
        }

        unsigned int winding_bus = trans->get_winding_bus(winding);

        string winding_bus_name = psdb.bus_number2bus_name(winding_bus);

        double winding_base_voltage = psdb.get_bus_base_voltage_in_kV(winding_bus);

        unsigned int non_metered_bus = trans->get_non_metered_end_bus();

        unsigned int meter_flag;
        if(non_metered_bus!=winding_bus)
            meter_flag = 1;
        else
            meter_flag = 2;

        string identifier = trans->get_identifier();

        TRANSFORMER_WINDING_SIDE winding2=SECONDARY_SIDE;
        if(i==0) winding2 = SECONDARY_SIDE;
        if(i==1) winding2 = TERTIARY_SIDE;
        if(i==2) winding2 = PRIMARY_SIDE;

        double winding_capacity   = trans->get_winding_nominal_capacity_in_MVA(winding, winding2);

        double tap_winding = trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding);
        tap_winding *= trans->get_winding_nominal_voltage_in_kV(winding);

        complex<double> Z_winding_based_on_winding_base_voltage;
        if(i==0)  Z_winding_based_on_winding_base_voltage=Z_primary*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage)*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage);
        if(i==1)  Z_winding_based_on_winding_base_voltage=Z_secondary*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage)*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage);
        if(i==2)  Z_winding_based_on_winding_base_voltage=Z_tertiary*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage)*(trans->get_winding_nominal_voltage_in_kV(winding)/winding_base_voltage);

        if(is_angle_shift_transformer)
            osstream<<"TP";
        else
            osstream<<"T ";

        osstream<<" ";
        osstream<<convert_data_into_bpa_format(owner_name,"A3");
        osstream<<convert_data_into_bpa_format(winding_bus_name,"A8")
          <<convert_data_into_bpa_format(winding_base_voltage,"F4.0");
        osstream<<meter_flag;
        osstream<<convert_data_into_bpa_format(neutral_point_name,"A8")
          <<convert_data_into_bpa_format(neutral_voltage_in_kv,"F4.0");
        osstream<<convert_data_into_bpa_format(identifier,"A1");
        osstream<<" ";
        osstream<<convert_data_into_bpa_format(winding_capacity, "F4.0");
        osstream<<" ";
        osstream<<convert_data_into_bpa_format(Z_winding_based_on_winding_base_voltage.real(), "F6.5")
          <<convert_data_into_bpa_format(Z_winding_based_on_winding_base_voltage.imag(), "F6.5");

        if (i==0)
        {   osstream<<convert_data_into_bpa_format(Y.real(), "F6.5")
            <<convert_data_into_bpa_format(Y.imag(), "F6.5");
        }
        else
        {
            osstream<<convert_data_into_bpa_format(0.0, "F6.5")
            <<convert_data_into_bpa_format(0.0, "F6.5");
        }

        if(is_angle_shift_transformer)
        {
            if (i==0) osstream<<convert_data_into_bpa_format(0.0, "F5.2");
            if (i==1) osstream<<convert_data_into_bpa_format(-1*angle_shift_bwtween_primary_and_secondary, "F5.2");
            if (i==2) osstream<<convert_data_into_bpa_format(-1*angle_shift_bwtween_primary_and_tertiary, "F5.2");
        }
        else
            osstream<<convert_data_into_bpa_format(tap_winding, "F5.2")
              <<convert_data_into_bpa_format(1.0, "F5.2");
        osstream<<endl;

        // now exporting tap
        TRANSFORMER_WINDING_CONTROL_MODE winding_control_mode = trans->get_winding_control_mode(winding);
        if (winding_control_mode==TRANSFORMER_TAP_NO_CONTROL)
            continue;

        unsigned int number_of_taps=trans->get_winding_number_of_taps(winding);
        // control voltage
        unsigned int controlled_bus=trans->get_winding_controlled_bus(winding);
        string controlled_bus_name = psdb.bus_number2bus_name(controlled_bus);
        double controlled_bus_base_voltage = psdb.get_bus_base_voltage_in_kV(controlled_bus);

        double winding_max_tap = trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)*winding_base_voltage;
        double winding_min_tap = trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)*winding_base_voltage;

        // control power
        double controlled_max_active_power_into_winding_in_MW = trans->get_controlled_max_active_power_into_winding_in_MW(winding);
        double controlled_min_active_power_into_winding_in_MW = trans->get_controlled_min_active_power_into_winding_in_MW(winding);
        double controlled_max_reactive_power_into_winding_in_MVar = trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding);
        double controlled_min_reactive_power_into_winding_in_MVar = trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding);
        double winding_max_angle_shift_in_deg = trans->get_winding_max_angle_shift_in_deg(winding);
        double winding_min_angle_shift_in_deg = trans->get_winding_min_angle_shift_in_deg(winding);


        if(trans->get_winding_breaker_status(winding) == false)
            osstream<<".";

        if(winding_control_mode==TRANSFORMER_TAP_VOLTAGE_CONTROL)
            osstream<<"R ";
        else if(winding_control_mode==TRANSFORMER_TAP_REACTIVE_POWER_CONTROL )
            osstream<<"RN";
        else if(winding_control_mode==TRANSFORMER_TAP_ACTIVE_POWER_CONTROL )
            osstream<<"RM";

        osstream<<" ";
        osstream<<convert_data_into_bpa_format(owner_name,"A3");
        osstream<<convert_data_into_bpa_format(winding_bus_name,"A8")
          <<convert_data_into_bpa_format(winding_base_voltage,"F4.0");
        osstream<<convert_data_into_bpa_format(1,"I1");
        osstream<<convert_data_into_bpa_format(neutral_point_name,"A8")
          <<convert_data_into_bpa_format(neutral_voltage_in_kv,"F4.0");
        osstream<<"  ";
        if(winding_control_mode==TRANSFORMER_TAP_VOLTAGE_CONTROL)
            osstream<<convert_data_into_bpa_format(controlled_bus_name,"A8")
              <<convert_data_into_bpa_format(controlled_bus_base_voltage,"F4.0");
        else
            osstream<<convert_data_into_bpa_format(winding_bus_name,"A8")
              <<convert_data_into_bpa_format(winding_base_voltage,"F4.0");


        if(winding_control_mode==TRANSFORMER_TAP_VOLTAGE_CONTROL || winding_control_mode == TRANSFORMER_TAP_REACTIVE_POWER_CONTROL )
        {
            osstream<<convert_data_into_bpa_format(winding_max_tap,"F5.2");
            osstream<<convert_data_into_bpa_format(winding_min_tap,"F5.2");
        }
        else
        {
            osstream<<convert_data_into_bpa_format(winding_max_angle_shift_in_deg,"F5.2");
            osstream<<convert_data_into_bpa_format(winding_min_angle_shift_in_deg,"F5.2");
        }
        osstream<<convert_data_into_bpa_format(number_of_taps,"I2");

        if(winding_control_mode==TRANSFORMER_TAP_REACTIVE_POWER_CONTROL)
        {
            osstream<<convert_data_into_bpa_format(controlled_max_reactive_power_into_winding_in_MVar,"F5.0");
            osstream<<convert_data_into_bpa_format(controlled_min_reactive_power_into_winding_in_MVar,"F5.0");
        }
        else
        {
            if(winding_control_mode==TRANSFORMER_TAP_ACTIVE_POWER_CONTROL)
            {
                osstream<<convert_data_into_bpa_format(controlled_max_active_power_into_winding_in_MW,"F5.0");
                osstream<<convert_data_into_bpa_format(controlled_min_active_power_into_winding_in_MW,"F5.0");
            }
        }
        osstream<<endl;
    }
    return osstream.str();
}

string BPA_IMEXPORTER::export_hvdc_data() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    unsigned int n = hvdcs.size();

    double sbase_MVA = psdb.get_system_base_power_in_MVA();

    for(unsigned int i=0; i<n; ++i)
    {
        HVDC* hvdc = hvdcs[i];

        string hvdc_name = hvdc->get_name();

        unsigned int rectifier_grid_bus_number=hvdc->get_converter_bus(RECTIFIER);
        unsigned int inverter_grid_bus_number=hvdc->get_converter_bus(INVERTER);

        BUS* rectifier_grid_bus = psdb.get_bus(rectifier_grid_bus_number);
        BUS* inverter_grid_bus = psdb.get_bus(inverter_grid_bus_number);

        if(rectifier_grid_bus==NULL||inverter_grid_bus==NULL)
            continue;

        string rectifier_grid_bus_name = rectifier_grid_bus->get_bus_name();
        string inverter_grid_bus_name = inverter_grid_bus->get_bus_name();
        //double rectifier_grid_bus_base_voltage=rectifier_grid_bus->get_base_voltage_in_kV();
        //double inverter_grid_bus_base_voltage=inverter_grid_bus->get_base_voltage_in_kV();

        unsigned int pole_number = 1;

        double line_resistance_in_ohm=hvdc->get_line_resistance_in_ohm();
        double line_inductance_in_mH=hvdc->get_line_inductance_in_mH();
        double line_capacitance_in_uF=hvdc->get_line_capacitance_in_uF();
        double line_rectifier_smooting_inductance_in_mH=hvdc->get_line_smooting_inductance_in_mH(RECTIFIER);
        double line_inverter_smooting_inductance_in_mH=hvdc->get_line_smooting_inductance_in_mH(INVERTER);
        double nominal_dc_power_per_pole_in_MW=hvdc->get_nominal_dc_power_in_MW();

        CONVERTER_SIDE power_controlled_side=hvdc->get_side_to_hold_dc_power();

        double nominal_dc_current_per_pole_in_A=hvdc->get_nominal_dc_current_in_kA()*1000;
        double nominal_dc_voltage_per_pole_in_kV=hvdc->get_nominal_dc_voltage_in_kV();

        //double rectifier_number_of_bridge=hvdc->get_converter_number_of_bridge(RECTIFIER);
        //double inverter_number_of_bridge=hvdc->get_converter_number_of_bridge(INVERTER);

        double rectifier_max_alpha=hvdc->get_converter_max_alpha_or_gamma_in_deg(RECTIFIER);
        double inverter_max_alpha=hvdc->get_converter_max_alpha_or_gamma_in_deg(INVERTER);
        double rectifier_min_alpha=hvdc->get_converter_min_alpha_or_gamma_in_deg(RECTIFIER);
        double inverter_min_alpha=hvdc->get_converter_min_alpha_or_gamma_in_deg(INVERTER);

        double rectifier_grid_side_bus_base_voltage_in_kV=hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER);
        double inverter_grid_side_bus_base_voltage_in_kV=hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER);

        double rectifier_valve_side_bus_base_voltage_in_kV=hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER);
        double inverter_valve_side_bus_base_voltage_in_kV=hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

        double rectifier_z_base_based_on_grid_side_bus_nominal=rectifier_grid_side_bus_base_voltage_in_kV*rectifier_grid_side_bus_base_voltage_in_kV/sbase_MVA;
        double rectifier_y_base_based_on_grid_side_bus_nominal=1/rectifier_z_base_based_on_grid_side_bus_nominal;

        double inverter_z_base_based_on_grid_side_bus_nominal=inverter_grid_side_bus_base_voltage_in_kV*inverter_grid_side_bus_base_voltage_in_kV/sbase_MVA;
        double inverter_y_base_based_on_grid_side_bus_nominal=1/inverter_z_base_based_on_grid_side_bus_nominal;


        //get r,x,g,s pu value of inverter side
        double inverter_r_in_ohm=hvdc->get_converter_transformer_impedance_in_ohm(INVERTER).real();
        double inverter_r_in_pu=inverter_r_in_ohm/inverter_z_base_based_on_grid_side_bus_nominal;

        double inverter_x_in_ohm=hvdc->get_converter_transformer_impedance_in_ohm(INVERTER).imag();
        double inverter_x_in_pu=inverter_x_in_ohm/inverter_z_base_based_on_grid_side_bus_nominal;

        double inverter_g_in_siemens=hvdc->get_converter_transformer_admittance_in_siemens(INVERTER).real();
        double inverter_g_in_pu=inverter_g_in_siemens/inverter_y_base_based_on_grid_side_bus_nominal;

        double inverter_s_in_siemens=hvdc->get_converter_transformer_admittance_in_siemens(INVERTER).imag();
        double inverter_s_in_pu=inverter_s_in_siemens/inverter_y_base_based_on_grid_side_bus_nominal;


        //get r,x,g,s pu value of rectifier side
        double rectifier_r_in_ohm=hvdc->get_converter_transformer_impedance_in_ohm(RECTIFIER).real();
        double rectifier_r_in_pu=rectifier_r_in_ohm/rectifier_z_base_based_on_grid_side_bus_nominal;

        double rectifier_x_in_ohm=hvdc->get_converter_transformer_impedance_in_ohm(RECTIFIER).imag();
        double rectifier_x_in_pu=rectifier_x_in_ohm/rectifier_z_base_based_on_grid_side_bus_nominal;

        double rectifier_g_in_siemens=hvdc->get_converter_transformer_admittance_in_siemens(RECTIFIER).real();
        double rectifier_g_in_pu=rectifier_g_in_siemens/rectifier_y_base_based_on_grid_side_bus_nominal;

        //double rectifier_s_in_siemens=hvdc->get_converter_transformer_admittance_in_siemens(RECTIFIER).imag();
        double rectifier_s_in_pu=inverter_s_in_siemens/inverter_y_base_based_on_grid_side_bus_nominal;

        unsigned int inverter_side_number_of_bridge=hvdc->get_converter_number_of_bridge(INVERTER);
        unsigned int rectifier_side_number_of_bridge=hvdc->get_converter_number_of_bridge(RECTIFIER);

        double inverter_transformer_max_tap_in_pu=hvdc->get_converter_transformer_max_tap_in_pu(INVERTER);
        double inverter_transformer_min_tap_in_pu=hvdc->get_converter_transformer_min_tap_in_pu(INVERTER);
        unsigned int inverter_transformer_number_of_taps=hvdc->get_converter_transformer_number_of_taps(INVERTER);
        double inverter_transformer_max_tap_in_kv=inverter_transformer_max_tap_in_pu*inverter_grid_side_bus_base_voltage_in_kV;
        double inverter_transformer_min_tap_in_kv=inverter_transformer_min_tap_in_pu*inverter_grid_side_bus_base_voltage_in_kV;

        //double rectifier_transformer_max_tap_in_pu=hvdc->get_converter_transformer_max_tap_in_pu(RECTIFIER);
        //double rectifier_transformer_min_tap_in_pu=hvdc->get_converter_transformer_min_tap_in_pu(RECTIFIER);
        unsigned int rectifier_transformer_number_of_taps=hvdc->get_converter_transformer_number_of_taps(RECTIFIER);
        double rectifier_transformer_max_tap_in_kv=inverter_transformer_max_tap_in_pu*inverter_grid_side_bus_base_voltage_in_kV;
        double rectifier_transformer_min_tap_in_kv=inverter_transformer_min_tap_in_pu*inverter_grid_side_bus_base_voltage_in_kV;

        //imexporter inverter data
        for (unsigned int j=0; j!=pole_number; ++j)
        {
            string rectifier_valve_bus_name;
            string inverter_valve_bus_name;

            if (j==0)
            {
                rectifier_valve_bus_name=rectifier_grid_bus_name;
                inverter_valve_bus_name=inverter_grid_bus_name;
            }
            if (j==1)
            {
                rectifier_valve_bus_name=rectifier_grid_bus_name;
                inverter_valve_bus_name=inverter_grid_bus_name;
            }
            osstream<<".HVDC: "<<hvdc_name<<endl;
            osstream<<"LD";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(rectifier_valve_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_base_voltage_in_kV,"F4.0");

            osstream<<" ";

            osstream<<convert_data_into_bpa_format(inverter_valve_bus_name,"A8")
              <<convert_data_into_bpa_format(inverter_valve_side_bus_base_voltage_in_kV,"F4.0");

            osstream<<"  ";

            osstream<<convert_data_into_bpa_format(nominal_dc_current_per_pole_in_A,"F4.0");
            osstream<<convert_data_into_bpa_format(line_resistance_in_ohm,"F6.2");
            osstream<<convert_data_into_bpa_format(line_inductance_in_mH,"F6.2");
            osstream<<convert_data_into_bpa_format(line_capacitance_in_uF,"F6.2");

            if(power_controlled_side==RECTIFIER) osstream<<convert_data_into_bpa_format("R","A1");
            if(power_controlled_side==INVERTER) osstream<<convert_data_into_bpa_format("I","A1");

            osstream<<convert_data_into_bpa_format(nominal_dc_power_per_pole_in_MW,"F5.1");
            osstream<<convert_data_into_bpa_format(nominal_dc_voltage_per_pole_in_kV,"F5.1");
            osstream<<endl;
        }

        for (unsigned int j=0; j!=pole_number; ++j)
        {
            string inverter_valve_side_bus_name;

            if (j==0)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }
            if (j==1)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }
            osstream<<"BD";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(inverter_side_number_of_bridge,"I2");
            osstream<<convert_data_into_bpa_format(line_inverter_smooting_inductance_in_mH,"F5.1");
            osstream<<convert_data_into_bpa_format(inverter_min_alpha,"F5.1");
            osstream<<convert_data_into_bpa_format(inverter_max_alpha,"F5.1");
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(nominal_dc_current_per_pole_in_A,"F5.1");
            osstream<<convert_data_into_bpa_format(inverter_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<endl;
        }

        for (unsigned int j=0; j != pole_number; ++j)
        {
            string inverter_valve_side_bus_name;

            if (j==0)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }
            if (j==1)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }

            osstream<<"T ";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(inverter_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(inverter_r_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(inverter_x_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(inverter_g_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(inverter_s_in_pu,"F6.5");
            osstream<<endl;
        }

        for (unsigned int j=0; j!=pole_number; ++j)
        {
            string inverter_valve_side_bus_name;

            if (j==0)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }
            if (j==1)
            {
                inverter_valve_side_bus_name=inverter_grid_bus_name;
            }
            osstream<<"R ";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(inverter_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(inverter_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<string(14,' ');
            osstream<<convert_data_into_bpa_format(inverter_transformer_max_tap_in_kv,"F5.2");
            osstream<<convert_data_into_bpa_format(inverter_transformer_min_tap_in_kv,"F5.2");
            osstream<<convert_data_into_bpa_format(inverter_transformer_number_of_taps,"I2");
            osstream<<endl;
        }

       //imeporter rectifier data
        for (unsigned int j=0; j!=pole_number; ++j)
        {
            string rectifier_valve_side_bus_name;

            if (j==0)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }
            if (j==1)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }
            osstream<<"BD";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(rectifier_side_number_of_bridge,"I2");
            osstream<<convert_data_into_bpa_format(line_rectifier_smooting_inductance_in_mH,"F5.1");
            osstream<<convert_data_into_bpa_format(rectifier_min_alpha,"F5.1");
            osstream<<convert_data_into_bpa_format(rectifier_max_alpha,"F5.1");
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(nominal_dc_current_per_pole_in_A,"F5.1");
            osstream<<convert_data_into_bpa_format(rectifier_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<endl;
        }

        for (unsigned int j=0; j<pole_number; ++j)
        {
            string rectifier_valve_side_bus_name;

            if (j==0)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }
            if (j==1)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }

            osstream<<"T ";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(rectifier_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<string(5,' ');
            osstream<<convert_data_into_bpa_format(rectifier_r_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(rectifier_x_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(rectifier_g_in_pu,"F6.5");
            osstream<<convert_data_into_bpa_format(rectifier_s_in_pu,"F6.5");
            osstream<<endl;
        }

        for (unsigned int j=0; j!=pole_number; ++j)
        {
            string rectifier_valve_side_bus_name;

            if (j==0)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }
            if (j==1)
            {
                rectifier_valve_side_bus_name=rectifier_grid_bus_name;
            }
            osstream<<"R ";
            osstream<<string(4,' ');
            osstream<<convert_data_into_bpa_format(rectifier_grid_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_grid_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<" ";
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_name,"A8");
            osstream<<convert_data_into_bpa_format(rectifier_valve_side_bus_base_voltage_in_kV,"F4.0");
            osstream<<string(14,' ');
            osstream<<convert_data_into_bpa_format(rectifier_transformer_max_tap_in_kv,"F5.2");
            osstream<<convert_data_into_bpa_format(rectifier_transformer_min_tap_in_kv,"F5.2");
            osstream<<convert_data_into_bpa_format(rectifier_transformer_number_of_taps,"I2");
            osstream<<endl;
        }
    }
    return osstream.str();
}


string BPA_IMEXPORTER::export_vsc_hvdc_data() const
{
    return "";
}

string BPA_IMEXPORTER::export_multi_terminal_hvdc_data() const
{
    return "";
}

string BPA_IMEXPORTER::export_switched_shunt_data() const
{
    return "";
}

void BPA_IMEXPORTER::export_sequence_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}

