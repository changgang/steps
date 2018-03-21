#ifndef LEAD_LAG_BLOCK_H
#define LEAD_LAG_BLOCK_H

#include "header/block/block.h"
#include "header/block/first_order_block.h"

class LEAD_LAG_BLOCK : public BLOCK
{
    // K(1+sT1)/(1+sT2)
    public:
        LEAD_LAG_BLOCK();
        ~LEAD_LAG_BLOCK();

        void set_K(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);

        double get_K() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        double K, T1, T2;
        FIRST_ORDER_BLOCK first_order_block;
};

#endif // LEAD_LAG_BLOCK_H
