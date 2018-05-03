#ifndef SOURCE_MODEL_TEST_H
#define SOURCE_MODEL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/power_system_database.h"
using namespace std;

class SOURCE_MODEL_TEST : public Test::Suite
{
    public:
        SOURCE_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_mbase_in_MVA();
        void test_get_terminal_voltage();
    private:
        GENCLS* model;
        POWER_SYSTEM_DATABASE* db;
};

#endif//SOURCE_MODEL_TEST_H
