#include "header/model/energy_storage_model/estr0_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

ESTR0_TEST::ESTR0_TEST() : ENERGY_STORAGE_MODEL_TEST()
{
    TEST_ADD(ESTR0_TEST::test_get_model_name);
    TEST_ADD(ESTR0_TEST::test_set_get_parameters);

    //TEST_ADD(ESTR0_TEST::test_initialize_and_get_initialized_inputs);
}

void ESTR0_TEST::setup()
{
    ENERGY_STORAGE_MODEL_TEST::setup();

    ESTR0 model;

    model.set_Tp1_in_s(0.01);
    model.set_Tp2_in_s(0.05);
    model.set_Tp3_in_s(0.1);
    model.set_Tp4_in_s(0.2);
    model.set_Kpp(1.7);
    model.set_Kip(1.7);
    model.set_Kdp(1.7);
    model.set_Tdp_in_s(0.2);
    model.set_Pmax_in_pu(1.2);
    model.set_Tr_in_s(0.2);
    model.set_Iacmax_in_pu(1.2);
    model.set_Kin_in_pu(0.95);
    model.set_Kout_in_pu(0.9);
    model.set_E0_in_pu(0.5);
    model.set_En_in_MWh(100.0);
    model.set_Tq1_in_s(0.1);
    model.set_Tq2_in_s(0.2);
    model.set_Tq3_in_s(0.1);
    model.set_Tq4_in_s(0.5);
    model.set_Kq(3.0);
    model.set_Dq(0.5);

    ENERGY_STORAGE* esptr = get_test_energy_storage();
    esptr->set_model(&model);
}

void ESTR0_TEST::tear_down()
{
    ENERGY_STORAGE_MODEL_TEST::tear_down();
    show_test_end_information();
}

void ESTR0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ESTR0_TEST");

    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();

    TEST_ASSERT(model->get_model_name()=="ESTR0");
}

void ESTR0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ESTR0_TEST");

    ESTR0* model = (ESTR0*) get_test_energy_storage_model();

    model->set_Tp1_in_s(0.01);
    TEST_ASSERT(fabs(model->get_Tp1_in_s()-0.01)<FLOAT_EPSILON);
    model->set_Tp2_in_s(0.05);
    TEST_ASSERT(fabs(model->get_Tp2_in_s()-0.05)<FLOAT_EPSILON);
    model->set_Tp3_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tp3_in_s()-0.1)<FLOAT_EPSILON);
    model->set_Tp4_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tp4_in_s()-0.2)<FLOAT_EPSILON);
    model->set_Kpp(1.7);
    TEST_ASSERT(fabs(model->get_Kpp()-1.7)<FLOAT_EPSILON);
    model->set_Kip(2.0);
    TEST_ASSERT(fabs(model->get_Kip()-1.7)<FLOAT_EPSILON);
    model->set_Kdp(1.5);
    TEST_ASSERT(fabs(model->get_Kdp()-1.5)<FLOAT_EPSILON);
    model->set_Tdp_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tdp_in_s()-0.2)<FLOAT_EPSILON);
    model->set_Pmax_in_pu(1.2);
    TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.2)<FLOAT_EPSILON);
    model->set_Tr_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tr_in_s()-0.2)<FLOAT_EPSILON);
    model->set_Iacmax_in_pu(1.2);
    TEST_ASSERT(fabs(model->get_Iacmax_in_pu()-1.2)<FLOAT_EPSILON);
    model->set_Kin_in_pu(0.95);
    TEST_ASSERT(fabs(model->get_Kin_in_pu()-0.95)<FLOAT_EPSILON);
    model->set_Kout_in_pu(0.9);
    TEST_ASSERT(fabs(model->get_Kout_in_pu()-0.9)<FLOAT_EPSILON);
    model->set_E0_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_E0_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_En_in_MWh(100.0);
    TEST_ASSERT(fabs(model->get_En_in_MWh()-100.0)<FLOAT_EPSILON);
    model->set_Tq1_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tq1_in_s()-0.1)<FLOAT_EPSILON);
    model->set_Tq2_in_s(0.2);
    TEST_ASSERT(fabs(model->get_Tq2_in_s()-0.2)<FLOAT_EPSILON);
    model->set_Tq3_in_s(0.1);
    TEST_ASSERT(fabs(model->get_Tq3_in_s()-0.1)<FLOAT_EPSILON);
    model->set_Tq4_in_s(0.5);
    TEST_ASSERT(fabs(model->get_Tq4_in_s()-0.5)<FLOAT_EPSILON);
    model->set_Kq(3.0);
    TEST_ASSERT(fabs(model->get_Kq()-3.0)<FLOAT_EPSILON);
    model->set_Dq(0.5);
    TEST_ASSERT(fabs(model->get_Dq()-0.5)<FLOAT_EPSILON);
}


void ESTR0_TEST::test_initialize_and_get_initialized_inputs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"ESTR0_TEST");
}
