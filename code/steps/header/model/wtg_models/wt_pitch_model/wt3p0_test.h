#ifndef WT3P0_TEST_H
#define WT3P0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/wtg_models/wt_pitch_model/wt3p0.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model_test.h"
#include "header/STEPS.h"
using namespace std;

class WT3P0_TEST : public WT_PITCH_MODEL_TEST
{
    public:
        WT3P0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
    private:
};

#endif//WT3P0_TEST_H
