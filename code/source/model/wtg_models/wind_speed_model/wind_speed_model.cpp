#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"

#include "header/basic/utility.h"
#include <fstream>

WIND_SPEED_MODEL::WIND_SPEED_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
    set_wind_speed_serial_file("");
    time.clear();
    wind_speed.clear();
    wind_direction.clear();
    wind_speed_sigma.clear();
    current_time_pointer = 0;
}

WIND_SPEED_MODEL::~WIND_SPEED_MODEL()
{
    ;
}

string WIND_SPEED_MODEL::get_model_type() const
{
    return "WIND SPEED";
}
