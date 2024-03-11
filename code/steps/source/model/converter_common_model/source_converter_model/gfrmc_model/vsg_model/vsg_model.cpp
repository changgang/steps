#include "header/model/converter_common_models/source_converter_model/gfrmc_model/vsg_model/vsg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VSG_MODEL::VSG_MODEL(STEPS& toolkit) : GFRMC_MODEL(toolkit)
{
    set_toolkit(toolkit);
}

VSG_MODEL::~VSG_MODEL()
{
    ;
}


