#ifndef PUFLS_TEST_H
#define PUFLS_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/load_relay_model/load_frequency_relay_model_test.h"
#include "header/model/load_relay_model/PUFLS.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class PUFLS_TEST : public LOAD_FREQUENCY_RELAY_MODEL_TEST
{
    public:
        PUFLS_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_model_name();
        virtual void test_set_get_parameters();
        void test_continuous_load_relay_scheme_with_realtime_frequency_additional_stage();
        void test_continuous_load_relay_scheme_with_minimum_frequency_additional_stage();
        void test_discrete_load_relay_scheme_with_realtime_frequency_additional_stage();
        void test_discrete_load_relay_scheme_with_minimum_frequency_additional_stage();
        void test_composite_load_relay_scheme_with_realtime_frequency_additional_stage();
        void test_composite_load_relay_scheme_with_minimum_frequency_additional_stage();
    private:

};

#endif//PUFLS_TEST_H
