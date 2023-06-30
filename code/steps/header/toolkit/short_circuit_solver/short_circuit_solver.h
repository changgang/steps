#ifndef SHORT_CIRCUIT_SOLVER_H
#define SHORT_CIRCUIT_SOLVER_H

#include "header/basic/complex3.h"
#include "header/network/network_matrix.h"
#include "header/network/jacobian_builder.h"
#include "header/basic/sparse_matrix_define.h"

class BUS;
class SOURCE;
class SHORT_CIRCUIT_SOLVER
{
    public:
        SHORT_CIRCUIT_SOLVER(STEPS& toolkit);
        ~SHORT_CIRCUIT_SOLVER();
        STEPS& get_toolkit() const;
        void clear();

        NETWORK_MATRIX& get_network_matrix();

        void initialize_short_circuit_solver();
        void show_short_circuit_solver_configuration();

        void set_generator_reactance_option(GENERATOR_REACTANCE_OPTION option);
        GENERATOR_REACTANCE_OPTION get_generator_reactance_option();
        void set_units_of_currents_and_voltages(UNITS_OPTION option);
        UNITS_OPTION get_units_of_currents_and_voltages();
        void set_coordinates_of_currents_and_voltages(COORDINATES_OPTION option);
        COORDINATES_OPTION get_coordinates_of_currents_and_voltages();
        void set_max_iteration_cout_for_iterative_method(unsigned int n);
        unsigned int get_max_iteration_count_for_iterative_method();
        void set_voltage_threshold_for_iterative_method(double threshold);
        double get_voltage_threshold_for_iterative_method();

        void set_consider_load_logic(bool logic);
        bool get_consider_load_logic();
        void set_consider_motor_load_logic(bool logic);
        bool get_consider_motor_load_logic();
        void set_option_of_DC_lines(DC_LINES_OPTION option);
        DC_LINES_OPTION get_option_of_DC_lines();

