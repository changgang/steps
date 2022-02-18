#ifndef INPHNO_TEST_H
#define INPHNO_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/inphno.h"
#include "header/STEPS.h"

using namespace std;

class INPHNO_TEST : public Test::Suite
{
    public:
        INPHNO_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_copy_constructor();
        void test_set_get_physical_internal_bus_number_pair();
        void test_update_with_new_internal_bus_permutation();
        void test_is_table_full();
        void test_is_emtpy();
        void test_clear();
        void test_operator_equal();
    private:
        INPHNO inphno;

};

#endif //INPHNO_TEST_H
