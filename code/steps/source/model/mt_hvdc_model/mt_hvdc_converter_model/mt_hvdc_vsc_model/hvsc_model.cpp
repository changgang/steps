#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_vsc_model/hvsc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

HVSC_MODEL::HVSC_MODEL(STEPS& toolkit) : MT_HVDC_MODEL(toolkit)
{
    ;
}

HVSC_MODEL::~HVSC_MODEL()
{
    ;
}



void HVSC_MODEL::set_hvsc_pointer(HVSC* vsc)
{
    vsc_ptr = vsc;
}

HVSC* HVSC_MODEL::get_hvsc_pointer_with_name(string name) const
{
    MT_HVDC* mtdc_ptr = get_mt_hvdc_pointer();
    MT_HVDC& mtdc = *mtdc_ptr;
    HVSC* vsc = mtdc.get_vsc_converter_with_name(name);
    return vsc;
}

HVSC* HVSC_MODEL::get_hvsc_pointer() const
{
    return vsc_ptr;
}

string HVSC_MODEL::get_model_type() const
{
    return "MT HVDC VSC MODEL";
}

void HVSC_MODEL::set_model_converter_name(const string name)
{
    hvsc_name = name;
}
string HVSC_MODEL::get_model_converter_name()
{
    return hvsc_name;
}

double HVSC_MODEL::get_dynamic_pll_angle_in_deg() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVSC* vsc_ptr = get_hvsc_pointer();
    HVSC& vsc = *vsc_ptr;
    double angle = psdb.get_bus_positive_sequence_angle_in_deg(vsc.get_converter_ac_bus());
    return angle;
}

double HVSC_MODEL::get_dynamic_pll_angle_in_rad() const
{
    double angle_in_deg = get_dynamic_pll_angle_in_deg();
    return deg2rad(angle_in_deg);
}
