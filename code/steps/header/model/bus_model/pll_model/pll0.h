#ifndef PLL0_H
#define PLL0_H

#include "header/model/bus_model/pll_model/pll_model.h"
#include "header/block/integral_block.h"

class PLL0 : public PLL_MODEL
{
    public:
        PLL0(STEPS& toolkit);
        ~PLL0();
        PLL0(const PLL0& model);
        virtual PLL0& operator=(const PLL0& model);
        void clear();
        void copy_from_const_model(const PLL0& model);
    public:
        void set_Kp(double K);
        void set_Ki(double K);
        void set_Pllmax(double P);
        void set_Pllmin(double P);

        double get_Kp() const;
        double get_Ki() const;
        double get_Pllmax() const;
        double get_Pllmin() const;

        double get_frquency_deviation_block_state() const;
        double get_angle_block_state() const;
    public:
        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual double get_pll_frequency_deviation_in_Hz() const;
        virtual double get_pll_angle_in_rad() const;
    private:
        double Kp;
        INTEGRAL_BLOCK frequency_deviation_in_Hz_block, angle_in_rad_block;
};

#endif // PLL0_H
