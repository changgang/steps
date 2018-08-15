#ifndef CSEET2_TEST_H
#define CSEET2_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/exciter_model/CSEET2.h"
#include "header/model/sg_models/exciter_model/exciter_model_test.h"
using namespace std;

class CSEET2_TEST : public EXCITER_MODEL_TEST
{
    public:
        CSEET2_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SEPARATE_EXCIATION_and_stabilizer_AT_REGULATOR();
        virtual void test_step_response_SELF_EXCIATION_and_stabilizer_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SELF_EXCIATION_and_stabilizer_AT_REGULATOR();
    private:
};

#endif//CSEET2_TEST_H
