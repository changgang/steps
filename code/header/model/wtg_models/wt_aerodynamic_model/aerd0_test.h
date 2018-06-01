#ifndef AERD0_TEST_H
#define AERD0_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model_test.h"

#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"


using namespace std;

class AERD0_TEST : public WT_AERODYNAMIC_MODEL_TEST
{
    public:
        AERD0_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize_and_get_initialized_inputs_with_overspeed_flag();
        virtual void test_initialize_and_get_initialized_inputs_without_overspeed_flag();
        virtual void test_get_turbine_speed_reference();
        virtual void test_set_as_typical_wt_generator();
        void test_list_Cp_and_mechanical_power_data_of_different_wind_speed();
    private:
};

#endif//AERD0_TEST_H
