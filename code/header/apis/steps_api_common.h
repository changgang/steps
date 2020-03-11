#ifndef STEPS_API_COMMON_H
#define STEPS_API_COMMON_H


#include "header/STEPS.h"
#include "header/power_system_database.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"

void show_side_not_supported_for_device_with_api(string side, DEVICE_ID did, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);
void show_parameter_not_supported_for_device_with_api(string par_name, DEVICE_ID did, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);
void show_parameter_not_supported_with_api(string par_name, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);
void show_device_not_exist_with_api(DEVICE_ID did, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);
void show_model_type_not_supported_for_device_with_api(string model_type, DEVICE_ID did, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);

void show_parameter_not_supported_for_area_zone_owner_with_api(string par_name, unsigned int no, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);
void show_area_zone_owner_not_exist_with_api(unsigned int no, string api_func, unsigned int toolkit_index=INDEX_NOT_EXIST);

unsigned int get_owner_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
void set_owner_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
double get_owner_fraction_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
void set_owner_fraction_of_nonbus_device(NONBUS_DEVICE* device, string parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);

#endif // STEPS_API_COMMON_H
