#ifndef FIRST_ORDER_BLOCK_H
#define FIRST_ORDER_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"

class FIRST_ORDER_BLOCK : public BLOCK
{
    // K/(1+sT)
    public:
        FIRST_ORDER_BLOCK(STEPS& toolkit);
        ~FIRST_ORDER_BLOCK();

        void set_K(double K);
        void set_T_in_s(double T);

        double get_K() const;
        double get_T_in_s() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        void integrate();
        void update();
        double K, T;

        double one_over_t, one_over_h, one_over_k, t_over_h, h_plus_2t, one_over_h_plus_2t, h_minus_2t;
        double h;
};
#endif // FIRST_ORDER_BLOCK_H
