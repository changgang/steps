#ifndef JACOBIAN_BUILDER_H
#define JACOBIAN_BUILDER_H

#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"

#include "header/basic/sparse_matrix.h"
#include "header/network_database.h"

#include <ctime>

class JACOBIAN_BUILDER
{
    public:
        JACOBIAN_BUILDER();
        ~JACOBIAN_BUILDER();

        void set_network_database(NETWORK_DATABASE* ndb);
        NETWORK_DATABASE* get_network_database() const;

        bool is_network_database_set() const;

        void build_seprate_jacobians();
        void update_seprate_jacobians();

        double get_jacobian_delta_p_over_angle_of_internal_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_p_over_voltage_of_internal_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_q_over_angle_of_internal_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_q_over_voltage_of_internal_bus(size_t ibus, size_t jbus);

        double get_jacobian_delta_p_over_angle_of_physical_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_p_over_voltage_of_physical_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_q_over_angle_of_physical_bus(size_t ibus, size_t jbus);
        double get_jacobian_delta_q_over_voltage_of_physical_bus(size_t ibus, size_t jbus);

        SPARSE_MATRIX& get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(const vector<size_t> internal_P_equation_buses,
                                                                                    const vector<size_t> internal_Q_equation_buses);

        SPARSE_MATRIX& get_decoupled_B_jacobian_with_P_equation_internal_buses(const vector<size_t> internal_P_equation_buses);

        SPARSE_MATRIX& get_decoupled_B_jacobian_with_Q_equation_internal_buses(const vector<size_t> internal_Q_equation_buses);


        void show_seprate_jacobians();
        void save_jacobian_matrix_to_file(string filename) const;
    private:
        NETWORK_DATABASE* network_db;
        void update_jacobian_delta_p_over_angle();
        void update_jacobian_delta_p_over_voltage();
        void update_jacobian_delta_q_over_angle();
        void update_jacobian_delta_q_over_voltage();

        SPARSE_MATRIX jacobian_delta_p_over_angle,
                      jacobian_delta_p_over_voltage,
                      jacobian_delta_q_over_angle,
                      jacobian_delta_q_over_voltage;
};


#endif // JACOBIAN_BUILDER_H
