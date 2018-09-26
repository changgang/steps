#include "header/basic/timer_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/device/bus.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

TIMER_TEST::TIMER_TEST()
{
    TEST_ADD(TIMER_TEST::test_constructor);
    TEST_ADD(TIMER_TEST::test_set_get_time_interval);
    TEST_ADD(TIMER_TEST::test_start_reset_timer);
    TEST_ADD(TIMER_TEST::test_is_timed_out);
    TEST_ADD(TIMER_TEST::test_is_valid);
    TEST_ADD(TIMER_TEST::test_copy_with_operator_equal);
}

void TIMER_TEST::setup()
{
    timer = new TIMER;
    psdb = get_default_power_system_database();
    simulator = new DYNAMICS_SIMULATOR(psdb);

    psdb->set_allowed_max_bus_number(1000);
    BUS bus(psdb);
    bus.set_bus_number(1);
    bus.set_bus_name("bus");
    bus.set_base_voltage_in_kV(100.0);
    bus.set_bus_type(PQ_TYPE);

    psdb->append_bus(bus);

    BUS* busptr = psdb->get_bus(1);
    timer->set_attached_device(busptr);
}

void TIMER_TEST::tear_down()
{
    delete timer;
    delete simulator;
    psdb->clear_database();

    show_test_end_information();
}

void TIMER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    TEST_ASSERT(fabs(timer->get_timer_interval_in_s()-INFINITE_THRESHOLD)<FLOAT_EPSILON);
    TEST_ASSERT(timer->is_started()==false);
    TEST_ASSERT(timer->is_timed_out()==false);
}


void TIMER_TEST::test_set_get_time_interval()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    timer->set_timer_interval_in_s(2.0);
    TEST_ASSERT(fabs(timer->get_timer_interval_in_s()-2.0)<FLOAT_EPSILON);
    timer->set_timer_interval_in_s(5.0);
    TEST_ASSERT(fabs(timer->get_timer_interval_in_s()-5.0)<FLOAT_EPSILON);
}

void TIMER_TEST::test_start_reset_timer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    timer->set_timer_interval_in_s(2.0);

    TEST_ASSERT(timer->is_started()==false);

    set_dynamic_simulation_time_in_s(1.0);

    timer->start();
    TEST_ASSERT(timer->is_started()==true);
    TEST_ASSERT(fabs(timer->get_time_when_started_in_s()-1.0)<FLOAT_EPSILON);

    set_dynamic_simulation_time_in_s(4.0);
    timer->reset();
    TEST_ASSERT(timer->is_started()==false);
    TEST_ASSERT(fabs(timer->get_time_when_started_in_s()-INFINITE_THRESHOLD)<FLOAT_EPSILON);
}

void TIMER_TEST::test_is_timed_out()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    timer->set_timer_interval_in_s(2.0);

    set_dynamic_simulation_time_in_s(1.0);
    timer->start();

    TEST_ASSERT(timer->is_timed_out()==false);

    set_dynamic_simulation_time_in_s(1.5);
    TEST_ASSERT(timer->is_timed_out()==false);

    set_dynamic_simulation_time_in_s(2.5);
    TEST_ASSERT(timer->is_timed_out()==false);

    set_dynamic_simulation_time_in_s(3.0);
    TEST_ASSERT(timer->is_timed_out()==true);

    set_dynamic_simulation_time_in_s(3.5);
    TEST_ASSERT(timer->is_timed_out()==true);
}

void TIMER_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    TEST_ASSERT(timer->is_valid() == false);

    timer->set_timer_interval_in_s(2.0);
    TEST_ASSERT(timer->is_valid() == true);
}

void TIMER_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIMER_TEST");

    timer->set_timer_interval_in_s(2.0);

    TIMER newtimer = (*timer);

    TEST_ASSERT(fabs(newtimer.get_timer_interval_in_s()-2.0)<FLOAT_EPSILON);

    TIMER newtimer2;
    newtimer2 = (*timer);
    TEST_ASSERT(fabs(newtimer2.get_timer_interval_in_s()-2.0)<FLOAT_EPSILON);

}
