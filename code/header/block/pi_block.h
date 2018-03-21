#ifndef PI_BLOCK_H
#define PI_BLOCK_H

#include "header/block/block.h"
#include "header/block/pid_block.h"

class PI_BLOCK : public BLOCK
{
    // Kp+Ki/s
    public:
        PI_BLOCK();
        ~PI_BLOCK();

        void set_Kp(double K);
        void set_Ki(double K);

        double get_Kp() const;
        double get_Ki() const;


        void set_input(double input);
        void set_output(double output);

        double get_input() const;
        double get_output() const;

        double get_state() const;
        double get_new_state() const;
        double get_dstate() const;
        double get_store() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        PID_BLOCK pid_block;
};
#endif // PI_BLOCK_H
