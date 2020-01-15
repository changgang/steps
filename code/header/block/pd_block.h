#ifndef PD_BLOCK_H
#define PD_BLOCK_H

#include "header/block/block.h"
#include "header/block/proportional_block.h"
#include "header/block/differential_block.h"

class PD_BLOCK : public BLOCK
{
    // Kp+sKd/(1+sTd)
    public:
        PD_BLOCK();
        ~PD_BLOCK();

        void set_Kp(double K);
        void set_Kd(double K);
        void set_Td_in_s(double T);

        double get_Kp() const;
        double get_Kd() const;
        double get_Td_in_s() const;

        double get_differentiator_state() const;
        double get_differentiator_new_state() const;
        double get_differentiator_dstate() const;
        double get_differentiator_store() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
        virtual void check();
    private:
        void integrate();
        void update();
        PROPORTIONAL_BLOCK p_block;
        DIFFERENTIAL_BLOCK d_block;
};
#endif // PD_BLOCK_H
