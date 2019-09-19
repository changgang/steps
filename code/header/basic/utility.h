#ifndef UTILITY_H
#define UTILITY_H

#include "header/power_system_database.h"
#include "header/network/network_matrix.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <complex>

string num2str(int number);
string num2str(size_t number);
string num2str(double number);
string num2hex_str(int number);
string num2hex_str(size_t number);
string num2hex_str(double number);
int str2int(string str);
double str2double(string str);

int get_integer_data(string strval, string strdefault);
double get_double_data(string strval, string strdefault);
string get_string_data(string strval, string strdefault);

size_t get_sparse_matrix_identity(const SPARSE_MATRIX& matrix);
size_t get_vector_identity(const vector<double>& vec);

string string2upper(string str);

double rad2deg(double angle);
double deg2rad(double angle);

double radps2hz(double w);
double hz2radps(double f);

double round_angle_in_rad_to_PI(double angle);
double steps_fast_complex_abs(complex<double> z);
double steps_fast_complex_arg(complex<double> z);
double steps_fast_pow(double base, double exp);
double steps_fast_sine(double angle_in_rad);
double steps_fast_arcsine(double angle_in_rad);
double steps_fast_cosine(double angle_in_rad);
double steps_fast_arccosine(double angle_in_rad);
double steps_fast_tangent(double angle_in_rad);
double steps_fast_arctangent(double angle_in_rad);

string trim_string(string str, string garbage="");
string replace_string_contents(string str, string source, string destination);
string shrink_sucessive_blanks(string str);
string string2csv(string str);
vector<string> split_string(string str, const string sep);
string string_vector2csv(const vector<string>& vec);
string swap_data_in_csv_string(const string& data, size_t i, size_t j);

complex<double> xy2dq_with_angle_in_deg(complex<double> V, double angle);
complex<double> xy2dq_with_angle_in_rad(complex<double> V, double angle);
complex<double> dq2xy_with_angle_in_deg(complex<double> V, double angle);
complex<double> dq2xy_with_angle_in_rad(complex<double> V, double angle);

bool is_file_exist(const string file);

void show_information_with_leading_time_stamp_with_default_toolkit(string info);
void show_information_with_leading_time_stamp_with_default_toolkit(ostringstream& stream);
string get_system_time_stamp_string_with_default_toolkit();

void show_test_information_for_function_of_class(string func, string cls);
void show_test_end_information();

vector<string> psse_dyr_string2steps_string_vector(string& data);
string psse_dyr_string2steps_string(string& data);

void initialize_package();
size_t generate_new_toolkit(string log_file="");
void delete_toolkit(size_t toolkit_index);
size_t get_toolkit_count();
STEPS& get_default_toolkit();
STEPS& get_toolkit(size_t toolkit_index);


#endif // UTILITY_H
