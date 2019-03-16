#ifndef NETWORK_MATRIX_H
#define NETWORK_MATRIX_H

#include "header/basic/base.h"
#include "header/power_system_database.h"
#include "header/basic/sparse_matrix_define.h"
#include "header/basic/inphno.h"
#include <string>


using namespace std;

class NETWORK_MATRIX : public BASE
{
    public:
        NETWORK_MATRIX();
        ~NETWORK_MATRIX();
        void clear_database();

        void build_network_matrix();
        void build_decoupled_network_matrix();
        void build_dc_network_matrix();
        void build_dynamic_network_matrix();

        STEPS_SPARSE_MATRIX& get_network_matrix();
        STEPS_SPARSE_MATRIX& get_decoupled_network_BP_matrix();
        STEPS_SPARSE_MATRIX& get_decoupled_network_BQ_matrix();
        STEPS_SPARSE_MATRIX& get_dc_network_matrix();
        STEPS_SPARSE_MATRIX& get_dynamic_network_matrix();

        void optimize_network_ordering();
        void check_newtork_connectivity();
        vector< vector<size_t> > get_islands_with_internal_bus_number();
        vector< vector<size_t> > get_islands_with_physical_bus_number();

        size_t get_internal_bus_number_of_physical_bus(size_t bus) const;
        size_t get_physical_bus_number_of_internal_bus(size_t bus) const;

        void report_network_matrix() const;
        void report_decoupled_network_matrix() const;
        void report_dc_network_matrix() const;
        void report_dynamic_network_matrix() const;
        void report_physical_internal_bus_number_pair() const;

        void save_network_matrix_to_file(string filename) const;
        void save_decoupled_network_matrix_to_file(string filename) const;
        void save_dc_network_matrix_to_file(string filename) const;
        void save_dynamic_network_matrix_to_file(string filename) const;
    private:
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

        bool is_condition_ok() const;
        void initialize_physical_internal_bus_pair();
        void reorder_physical_internal_bus_pair();

        void report_network_matrix_common() const;
        void save_network_matrix_common(ofstream& file) const;
    private:
        STEPS_SPARSE_MATRIX network_Y_matrix, network_BP_matrix, network_BQ_matrix, network_DC_B_matrix;
        INPHNO inphno;
    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
};
#endif // NETWORK_MATRIX_H
