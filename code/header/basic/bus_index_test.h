#ifndef BUS_INDEX_TEST_H
#define BUS_INDEX_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/bus_index.h"

using namespace std;

class BUS_INDEX_TEST : public Test::Suite
{
    public:
        BUS_INDEX_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_bus_index();
        void test_capacity();
        void test_clear_index();
        void test_get_index_of_bus_and_operator_bracket();
    private:
        BUS_INDEX bus_index;
};

#endif //BUS_INDEX_TEST_H
