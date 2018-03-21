#ifndef CSEET1_TEST_H
#define CSEET1_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/exciter_model/CSEET1.h"
#include "header/model/exciter_model/exciter_model_test.h"
using namespace std;

class CSEET1_TEST : public EXCITER_MODEL_TEST
{
    public:
        CSEET1_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SEPARATE_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR();
        virtual void test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SEPARATE_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR();
        virtual void test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SELF_EXCIATION_and_WITH_BRUSH_and_feedbacker_AT_REGULATOR();
        virtual void test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_VOLTAGE_ERROR();
        virtual void test_step_response_SELF_EXCIATION_and_WITHOUT_BRUSH_and_feedbacker_AT_REGULATOR();
    private:
};

#endif//CSEET1_TEST_H
