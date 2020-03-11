#ifndef MODEL_VAR_TABLE_TEST_H
#define MODEL_VAR_TABLE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/model_var_table.h"
#include "header/STEPS.h"

using namespace std;

class MODEL_VAR_TABLE_TEST : public Test::Suite
{
    public:
        MODEL_VAR_TABLE_TEST();

    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_clear();
        void test_add_get_variable_name_index_pair();
    private:
        MODEL_VAR_TABLE* table;
};

#endif//MODEL_VAR_TABLE_TEST_H
