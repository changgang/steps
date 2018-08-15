#ifndef TERMINAL_TEST_H
#define TERMINAL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/terminal.h"

using namespace std;

class TERMINAL_TEST : public Test::Suite
{
    public:
        TERMINAL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_append_and_get_bus();

        void test_has_bus();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_operator_bracket();
    private:
        TERMINAL* terminal;
};

#endif//TERMINAL_TEST_H
