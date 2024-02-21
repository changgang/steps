#include "header/basic/test_macro.h"
#include "header/model/converter_common_models/vrt_model/lvrt_control_test.h"
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

LVRT_CONTROL_TEST::LVRT_CONTROL_TEST()
{
    TEST_ADD(LVRT_CONTROL_TEST::test_get_bus);
    TEST_ADD(LVRT_CONTROL_TEST::test_get_device);
    TEST_ADD(LVRT_CONTROL_TEST::test_get_bus_voltage);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_vrt_voltage_threshold);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_vrt_prepare2recover_delay_time_in_s);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_vrt_status);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_during_status);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_prepare2recover_status);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_recover_status);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_P0_vrt_activated_in_pu_based_on_mbase);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Q0_vrt_activated_in_pu_based_on_mbase);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_prepare2recover_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_prepare2recover_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_recover_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_recover_activated_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_P0_vrt_recover_activated_in_pu_based_on_mbase);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Q0_vrt_recover_activated_in_pu_based_on_mbase);

    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_PQ_vrt_during_mode);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_kP);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_P_set_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_k1_Ip);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_k2_Ip);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_Ip_set_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_LVPL_low_voltage_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_LVPL_high_voltage_in_pu);
    TEST_ADD(LVRT_CONTROL_TEST::test_set_get_LVPL_gain_at_high_voltage);
    TEST_ADD(LVRT_CONTROL_TEST::test_check_vrt_status_basic);
    TEST_ADD(LVRT_CONTROL_TEST::test_check_vrt_status_comprehensive);
}

void LVRT_CONTROL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(0.69);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(30.0);

    psdb.append_bus(bus);

    WT_GENERATOR wt_generator(default_toolkit);
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

    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("#1"));

    lvrt = new LVRT_CONTROL(default_toolkit);

    lvrt->set_bus_pointer(psdb.get_bus(1));
    lvrt->set_device_pointer(psdb.get_wt_generator(did));
}

void LVRT_CONTROL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    delete lvrt;
    lvrt = nullptr;
}

void LVRT_CONTROL_TEST::test_get_bus()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    TEST_ASSERT(lvrt->get_bus()==1);
    TEST_ASSERT(lvrt->get_bus_pointer()==psdb.get_bus(1));
}

void LVRT_CONTROL_TEST::test_get_device()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("#1"));

    TEST_ASSERT(lvrt->get_device_pointer()==psdb.get_wt_generator(did));
    TEST_ASSERT(lvrt->get_device_id()==did);
}

