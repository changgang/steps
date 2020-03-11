#ifndef ZONE_TEST_H
#define ZONE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/zone.h"
#include "header/STEPS.h"

using namespace std;

class ZONE_TEST : public Test::Suite
{
    public:
        ZONE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_zone_number();
        void test_set_get_zone_name();
        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
    private:
        ZONE zone;
};

#endif
