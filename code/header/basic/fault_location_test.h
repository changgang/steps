#ifndef FAULT_LOCATION_TEST_H
#define FAULT_LOCATION_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest-1.1.2/src/cpptest.h"

#include "header/basic/fault_location.h"

using namespace std;

class FAULT_LOCATION_TEST : public Test::Suite
{
    public:
        FAULT_LOCATION_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_fault_location_in_pu();
        void test_operator_less();
        void test_operator_equal();
    private:
        FAULT_LOCATION* fault_location;
};
#endif // FAULT_LOCATION_TEST_H
