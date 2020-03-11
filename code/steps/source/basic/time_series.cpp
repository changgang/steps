#include "header/basic/time_series.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

TIME_SERIES::TIME_SERIES()
{
    clear();
}

TIME_SERIES::TIME_SERIES(const TIME_SERIES& ts)
{
    copy_from_const_time_series(ts);
}

TIME_SERIES& TIME_SERIES::operator=(const TIME_SERIES& ts)
{
    if(this!=&ts) copy_from_const_time_series(ts);
    return *this;
}

void TIME_SERIES::copy_from_const_time_series(const TIME_SERIES& ts)
{
    clear();
    time = ts.get_time_vector();
    value = ts.get_value_vector();
    value_name = ts.get_value_name_vector();
}

TIME_SERIES::~TIME_SERIES()
{
    clear();
}

void TIME_SERIES::load_time_series_from_file(string file)
{
    FILE* fid = fopen(file.c_str(),"rt");
    if(fid != NULL)
    {
        clear();
        char buffer[1024];
        string sbuffer;

        fgets(buffer, 1024, fid);
        sbuffer = buffer;
        sbuffer = trim_string(sbuffer);
        sbuffer = string2upper(sbuffer);
        vector<string> header = split_string(sbuffer, ",");
        unsigned int n = header.size();
        for(unsigned int i=1; i<n; ++i)
            value_name.push_back(header[i]);

        vector<double> temp_value;
        while(true)
        {
            if(fgets(buffer, 1024, fid)==NULL)
            {
                fclose(fid);
                break;
            }
            sbuffer = buffer;
            sbuffer = trim_string(sbuffer);
            vector<string> value_string = split_string(sbuffer, ",");

            time.push_back(str2double(value_string[0]));
            temp_value.clear();
            for(unsigned int i=1; i<n; ++i)
                temp_value.push_back(str2double(value_string[i]));
            value.push_back(temp_value);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Time series file '"<<file<<"' is not accessible. Loading time series data is failed.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

void TIME_SERIES::clear()
{
    time.clear();
    value_name.clear();
    value.clear();
    current_time_index = 0;
}

vector<double> TIME_SERIES::get_values_at_time(double t)
{
    unsigned int n = time.size();
    if(n!=0)
    {
        if(current_time_index>=n) current_time_index=0;
        double current_time = time[current_time_index];
        if(fabs(current_time-t)<DOUBLE_EPSILON)
            return value[current_time_index];
        else
        {
            if(current_time<t)
            {
                unsigned int next_time_index = current_time_index+1;
                while(next_time_index<n)
                {
                    if(time[next_time_index]<t)
                    {
                        current_time_index = next_time_index;
                        ++next_time_index;
                    }
                    else
                        break;
                }
                if(next_time_index<n)
                {
                    double current_time = time[current_time_index];
                    double next_time = time[next_time_index];
                    vector<double> current_value=value[current_time_index];
                    vector<double> next_value=value[next_time_index];
                    unsigned int m = current_value.size();
                    for(unsigned int i=0; i<m; ++i)
                        current_value[i] = (current_value[i]*(next_time-t)+next_value[i]*(t-current_time))/(next_time-current_time);
                    return current_value;
                }
                else
                    return value[n-1];
            }
            else
            {
                if(current_time_index==0)
                    return value[0];

                unsigned int next_time_index = current_time_index-1;
                while(next_time_index!=0)
                {
                    if(time[next_time_index]>t)
                    {
                        current_time_index = next_time_index;
                        --next_time_index;
                    }
                    else
                        break;
                }
                if(next_time_index!=0)
                {
                    double current_time = time[current_time_index];
                    double next_time = time[next_time_index];
                    vector<double> current_value=value[current_time_index];
                    vector<double> next_value=value[next_time_index];
                    unsigned int m = current_value.size();
                    for(unsigned int i=0; i<m; ++i)
                        current_value[i] = (current_value[i]*(next_time-t)+next_value[i]*(t-current_time))/(next_time-current_time);
                    return current_value;
                }
                else
                    return value[0];
            }
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. No data in the time series. Returned value vector is empty and may be incorrect.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        vector<double> temp_v;
        return temp_v;
    }
}

double TIME_SERIES::get_value_of_name_at_time(string vname, double t)
{
    vname = string2upper(vname);
    unsigned int index = INDEX_NOT_EXIST;
    unsigned int n = value_name.size();
    for(unsigned int i=0; i<n; ++i)
    {
        if(value_name[i]==vname)
        {
            index = i;
            break;
        }
    }
    if(index!=INDEX_NOT_EXIST)
    {
        vector<double> vs = get_values_at_time(t);
        return vs[index];
    }
    else
        return 0.0;
}

vector<double> TIME_SERIES::get_time_vector() const
{
    return time;
}

vector< vector<double> > TIME_SERIES::get_value_vector() const
{
    return value;
}

vector<string> TIME_SERIES::get_value_name_vector() const
{
    return value_name;
}
