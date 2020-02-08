#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"

#include <cstdio>
#include <algorithm>
//#include <cmath>
#include <istream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <omp.h>

using namespace std;

double four_over_pi = 4.0*ONE_OVER_PI;
double four_over_pi2 = 4.0*ONE_OVER_PI*ONE_OVER_PI;

string num2str(int number)
{
    char str[1000];
    snprintf(str,1000, "%d",number);
    return string(str);
}

string num2str(unsigned int number)
{
    return num2str(int(number));
}

string num2str(double number)
{
    char str[1000];
    snprintf(str,100, "%G",number);
    return string(str);
}

string num2hex_str(int number)
{
    char str[64];
    sprintf(str, "%X", number);
    return string(str);
}

string num2hex_str(unsigned int number)
{
    return num2hex_str(int(number));
}

string num2hex_str(size_t number)
{
    return num2hex_str(int(number));
}

string num2hex_str(double number)
{
    char str[64];
    sprintf(str, "%A", number);
    return string(str);
}

int str2int(const string& str)
{
    return int(round(str2double(str)));
}

double str2double(const string& str)
{
    double num = 0.0;
    sscanf(str.c_str(),"%lf", &num);
    return num;
}

int get_integer_data(const string& strval, const string& strdefault)
{
    if(strval.size()!=0)
        return str2int(strval);
    else
        return str2int(strdefault);
}

double get_double_data(const string& strval, const string& strdefault)
{
    if(strval.size()!=0)
        return str2double(strval);
    else
        return str2double(strdefault);
}

string get_string_data(string strval, const string& strdefault)
{
    if(strval.size()==0) strval=strdefault;
    while(true)
    {
        size_t found = strval.find_first_of("\'\"");
        if(found==string::npos) break;
        strval.erase(found,1);
    }
    strval =  trim_string(strval);
    if(strval!="")
    {
        return strval;
    }
    else
    {
        return strdefault;
    }
}

unsigned int get_sparse_matrix_identity(const SPARSE_MATRIX& matrix)
{
    int n = matrix.get_matrix_entry_count();
    int identity = 0;
    for(int i=0; i<n; ++i)
    {
        int row = matrix.get_row_number_of_entry_index(i);
        int col = matrix.get_column_number_of_entry_index(i);
        complex<double> value = matrix.get_entry_value(i);
        int rvalue = int(value.real()*1000.0)<<10;
        int ivalue = int(value.imag()*1000.0)<<10;
        row = row^STEPS_MAGIC1;
        col = col^STEPS_MAGIC2;
        rvalue = rvalue^STEPS_MAGIC3;
        ivalue = ivalue^STEPS_MAGIC4;

        identity = identity^((row^col)^(rvalue^ivalue));
    }
    return (unsigned int)(identity);
}

unsigned int get_vector_identity(const vector<double>& vec)
{
    int n = vec.size();
    int identity = 0;
    for(int i=0; i<n; ++i)
    {
        int index = i;
        int value = int(vec[i]*10000.0)<<10;
        index = index^STEPS_MAGIC1;
        value = value^STEPS_MAGIC2;

        identity = identity^(index^value);
    }
    return (unsigned int)(identity);
}


string string2upper(string str)
{
    transform(str.begin(),str.end(),str.begin(),::toupper);
    return str;
}

double rad2deg(double angle)
{
    return angle*(180.0*ONE_OVER_PI);
}

double deg2rad(double angle)
{
    return angle*(PI/180.0);
}

double radps2hz(double w)
{
    return w*ONE_OVER_PI2;
    //return w/(2.0*PI);
}

double hz2radps(double f)
{
    return PI2*f;
    //return 2.0*PI*f;
}

double round_angle_in_rad_to_PI(double angle)
{
    double abs_angle = fabs(angle);
    if(abs_angle>PI)
        return (angle>0.0?1.0:-1.0)*(abs_angle - ceil((abs_angle-PI)*ONE_OVER_PI2)*PI2);
    else
        return angle;
}

