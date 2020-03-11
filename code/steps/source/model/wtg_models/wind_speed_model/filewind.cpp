#include "header/model/wtg_models/wind_speed_model/filewind.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
#include <cstring>

using namespace std;
FILEWIND::FILEWIND(STEPS& toolkit) : WIND_SPEED_MODEL(toolkit)
{
    clear();
}

FILEWIND::~FILEWIND()
{
}

void FILEWIND::clear()
{
    clear_wind_speed_serial_file();
    clear_wind_data();

    current_time = -INFINITE_THRESHOLD;
    current_wind_speed = 0.0;
    current_wind_direction = 0.0;

    set_previous_position(0);
}


void FILEWIND::clear_wind_speed_serial_file()
{
    wind_speed_file[0] = '\0';
}

void FILEWIND::clear_wind_data()
{
    for(unsigned int i=0; i<STEPS_MAX_WIND_RECORD_SIZE; ++i)
    {
        wind_data[i][0]=INFINITE_THRESHOLD; // time
        wind_data[i][1]=0.0; // wind speed
        wind_data[i][2]=0.0; // wind direction
    }
}

void FILEWIND::copy_from_const_model(const FILEWIND& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    set_wind_speed_serial_file(model.get_wind_speed_serial_file());
    load_wind_speed_from_file();
}

FILEWIND::FILEWIND(const FILEWIND& model) : WIND_SPEED_MODEL(model.get_toolkit())
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
    strncpy(wind_speed_file, file.c_str(), STEPS_LONG_STRING_SIZE-1);
    wind_speed_file[STEPS_LONG_STRING_SIZE-1]='\0';
}

