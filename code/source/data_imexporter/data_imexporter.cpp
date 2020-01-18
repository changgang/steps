#include "header/data_imexporter/data_imexporter.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"


DATA_IMEXPORTER::DATA_IMEXPORTER()
{
    set_base_frequency_in_Hz(50.0);
    set_export_zero_impedance_line_logic(true);
    set_export_zero_impedance_line_logic(true);
    set_powerflow_data_save_mode(KEEP_POWERFLOW_DATA);
}

DATA_IMEXPORTER::~DATA_IMEXPORTER()
{
    ;
}

void DATA_IMEXPORTER::set_base_frequency_in_Hz(double fbase)
{
    base_frequency_in_Hz = fbase;
}

double DATA_IMEXPORTER::get_base_frequency_in_Hz() const
{
    return base_frequency_in_Hz;
}


void DATA_IMEXPORTER::set_export_zero_impedance_line_logic(bool logic)
{
    export_zero_impedance_line = logic;
}

void DATA_IMEXPORTER::set_powerflow_data_save_mode(POWERFLOW_DATA_SAVE_MODE mode)
{
    powerflow_data_save_mode = mode;
}

bool DATA_IMEXPORTER::get_export_zero_impedance_line_logic() const
{
    return export_zero_impedance_line;
}

void DATA_IMEXPORTER::set_export_out_of_service_bus_logic(bool logic)
{
    export_out_of_service_bus = logic;
}

bool DATA_IMEXPORTER::get_export_out_of_service_bus_logic() const
{
    return export_out_of_service_bus;
}

POWERFLOW_DATA_SAVE_MODE DATA_IMEXPORTER::get_powerflow_data_save_mode() const
{
    return powerflow_data_save_mode;
}

void DATA_IMEXPORTER::export_shadowed_bus_pair(string file) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. Shadowed bus pair file "<<file<<" cannot be opened for exporting shadowed bus data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    map<unsigned int, vector<unsigned int> > bus_pairs;

    bus_pairs.clear();

    vector<BUS*> buses = psdb.get_all_buses();
    unsigned int n = buses.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        BUS* bus = buses[i];
        unsigned int bus_number = bus->get_bus_number();
        unsigned int equiv_bus_number = bus->get_equivalent_bus_number();
        if(equiv_bus_number!=0 and bus_number!=equiv_bus_number)
        {
            map<unsigned int, vector<unsigned int> >::iterator iter = bus_pairs.begin();
            iter = bus_pairs.find(equiv_bus_number);
            if(iter!=bus_pairs.end())
            {
                (iter->second).push_back(bus_number);
            }
            else
            {
                vector<unsigned int> new_pair;
                new_pair.push_back(bus_number);
                bus_pairs.insert(pair<unsigned int, vector<unsigned int> >(equiv_bus_number, new_pair));
            }
        }
    }
    for(map<unsigned int, vector<unsigned int> >::const_iterator iter = bus_pairs.begin(); iter!=bus_pairs.end(); ++iter)
    {
        unsigned int equiv_bus_number = iter->first;
        vector<unsigned int> bus_pair = iter->second;
        unsigned int n = bus_pair.size();
        if(n==0)
            continue;
        ostringstream oss;
        oss<<equiv_bus_number<<":";
        for(unsigned int i=0; i!=(n-1); ++i)
            oss<<bus_pair[i]<<",";
        oss<<bus_pair[n-1]<<"\n";
        ofs<<oss.str();
    }
    ofs.close();
}


bool DATA_IMEXPORTER::is_valid() const
{
    return true;
}

void DATA_IMEXPORTER::check()
{
    ;
}

void DATA_IMEXPORTER::clear()
{
    ;
}
