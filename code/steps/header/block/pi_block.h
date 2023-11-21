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

        double get_linearized_system_variable(char var) const;
        double get_linearized_system_A() const;
        double get_linearized_system_B() const;
        double get_linearized_system_C() const;
        double get_linearized_system_D() const;
        double get_linearized_system_E() const;
        double get_linearized_system_F() const;
        double get_linearized_system_G() const;
        double get_linearized_system_H() const;

        virtual void check();

    private:
        void integrate();
        void update();
        PID_BLOCK pid_block;
};
#endif // PI_BLOCK_H
