#include "header/basic/test_macro.h"
#include "header/model/load_model/IEEL_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
IEEL_TEST::IEEL_TEST() : LOAD_MODEL_TEST()
{
    ;
}



void IEEL_TEST::setup()
{
    LOAD_MODEL_TEST::setup();

    LOAD* load = get_load();

    IEEL model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(load->get_device_id());

    model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    model.set_P_alpha_1(0.5);
    model.set_P_alpha_2(0.4);
    model.set_P_alpha_3(0.1);
    model.set_Q_alpha_1(0.6);
    model.set_Q_alpha_2(0.2);
    model.set_Q_alpha_3(0.2);
    model.set_P_n_power_1(2.0);
    model.set_P_n_power_2(1.0);
    model.set_P_n_power_3(0.0);
    model.set_Q_n_power_1(2.0);
    model.set_Q_n_power_2(1.0);
    model.set_Q_n_power_3(0.0);
    model.set_P_Kf(2.0);
    model.set_Q_Kf(-1.8);

    dmdb.add_model(&model);
}

void IEEL_TEST::tear_down()
{
    LOAD_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
}

void IEEL_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    TEST_ASSERT(model->get_model_name()=="IEEL");
}

void IEEL_TEST::test_get_detailed_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    model->set_subsystem_type(BUS_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="IEELBL");

    model->set_subsystem_type(AREA_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="IEELAR");

    model->set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="IEELZN");

    model->set_subsystem_type(OWNER_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="IEELOW");

    model->set_subsystem_type(ALL_SYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="IEELAL");

}
void IEEL_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"IEEL_TEST");

    LOAD* load = get_load();
    IEEL* model = (IEEL*) load->get_load_model();

    model->set_P_alpha_1(0.5);
    TEST_ASSERT(fabs(model->get_P_alpha_1()-0.5)<FLOAT_EPSILON);

    model->set_P_alpha_2(0.4);
    TEST_ASSERT(fabs(model->get_P_alpha_2()-0.4)<FLOAT_EPSILON);

    model->set_P_alpha_3(0.3);
    TEST_ASSERT(fabs(model->get_P_alpha_3()-0.3)<FLOAT_EPSILON);

    model->set_Q_alpha_1(0.6);
    TEST_ASSERT(fabs(model->get_Q_alpha_1()-0.6)<FLOAT_EPSILON);

    model->set_Q_alpha_2(0.7);
    TEST_ASSERT(fabs(model->get_Q_alpha_2()-0.7)<FLOAT_EPSILON);

    model->set_Q_alpha_3(0.8);
    TEST_ASSERT(fabs(model->get_Q_alpha_3()-0.8)<FLOAT_EPSILON);

    model->set_P_n_power_1(1.0);
    TEST_ASSERT(fabs(model->get_P_n_power_1()-1.0)<FLOAT_EPSILON);

    model->set_P_n_power_2(2.0);
    TEST_ASSERT(fabs(model->get_P_n_power_2()-2.0)<FLOAT_EPSILON);

    model->set_P_n_power_3(0.0);
    TEST_ASSERT(fabs(model->get_P_n_power_3()-0.0)<FLOAT_EPSILON);

    model->set_Q_n_power_1(6.0);
    TEST_ASSERT(fabs(model->get_Q_n_power_1()-6.0)<FLOAT_EPSILON);

    model->set_Q_n_power_2(5.0);
    TEST_ASSERT(fabs(model->get_Q_n_power_2()-5.0)<FLOAT_EPSILON);

    model->set_Q_n_power_3(4.0);
    TEST_ASSERT(fabs(model->get_Q_n_power_3()-4.0)<FLOAT_EPSILON);

    model->set_P_Kf(2.0);
    TEST_ASSERT(fabs(model->get_P_Kf()-2.0)<FLOAT_EPSILON);

    model->set_Q_Kf(-2.0);
    TEST_ASSERT(fabs(model->get_Q_Kf()-(-2.0))<FLOAT_EPSILON);
}


#endif
