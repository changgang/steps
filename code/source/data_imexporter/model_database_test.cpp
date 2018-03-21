#include "header/model_database_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/source_model/sync_generator_model/gencls.h"
#include "header/model/stabilizer_model/IEE2ST.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

MODEL_DATABASE_TEST::MODEL_DATABASE_TEST()
{
    TEST_ADD(MODEL_DATABASE_TEST::test_constructor);
    TEST_ADD(MODEL_DATABASE_TEST::test_append_and_get_model);
    TEST_ADD(MODEL_DATABASE_TEST::test_get_model_table_and_valid_model_count);
    TEST_ADD(MODEL_DATABASE_TEST::test_all_models_initialize_flag);
    TEST_ADD(MODEL_DATABASE_TEST::test_all_models_update_flag);
}

void MODEL_DATABASE_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_system_name("TEST SYSTEM");
    db->set_allowed_max_bus_number(100);
    db->set_system_base_frequency_in_Hz(50.0);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    for(size_t bus_no=1; bus_no<=10; bus_no++)
    {
        bus.set_bus_number(bus_no);
        bus.set_base_voltage_in_kV(21.0);
        bus.set_bus_type(PV_TYPE);
        db->append_bus(bus);
    }
    GENERATOR generator(db);
    for(size_t bus_no=1; bus_no<=10; bus_no++)
    {
        generator.set_generator_bus(bus_no);
        generator.set_identifier("#1");
        generator.set_status(true);
        db->append_generator(generator);
    }
    STEPS::model_db.clear_database();
}

void MODEL_DATABASE_TEST::tear_down()
{
    delete db;
    STEPS::model_db.clear_database();
}
void MODEL_DATABASE_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==0);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==0);
}

void MODEL_DATABASE_TEST::test_append_and_get_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);

    STEPS::model_db.append_model(gen_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==1);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==1);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(0)->get_model_name()=="GENCLS");


    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==2);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==2);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(0)->get_model_name()=="GENCLS");
    TEST_ASSERT(STEPS::model_db.get_model_of_index(1)->get_model_name()=="IEE2ST");

    GENERATOR* generator = db->get_generator(did);
    TEST_ASSERT(generator->get_sync_generator_model_index()==0);
    TEST_ASSERT(generator->get_stabilizer_model_index()==1);
    TEST_ASSERT(generator->get_sync_generator_model()==gen_model);
    TEST_ASSERT(generator->get_stabilizer_model()==sta_model);
}

void MODEL_DATABASE_TEST::test_get_model_table_and_valid_model_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    GENERATOR* generator = db->get_generator(did);

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);

    STEPS::model_db.append_model(gen_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==1);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==1);
    TEST_ASSERT(generator->get_sync_generator_model()==gen_model);

    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==2);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==2);
    TEST_ASSERT(generator->get_stabilizer_model()==sta_model);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==3);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==2);
    TEST_ASSERT(generator->get_stabilizer_model()==sta_model);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==2);
    TEST_ASSERT(generator->get_stabilizer_model()==sta_model);

    STEPS::model_db.delete_model_of_index(3);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==1);
    TEST_ASSERT(generator->get_stabilizer_model()==NULL);
}

void MODEL_DATABASE_TEST::test_clear_model_database()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);

    STEPS::model_db.append_model(gen_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==1);

    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==2);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==3);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);

    STEPS::model_db.clear_database();
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==0);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==0);
}

void MODEL_DATABASE_TEST::test_delete_model_of_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);

    STEPS::model_db.append_model(gen_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==1);

    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==2);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==3);

    sta_model= new IEE2ST(db, did);

    STEPS::model_db.append_model(sta_model);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);

    STEPS::model_db.delete_model_of_index(0);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==3);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(0)==NULL);

    STEPS::model_db.delete_model_of_index(1);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==2);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(1)==NULL);

    STEPS::model_db.delete_model_of_index(2);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==1);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(2)==NULL);

    STEPS::model_db.delete_model_of_index(3);
    TEST_ASSERT(STEPS::model_db.get_model_table_count()==4);
    TEST_ASSERT(STEPS::model_db.get_valid_model_count()==0);
    TEST_ASSERT(STEPS::model_db.get_model_of_index(3)==NULL);
}

void MODEL_DATABASE_TEST::test_all_models_initialize_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);
    STEPS::model_db.append_model(gen_model);

    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);
    STEPS::model_db.append_model(sta_model);

    STEPS::model_db.set_all_models_initialize_flag_as_false();
    for(size_t i=0; i<2; i++)
        TEST_ASSERT(STEPS::model_db.get_model_of_index(i)->is_model_initialized()==false);

    STEPS::model_db.set_all_models_initialize_flag_as_true();
    for(size_t i=0; i<2; i++)
        TEST_ASSERT(STEPS::model_db.get_model_of_index(i)->is_model_initialized()==true);
}

void MODEL_DATABASE_TEST::test_all_models_update_flag()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    SYNC_GENERATOR_MODEL* gen_model = new GENCLS(db, did);
    STEPS::model_db.append_model(gen_model);

    STABILIZER_MODEL* sta_model= new IEE2ST(db, did);
    STEPS::model_db.append_model(sta_model);

    STEPS::model_db.set_all_models_update_flag_as_false();
    for(size_t i=0; i<2; i++)
        TEST_ASSERT(STEPS::model_db.get_model_of_index(i)->is_model_updated()==false);

    STEPS::model_db.set_all_models_update_flag_as_true();
    for(size_t i=0; i<2; i++)
        TEST_ASSERT(STEPS::model_db.get_model_of_index(i)->is_model_updated()==true);
}


void MODEL_DATABASE_TEST::test_append_sync_generator_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_exciter_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_stabilizer_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_turbine_governor_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_wind_turbine_generator_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_wind_electrical_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_wind_mechanical_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_wind_pitch_control_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_load_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_load_frequency_relay_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_load_voltage_relay_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_hvdc_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
void MODEL_DATABASE_TEST::test_append_line_relay_model()
{
    show_test_information_for_function_of_class(__FUNCTION__,"MODEL_DATABASE_TEST");
}
