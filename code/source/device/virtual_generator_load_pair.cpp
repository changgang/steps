#include "header/device/virtual_generator_load_pair.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/device/generator.h"
#include "header/device/load.h"
#include "header/model/source_model/sync_generator_model/gencls.h"

#include <cstdio>
#include <iostream>

using namespace std;

VIRTUAL_GENERATOR_LOAD_PAIR::VIRTUAL_GENERATOR_LOAD_PAIR()
{
    clear();
}

VIRTUAL_GENERATOR_LOAD_PAIR::~VIRTUAL_GENERATOR_LOAD_PAIR()
{
    ;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_generator_power_system_name(string name)
{
    ostringstream sstream;
    bool found = false;
    size_t nps = MAX_DATABASE_COUNT;
    string power_system_name = "";
    for(size_t i=0; i!=nps; ++i)
    {
        power_system_name = STEPS::power_system_db[i].get_system_name();
        if(power_system_name == name)
        {
            found = true;
            generator_power_system_db = STEPS::power_system_db+i;
            break;
        }
    }
    if(not found)
    {
        sstream<<"Warning. Power system with name '"<<name<<"' cannot be found in STEPS::power_system_db."<<endl
          <<"No generator power system databse will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
    return;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_load_power_system_name(string name)
{
    ostringstream sstream;
    bool found = false;
    size_t nps = MAX_DATABASE_COUNT;
    string power_system_name = "";
    for(size_t i=0; i!=nps; ++i)
    {
        power_system_name = STEPS::power_system_db[i].get_system_name();
        if(power_system_name == name)
        {
            found = true;
            load_power_system_db = STEPS::power_system_db+i;
            break;
        }
    }
    if(not found)
    {
        sstream<<"Warning. Power system with name '"<<name<<"' cannot be found in STEPS::power_system_db."<<endl
          <<"No load power system databse will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
    return;
}


string VIRTUAL_GENERATOR_LOAD_PAIR::get_generator_power_system_name() const
{
    if(generator_power_system_db != NULL)
        return generator_power_system_db->get_system_name();
    else
        return "";
}

string VIRTUAL_GENERATOR_LOAD_PAIR::get_load_power_system_name() const
{
    if(load_power_system_db != NULL)
        return load_power_system_db->get_system_name();
    else
        return "";
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_generator_power_system_index(size_t index)
{
    ostringstream sstream;
    size_t nps = MAX_DATABASE_COUNT;
    if(index<nps)
        generator_power_system_db = STEPS::power_system_db+index;
    else
    {
        sstream<<"Warning. Power system with index "<<index<<" exceeds the size of STEPS::power_system_db."<<endl
          <<"No generator power system databse will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
    return;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_load_power_system_index(size_t index)
{
    ostringstream sstream;
    size_t nps = MAX_DATABASE_COUNT;
    if(index<nps)
        load_power_system_db = STEPS::power_system_db+index;
    else
    {
        sstream<<"Warning. Power system with index "<<index<<" exceeds the size of STEPS::power_system_db."<<endl
          <<"No load power system databse will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
    return;
}

size_t VIRTUAL_GENERATOR_LOAD_PAIR::get_generator_power_system_index() const
{
    size_t nps = MAX_DATABASE_COUNT;
    for(size_t i=0; i!=nps; ++i)
    {
        if(generator_power_system_db == (STEPS::power_system_db+i))
            return i;
    }
    return INDEX_NOT_EXIST;
}

size_t VIRTUAL_GENERATOR_LOAD_PAIR::get_load_power_system_index() const
{
    size_t nps = MAX_DATABASE_COUNT;
    for(size_t i=0; i!=nps; ++i)
    {
        if(load_power_system_db == (STEPS::power_system_db+i))
            return i;
    }
    return INDEX_NOT_EXIST;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_virtual_generator(DEVICE_ID generator)
{
    ostringstream sstream;
    POWER_SYSTEM_DATABASE* psdb = generator_power_system_db;
    if(psdb == NULL)
    {
        sstream<<"Warning. No generator power system is set when setting up virtual generator."<<endl
          <<"No virtual generator will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(generator.get_device_type()!="GENERATOR")
    {
        sstream<<"Warning. The input virtual 'generator' is not of GENERATOR type. It is a "<<generator.get_device_type()<<"."<<endl
          <<"No virtual generator will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    bool exist = psdb->is_generator_exist(generator);
    if(exist)
        generator_id = generator;
    else
    {
        sstream<<"Warning. "<<generator.get_device_name()<<" doesn't exit in power system '"<<psdb->get_system_name()<<"'."<<endl
          <<"No virtual generator will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
}

DEVICE_ID VIRTUAL_GENERATOR_LOAD_PAIR::get_virtual_generator() const
{
    return generator_id;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_virtual_load(DEVICE_ID load)
{
    ostringstream sstream;
    POWER_SYSTEM_DATABASE* psdb = load_power_system_db;
    if(psdb == NULL)
    {
        sstream<<"Warning. No load power system is set when setting up virtual load."<<endl
          <<"No virtual load will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(load.get_device_type()!="LOAD")
    {
        sstream<<"Warning. The input virtual 'load' is not of LOAD type. It is a "<<load.get_device_type()<<"."<<endl
          <<"No virtual load will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    bool exist = psdb->is_load_exist(load);
    if(exist)
        load_id = load;
    else
    {
        sstream<<"Warning. "<<load.get_device_name()<<" doesn't exit in power system '"<<psdb->get_system_name()<<"'."<<endl
          <<"No virtual load will be set for virtual generator load pair.";
        show_information_with_leading_time_stamp(sstream);
    }
}

DEVICE_ID VIRTUAL_GENERATOR_LOAD_PAIR::get_virtual_load() const
{
    return load_id;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::update()
{
    ostringstream sstream;
    if(not is_valid())
    {
        sstream<<"Warning. Virtual generator load pair is invalid."<<endl
          <<"Neither virtual generator voltage nor load power will be updated for powerflow.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = generator_power_system_db;
    GENERATOR* generator = psdb->get_generator(generator_id);
    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    complex<double> S(P,Q);

    psdb = load_power_system_db;
    LOAD* load = psdb->get_load(load_id);
    size_t bus = load->get_load_bus();
    double voltage = psdb->get_bus_voltage_in_pu(bus);
    double angle = psdb->get_bus_angle_in_rad(bus);

    load->set_nominal_constant_power_load_in_MVA(S);

    psdb = generator_power_system_db;
    bus = generator->get_generator_bus();
    BUS* bus_ptr = psdb->get_bus(bus);
    bus_ptr->set_voltage_in_pu(voltage);
    bus_ptr->set_angle_in_rad(angle);
}

bool VIRTUAL_GENERATOR_LOAD_PAIR::is_valid() const
{
    if(generator_power_system_db!=NULL and load_power_system_db!=NULL and generator_id.is_valid() and load_id.is_valid())
        return true;
    else
        return false;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::check()
{
    ;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::clear()
{
    generator_power_system_db = NULL;
    load_power_system_db = NULL;

    generator_id.clear();
    load_id.clear();
}

bool VIRTUAL_GENERATOR_LOAD_PAIR::is_connected_to_bus(size_t bus) const
{
    bool virtual_generator_connecting_to_bus = false;
    bool virtual_load_connecting_to_bus = false;

    if(generator_id.is_valid())
    {
        TERMINAL terminal = generator_id.get_device_terminal();
        vector<size_t> buses = terminal.get_buses();
        if(buses[0]==bus)
            virtual_generator_connecting_to_bus = true;
    }

    if(load_id.is_valid())
    {
        TERMINAL terminal = load_id.get_device_terminal();
        vector<size_t> buses = terminal.get_buses();
        if(buses[0]==bus)
            virtual_load_connecting_to_bus = true;
    }

    if(virtual_generator_connecting_to_bus or virtual_load_connecting_to_bus)
        return true;
    else
        return false;
}


bool VIRTUAL_GENERATOR_LOAD_PAIR::is_in_area(size_t area) const
{
    bool in_area = false;
    if(generator_power_system_db!=NULL)
    {
        DEVICE_ID did = get_virtual_generator();
        GENERATOR* gen = generator_power_system_db->get_generator(did);
        if(gen!=NULL)
            in_area = in_area or gen->is_in_area(area);
    }
    if(load_power_system_db!=NULL)
    {
        DEVICE_ID did = get_virtual_load();
        LOAD* load = load_power_system_db->get_load(did);
        if(load!=NULL)
            in_area = in_area or load->is_in_area(area);
    }
    return in_area;
}

bool VIRTUAL_GENERATOR_LOAD_PAIR::is_in_zone(size_t zone) const
{
    bool in_zone = false;
    if(generator_power_system_db!=NULL)
    {
        DEVICE_ID did = get_virtual_generator();
        GENERATOR* gen = generator_power_system_db->get_generator(did);
        if(gen!=NULL)
            in_zone = in_zone or gen->is_in_zone(zone);
    }
    if(load_power_system_db!=NULL)
    {
        DEVICE_ID did = get_virtual_load();
        LOAD* load = load_power_system_db->get_load(did);
        if(load!=NULL)
            in_zone = in_zone or load->is_in_zone(zone);
    }
    return in_zone;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::report() const
{
    ;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::save() const
{
    ;
}


DEVICE_ID VIRTUAL_GENERATOR_LOAD_PAIR::get_device_id() const
{
    DEVICE_ID did;
    return did; // it is an invalid device_id
}

/*string VIRTUAL_GENERATOR_LOAD_PAIR::get_device_name() const
{
    return "VIRTUAL GENERATOR LOAD PAIR";
}*/

/*
void VIRTUAL_GENERATOR_LOAD_PAIR::initialize()
{
    ostringstream sstream;
    if(not is_valid())
    {
        sstream<<"Warning. Virtual generator load pair is invalid. Pair is not initialized.");
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = generator_power_system_db;
    GENERATOR* generator = db->get_generator(generator_id);

    generator->clear_exciter_model();
    generator->clear_stabilizer_model();
    generator->clear_turbine_governor_model();

    SYNC_GENERATOR_MODEL* gen_model = generator->get_generator_model();
    if(gen_model==NULL)
    {
        sstream<<"Warning. No dynamic model of virtual generator %s of power system %s is set. It will be set automatically",
                generator_id.get_device_name().c_str(), generator_power_system_db->get_system_name().c_str());
        show_information_with_leading_time_stamp(sstream);

        GENCLS* model = new GENCLS(db, get_virtual_generator());
        model->set_Rs(0.0);
        model->set_Xdp(1e-6);
        model->set_Tj_in_s(1e6);
        model->set_D(0.0);
        generator->set_generator_model(model);
    }
    else
    {
        string model_name = gen_model->get_model_name();
        if(model_name!="GENCLS")
        {
            sstream<<"Error. The dynamic model of virtual generator %s of power system %s is not 'GENCLS'. It is '%s'.",
                    generator_id.get_device_name().c_str(), generator_power_system_db->get_system_name().c_str(),
                    model_name.c_str());
            show_information_with_leading_time_stamp(sstream);
            return;
        }
    }

    db = load_power_system_db;
    LOAD* load = db->get_load(load_id);

    load->clear_all_models();
}

void VIRTUAL_GENERATOR_LOAD_PAIR::run(DYNAMIC_MODE mode)
{
    ostringstream sstream;
    if(not is_valid())
    {
        sstream<<"Warning. Virtual generator load pair is invalid. Neither virtual generator voltage nor load power will be updated for dynamics.");
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = generator_power_system_db;
    GENERATOR* generator = db->get_generator(generator_id);

    GENCLS* gen_model = (GENCLS*) generator->get_generator_model();
    complex<double> S = gen_model->get_terminal_complex_power_in_pu_based_on_mbase();
    double mbase = generator->get_mbase_in_MVA();
    S = S*mbase;

    db = load_power_system_db;
    LOAD* load = db->get_load(load_id);
    size_t bus = load->get_load_bus();
    double voltage = db->get_bus_voltage_in_pu(bus);
    double angle = db->get_bus_angle_in_deg(bus);

    load->set_nominal_constant_power_load_in_MVA(S);

    gen_model->set_excitation_voltage_in_pu(voltage);
    gen_model->set_rotor_angle_in_deg(angle);
}
*/

void VIRTUAL_GENERATOR_LOAD_PAIR::set_model(const MODEL* model)
{
    ;
}

void VIRTUAL_GENERATOR_LOAD_PAIR::set_power_system_database(POWER_SYSTEM_DATABASE* psdb)
{
    ;// this is a private method of the class. it is for disabling the set method of BASE class.
}
POWER_SYSTEM_DATABASE* VIRTUAL_GENERATOR_LOAD_PAIR::get_power_system_database() const
{
    return NULL;// this is a private method of the class. it is for disabling the get method of BASE class.
}



