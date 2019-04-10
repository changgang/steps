#ifndef WTRLY0_TEST_H
#define WTRLY0_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wt_relay_model/wtrly0.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_model_test.h"
using namespace std;

class WTRLY0_TEST : public WT_RELAY_MODEL_TEST
{
    public:
        WTRLY0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
    private:
};

#endif//WTRLY0_TEST_H
