#ifndef SHORT_CIRCUIT_SOLVER_H
#define SHORT_CIRCUIT_SOLVER_H

#include "header/network/network_matrix.h"
#include "header/network/jacobian_builder.h"
#include "header/basic/sparse_matrix_define.h"

class BUS;
class SOURCE;
class SHORT_CIRCUIT_SOLVER : public BASE
{
    public:
        SHORT_CIRCUIT_SOLVER();
        ~SHORT_CIRCUIT_SOLVER();
        virtual void clear();

        void set_maximum_iteration(unsigned int itermax);
        unsigned int get_maximum_iteration() const;

        void initialize_short_circuit_solver();

        NETWORK_MATRIX& get_network_matrix();

        void append_fault(FAULT falut);

        void show_short_circuit_solver_configuration() const;

        complex<double> get_positive_sequence_equivalent_impedance_in_pu_of_fault(FAULT falut);
        complex<double> get_negative_sequence_equivalent_impedance_in_pu_of_fault(FAULT falut);
        complex<double> get_zero_sequence_equivalent_impedance_in_pu_of_fault(FAULT falut);

        void solve_positive_sequence_network();
        void solve_negative_sequence_network();
        void solve_zero_sequence_network();

        void show_short_circuit_result();
        void save_short_circuit_result_to_file(const string& filename) const;
        void save_extended_short_circuit_result_to_file(const string& filename) const;
    private:
        unsigned int iteration_count, max_iteration;
        bool converged;

        vector<FAULT> faults;
        vector< complex<double> > Z1, Z2, Z0;

        vector<BUS*> internal_bus_pointers;
    private:
        virtual bool is_valid() const;
        virtual void check();
};

#endif // SHORT_CIRCUIT_SOLVER_H
