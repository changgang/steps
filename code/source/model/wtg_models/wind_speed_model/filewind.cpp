#include "header/model/wtg_models/wind_speed_model/filewind.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;
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
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    time.clear();
    wind_speed.clear();
    wind_direction.clear();

    current_time = -INFINITE_THRESHOLD;
    current_wind_speed = 0.0;
    current_wind_direction = 0.0;

    set_previous_position(0);
}

void FILEWIND::copy_from_const_model(const FILEWIND& model)
{
    clear();
    set_wind_speed_serial_file(model.get_wind_speed_serial_file());
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

bool FILEWIND::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<4)
        return is_successful;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;

    string file;
    file = get_string_data(data[3],"0.0");

    set_wind_speed_serial_file(file);

    is_successful = true;
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


void FILEWIND::initialize()
{
    load_wind_speed_from_file();

    current_time = -INFINITE_THRESHOLD;

	if (wind_speed.size() == 0)
		current_wind_speed = 1.0;
	else
		current_wind_speed = wind_speed[0];

	if (wind_direction.size() == 0)
		current_wind_direction = 0.0;
	else
		current_wind_direction = wind_direction[0];

    set_previous_position(0);

    set_flag_model_initialized_as_true();
}


void FILEWIND::load_wind_speed_from_file()
{
    STEPS& toolkit = get_toolkit();
    ostringstream oosstream;
    string file = get_wind_speed_serial_file();
    if(file.size()<1)
    {
        oosstream<<"Initialization error. No file is provided for loading wind speed data. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(oosstream);
        return;
    }
    ifstream fid(file);
    if(not fid.is_open())
    {
        oosstream<<"Initialization error. Fail to load wind speed data from file '"<<file<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(oosstream);
        return;
    }

    time.clear();
    wind_speed.clear();
    wind_direction.clear();

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
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

        time.push_back(t);
        wind_speed.push_back(v);
        wind_direction.push_back(direction);
    }
    fid.close();
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
    if(time.size()==0)
        return 1.0;

    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    if(fabs(simulation_time-current_time)<FLOAT_EPSILON)
        return current_wind_speed;

    search_wind_data_at_simulation_time();

    return current_wind_speed;
}

double FILEWIND::get_wind_direction_in_deg()
{
    if(time.size()==0)
        return 0.0;

    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    if(fabs(simulation_time-current_time)<FLOAT_EPSILON)
        return current_wind_direction;

    search_wind_data_at_simulation_time();

    return current_wind_direction;
}

/*
double FILEWIND::get_wind_speed_in_pu()
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator==NULL)
        return 0.0;

    WT_AERODYNAMIC_MODEL* aero_model = generator->get_wt_aerodynamic_model();
    if(aero_model==NULL)
        return 0.0;

    double vwind = get_wind_speed_in_mps();
    return vwind/aero_model->get_nominal_wind_speed_in_mps();
}
*/
void FILEWIND::set_previous_position(size_t pos)
{
    if(pos<=time.size()-1)
        previous_position = pos;
    else
        previous_position = time.size()-1;
}

size_t FILEWIND::get_previous_position() const
{
    return previous_position;
}
void FILEWIND::search_wind_data_at_simulation_time()
{
    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    current_time = simulation_time;

    size_t n = time.size();
    double time_min = time[0];
    double time_max = time[n-1];

    if(current_time<=time_min)
    {
        current_wind_speed = wind_speed[0];
        current_wind_direction = wind_direction[0];
    }
    else
    {
        if(current_time>=time_max)
        {
            current_wind_speed = wind_speed[n-1];
            current_wind_direction = wind_direction[n-1];
        }
        else
        {
            size_t previous_index = 0, next_index = n-1;
            //size_t previous_time = time[previous_index], next_time = time[next_index];
            while(true)
            {
                size_t temp_index = ((previous_index+next_index)>>1);
                double temp_time = time[temp_index];
                if(fabs(temp_time-current_time)<FLOAT_EPSILON)
                {
                    current_wind_speed = wind_speed[temp_index];
                    current_wind_direction = wind_direction[temp_index];
                    break;
                }
                else
                {
                    if(temp_time>current_time)
                    {
                        next_index = temp_index;
                        //next_time = time[next_index];
                    }
                    else
                    {
                        previous_index = temp_index;
                        //previous_time = time[previous_index];
                    }

                    if(next_index-previous_index==1)
                    {
                        double slope;
                        slope = (wind_speed[next_index]-wind_speed[previous_index])/(time[next_index]-time[previous_index]);
                        current_wind_speed = wind_speed[previous_index]+slope*(current_time-time[previous_index]);

                        slope = (wind_direction[next_index]-wind_direction[previous_index])/(time[next_index]-time[previous_index]);
                        current_wind_direction = wind_direction[previous_index]+slope*(current_time-time[previous_index]);
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
    toolkit.show_information_with_leading_time_stamp(get_standard_model_string());
}

void FILEWIND::save()
{
    ;
}

string FILEWIND::get_standard_model_string() const
{
    ostringstream osstream;
    WT_GENERATOR* gen = get_wt_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();
    string file = get_wind_speed_serial_file();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<"'"<<file<<"'"
      <<"  /";
    return osstream.str();
}

void FILEWIND::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
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

void FILEWIND::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
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
    return get_standard_model_string();
}

string FILEWIND::get_dynamic_data_in_bpa_format() const
{
    return get_standard_model_string();
}

string FILEWIND::get_dynamic_data_in_steps_format() const
{
    return get_standard_model_string();
}
