#ifndef NETWORK_MATRIX_H
#define NETWORK_MATRIX_H

#include "header/power_system_database.h"
#include "header/basic/sparse_matrix_define.h"
#include "header/basic/inphno.h"
#include <string>


using namespace std;

class NETWORK_MATRIX
{
    public:
        NETWORK_MATRIX(STEPS& toolkit);
        ~NETWORK_MATRIX();
        STEPS& get_toolkit() const;

        void clear();

        void set_generator_reactance_option(GENERATOR_REACTANCE_OPTION option);
        GENERATOR_REACTANCE_OPTION get_generator_reactance_option();
        void set_consider_load_logic(bool logic);
        bool get_consider_load_logic();
        void set_consider_motor_load_logic(bool logic);
        bool get_consider_motor_load_logic();
        void set_option_of_dc_lines(DC_LINES_OPTION option);
        DC_LINES_OPTION get_option_of_dc_lines();

        void build_network_Y_matrix();
        void build_decoupled_network_B_matrix();
        void build_dc_network_B_matrix();
        void build_dynamic_network_Y_matrix();

        void build_sequence_network_Y_matrix();
        void build_positive_sequence_network_Y_matrix();
        void build_negative_sequence_network_Y_matrix();
        void build_zero_sequence_network_Y_matrix();

        void build_sequence_network_Z_matrix();
        void build_positive_sequence_network_Z_matrix();
        void build_negative_sequence_network_Z_matrix();
        void build_zero_sequence_network_Z_matrix();

        STEPS_COMPLEX_SPARSE_MATRIX& get_network_Y_matrix();
        STEPS_SPARSE_MATRIX& get_decoupled_network_BP_matrix();
        STEPS_SPARSE_MATRIX& get_decoupled_network_BQ_matrix();
        STEPS_SPARSE_MATRIX& get_dc_network_B_matrix();
        STEPS_COMPLEX_SPARSE_MATRIX& get_dynamic_network_Y_matrix();

        STEPS_COMPLEX_SPARSE_MATRIX& get_positive_sequence_network_Y_matrix();
        STEPS_COMPLEX_SPARSE_MATRIX& get_negative_sequence_network_Y_matrix();
        STEPS_COMPLEX_SPARSE_MATRIX& get_zero_sequence_network_Y_matrix();

        STEPS_COMPLEX_SPARSE_MATRIX& get_positive_sequence_network_Z_matrix();
        STEPS_COMPLEX_SPARSE_MATRIX& get_negative_sequence_network_Z_matrix();
        STEPS_COMPLEX_SPARSE_MATRIX& get_zero_sequence_network_Z_matrix();

        vector<complex<double> > get_positive_sequence_complex_impedance_of_column_with_physical_bus(unsigned int bus);
        vector<complex<double> > get_negative_sequence_complex_impedance_of_column_with_physical_bus(unsigned int bus);
        vector<complex<double> > get_zero_sequence_complex_impedance_of_column_with_physical_bus(unsigned int bus);


        complex<double> get_positive_sequence_self_admittance_of_physical_bus(unsigned int bus);
        complex<double> get_positive_sequence_mutual_admittance_between_physical_bus(unsigned int ibus, unsigned int jbus);
        complex<double> get_positive_sequence_self_impedance_of_physical_bus(unsigned int bus);
        complex<double> get_positive_sequence_mutual_impedance_between_physical_bus(unsigned int ibus, unsigned int jbus);

        complex<double> get_negative_sequence_self_admittance_of_physical_bus(unsigned int bus);
        complex<double> get_negative_sequence_mutual_admittance_between_physical_bus(unsigned int ibus, unsigned int jbus);
        complex<double> get_negative_sequence_self_impedance_of_physical_bus(unsigned int bus);
        complex<double> get_negative_sequence_mutual_impedance_between_physical_bus(unsigned int ibus, unsigned int jbus);

        complex<double> get_zero_sequence_self_admittance_of_physical_bus(unsigned int bus);
        complex<double> get_zero_sequence_mutual_admittance_between_physical_bus(unsigned int ibus, unsigned int jbus);
        complex<double> get_zero_sequence_self_impedance_of_physical_bus(unsigned int bus);
        complex<double> get_zero_sequence_mutual_impedance_between_physical_bus(unsigned int ibus, unsigned int jbus);

        void optimize_network_ordering();
        void check_network_connectivity(bool remove_void_island=false);
        vector< vector<unsigned int> > get_islands_with_internal_bus_number();
        vector< vector<unsigned int> > get_islands_with_physical_bus_number();

        unsigned int get_internal_bus_number_of_physical_bus(unsigned int bus) const;
        unsigned int get_physical_bus_number_of_internal_bus(unsigned int bus) const;

        void report_network_matrix();
        void report_decoupled_network_matrix() const;
        void report_dc_network_matrix() const;
        void report_dynamic_network_matrix();
        void report_positive_sequence_Y_matrix();
        void report_negative_sequence_Y_matrix();
        void report_zero_sequence_Y_matrix();
        void report_physical_internal_bus_number_pair() const;

