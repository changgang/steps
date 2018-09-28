#ifndef FIRST_ORDER_BLOCK_H
#define FIRST_ORDER_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"

class FIRST_ORDER_BLOCK : public BLOCK
{
    // K/(1+sT)
    public:
        FIRST_ORDER_BLOCK();
        ~FIRST_ORDER_BLOCK();

        void set_K(double K);
        void set_T_in_s(double T);

        double get_K() const;
        double get_T_in_s() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        double K, T;
};
#endif // FIRST_ORDER_BLOCK_H
