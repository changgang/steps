#ifndef PSSE_IMEXPORTER_TEST_H
#define PSSE_IMEXPORTER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/data_imexporter/psse_imexporter.h"

using namespace std;

class PSSE_IMEXPORTER_TEST : public Test::Suite
{
    public:
        PSSE_IMEXPORTER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_load_case_data();
        void test_load_bus_data();
        void test_load_load_data();
        void test_load_fixed_shunt_data();
        void test_load_generator_data();
        void test_load_pe_source_data();
        void test_load_line_data();
        void test_load_transformer_data();
        void test_load_area_data();
        void test_load_hvdc_data();
        void test_load_zone_data();
        void test_load_owner_data();

        void test_export_powerflow_data();
        void test_export_powerflow_data_imported_from_psse();
        void test_export_powerflow_data_imported_from_bpa();

        void test_load_dynamic_data();
    private:
        POWER_SYSTEM_DATABASE* db;
        PSSE_IMEXPORTER* importer;
};
#endif // PSSE_IMEXPORTER_TEST_H
