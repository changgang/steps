#ifndef FILEWIND_TEST_H
#define FILEWIND_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/wtg_models/wind_speed_model/wind_speed_model_test.h"
using namespace std;

class FILEWIND_TEST : public WIND_SPEED_MODEL_TEST
{
    public:
        FILEWIND_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_get_wind_speed();
        virtual void test_get_wind_direction();
    private:
        void prepare_wind_speed_file(string file);
};

#endif//FILEWIND_TEST_H
