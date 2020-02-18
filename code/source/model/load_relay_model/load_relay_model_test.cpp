#include "header/basic/test_macro.h"
#include "header/model/load_relay_model/load_relay_model_test.h"
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
LOAD_RELAY_MODEL_TEST::LOAD_RELAY_MODEL_TEST()
{
    ;
}



void LOAD_RELAY_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);

    psdb.append_bus(bus);

    LOAD load(default_toolkit);
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

void LOAD_RELAY_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    DYNAMICS_SIMULATOR& simulator = default_toolkit.get_dynamic_simulator();
    simulator.clear();

    show_test_end_information();
}

LOAD* LOAD_RELAY_MODEL_TEST::get_load()
{
    return loadptr;
}

#endif
