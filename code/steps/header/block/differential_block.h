#ifndef DIFFERENTIAL_BLOCK_H
#define DIFFERENTIAL_BLOCK_H

#include "header/block/block.h"
#include "header/basic/continuous_buffer.h"

class DIFFERENTIAL_BLOCK : public BLOCK
{
    // sK/(1+sT), a.k.a., washout
    public:
        DIFFERENTIAL_BLOCK(STEPS& toolkit);
        ~DIFFERENTIAL_BLOCK();

        void set_K(double K);
        void set_T_in_s(double T);

        double get_K() const;
        double get_T_in_s() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
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

        double K, T;

        double h;
        double k_over_t, t_over_h, one_over_t, h_over_t;
        CONTINUOUS_BUFFER history_output_for_large_time_step_integration;
        unsigned int count_of_time_slice_when_in_small_integration_time_step_mode;
};
#endif // DIFFERENTIAL_BLOCK_H
