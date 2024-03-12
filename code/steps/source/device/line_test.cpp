#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"

#include "header/device/line_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

LINE_TEST::LINE_TEST() : line(default_toolkit)
{
    TEST_ADD(LINE_TEST::test_constructor);
    TEST_ADD(LINE_TEST::test_set_get_sending_receiving_side_bus);
    TEST_ADD(LINE_TEST::test_set_get_identifier);
    TEST_ADD(LINE_TEST::test_set_get_name);
    TEST_ADD(LINE_TEST::test_set_get_breaker_status);
    TEST_ADD(LINE_TEST::test_set_get_line_z120);
    TEST_ADD(LINE_TEST::test_set_get_line_y120);
    TEST_ADD(LINE_TEST::test_set_get_shunt_y120_at_sending_side);
    TEST_ADD(LINE_TEST::test_set_get_shunt_y120_at_receiving_side);
    TEST_ADD(LINE_TEST::test_set_get_rating);
    TEST_ADD(LINE_TEST::test_set_get_meter_end_bus);
    TEST_ADD(LINE_TEST::test_set_get_length);
    TEST_ADD(LINE_TEST::test_set_get_ownership);
    TEST_ADD(LINE_TEST::test_is_zero_impedance_line);
    TEST_ADD(LINE_TEST::test_set_get_fault);
    TEST_ADD(LINE_TEST::test_clear_fault);
    TEST_ADD(LINE_TEST::test_get_fault_count);
    TEST_ADD(LINE_TEST::test_is_faulted);
    TEST_ADD(LINE_TEST::test_is_valid);
    TEST_ADD(LINE_TEST::test_clear);
    TEST_ADD(LINE_TEST::test_copy_with_operator_equal);
    TEST_ADD(LINE_TEST::test_is_connected_to_bus);
    TEST_ADD(LINE_TEST::test_get_device_id);

    TEST_ADD(LINE_TEST::test_get_line_base_voltage);
    TEST_ADD(LINE_TEST::test_get_line_voltage_at_two_sides);
    TEST_ADD(LINE_TEST::test_get_line_current_at_two_sides);
    TEST_ADD(LINE_TEST::test_get_line_power_at_two_sides);
    TEST_ADD(LINE_TEST::test_get_line_apparent_impedance_at_two_sides);

}

void LINE_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(100);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_positive_sequence_voltage_in_pu(1.1);
    bus.set_positive_sequence_angle_in_rad(0.1);
    psdb.append_bus(bus);
    bus.set_bus_number(2);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_rad(0.0);
    psdb.append_bus(bus);
    bus.set_bus_number(3);
    psdb.append_bus(bus);
    bus.set_bus_number(4);
    psdb.append_bus(bus);
}

void LINE_TEST::tear_down()
{
    line.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void LINE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    TEST_ASSERT(line.get_sending_side_bus()==0);
    TEST_ASSERT(line.get_receiving_side_bus()==0);
    TEST_ASSERT(line.get_identifier()=="");
    TEST_ASSERT(line.get_name()=="");
    TEST_ASSERT(line.get_sending_side_breaker_status()==false);
    TEST_ASSERT(line.get_receiving_side_breaker_status()==false);
    TEST_ASSERT(line.get_line_positive_sequence_z_in_pu()==0.0);
    TEST_ASSERT(line.get_line_positive_sequence_y_in_pu()==0.0);
    TEST_ASSERT(line.get_shunt_positive_sequence_y_at_sending_side_in_pu()==0.0);
    TEST_ASSERT(line.get_shunt_positive_sequence_y_at_receiving_side_in_pu()==0.0);
    TEST_ASSERT(line.get_line_zero_sequence_z_in_pu()==0.0);
    TEST_ASSERT(line.get_line_zero_sequence_y_in_pu()==0.0);
    TEST_ASSERT(line.get_shunt_zero_sequence_y_at_sending_side_in_pu()==0.0);
    TEST_ASSERT(line.get_shunt_zero_sequence_y_at_receiving_side_in_pu()==0.0);
    TEST_ASSERT(line.get_meter_end_bus()==line.get_sending_side_bus());
    TEST_ASSERT(line.get_length()==0.0);

    RATING rating = line.get_rating();
    TEST_ASSERT(rating.get_rating_A_MVA()==0.0);
    TEST_ASSERT(rating.get_rating_B_MVA()==0.0);
    TEST_ASSERT(rating.get_rating_C_MVA()==0.0);

    TEST_ASSERT(line.is_faulted()==false);
    TEST_ASSERT(line.get_fault_count()==0);
}

