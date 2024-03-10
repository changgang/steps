#ifndef PROPORTIONAL_BLOCK_H
#define PROPORTIONAL_BLOCK_H

#include "header/block/block.h"

class PROPORTIONAL_BLOCK : public BLOCK
{
    // K
    public:
        PROPORTIONAL_BLOCK(STEPS& toolkit);
        ~PROPORTIONAL_BLOCK();

        void set_K(double K);
        double get_K() const;

        void initialize();
        void run(DYNAMIC_MODE mode);

        STEPS_SPARSE_MATRIX get_linearized_matrix_variable(char var) const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_A() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_B() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_C() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_D() const;

        virtual void check();
    private:
        void integrate();
        void update();
        double K;
};
#endif // PROPORTIONAL_BLOCK_H
