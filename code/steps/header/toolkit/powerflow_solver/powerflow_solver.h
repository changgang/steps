#ifndef POWERFLOW_SOLVER_H
#define POWERFLOW_SOLVER_H

#include "header/network/network_matrix.h"
#include "header/network/jacobian_builder.h"
#include "header/basic/sparse_matrix_define.h"

class BUS;
class SOURCE;
class POWERFLOW_SOLVER
{
    public:
        POWERFLOW_SOLVER(STEPS& toolkit);
        ~POWERFLOW_SOLVER();

        STEPS& get_toolkit() const;

        void clear();

        NETWORK_MATRIX& get_network_matrix();

        void initialize_powerflow_solver();

        void set_max_iteration(unsigned int iteration);
        void set_allowed_max_active_power_imbalance_in_MW(double P);
        void set_allowed_max_reactive_power_imbalance_in_MVar(double Q);
        void set_maximum_voltage_change_in_pu(double v);
        void set_maximum_angle_change_in_deg(double a);
        void set_flat_start_logic(bool logic);
        void set_transformer_tap_adjustment_logic(bool logic);
        void set_iteration_accelerator(double alpha);
        void set_non_divergent_solution_logic(bool logic);
        void set_var_limit_check_logic(bool logic);
        void set_export_jacobian_matrix_step_by_step_logic(bool flag);

        unsigned int get_max_iteration() const;
        double get_allowed_max_active_power_imbalance_in_MW() const;
        double get_allowed_max_reactive_power_imbalance_in_MVar() const;
        double get_maximum_voltage_change_in_pu() const;
        double get_maximum_angle_change_in_deg() const;
        double get_maximum_angle_change_in_rad() const;
        bool get_flat_start_logic() const;
        bool get_transformer_tap_adjustment_logic() const;
        double get_iteration_accelerator() const;
        bool get_non_divergent_solution_logic() const;
        bool get_var_limit_check_logic() const;
        bool get_export_jacobian_matrix_step_by_step_logic() const;

        void show_powerflow_solver_configuration() const;

        void solve_with_full_Newton_Raphson_solution();
        void solve_with_fast_decoupled_solution();
        void solve_with_modified_Gaussian_Seidel_solution();

        bool get_convergence_flag() const;
        bool is_converged();

        double get_maximum_active_power_mismatch_in_MW() const;
        double get_maximum_reactive_power_mismatch_in_MVar() const;

        void show_powerflow_result();
        void save_powerflow_result_to_file(const string& filename) const;
        void save_extended_powerflow_result_to_file(const string& filename) const;
        void save_network_Y_matrix_to_file(const string& filename) const;
        void save_jacobian_matrix_to_file(const string& filename);
        void save_bus_powerflow_result_to_file(const string& filename) const;
        unsigned int get_iteration_count() const;

        unsigned int get_memory_usage_in_bytes();
    private:
        void prepare_devices_for_solution();
        void initialize_bus_type();
        void initialize_PQ_bus_type();
        void initialize_PV_bus_type();
        void initialize_SLACK_bus_type();
        void initialize_bus_voltage_to_regulate();
        void initialize_bus_voltage();

        void optimize_bus_numbers();
        void set_internal_bus_pointer();

        complex<double> get_bus_complex_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const;
        double get_bus_positive_sequence_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const;

        void update_P_and_Q_equation_internal_buses();
        void update_P_and_Q_equation_internal_buses_with_generator_variables();
        void remove_physical_bus_regulating_angle_from_internal_P_equation_buses(unsigned int bus);
        void remove_physical_bus_regulating_voltage_from_internal_Q_equation_buses(unsigned int bus);

        void set_bus_power_mismatch_vector_for_solution();

        void try_to_solve_dc_system_steady_state();
        void try_to_solve_hvdc_steady_state();
        void try_to_solve_vsc_hvdc_steady_state();
        void calculate_raw_bus_power_mismatch();
        void calculate_raw_bus_power_into_network();
        void calculate_raw_bus_current_into_network();

        bool check_bus_type_constraints();
        void check_SLACK_bus_constraint_of_physical_bus(unsigned int physical_bus);
        bool check_PV_bus_constraint_of_physical_bus(unsigned int physical_bus);
        bool check_PV_TO_PQ_bus_constraint_of_physical_bus(unsigned int physical_bus);
        void set_all_sources_at_physical_bus_to_q_min(unsigned int physical_bus);
        void set_all_sources_at_physical_bus_to_q_max(unsigned int physical_bus);
        void set_all_regulating_vsc_hvdcs_at_physical_bus_to_q_min(unsigned int physical_bus);
        void set_all_regulating_vsc_hvdcs_at_physical_bus_to_q_max(unsigned int physical_bus);

        void allocate_bus_power_mismatch_without_checking_bus_type_constraints();
        void allocate_SLACK_bus_power_mismatch_without_checking_bus_type_constraint(unsigned int physical_bus);
        void allocate_PV_bus_power_mismatch_without_checking_bus_type_constraint(unsigned int physical_bus);

        void build_bus_power_mismatch_vector_for_coupled_solution();
        void build_bus_P_power_mismatch_vector_for_decoupled_solution();
        void build_bus_Q_power_mismatch_vector_for_decoupled_solution();

        void update_bus_voltage_and_angle(vector<double>& update);
        void update_bus_voltage(vector<double>& update);
        void update_bus_angle(vector<double>& update);

        void add_source_to_bus_power_mismatch();
        void add_load_to_bus_power_mismatch();
        void add_hvdc_to_bus_power_mismatch();
        void add_vsc_hvdc_to_bus_power_mismatch();
        void add_generator_to_bus_power_mismatch();
        void add_wt_generator_to_bus_power_mismatch();

        void extract_bus_power_mismatch_for_solution();

        void set_convergence_flag(bool flag);

        STEPS* toolkit;

        JACOBIAN_BUILDER* jacobian_builder;

        STEPS_SPARSE_MATRIX jacobian, BP, BQ;

        vector<double> bus_active_power_mismatch_in_pu, bus_reactive_power_mismatch_in_pu;

        vector<unsigned int> internal_P_equation_buses, internal_Q_equation_buses;

        vector<double> S_mismatch, P_mismatch, Q_mismatch;

        double P_threshold_in_MW, Q_threshold_in_MVar;
        bool flat_start_enabled, transformer_tap_adjustment_enabled;
        double alpha;
        double maximum_voltage_change_in_pu, maximum_angle_change_in_deg;
        bool non_divergent_solution_enabled;
        bool var_limit_check_enabled;
        bool export_jacobian_matrix_step_by_step;

        bool converged;

        vector< complex<double> > bus_current, bus_power;

        unsigned int iteration_count, max_iteration;

        vector<BUS*> buses;
        vector<SOURCE*> sources;
        vector<GENERATOR*> generators;
        vector<WT_GENERATOR*> wt_generators;
        vector<PV_UNIT*> pv_units;
        vector<ENERGY_STORAGE*> e_storages;
        vector<LOAD*> loads;
        vector<LINE*> lines;
        vector<TRANSFORMER*> transformers;
        vector<HVDC*> hvdcs;
        vector<VSC_HVDC*> vsc_hvdcs;
        vector<EQUIVALENT_DEVICE*> e_devices;

        vector<BUS*> internal_bus_pointers;
};

#endif // POWERFLOW_SOLVER_H