double round_angle_in_rad_to_HALF_PI(double angle)
{
    double abs_angle = fabs(angle);
    if(abs_angle>HALF_PI)
        return (angle>0.0?1.0:-1.0)*(abs_angle - ceil((abs_angle-HALF_PI)*ONE_OVER_PI)*PI);
    else
        return angle;
}

double steps_fast_complex_abs(const complex<double>& z)
{
	double x = z.real();
	double y = z.imag();
	return steps_sqrt(x*x+y*y);
}

double steps_fast_complex_arg(const complex<double>& z)
{
	double x = z.real();
	double y = z.imag();

	if(x != 0.0 and y != 0.0)
	{
	    double angle = steps_atan(y / x);
        if (x < 0.0)
        {
            if (y > 0.0)
                angle += PI;
            else
                angle -= PI;
        }
        return angle;
	}
	else
    {
        if (x == 0.0 and y != 0.0)
        {
            if (y > 0)
                return PI * 0.5;
            else
            {
                if (y < 0.0)
                    return -PI * 0.5;
                else
                    return 0.0;
            }
        }
        else
        {
            if (y == 0.0 and x != 0.0)
            {
                if (x > 0.0)
                    return 0.0;
                else
                {
                    if (x < 0.0)
                        return PI;
                    else
                        return 0.0;
                }
            }
            else//x == 0.0 and y == 0.0
                return 0.0;
        }
    }
}

double steps_fast_pow(double base, double exp)
{
	return pow(base, exp);
	if (fabs(exp - 0.0) < FLOAT_EPSILON)
		return 1.0;
	if (fabs(exp - 1.0) < FLOAT_EPSILON)
		return base;
	if (fabs(exp - 2.0) < FLOAT_EPSILON)
		return base*base;
	if (fabs(exp - 3.0) < FLOAT_EPSILON)
		return base*base*base;
}

double steps_sin(double angle_in_rad)
{
    if(use_steps_fast_math==true)
        return steps_fast_sin(angle_in_rad);
    else
        return sin(angle_in_rad);
}

double steps_fast_sin(double angle_in_rad)
{
    //return sin(angle_in_rad);
    double x = angle_in_rad;
    x = round_angle_in_rad_to_PI(x);
    if(x>HALF_PI)
        x = PI-x;
    else
        if(x<-HALF_PI)
            x = -PI - x;

    double x2 = x*x;
    double x3 = x*x2;
    double x5 = x3*x2;
    double x7 = x5*x2;
    double x9 = x7*x2;
    double x11 = x9*x2;
    double x13 = x11*x2;

    return x-0.1666666666666666666666666666667*x3
            +8.3333333333333333333333333333333e-3*x5
            -1.984126984126984126984126984127e-4*x7
            +2.7557319223985890652557319223986e-6*x9
            -2.5052108385441718775052108385442e-8*x11
            +1.6059043836821614599392377170155e-10*x13;

    return 0.99999660*x-0.16664824*x3+0.00830629*x5-0.00018363*x7;
    //compute sine
    double sin = 0.0;
    if (x < 0)
    {
        sin= four_over_pi* x + four_over_pi2 * x* x;
        if (sin< 0) sin= 0.225*(sin*(-sin)- sin)+ sin;
        else        sin= 0.225*(sin*  sin - sin)+ sin;
    }
    else
    {
        sin= four_over_pi * x-four_over_pi2 * x* x;
        if (sin< 0) sin= 0.225 * (sin*(-sin)- sin) + sin;
        else        sin= 0.225 * (sin*  sin-  sin) + sin;
    }
    return sin;
}

double steps_cos(double angle_in_rad)
{
    if(use_steps_fast_math==true)
        return steps_fast_cos(angle_in_rad);
    else
        return cos(angle_in_rad);
}

double steps_fast_cos(double angle_in_rad)
{
    angle_in_rad += HALF_PI;
    return steps_fast_sin(angle_in_rad);
}

double steps_tan(double angle_in_rad)
{
    if(use_steps_fast_math==true)
        return steps_fast_tan(angle_in_rad);
    else
        return tan(angle_in_rad);
}