void LVRT_CONTROL_TEST::test_get_bus_voltage()
{
    TEST_ASSERT(fabs(lvrt->get_bus_voltage()-1)<FLOAT_EPSILON);
    BUS* bus = lvrt->get_bus_pointer();
    bus->set_positive_sequence_voltage_in_pu(0.9);
    TEST_ASSERT(fabs(lvrt->get_bus_voltage()-0.9)<FLOAT_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_vrt_voltage_threshold()
{
    lvrt->set_vrt_voltage_threshold(0.9);
    TEST_ASSERT(fabs(lvrt->get_vrt_voltage_threshold()-0.9)<FLOAT_EPSILON);
    lvrt->set_vrt_voltage_threshold(0.8);
    TEST_ASSERT(fabs(lvrt->get_vrt_voltage_threshold()-0.8)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_vrt_prepare2recover_delay_time_in_s()
{
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.5);
    TEST_ASSERT(fabs(lvrt->get_vrt_prepare2recover_delay_time_in_s()-0.5)<FLOAT_EPSILON);
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.0);
    TEST_ASSERT(fabs(lvrt->get_vrt_prepare2recover_delay_time_in_s()-0.0)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_vrt_status()
{
    lvrt->set_vrt_status(VRT_NORMAL_STATUS);
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);
    lvrt->set_vrt_status(VRT_DURING_STATUS);
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);
}

void LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_during_status()
{
    lvrt->set_time_when_vrt_enter_during_status(1);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_during_status()-1)<FLOAT_EPSILON);
    lvrt->set_time_when_vrt_enter_during_status(2);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_during_status()-2)<FLOAT_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_prepare2recover_status()
{
    lvrt->set_time_when_vrt_enter_prepare2recover_status(1);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_prepare2recover_status()-1)<FLOAT_EPSILON);
    lvrt->set_time_when_vrt_enter_prepare2recover_status(2);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_prepare2recover_status()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_time_when_vrt_enter_recover_status()
{
    lvrt->set_time_when_vrt_enter_recover_status(1);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_recover_status()-1)<FLOAT_EPSILON);
    lvrt->set_time_when_vrt_enter_recover_status(2);
    TEST_ASSERT(fabs(lvrt->get_time_when_vrt_enter_recover_status()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_activated_in_pu()
{
    lvrt->set_Ip0_vrt_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Ip0_vrt_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_activated_in_pu()
{
    lvrt->set_Iq0_vrt_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Iq0_vrt_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_P0_vrt_activated_in_pu_based_on_mbase()
{
    lvrt->set_P0_vrt_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_P0_vrt_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Q0_vrt_activated_in_pu_based_on_mbase()
{
    lvrt->set_Q0_vrt_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_Q0_vrt_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_prepare2recover_activated_in_pu()
{
    lvrt->set_Ip0_vrt_prepare2recover_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_prepare2recover_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Ip0_vrt_prepare2recover_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_prepare2recover_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_prepare2recover_activated_in_pu()
{
    lvrt->set_Iq0_vrt_prepare2recover_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_prepare2recover_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Iq0_vrt_prepare2recover_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_prepare2recover_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase()
{
    lvrt->set_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase()
{
    lvrt->set_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Ip0_vrt_recover_activated_in_pu()
{
    lvrt->set_Ip0_vrt_recover_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_recover_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Ip0_vrt_recover_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Ip0_vrt_recover_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Iq0_vrt_recover_activated_in_pu()
{
    lvrt->set_Iq0_vrt_recover_activated_in_pu(1);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_recover_activated_in_pu()-1)<FLOAT_EPSILON);
    lvrt->set_Iq0_vrt_recover_activated_in_pu(2);
    TEST_ASSERT(fabs(lvrt->get_Iq0_vrt_recover_activated_in_pu()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_P0_vrt_recover_activated_in_pu_based_on_mbase()
{
    lvrt->set_P0_vrt_recover_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_recover_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_P0_vrt_recover_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_P0_vrt_recover_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_Q0_vrt_recover_activated_in_pu_based_on_mbase()
{
    lvrt->set_Q0_vrt_recover_activated_in_pu_based_on_mbase(1);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_recover_activated_in_pu_based_on_mbase()-1)<FLOAT_EPSILON);
    lvrt->set_Q0_vrt_recover_activated_in_pu_based_on_mbase(2);
    TEST_ASSERT(fabs(lvrt->get_Q0_vrt_recover_activated_in_pu_based_on_mbase()-2)<FLOAT_EPSILON);
}

void LVRT_CONTROL_TEST::test_set_get_PQ_vrt_during_mode()
{
    lvrt->set_P_vrt_during_mode(VRT_DURING_POWER_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_P_vrt_during_mode()== VRT_DURING_POWER_CONTROL_MODE);
    lvrt->set_P_vrt_during_mode(VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_P_vrt_during_mode()== VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE);
    lvrt->set_P_vrt_during_mode(VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_P_vrt_during_mode()== VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE);
    lvrt->set_P_vrt_during_mode(VRT_DURING_LVPL_CURRENT_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_P_vrt_during_mode()== VRT_DURING_LVPL_CURRENT_CONTROL_MODE);

    lvrt->set_Q_vrt_during_mode(VRT_DURING_POWER_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_Q_vrt_during_mode()== VRT_DURING_POWER_CONTROL_MODE);
    lvrt->set_Q_vrt_during_mode(VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_Q_vrt_during_mode()== VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE);
    lvrt->set_Q_vrt_during_mode(VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE);
    TEST_ASSERT(lvrt->get_Q_vrt_during_mode()== VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE);
}
void LVRT_CONTROL_TEST::test_set_get_kP()
{
    lvrt->set_kP(1.0);
    TEST_ASSERT(fabs(lvrt->get_kP()-1.0)<DOUBLE_EPSILON);
    lvrt->set_kP(0.9);
    TEST_ASSERT(fabs(lvrt->get_kP()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_P_set_in_pu()
{
    lvrt->set_P_set_in_pu(1.0);
    TEST_ASSERT(fabs(lvrt->get_P_set_in_pu()-1.0)<DOUBLE_EPSILON);
    lvrt->set_P_set_in_pu(0.9);
    TEST_ASSERT(fabs(lvrt->get_P_set_in_pu()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_k1_Ip()
{
    lvrt->set_k1_Ip(1.0);
    TEST_ASSERT(fabs(lvrt->get_k1_Ip()-1.0)<DOUBLE_EPSILON);
    lvrt->set_k1_Ip(0.9);
    TEST_ASSERT(fabs(lvrt->get_k1_Ip()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_k2_Ip()
{
    lvrt->set_k2_Ip(1.0);
    TEST_ASSERT(fabs(lvrt->get_k2_Ip()-1.0)<DOUBLE_EPSILON);
    lvrt->set_k2_Ip(0.9);
    TEST_ASSERT(fabs(lvrt->get_k2_Ip()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_Ip_set_in_pu()
{
    lvrt->set_Ip_set_in_pu(1.0);
    TEST_ASSERT(fabs(lvrt->get_Ip_set_in_pu()-1.0)<DOUBLE_EPSILON);
    lvrt->set_Ip_set_in_pu(0.9);
    TEST_ASSERT(fabs(lvrt->get_Ip_set_in_pu()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_LVPL_low_voltage_in_pu()
{
    lvrt->set_LVPL_low_voltage_in_pu(1.0);
    TEST_ASSERT(fabs(lvrt->get_LVPL_low_voltage_in_pu()-1.0)<DOUBLE_EPSILON);
    lvrt->set_LVPL_low_voltage_in_pu(0.9);
    TEST_ASSERT(fabs(lvrt->get_LVPL_low_voltage_in_pu()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_LVPL_high_voltage_in_pu()
{
    lvrt->set_LVPL_high_voltage_in_pu(1.0);
    TEST_ASSERT(fabs(lvrt->get_LVPL_high_voltage_in_pu()-1.0)<DOUBLE_EPSILON);
    lvrt->set_LVPL_high_voltage_in_pu(0.9);
    TEST_ASSERT(fabs(lvrt->get_LVPL_high_voltage_in_pu()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_set_get_LVPL_gain_at_high_voltage()
{
    lvrt->set_LVPL_gain_at_high_voltage(1.0);
    TEST_ASSERT(fabs(lvrt->get_LVPL_gain_at_high_voltage()-1.0)<DOUBLE_EPSILON);
    lvrt->set_LVPL_gain_at_high_voltage(0.9);
    TEST_ASSERT(fabs(lvrt->get_LVPL_gain_at_high_voltage()-0.9)<DOUBLE_EPSILON);
}
void LVRT_CONTROL_TEST::test_check_vrt_status_basic()
{
    default_toolkit.set_dynamic_simulation_time_in_s(0.0);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    BUS* bus = lvrt->get_bus_pointer();
    bus->set_positive_sequence_voltage_in_pu(1.0);

    lvrt->set_vrt_voltage_threshold(0.9);
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.2);

    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.91);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.89);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.2);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);


    default_toolkit.set_dynamic_simulation_time_in_s(0.5);
    bus->set_positive_sequence_voltage_in_pu(0.95);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_PREPARE2RECOVER_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.69);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_PREPARE2RECOVER_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.71);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_RECOVER_STATUS);

    //check when timer delay is 0
    lvrt->set_vrt_status(VRT_NORMAL_STATUS);
    default_toolkit.set_dynamic_simulation_time_in_s(0.0);

    bus->set_positive_sequence_voltage_in_pu(1.0);

    lvrt->set_vrt_voltage_threshold(0.9);
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.);

    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.91);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.89);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.2);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);


    default_toolkit.set_dynamic_simulation_time_in_s(0.5);
    bus->set_positive_sequence_voltage_in_pu(0.95);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_RECOVER_STATUS);

    //check when voltage drop again during recover
    lvrt->set_vrt_status(VRT_NORMAL_STATUS);
    default_toolkit.set_dynamic_simulation_time_in_s(0.0);

    bus->set_positive_sequence_voltage_in_pu(1.0);

    lvrt->set_vrt_voltage_threshold(0.9);
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.2);

    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.91);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.89);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);


    default_toolkit.set_dynamic_simulation_time_in_s(0.5);
    bus->set_positive_sequence_voltage_in_pu(0.95);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_PREPARE2RECOVER_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.6);
    bus->set_positive_sequence_voltage_in_pu(0.8);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.7);
    bus->set_positive_sequence_voltage_in_pu(0.99);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_PREPARE2RECOVER_STATUS);

    default_toolkit.set_dynamic_simulation_time_in_s(0.91);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_RECOVER_STATUS);

    bus->set_positive_sequence_voltage_in_pu(0.8);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);
}

void LVRT_CONTROL_TEST::test_check_vrt_status_comprehensive()
{
    lvrt->set_vrt_voltage_threshold(0.9);
    lvrt->set_vrt_prepare2recover_delay_time_in_s(0.2);

    lvrt->set_P_vrt_during_mode(VRT_DURING_POWER_CONTROL_MODE);
    lvrt->set_kP(0.3);
    lvrt->set_P_set_in_pu(0.1);
    lvrt->set_k1_Ip(0);
    lvrt->set_k2_Ip(0);
    lvrt->set_Ip_set_in_pu(0);
    lvrt->set_LVPL_low_voltage_in_pu(0);
    lvrt->set_LVPL_high_voltage_in_pu(0);
    lvrt->set_LVPL_gain_at_high_voltage(0);

    lvrt->set_P_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE);
    lvrt->set_k_Ip_prepare2recover(0.2);
    lvrt->set_Ip_set_prepare2recover(0.1);

    lvrt->set_P_vrt_recover_mode(VRT_RECOVER_LINEAR_CONTROL_MODE);
    lvrt->set_P_recover_slope_in_pu_per_s(2.0);
    lvrt->set_P_recover_inertia_in_s(0);

    lvrt->set_Q_vrt_during_mode(VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE);
    lvrt->set_kQ(0);
    lvrt->set_Q_set_in_pu(0);
    lvrt->set_k1_Iq(0.1);
    lvrt->set_k2_Iq(0.2);
    lvrt->set_Iq_set_in_pu(0.3);

    lvrt->set_Q_vrt_prepare2recover_mode(VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE);
    lvrt->set_k_Iq_prepare2recover(0.2);
    lvrt->set_Iq_set_prepare2recover(0.15);

    lvrt->set_Q_vrt_recover_mode(VRT_RECOVER_EXPOENTIAL_CONTROL_MODE);
    lvrt->set_Q_recover_slope_in_pu_per_s(0);
    lvrt->set_Q_recover_inertia_in_s(0.5);

    default_toolkit.set_dynamic_simulation_time_in_s(0.0);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);

    BUS* bus = lvrt->get_bus_pointer();
    bus->set_positive_sequence_voltage_in_pu(1.0);
    lvrt->update_PQI0_in_vrt_normal_status(1.0, 1.1, 0.5, 0.6);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_NORMAL_STATUS);


    default_toolkit.set_dynamic_simulation_time_in_s(1.0);
    bus->set_positive_sequence_voltage_in_pu(0.6);
    lvrt->check_vrt_status();
    TEST_ASSERT(lvrt->get_vrt_status()==VRT_DURING_STATUS);

    double Pcmd = lvrt->get_vrt_P_command();
    double Iqcmd = lvrt->get_vrt_Iq_command();

    TEST_ASSERT(fabs(Pcmd-(0.3*1.0+0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(Iqcmd-(0.1*(0.9-0.6)+0.2*0.6+0.3))<FLOAT_EPSILON);

    lvrt->update_PQI0_in_vrt_prepare2recover_status(0.5, 0.7, 0.3, 0.4);
}

#endif
