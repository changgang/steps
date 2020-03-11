#include "header/basic/model_index.h"
#include "header/basic/utility.h"

MODEL_INDEX::MODEL_INDEX()
{
    device_index = 0;
    icon_index   = 0;
    con_index    = 0;
    state_index  = 0;
    var_index    = 0;
}

void MODEL_INDEX::set_device_id(const DEVICE_ID& device_id)
{
    this->device_id = device_id;
    set_device_index();
}

void MODEL_INDEX::set_device_index()
{
    /*
    POWER_SYSTEM_DATABASE db;

    device_index = -1;

    string device_type = device_id.get_device_type();
    if(device_type=="GENERATOR")
    {
        device_index = db->get_generator_index(device_id);
        return;
    }
    if(device_type=="LOAD")
    {
        device_index = db->get_load_index(device_id);
        return;
    }
    if(device_type=="LINE")
    {
        device_index = db->get_line_index(device_id);
        return;
    }
    device_index = -1;*/
}

void MODEL_INDEX::set_icon_index(size_t index)
{
    icon_index = index;
}

void MODEL_INDEX::set_con_index(size_t index)
{
    con_index = index;
}

void MODEL_INDEX::set_state_index(size_t index)
{
    state_index = index;
}

void MODEL_INDEX::set_var_index(size_t index)
{
    var_index = index;
}

DEVICE_ID MODEL_INDEX::get_device_id() const
{
    return device_id;
}

size_t MODEL_INDEX::get_icon_index() const
{
    return icon_index;
}

size_t MODEL_INDEX::get_con_index() const
{
    return con_index;
}

size_t MODEL_INDEX::get_state_index() const
{
    return state_index;
}

size_t MODEL_INDEX::get_var_index() const
{
    return var_index;
}

size_t MODEL_INDEX::get_device_index() const
{
    return device_index;
}