double steps_fast_tan(double angle_in_rad)
{
    //return tan(angle_in_rad);
    angle_in_rad = round_angle_in_rad_to_PI(angle_in_rad);
    double sin = steps_fast_sin(angle_in_rad);
    double cos = steps_fast_cos(angle_in_rad);
    if(cos!=0.0)
        return sin/cos;
    else
    {
        cout<<"Fatal error of steps_fast_tan(). Inf found.\n";
        return INFINITE_THRESHOLD;
    }
}

double steps_asin(double x)
{
    if(use_steps_fast_math==true)
        return steps_fast_asin(x);
    else
        return asin(x);
}

double steps_fast_asin(double x)
{
    return asin(x);
}

double steps_acos(double x)
{
    if(use_steps_fast_math==true)
        return steps_fast_acos(x);
    else
        return acos(x);
}

double steps_fast_acos(double x)
{
    return acos(x);
}

double steps_atan(double x)
{
    if(use_steps_fast_math==true)
        return steps_fast_atan(x);
    else
        return atan(x);
}

double steps_fast_atan(double x)
{
    return atan(x);
}

double steps_sqrt(double x)
{
    return sqrt(x);
    if(use_steps_fast_math==true)
        return steps_fast_sqrt(x);
    else
        return sqrt(x);
}

double steps_fast_sqrt(double x)
{
    return x*quick_inv_sqrt_Lomont(x);
}

double steps_inv_sqrt(double x)
{
    if(use_steps_fast_math==true)
        return steps_fast_inv_sqrt(x);
    else
        return 1.0/sqrt(x);
}

double steps_fast_inv_sqrt(double x)
{
    return quick_inv_sqrt_Lomont(x);
}

float quick_inv_sqrt_Quake3(float x)
{
	float xhalf = 0.5f*x;
	union
	{
	    float x;
	    int i;
	} u;
	u.x = x;
	u.i = 0x5f3759df- (u.i>>1); // gives initial guess y0
	for(unsigned int i=0; i<5; ++i)
        u.x *= (1.5f-xhalf*u.x*u.x); // Newton step, repeating increases accuracy
	return u.x;
}

float quick_inv_sqrt_Lomont(float x)
{
	float xhalf = 0.5f*x;
	union
	{
	    float x;
	    int i;
	} u;
	u.x = x;
	u.i = 0x5f375a86- (u.i>>1); // gives initial guess y0
	for(unsigned int i=0; i<5; ++i)
        u.x *= (1.5f-xhalf*u.x*u.x); // Newton step, repeating increases accuracy
	return u.x;
}

double quick_double_inv_sqrt_Lomont(double x)
{
	double xhalf = 0.5F*x;
	union
	{
	    double x;
	    long i;
	} u;
	u.x = x;
	u.i = 0x5fe6ec85e7de30da- (u.i>>1); // gives initial guess y0
	for(unsigned int i=0; i<5; ++i)
        u.x *= (1.5F-xhalf*u.x*u.x); // Newton step, repeating increases accuracy
	return u.x;
}

string trim_string(string str, const string& garbage)
{
    if(not str.empty())
    {
        if(garbage=="")
        {
            str.erase(0,str.find_first_not_of(" \t\n\r"));
            str.erase(str.find_last_not_of(" \t\n\r")+1);
        }
        else
        {
            str.erase(0,str.find_first_not_of(garbage));
            str.erase(str.find_last_not_of(garbage)+1);
        }
    }
    return str;
}

string replace_string_contents(string str, const string& source, const string& destination)
{
    size_t index = destination.find(source);
    if(index==string::npos)
    {
        while(true)
        {
            size_t ind = str.find(source);
            if(ind!=string::npos)
                str = str.replace(ind, source.size(), destination);
            else
                break;
        }
    }
    return str;
}

string shrink_sucessive_blanks(string str)
{
    str = replace_string_contents(str, "\t", " ");
    str = replace_string_contents(str,"  ", " ");
    return str;
}

