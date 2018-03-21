#ifndef NETWORK_H
#define NETWORK_H

#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"
#include "header/device/generator.h"

#include "header/basic/sparse_matrix.h"

#include <time.h>

class NETWORK
{
    public:
        NETWORK();
        ~NETWORK();

        void build_network_matrix();
        void build_decoupled_network_matrix();
        void build_dynamic_network_matrix();
        void show_network_matrix() const;

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
};

#endif // NETWORK_H
