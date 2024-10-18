#include "header/model/energy_storage_model/es_soc_model/es_soc_model.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

ES_SOC_MODEL::ES_SOC_MODEL(STEPS& toolkit) : ES_MODEL(toolkit),
                                                     energy_state_block(toolkit)
{
    energy_state_block.set_limiter_type(NO_LIMITER);
    set_nominal_charge_time_in_s(200);
    set_charge_efficiency(1.0);
    set_discharge_efficiency(1.0);
    set_initial_SOC_in_pu(0.5);
    set_max_SOC_in_pu(0.8);
    set_min_SOC_in_pu(0.2);
}

ES_SOC_MODEL::~ES_SOC_MODEL()
{

}

string ES_SOC_MODEL::get_model_type() const
{
    return "ES SOC";
}

void ES_SOC_MODEL::set_energy_state_block_toolkit(STEPS& toolkit)
{
    energy_state_block.set_toolkit(toolkit);
}

void ES_SOC_MODEL::set_max_SOC_in_pu(double s)
{
    SOC_max = s;
}

void ES_SOC_MODEL::set_min_SOC_in_pu(double s)
{
    SOC_min = s;
}

void ES_SOC_MODEL::set_initial_SOC_in_pu(double s)
{
    SOC_initial = s;
}

void ES_SOC_MODEL::set_nominal_charge_time_in_s(double t)
{
    energy_state_block.set_T_in_s(t);
}

void ES_SOC_MODEL::set_charge_efficiency(double u)
{
    Kin = u;
}

void ES_SOC_MODEL::set_discharge_efficiency(double u)
{
    Kout = u;
}

double ES_SOC_MODEL::get_max_SOC_in_pu() const
{
    return SOC_max;
}

double ES_SOC_MODEL::get_min_SOC_in_pu() const
{
    return SOC_min;
}

double ES_SOC_MODEL::get_initial_SOC_in_pu() const
{
    return SOC_initial;
}

double ES_SOC_MODEL::get_nominal_charge_time_in_s() const
{
    return energy_state_block.get_T_in_s();
}

double ES_SOC_MODEL::get_charge_efficiency() const
{
    return Kin;
}

double ES_SOC_MODEL::get_discharge_efficiency() const
{
    return Kout;
}

double ES_SOC_MODEL::get_terminal_active_power_in_pu_based_on_mbase() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* model = energy_storage->get_es_converter_model();
    if(model!=NULL)
    {
        if(not model->is_model_initialized())
            model->initialize();

        return model->get_terminal_active_power_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double ES_SOC_MODEL::get_soc_power_in_pu_based_on_mbase()
{
    double P = get_terminal_active_power_in_pu_based_on_mbase();
    if(P>=0)
        return P/get_discharge_efficiency();
    else
        return P*get_charge_efficiency();
}

double ES_SOC_MODEL::get_soc_power_in_MW()
{
    return get_soc_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double ES_SOC_MODEL::get_state_of_charge_in_pu() const
{
    return energy_state_block.get_output();
}

INTEGRAL_BLOCK* ES_SOC_MODEL::get_energy_state_block()
{
    return &energy_state_block;
}
