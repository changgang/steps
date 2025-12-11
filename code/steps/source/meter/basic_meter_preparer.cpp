#include "header/meter/basic_meter_preparer.h"
#include "header/basic/terminal.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;


bool basic_meter_preparer::prepare_wt_generator_meter(STEPS& toolkit, METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    STEPS_DEVICE_TYPE device_type = device_id.get_device_type();
    if(device_type!=STEPS_WT_GENERATOR)
    {
        osstream<<"Warning. This device given ("<<device_type2string(device_type)<<") is not a wt generator when trying to set up a wt generator meter. "
                <<"No wt generator meter will be added.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_wt_generator_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_compound_device_name()<<" does not exist in current power system database. "
                <<"No wt generator meter will be added.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}
