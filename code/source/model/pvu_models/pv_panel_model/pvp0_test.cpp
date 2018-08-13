#include "header/model/pvu_models/pv_panel_model/pvp0_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

PVP0_TEST::PVP0_TEST() : PV_PANEL_MODEL_TEST()
{
    TEST_ADD(PVP0_TEST::test_get_model_name);
    TEST_ADD(PVP0_TEST::test_set_get_parameters);
}

void PVP0_TEST::setup()
{
    PV_PANEL_MODEL_TEST::setup();

    PV_UNIT* pv_unit = get_test_pv_unit();
}

void PVP0_TEST::tear_down()
{
    PV_PANEL_MODEL_TEST::tear_down();

    show_test_end_information();
}


void PVP0_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVP0_TEST");

    PV_PANEL_MODEL* model = get_test_pv_panel_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="PVP0");
    }
    else
        TEST_ASSERT(false);
}

void PVP0_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVP0_TEST");

}