void LINE_TEST::test_set_get_sending_receiving_side_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    TEST_ASSERT(line.get_sending_side_bus()==1);
    line.set_sending_side_bus(2);
    TEST_ASSERT(line.get_sending_side_bus()==2);
    line.set_sending_side_bus(0);
    TEST_ASSERT(line.get_sending_side_bus()==0);
    line.set_receiving_side_bus(1);
    TEST_ASSERT(line.get_receiving_side_bus()==1);
    line.set_receiving_side_bus(2);
    TEST_ASSERT(line.get_receiving_side_bus()==2);
    line.set_receiving_side_bus(0);
    TEST_ASSERT(line.get_receiving_side_bus()==0);
}

void LINE_TEST::test_set_get_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_identifier("1#");
    TEST_ASSERT(line.get_identifier()=="1#");
}

void LINE_TEST::test_set_get_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_name("line-1#");
    TEST_ASSERT(line.get_name()=="line-1#");
}

void LINE_TEST::test_set_get_breaker_status()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_breaker_status(true);
    TEST_ASSERT(line.get_sending_side_breaker_status()==true);
    line.set_receiving_side_breaker_status(true);
    TEST_ASSERT(line.get_receiving_side_breaker_status()==true);
    line.set_sending_side_breaker_status(false);
    TEST_ASSERT(line.get_sending_side_breaker_status()==false);
    line.set_receiving_side_breaker_status(false);
    TEST_ASSERT(line.get_receiving_side_breaker_status()==false);
}

void LINE_TEST::test_set_get_line_z120()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    complex<double> z;
    z=complex<double>(0.01, 0.05);

    line.set_line_positive_sequence_z_in_pu(z);
    TEST_ASSERT(abs(line.get_line_positive_sequence_z_in_pu()-z)<FLOAT_EPSILON);

    line.set_line_zero_sequence_z_in_pu(z);
    TEST_ASSERT(abs(line.get_line_zero_sequence_z_in_pu()-z)<FLOAT_EPSILON);
}

void LINE_TEST::test_set_get_line_y120()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    complex<double> y;
    y=complex<double>(0.00001, 0.024);

    line.set_line_positive_sequence_y_in_pu(y);
    TEST_ASSERT(abs(line.get_line_positive_sequence_y_in_pu()-y)<FLOAT_EPSILON);

    line.set_line_zero_sequence_y_in_pu(y);
    TEST_ASSERT(abs(line.get_line_zero_sequence_y_in_pu()-y)<FLOAT_EPSILON);
}

void LINE_TEST::test_set_get_shunt_y120_at_sending_side()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    complex<double> y;
    y=complex<double>(0.00001, 0.024);

    line.set_shunt_positive_sequence_y_at_sending_side_in_pu(y);
    TEST_ASSERT(abs(line.get_shunt_positive_sequence_y_at_sending_side_in_pu()-y)<FLOAT_EPSILON);

    line.set_shunt_zero_sequence_y_at_sending_side_in_pu(y);
    TEST_ASSERT(abs(line.get_shunt_zero_sequence_y_at_sending_side_in_pu()-y)<FLOAT_EPSILON);
}

