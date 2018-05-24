#ifndef GENSAL_TEST_H
#define GENSAL_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sync_generator_model/sync_generator_model_test.h"

#include "header/model/sg_models/sync_generator_model/gensal.h"


using namespace std;

class GENSAL_TEST : public SYNC_GENERATOR_MODEL_TEST
{
    public:
        GENSAL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_name();
        void test_update_source_impedance();

        void test_initialize_and_get_initialized_inputs();
};

#endif//GENSAL_TEST_H
