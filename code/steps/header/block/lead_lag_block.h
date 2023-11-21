#ifndef LEAD_LAG_BLOCK_H
#define LEAD_LAG_BLOCK_H

#include "header/block/block.h"
#include "header/block/first_order_block.h"

class LEAD_LAG_BLOCK : public BLOCK
{
    // K(1+sT1)/(1+sT2)
    public:
        LEAD_LAG_BLOCK(STEPS& toolkit);
        ~LEAD_LAG_BLOCK();

        void set_K(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);

        double get_K() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;

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
        void determine_block_integration_time_step_mode();
        void determine_block_integration_time_step();
        void determine_block_temp_variables();

        void initialize_normal_time_step_mode();
        void initialize_small_time_step_mode();
        void initialize_large_time_step_mode();

        void integrate();
        void integrate_normal_time_step_mode();
        void integrate_small_time_step_mode();
        void integrate_large_time_step_mode();

        void update();
        void update_normal_time_step_mode();
        void update_small_time_step_mode();
        void update_large_time_step_mode();

        double K, T1, T2;
        FIRST_ORDER_BLOCK first_order_block;
        double h;
        double h_over_2t1, h_over_2t2, t1_over_t2, t2_over_t1, one_over_t1, one_over_t2, one_over_1_plus_h_over_2t2;
        unsigned int count_of_time_slice_when_in_small_integration_time_step_mode;
};

#endif // LEAD_LAG_BLOCK_H
