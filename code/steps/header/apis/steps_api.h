#ifndef STEPS_API_H
#define STEPS_API_H

#ifdef _MSC_VER
#define EXPORT_STEPS_DLL __declspec(dllexport)
#else
#define EXPORT_STEPS_DLL
#endif

#include <cstdlib>
#include "header/basic/constants.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXPORT_STEPS_DLL unsigned int api_get_const_INDEX_NOT_EXIST();

EXPORT_STEPS_DLL void api_set_toolkit_log_file(char* log_file, bool log_file_append_mode=false, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL unsigned int api_generate_new_toolkit(char* log_fie=NULL);
EXPORT_STEPS_DLL void api_delete_toolkit(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_initialize_toolkit(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_clear_toolkit(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_toolkit_parallel_thread_number(unsigned int n, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_toolkit_parallel_thread_number(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_toolkit_dynamic_model_database_capacity(unsigned int n, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_toolkit_dynamic_model_database_capacity(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL double api_get_toolkit_float_data(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_toolkit_float_data(char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_toolkit_string_data(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_toolkit_string_data(char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_toolkit_bool_data(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_toolkit_bool_data(char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL unsigned int api_get_allowed_maximum_bus_number(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_allowed_maximum_bus_number(unsigned int max_bus, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL unsigned int api_get_device_capacity(const char* device_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_area_capacity(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_zone_capacity(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_owner_capacity(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_device_capacity(const char* device_type, unsigned int cap, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_area_capacity(unsigned int cap, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_zone_capacity(unsigned int cap, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_owner_capacity(unsigned int cap, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL void api_load_powerflow_data_from_file(char* file, char* file_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_load_powerflow_result_from_file(char* file, char* file_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_powerflow_data_to_file(char* file, char* file_type, bool export_zero_impedance_line=true, bool export_out_of_service_bus=true, unsigned int powerflow_data_save_mode=0, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_load_dynamic_data_from_file(char* file, char* file_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_dynamic_data_to_file(char* file, char* file_type, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_add_bus(unsigned int bus_number, char* bus_name, double base_voltage_in_kV, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_wt_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_pv_unit(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_load(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_fixed_shunt(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_line(unsigned int sending_side_bus_number, unsigned int receiving_side_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_hvdc(unsigned int rectifier_bus_number, unsigned int inverter_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_transformer(unsigned int primary_side_bus_number, unsigned int secondary_side_bus_number, unsigned int tertiary_side_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_equivalent_device(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_energy_storage(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_area(unsigned int area_number, char* area_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_zone(unsigned int zone_number, char* zone_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_add_owner(unsigned int owner_number, char* owner_name, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL void api_remove_bus(unsigned int bus_number, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_wt_generator(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_pv_unit(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_load(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_fixed_shunt(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_line(unsigned int sending_side_bus_number, unsigned int receiving_side_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_hvdc(unsigned int rectifier_bus_number, unsigned int inverter_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_transformer(unsigned int primary_side_bus_number, unsigned int secondary_side_bus_number, unsigned int tertiary_side_bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_equivalent_device(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_energy_storage(unsigned int bus_number, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_area(unsigned int area_number, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_zone(unsigned int zone_number, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_remove_owner(unsigned int owner_number, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_change_bus_number(unsigned int original_bus_number, unsigned int new_bus_number, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_change_bus_number_with_file(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL bool api_is_bus_exist(unsigned int bus, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_generator_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_wt_generator_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_pv_unit_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_load_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_fixed_shunt_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_line_exist(unsigned int ibus, unsigned int jbus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_transformer_exist(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_hvdc_exist(unsigned int ibus, unsigned int jbus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_equivalent_device_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_energy_storage_exist(unsigned int bus, char* ickt, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL unsigned int api_get_device_count(const char* device_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_area_count(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_zone_count(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_owner_count(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_in_service_bus_count(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_update_overshadowed_buses(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_all_buses_un_overshadowed(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_overshadowed_bus_count(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_show_device_data(const char* device_type, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL unsigned int api_bus_name2bus_number(const char* bus_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_bus_number2bus_name(unsigned int bus_number, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL void api_initialize_bus_search(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_initialize_all_bus_search(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_current_bus_number(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_goto_next_bus(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_initialize_device_search(const char* device_type, unsigned int bus, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_current_device_bus_number(const char* device_type, const char* side, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_current_device_identifier(const char* device_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_goto_next_device(const char* device_type, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_initialize_area_search(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_current_area_number(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_goto_next_area(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_initialize_zone_search(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_current_zone_number(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_goto_next_zone(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_initialize_owner_search(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_current_owner_number(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_goto_next_owner(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_bus_integer_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_bus_integer_data(unsigned int bus, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_bus_float_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_bus_float_data(unsigned int bus, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_bus_string_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_bus_string_data(unsigned int bus, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_bus_boolean_data(unsigned int bus, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_bus_boolean_data(unsigned int bus, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_source_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_source_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_source_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_source_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_source_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_source_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_source_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_source_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_load_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_load_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_load_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_load_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_load_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_load_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_load_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_load_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_fixed_shunt_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_fixed_shunt_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_fixed_shunt_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_fixed_shunt_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_fixed_shunt_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_fixed_shunt_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_fixed_shunt_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_fixed_shunt_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_line_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_line_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_line_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_line_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_line_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_line_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_line_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_line_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_transformer_integer_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_transformer_integer_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_transformer_float_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_transformer_float_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_transformer_string_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_transformer_string_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_transformer_boolean_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_transformer_boolean_data(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, char* side, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_hvdc_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_integer_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_hvdc_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_float_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_hvdc_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_string_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_hvdc_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_boolean_data(unsigned int ibus, unsigned int jbus, char* identifier, char* side, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_equivalent_device_integer_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_equivalent_device_integer_data(unsigned int bus, char* identifier, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_equivalent_device_float_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_equivalent_device_float_data(unsigned int bus, char* identifier, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_equivalent_device_string_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_equivalent_device_string_data(unsigned int bus, char* identifier, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_equivalent_device_boolean_data(unsigned int bus, char* identifier, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_equivalent_device_boolean_data(unsigned int bus, char* identifier, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_area_integer_data(unsigned int area, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_area_integer_data(unsigned int area, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_area_float_data(unsigned int area, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_area_float_data(unsigned int area, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_area_string_data(unsigned int area, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_area_string_data(unsigned int area, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_area_boolean_data(unsigned int area, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_area_boolean_data(unsigned int area, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_zone_integer_data(unsigned int zone, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_zone_integer_data(unsigned int zone, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_zone_float_data(unsigned int zone, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_zone_float_data(unsigned int zone, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_zone_string_data(unsigned int zone, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_zone_string_data(unsigned int zone, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_zone_boolean_data(unsigned int zone, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_zone_boolean_data(unsigned int zone, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL int api_get_owner_integer_data(unsigned int owner, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_owner_integer_data(unsigned int owner, char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_owner_float_data(unsigned int owner, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_owner_float_data(unsigned int owner, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_owner_string_data(unsigned int owner, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_owner_string_data(unsigned int owner, char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_owner_boolean_data(unsigned int owner, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_owner_boolean_data(unsigned int owner, char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_dynamic_model(char* model_string, char* file_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_disable_generator_related_model(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_enable_generator_related_model(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_generator_related_model_name(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_generator_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_generator_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_generator_related_model_float_parameter_count(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_generator_related_model_float_parameter_name(unsigned int bus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_wt_generator_related_model_name(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_wt_generator_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_wt_generator_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_wt_generator_related_model_float_parameter_count(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_wt_generator_related_model_float_parameter_name(unsigned int bus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_pv_unit_related_model_name(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_pv_unit_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_pv_unit_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_pv_unit_related_model_float_parameter_count(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_pv_unit_related_model_float_parameter_name(unsigned int bus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_load_related_model_name(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_load_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_load_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_load_related_model_float_parameter_count(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_load_related_model_float_parameter_name(unsigned int bus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_line_related_model_name(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_line_related_model_float_parameter(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_line_related_model_float_parameter(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_line_related_model_float_parameter_count(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_line_related_model_float_parameter_name(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_hvdc_related_model_name(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_hvdc_related_model_float_parameter(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_related_model_float_parameter(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_hvdc_related_model_float_parameter_count(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_hvdc_related_model_float_parameter_name(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL const char* api_get_energy_storage_related_model_name(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_energy_storage_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_energy_storage_related_model_float_parameter(unsigned int bus, char* identifier, char* model_type, char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL unsigned int api_get_energy_storage_related_model_float_parameter_count(unsigned int bus, char* identifier, char* model_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_energy_storage_related_model_float_parameter_name(unsigned int bus, char* identifier, char* model_type, unsigned int parameter_index, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL double api_get_generator_related_model_float_variable(unsigned int bus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_wt_generator_related_model_float_variable(unsigned int bus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_pv_unit_related_model_float_variable(unsigned int bus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_load_related_model_float_variable(unsigned int bus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_line_related_model_float_variable(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_hvdc_related_model_float_variable(unsigned int ibus, unsigned int jbus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_energy_storage_related_model_float_variable(unsigned int bus, char* identifier, char* model_type, char* variable_name, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL unsigned int api_get_powerflow_solver_integer_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_powerflow_solver_integer_parameter(char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_powerflow_solver_float_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_powerflow_solver_float_parameter(char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_powerflow_solver_boolean_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_powerflow_solver_boolean_parameter(char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_show_powerflow_solver_configuration(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_solve_powerflow(char* method, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_is_powerflow_converged(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_show_powerflow_result(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_powerflow_result(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_extended_powerflow_result(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_jacobian_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_build_network_Y_matrix(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_build_decoupled_network_B_matrix(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_build_dc_network_B_matrix(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_build_dynamic_network_Y_matrix(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_build_network_Z_matrix(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_network_Y_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_decoupled_network_B_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_dc_network_B_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_dynamic_network_Y_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_save_network_Z_matrix(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL unsigned int api_get_dynamic_simulator_integer_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_dynamic_simulator_integer_parameter(char* parameter_name, int value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_dynamic_simulator_float_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_dynamic_simulator_float_parameter(char* parameter_name, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_dynamic_simulator_string_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_dynamic_simulator_string_parameter(char* parameter_name, char* value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_dynamic_simulator_boolean_parameter(char* parameter_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_dynamic_simulator_boolean_parameter(char* parameter_name, bool value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_dynamic_simulator_output_file(char* file, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL const char* api_get_dynamic_simulator_output_file(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_dynamic_simulation_time_step(double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_dynamic_simulation_time_step(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_dynamic_simulation_time(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_show_dynamic_simulation_configuration(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_clear_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_bus_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_generator_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_wt_generator_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_pv_unit_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_energy_storage_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_load_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_line_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_transformer_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_hvdc_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_equivalent_device_related_meters(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_prepare_bus_related_meter(unsigned int bus, char* meter_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_generator_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_wt_generator_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_pv_unit_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_energy_storage_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_load_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_line_related_meter(unsigned int ibus, unsigned int jbus, char* id, char* meter_type, char* side, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_transformer_related_meter(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* id, char* meter_type, char* side, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_hvdc_related_meter(unsigned int ibus, unsigned int jbus, char* id, char* meter_type, char* side, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_prepare_equivalent_device_related_meter(unsigned int bus, char* id, char* meter_type, char* var_name, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_start_dynamic_simulation(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_stop_dynamic_simulation(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_run_simulation_to_time(double t_end, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_run_a_step(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL bool api_get_system_angular_stable_flag(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_bus_fault(unsigned int bus, char* fault_type, double fault_G, double fault_B, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_clear_bus_fault(unsigned int bus, char* fault_type, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_trip_bus(unsigned int bus, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_line_fault(unsigned int ibus, unsigned int jbus, char* identifier, char* fault_type, double fault_location, double fault_G, double fault_B, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_clear_line_fault(unsigned int ibus, unsigned int jbus, char* identifier, char* fault_type, double fault_location, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_trip_line(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_trip_line_breaker(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_line(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_line_breaker(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_trip_transformer(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_trip_transformer_breaker(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_transformer(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_transformer_breaker(unsigned int ibus, unsigned int jbus, unsigned int kbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_trip_generator(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_shed_generator(unsigned int bus, char* identifier, double percent, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_trip_wt_generator(unsigned int bus, char* identifier, unsigned int n, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_shed_wt_generator(unsigned int bus, char* identifier, double percent, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_trip_load(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_load(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_scale_load(unsigned int bus, char* identifier, double percent, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_scale_all_loads(double percent, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_trip_fixed_shunt(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_close_fixed_shunt(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_manually_bypass_hvdc(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_manually_unbypass_hvdc(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_manually_block_hvdc(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_manually_unblock_hvdc(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL double api_get_generator_voltage_reference_in_pu(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_generator_mechanical_power_reference_in_pu_based_on_mbase(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_generator_mechanical_power_reference_in_MW(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_generator_voltage_reference_in_pu(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_generator_mechanical_power_reference_in_pu_based_on_mbase(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_generator_mechanical_power_reference_in_MW(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL double api_get_generator_excitation_voltage_in_pu(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_generator_mechanical_power_in_pu_based_on_mbase(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL double api_get_generator_mechanical_power_in_MW(unsigned int bus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_set_generator_excitation_voltage_in_pu(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_generator_mechanical_power_in_pu_based_on_mbase(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_generator_mechanical_power_in_MW(unsigned int bus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL double api_get_hvdc_power_order_in_MW(unsigned int ibus, unsigned int jbus, char* identifier, unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_set_hvdc_power_order_in_MW(unsigned int ibus, unsigned int jbus, char* identifier, double value, unsigned int toolkit_index=INDEX_NOT_EXIST);


EXPORT_STEPS_DLL void api_check_powerflow_data(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_check_dynamic_data(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_check_missing_models(unsigned int toolkit_index=INDEX_NOT_EXIST);
EXPORT_STEPS_DLL void api_check_least_dynamic_time_constants(unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL void api_check_network_connectivity(bool remove_void_island=false, unsigned int toolkit_index=INDEX_NOT_EXIST);

EXPORT_STEPS_DLL double api_search_cct(char* pf_file, char* dy_file, unsigned int ibus, unsigned int jbus, char* id, unsigned int sidebus, unsigned int trip_line, unsigned int toolkit_index=INDEX_NOT_EXIST);

#ifdef  __cplusplus
}
#endif

#endif // STEPS_API_H
