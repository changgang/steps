#ifndef PSASPE13_TEST_H
#define PSASPE13_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/exciter_model/PSASPE13.h"
#include "header/model/sg_models/exciter_model/exciter_model_test.h"
#include "header/STEPS.h"
using namespace std;

class PSASPE13_TEST : public EXCITER_MODEL_TEST
{
    public:
        PSASPE13_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
    private:

};

#endif//PSASPE13_TEST_H
