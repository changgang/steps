#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"

#include <cstdio>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

streambuf * stdout_backup = NULL;
ofstream output_file;

string num2str(int number)
{
    char str[1000];
    sprintf(str,"%d",number);
    return string(str);
}

string num2str(size_t number)
{
    return num2str(int(number));
}

string num2str(double number)
{
    char str[1000];
    sprintf(str,"%G",number);
    return string(str);
}

int str2int(string str)
{
    return round(str2double(str));
}

double str2double(string str)
{
    double num;
    sscanf(str.c_str(),"%lf", &num);
    return num;
}

int get_integer_data(string strval, string strdefault)
{
    if(strval.size()==0) strval=strdefault;
    return str2int(strval);
}

double get_double_data(string strval, string strdefault)
{
    if(strval.size()==0) strval=strdefault;
    return str2double(strval);
}

string get_string_data(string strval, string strdefault)
{
    if(strval.size()==0) strval=strdefault;
    while(true)
    {
        size_t found = strval.find_first_of("\'\"");
        if(found==string::npos) break;
        strval.erase(found,1);
    }
    strval =  trim_string(strval);
    if(strval=="")
    {
        return strdefault;
    }
    else
    {
        return strval;
    }
}

string string2upper(string str)
{
    transform(str.begin(),str.end(),str.begin(),::toupper);
    return str;
}

double rad2deg(double angle)
{
    return angle*180.0/PI;
}

double deg2rad(double angle)
{
    return angle*PI/180.0;
}

double radps2hz(double w)
{
    return w/(2.0*PI);
}

double hz2radps(double f)
{
    return 2.0*PI*f;
}

string trim_string(string str)
{
    if(str.empty())
        return str;

    str.erase(0,str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r")+1);
    return str;
}

string replace_string_contents(string str, string source, string destination)
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

    size_t n = str.size();

    char* csv = (char*) malloc(sizeof(char)*n*2);
    size_t n2 = n*2;
    for(size_t i=0; i!=n2; ++i)
        csv[i]='\0';

    size_t ncsv = 0;
    for(size_t i=0; i!=n; ++i)
    {
        char source = str[i];
        if(source=='"')
        {
            csv[ncsv] = source;
            ncsv++;
            for(i++; i!=n; ++i)
            {
                source=str[i];
                if(source=='"')
                {
                    csv[ncsv] = source;
                    ncsv++;
                    break;
                }
                else
                {
                    csv[ncsv] = source;
                    ncsv++;
                }
            }
            continue;
        }
        else
        {
            if(source==',' or source==' ')
            {
                csv[ncsv]=',';
                ncsv++;
                continue;
            }
            else
            {
                csv[ncsv] = source;
                ncsv++;
                continue;
            }
        }
    }

    string newstr(csv);
    free(csv);
    return newstr;
}

