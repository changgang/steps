#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/bus_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;


BUS_TEST::BUS_TEST()
{
    TEST_ADD(BUS_TEST::test_constructor);
    TEST_ADD(BUS_TEST::test_set_get_bus_number);
    TEST_ADD(BUS_TEST::test_set_get_bus_name);
    TEST_ADD(BUS_TEST::test_set_get_base_voltage);
    TEST_ADD(BUS_TEST::test_set_get_bus_type);
    TEST_ADD(BUS_TEST::test_set_get_area_number);
    TEST_ADD(BUS_TEST::test_set_get_zone_number);
    TEST_ADD(BUS_TEST::test_set_get_owner_number);
    TEST_ADD(BUS_TEST::test_set_get_voltage_with_nonzero_base_voltage);
    TEST_ADD(BUS_TEST::test_set_get_voltage_with_zero_base_voltage);
    TEST_ADD(BUS_TEST::test_set_get_angle);
    TEST_ADD(BUS_TEST::test_set_get_normal_voltage_upper_limit);
    TEST_ADD(BUS_TEST::test_set_get_normal_voltage_lower_limit);
    TEST_ADD(BUS_TEST::test_set_get_emergency_voltage_upper_limit);
    TEST_ADD(BUS_TEST::test_set_get_emergency_voltage_lower_limit);
    TEST_ADD(BUS_TEST::test_set_get_voltage_upper_limit);
    TEST_ADD(BUS_TEST::test_set_get_voltage_lower_limit);
    TEST_ADD(BUS_TEST::test_set_get_base_frequency);
    TEST_ADD(BUS_TEST::test_set_get_voltage_to_regulate);
    TEST_ADD(BUS_TEST::test_is_valid);
    TEST_ADD(BUS_TEST::test_clear);
    TEST_ADD(BUS_TEST::test_copy_with_operator_equal);
    TEST_ADD(BUS_TEST::test_is_connected_to_bus);

    TEST_ADD(BUS_TEST::test_get_device_id);

    TEST_ADD(BUS_TEST::test_set_get_fault);
    TEST_ADD(BUS_TEST::test_clear_fault);
    TEST_ADD(BUS_TEST::test_is_faulted);
    TEST_ADD(BUS_TEST::test_set_get_frequency_deviation);
}

void BUS_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_allowed_max_bus_number(100);
}

void BUS_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    bus.clear();

    show_test_end_information();
}

void BUS_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(bus.get_bus_number() == 0);
    TEST_ASSERT(bus.get_bus_name()=="");
    TEST_ASSERT(bus.get_base_voltage_in_kV()==0.0);
    TEST_ASSERT(bus.get_bus_type()==OUT_OF_SERVICE);
    TEST_ASSERT(bus.get_area_number()== 0);
    TEST_ASSERT(bus.get_zone_number()== 0);
    TEST_ASSERT(bus.get_owner_number()== 0);
    TEST_ASSERT(bus.get_voltage_in_pu()==1.0);
    TEST_ASSERT(bus.get_angle_in_rad()==0.0);
    TEST_ASSERT(bus.get_angle_in_deg()==0.0);
    TEST_ASSERT(bus.get_base_frequency_in_Hz()==50.0);
    TEST_ASSERT(bus.get_voltage_to_regulate_in_pu()==0.0);

    TEST_ASSERT(bus.is_faulted()==false);
}

void BUS_TEST::test_set_get_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);
    TEST_ASSERT(bus.get_bus_number()==1);
    bus.set_bus_number(0);
    TEST_ASSERT(bus.get_bus_number()==0);
    bus.set_bus_number(1000);
    TEST_ASSERT(bus.get_bus_number()==1000);
    bus.set_bus_number(2);
    TEST_ASSERT(bus.get_bus_number()==2);
}

void BUS_TEST::test_set_get_bus_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_name("bus 1");
    TEST_ASSERT(bus.get_bus_name()=="bus 1");
}

void BUS_TEST::test_set_get_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_base_voltage_in_kV(110.0);
    TEST_ASSERT(bus.get_base_voltage_in_kV()==110.0);
    bus.set_base_voltage_in_kV(-220.0);
    TEST_ASSERT(bus.get_base_voltage_in_kV()==0.0);
    bus.set_base_voltage_in_kV(0.0);
    TEST_ASSERT(bus.get_base_voltage_in_kV()==0.0);
}

void BUS_TEST::test_set_get_bus_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_type(PQ_TYPE);
    TEST_ASSERT(bus.get_bus_type()==PQ_TYPE);
    bus.set_bus_type(PV_TYPE);
    TEST_ASSERT(bus.get_bus_type()==PV_TYPE);
    bus.set_bus_type(SLACK_TYPE);
    TEST_ASSERT(bus.get_bus_type()==SLACK_TYPE);
}

void BUS_TEST::test_set_get_area_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_area_number(1);
    TEST_ASSERT(bus.get_area_number()==1);
    bus.set_area_number(0);
    TEST_ASSERT(bus.get_area_number()==0);
    bus.set_area_number(2);
    TEST_ASSERT(bus.get_area_number()==2);
}

