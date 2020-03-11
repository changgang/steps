#ifndef ENERGY_STORAGE_TEST_H
#define ENERGY_STORAGE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/energy_storage.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class ENERGY_STORAGE_TEST : public Test::Suite
{
    public:
        ENERGY_STORAGE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_energy_storage_bus();

        void test_set_get_energy_storage_impedance();

        void test_set_get_energy_storage_model();

        void test_copy_with_operator_equal();

        void test_get_device_id();

    private:
        ENERGY_STORAGE energy_storage;
};

#endif //ENERGY_STORAGE_TEST_H
