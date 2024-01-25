#ifndef WT_GENERATOR_MODEL_TEST_H
#define WT_GENERATOR_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/vsg_model/vsg_model_test.h"

#include "header/model/vsg_model/vsgcontrol0.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class VSGCONTROL0_TEST : public VSG_MODEL_TEST
{
    public:
        VSGCONTROL0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        VSGCONTROL0* get_model();

        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
    private:

};

#endif//WT_GENERATOR_MODEL_TEST_H
