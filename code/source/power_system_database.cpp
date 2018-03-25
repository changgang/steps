#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/prettywriter.h"
#include "thirdparty/rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace rapidjson;

POWER_SYSTEM_DATABASE::POWER_SYSTEM_DATABASE()
{
    set_system_name("");
    set_system_base_power_in_MVA(100.0);
    set_system_base_frequency_in_Hz(50.0);

    set_allowed_max_bus_number(1000);

    set_database_capacity();

    clear_database();

    set_dynamic_simulator(NULL);

    update_in_service_bus_count();
}


POWER_SYSTEM_DATABASE::~POWER_SYSTEM_DATABASE()
{
    clear_database();
}

void POWER_SYSTEM_DATABASE::set_database_capacity()
{
    size_t bus_capacity = 1000;
    size_t generator_capacity = size_t(round(bus_capacity*0.5));
    size_t pesource_capacity = size_t(round(bus_capacity*0.5));
    size_t load_capacity = size_t(round(bus_capacity*0.5));
    size_t fixedshunt_capacity = size_t(round(bus_capacity*0.5));
    size_t line_capacity = size_t(round(bus_capacity*3.0));
    size_t transformer_capacity = bus_capacity;
    size_t hvdc_capacity = size_t(round(bus_capacity*0.1));
    size_t equivalentdevice_capacity = size_t(round(bus_capacity*0.1));
    size_t area_capacity = size_t(round(bus_capacity*0.1));
    size_t zone_capacity = size_t(round(bus_capacity*0.1));
    size_t owner_capacity = size_t(round(bus_capacity*0.1));

    ifstream instream("steps_config.json");
    if(instream.is_open())
    {
        ostringstream tmp;
        tmp<<instream.rdbuf();
        string str = tmp.str();

        Document document;
        document.Parse(str.c_str());


        if(document.HasMember("bus") and document["bus"].IsInt())
            bus_capacity = size_t(document["bus"].GetInt());

        if(document.HasMember("generator") and document["generator"].IsInt())
            generator_capacity = size_t(document["generator"].GetInt());

        if(document.HasMember("peSource") and document["peSource"].IsInt())
            pesource_capacity = size_t(document["peSource"].GetInt());

        if(document.HasMember("load") and document["load"].IsInt())
            load_capacity = size_t(document["load"].GetInt());

        if(document.HasMember("fixedShunt") and document["fixedShunt"].IsInt())
            fixedshunt_capacity = size_t(document["fixedShunt"].GetInt());

        if(document.HasMember("line") and document["line"].IsInt())
            line_capacity = size_t(document["line"].GetInt());

        if(document.HasMember("transformer") and document["transformer"].IsInt())
            transformer_capacity = size_t(document["transformer"].GetInt());

        if(document.HasMember("hvdc") and document["hvdc"].IsInt())
            hvdc_capacity = size_t(document["hvdc"].GetInt());

        if(document.HasMember("equivalentDevice") and document["equivalentDevice"].IsInt())
            equivalentdevice_capacity = size_t(document["equivalentDevice"].GetInt());

        if(document.HasMember("area") and document["area"].IsInt())
            area_capacity = size_t(document["area"].GetInt());

        if(document.HasMember("zone") and document["zone"].IsInt())
            zone_capacity = size_t(document["zone"].GetInt());

        if(document.HasMember("owner") and document["owner"].IsInt())
            owner_capacity = size_t(document["owner"].GetInt());
    }
    else
    {
        ostringstream sstream;
        sstream<<"No valid steps_config.json found. Use default power system database capacity.";
        show_information_with_leading_time_stamp(sstream);
    }

    set_bus_capacity(bus_capacity);
    set_generator_capacity(generator_capacity);
    set_pe_source_capacity(pesource_capacity);
    set_load_capacity(load_capacity);
    set_fixed_shunt_capacity(fixedshunt_capacity);
    set_line_capacity(line_capacity);
    set_transformer_capacity(transformer_capacity);
    set_hvdc_capacity(hvdc_capacity);
    set_equivalent_device_capacity(equivalentdevice_capacity);
    set_area_capacity(area_capacity);
    set_zone_capacity(zone_capacity);
    set_owner_capacity(owner_capacity);
}

