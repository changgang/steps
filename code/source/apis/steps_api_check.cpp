#include "header/apis/steps_api.h"
#include "header/basic/utility.h"

void api_check_database(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check();
}

void api_check_dynamic_data(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit_of_index(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_dynamic_data();
}
