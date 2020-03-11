#include "header/data_imexporter/data_imexporter.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

using namespace std;

DATA_IMEXPORTER::DATA_IMEXPORTER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);

    set_base_frequency_in_Hz(50.0);
    set_export_zero_impedance_line_logic(true);
    set_export_zero_impedance_line_logic(true);
    set_powerflow_data_save_mode(SAVE_TO_KEEP_ORIGINAL_BUS_ORDER);
}

DATA_IMEXPORTER::~DATA_IMEXPORTER()
{
    toolkit = nullptr;
}

STEPS& DATA_IMEXPORTER::get_toolkit() const
{
    return *toolkit;
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
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. Shadowed bus pair file "<<file<<" cannot be opened for exporting shadowed bus data.";
        toolkit->show_information_with_leading_time_stamp(osstream);
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

void DATA_IMEXPORTER::setup_ordered_buses_with_powerflow_data_save_mode(POWERFLOW_DATA_SAVE_MODE mode)
{
    switch(mode)
    {
        case SAVE_TO_ORDER_BUS_WITH_BUS_NUMBER:
        {
            setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NUMBER();
            break;
        }
        case SAVE_TO_ORDER_BUS_WITH_BUS_NAME:
        {
            setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NAME();
            break;
        }
        case SAVE_TO_ORDER_BUS_WITH_DYNAMIC_DEVICE_ORDER:
        {
            setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_DYNAMIC_DEVICE_ORDER();
            break;
        }
        case SAVE_TO_KEEP_ORIGINAL_BUS_ORDER:
        default:
        {
            setup_ordered_buses_with_mode_SAVE_TO_KEEP_ORIGINAL_BUS_ORDER();
            break;
        }
    }
}

const vector<unsigned int>& DATA_IMEXPORTER::get_ordered_buses() const
{
    return ordered_buses_to_export;
}

void DATA_IMEXPORTER::setup_ordered_buses_with_mode_SAVE_TO_KEEP_ORIGINAL_BUS_ORDER()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ordered_buses_to_export = psdb.get_all_buses_number();
}

void DATA_IMEXPORTER::setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NUMBER()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ordered_buses_to_export = psdb.get_all_buses_number();
    unsigned int n = ordered_buses_to_export.size();

    bool swap_found = false;
    for(unsigned int i=0; i<n-1; ++i)
    {
        swap_found = false;
        for(unsigned int j=1; j<n; ++j)
        {
            unsigned int previous_bus = ordered_buses_to_export[j-1];
            unsigned int next_bus = ordered_buses_to_export[j];
            if(next_bus<previous_bus)
            {
                ordered_buses_to_export[j] = previous_bus;
                ordered_buses_to_export[j-1] = next_bus;
                swap_found = true;
            }
        }
        if(swap_found==false)
            break;
    }
}

void DATA_IMEXPORTER::setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NAME()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ordered_buses_to_export = psdb.get_all_buses_number();
    unsigned int n = ordered_buses_to_export.size();

    bool swap_found = false;
    for(unsigned int i=0; i<n-1; ++i)
    {
        swap_found = false;
        for(unsigned int j=1; j<n; ++j)
        {
            unsigned int previous_bus = ordered_buses_to_export[j-1];
            unsigned int next_bus = ordered_buses_to_export[j];
            if(psdb.bus_number2bus_name(next_bus) < psdb.bus_number2bus_name(previous_bus) )
            {
                ordered_buses_to_export[j] = previous_bus;
                ordered_buses_to_export[j-1] = next_bus;
                swap_found = true;
            }
        }
        if(swap_found==false)
            break;
    }
}

void DATA_IMEXPORTER::setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_DYNAMIC_DEVICE_ORDER()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    unsigned int n = psdb.get_bus_count();
    ordered_buses_to_export.reserve(n);
    ordered_buses_to_export.clear();

    set<unsigned int> existing_buses;
    existing_buses.clear();

    vector<unsigned int> buses;

    buses = get_all_generator_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_wt_generator_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_pv_unit_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_energy_storage_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_load_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_hvdc_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = get_all_fixed_shunt_buses();
    append_buses_to_ordered_buses(buses, existing_buses);

    buses = psdb.get_all_buses_number();
    append_buses_to_ordered_buses(buses, existing_buses);
}

vector<unsigned int> DATA_IMEXPORTER::get_all_generator_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<GENERATOR*> devices = psdb.get_all_generators();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        GENERATOR* dvc = devices[i];
        buses.push_back(dvc->get_generator_bus());
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_wt_generator_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<WT_GENERATOR*> devices = psdb.get_all_wt_generators();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        WT_GENERATOR* dvc = devices[i];
        buses.push_back(dvc->get_generator_bus());
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_pv_unit_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<PV_UNIT*> devices = psdb.get_all_pv_units();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        PV_UNIT* dvc = devices[i];
        buses.push_back(dvc->get_unit_bus());
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_energy_storage_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<ENERGY_STORAGE*> devices = psdb.get_all_energy_storages();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        ENERGY_STORAGE* dvc = devices[i];
        buses.push_back(dvc->get_energy_storage_bus());
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_load_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<LOAD*> devices = psdb.get_all_loads();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        LOAD* dvc = devices[i];
        buses.push_back(dvc->get_load_bus());
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_hvdc_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<HVDC*> devices = psdb.get_all_hvdcs();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        HVDC* dvc = devices[i];
        buses.push_back(dvc->get_converter_bus(RECTIFIER));
        buses.push_back(dvc->get_converter_bus(INVERTER));
    }
    return buses;
}

vector<unsigned int> DATA_IMEXPORTER::get_all_fixed_shunt_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<unsigned int> buses;
    vector<FIXED_SHUNT*> devices = psdb.get_all_fixed_shunts();
    unsigned int n = devices.size();
    buses.reserve(n);
    buses.clear();

    for(unsigned int i=0; i<n; ++i)
    {
        FIXED_SHUNT* dvc = devices[i];
        buses.push_back(dvc->get_shunt_bus());
    }
    return buses;
}

void DATA_IMEXPORTER::append_buses_to_ordered_buses(const vector<unsigned int>& buses, set<unsigned int>& existing_buses)
{
    unsigned int n = buses.size();
    for(unsigned int i=0; i<n; ++i)
    {
        unsigned int bus = buses[i];

        set<unsigned int>::iterator iter = existing_buses.find(bus);
        if(iter == existing_buses.end())
        {
            ordered_buses_to_export.push_back(bus);
            existing_buses.insert(bus);
        }
        else
            continue;
    }
}
