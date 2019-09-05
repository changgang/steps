#ifndef PSASPVC_TEST_H
#define PSASPVC_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/sg_models/compensator_model/PSASPVC.h"
#include "header/model/sg_models/compensator_model/compensator_model_test.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class PSASPVC_TEST : public COMPENSATOR_MODEL_TEST
{
    public:
        PSASPVC_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_type();
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_step_response();
        virtual void test_get_standard_psse_string();
    private:
};

#endif//PSASPVC_TEST_H
