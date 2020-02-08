#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include "header/power_system_database.h"
void api_check_powerflow_data(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_powerflow_data();
}

void api_check_dynamic_data(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_dynamic_data();
}

void api_check_missing_models(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_missing_models();
}

void api_check_least_dynamic_time_constants(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DYNAMIC_MODEL_DATABASE& dmdb = toolkit.get_dynamic_model_database();
    dmdb.check_device_model_minimum_time_constants();
}

void api_check_network_connectivity(bool remove_void_island, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    network.build_network_Y_matrix();
    network.check_network_connectivity(remove_void_island);
}
