#ifndef CDC6T_TEST_H
#define CDC6T_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/hvdc_model/CDC6T.h"
#include "header/model/hvdc_model/hvdc_model_test.h"
using namespace std;

class CDC6T_TEST : public HVDC_MODEL_TEST
{
    public:
        CDC6T_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_type();
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
    private:
};

#endif//CDC6T_TEST_H
