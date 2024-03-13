#include "header/model/converter_common_models/converter_model/vs_converter_model/vsg_model/vsg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VSG_MODEL::VSG_MODEL(STEPS& toolkit) : VS_CONVERTER_MODEL(toolkit)
{
    set_toolkit(toolkit);
}

VSG_MODEL::~VSG_MODEL()
{
    ;
}


