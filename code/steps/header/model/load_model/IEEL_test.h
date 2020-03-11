#ifndef IEEL_TEST_H
#define IEEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/load_model/load_model_test.h"

#include "header/model/load_model/IEEL.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class IEEL_TEST : public LOAD_MODEL_TEST
{
    public:
        IEEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_get_detailed_model_name();
        virtual void test_set_get_parameters();
    private:

};

#endif//IEEL_TEST_H
