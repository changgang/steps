#include "header/model/common_model/converter_common_model/pll_model/pll_model.h"

PLL_MODEL::PLL_MODEL(STEPS& toolkit):COMMON_MODEL(toolkit)
{

}

PLL_MODEL::~PLL_MODEL()
{
    ;
}

double PLL_MODEL::get_pll_frequency_deviation_in_Hz() const
{
    return get_pll_frequency_deviation_in_pu()*get_base_frequency_in_Hz();
}

double PLL_MODEL::get_pll_frequency_in_pu() const
{
    return 1.0+get_pll_frequency_deviation_in_pu();
}

double PLL_MODEL::get_pll_frequency_in_Hz() const
{
    return get_pll_frequency_in_pu()*get_base_frequency_in_Hz();
}

double PLL_MODEL::get_pll_angle_in_deg() const
{
    return rad2deg(get_pll_angle_in_rad());
}
