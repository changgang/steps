#ifndef STR_INT_MAP_TEST_H
#define STR_INT_MAP_TEST_H

#include <istream>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/string_int_map.h"
#include "header/STEPS.h"

using namespace std;

class STR_INT_MAP_TEST : public Test::Suite
{
    public:
        STR_INT_MAP_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_map_add_and_index();
    private:
        STR_INT_MAP test_map;
};

#endif //STR_INT_MAP_TEST_H
