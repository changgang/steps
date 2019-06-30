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

void api_check_network_connectivity(bool remove_void_island, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    network.build_network_matrix();
    network.check_network_connectivity(remove_void_island);
}
