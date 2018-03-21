#ifndef OPTIMAL_POWERFLOW_H
#define OPTIMAL_POWERFLOW_H

#include "header/device/bus.h"
#include "header/device/source.h"
#include "header/basic/sparse_matrix.h"
#include "header/network_database.h"

class OPTIMAL_POWERFLOW
{
    public:
        OPTIMAL_POWERFLOW();
        ~OPTIMAL_POWERFLOW();

        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;
        NETWORK_DATABASE* get_network_database() const;

        bool is_power_system_database_set() const;

        void initialize_powerflow_solver();

        void set_max_iteration(size_t iteration);
        void set_allowed_max_active_power_imbalance_in_MW(double P);
        void set_allowed_max_reactive_power_imbalance_in_MVar(double Q);
        void set_flat_start_logic(bool logic);
        void set_transformer_tap_adjustment_logic(bool logic);
        void set_iteration_accelerator(double alpha);
        void set_non_divergent_solution_logic(bool logic);

        size_t get_max_iteration() const;
        double get_allowed_max_active_power_imbalance_in_MW() const;
        double get_allowed_max_reactive_power_imbalance_in_MVar() const;
        bool get_flat_start_logic() const;
        bool get_transformer_tap_adjustment_logic() const;
        double get_iteration_accelerator() const;
        bool get_non_divergent_solution_logic() const;

        void solve_with_full_Newton_Raphson_solution();
        void solve_with_fast_decoupled_solution();
        void solve_with_modified_Gaussian_Seidel_solution();

        bool get_convergence_flag() const;
        bool is_converged() const;

        double get_maximum_active_power_mismatch_in_MW() const;
        double get_maximum_reactive_power_mismatch_in_MVar() const;

        void show_powerflow_result() const;
        void save_network_matrix_to_file(string filename) const;
        void save_jacobian_matrix_to_file(string filename) const;
        void save_bus_powerflow_result_to_file(string filename) const;
        size_t get_iteration_count() const;
    private:
        void initialize_bus_type_and_voltage_to_regulate();
        void set_bus_type_and_voltage_to_regulate_with_source(SOURCE& source);
        void initialize_bus_voltage();


        void optimize_bus_numbers();

        void update_P_and_Q_equation_internal_buses();
        void update_P_and_Q_equation_internal_buses_with_generator_variables();
        void remove_physical_bus_regulating_angle_from_internal_P_equation_buses(size_t bus);
        void remove_physical_bus_regulating_voltage_from_internal_Q_equation_buses(size_t bus);

        void set_bus_power_mismatch_vector_for_solution();

        void calculate_raw_bus_power_mismatch();
        void calculate_raw_bus_power_into_network();
        void calculate_raw_bus_current_into_network();

        bool check_bus_type_constraints();
        void check_SLACK_bus_constraint_of_physical_bus(size_t physical_bus);
        bool check_PV_bus_constraint_of_physical_bus(size_t physical_bus);
        bool check_PV_TO_PQ_bus_constraint_of_physical_bus(size_t physical_bus);
        void set_all_sources_at_physical_bus_to_q_min(size_t physical_bus);
        void set_all_sources_at_physical_bus_to_q_max(size_t physical_bus);

        vector<double> get_bus_power_mismatch_vector_for_coupled_solution();
        vector<double> get_bus_P_power_mismatch_vector_for_decoupled_solution();
        vector<double> get_bus_Q_power_mismatch_vector_for_decoupled_solution();

        void update_bus_voltage_and_angle(const vector<double>& update);
        void update_bus_voltage(const vector<double>& update);
        void update_bus_angle(const vector<double>& update);

        void add_source_to_bus_power_mismatch();
        void add_load_to_bus_power_mismatch();
        void add_hvdc_to_bus_power_mismatch();
        void add_generator_to_bus_power_mismatch();
        void add_pe_source_to_bus_power_mismatch();

        void extract_bus_power_mismatch_for_solution();

        void set_convergence_flag(bool flag);


        SPARSE_MATRIX jacobian, BP, BQ;

        vector<double> bus_active_power_mismatch_in_pu, bus_reactive_power_mismatch_in_pu;

        vector<size_t> internal_P_equation_buses, internal_Q_equation_buses;

        double P_threshold_in_MW, Q_threshold_in_MVar;
        bool flat_start_enabled, transformer_tap_adjustment_enabled;
        double alpha;
        bool non_divergent_solution_enabled;

        bool converged;

        vector< complex<double> > bus_current, bus_power;

        size_t iteration_count, max_iteration;
    private:
        POWER_SYSTEM_DATABASE*db ;
        NETWORK_DATABASE* network_db;
};
#endif // OPTIMAL_POWERFLOW_H
