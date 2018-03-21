#ifndef UTILITY_H
#define UTILITY_H

#include "header/power_system_database.h"
#include "header/network_database.h"
#include <complex>

string num2str(int number);
string num2str(size_t number);
string num2str(double number);
int str2int(string str);
double str2double(string str);

int get_integer_data(string strval, string strdefault);
double get_double_data(string strval, string strdefault);
string get_string_data(string strval, string strdefault);

string string2upper(string str);

double rad2deg(double angle);
double deg2rad(double angle);

string trim_string(string str);
string replace_string_contents(string str, string source, string destination);
string shrink_sucessive_blanks(string str);
string string2csv(string str);
vector<string> split_string(string str, const string sep);

void redirect_stdout_to_file(string file);
void recover_stdout();

complex<double> xy2dq_with_angle_in_deg(complex<double> V, double angle);
complex<double> xy2dq_with_angle_in_rad(complex<double> V, double angle);
complex<double> dq2xy_with_angle_in_deg(complex<double> V, double angle);
complex<double> dq2xy_with_angle_in_rad(complex<double> V, double angle);

void initialize_simulator();
void reset_simulator();
void terminate_simulator();
void show_test_information_for_function_of_class(string func, string cls);
void show_test_end_information();
void show_information_with_leading_time_stamp(string info);
void show_information_with_leading_time_stamp(ostringstream& stream);
string get_system_time_stamp_string();

bool is_file_exist(const string file);

POWER_SYSTEM_DATABASE* get_built_in_power_system_database_of_number(size_t i);

void reset_power_system_database(size_t i=0);

void set_dynamic_simulation_time_step_in_s(double delt);
double get_dynamic_simulation_time_step_in_s();
#endif // UTILITY_H
