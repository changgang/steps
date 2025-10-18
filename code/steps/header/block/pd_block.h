#ifndef PD_BLOCK_H
#define PD_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"
#include "header/block/differential_block.h"

class PD_BLOCK : public BLOCK
{
    // Kp+sKd/(1+sTd)
    public:
        PD_BLOCK(STEPS& toolkit);
        ~PD_BLOCK();

        void set_Kp(double K);
        void set_Kd(double K);
        void set_Td_in_s(double T);

        inline double get_Kp() const{return p_block.get_K();}
        inline double get_Kd() const{return d_block.get_K();}
        inline double get_Td_in_s() const{return d_block.get_T_in_s();}

        inline double get_differentiator_state() const{return d_block.get_state();}
        inline double get_differentiator_store() const{return d_block.get_store();}

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

        void update_simulation_time_step();

        PROPORTIONAL_BLOCK p_block;
        DIFFERENTIAL_BLOCK d_block;
};
#endif // PD_BLOCK_H
