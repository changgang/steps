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

    LOAD* load = get_load();

    CIM6 model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(load->get_device_id());

    model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    model.set_motor_type(1);
    model.set_Ra_in_pu(0.01);
    model.set_Xa_in_pu(0.09);
    model.set_Xm_in_pu(4.2);
    model.set_R1_in_pu(0.035);
    model.set_X1_in_pu(0.09);
    model.set_R2_in_pu(0.012);
    model.set_X2_in_pu(0.04);
    model.set_Mbase_in_MVA(0.0);
    model.set_Pmult(1.5);
    model.set_H_in_s(1.5);
    model.set_A(0.0);
    model.set_B(0.1);
    model.set_C(1.0);
    model.set_D(0.0);
    model.set_E(0.5);

    dmdb.add_model(&model);
}

void CIM6_TEST::tear_down()
{
    LOAD_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();
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

    model->set_motor_type(1);
    TEST_ASSERT(model->get_motor_type()==1);
    model->set_motor_type(2);
    TEST_ASSERT(model->get_motor_type()==2);

    model->set_Ra_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_Ra_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_Ra_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_Ra_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_Xa_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_Xa_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_Xa_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_Xa_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_Xm_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_Xm_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_Xm_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_Xm_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_R1_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_R1_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_R1_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_R1_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_X1_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_X1_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_X1_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_X1_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_R2_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_R2_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_R2_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_R2_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_X2_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_X2_in_pu()-0.5)<FLOAT_EPSILON);
    model->set_X2_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_X2_in_pu()-1.0)<FLOAT_EPSILON);

    model->set_Mbase_in_MVA(0.5);
    TEST_ASSERT(fabs(model->get_Mbase_in_MVA()-0.5)<FLOAT_EPSILON);
    model->set_Mbase_in_MVA(1.0);
    TEST_ASSERT(fabs(model->get_Mbase_in_MVA()-1.0)<FLOAT_EPSILON);

    model->set_Pmult(0.5);
    TEST_ASSERT(fabs(model->get_Pmult()-0.5)<FLOAT_EPSILON);
    model->set_Pmult(1.0);
    TEST_ASSERT(fabs(model->get_Pmult()-1.0)<FLOAT_EPSILON);

    model->set_H_in_s(0.5);
    TEST_ASSERT(fabs(model->get_H_in_s()-0.5)<FLOAT_EPSILON);
    model->set_H_in_s(1.0);
    TEST_ASSERT(fabs(model->get_H_in_s()-1.0)<FLOAT_EPSILON);

    model->set_A(0.5);
    TEST_ASSERT(fabs(model->get_A()-0.5)<FLOAT_EPSILON);
    model->set_A(1.0);
    TEST_ASSERT(fabs(model->get_A()-1.0)<FLOAT_EPSILON);

    model->set_B(0.5);
    TEST_ASSERT(fabs(model->get_B()-0.5)<FLOAT_EPSILON);
    model->set_B(1.0);
    TEST_ASSERT(fabs(model->get_B()-1.0)<FLOAT_EPSILON);

    model->set_C(0.5);
    TEST_ASSERT(fabs(model->get_C()-0.5)<FLOAT_EPSILON);
    model->set_C(1.0);
    TEST_ASSERT(fabs(model->get_C()-1.0)<FLOAT_EPSILON);

    model->set_D(0.5);
    TEST_ASSERT(fabs(model->get_D()-0.5)<FLOAT_EPSILON);
    model->set_D(1.0);
    TEST_ASSERT(fabs(model->get_D()-1.0)<FLOAT_EPSILON);

    model->set_E(0.5);
    TEST_ASSERT(fabs(model->get_E()-0.5)<FLOAT_EPSILON);
    model->set_E(1.0);
    TEST_ASSERT(fabs(model->get_E()-1.0)<FLOAT_EPSILON);
}


#endif
