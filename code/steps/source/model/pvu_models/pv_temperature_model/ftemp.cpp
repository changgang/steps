#include "header/model/pvu_models/pv_temperature_model/ftemp.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
#include <cstring>

using namespace std;
FTEMP::FTEMP(STEPS& toolkit) : PV_TEMPERATURE_MODEL(toolkit)
{
    clear();
}

FTEMP::~FTEMP()
{
}

void FTEMP::clear()
{
    clear_temperature_serial_file();
    clear_temperature_data();

    current_time = -INFINITE_THRESHOLD;
    current_temperature = 0.0;

    set_previous_position(0);
}

void FTEMP::clear_temperature_serial_file()
{
    temperature_file[0] = '\0';
}

void FTEMP::clear_temperature_data()
{
    for(unsigned int i=0; i<STEPS_MAX_TEMPERATURE_RECORD_SIZE; ++i)
    {
        temperature_data[i][0]=INFINITE_THRESHOLD; // time
        temperature_data[i][1]=0.0; // temperature
    }
}

void FTEMP::copy_from_const_model(const FTEMP& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    set_temperature_serial_file(model.get_temperature_serial_file());
    load_temperature_from_file();
}

FTEMP::FTEMP(const FTEMP& model):PV_TEMPERATURE_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

FTEMP& FTEMP::operator=(const FTEMP& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string FTEMP::get_model_name() const
{
    return "FTEMP";
}


void FTEMP::set_temperature_serial_file(string file)
{
    strncpy(temperature_file, file.c_str(), STEPS_LONG_STRING_SIZE-1);
    temperature_file[STEPS_LONG_STRING_SIZE-1]='\0';
}

void FTEMP::load_temperature_from_file()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    string file = get_temperature_serial_file();
    if(file.size()<1)
    {
        osstream<<"Initialization error. No file is provided for loading temperature data. Check model "<<get_model_name()<<" of "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    ifstream fid(file);
    if(fid.is_open())
    {
        clear_temperature_data();

        string data;
        vector<string> datavec;
        getline(fid, data); // skip the head line
        unsigned int n = 0;
        while(true)
        {
            if(getline(fid, data))
                ;
            else
                break;

            if(data.size()<3)
                break;
            data = trim_string(data);
            data = string2csv(data);
            datavec = split_string(data,",");
            if(datavec.size()<2)
                break;

            if(n==STEPS_MAX_TEMPERATURE_RECORD_SIZE)
            {
                osstream<<"Warning. Temperature data in file '"<<temperature_file<<"' has more records than preset size ("<<STEPS_MAX_TEMPERATURE_RECORD_SIZE<<".\n"
                        <<"Only the first "<<STEPS_MAX_TEMPERATURE_RECORD_SIZE<<" records will be loaded, and other records are discarded.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
            double t = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());

            double T = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());

            temperature_data[n][0] = t;
            temperature_data[n][1] = T;

            ++n;
        }
        fid.close();
    }
    else
    {
        osstream<<"Initialization error. Fail to load temperature data from file '"<<file<<"'. Check model "<<get_model_name()<<" of "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

string FTEMP::get_temperature_serial_file() const
{
    return temperature_file;
}

bool FTEMP::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            string file;
            file = get_string_data(data[3],"0.0");

            set_temperature_serial_file(file);
            load_temperature_from_file();

            is_successful = true;
            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool FTEMP::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool FTEMP::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


void FTEMP::setup_block_toolkit_and_parameters()
{
    ;
}

void FTEMP::initialize()
{
    setup_block_toolkit_and_parameters();

    current_time = -INFINITE_THRESHOLD;

	if (get_temperature_record_count() == 0)
    {
        current_temperature = get_Tref_in_C_deg();
    }
	else
    {
        current_temperature = temperature_data[0][1];
    }

    set_previous_position(0);

    set_flag_model_initialized_as_true();
}

unsigned int FTEMP::get_temperature_record_count() const
{
    unsigned int n = 0;
    for(unsigned int i=0; i<STEPS_MAX_TEMPERATURE_RECORD_SIZE; ++i)
    {
        if(temperature_data[i][0]==INFINITE_THRESHOLD)
        {
            n = i;
            break;
        }
    }
    return n;
}

void FTEMP::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

double FTEMP::get_T_in_C_deg()
{
    if(get_temperature_record_count()==0)
        return get_Tref_in_C_deg();

    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    if(fabs(simulation_time-current_time)<DOUBLE_EPSILON)
        return current_temperature;

    search_temperature_data_at_simulation_time();

    return current_temperature;
}

void FTEMP::set_previous_position(unsigned int pos)
{
    unsigned int n = get_temperature_record_count();
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

unsigned int FTEMP::get_previous_position() const
{
    return previous_position;
}

void FTEMP::search_temperature_data_at_simulation_time()
{
    STEPS& toolkit = get_toolkit();
    double simulation_time = toolkit.get_dynamic_simulation_time_in_s();

    current_time = simulation_time;

    unsigned int n = get_temperature_record_count();
    double time_min = temperature_data[0][0];
    double time_max = temperature_data[n-1][0];

    if(current_time<=time_min)
    {
        current_temperature = temperature_data[0][1];
    }
    else
    {
        if(current_time>=time_max)
        {
            current_temperature = temperature_data[n-1][1];
        }
        else
        {
            unsigned int previous_index = 0, next_index = n-1;
            //unsigned int previous_time = (*time)[previous_index], next_time = (*time)[next_index];
            while(true)
            {
                unsigned int temp_index = ((previous_index+next_index)>>1);
                double temp_time = temperature_data[temp_index][0];
                if(fabs(temp_time-current_time)<DOUBLE_EPSILON)
                {
                    current_temperature = temperature_data[temp_index][1];
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
                        double previous_time = temperature_data[previous_index][0];
                        double next_time = temperature_data[next_index][0];
                        double previous_temperature = temperature_data[previous_index][1];
                        double next_temperature = temperature_data[next_index][1];

                        double one_over_time_change = 1.0/(next_time-previous_time);
                        slope = (next_temperature-previous_temperature)*one_over_time_change;
                        current_temperature = previous_temperature+slope*(current_time-previous_time);
                        break;
                    }
                }
            }
        }
    }
}


void FTEMP::check()
{
    ;
}


void FTEMP::report()
{
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(get_standard_psse_string());
}

void FTEMP::save()
{
    ;
}

string FTEMP::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    unsigned int bus = pv_unit->get_unit_bus();
    string identifier = "'"+pv_unit->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    string file = get_temperature_serial_file();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<"'"<<file<<"' /";
    return osstream.str();
}

void FTEMP::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double FTEMP::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void FTEMP::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double FTEMP::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void FTEMP::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("A", i); i++;
}

double FTEMP::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="A")
        return 0.0;
    return 0.0;
}

string FTEMP::get_dynamic_data_in_psse_format() const
{
    return get_standard_psse_string();
}

string FTEMP::get_dynamic_data_in_bpa_format() const
{
    return get_standard_psse_string();
}

string FTEMP::get_dynamic_data_in_steps_format() const
{
    return get_standard_psse_string();
}
