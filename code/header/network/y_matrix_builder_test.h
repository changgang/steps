#ifndef Y_MATRIX_BUILDER_TEST_H
#define Y_MATRIX_BUILDER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest-1.1.2/src/cpptest.h"

#include "header/network/y_matrix_builder.h"

using namespace std;

class Y_MATRIX_BUILDER_TEST : public Test::Suite
{
    public:
        Y_MATRIX_BUILDER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_build_and_show_network_matrix();
        void test_remove_line_from_network_matrix();
        void test_remove_transformer_from_network_matrix();
};

#endif
