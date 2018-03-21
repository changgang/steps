#include "header/model/source_model/sync_generator_model/gensal_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

GENSAL_TEST::GENSAL_TEST()
{
    TEST_ADD(GENSAL_TEST::test_get_model_name);
    TEST_ADD(GENSAL_TEST::test_update_source_impedance);
    TEST_ADD(GENSAL_TEST::test_initialize_and_get_initialized_inputs);

}

void GENSAL_TEST::setup()
{
    SYNC_GENERATOR_MODEL_TEST::setup();

    GENSAL model;

    model.set_Rs(0.001);
    model.set_Xd(0.4);
    model.set_Xq(0.2);
    model.set_Xdp(0.15);
    model.set_Xpp(0.1);
    model.set_Xl(0.08);
    model.set_Td0p_in_s(9.0);
    model.set_Td0pp_in_s(0.04);
    model.set_Tq0pp_in_s(0.06);
    model.set_Tj_in_s(6.0);
    model.set_D(2.0);

    GENERATOR* genptr = get_generator();
    genptr->set_model(&model);
}

void GENSAL_TEST::tear_down()
{
    SYNC_GENERATOR_MODEL_TEST::tear_down();

    show_test_end_information();
}


void GENSAL_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENSAL_TEST");

    GENERATOR* genptr = get_generator();
    SYNC_GENERATOR_MODEL* model = genptr->get_sync_generator_model();

    TEST_ASSERT(model->get_model_name()=="GENSAL");
}


void GENSAL_TEST::test_update_source_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENSAL_TEST");

    GENERATOR* genptr = get_generator();
    SYNC_GENERATOR_MODEL* model = genptr->get_sync_generator_model();

    model->update_source_impedance();

    complex<double> Z = genptr->get_source_impedance_in_pu();

    TEST_ASSERT(fabs(model->get_Rs()-Z.real())<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_Xdpp()-Z.imag())<FLOAT_EPSILON);
}

void GENSAL_TEST::test_initialize_and_get_initialized_inputs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENSAL_TEST");

    GENERATOR* genptr = get_generator();

    POWER_SYSTEM_DATABASE* db = genptr->get_power_system_database();

    SYNC_GENERATOR_MODEL* model = genptr->get_sync_generator_model();

    complex<double> V=db->get_bus_complex_voltage_in_pu(1);

    complex<double> Z=genptr->get_source_impedance_in_pu();
    complex<double> S(genptr->get_p_generation_in_MW(), genptr->get_q_generation_in_MVar());
    double mbase = genptr->get_mbase_in_MVA();
    S = S/mbase;

    model->initialize();

    complex<double> I, E, SS;
    I = conj(S/V);
    E = V+I*Z;
    SS = S+abs(I)*abs(I)*Z;

    //TEST_ASSERT(fabs(model->get_rotor_angle_in_deg()-rad2deg(arg(E)))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_rotor_speed_deviation_in_pu()-0.0)<FLOAT_EPSILON);
    //TEST_ASSERT(fabs(model->get_initial_excitation_voltage_in_pu()-abs(E))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_initial_mechanical_power_in_pu_based_on_mbase()-SS.real())<FLOAT_EPSILON);
}

