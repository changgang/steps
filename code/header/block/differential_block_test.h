#ifndef DIFFERENTIAL_BLOCK_TEST_H
#define DIFFERENTIAL_BLOCK_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/block/differential_block.h"
#include "header/STEPS.h"
using namespace std;

class DIFFERENTIAL_BLOCK_TEST : public Test::Suite
{
    public:
        DIFFERENTIAL_BLOCK_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_K();
        void test_set_get_T();
        void test_step_response_without_limiter();
    private:
        DIFFERENTIAL_BLOCK block;
};
#endif // DIFFERENTIAL_BLOCK_TEST_H
