#ifndef FAULT_TEST_H
#define FAULT_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/fault.h"

using namespace std;

class FAULT_TEST : public Test::Suite
{
    public:
        FAULT_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_fault_type();
        void test_set_get_fault_shunt();
        void test_is_faulted();
        void test_clear();
        void test_copy_with_opeartor_equal();
    private:
        FAULT fault;
};
#endif // FAULT_TEST_H