void FILEWIND::load_wind_speed_from_file()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    string file = get_wind_speed_serial_file();
    if(file.size()<1)
    {
        osstream<<"Initialization error. No file is provided for loading wind speed data. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    ifstream fid(file);
    if(fid.is_open())
    {
        clear_wind_data();

        string data;
        vector<string> datavec;
        getline(fid, data); // skip the head line
        unsigned int n = 0;
        while(true)
        {
            getline(fid, data);
            if(data.size()<3)
                break;
            data = trim_string(data);
            data = string2csv(data);
            datavec = split_string(data,",");
            if(datavec.size()<2)
                break;

            if(n==STEPS_MAX_WIND_RECORD_SIZE)
            {
                osstream<<"Warning. Wind data in file '"<<wind_speed_file<<"' has more records than preset size ("<<STEPS_MAX_WIND_RECORD_SIZE<<".\n"
                        <<"Only the first "<<STEPS_MAX_WIND_RECORD_SIZE<<" records will be loaded, and other records are discarded.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            double t = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());

            double v = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());

            double direction = 0.0;
            if(datavec.size()>0)
            {
                direction = get_double_data(datavec.front(),"0.0");
                datavec.erase(datavec.begin());
            }

            wind_data[n][0] = t;
            wind_data[n][1] = v;
            wind_data[n][2] = direction;

            ++n;
        }
        fid.close();
    }
    else
    {
        osstream<<"Initialization error. Fail to load wind speed data from file '"<<file<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

string FILEWIND::get_wind_speed_serial_file() const
{
    return wind_speed_file;
}

bool FILEWIND::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            string file;
            file = get_string_data(data[3],"0.0");

            set_wind_speed_serial_file(file);
            load_wind_speed_from_file();

            is_successful = true;
            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool FILEWIND::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool FILEWIND::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void FILEWIND::setup_block_toolkit_and_parameters()
{
    ;
}

void FILEWIND::initialize()
{
    setup_block_toolkit_and_parameters();

    current_time = -INFINITE_THRESHOLD;

	if (get_wind_record_count() == 0)
    {
        current_wind_speed = 1.0;
		current_wind_direction = 0.0;
    }
	else
    {
        current_wind_speed = wind_data[0][1];
		current_wind_direction = wind_data[0][2];
    }

    set_previous_position(0);

    set_flag_model_initialized_as_true();
}

unsigned int FILEWIND::get_wind_record_count() const
{
    unsigned int n = 0;
    for(unsigned int i=0; i<STEPS_MAX_WIND_RECORD_SIZE; ++i)
    {
        if(wind_data[i][0]==INFINITE_THRESHOLD)
        {
            n = i;
            break;
        }
    }
    return n;
}

void FILEWIND::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

double FILEWIND::get_wind_speed_in_pu()
{
    if(get_wind_record_count()==0)
        return 1.0;

    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    if(fabs(simulation_time-current_time)<DOUBLE_EPSILON)
        return current_wind_speed;

    search_wind_data_at_simulation_time();

    return current_wind_speed;
}

double FILEWIND::get_wind_direction_in_deg()
{
    if(get_wind_record_count()==0)
        return 0.0;

    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    if(fabs(simulation_time-current_time)<DOUBLE_EPSILON)
        return current_wind_direction;

    search_wind_data_at_simulation_time();

    return current_wind_direction;
}

void FILEWIND::set_previous_position(unsigned int pos)
{
    unsigned int n = get_wind_record_count();
    if(n>=1)
    {
        if(pos<=n-1)
            previous_position = pos;
        else
            previous_position = n-1;
    }
    else
        previous_position = 0;
}

unsigned int FILEWIND::get_previous_position() const
{
    return previous_position;
}

void FILEWIND::search_wind_data_at_simulation_time()
{
    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    current_time = simulation_time;

    unsigned int n = get_wind_record_count();
    double time_min = wind_data[0][0];
    double time_max = wind_data[n-1][0];

    if(current_time<=time_min)
    {
        current_wind_speed = wind_data[0][1];
        current_wind_direction = wind_data[0][2];
    }
    else
    {
        if(current_time>=time_max)
        {
            current_wind_speed = wind_data[n-1][1];
            current_wind_direction = wind_data[n-1][2];
        }
        else
        {
            unsigned int previous_index = 0, next_index = n-1;
            //unsigned int previous_time = (*time)[previous_index], next_time = (*time)[next_index];
            while(true)
            {
                unsigned int temp_index = ((previous_index+next_index)>>1);
                double temp_time = wind_data[temp_index][0];
                if(fabs(temp_time-current_time)<DOUBLE_EPSILON)
                {
                    current_wind_speed = wind_data[temp_index][1];
                    current_wind_direction = wind_data[temp_index][2];
                    break;
                }
                else
                {
                    if(temp_time>current_time)
                    {
                        next_index = temp_index;
                        //next_time = (*time)[next_index];
                    }
                    else
                    {
                        previous_index = temp_index;
                        //previous_time = (*time)[previous_index];
                    }

                    if(next_index-previous_index==1)
                    {
                        double slope;
                        double previous_time = wind_data[previous_index][0];
                        double next_time = wind_data[next_index][0];
                        double previous_speed = wind_data[previous_index][1];
                        double next_speed = wind_data[next_index][1];
                        double previous_direction = wind_data[previous_index][2];
                        double next_direction = wind_data[next_index][2];

                        double one_over_time_change = 1.0/(next_time-previous_time);
                        slope = (next_speed-previous_speed)*one_over_time_change;
                        current_wind_speed = previous_speed+slope*(current_time-previous_time);

                        slope = (next_direction-previous_direction)*one_over_time_change;
                        current_wind_direction = previous_direction+slope*(current_time-previous_time);
                        break;
                    }
                }
            }
        }
    }
}


void FILEWIND::check()
{
    ;
}


void FILEWIND::report()
{
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(get_standard_psse_string());
}

void FILEWIND::save()
{
    ;
}

string FILEWIND::get_standard_psse_string() const
{
    ostringstream osstream;
    WT_GENERATOR* gen = get_wt_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    string file = get_wind_speed_serial_file();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<"'"<<file<<"' /";
    return osstream.str();
}

void FILEWIND::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double FILEWIND::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void FILEWIND::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double FILEWIND::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void FILEWIND::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("WIND SPEED IN M/S", i); i++;
}

double FILEWIND::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="WIND SPEED IN M/S")
        return get_wind_speed_in_mps();
    return 0.0;
}

string FILEWIND::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string FILEWIND::get_dynamic_data_in_bpa_format() const
{
    return get_standard_psse_string();
}

string FILEWIND::get_dynamic_data_in_steps_format() const
{
    return get_standard_psse_string();
}
