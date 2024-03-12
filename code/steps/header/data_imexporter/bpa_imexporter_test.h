#ifndef BPA_IMEXPORTER_TEST_H
#define BPA_IMEXPORTER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/data_imexporter/bpa_imexporter.h"
#include "header/STEPS.h"
using namespace std;

class BPA_IMEXPORTER_TEST : public Test::Suite
{
    public:
        BPA_IMEXPORTER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_load_powerflow_data();
        void test_load_zone_data();
        void test_load_owner_data();
        void test_load_bus_data();
        void test_load_area_data();
        void test_load_case_data();

        void test_load_load_and_fixed_shunt_data();
        void test_load_fixed_shunt_data();
        void test_load_generator_data();
        void test_load_wt_generator_data();
        void test_load_ac_line_data();
        void test_load_transformer_data();
        void test_load_2t_lcc_hvdc_data();

        void test_convert_data_into_bpa_format();

        void test_export_powerflow_data();
        void test_export_powerflow_data_imported_from_psse();
        void test_export_powerflow_data_imported_from_bpa();


        void test_load_dynamic_data();

    private:
        BPA_IMEXPORTER importer;
};
#endif // BPA_IMEXPORTER_TEST_H
