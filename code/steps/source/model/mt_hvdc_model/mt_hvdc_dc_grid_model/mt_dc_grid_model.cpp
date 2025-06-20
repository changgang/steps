#include "header/model/mt_hvdc_model/mt_hvdc_dc_grid_model/mt_dc_grid_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

MT_DC_GRID_MODEL::MT_DC_GRID_MODEL(STEPS& toolkit) : MT_HVDC_MODEL(toolkit)
{
    ;
}

MT_DC_GRID_MODEL::~MT_DC_GRID_MODEL()
{
    ;
}

DC_BUS* MT_DC_GRID_MODEL::get_dc_bus_pointer_with_bus_number(unsigned int bus) const
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    DC_BUS* dc_bus = mtdc->get_dc_bus_with_bus_number(bus);
    return dc_bus;
}

DC_LINE* MT_DC_GRID_MODEL::get_dc_line_pointer_with_bus_number(unsigned int ibus, unsigned int jbus) const
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    DC_LINE* dc_line = mtdc->get_dc_line_with_ibus_and_jbus(ibus, jbus);
    return dc_line;
}

string MT_DC_GRID_MODEL::get_model_type() const
{
    return "MT DC GRID MODEL";
}
