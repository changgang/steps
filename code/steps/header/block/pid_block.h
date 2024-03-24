#ifndef PID_BLOCK_H
#define PID_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"
#include "header/block/integral_block.h"
#include "header/block/differential_block.h"

class PID_BLOCK : public BLOCK
{
    // Kp+Ki/s+sKd/(1+sTd)
    public:
        PID_BLOCK(STEPS& toolkit);
        ~PID_BLOCK();

        void set_Kp(double K);
        void set_Ki(double K);
        void set_Kd(double K);
        void set_Td_in_s(double T);

        double get_Kp() const;
        double get_Ki() const;
        double get_Kd() const;
        double get_Td_in_s() const;

        double get_integrator_state() const;
        double get_differentiator_state() const;
        double get_integrator_store() const;
        double get_differentiator_store() const;

        double get_state() const;
        double get_store() const;

        void initialize();
        void run(DYNAMIC_MODE mode);

        STEPS_SPARSE_MATRIX get_linearized_matrix_variable(char var) const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_A() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_B() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_C() const;
        STEPS_SPARSE_MATRIX get_linearized_matrix_D() const;

        virtual void check();

        void update_simulation_time_step();
    private:
        void integrate();
        void update();

        PROPORTIONAL_BLOCK p_block;
        INTEGRAL_BLOCK i_block;
        DIFFERENTIAL_BLOCK d_block;
};
#endif // PID_BLOCK_H
