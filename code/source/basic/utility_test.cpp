#include "header/basic/utility_test.h"
#include "header/steps_namespace.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

UTILITY_TEST::UTILITY_TEST()
{
    //TEST_ADD(UTILITY_TEST::test_num2str);
    TEST_ADD(UTILITY_TEST::test_str2int);
    TEST_ADD(UTILITY_TEST::test_str2double);
    TEST_ADD(UTILITY_TEST::test_string2upper);

    TEST_ADD(UTILITY_TEST::test_get_integer_data);
    TEST_ADD(UTILITY_TEST::test_get_double_data);
    TEST_ADD(UTILITY_TEST::test_get_string_data);

    TEST_ADD(UTILITY_TEST::test_rad2deg);
    TEST_ADD(UTILITY_TEST::test_deg2rad);
    TEST_ADD(UTILITY_TEST::test_round_angle_to_PI);
    TEST_ADD(UTILITY_TEST::test_steps_fast_complex_abs);
    TEST_ADD(UTILITY_TEST::test_steps_fast_complex_arg);
    TEST_ADD(UTILITY_TEST::test_radps2hz);
    TEST_ADD(UTILITY_TEST::test_hz2radps);

    TEST_ADD(UTILITY_TEST::test_trim_string);
    TEST_ADD(UTILITY_TEST::test_replace_string_contents);
    TEST_ADD(UTILITY_TEST::test_shrink_sucessive_blanks);
    TEST_ADD(UTILITY_TEST::test_string2csv);
    TEST_ADD(UTILITY_TEST::test_split_string);

    TEST_ADD(UTILITY_TEST::test_redirect_and_recover_stdout);

    TEST_ADD(UTILITY_TEST::test_is_file_exist);
    TEST_ADD(UTILITY_TEST::test_set_get_dynamic_simulation_time_step);
    TEST_ADD(UTILITY_TEST::test_set_get_dynamic_simulation_time);
}

void UTILITY_TEST::setup()
{
    ;
}

void UTILITY_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    show_test_end_information();
}

void UTILITY_TEST::test_get_next_alphabeta()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(get_next_alphabeta()=='A');
    TEST_ASSERT(get_next_alphabeta()=='B');
    TEST_ASSERT(get_next_alphabeta()=='C');
    TEST_ASSERT(get_next_alphabeta()=='D');
    TEST_ASSERT(get_next_alphabeta()=='E');
    TEST_ASSERT(get_next_alphabeta()=='F');
    TEST_ASSERT(get_next_alphabeta()=='G');
    TEST_ASSERT(get_next_alphabeta()=='H');
    TEST_ASSERT(get_next_alphabeta()=='I');
    TEST_ASSERT(get_next_alphabeta()=='J');
    TEST_ASSERT(get_next_alphabeta()=='K');
    TEST_ASSERT(get_next_alphabeta()=='L');
    TEST_ASSERT(get_next_alphabeta()=='M');
    TEST_ASSERT(get_next_alphabeta()=='N');
    TEST_ASSERT(get_next_alphabeta()=='O');
    TEST_ASSERT(get_next_alphabeta()=='P');
    TEST_ASSERT(get_next_alphabeta()=='Q');
    TEST_ASSERT(get_next_alphabeta()=='R');
    TEST_ASSERT(get_next_alphabeta()=='S');
    TEST_ASSERT(get_next_alphabeta()=='T');
    TEST_ASSERT(get_next_alphabeta()=='U');
    TEST_ASSERT(get_next_alphabeta()=='V');
    TEST_ASSERT(get_next_alphabeta()=='W');
    TEST_ASSERT(get_next_alphabeta()=='X');
    TEST_ASSERT(get_next_alphabeta()=='Y');
    TEST_ASSERT(get_next_alphabeta()=='Z');
    TEST_ASSERT(get_next_alphabeta()=='A');
    TEST_ASSERT(get_next_alphabeta()=='B');
}

void UTILITY_TEST::test_num2str()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(num2str(123)=="123"); // integer
    TEST_ASSERT(num2str(123.45)=="123.45"); // float
    TEST_ASSERT(num2str(123456.7)=="123456.7");
    cout<<get_system_time_stamp_string()<<" "<<num2str(123456.7)<<endl;
    TEST_ASSERT(num2str(1000000)=="1E+6");
    cout<<get_system_time_stamp_string()<<" "<<num2str(1000000)<<endl;
}

void UTILITY_TEST::test_str2int()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(str2int("123")==123);
    TEST_ASSERT(str2int("123.45")==123);
    TEST_ASSERT(str2int("123.5")==124);
}
void UTILITY_TEST::test_str2double()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(fabs(str2double("123.0")-123)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(str2double("123.45")-123.45)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(str2double("1.23e3")-1230)<FLOAT_EPSILON);
}


void UTILITY_TEST::test_get_integer_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(get_integer_data("10","2")==10);
    TEST_ASSERT(get_integer_data("","2")==2);
}

void UTILITY_TEST::test_get_double_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(fabs(get_double_data("10.0","100.0")-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(get_double_data("","100.0")-100.0)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_get_string_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(get_string_data("AB","DEF")=="AB");
    TEST_ASSERT(get_string_data("'AB'","DEF")=="AB");
    TEST_ASSERT(get_string_data("","DEF")=="DEF");
    TEST_ASSERT(get_string_data("1","1")=="1");
}


void UTILITY_TEST::test_string2upper()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(string2upper("abc DeF.")=="ABC DEF.");
}