void LINE_TEST::test_set_get_shunt_y120_at_receiving_side()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    complex<double> y;
    y=complex<double>(0.00001, 0.024);
    line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(y);
    TEST_ASSERT(abs(line.get_shunt_positive_sequence_y_at_receiving_side_in_pu()-y)<FLOAT_EPSILON);

    line.set_shunt_zero_sequence_y_at_receiving_side_in_pu(y);
    TEST_ASSERT(abs(line.get_shunt_zero_sequence_y_at_receiving_side_in_pu()-y)<FLOAT_EPSILON);
}

void LINE_TEST::test_set_get_rating()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    RATING rating, rating_out;
    rating.set_rating_A_MVA(50.0);
    rating.set_rating_B_MVA(70.0);
    rating.set_rating_C_MVA(90.0);

    line.set_rating(rating);
    rating_out = line.get_rating();
    TEST_ASSERT(rating_out.get_rating_A_MVA()==50.0);
    TEST_ASSERT(rating_out.get_rating_B_MVA()==70.0);
    TEST_ASSERT(rating_out.get_rating_C_MVA()==90.0);
}

void LINE_TEST::test_set_get_meter_end_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);

    line.set_meter_end_bus(0);
    TEST_ASSERT(line.get_meter_end_bus()==line.get_sending_side_bus());

    line.set_meter_end_bus(line.get_sending_side_bus());
    TEST_ASSERT(line.get_meter_end_bus()==line.get_sending_side_bus());

    line.set_meter_end_bus(3);
    TEST_ASSERT(line.get_meter_end_bus()==line.get_sending_side_bus());

    line.set_meter_end_bus(line.get_receiving_side_bus());
    TEST_ASSERT(line.get_meter_end_bus()==line.get_receiving_side_bus());
}

void LINE_TEST::test_set_get_length()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_length(123.0);
    TEST_ASSERT(line.get_length()==123.0);

    line.set_length(-3.0);
    TEST_ASSERT(line.get_length()==123.0);
}

void LINE_TEST::test_set_get_ownership()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    OWNERSHIP os0, os1;
    os0.append_owner_and_its_fraction(1, 0.5);
    os0.append_owner_and_its_fraction(2, 0.3);
    os0.append_owner_and_its_fraction(3, 0.2);
    line.set_ownership(os0);

    os1 = line.get_ownership();
    TEST_ASSERT(os1.get_owner_count()==3);
    TEST_ASSERT(os1.get_owner_of_index(0)==1);
    TEST_ASSERT(os1.get_owner_of_index(1)==2);
    TEST_ASSERT(os1.get_owner_of_index(2)==3);
    TEST_ASSERT(fabs(os1.get_fraction_of_owner_of_index(0)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(os1.get_fraction_of_owner_of_index(1)-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(os1.get_fraction_of_owner_of_index(2)-0.2)<FLOAT_EPSILON);
}

void LINE_TEST::test_is_zero_impedance_line()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    complex<double> z(0.0, 0.001), y(0.0, 0.002);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.set_zero_impedance_threshold_in_pu(0.001);
    TEST_ASSERT(line.is_zero_impedance_line()==false);
    psdb.set_zero_impedance_threshold_in_pu(0.01);
    TEST_ASSERT(line.is_zero_impedance_line()==false);

    line.set_line_positive_sequence_y_in_pu(0.0);

    psdb.set_zero_impedance_threshold_in_pu(0.0001);
    TEST_ASSERT(line.is_zero_impedance_line()==false);
    psdb.set_zero_impedance_threshold_in_pu(0.001);
    TEST_ASSERT(line.is_zero_impedance_line()==true);
    psdb.set_zero_impedance_threshold_in_pu(0.01);
    TEST_ASSERT(line.is_zero_impedance_line()==true);
}

