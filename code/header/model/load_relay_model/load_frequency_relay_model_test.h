#ifndef LOAD_FREQUENCY_RELAY_MODEL_TEST_H
#define LOAD_FREQUENCY_RELAY_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/model/load_relay_model/load_relay_model_test.h"
#include "header/power_system_database.h"
using namespace std;

class LOAD_FREQUENCY_RELAY_MODEL_TEST : public LOAD_RELAY_MODEL_TEST
{
    public:
        LOAD_FREQUENCY_RELAY_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_model_type();
        void test_get_bus_frequency();
        void test_run();
        void run_model(string outputfile);

        virtual void test_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
    private:
        void export_meter_title();
        void export_meter_values(double time);
};

#endif//LOAD_FREQUENCY_RELAY_MODEL_TEST_H
