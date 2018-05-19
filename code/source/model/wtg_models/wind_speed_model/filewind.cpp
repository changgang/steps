#include "header/model/wtg_models/wind_speed_model/filewind.h"
#include "header/basic/utility.h"

static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@GOVERNOR",     //4
                                            "STATE@TURBINE",       //5
                                            };//8

FILEWIND::FILEWIND()
{
    clear();
}

FILEWIND::~FILEWIND()
{
    clear();
}

void FILEWIND::clear()
{
}

void FILEWIND::copy_from_const_model(const FILEWIND& model)
{
    clear();
}

FILEWIND::FILEWIND(const FILEWIND& model)
{
    copy_from_const_model(model);
}

FILEWIND& FILEWIND::operator=(const FILEWIND& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string FILEWIND::get_model_name() const
{
    return "FILEWIND";
}


void FILEWIND::set_wind_speed_serial_file(string file)
{
    wind_speed_file = file;
}

string FILEWIND::get_wind_speed_serial_file() const
{
    return wind_speed_file;
}


double FILEWIND::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double FILEWIND::get_double_data_with_name(string par_name) const
{
    return 0.0;
}

void FILEWIND::set_double_data_with_index(size_t index, double value)
{
}

void FILEWIND::set_double_data_with_name(string par_name, double value)
{
}

bool FILEWIND::setup_model_with_steps_string(string data)
{
    return false;
}

bool FILEWIND::setup_model_with_psse_string(string data)
{
    return false;
}

bool FILEWIND::setup_model_with_bpa_string(string data)
{
    return false;
}


void FILEWIND::initialize()
{
    return;
}


void FILEWIND::load_wind_speed_from_file()
{
    string file = get_wind_speed_serial_file();
    ifstream fid(file);
    if(not fid.is_open())
        return;

    time.clear();
    wind_speed.clear();
    wind_direction.clear();
    wind_speed_sigma.clear();

    string data;
    vector<string> datavec;
    fid>>data; // skip the head line
    while(true)
    {
        fid>>data;
        if(data.size()<3)
            break;
        data = trim_string(data);
        data = string2csv(data);
        datavec = split_string(data,",");
        if(datavec.size()<2)
            break;

        double t = get_double_data(datavec.front(),"0.0");
        datavec.erase(datavec.begin());

        double v = get_double_data(datavec.front(),"0.0");
        datavec.erase(datavec.begin());

        double direction = 0.0, sigma = 0.0;
        if(datavec.size()>0)
        {
            direction = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());
        }
        if(datavec.size()>0)
        {
            sigma = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());
        }

        time.push_back(t);
        wind_speed.push_back(v);
        wind_direction.push_back(direction);
        wind_speed_sigma.push_back(sigma);
    }

    fid.close();
    current_time_pointer = 0;
}

void FILEWIND::run(DYNAMIC_MODE mode)
{
    ;
}

double FILEWIND::get_wind_speed_in_mps() const
{
    if(time.size()==0)
        return 0.0;

    vector<double>::iterator it = time.begin();
    double time_min = *it;
    it = time.end(); it--;
    double time_max = *it;

    if(t<time_min)
    {
        it = wind_speed.begin();
        return *it;
    }

    if(t>time_max)
    {
        it = wind_speed.end(); it--;
        return *it;
    }

    double current_time = time.at(current_time_pointer);
    if(fabs(current_time-t)<FLOAT_EPSILON)
        return wind_speed.at(current_time_pointer);
    while(current_time>t and current_time>time_min)
    {
        current_time_pointer--;
        current_time = time.at(current_time_pointer);
    }




}
double FILEWIND::get_wind_speed_in_pu() const;

void FILEWIND::check();
void FILEWIND::clear();
void FILEWIND::report();
void FILEWIND::save();
string FILEWIND::get_standard_model_string() const;

size_t FILEWIND::get_variable_index_from_variable_name(string var_name);
string FILEWIND::get_variable_name_from_variable_index(size_t var_index);
double FILEWIND::get_variable_with_index(size_t var_index);
double FILEWIND::get_variable_with_name(string var_name);

string FILEWIND::get_dynamic_data_in_psse_format() const;
string FILEWIND::get_dynamic_data_in_bpa_format() const;
string FILEWIND::get_dynamic_data_in_steps_format() const;
