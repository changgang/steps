#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/prettywriter.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include "header/data_imexporter/psse_imexporter.h"
#include <istream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

POWER_SYSTEM_DATABASE::POWER_SYSTEM_DATABASE(STEPS& toolkit)
{
    this->toolkit = (&toolkit);

    set_system_name("");
    set_system_base_power_in_MVA(100.0);

    set_allowed_max_bus_number(1000);

    set_zero_impedance_threshold_in_pu(0.0001);

    clear();

    update_in_service_bus_count();
 }


POWER_SYSTEM_DATABASE::~POWER_SYSTEM_DATABASE()
{
    clear();

    toolkit = NULL;
}

STEPS& POWER_SYSTEM_DATABASE::get_toolkit() const
{
    return *toolkit;
}

void POWER_SYSTEM_DATABASE::set_zero_impedance_threshold_in_pu(double z)
{
    zero_impedance_threshold = z;
}

double POWER_SYSTEM_DATABASE::get_zero_impedance_threshold_in_pu() const
{
    return zero_impedance_threshold;
}

void POWER_SYSTEM_DATABASE::set_database_capacity()
{
    ostringstream osstream;
    unsigned int bus_capacity = 1000;
    unsigned int generator_capacity = (unsigned int)(round(bus_capacity*0.5));
    unsigned int wt_generator_capacity = (unsigned int)(round(bus_capacity*0.5));
    unsigned int pv_unit_capacity = (unsigned int)(round(bus_capacity*0.5));
    unsigned int sv_source_capacity = (unsigned int)(round(bus_capacity*0.2));
    unsigned int load_capacity = (unsigned int)(round(bus_capacity*0.5));
    unsigned int fixedshunt_capacity = (unsigned int)(round(bus_capacity*0.5));
    unsigned int line_capacity = (unsigned int)(round(bus_capacity*3.0));
    unsigned int transformer_capacity = bus_capacity;
    unsigned int hvdc_capacity = (unsigned int)(round(bus_capacity*0.1));
    unsigned int vsc_hvdc_capacity = (unsigned int)(round(bus_capacity*0.01));
    unsigned int mt_hvdc_capacity = (unsigned int)(round(bus_capacity*0.01));
    unsigned int equivalentdevice_capacity = (unsigned int)(round(bus_capacity*0.1));
    unsigned int energy_storage_capacity = (unsigned int)(round(bus_capacity*0.1));
    unsigned int area_capacity = (unsigned int)(round(bus_capacity*0.1));
    unsigned int zone_capacity = (unsigned int)(round(bus_capacity*0.1));
    unsigned int owner_capacity = (unsigned int)(round(bus_capacity*0.1));

    ifstream instream("steps_config.json");
    if(instream.is_open())
    {
        ostringstream tmp;
        tmp<<instream.rdbuf();
        string str = tmp.str();

        Document document;
        document.Parse(str.c_str());

        if(document.HasMember("bus") and document["bus"].IsInt())
            bus_capacity = (unsigned int)(document["bus"].GetInt());

        if(document.HasMember("generator") and document["generator"].IsInt())
            generator_capacity = (unsigned int)(document["generator"].GetInt());

        if(document.HasMember("wtGenerator") and document["wtGenerator"].IsInt())
            wt_generator_capacity = (unsigned int)(document["wtGenerator"].GetInt());

        if(document.HasMember("pvUnit") and document["pvUnit"].IsInt())
            pv_unit_capacity = (unsigned int)(document["pvUnit"].GetInt());

        if(document.HasMember("energyStorage") and document["energyStorage"].IsInt())
            energy_storage_capacity = (unsigned int)(document["energyStorage"].GetInt());

        if(document.HasMember("svSource") and document["svSource"].IsInt())
            sv_source_capacity = (unsigned int)(document["svSource"].GetInt());

        if(document.HasMember("load") and document["load"].IsInt())
            load_capacity = (unsigned int)(document["load"].GetInt());

        if(document.HasMember("fixedShunt") and document["fixedShunt"].IsInt())
            fixedshunt_capacity = (unsigned int)(document["fixedShunt"].GetInt());

        if(document.HasMember("line") and document["line"].IsInt())
            line_capacity = (unsigned int)(document["line"].GetInt());

        if(document.HasMember("transformer") and document["transformer"].IsInt())
            transformer_capacity = (unsigned int)(document["transformer"].GetInt());

        if(document.HasMember("hvdc") and document["hvdc"].IsInt())
            hvdc_capacity = (unsigned int)(document["hvdc"].GetInt());

        if(document.HasMember("vschvdc") and document["vschvdc"].IsInt())
            vsc_hvdc_capacity = (unsigned int)(document["vschvdc"].GetInt());

        if(document.HasMember("mthvdc") and document["mthvdc"].IsInt())
            mt_hvdc_capacity = (unsigned int)(document["mthvdc"].GetInt());

        if(document.HasMember("equivalentDevice") and document["equivalentDevice"].IsInt())
            equivalentdevice_capacity = (unsigned int)(document["equivalentDevice"].GetInt());

        if(document.HasMember("area") and document["area"].IsInt())
            area_capacity = (unsigned int)(document["area"].GetInt());

        if(document.HasMember("zone") and document["zone"].IsInt())
            zone_capacity = (unsigned int)(document["zone"].GetInt());

        if(document.HasMember("owner") and document["owner"].IsInt())
            owner_capacity = (unsigned int)(document["owner"].GetInt());
    }
    else
    {
        osstream<<"No configuration file <steps_config.json> is found. Use default power system database capacity.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }

    set_bus_capacity(bus_capacity);
    set_generator_capacity(generator_capacity);
    set_wt_generator_capacity(wt_generator_capacity);
    set_pv_unit_capacity(pv_unit_capacity);
    set_sv_source_capacity(sv_source_capacity);
    set_load_capacity(load_capacity);
    set_fixed_shunt_capacity(fixedshunt_capacity);
    set_ac_line_capacity(line_capacity);
    set_transformer_capacity(transformer_capacity);
    set_lcc_hvdc2t_capacity(hvdc_capacity);
    set_vsc_hvdc_capacity(vsc_hvdc_capacity);
    set_mt_hvdc_capacity(mt_hvdc_capacity);
    set_equivalent_device_capacity(equivalentdevice_capacity);
    set_energy_storage_capacity(energy_storage_capacity);
    set_area_capacity(area_capacity);
    set_zone_capacity(zone_capacity);
    set_owner_capacity(owner_capacity);
}

unsigned int POWER_SYSTEM_DATABASE::get_bus_capacity() const
{
    return Bus.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_generator_capacity() const
{
    return Generator.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_wt_generator_capacity() const
{
    return WT_Generator.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_pv_unit_capacity() const
{
    return PV_Unit.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_energy_storage_capacity() const
{
    return Energy_storage.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_sv_source_capacity() const
{
    return SV_source.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_load_capacity() const
{
    return Load.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_fixed_shunt_capacity() const
{
    return Fixed_shunt.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_ac_line_capacity() const
{
    return Ac_line.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_transformer_capacity() const
{
    return Transformer.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_lcc_hvdc2t_capacity() const
{
    return Lcc_hvdc2t.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_vsc_hvdc_capacity() const
{
    return Vsc_hvdc.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_mt_hvdc_capacity() const
{
    return Mt_hvdc.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_equivalent_device_capacity() const
{
    return Equivalent_device.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_area_capacity() const
{
    return Area.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_zone_capacity() const
{
    return Zone.capacity();
}

unsigned int POWER_SYSTEM_DATABASE::get_owner_capacity() const
{
    return Owner.capacity();
}

void POWER_SYSTEM_DATABASE::set_bus_capacity(unsigned int n)
{
    Bus.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_generator_capacity(unsigned int n)
{
    Generator.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_wt_generator_capacity(unsigned int n)
{
    WT_Generator.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_pv_unit_capacity(unsigned int n)
{
    PV_Unit.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_energy_storage_capacity(unsigned int n)
{
    Energy_storage.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_sv_source_capacity(unsigned int n)
{
    SV_source.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_load_capacity(unsigned int n)
{
    Load.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_fixed_shunt_capacity(unsigned int n)
{
    Fixed_shunt.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_ac_line_capacity(unsigned int n)
{
    Ac_line.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_transformer_capacity(unsigned int n)
{
    Transformer.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_lcc_hvdc2t_capacity(unsigned int n)
{
    Lcc_hvdc2t.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_vsc_hvdc_capacity(unsigned int n)
{
    Vsc_hvdc.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_mt_hvdc_capacity(unsigned int n)
{
    Mt_hvdc.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_equivalent_device_capacity(unsigned int n)
{
    Equivalent_device.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_area_capacity(unsigned int n)
{
    Area.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_zone_capacity(unsigned int n)
{
    Zone.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_owner_capacity(unsigned int n)
{
    Owner.reserve(n);
}

void POWER_SYSTEM_DATABASE::clear()
{
    set_case_information("");
    set_case_additional_information("");
    clear_all_buses();
    clear_all_sources();
    clear_all_loads();
    clear_all_ac_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_lcc_hvdc2ts();
    clear_all_vsc_hvdcs();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::set_system_name(const string& name)
{
    this->system_name = name;
}

string POWER_SYSTEM_DATABASE::get_system_name() const
{
    return this->system_name;
}

void POWER_SYSTEM_DATABASE::set_allowed_max_bus_number(unsigned int max_bus_num)
{
    bus_index.set_max_bus_number(max_bus_num);
}

unsigned int POWER_SYSTEM_DATABASE::get_allowed_max_bus_number() const
{
    return bus_index.get_max_bus_number();
}

bool POWER_SYSTEM_DATABASE::is_bus_in_allowed_range(unsigned int bus) const
{
    return (bus>0 and bus<=this->get_allowed_max_bus_number());
}

void POWER_SYSTEM_DATABASE::set_system_base_power_in_MVA(const double s)
{
    if(s>0.0)
    {
        system_base_power_in_MVA = s;
        one_over_system_base_power = 1.0/s;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Non positive power ("<<s<<" MVA) is not allowed for setting system base power."<<endl
          <<"Nothing will be changed.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::set_case_information(const string& title)
{
    case_information = title;
}

void POWER_SYSTEM_DATABASE::set_case_additional_information(const string& title)
{
    case_additional_information = title;
}

string POWER_SYSTEM_DATABASE::get_case_information() const
{
    return case_information;
}

string POWER_SYSTEM_DATABASE::get_case_additional_information() const
{
    return case_additional_information;
}

void POWER_SYSTEM_DATABASE::append_bus(BUS& bus)
{
    bus.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not bus.is_valid())
    {
        osstream<<"Warning. Failed to append invalid bus "<<bus.get_bus_number()<<" due to either 0 bus number of 0 base voltage.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int bus_number = bus.get_bus_number();

    if(not this->is_bus_in_allowed_range(bus_number))
    {
        osstream<<"Warning. Bus number "<<bus_number<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<bus.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Bus will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(this->is_bus_exist(bus_number))
    {
        osstream<<"Warning. Bus "<<bus_number<<" already exists in power system database '"<<get_system_name()<<"': Bus."<<endl
          <<"Duplicate copy is not allowed.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Bus.capacity()==Bus.size())
    {
        osstream<<"Warning. Capacity limit ("<<Bus.capacity()<<") reached when appending bus to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    unsigned int bus_count = get_bus_count();

    Bus.push_back(bus);

    bus_index.set_bus_with_index(bus_number, bus_count);
}

void POWER_SYSTEM_DATABASE::append_generator(GENERATOR& generator)
{
    generator.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not generator.is_valid())
    {
        osstream<<"Warning. Failed to append invalid generator to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int generator_bus = generator.get_generator_bus();

    if(not this->is_bus_in_allowed_range(generator_bus))
    {
        osstream<<"Warning. Bus number "<<generator_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<generator.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Generator will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = generator.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(generator_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_generator_exist(device_id))
    {
        osstream<<"Warning. "<<generator.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Generator."<<endl
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_generator_index(device_id);
        DEVICE_ID did_exist = Generator[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Generator.capacity()==Generator.size())
    {
        osstream<<"Warning. Capacity limit ("<<Generator.capacity()<<") reached when appending generator to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int generator_count = get_generator_count();
    Generator.push_back(generator);
    generator_index.set_device_index(device_id, generator_count);
}

void POWER_SYSTEM_DATABASE::append_wt_generator(WT_GENERATOR& wt_generator)
{
    wt_generator.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not wt_generator.is_valid())
    {
        osstream<<"Warning. Failed to append invalid wt generator to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int wt_generator_bus = wt_generator.get_source_bus();

    if(not this->is_bus_in_allowed_range(wt_generator_bus))
    {
        osstream<<"Warning. Bus "<<wt_generator_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<wt_generator.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"WT generator will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = wt_generator.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_WT_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(wt_generator_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_wt_generator_exist(device_id))
    {
        osstream<<"Warning. "<<wt_generator.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': WT_Generator."<<endl
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_wt_generator_index(device_id);
        DEVICE_ID did_exist = WT_Generator[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(WT_Generator.capacity()==WT_Generator.size())
    {
        osstream<<"Warning. Capacity limit ("<<WT_Generator.capacity()<<") reached when appending WT generator to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int wt_generator_count = get_wt_generator_count();
    WT_Generator.push_back(wt_generator);
    wt_generator_index.set_device_index(device_id, wt_generator_count);
}

void POWER_SYSTEM_DATABASE::append_pv_unit(PV_UNIT& pv_unit)
{
    pv_unit.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not pv_unit.is_valid())
    {
        osstream<<"Warning. Failed to append invalid pv unit to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int pv_unit_bus = pv_unit.get_unit_bus();

    if(not this->is_bus_in_allowed_range(pv_unit_bus))
    {
        osstream<<"Warning. Bus "<<pv_unit_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<pv_unit.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"PV unit will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = pv_unit.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_PV_UNIT);
    TERMINAL terminal;
    terminal.append_bus(pv_unit_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_pv_unit_exist(device_id))
    {
        osstream<<"Warning. "<<pv_unit.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': PV_Unit."<<endl
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_pv_unit_index(device_id);
        DEVICE_ID did_exist = PV_Unit[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(PV_Unit.capacity()==PV_Unit.size())
    {
        osstream<<"Warning. Capacity limit ("<<PV_Unit.capacity()<<") reached when appending PV Unit to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int pv_unit_count = get_pv_unit_count();
    PV_Unit.push_back(pv_unit);
    pv_unit_index.set_device_index(device_id, pv_unit_count);
}

void POWER_SYSTEM_DATABASE::append_energy_storage(ENERGY_STORAGE& estorage)
{
    estorage.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not estorage.is_valid())
    {
        osstream<<"Warning. Failed to append invalid energy storage to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int bus = estorage.get_energy_storage_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        osstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<estorage.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Energy storage will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = estorage.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_ENERGY_STORAGE);
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_energy_storage_exist(device_id))
    {
        osstream<<"Warning. "<<estorage.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Energy_storage.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_energy_storage_index(device_id);
        DEVICE_ID did_exist = Energy_storage[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Energy_storage.capacity()==Energy_storage.size())
    {
        osstream<<"Warning. Capacity limit ("<<Energy_storage.capacity()<<") reached when appending energy storage to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int energy_storage_count = get_energy_storage_count();
    Energy_storage.push_back(estorage);
    energy_storage_index.set_device_index(device_id, energy_storage_count);
 }

void POWER_SYSTEM_DATABASE::append_sv_source(SV_SOURCE& sv_source)
{
    sv_source.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not sv_source.is_valid())
    {
        osstream<<"Warning. Failed to append invalid sv source to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int bus = sv_source.get_sv_source_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        osstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<sv_source.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
                <<"SV source will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = sv_source.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_SV_SOURCE);
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_sv_source_exist(device_id))
    {
        osstream<<"Warning. "<<sv_source.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': SV_source.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_sv_source_index(device_id);
        DEVICE_ID did_exist = SV_source[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(SV_source.capacity()==SV_source.size())
    {
        osstream<<"Warning. Capacity limit ("<<SV_source.capacity()<<") reached when appending sv source to power system database '"<<get_system_name()<<"'."<<endl
                <<"Increase capacity by odifing steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int sv_source_count = get_sv_source_count();
    SV_source.push_back(sv_source);
    sv_source_index.set_device_index(device_id, sv_source_count);
}

void POWER_SYSTEM_DATABASE::append_load(LOAD& load)
{
    load.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not load.is_valid())
    {
        osstream<<"Warning. Failed to append invalid load to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int load_bus = load.get_load_bus();

    if(not this->is_bus_in_allowed_range(load_bus))
    {
        osstream<<"Warning. Bus "<<load_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<load.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Load will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = load.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_LOAD);
    TERMINAL terminal;
    terminal.append_bus(load_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(is_load_exist(device_id))
    {
        osstream<<"Warning. "<<load.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Load."<<endl
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_load_index(device_id);
        DEVICE_ID did_exist = Load[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Load.capacity()==Load.size())
    {
        osstream<<"Warning. Capacity limit ("<<Load.capacity()<<") reached when appending load to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int load_count = get_load_count();
    Load.push_back(load);
    load_index.set_device_index(device_id, load_count);
}

void POWER_SYSTEM_DATABASE::append_fixed_shunt(FIXED_SHUNT& shunt)
{
    shunt.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not shunt.is_valid())
    {
        osstream<<"Warning. Failed to append invalid fixed shunt to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int shunt_bus = shunt.get_shunt_bus();

    if(not this->is_bus_in_allowed_range(shunt_bus))
    {
        osstream<<"Warning. Bus "<<shunt_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<shunt.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Fixed shunt will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = shunt.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_FIXED_SHUNT);
    TERMINAL terminal;
    terminal.append_bus(shunt_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_fixed_shunt_exist(device_id))
    {
        osstream<<"Warning. "<<shunt.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Fixed_shunt.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_fixed_shunt_index(device_id);
        DEVICE_ID did_exist = Fixed_shunt[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Fixed_shunt.capacity()==Fixed_shunt.size())
    {
        osstream<<"Warning. Capacity limit ("<<Fixed_shunt.capacity()<<") reached when appending fixed shunt to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int shunt_count = get_fixed_shunt_count();
    Fixed_shunt.push_back(shunt);
    fixed_shunt_index.set_device_index(device_id, shunt_count);
}

void POWER_SYSTEM_DATABASE::append_ac_line(AC_LINE& line)
{
    line.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not line.is_valid())
    {
        osstream<<"Warning. Failed to append invalid line to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int sending_side_bus = line.get_sending_side_bus();
    unsigned int receiving_side_bus = line.get_receiving_side_bus();

    if(not this->is_bus_in_allowed_range(sending_side_bus))
    {
        osstream<<"Warning. Bus "<<sending_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(receiving_side_bus))
    {
        osstream<<"Warning. Bus "<<receiving_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = line.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(sending_side_bus);
    terminal.append_bus(receiving_side_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_ac_line_exist(device_id))
    {
        osstream<<"Warning. "<<line.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Ac_line."<<endl
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_ac_line_index(device_id);
        DEVICE_ID did_exist = Ac_line[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Ac_line.capacity()==Ac_line.size())
    {
        osstream<<"Warning. Capacity limit ("<<Ac_line.capacity()<<") reached when appending line to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int line_count = get_ac_line_count();
    Ac_line.push_back(line);
    ac_line_index.set_device_index(device_id, line_count);
}

void POWER_SYSTEM_DATABASE::append_transformer(TRANSFORMER& transformer)
{
    transformer.set_toolkit(*toolkit);
    ostringstream osstream;

    if(not transformer.is_valid())
    {
        osstream<<"Warning. Failed to append invalid transformer to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int primary_winding_bus = transformer.get_winding_bus(PRIMARY_SIDE);
    unsigned int secondary_winding_bus = transformer.get_winding_bus(SECONDARY_SIDE);
    unsigned int tertiary_winding_bus = transformer.get_winding_bus(TERTIARY_SIDE);

    if(not is_bus_in_allowed_range(primary_winding_bus))
    {
        osstream<<"Warning. Bus "<<primary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_bus_in_allowed_range(secondary_winding_bus))
    {
        osstream<<"Warning. Bus "<<secondary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not(is_bus_in_allowed_range(tertiary_winding_bus) or tertiary_winding_bus==0))
    {
        osstream<<"Warning. Bus "<<tertiary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = transformer.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_TRANSFORMER);
    TERMINAL terminal;
    terminal.append_bus(primary_winding_bus);
    terminal.append_bus(secondary_winding_bus);
    terminal.append_bus(tertiary_winding_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(is_transformer_exist(device_id))
    {
        osstream<<"Warning. "<<transformer.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"':Transformer.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_transformer_index(device_id);
        DEVICE_ID did_exist = Transformer[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Transformer.capacity()==Transformer.size())
    {
        osstream<<"Warning. Capacity limit ("<<Transformer.capacity()<<") reached when appending transformer to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int transformer_count = get_transformer_count();
    Transformer.push_back(transformer);
    transformer_index.set_device_index(device_id, transformer_count);
}

void POWER_SYSTEM_DATABASE::append_lcc_hvdc2t(LCC_HVDC2T& hvdc)
{
    hvdc.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not hvdc.is_valid())
    {
        osstream<<"Warning. Failed to append invalid hvdc to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int rec_bus = hvdc.get_converter_bus(RECTIFIER);
    unsigned int inv_bus = hvdc.get_converter_bus(INVERTER);

    if(not this->is_bus_in_allowed_range(rec_bus))
    {
        osstream<<"Warning. Bus "<<rec_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(inv_bus))
    {
        osstream<<"Warning. Bus "<<inv_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = hvdc.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_LCC_HVDC2T);
    TERMINAL terminal;
    terminal.append_bus(rec_bus);
    terminal.append_bus(inv_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_lcc_hvdc2t_exist(device_id))
    {
        osstream<<"Warning. "<<hvdc.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Lcc_hvdc2t.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_lcc_hvdc2t_index(device_id);
        DEVICE_ID did_exist = Lcc_hvdc2t[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Lcc_hvdc2t.capacity()==Lcc_hvdc2t.size())
    {
        osstream<<"Warning. Capacity limit ("<<Lcc_hvdc2t.capacity()<<") reached when appending Hvdc to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int hvdc_count = get_lcc_hvdc2t_count();
    Lcc_hvdc2t.push_back(hvdc);
    lcc_hvdc2t_index.set_device_index(device_id, hvdc_count);
}

void POWER_SYSTEM_DATABASE::append_vsc_hvdc(VSC_HVDC& vsc_hvdc)
{
    vsc_hvdc.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not vsc_hvdc.is_valid())
    {
        osstream<<"Warning. Failed to append invalid VSC HVDC to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    string identifier = vsc_hvdc.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_VSC_HVDC);
    TERMINAL terminal;
    unsigned int n_converter = vsc_hvdc.get_converter_count();
    for(unsigned int i=0; i!=n_converter; ++i)
    {
        unsigned int bus_number=vsc_hvdc.get_converter_ac_bus(i);
        terminal.append_bus(bus_number);
    }
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_vsc_hvdc_exist(device_id))
    {
        osstream<<"Warning. "<<vsc_hvdc.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Vsc_hvdc.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_vsc_hvdc_index(device_id);
        DEVICE_ID did_exist = Vsc_hvdc[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Vsc_hvdc.capacity()==Vsc_hvdc.size())
    {
        osstream<<"Warning. Capacity limit ("<<Vsc_hvdc.capacity()<<") reached when appending VSC HVDC to power system database '"<<get_system_name()<<"'."<<endl
                <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    unsigned int vsc_hvdc_count = get_vsc_hvdc_count();
    Vsc_hvdc.push_back(vsc_hvdc);
    vsc_hvdc_index.set_device_index(device_id, vsc_hvdc_count);
}

void POWER_SYSTEM_DATABASE::append_mt_hvdc(MT_HVDC& mt_hvdc)
{
    mt_hvdc.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not mt_hvdc.is_valid())
    {
        osstream<<"Warning. Failed to append invalid MT HVDC to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    string identifier = mt_hvdc.get_identifier();
    string name = mt_hvdc.get_name();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_HYBRID_DC);
    device_id.set_device_name_index(get_index_of_string(name));

    if(this->is_mt_hvdc_exist(device_id))
    {
        osstream<<"Warning. "<<mt_hvdc.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Mt_hvdc.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_mt_hvdc_index(device_id);
        DEVICE_ID did_exist = Mt_hvdc[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Mt_hvdc.capacity()==Mt_hvdc.size())
    {
        osstream<<"Warning. Capacity limit ("<<Mt_hvdc.capacity()<<") reached when appending MT HVDC to power system database '"<<get_system_name()<<"'."<<endl
                <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    unsigned int mt_hvdc_count = get_mt_hvdc_count();
    Mt_hvdc.push_back(mt_hvdc);
    mt_hvdc_index.set_device_index(device_id, mt_hvdc_count);
}

void POWER_SYSTEM_DATABASE::append_equivalent_device(EQUIVALENT_DEVICE& edevice)
{
    edevice.set_toolkit(*toolkit);

    ostringstream osstream;

    if(not edevice.is_valid())
    {
        osstream<<"Warning. Failed to append invalid equivalent device to power system database '"<<get_system_name()<<"'.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int bus = edevice.get_equivalent_device_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        osstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<edevice.get_compound_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
                <<"Equivalent device will not be appended into the database.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    string identifier = edevice.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_EQUIVALENT_DEVICE);
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string(identifier));

    if(this->is_equivalent_device_exist(device_id))
    {
        osstream<<"Warning. "<<edevice.get_compound_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Equivalent_device.\n"
                <<"Duplicate copy is not allowed.";
        unsigned int index = get_equivalent_device_index(device_id);
        DEVICE_ID did_exist = Equivalent_device[index].get_device_id();
        if(std::hash<DEVICE_ID>{}(device_id)==std::hash<DEVICE_ID>{}(did_exist) and did_exist!=device_id)
            osstream<<endl<<"CAUTION! Different devices with the same hash DEVICE_ID detected: "<<device_id.get_compound_device_name()
                    <<", "<<did_exist.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(Equivalent_device.capacity()==Equivalent_device.size())
    {
        osstream<<"Warning. Capacity limit ("<<Equivalent_device.capacity()<<") reached when appending equivalent device to power system database '"<<get_system_name()<<"'."<<endl
                <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    unsigned int equivalent_device_count = get_equivalent_device_count();
    Equivalent_device.push_back(edevice);
    equivalent_device_index.set_device_index(device_id, equivalent_device_count);
 }

void POWER_SYSTEM_DATABASE::append_area(AREA& area)
{
    area.set_toolkit(*toolkit);

    if(area.is_valid() and
       (not this->is_area_exist(area.get_area_number())) and
       Area.capacity()>Area.size())
    {
        unsigned int area_count = get_area_count();
        Area.push_back(area);
        area_index[area.get_area_number()]= area_count;
    }
    else
    {
        ostringstream osstream;
        if(not area.is_valid())
            osstream<<"Warning. Failed to append invalid area to power system database '"<<get_system_name()<<"'.";
        if(this->is_area_exist(area.get_area_number()))
            osstream<<"Warning. Area "<<area.get_area_number()<<" already exists in power system database '"<<get_system_name()<<"': Area."<<endl
                    <<"Duplicate copy is not allowed.";
        if(Area.capacity()==Area.size())
            osstream<<"Warning. Capacity limit ("<<Area.capacity()<<") reached when appending area to power system database '"<<get_system_name()<<"'."<<endl
                    <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::append_zone(ZONE& zone)
{
    zone.set_toolkit(*toolkit);

    if(zone.is_valid() and
       (not this->is_zone_exist(zone.get_zone_number())) and
       Zone.capacity()>Zone.size())
    {
        unsigned int zone_count = get_zone_count();
        Zone.push_back(zone);
        zone_index[zone.get_zone_number()]= zone_count;
    }
    else
    {
        ostringstream osstream;
        if(not zone.is_valid())
            osstream<<"Warning. Failed to append invalid zone to power system database '"<<get_system_name()<<"'.";
        if(this->is_zone_exist(zone.get_zone_number()))
            osstream<<"Warning. Zone "<<zone.get_zone_number()<<" already exists in power system database '"<<get_system_name()<<"': Zone."<<endl
                    <<"Duplicate copy is not allowed.";
        if(Zone.capacity()==Zone.size())
            osstream<<"Warning. Capacity limit ("<<Zone.capacity()<<") reached when appending zone to power system database '"<<get_system_name()<<"'."<<endl
                    <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::append_owner(OWNER& owner)
{
    owner.set_toolkit(*toolkit);

    if(owner.is_valid() and
       (not this->is_owner_exist(owner.get_owner_number())) and
       Owner.capacity()>Owner.size())
    {
        unsigned int owner_count = get_owner_count();
        Owner.push_back(owner);
        owner_index[owner.get_owner_number()]= owner_count;
    }
    else
    {
        ostringstream osstream;
        if(not owner.is_valid())
            osstream<<"Warning. Failed to append invalid owner to power system database '"<<get_system_name()<<"'.";
        if(this->is_owner_exist(owner.get_owner_number()))
            osstream<<"Warning. Owner "<<owner.get_owner_number()<<" already exists in power system database '"<<get_system_name()<<"': Owner."<<endl
                    <<"Duplicate copy is not allowed.";
        if(Owner.capacity()==Owner.size())
            osstream<<"Warning. Capacity limit ("<<Owner.capacity()<<") reached when appending owner to power system database '"<<get_system_name()<<"'."<<endl
                    <<"Increase capacity by modified steps_config.json.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::append_mutual_data(MUTUAL_DATA& mutual_data)
{
    mutual_data.set_toolkit(*toolkit);
    Mutual_data.push_back(mutual_data);
}


void POWER_SYSTEM_DATABASE::replace_generator_with_wt_generator(DEVICE_ID& gendid)
{
    if(gendid.get_device_type() == STEPS_GENERATOR)
    {
        if(is_generator_exist(gendid))
        {
            GENERATOR* gen = get_pointer_to_generator(gendid);

            DEVICE_ID wtgendid;
            wtgendid.set_device_type(STEPS_WT_GENERATOR);
            TERMINAL terminal;
            terminal.append_bus(gen->get_generator_bus());
            wtgendid.set_device_terminal(terminal);
            wtgendid.set_device_identifier_index(gen->get_identifier_index());

            if(not is_wt_generator_exist(wtgendid))
            {
                WT_GENERATOR wtgen(*toolkit);
                wtgen.set_generator_bus(gen->get_generator_bus());
                wtgen.set_identifier(gen->get_identifier());
                wtgen.set_status(gen->get_status());
                wtgen.set_mbase_in_MVA(gen->get_mbase_in_MVA());
                wtgen.set_p_generation_in_MW(gen->get_p_generation_in_MW());
                wtgen.set_q_generation_in_MVar(gen->get_q_generation_in_MVar());
                wtgen.set_p_max_in_MW(gen->get_p_max_in_MW());
                wtgen.set_p_min_in_MW(gen->get_p_min_in_MW());
                wtgen.set_q_max_in_MVar(gen->get_q_max_in_MVar());
                wtgen.set_q_min_in_MVar(gen->get_q_min_in_MVar());
                wtgen.set_q_max_nominal_in_MVar(gen->get_q_max_nominal_in_MVar());
                wtgen.set_q_min_nominal_in_MVar(gen->get_q_min_nominal_in_MVar());
                wtgen.set_source_impedance_in_pu(gen->get_source_impedance_in_pu());
                wtgen.set_voltage_to_regulate_in_pu(gen->get_voltage_to_regulate_in_pu());
                wtgen.set_bus_to_regulate(gen->get_bus_to_regulate());

                append_wt_generator(wtgen);
                clear_generator(gendid);
            }
            else
            {
                ostringstream osstream;
                osstream<<"Warning. "<<gendid.get_compound_device_name()<<" cannot be replaced with WT GENERATOR since "
                        <<wtgendid.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABSE.";
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::replace_generator_with_pv_unit(DEVICE_ID& gendid)
{
    if(gendid.get_device_type() == STEPS_GENERATOR)
    {
        if(is_generator_exist(gendid))
        {
            GENERATOR* gen = get_pointer_to_generator(gendid);

            DEVICE_ID pvudid;
            pvudid.set_device_type(STEPS_PV_UNIT);
            TERMINAL terminal;
            terminal.append_bus(gen->get_generator_bus());
            pvudid.set_device_terminal(terminal);
            pvudid.set_device_identifier_index(gen->get_identifier_index());

            if(not is_pv_unit_exist(pvudid))
            {
                PV_UNIT pvu(*toolkit);
                pvu.set_unit_bus(gen->get_generator_bus());
                pvu.set_identifier(gen->get_identifier());
                pvu.set_status(gen->get_status());
                pvu.set_mbase_in_MVA(gen->get_mbase_in_MVA());
                pvu.set_p_generation_in_MW(gen->get_p_generation_in_MW());
                pvu.set_q_generation_in_MVar(gen->get_q_generation_in_MVar());
                pvu.set_p_max_in_MW(gen->get_p_max_in_MW());
                pvu.set_p_min_in_MW(gen->get_p_min_in_MW());
                pvu.set_q_max_in_MVar(gen->get_q_max_in_MVar());
                pvu.set_q_min_in_MVar(gen->get_q_min_in_MVar());
                pvu.set_q_max_nominal_in_MVar(gen->get_q_max_nominal_in_MVar());
                pvu.set_q_min_nominal_in_MVar(gen->get_q_min_nominal_in_MVar());
                pvu.set_source_impedance_in_pu(gen->get_source_impedance_in_pu());
                pvu.set_voltage_to_regulate_in_pu(gen->get_voltage_to_regulate_in_pu());
                pvu.set_bus_to_regulate(gen->get_bus_to_regulate());

                append_pv_unit(pvu);
                clear_generator(gendid);
            }
            else
            {
                ostringstream osstream;
                osstream<<"Warning. "<<gendid.get_compound_device_name()<<" cannot be replaced with PV UNIT since "
                        <<pvudid.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABSE.";
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::replace_generator_with_energy_storage(DEVICE_ID& gendid)
{
    if(gendid.get_device_type() == STEPS_GENERATOR)
    {
        if(is_generator_exist(gendid))
        {
            GENERATOR* gen = get_pointer_to_generator(gendid);

            DEVICE_ID esdid;
            esdid.set_device_type(STEPS_ENERGY_STORAGE);
            TERMINAL terminal;
            terminal.append_bus(gen->get_generator_bus());
            esdid.set_device_terminal(terminal);
            esdid.set_device_identifier_index(gen->get_identifier_index());

            if(not is_energy_storage_exist(esdid))
            {
                ENERGY_STORAGE es(*toolkit);
                es.set_energy_storage_bus(gen->get_generator_bus());
                es.set_identifier(gen->get_identifier());
                es.set_status(gen->get_status());
                es.set_mbase_in_MVA(gen->get_mbase_in_MVA());
                es.set_p_generation_in_MW(gen->get_p_generation_in_MW());
                es.set_q_generation_in_MVar(gen->get_q_generation_in_MVar());
                es.set_p_max_in_MW(gen->get_p_max_in_MW());
                es.set_p_min_in_MW(gen->get_p_min_in_MW());
                es.set_q_max_in_MVar(gen->get_q_max_in_MVar());
                es.set_q_min_in_MVar(gen->get_q_min_in_MVar());
                es.set_q_max_nominal_in_MVar(gen->get_q_max_nominal_in_MVar());
                es.set_q_min_nominal_in_MVar(gen->get_q_min_nominal_in_MVar());
                es.set_source_impedance_in_pu(gen->get_source_impedance_in_pu());
                es.set_voltage_to_regulate_in_pu(gen->get_voltage_to_regulate_in_pu());
                es.set_bus_to_regulate(gen->get_bus_to_regulate());

                append_energy_storage(es);
                clear_generator(gendid);
            }
            else
            {
                ostringstream osstream;
                osstream<<"Warning. "<<gendid.get_compound_device_name()<<" cannot be replaced with ENERGY STORAGE since "
                        <<esdid.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABSE.";
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::replace_generator_with_sv_source(DEVICE_ID& gendid)
{
    if(gendid.get_device_type() == STEPS_GENERATOR)
    {
        if(is_generator_exist(gendid))
        {
            GENERATOR* gen = get_pointer_to_generator(gendid);

            DEVICE_ID svsdid;
            svsdid.set_device_type(STEPS_SV_SOURCE);
            TERMINAL terminal;
            terminal.append_bus(gen->get_generator_bus());
            svsdid.set_device_terminal(terminal);
            svsdid.set_device_identifier_index(gen->get_identifier_index());

            if(not is_sv_source_exist(svsdid))
            {
                SV_SOURCE svs(*toolkit);
                svs.set_sv_source_bus(gen->get_generator_bus());
                svs.set_identifier(gen->get_identifier());
                svs.set_status(gen->get_status());
                svs.set_mbase_in_MVA(gen->get_mbase_in_MVA());
                svs.set_p_generation_in_MW(gen->get_p_generation_in_MW());
                svs.set_q_generation_in_MVar(gen->get_q_generation_in_MVar());
                svs.set_p_max_in_MW(gen->get_p_max_in_MW());
                svs.set_p_min_in_MW(gen->get_p_min_in_MW());
                svs.set_q_max_in_MVar(gen->get_q_max_in_MVar());
                svs.set_q_min_in_MVar(gen->get_q_min_in_MVar());
                svs.set_q_max_nominal_in_MVar(gen->get_q_max_nominal_in_MVar());
                svs.set_q_min_nominal_in_MVar(gen->get_q_min_nominal_in_MVar());
                svs.set_source_impedance_in_pu(gen->get_source_impedance_in_pu());
                svs.set_voltage_to_regulate_in_pu(gen->get_voltage_to_regulate_in_pu());
                svs.set_bus_to_regulate(gen->get_bus_to_regulate());

                append_sv_source(svs);
                clear_generator(gendid);
            }
            else
            {
                ostringstream osstream;
                osstream<<"Warning. "<<gendid.get_compound_device_name()<<" cannot be replaced with SV SOURCE since "
                        <<svsdid.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABSE.";
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::copy_bus(unsigned int new_bus_number, string new_bus_name, double new_basekV, unsigned int old_bus_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_bus_exist(new_bus_number))
    {
        if(is_bus_exist(old_bus_number))
        {
            BUS new_bus(toolkit);
            BUS* old_bus = get_pointer_to_ac_bus(old_bus_number);

            new_bus.set_bus_number(new_bus_number);
            new_bus.set_bus_name(new_bus_name);
            new_bus.set_base_voltage_in_kV(new_basekV);

            new_bus.copy_basic_data_from_bus(*old_bus);

            append_bus(new_bus);
        }
        else
        {
            osstream<<"Bus "<<old_bus_number<<" does not exist in POWER_SYSTEM_DATABASE when copying bus as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Bus "<<new_bus_number<<" already exists in POWER_SYSTEM_DATABASE when copying bus as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_generator(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_GENERATOR or old_did.get_device_type() != STEPS_GENERATOR)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_GENERATOR is required when copying generator in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_generator_exist(new_did))
    {
        if(is_generator_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            GENERATOR new_gen(toolkit);
            GENERATOR* old_gen = get_pointer_to_generator(old_did);

            new_gen.set_generator_bus(bus);
            new_gen.set_identifier(identifier);

            new_gen.copy_basic_data_from_generator(*old_gen);

            append_generator(new_gen);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying generator as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying generator as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_wt_generator(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_WT_GENERATOR or old_did.get_device_type() != STEPS_WT_GENERATOR)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_WT_GENERATOR is required when copying wt generator in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_wt_generator_exist(new_did))
    {
        if(is_wt_generator_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            WT_GENERATOR new_wt_gen(toolkit);
            WT_GENERATOR* old_wt_gen = get_pointer_to_wt_generator(old_did);

            new_wt_gen.set_generator_bus(bus);
            new_wt_gen.set_identifier(identifier);

            new_wt_gen.copy_basic_data_from_wt_generator(*old_wt_gen);

            append_wt_generator(new_wt_gen);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying wt generator as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying wt generator as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_pv_unit(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_PV_UNIT or old_did.get_device_type() != STEPS_PV_UNIT)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_PV_UNIT is required when copying pv unit in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_pv_unit_exist(new_did))
    {
        if(is_pv_unit_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            PV_UNIT new_pvu(toolkit);
            PV_UNIT* old_pvu = get_pointer_to_pv_unit(old_did);

            new_pvu.set_unit_bus(bus);
            new_pvu.set_identifier(identifier);

            new_pvu.copy_basic_data_from_pv_unit(*old_pvu);

            append_pv_unit(new_pvu);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying pv unit as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying pv unit as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_energy_storage(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_ENERGY_STORAGE or old_did.get_device_type() != STEPS_ENERGY_STORAGE)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_ENERGY_STORAGE is required when copying energy storage in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_energy_storage_exist(new_did))
    {
        if(is_energy_storage_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            ENERGY_STORAGE new_es(toolkit);
            ENERGY_STORAGE* old_es = get_pointer_to_energy_storage(old_did);

            new_es.set_energy_storage_bus(bus);
            new_es.set_identifier(identifier);

            new_es.copy_basic_data_from_energy_storage(*old_es);

            append_energy_storage(new_es);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying energy storage as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying energy storage as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_sv_source(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_SV_SOURCE or old_did.get_device_type() != STEPS_SV_SOURCE)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_SV_SOURCE is required when copying sv source in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_sv_source_exist(new_did))
    {
        if(is_sv_source_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            SV_SOURCE new_svs(toolkit);
            SV_SOURCE* old_svs = get_pointer_to_sv_source(old_did);

            new_svs.set_sv_source_bus(bus);
            new_svs.set_identifier(identifier);

            new_svs.copy_basic_data_from_sv_source(*old_svs);

            append_sv_source(new_svs);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying sv source as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying sv source as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_load(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_LOAD or old_did.get_device_type() != STEPS_LOAD)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_LOAD is required when copying load in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_load_exist(new_did))
    {
        if(is_load_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            LOAD new_load(toolkit);
            LOAD* old_load = get_pointer_to_load(old_did);

            new_load.set_load_bus(bus);
            new_load.set_identifier(identifier);

            new_load.copy_basic_data_from_load(*old_load);

            append_load(new_load);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying load as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying load as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_fixed_shunt(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_FIXED_SHUNT or old_did.get_device_type() != STEPS_FIXED_SHUNT)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_FIXED_SHUNT is required when copying fixed shunt in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_fixed_shunt_exist(new_did))
    {
        if(is_fixed_shunt_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int bus = buses[0];
            string identifier = new_did.get_device_identifier();

            FIXED_SHUNT new_fixed_shunt(toolkit);
            FIXED_SHUNT* old_fixed_shunt = get_pointer_to_fixed_shunt(old_did);

            new_fixed_shunt.set_shunt_bus(bus);
            new_fixed_shunt.set_identifier(identifier);

            new_fixed_shunt.copy_basic_data_from_fixed_shunt(*old_fixed_shunt);

            append_fixed_shunt(new_fixed_shunt);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying fixed shunt as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying fixed shunt as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_ac_line(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_AC_LINE or old_did.get_device_type() != STEPS_AC_LINE)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_AC_LINE is required when copying ac line in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_ac_line_exist(new_did))
    {
        if(is_ac_line_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int ibus = buses[0];
            unsigned int jbus = buses[1];
            string identifier = new_did.get_device_identifier();

            AC_LINE new_line(toolkit);
            AC_LINE* old_line = get_pointer_to_ac_line(old_did);

            new_line.set_sending_side_bus(ibus);
            new_line.set_receiving_side_bus(jbus);
            new_line.set_identifier(identifier);

            new_line.copy_basic_data_from_ac_line(*old_line);

            append_ac_line(new_line);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying ac line as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying ac line as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::copy_transformer(DEVICE_ID& new_did, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(new_did.get_device_type() != STEPS_TRANSFORMER or old_did.get_device_type() != STEPS_TRANSFORMER)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_TRANSFORMER is required when copying transformer in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_transformer_exist(new_did))
    {
        if(is_transformer_exist(old_did))
        {
            TERMINAL terminal = new_did.get_device_terminal();
            vector<unsigned int> buses = terminal.get_buses();
            unsigned int ibus = buses[0];
            unsigned int jbus = buses[1];
            unsigned int kbus = 0;
            if(buses.size()==3)
                kbus = buses[2];
            string identifier = new_did.get_device_identifier();

            terminal = old_did.get_device_terminal();
            buses = terminal.get_buses();
            unsigned int ibus2 = buses[0];
            unsigned int jbus2 = buses[1];
            unsigned int kbus2 = 0;
            if(buses.size()==3)
                kbus2 = buses[2];

            if((kbus==0 and kbus2!=0) or (kbus!=0 and kbus2==0))
            {
                osstream<<"Warning. 2-winding and 3-winding transformers are not copiable when copying transformer in POWER_SYSTEM_DATABASE."<<endl
                        <<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                        <<"No copy operation will be executed.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
            TRANSFORMER  new_transformer(toolkit);
            TRANSFORMER* old_transformer = get_pointer_to_transformer(old_did);
            if(kbus == 0)
            {
                double ibasekV = get_bus_base_voltage_in_kV(ibus);
                double jbasekV = get_bus_base_voltage_in_kV(jbus);
                double ibasekV2 = get_bus_base_voltage_in_kV(ibus2);
                double jbasekV2 = get_bus_base_voltage_in_kV(jbus2);
                if(ibasekV>jbasekV and ibasekV2>jbasekV2)
                {
                    new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(ibus2), ibus);
                    new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(jbus2), jbus);
                }
                else
                {
                    new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(ibus2), jbus);
                    new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(jbus2), ibus);
                }
            }
            else
            {
                unsigned int bus_with_greatest_vbase = get_bus_with_greatest_vbase(ibus, jbus, kbus);
                unsigned int bus_with_medium_vbase   =   get_bus_with_medium_vbase(ibus, jbus, kbus);
                unsigned int bus_with_lowest_vbase   =   get_bus_with_lowest_vbase(ibus, jbus, kbus);

                unsigned int bus_with_greatest_vbase2 = get_bus_with_greatest_vbase(ibus2, jbus2, kbus2);
                unsigned int bus_with_medium_vbase2   =   get_bus_with_medium_vbase(ibus2, jbus2, kbus2);
                unsigned int bus_with_lowest_vbase2   =   get_bus_with_lowest_vbase(ibus2, jbus2, kbus2);


                new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(bus_with_greatest_vbase2), bus_with_greatest_vbase);
                new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(bus_with_medium_vbase2), bus_with_medium_vbase);
                new_transformer.set_winding_bus(old_transformer->get_winding_bus_side(bus_with_lowest_vbase2), bus_with_lowest_vbase);
            }
            new_transformer.set_identifier(identifier);

            new_transformer.copy_basic_data_from_transformer(*old_transformer);

            append_transformer(new_transformer);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying transformer as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying transformer as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

unsigned int POWER_SYSTEM_DATABASE::get_bus_with_greatest_vbase(unsigned int ibus, unsigned int jbus, unsigned int kbus) const
{
    double ibasekV = get_bus_base_voltage_in_kV(ibus);
    double jbasekV = get_bus_base_voltage_in_kV(jbus);
    double kbasekV = get_bus_base_voltage_in_kV(kbus);
    if(ibasekV>jbasekV and ibasekV>kbasekV)
        return ibus;
    else if(jbasekV>ibasekV and jbasekV>kbasekV)
        return jbus;
    else
        return kbus;
}

unsigned int POWER_SYSTEM_DATABASE::get_bus_with_medium_vbase(unsigned int ibus, unsigned int jbus, unsigned int kbus) const
{
    double ibasekV = get_bus_base_voltage_in_kV(ibus);
    double jbasekV = get_bus_base_voltage_in_kV(jbus);
    double kbasekV = get_bus_base_voltage_in_kV(kbus);
    if((jbasekV-ibasekV)*(kbasekV-ibasekV)>0.0)
        return ibus;
    else if((ibasekV-jbasekV)*(kbasekV-jbasekV)>0.0)
        return jbus;
    else
        return kbus;
}

unsigned int POWER_SYSTEM_DATABASE::get_bus_with_lowest_vbase(unsigned int ibus, unsigned int jbus, unsigned int kbus) const
{
    double ibasekV = get_bus_base_voltage_in_kV(ibus);
    double jbasekV = get_bus_base_voltage_in_kV(jbus);
    double kbasekV = get_bus_base_voltage_in_kV(kbus);
    if(ibasekV<jbasekV and ibasekV<kbasekV)
        return ibus;
    else if(jbasekV<ibasekV and jbasekV<kbasekV)
        return jbus;
    else
        return kbus;
}

void POWER_SYSTEM_DATABASE::copy_lcc_hvdc2t(unsigned int new_rec_bus_number, unsigned int new_inv_bus_number, char* new_hvdcname, DEVICE_ID& old_did)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    DEVICE_ID new_did = get_lcc_hvdc2t_device_id(new_rec_bus_number, new_inv_bus_number, new_hvdcname);
    if(new_did.get_device_type() != STEPS_LCC_HVDC2T or old_did.get_device_type() != STEPS_LCC_HVDC2T)
    {
        osstream<<"Warning. DEVICE_ID with type STEPS_LCC_HVDC2T is required when copying lcc hvdc 2t in POWER_SYSTEM_DATABASE."<<endl
                <<"However, "<<new_did.get_compound_device_name()<<" and "<<old_did.get_compound_device_name()<<" are provided."<<endl
                <<"No copy operation will be executed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(not is_lcc_hvdc2t_exist(new_did))
    {
        if(is_lcc_hvdc2t_exist(old_did))
        {
            LCC_HVDC2T new_hvdc(toolkit);
            LCC_HVDC2T* old_hvdc = get_pointer_to_lcc_hvdc2t(old_did);

            new_hvdc.set_converter_bus(RECTIFIER, new_rec_bus_number);
            new_hvdc.set_converter_bus(INVERTER, new_inv_bus_number);
            new_hvdc.set_identifier(new_hvdcname);

            new_hvdc.copy_basic_data_from_lcc_hvdc2t(*old_hvdc);

            append_lcc_hvdc2t(new_hvdc);
        }
        else
        {
            osstream<<old_did.get_compound_device_name()<<" does not exist in POWER_SYSTEM_DATABASE when copying lcc hvdc 2t as source.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<new_did.get_compound_device_name()<<" already exists in POWER_SYSTEM_DATABASE when copying lcc hvdc 2t as destination.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::update_device_id(const DEVICE_ID& did_old, const DEVICE_ID& did_new)
{
    ostringstream osstream;
    STEPS_DEVICE_TYPE old_device_type=did_old.get_device_type();
    STEPS_DEVICE_TYPE new_device_type=did_new.get_device_type();
    if(old_device_type!=new_device_type)
    {
        osstream<<"Device ID types are inconsistent with each other when calling "<<__FUNCTION__<<":\n"
                <<"Old device type is: "<<device_type2string(old_device_type)<<". New device type is: "<<device_type2string(new_device_type)<<".\n"
                <<"No device id will be updated.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    STEPS_DEVICE_TYPE device_type = old_device_type;
    if(device_type!=STEPS_GENERATOR and device_type!=STEPS_WT_GENERATOR and device_type!=STEPS_PV_UNIT and
       device_type!=STEPS_ENERGY_STORAGE and device_type!=STEPS_SV_SOURCE and
       device_type!=STEPS_LOAD and device_type!=STEPS_FIXED_SHUNT and
       device_type!=STEPS_AC_LINE and device_type!=STEPS_TRANSFORMER and device_type!=STEPS_LCC_HVDC2T and
       device_type!=STEPS_VSC_HVDC and device_type!=STEPS_HYBRID_DC)
    {
        osstream<<"Device ID type is not in the following allowed types when calling "<<__FUNCTION__<<":\n"
                <<"[GENERATOR, WT GENERATOR, PV UNIT, ENERGY STORAGE, SV SOURCE, LOAD, FIXED SHUNT, LINE, TRANSFORMER, 2T LCC HVDC, VSC HVDC, LCC HVDC].\n"
                <<"The input device type is: "<<device_type2string(device_type)<<".\n"
                <<"No device id will be updated.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    TERMINAL new_terminal = did_new.get_device_terminal();
    string new_id = did_new.get_device_identifier();
    if(device_type==STEPS_VSC_HVDC)
    {
        if(not is_vsc_hvdc_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_vsc_hvdc_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        VSC_HVDC* vsc_hvdc = get_pointer_to_vsc_hvdc(did_old);
        vsc_hvdc_index.swap_device_index(did_old, did_new);
        unsigned int n_bus = new_terminal.get_bus_count();
        for(unsigned int i=0; i != n_bus; ++i)
        {
            vsc_hvdc->set_converter_ac_bus(i, new_terminal[i]);
        }

        vsc_hvdc->set_identifier(new_id);
        return;

    }

    if(device_type==STEPS_HYBRID_DC)
    {
        if(not is_mt_hvdc_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_mt_hvdc_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        MT_HVDC* mt_hvdc = get_pointer_to_mt_hvdc(did_old);
        mt_hvdc_index.swap_device_index(did_old, did_new);
        string name = did_new.get_device_name();
        mt_hvdc->set_name(name);
        return;

    }
    // The following handle devices with buses <=3
    unsigned int ibus = new_terminal[0];
    unsigned int jbus = new_terminal[1];
    unsigned int kbus = new_terminal[2];

    if(device_type==STEPS_GENERATOR)
    {
        if(not is_generator_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_generator_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        GENERATOR* gen = get_pointer_to_generator(did_old);
        generator_index.swap_device_index(did_old, did_new);
        gen->set_generator_bus(ibus);
        gen->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_WT_GENERATOR)
    {
        if(not is_wt_generator_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_wt_generator_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        WT_GENERATOR* gen = get_pointer_to_wt_generator(did_old);
        wt_generator_index.swap_device_index(did_old, did_new);
        gen->set_generator_bus(ibus);
        gen->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_PV_UNIT)
    {
        if(not is_pv_unit_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_pv_unit_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        PV_UNIT* pv = get_pointer_to_pv_unit(did_old);
        pv_unit_index.swap_device_index(did_old, did_new);
        pv->set_unit_bus(ibus);
        pv->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_ENERGY_STORAGE)
    {
        if(not is_energy_storage_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_energy_storage_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        ENERGY_STORAGE* es = get_pointer_to_energy_storage(did_old);
        energy_storage_index.swap_device_index(did_old, did_new);
        es->set_energy_storage_bus(ibus);
        es->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_SV_SOURCE)
    {
        if(not is_sv_source_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_sv_source_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        SV_SOURCE* svs = get_pointer_to_sv_source(did_old);
        sv_source_index.swap_device_index(did_old, did_new);
        svs->set_sv_source_bus(ibus);
        svs->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_LOAD)
    {
        if(not is_load_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_load_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        LOAD* load = get_pointer_to_load(did_old);
        load_index.swap_device_index(did_old, did_new);
        load->set_load_bus(ibus);
        load->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_FIXED_SHUNT)
    {
        if(not is_fixed_shunt_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_fixed_shunt_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        FIXED_SHUNT* fixed_shunt = get_pointer_to_fixed_shunt(did_old);
        fixed_shunt_index.swap_device_index(did_old, did_new);
        fixed_shunt->set_shunt_bus(ibus);
        fixed_shunt->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_AC_LINE)
    {
        if(not is_ac_line_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_ac_line_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        AC_LINE* line = get_pointer_to_ac_line(did_old);
        ac_line_index.swap_device_index(did_old, did_new);
        line->set_sending_side_bus(ibus);
        line->set_receiving_side_bus(jbus);
        line->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_TRANSFORMER)
    {
        if(not is_transformer_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_transformer_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        TERMINAL old_terminal = did_old.get_device_terminal();
        if(old_terminal.get_bus_count()!=new_terminal.get_bus_count())
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" bus count is inconsistent with the old device "<<did_old.get_compound_device_name()<<". No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        TRANSFORMER* trans = get_pointer_to_transformer(did_old);
        transformer_index.swap_device_index(did_old, did_new);
        trans->set_winding_bus(PRIMARY_SIDE, ibus);
        trans->set_winding_bus(SECONDARY_SIDE, jbus);
        trans->set_winding_bus(TERTIARY_SIDE, kbus);
        trans->set_identifier(new_id);
        return;
    }

    if(device_type==STEPS_LCC_HVDC2T)
    {
        if(not is_lcc_hvdc2t_exist(did_old))
        {
            osstream<<"Warning. The old device "<<did_old.get_compound_device_name()<<" does not exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        if(is_lcc_hvdc2t_exist(did_new))
        {
            osstream<<"Warning. The new device "<<did_new.get_compound_device_name()<<" already exist in database. No device id will be updated.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return;
        }
        LCC_HVDC2T* hvdc = get_pointer_to_lcc_hvdc2t(did_old);
        lcc_hvdc2t_index.swap_device_index(did_old, did_new);
        hvdc->set_converter_bus(RECTIFIER, ibus);
        hvdc->set_converter_bus(INVERTER, jbus);
        hvdc->set_identifier(new_id);
        return;
    }
}

void POWER_SYSTEM_DATABASE::update_all_bus_base_frequency(double fbase_Hz)
{
    unsigned int n = Bus.size();
    for(unsigned int i=0; i!=n; ++i)
        Bus[i].set_base_frequency_in_Hz(fbase_Hz);
}

bool POWER_SYSTEM_DATABASE::is_bus_exist(unsigned int bus) const
{
    if(bus != 0)
    {
        unsigned int index = this->get_bus_index(bus);
        if(index!=INDEX_NOT_EXIST) return true;
        else                       return false;
    }
    else
        return false;
}

bool POWER_SYSTEM_DATABASE::is_generator_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_generator_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_wt_generator_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_wt_generator_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_pv_unit_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_pv_unit_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_energy_storage_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_energy_storage_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_sv_source_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_sv_source_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_load_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_load_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_fixed_shunt_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_fixed_shunt_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_ac_line_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_ac_line_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_transformer_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_transformer_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_lcc_hvdc2t_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_lcc_hvdc2t_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_vsc_hvdc_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_vsc_hvdc_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_vsc_hvdc_exist(const string vsc_name) const
{
    unsigned int index = get_vsc_hvdc_index(vsc_name);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_mt_hvdc_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_mt_hvdc_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_mt_hvdc_exist(const string mt_hvdc_name) const
{
    unsigned int index = get_mt_hvdc_index(mt_hvdc_name);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_equivalent_device_exist(const DEVICE_ID& device_id) const
{
    unsigned int index = get_equivalent_device_index(device_id);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_area_exist(const unsigned int no) const
{
    unsigned int index = get_area_index(no);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_zone_exist(const unsigned int no) const
{
    unsigned int index = get_zone_index(no);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_owner_exist(const unsigned int no) const
{
    unsigned int index = get_owner_index(no);
    if(index!=INDEX_NOT_EXIST) return true;
    else                       return false;
}

bool POWER_SYSTEM_DATABASE::is_device_exist(const DEVICE_ID& device_id) const
{
    switch(device_id.get_device_type())
    {
        case STEPS_BUS:
        {
            unsigned int bus = device_id.get_device_terminal()[0];
            return is_bus_exist(bus);
        }
        case STEPS_GENERATOR:
            return is_generator_exist(device_id);
        case STEPS_WT_GENERATOR:
            return is_wt_generator_exist(device_id);
        case STEPS_PV_UNIT:
            return is_pv_unit_exist(device_id);
        case STEPS_ENERGY_STORAGE:
            return is_energy_storage_exist(device_id);
        case STEPS_SV_SOURCE:
            return is_sv_source_exist(device_id);
        case STEPS_LOAD:
            return is_load_exist(device_id);
        case STEPS_AC_LINE:
            return is_ac_line_exist(device_id);
        case STEPS_TRANSFORMER:
            return is_transformer_exist(device_id);
        case STEPS_LCC_HVDC2T:
            return is_lcc_hvdc2t_exist(device_id);
        case STEPS_VSC_HVDC:
            return is_vsc_hvdc_exist(device_id);
        case STEPS_HYBRID_DC:
            return is_mt_hvdc_exist(device_id);
        case STEPS_EQUIVALENT_DEVICE:
            return is_equivalent_device_exist(device_id);
        default:
            return false;
    }
}

void POWER_SYSTEM_DATABASE::change_bus_number(unsigned int original_bus_number, unsigned int new_bus_number)
{
    ostringstream osstream;

    if(is_bus_exist(original_bus_number) and
       (not is_bus_exist(new_bus_number)) and
       is_bus_in_allowed_range(new_bus_number))
    {
        BUS* bus = get_pointer_to_ac_bus(original_bus_number);

        bus->set_bus_number(new_bus_number);

        bus_index.set_bus_with_index(new_bus_number, get_bus_index(original_bus_number));
        bus_index.set_bus_with_index(original_bus_number, INDEX_NOT_EXIST);

        vector<GENERATOR*> gens = get_generators_connecting_to_bus(original_bus_number);
        unsigned int ngens = gens.size();
        for(unsigned int i=0; i!=ngens; ++i)
        {
            GENERATOR* gen = gens[i];
            DEVICE_ID old_did = gen->get_device_id();
            unsigned int index = get_generator_index(old_did);
            generator_index.set_device_index(old_did, INDEX_NOT_EXIST);

            gen->set_generator_bus(new_bus_number);
            DEVICE_ID new_did = gen->get_device_id();
            generator_index.set_device_index(new_did, index);
        }

        vector<WT_GENERATOR*> wt_generators = get_wt_generators_connecting_to_bus(original_bus_number);
        unsigned int nwt_generator = wt_generators.size();
        for(unsigned int i=0; i!=nwt_generator; ++i)
        {
            WT_GENERATOR* wt_generator = wt_generators[i];
            DEVICE_ID old_did = wt_generator->get_device_id();
            unsigned int index = get_wt_generator_index(old_did);
            wt_generator_index.set_device_index(old_did, INDEX_NOT_EXIST);

            wt_generator->set_source_bus(new_bus_number);
            DEVICE_ID new_did = wt_generator->get_device_id();
            wt_generator_index.set_device_index(new_did, index);
        }

        vector<PV_UNIT*> pv_units = get_pv_units_connecting_to_bus(original_bus_number);
        unsigned int npv_unit = pv_units.size();
        for(unsigned int i=0; i!=npv_unit; ++i)
        {
            PV_UNIT* pv_unit = pv_units[i];
            DEVICE_ID old_did = pv_unit->get_device_id();
            unsigned int index = get_pv_unit_index(old_did);
            pv_unit_index.set_device_index(old_did, INDEX_NOT_EXIST);

            pv_unit->set_unit_bus(new_bus_number);
            DEVICE_ID new_did = pv_unit->get_device_id();
            pv_unit_index.set_device_index(new_did, index);
        }

        vector<ENERGY_STORAGE*> estorages = get_energy_storages_connecting_to_bus(original_bus_number);
        unsigned int nestorage = estorages.size();
        for(unsigned int i=0; i!=nestorage; ++i)
        {
            ENERGY_STORAGE* estorage = estorages[i];
            DEVICE_ID old_did = estorage->get_device_id();
            unsigned int index = get_energy_storage_index(old_did);
            energy_storage_index.set_device_index(old_did, INDEX_NOT_EXIST);

            estorage->set_energy_storage_bus(new_bus_number);
            DEVICE_ID new_did = estorage->get_device_id();
            energy_storage_index.set_device_index(new_did, index);
        }

        vector<SV_SOURCE*> sv_sources = get_sv_sources_connecting_to_bus(original_bus_number);
        unsigned int nsv_source = sv_sources.size();
        for(unsigned int i=0; i!=nsv_source; ++i)
        {
            SV_SOURCE* sv_source = sv_sources[i];
            DEVICE_ID old_did = sv_source->get_device_id();
            unsigned int index = get_sv_source_index(old_did);
            sv_source_index.set_device_index(old_did, INDEX_NOT_EXIST);

            sv_source->set_sv_source_bus(new_bus_number);
            DEVICE_ID new_did = sv_source->get_device_id();
            sv_source_index.set_device_index(new_did, index);
        }

        vector<LOAD*> loads = get_loads_connecting_to_bus(original_bus_number);
        unsigned int nload = loads.size();
        for(unsigned int i=0; i!=nload; ++i)
        {
            LOAD* load = loads[i];
            DEVICE_ID old_did = load->get_device_id();
            unsigned int index = get_load_index(old_did);
            load_index.set_device_index(old_did, INDEX_NOT_EXIST);

            load->set_load_bus(new_bus_number);
            DEVICE_ID new_did = load->get_device_id();
            load_index.set_device_index(new_did, index);
        }

        vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(original_bus_number);
        unsigned int nfshunt = fshunts.size();
        for(unsigned int i=0; i!=nfshunt; ++i)
        {
            FIXED_SHUNT* fshunt = fshunts[i];
            DEVICE_ID old_did = fshunt->get_device_id();
            unsigned int index = get_fixed_shunt_index(old_did);
            fixed_shunt_index.set_device_index(old_did, INDEX_NOT_EXIST);

            fshunt->set_shunt_bus(new_bus_number);
            DEVICE_ID new_did = fshunt->get_device_id();
            fixed_shunt_index.set_device_index(new_did, index);
        }

        vector<AC_LINE*> lines = get_ac_lines_connecting_to_bus(original_bus_number);
        unsigned int nline = lines.size();
        for(unsigned int i=0; i!=nline; ++i)
        {
            AC_LINE* line = lines[i];
            DEVICE_ID old_did = line->get_device_id();
            unsigned int index = get_ac_line_index(old_did);
            ac_line_index.set_device_index(old_did, INDEX_NOT_EXIST);

            if(line->get_sending_side_bus()==original_bus_number)
                line->set_sending_side_bus(new_bus_number);
            else
                line->set_receiving_side_bus(new_bus_number);

            DEVICE_ID new_did = line->get_device_id();
            ac_line_index.set_device_index(new_did, index);

        }

        vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_connecting_to_bus(original_bus_number);
        unsigned int nhvdc = hvdcs.size();
        for(unsigned int i=0; i!=nhvdc; ++i)
        {
            LCC_HVDC2T* hvdc = hvdcs[i];
            DEVICE_ID old_did = hvdc->get_device_id();
            unsigned int index = get_lcc_hvdc2t_index(old_did);
            lcc_hvdc2t_index.set_device_index(old_did, INDEX_NOT_EXIST);

            if(hvdc->get_converter_bus(RECTIFIER)==original_bus_number)
                hvdc->set_converter_bus(RECTIFIER, new_bus_number);
            else
                hvdc->set_converter_bus(INVERTER, new_bus_number);

            DEVICE_ID new_did = hvdc->get_device_id();
            lcc_hvdc2t_index.set_device_index(new_did, index);
        }

        vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(original_bus_number);
        unsigned int ntrans = transformers.size();
        for(unsigned int i=0; i!=ntrans; ++i)
        {
            TRANSFORMER* trans = transformers[i];
            DEVICE_ID old_did = trans->get_device_id();
            unsigned int index = get_transformer_index(old_did);
            transformer_index.set_device_index(old_did, INDEX_NOT_EXIST);

            if(trans->get_winding_bus(PRIMARY_SIDE)==original_bus_number)
                trans->set_winding_bus(PRIMARY_SIDE, new_bus_number);
            else
            {
                if(trans->get_winding_bus(SECONDARY_SIDE)==original_bus_number)
                    trans->set_winding_bus(SECONDARY_SIDE, new_bus_number);
                else
                    trans->set_winding_bus(TERTIARY_SIDE, new_bus_number);
            }

            DEVICE_ID new_did = trans->get_device_id();
            transformer_index.set_device_index(new_did, index);
        }

        vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(original_bus_number);
        unsigned int nvsc = vsc_hvdcs.size();
        for(unsigned int i=0; i!=nvsc; ++i)
        {
            VSC_HVDC* vsc_hvdc = vsc_hvdcs[i];
            DEVICE_ID old_did = vsc_hvdc->get_device_id();
            unsigned int index = get_vsc_hvdc_index(old_did);
            vsc_hvdc_index.set_device_index(old_did, INDEX_NOT_EXIST);

            unsigned int n_converter = vsc_hvdc->get_converter_count();
            for (unsigned int j=0; j!=n_converter; ++j)
            {
                if(vsc_hvdc->get_converter_ac_bus(j)==original_bus_number)
                {
                    vsc_hvdc->set_converter_ac_bus(j, new_bus_number);
                    break;
                }
            }

            DEVICE_ID new_did = vsc_hvdc->get_device_id();
            vsc_hvdc_index.set_device_index(new_did, index);
        }

        vector<MT_HVDC*> mt_hvdcs = get_mt_hvdcs_connecting_to_bus(original_bus_number);
        unsigned int n_mtdc = mt_hvdcs.size();
        for(unsigned int i=0; i!=n_mtdc; ++i)
        {
            MT_HVDC* mt_hvdc = mt_hvdcs[i];
            DEVICE_ID old_did = mt_hvdc->get_device_id();
            unsigned int index = get_mt_hvdc_index(old_did);
            mt_hvdc_index.set_device_index(old_did, INDEX_NOT_EXIST);

            vector<HLCC*> lccs = mt_hvdc->get_all_lcc_converters();
            unsigned int n_lcc = lccs.size();
            for(unsigned int j=0;j!=n_lcc;j++)
            {
                if(lccs[j]->get_converter_ac_bus()==original_bus_number)
                    lccs[j]->set_converter_ac_bus(new_bus_number);
            }

            vector<HVSC*> vscs = mt_hvdc->get_all_vsc_converters();
            unsigned int n_vsc = vscs.size();
            for(unsigned int j=0;j!=n_vsc;j++)
            {
                if(vscs[j]->get_converter_ac_bus()==original_bus_number)
                    vscs[j]->set_converter_ac_bus(new_bus_number);
            }

            DEVICE_ID new_did = mt_hvdc->get_device_id();
            vsc_hvdc_index.set_device_index(new_did, index);
        }


        vector<AREA*> areas = get_all_areas();
        unsigned int narea = areas.size();
        for(unsigned int i=0; i!=narea; ++i)
        {
            AREA* area = areas[i];
            if(area->get_area_swing_bus()==original_bus_number)
                area->set_area_swing_bus(new_bus_number);
        }
    }
    else
    {
        if(not is_bus_exist(original_bus_number))
            osstream<<"Warning. The original bus number to change ("<<original_bus_number<<") does not exist in the power system database. No bus number will be changed.";
        if(is_bus_exist(new_bus_number))
            osstream<<"Warning. The new bus number to change ("<<new_bus_number<<") already exists in the power system database. No bus number will be changed.";
        if(not is_bus_in_allowed_range(new_bus_number))
            osstream<<"Warning. The new bus number to change ("<<new_bus_number<<") exceeds the allowed maximum bus number range [1, "<<get_allowed_max_bus_number()<<"] in the power system database. No bus number will be changed.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void POWER_SYSTEM_DATABASE::change_bus_number_with_file(string file)
{
    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        ostringstream osstream;
        osstream<<"Bus number pair file '"<<file<<"' is not accessible. Change bus number with file is failed.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    const unsigned int buffer_size = 64;
    char buffer[buffer_size];
    string sbuffer;
    vector<string> splitted_str;
    while(true)
    {
        if(fgets(buffer, buffer_size, fid)==NULL)
            break;
        sbuffer = trim_string(buffer);
        splitted_str = split_string(sbuffer, ", ");
        if(splitted_str.size()==2)
        {
            unsigned int original_bus_number = str2int(splitted_str[0]);
            unsigned int new_bus_number = str2int(splitted_str[1]);
            change_bus_number(original_bus_number, new_bus_number);
        }
        else
            break;
    }
    fclose(fid);
}

DEVICE* POWER_SYSTEM_DATABASE::get_pointer_to_device(const DEVICE_ID& device_id) const
{
    // this function is not tested
    if(device_id.is_valid())
    {
        STEPS_DEVICE_TYPE dtype = device_id.get_device_type();
        if(dtype==STEPS_BUS)
            return get_pointer_to_ac_bus(device_id);
        else
            return get_pointer_to_nonbus_device(device_id);
    }
    else
        return NULL;
}

NONBUS_DEVICE* POWER_SYSTEM_DATABASE::get_pointer_to_nonbus_device(const DEVICE_ID& device_id) const
{
    // this function is not tested
    if(device_id.is_valid())
    {
        STEPS_DEVICE_TYPE dtype = device_id.get_device_type();
        if(dtype==STEPS_GENERATOR)
            return get_pointer_to_generator(device_id);

        if(dtype==STEPS_WT_GENERATOR)
            return get_pointer_to_wt_generator(device_id);

        if(dtype==STEPS_PV_UNIT)
            return get_pointer_to_pv_unit(device_id);

        if(dtype==STEPS_ENERGY_STORAGE)
            return get_pointer_to_energy_storage(device_id);

        if(dtype==STEPS_SV_SOURCE)
            return get_pointer_to_sv_source(device_id);

        if(dtype==STEPS_LOAD)
            return get_pointer_to_load(device_id);

        if(dtype==STEPS_FIXED_SHUNT)
            return get_pointer_to_fixed_shunt(device_id);

        if(dtype==STEPS_AC_LINE)
            return get_pointer_to_ac_line(device_id);

        if(dtype==STEPS_LCC_HVDC2T)
            return get_pointer_to_lcc_hvdc2t(device_id);

        if(dtype==STEPS_VSC_HVDC)
            return get_pointer_to_vsc_hvdc(device_id);

        if(dtype==STEPS_HYBRID_DC)
            return get_pointer_to_mt_hvdc(device_id);

        if(dtype==STEPS_TRANSFORMER)
            return get_pointer_to_transformer(device_id);

        if(dtype==STEPS_EQUIVALENT_DEVICE)
            return get_pointer_to_equivalent_device(device_id);

        return NULL;
    }
    else
        return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_pointer_to_ac_bus(unsigned int bus) const
{
    unsigned int index = bus_index.get_index_of_bus(bus);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<BUS*>(&(Bus[index]));
    else
        return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_pointer_to_ac_bus(const DEVICE_ID & device_id) const
{
    if(device_id.is_valid())
    {
        if(device_id.get_device_type()==STEPS_BUS)
        {
            TERMINAL terminal = device_id.get_device_terminal();
            unsigned int bus = terminal[0];
            return get_pointer_to_ac_bus(bus);
        }
        else
            return NULL;
    }
    else
        return NULL;
}

GENERATOR* POWER_SYSTEM_DATABASE::get_pointer_to_generator(const DEVICE_ID & device_id) const
{
    unsigned int index = generator_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<GENERATOR*>(&(Generator[index]));
    else
        return NULL;
}

WT_GENERATOR* POWER_SYSTEM_DATABASE::get_pointer_to_wt_generator(const DEVICE_ID & device_id) const
{
    unsigned int index = wt_generator_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<WT_GENERATOR*>(&(WT_Generator[index]));
    else
        return NULL;
}

PV_UNIT* POWER_SYSTEM_DATABASE::get_pointer_to_pv_unit(const DEVICE_ID & device_id) const
{
    unsigned int index = pv_unit_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<PV_UNIT*>(&(PV_Unit[index]));
    else
        return NULL;
}

ENERGY_STORAGE* POWER_SYSTEM_DATABASE::get_pointer_to_energy_storage(const DEVICE_ID & device_id) const
{
    unsigned int index = energy_storage_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<ENERGY_STORAGE*>(&(Energy_storage[index]));
    else
        return NULL;
}

SV_SOURCE* POWER_SYSTEM_DATABASE::get_pointer_to_sv_source(const DEVICE_ID & device_id) const
{
    unsigned int index = sv_source_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<SV_SOURCE*>(&(SV_source[index]));
    else
        return NULL;
}

SOURCE* POWER_SYSTEM_DATABASE::get_pointer_to_source(const DEVICE_ID & device_id) const
{
    switch(device_id.get_device_type())
    {
        case STEPS_GENERATOR:
            return (SOURCE*) get_pointer_to_generator(device_id);
        case STEPS_WT_GENERATOR:
            return (SOURCE*) get_pointer_to_wt_generator(device_id);
        case STEPS_PV_UNIT:
            return (SOURCE*) get_pointer_to_pv_unit(device_id);
        case STEPS_ENERGY_STORAGE:
            return (SOURCE*) get_pointer_to_energy_storage(device_id);
        case STEPS_SV_SOURCE:
            return (SOURCE*) get_pointer_to_sv_source(device_id);
        default:
            return NULL;
    }
}

LOAD* POWER_SYSTEM_DATABASE::get_pointer_to_load(const DEVICE_ID & device_id) const
{
    unsigned int index = load_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<LOAD*>(&(Load[index]));
    else
        return NULL;
}

FIXED_SHUNT* POWER_SYSTEM_DATABASE::get_pointer_to_fixed_shunt(const DEVICE_ID & device_id) const
{
    unsigned int index = fixed_shunt_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<FIXED_SHUNT*>(&(Fixed_shunt[index]));
    else
        return NULL;
}

AC_LINE* POWER_SYSTEM_DATABASE::get_pointer_to_ac_line(const DEVICE_ID & device_id) const
{
    unsigned int index = ac_line_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<AC_LINE*>(&(Ac_line[index]));
    else
        return NULL;
}

TRANSFORMER* POWER_SYSTEM_DATABASE::get_pointer_to_transformer(const DEVICE_ID & device_id) const
{
    unsigned int index = transformer_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<TRANSFORMER*>(&(Transformer[index]));
    else
        return NULL;
}

LCC_HVDC2T* POWER_SYSTEM_DATABASE::get_pointer_to_lcc_hvdc2t(const DEVICE_ID & device_id) const
{
    unsigned int index = lcc_hvdc2t_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<LCC_HVDC2T*>(&(Lcc_hvdc2t[index]));
    else
        return NULL;
}

VSC_HVDC* POWER_SYSTEM_DATABASE::get_pointer_to_vsc_hvdc(const DEVICE_ID & device_id) const
{
    unsigned int index = vsc_hvdc_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<VSC_HVDC*>(&(Vsc_hvdc[index]));
    else
        return NULL;
}

VSC_HVDC* POWER_SYSTEM_DATABASE::get_pointer_to_vsc_hvdc(string name) const
{
    unsigned int n = Vsc_hvdc.size();
    for(unsigned int index=0; index!=n; ++index)
    {
        if(Vsc_hvdc[index].get_name()==name)
            return const_cast<VSC_HVDC*>(&(Vsc_hvdc[index]));
    }
    return NULL;
}

MT_HVDC* POWER_SYSTEM_DATABASE::get_pointer_to_mt_hvdc(const DEVICE_ID & device_id) const
{
    unsigned int index = mt_hvdc_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<MT_HVDC*>(&(Mt_hvdc[index]));
    else
        return NULL;
}

MT_HVDC* POWER_SYSTEM_DATABASE::get_pointer_to_mt_hvdc(const string name) const
{
    unsigned int n = Mt_hvdc.size();
    for(unsigned int index=0; index!=n; ++index)
    {
        if(Mt_hvdc[index].get_name()==name)
            return const_cast<MT_HVDC*>(&(Mt_hvdc[index]));
    }
    return NULL;
}

EQUIVALENT_DEVICE* POWER_SYSTEM_DATABASE::get_pointer_to_equivalent_device(const DEVICE_ID & device_id) const
{
    unsigned int index = equivalent_device_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<EQUIVALENT_DEVICE*>(&(Equivalent_device[index]));
    else
        return NULL;
}

AREA* POWER_SYSTEM_DATABASE::get_pointer_to_area(const unsigned int no) const
{
    unsigned int index = get_area_index(no);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<AREA*>(&(Area[index]));
    else
        return NULL;
}

ZONE* POWER_SYSTEM_DATABASE::get_pointer_to_zone(const unsigned int no) const
{
    unsigned int index = get_zone_index(no);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<ZONE*>(&(Zone[index]));
    else
        return NULL;
}

OWNER* POWER_SYSTEM_DATABASE::get_pointer_to_owner(const unsigned int no) const
{
    unsigned int index = get_owner_index(no);
    if(index!=INDEX_NOT_EXIST)
        return const_cast<OWNER*>(&(Owner[index]));
    else
        return NULL;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_connecting_to_bus(const unsigned int bus)
{
    vector<DEVICE*> devices;
    unsigned int n=0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<AC_LINE*> lines = get_ac_lines_connecting_to_bus(bus);
    n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
    n = transes.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
    n = fshunts.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_connecting_to_bus(bus);
    n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);
    n = vsc_hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(vsc_hvdcs[i]);

    vector<MT_HVDC*> mt_hvdcs = get_mt_hvdcs_connecting_to_bus(bus);
    n = mt_hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(mt_hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
    n = edevices.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_connecting_to_bus(const unsigned int bus)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Generator[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_connecting_to_bus(const unsigned int bus)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_wt_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not WT_Generator[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}


vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_connecting_to_bus(const unsigned int bus)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    unsigned int n = get_pv_unit_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not PV_Unit[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_connecting_to_bus(const unsigned int bus)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    unsigned int n = get_energy_storage_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Energy_storage[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}

vector<SV_SOURCE*> POWER_SYSTEM_DATABASE::get_sv_sources_connecting_to_bus(const unsigned int bus)
{
    vector<SV_SOURCE*> device;
    device.reserve(8);

    unsigned int n = get_sv_source_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not SV_source[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(SV_source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_connecting_to_bus(const unsigned int bus)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    vector<SV_SOURCE*> sv_source_device;
    device.reserve(8);

    gen_device = get_generators_connecting_to_bus(bus);
    unsigned int ngens = gen_device.size();
    for(unsigned int i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_connecting_to_bus(bus);
    unsigned int nwt_generator = wt_generator_device.size();
    for(unsigned int i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_connecting_to_bus(bus);
    unsigned int npv_unit = pv_unit_device.size();
    for(unsigned int i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_connecting_to_bus(bus);
    unsigned int nes = estorage_device.size();
    for(unsigned int i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    sv_source_device = get_sv_sources_connecting_to_bus(bus);
    unsigned int nsvs = sv_source_device.size();
    for(unsigned int i=0; i!=nsvs; ++i)
        device.push_back(sv_source_device[i]);
    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_connecting_to_bus(const unsigned int bus)
{
    vector<LOAD*> device;
    device.reserve(8);

    unsigned int n = get_load_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Load[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_connecting_to_bus(const unsigned int bus)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    unsigned int n = get_fixed_shunt_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Fixed_shunt[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<AC_LINE*> POWER_SYSTEM_DATABASE::get_ac_lines_connecting_to_bus(const unsigned int bus)
{
    vector<AC_LINE*> device;
    device.reserve(8);

    unsigned int n = get_ac_line_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Ac_line[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Ac_line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_connecting_to_bus(const unsigned int bus)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    unsigned int n = get_transformer_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Transformer[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<LCC_HVDC2T*> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_connecting_to_bus(const unsigned int bus)
{
    vector<LCC_HVDC2T*> device;
    device.reserve(8);

    unsigned int n = get_lcc_hvdc2t_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Lcc_hvdc2t[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Lcc_hvdc2t[i]));
    }
    return device;
}

vector<VSC_HVDC*> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_connecting_to_bus(const unsigned int bus)
{
    vector<VSC_HVDC*> device;
    device.reserve(8);

    unsigned int n = get_vsc_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Vsc_hvdc[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Vsc_hvdc[i]));
    }
    return device;
}

vector<MT_HVDC*> POWER_SYSTEM_DATABASE::get_mt_hvdcs_connecting_to_bus(const unsigned int bus)
{
    vector<MT_HVDC*> device;
    device.reserve(8);

    unsigned int n = get_mt_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Mt_hvdc[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Mt_hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_connecting_to_bus(const unsigned int bus)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    unsigned int n = get_equivalent_device_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Equivalent_device[i].is_connected_to_bus(bus))
            continue;
        else
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<MUTUAL_DATA*> POWER_SYSTEM_DATABASE::get_mutual_data_with_line(const unsigned int ibus, const unsigned int jbus, string identifier)
{
    vector<MUTUAL_DATA*> device;
    device.reserve(4);
    unsigned int n = get_mutual_data_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Mutual_data[i].is_related_to_ac_line(ibus, jbus, identifier))
            device.push_back(&(Mutual_data[i]));
        else
            continue;
    }
    return device;
}


vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<DEVICE*> devices = get_all_devices_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<GENERATOR*> devices = get_generators_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<PV_UNIT*> devices = get_pv_units_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sv_sources_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<SV_SOURCE*> devices = get_sv_sources_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<SOURCE*> devices = get_sources_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<LOAD*> devices = get_loads_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_ac_lines_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<AC_LINE*> devices = get_ac_lines_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<TRANSFORMER*> devices = get_transformers_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<LCC_HVDC2T*> devices = get_lcc_hvdc2ts_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<VSC_HVDC*> devices = get_vsc_hvdcs_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_mt_hvdcs_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<MT_HVDC*> devices = get_mt_hvdcs_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_connecting_to_bus(const unsigned int bus)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_connecting_to_bus(bus);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_area(const unsigned int area)
{
    vector<DEVICE*> devices;
    unsigned int n=0;

    vector<BUS*> buses = get_buses_in_area(area);
    n = buses.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_area(area);
    n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_area(area);
    n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<AC_LINE*> lines = get_ac_lines_in_area(area);
    n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_area(area);
    n = transes.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_area(area);
    n = fshunts.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_in_area(area);
    n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_in_area(area);
    n = vsc_hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(vsc_hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_area(area);
    n = edevices.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_area(const unsigned int area)
{
    vector<BUS*> device;
    device.reserve(8);

    unsigned int n = get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Bus[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_area(const unsigned int area)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Generator[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_in_area(const unsigned int area)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_wt_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not WT_Generator[i].is_in_area(area))
            continue;
        else
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}


vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_in_area(const unsigned int area)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    unsigned int n = get_pv_unit_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not PV_Unit[i].is_in_area(area))
            continue;
        else
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_in_area(const unsigned int area)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    unsigned int n = get_energy_storage_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Energy_storage[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}

vector<SV_SOURCE*> POWER_SYSTEM_DATABASE::get_sv_sources_in_area(const unsigned int area)
{
    vector<SV_SOURCE*> device;
    device.reserve(8);

    unsigned int n = get_sv_source_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not SV_source[i].is_in_area(area))
            continue;
        else
            device.push_back(&(SV_source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_area(const unsigned int area)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    vector<SV_SOURCE*> sv_source_device;
    device.reserve(8);

    gen_device = get_generators_in_area(area);
    unsigned int ngens = gen_device.size();
    for(unsigned int i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_in_area(area);
    unsigned int nwt_generator = wt_generator_device.size();
    for(unsigned int i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_in_area(area);
    unsigned int npv_unit = pv_unit_device.size();
    for(unsigned int i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_in_area(area);
    unsigned int nes = estorage_device.size();
    for(unsigned int i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    sv_source_device = get_sv_sources_in_area(area);
    unsigned int nsvs = sv_source_device.size();
    for(unsigned int i=0; i!=nsvs; ++i)
        device.push_back(sv_source_device[i]);
    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_area(const unsigned int area)
{
    vector<LOAD*> device;
    device.reserve(8);

    unsigned int n = get_load_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Load[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_area(const unsigned int area)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    unsigned int n = get_fixed_shunt_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Fixed_shunt[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<AC_LINE*> POWER_SYSTEM_DATABASE::get_ac_lines_in_area(const unsigned int area)
{
    vector<AC_LINE*> device;
    device.reserve(8);

    unsigned int n = get_ac_line_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Ac_line[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Ac_line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_area(const unsigned int area)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    unsigned int n = get_transformer_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Transformer[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<LCC_HVDC2T*> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_in_area(const unsigned int area)
{
    vector<LCC_HVDC2T*> device;
    device.reserve(8);

    unsigned int n = get_lcc_hvdc2t_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Lcc_hvdc2t[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Lcc_hvdc2t[i]));
    }
    return device;
}

vector<VSC_HVDC*> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_in_area(const unsigned int area)
{
    vector<VSC_HVDC*> device;
    device.reserve(8);

    unsigned int n = get_vsc_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Vsc_hvdc[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Vsc_hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_area(const unsigned int area)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    unsigned int n = get_equivalent_device_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Equivalent_device[i].is_in_area(area))
            continue;
        else
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_area(const unsigned int area)
{
    vector<DEVICE*> devices = get_all_devices_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_area(const unsigned int area)
{
    vector<BUS*> devices = get_buses_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_area(const unsigned int area)
{
    vector<GENERATOR*> devices = get_generators_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_in_area(const unsigned int area)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_in_area(const unsigned int area)
{
    vector<PV_UNIT*> devices = get_pv_units_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_in_area(const unsigned int area)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sv_sources_device_id_in_area(const unsigned int area)
{
    vector<SV_SOURCE*> devices = get_sv_sources_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_area(const unsigned int area)
{
    vector<SOURCE*> devices = get_sources_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_area(const unsigned int area)
{
    vector<LOAD*> devices = get_loads_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_area(const unsigned int area)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_ac_lines_device_id_in_area(const unsigned int area)
{
    vector<AC_LINE*> devices = get_ac_lines_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_area(const unsigned int area)
{
    vector<TRANSFORMER*> devices = get_transformers_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_device_id_in_area(const unsigned int area)
{
    vector<LCC_HVDC2T*> devices = get_lcc_hvdc2ts_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_device_id_in_area(const unsigned int area)
{
    vector<VSC_HVDC*> devices = get_vsc_hvdcs_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_area(const unsigned int area)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_area(area);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_zone(const unsigned int zone)
{
    vector<DEVICE*> devices;
    unsigned int n=0;

    vector<BUS*> buses = get_buses_in_zone(zone);
    n = buses.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_zone(zone);
    n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_zone(zone);
    n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<AC_LINE*> lines = get_ac_lines_in_zone(zone);
    n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_zone(zone);
    n = transes.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_zone(zone);
    n = fshunts.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_in_zone(zone);
    n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_in_zone(zone);
    n = vsc_hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(vsc_hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_zone(zone);
    n = edevices.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_zone(const unsigned int zone)
{
    vector<BUS*> device;
    device.reserve(8);

    unsigned int n = get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Bus[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_zone(const unsigned int zone)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Generator[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_wt_generators_in_zone(const unsigned int zone)
{
    vector<WT_GENERATOR*> device;
    device.reserve(8);

    unsigned int n = get_wt_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not WT_Generator[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(WT_Generator[i]));
    }
    return device;
}

vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_pv_units_in_zone(const unsigned int zone)
{
    vector<PV_UNIT*> device;
    device.reserve(8);

    unsigned int n = get_pv_unit_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not PV_Unit[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(PV_Unit[i]));
    }
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_energy_storages_in_zone(const unsigned int zone)
{
    vector<ENERGY_STORAGE*> device;
    device.reserve(8);

    unsigned int n = get_energy_storage_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Energy_storage[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Energy_storage[i]));
    }
    return device;
}

vector<SV_SOURCE*> POWER_SYSTEM_DATABASE::get_sv_sources_in_zone(const unsigned int zone)
{
    vector<SV_SOURCE*> device;
    device.reserve(8);

    unsigned int n = get_sv_source_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not SV_source[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(SV_source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_zone(const unsigned int zone)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    vector<SV_SOURCE*> sv_source_device;
    device.reserve(8);

    gen_device = get_generators_in_zone(zone);
    unsigned int ngens = gen_device.size();
    for(unsigned int i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_wt_generators_in_zone(zone);
    unsigned int nwt_generator = wt_generator_device.size();
    for(unsigned int i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_pv_units_in_zone(zone);
    unsigned int npv_unit = pv_unit_device.size();
    for(unsigned int i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_energy_storages_in_zone(zone);
    unsigned int nes = estorage_device.size();
    for(unsigned int i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    sv_source_device = get_sv_sources_in_zone(zone);
    unsigned int nsvs = sv_source_device.size();
    for(unsigned int i=0; i!=nsvs; ++i)
        device.push_back(sv_source_device[i]);
    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_zone(const unsigned int zone)
{
    vector<LOAD*> device;
    device.reserve(8);

    unsigned int n = get_load_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Load[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_zone(const unsigned int zone)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    unsigned int n = get_fixed_shunt_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Fixed_shunt[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<AC_LINE*> POWER_SYSTEM_DATABASE::get_ac_lines_in_zone(const unsigned int zone)
{
    vector<AC_LINE*> device;
    device.reserve(8);

    unsigned int n = get_ac_line_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Ac_line[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Ac_line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_zone(const unsigned int zone)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    unsigned int n = get_transformer_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Transformer[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<LCC_HVDC2T*> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_in_zone(const unsigned int zone)
{
    vector<LCC_HVDC2T*> device;
    device.reserve(8);

    unsigned int n = get_lcc_hvdc2t_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Lcc_hvdc2t[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Lcc_hvdc2t[i]));
    }
    return device;
}

vector<VSC_HVDC*> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_in_zone(const unsigned int zone)
{
    vector<VSC_HVDC*> device;
    device.reserve(8);

    unsigned int n = get_vsc_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Vsc_hvdc[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Vsc_hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_zone(const unsigned int zone)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    unsigned int n = get_equivalent_device_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Equivalent_device[i].is_in_zone(zone))
            continue;
        else
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_zone(const unsigned int zone)
{
    vector<DEVICE*> devices = get_all_devices_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_zone(const unsigned int zone)
{
    vector<BUS*> devices = get_buses_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_zone(const unsigned int zone)
{
    vector<GENERATOR*> devices = get_generators_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_wt_generators_device_id_in_zone(const unsigned int zone)
{
    vector<WT_GENERATOR*> devices = get_wt_generators_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pv_units_device_id_in_zone(const unsigned int zone)
{
    vector<PV_UNIT*> devices = get_pv_units_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_energy_storages_device_id_in_zone(const unsigned int zone)
{
    vector<ENERGY_STORAGE*> devices = get_energy_storages_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sv_sources_device_id_in_zone(const unsigned int zone)
{
    vector<SV_SOURCE*> devices = get_sv_sources_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_zone(const unsigned int zone)
{
    vector<SOURCE*> devices = get_sources_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_zone(const unsigned int zone)
{
    vector<LOAD*> devices = get_loads_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_zone(const unsigned int zone)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_ac_lines_device_id_in_zone(const unsigned int zone)
{
    vector<AC_LINE*> devices = get_ac_lines_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_zone(const unsigned int zone)
{
    vector<TRANSFORMER*> devices = get_transformers_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lcc_hvdc2ts_device_id_in_zone(const unsigned int zone)
{
    vector<LCC_HVDC2T*> devices = get_lcc_hvdc2ts_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_vsc_hvdcs_device_id_in_zone(const unsigned int zone)
{
    vector<VSC_HVDC*> devices = get_vsc_hvdcs_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_zone(const unsigned int zone)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_zone(zone);
    unsigned int n = devices.size();

    vector<DEVICE_ID> dids;
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}


vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices()
{
    vector<DEVICE*> devices;
    unsigned int n=0;

    vector<BUS*> buses = get_all_buses();
    n = buses.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_all_sources();
    n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_all_loads();
    n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<FIXED_SHUNT*> fshunts = get_all_fixed_shunts();
    n = fshunts.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<AC_LINE*> lines = get_all_ac_lines();
    n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_all_transformers();
    n = transes.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<LCC_HVDC2T*> hvdcs = get_all_lcc_hvdc2ts();
    n = hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<VSC_HVDC*> vsc_hvdcs = get_all_vsc_hvdcs();
    n = vsc_hvdcs.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(vsc_hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_all_equivalent_devices();
    n = edevices.size();
    for(unsigned int i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_all_buses()
{
    vector<BUS*> device;
    unsigned int n = get_bus_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Bus[i]));
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner)
{
    vector<BUS*> device;
    unsigned int n = get_bus_count();
    device.reserve(n);
    double vbase=0.0, v=0.0;
    unsigned int this_area=0, this_zone=0, this_owner=0;
    for(unsigned int i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_positive_sequence_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if((area!=0 and this_area!=area) or
           (zone!=0 and this_zone!=zone) or
           (owner!=0 and this_owner!=owner))
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_all_in_service_buses()
{
    vector<BUS*> device;
    unsigned int n = get_bus_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_all_generators()
{
    vector<GENERATOR*> device;
    unsigned int n = get_generator_count();
    device.reserve(n);

    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Generator[i]));
    return device;
}

vector<WT_GENERATOR*> POWER_SYSTEM_DATABASE::get_all_wt_generators()
{
    vector<WT_GENERATOR*> device;
    unsigned int n = get_wt_generator_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(WT_Generator[i]));
    return device;
}

vector<PV_UNIT*> POWER_SYSTEM_DATABASE::get_all_pv_units()
{
    vector<PV_UNIT*> device;
    unsigned int n = get_pv_unit_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(PV_Unit[i]));
    return device;
}

vector<ENERGY_STORAGE*> POWER_SYSTEM_DATABASE::get_all_energy_storages()
{
    vector<ENERGY_STORAGE*> device;
    unsigned int n = get_energy_storage_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Energy_storage[i]));
    return device;
}

vector<SV_SOURCE*> POWER_SYSTEM_DATABASE::get_all_sv_sources()
{
    vector<SV_SOURCE*> device;
    unsigned int n = get_sv_source_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(SV_source[i]));
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_all_sources()
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<WT_GENERATOR*> wt_generator_device;
    vector<PV_UNIT*> pv_unit_device;
    vector<ENERGY_STORAGE*> estorage_device;
    vector<SV_SOURCE*> sv_source_device;
    device.reserve(get_source_count());

    gen_device = get_all_generators();
    unsigned int ngens = gen_device.size();
    for(unsigned int i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    wt_generator_device = get_all_wt_generators();
    unsigned int nwt_generator = wt_generator_device.size();
    for(unsigned int i=0; i!=nwt_generator; ++i)
        device.push_back(wt_generator_device[i]);

    pv_unit_device = get_all_pv_units();
    unsigned int npv_unit = pv_unit_device.size();
    for(unsigned int i=0; i!=npv_unit; ++i)
        device.push_back(pv_unit_device[i]);

    estorage_device = get_all_energy_storages();
    unsigned int nes = estorage_device.size();
    for(unsigned int i=0; i!=nes; ++i)
        device.push_back(estorage_device[i]);

    sv_source_device = get_all_sv_sources();
    unsigned int nsvs = sv_source_device.size();
    for(unsigned int i=0; i!=nsvs; ++i)
        device.push_back(sv_source_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_all_loads()
{
    vector<LOAD*> device;
    unsigned int n = get_load_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Load[i]));
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_all_fixed_shunts()
{
    vector<FIXED_SHUNT*> device;
    unsigned int n = get_fixed_shunt_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Fixed_shunt[i]));
    return device;
}

vector<AC_LINE*> POWER_SYSTEM_DATABASE::get_all_ac_lines()
{
    vector<AC_LINE*> device;
    unsigned int n = get_ac_line_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Ac_line[i]));
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_all_transformers()
{
    vector<TRANSFORMER*> device;
    unsigned int n = get_transformer_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Transformer[i]));
    return device;
}

vector<LCC_HVDC2T*> POWER_SYSTEM_DATABASE::get_all_lcc_hvdc2ts()
{
    vector<LCC_HVDC2T*> device;
    unsigned int n = get_lcc_hvdc2t_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Lcc_hvdc2t[i]));
    return device;
}

vector<VSC_HVDC*> POWER_SYSTEM_DATABASE::get_all_vsc_hvdcs()
{
    vector<VSC_HVDC*> device;
    unsigned int n = get_vsc_hvdc_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Vsc_hvdc[i]));
    return device;
}

vector<MT_HVDC*> POWER_SYSTEM_DATABASE::get_all_mt_hvdcs()
{
    vector<MT_HVDC*> device;
    unsigned int n = get_mt_hvdc_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Mt_hvdc[i]));
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_all_equivalent_devices()
{
    vector<EQUIVALENT_DEVICE*> device;
    unsigned int n = get_equivalent_device_count();
    device.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        device.push_back(&(Equivalent_device[i]));
    return device;
}

vector<AREA*> POWER_SYSTEM_DATABASE::get_all_areas()
{
    vector<AREA*> areas;
    areas.reserve(get_area_count());

    unsigned int n = get_area_count();
    for(unsigned int i=0; i!=n; ++i)
        areas.push_back(&(Area[i]));
    return areas;
}

vector<ZONE*> POWER_SYSTEM_DATABASE::get_all_zones()
{
    vector<ZONE*> zones;
    zones.reserve(get_zone_count());

    unsigned int n = get_zone_count();
    for(unsigned int i=0; i!=n; ++i)
        zones.push_back(&(Zone[i]));
    return zones;
}

vector<OWNER*> POWER_SYSTEM_DATABASE::get_all_owners()
{
    vector<OWNER*> owners;
    owners.reserve(get_owner_count());

    unsigned int n = get_owner_count();
    for(unsigned int i=0; i!=n; ++i)
        owners.push_back(&(Owner[i]));
    return owners;
}

vector<MUTUAL_DATA*> POWER_SYSTEM_DATABASE::get_all_mutual_data()
{
    vector <MUTUAL_DATA*> mutuals;
    mutuals.reserve(get_mutual_data_count());

    unsigned int n = get_mutual_data_count();
    for(unsigned int i=0; i!=n; ++i)
        mutuals.push_back(&(Mutual_data[i]));
    return mutuals;
}



vector<unsigned int> POWER_SYSTEM_DATABASE::get_all_buses_number()
{
    vector<unsigned int> buses_number;
    unsigned int n=get_bus_count();
    buses_number.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        buses_number.push_back(Bus[i].get_bus_number());
    return buses_number;
}

vector<unsigned int> POWER_SYSTEM_DATABASE::get_buses_number_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner)
{
    vector<unsigned int> buses_number;
    unsigned int n = get_bus_count();
    buses_number.reserve(n);
    double vbase=0.0, v=0.0;
    unsigned int this_area=0, this_zone=0, this_owner=0;
    for(unsigned int i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_positive_sequence_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if((area!=0 and this_area!=area) or
           (zone!=0 and this_zone!=zone) or
           (owner!=0 and this_owner!=owner))
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            buses_number.push_back(Bus[i].get_bus_number());
    }
    return buses_number;
}

vector<unsigned int> POWER_SYSTEM_DATABASE::get_all_in_service_buses_number()
{
    vector<unsigned int> buses_number;
    unsigned int n=get_bus_count();
    buses_number.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            buses_number.push_back(Bus[i].get_bus_number());
        else
            continue;
    }
    return buses_number;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_buses_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_bus_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Bus[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_generators_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_generator_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Generator[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_wt_generators_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_wt_generator_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(WT_Generator[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_pv_units_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_pv_unit_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(PV_Unit[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_energy_storages_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_energy_storage_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Energy_storage[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_sv_sources_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_sv_source_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(SV_source[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_sources_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_source_count();
    dids.reserve(n);
    unsigned int ngens = get_generator_count();
    for(unsigned int i=0; i!=ngens; ++i)
        dids.push_back(Generator[i].get_device_id());

    unsigned int nwt_generator = get_wt_generator_count();
    for(unsigned int i=0; i!=nwt_generator; ++i)
        dids.push_back(WT_Generator[i].get_device_id());

    unsigned int npv_unit = get_pv_unit_count();
    for(unsigned int i=0; i!=npv_unit; ++i)
        dids.push_back(PV_Unit[i].get_device_id());

    unsigned int nes = get_energy_storage_count();
    for(unsigned int i=0; i!=nes; ++i)
        dids.push_back(Energy_storage[i].get_device_id());

    unsigned int nsvs = get_sv_source_count();
    for(unsigned int i=0; i!=nsvs; ++i)
        dids.push_back(SV_source[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_loads_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_load_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Load[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_fixed_shunts_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_fixed_shunt_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Fixed_shunt[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_ac_lines_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_ac_line_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Ac_line[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_transformers_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_transformer_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Transformer[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_lcc_hvdc2ts_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_lcc_hvdc2t_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Lcc_hvdc2t[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_vsc_hvdcs_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_vsc_hvdc_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Vsc_hvdc[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_mt_hvdcs_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n= get_mt_hvdc_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Mt_hvdc[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_equivalent_devices_device_id()
{
    vector<DEVICE_ID> dids;
    unsigned int n=get_equivalent_device_count();
    dids.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        dids.push_back(Equivalent_device[i].get_device_id());
    return dids;
}

vector<unsigned int> POWER_SYSTEM_DATABASE::get_all_areas_number()
{
    vector<unsigned int> areas_number;
    unsigned int n=get_area_count();
    areas_number.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        areas_number.push_back(Area[i].get_area_number());
    return areas_number;
}

vector<unsigned int> POWER_SYSTEM_DATABASE::get_all_zones_number()
{
    vector<unsigned int> zones_number;
    unsigned int n=get_zone_count();
    zones_number.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        zones_number.push_back(Zone[i].get_zone_number());
    return zones_number;
}

vector<unsigned int> POWER_SYSTEM_DATABASE::get_all_owners_number()
{
    vector<unsigned int> owners_number;
    unsigned int n=get_owner_count();
    owners_number.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
        owners_number.push_back(Owner[i].get_owner_number());
    return owners_number;
}

unsigned int POWER_SYSTEM_DATABASE::get_in_service_bus_count()
{
    if(in_service_bus_count == 0)
        update_in_service_bus_count();
    return in_service_bus_count;
}

unsigned int POWER_SYSTEM_DATABASE::get_overshadowed_bus_count()
{
    if(overshadowed_bus_count == 0)
        update_overshadowed_bus_count();
    return overshadowed_bus_count;
}

void POWER_SYSTEM_DATABASE::update_in_service_bus_count()
{
    unsigned int n = Bus.size();
    unsigned int n_out_of_service = 0;
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            continue;
        else
			++n_out_of_service;
    }

    in_service_bus_count = n-n_out_of_service;
}

void POWER_SYSTEM_DATABASE::set_all_buses_un_overshadowed()
{
    unsigned int n = Bus.size();
    for(unsigned int i=0; i!=n; ++i)
        Bus[i].set_equivalent_bus_number(0);
}

void POWER_SYSTEM_DATABASE::update_overshadowed_bus_count()
{
    set_all_buses_un_overshadowed();

    overshadowed_bus_count = 0;
    unsigned int n = Ac_line.size();
    while(true)
    {
        bool new_bus_is_overshadowed = false;
        for(unsigned int i=0; i!=n; ++i)
        {
            if(not Ac_line[i].is_zero_impedance_line())
                continue;
            else
            {
                bool istatus = Ac_line[i].get_sending_side_breaker_status();
                bool jstatus = Ac_line[i].get_receiving_side_breaker_status();
                if(istatus!=false and jstatus!=false)
                {
                    unsigned int ibus = Ac_line[i].get_sending_side_bus();
                    unsigned int jbus = Ac_line[i].get_receiving_side_bus();

                    unsigned int iequvilent_bus = get_equivalent_bus_of_bus(ibus);
                    unsigned int jequvilent_bus = get_equivalent_bus_of_bus(jbus);

                    if(iequvilent_bus==0)
                        iequvilent_bus = ibus;
                    if(jequvilent_bus==0)
                        jequvilent_bus = jbus;

                    if(iequvilent_bus==jequvilent_bus)
                        continue;
                    else
                    {
                        if(iequvilent_bus<jequvilent_bus)
                        {
                            BUS* busptr = get_pointer_to_ac_bus(jbus);
                            busptr->set_equivalent_bus_number(iequvilent_bus);
                        }
                        else
                        {
                            BUS* busptr = get_pointer_to_ac_bus(ibus);
                            busptr->set_equivalent_bus_number(jequvilent_bus);
                        }
                        if(new_bus_is_overshadowed==false)
                            new_bus_is_overshadowed = true;
                    }
                }
                else
                    continue;
            }
        }
        if(new_bus_is_overshadowed==false)
            break;
    }

    n = Bus.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(not Bus[i].is_bus_overshadowed())
            continue;
        else
            ++overshadowed_bus_count;
    }
}

unsigned int POWER_SYSTEM_DATABASE::get_equivalent_bus_of_bus(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_equivalent_bus_number();
    else
        return 0;
}

unsigned int POWER_SYSTEM_DATABASE::get_source_count() const
{
    unsigned int n = 0;
    n += get_generator_count();
    n += get_wt_generator_count();
    n += get_pv_unit_count();
    n += get_energy_storage_count();
    n += get_sv_source_count();
    return n;
}

unsigned int POWER_SYSTEM_DATABASE::get_lcc_hvdc2t_count() const
{
    return Lcc_hvdc2t.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_vsc_hvdc_count() const
{
    return Vsc_hvdc.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_mt_hvdc_count() const
{
    return Mt_hvdc.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_equivalent_device_count() const
{
    return Equivalent_device.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_area_count() const
{
    return Area.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_zone_count() const
{
    return Zone.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_owner_count() const
{
    return Owner.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_mutual_data_count() const
{
    return Mutual_data.size();
}

unsigned int POWER_SYSTEM_DATABASE::get_bus_index(unsigned int bus) const
{
    return bus_index.get_index_of_bus(bus);
}

unsigned int POWER_SYSTEM_DATABASE::get_generator_index(const DEVICE_ID & device_id) const
{
    return generator_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_wt_generator_index(const DEVICE_ID & device_id) const
{
    return wt_generator_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_pv_unit_index(const DEVICE_ID & device_id) const
{
    return pv_unit_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_energy_storage_index(const DEVICE_ID & device_id) const
{
    return energy_storage_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_sv_source_index(const DEVICE_ID & device_id) const
{
    return sv_source_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_load_index(const DEVICE_ID & device_id) const
{
    return load_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_fixed_shunt_index(const DEVICE_ID & device_id) const
{
    return fixed_shunt_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_ac_line_index(const DEVICE_ID & device_id) const
{
    return ac_line_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_transformer_index(const DEVICE_ID & device_id) const
{
    return transformer_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_lcc_hvdc2t_index(const DEVICE_ID & device_id) const
{
    return lcc_hvdc2t_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_vsc_hvdc_index(const DEVICE_ID & device_id) const
{
    return vsc_hvdc_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_vsc_hvdc_index(const string vsc_name) const
{
    return vsc_hvdc_index.get_index_of_device(vsc_name);
}

unsigned int POWER_SYSTEM_DATABASE::get_mt_hvdc_index(const DEVICE_ID & device_id) const
{
    return mt_hvdc_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_mt_hvdc_index(const string mt_hvdc_name) const
{

    unsigned int n = Mt_hvdc.size();
    for(unsigned int index=0;index!=n;index++)
    {
        if(Mt_hvdc[index].get_name()==mt_hvdc_name)
            return mt_hvdc_index.get_index_of_device(Mt_hvdc[index].get_device_id());
    }
    return INDEX_NOT_EXIST;
}

unsigned int POWER_SYSTEM_DATABASE::get_equivalent_device_index(const DEVICE_ID & device_id) const
{
    return equivalent_device_index.get_index_of_device(device_id);
}

unsigned int POWER_SYSTEM_DATABASE::get_area_index(const unsigned int no) const
{
    map<unsigned int, unsigned int>::const_iterator iter = area_index.begin();
    iter = area_index.find(no);
    if(iter!=area_index.end()) return iter->second;
    else                       return INDEX_NOT_EXIST;
}

unsigned int POWER_SYSTEM_DATABASE::get_zone_index(const unsigned int no) const
{
    map<unsigned int, unsigned int>::const_iterator iter = zone_index.begin();
    iter = zone_index.find(no);
    if(iter!=zone_index.end()) return iter->second;
    else                       return INDEX_NOT_EXIST;
}

unsigned int POWER_SYSTEM_DATABASE::get_owner_index(const unsigned int no) const
{
    map<unsigned int, unsigned int>::const_iterator iter = owner_index.begin();
    iter = owner_index.find(no);
    if(iter!=owner_index.end()) return iter->second;
    else                        return INDEX_NOT_EXIST;
}

unsigned int POWER_SYSTEM_DATABASE::bus_name2bus_number(const string&name, double vbase) const
{
    string trimmed_name = trim_string(name);
    unsigned int n = get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_name()!=trimmed_name)
            continue;
        else
        {
            if(vbase==0.0 or Bus[i].get_base_voltage_in_kV()==vbase)
                return Bus[i].get_bus_number();
        }

    }
    return 0;
}

unsigned int POWER_SYSTEM_DATABASE::area_name2area_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    unsigned int n = get_area_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Area[i].get_area_name()!=trimmed_name)
            continue;
        else
            return Area[i].get_area_number();
    }
    return 0;
}

unsigned int POWER_SYSTEM_DATABASE::zone_name2zone_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    unsigned int n = get_zone_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Zone[i].get_zone_name()!=trimmed_name)
            continue;
        else
            return Zone[i].get_zone_number();
    }
    return 0;
}

unsigned int POWER_SYSTEM_DATABASE::owner_name2owner_number(const string& name) const
{
    string trimmed_name = trim_string(name);
    unsigned int n = get_owner_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Owner[i].get_owner_name()!=trimmed_name)
            continue;
        else
            return Owner[i].get_owner_number();
    }
    return 0;
}

string POWER_SYSTEM_DATABASE::bus_number2bus_name(unsigned int number)
{
    if(is_bus_in_allowed_range(number))
    {
        BUS* bus = get_pointer_to_ac_bus(number);
        if(bus!=NULL)
            return bus->get_bus_name();
        else
            return "";
    }
    else
        return "";
}


string POWER_SYSTEM_DATABASE::area_number2area_name(unsigned int number)
{
    AREA* area = get_pointer_to_area(number);
    if(area!=NULL)
        return area->get_area_name();
    else
        return "";
}

string POWER_SYSTEM_DATABASE::zone_number2zone_name(unsigned int number)
{
    ZONE* zone = get_pointer_to_zone(number);
    if(zone!=NULL)
        return zone->get_zone_name();
    else
        return "";
}

string POWER_SYSTEM_DATABASE::owner_number2owner_name(unsigned int number)
{
    OWNER* owner = get_pointer_to_owner(number);
    if(owner!=NULL)
        return owner->get_owner_name();
    else
        return "";
}

string POWER_SYSTEM_DATABASE::get_bus_extended_name(unsigned int bus)
{
    ostringstream osstream;
    osstream<<"BUS "<<bus<<get_bus_name_and_base_voltage_in_square_brackets(bus);
    return osstream.str();
}

string POWER_SYSTEM_DATABASE::get_device_extended_name(DEVICE_ID did)
{
    if(did.get_device_type() != STEPS_BUS)
    {
        ostringstream osstream;
        osstream<<did.get_compound_device_name();
        TERMINAL terminal = did.get_device_terminal();
        unsigned int n = terminal.get_bus_count();
        if(n==1)
            osstream<<get_bus_name_and_base_voltage_in_square_brackets(terminal[0]);
        else
        {
            osstream<<"{";
            for(unsigned int i=0; i<n; ++i)
            {
                unsigned int bus = terminal[i];
                osstream<<get_bus_name_and_base_voltage_in_square_brackets(bus);
                if(i+1<n)
                    osstream<<"--";
            }
            osstream<<"}";
        }
        return osstream.str();
    }
    else
    {
        unsigned int bus = did.get_device_terminal()[0];
        return get_bus_extended_name(bus);
    }
}

string POWER_SYSTEM_DATABASE::get_bus_name_and_base_voltage_in_square_brackets(unsigned int bus)
{
    ostringstream osstream;
    osstream<<"["<<bus_number2bus_name(bus)<<" "<<get_bus_base_voltage_in_kV(bus)<<"kV]";
    return osstream.str();
}

void POWER_SYSTEM_DATABASE::check_powerflow_data()
{
    check_all_devices();
    check_all_areas();
    check_all_zones();
    check_all_owners();
}

void POWER_SYSTEM_DATABASE::check()
{
    check_powerflow_data();
}

void POWER_SYSTEM_DATABASE::check_all_devices()
{
    check_all_buses();
    check_all_sources();
    check_all_loads();
    check_all_fixed_shunts();
    check_all_ac_lines();
    check_all_transformers();
    check_all_lcc_hvdc2ts();
    check_all_vsc_hvdcs();
    check_all_mt_hvdcs();
    check_all_equivalent_devices();
}

void POWER_SYSTEM_DATABASE::check_all_buses()
{
    unsigned int n = get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
        Bus[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_sources()
{
    check_all_generators();
    check_all_wt_generators();
    check_all_pv_units();
    check_all_energy_storages();
    check_all_sv_sources();
}

void POWER_SYSTEM_DATABASE::check_all_generators()
{
    unsigned int n = get_generator_count();
    for(unsigned int i=0; i!=n; ++i)
        Generator[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_wt_generators()
{
    unsigned int n = get_wt_generator_count();
    for(unsigned int i=0; i!=n; ++i)
        WT_Generator[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_pv_units()
{
    unsigned int n = get_pv_unit_count();
    for(unsigned int i=0; i!=n; ++i)
        PV_Unit[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_energy_storages()
{
    unsigned int n = get_energy_storage_count();
    for(unsigned int i=0; i!=n; ++i)
        Energy_storage[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_sv_sources()
{
    unsigned int n = get_sv_source_count();
    for(unsigned int i=0; i!=n; ++i)
        SV_source[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_loads()
{
    unsigned int n = get_load_count();
    for(unsigned int i=0; i!=n; ++i)
        Load[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_fixed_shunts()
{
    unsigned int n = get_fixed_shunt_count();
    for(unsigned int i=0; i!=n; ++i)
        Fixed_shunt[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_ac_lines()
{
    unsigned int n = get_ac_line_count();
    for(unsigned int i=0; i!=n; ++i)
        Ac_line[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_transformers()
{
    unsigned int n = get_transformer_count();
    for(unsigned int i=0; i!=n; ++i)
        Transformer[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_lcc_hvdc2ts()
{
    unsigned int n = get_lcc_hvdc2t_count();
    for(unsigned int i=0; i!=n; ++i)
        Lcc_hvdc2t[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_vsc_hvdcs()
{
    unsigned int n = get_vsc_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
        Vsc_hvdc[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_mt_hvdcs()
{
    unsigned int n = get_mt_hvdc_count();
    for(unsigned int i=0; i!=n; ++i)
        Mt_hvdc[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_equivalent_devices()
{
    unsigned int n = get_equivalent_device_count();
    for(unsigned int i=0; i!=n; ++i)
        Equivalent_device[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_areas()
{
    unsigned int n = get_area_count();
    for(unsigned int i=0; i!=n; ++i)
        Area[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_zones()
{
    unsigned int n = get_zone_count();
    for(unsigned int i=0; i!=n; ++i)
        Zone[i].check();
}

void POWER_SYSTEM_DATABASE::check_all_owners()
{
    unsigned int n = get_owner_count();
    for(unsigned int i=0; i!=n; ++i)
        Owner[i].check();
}


void POWER_SYSTEM_DATABASE::check_dynamic_data()
{
    check_generator_related_dynamic_data();
    check_wt_generator_related_dynamic_data();
    check_pv_unit_related_dynamic_data();
    check_sv_source_related_dynamic_data();
    check_energy_storage_related_dynamic_data();
    check_load_related_dynamic_data();
    check_ac_line_related_dynamic_data();
    check_lcc_hvdc2t_related_dynamic_data();
    check_equivalent_device_related_dynamic_data();
}

void POWER_SYSTEM_DATABASE::check_generator_related_dynamic_data()
{
    ostringstream osstream;
    vector<GENERATOR*> generators = get_all_generators();
    unsigned int n = generators.size();
    GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
        if(genmodel!=NULL)
            genmodel->check();

        EXCITER_MODEL* avrmodel = generator->get_exciter_model();
        if(avrmodel!=NULL)
            avrmodel->check();

        STABILIZER_MODEL* pssmodel = generator->get_stabilizer_model();
        if(pssmodel!=NULL)
            pssmodel->check();

        COMPENSATOR_MODEL* compmodel = generator->get_compensator_model();
        if(compmodel!=NULL)
            compmodel->check();

        TURBINE_GOVERNOR_MODEL* govmodel = generator->get_turbine_governor_model();
        if(govmodel!=NULL)
            govmodel->check();

        TURBINE_LOAD_CONTROLLER_MODEL* tlcmodel = generator->get_turbine_load_controller_model();
        if(tlcmodel!=NULL)
            tlcmodel->check();

        if(toolkit->is_detailed_log_enabled())
        {
            osstream<<"Model entry address of "<<generator->get_compound_device_name()<<":\n"
                    <<"Compensator: "<<compmodel<<"\n"
                    <<"PSS:         "<<pssmodel<<"\n"
                    <<"Exciter:     "<<avrmodel<<"\n"
                    <<"TurbineLCtrl:"<<tlcmodel<<"\n"
                    <<"Turbine:     "<<govmodel<<"\n"
                    <<"Generator:   "<<genmodel;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_wt_generator_related_dynamic_data()
{
    vector<WT_GENERATOR*> generators = get_all_wt_generators();
    unsigned int n = generators.size();
    WT_GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_GENERATOR_MODEL* genmodel = generator->get_wt_generator_model();
        if(genmodel!=NULL)
            genmodel->check();

        WT_ELECTRICAL_MODEL* elecmodel = generator->get_wt_electrical_model();
        if(elecmodel!=NULL)
            elecmodel->check();

        WT_AERODYNAMIC_MODEL* aerdmodel = generator->get_wt_aerodynamic_model();
        if(aerdmodel!=NULL)
            aerdmodel->check();

        WT_TURBINE_MODEL* turbmodel = generator->get_wt_turbine_model();
        if(turbmodel!=NULL)
            turbmodel->check();

        WT_PITCH_MODEL* pitchmodel = generator->get_wt_pitch_model();
        if(pitchmodel!=NULL)
            pitchmodel->check();

        WT_VRT_MODEL * vrtmodel = generator->get_wt_vrt_model();
        if(vrtmodel!=NULL)
            vrtmodel->check();

        WT_RELAY_MODEL* relaymodel = generator->get_wt_relay_model();
        if(relaymodel!=NULL)
            relaymodel->check();

        WIND_SPEED_MODEL* windmodel = generator->get_wind_speed_model();
        if(windmodel!=NULL)
            windmodel->check();

        WT_DBR_MODEL* dbrmodel = generator->get_wt_dbr_model();
        if(dbrmodel!=NULL)
            dbrmodel->check();


        if(toolkit->is_detailed_log_enabled())
        {
            ostringstream osstream;
            osstream<<"Model entry address of "<<generator->get_compound_device_name()<<":\n"
                    <<"WT Generator: "<<genmodel<<"\n"
                    <<"Electrical:   "<<elecmodel<<"\n"
                    <<"Aero dyanmic: "<<aerdmodel<<"\n"
                    <<"Turbine:      "<<turbmodel<<"\n"
                    <<"Pitch:        "<<pitchmodel<<"\n"
                    <<"VRT:          "<<vrtmodel<<"\n"
                    <<"Relay:        "<<relaymodel<<"\n"
                    <<"Dbr:          "<<dbrmodel<<"\n"
                    <<"Wind:         "<<windmodel;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_pv_unit_related_dynamic_data()
{
    vector<PV_UNIT*> pvs = get_all_pv_units();
    unsigned int n = pvs.size();
    PV_UNIT* pv;
    for(unsigned int i=0; i!=n; ++i)
    {
        pv = pvs[i];

        PV_CONVERTER_MODEL* convmodel = pv->get_pv_converter_model();
        if(convmodel!=NULL)
            convmodel->check();

        PV_PANEL_MODEL* panelmodel = pv->get_pv_panel_model();
        if(panelmodel!=NULL)
            panelmodel->check();

        PV_ELECTRICAL_MODEL* elecmodel = pv->get_pv_electrical_model();
        if(elecmodel!=NULL)
            elecmodel->check();

        PV_IRRADIANCE_MODEL* irradmodel = pv->get_pv_irradiance_model();
        if(irradmodel!=NULL)
            irradmodel->check();

        PV_TEMPERATURE_MODEL* tempmodel = pv->get_pv_temperature_model();
        if(tempmodel!=NULL)
            tempmodel->check();

        PV_VRT_MODEL* vrtmodel = pv->get_pv_vrt_model();
        if(vrtmodel!=NULL)
            vrtmodel->check();

        PV_RELAY_MODEL* relay = pv->get_pv_relay_model();
        if(relay!=NULL)
            relay->check();

        if(toolkit->is_detailed_log_enabled())
        {
            ostringstream osstream;
            osstream<<"Model entry address of "<<pv->get_compound_device_name()<<":\n"
                    <<"Pv converter: "<<convmodel<<"\n"
                    <<"Electrical:   "<<elecmodel<<"\n"
                    <<"Temperature:  "<<tempmodel<<"\n"
                    <<"Irradiance:   "<<irradmodel<<"\n"
                    <<"VRT:          "<<vrtmodel<<"\n"
                    <<"Relay:        "<<relay;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_energy_storage_related_dynamic_data()
{
    vector<ENERGY_STORAGE*> estorages = get_all_energy_storages();
    unsigned int n = estorages.size();
    ENERGY_STORAGE* estorage;
    for(unsigned int i=0; i!=n; ++i)
    {
        estorage = estorages[i];

        ES_CONVERTER_MODEL* conv = estorage->get_es_converter_model();
        if(conv!=NULL)
            conv->check();

        ES_ELECTRICAL_MODEL* elec = estorage->get_es_electrical_model();
        if(elec!=NULL)
            elec->check();

        ES_SOC_MODEL* soc = estorage->get_es_soc_model();
        if(soc!=NULL)
            soc->check();

        ES_VRT_MODEL* vrt = estorage->get_es_vrt_model();
        if(vrt!=NULL)
            vrt->check();

        ES_RELAY_MODEL* relay = estorage->get_es_relay_model();
        if(relay!=NULL)
            relay->check();

        ES_PRIMARY_MODEL* primary = estorage->get_es_primary_model();
        if(primary!=NULL)
            primary->check();

        ES_ROTOR_MODEL* rotor = estorage->get_es_rotor_model();
        if(rotor!=NULL)
            rotor->check();

        ES_AUXILIARY_MODEL* aux = estorage->get_es_auxiliary_model();
        if(aux!=NULL)
            aux->check();

        if(toolkit->is_detailed_log_enabled())
        {
            ostringstream osstream;
            osstream<<"Model entry address of "<<estorage->get_compound_device_name()<<":\n"
                    <<"Converter:    "<<conv<<"\n"
                    <<"Primary:      "<<primary<<"\n"
                    <<"Electrical:   "<<elec<<"\n"
                    <<"Auxiliary:    "<<aux<<"\n"
                    <<"Rotor:        "<<rotor<<"\n"
                    <<"Soc:          "<<soc<<"\n"
                    <<"Vrt:          "<<vrt<<"\n"
                    <<"Relay:        "<<relay;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_sv_source_related_dynamic_data()
{
    vector<SV_SOURCE*> sv_sources = get_all_sv_sources();
    unsigned int n = sv_sources.size();
    SV_SOURCE* sv_source;
    for(unsigned int i=0; i!=n; ++i)
    {
        sv_source = sv_sources[i];

        SVS_CONTROL_MODEL* svs_ctrl_model = sv_source->get_svs_control_model();
        if(svs_ctrl_model!=NULL)
            svs_ctrl_model->check();

        if(toolkit->is_detailed_log_enabled())
        {
            ostringstream osstream;
            osstream<<"Model entry address of "<<sv_source->get_compound_device_name()<<":\n"
                    <<"SVS CONTROL MODEL:    "<<svs_ctrl_model;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_load_related_dynamic_data()
{
    ostringstream osstream;
    vector<LOAD*> loads = get_all_loads();
    unsigned int n = loads.size();
    LOAD* load;
    for(unsigned int i=0; i!=n; ++i)
    {
        load = loads[i];

        LOAD_MODEL* loadmodel = load->get_load_model();
        if(loadmodel!=NULL)
            loadmodel->check();

        LOAD_VOLTAGE_RELAY_MODEL* uvlsmodel = load->get_load_voltage_relay_model();
        if(uvlsmodel!=NULL)
            uvlsmodel->check();

        LOAD_FREQUENCY_RELAY_MODEL* uflsmodel = load->get_load_frequency_relay_model();
        if(uflsmodel!=NULL)
            uflsmodel->check();

        if(toolkit->is_detailed_log_enabled())
        {
            osstream<<"Model entry address of "<<load->get_compound_device_name()<<":\n"
                    <<"uvls: "<<uvlsmodel<<"\n"
                    <<"ufls: "<<uflsmodel<<"\n"
                    <<"load: "<<loadmodel;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_ac_line_related_dynamic_data()
{
    /*vector<AC_LINE*> lines = get_all_ac_lines();
    unsigned int n = lines.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        AC_LINE *line = lines[i];
    }*/
}

void POWER_SYSTEM_DATABASE::check_lcc_hvdc2t_related_dynamic_data()
{
    vector<LCC_HVDC2T*> hvdcs = get_all_lcc_hvdc2ts();
    unsigned int n = hvdcs.size();
    LCC_HVDC2T* hvdc;
    for(unsigned int i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];

        LCC_HVDC2T_MODEL* hvdcmodel = hvdc->get_lcc_hvdc2t_model();
        if(hvdcmodel!=NULL)
            hvdcmodel->check();
    }
}

void POWER_SYSTEM_DATABASE::check_vsc_hvdc_related_dynamic_data()
{
    vector<VSC_HVDC*> vsc_hvdcs = get_all_vsc_hvdcs();
    unsigned int n = vsc_hvdcs.size();
    VSC_HVDC* vsc_hvdc;
    for(unsigned int i=0; i!=n; ++i)
    {
        vsc_hvdc = vsc_hvdcs[i];

        VSC_HVDC_NETWORK_MODEL* vsc_project_model = vsc_hvdc->get_vsc_hvdc_network_model();
        if(vsc_project_model!=NULL)
            vsc_project_model->check();
        vector<VSC_HVDC_CONVERTER_MODEL*> vsc_converter_models = vsc_hvdc->get_vsc_hvdc_converter_models();
        unsigned int n = vsc_converter_models.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            VSC_HVDC_CONVERTER_MODEL* model = vsc_converter_models[i];
            if(model!=NULL)
                model->check();
        }
    }
}

void POWER_SYSTEM_DATABASE::check_mt_hvdc_related_dynamic_data()
{
    vector<MT_HVDC*> mt_hvdcs = get_all_mt_hvdcs();
    unsigned int n = mt_hvdcs.size();
    MT_HVDC* mt_hvdc;
    for(unsigned int i=0; i!=n; ++i)
    {
        mt_hvdc = mt_hvdcs[i];

        MT_DC_GRID_MODEL* mtdc_grid_model = mt_hvdc->get_mtdc_network_model();
        if(mtdc_grid_model!=NULL)
            mtdc_grid_model->check();

        vector<HLCC_MODEL*> lcc_models = mt_hvdc->get_all_hlcc_models();
        unsigned int n_lcc = lcc_models.size();
        for(unsigned int j=0; j!=n_lcc; j++)
        {
            HLCC_MODEL* lcc_model = lcc_models[i];
            if(lcc_model!=NULL)
                lcc_model->check();
        }

        vector<HVSC_MODEL*> vsc_models = mt_hvdc->get_all_hvsc_models();
        unsigned int n_vsc = vsc_models.size();
        for(unsigned int i=0;i!=n_vsc;i++)
        {
            HVSC_MODEL* vsc_model = vsc_models[i];
            if(vsc_model!=NULL)
                vsc_model->check();
        }
    }
}

void POWER_SYSTEM_DATABASE::check_equivalent_device_related_dynamic_data()
{
    vector<EQUIVALENT_DEVICE*> edevices = get_all_equivalent_devices();
    unsigned int n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    for(unsigned int i=0; i!=n; ++i)
    {
        edevice = edevices[i];

        EQUIVALENT_MODEL* eqmodel = edevice->get_equivalent_model();
        if(eqmodel!=NULL)
            eqmodel->check();
    }
}


void POWER_SYSTEM_DATABASE::check_missing_models()
{
    check_missing_generator_related_model();
    check_missing_wt_generator_related_model();
    check_missing_pv_unit_related_model();
    check_missing_energy_storage_related_model();
    check_missing_sv_source_related_model();
    check_missing_load_related_model();
    check_missing_ac_line_related_model();
    check_missing_lcc_hvdc2t_related_model();
    check_missing_equivalent_device_related_model();
    check_missing_mt_hvdc_related_model();
}

void POWER_SYSTEM_DATABASE::check_missing_generator_related_model()
{
    ostringstream osstream;
    vector<GENERATOR*> generators = get_all_generators();
    unsigned int n = generators.size();
    GENERATOR* generator;

    bool model_missing_detected = false;
    osstream<<"Error. Synchronous generator model is missing for the following generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
        if(genmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]\n"
                    <<"Use default GENCLS model with H=1.0, D=0.0\n";
            model_missing_detected = true;
            string model_string = num2str(generator->get_generator_bus())+", GENCLS, "+generator->get_identifier()+", 1.0, 0.0/";
            PSSE_IMEXPORTER importer(*toolkit);
            importer.load_one_model(model_string);
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. Exciter model is missing for the following generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
        if(genmodel!=NULL and genmodel->get_model_name()!="GENCLS")
        {
            EXCITER_MODEL* avrmodel = generator->get_exciter_model();
            if(avrmodel==NULL)
            {
                osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
                model_missing_detected = true;
            }
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. Turbine governor model is missing for the following generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        TURBINE_GOVERNOR_MODEL* govmodel = generator->get_turbine_governor_model();
        if(govmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_wt_generator_related_model()
{
    ostringstream osstream;
    vector<WT_GENERATOR*> generators = get_all_wt_generators();
    unsigned int n = generators.size();
    WT_GENERATOR* generator;

    bool model_missing_detected = false;
    osstream<<"Error. WT generator model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_GENERATOR_MODEL* genmodel = generator->get_wt_generator_model();
        if(genmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Error. WT turbine model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_TURBINE_MODEL* turbmodel = generator->get_wt_turbine_model();
        if(turbmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. WT electrical model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_ELECTRICAL_MODEL* elecmodel = generator->get_wt_electrical_model();
        if(elecmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. WT aerodynamic model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_AERODYNAMIC_MODEL* aerdmodel = generator->get_wt_aerodynamic_model();
        if(aerdmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. WT electrical model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_ELECTRICAL_MODEL* elecmodel = generator->get_wt_electrical_model();
        if(elecmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");

    model_missing_detected = false;
    osstream<<"Warning. WT pitch model is missing for the following wt generators:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];

        WT_PITCH_MODEL* pitchmodel = generator->get_wt_pitch_model();
        if(pitchmodel==NULL)
        {
            osstream<<generator->get_compound_device_name()<<" ["<<bus_number2bus_name(generator->get_generator_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_pv_unit_related_model()
{
    ostringstream osstream;
    vector<PV_UNIT*> pvs = get_all_pv_units();
    unsigned int n = pvs.size();
    PV_UNIT* pv;

    bool model_missing_detected = false;
    osstream<<"Error. PV converter model is missing for the following PV units:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        pv = pvs[i];

        PV_CONVERTER_MODEL* convmodel = pv->get_pv_converter_model();
        if(convmodel==NULL)
        {
            osstream<<pv->get_compound_device_name()<<" ["<<bus_number2bus_name(pv->get_unit_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_energy_storage_related_model()
{
    ostringstream osstream;
    osstream<<"No energy storage is checked for model missing.";
    toolkit->show_information_with_leading_time_stamp(osstream);
    return;

    vector<ENERGY_STORAGE*> estorages = get_all_energy_storages();
    unsigned int n = estorages.size();
    ENERGY_STORAGE* estorage;

    bool model_missing_detected = false;
    osstream<<"Error. Energy storage model is missing for the following energy storages:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        estorage = estorages[i];

        ES_CONVERTER_MODEL* conv = estorage->get_es_converter_model();
        if(conv==NULL)
        {
            osstream<<estorage->get_compound_device_name()<<" ["<<bus_number2bus_name(estorage->get_energy_storage_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_sv_source_related_model()
{
    ostringstream osstream;
    osstream<<"No sv source is checked for model missing.";
    toolkit->show_information_with_leading_time_stamp(osstream);
    return;
}

void POWER_SYSTEM_DATABASE::check_missing_load_related_model()
{
    ostringstream osstream;
    vector<LOAD*> loads = get_all_loads();
    unsigned int n = loads.size();
    LOAD* load;

    bool model_missing_detected = false;
    osstream<<"Error. Load model is missing for the following loads:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        load = loads[i];

        LOAD_MODEL* loadmodel = load->get_load_model();
        if(loadmodel==NULL)
        {
            osstream<<load->get_compound_device_name()<<" ["<<bus_number2bus_name(load->get_load_bus())<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_ac_line_related_model()
{
    ostringstream osstream;
    osstream<<"No line is checked for model missing.";
    toolkit->show_information_with_leading_time_stamp(osstream);
    return;
}

void POWER_SYSTEM_DATABASE::check_missing_lcc_hvdc2t_related_model()
{
    ostringstream osstream;
    vector<LCC_HVDC2T*> hvdcs = get_all_lcc_hvdc2ts();
    unsigned int n = hvdcs.size();
    LCC_HVDC2T* hvdc;

    bool model_missing_detected = false;
    osstream<<"Error. LCC_HVDC2T model is missing for the following LCC_HVDC2T links:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];

        LCC_HVDC2T_MODEL* hvdcmodel = hvdc->get_lcc_hvdc2t_model();
        if(hvdcmodel==NULL)
        {
            osstream<<hvdc->get_compound_device_name()<<" ["<<bus_number2bus_name(hvdc->get_converter_bus(RECTIFIER))<<" -- "<<bus_number2bus_name(hvdc->get_converter_bus(INVERTER))<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_vsc_hvdc_related_model()
{
    ostringstream osstream;
    vector<VSC_HVDC*> vsc_hvdcs = get_all_vsc_hvdcs();
    unsigned int n = vsc_hvdcs.size();
    VSC_HVDC* vsc_hvdc;

    bool model_missing_detected = false;
    osstream<<"Error. VSC_HVDC model is missing for the following VSC_HVDC links:\n";
    for(unsigned int i=0; i!=n; ++i)
    {
        vsc_hvdc = vsc_hvdcs[i];

        VSC_HVDC_NETWORK_MODEL* vscmodel = vsc_hvdc->get_vsc_hvdc_network_model();
        unsigned int n_converter = vsc_hvdc->get_converter_count();
        if(vscmodel==NULL)
        {
            osstream<<vsc_hvdc->get_compound_device_name()<<"[";
            for(unsigned int j=0; j!=n_converter-1; ++j)
            {
                osstream<<bus_number2bus_name(vsc_hvdc->get_converter_ac_bus(j))<<" -- ";
            }
            osstream<<bus_number2bus_name(vsc_hvdc->get_converter_ac_bus(n_converter-1))<<"]";
            model_missing_detected = true;
        }
    }
    if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_mt_hvdc_related_model()
{
    ostringstream osstream;
    vector<MT_HVDC*> mtdcs = get_all_mt_hvdcs();
    unsigned int n = mtdcs.size();
    MT_HVDC* mtdc;

    bool model_missing_detected = false;
    osstream<<"Error. MT_HVDC model is missing for the following MT_HVDC projects:\n";
    for(unsigned int i=0;i!=n;i++)
    {
        mtdc = mtdcs[i];
        vector<HLCC*> lccs = mtdc->get_all_lcc_converters();
        vector<HVSC*> vscs = mtdc->get_all_vsc_converters();
        MT_DC_GRID_MODEL* mtdc_grid_model = mtdc->get_mtdc_network_model();
        if(mtdc_grid_model==NULL)
        {
            osstream<<mtdc->get_compound_device_name()<<"[ MTDC GRID MODEL ]";
            model_missing_detected = true;
        }

        unsigned int n_lcc = lccs.size();
        for(unsigned int j=0;j!=n_lcc;j++)
        {
            if(lccs[j]->get_hlcc_model()==NULL)
            {
                osstream<<mtdc->get_compound_device_name()<<"["<<lccs[j]->get_converter_name()<<"in ac bus:"<<bus_number2bus_name(lccs[j]->get_converter_ac_bus())<<"]";
                model_missing_detected = true;
            }
        }

        unsigned int n_vsc = vscs.size();
        for(unsigned int j=0;j!=n_vsc;j++)
        {
            if(vscs[j]->get_hvsc_model()==NULL)
            {
                osstream<<mtdc->get_compound_device_name()<<"["<<vscs[j]->get_converter_name()<<"in ac bus:"<<bus_number2bus_name(vscs[j]->get_converter_ac_bus())<<"]";;
                model_missing_detected = true;
            }
        }
    }
     if(model_missing_detected==true)
        toolkit->show_information_with_leading_time_stamp(osstream);
    else
        osstream.str("");
}

void POWER_SYSTEM_DATABASE::check_missing_equivalent_device_related_model()
{
    ostringstream osstream;
    osstream<<"No equivalent device is checked for model missing.";
    toolkit->show_information_with_leading_time_stamp(osstream);

    return;
}


void POWER_SYSTEM_DATABASE::scale_load_power(const DEVICE_ID& did, double scale)
{
    LOAD* load = get_pointer_to_load(did);
    if(load!=NULL)
    {
        scale += 1.0;

        load->set_nominal_constant_power_load_in_MVA(load->get_nominal_constant_power_load_in_MVA()*scale);
        load->set_nominal_constant_current_load_in_MVA(load->get_nominal_constant_current_load_in_MVA()*scale);
        load->set_nominal_constant_impedance_load_in_MVA(load->get_nominal_constant_impedance_load_in_MVA()*scale);
    }
}


void POWER_SYSTEM_DATABASE::scale_all_loads_power(double scale)
{
    vector<DEVICE_ID> loads = get_all_loads_device_id();
    unsigned int n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_connecting_to_bus(bus);
    unsigned int n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_area(area_number);
    unsigned int n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_zone(zone_number);
    unsigned int n = loads.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}



void POWER_SYSTEM_DATABASE::scale_source_power(const DEVICE_ID& did, double scale)
{
    SOURCE* source = get_pointer_to_source(did);
    if(source!=NULL)
    {
        scale += 1.0;

        double P = source->get_p_generation_in_MW();
        double Q = source->get_q_generation_in_MVar();

        P *= scale;
        double pmax = source->get_p_max_in_MW();
        double pmin = source->get_p_min_in_MW();
        if(P>pmax)
            source->set_p_generation_in_MW(pmax);
        else
        {
            if(P<pmin)
                source->set_p_generation_in_MW(pmin);
            else
                source->set_p_generation_in_MW(P);
        }

        Q *= scale;
        double qmax = source->get_q_max_in_MVar();
        double qmin = source->get_q_min_in_MVar();
        if(Q>qmax)
            source->set_q_generation_in_MVar(qmax);
        else
        {
            if(Q<qmin)
                source->set_q_generation_in_MVar(qmin);
            else
                source->set_q_generation_in_MVar(Q);
        }
    }
}

void POWER_SYSTEM_DATABASE::scale_all_sources_power(double scale)
{
    vector<DEVICE_ID> sources = get_all_sources_device_id();
    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_connecting_to_bus(bus);
    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_area(area_number);
    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_zone(zone_number);
    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generator_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_generators_power(double scale)
{
    vector<DEVICE_ID> generators = get_all_generators_device_id();
    unsigned int n = generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_connecting_to_bus(bus);
    unsigned int n = generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_area(area_number);
    unsigned int n = generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_zone(zone_number);
    unsigned int n = generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generator_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_wt_generators_power(double scale)
{
    vector<DEVICE_ID> wt_generators = get_all_wt_generators_device_id();
    unsigned int n = wt_generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_connecting_to_bus(bus);
    unsigned int n = wt_generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_in_area(area_number);
    unsigned int n = wt_generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_wt_generators_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> wt_generators = get_wt_generators_device_id_in_zone(zone_number);
    unsigned int n = wt_generators.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_wt_generator_power(wt_generators[i], scale);
}


void POWER_SYSTEM_DATABASE::scale_pv_unit_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_pv_units_power(double scale)
{
    vector<DEVICE_ID> pv_units = get_all_pv_units_device_id();
    unsigned int n = pv_units.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_connecting_to_bus(bus);
    unsigned int n = pv_units.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_in_area(area_number);
    unsigned int n = pv_units.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pv_units_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> pv_units = get_pv_units_device_id_in_zone(zone_number);
    unsigned int n = pv_units.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_pv_unit_power(pv_units[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storage_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_energy_storages_power(double scale)
{
    vector<DEVICE_ID> ess = get_all_energy_storages_device_id();
    unsigned int n = ess.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_connecting_to_bus(bus);
    unsigned int n = ess.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_in_area(area_number);
    unsigned int n = ess.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_energy_storages_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> ess = get_energy_storages_device_id_in_zone(zone_number);
    unsigned int n = ess.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_energy_storage_power(ess[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sv_source_power(const DEVICE_ID& did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_sv_sources_power(double scale)
{
    vector<DEVICE_ID> svss = get_all_sv_sources_device_id();
    unsigned int n = svss.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_sv_source_power(svss[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sv_sources_power_at_bus(unsigned int bus, double scale)
{
    vector<DEVICE_ID> svss = get_sv_sources_device_id_connecting_to_bus(bus);
    unsigned int n = svss.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_sv_source_power(svss[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sv_sources_power_in_area(unsigned int area_number, double scale)
{
    vector<DEVICE_ID> svss = get_sv_sources_device_id_in_area(area_number);
    unsigned int n = svss.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_sv_source_power(svss[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sv_sources_power_in_zone(unsigned int zone_number, double scale)
{
    vector<DEVICE_ID> svss = get_sv_sources_device_id_in_zone(zone_number);
    unsigned int n = svss.size();
    for(unsigned int i=0; i!=n; ++i)
        scale_sv_source_power(svss[i], scale);
}

void POWER_SYSTEM_DATABASE::clear_bus(unsigned int bus)
{
    if(is_bus_exist(bus))
    {
        clear_sources_connecting_to_bus(bus);
        clear_loads_connecting_to_bus(bus);
        clear_ac_lines_connecting_to_bus(bus);
        clear_transformers_connecting_to_bus(bus);
        clear_fixed_shunts_connecting_to_bus(bus);
        clear_lcc_hvdc2ts_connecting_to_bus(bus);
        clear_equivalent_devices_connecting_to_bus(bus);

        unsigned int current_index = get_bus_index(bus);

        vector<BUS>::iterator iter = Bus.begin();
        std::advance(iter, current_index);
        Bus.erase(iter);
        bus_index.set_bus_with_index(bus, INDEX_NOT_EXIST);

        unsigned int max_bus_number = bus_index.get_max_bus_number();

        unsigned int max_bus_number_plus_1 = max_bus_number+1;
        for(unsigned int i=1; i!=max_bus_number_plus_1; ++i)
        {
            unsigned int index = bus_index.get_index_of_bus(i);
            if(index!= INDEX_NOT_EXIST and index>current_index)
                bus_index.set_bus_with_index(i, index-1);
        }
        update_in_service_bus_count();
    }
}

void POWER_SYSTEM_DATABASE::clear_all_buses()
{
    Bus.clear();
    bus_index.clear();

    clear_all_sources();
    clear_all_loads();
    clear_all_ac_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_lcc_hvdc2ts();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}


void POWER_SYSTEM_DATABASE::clear_generator(const DEVICE_ID& device_id)
{
    if(is_generator_exist(device_id))
    {
        unsigned int current_index = get_generator_index(device_id);

        vector<GENERATOR>::iterator iter_generator = Generator.begin();

        std::advance(iter_generator, current_index);

        Generator.erase(iter_generator);

        generator_index.set_device_index(device_id, INDEX_NOT_EXIST);

        generator_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_generators_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_GENERATOR);
    while(true)
    {
        vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);
        if(generator.size()!=0)
        {
            device_id = generator[0]->get_device_id();
            clear_generator(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_generators()
{
    Generator.clear();
    generator_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_wt_generator(const DEVICE_ID& device_id)
{
    if(is_wt_generator_exist(device_id))
    {
        unsigned int current_index = get_wt_generator_index(device_id);

        vector<WT_GENERATOR>::iterator iter_wt_generator = WT_Generator.begin();

        std::advance(iter_wt_generator, current_index);
        WT_Generator.erase(iter_wt_generator);
        wt_generator_index.set_device_index(device_id, INDEX_NOT_EXIST);

        wt_generator_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_wt_generators_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_WT_GENERATOR);
    while(true)
    {
        vector<WT_GENERATOR*> wt_generator = get_wt_generators_connecting_to_bus(bus);
        if(wt_generator.size()!=0)
        {
            device_id = wt_generator[0]->get_device_id();
            clear_wt_generator(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_wt_generators()
{
    WT_Generator.clear();
    wt_generator_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_pv_unit(const DEVICE_ID& device_id)
{
    if(is_pv_unit_exist(device_id))
    {
        unsigned int current_index = get_pv_unit_index(device_id);

        vector<PV_UNIT>::iterator iter_pv_unit = PV_Unit.begin();

        std::advance(iter_pv_unit, current_index);
        PV_Unit.erase(iter_pv_unit);
        pv_unit_index.set_device_index(device_id, INDEX_NOT_EXIST);

        pv_unit_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}


void POWER_SYSTEM_DATABASE::clear_pv_units_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_PV_UNIT);
    while(true)
    {
        vector<PV_UNIT*> pv_unit = get_pv_units_connecting_to_bus(bus);
        if(pv_unit.size()!=0)
        {
            device_id = pv_unit[0]->get_device_id();
            clear_pv_unit(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_pv_units()
{
    PV_Unit.clear();
    pv_unit_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_energy_storage(const DEVICE_ID& device_id)
{
    if(is_energy_storage_exist(device_id))
    {
        unsigned int current_index = get_energy_storage_index(device_id);

        vector<ENERGY_STORAGE>::iterator iter_edevice = Energy_storage.begin();


        std::advance(iter_edevice, current_index);
        Energy_storage.erase(iter_edevice);
        energy_storage_index.set_device_index(device_id, INDEX_NOT_EXIST);

        energy_storage_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_energy_storages_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_ENERGY_STORAGE);
    while(true)
    {
        vector<ENERGY_STORAGE*> estorage = get_energy_storages_connecting_to_bus(bus);
        if(estorage.size()!=0)
        {
            device_id = estorage[0]->get_device_id();
            clear_energy_storage(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_energy_storages()
{
    Energy_storage.clear();
    energy_storage_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_sv_source(const DEVICE_ID& device_id)
{
    if(is_sv_source_exist(device_id))
    {
        unsigned int current_index = get_sv_source_index(device_id);

        vector<SV_SOURCE>::iterator iter_sv_source = SV_source.begin();

        std::advance(iter_sv_source, current_index);
        SV_source.erase(iter_sv_source);
        sv_source_index.set_device_index(device_id, INDEX_NOT_EXIST);

        sv_source_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}


void POWER_SYSTEM_DATABASE::clear_sv_sources_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_SV_SOURCE);
    while(true)
    {
        vector<SV_SOURCE*> sv_source = get_sv_sources_connecting_to_bus(bus);
        if(sv_source.size()!=0)
        {
            device_id = sv_source[0]->get_device_id();
            clear_sv_source(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_sv_sources()
{
    SV_source.clear();
    sv_source_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_sources_connecting_to_bus(const unsigned int bus)
{
    clear_generators_connecting_to_bus(bus);
    clear_wt_generators_connecting_to_bus(bus);
    clear_pv_units_connecting_to_bus(bus);
    clear_energy_storages_connecting_to_bus(bus);
    clear_sv_sources_connecting_to_bus(bus);
}

void POWER_SYSTEM_DATABASE::clear_all_sources()
{
    clear_all_generators();
    clear_all_wt_generators();
    clear_all_pv_units();
    clear_all_energy_storages();
    clear_all_sv_sources();
}

void POWER_SYSTEM_DATABASE::clear_load(const DEVICE_ID& device_id)
{
    if(is_load_exist(device_id))
    {
        unsigned int current_index = get_load_index(device_id);

        vector<LOAD>::iterator iter_load = Load.begin();


        std::advance(iter_load, current_index);
        Load.erase(iter_load);
        load_index.set_device_index(device_id, INDEX_NOT_EXIST);

        load_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_loads_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_LOAD);
    while(true)
    {
        vector<LOAD*> load = get_loads_connecting_to_bus(bus);
        if(load.size()!=0)
        {
            device_id = load[0]->get_device_id();
            clear_load(device_id);
        }
        else
            break;

    }
}

void POWER_SYSTEM_DATABASE::clear_all_loads()
{
    Load.clear();
    load_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_fixed_shunt(const DEVICE_ID& device_id)
{
    if(is_fixed_shunt_exist(device_id))
    {
        unsigned int current_index = get_fixed_shunt_index(device_id);

        vector<FIXED_SHUNT>::iterator iter_fs = Fixed_shunt.begin();


        std::advance(iter_fs, current_index);
        Fixed_shunt.erase(iter_fs);
        fixed_shunt_index.set_device_index(device_id, INDEX_NOT_EXIST);

        fixed_shunt_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);

        /*
        unsigned int current_index = get_fixed_shunt_index(device_id);

        vector<FIXED_SHUNT> fs;
        unsigned int nfshunt = get_fixed_shunt_count();
        for(unsigned int i=0; i!=nfshunt; ++i)
            if(i!=current_index) fs.push_back(Fixed_shunt[i]);

        Fixed_shunt.clear();
        Fixed_shunt = fs;

        fixed_shunt_index.set_device_index(device_id, INDEX_NOT_EXIST);

        fixed_shunt_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
        */
    }
}


void POWER_SYSTEM_DATABASE::clear_fixed_shunts_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_FIXED_SHUNT);
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    while(true)
    {
        vector<FIXED_SHUNT*> fixed_shunt = get_fixed_shunts_connecting_to_bus(bus);
        if(fixed_shunt.size()!=0)
        {
            /*device_id.set_device_identifier_index(fixed_shunt[0]->get_identifier_index());*/
            device_id = fixed_shunt[0]->get_device_id();
            clear_fixed_shunt(device_id);
            fixed_shunt.clear();
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_fixed_shunts()
{
    Fixed_shunt.clear();
    fixed_shunt_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_ac_line(const DEVICE_ID& device_id)
{
    if(is_ac_line_exist(device_id))
    {
        unsigned int current_index = get_ac_line_index(device_id);

        vector<AC_LINE>::iterator iter_line = Ac_line.begin();


        std::advance(iter_line, current_index);
        Ac_line.erase(iter_line);
        ac_line_index.set_device_index(device_id, INDEX_NOT_EXIST);

        ac_line_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_ac_lines_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_AC_LINE);
    while(true)
    {
        vector<AC_LINE*> line = get_ac_lines_connecting_to_bus(bus);
        if(line.size()!=0)
        {
            device_id = line[0]->get_device_id();
            clear_ac_line(device_id);
        }
        else
            break;

    }
}

void POWER_SYSTEM_DATABASE::clear_all_ac_lines()
{
    Ac_line.clear();
    ac_line_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_transformer(const DEVICE_ID& device_id)
{
    if(is_transformer_exist(device_id))
    {
        unsigned int current_index = get_transformer_index(device_id);

        vector<TRANSFORMER>::iterator iter_transformer = Transformer.begin();

        std::advance(iter_transformer, current_index);
        Transformer.erase(iter_transformer);
        transformer_index.set_device_index(device_id, INDEX_NOT_EXIST);

        transformer_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_transformers_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_TRANSFORMER);
    while(true)
    {
        vector<TRANSFORMER*> transformer = get_transformers_connecting_to_bus(bus);

        if(transformer.size()!=0)
        {
            device_id = transformer[0]->get_device_id();
            clear_transformer(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_transformers()
{
    Transformer.clear();
    transformer_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_lcc_hvdc2t(const DEVICE_ID& device_id)
{
    if(is_lcc_hvdc2t_exist(device_id))
    {
        unsigned int current_index = get_lcc_hvdc2t_index(device_id);

        vector<LCC_HVDC2T>::iterator iter_hvdc = Lcc_hvdc2t.begin();


        std::advance(iter_hvdc, current_index);
        Lcc_hvdc2t.erase(iter_hvdc);
        lcc_hvdc2t_index.set_device_index(device_id, INDEX_NOT_EXIST);

        lcc_hvdc2t_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_lcc_hvdc2ts_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_LCC_HVDC2T);
    while(true)
    {
        vector<LCC_HVDC2T*> hvdc = get_lcc_hvdc2ts_connecting_to_bus(bus);
        if(hvdc.size()!=0)
        {
            device_id = hvdc[0]->get_device_id();
            clear_lcc_hvdc2t(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_lcc_hvdc2ts()
{
    Lcc_hvdc2t.clear();
    lcc_hvdc2t_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_vsc_hvdc(const DEVICE_ID& device_id)
{
    if(is_vsc_hvdc_exist(device_id))
    {
        unsigned int current_index = get_vsc_hvdc_index(device_id);

        vector<VSC_HVDC>::iterator iter_vsc_hvdc = Vsc_hvdc.begin();


        std::advance(iter_vsc_hvdc, current_index);
        Vsc_hvdc.erase(iter_vsc_hvdc);
        vsc_hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

        vsc_hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_vsc_hvdc(const string vsc_name)
{
    if(is_vsc_hvdc_exist(vsc_name))
    {
        unsigned int current_index = get_vsc_hvdc_index(vsc_name);

        VSC_HVDC* vsc_hvdc = get_pointer_to_vsc_hvdc(vsc_name);
        DEVICE_ID device_id = vsc_hvdc->get_device_id();

        vector<VSC_HVDC>::iterator iter_vsc_hvdc = Vsc_hvdc.begin();

        std::advance(iter_vsc_hvdc, current_index);
        Vsc_hvdc.erase(iter_vsc_hvdc);
        vsc_hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

        vsc_hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_vsc_hvdcs_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type(STEPS_VSC_HVDC);
    while(true)
    {
        vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);
        if(vsc_hvdcs.size()!=0)
        {
            device_id = vsc_hvdcs[0]->get_device_id();
            clear_vsc_hvdc(device_id);
            vsc_hvdcs.clear();
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_vsc_hvdcs()
{
    Vsc_hvdc.clear();
    vsc_hvdc_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_mt_hvdc(const DEVICE_ID& device_id)
{
    if(is_mt_hvdc_exist(device_id))
    {
        unsigned int current_index = get_mt_hvdc_index(device_id);
        vector<MT_HVDC>::iterator iter_mt_hvdc = Mt_hvdc.begin();
        std::advance(iter_mt_hvdc, current_index);
        Mt_hvdc.erase(iter_mt_hvdc);
        mt_hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

        mt_hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_mt_hvdc(const string mtdc_name)
{
    if(is_mt_hvdc_exist(mtdc_name))
    {
        unsigned int current_index = get_mt_hvdc_index(mtdc_name);

        MT_HVDC* mt_hvdc = get_pointer_to_mt_hvdc(mtdc_name);
        DEVICE_ID device_id = mt_hvdc->get_device_id();

        vector<MT_HVDC>::iterator iter_mt_hvdc = Mt_hvdc.begin();

        std::advance(iter_mt_hvdc, current_index);
        Mt_hvdc.erase(iter_mt_hvdc);
        mt_hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

        mt_hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_mt_hvdcs()
{
    Mt_hvdc.clear();
    mt_hvdc_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_equivalent_device(const DEVICE_ID& device_id)
{
    if(is_equivalent_device_exist(device_id))
    {
        unsigned int current_index = get_equivalent_device_index(device_id);

        vector<EQUIVALENT_DEVICE>::iterator iter_edevice = Equivalent_device.begin();


        std::advance(iter_edevice, current_index);
        Equivalent_device.erase(iter_edevice);
        equivalent_device_index.set_device_index(device_id, INDEX_NOT_EXIST);

        equivalent_device_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
    }
}

void POWER_SYSTEM_DATABASE::clear_equivalent_devices_connecting_to_bus(const unsigned int bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type(STEPS_EQUIVALENT_DEVICE);
    while(true)
    {
        vector<EQUIVALENT_DEVICE*> edevice = get_equivalent_devices_connecting_to_bus(bus);
        if(edevice.size()!=0)
        {
            device_id = edevice[0]->get_device_id();
            clear_equivalent_device(device_id);
        }
        else
            break;
    }
}

void POWER_SYSTEM_DATABASE::clear_all_equivalent_devices()
{
    Equivalent_device.clear();
    equivalent_device_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_area(unsigned int area)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_areas()
{
    Area.clear();
    area_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_zone(unsigned int zone)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_zones()
{
    Zone.clear();
    zone_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_owner(unsigned int owner)
{
    ;
}

void POWER_SYSTEM_DATABASE::clear_all_owners()
{
    Owner.clear();
    owner_index.clear();
}


void POWER_SYSTEM_DATABASE::trip_bus(unsigned int bus)
{
    if(is_bus_exist(bus))
    {
        unsigned int index = get_bus_index(bus);
        if(Bus[index].get_bus_type()!=OUT_OF_SERVICE)
        {
            ostringstream osstream;

            osstream<<"The following devices are tripped before bus "<<bus<<" is tripped."<<endl;

            unsigned int n = 0;

            vector<GENERATOR*> gens = get_generators_connecting_to_bus(bus);
            n=gens.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(gens[i]->get_status()==true)
                {
                    gens[i]->set_status(false);
                    osstream<<gens[i]->get_compound_device_name()<<endl;
                }
            }

            vector<WT_GENERATOR*> wts = get_wt_generators_connecting_to_bus(bus);
            n=wts.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(wts[i]->get_status()==true)
                {
                    wts[i]->set_status(false);
                    osstream<<wts[i]->get_compound_device_name()<<endl;
                }
            }

            vector<PV_UNIT*> pvs = get_pv_units_connecting_to_bus(bus);
            n=pvs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(pvs[i]->get_status()==true)
                {
                    pvs[i]->set_status(false);
                    osstream<<pvs[i]->get_compound_device_name()<<endl;
                }
            }

            vector<ENERGY_STORAGE*> ess = get_energy_storages_connecting_to_bus(bus);
            n=ess.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(ess[i]->get_status()==true)
                {
                    ess[i]->set_status(false);
                    osstream<<ess[i]->get_compound_device_name()<<endl;
                }
            }

            vector<SV_SOURCE*> svss = get_sv_sources_connecting_to_bus(bus);
            n=svss.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(svss[i]->get_status()==true)
                {
                    svss[i]->set_status(false);
                    osstream<<svss[i]->get_compound_device_name()<<endl;
                }
            }

            vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
            n=loads.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(loads[i]->get_status()==true)
                {
                    loads[i]->set_status(false);
                    osstream<<loads[i]->get_compound_device_name()<<endl;
                }
            }

            vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
            n=fshunts.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(fshunts[i]->get_status()==true)
                {
                    fshunts[i]->set_status(false);
                    osstream<<fshunts[i]->get_compound_device_name()<<endl;
                }
            }

            vector<AC_LINE*> lines = get_ac_lines_connecting_to_bus(bus);
            n=lines.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(lines[i]->get_sending_side_breaker_status()==true or lines[i]->get_receiving_side_breaker_status()==true)
                {
                    lines[i]->set_sending_side_breaker_status(false);
                    lines[i]->set_receiving_side_breaker_status(false);
                    osstream<<lines[i]->get_compound_device_name()<<endl;
                }
            }

            vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
            n=transes.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(transes[i]->get_winding_breaker_status(PRIMARY_SIDE)==true or transes[i]->get_winding_breaker_status(SECONDARY_SIDE)==true or
                   (transes[i]->is_three_winding_transformer() and transes[i]->get_winding_breaker_status(TERTIARY_SIDE)==true) )
                {
                    transes[i]->set_winding_breaker_status(PRIMARY_SIDE, false);
                    transes[i]->set_winding_breaker_status(SECONDARY_SIDE, false);
                    if(transes[i]->is_two_winding_transformer())
                        continue;
                    else
                        transes[i]->set_winding_breaker_status(TERTIARY_SIDE, false);

                    osstream<<transes[i]->get_compound_device_name()<<endl;
                }
            }

            vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_connecting_to_bus(bus);
            n=hvdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(hvdcs[i]->get_status()==true)
                {
                    hvdcs[i]->set_status(false);
                    osstream<<hvdcs[i]->get_compound_device_name()<<endl;
                }
            }

            vector<VSC_HVDC*> vscs = get_vsc_hvdcs_connecting_to_bus(bus);
            n=vscs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(vscs[i]->get_status()==true)
                {
                    vscs[i]->set_status(false);
                    osstream<<vscs[i]->get_compound_device_name()<<endl;
                }
            }

            vector<MT_HVDC*> mtdcs = get_mt_hvdcs_connecting_to_bus(bus);
            n=mtdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(mtdcs[i]->get_status()==true)
                {
                    mtdcs[i]->set_status(false);
                    osstream<<mtdcs[i]->get_compound_device_name()<<endl;
                }
            }

            vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
            n=edevices.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(edevices[i]->get_status()==true)
                {
                    edevices[i]->set_status(false);
                    osstream<<edevices[i]->get_compound_device_name()<<endl;
                }
            }

            Bus[index].set_bus_type(OUT_OF_SERVICE);
            osstream<<"Bus "<<bus<<" is tripped.";
            toolkit->show_information_with_leading_time_stamp(osstream);

            update_in_service_bus_count();
        }
    }
}

void POWER_SYSTEM_DATABASE::check_device_status_for_out_of_service_buses()
{
    vector<BUS*> buses = get_all_buses();
    BUS* bus;
    unsigned int nbus = buses.size();
    for(unsigned int i=0; i!=nbus; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()!=OUT_OF_SERVICE)
            continue;
        else
        {
            unsigned int bus_number = bus->get_bus_number();
            check_source_status_for_out_of_service_bus(bus_number);
            check_load_status_for_out_of_service_bus(bus_number);
            check_fixed_shunt_status_for_out_of_service_bus(bus_number);
            check_ac_line_status_for_out_of_service_bus(bus_number);
            check_transformer_status_for_out_of_service_bus(bus_number);
            check_lcc_hvdc2t_status_for_out_of_service_bus(bus_number);
            check_vsc_hvdc_status_for_out_of_service_bus(bus_number);
            check_mt_hvdc_status_for_out_of_service_bus(bus_number);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_source_status_for_out_of_service_bus(unsigned int bus)
{
    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    SOURCE* source;
    unsigned int nsource = sources.size();
    for(unsigned int i=0; i!=nsource; ++i)
    {
        source = sources[i];
        if(source->get_status()==true)
            source->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_load_status_for_out_of_service_bus(unsigned int bus)
{
    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    LOAD* load;
    unsigned int nload = loads.size();
    for(unsigned int i=0; i!=nload; ++i)
    {
        load = loads[i];
        if(load->get_status()==true)
            load->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_fixed_shunt_status_for_out_of_service_bus(unsigned int bus)
{
    vector<FIXED_SHUNT*> shunts = get_fixed_shunts_connecting_to_bus(bus);
    FIXED_SHUNT* shunt;
    unsigned int nshunt = shunts.size();
    for(unsigned int i=0; i!=nshunt; ++i)
    {
        shunt = shunts[i];
        if(shunt->get_status()==true)
            shunt->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_ac_line_status_for_out_of_service_bus(unsigned int bus)
{
    vector<AC_LINE*> lines = get_ac_lines_connecting_to_bus(bus);
    AC_LINE* line;
    unsigned int nline = lines.size();
    for(unsigned int i=0; i!=nline; ++i)
    {
        line = lines[i];
        if(line->get_sending_side_bus()==bus)
        {
            if(line->get_sending_side_breaker_status()==true)
                line->set_sending_side_breaker_status(false);
        }
        else
        {
            if(line->get_receiving_side_breaker_status()==true)
                line->set_receiving_side_breaker_status(false);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_transformer_status_for_out_of_service_bus(unsigned int bus)
{
    vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(bus);
    TRANSFORMER* transformer;
    unsigned int ntrans = transformers.size();
    for(unsigned int i=0; i!=ntrans; ++i)
    {
        transformer = transformers[i];
        if(transformer->get_winding_bus(PRIMARY_SIDE)==bus)
        {
            if(transformer->get_winding_breaker_status(PRIMARY_SIDE)==true)
                transformer->set_winding_breaker_status(PRIMARY_SIDE, false);
        }
        else
        {
            if(transformer->get_winding_bus(SECONDARY_SIDE)==bus)
            {
                if(transformer->get_winding_breaker_status(SECONDARY_SIDE)==true)
                    transformer->set_winding_breaker_status(SECONDARY_SIDE, false);
            }
            else
            {
                if(transformer->get_winding_breaker_status(TERTIARY_SIDE)==true)
                    transformer->set_winding_breaker_status(TERTIARY_SIDE, false);
            }
        }
    }
}

void POWER_SYSTEM_DATABASE::check_lcc_hvdc2t_status_for_out_of_service_bus(unsigned int bus)
{
    vector<LCC_HVDC2T*> hvdcs = get_lcc_hvdc2ts_connecting_to_bus(bus);
    LCC_HVDC2T* hvdc;
    unsigned int nhvdc = hvdcs.size();
    for(unsigned int i=0; i!=nhvdc; ++i)
    {
        hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
            hvdc->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_vsc_hvdc_status_for_out_of_service_bus(unsigned int bus)
{
    vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);
    VSC_HVDC* vsc_hvdc;
    unsigned int nvsc = vsc_hvdcs.size();
    for(unsigned int i=0; i!=nvsc; ++i)
    {
        vsc_hvdc = vsc_hvdcs[i];
        if(vsc_hvdc->get_status()==true)
            vsc_hvdc->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_mt_hvdc_status_for_out_of_service_bus(unsigned int bus)
{
    vector<MT_HVDC*> mt_hvdcs = get_mt_hvdcs_connecting_to_bus(bus);
    MT_HVDC* mt_hvdc;
    unsigned int n_mtdc = mt_hvdcs.size();
    for(unsigned int i=0; i!=n_mtdc; ++i)
    {
        mt_hvdc = mt_hvdcs[i];
        if(mt_hvdc->get_status()==true)
            mt_hvdc->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::calibrate_sources_pmax_and_pmin_with_dynamic_model()
{
    calibrate_generators_pmax_and_pmin_with_turbine_governor_model();
}

void POWER_SYSTEM_DATABASE::calibrate_generators_pmax_and_pmin_with_turbine_governor_model()
{
    vector<GENERATOR*> generators = get_all_generators();
    unsigned int n = generators.size();
    GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        double mbase = generator->get_mbase_in_MVA();
        TURBINE_GOVERNOR_MODEL* model = generator->get_turbine_governor_model();
        if(model!=NULL)
        {
            double pmax = model->get_mechanical_power_upper_limit_in_pu_based_on_mbase()*mbase;
            double pmin = model->get_mechanical_power_lower_limit_in_pu_based_on_mbase()*mbase;
            generator->set_p_max_in_MW(pmax);
            generator->set_p_min_in_MW(pmin);
        }
    }
}

BUS_TYPE POWER_SYSTEM_DATABASE::get_bus_type(unsigned int bus) const
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_bus_type();
    else
        return OUT_OF_SERVICE;
}

double POWER_SYSTEM_DATABASE::get_bus_base_frequency_in_Hz(unsigned int bus) const
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_base_frequency_in_Hz();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_base_voltage_in_kV(unsigned int bus) const
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_base_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_positive_sequence_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_positive_sequence_voltage_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_positive_sequence_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_positive_sequence_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_positive_sequence_angle_in_deg(unsigned int bus)
{
    return rad2deg(get_bus_positive_sequence_angle_in_rad(bus));
}

double POWER_SYSTEM_DATABASE::get_bus_positive_sequence_angle_in_rad(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_positive_sequence_angle_in_rad();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_positive_sequence_complex_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_positive_sequence_complex_voltage_in_pu();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_positive_sequence_complex_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_positive_sequence_complex_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_negative_sequence_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_negative_sequence_voltage_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_negative_sequence_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_negative_sequence_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_negative_sequence_angle_in_deg(unsigned int bus)
{
    return rad2deg(get_bus_negative_sequence_angle_in_rad(bus));
}

double POWER_SYSTEM_DATABASE::get_bus_negative_sequence_angle_in_rad(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_negative_sequence_angle_in_rad();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_negative_sequence_complex_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_negative_sequence_complex_voltage_in_pu();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_negative_sequence_complex_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_negative_sequence_complex_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_zero_sequence_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_zero_sequence_voltage_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_zero_sequence_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_zero_sequence_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_zero_sequence_angle_in_deg(unsigned int bus)
{
    return rad2deg(get_bus_zero_sequence_angle_in_rad(bus));
}

double POWER_SYSTEM_DATABASE::get_bus_zero_sequence_angle_in_rad(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_zero_sequence_angle_in_rad();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_zero_sequence_complex_voltage_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_zero_sequence_complex_voltage_in_pu();
    else
        return 0.0;
}

complex<double> POWER_SYSTEM_DATABASE::get_bus_zero_sequence_complex_voltage_in_kV(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_zero_sequence_complex_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_deviation_in_pu();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_Hz(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_deviation_in_Hz();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_in_Hz(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_in_Hz();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_in_pu();
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_voltage_to_regulate_of_physical_bus_in_pu(unsigned int bus)
{
    BUS* busptr = get_pointer_to_ac_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_to_regulate_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_max_at_physical_bus_in_MW(unsigned int bus)
{
    if(is_bus_exist(bus))
    {
        BUS* busptr = get_pointer_to_ac_bus(bus);
        BUS_TYPE btype = busptr->get_bus_type();
        if(btype==SLACK_TYPE)
        {
            double total_p_max_in_MW = 0.0;

            vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

            unsigned int n = sources.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(sources[i]->get_status() == false)
                    continue;
                total_p_max_in_MW += sources[i]->get_p_max_in_MW();
            }

            vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);

            n = vsc_hvdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(vsc_hvdcs[i]->get_status() == false)
                    continue;
                unsigned int index = vsc_hvdcs[i]->get_converter_index_with_ac_bus(bus);
                if(vsc_hvdcs[i]->get_converter_status(index)==true and
                   vsc_hvdcs[i]->get_converter_active_power_operation_mode(index)==VSC_AC_VOLTAGE_ANGLE_CONTROL)
                    total_p_max_in_MW += vsc_hvdcs[i]->get_converter_Pmax_in_MW(index);
            }

            return total_p_max_in_MW;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_min_at_physical_bus_in_MW(unsigned int bus)
{
    if(is_bus_exist(bus))
    {
        BUS* busptr = get_pointer_to_ac_bus(bus);
        BUS_TYPE btype = busptr->get_bus_type();
        if(btype==SLACK_TYPE)
        {
            double total_p_min_in_MW = 0.0;

            vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

            unsigned int n = sources.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(sources[i]->get_status() == false)
                    continue;
                total_p_min_in_MW += sources[i]->get_p_min_in_MW();
            }

            vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);

            n = vsc_hvdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(vsc_hvdcs[i]->get_status() == false)
                    continue;
                unsigned int index = vsc_hvdcs[i]->get_converter_index_with_ac_bus(bus);
                if(vsc_hvdcs[i]->get_converter_status(index)==true and
                   vsc_hvdcs[i]->get_converter_active_power_operation_mode(index)==VSC_AC_VOLTAGE_ANGLE_CONTROL)
                    total_p_min_in_MW += vsc_hvdcs[i]->get_converter_Pmin_in_MW(index);
            }

            return total_p_min_in_MW;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_total_regulating_p_generation_at_physical_bus_in_MW(unsigned int bus)
{
    if(is_bus_exist(bus))
    {
        BUS* busptr = get_pointer_to_ac_bus(bus);
        BUS_TYPE btype = busptr->get_bus_type();
        if(btype==SLACK_TYPE)
        {
            double total_p_in_MW = 0.0;

            vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

            unsigned int n = sources.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(sources[i]->get_status() == true)
                    total_p_in_MW += sources[i]->get_p_generation_in_MW();
            }

            vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);

            n = vsc_hvdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(vsc_hvdcs[i]->get_status() == false)
                    continue;
                unsigned int index = vsc_hvdcs[i]->get_converter_index_with_ac_bus(bus);
                if(vsc_hvdcs[i]->get_converter_status(index)==true and
                   vsc_hvdcs[i]->get_converter_active_power_operation_mode(index)==VSC_AC_VOLTAGE_ANGLE_CONTROL)
                    total_p_in_MW += vsc_hvdcs[i]->get_converter_P_to_AC_bus_in_MW(index);
            }
            return total_p_in_MW;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_total_regulating_q_generation_at_physical_bus_in_MVar(unsigned int bus)
{

    if(is_bus_exist(bus))
    {
        BUS* busptr = get_pointer_to_ac_bus(bus);
        BUS_TYPE btype = busptr->get_bus_type();
        if(btype==PV_TYPE or
           btype==PV_TO_PQ_TYPE_1 or btype==PV_TO_PQ_TYPE_2 or
           btype==PV_TO_PQ_TYPE_3 or btype==PV_TO_PQ_TYPE_4 or
           btype==SLACK_TYPE)
        {
            double total_q_in_MVar = 0.0;

            vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

            unsigned int n = sources.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(sources[i]->get_status() == true)
                    total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
            }
            vector<VSC_HVDC*> vsc_hvdcs = get_vsc_hvdcs_connecting_to_bus(bus);

            n = vsc_hvdcs.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                if(vsc_hvdcs[i]->get_status() == false)
                    continue;
                unsigned int index = vsc_hvdcs[i]->get_converter_index_with_ac_bus(bus);
                if(vsc_hvdcs[i]->get_converter_status(index)==true and
                   vsc_hvdcs[i]->get_converter_reactive_power_operation_mode(index)==VSC_AC_VOLTAGE_CONTROL)
                    total_q_in_MVar += vsc_hvdcs[i]->get_converter_Q_to_AC_bus_in_MVar(index);
            }

            return total_q_in_MVar;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_total_p_generation_at_physical_bus_in_MW(unsigned int bus)
{
    double total_p_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == true)
            total_p_in_MW += sources[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;
}

double POWER_SYSTEM_DATABASE::get_total_q_generation_at_physical_bus_in_MVar(unsigned int bus)
{
    double total_q_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == true)
            total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_load_power_in_MVA()
{
    unsigned int n = get_load_count();
    complex<double> S=0.0;
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Load[i].get_status()==true)
            S += Load[i].get_actual_total_load_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_generation_power_in_MVA()
{
    unsigned int n = get_generator_count();
    complex<double> S=0.0;
    for(unsigned int i=0; i!=n; ++i)
    {
        if(Generator[i].get_status()==true)
            S += Generator[i].get_complex_generation_in_MVA();
    }
    n = get_wt_generator_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(WT_Generator[i].get_status()==true)
            S += WT_Generator[i].get_complex_generation_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_loss_power_in_MVA()
{
    return get_total_generation_power_in_MVA()-get_total_load_power_in_MVA();
}

unsigned int POWER_SYSTEM_DATABASE::get_memory_usage_in_bytes()
{
    ostringstream osstream;

    osstream<<"Size of some basic types:\n"
            <<"ownership   : "<<setw(9)<<sizeof(OWNERSHIP)<<"B\n"
            <<"fault       : "<<setw(9)<<sizeof(FAULT)<<"B\n"
            <<"bus freq mdl: "<<setw(9)<<sizeof(BUS_FREQUENCY_MODEL)<<"B\n"
            <<"unsigned int: "<<setw(9)<<sizeof(unsigned int)<<"B\n"
            <<"pointer     : "<<setw(9)<<sizeof(BUS*)<<"B\n"
            <<"bool        : "<<setw(9)<<sizeof(bool)<<"B\n"
            <<"string      : "<<setw(9)<<sizeof(string)<<"B";
    toolkit->show_information_with_leading_time_stamp(osstream);

    osstream<<"Power system database device used:\n"
            <<"Bus           : "<<setw(9)<<Bus.capacity()*sizeof(BUS)<<"B, "<<setw(9)<<Bus.size()*sizeof(BUS)<<"B, "<<setw(9)<<sizeof(BUS)<<"B\n"
            <<"Generator     : "<<setw(9)<<Generator.capacity()*sizeof(GENERATOR)<<"B, "<<setw(9)<<Generator.size()*sizeof(GENERATOR)<<"B, "<<setw(9)<<sizeof(GENERATOR)<<"B\n"
            <<"WT generator  : "<<setw(9)<<WT_Generator.capacity()*sizeof(WT_GENERATOR)<<"B, "<<setw(9)<<WT_Generator.size()*sizeof(WT_GENERATOR)<<"B, "<<setw(9)<<sizeof(WT_GENERATOR)<<"B\n"
            <<"PV unit       : "<<setw(9)<<PV_Unit.capacity()*sizeof(PV_UNIT)<<"B, "<<setw(9)<<PV_Unit.size()*sizeof(PV_UNIT)<<"B, "<<setw(9)<<sizeof(PV_UNIT)<<"B\n"
            <<"Energy storage: "<<setw(9)<<Energy_storage.capacity()*sizeof(ENERGY_STORAGE)<<"B, "<<setw(9)<<Energy_storage.size()*sizeof(ENERGY_STORAGE)<<"B, "<<setw(9)<<sizeof(ENERGY_STORAGE)<<"B\n"
            <<"SV source     : "<<setw(9)<<SV_source.capacity()*sizeof(SV_SOURCE)<<"B, "<<setw(9)<<SV_source.size()*sizeof(SV_SOURCE)<<"B, "<<setw(9)<<sizeof(SV_SOURCE)<<"B\n"
            <<"Load          : "<<setw(9)<<Load.capacity()*sizeof(LOAD)<<"B, "<<setw(9)<<Load.size()*sizeof(LOAD)<<"B, "<<setw(9)<<sizeof(LOAD)<<"B\n"
            <<"Fixed shunt   : "<<setw(9)<<Fixed_shunt.capacity()*sizeof(FIXED_SHUNT)<<"B, "<<setw(9)<<Fixed_shunt.size()*sizeof(FIXED_SHUNT)<<"B, "<<setw(9)<<sizeof(FIXED_SHUNT)<<"B\n"
            <<"Line          : "<<setw(9)<<Ac_line.capacity()*sizeof(AC_LINE)<<"B, "<<setw(9)<<Ac_line.size()*sizeof(AC_LINE)<<"B, "<<setw(9)<<sizeof(AC_LINE)<<"B\n"
            <<"Transformer   : "<<setw(9)<<Transformer.capacity()*sizeof(TRANSFORMER)<<"B, "<<setw(9)<<Transformer.size()*sizeof(TRANSFORMER)<<"B, "<<setw(9)<<sizeof(TRANSFORMER)<<"B\n"
            <<"Hvdc          : "<<setw(9)<<Lcc_hvdc2t.capacity()*sizeof(LCC_HVDC2T)<<"B, "<<setw(9)<<Lcc_hvdc2t.size()*sizeof(LCC_HVDC2T)<<"B, "<<setw(9)<<sizeof(LCC_HVDC2T)<<"B\n"
            <<"Equiv device  : "<<setw(9)<<Equivalent_device.capacity()*sizeof(EQUIVALENT_DEVICE)<<"B, "<<setw(9)<<Equivalent_device.size()*sizeof(EQUIVALENT_DEVICE)<<"B, "<<setw(9)<<sizeof(EQUIVALENT_DEVICE)<<"B\n"
            <<"Area          : "<<setw(9)<<Area.capacity()*sizeof(AREA)<<"B, "<<setw(9)<<Area.size()*sizeof(AREA)<<"B, "<<setw(9)<<sizeof(AREA)<<"B\n"
            <<"Zone          : "<<setw(9)<<Zone.capacity()*sizeof(ZONE)<<"B, "<<setw(9)<<Zone.size()*sizeof(ZONE)<<"B, "<<setw(9)<<sizeof(ZONE)<<"B\n"
            <<"Owner         : "<<setw(9)<<Owner.capacity()*sizeof(OWNER)<<"B, "<<setw(9)<<Owner.size()*sizeof(OWNER)<<"B, "<<setw(9)<<sizeof(OWNER)<<"B.";
            toolkit->show_information_with_leading_time_stamp(osstream);

    return Bus.capacity()*sizeof(BUS)+
           Generator.capacity()*sizeof(GENERATOR)+
           WT_Generator.capacity()*sizeof(WT_GENERATOR)+
           PV_Unit.capacity()*sizeof(PV_UNIT)+
           Energy_storage.capacity()*sizeof(ENERGY_STORAGE)+
           SV_source.capacity()*sizeof(SV_SOURCE)+
           Load.capacity()*sizeof(LOAD)+
           Fixed_shunt.capacity()*sizeof(FIXED_SHUNT)+
           Ac_line.capacity()*sizeof(AC_LINE)+
           Transformer.capacity()*sizeof(TRANSFORMER)+
           Lcc_hvdc2t.capacity()*sizeof(LCC_HVDC2T)+
           Vsc_hvdc.capacity()*sizeof(VSC_HVDC)+
           Mt_hvdc.capacity()*sizeof(MT_HVDC)+
           Equivalent_device.capacity()*sizeof(EQUIVALENT_DEVICE)+
           Area.capacity()*sizeof(AREA)+
           Zone.capacity()*sizeof(ZONE)+
           Owner.capacity()*sizeof(OWNER)+
           bus_index.get_max_bus_number()*sizeof(unsigned int)+
           generator_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           wt_generator_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           pv_unit_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           energy_storage_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           sv_source_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           load_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           fixed_shunt_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           switched_shunt_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           ac_line_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           transformer_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           lcc_hvdc2t_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           vsc_hvdc_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           mt_hvdc_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           equivalent_device_index.get_map_size()*(sizeof(DEVICE_ID)+sizeof(unsigned int))*2+
           area_index.size()*sizeof(unsigned int)*2+
           zone_index.size()*sizeof(unsigned int)*2+
           owner_index.size()*sizeof(unsigned int)*2;
}