void BUS_TEST::test_set_get_zone_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_zone_number(1);
    TEST_ASSERT(bus.get_zone_number()==1);
    bus.set_zone_number(0);
    TEST_ASSERT(bus.get_zone_number()==0);
    bus.set_zone_number(2);
    TEST_ASSERT(bus.get_zone_number()==2);
}

void BUS_TEST::test_set_get_owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_owner_number(1);
    TEST_ASSERT(bus.get_owner_number()==1);
    bus.set_owner_number(0);
    TEST_ASSERT(bus.get_owner_number()==0);
    bus.set_owner_number(2);
    TEST_ASSERT(bus.get_owner_number()==2);
}

void BUS_TEST::test_set_get_voltage_with_nonzero_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_base_voltage_in_kV(100.00);
    TEST_ASSERT(bus.get_base_voltage_in_kV()==100.0);
    bus.set_voltage_in_pu(1.1);
    TEST_ASSERT(bus.get_voltage_in_pu()==1.1);
    TEST_ASSERT(fabs(bus.get_voltage_in_kV()-110.0)<FLOAT_EPSILON);
    bus.set_voltage_in_kV(220.0);
    TEST_ASSERT(bus.get_voltage_in_pu()==2.2);
    TEST_ASSERT(fabs(bus.get_voltage_in_kV()-220.0)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_voltage_with_zero_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_base_voltage_in_kV(0.0);
    bus.set_voltage_in_pu(1.1);
    TEST_ASSERT(bus.get_voltage_in_pu()==1.1);
    TEST_ASSERT(bus.get_voltage_in_kV()==0.0);
    bus.set_voltage_in_kV(110.0);
    TEST_ASSERT(bus.get_voltage_in_pu()==0.0);
    TEST_ASSERT(bus.get_voltage_in_kV()==0.0);
}

void BUS_TEST::test_set_get_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_angle_in_rad(0.5);
    TEST_ASSERT(bus.get_angle_in_rad()==0.5);
    TEST_ASSERT(fabs(bus.get_angle_in_deg()-0.5/PI*180.0)<FLOAT_EPSILON);
    bus.set_angle_in_deg(90.0);
    TEST_ASSERT(bus.get_angle_in_rad()==0.5*PI);
    TEST_ASSERT(bus.get_angle_in_deg()==90.0);
}


void BUS_TEST::test_set_get_normal_voltage_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_normal_voltage_upper_limit_in_pu()-1.1)<FLOAT_EPSILON);
    bus.set_normal_voltage_upper_limit_in_pu(1.2);
    TEST_ASSERT(fabs(bus.get_normal_voltage_upper_limit_in_pu()-1.2)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_normal_voltage_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_normal_voltage_lower_limit_in_pu()-0.9)<FLOAT_EPSILON);
    bus.set_normal_voltage_lower_limit_in_pu(0.8);
    TEST_ASSERT(fabs(bus.get_normal_voltage_lower_limit_in_pu()-0.8)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_emergency_voltage_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_emergency_voltage_upper_limit_in_pu()-1.1)<FLOAT_EPSILON);
    bus.set_emergency_voltage_upper_limit_in_pu(1.3);
    TEST_ASSERT(fabs(bus.get_emergency_voltage_upper_limit_in_pu()-1.3)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_emergency_voltage_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_emergency_voltage_lower_limit_in_pu()-0.9)<FLOAT_EPSILON);
    bus.set_emergency_voltage_lower_limit_in_pu(0.7);
    TEST_ASSERT(fabs(bus.get_emergency_voltage_lower_limit_in_pu()-0.7)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_voltage_upper_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_voltage_upper_limit_in_pu()-1.1)<FLOAT_EPSILON);
    bus.set_voltage_upper_limit_in_pu(1.25);
    TEST_ASSERT(fabs(bus.get_voltage_upper_limit_in_pu()-1.25)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_voltage_lower_limit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_voltage_lower_limit_in_pu()-0.9)<FLOAT_EPSILON);
    bus.set_voltage_lower_limit_in_pu(0.75);
    TEST_ASSERT(fabs(bus.get_voltage_lower_limit_in_pu()-0.75)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_base_frequency()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(fabs(bus.get_base_frequency_in_Hz()-50.0)<FLOAT_EPSILON);
    bus.set_base_frequency_in_Hz(60.0);
    TEST_ASSERT(fabs(bus.get_base_frequency_in_Hz()-60.0)<FLOAT_EPSILON);
}

void BUS_TEST::test_set_get_voltage_to_regulate()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_voltage_to_regulate_in_pu(1.0);
    TEST_ASSERT(bus.get_voltage_to_regulate_in_pu()==1.0);
    bus.set_voltage_to_regulate_in_pu(1.0);
    TEST_ASSERT(bus.get_voltage_to_regulate_in_pu()==1.0);
    bus.set_voltage_to_regulate_in_pu(1.1);
    TEST_ASSERT(bus.get_voltage_to_regulate_in_pu()==1.0);
}

