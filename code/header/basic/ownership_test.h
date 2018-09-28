#ifndef OWNERSHIP_TEST_H
#define OWNERSHIP_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/ownership.h"

using namespace std;

class OWNERSHIP_TEST : public Test::Suite
{
    public:
        OWNERSHIP_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_append_and_get_ownership();
        void test_delete_owner_and_its_fraction();
        void test_normalize();
        void test_get_owner_count();
        void test_is_empty();
        void test_get_all_owners();
        void test_get_all_fraction();
        void test_get_fraction_of_owner();
        void test_get_owner_of_index();
        void test_get_fraction_of_index();
        void test_clear();
        void test_copy_with_operator_equal();
    private:
        OWNERSHIP ownership;
};

#endif // OWNERSHIP_TEST_H