string string2csv(string str)
{
    str = trim_string(str);
    str = shrink_sucessive_blanks(str);
    str = replace_string_contents(str, "'", "\"");
    str = replace_string_contents(str, ", ", ",");
    str = replace_string_contents(str, " ,", ",");

    unsigned int n = str.size();
    unsigned int n2 = n<<1;

    char* csv = (char*) malloc(sizeof(char)*n2);
    if(csv!=NULL)
    {
        for(unsigned int i=0; i!=n2; ++i)
            csv[i]='\0';

        unsigned int ncsv = 0;
        for(unsigned int i=0; i!=n; ++i)
        {
            char source = str[i];
            if(source=='"')
            {
                csv[ncsv] = source;
                ++ncsv;
                for(++i; i!=n; ++i)
                {
                    source=str[i];
                    if(source=='"')
                    {
                        csv[ncsv] = source;
                        ++ncsv;
                        break;
                    }
                    else
                    {
                        csv[ncsv] = source;
                        ++ncsv;
                    }
                }
                continue;
            }
            else
            {
                if(source==',' or source==' ')
                {
                    csv[ncsv]=',';
                    ++ncsv;
                    continue;
                }
                else
                {
                    csv[ncsv] = source;
                    ++ncsv;
                    continue;
                }
            }
        }

        string newstr(csv);
        free(csv);
        return newstr;
    }
    else
    {
        cout<<"Warning. Failed to allocate memory for "<<__FUNCTION__<<"().\n";
        return "";
    }
}

vector<string> split_string(string str, const string& sep)
{
    vector<string> splitted_str;
    str = trim_string(str);
    size_t newline_index = 0;
    while(not str.empty())
    {
        newline_index = str.find_first_of(sep);
        if(newline_index != string::npos)
        {
            splitted_str.push_back(str.substr(0, newline_index));
            str.erase(0, newline_index+1);
        }
        else
        {
            splitted_str.push_back(str);
            str.clear();
        }
    }

    unsigned int n_size = splitted_str.size();
    for(unsigned int i = 0; i!=n_size; ++i)
        splitted_str[i] = trim_string(splitted_str[i]);
    return splitted_str;
}

string string_vector2csv(const vector<string>& vec)
{
    string str="";
    unsigned int n = vec.size();
    if(n!=0)
    {
        str = vec[0];
        for(unsigned int i=1; i<n; ++i)
            str = str+", "+vec[i];
    }
    return str;
}

string swap_data_in_csv_string(const string& data, unsigned int i, unsigned int j)
{
    vector<string> record = split_string(data,",");
    unsigned int n = record.size();
    if(i<=n and j<=n)
    {
        string temp = record[i];
        record[i]=record[j];
        record[j]=temp;
        return string_vector2csv(record);
    }
    else
        return data;
}

complex<double> xy2dq_with_angle_in_deg(const complex<double>& V, double angle)
{
    angle = deg2rad(angle);
    return xy2dq_with_angle_in_rad(V, angle);
}

complex<double> xy2dq_with_angle_in_rad(const complex<double>& V, double angle)
{
    // (Vx+jVy)*(sin+jcos) =(Vx*sin-Vy*cos)+j(Vx*cos+Vy*sin)
    complex<double> rotation(steps_sin(angle), steps_cos(angle));
    return V*rotation;
}

complex<double> dq2xy_with_angle_in_deg(const complex<double>& V, double angle)
{
    angle = deg2rad(angle);
    return dq2xy_with_angle_in_rad(V, angle);
}

complex<double> dq2xy_with_angle_in_rad(const complex<double>& V, double angle)
{
    // (Vd+jVq)*(sin-jcos) =(Vd*sin+Vq*cos)+j(Vq*sin-Vd*cos)
    complex<double> rotation(steps_sin(angle), -steps_cos(angle));
    return V*rotation;
}


bool is_file_exist(const string& file)
{
    // check if the file exist
    // date: Sep. 1, 2016
    FILE* fid = fopen(file.c_str(),"r");
    if(fid!=NULL)
    {
        fclose(fid);
        return true;
    }
    else
    {
        return false;
    }
}


