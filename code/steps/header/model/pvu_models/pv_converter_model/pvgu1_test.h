#ifndef PVGU1_TEST_H
#define PVGU1_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pv_converter_model/pv_converter_model_test.h"

#include "header/model/pvu_models/pv_converter_model/pvgu1.h"

#include "header/STEPS.h"

using namespace std;

class PVGU1_TEST : public PV_CONVERTER_MODEL_TEST
{
    public:
        PVGU1_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize_and_get_initialized_inputs();
    private:
};

#endif//PVGU1_TEST_H
