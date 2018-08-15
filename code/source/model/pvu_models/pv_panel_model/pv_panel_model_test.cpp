#include "header/model/pvu_models/pv_panel_model/pv_panel_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

PV_PANEL_MODEL_TEST::PV_PANEL_MODEL_TEST()
{
    TEST_ADD(PV_PANEL_MODEL_TEST::test_get_model_type);

    TEST_ADD(PV_PANEL_MODEL_TEST::test_get_standard_model_string);
}

void PV_PANEL_MODEL_TEST::setup()
{
    PVU_MODEL_TEST::setup();

    PV_UNIT* pv_unit = get_test_pv_unit();
    pv_unit->set_p_generation_in_MW(28.0);
    pv_unit->set_rated_power_per_pv_unit_in_MW(1.5);
    pv_unit->set_number_of_lumped_pv_units(20);

    //pv_unit->set_model(&genmodel);
}

void PV_PANEL_MODEL_TEST::tear_down()
{
    PVU_MODEL_TEST::tear_down();
}

void PV_PANEL_MODEL_TEST::test_get_model_type()
{
    PV_PANEL_MODEL* model = get_test_pv_panel_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="WT AERODYNAMIC");
    }
    else
        TEST_ASSERT(false);
}

void PV_PANEL_MODEL_TEST::test_get_standard_model_string()
{
    PV_PANEL_MODEL* model = get_test_pv_panel_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->get_standard_model_string();
    }
    else
        TEST_ASSERT(false);
}

