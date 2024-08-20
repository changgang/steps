#include "header/model/wtg_models/wt_dbr_model/wt_dbr_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

WT_DBR_MODEL::WT_DBR_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    common_constructor();
}

WT_DBR_MODEL::~WT_DBR_MODEL()
{
    ;
}

void WT_DBR_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_WT_GENERATOR);

    set_initial_braking_resistor_energy_in_pu(1.0);
    set_braking_resistor_energy_threshold_in_pu(1.0);
}

string WT_DBR_MODEL::get_model_type() const
{
    return "WT DBR";
}

void WT_DBR_MODEL::set_initial_braking_resistor_energy_in_pu(double E)
{
    this->Edbr = E;
}

void WT_DBR_MODEL::set_braking_resistor_energy_threshold_in_pu(double E)
{
    this->Ebst = E;
}

double WT_DBR_MODEL::get_initial_braking_resistor_energy_in_pu() const
{
    return Edbr;
}

double WT_DBR_MODEL::get_braking_resistor_energy_threshold_in_pu() const
{
    return Ebst;
}

double WT_DBR_MODEL::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel!=NULL)
    {
        if(not genmodel->is_model_initialized())
            genmodel->initialize();

        return genmodel->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    }
    else
        return 0.0;

}

double WT_DBR_MODEL::get_active_power_command_in_pu_based_on_mbase() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel!=NULL)
    {
        if(not genmodel->is_model_initialized())
            genmodel->initialize();

        return genmodel->get_active_current_command_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double WT_DBR_MODEL::get_braking_resistor_power_in_MW()
{
    return get_braking_resistor_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}
