#include "header/basic/test_macro.h"
#include "header/model/pvu_models/pv_panel_model/pvpnlx_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

PVPNLX_TEST::PVPNLX_TEST() : PV_PANEL_MODEL_TEST()
{
    TEST_ADD(PVPNLX_TEST::test_get_model_name);
    TEST_ADD(PVPNLX_TEST::test_set_get_parameters);
}

void PVPNLX_TEST::setup()
{
    PV_PANEL_MODEL_TEST::setup();
}

void PVPNLX_TEST::tear_down()
{
    PV_PANEL_MODEL_TEST::tear_down();

    show_test_end_information();
}


void PVPNLX_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVPNLX_TEST");

    PV_PANEL_MODEL* model = get_test_pv_panel_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="PVPNLX");
    }
    else
        TEST_ASSERT(false);
}

void PVPNLX_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PVPNLX_TEST");

}

#endif