void UTILITY_TEST::test_rad2deg()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    double angle = 0.234;
    double angle2 = angle/PI*180.0;
    TEST_ASSERT(fabs(rad2deg(angle)-angle2)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_deg2rad()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    double angle = 23.45;
    double angle2 = angle/180.0*PI;
    TEST_ASSERT(fabs(deg2rad(angle)-angle2)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_round_angle_to_PI()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    double angle = 3.2;
    TEST_ASSERT(fabs(round_angle_in_rad_to_PI(angle)-(angle-2*PI))<FLOAT_EPSILON);

    angle = -3.2;
    TEST_ASSERT(fabs(round_angle_in_rad_to_PI(angle)-(angle+2*PI))<FLOAT_EPSILON);
}

void UTILITY_TEST::test_steps_fast_complex_abs()
{
	show_test_information_for_function_of_class(__FUNCTION__, "UTILITY_TEST");

	complex<double> V;
	V = complex<double>(0.0, 0.0);
	TEST_ASSERT(fabs(steps_fast_complex_abs(V) - abs(V))<FLOAT_EPSILON);
	V = complex<double>(1.0, 0.0);
	TEST_ASSERT(fabs(steps_fast_complex_abs(V) - abs(V))<FLOAT_EPSILON);
	V = complex<double>(0.0, 1.0);
	TEST_ASSERT(fabs(steps_fast_complex_abs(V) - abs(V))<FLOAT_EPSILON);
}

void UTILITY_TEST::test_steps_fast_complex_arg()
{
	show_test_information_for_function_of_class(__FUNCTION__, "UTILITY_TEST");

	complex<double> V;
	V = complex<double>(0.0, 0.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(1.0, 0.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(-1.0, 0.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(0.0, 1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(0.0, -1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(1.0, 1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(-1.0, 1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(-1.0, -1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
	V = complex<double>(1.0, -1.0);
	TEST_ASSERT(fabs(steps_fast_complex_arg(V) - arg(V))<FLOAT_EPSILON);
}

void UTILITY_TEST::test_radps2hz()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    double w = 1.0;
    double f = w/(2.0*PI);
    TEST_ASSERT(fabs(radps2hz(w)-f)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_hz2radps()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    double f = 1.0;
    double w = f*(2.0*PI);
    TEST_ASSERT(fabs(hz2radps(f)-w)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_trim_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    string str=" \t  \n \t\nA\tB C\nD\n\t  \t \n";
    TEST_ASSERT(trim_string(str)=="A\tB C\nD");

}
void UTILITY_TEST::test_replace_string_contents()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    string str="abAB12''\"\"";
    TEST_ASSERT(replace_string_contents(str, "ab", "AB")=="ABAB12''\"\"");
    TEST_ASSERT(replace_string_contents(str, "12", "21")=="abAB21''\"\"");
    TEST_ASSERT(replace_string_contents(str, "'", "\"")=="abAB12\"\"\"\"");
    TEST_ASSERT(replace_string_contents(str, "\"", "'")=="abAB12''''");
    TEST_ASSERT(replace_string_contents(str, "c", "C")=="abAB12''\"\"");

}
void UTILITY_TEST::test_shrink_sucessive_blanks()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    string str="a    bc  \t d";
    TEST_ASSERT(shrink_sucessive_blanks(str)=="a bc d");
}
void UTILITY_TEST::test_string2csv()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");
    string str="1 2  3,4 ,5, 6 , 7 '8 9' \" 0 1\"";
    str = string2csv(str);
    TEST_ASSERT(str=="1,2,3,4,5,6,7,\"8 9\",\" 0 1\"");

}
void UTILITY_TEST::test_split_string()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

}

void UTILITY_TEST::test_redirect_and_recover_stdout()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    redirect_stdout_to_file("test_log/test_redirect_stdout.dat");
    show_information_with_leading_time_stamp("This is a test line for redirecting stdout to this file.");
    show_information_with_leading_time_stamp("This is the second test line for redirecting stdout to this file.");

    recover_stdout();
    show_information_with_leading_time_stamp("This line should be outputted to stdout.");
}

void UTILITY_TEST::test_is_file_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    TEST_ASSERT(is_file_exist("sample.raw")==true);
    TEST_ASSERT(is_file_exist("sample2.raw")==false);
}

void UTILITY_TEST::test_set_get_dynamic_simulation_time_step()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    set_dynamic_simulation_time_step_in_s(0.01);
    TEST_ASSERT(fabs(get_dynamic_simulation_time_step_in_s()-0.01)<FLOAT_EPSILON);

    set_dynamic_simulation_time_step_in_s(0.02);
    TEST_ASSERT(fabs(get_dynamic_simulation_time_step_in_s()-0.02)<FLOAT_EPSILON);
}

void UTILITY_TEST::test_set_get_dynamic_simulation_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"UTILITY_TEST");

    set_dynamic_simulation_time_in_s(0.01);
    TEST_ASSERT(fabs(get_dynamic_simulation_time_in_s()-0.01)<FLOAT_EPSILON);

    set_dynamic_simulation_time_in_s(0.02);
    TEST_ASSERT(fabs(get_dynamic_simulation_time_in_s()-0.02)<FLOAT_EPSILON);
}
