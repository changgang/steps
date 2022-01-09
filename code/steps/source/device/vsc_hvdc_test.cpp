#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"

#include "header/device/vsc_hvdc_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/hvdc_model/hvdc_models.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

//#ifdef ENABLE_STEPS_TEST
using namespace std;

VSC_HVDC_TEST::VSC_HVDC_TEST():vsc(default_toolkit)
{
    TEST_ADD(VSC_HVDC_TEST::test_constructor);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_identifier);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_name);
    TEST_ADD(VSC_HVDC_TEST::test_build_conductance_matrix);
    TEST_ADD(VSC_HVDC_TEST::test_set_get_vsc_hvdc_model);
}

void VSC_HVDC_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_allowed_max_bus_number(200);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(500.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);
    bus.set_bus_type(PQ_TYPE);
    psdb.append_bus(bus);
    bus.set_bus_number(2);
    psdb.append_bus(bus);
    bus.set_bus_number(3);
    psdb.append_bus(bus);
    bus.set_bus_number(4);
    psdb.append_bus(bus);
}

void VSC_HVDC_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}

void VSC_HVDC_TEST::test_constructor()
{
    TEST_ASSERT(vsc.get_identifier()=="");
    TEST_ASSERT(vsc.get_name()=="");
    TEST_ASSERT(vsc.get_converter_count()==0);
    TEST_ASSERT(vsc.get_dc_bus_count()==0);
    TEST_ASSERT(vsc.get_dc_line_count()==0);
}

void VSC_HVDC_TEST::test_set_get_identifier()
{
    vsc.set_identifier("1");
    TEST_ASSERT(vsc.get_identifier()=="1");
    vsc.set_identifier("#1");
    TEST_ASSERT(vsc.get_identifier()=="#1");
    TEST_ASSERT(vsc.get_name()=="#1");
}

void VSC_HVDC_TEST::test_set_get_name()
{
    vsc.set_name("1");
    TEST_ASSERT(vsc.get_name()=="1");
    vsc.set_name("#1");
    TEST_ASSERT(vsc.get_name()=="#1");
    TEST_ASSERT(vsc.get_identifier()=="#1");
}

void VSC_HVDC_TEST::test_build_conductance_matrix()
{
    /*

    */
    prepare_3_terminal_vsc_hvdc_buses();
    vsc.set_converter_count(3);
    vsc.set_dc_bus_count(3);
    vsc.set_dc_line_count(3);

    /*vsc.build_matrix();
    y = vsc.get_matrix();
    TEST_ASSERT();
    TEST_ASSERT(fabs(y(1,1)-0.9)<FLOAT_EPSILON);
    */
}

void VSC_HVDC_TEST::test_set_get_vsc_hvdc_model()
{
    ;
}

void VSC_HVDC_TEST::prepare_2_terminal_vsc_hvdc_buses()
{
    vsc.set_converter_bus(0,1);
    vsc.set_converter_bus(1,2);
}

void VSC_HVDC_TEST::prepare_3_terminal_vsc_hvdc_buses()
{
    prepare_2_terminal_vsc_hvdc_buses();
    vsc.set_converter_bus(2,3);
}

void VSC_HVDC_TEST::prepare_4_terminal_vsc_hvdc_buses()
{
    prepare_3_terminal_vsc_hvdc_buses();
    vsc.set_converter_bus(3,4);
}

//#endif
