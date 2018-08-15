#ifndef PVCV0_TEST_H
#define PVCV0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pv_converter_model/pv_converter_model_test.h"

#include "header/model/pvu_models/pv_converter_model/pvcv0.h"


using namespace std;

class PVCV0_TEST : public PV_CONVERTER_MODEL_TEST
{
    public:
        PVCV0_TEST();
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

#endif//PVCV0_TEST_H
