#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

PV_PANEL_MODEL::PV_PANEL_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
    set_Sref_in_Wpm2(1000.0);
    set_Tref_in_C_deg(25.0);
    set_S0_in_Wpm2(1000.0);
}

PV_PANEL_MODEL::PV_PANEL_MODEL(const PV_PANEL_MODEL& model) : PVU_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

PV_PANEL_MODEL::~PV_PANEL_MODEL()
{
    ;
}

void PV_PANEL_MODEL::copy_from_const_model(const PV_PANEL_MODEL& model)
{
    set_toolkit(model.get_toolkit());

    set_Sref_in_Wpm2(model.get_Sref_in_Wpm2());
}

string PV_PANEL_MODEL::get_model_type() const
{
    return "PV PANEL";
}

void PV_PANEL_MODEL::set_S0_in_Wpm2(double S)
{
    this->S0_Wpm2 = S;
}

void PV_PANEL_MODEL::set_Sref_in_Wpm2(double S)
{
    this->Sref_Wpm2 = S;
}

void PV_PANEL_MODEL::set_Tref_in_C_deg(double T)
{
    this->Tref_in_C_deg = T;
}

double PV_PANEL_MODEL::get_S0_in_Wpm2() const
{
    return S0_Wpm2;
}

double PV_PANEL_MODEL::get_Sref_in_Wpm2() const
{
    return Sref_Wpm2;
}


double PV_PANEL_MODEL::get_Tref_in_C_deg() const
{
    return this->Tref_in_C_deg;
}


double PV_PANEL_MODEL::get_solar_irradiance_in_Wpm2()
{
    return get_solar_irradiance_in_pu_based_on_S0()*get_S0_in_Wpm2();
}

double PV_PANEL_MODEL::get_solar_irradiance_in_pu_based_on_Sref()
{
    return get_solar_irradiance_in_Wpm2()/get_Sref_in_Wpm2();
}

double PV_PANEL_MODEL::get_solar_irradiance_in_pu_based_on_S0()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_IRRADIANCE_MODEL* pv_irrd = pv_unit->get_pv_irradiance_model();
    if(pv_irrd!=NULL)
    {
        if(not pv_irrd->is_model_initialized())
            pv_irrd->initialize();

        return pv_irrd->get_solar_irradiance_in_pu_based_on_S0();
    }
    else
        return 1.0;
}

double PV_PANEL_MODEL::get_pv_unit_terminal_active_power_generation_in_pu_based_on_mbase() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* pv_unitmodel = pv_unit->get_pv_converter_model();
    if(pv_unitmodel!=NULL)
    {
        if(not pv_unitmodel->is_model_initialized())
            pv_unitmodel->initialize();

        return pv_unitmodel->get_terminal_active_power_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double PV_PANEL_MODEL::get_active_power_generation_including_stator_loss_in_MW() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* pv_unitmodel = pv_unit->get_pv_converter_model();
    if(pv_unitmodel!=NULL)
    {
        if(not pv_unitmodel->is_model_initialized())
            pv_unitmodel->initialize();

        return pv_unitmodel->get_active_power_generation_including_stator_loss_in_MW();
    }
    else
        return 0.0;
}

double PV_PANEL_MODEL::get_temperature_in_C_deg() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_TEMPERATURE_MODEL* pv_t = pv_unit->get_pv_temperature_model();
    if(pv_t!=NULL)
    {
        if(not pv_t->is_model_initialized())
            pv_t->initialize();

        return pv_t->get_T_in_C_deg();
    }
    else
        return get_Tref_in_C_deg();
}

double PV_PANEL_MODEL::get_maximum_available_power_extracted_power_from_irradiance_in_MW()
{
    return get_maximum_available_power_extracted_power_from_irradiance_in_pu_on_mbase()*get_mbase_in_MVA();
}

double PV_PANEL_MODEL::get_pv_panel_power_in_MW()
{
    return get_pv_panel_power_in_pu_on_mbase()*get_mbase_in_MVA();
}
