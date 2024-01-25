#ifndef VSGCONTROL2_TEST_H
#define VSGCONTROL2_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/vsg_model/vsg_model_test.h"

#include "header/model/vsg_model/vsgcontrol2.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class VSGCONTROL2_TEST : public VSG_MODEL_TEST
{
    public:
        VSGCONTROL2_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        VSGCONTROL2* get_model();

        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
    private:

};

#endif//VSGCONTROL2_TEST_H
