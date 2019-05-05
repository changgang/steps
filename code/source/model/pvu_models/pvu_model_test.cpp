#include "header/model/pvu_models/pvu_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

PVU_MODEL_TEST::PVU_MODEL_TEST()
{
    ;
}



void PVU_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(0.69);
    bus.set_base_frequency_in_Hz(50.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_deg(30.0);

    psdb.append_bus(bus);

    PV_UNIT pv_unit;
    pv_unit.set_unit_bus(1);
    pv_unit.set_identifier("#1");
    pv_unit.set_status(true);
    pv_unit.set_mbase_in_MVA(30.0);
    pv_unit.set_source_impedance_in_pu(complex<double>(0.0, 0.1));
    pv_unit.set_p_generation_in_MW(70.0);
    pv_unit.set_q_generation_in_MVar(30.0);
    pv_unit.set_number_of_lumped_pv_units(50);
    pv_unit.set_rated_power_per_pv_unit_in_MW(2.0);

    psdb.append_pv_unit(pv_unit);
}

void PVU_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}

PV_UNIT* PVU_MODEL_TEST::get_test_pv_unit()
{
    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    return psdb.get_pv_unit(did);
}


PV_CONVERTER_MODEL* PVU_MODEL_TEST::get_test_pv_converter_model()
{
    PV_UNIT* pvptr = get_test_pv_unit();
    if(pvptr==NULL)
        return NULL;
    else
        return pvptr->get_pv_converter_model();
}

PV_PANEL_MODEL* PVU_MODEL_TEST::get_test_pv_panel_model()
{
    PV_UNIT* pvptr = get_test_pv_unit();
    if(pvptr==NULL)
        return NULL;
    else
        return pvptr->get_pv_panel_model();
}

PV_ELECTRICAL_MODEL* PVU_MODEL_TEST::get_test_pv_electrical_model()
{
    PV_UNIT* pvptr = get_test_pv_unit();
    if(pvptr==NULL)
        return NULL;
    else
        return pvptr->get_pv_electrical_model();
}

PV_IRRADIANCE_MODEL* PVU_MODEL_TEST::get_test_pv_irradiance_model()
{
    PV_UNIT* pvptr = get_test_pv_unit();
    if(pvptr==NULL)
        return NULL;
    else
        return pvptr->get_pv_irradiance_model();
}

