#ifndef UVLS_TEST_H
#define UVLS_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/load_relay_model/load_voltage_relay_model_test.h"
#include "header/model/load_relay_model/UVLS.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class UVLS_TEST : public LOAD_VOLTAGE_RELAY_MODEL_TEST
{
    public:
        UVLS_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_model_name();
        virtual void test_set_get_parameters();
    private:

};

#endif//UVLS_TEST_H
