#ifndef IEE2ST_TEST_H
#define IEE2ST_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/stabilizer_model/IEE2ST.h"
#include "header/power_system_database.h"
using namespace std;

class IEE2ST_TEST : public Test::Suite
{
    public:
        IEE2ST_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_name();
        void test_set_get_parameters();
        void test_set_get_input_signals();
    private:
        IEE2ST* model;
        POWER_SYSTEM_DATABASE* db;
};

#endif//IEE2ST_TEST_H