size_t POWER_SYSTEM_DATABASE::get_bus_capacity() const
{
    return Bus.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_generator_capacity() const
{
    return Generator.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_pe_source_capacity() const
{
    return PE_Source.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_load_capacity() const
{
    return Load.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_capacity() const
{
    return Fixed_shunt.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_line_capacity() const
{
    return Line.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_transformer_capacity() const
{
    return Transformer.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_capacity() const
{
    return Hvdc.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_capacity() const
{
    return Equivalent_device.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_area_capacity() const
{
    return Area.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_zone_capacity() const
{
    return Zone.capacity();
}

size_t POWER_SYSTEM_DATABASE::get_owner_capacity() const
{
    return Owner.capacity();
}

void POWER_SYSTEM_DATABASE::set_bus_capacity(size_t n)
{
    Bus.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_generator_capacity(size_t n)
{
    Generator.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_pe_source_capacity(size_t n)
{
    PE_Source.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_load_capacity(size_t n)
{
    Load.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_fixed_shunt_capacity(size_t n)
{
    Fixed_shunt.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_line_capacity(size_t n)
{
    Line.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_transformer_capacity(size_t n)
{
    Transformer.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_hvdc_capacity(size_t n)
{
    Hvdc.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_equivalent_device_capacity(size_t n)
{
    Equivalent_device.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_area_capacity(size_t n)
{
    Area.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_zone_capacity(size_t n)
{
    Zone.reserve(n);
}

void POWER_SYSTEM_DATABASE::set_owner_capacity(size_t n)
{
    Owner.reserve(n);
}

void POWER_SYSTEM_DATABASE::clear_database()
{
    set_case_title_1("");
    set_case_title_2("");
    clear_all_buses();
    clear_all_generators();
    clear_all_pe_sources();
    clear_all_loads();
    clear_all_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_hvdcs();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}


void POWER_SYSTEM_DATABASE::set_dynamic_simulator(DYNAMICS_SIMULATOR* simulator)
{
    if(simulator!=NULL)
        this->simulator = simulator;
}

double POWER_SYSTEM_DATABASE::get_dynamic_simulator_time_in_s() const
{
    if(simulator!=NULL)
        return simulator->get_current_simulation_time_in_s();
    else
        return 0.0;
}

void POWER_SYSTEM_DATABASE::set_system_name(const string name)
{
    this->system_name = name;
}

string POWER_SYSTEM_DATABASE::get_system_name() const
{
    return this->system_name;
}

void POWER_SYSTEM_DATABASE::set_allowed_max_bus_number(size_t max_bus_num)
{
    bus_index.set_max_bus_number(max_bus_num);
}

size_t POWER_SYSTEM_DATABASE::get_allowed_max_bus_number() const
{
    return bus_index.get_max_bus_number();
}

bool POWER_SYSTEM_DATABASE::is_bus_in_allowed_range(size_t bus) const
{
    return (bus>0 and bus<=this->get_allowed_max_bus_number());
}

void POWER_SYSTEM_DATABASE::set_system_base_power_in_MVA(const double s)
{
    if(s>0.0)
        system_base_power_in_MVA = s;
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Non positive power ("<<s<<" MVA) is not allowed for setting system base power."<<endl
          <<"Nothing will be changed.";
        show_information_with_leading_time_stamp(sstream);
    }
}

double POWER_SYSTEM_DATABASE::get_system_base_power_in_MVA() const
{
    return system_base_power_in_MVA;
}

void POWER_SYSTEM_DATABASE::set_system_base_frequency_in_Hz(const double f)
{
    if(f>0.0)
        system_base_fequency_in_Hz = f;
    else
    {
        ostringstream sstream;
        sstream<<"Error. Non positive frequency ("<<f<<" Hz) is not allowed for setting system base frequency."<<endl
          <<"System base frequency will not be set. Original frequency is "<<get_system_base_frequency_in_Hz()<<" Hz.";
        show_information_with_leading_time_stamp(sstream);
    }
}

double POWER_SYSTEM_DATABASE::get_system_base_frequency_in_Hz() const
{
    return system_base_fequency_in_Hz;
}

void POWER_SYSTEM_DATABASE::set_case_title_1(string title)
{
    case_title_1 = title;
}

void POWER_SYSTEM_DATABASE::set_case_title_2(string title)
{
    case_title_2 = title;
}

string POWER_SYSTEM_DATABASE::get_case_title_1() const
{
    return case_title_1;
}

string POWER_SYSTEM_DATABASE::get_case_title_2() const
{
    return case_title_2;
}

void POWER_SYSTEM_DATABASE::append_bus(BUS& bus)
{
    ostringstream sstream;

    if(bus.get_power_system_database()==NULL)
        bus.set_power_system_database(this);

    if(this != bus.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = bus.get_power_system_database();
        sstream<<"Warning. "<<bus.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not bus.is_valid())
    {
        sstream<<"Warning. Failed to append invalid bus "<<bus.get_bus_number()<<" due to either 0 bus number of 0 base voltage.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t bus_number = bus.get_bus_number();

    if(not this->is_bus_in_allowed_range(bus_number))
    {
        sstream<<"Warning. Bus number "<<bus_number<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<bus.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Bus will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(this->is_bus_exist(bus_number))
    {
        sstream<<"Warning. Bus "<<bus_number<<" already exists in power system database '"<<get_system_name()<<"': Bus."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Bus.capacity()==Bus.size())
    {
        sstream<<"Warning. Capacity limit ("<<Bus.capacity()<<") reached when appending bus to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    size_t bus_count = get_bus_count();
    Bus.push_back(bus);
    bus_index.set_bus_with_index(bus_number, bus_count);

    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::append_generator(GENERATOR& generator)
{
    ostringstream sstream;

    if(generator.get_power_system_database()==NULL)
    {
        generator.set_power_system_database(this);
    }

    if(this != generator.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = generator.get_power_system_database();
        sstream<<"Warning. "<<generator.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not generator.is_valid())
    {
        sstream<<"Warning. Failed to append invalid generator to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t generator_bus = generator.get_generator_bus();

    if(not this->is_bus_in_allowed_range(generator_bus))
    {
        sstream<<"Warning. Bus number "<<generator_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<generator.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Generator will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = generator.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(generator_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_generator_exist(device_id))
    {
        sstream<<"Warning. "<<generator.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Generator."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Generator.capacity()==Generator.size())
    {
        sstream<<"Warning. Capacity limit ("<<Generator.capacity()<<") reached when appending generator to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t generator_count = get_generator_count();
    Generator.push_back(generator);
    generator_index.set_device_index(device_id, generator_count);
}

void POWER_SYSTEM_DATABASE::append_pe_source(PE_SOURCE& pesource)
{
    ostringstream sstream;

    if(pesource.get_power_system_database()==NULL)
    {
        pesource.set_power_system_database(this);
    }

    if(this != pesource.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = pesource.get_power_system_database();
        sstream<<"Warning. "<<pesource.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not pesource.is_valid())
    {
        sstream<<"Warning. Failed to append invalid wind-turbine genertor to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }


    size_t pesource_bus = pesource.get_source_bus();

    if(not this->is_bus_in_allowed_range(pesource_bus))
    {
        sstream<<"Warning. Bus "<<pesource_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<pesource.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"PE source will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = pesource.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("PE SOURCE");
    TERMINAL terminal;
    terminal.append_bus(pesource_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_pe_source_exist(device_id))
    {
        sstream<<"Warning. "<<pesource.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': PE_Source."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(PE_Source.capacity()==PE_Source.size())
    {
        sstream<<"Warning. Capacity limit ("<<PE_Source.capacity()<<") reached when appending pe source to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t pesource_count = get_pe_source_count();
    PE_Source.push_back(pesource);
    pe_source_index.set_device_index(device_id, pesource_count);
}

void POWER_SYSTEM_DATABASE::append_load(LOAD& load)
{
    ostringstream sstream;

    if(load.get_power_system_database()==NULL)
    {
        load.set_power_system_database(this);
    }

    if(this != load.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = load.get_power_system_database();
        sstream<<"Warning. "<<load.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'. "
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not load.is_valid())
    {
        sstream<<"Warning. Failed to append invalid load to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t load_bus = load.get_load_bus();

    if(not this->is_bus_in_allowed_range(load_bus))
    {
        sstream<<"Warning. Bus "<<load_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<load.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Load will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = load.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(load_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(is_load_exist(device_id))
    {
        sstream<<"Warning. "<<load.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Load."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Load.capacity()==Load.size())
    {
        sstream<<"Warning. Capacity limit ("<<Load.capacity()<<") reached when appending load to power system database "<<get_system_name()<<"."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t load_count = get_load_count();
    Load.push_back(load);
    load_index.set_device_index(device_id, load_count);
}

void POWER_SYSTEM_DATABASE::append_line(LINE& line)
{
    ostringstream sstream;

    if(line.get_power_system_database()==NULL)
    {
        line.set_power_system_database(this);
    }

    if(this != line.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = line.get_power_system_database();
        sstream<<"Warning. "<<line.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not line.is_valid())
    {
        sstream<<"Warning. Failed to append invalid line to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t sending_side_bus = line.get_sending_side_bus();
    size_t receiving_side_bus = line.get_receiving_side_bus();

    if(not this->is_bus_in_allowed_range(sending_side_bus))
    {
        sstream<<"Warning. Bus "<<sending_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(receiving_side_bus))
    {
        sstream<<"Warning. Bus "<<receiving_side_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<line.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Line will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = line.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(sending_side_bus);
    terminal.append_bus(receiving_side_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_line_exist(device_id))
    {
        sstream<<"Warning. "<<line.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Line."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Line.capacity()==Line.size())
    {
        sstream<<"Warning. Capacity limit ("<<Line.capacity()<<") reached when appending line to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t line_count = get_line_count();
    Line.push_back(line);
    line_index.set_device_index(device_id, line_count);
}

void POWER_SYSTEM_DATABASE::append_transformer(TRANSFORMER& transformer)
{
    ostringstream sstream;

    if(transformer.get_power_system_database()==NULL)
    {
        transformer.set_power_system_database(this);
    }

    if(this != transformer.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = transformer.get_power_system_database();
        sstream<<"Warning. "<<transformer.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not transformer.is_valid())
    {
        sstream<<"Warning. Failed to append invalid transformer to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t primary_winding_bus = transformer.get_winding_bus(PRIMARY_SIDE);
    size_t secondary_winding_bus = transformer.get_winding_bus(SECONDARY_SIDE);
    size_t tertiary_winding_bus = transformer.get_winding_bus(TERTIARY_SIDE);

    if(not is_bus_in_allowed_range(primary_winding_bus))
    {
        sstream<<"Warning. Bus "<<primary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(not is_bus_in_allowed_range(secondary_winding_bus))
    {
        sstream<<"Warning. Bus "<<secondary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(not(is_bus_in_allowed_range(tertiary_winding_bus) or tertiary_winding_bus==0))
    {
        sstream<<"Warning. Bus "<<tertiary_winding_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<transformer.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Transformer will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = transformer.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(primary_winding_bus);
    terminal.append_bus(secondary_winding_bus);
    terminal.append_bus(tertiary_winding_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(is_transformer_exist(device_id))
    {
        sstream<<"Warning. "<<transformer.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"':Transformer.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Transformer.capacity()==Transformer.size())
    {
        sstream<<"Warning. Capacity limit ("<<Transformer.capacity()<<") reached when appending transformer to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t transformer_count = get_transformer_count();
    Transformer.push_back(transformer);
    transformer_index.set_device_index(device_id, transformer_count);
}

void POWER_SYSTEM_DATABASE::append_fixed_shunt(FIXED_SHUNT& shunt)
{
    ostringstream sstream;

    if(shunt.get_power_system_database()==NULL)
    {
        shunt.set_power_system_database(this);
    }

    if(this != shunt.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = shunt.get_power_system_database();
        sstream<<"Warning. "<<shunt.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not shunt.is_valid())
    {
        sstream<<"Warning. Failed to append invalid fixed shunt to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t shunt_bus = shunt.get_shunt_bus();

    if(not this->is_bus_in_allowed_range(shunt_bus))
    {
        sstream<<"Warning. Bus "<<shunt_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<shunt.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Fixed shunt will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = shunt.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(shunt_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_fixed_shunt_exist(device_id))
    {
        sstream<<"Warning. "<<shunt.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Fixed_shunt.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Fixed_shunt.capacity()==Fixed_shunt.size())
    {
        sstream<<"Warning. Capacity limit ("<<Fixed_shunt.capacity()<<") reached when appending fixed shunt to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t shunt_count = get_fixed_shunt_count();
    Fixed_shunt.push_back(shunt);
    fixed_shunt_index.set_device_index(device_id, shunt_count);
}

void POWER_SYSTEM_DATABASE::append_hvdc(HVDC& hvdc)
{
    ostringstream sstream;

    if(hvdc.get_power_system_database()==NULL)
    {
        hvdc.set_power_system_database(this);
    }

    if(this != hvdc.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = hvdc.get_power_system_database();
        sstream<<"Warning. "<<hvdc.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not hvdc.is_valid())
    {
        sstream<<"Warning. Failed to append invalid hvdc to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t rec_bus = hvdc.get_converter_bus(RECTIFIER);
    size_t inv_bus = hvdc.get_converter_bus(INVERTER);

    if(not this->is_bus_in_allowed_range(rec_bus))
    {
        sstream<<"Warning. Bus "<<rec_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(not this->is_bus_in_allowed_range(inv_bus))
    {
        sstream<<"Warning. Bus "<<inv_bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<hvdc.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Hvdc will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = hvdc.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(rec_bus);
    terminal.append_bus(inv_bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_hvdc_exist(device_id))
    {
        sstream<<"Warning. "<<hvdc.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Hvdc.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Hvdc.capacity()==Hvdc.size())
    {
        sstream<<"Warning. Capacity limit ("<<Hvdc.capacity()<<") reached when appending Hvdc to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t hvdc_count = get_hvdc_count();
    Hvdc.push_back(hvdc);
    hvdc_index.set_device_index(device_id, hvdc_count);
}

void POWER_SYSTEM_DATABASE::append_equivalent_device(EQUIVALENT_DEVICE& edevice)
{
    ostringstream sstream;

    if(edevice.get_power_system_database()==NULL)
    {
        edevice.set_power_system_database(this);
    }

    if(this != edevice.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = edevice.get_power_system_database();
        sstream<<"Warning. "<<edevice.get_device_name()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not edevice.is_valid())
    {
        sstream<<"Warning. Failed to append invalid equivalent device to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t bus = edevice.get_equivalent_device_bus();

    if(not this->is_bus_in_allowed_range(bus))
    {
        sstream<<"Warning. Bus "<<bus<<" is not in the allowed range [1, "<<get_allowed_max_bus_number()<<"] when appending "<<edevice.get_device_name()<<" to power system database '"<<get_system_name()<<"'."<<endl
          <<"Equivalent device will not be appended into the database.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    string identifier = edevice.get_identifier();
    DEVICE_ID device_id;
    device_id.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier(identifier);

    if(this->is_equivalent_device_exist(device_id))
    {
        sstream<<"Warning. "<<edevice.get_device_name()<<" already exists in power system database '"<<get_system_name()<<"': Equivalent_device.\n"
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Equivalent_device.capacity()==Equivalent_device.size())
    {
        sstream<<"Warning. Capacity limit ("<<Equivalent_device.capacity()<<") reached when appending equivalent device to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t equivalent_device_count = get_equivalent_device_count();
    Equivalent_device.push_back(edevice);
    equivalent_device_index.set_device_index(device_id, equivalent_device_count);
 }

void POWER_SYSTEM_DATABASE::append_area(AREA& area)
{
    ostringstream sstream;

    if(area.get_power_system_database()==NULL)
        area.set_power_system_database(this);

    if(this != area.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = area.get_power_system_database();
        sstream<<"Warning. Area "<<area.get_area_number()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not area.is_valid())
    {
        sstream<<"Warning. Failed to append invalid area to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(this->is_area_exist(area.get_area_number()))
    {
        sstream<<"Warning. Area "<<area.get_area_number()<<" already exists in power system database '"<<get_system_name()<<"': Area."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Area.capacity()==Area.size())
    {
        sstream<<"Warning. Capacity limit ("<<Area.capacity()<<") reached when appending area to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t area_count = get_area_count();
    Area.push_back(area);
    area_index[area.get_area_number()]= area_count;
}

void POWER_SYSTEM_DATABASE::append_zone(ZONE& zone)
{
    ostringstream sstream;

    if(zone.get_power_system_database()==NULL)
        zone.set_power_system_database(this);

    if(this != zone.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = zone.get_power_system_database();
        sstream<<"Warning. Zone "<<zone.get_zone_number()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not zone.is_valid())
    {
        sstream<<"Warning. Failed to append invalid zone to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(this->is_zone_exist(zone.get_zone_number()))
    {
        sstream<<"Warning. Zone "<<zone.get_zone_number()<<" already exists in power system database '"<<get_system_name()<<"': Zone."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Zone.capacity()==Zone.size())
    {
        sstream<<"Warning. Capacity limit ("<<Zone.capacity()<<") reached when appending zone to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t zone_count = get_zone_count();
    Zone.push_back(zone);
    zone_index[zone.get_zone_number()]= zone_count;
}

void POWER_SYSTEM_DATABASE::append_owner(OWNER& owner)
{
    ostringstream sstream;

    if(owner.get_power_system_database()==NULL)
        owner.set_power_system_database(this);

    if(this != owner.get_power_system_database())
    {
        POWER_SYSTEM_DATABASE* db = owner.get_power_system_database();
        sstream<<"Warning. Owner "<<owner.get_owner_number()<<" was assigned to power system database '"<<db->get_system_name()<<"'."<<endl
          <<"It cannot be appended into the new power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(not owner.is_valid())
    {
        sstream<<"Warning. Failed to append invalid owner to power system database '"<<get_system_name()<<"'.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(this->is_owner_exist(owner.get_owner_number()))
    {
        sstream<<"Warning. Owner "<<owner.get_owner_number()<<" already exists in power system database '"<<get_system_name()<<"': Owner."<<endl
          <<"Duplicate copy is not allowed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(Owner.capacity()==Owner.size())
    {
        sstream<<"Warning. Capacity limit ("<<Owner.capacity()<<") reached when appending owner to power system database '"<<get_system_name()<<"'."<<endl
          <<"Increase capacity by modified steps_config.json.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    size_t owner_count = get_owner_count();
    Owner.push_back(owner);
    owner_index[owner.get_owner_number()]= owner_count;
}


bool POWER_SYSTEM_DATABASE::is_bus_exist(size_t bus) const
{
    if(bus == 0) return false;

    size_t index = this->get_bus_index(bus);
    if(index != INDEX_NOT_EXIST)
        return true;
    else
        return false;
}

bool POWER_SYSTEM_DATABASE::is_generator_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_generator_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_pe_source_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_pe_source_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_load_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_load_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_line_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_line_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_transformer_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_transformer_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_fixed_shunt_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_fixed_shunt_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_hvdc_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_hvdc_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_equivalent_device_exist(const DEVICE_ID& device_id) const
{
    size_t index = get_equivalent_device_index(device_id);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_area_exist(const size_t no) const
{
    size_t index = get_area_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_zone_exist(const size_t no) const
{
    size_t index = get_zone_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

bool POWER_SYSTEM_DATABASE::is_owner_exist(const size_t no) const
{
    size_t index = get_owner_index(no);
    if(index==INDEX_NOT_EXIST) return false;
    else return true;
}

void POWER_SYSTEM_DATABASE::change_bus_number(size_t original_bus_number, size_t new_bus_number)
{
    ostringstream sstream;
    if(not is_bus_exist(original_bus_number))
    {
        sstream<<"Warning. The original bus number to change ("<<original_bus_number<<") does not exist in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(is_bus_exist(new_bus_number))
    {
        sstream<<"Warning. The new bus number to change ("<<new_bus_number<<") already exists in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(not is_bus_in_allowed_range(new_bus_number))
    {
        sstream<<"Warning. The new bus number to change ("<<new_bus_number<<") exceeds the allowed maximum bus number range [1, "<<get_allowed_max_bus_number()<<"] in the power system database. No bus number will be changed.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }


    BUS* bus = get_bus(original_bus_number);

    bus->set_bus_number(new_bus_number);

    bus_index.set_bus_with_index(new_bus_number, get_bus_index(original_bus_number));
    bus_index.set_bus_with_index(original_bus_number, INDEX_NOT_EXIST);

    vector<GENERATOR*> gens = get_generators_connecting_to_bus(original_bus_number);
    size_t ngens = gens.size();
    for(size_t i=0; i!=ngens; ++i)
    {
        GENERATOR* gen = gens[i];
        DEVICE_ID old_did = gen->get_device_id();
        size_t index = get_generator_index(old_did);
        generator_index.set_device_index(old_did, INDEX_NOT_EXIST);

        gen->set_generator_bus(new_bus_number);
        DEVICE_ID new_did = gen->get_device_id();
        generator_index.set_device_index(new_did, index);
    }

    vector<PE_SOURCE*> pesources = get_pe_sources_connecting_to_bus(original_bus_number);
    size_t npesource = pesources.size();
    for(size_t i=0; i!=npesource; ++i)
    {
        PE_SOURCE* pesource = pesources[i];
        DEVICE_ID old_did = pesource->get_device_id();
        size_t index = get_pe_source_index(old_did);
        pe_source_index.set_device_index(old_did, INDEX_NOT_EXIST);

        pesource->set_source_bus(new_bus_number);
        DEVICE_ID new_did = pesource->get_device_id();
        pe_source_index.set_device_index(new_did, index);
    }

    vector<LOAD*> loads = get_loads_connecting_to_bus(original_bus_number);
    size_t nload = loads.size();
    for(size_t i=0; i!=nload; ++i)
    {
        LOAD* load = loads[i];
        DEVICE_ID old_did = load->get_device_id();
        size_t index = get_load_index(old_did);
        load_index.set_device_index(old_did, INDEX_NOT_EXIST);

        load->set_load_bus(new_bus_number);
        DEVICE_ID new_did = load->get_device_id();
        load_index.set_device_index(new_did, index);
    }

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(original_bus_number);
    size_t nfshunt = fshunts.size();
    for(size_t i=0; i!=nfshunt; ++i)
    {
        FIXED_SHUNT* fshunt = fshunts[i];
        DEVICE_ID old_did = fshunt->get_device_id();
        size_t index = get_fixed_shunt_index(old_did);
        fixed_shunt_index.set_device_index(old_did, INDEX_NOT_EXIST);

        fshunt->set_shunt_bus(new_bus_number);
        DEVICE_ID new_did = fshunt->get_device_id();
        fixed_shunt_index.set_device_index(new_did, index);
    }

    vector<LINE*> lines = get_lines_connecting_to_bus(original_bus_number);
    size_t nline = lines.size();
    for(size_t i=0; i!=nline; ++i)
    {
        LINE* line = lines[i];
        DEVICE_ID old_did = line->get_device_id();
        size_t index = get_line_index(old_did);
        line_index.set_device_index(old_did, INDEX_NOT_EXIST);

        if(line->get_sending_side_bus()==original_bus_number)
            line->set_sending_side_bus(new_bus_number);
        else
            line->set_receiving_side_bus(new_bus_number);

        DEVICE_ID new_did = line->get_device_id();
        line_index.set_device_index(new_did, index);

    }

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(original_bus_number);
    size_t nhvdc = hvdcs.size();
    for(size_t i=0; i!=nhvdc; ++i)
    {
        HVDC* hvdc = hvdcs[i];
        DEVICE_ID old_did = hvdc->get_device_id();
        size_t index = get_hvdc_index(old_did);
        hvdc_index.set_device_index(old_did, INDEX_NOT_EXIST);

        if(hvdc->get_converter_bus(RECTIFIER)==original_bus_number)
            hvdc->set_converter_bus(RECTIFIER, new_bus_number);
        else
            hvdc->set_converter_bus(INVERTER, new_bus_number);

        DEVICE_ID new_did = hvdc->get_device_id();
        hvdc_index.set_device_index(new_did, index);
    }

    vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(original_bus_number);
    size_t ntrans = transformers.size();
    for(size_t i=0; i!=ntrans; ++i)
    {
        TRANSFORMER* trans = transformers[i];
        DEVICE_ID old_did = trans->get_device_id();
        size_t index = get_transformer_index(old_did);
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
}

DEVICE* POWER_SYSTEM_DATABASE::get_device(DEVICE_ID& device_id)
{
    // this function is not tested
    if(not device_id.is_valid())
        return NULL;

    string dtype = device_id.get_device_type();
    if(dtype=="BUS")
        return (DEVICE*) get_bus(device_id);

    if(dtype=="GENERATOR")
        return (DEVICE*) get_generator(device_id);

    if(dtype=="PE SOURCE")
        return (DEVICE*) get_pe_source(device_id);

    if(dtype=="LOAD")
        return (DEVICE*) get_load(device_id);

    if(dtype=="FIXED SHUNT")
        return (DEVICE*) get_fixed_shunt(device_id);

    if(dtype=="LINE")
        return (DEVICE*) get_line(device_id);

    if(dtype=="HVDC")
        return (DEVICE*) get_hvdc(device_id);

    if(dtype=="TRANSFORMER")
        return (DEVICE*) get_transformer(device_id);

    if(dtype=="EQUIVALENT DEVICE")
        return (DEVICE*) get_equivalent_device(device_id);

    return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_bus(size_t bus)
{
    size_t index = bus_index.get_index_of_bus(bus);
    if(index!=INDEX_NOT_EXIST)
        return &(Bus[index]);
    else
        return NULL;
}

BUS* POWER_SYSTEM_DATABASE::get_bus(const DEVICE_ID & device_id)
{
    if(device_id.is_valid())
    {
        if(device_id.get_device_type()=="BUS")
        {
            TERMINAL terminal = device_id.get_device_terminal();
            size_t bus = terminal[0];
            return get_bus(bus);
        }
        else
            return NULL;
    }
    else
        return NULL;
}

GENERATOR* POWER_SYSTEM_DATABASE::get_generator(const DEVICE_ID & device_id)
{
    size_t index = generator_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Generator[index]);
    else
        return NULL;
}

PE_SOURCE* POWER_SYSTEM_DATABASE::get_pe_source(const DEVICE_ID & device_id)
{
    size_t index = pe_source_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(PE_Source[index]);
    else
        return NULL;
}

SOURCE* POWER_SYSTEM_DATABASE::get_source(const DEVICE_ID & device_id)
{
    SOURCE* source = NULL;
    source = (SOURCE*) get_generator(device_id);
    if(source!=NULL)
        return source;
    else
    {
        source = (SOURCE*) get_pe_source(device_id);
        return source;
    }
}

LOAD* POWER_SYSTEM_DATABASE::get_load(const DEVICE_ID & device_id)
{
    size_t index = load_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Load[index]);
    else
        return NULL;
}

LINE* POWER_SYSTEM_DATABASE::get_line(const DEVICE_ID & device_id)
{
    size_t index = line_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Line[index]);
    else
        return NULL;
}

TRANSFORMER* POWER_SYSTEM_DATABASE::get_transformer(const DEVICE_ID & device_id)
{
    size_t index = transformer_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Transformer[index]);
    else
        return NULL;
}

FIXED_SHUNT* POWER_SYSTEM_DATABASE::get_fixed_shunt(const DEVICE_ID & device_id)
{
    size_t index = fixed_shunt_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Fixed_shunt[index]);
    else
        return NULL;
}

HVDC* POWER_SYSTEM_DATABASE::get_hvdc(const DEVICE_ID & device_id)
{
    size_t index = hvdc_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Hvdc[index]);
    else
        return NULL;
}

EQUIVALENT_DEVICE* POWER_SYSTEM_DATABASE::get_equivalent_device(const DEVICE_ID & device_id)
{
    size_t index = equivalent_device_index.get_index_of_device(device_id);
    if(index!=INDEX_NOT_EXIST)
        return &(Equivalent_device[index]);
    else
        return NULL;
}

AREA* POWER_SYSTEM_DATABASE::get_area(const size_t no)
{
    size_t index = get_area_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Area[index]);
    else
        return NULL;
}

ZONE* POWER_SYSTEM_DATABASE::get_zone(const size_t no)
{
    size_t index = get_zone_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Zone[index]);
    else
        return NULL;
}

OWNER* POWER_SYSTEM_DATABASE::get_owner(const size_t no)
{
    size_t index = get_owner_index(no);
    if(index!=INDEX_NOT_EXIST)
        return &(Owner[index]);
    else
        return NULL;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_connecting_to_bus(const size_t bus)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_connecting_to_bus(const size_t bus)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_connected_to_bus(bus))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<PE_SOURCE*> POWER_SYSTEM_DATABASE::get_pe_sources_connecting_to_bus(const size_t bus)
{
    vector<PE_SOURCE*> device;
    device.reserve(8);

    size_t n = get_pe_source_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PE_Source[i].is_connected_to_bus(bus))
            device.push_back(&(PE_Source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_connecting_to_bus(const size_t bus)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<PE_SOURCE*> pesource_device;
    device.reserve(8);

    gen_device = get_generators_connecting_to_bus(bus);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    pesource_device = get_pe_sources_connecting_to_bus(bus);
    size_t npesource = pesource_device.size();
    for(size_t i=0; i!=npesource; ++i)
        device.push_back(pesource_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_connecting_to_bus(const size_t bus)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_connected_to_bus(bus))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_connecting_to_bus(const size_t bus)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_connected_to_bus(bus))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_connecting_to_bus(const size_t bus)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_connected_to_bus(bus))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_connecting_to_bus(const size_t bus)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_connected_to_bus(bus))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_connecting_to_bus(const size_t bus)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_connected_to_bus(bus))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_connecting_to_bus(const size_t bus)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_connected_to_bus(bus))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}



vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_connecting_to_bus(const size_t bus)
{
    vector<DEVICE*> devices = get_all_devices_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_connecting_to_bus(const size_t bus)
{
    vector<GENERATOR*> devices = get_generators_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pe_sources_device_id_connecting_to_bus(const size_t bus)
{
    vector<PE_SOURCE*> devices = get_pe_sources_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_connecting_to_bus(const size_t bus)
{
    vector<SOURCE*> devices = get_sources_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_connecting_to_bus(const size_t bus)
{
    vector<LOAD*> devices = get_loads_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_connecting_to_bus(const size_t bus)
{
    vector<LINE*> devices = get_lines_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_connecting_to_bus(const size_t bus)
{
    vector<TRANSFORMER*> devices = get_transformers_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_connecting_to_bus(const size_t bus)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_connecting_to_bus(const size_t bus)
{
    vector<HVDC*> devices = get_hvdcs_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_connecting_to_bus(const size_t bus)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_connecting_to_bus(bus);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}


vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_area(const size_t area)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<BUS*> buses = get_buses_in_area(area);
    n = buses.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_area(area);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_area(area);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_in_area(area);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_area(area);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_area(area);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_in_area(area);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_area(area);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_area(const size_t area)
{
    vector<BUS*> device;
    device.reserve(8);

    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].is_in_area(area))
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_area(const size_t area)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_in_area(area))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<PE_SOURCE*> POWER_SYSTEM_DATABASE::get_pe_sources_in_area(const size_t area)
{
    vector<PE_SOURCE*> device;
    device.reserve(8);

    size_t n = get_pe_source_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PE_Source[i].is_in_area(area))
            device.push_back(&(PE_Source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_area(const size_t area)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<PE_SOURCE*> pesource_device;
    device.reserve(8);

    gen_device = get_generators_in_area(area);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    pesource_device = get_pe_sources_in_area(area);
    size_t npesource = pesource_device.size();
    for(size_t i=0; i!=npesource; ++i)
        device.push_back(pesource_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_area(const size_t area)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_in_area(area))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_in_area(const size_t area)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_in_area(area))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_area(const size_t area)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_in_area(area))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_area(const size_t area)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_in_area(area))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_in_area(const size_t area)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_in_area(area))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_area(const size_t area)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_in_area(area))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}



vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_area(const size_t area)
{
    vector<DEVICE*> devices = get_all_devices_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_area(const size_t area)
{
    vector<BUS*> devices = get_buses_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_area(const size_t area)
{
    vector<GENERATOR*> devices = get_generators_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pe_sources_device_id_in_area(const size_t area)
{
    vector<PE_SOURCE*> devices = get_pe_sources_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_area(const size_t area)
{
    vector<SOURCE*> devices = get_sources_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_area(const size_t area)
{
    vector<LOAD*> devices = get_loads_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_in_area(const size_t area)
{
    vector<LINE*> devices = get_lines_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_area(const size_t area)
{
    vector<TRANSFORMER*> devices = get_transformers_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_area(const size_t area)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_in_area(const size_t area)
{
    vector<HVDC*> devices = get_hvdcs_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_area(const size_t area)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_area(area);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE*> POWER_SYSTEM_DATABASE::get_all_devices_in_zone(const size_t zone)
{
    vector<DEVICE*> devices;
    size_t n=0;

    vector<BUS*> buses = get_buses_in_zone(zone);
    n = buses.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(buses[i]);

    vector<SOURCE*> sources = get_sources_in_zone(zone);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(sources[i]);

    vector<LOAD*> loads = get_loads_in_zone(zone);
    n = loads.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(loads[i]);

    vector<LINE*> lines = get_lines_in_zone(zone);
    n = lines.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(lines[i]);

    vector<TRANSFORMER*> transes = get_transformers_in_zone(zone);
    n = transes.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(transes[i]);

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_in_zone(zone);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(fshunts[i]);

    vector<HVDC*> hvdcs = get_hvdcs_in_zone(zone);
    n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(hvdcs[i]);

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_in_zone(zone);
    n = edevices.size();
    for(size_t i=0; i!=n; ++i)
        devices.push_back(edevices[i]);

    return devices;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_in_zone(const size_t zone)
{
    vector<BUS*> device;
    device.reserve(8);

    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].is_in_zone(zone))
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_generators_in_zone(const size_t zone)
{
    vector<GENERATOR*> device;
    device.reserve(8);

    size_t n = get_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].is_in_zone(zone))
            device.push_back(&(Generator[i]));
    }
    return device;
}

vector<PE_SOURCE*> POWER_SYSTEM_DATABASE::get_pe_sources_in_zone(const size_t zone)
{
    vector<PE_SOURCE*> device;
    device.reserve(8);

    size_t n = get_pe_source_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PE_Source[i].is_in_zone(zone))
            device.push_back(&(PE_Source[i]));
    }
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_sources_in_zone(const size_t zone)
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<PE_SOURCE*> pesource_device;
    device.reserve(8);

    gen_device = get_generators_in_zone(zone);
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    pesource_device = get_pe_sources_in_zone(zone);
    size_t npesource = pesource_device.size();
    for(size_t i=0; i!=npesource; ++i)
        device.push_back(pesource_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_loads_in_zone(const size_t zone)
{
    vector<LOAD*> device;
    device.reserve(8);

    size_t n = get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].is_in_zone(zone))
            device.push_back(&(Load[i]));
    }
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_lines_in_zone(const size_t zone)
{
    vector<LINE*> device;
    device.reserve(8);

    size_t n = get_line_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Line[i].is_in_zone(zone))
            device.push_back(&(Line[i]));
    }
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_transformers_in_zone(const size_t zone)
{
    vector<TRANSFORMER*> device;
    device.reserve(8);

    size_t n = get_transformer_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Transformer[i].is_in_zone(zone))
            device.push_back(&(Transformer[i]));
    }
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_fixed_shunts_in_zone(const size_t zone)
{
    vector<FIXED_SHUNT*> device;
    device.reserve(8);

    size_t n = get_fixed_shunt_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Fixed_shunt[i].is_in_zone(zone))
            device.push_back(&(Fixed_shunt[i]));
    }
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_hvdcs_in_zone(const size_t zone)
{
    vector<HVDC*> device;
    device.reserve(8);

    size_t n = get_hvdc_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Hvdc[i].is_in_zone(zone))
            device.push_back(&(Hvdc[i]));
    }
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_equivalent_devices_in_zone(const size_t zone)
{
    vector<EQUIVALENT_DEVICE*> device;
    device.reserve(8);

    size_t n = get_equivalent_device_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Equivalent_device[i].is_in_zone(zone))
            device.push_back(&(Equivalent_device[i]));
    }
    return device;
}



vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_devices_device_id_in_zone(const size_t zone)
{
    vector<DEVICE*> devices = get_all_devices_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_buses_device_id_in_zone(const size_t zone)
{
    vector<BUS*> devices = get_buses_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_generators_device_id_in_zone(const size_t zone)
{
    vector<GENERATOR*> devices = get_generators_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_pe_sources_device_id_in_zone(const size_t zone)
{
    vector<PE_SOURCE*> devices = get_pe_sources_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_sources_device_id_in_zone(const size_t zone)
{
    vector<SOURCE*> devices = get_sources_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_loads_device_id_in_zone(const size_t zone)
{
    vector<LOAD*> devices = get_loads_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_lines_device_id_in_zone(const size_t zone)
{
    vector<LINE*> devices = get_lines_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_transformers_device_id_in_zone(const size_t zone)
{
    vector<TRANSFORMER*> devices = get_transformers_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_fixed_shunts_device_id_in_zone(const size_t zone)
{
    vector<FIXED_SHUNT*> devices = get_fixed_shunts_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_hvdcs_device_id_in_zone(const size_t zone)
{
    vector<HVDC*> devices = get_hvdcs_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_equivalent_devices_device_id_in_zone(const size_t zone)
{
    vector<EQUIVALENT_DEVICE*> devices = get_equivalent_devices_in_zone(zone);
    size_t n = devices.size();

    vector<DEVICE_ID> dids;
    for(size_t i=0; i!=n; ++i)
        dids.push_back(devices[i]->get_device_id());

    return dids;
}


vector<BUS*> POWER_SYSTEM_DATABASE::get_all_buses()
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Bus[i]));
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_buses_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner)
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    double vbase=0.0, v=0.0;
    size_t this_area=0, this_zone=0, this_owner=0;
    for(size_t i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if(area!=0 and this_area!=area)
            continue;
        if(zone!=0 and this_zone!=zone)
            continue;
        if(owner!=0 and this_owner!=owner)
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<BUS*> POWER_SYSTEM_DATABASE::get_all_in_service_buses()
{
    vector<BUS*> device;
    size_t n = get_bus_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            device.push_back(&(Bus[i]));
    }
    return device;
}

vector<GENERATOR*> POWER_SYSTEM_DATABASE::get_all_generators()
{
    vector<GENERATOR*> device;
    size_t n = get_generator_count();
    device.reserve(n);

    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Generator[i]));
    return device;
}

vector<PE_SOURCE*> POWER_SYSTEM_DATABASE::get_all_pe_sources()
{
    vector<PE_SOURCE*> device;
    size_t n = get_pe_source_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(PE_Source[i]));
    return device;
}

vector<SOURCE*> POWER_SYSTEM_DATABASE::get_all_sources()
{
    vector<SOURCE*> device;
    vector<GENERATOR*> gen_device;
    vector<PE_SOURCE*> pesource_device;
    device.reserve(get_source_count());

    gen_device = get_all_generators();
    size_t ngens = gen_device.size();
    for(size_t i=0; i!=ngens; ++i)
        device.push_back(gen_device[i]);

    pesource_device = get_all_pe_sources();
    size_t npesource = pesource_device.size();
    for(size_t i=0; i!=npesource; ++i)
        device.push_back(pesource_device[i]);

    return device;
}

vector<LOAD*> POWER_SYSTEM_DATABASE::get_all_loads()
{
    vector<LOAD*> device;
    size_t n = get_load_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Load[i]));
    return device;
}

vector<LINE*> POWER_SYSTEM_DATABASE::get_all_lines()
{
    vector<LINE*> device;
    size_t n = get_line_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Line[i]));
    return device;
}

vector<TRANSFORMER*> POWER_SYSTEM_DATABASE::get_all_transformers()
{
    vector<TRANSFORMER*> device;
    size_t n = get_transformer_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Transformer[i]));
    return device;
}

vector<FIXED_SHUNT*> POWER_SYSTEM_DATABASE::get_all_fixed_shunts()
{
    vector<FIXED_SHUNT*> device;
    size_t n = get_fixed_shunt_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Fixed_shunt[i]));
    return device;
}

vector<HVDC*> POWER_SYSTEM_DATABASE::get_all_hvdcs()
{
    vector<HVDC*> device;
    size_t n = get_hvdc_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Hvdc[i]));
    return device;
}

vector<EQUIVALENT_DEVICE*> POWER_SYSTEM_DATABASE::get_all_equivalent_devices()
{
    vector<EQUIVALENT_DEVICE*> device;
    size_t n = get_equivalent_device_count();
    device.reserve(n);
    for(size_t i=0; i!=n; ++i)
        device.push_back(&(Equivalent_device[i]));
    return device;
}

vector<AREA*> POWER_SYSTEM_DATABASE::get_all_areas()
{
    vector<AREA*> areas;
    areas.reserve(get_area_count());

    size_t n = get_area_count();
    for(size_t i=0; i!=n; ++i)
        areas.push_back(&(Area[i]));
    return areas;
}

vector<ZONE*> POWER_SYSTEM_DATABASE::get_all_zones()
{
    vector<ZONE*> zones;
    zones.reserve(get_zone_count());

    size_t n = get_zone_count();
    for(size_t i=0; i!=n; ++i)
        zones.push_back(&(Zone[i]));
    return zones;
}

vector<OWNER*> POWER_SYSTEM_DATABASE::get_all_owners()
{
    vector<OWNER*> owners;
    owners.reserve(get_owner_count());

    size_t n = get_owner_count();
    for(size_t i=0; i!=n; ++i)
        owners.push_back(&(Owner[i]));
    return owners;
}



vector<size_t> POWER_SYSTEM_DATABASE::get_all_buses_number()
{
    vector<size_t> buses_number;
    size_t n=get_bus_count();
    buses_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        buses_number.push_back(Bus[i].get_bus_number());
    return buses_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_buses_number_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner)
{
    vector<size_t> buses_number;
    size_t n = get_bus_count();
    buses_number.reserve(n);
    double vbase=0.0, v=0.0;
    size_t this_area=0, this_zone=0, this_owner=0;
    for(size_t i=0; i!=n; ++i)
    {
        vbase = Bus[i].get_base_voltage_in_kV();
        v = Bus[i].get_voltage_in_pu();
        this_area = Bus[i].get_area_number();
        this_zone = Bus[i].get_zone_number();
        this_owner = Bus[i].get_owner_number();

        if(area!=0 and this_area!=area)
            continue;
        if(zone!=0 and this_zone!=zone)
            continue;
        if(owner!=0 and this_owner!=owner)
            continue;
        if(vbase>=vbase_kV_min and vbase<=vbase_kV_max and v>=v_pu_min and v<=v_pu_max)
            buses_number.push_back(Bus[i].get_bus_number());
    }
    return buses_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_in_service_buses_number()
{
    vector<size_t> buses_number;
    size_t n=get_bus_count();
    buses_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()!=OUT_OF_SERVICE)
            buses_number.push_back(Bus[i].get_bus_number());
    }
    return buses_number;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_generators_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_generator_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Generator[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_pe_sources_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_pe_source_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(PE_Source[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_sources_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_source_count();
    dids.reserve(n);
    size_t ngens = get_generator_count();
    for(size_t i=0; i!=ngens; ++i)
        dids.push_back(Generator[i].get_device_id());
    size_t npesource = get_pe_source_count();
    for(size_t i=0; i!=npesource; ++i)
        dids.push_back(PE_Source[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_loads_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_load_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Load[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_lines_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_line_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Line[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_transformers_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_transformer_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Transformer[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_fixed_shunts_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_fixed_shunt_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Fixed_shunt[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_hvdcs_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_hvdc_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Hvdc[i].get_device_id());
    return dids;
}

vector<DEVICE_ID> POWER_SYSTEM_DATABASE::get_all_equivalent_devices_device_id()
{
    vector<DEVICE_ID> dids;
    size_t n=get_equivalent_device_count();
    dids.reserve(n);
    for(size_t i=0; i!=n; ++i)
        dids.push_back(Equivalent_device[i].get_device_id());
    return dids;
}
vector<size_t> POWER_SYSTEM_DATABASE::get_all_areas_number()
{
    vector<size_t> areas_number;
    size_t n=get_area_count();
    areas_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        areas_number.push_back(Area[i].get_area_number());
    return areas_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_zones_number()
{
    vector<size_t> zones_number;
    size_t n=get_zone_count();
    zones_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        zones_number.push_back(Zone[i].get_zone_number());
    return zones_number;
}

vector<size_t> POWER_SYSTEM_DATABASE::get_all_owners_number()
{
    vector<size_t> owners_number;
    size_t n=get_owner_count();
    owners_number.reserve(n);
    for(size_t i=0; i!=n; ++i)
        owners_number.push_back(Owner[i].get_owner_number());
    return owners_number;
}


size_t POWER_SYSTEM_DATABASE::get_bus_count() const
{
    return Bus.size();
}

size_t POWER_SYSTEM_DATABASE::get_in_service_bus_count() const
{
    return in_service_bus_count;
}

void POWER_SYSTEM_DATABASE::update_in_service_bus_count()
{
    size_t n = Bus.size();
    size_t n_out_of_service = 0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_type()==OUT_OF_SERVICE)
            n_out_of_service++;
    }

    in_service_bus_count = n-n_out_of_service;
}

size_t POWER_SYSTEM_DATABASE::get_generator_count() const
{
    return Generator.size();
}

size_t POWER_SYSTEM_DATABASE::get_pe_source_count() const
{
    return PE_Source.size();
}

size_t POWER_SYSTEM_DATABASE::get_source_count() const
{
    return get_generator_count()+get_pe_source_count();
}

size_t POWER_SYSTEM_DATABASE::get_load_count() const
{
    return Load.size();
}

size_t POWER_SYSTEM_DATABASE::get_line_count() const
{
    return Line.size();
}

size_t POWER_SYSTEM_DATABASE::get_transformer_count() const
{
    return Transformer.size();
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_count() const
{
    return Fixed_shunt.size();
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_count() const
{
    return Hvdc.size();
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_count() const
{
    return Equivalent_device.size();
}

size_t POWER_SYSTEM_DATABASE::get_area_count() const
{
    return Area.size();
}

size_t POWER_SYSTEM_DATABASE::get_zone_count() const
{
    return Zone.size();
}

size_t POWER_SYSTEM_DATABASE::get_owner_count() const
{
    return Owner.size();
}

size_t POWER_SYSTEM_DATABASE::get_bus_index(size_t bus) const
{
    return bus_index.get_index_of_bus(bus);
}

size_t POWER_SYSTEM_DATABASE::get_generator_index(const DEVICE_ID & device_id) const
{
    return generator_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_pe_source_index(const DEVICE_ID & device_id) const
{
    return pe_source_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_load_index(const DEVICE_ID & device_id) const
{
    return load_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_line_index(const DEVICE_ID & device_id) const
{
    return line_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_transformer_index(const DEVICE_ID & device_id) const
{
    return transformer_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_fixed_shunt_index(const DEVICE_ID & device_id) const
{
    return fixed_shunt_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_hvdc_index(const DEVICE_ID & device_id) const
{
    return hvdc_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_equivalent_device_index(const DEVICE_ID & device_id) const
{
    return equivalent_device_index.get_index_of_device(device_id);
}

size_t POWER_SYSTEM_DATABASE::get_area_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = area_index.begin();
    iter = area_index.find(no);
    if(iter==area_index.end()) return INDEX_NOT_EXIST;
    else                       return iter->second;
}

size_t POWER_SYSTEM_DATABASE::get_zone_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = zone_index.begin();
    iter = zone_index.find(no);
    if(iter==zone_index.end()) return INDEX_NOT_EXIST;
    else                       return iter->second;
}

size_t POWER_SYSTEM_DATABASE::get_owner_index(const size_t no) const
{
    map<size_t, size_t>::const_iterator iter = owner_index.begin();
    iter = owner_index.find(no);
    if(iter==owner_index.end()) return INDEX_NOT_EXIST;
    else                        return iter->second;
}

size_t POWER_SYSTEM_DATABASE::bus_name2bus_number(const string name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_bus_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Bus[i].get_bus_name()==trimmed_name)
            return Bus[i].get_bus_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::area_name2area_number(const string name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_area_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Area[i].get_area_name()==trimmed_name)
            return Area[i].get_area_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::zone_name2zone_number(const string name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_zone_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Zone[i].get_zone_name()==trimmed_name)
            return Zone[i].get_zone_number();
    }
    return 0;
}

size_t POWER_SYSTEM_DATABASE::owner_name2owner_number(const string name) const
{
    string trimmed_name = trim_string(name);
    size_t n = get_owner_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(Owner[i].get_owner_name()==trimmed_name)
            return Owner[i].get_owner_number();
    }
    return 0;
}

string POWER_SYSTEM_DATABASE::bus_number2bus_name(size_t number)
{
    if(not is_bus_in_allowed_range(number))
        return "";

    BUS* bus = get_bus(number);
    if(bus==NULL)
        return "";
    else
        return bus->get_bus_name();
}


string POWER_SYSTEM_DATABASE::area_number2area_name(size_t number)
{
    AREA* area = get_area(number);
    if(area==NULL)
        return "";
    else
        return area->get_area_name();
}

string POWER_SYSTEM_DATABASE::zone_number2zone_name(size_t number)
{
    ZONE* zone = get_zone(number);
    if(zone==NULL)
        return "";
    else
        return zone->get_zone_name();
}

string POWER_SYSTEM_DATABASE::owner_number2owner_name(size_t number)
{
    OWNER* owner = get_owner(number);
    if(owner==NULL)
        return "";
    else
        return owner->get_owner_name();
}



void POWER_SYSTEM_DATABASE::scale_load_power(DEVICE_ID did, double scale)
{
    LOAD* load = get_load(did);
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
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_connecting_to_bus(bus);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_area(area_number);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_loads_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> loads = get_loads_device_id_in_zone(zone_number);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        scale_load_power(loads[i], scale);
}



void POWER_SYSTEM_DATABASE::scale_source_power(DEVICE_ID did, double scale)
{
    SOURCE* source = get_source(did);
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
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_connecting_to_bus(bus);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_area(area_number);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_sources_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> sources = get_sources_device_id_in_zone(zone_number);
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generator_power(DEVICE_ID did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_generators_power(double scale)
{
    vector<DEVICE_ID> generators = get_all_generators_device_id();
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_generator_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_connecting_to_bus(bus);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_area(area_number);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_generators_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> generators = get_generators_device_id_in_zone(zone_number);
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(generators[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pe_source_power(DEVICE_ID did, double scale)
{
    scale_source_power(did, scale);
}

void POWER_SYSTEM_DATABASE::scale_all_pe_sources_power(double scale)
{
    vector<DEVICE_ID> pe_sources = get_all_pe_sources_device_id();
    size_t n = pe_sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_pe_source_power(pe_sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pe_sources_power_at_bus(size_t bus, double scale)
{
    vector<DEVICE_ID> pe_sources = get_pe_sources_device_id_connecting_to_bus(bus);
    size_t n = pe_sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(pe_sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pe_sources_power_in_area(size_t area_number, double scale)
{
    vector<DEVICE_ID> pe_sources = get_pe_sources_device_id_in_area(area_number);
    size_t n = pe_sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(pe_sources[i], scale);
}

void POWER_SYSTEM_DATABASE::scale_pe_sources_power_in_zone(size_t zone_number, double scale)
{
    vector<DEVICE_ID> pe_sources = get_pe_sources_device_id_in_zone(zone_number);
    size_t n = pe_sources.size();
    for(size_t i=0; i!=n; ++i)
        scale_source_power(pe_sources[i], scale);
}

void POWER_SYSTEM_DATABASE::clear_bus(size_t bus)
{
    if(not is_bus_exist(bus)) return;

    clear_generators_connecting_to_bus(bus);
    clear_pe_sources_connecting_to_bus(bus);
    clear_loads_connecting_to_bus(bus);
    clear_lines_connecting_to_bus(bus);
    clear_transformers_connecting_to_bus(bus);
    clear_fixed_shunts_connecting_to_bus(bus);
    clear_hvdcs_connecting_to_bus(bus);
    clear_equivalent_devices_connecting_to_bus(bus);

    size_t current_index = get_bus_index(bus);

    vector<BUS>::iterator iter = Bus.begin();
    std::advance(iter, current_index);
    Bus.erase(iter);
    bus_index.set_bus_with_index(bus, INDEX_NOT_EXIST);

    size_t max_bus_number = bus_index.get_max_bus_number();

    size_t max_bus_number_plus_1 = max_bus_number+1;
    for(size_t i=1; i!=max_bus_number_plus_1; ++i)
    {
        size_t index = bus_index.get_index_of_bus(i);
        if(index!= INDEX_NOT_EXIST and index>current_index)
            bus_index.set_bus_with_index(i, index-1);
    }
    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::clear_all_buses()
{
    Bus.clear();
    bus_index.clear();

    clear_all_generators();
    clear_all_pe_sources();
    clear_all_loads();
    clear_all_lines();
    clear_all_transformers();
    clear_all_fixed_shunts();
    clear_all_hvdcs();
    clear_all_equivalent_devices();
    clear_all_areas();
    clear_all_zones();
    clear_all_owners();
    update_in_service_bus_count();
}


void POWER_SYSTEM_DATABASE::clear_generator(DEVICE_ID& device_id)
{
    if(not is_generator_exist(device_id)) return;

    size_t current_index = get_generator_index(device_id);

    vector<GENERATOR>::iterator iter_generator = Generator.begin();

    std::advance(iter_generator, current_index);
    Generator.erase(iter_generator);
    generator_index.set_device_index(device_id, INDEX_NOT_EXIST);

    generator_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_generators_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("GENERATOR");
    while(true)
    {
        vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);
        if(generator.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(generator[0]->get_identifier());
        clear_generator(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_generators()
{
    Generator.clear();
    generator_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_pe_source(DEVICE_ID& device_id)
{
    if(not is_pe_source_exist(device_id)) return;

    size_t current_index = get_pe_source_index(device_id);

    vector<PE_SOURCE>::iterator iter_pesource = PE_Source.begin();

    std::advance(iter_pesource, current_index);
    PE_Source.erase(iter_pesource);
    pe_source_index.set_device_index(device_id, INDEX_NOT_EXIST);

    pe_source_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}


void POWER_SYSTEM_DATABASE::clear_pe_sources_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("PE SOURCE");
    while(true)
    {
        vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
        if(pesource.size()==0)
            break;
        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(pesource[0]->get_identifier());
        clear_pe_source(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_pe_sources()
{
    PE_Source.clear();
    pe_source_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_sources_connecting_to_bus(const size_t bus)
{
    clear_generators_connecting_to_bus(bus);
    clear_pe_sources_connecting_to_bus(bus);
}

void POWER_SYSTEM_DATABASE::clear_all_sources()
{
    clear_all_generators();
    clear_all_pe_sources();
}

void POWER_SYSTEM_DATABASE::clear_load(DEVICE_ID& device_id)
{
    if(not is_load_exist(device_id)) return;

    size_t current_index = get_load_index(device_id);

    vector<LOAD>::iterator iter_load = Load.begin();


    std::advance(iter_load, current_index);
    Load.erase(iter_load);
    load_index.set_device_index(device_id, INDEX_NOT_EXIST);

    load_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_loads_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("LOAD");
    while(true)
    {
        vector<LOAD*> load = get_loads_connecting_to_bus(bus);
        if(load.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(bus);
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(load[0]->get_identifier());
        clear_load(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_loads()
{
    Load.clear();
    load_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_line(DEVICE_ID& device_id)
{
    if(not is_line_exist(device_id)) return;

    size_t current_index = get_line_index(device_id);

    vector<LINE>::iterator iter_line = Line.begin();


    std::advance(iter_line, current_index);
    Line.erase(iter_line);
    line_index.set_device_index(device_id, INDEX_NOT_EXIST);

    line_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_lines_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("LINE");
    while(true)
    {
        vector<LINE*> line = get_lines_connecting_to_bus(bus);
        if(line.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(line[0]->get_sending_side_bus());
        terminal.append_bus(line[0]->get_receiving_side_bus());
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(line[0]->get_identifier());
        clear_line(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_lines()
{
    Line.clear();
    line_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_transformer(DEVICE_ID& device_id)
{
    if(not is_transformer_exist(device_id)) return;

    size_t current_index = get_transformer_index(device_id);

    vector<TRANSFORMER>::iterator iter_transformer = Transformer.begin();

    std::advance(iter_transformer, current_index);
    Transformer.erase(iter_transformer);
    transformer_index.set_device_index(device_id, INDEX_NOT_EXIST);

    transformer_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_transformers_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("TRANSFORMER");
    while(true)
    {
        vector<TRANSFORMER*> transformer = get_transformers_connecting_to_bus(bus);

        if(transformer.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(transformer[0]->get_winding_bus(PRIMARY_SIDE));
        terminal.append_bus(transformer[0]->get_winding_bus(SECONDARY_SIDE));
        terminal.append_bus(transformer[0]->get_winding_bus(TERTIARY_SIDE));
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(transformer[0]->get_identifier());
        clear_transformer(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_transformers()
{
    Transformer.clear();
    transformer_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_fixed_shunt(DEVICE_ID& device_id)
{
    if(not is_fixed_shunt_exist(device_id))
        return;

    size_t current_index = get_fixed_shunt_index(device_id);

    vector<FIXED_SHUNT> fs;
    size_t nfshunt = get_fixed_shunt_count();
    for(size_t i=0; i!=nfshunt; ++i)
        if(i!=current_index) fs.push_back(Fixed_shunt[i]);

    Fixed_shunt.clear();
    Fixed_shunt = fs;

    fixed_shunt_index.set_device_index(device_id, INDEX_NOT_EXIST);

    fixed_shunt_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}


void POWER_SYSTEM_DATABASE::clear_fixed_shunts_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    device_id.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);
    while(true)
    {
        vector<FIXED_SHUNT*> fixed_shunt = get_fixed_shunts_connecting_to_bus(bus);
        if(fixed_shunt.size()==0)
            break;

        device_id.set_device_identifier(fixed_shunt[0]->get_identifier());
        clear_fixed_shunt(device_id);
        fixed_shunt.clear();
    }
}

void POWER_SYSTEM_DATABASE::clear_all_fixed_shunts()
{
    Fixed_shunt.clear();
    fixed_shunt_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_hvdc(DEVICE_ID& device_id)
{
    if(not is_hvdc_exist(device_id)) return;

    size_t current_index = get_hvdc_index(device_id);

    vector<HVDC>::iterator iter_hvdc = Hvdc.begin();


    std::advance(iter_hvdc, current_index);
    Hvdc.erase(iter_hvdc);
    hvdc_index.set_device_index(device_id, INDEX_NOT_EXIST);

    hvdc_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_hvdcs_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("HVDC");
    while(true)
    {
        vector<HVDC*> hvdc = get_hvdcs_connecting_to_bus(bus);
        if(hvdc.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(hvdc[0]->get_converter_bus(RECTIFIER));
        terminal.append_bus(hvdc[0]->get_converter_bus(INVERTER));
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(hvdc[0]->get_identifier());
        clear_hvdc(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_hvdcs()
{
    Hvdc.clear();
    hvdc_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_equivalent_device(DEVICE_ID& device_id)
{
    if(not is_equivalent_device_exist(device_id)) return;

    size_t current_index = get_equivalent_device_index(device_id);

    vector<EQUIVALENT_DEVICE>::iterator iter_edevice = Equivalent_device.begin();


    std::advance(iter_edevice, current_index);
    Equivalent_device.erase(iter_edevice);
    equivalent_device_index.set_device_index(device_id, INDEX_NOT_EXIST);

    equivalent_device_index.decrease_index_by_1_for_device_with_index_greater_than(current_index);
}

void POWER_SYSTEM_DATABASE::clear_equivalent_devices_connecting_to_bus(const size_t bus)
{
    DEVICE_ID device_id;
    TERMINAL terminal;
    device_id.set_device_type("EQUIVALENT DEVICE");
    while(true)
    {
        vector<EQUIVALENT_DEVICE*> edevice = get_equivalent_devices_connecting_to_bus(bus);
        if(edevice.size()==0)
            break;

        terminal.clear();
        terminal.append_bus(edevice[0]->get_equivalent_device_bus());
        device_id.set_device_terminal(terminal);
        device_id.set_device_identifier(edevice[0]->get_identifier());
        clear_equivalent_device(device_id);
    }
}

void POWER_SYSTEM_DATABASE::clear_all_equivalent_devices()
{
    Equivalent_device.clear();
    equivalent_device_index.clear();
}


void POWER_SYSTEM_DATABASE::clear_all_areas()
{
    Area.clear();
    area_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_all_zones()
{
    Zone.clear();
    zone_index.clear();
}

void POWER_SYSTEM_DATABASE::clear_all_owners()
{
    Owner.clear();
    owner_index.clear();
}


void POWER_SYSTEM_DATABASE::trip_bus(size_t bus)
{
    if(not is_bus_exist(bus)) return;

    size_t index = get_bus_index(bus);
    if(Bus[index].get_bus_type()==OUT_OF_SERVICE)
        return;

    ostringstream sstream;

    sstream<<"The following devices are tripped before bus "<<bus<<" is tripped."<<endl;

    size_t n = 0;

    vector<GENERATOR*> gens = get_generators_connecting_to_bus(bus);
    n=gens.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(gens[i]->get_status()==true)
        {
            gens[i]->set_status(false);
            sstream<<gens[i]->get_device_name()<<endl;
        }
    }

    vector<PE_SOURCE*> pes = get_pe_sources_connecting_to_bus(bus);
    n=pes.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pes[i]->get_status()==true)
        {
            pes[i]->set_status(false);
            sstream<<pes[i]->get_device_name()<<endl;
        }
    }

    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    n=loads.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(loads[i]->get_status()==true)
        {
            loads[i]->set_status(false);
            sstream<<loads[i]->get_device_name()<<endl;
        }
    }

    vector<FIXED_SHUNT*> fshunts = get_fixed_shunts_connecting_to_bus(bus);
    n=fshunts.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(fshunts[i]->get_status()==true)
        {
            fshunts[i]->set_status(false);
            sstream<<fshunts[i]->get_device_name()<<endl;
        }
    }

    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    n=lines.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(lines[i]->get_sending_side_breaker_status()==true or lines[i]->get_receiving_side_breaker_status()==true)
        {
            lines[i]->set_sending_side_breaker_status(false);
            lines[i]->set_receiving_side_breaker_status(false);
            sstream<<lines[i]->get_device_name()<<endl;
        }
    }

    vector<TRANSFORMER*> transes = get_transformers_connecting_to_bus(bus);
    n=transes.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(transes[i]->get_winding_breaker_status(PRIMARY_SIDE)==true or transes[i]->get_winding_breaker_status(SECONDARY_SIDE)==true or
           (transes[i]->is_three_winding_transformer() and transes[i]->get_winding_breaker_status(TERTIARY_SIDE)==true) )
        {
            transes[i]->set_winding_breaker_status(PRIMARY_SIDE, false);
            transes[i]->set_winding_breaker_status(SECONDARY_SIDE, false);
            if(transes[i]->is_three_winding_transformer())
                transes[i]->set_winding_breaker_status(TERTIARY_SIDE, false);

            sstream<<transes[i]->get_device_name()<<endl;
        }
    }

    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    n=hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(hvdcs[i]->get_status()==true)
        {
            hvdcs[i]->set_status(false);
            sstream<<hvdcs[i]->get_device_name()<<endl;
        }
    }

    vector<EQUIVALENT_DEVICE*> edevices = get_equivalent_devices_connecting_to_bus(bus);
    n=edevices.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(edevices[i]->get_status()==true)
        {
            edevices[i]->set_status(false);
            sstream<<edevices[i]->get_device_name()<<endl;
        }
    }

    Bus[index].set_bus_type(OUT_OF_SERVICE);
    sstream<<"Bus "<<bus<<" is tripped.";
    show_information_with_leading_time_stamp(sstream);

    update_in_service_bus_count();
}

void POWER_SYSTEM_DATABASE::check_device_status_for_out_of_service_buses()
{
    vector<BUS*> buses = get_all_buses();
    BUS* bus;
    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()==OUT_OF_SERVICE)
        {
            size_t bus_number = bus->get_bus_number();
            check_source_status_for_out_of_service_bus(bus_number);
            check_load_status_for_out_of_service_bus(bus_number);
            check_fixed_shunt_status_for_out_of_service_bus(bus_number);
            check_line_status_for_out_of_service_bus(bus_number);
            check_transformer_status_for_out_of_service_bus(bus_number);
            check_hvdc_status_for_out_of_service_bus(bus_number);
        }
    }
}

void POWER_SYSTEM_DATABASE::check_source_status_for_out_of_service_bus(size_t bus)
{
    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);
    SOURCE* source;
    size_t nsource = sources.size();
    for(size_t i=0; i!=nsource; ++i)
    {
        source = sources[i];
        if(source->get_status()==true)
            source->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_load_status_for_out_of_service_bus(size_t bus)
{
    vector<LOAD*> loads = get_loads_connecting_to_bus(bus);
    LOAD* load;
    size_t nload = loads.size();
    for(size_t i=0; i!=nload; ++i)
    {
        load = loads[i];
        if(load->get_status()==true)
            load->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_fixed_shunt_status_for_out_of_service_bus(size_t bus)
{
    vector<FIXED_SHUNT*> shunts = get_fixed_shunts_connecting_to_bus(bus);
    FIXED_SHUNT* shunt;
    size_t nshunt = shunts.size();
    for(size_t i=0; i!=nshunt; ++i)
    {
        shunt = shunts[i];
        if(shunt->get_status()==true)
            shunt->set_status(false);
    }
}

void POWER_SYSTEM_DATABASE::check_line_status_for_out_of_service_bus(size_t bus)
{
    vector<LINE*> lines = get_lines_connecting_to_bus(bus);
    LINE* line;
    size_t nline = lines.size();
    for(size_t i=0; i!=nline; ++i)
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

void POWER_SYSTEM_DATABASE::check_transformer_status_for_out_of_service_bus(size_t bus)
{
    vector<TRANSFORMER*> transformers = get_transformers_connecting_to_bus(bus);
    TRANSFORMER* transformer;
    size_t ntrans = transformers.size();
    for(size_t i=0; i!=ntrans; ++i)
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

void POWER_SYSTEM_DATABASE::check_hvdc_status_for_out_of_service_bus(size_t bus)
{
    vector<HVDC*> hvdcs = get_hvdcs_connecting_to_bus(bus);
    HVDC* hvdc;
    size_t nhvdc = hvdcs.size();
    for(size_t i=0; i!=nhvdc; ++i)
    {
        hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
            hvdc->set_status(false);
    }
}


void POWER_SYSTEM_DATABASE::calibrate_sources_pmax_and_pmin_with_dynamic_model()
{
    calibrate_generators_pmax_and_pmin_with_turbine_governor_model();
}

void POWER_SYSTEM_DATABASE::calibrate_generators_pmax_and_pmin_with_turbine_governor_model()
{
    vector<GENERATOR*> generators = get_all_generators();
    size_t n = generators.size();
    GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
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

complex<double> POWER_SYSTEM_DATABASE::get_bus_complex_voltage_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        double Vm = busptr->get_voltage_in_pu();
        double angle = busptr->get_angle_in_rad();

        return complex<double>(Vm*cos(angle), Vm*sin(angle));
    }
    else
        return 0.0;

}

complex<double> POWER_SYSTEM_DATABASE::get_bus_complex_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        double Vm = busptr->get_voltage_in_kV();
        double angle = busptr->get_angle_in_rad();

        return complex<double>(Vm*cos(angle), Vm*sin(angle));
    }
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_base_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_base_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_voltage_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_in_pu();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_voltage_in_kV(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_in_kV();
    else
        return 0.0;
}

double POWER_SYSTEM_DATABASE::get_bus_angle_in_deg(size_t bus)
{
    return rad2deg(get_bus_angle_in_rad(bus));
}

double POWER_SYSTEM_DATABASE::get_bus_angle_in_rad(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_angle_in_rad();
    else
        return 0.0;
}


double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_pu(size_t bus)
{
    double fbase = get_system_base_frequency_in_Hz();
    return get_bus_frequency_deviation_in_Hz(bus)/fbase;
}

double POWER_SYSTEM_DATABASE::get_bus_frequency_deviation_in_Hz(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
    {
        BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

        return fmodel->get_frequency_deviation_in_Hz();
    }
    else
        return 0.0;

}

double POWER_SYSTEM_DATABASE::get_bus_frequency_in_Hz(size_t bus)
{
    double fbase = get_system_base_frequency_in_Hz();
    return get_bus_frequency_deviation_in_Hz(bus)+fbase;
}

double POWER_SYSTEM_DATABASE::get_voltage_to_regulate_of_physical_bus_in_pu(size_t bus)
{
    BUS* busptr = get_bus(bus);
    if(busptr!=NULL)
        return busptr->get_voltage_to_regulate_in_pu();
    else
        return 0.0;
    /*

    double v_schedule = 0.0;
    vector<size_t> index = get_index_of_all_generators_connecting_to_bus(bus);

    SOURCE_REGULATING_MODE mode;
    double voltage;
    if(not index.empty())
    {
        size_t n = index.size();
        for(size_t i=0; i!=n; ++i)
        {
            size_t j= index[i];
            if(Generator[j].get_status() == false)
                continue;

            mode = Generator[j].get_regulating_mode();
            if(mode != REGULATING_PV and mode != REGULATING_VA)
                continue;

            voltage = Generator[j].get_voltage_to_regulate_in_pu();
            if(v_schedule == 0.0)
                v_schedule = voltage;
            else
            {
                if(v_schedule != voltage)
                {
                    ostringstream sstream;
                    sstream<<"Warning. Generators at bus %u are regulating different voltages.\n"
                                 "The regulating voltage of the first generator is returned.", bus);
                    show_information_with_leading_time_stamp(sstream);
                    break;
                }
            }
        }
    }
    index = get_index_of_all_pe_sources_connecting_to_bus(bus);
    if(not index.empty())
    {
        size_t n = index.size();
        for(size_t i=0; i!=n; ++i)
        {
            size_t j= index[i];
            if(PE_Source[j].get_status() == false)
                continue;

            mode = PE_Source[j].get_regulating_mode();
            if(mode != REGULATING_PV and mode != REGULATING_VA)
                continue;

            voltage = PE_Source[j].get_voltage_to_regulate_in_pu();
            if(v_schedule == 0.0)
                v_schedule = voltage;
            else
            {
                if(v_schedule != voltage)
                {
                    ostringstream sstream;
                    sstream<<"Warning. PE sources at bus %u are regulating different voltages.\n"
                                 "The regulating voltage of the first source is returned.", bus);
                    show_information_with_leading_time_stamp(sstream);
                    break;
                }
            }
        }
    }
    if(v_schedule == 0.0)
        v_schedule = 1.0;

    return v_schedule;
    */
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_max_at_physical_bus_in_MW(size_t bus)
{
    double total_p_max_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        if(sources[i]->get_regulating_mode()==REGULATING_VA)
            total_p_max_in_MW += sources[i]->get_p_max_in_MW();
    }
    return total_p_max_in_MW;

    /*
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n = generator.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;

        if(generator[i]->get_regulating_mode()==REGULATING_VA)
            total_p_max_in_MW += generator[i]->get_p_max_in_MW();
    }


    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;

        if(pesource[i]->get_regulating_mode()==REGULATING_VA)
            total_p_max_in_MW += pesource[i]->get_p_max_in_MW();
    }
    return total_p_max_in_MW;*/
}

double POWER_SYSTEM_DATABASE::get_regulatable_p_min_at_physical_bus_in_MW(size_t bus)
{
    double total_p_min_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        if(sources[i]->get_regulating_mode()==REGULATING_VA)
            total_p_min_in_MW += sources[i]->get_p_min_in_MW();
    }
    return total_p_min_in_MW;

    /*

    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n = generator.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;

        if(generator[i]->get_regulating_mode()==REGULATING_VA)
            total_p_min_in_MW += generator[i]->get_p_min_in_MW();
    }


    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;

        if(pesource[i]->get_regulating_mode()==REGULATING_VA)
            total_p_min_in_MW += pesource[i]->get_p_min_in_MW();
    }
    return total_p_min_in_MW;*/
}

double POWER_SYSTEM_DATABASE::get_regulatable_q_max_at_physical_bus_in_MVar(size_t bus)
{
    double total_q_max_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    SOURCE_REGULATING_MODE mode;

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        mode = sources[i]->get_regulating_mode();

        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_max_in_MVar += sources[i]->get_q_max_in_MVar();
    }
    return total_q_max_in_MVar;

/*
    double total_q_max_in_MVar = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n = generator.size();
    SOURCE_REGULATING_MODE mode;
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;
        mode = generator[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_max_in_MVar += generator[i]->get_q_max_in_MVar();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;
        mode = pesource[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_max_in_MVar += pesource[i]->get_q_max_in_MVar();
    }
    return total_q_max_in_MVar;*/
}

double POWER_SYSTEM_DATABASE::get_regulatable_q_min_at_physical_bus_in_MVar(size_t bus)
{
    double total_q_min_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    SOURCE_REGULATING_MODE mode;

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        mode = sources[i]->get_regulating_mode();

        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_min_in_MVar += sources[i]->get_q_min_in_MVar();
    }
    return total_q_min_in_MVar;
    /*
    double total_q_min_in_MVar = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n = generator.size();
    SOURCE_REGULATING_MODE mode;
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;
        mode = generator[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_min_in_MVar += generator[i]->get_q_min_in_MVar();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;
        mode = pesource[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_min_in_MVar += pesource[i]->get_q_min_in_MVar();
    }
    return total_q_min_in_MVar;*/
}

double POWER_SYSTEM_DATABASE::get_total_regulating_p_generation_at_physical_bus_in_MW(size_t bus)
{
    double total_p_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    SOURCE_REGULATING_MODE mode;

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        mode = sources[i]->get_regulating_mode();

        if(mode==REGULATING_VA)
            total_p_in_MW += sources[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;

 /*   double total_p_in_MW = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n;
    n = generator.size();
    SOURCE_REGULATING_MODE mode;
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;

        mode = generator[i]->get_regulating_mode();

        if(mode == REGULATING_VA)
            total_p_in_MW += generator[i]->get_p_generation_in_MW();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;

        mode = pesource[i]->get_regulating_mode();

        if(mode == REGULATING_VA)
            total_p_in_MW += pesource[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;*/
}

double POWER_SYSTEM_DATABASE::get_total_regulating_q_generation_at_physical_bus_in_MVar(size_t bus)
{
    double total_q_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    SOURCE_REGULATING_MODE mode;

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        mode = sources[i]->get_regulating_mode();

        if(mode==REGULATING_PV or mode==REGULATING_VA)
            total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;

    /*
    double total_q_in_MVar = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n;
    n = generator.size();
    SOURCE_REGULATING_MODE mode;
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;
        mode = generator[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode == REGULATING_VA)
            total_q_in_MVar += generator[i]->get_q_generation_in_MVar();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;
        mode = generator[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode == REGULATING_VA)
            total_q_in_MVar += pesource[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;*/
}

double POWER_SYSTEM_DATABASE::get_total_p_generation_at_physical_bus_in_MW(size_t bus)
{
    double total_p_in_MW = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        total_p_in_MW += sources[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;
    /*
    double total_p_in_MW = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n;
    n = generator.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;
        else
            total_p_in_MW += generator[i]->get_p_generation_in_MW();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;
        else
            total_p_in_MW += pesource[i]->get_p_generation_in_MW();
    }
    return total_p_in_MW;*/
}

double POWER_SYSTEM_DATABASE::get_total_q_generation_at_physical_bus_in_MVar(size_t bus)
{
    double total_q_in_MVar = 0.0;

    vector<SOURCE*> sources = get_sources_connecting_to_bus(bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        total_q_in_MVar += sources[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;
    /*
    double total_q_in_MVar = 0.0;
    vector<GENERATOR*> generator = get_generators_connecting_to_bus(bus);

    size_t n;
    n = generator.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(generator[i]->get_status() == false)
            continue;
        else
            total_q_in_MVar += generator[i]->get_q_generation_in_MVar();
    }

    vector<PE_SOURCE*> pesource = get_pe_sources_connecting_to_bus(bus);
    n = pesource.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(pesource[i]->get_status() == false)
            continue;
        else
            total_q_in_MVar += pesource[i]->get_q_generation_in_MVar();
    }
    return total_q_in_MVar;*/
}


complex<double> POWER_SYSTEM_DATABASE::get_total_load_power_in_MVA()
{
    size_t n = get_load_count();
    complex<double> S=0.0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Load[i].get_status()==true)
            S += Load[i].get_actual_total_load_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_generation_power_in_MVA()
{
    size_t n = get_generator_count();
    complex<double> S=0.0;
    for(size_t i=0; i!=n; ++i)
    {
        if(Generator[i].get_status()==true)
            S += Generator[i].get_complex_generation_in_MVA();
    }
    n = get_pe_source_count();
    for(size_t i=0; i!=n; ++i)
    {
        if(PE_Source[i].get_status()==true)
            S += PE_Source[i].get_complex_generation_in_MVA();
    }
    return S;
}

complex<double> POWER_SYSTEM_DATABASE::get_total_loss_power_in_MVA()
{
    return get_total_generation_power_in_MVA()-get_total_load_power_in_MVA();
}

