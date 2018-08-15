#ifndef GENCLS_TEST_H
#define GENCLS_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sync_generator_model/sync_generator_model_test.h"

#include "header/model/sg_models/sync_generator_model/gencls.h"


using namespace std;

class GENCLS_TEST : public SYNC_GENERATOR_MODEL_TEST
{
    public:
        GENCLS_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_update_source_impedance();
        void test_initialize_and_get_initialized_inputs();

        void test_set_get_rotor_angle();
        void test_set_get_rotor_speed();
    private:
};

#endif//GENCLS_TEST_H