void show_information_with_leading_time_stamp_with_default_toolkit(const string& info)
{
    STEPS& toolkit = get_default_toolkit();
    toolkit.show_information_with_leading_time_stamp(info);
}

void show_information_with_leading_time_stamp_with_default_toolkit(ostringstream& stream)
{
    STEPS& toolkit = get_default_toolkit();
    toolkit.show_information_with_leading_time_stamp(stream);
}

void show_test_information_for_function_of_class(const string& func, const string& cls)
{
    ostringstream osstream;
    osstream<<"--s--t--a--r--t-----------------------------------------------------\n"
            <<"Run testing of "<<cls<<"::"<<func;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
}

void show_test_end_information()
{
    ostringstream osstream;
    osstream<<"--d--o--n--e-------------------------------------------------------";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
}

vector<string> psse_dyr_string2steps_string_vector(const string& data)
{
    vector<string> record = split_string(data,",");
    string temp = record[0];
    record[0]=record[1];
    record[1]=temp;
    return record;
}

string psse_dyr_string2steps_string(const string& data)
{
    return swap_data_in_csv_string(data, 1, 2);
}


void initialize_package()
{
    return;
    default_toolkit.set_toolkit_name("TK DFLT");
    for(unsigned int i=0; i<STEPS_MAX_TOOLKIT_SIZE; ++i)
        toolkits[i] = NULL;
}

unsigned int generate_new_toolkit(string log_file)
{
    mtx.lock();
    /*while(get_toolkit_count()>=STEPS_MAX_TOOLKIT_SIZE)
    {
        ostringstream osstream;
        osstream<<"Warning. Toolkit table is full when calling "<<__FUNCTION__<<"().\nTry to generate new toolkit in 5 sec.\n";

        show_information_with_leading_time_stamp_with_default_toolkit(osstream);

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }*/

    ostringstream osstream;
    unsigned int index=INDEX_NOT_EXIST;
    while(true)
    {
        bool toolkit_index_is_set = false;
        for(unsigned int i=0; i!=STEPS_MAX_TOOLKIT_SIZE; ++i)
        {
            if(toolkits[i]==NULL)
            {
                index = i;
                //osstream<<"NULL toolkit is found at "<<i<<endl;
                //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                osstream<<"TK "<<setfill('0')<<setw(4)<<num2str(index);
                toolkits[index] = new STEPS(osstream.str(), log_file);
                toolkit_index_is_set = true;
                break;
            }

        }
        if(toolkit_index_is_set==true)
            break;
    }
    mtx.unlock();
    return index;
}

void delete_toolkit(unsigned int toolkit_index)
{
    mtx.lock();
    if(toolkit_index<STEPS_MAX_TOOLKIT_SIZE)
    {
        if(toolkits[toolkit_index]!=NULL)
        {
            delete toolkits[toolkit_index];
            toolkits[toolkit_index] = NULL;
        }
    }
    mtx.unlock();
}

unsigned int get_toolkit_count()
{
    unsigned int count = 0;
    for(unsigned int i=0; i!=STEPS_MAX_TOOLKIT_SIZE; ++i)
    {
        if(toolkits[i]!=NULL)
            ++count;
    }
    return count;
}

STEPS& get_default_toolkit()
{
    return default_toolkit;
}

STEPS& get_toolkit(unsigned int toolkit_index)
{
    if(toolkit_index==INDEX_NOT_EXIST)
        return default_toolkit;
    if(toolkit_index<STEPS_MAX_TOOLKIT_SIZE and toolkits[toolkit_index]!=NULL)
        return *(toolkits[toolkit_index]);
    else
    {
        ostringstream osstream;
        osstream<<"Fatal Error. The toolkit_index is either invalid or toolkit is NULL when calling "<<__FUNCTION__<<"(). toolkit_index is: "<<toolkit_index<<"\n"
                <<"Default toolkit will be returned. However, no further simulation is guaranteed.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return default_toolkit;
    }
}

void set_openmp_number_of_threads(unsigned int n)
{
    omp_set_num_threads(n);
}
