#ifndef STEPS_H
#define STEPS_H

#include "header/power_system_database.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/network/network_matrix.h"
#include "header/basic/constants.h"
#include "header/apis/steps_api_search_buffer.h"
#include <ctime>
#include <string>

using namespace std;

class STEPS
{
    public:
        STEPS(string name="TK DFLT", string log_file="");
        virtual ~STEPS();
        void set_toolkit_name(string name);
        string get_toolkit_name() const;

        char get_next_alphabeta();
        void redirect_stdout_to_file(string file);
        void recover_stdout();

        void show_information_with_leading_time_stamp(string info);
        void show_information_with_leading_time_stamp(ostringstream& stream);
        void show_set_get_model_data_with_index_error(string device, string model, string func, size_t index);
        void show_set_get_model_data_with_name_error(string device, string model, string func, string par_name);
        string get_system_time_stamp_string();

        void clear();
        void reset();
        void terminate();

        POWER_SYSTEM_DATABASE& get_power_system_database();
        POWERFLOW_SOLVER& get_powerflow_solver();
        DYNAMICS_SIMULATOR& get_dynamic_simulator();
        NETWORK_MATRIX& get_network_matrix();

        void set_dynamic_simulation_time_step_in_s(double delt);
        double get_dynamic_simulation_time_step_in_s();
        void set_dynamic_simulation_time_in_s(double time);
        double get_dynamic_simulation_time_in_s();

    public:
        char steps_char_buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        STEPS_API_SEARCH_BUFFER api_search_buffer;

    private:
        string toolkit_name;

        POWER_SYSTEM_DATABASE power_system_db;

        POWERFLOW_SOLVER powerflow_solver;
        DYNAMICS_SIMULATOR dynamic_simulator;

        NETWORK_MATRIX network_matrix;

        time_t clock_when_system_started;

        streambuf * stdout_backup;
        ofstream output_file;
        char current_alphabeta;
};
#endif // STEPS_H
