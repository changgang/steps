#ifndef OWNER_TEST_H
#define OWNER_TEST_H


#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/owner.h"

using namespace std;

class OWNER_TEST : public Test::Suite
{
    public:
        OWNER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_owner_number();
        void test_set_get_owner_name();
        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
    private:
        POWER_SYSTEM_DATABASE* db;
        OWNER* owner;
};

#endif