void BUS_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);

    DEVICE_ID did = bus.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}


void BUS_TEST::test_set_get_fault()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);

    FAULT fault, fault2;
    complex<double> y;

    fault.set_fault_type(THREE_PHASES_FAULT);
    y=complex<double>(0.0,-2e10);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    fault2 = bus.get_fault();
    TEST_ASSERT(fault2.get_fault_type()==THREE_PHASES_FAULT);
    TEST_ASSERT(fault2.get_fault_shunt_in_pu()==y);

    fault.set_fault_type(DOUBLE_PHASES_GROUNDED_FAULT);
    y = complex<double>(0.0,-3e10);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    fault2 = bus.get_fault();
    TEST_ASSERT(fault2.get_fault_type()==DOUBLE_PHASES_GROUNDED_FAULT);
    TEST_ASSERT(fault2.get_fault_shunt_in_pu()==y);


    fault.set_fault_type(THREE_PHASES_FAULT);
    y = complex<double>(0.0,-3e10);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    fault2 = bus.get_fault();
    TEST_ASSERT(fault2.get_fault_type()==THREE_PHASES_FAULT);
    TEST_ASSERT(fault2.get_fault_shunt_in_pu()==y);
}

void BUS_TEST::test_clear_fault()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);

    FAULT fault, fault2;
    complex<double> y;

    fault.set_fault_type(THREE_PHASES_FAULT);
    y=complex<double>(0.0,-2e10);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    bus.clear_fault();
    fault2 = bus.get_fault();
    TEST_ASSERT(bus.is_faulted()==false);
    TEST_ASSERT(fault2.get_fault_type()==THREE_PHASES_FAULT);
    TEST_ASSERT(fault2.get_fault_shunt_in_pu()==0.0);
}


void BUS_TEST::test_is_faulted()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(1);

    FAULT fault;
    complex<double> y;

    fault.set_fault_type(SINGLE_PHASE_GROUNDED_FAULT);
    y=complex<double>(0.0,-2e10);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    TEST_ASSERT(bus.is_faulted()==true);

    fault.set_fault_type(DOUBLE_PHASES_FAULT);
    bus.set_fault(fault);
    TEST_ASSERT(bus.is_faulted()==true);

    fault.set_fault_type(DOUBLE_PHASES_GROUNDED_FAULT);
    bus.set_fault(fault);
    TEST_ASSERT(bus.is_faulted()==true);

    fault.set_fault_type(THREE_PHASES_FAULT);
    bus.set_fault(fault);
    TEST_ASSERT(bus.is_faulted()==true);

    y = complex<double>(0.0,0.0);
    fault.set_fault_shunt_in_pu(y);
    bus.set_fault(fault);
    TEST_ASSERT(bus.is_faulted()==true);
}

void BUS_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    TEST_ASSERT(bus.is_valid()==false);

    bus.set_bus_number(1);
    TEST_ASSERT(bus.is_valid()==false);
    bus.set_base_voltage_in_kV(220.0);
    TEST_ASSERT(bus.is_valid()==true);
}

void BUS_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(2);
    bus.set_bus_name("mybus");
    bus.set_bus_type(PV_TYPE);
    bus.set_voltage_in_pu(1.12);

    bus.clear();

    test_constructor();
}
void BUS_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_name("mybus");
    bus.set_bus_type(PV_TYPE);
    bus.set_voltage_in_pu(1.12);
    FAULT fault;
    fault.set_fault_type(SINGLE_PHASE_GROUNDED_FAULT);
    fault.set_fault_shunt_in_pu(complex<double>(0.0, -2e10));
    bus.set_fault(fault);

    BUS newbus = bus;
    TEST_ASSERT(newbus.get_bus_number()==2);
    TEST_ASSERT(newbus.get_bus_name()=="mybus");
    TEST_ASSERT(newbus.get_bus_type()==PV_TYPE);
    TEST_ASSERT(newbus.get_voltage_in_pu()==1.12);
    TEST_ASSERT(newbus.is_faulted()==true);
    fault = newbus.get_fault();
    TEST_ASSERT(fault.get_fault_type()==SINGLE_PHASE_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_shunt_in_pu()==complex<double>(0.0, -2e10));
}

void BUS_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_bus_number(2);
    TEST_ASSERT(bus.is_connected_to_bus(2)==true);
    TEST_ASSERT(bus.is_connected_to_bus(1)==false);
}

void BUS_TEST::test_set_get_frequency_deviation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BUS_TEST");

    bus.set_frequency_deviation_in_pu(0.0);
    TEST_ASSERT(fabs(bus.get_frequency_deviation_in_pu()-0.0)<FLOAT_EPSILON);
    bus.set_frequency_deviation_in_pu(0.01);
    TEST_ASSERT(fabs(bus.get_frequency_deviation_in_pu()-0.01)<FLOAT_EPSILON);
    bus.set_frequency_deviation_in_pu(-0.01);
    TEST_ASSERT(fabs(bus.get_frequency_deviation_in_pu()+0.01)<FLOAT_EPSILON);
}
