#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/device/generator_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/model/sg_models/turbine_governor_model/TGOV1.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;

GENERATOR_TEST::GENERATOR_TEST()
{
    TEST_ADD(GENERATOR_TEST::test_constructor);

    TEST_ADD(GENERATOR_TEST::test_set_get_generator_bus);
    TEST_ADD(GENERATOR_TEST::test_set_get_generator_impedance);
    TEST_ADD(GENERATOR_TEST::test_copy_with_operator_equal);
    TEST_ADD(GENERATOR_TEST::test_get_device_id);

    TEST_ADD(GENERATOR_TEST::test_set_get_sync_generator_model);
    TEST_ADD(GENERATOR_TEST::test_set_get_compensator_model);
    TEST_ADD(GENERATOR_TEST::test_set_get_exciter_model);
    TEST_ADD(GENERATOR_TEST::test_set_get_stabilizer_model);
    TEST_ADD(GENERATOR_TEST::test_set_get_turbine_governor_model);
}

void GENERATOR_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_system_base_power_in_MVA(100.0);
    db->set_allowed_max_bus_number(100);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(35.0);
    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    db->append_bus(bus);

    generator = new GENERATOR(db);
}

void GENERATOR_TEST::tear_down()
{
    delete generator;
    generator = NULL;

    delete db;
    db = NULL;

    show_test_end_information();
}

void GENERATOR_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    TEST_ASSERT(generator->get_generator_bus()==0);
    TEST_ASSERT(generator->get_identifier()=="");
    TEST_ASSERT(generator->get_status()==false);
    TEST_ASSERT(generator->get_mbase_in_MVA()==db->get_system_base_power_in_MVA());
    TEST_ASSERT(generator->get_p_generation_in_MW()==0.0);
    TEST_ASSERT(generator->get_q_generation_in_MVar()==0.0);
    TEST_ASSERT(generator->get_p_max_in_MW()==0.0);
    TEST_ASSERT(generator->get_p_min_in_MW()==0.0);
    TEST_ASSERT(generator->get_q_max_in_MVar()==0.0);
    TEST_ASSERT(generator->get_q_min_in_MVar()==0.0);
    TEST_ASSERT(generator->get_voltage_to_regulate_in_pu()==1.0);
    TEST_ASSERT(generator->get_bus_to_regulate()==0);
    TEST_ASSERT(generator->get_generator_impedance_in_pu()==0.0);
}

void GENERATOR_TEST::test_set_get_generator_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    TEST_ASSERT(generator->get_generator_bus()==1);
    generator->set_generator_bus(2);
    TEST_ASSERT(generator->get_generator_bus()==2);
    generator->set_generator_bus(0);
    TEST_ASSERT(generator->get_generator_bus()==0);
}
void GENERATOR_TEST::test_set_get_generator_impedance()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    complex<double> z(0.0, 0.01);
    generator->set_generator_impedance_in_pu(z);
    TEST_ASSERT(generator->get_generator_impedance_in_pu()==z);
}



void GENERATOR_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);
    generator->set_mbase_in_MVA(120.0);

    GENERATOR newgenerator = (*generator);

    TEST_ASSERT(newgenerator.get_generator_bus()==1);
    TEST_ASSERT(newgenerator.get_identifier()=="1#");
    TEST_ASSERT(newgenerator.get_status()==true);
    TEST_ASSERT(newgenerator.get_mbase_in_MVA()==120.0);
}

void GENERATOR_TEST::test_get_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");

    DEVICE_ID did = generator->get_device_id();

    DEVICE_ID did2;
    did2.set_device_type("GENERATOR");
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


