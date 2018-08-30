#include "header/apis/steps_api.h"
#include "header/basic/utility.h"

void api_check_database()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    psdb->check_database();
}

void api_check_dynamic_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    psdb->check_dynamic_data();
}
