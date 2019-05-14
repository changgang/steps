#ifndef UTILITY_TEST_H
#define UTILITY_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class UTILITY_TEST : public Test::Suite
{
    public:
        UTILITY_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_get_next_alphabeta();
        void test_num2str();
        void test_str2int();
        void test_str2double();

        void test_get_integer_data();
        void test_get_double_data();
        void test_get_string_data();

        void test_string2upper();

        void test_rad2deg();
        void test_deg2rad();

        void test_round_angle_to_PI();

        void test_steps_fast_complex_abs();
        void test_steps_fast_complex_arg();

        void test_radps2hz();
        void test_hz2radps();

        void test_trim_string();
        void test_replace_string_contents();
        void test_shrink_sucessive_blanks();
        void test_string2csv();
        void test_split_string();
        void test_string_vector2csv();

        void test_is_file_exist();

        void test_generate_and_delete_toolkit();
        void test_get_toolkit();
};

#endif//UTILITY_TEST_H
