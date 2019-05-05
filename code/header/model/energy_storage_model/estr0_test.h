#ifndef ESTR0_TEST_H
#define ESTR0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/energy_storage_model/energy_storage_model_test.h"

#include "header/model/energy_storage_model/estr0.h"
#include "header/STEPS.h"

using namespace std;

class ESTR0_TEST : public ENERGY_STORAGE_MODEL_TEST
{
    public:
        ESTR0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();

        void test_initialize_and_get_initialized_inputs();
    private:

};

#endif//ESTR0_TEST_H
