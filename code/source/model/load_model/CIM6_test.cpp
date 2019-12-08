#include "header/basic/test_macro.h"
#include "header/model/load_model/CIM6_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
CIM6_TEST::CIM6_TEST() : LOAD_MODEL_TEST()
{
    ;
}



void CIM6_TEST::setup()
{
    LOAD_MODEL_TEST::setup();

    CIM6 model;
    model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    model.set_Ra_in_pu(0.5);

    LOAD* load = get_load();
    load->set_model(&model);
}

void CIM6_TEST::tear_down()
{
    LOAD_MODEL_TEST::tear_down();
}

void CIM6_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CIM6_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    TEST_ASSERT(model->get_model_name()=="CIM6");
}

void CIM6_TEST::test_get_detailed_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CIM6_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    model->set_subsystem_type(BUS_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="CIM6BL");

    model->set_subsystem_type(AREA_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="CIM6AR");

    model->set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="CIM6ZN");

    model->set_subsystem_type(OWNER_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="CIM6OW");

    model->set_subsystem_type(ALL_SYSTEM_TYPE);
    TEST_ASSERT(model->get_detailed_model_name()=="CIM6AL");

}
void CIM6_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CIM6_TEST");

    LOAD* load = get_load();
    CIM6* model = (CIM6*) load->get_load_model();

    model->set_Ra_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_Ra_in_pu()-0.5)<FLOAT_EPSILON);

}


#endif
