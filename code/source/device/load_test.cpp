#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"

#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/device/load_test.h"

#include "header/model/load_model/IEEL.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;


LOAD_TEST::LOAD_TEST()
{
    TEST_ADD(LOAD_TEST::test_constructor);
    TEST_ADD(LOAD_TEST::test_set_get_load_bus);
    TEST_ADD(LOAD_TEST::test_set_get_identifier);
    TEST_ADD(LOAD_TEST::test_set_get_status);
    TEST_ADD(LOAD_TEST::test_set_get_nominal_constant_power_load);
    TEST_ADD(LOAD_TEST::test_set_get_nominal_constant_current_load);
    TEST_ADD(LOAD_TEST::test_set_get_nominal_constant_impedance_load);
    TEST_ADD(LOAD_TEST::test_set_get_area_number);
    TEST_ADD(LOAD_TEST::test_set_get_zone_number);
    TEST_ADD(LOAD_TEST::test_set_get_owner_number);
    TEST_ADD(LOAD_TEST::test_set_get_flag_interruptable);
    TEST_ADD(LOAD_TEST::test_set_get_load_manually_scale);

    TEST_ADD(LOAD_TEST::test_is_valid);
    TEST_ADD(LOAD_TEST::test_clear);
    TEST_ADD(LOAD_TEST::test_copy_with_operator_equal);
    TEST_ADD(LOAD_TEST::test_is_connected_to_bus);
    TEST_ADD(LOAD_TEST::test_get_device_id);

    TEST_ADD(LOAD_TEST::test_get_nominal_total_load);
    TEST_ADD(LOAD_TEST::test_get_actual_total_load);
    TEST_ADD(LOAD_TEST::test_get_actual_constant_power_load);
    TEST_ADD(LOAD_TEST::test_get_actual_constant_current_load);
    TEST_ADD(LOAD_TEST::test_get_actual_constant_impedance_load);

    TEST_ADD(LOAD_TEST::test_set_get_load_model);
    TEST_ADD(LOAD_TEST::test_set_get_load_frequency_relay_model);
    TEST_ADD(LOAD_TEST::test_set_get_load_voltage_relay_model);
}

void LOAD_TEST::setup()
{
    load.set_toolkit(default_toolkit);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.1);
    psdb.append_bus(bus);
    bus.set_bus_number(2);
    psdb.append_bus(bus);
    bus.set_bus_number(3);
    psdb.append_bus(bus);
}
void LOAD_TEST::tear_down()
{
    load.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void LOAD_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    TEST_ASSERT(load.get_load_bus()==0);
    TEST_ASSERT(load.get_identifier()=="");
    TEST_ASSERT(load.get_status()==false);
    TEST_ASSERT(load.get_nominal_constant_power_load_in_MVA()==0.0);
    TEST_ASSERT(load.get_nominal_constant_current_load_in_MVA()==0.0);
    TEST_ASSERT(load.get_nominal_constant_impedance_load_in_MVA()==0.0);
    TEST_ASSERT(load.get_area_number()==0);
    TEST_ASSERT(load.get_zone_number()==0);
    TEST_ASSERT(load.get_owner_number()==0);
}

void LOAD_TEST::test_set_get_load_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    TEST_ASSERT(load.get_load_bus()==1);
    load.set_load_bus(0);
    TEST_ASSERT(load.get_load_bus()==0);
}

void LOAD_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_identifier("1#");
    TEST_ASSERT(load.get_identifier()=="1#");
}

void LOAD_TEST::test_set_get_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_status(true);
    TEST_ASSERT(load.get_status()==true);
    load.set_status(false);
    TEST_ASSERT(load.get_status()==false);
}

void LOAD_TEST::test_set_get_nominal_constant_power_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    complex<double> s(100, 30);
    load.set_nominal_constant_power_load_in_MVA(s);
    TEST_ASSERT(load.get_nominal_constant_power_load_in_MVA()==s);
}

void LOAD_TEST::test_set_get_nominal_constant_current_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    complex<double> s(100, 30);
    load.set_nominal_constant_current_load_in_MVA(s);
    TEST_ASSERT(load.get_nominal_constant_current_load_in_MVA()==s);
}

void LOAD_TEST::test_set_get_nominal_constant_impedance_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    complex<double> s(100, 30);
    load.set_nominal_constant_impedance_load_in_MVA(s);
    TEST_ASSERT(load.get_nominal_constant_impedance_load_in_MVA()==s);
}

void LOAD_TEST::test_set_get_area_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_area_number(1);
    TEST_ASSERT(load.get_area_number()==1);
}

void LOAD_TEST::test_set_get_zone_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_zone_number(1);
    TEST_ASSERT(load.get_zone_number()==1);
}

void LOAD_TEST::test_set_get_owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_owner_number(1);
    TEST_ASSERT(load.get_owner_number()==1);
}

void LOAD_TEST::test_set_get_flag_interruptable()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    TEST_ASSERT(load.get_flag_interruptable()==false);
    load.set_flag_interruptable(true);
    TEST_ASSERT(load.get_flag_interruptable()==true);
    load.set_flag_interruptable(false);
    TEST_ASSERT(load.get_flag_interruptable()==false);
}

void LOAD_TEST::test_set_get_load_manually_scale()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    TEST_ASSERT(fabs(load.get_load_manually_scale_factor_in_pu()-0.0)<FLOAT_EPSILON);
    load.set_load_manually_scale_factor_in_pu(0.1);
    TEST_ASSERT(fabs(load.get_load_manually_scale_factor_in_pu()-0.1)<FLOAT_EPSILON);
    load.set_load_manually_scale_factor_in_pu(0.2);
    TEST_ASSERT(fabs(load.get_load_manually_scale_factor_in_pu()-0.2)<FLOAT_EPSILON);
    load.set_load_manually_scale_factor_in_pu(-0.2);
    TEST_ASSERT(fabs(load.get_load_manually_scale_factor_in_pu()+0.2)<FLOAT_EPSILON);
}

