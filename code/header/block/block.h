#ifndef BLOCK_H
#define BLOCK_H

#include "header/basic/steps_enum.h"
#include "header/basic/base.h"
#include <vector>


using namespace std;

class BLOCK
{
    public:
        BLOCK();
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
        double get_new_state() const;
        double get_dstate() const;
        double get_store() const;

        void check_limiter() const;
    public:
        void set_state_WITH_CAUTION(double value);
    protected:
        void set_state(double value);
        void set_new_state(double value);
        void set_dstate(double value);
        void set_store(double value);
    private:
        STEPS* toolkit;
        double state, new_state, dstate, store;
        LIMITER_TYPE limiter_type;
        double upper_limit, lower_limit;
        double input, output;
};
#endif // BLOCK_H
