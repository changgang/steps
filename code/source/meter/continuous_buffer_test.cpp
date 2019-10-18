#include "header/basic/test_macro.h"
#include "header/meter/continuous_buffer_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>


#ifdef ENABLE_STEPS_TEST
using namespace std;

CONTINUOUS_BUFFER_TEST::CONTINUOUS_BUFFER_TEST()
{
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_constructor);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_set_get_buffer_size);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_initialize_buffer);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_append_and_get_data);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_index_of_buffer_head);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_buffer_time_at_head);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_head);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_buffer_time_at_delay_index);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_delay_index);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_time);
    TEST_ADD(CONTINUOUS_BUFFER_TEST::test_get_delay_index_of_time);
}

void CONTINUOUS_BUFFER_TEST::setup()
{
    buffer.set_toolkit(default_toolkit);
    default_toolkit.set_dynamic_simulation_time_step_in_s(0.01);
}

void CONTINUOUS_BUFFER_TEST::tear_down()
{
    buffer.clear();

    show_test_end_information();
}

void CONTINUOUS_BUFFER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    TEST_ASSERT(buffer.get_buffer_size()==1);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==0);
}

void CONTINUOUS_BUFFER_TEST::test_set_get_buffer_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    buffer.set_buffer_size(10);
    TEST_ASSERT(buffer.get_buffer_size()==10);
    buffer.set_buffer_size(100);
    TEST_ASSERT(buffer.get_buffer_size()==100);
}

void CONTINUOUS_BUFFER_TEST::test_initialize_buffer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double delt = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double current_time = -2.0*delt;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);

    TEST_ASSERT(buffer.get_index_of_buffer_head()==0);

    size_t nbuffer = buffer.get_buffer_size();
    for(size_t i=0; i!=nbuffer; ++i)
    {
        TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(i)-(current_time-delt*i))<FLOAT_EPSILON);
        TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(i)-1.0)<FLOAT_EPSILON);
    }
}

void CONTINUOUS_BUFFER_TEST::test_append_and_get_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double current_time = 0.0;

    buffer.set_buffer_size(5);
    buffer.initialize_buffer(current_time, 1.0);
    buffer.show_buffer();

    buffer.append_data(0.01, 2.0);
    buffer.show_buffer();

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-1);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-2.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-1.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-(-0.01))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 3.0);
    buffer.show_buffer();

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-2);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-3.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-2.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-(-0.01))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 4.0);
    buffer.show_buffer();

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-4.0)<FLOAT_EPSILON);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-2);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-4.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-2.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);

    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-(-0.01))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 6.0);
    buffer.show_buffer();

    buffer.append_data(0.04, 7.0);
    buffer.show_buffer();

    buffer.append_data(0.05, 9.0);
    buffer.show_buffer();

    buffer.append_data(0.06, 10.0);
    buffer.show_buffer();

    buffer.append_data(0.06, 12.0);
    buffer.show_buffer();
}

void CONTINUOUS_BUFFER_TEST::test_get_index_of_buffer_head()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double delt = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double current_time = -2.0*delt;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==0);

    buffer.append_data(0.01, 2.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-1);

    buffer.append_data(0.02, 3.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-2);

    buffer.append_data(0.03, 2.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-3);

    buffer.append_data(0.03, 3.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-3);

    buffer.append_data(0.02, 4.0);
    TEST_ASSERT(buffer.get_index_of_buffer_head()==buffer.get_buffer_size()-3);
}

void CONTINUOUS_BUFFER_TEST::test_get_buffer_time_at_head()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double current_time = 0.0;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.0)<FLOAT_EPSILON);

    buffer.append_data(0.01, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.01)<FLOAT_EPSILON);

    buffer.append_data(0.02, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.02)<FLOAT_EPSILON);

    buffer.append_data(0.03, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.03)<FLOAT_EPSILON);

    buffer.append_data(0.03, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.03)<FLOAT_EPSILON);

    buffer.append_data(0.02, 4.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_head()-0.03)<FLOAT_EPSILON);
}

void CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_head()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double delt = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double current_time = -2.0*delt;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.01, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-2.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-3.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-2.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-3.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 4.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_head()-3.0)<FLOAT_EPSILON);
}

void CONTINUOUS_BUFFER_TEST::test_get_buffer_time_at_delay_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double current_time = 0.0;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-(-0.01))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-(-0.02))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-(-0.03))<FLOAT_EPSILON);

    buffer.append_data(0.01, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-(-0.01))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-(-0.02))<FLOAT_EPSILON);

    buffer.append_data(0.02, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-(-0.01))<FLOAT_EPSILON);

    buffer.append_data(0.03, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-0.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-0.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 4.0);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(0)-0.03)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(1)-0.02)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(2)-0.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_time_at_delay_index(3)-0.0)<FLOAT_EPSILON);
}

void CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_delay_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double delt = default_toolkit.get_dynamic_simulation_time_step_in_s();
    double current_time = -2.0*delt;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.01, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 2.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.03, 3.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);

    buffer.append_data(0.02, 4.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(0)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(1)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(2)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_delay_index(3)-1.0)<FLOAT_EPSILON);
}

void CONTINUOUS_BUFFER_TEST::test_get_buffer_value_at_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double current_time = 0.0;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);

    buffer.append_data(0.01, 2.0);
    buffer.append_data(0.02, 3.0);
    buffer.append_data(0.03, 3.0);
    buffer.append_data(0.06, 5.0);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_time(0.0)-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_time(0.01)-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_time(0.02)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_time(0.03)-3.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(buffer.get_buffer_value_at_time(0.06)-5.0)<FLOAT_EPSILON);
}

void CONTINUOUS_BUFFER_TEST::test_get_delay_index_of_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONTINUOUS_BUFFER_TEST");

    double current_time = 0.0;

    buffer.set_buffer_size(100);
    buffer.initialize_buffer(current_time, 1.0);

    buffer.append_data(0.01, 2.0);
    buffer.append_data(0.02, 3.0);
    buffer.append_data(0.03, 3.0);
    buffer.append_data(0.06, 5.0);
    TEST_ASSERT(buffer.get_delay_index_of_time(0.0)==4);
    TEST_ASSERT(buffer.get_delay_index_of_time(0.01)==3);
    TEST_ASSERT(buffer.get_delay_index_of_time(0.02)==2);
    TEST_ASSERT(buffer.get_delay_index_of_time(0.03)==1);
    TEST_ASSERT(buffer.get_delay_index_of_time(0.06)==0);
}

#endif
