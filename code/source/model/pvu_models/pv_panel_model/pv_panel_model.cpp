#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include "header/basic/utility.h"
#include <iostream>

using namespace std;

PV_PANEL_MODEL::PV_PANEL_MODEL()
{
}

PV_PANEL_MODEL::PV_PANEL_MODEL(const PV_PANEL_MODEL& model)
{
    copy_from_const_model(model);
}

PV_PANEL_MODEL::~PV_PANEL_MODEL()
{

}

PV_PANEL_MODEL& PV_PANEL_MODEL::operator=(const PV_PANEL_MODEL& model)
{
    if(this==(&model))
        return *this;

    copy_from_const_model(model);
    return *this;
}

void PV_PANEL_MODEL::copy_from_const_model(const PV_PANEL_MODEL& model)
{

}

string PV_PANEL_MODEL::get_model_type() const
{
    return "PV PANEL";
}


void PV_PANEL_MODEL::initialize()
{
    ostringstream osstream;

    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit==NULL)
        return;
}

void PV_PANEL_MODEL::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    show_information_with_leading_time_stamp(osstream);
}

