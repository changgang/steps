#include "header/model/converter_common_model/converter_model/cs_converter_model/cs_converter_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
CS_CONVERTER_MODEL::CS_CONVERTER_MODEL(STEPS& toolkit) : CONVERTER_MODEL(toolkit)
{
    set_toolkit(toolkit);
}

CS_CONVERTER_MODEL::~CS_CONVERTER_MODEL()
{
    ;
}

void CS_CONVERTER_MODEL::set_Ipcmd_in_pu_based_on_mbase(double Ipcmd)
{
    this->Ipcmd = Ipcmd;
}

void CS_CONVERTER_MODEL::set_Iqcmd_in_pu_based_on_mbase(double Iqcmd)
{
    this->Iqcmd = Iqcmd;
}

void CS_CONVERTER_MODEL::set_Eqcmd_in_pu(double Eqcmd)
{
    this->Eqcmd = Eqcmd;
}

double CS_CONVERTER_MODEL::get_Ipcmd_in_pu_based_on_mbase() const
{
    return Ipcmd;
}

double CS_CONVERTER_MODEL::get_Iqcmd_in_pu_based_on_mbase() const
{
    return Iqcmd;
}

double CS_CONVERTER_MODEL::get_Eqcmd_in_pu() const
{
    return Eqcmd;
}

double CS_CONVERTER_MODEL::get_pll_angle_in_deg()
{
    return rad2deg(get_pll_angle_in_rad());
}

double CS_CONVERTER_MODEL::get_pll_frequency_deviation_in_Hz()
{
    return get_pll_frequency_deviation_in_pu()*get_base_frequency_in_Hz();
}

double CS_CONVERTER_MODEL::get_pll_frequency_in_pu()
{
    return 1.0 + get_pll_frequency_deviation_in_pu();
}

double CS_CONVERTER_MODEL::get_pll_frequency_in_Hz()
{
    return get_pll_frequency_in_pu()*get_base_frequency_in_Hz();
}
