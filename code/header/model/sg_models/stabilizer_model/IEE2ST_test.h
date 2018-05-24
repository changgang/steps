#ifndef IEE2ST_TEST_H
#define IEE2ST_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/stabilizer_model/stabilizer_model_test.h"

#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/power_system_database.h"
using namespace std;

class IEE2ST_TEST : public STABILIZER_MODEL_TEST
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
};

#endif//IEE2ST_TEST_H
