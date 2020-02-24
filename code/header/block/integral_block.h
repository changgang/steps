#ifndef INTEGRAL_BLOCK_H
#define INTEGRAL_BLOCK_H

#include "header/block/block.h"

class INTEGRAL_BLOCK : public BLOCK
{
    // 1/(sT)
    public:
        INTEGRAL_BLOCK(STEPS& toolkit);
        ~INTEGRAL_BLOCK();

        void set_T_in_s(double T);
        double get_T_in_s() const;

        void initialize();
        void run(DYNAMIC_MODE);
        virtual void check();
    private:
        void integrate();
        void update();
        double T;

        double one_over_t, h_over_t;
};
#endif // INTEGRAL_BLOCK_H