void LINE_TEST::test_set_get_fault()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");


    unsigned int sending_side_bus = line.get_sending_side_bus();
    unsigned int receiving_side_bus = line.get_receiving_side_bus();

    double location[6] = {0.0, 0.2, 0.3, 0.5, 0.6, 1.0};
    // actual location  [0.0, 0.8, 0.3, 0.5, 0.6, 1.0]
    unsigned int fault_bus[6] = {sending_side_bus, receiving_side_bus, sending_side_bus, receiving_side_bus, sending_side_bus, sending_side_bus};
    complex<double> y[6] = {complex<double>(0.0, -0.01), complex<double>(0.0, -0.1),
                           complex<double>(0.1, 0.0),   complex<double>(0.0, -0.02),
                           complex<double>(0.0, 0.5),   complex<double>(0.0, -0.001)};
    FAULT fault;

    for(unsigned int i=0; i!=6; ++i)
    {
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(y[i]);
        line.set_fault(fault_bus[i],location[i], fault);
    }
    //index of location [0.0, 0.8, 0.3, 0.5, 0.6, 1.0]
    //                   0    4    1    2    3    5
    for(unsigned int i=0; i!=6; ++i)
    {
        double loc = line.get_fault_location_of_fault(i);
        FAULT fault2 = line.get_fault_at_location(line.get_sending_side_bus(),loc);
        if(fabs(loc-0.0)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[0])<FLOAT_EPSILON)
            continue;
        }
        if(fabs(loc-0.8)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[1])<FLOAT_EPSILON)
            continue;
        }
        if(fabs(loc-0.3)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[2])<FLOAT_EPSILON)
            continue;
        }
        if(fabs(loc-0.5)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[3])<FLOAT_EPSILON)
            continue;
        }
        if(fabs(loc-0.6)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[4])<FLOAT_EPSILON)
            continue;
        }
        if(fabs(loc-1.0)<FLOAT_EPSILON)
        {
            TEST_ASSERT(abs(fault2.get_fault_shunt_in_pu()-y[5])<FLOAT_EPSILON)
            continue;
        }
        TEST_ASSERT(false);
    }
}

void LINE_TEST::test_clear_fault()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");


    unsigned int sending_side_bus = line.get_sending_side_bus();
    unsigned int receiving_side_bus = line.get_receiving_side_bus();

    double location[6] = {0.0, 0.2, 0.3, 0.5, 0.6, 1.0};
    // actual location  [0.0, 0.8, 0.3, 0.5, 0.6, 1.0]
    unsigned int fault_bus[6] = {sending_side_bus, receiving_side_bus, sending_side_bus, receiving_side_bus, sending_side_bus, sending_side_bus};
    complex<double> y[6] = {complex<double>(0.0, -0.01), complex<double>(0.0, -0.1),
                           complex<double>(0.1, 0.0),   complex<double>(0.0, -0.02),
                           complex<double>(0.0, 0.5),   complex<double>(0.0, -0.001)};
    FAULT fault;

    for(unsigned int i=0; i!=6; ++i)
    {
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(y[i]);
        line.set_fault(fault_bus[i],location[i], fault);
        TEST_ASSERT(line.is_faulted()==true);
        TEST_ASSERT(line.get_fault_count()==i+1);
    }

    line.clear_fault_at_location(sending_side_bus, 0.0);
    TEST_ASSERT(line.get_fault_count()==5);

    line.clear_fault_at_location(sending_side_bus, 0.8);
    TEST_ASSERT(line.get_fault_count()==4);

    line.clear_fault_at_location(receiving_side_bus, 0.4);
    TEST_ASSERT(line.get_fault_count()==3);

    line.clear_all_faults();
    TEST_ASSERT(line.is_faulted()==false);
}
void LINE_TEST::test_get_fault_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");


    unsigned int sending_side_bus = line.get_sending_side_bus();
    unsigned int receiving_side_bus = line.get_receiving_side_bus();

    double location[6] = {0.0, 0.2, 0.3, 0.5, 0.6, 1.0};
    // actual location  [0.0, 0.8, 0.3, 0.5, 0.6, 1.0]
    unsigned int fault_bus[6] = {sending_side_bus, receiving_side_bus, sending_side_bus, receiving_side_bus, sending_side_bus, sending_side_bus};
    complex<double> y[6] = {complex<double>(0.0, -0.01), complex<double>(0.0, -0.1),
                           complex<double>(0.1, 0.0),   complex<double>(0.0, -0.02),
                           complex<double>(0.0, 0.5),   complex<double>(0.0, -0.001)};
    FAULT fault;

    for(unsigned int i=0; i!=6; ++i)
    {
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(y[i]);
        line.set_fault(fault_bus[i],location[i], fault);
        TEST_ASSERT(line.get_fault_count()==i+1);
    }
}
void LINE_TEST::test_is_faulted()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");


    unsigned int sending_side_bus = line.get_sending_side_bus();
    unsigned int receiving_side_bus = line.get_receiving_side_bus();

    double location[6] = {0.0, 0.2, 0.3, 0.5, 0.6, 1.0};
    // actual location  [0.0, 0.8, 0.3, 0.5, 0.6, 1.0]
    unsigned int fault_bus[6] = {sending_side_bus, receiving_side_bus, sending_side_bus, receiving_side_bus, sending_side_bus, sending_side_bus};
    complex<double> y[6] = {complex<double>(0.0, -0.01), complex<double>(0.0, -0.1),
                           complex<double>(0.1, 0.0),   complex<double>(0.0, -0.02),
                           complex<double>(0.0, 0.5),   complex<double>(0.0, -0.001)};
    FAULT fault;

    for(unsigned int i=0; i!=6; ++i)
    {
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(y[i]);
        line.set_fault(fault_bus[i],location[i], fault);
        TEST_ASSERT(line.is_faulted()==true);
    }
}

