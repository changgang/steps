#ifndef SATURATION_BLOCK_TEST_H
#define SATURATION_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/saturation_block.h"
#include "header/STEPS.h"

using namespace std;

class SATURATION_BLOCK_TEST : public Test::Suite
{
    public:
        SATURATION_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_saturation_type();
        void test_set_get_V1();
        void test_set_get_V2();
        void test_set_get_S1();
        void test_set_get_S2();
        void test_is_saturation_considered();
        void test_get_saturation_quadratic_type();
        void test_get_saturation_exponential_type_input_as_base();
        void test_get_saturation_exponential_type_input_as_exponential();
    private:
        SATURATION_BLOCK block;

};
#endif // SATURATION_BLOCK_TEST_H
