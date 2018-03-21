#ifndef CONTINUOUS_BUFFER_TEST_H
#define CONTINUOUS_BUFFER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/meter/continuous_buffer.h"

using namespace std;

class CONTINUOUS_BUFFER_TEST : public Test::Suite
{
    public:
        CONTINUOUS_BUFFER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_buffer_size();
        void test_initialize_buffer();
        void test_append_and_get_data();
        void test_get_index_of_buffer_head();
        void test_get_buffer_time_at_head();
        void test_get_buffer_value_at_head();
        void test_get_buffer_time_at_delay_index();
        void test_get_buffer_value_at_delay_index();
        void test_get_buffer_value_at_time();
        void test_get_delay_index_of_time();
    private:
        CONTINUOUS_BUFFER* buffer;
};

#endif
