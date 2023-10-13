#ifndef BLOCK_H
#define BLOCK_H

#include "header/basic/steps_enum.h"
#include <vector>

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

        double get_state() const;
        double get_store() const;

        static void enable_automatic_large_step_logic();
        static void disable_automatic_large_step_logic();
        static bool get_automatic_large_step_logic();

        void check_limiter() const;
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

        static bool automatic_large_step_logic;
};
#endif // BLOCK_H
