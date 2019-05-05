#ifndef PVP0_TEST_H
#define PVP0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pv_panel_model/pv_panel_model_test.h"

#include "header/model/pvu_models/pv_panel_model/pvp0.h"
#include "header/STEPS.h"

using namespace std;

class PVP0_TEST : public PV_PANEL_MODEL_TEST
{
    public:
        PVP0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
    private:

};

#endif//PVP0_TEST_H