        void set_bus_fault(unsigned int bus, FAULT_TYPE type, const complex<double>& fault_shunt);
        void set_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location, FAULT_TYPE fault_type, const complex<double>& fault_shunt);
        void clear_fault();
        bool is_bus_fault();
        bool is_line_fault();
        bool is_fault();
        FAULT_TYPE get_fault_type();

        void build_sequence_network();

        complex<double> get_positive_sequence_thevenin_impedance_at_fault_in_pu();
        complex<double> get_negative_sequence_thevenin_impedance_at_fault_in_pu();
        complex<double> get_zero_sequence_thevenin_impedance_at_fault_in_pu();

        complex<double> get_positive_sequence_voltage_of_fault_point_in_pu();
        complex<double> get_negative_sequence_voltage_of_fault_point_in_pu();
        complex<double> get_zero_sequence_voltage_of_fault_point_in_pu();
        complex<double> get_positive_sequence_voltage_of_fault_point_in_kV();
        complex<double> get_negative_sequence_voltage_of_fault_point_in_kV();
        complex<double> get_zero_sequence_voltage_of_fault_point_in_kV();

        complex<double> get_positive_sequence_fault_current_in_pu();
        complex<double> get_negative_sequence_fault_current_in_pu();
        complex<double> get_zero_sequence_fault_current_in_pu();
        complex<double> get_positive_sequence_fault_current_in_kA();
        complex<double> get_negative_sequence_fault_current_in_kA();
        complex<double> get_zero_sequence_fault_current_in_kA();

        complex<double> get_positive_sequence_equivalent_fault_admittance();

        complex<double> get_positive_sequence_short_circuit_capacity_in_pu();
        complex<double> get_negative_sequence_short_circuit_capacity_in_pu();
        complex<double> get_zero_sequence_short_circuit_capacity_in_pu();

        void solve();
        void solve_with_iteration();

        complex<double> get_initial_voltage_of_fault_location_before_short_circuit();

        void show_short_circuit_result();
        void save_short_circuit_result_to_file(const string& filename);
        void save_extended_short_circuit_result_to_file(const string& filename);

        void report_positive_sequence_Y_matrix();
        void report_negative_sequence_Y_matrix();
        void report_zero_sequence_Y_matrix();
        void save_positive_sequence_Y_matrix_to_file(const string& filename);
        void save_negative_sequence_Y_matrix_to_file(const string& filename);
        void save_zero_sequence_Y_matrix_to_file(const string& filename);
    private:
        void store_bus_initial_voltage_before_short_circuit();
        void restore_bus_initial_voltage();

        complex<double> get_initial_voltage_of_bus_before_short_circuit(unsigned int bus);
        complex<double> get_initial_voltage_of_faulted_line_point_before_short_circuit();

        void update_internal_voltage_of_all_generators_and_wt_generators();
        void update_internal_voltage_of_all_generators();
        void update_internal_voltage_of_all_wt_generators();
        void update_all_motor_load_data();
        void updata_all_wt_generator_motor_data();

        void update_node_voltages_with_devices_equivalent_to_source();
        void add_generators_to_injection_current_vector();
        void add_wt_generators_to_injection_current_vector();
        void add_constant_speed_wt_generator_to_vector(WT_GENERATOR& wt_gen);
        void add_direct_driven_wt_generator_to_vector(WT_GENERATOR& wt_gen);
        void add_doubly_fed_wt_generator_to_vector(WT_GENERATOR& wt_gen);
        void add_pv_units_to_injection_current_vector();
        void add_energy_storages_to_injection_current_vector();
        void add_motor_load_to_injection_vector();
        void add_hvdcs_to_injection_current_vector();
        void add_vsc_hvdcs_to_injection_current_vector();
        void update_voltages_with_current_vector();
        void add_wt_generators_to_injection_current_vector_for_iterative_method();
        void add_pv_units_to_injection_current_vector_for_iterative_method();


        void calculate_and_store_equivalent_impedance_between_bus_and_fault_place();

        void solve_fault_current(complex<double> Uf);

        unsigned int get_internal_bus_number_of_physical_bus(unsigned int bus);
        unsigned int get_physical_bus_number_of_internal_bus(unsigned int bus);

        complex<double> get_positive_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus);
        complex<double> get_negative_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus);
        complex<double> get_zero_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus);

        void update_bus_sequence_voltage();

        void show_short_circuit_with_bus_fault();
        void show_contributions_of_fault_current_with_bus_fault();
        void show_short_circuit_with_line_fault();
        void show_contributions_of_fault_current_with_line_fault();
        void save_short_circuit_result_to_file_with_bus_fault(const string& filename);
        void save_short_circuit_result_to_file_with_line_fault(const string& filename);


        complex<double> get_positive_sequence_voltage_at_line_fault_location_in_pu();
        complex<double> get_negative_sequence_voltage_at_line_fault_location_in_pu();
        complex<double> get_zero_sequence_voltage_at_line_fault_location_in_pu();
        complex<double> get_positive_sequence_voltage_at_line_fault_location_in_kV();
        complex<double> get_negative_sequence_voltage_at_line_fault_location_in_kV();
        complex<double> get_zero_sequence_voltage_at_line_fault_location_in_kV();

        string get_fault_information();
        void show_current_table_header();
        void show_voltage_table_header();

        COMPLEX3 convert_sequence_data_to_phase_data(complex<double> pos, complex<double> neg, complex<double> zero);

        string get_formatted_sequence_data(unsigned int bus,string ID,complex<double> F1, complex<double>F2, complex<double> F0, bool to_file=false);
        string get_formatted_sequence_data(string str, complex<double> F1, complex<double> F2, complex<double> F0, bool to_file=false);
        string get_formatted_phase_data(string busname,double Vbase, complex<double> F1, complex<double>F2, complex<double> F0, bool to_file=false);
        string get_formatted_phase_data(string str, complex<double> F1, complex<double> F2, complex<double> F0, bool to_file=false);

        STEPS* toolkit;

        UNITS_OPTION units_of_currents_and_voltages;
        COORDINATES_OPTION coordindates_of_currents_and_volatges;

        BUS* faulted_bus_pointer;
        LINE* faulted_line_pointer;

        vector<complex<double>> bus_initial_voltage_before_short_circuit;

        complex<double> If1, If2, If0;
        complex<double> Z1, Z2, Z0;

        vector<complex<double> > Z1_between_internal_bus_to_fault_place, Z2_between_internal_bus_to_fault_place, Z0_between_internal_bus_to_fault_place;

        FAULT fault;
        vector<complex<double> > Yif_mutual, Yfj_mutual;
        vector<LINE*> lineptrs_of_mutual_with_line_fault;

        vector<complex<double> > injection_current_vector_with_internal_order;

        unsigned int max_iteration_count;
        double voltage_threshold_in_pu_for_iterative_method;
};

#endif // SHORT_CIRCUIT_SOLVER_H
