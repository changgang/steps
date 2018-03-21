#ifndef Y_MATRIX_BUILDER_H
#define Y_MATRIX_BUILDER_H

#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"
#include "header/device/generator.h"

#include "header/basic/sparse_matrix.h"

#include <time.h>

class Y_MATRIX_BUILDER
{
    public:
        Y_MATRIX_BUILDER();
        ~Y_MATRIX_BUILDER();

        SPARSE_MATRIX build_network_matrix();
        SPARSE_MATRIX build_dynamic_network_matrix();

        void build_decoupled_network_matrix();
        SPARSE_MATRIX get_decoupled_network_BP_matrix();
        SPARSE_MATRIX get_decoupled_network_BQ_matrix();
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

        void add_lines_to_dynamic_network();
        void add_faulted_line_to_dynamic_network(const LINE& line);
        void add_generators_to_dynamic_network();
        void add_generator_to_dynamic_network(const GENERATOR& gen);
    private:
        SPARSE_MATRIX network_Y_matrix, network_BP_matrix, network_BQ_matrix;
};

#endif // Y_MATRIX_BUILDER_H
