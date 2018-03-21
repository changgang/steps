#ifndef INTEGRAL_BLOCK_H
#define INTEGRAL_BLOCK_H

#include "header/block/block.h"

class INTEGRAL_BLOCK : public BLOCK
{
    // 1/(sT)
    public:
        INTEGRAL_BLOCK();
        ~INTEGRAL_BLOCK();

        void set_T_in_s(double T);
        double get_T_in_s() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        double T;
};
#endif // INTEGRAL_BLOCK_H
