#include "header/basic/test_macro.h"
#include "header/model/vsg_model/vsg_model_test.h"
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

VSG_MODEL_TEST::VSG_MODEL_TEST()
{
    TEST_ADD(test_get_mbase_in_MVA);
    TEST_ADD(test_get_one_over_mbase_in_one_over_MVA);

    TEST_ADD(test_get_terminal_complex_power_in_pu_based_on_mbase);
    TEST_ADD(test_get_terminal_voltage_in_pu);
    TEST_ADD(test_get_terminal_complex_voltage_in_pu);
    test_get_terminal_voltage_angle_in_rad);
    TEST_ADD(test_get_bus_base_frequency_in_Hz);
    TEST_ADD(test_get_bus_base_angle_speed_in_radps);
    TEST_ADD(test_get_source_impedance_in_pu_based_on_mbase);

    TEST_ADD(test_set_get_Pref_in_pu_based_on_mbase);
    TEST_ADD(test_set_get_Qref_in_pu_based_on_mbase);
    TEST_ADD(test_set_get_Vref_in_pu);

    TEST_ADD(test_get_virtual_speed_in_pu);

    TEST_ADD(test_active_power_step_response_of_vsg_model);
    TEST_ADD(test_reactive_power_step_response_of_vsg_model);
    TEST_ADD(test_voltage_step_response_of_vsg_model);
}

void VSG_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database);
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


    WT_GENERATOR* wt_gen = get_test_wt_generator();

    wtg_model = new WT3G0;
    wtg_model->set_toolkit(default_toolkit);
    wtg_model->set_device_id(wt_gen->get_device_id());

    wt_gen->set_number_of_lumped_wt_generators(50);
    wt_gen->set_rated_power_per_wt_generator_in_MW(2.0);
    wtg_model->set_converter_activer_current_command_T_in_s(0.2);
    wtg_model->set_converter_reactiver_voltage_command_T_in_s(0.2);
    wtg_model->set_KPLL(20.0);
    wtg_model->set_KIPLL(10.0);
    wtg_model->set_PLLmax(0.1);
    wtg_model->set_PLLmin(-0.1);
    LVPL lvpl;
    lvpl.set_low_voltage_in_pu(0.5);
    lvpl.set_high_voltage_in_pu(0.8);
    lvpl.set_gain_at_high_voltage(20.0);
    wtg_model->set_LVPL(lvpl);
    wtg_model->set_HVRC_voltage_in_pu(0.8);
    wtg_model->set_HVRC_current_in_pu(20.0);
    wtg_model->set_LVPL_max_rate_of_active_current_change(0.2);
    wtg_model->set_LVPL_voltage_sensor_T_in_s(0.1);

    wt_gen->set_wt_generator_model(wtg_model);
}

void VSG_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    delete wtg_model;
}

WT_GENERATOR* VSG_MODEL_TEST::get_test_wt_generator()
{
    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_index_of_string("#1"));

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    return psdb.get_wt_generator(did);
}

VSG_MODEL* VSG_MODEL_TEST::get_test_model()
{
    return model;
}

void VSG_MODEL_TEST::set_test_model(VSG_MODEL* model)
{
    this->model = model;
}

void VSG_MODEL_TEST::test_get_mbase_in_MVA()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    genptr->set_mbase_in_MVA(23.4);

    TEST_ASSERT(fabs(model->get_mbase_in_MVA()-23.4)<FLOAT_EPSILON);
}

void VSG_MODEL_TEST::test_get_one_over_mbase_in_one_over_MVA()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    genptr->set_mbase_in_MVA(23.4);

    TEST_ASSERT(fabs(model->get_one_over_mbase_in_one_over_MVA()-1.0/23.4)<FLOAT_EPSILON);
}


void VSG_MODEL_TEST::test_get_terminal_complex_power_in_pu_based_on_mbase()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    genptr->set_mbase_in_MVA(23.4);
    genptr->set_p_generation_in_MW()

    TEST_ASSERT(fabs(model->get_terminal_complex_power_in_pu_based_on_mbase()-23.4)<FLOAT_EPSILON);
}

void VSG_MODEL_TEST::test_get_terminal_voltage_in_pu()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_voltage_in_pu(1.05);

    TEST_ASSERT(fabs(model->get_terminal_voltage_in_pu()-1.05)<FLOAT_EPSILON);
}

void VSG_MODEL_TEST::test_get_terminal_complex_voltage_in_pu()
{

}
void VSG_MODEL_TEST::test_get_terminal_voltage_angle_in_rad()
{

}
void VSG_MODEL_TEST::test_get_bus_base_frequency_in_Hz()
{

}
void VSG_MODEL_TEST::test_get_bus_base_angle_speed_in_radps()
{

}
void VSG_MODEL_TEST::test_get_source_impedance_in_pu_based_on_mbase()
{

}

void VSG_MODEL_TEST::test_set_get_Pref_in_pu_based_on_mbase()
{

}
void VSG_MODEL_TEST::test_set_get_Qref_in_pu_based_on_mbase()
{

}
void VSG_MODEL_TEST::test_set_get_Vref_in_pu()
{

}

void VSG_MODEL_TEST::test_get_virtual_speed_in_pu()
{

}

void VSG_MODEL_TEST::test_active_power_step_response_of_vsg_model()
{

}
void VSG_MODEL_TEST::test_reactive_power_step_response_of_vsg_model()
{

}
void VSG_MODEL_TEST::test_voltage_step_response_of_vsg_model()
{

}

void VSG_MODEL_TEST::run_a_step()
{

}
void VSG_MODEL_TEST::export_meter_title()
{

}
void VSG_MODEL_TEST::export_meter_values(double time)
{

}

#endif
