#ifndef FILEIRRAD_TEST_H
#define FILEIRRAD_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model_test.h"

using namespace std;

class FILEIRRAD_TEST : public PV_IRRADIANCE_MODEL_TEST
{
    public:
        FILEIRRAD_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_get_solar_irradiance();
        virtual void test_get_solar_irradiance_direction();
    private:
        void prepare_solar_irradiance_file(string file);
};

#endif//FILEIRRAD_TEST_H
