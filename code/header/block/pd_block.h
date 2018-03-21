#ifndef PD_BLOCK_H
#define PD_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"
#include "header/block/differential_block.h"

class PD_BLOCK : public BLOCK
{
    // Kp+sKd/Td
    public:
        PD_BLOCK();
        ~PD_BLOCK();

        void set_Kp(double K);
        void set_Kd(double K);
        void set_Td_in_s(double T);

        double get_Kp() const;
        double get_Kd() const;
        double get_Td_in_s() const;


        void set_input(double input);
        void set_output(double output);

        double get_input() const;
        double get_output() const;

        double get_differentiator_state() const;
        double get_differentiator_new_state() const;
        double get_differentiator_dstate() const;
        double get_differentiator_store() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();

    private:
        virtual void integrate();
        virtual void update();
        PROPORTIONAL_BLOCK p_block;
        DIFFERENTIAL_BLOCK d_block;
};
#endif // PD_BLOCK_H
