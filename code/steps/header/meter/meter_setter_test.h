#ifndef METER_SETTER_TEST_H
#define METER_SETTER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/meter/meter_setter.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class METER_SETTER_TEST : public Test::Suite
{
    public:
        METER_SETTER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_prepare_bus_related_meters();
        void test_prepare_line_related_meters();
        void test_prepare_transformer_related_meters();
        void test_prepare_load_related_meters();
        void test_prepare_generator_related_meters();
        void test_prepare_wt_generator_related_meters();
        void test_prepare_pv_unit_related_meters();
        void test_prepare_energy_storage_related_meters();
        void test_prepare_2t_lcc_hvdc_related_meters();
        void test_prepare_vsc_hvdc_related_meters();
        void test_prepare_equivalent_device_related_meters();
    private:
        METER_SETTER setter;

};

#endif
