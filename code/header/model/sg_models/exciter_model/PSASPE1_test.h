#ifndef PSASPE1_TEST_H
#define PSASPE1_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/sg_models/exciter_model/PSASPE1.h"
#include "header/model/sg_models/exciter_model/exciter_model_test.h"
using namespace std;

class PSASPE1_TEST : public EXCITER_MODEL_TEST
{
    public:
        PSASPE1_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
    private:
};

#endif//PSASPE1_TEST_H
