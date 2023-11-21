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

        double get_linearized_system_variable(char var) const;
        double get_linearized_system_A() const;
        double get_linearized_system_B() const;
        double get_linearized_system_C() const;
        double get_linearized_system_D() const;
        double get_linearized_system_E() const;
        double get_linearized_system_F() const;
        double get_linearized_system_G() const;
        double get_linearized_system_H() const;

        virtual void check();
    private:
        void integrate();
        void update();
        PROPORTIONAL_BLOCK p_block;
        INTEGRAL_BLOCK i_block;
        DIFFERENTIAL_BLOCK d_block;
};
#endif // PID_BLOCK_H
