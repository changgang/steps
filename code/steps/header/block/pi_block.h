#ifndef PI_BLOCK_H
#define PI_BLOCK_H

#include "header/block/block.h"
#include "header/block/pid_block.h"

class PI_BLOCK : public BLOCK
{
    // Kp+Ki/s
    public:
        PI_BLOCK(STEPS& toolkit);
        ~PI_BLOCK();

        void set_Kp(double K);
        void set_Ki(double K);

        double get_Kp() const;
        double get_Ki() const;

        double get_state() const;
        double get_store() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        void integrate();
        void update();
        PID_BLOCK pid_block;
};
#endif // PI_BLOCK_H
