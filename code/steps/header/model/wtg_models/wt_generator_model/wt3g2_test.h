#ifndef WT3G2_TEST_H
#define WT3G2_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wt_generator_model/wt_generator_model_test.h"

#include "header/model/wtg_models/wt_generator_model/wt3g2.h"

#include "header/STEPS.h"

using namespace std;

class WT3G2_TEST : public WT_GENERATOR_MODEL_TEST
{
    public:
        WT3G2_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize_and_get_initialized_inputs();
        void test_set_get_pll_angle();
    private:
};

#endif//WT3G2_TEST_H
