#ifndef SECOND_ORDER_BLOCK_H
#define SECOND_ORDER_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"
#include "header/block/first_order_block.h"

class SECOND_ORDER_BLOCK : public BLOCK
{
    // (ds+e)/(as^2+bs+c)
    public:
        SECOND_ORDER_BLOCK();
        ~SECOND_ORDER_BLOCK();

        void set_a(double a);
        void set_b(double b);
        void set_c(double c);
        void set_d(double d);
        void set_e(double e);

        double get_a() const;
        double get_b() const;
        double get_c() const;
        double get_d() const;
        double get_e() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        void integrate();
        void update();
        float a, b, c, d, e;
        FIRST_ORDER_BLOCK block1, block2;

        double one_over_t, one_over_h, one_over_k, t_over_h, h_plus_2t, one_over_h_plus_2t, h_minus_2t;
};
#endif // SECOND_ORDER_BLOCK_H