void GENERATOR_TEST::test_set_get_sync_generator_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    db->append_generator(*generator);

    DEVICE_ID did = generator->get_device_id();

    GENERATOR* gen = db->get_generator(did);

    GENCLS model;

    model.set_Rs(0.0);
    model.set_Xdp(0.1);
    model.set_Tj_in_s(6.0);
    model.set_D(0.0);

    gen->set_model(&model);

    SYNC_GENERATOR_MODEL* modelptr = gen->get_sync_generator_model();
    TEST_ASSERT(modelptr!=NULL);

    TEST_ASSERT(fabs(modelptr->get_Rs()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(modelptr->get_Xdp()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(modelptr->get_Tj_in_s()-6.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(modelptr->get_D()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(modelptr->get_power_system_database()==db);
}
void GENERATOR_TEST::test_set_get_compensator_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    db->append_generator(*generator);

    DEVICE_ID did = generator->get_device_id();

    GENERATOR* gen = db->get_generator(did);

    COMP model;

    model.set_Xe(0.1);

    gen->set_model(&model);

    COMPENSATOR_MODEL* modelptr = gen->get_compensator_model();
    TEST_ASSERT(modelptr!=NULL);

    COMP* smodelptr = (COMP*) modelptr;

    TEST_ASSERT(fabs(smodelptr->get_Xe()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(modelptr->get_power_system_database()==db);
}
void GENERATOR_TEST::test_set_get_exciter_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    db->append_generator(*generator);

    DEVICE_ID did = generator->get_device_id();

    GENERATOR* gen = db->get_generator(did);

    SEXS model;

    model.set_TA_in_s(0.1);
    model.set_TB_in_s(0.3);
    model.set_K(5);
    model.set_TE_in_s(1.0);
    model.set_Efdmax_in_pu(10.0);
    model.set_Efdmin_in_pu(2.0);

    gen->set_model(&model);

    EXCITER_MODEL* modelptr = gen->get_exciter_model();
    TEST_ASSERT(modelptr!=NULL);

    SEXS* smodelptr = (SEXS*) modelptr;

    TEST_ASSERT(fabs(smodelptr->get_TA_in_s()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_TB_in_s()-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_K()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_TE_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_Efdmax_in_pu()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_Efdmin_in_pu()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(modelptr->get_power_system_database()==db);
}
void GENERATOR_TEST::test_set_get_stabilizer_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    db->append_generator(*generator);

    DEVICE_ID did = generator->get_device_id();

    GENERATOR* gen = db->get_generator(did);

    IEE2ST model;

    model.set_K1(1.0);
    model.set_K2(2.0);
    model.set_T1_in_s(1.0);
    model.set_T2_in_s(2.0);
    model.set_T3_in_s(3.0);
    model.set_T4_in_s(4.0);
    model.set_T5_in_s(5.0);
    model.set_T6_in_s(6.0);
    model.set_T7_in_s(7.0);
    model.set_T8_in_s(8.0);
    model.set_T9_in_s(9.0);
    model.set_T10_in_s(10.0);


    gen->set_model(&model);

    STABILIZER_MODEL* modelptr = gen->get_stabilizer_model();
    TEST_ASSERT(modelptr!=NULL);

    IEE2ST* smodelptr = (IEE2ST*) modelptr;

    TEST_ASSERT(fabs(smodelptr->get_K1()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_K2()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T1_in_s()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T2_in_s()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T3_in_s()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T4_in_s()-4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T5_in_s()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T6_in_s()-6.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T7_in_s()-7.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T8_in_s()-8.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T9_in_s()-9.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T10_in_s()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(modelptr->get_power_system_database()==db);
}
void GENERATOR_TEST::test_set_get_turbine_governor_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"GENERATOR_TEST");

    generator->set_generator_bus(1);
    generator->set_identifier("1#");
    generator->set_status(true);

    db->append_generator(*generator);

    DEVICE_ID did = generator->get_device_id();

    GENERATOR* gen = db->get_generator(did);

    TGOV1 model;

    model.set_R(0.5);
    model.set_D(2.0);
    model.set_T1_in_s(3.0);
    model.set_T2_in_s(4.0);
    model.set_T3_in_s(5.0);
    model.set_Valvemax_in_pu(1.0);
    model.set_Valvemin_in_pu(0.2);

    gen->set_model(&model);

    TURBINE_GOVERNOR_MODEL* modelptr = gen->get_turbine_governor_model();
    TEST_ASSERT(modelptr!=NULL);

    TGOV1* smodelptr = (TGOV1*) modelptr;

    TEST_ASSERT(fabs(smodelptr->get_R()-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_D()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T1_in_s()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T2_in_s()-4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_T3_in_s()-5.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_Valvemax_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(smodelptr->get_Valvemin_in_pu()-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(modelptr->get_power_system_database()==db);
}
