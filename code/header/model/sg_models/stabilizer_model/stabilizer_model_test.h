#ifndef STABILIZER_TEST_H
#define STABILIZER_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/power_system_database.h"
using namespace std;

class STABILIZER_MODEL_TEST : public Test::Suite
{
    public:
        STABILIZER_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_set_get_input_signal();
        void test_get_stabilizing_signal();
    private:
        IEE2ST* model;
        SEXS* exciter_model;
        POWER_SYSTEM_DATABASE* db;
};

#endif//STABILIZER_TEST_H
