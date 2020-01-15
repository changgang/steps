#ifndef DIFFERENTIAL_BLOCK_H
#define DIFFERENTIAL_BLOCK_H

#include "header/block/block.h"

class DIFFERENTIAL_BLOCK : public BLOCK
{
    // sK/(1+sT), a.k.a., washout
    public:
        DIFFERENTIAL_BLOCK();
        ~DIFFERENTIAL_BLOCK();

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

        double k_over_t, t_over_h, one_over_t;
};
#endif // DIFFERENTIAL_BLOCK_H
