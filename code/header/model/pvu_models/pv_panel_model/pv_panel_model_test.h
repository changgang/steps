#ifndef PV_PANEL_MODEL_TEST_H
#define PV_PANEL_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pvu_model_test.h"
#include "header/power_system_database.h"

using namespace std;

class PV_PANEL_MODEL_TEST : public PVU_MODEL_TEST
{
    public:
        PV_PANEL_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();

        void test_get_standard_model_string();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
};

#endif//PV_PANEL_MODEL_TEST_H
