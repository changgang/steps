#include "header/steps.h"
#include "header/basic/utility.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

STEPS::STEPS(string name, string log_file)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    clock_when_system_started=clock();

    if(log_file!="")
        redirect_stdout_to_file(log_file);

    toolkit_name = name;

    power_system_db.set_toolkit(*this);
    power_system_db.set_database_capacity();

    powerflow_solver.set_toolkit(*this);

    dynamic_simulator.set_toolkit(*this);

    network_matrix.set_toolkit(*this);

    show_information_with_leading_time_stamp("STEPS simulation toolkit "+toolkit_name+" is created @ "+num2str(int(this)));

    stdout_backup = NULL;

    clear();
}

STEPS::~STEPS()
{
    show_information_with_leading_time_stamp("STEPS simulation toolkit "+toolkit_name+" @ "+num2str(int(this))+" is deleted.");
    clear();
    recover_stdout();
}

void STEPS::set_toolkit_name(string name)
{
    if(toolkit_name!=name)
    {
        ostringstream osstream;
        osstream<<"STEPS toolkit "<<toolkit_name<<" is renamed as: "<<name;
        show_information_with_leading_time_stamp(osstream);
        toolkit_name = name;
    }
}

string STEPS::get_toolkit_name() const
{
    return toolkit_name;
}

void STEPS::clear()
{
    current_alphabeta = 'Z';

    power_system_db.clear();

    powerflow_solver.clear();
    dynamic_simulator.clear();

    recover_stdout();
}

void STEPS::reset()
{
    ostringstream osstream;
    osstream<<"STEPS simulation toolkit is reset.";
    show_information_with_leading_time_stamp(osstream);

    power_system_db.clear();
}

void STEPS::terminate()
{
    reset();
    ostringstream osstream;
    osstream<<"STEPS simulation toolkit is terminated.";
    show_information_with_leading_time_stamp(osstream);
}

POWER_SYSTEM_DATABASE& STEPS::get_power_system_database()
{
    return power_system_db;
}

POWERFLOW_SOLVER& STEPS::get_powerflow_solver()
{
    return powerflow_solver;
}

DYNAMICS_SIMULATOR& STEPS::get_dynamic_simulator()
{
    return dynamic_simulator;
}

NETWORK_MATRIX& STEPS::get_network_matrix()
{
    return network_matrix;
}

void STEPS::set_dynamic_simulation_time_step_in_s(double delt)
{
    dynamic_simulator.set_dynamic_simulation_time_step_in_s(delt);
}

double STEPS::get_dynamic_simulation_time_step_in_s()
{
    return dynamic_simulator.get_dynamic_simulation_time_step_in_s();;
}

void STEPS::set_dynamic_simulation_time_in_s(double time)
{
    dynamic_simulator.set_dynamic_simulation_time_in_s(time);
}

double STEPS::get_dynamic_simulation_time_in_s()
{
    return dynamic_simulator.get_dynamic_simulation_time_in_s();;
}

void STEPS::show_information_with_leading_time_stamp(ostringstream& stream)
{
    show_information_with_leading_time_stamp(stream.str());
    stream.str("");
}

void STEPS::show_information_with_leading_time_stamp(string info)
{
    vector<string> splitted_info = split_string(info,"\n");
    size_t info_size = splitted_info.size();
    if(info_size==0)
        return;

    string sys_time = get_system_time_stamp_string();
    cout<<"["<<get_toolkit_name()<<"]"<<sys_time<<" "<<splitted_info[0]<<"\n";

    for(size_t i=1; i!=info_size; ++i)
        cout<<"["<<get_toolkit_name()<<"]"<<sys_time<<" + "<<splitted_info[i]<<"\n";
}

string STEPS::get_system_time_stamp_string()
{
    time_t tt = time(NULL);
    tm* local_time= localtime(&tt);
    time_t clock_now = clock();

    double elapsed_time_in_s = double(clock_now-STEPS::clock_when_system_started)/double(CLOCKS_PER_SEC);

    char time_stamp[40];
    snprintf(time_stamp,40, "[%d-%02d-%02d %02d:%02d:%02d][% 8.3f]", local_time->tm_year + 1900, local_time->tm_mon + 1,
            local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec, elapsed_time_in_s);
    return string(time_stamp);
}


void STEPS::show_set_get_model_data_with_index_error(string device, string model, string func, size_t index)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    if(func=="set_model_data_with_index")
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Index %lu is out of range when calling %s:%s() for %s.\n0.0 will be returned.",
                 index, model.c_str(), func.c_str(), device.c_str());
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    if(func=="get_model_data_with_index")
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Index %lu is out of range when calling %s:%s() for %s.\nNothing will be set.",
                 index, model.c_str(), func.c_str(), device.c_str());
        show_information_with_leading_time_stamp(buffer);
        return;
    }
}

void STEPS::show_set_get_model_data_with_name_error(string device, string model, string func, string par_name)
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    if(func=="set_model_data_with_name")
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s is not supported when calling %s:%s() of %s.\n0.0 will be returned.",
                 par_name.c_str(), model.c_str(), func.c_str(), device.c_str());
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    if(func=="get_model_data_with_name")
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s is not supported when calling %s:%s() of %s.\nNothing will be set.",
                 par_name.c_str(), model.c_str(), func.c_str(), device.c_str());
        show_information_with_leading_time_stamp(buffer);
        return;
    }
}
char STEPS::get_next_alphabeta()
{
    current_alphabeta ++;
    if(current_alphabeta>'Z')
        current_alphabeta = 'A';
    return current_alphabeta;
}

void STEPS::redirect_stdout_to_file(string file)
{
    stdout_backup = cout.rdbuf();
    output_file.open(file);
    cout.rdbuf(output_file.rdbuf());
}

void STEPS::recover_stdout()
{
    if(stdout_backup != NULL)
    {
        output_file.close();
        cout.rdbuf(stdout_backup);
        stdout_backup = NULL;
    }
}
