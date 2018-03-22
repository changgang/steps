#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

void show_side_not_supported_for_device_with_api(string side, DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<"Side '"<<side<<"' is not supported for "<<did.get_device_name()<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_parameter_not_supported_for_device_with_api(string par_name, DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported for "<<did.get_device_name()<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_parameter_not_supported_with_api(string par_name, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_device_not_exist_with_api(DEVICE_ID did, string api_func)
{
    ostringstream sstream;
    sstream<<did.get_device_name()<<" does not exist in database with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}


void show_parameter_not_supported_for_area_zone_owner_with_api(string par_name, size_t no, string api_func)
{
    ostringstream sstream;
    sstream<<"Parameter '"<<par_name<<"' is not supported for Area/Zone/Owner "<<no<<" with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void show_area_zone_owner_not_exist_with_api(size_t no, string api_func)
{
    ostringstream sstream;
    sstream<<"Area/Zone/Owner "<<no<<" does not exist in database with api "<<api_func<<endl
           <<"Nothing will be changed."<<endl
           <<"If return value is expected, 0/0.0/False/EMPTY STRING will be returned"<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void api_initialize_package()
{
    initialize_simulator();
}

void api_clear_package()
{
    STEPS::power_system_db[0].clear_database();
}

void api_terminate_package()
{
    terminate_simulator();
}

POWER_SYSTEM_DATABASE* api_get_default_power_system_database()
{
    return &(STEPS::default_power_system_db);
}