void LINE_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    TEST_ASSERT(line.is_valid()==false);

    line.set_sending_side_bus(1);
    TEST_ASSERT(line.is_valid()==false);
    line.set_receiving_side_bus(2);
    TEST_ASSERT(line.is_valid()==true);
}

void LINE_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);

    line.clear();
    test_constructor();
}

void LINE_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(1);
    line.set_identifier("1#");
    line.set_name("line-1#");
    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(false);
    line.set_line_positive_sequence_z_in_pu(complex<double>(0.01, 0.05));
    line.set_shunt_zero_sequence_y_at_receiving_side_in_pu(complex<double>(0.01, 0.05));
    line.set_meter_end_bus(1);
    line.set_length(234.0);

    FAULT fault;
    fault.set_fault_type(THREE_PHASES_FAULT);
    fault.set_fault_shunt_in_pu(complex<double>(0.0,-2e10));
    line.set_fault(1,0.2,fault);

    LINE newline = line;
    TEST_ASSERT(newline.get_sending_side_bus()==2);
    TEST_ASSERT(newline.get_receiving_side_bus()==1);
    TEST_ASSERT(newline.get_identifier()=="1#");
    TEST_ASSERT(newline.get_name()=="line-1#");
    TEST_ASSERT(newline.get_sending_side_breaker_status()==false);
    TEST_ASSERT(newline.get_receiving_side_breaker_status()==false);
    TEST_ASSERT(newline.get_length()==234.0);
    TEST_ASSERT(abs(newline.get_line_positive_sequence_z_in_pu()-complex<double>(0.01,0.05))<FLOAT_EPSILON);
    TEST_ASSERT(abs(newline.get_shunt_zero_sequence_y_at_receiving_side_in_pu()-complex<double>(0.01,0.05))<FLOAT_EPSILON);
    TEST_ASSERT(newline.get_meter_end_bus()==1);
    TEST_ASSERT(newline.get_length()==234.0);

    TEST_ASSERT(newline.is_faulted()==true);
    TEST_ASSERT(newline.get_fault_count()==1);
    double location = newline.get_fault_location_of_fault(0);
    FAULT fault2 = newline.get_fault_at_location(newline.get_sending_side_bus(),location);
    TEST_ASSERT(fault2.get_fault_shunt_in_pu()==complex<double>(0.0,-2e10));

}

void LINE_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("#1");

    DEVICE_ID did = line.get_device_id();

    DEVICE_ID did2;
    did2.set_device_type(STEPS_AC_LINE);
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did2.set_device_terminal(terminal);
    did2.set_device_identifier_index(get_index_of_string("#1"));

    TEST_ASSERT(did==did2);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did2.set_device_terminal(terminal);

    TEST_ASSERT(did!=did2);
}