        void save_network_Y_matrix_to_file(const string& filename, bool export_full_matrix=true);
        void save_decoupled_network_B_matrix_to_file(const string& filename) const;
        void save_dc_network_B_matrix_to_file(const string& filename) const;
        void save_dynamic_network_Y_matrix_to_file(const string& filename);
        void save_network_Z_matrix_to_file(const string& filename) const;
        void save_positive_sequence_Y_matrix_to_file(const string& filename);
        void save_negative_sequence_Y_matrix_to_file(const string& filename);
        void save_zero_sequence_Y_matrix_to_file(const string& filename);

        unsigned int get_memory_usage_in_bytes();
    private:
        void build_initial_zero_matrix(STEPS_COMPLEX_SPARSE_MATRIX& matrix);
        void build_initial_zero_matrix(STEPS_SPARSE_MATRIX& matrix);
        void add_lines_to_network();
        void add_transformers_to_network();
        void add_fixed_shunts_to_network();

        void add_line_to_network(const LINE& line);
        void add_transformer_to_network(const TRANSFORMER& trans);

        void add_two_winding_transformer_to_network(const TRANSFORMER& trans);
        void add_three_winding_transformer_to_network(const TRANSFORMER& trans);
        void add_two_winding_transformer_to_network_v2(const TRANSFORMER& trans);
        void add_fixed_shunt_to_network(const FIXED_SHUNT& shunt);

        void add_lines_to_decoupled_network();
        void add_transformers_to_decoupled_network();
        void add_fixed_shunts_to_decoupled_network();

        void add_line_to_decoupled_network(const LINE& line);
        void add_transformer_to_decoupled_network(const TRANSFORMER& trans);
        void add_three_winding_transformer_to_decoupled_network(const TRANSFORMER& trans);
        void add_two_winding_transformer_to_decoupled_network_v2(const TRANSFORMER& trans);
        void add_fixed_shunt_to_decoupled_network(const FIXED_SHUNT& shunt);

        void add_lines_to_dc_network();
        void add_transformers_to_dc_network();

        void add_line_to_dc_network(const LINE& line);
        void add_transformer_to_dc_network(const TRANSFORMER& trans);
        void add_three_winding_transformer_to_dc_network(const TRANSFORMER& trans);
        void add_two_winding_transformer_to_dc_network(const TRANSFORMER& trans);

        void add_bus_fault_to_dynamic_network();
        void add_lines_to_dynamic_network();
        void add_faulted_line_to_dynamic_network(const LINE& line);
        void add_generators_to_dynamic_network();
        void add_generator_to_dynamic_network(const GENERATOR& gen);
        void add_wt_generators_to_dynamic_network();
        void add_wt_generator_to_dynamic_network(WT_GENERATOR& gen);
        void add_pv_units_to_dynamic_network();
        void add_pv_unit_to_dynamic_network(PV_UNIT& pv_unit);
        void add_energy_storages_to_dynamic_network();
        void add_energy_storage_to_dynamic_network(ENERGY_STORAGE& es);
        void add_motor_loads_to_dynamic_network();
        void add_motor_load_to_dynamic_network(const LOAD& load);

        void add_vsc_hvdcs_to_dynamic_network();
        void add_vsc_hvdc_to_dynamic_network(const VSC_HVDC& vsc_hvdc);

//        void add_lines_to_sequence_network();
//        void add_faulted_line_to_sequence_network(const LINE& line);
//        void add_generators_to_sequence_network();
//        void add_generator_to_sequence_network(const GENERATOR& gen);
//        void add_wt_generators_to_sequence_network();
//        void add_wt_generator_to_sequence_network(const WT_GENERATOR& gen);

        void add_lines_to_positive_sequence_network();
        void add_faulted_line_to_positive_sequence_network(const LINE& line);
        void add_transformers_to_positive_sequence_network();
        void add_loads_to_positive_sequence_network();
        void add_load_to_positive_sequence_network(const LOAD& load);
        void add_sources_to_positive_sequence_network();
        void add_generators_to_positive_sequence_network();
        void add_generator_to_positive_sequence_network(const GENERATOR& gen);
        void add_wt_generators_to_positive_sequence_network();
        void add_wt_generator_to_positive_sequence_network(const WT_GENERATOR& wt_gen);
        void add_const_speed_wtg_to_positve_sequence_network(const WT_GENERATOR& wt_gen);
        void add_doubly_fed_wtg_to_positive_sequence_network(const WT_GENERATOR& wt_gen);
        void add_direct_driven_wtg_to_positive_sequence_network(const WT_GENERATOR& wt_gen);
        void add_pv_units_to_positive_sequence_network();
        void add_pv_unit_to_positive_sequence_network(const PV_UNIT& pv_unit);
        void add_energy_storages_to_positive_sequence_network();
        void add_energy_storage_to_positive_sequence_network(const ENERGY_STORAGE& es);
        void add_fixed_shunts_to_positive_sequence_network();
        void add_2t_lcc_hvdcs_to_positive_sequence_network();
        void add_2t_lcc_hvdc_to_positive_sequence_network(const LCC_HVDC2T& hvdc);
        void add_vsc_hvdcs_to_positive_sequence_network();
        void add_vsc_hvdc_to_positive_sequence_network(const VSC_HVDC& vsc_hvdc);

