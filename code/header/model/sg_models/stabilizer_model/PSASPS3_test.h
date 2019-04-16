#ifndef PSASPS3_TEST_H
#define PSASPS3_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/stabilizer_model/stabilizer_model_test.h"

#include "header/model/sg_models/stabilizer_model/PSASPS3.h"
#include "header/power_system_database.h"
using namespace std;

class PSASPS3_TEST : public STABILIZER_MODEL_TEST
{
    public:
        PSASPS3_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_name();
        void test_set_get_parameters();
        void test_set_get_input_signals();
    private:
};

#endif//PSASPS3_TEST_H
