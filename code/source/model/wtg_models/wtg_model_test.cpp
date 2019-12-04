#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wtg_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

WTG_MODEL_TEST::WTG_MODEL_TEST()
{
    ;
}

void WTG_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(0.69);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(30.0);

    psdb.append_bus(bus);

    WT_GENERATOR wt_generator;
    wt_generator.set_toolkit(default_toolkit);
    wt_generator.set_generator_bus(1);
    wt_generator.set_identifier("#1");
    wt_generator.set_status(true);
    wt_generator.set_mbase_in_MVA(30.0);
    wt_generator.set_source_impedance_in_pu(complex<double>(0.0, 0.1));
    wt_generator.set_p_generation_in_MW(70.0);
    wt_generator.set_q_generation_in_MVar(30.0);
    wt_generator.set_number_of_lumped_wt_generators(50);
    wt_generator.set_rated_power_per_wt_generator_in_MW(2.0);

    psdb.append_wt_generator(wt_generator);
}

void WTG_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}

WT_GENERATOR* WTG_MODEL_TEST::get_test_wt_generator()
{
    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    return psdb.get_wt_generator(did);
}


WT_GENERATOR_MODEL* WTG_MODEL_TEST::get_test_wt_generator_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_generator_model();
}

WT_AERODYNAMIC_MODEL* WTG_MODEL_TEST::get_test_wt_aerodynamic_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_aerodynamic_model();
}

WT_ELECTRICAL_MODEL* WTG_MODEL_TEST::get_test_wt_electrical_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_electrical_model();
}

WT_TURBINE_MODEL* WTG_MODEL_TEST::get_test_wt_turbine_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_turbine_model();
}

WT_PITCH_MODEL* WTG_MODEL_TEST::get_test_wt_pitch_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_pitch_model();
}

WIND_SPEED_MODEL* WTG_MODEL_TEST::get_test_wind_speed_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wind_speed_model();
}

WT_RELAY_MODEL* WTG_MODEL_TEST::get_test_wt_relay_model()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    if(genptr==NULL)
        return NULL;
    else
        return genptr->get_wt_relay_model();
}

#endif
