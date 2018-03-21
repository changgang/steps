#ifndef PE_SOURCE_TEST_H
#define PE_SOURCE_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/pe_source.h"
#include "header/power_system_database.h"
using namespace std;

class PE_SOURCE_TEST : public Test::Suite
{
    public:
        PE_SOURCE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_source_bus();
        void test_set_get_source_impedance();
        void test_copy_with_operator_equal();
        void test_get_device_id();

    private:
        POWER_SYSTEM_DATABASE* db;
        PE_SOURCE* pesource;
};

#endif //PE_SOURCE_TEST_H
