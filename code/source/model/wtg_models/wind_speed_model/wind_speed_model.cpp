#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"

#include "header/basic/utility.h"
#include <fstream>

WIND_SPEED_MODEL::WIND_SPEED_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");

}

WIND_SPEED_MODEL::~WIND_SPEED_MODEL()
{
    ;
}

string WIND_SPEED_MODEL::get_model_type() const
{
    return "WIND SPEED";
}

double WIND_SPEED_MODEL::get_nominal_wind_speed_in_mps() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_AERODYNAMIC_MODEL* aerd = gen->get_wt_aerodynamic_model();
        if(aerd!=NULL)
            return aerd->get_nominal_wind_speed_in_mps();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WIND_SPEED_MODEL::get_wind_speed_in_mps()
{
    double vwind0 = get_nominal_wind_speed_in_mps();
    double vwind = get_wind_speed_in_pu();
    return vwind*vwind0;
}
