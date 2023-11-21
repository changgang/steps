#ifndef PROPORTIONAL_BLOCK_H
#define PROPORTIONAL_BLOCK_H

#include "header/block/block.h"

class PROPORTIONAL_BLOCK : public BLOCK
{
    // K
    public:
        PROPORTIONAL_BLOCK(STEPS& toolkit);
        ~PROPORTIONAL_BLOCK();

        void set_K(double K);
        double get_K() const;

        void initialize();
        void run(DYNAMIC_MODE mode);

        virtual void check();
    private:
        void integrate();
        void update();
        double K;
};
#endif // PROPORTIONAL_BLOCK_H
