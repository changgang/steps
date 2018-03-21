#ifndef NETWORK_OPTIMIZER_H
#define NETWORK_OPTIMIZER_H

#include "header/basic/inphno.h"
#include "header/basic/sparse_matrix.h"

class NETWORK_OPTIMIZER
{
    public:
        NETWORK_OPTIMIZER();
        ~NETWORK_OPTIMIZER();

        void optimize_network_ordering();
        void check_newtork_connectivity();
    private:
        bool is_condition_ok() const;
        void initialize_physical_internal_bus_pair();
        void reorder_physical_internal_bus_pair();

    private:
        SPARSE_MATRIX network_Y_matrix;
};


#endif // NETWORK_OPTIMIZER_H
