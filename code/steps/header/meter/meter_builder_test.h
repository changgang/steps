#ifndef METER_BUILDER_TEST_H
#define METER_BUILDER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/meter/meter_builder.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class METER_BUILDER_TEST : public Test::Suite
{
    public:
        METER_BUILDER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_prepare_bus_complete_meters();
        void test_prepare_ac_line_complete_meters();
        void test_prepare_transformer_complete_meters();
        void test_prepare_load_complete_meters();
        void test_prepare_generator_complete_meters();
        void test_prepare_wt_generator_complete_meters();
        void test_prepare_pv_unit_complete_meters();
        void test_prepare_energy_storage_complete_meters();
        void test_prepare_lcc_hvdc2t_complete_meters();
        void test_prepare_vsc_hvdc_complete_meters();
        void test_prepare_equivalent_device_complete_meters();
    private:
        METER_BUILDER builder;

};

#endif
