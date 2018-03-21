#ifndef PROPORTIONAL_BLOCK_H
#define PROPORTIONAL_BLOCK_H

#include "header/block/block.h"

class PROPORTIONAL_BLOCK : public BLOCK
{
    // K
    public:
        PROPORTIONAL_BLOCK();
        ~PROPORTIONAL_BLOCK();

        void set_K(double K);
        double get_K() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        double K;
};
#endif // PROPORTIONAL_BLOCK_H
