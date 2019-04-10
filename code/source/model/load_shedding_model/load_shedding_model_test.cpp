#include "header/model/load_shedding_model/load_shedding_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
LOAD_SHEDDING_MODEL_TEST::LOAD_SHEDDING_MODEL_TEST()
{
    ;
}



void LOAD_SHEDDING_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_rad(0.0);

    psdb.append_bus(bus);

    LOAD load;
    load.set_load_bus(1);
    load.set_identifier("#1");
    load.set_status(true);
    load.set_nominal_constant_power_load_in_MVA(complex<double>(100.0, 50.0));
    load.set_nominal_constant_current_load_in_MVA(complex<double>(100.0, 50.0));
    load.set_nominal_constant_impedance_load_in_MVA(complex<double>(100.0, 50.0));

    psdb.append_load(load);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    loadptr = psdb.get_load(did);
}

void LOAD_SHEDDING_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    DYNAMICS_SIMULATOR& simulator = get_default_dynamic_simulator();
    simulator.clear();

    show_test_end_information();
}

LOAD* LOAD_SHEDDING_MODEL_TEST::get_load()
{
    return loadptr;
}
