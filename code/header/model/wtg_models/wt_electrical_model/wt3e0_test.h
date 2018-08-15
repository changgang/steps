#ifndef WT3E0_TEST_H
#define WT3E0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/wtg_models/wt_electrical_model/wt3e0.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model_test.h"
using namespace std;

class WT3E0_TEST : public WT_ELECTRICAL_MODEL_TEST
{
    public:
        WT3E0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
    private:
};

#endif//WT3E0_TEST_H
