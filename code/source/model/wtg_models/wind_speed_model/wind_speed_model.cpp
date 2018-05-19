#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"

#include "header/basic/utility.h"
#include <fstream>

WIND_SPEED_MODEL::WIND_SPEED_MODEL()
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
