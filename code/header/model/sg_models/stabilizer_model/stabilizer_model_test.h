#ifndef STABILIZER_TEST_H
#define STABILIZER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"

#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/power_system_database.h"
using namespace std;

class STABILIZER_MODEL_TEST : public SG_MODEL_TEST
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
};

#endif//STABILIZER_TEST_H