void LINE_TEST::test_is_connected_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    TEST_ASSERT(line.is_connected_to_bus(1)==true);
    TEST_ASSERT(line.is_connected_to_bus(2)==true);
    TEST_ASSERT(line.is_connected_to_bus(3)==false);
}

void LINE_TEST::test_get_line_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);

    TEST_ASSERT(line.get_line_base_voltage_in_kV()==110.0);
}

void LINE_TEST::test_get_line_voltage_at_two_sides()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);

    complex<double> Vs(1.1*steps_cos(0.1), 1.1*steps_sin(0.1)), Vr(1.05, 0.0);

    TEST_ASSERT(abs(line.get_line_complex_voltage_at_sending_side_in_pu()-Vs)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_voltage_at_receiving_side_in_pu()-Vr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_voltage_at_sending_side_in_kV()-Vs*110.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_voltage_at_receiving_side_in_kV()-Vr*110.0)<FLOAT_EPSILON);
}

void LINE_TEST::test_get_line_current_at_two_sides()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);

    complex<double> Zline(0.001, 0.02), Yline(0.0001, 0.03), Yshunt_s(0.001, 0.2), Yshunt_r(0.001, 0.3);
    line.set_line_positive_sequence_z_in_pu(Zline);
    line.set_line_positive_sequence_y_in_pu(Yline);
    line.set_shunt_positive_sequence_y_at_sending_side_in_pu(Yshunt_s);
    line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(Yshunt_r);


    complex<double> Vs(1.1*steps_cos(0.1), 1.1*steps_sin(0.1)), Vr(1.05, 0.0);
    complex<double> Isr, Is, Ir;
    double Ibase = 100.0/(SQRT3*110.0);

    Isr = (Vs-Vr)/Zline;
    Is = Isr+Vs*(0.5*Yline+Yshunt_s);
    Ir = -Isr+Vr*(0.5*Yline+Yshunt_r);

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_pu()-Is)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_pu()-Ir)<FLOAT_EPSILON);

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_kA()-Is*Ibase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_kA()-Ir*Ibase)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(false);

    Is = Vs*(0.5*Yline+Yshunt_s+1.0/(Zline+1.0/(0.5*Yline+Yshunt_r)));
    Ir = 0.0;

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_pu()-Is)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_pu()-Ir)<FLOAT_EPSILON);

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_kA()-Is*Ibase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_kA()-Ir*Ibase)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(true);

    Is = 0.0;
    Ir = Vr*(0.5*Yline+Yshunt_r+1.0/(Zline+1.0/(0.5*Yline+Yshunt_s)));

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_pu()-Is)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_pu()-Ir)<FLOAT_EPSILON);

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_kA()-Is*Ibase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_kA()-Ir*Ibase)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(false);

    Is = 0.0;
    Ir = 0.0;

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_pu()-Is)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_pu()-Ir)<FLOAT_EPSILON);

    TEST_ASSERT(abs(line.get_line_complex_current_at_sending_side_in_kA()-Is*Ibase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_current_at_receiving_side_in_kA()-Ir*Ibase)<FLOAT_EPSILON);
}

