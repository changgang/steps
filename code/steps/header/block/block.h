#ifndef BLOCK_H
#define BLOCK_H

#include "header/basic/steps_enum.h"
#include "header/basic/sparse_matrix_define.h"
#include <vector>
#include <cctype>

using namespace std;

class STEPS;

class BLOCK
{
    public:
        BLOCK(STEPS& toolkit);
        virtual ~BLOCK();

        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        virtual void check() = 0;

        void set_input(double input);
        void set_output(double output);

        double get_input() const;
        double get_output() const;

        void set_limiter_type(LIMITER_TYPE limiter_type);
        void set_upper_limit(double vmax);
        void set_lower_limit(double vmin);

        LIMITER_TYPE get_limiter_type() const;
        double get_upper_limit() const;
        double get_lower_limit() const;

        void set_integration_time_step_mode(BLOCK_INTEGRATION_TIME_STEP_MODE mode);
        BLOCK_INTEGRATION_TIME_STEP_MODE get_integration_time_step_mode();

        void copy_current_input_to_old_input_in_last_time_step();
        double get_old_input_in_last_time_step() const;

        double get_state() const;
        double get_store() const;

        static void enable_automatic_large_time_step_logic();
        static void disable_automatic_large_time_step_logic();
        static bool get_automatic_large_time_step_logic();

        void check_limiter() const;

        void set_state_index(unsigned int index);
        unsigned int get_state_index() const;
    public:
        void set_state_WITH_CAUTION(double value);
    protected:
        void set_state(double value);
        void set_store(double value);
    private:
        STEPS* toolkit;
        double state, store;
        //double new_state, dstate;
        LIMITER_TYPE limiter_type;
        double upper_limit, lower_limit;
        double input, output;

        static bool automatic_large_time_step_logic;

        BLOCK_INTEGRATION_TIME_STEP_MODE integration_time_step_mode;
        double old_input_of_last_main_step;

        unsigned int state_index;
};
#endif // BLOCK_H