        void add_lines_to_negative_sequence_network();
        void add_faulted_line_to_negative_sequence_network(const LINE& line);
        void add_transformers_to_negative_sequence_network();
        void add_loads_to_negative_sequence_network();
        void add_load_to_negative_sequence_network(const LOAD& load);
        void add_sources_to_negative_sequence_network();
        void add_generators_to_negative_sequence_network();
        void add_generator_to_negative_sequence_network(const GENERATOR& gen);
        void add_wt_generators_to_negative_sequence_network();
        void add_wt_generator_to_negative_sequence_network(const WT_GENERATOR& wt_gen);
        void add_pv_units_to_negative_sequence_network();
        void add_pv_unit_to_negative_sequence_network(const PV_UNIT& pv_unit);
        void add_energy_storages_to_negative_sequence_network();
        void add_energy_storage_to_negative_sequence_network(const ENERGY_STORAGE& es);
        void add_fixed_shunts_to_negative_sequence_network();

        void add_lines_to_zero_sequence_network();
        void preprocess_mutual_data();
        void add_line_to_zero_sequence_network(const LINE& line);
        void add_faulted_line_to_zero_sequence_network(const LINE& line);
        void add_mutual_line_to_zero_sequence_network(const LINE& line);
        void add_transformers_to_zero_sequence_network();
        void add_transformer_to_zero_sequence_network(const TRANSFORMER& trans);
        void add_two_winding_transformer_to_zero_sequence_network(const TRANSFORMER& trans);
        void add_three_winding_transformer_to_zero_sequence_network(const TRANSFORMER& trans);
        void add_loads_to_zero_sequence_network();
        void add_load_to_zero_sequence_network(const LOAD& load);
        void add_sources_to_zero_sequence_network();
        void add_generators_to_zero_sequence_network();
        void add_generator_to_zero_sequence_network(const GENERATOR& gen);
        void add_wt_generators_to_zero_sequence_network();
        void add_wt_generator_to_zero_sequence_network(const WT_GENERATOR& wt_gen);
        void add_pv_units_to_zero_sequence_network();
        void add_pv_unit_to_zero_sequence_network(const PV_UNIT& pv_unit);
        void add_energy_storages_to_zero_sequence_network();
        void add_energy_storage_to_zero_sequence_network(const ENERGY_STORAGE& es);
        void add_fixed_shunts_to_zero_sequence_network();
        void add_fixed_shunt_to_zero_sequence_network(const FIXED_SHUNT& shunt);

        void set_this_Y_and_Z_matrix_as(STEPS_COMPLEX_SPARSE_MATRIX& matrix);
        void build_this_jacobian_for_getting_impedance_from_this_Y_matrix();
        void build_network_Z_matrix_from_this_Y_matrix();
        vector<double> get_impedance_of_column_from_this_Y_matrix(unsigned int col);
        vector<complex<double> > get_complex_impedance_of_column_from_this_Y_matrix(unsigned int col);
        complex<double> get_self_impedance_of_physical_bus_from_this_Y_matrix(unsigned int bus);
        complex<double> get_mutual_impedance_between_physical_bus_from_this_Y_matrix(unsigned int ibus, unsigned int jbus);


        bool is_condition_ok() const;
        void initialize_physical_internal_bus_pair();
        void reorder_physical_internal_bus_pair();

        void report_network_matrix_common() const;
        void save_network_matrix_common(ofstream& file) const;
        void save_full_network_matrix_common(ofstream& file) const;
    private:
        STEPS* toolkit;

        STEPS_COMPLEX_SPARSE_MATRIX network_Y1_matrix, network_Y2_matrix, network_Y0_matrix;
        STEPS_COMPLEX_SPARSE_MATRIX network_Z1_matrix, network_Z2_matrix, network_Z0_matrix;
        STEPS_SPARSE_MATRIX network_BP_matrix, network_BQ_matrix, network_DC_B_matrix;

        STEPS_COMPLEX_SPARSE_MATRIX* this_Y_matrix_pointer;
        STEPS_COMPLEX_SPARSE_MATRIX* this_Z_matrix_pointer;
        STEPS_SPARSE_MATRIX this_jacobian;
        INPHNO inphno;

        GENERATOR_REACTANCE_OPTION gen_X_option;
        bool consider_load;
        bool consider_motor_load;
        DC_LINES_OPTION option_of_all_ordinary_dc_lines;
};
#endif // NETWORK_MATRIX_H
