#ifndef UTILITY_H
#define UTILITY_H

#include "header/power_system_database.h"
#include "header/network/network_matrix.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <complex>

string num2str(int number);
string num2str(unsigned int number);
string num2str(double number);
string num2hex_str(int number);
string num2hex_str(unsigned int number);
string num2hex_str(size_t number);
string num2hex_str(double number);
int str2int(const string& str);
double str2double(const string& str);

int get_integer_data(const string& strval, const string& strdefault);
double get_double_data(const string& strval, const string& strdefault);
string get_string_data(string strval, const string& strdefault);

unsigned int get_sparse_matrix_identity(const SPARSE_MATRIX& matrix);
unsigned int get_vector_identity(const vector<double>& vec);

string string2upper(string str);

double rad2deg(double angle);
double deg2rad(double angle);

double radps2hz(double w);
double hz2radps(double f);

double steps_sin(double angle_in_rad);
double steps_fast_sin(double angle_in_rad);

double steps_cos(double angle_in_rad);
double steps_fast_cos(double angle_in_rad);

double steps_tan(double angle_in_rad);
double steps_fast_tan(double angle_in_rad);

double steps_asin(double x);
double steps_fast_asin(double x);

double steps_acos(double x);
double steps_fast_acos(double x);

double steps_atan(double x);
double steps_fast_atan(double x);

double round_angle_in_rad_to_PI(double angle);
double round_angle_in_rad_to_HALF_PI(double angle);

double steps_fast_complex_abs(const complex<double>& z);
double steps_fast_complex_arg(const complex<double>& z);

double steps_pow(double base, double exp);

double steps_sqrt(double x);
double steps_fast_sqrt(double x);

double steps_inv_sqrt(double x);
double steps_fast_inv_sqrt(double x);
float quick_inv_sqrt_Quake3(float x);
float quick_inv_sqrt_Lomont(float x);
double quick_double_inv_sqrt_Lomont(double x);

string trim_string(string str, const string& garbage="");
string replace_string_contents(string str, const string& source, const string& destination);
string shrink_sucessive_blanks(string str);
string string2csv(string str);
vector<string> split_string(string str, const string& sep);
string string_vector2csv(const vector<string>& vec);
string swap_data_in_csv_string(const string& data, unsigned int i, unsigned int j);

complex<double> xy2dq_with_angle_in_deg(const complex<double>& V, double angle);
complex<double> xy2dq_with_angle_in_rad(const complex<double>& V, double angle);
complex<double> dq2xy_with_angle_in_deg(const complex<double>& V, double angle);
complex<double> dq2xy_with_angle_in_rad(const complex<double>& V, double angle);

complex<double> get_xy2dq_rotation_with_angle_in_deg(double angle);
complex<double> get_xy2dq_rotation_with_angle_in_rad(double angle);
complex<double> get_dq2xy_rotation_with_angle_in_deg(double angle);
complex<double> get_dq2xy_rotation_with_angle_in_rad(double angle);

complex<double> ab2dq_with_angle_in_deg(const complex<double>& V, double angle);
complex<double> ab2dq_with_angle_in_rad(const complex<double>& V, double angle);
complex<double> dq2ab_with_angle_in_deg(const complex<double>& V, double angle);
complex<double> dq2ab_with_angle_in_rad(const complex<double>& V, double angle);

complex<double> get_ab2dq_rotation_with_angle_in_deg(double angle);
complex<double> get_ab2dq_rotation_with_angle_in_rad(double angle);
complex<double> get_dq2ab_rotation_with_angle_in_deg(double angle);
complex<double> get_dq2ab_rotation_with_angle_in_rad(double angle);

bool is_file_exist(const string& file);

void show_information_with_leading_time_stamp_with_default_toolkit(const string& info);
void show_information_with_leading_time_stamp_with_default_toolkit(ostringstream& stream);
string get_system_time_stamp_string_with_default_toolkit();

void show_test_information_for_function_of_class(const string& func, const string &cls);
void show_test_end_information();

vector<string> psse_dyr_string2steps_string_vector(const string& data);
string psse_dyr_string2steps_string(const string& data);

void initialize_package();
unsigned int generate_new_toolkit(string log_file="");
void delete_toolkit(unsigned int toolkit_index);
unsigned int get_toolkit_count();
STEPS& get_default_toolkit();
STEPS& get_toolkit(unsigned int toolkit_index);

void add_string_to_str_int_map(const string& str);
unsigned int get_index_of_string(const string& str);
string get_string_of_index(unsigned int index);

string device_type2string(STEPS_DEVICE_TYPE device_type);
string dc_device_type2string(STEPS_DC_DEVICE_TYPE device_type);

void set_openmp_number_of_threads(unsigned int n);

#endif // UTILITY_H