void LINE_TEST::test_get_line_power_at_two_sides()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);

    complex<double> Zline(0.001, 0.02), Yline(0.0001, 0.03), Yshunt_s(0.001, 0.2), Yshunt_r(0.001, 0.3);
    line.set_line_positive_sequence_z_in_pu(Zline);
    line.set_line_positive_sequence_y_in_pu(Yline);
    line.set_shunt_positive_sequence_y_at_sending_side_in_pu(Yshunt_s);
    line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(Yshunt_r);


    complex<double> Vs(1.1*steps_cos(0.1), 1.1*steps_sin(0.1)), Vr(1.05, 0.0);
    complex<double> Isr, Is, Ir, Ss, Sr;

    Isr = (Vs-Vr)/Zline;
    Is = Isr+Vs*(0.5*Yline+Yshunt_s);
    Ir = -Isr+Vr*(0.5*Yline+Yshunt_r);
    Ss = Vs*conj(Is);
    Sr = Vr*conj(Ir);

    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_pu()-Ss)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_pu()-Sr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_MVA()-Ss*100.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_MVA()-Sr*100.0)<FLOAT_EPSILON);
    cout<<"calculated line power error "<<line.get_line_complex_power_at_receiving_side_in_MVA()-Sr*100.0<<endl;

    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(false);

    Is = Vs*(0.5*Yline+Yshunt_s+1.0/(Zline+1.0/(0.5*Yline+Yshunt_r)));
    Ir = 0.0;
    Ss = Vs*conj(Is);
    Sr = Vr*conj(Ir);

    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_pu()-Ss)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_pu()-Sr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_MVA()-Ss*100.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_MVA()-Sr*100.0)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(true);

    Is = 0.0;
    Ir = Vr*(0.5*Yline+Yshunt_r+1.0/(Zline+1.0/(0.5*Yline+Yshunt_s)));
    Ss = Vs*conj(Is);
    Sr = Vr*conj(Ir);

    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_pu()-Ss)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_pu()-Sr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_MVA()-Ss*100.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_MVA()-Sr*100.0)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(false);

    Is = 0.0;
    Ir = 0.0;
    Ss = Vs*conj(Is);
    Sr = Vr*conj(Ir);

    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_pu()-Ss)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_pu()-Sr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_sending_side_in_MVA()-Ss*100.0)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_power_at_receiving_side_in_MVA()-Sr*100.0)<FLOAT_EPSILON);

}

void LINE_TEST::test_get_line_apparent_impedance_at_two_sides()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LINE_TEST");

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);

    complex<double> Zline(0.001, 0.02), Yline(0.0001, 0.03), Yshunt_s(0.001, 0.2), Yshunt_r(0.001, 0.3);
    line.set_line_positive_sequence_z_in_pu(Zline);
    line.set_line_positive_sequence_y_in_pu(Yline);
    line.set_shunt_positive_sequence_y_at_sending_side_in_pu(Yshunt_s);
    line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(Yshunt_r);


    complex<double> Vs(1.1*steps_cos(0.1), 1.1*steps_sin(0.1)), Vr(1.05, 0.0);
    complex<double> Isr, Is, Ir, Zs, Zr;
    double Zbase = 110.0*110.0/100.0;

    Isr = (Vs-Vr)/Zline;
    Is = Isr+Vs*(0.5*Yline+Yshunt_s);
    Ir = -Isr+Vr*(0.5*Yline+Yshunt_r);
    Zs = Vs/Is;
    Zr = Vr/Ir;

    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_pu()-Zs)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_pu()-Zr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_ohm()-Zs*Zbase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_ohm()-Zr*Zbase)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(false);

    Is = Vs*(0.5*Yline+Yshunt_s+1.0/(Zline+1.0/(0.5*Yline+Yshunt_r)));
    Ir = 0.0;
    Zs = Vs/Is;

    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_pu()-Zs)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_pu())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_ohm()-Zs*Zbase)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_ohm())>1e6);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(true);

    Is = 0.0;
    Ir = Vr*(0.5*Yline+Yshunt_r+1.0/(Zline+1.0/(0.5*Yline+Yshunt_s)));
    Zr = Vr/Ir;

    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_pu())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_pu()-Zr)<FLOAT_EPSILON);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_ohm())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_ohm()-Zr*Zbase)<FLOAT_EPSILON);

    line.set_sending_side_breaker_status(false);
    line.set_receiving_side_breaker_status(false);

    Is = 0.0;
    Ir = 0.0;

    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_pu())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_pu())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_sending_side_in_ohm())>1e6);
    TEST_ASSERT(abs(line.get_line_complex_apparent_impedance_at_receiving_side_in_ohm())>1e6);
}


#endif
