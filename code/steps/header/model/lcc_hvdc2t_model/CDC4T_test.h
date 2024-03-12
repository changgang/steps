#ifndef CDC4T_TEST_H
#define CDC4T_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/lcc_hvdc2t_model/CDC4T.h"
#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model_test.h"
#include "header/STEPS.h"
using namespace std;

class CDC4T_TEST : public LCC_HVDC2T_MODEL_TEST
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