void LOAD_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    TEST_ASSERT(load.is_valid()==false);

    load.set_load_bus(1);
    TEST_ASSERT(load.is_valid()==true);
}

void LOAD_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_identifier("1#");
    load.set_status(true);

    load.clear();

    test_constructor();
}

void LOAD_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_identifier("1#");
    load.set_status(false);
    load.set_nominal_constant_power_load_in_MVA(complex<double>(100.0, 50.0));

    LOAD load2 = load;

    TEST_ASSERT(load2.get_load_bus()==1);
    TEST_ASSERT(load2.get_identifier()=="1#");
    TEST_ASSERT(load2.get_status()==false);
    TEST_ASSERT(load2.get_nominal_constant_power_load_in_MVA()==complex<double>(100.0, 50.0));
}

void LOAD_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    TEST_ASSERT(load.is_connected_to_bus(1)==true);
    TEST_ASSERT(load.is_connected_to_bus(2)==false);
}
void LOAD_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_identifier("1#");

    DEVICE_ID did = load.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier("1#");

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}

void LOAD_TEST::test_get_nominal_total_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_status(true);
    complex<double> s_P(100.0,50.0), s_I(100.0, 50.0), s_Z(100.0, 50.0);
    load.set_nominal_constant_power_load_in_MVA(s_P);
    load.set_nominal_constant_current_load_in_MVA(s_I);
    load.set_nominal_constant_impedance_load_in_MVA(s_Z);

    complex<double> s_total = s_P + s_I+s_Z;
    TEST_ASSERT(abs(load.get_nominal_total_load_in_MVA()-s_total)<FLOAT_EPSILON);
}

void LOAD_TEST::test_get_actual_total_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    LOAD::set_voltage_threshold_of_constant_power_load_in_pu(0.7);

    load.set_load_bus(1);
    load.set_status(true);
    complex<double> s_P(100.0,50.0), s_I(100.0, 50.0), s_Z(100.0, 50.0);
    load.set_nominal_constant_power_load_in_MVA(s_P);
    load.set_nominal_constant_current_load_in_MVA(s_I);
    load.set_nominal_constant_impedance_load_in_MVA(s_Z);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(load.get_load_bus());
    bus->set_voltage_in_pu(0.5);

    complex<double> s_total = s_P/0.7*0.5 + s_I*0.5+s_Z*0.5*0.5;
    TEST_ASSERT(abs(load.get_actual_total_load_in_MVA()-s_total)<FLOAT_EPSILON);
}

void LOAD_TEST::test_get_actual_constant_power_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_status(true);
    complex<double> s_P(100.0,50.0);
    load.set_nominal_constant_power_load_in_MVA(s_P);

    LOAD::set_voltage_threshold_of_constant_power_load_in_pu(0.7);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(load.get_load_bus());
    bus->set_voltage_in_pu(0.95);

    TEST_ASSERT(abs(load.get_actual_constant_power_load_in_MVA()-s_P)<FLOAT_EPSILON);

    bus->set_voltage_in_pu(0.7);
    TEST_ASSERT(abs(load.get_actual_constant_power_load_in_MVA()-s_P)<FLOAT_EPSILON);

    bus->set_voltage_in_pu(0.5);
    complex<double> s = s_P/0.7*0.5;
    TEST_ASSERT(abs(load.get_actual_constant_power_load_in_MVA()-s)<FLOAT_EPSILON);
}
void LOAD_TEST::test_get_actual_constant_current_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_status(true);
    complex<double> s_I(100.0, 50.0);
    load.set_nominal_constant_current_load_in_MVA(s_I);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(load.get_load_bus());

    bus->set_voltage_in_pu(0.95);

    complex<double> s = s_I*0.95;
    TEST_ASSERT(abs(load.get_actual_constant_current_load_in_MVA()-s)<FLOAT_EPSILON);
}

void LOAD_TEST::test_get_actual_constant_impedance_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_status(true);
    complex<double> s_Z(100.0, 50.0);
    load.set_nominal_constant_impedance_load_in_MVA(s_Z);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(load.get_load_bus());

    bus->set_voltage_in_pu(0.95);

    complex<double> s = s_Z*0.95*0.95;
    TEST_ASSERT(abs(load.get_actual_constant_impedance_load_in_MVA()-s)<FLOAT_EPSILON);
}


void LOAD_TEST::test_set_get_load_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_TEST");

    load.set_load_bus(1);
    load.set_identifier("1#");
    load.set_status(true);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.append_load(load);

    DEVICE_ID did = load.get_device_id();

    LOAD* ld = psdb.get_load(did);

    IEEL model;

    model.set_subsystem_type(AREA_SUBSYSTEM_TYPE);
    model.set_P_alpha_1(0.1);
    model.set_P_alpha_2(0.5);
    model.set_P_alpha_3(0.4);
    model.set_Q_alpha_1(0.6);


    ld->set_model(&model);

    LOAD_MODEL* modelptr = ld->get_load_model();
    TEST_ASSERT(modelptr!=NULL);

    IEEL* smodelptr = (IEEL*) modelptr;

    TEST_ASSERT(fabs(smodelptr->get_P_alpha_1()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_P_alpha_2()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_P_alpha_3()-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_Q_alpha_1()-0.6)<FLOAT_EPSILON);
}

void LOAD_TEST::test_set_get_load_frequency_relay_model()
{
    ;
}

void LOAD_TEST::test_set_get_load_voltage_relay_model()
{
    ;
}
