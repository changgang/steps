#ifndef PLL_MODEL_H
#define PLL_MODEL_H

#include "header/model/common_model/common_model.h"

class PLL_MODEL:public COMMON_MODEL
{
    public:
        PLL_MODEL(STEPS& toolkit);
        ~PLL_MODEL();

        double get_pll_frequency_deviation_in_Hz() const;
        double get_pll_frequency_in_pu() const;
        double get_pll_frequency_in_Hz() const;
        double get_pll_angle_in_deg() const;

    public:
        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_pll_frequency_deviation_in_pu() const = 0;
        virtual double get_pll_angle_in_rad() const = 0;
};

#endif // PLL_MODEL_H
