#ifndef PVU_MODEL_TEST_H
#define PVU_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/power_system_database.h"

using namespace std;

class PVU_MODEL_TEST : public Test::Suite
{
    public:
        PVU_MODEL_TEST();
        PV_UNIT* get_test_pv_unit();
        PV_CONVERTER_MODEL* get_test_pv_converter_model();
        PV_PANEL_MODEL* get_test_pv_panel_model();
        PV_ELECTRICAL_MODEL* get_test_pv_electrical_model();
        PV_IRRADIANCE_MODEL* get_test_pv_irradiance_model();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
};

#endif//PVU_MODEL_TEST_H
