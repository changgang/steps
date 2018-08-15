#ifndef CDC4T_TEST_H
#define CDC4T_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/hvdc_model/CDC4T.h"
#include "header/model/hvdc_model/hvdc_model_test.h"
using namespace std;

class CDC4T_TEST : public HVDC_MODEL_TEST
{
    public:
        CDC4T_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_type();
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
    private:
};

#endif//CDC4T_TEST_H
