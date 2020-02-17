#ifndef STEPS_H
#define STEPS_H

#include "header/power_system_database.h"
#include "header/dynamic_model_database.h"
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
        STEPS(const string& name="TK DFLT", const string& log_file="");
        virtual ~STEPS();
        void set_toolkit_name(const string& name);
        string get_toolkit_name() const;

        void enable_use_steps_fast_math_logic();
        void disable_use_steps_fast_math_logic();
        bool get_use_steps_fast_math_logic();

        void set_thread_number(unsigned int n);
        unsigned int get_thread_number() const;

        void update_device_thread_number();
        unsigned int get_bus_thread_number() const;
        unsigned int get_generator_thread_number() const;
        unsigned int get_wt_generator_thread_number() const;
        unsigned int get_pv_unit_thread_number() const;
        unsigned int get_energy_storage_thread_number() const;
        unsigned int get_load_thread_number() const;
        unsigned int get_fixed_shunt_thread_number() const;
        unsigned int get_line_thread_number() const;
        unsigned int get_transformer_thread_number() const;
        unsigned int get_hvdc_thread_number() const;
        unsigned int get_vsc_hvdc_thread_number() const;
        unsigned int get_equivalent_device_thread_number() const;

        void set_dynamic_model_database_size_in_bytes(unsigned int n);
        unsigned int get_dynamic_model_database_size_in_bytes();

        char get_next_alphabeta();
        void open_log_file(const string& file, bool log_file_append_mode=false);
        void close_log_file();
        void enable_detailed_log();
        void disable_detailed_log();
        bool is_detailed_log_enabled();
        void enable_optimize_network();
        void disable_optimize_network();
        bool is_optimize_network_enabled();

        void show_information_with_leading_time_stamp(const string& info);
        void show_information_with_leading_time_stamp(ostringstream& stream);
        void show_set_get_model_data_with_index_error(const string& device, const string& model, const string& func, unsigned int index);
        void show_set_get_model_data_with_name_error(const string& device, const string& model, const string& func, const string& par_name);
        string get_system_time_stamp_string();

        void clear();
        void reset();
        void terminate();

        POWER_SYSTEM_DATABASE& get_power_system_database();
        DYNAMIC_MODEL_DATABASE& get_dynamic_model_database();
        POWERFLOW_SOLVER& get_powerflow_solver();
        DYNAMICS_SIMULATOR& get_dynamic_simulator();
        NETWORK_MATRIX& get_network_matrix();

        double get_system_base_power_in_MVA() const;
        double get_one_over_system_base_power_in_one_over_MVA() const;

        void set_dynamic_simulation_time_step_in_s(double delt);
        double get_dynamic_simulation_time_step_in_s();
        void set_dynamic_simulation_time_in_s(double time);
        double get_dynamic_simulation_time_in_s();

    public:
        char steps_char_buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        STEPS_API_SEARCH_BUFFER api_search_buffer;
    private:
        string toolkit_name;

        POWER_SYSTEM_DATABASE power_system_db;
        DYNAMIC_MODEL_DATABASE dynamic_model_db;

        POWERFLOW_SOLVER *powerflow_solver;
        DYNAMICS_SIMULATOR *dynamic_simulator;

        NETWORK_MATRIX network_matrix;

        time_t clock_when_system_started;

        ofstream log_file;
        bool detailed_log_enabled;
        bool optimize_network_enabled;
        char current_alphabeta;

        unsigned int thread_number;
        unsigned int generator_thread_number, wt_generator_thread_number, pv_unit_thread_number, energy_storage_thread_number,
               load_thread_number, fixed_shunt_thread_number, line_thread_number, transformer_thread_number, hvdc_thread_number, vsc_hvdc_thread_number,
               equivalent_device_thread_number;
        unsigned int dynamic_model_db_size;

};
#endif // STEPS_H