vector<string> split_string(string str, const string sep)
{
    vector<string> splitted_str;
    str = trim_string(str);
    size_t newline_index = 0;
    while(true)
    {
        if(str.empty())
            break;
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
    size_t n_size = splitted_str.size();
    for(size_t i = 0; i!=n_size; ++i)
        splitted_str[i] = trim_string(splitted_str[i]);
    return splitted_str;
}


void redirect_stdout_to_file(string file)
{
    stdout_backup = cout.rdbuf();
    output_file.open(file);
    cout.rdbuf(output_file.rdbuf());
}

void recover_stdout()
{
    if(stdout_backup != NULL)
    {
        output_file.close();
        cout.rdbuf(stdout_backup);
        stdout_backup = NULL;
    }
}

complex<double> xy2dq_with_angle_in_deg(complex<double> V, double angle)
{
    angle = deg2rad(angle);
    return xy2dq_with_angle_in_rad(V, angle);
}

complex<double> xy2dq_with_angle_in_rad(complex<double> V, double angle)
{
    // (Vx+jVy)*(sin+jcos) =(Vx*sin-Vy*cos)+j(Vx*cos+Vy*sin)
    complex<double> rotation(sin(angle), cos(angle));
    return V*rotation;
}

complex<double> dq2xy_with_angle_in_deg(complex<double> V, double angle)
{
    angle = deg2rad(angle);
    return dq2xy_with_angle_in_rad(V, angle);
}

complex<double> dq2xy_with_angle_in_rad(complex<double> V, double angle)
{
    // (Vd+jVq)*(sin-jcos) =(Vd*sin+Vq*cos)+j(Vq*sin-Vd*cos)
    complex<double> rotation(sin(angle), -cos(angle));
    return V*rotation;
}

void initialize_simulator()
{
    ostringstream osstream;
    osstream<<"System started.";
    show_information_with_leading_time_stamp(osstream);
}

void reset_simulator()
{
    ostringstream osstream;
    osstream<<"Simulator reset.";
    show_information_with_leading_time_stamp(osstream);

    POWER_SYSTEM_DATABASE* db;
    for(size_t i=0; i!=MAX_DATABASE_COUNT; ++i)
    {
        db = STEPS::power_system_db+i;
        db->clear_database();
    }
}

void terminate_simulator()
{
    ostringstream osstream;
    osstream<<"System is terminated.";
    show_information_with_leading_time_stamp(osstream);
}

void show_test_information_for_function_of_class(string func, string cls)
{
    ostringstream osstream;
    //os<<"--------------------------------------------------------------------");
    osstream<<"--s--t--a--r--t-----------------------------------------------------";
    show_information_with_leading_time_stamp(osstream);
    osstream<<"Run testing of "<<cls<<"::"<<func;
    show_information_with_leading_time_stamp(osstream);
}

void show_test_end_information()
{
    ostringstream osstream;
    osstream<<"--d--o--n--e-------------------------------------------------------";
    show_information_with_leading_time_stamp(osstream);
}

void show_information_with_leading_time_stamp(string info)
{
    vector<string> splitted_info = split_string(info,"\n");
    size_t info_size = splitted_info.size();
    if(info_size==0)
        return;

    string sys_time = get_system_time_stamp_string();
    cout<<sys_time<<" "<<splitted_info[0]<<endl;

    for(size_t i=1; i!=info_size; ++i)
        cout<<sys_time<<" + "<<splitted_info[i]<<endl;
}

void show_information_with_leading_time_stamp(ostringstream& stream)
{
    show_information_with_leading_time_stamp(stream.str());
    stream.str("");
}

string get_system_time_stamp_string()
{
    time_t tt = time(NULL);
    tm* local_time= localtime(&tt);
    time_t clock_now = clock();

    double elapsed_time_in_s = double(clock_now-STEPS::clock_when_system_started)/double(CLOCKS_PER_SEC);

    char time_stamp[40];
    sprintf(time_stamp,"%d-%02d-%02d %02d:%02d:%02d [% 8.3f]", local_time->tm_year + 1900, local_time->tm_mon + 1,
            local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec, elapsed_time_in_s);
    return string(time_stamp);
}


bool is_file_exist(const string file)
{
    // check if the file exist
    // date: Sep. 1, 2016
    FILE* fid = fopen(file.c_str(),"r");
    if(fid==NULL)
    {
        return false;
    }
    else
    {
        fclose(fid);
        return true;
    }
}

POWER_SYSTEM_DATABASE* get_built_in_power_system_database_of_number(size_t i)
{
    if(i<10)
        return &(STEPS::power_system_db[i]);
    else
        return NULL;
}


POWER_SYSTEM_DATABASE* get_default_power_system_database()
{
    return &(STEPS::default_power_system_db);
}


POWERFLOW_SOLVER* api_get_default_powerflow_solver()
{
    return &STEPS::default_powerflow_solver;
}

DYNAMICS_SIMULATOR* api_get_default_dynamic_simulator()
{
    return &STEPS::default_dynamic_simulator;
}

void reset_power_system_database(size_t i)
{
    POWER_SYSTEM_DATABASE* db = get_built_in_power_system_database_of_number(i);
    if(db==NULL)
        return;

    db->clear_database();
}

void set_dynamic_simulation_time_step_in_s(double delt)
{
    STEPS::DELT = delt;
    ostringstream osstream;
    osstream<<"System dynamic simulation time step is set as :"<<STEPS::DELT<<" s.";
    show_information_with_leading_time_stamp(osstream);
}

double get_dynamic_simulation_time_step_in_s()
{
    return STEPS::DELT;
}

void set_dynamic_simulation_time_in_s(double time)
{
    STEPS::TIME = time;
}

double get_dynamic_simulation_time_in_s()
{
    return STEPS::TIME;
}
