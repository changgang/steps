#ifndef PI_BLOCK_H
#define PI_BLOCK_H

#include "header/block/block.h"
#include "header/block/pid_block.h"

class PI_BLOCK : public BLOCK
{
    // Kp+Ki/s
    public:
        PI_BLOCK(STEPS& toolkit);
        ~PI_BLOCK();

        void set_Kp(double K);
        void set_Ki(double K);

        inline double get_Kp() const{return pid_block.get_Kp();}
        inline double get_Ki() const{return pid_block.get_Ki();}

        inline double get_state() const{return pid_block.get_integrator_state();}
        inline double get_store() const{return pid_block.get_integrator_store();}

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

        PID_BLOCK pid_block;
};
#endif // PI_BLOCK_H
