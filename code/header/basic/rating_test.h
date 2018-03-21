#ifndef RATING_TEST_H
#define RATING_TEST_H


#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/rating.h"

using namespace std;

class RATING_TEST : public Test::Suite
{
    public:
        RATING_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_rating_A_MVA();
        void test_set_get_rating_B_MVA();
        void test_set_get_rating_C_MVA();
        void test_copy_with_operator_equal();
        void test_operator_bracket();
        void test_clear();

    private:
        RATING* rating;
};

#endif // RATING_TEST_H
