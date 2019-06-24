#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include "header/power_system_database.h"
void api_check_database(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check();
}

void api_check_dynamic_data(size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_dynamic_data();
}
