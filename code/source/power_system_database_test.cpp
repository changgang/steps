#include "header/basic/test_macro.h"
#include "header/power_system_database_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/steps_namespace.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;


POWER_SYSTEM_DATABASE_TEST::POWER_SYSTEM_DATABASE_TEST()
{
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_zero_impedance_threshold);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_bus_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_generator_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_wt_generator_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_pv_unit_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_load_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_fixed_shunt_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_line_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_transformer_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_hvdc_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_equivalent_device_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_energy_storage_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_area_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_zone_capacity);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_owner_capacity);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_system_name);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_allowed_max_bus_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_bus_in_allowed_range);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_set_get_system_base_power);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_bus_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_generator);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_wt_generator);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_pv_unit);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_load);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_line);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_transformer);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_fixed_shunt);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_hvdc);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_equivalent_device);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_energy_storage);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_append_and_get_owner);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_generator_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_wt_generator_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_pv_unit_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_load_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_line_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_transformer_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_fixed_shunt_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_hvdc_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_equivalent_device_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_energy_storage_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_area_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_zone_exist);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_is_owner_exist);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_change_bus_number);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_connecting_to_bus);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_connecting_to_bus);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_in_area);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_in_area);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_in_zone);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_devices);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_buses);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_with_constraints);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_in_service_buses);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_generators);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_wt_generators);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_pv_units);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_sources);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_loads);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_lines);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_transformers);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_fixed_shunts);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_hvdcs);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_equivalent_devices);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_energy_storages);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_areas);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_zones);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_owners);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_buses_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_buses_number_with_constraints);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_in_service_buses_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_generators_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_wt_generators_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_pv_units_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_sources_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_loads_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_lines_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_transformers_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_fixed_shunts_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_hvdcs_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_equivalent_devices_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_energy_storages_device_id);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_areas_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_zones_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_all_owners_number);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_in_service_bus_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_overshadowed_bus_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generator_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generator_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_unit_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_source_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_load_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_line_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformer_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunt_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdc_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_device_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storage_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_area_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_zone_count);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_owner_count);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_generator_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_wt_generator_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_pv_unit_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_load_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_line_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_transformer_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunt_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_hvdc_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_device_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_energy_storage_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_area_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_zone_index);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_owner_index);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_bus_name2bus_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_area_name2area_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_zone_name2zone_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_owner_name2owner_number);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_bus_number2bus_name);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_load_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_all_load_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_at_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_source_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_all_source_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_at_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_all_generator_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_at_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_all_wt_generator_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_at_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_all_pv_unit_power);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_at_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_in_area);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_in_zone);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear);
    //TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_buses);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_generator);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_generators_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_generators);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_wt_generator);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_wt_generators_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_wt_generators);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_pv_unit);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_pv_units_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_pv_units);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_sources_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_sources);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_load);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_loads_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_loads);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_line);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_lines_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_lines);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_transformer);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_transformers_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_transformers);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_fixed_shunt);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_fixed_shunts_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_fixed_shunts);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_hvdc);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_hvdcs_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_hvdcs);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_equivalent_device);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_equivalent_devices_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_equivalent_devices);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_energy_storage);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_energy_storages_connecting_to_bus);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_energy_storages);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_areas);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_zones);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_clear_all_owners);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_complex_voltage);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_base_frequency);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_base_voltage);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_voltage);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_bus_angle);

    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_voltage_to_regulate_of_physical_bus_in_pu);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_q_max_at_physical_bus_in_MVar);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_q_min_at_physical_bus_in_MVar);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_total_regulating_p_generation_at_physical_bus_in_MW);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_total_regulating_q_generation_at_physical_bus_in_MVar);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_total_p_generation_of_sources_at_physical_bus_in_MW);
    TEST_ADD(POWER_SYSTEM_DATABASE_TEST::test_get_total_q_generation_of_sources_at_physical_bus_in_MVar);
}

void POWER_SYSTEM_DATABASE_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}

void POWER_SYSTEM_DATABASE_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}


void POWER_SYSTEM_DATABASE_TEST::prepare_database_for_test()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();


    // all devices in the test database are:
    // Bus: 1, 2, 3
    // Generator: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // WT generator: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // PV UNIT: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Energy storage: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Load: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Fixed shunt: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Line: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2,
    // Transformer: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2, 1-2-3-#1, 1-2-3-#2
    // Hvdc: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2,
    // Equivalent device: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Area: 1, 2, 3
    // Zone: 1, 2, 3
    // Owner: 1, 2, 3
    psdb.set_allowed_max_bus_number(100);

    {
        BUS bus;
        bus.set_toolkit(default_toolkit);

        bus.set_bus_number(1);
        bus.set_bus_name("BUS A");
        bus.set_base_voltage_in_kV(110.0);
        bus.set_base_frequency_in_Hz(50.0);
        bus.set_bus_type(SLACK_TYPE);
        bus.set_area_number(1);
        bus.set_zone_number(1);
        bus.set_owner_number(1);
        psdb.append_bus(bus);

        bus.set_bus_number(2);
        bus.set_bus_name("BUS B");
        bus.set_base_voltage_in_kV(220.0);
        bus.set_bus_type(PV_TYPE);
        bus.set_area_number(2);
        bus.set_zone_number(2);
        bus.set_owner_number(2);
        psdb.append_bus(bus);

        bus.set_bus_number(3);
        bus.set_bus_name("BUS C");
        bus.set_base_voltage_in_kV(330.0);
        bus.set_bus_type(PQ_TYPE);
        bus.set_area_number(3);
        bus.set_zone_number(3);
        bus.set_owner_number(3);
        psdb.append_bus(bus);
    }

    {
        GENERATOR generator;
        generator.set_toolkit(default_toolkit);
        generator.set_generator_bus(1);
        generator.set_identifier("#1");
        generator.set_status(true);
        generator.set_p_generation_in_MW(10.0);
        generator.set_q_generation_in_MVar(10.0);
        generator.set_p_max_in_MW(110.0);
        generator.set_p_min_in_MW(-110.0);
        generator.set_q_max_in_MVar(110.0);
        generator.set_q_min_in_MVar(-110.0);
        psdb.append_generator(generator);

        generator.set_identifier("#2");
        generator.set_status(true);
        generator.set_p_generation_in_MW(10.0);
        generator.set_q_generation_in_MVar(20.0);
        generator.set_p_max_in_MW(120.0);
        generator.set_p_min_in_MW(-120.0);
        generator.set_q_max_in_MVar(120.0);
        generator.set_q_min_in_MVar(-120.0);
        psdb.append_generator(generator);

        generator.set_generator_bus(2);
        generator.set_identifier("#1");
        generator.set_status(true);
        generator.set_p_generation_in_MW(20.0);
        generator.set_q_generation_in_MVar(10.0);
        generator.set_p_max_in_MW(210.0);
        generator.set_p_min_in_MW(-210.0);
        generator.set_q_max_in_MVar(210.0);
        generator.set_q_min_in_MVar(-210.0);
        psdb.append_generator(generator);

        generator.set_identifier("#2");
        generator.set_status(true);
        generator.set_p_generation_in_MW(20.0);
        generator.set_q_generation_in_MVar(20.0);
        generator.set_p_max_in_MW(220.0);
        generator.set_p_min_in_MW(-220.0);
        generator.set_q_max_in_MVar(220.0);
        generator.set_q_min_in_MVar(-220.0);
        psdb.append_generator(generator);

        generator.set_generator_bus(3);
        generator.set_identifier("#1");
        generator.set_status(true);
        generator.set_p_generation_in_MW(30.0);
        generator.set_q_generation_in_MVar(10.0);
        generator.set_p_max_in_MW(310.0);
        generator.set_p_min_in_MW(-310.0);
        generator.set_q_max_in_MVar(310.0);
        generator.set_q_min_in_MVar(-310.0);
        psdb.append_generator(generator);

        generator.set_identifier("#2");
        generator.set_status(true);
        generator.set_p_generation_in_MW(30.0);
        generator.set_q_generation_in_MVar(20.0);
        generator.set_p_max_in_MW(320.0);
        generator.set_p_min_in_MW(-320.0);
        generator.set_q_max_in_MVar(320.0);
        generator.set_q_min_in_MVar(-320.0);
        psdb.append_generator(generator);
    }

    {
        WT_GENERATOR wt_generator;
        wt_generator.set_toolkit(default_toolkit);

        wt_generator.set_source_bus(1);
        wt_generator.set_identifier("#1");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(10.0);
        wt_generator.set_q_generation_in_MVar(10.0);
        wt_generator.set_p_max_in_MW(101.0);
        wt_generator.set_p_min_in_MW(-101.0);
        wt_generator.set_q_max_in_MVar(101.0);
        wt_generator.set_q_min_in_MVar(-101.0);
        psdb.append_wt_generator(wt_generator);

        wt_generator.set_identifier("#2");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(10.0);
        wt_generator.set_q_generation_in_MVar(20.0);
        wt_generator.set_p_max_in_MW(102.0);
        wt_generator.set_p_min_in_MW(-102.0);
        wt_generator.set_q_max_in_MVar(102.0);
        wt_generator.set_q_min_in_MVar(-102.0);
        psdb.append_wt_generator(wt_generator);

        wt_generator.set_source_bus(2);
        wt_generator.set_identifier("#1");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(20.0);
        wt_generator.set_q_generation_in_MVar(10.0);
        wt_generator.set_p_max_in_MW(201.0);
        wt_generator.set_p_min_in_MW(-201.0);
        wt_generator.set_q_max_in_MVar(201.0);
        wt_generator.set_q_min_in_MVar(-201.0);
        psdb.append_wt_generator(wt_generator);

        wt_generator.set_identifier("#2");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(20.0);
        wt_generator.set_q_generation_in_MVar(20.0);
        wt_generator.set_p_max_in_MW(202.0);
        wt_generator.set_p_min_in_MW(-202.0);
        wt_generator.set_q_max_in_MVar(202.0);
        wt_generator.set_q_min_in_MVar(-202.0);
        psdb.append_wt_generator(wt_generator);

        wt_generator.set_source_bus(3);
        wt_generator.set_identifier("#1");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(30.0);
        wt_generator.set_q_generation_in_MVar(10.0);
        wt_generator.set_p_max_in_MW(301.0);
        wt_generator.set_p_min_in_MW(-301.0);
        wt_generator.set_q_max_in_MVar(301.0);
        wt_generator.set_q_min_in_MVar(-301.0);
        psdb.append_wt_generator(wt_generator);

        wt_generator.set_identifier("#2");
        wt_generator.set_status(true);
        wt_generator.set_p_generation_in_MW(30.0);
        wt_generator.set_q_generation_in_MVar(20.0);
        wt_generator.set_p_max_in_MW(302.0);
        wt_generator.set_p_min_in_MW(-302.0);
        wt_generator.set_q_max_in_MVar(302.0);
        wt_generator.set_q_min_in_MVar(-302.0);
        psdb.append_wt_generator(wt_generator);
    }

    {
        PV_UNIT pv_unit;
        pv_unit.set_toolkit(default_toolkit);

        pv_unit.set_unit_bus(1);
        pv_unit.set_identifier("#1");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(10.0);
        pv_unit.set_q_generation_in_MVar(10.0);
        pv_unit.set_p_max_in_MW(101.0);
        pv_unit.set_p_min_in_MW(-101.0);
        pv_unit.set_q_max_in_MVar(101.0);
        pv_unit.set_q_min_in_MVar(-101.0);
        psdb.append_pv_unit(pv_unit);

        pv_unit.set_identifier("#2");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(10.0);
        pv_unit.set_q_generation_in_MVar(20.0);
        pv_unit.set_p_max_in_MW(102.0);
        pv_unit.set_p_min_in_MW(-102.0);
        pv_unit.set_q_max_in_MVar(102.0);
        pv_unit.set_q_min_in_MVar(-102.0);
        psdb.append_pv_unit(pv_unit);

        pv_unit.set_unit_bus(2);
        pv_unit.set_identifier("#1");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(20.0);
        pv_unit.set_q_generation_in_MVar(10.0);
        pv_unit.set_p_max_in_MW(201.0);
        pv_unit.set_p_min_in_MW(-201.0);
        pv_unit.set_q_max_in_MVar(201.0);
        pv_unit.set_q_min_in_MVar(-201.0);
        psdb.append_pv_unit(pv_unit);

        pv_unit.set_identifier("#2");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(20.0);
        pv_unit.set_q_generation_in_MVar(20.0);
        pv_unit.set_p_max_in_MW(202.0);
        pv_unit.set_p_min_in_MW(-202.0);
        pv_unit.set_q_max_in_MVar(202.0);
        pv_unit.set_q_min_in_MVar(-202.0);
        psdb.append_pv_unit(pv_unit);

        pv_unit.set_unit_bus(3);
        pv_unit.set_identifier("#1");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(30.0);
        pv_unit.set_q_generation_in_MVar(10.0);
        pv_unit.set_p_max_in_MW(301.0);
        pv_unit.set_p_min_in_MW(-301.0);
        pv_unit.set_q_max_in_MVar(301.0);
        pv_unit.set_q_min_in_MVar(-301.0);
        psdb.append_pv_unit(pv_unit);

        pv_unit.set_identifier("#2");
        pv_unit.set_status(true);
        pv_unit.set_p_generation_in_MW(30.0);
        pv_unit.set_q_generation_in_MVar(20.0);
        pv_unit.set_p_max_in_MW(302.0);
        pv_unit.set_p_min_in_MW(-302.0);
        pv_unit.set_q_max_in_MVar(302.0);
        pv_unit.set_q_min_in_MVar(-302.0);
        psdb.append_pv_unit(pv_unit);
    }

    {
        ENERGY_STORAGE energy_storage;
        energy_storage.set_toolkit(default_toolkit);

        energy_storage.set_energy_storage_bus(1);
        energy_storage.set_identifier("#1");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(10.0);
        energy_storage.set_q_generation_in_MVar(10.0);
        energy_storage.set_p_max_in_MW(101.0);
        energy_storage.set_p_min_in_MW(-101.0);
        energy_storage.set_q_max_in_MVar(101.0);
        energy_storage.set_q_min_in_MVar(-101.0);
        psdb.append_energy_storage(energy_storage);

        energy_storage.set_identifier("#2");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(10.0);
        energy_storage.set_q_generation_in_MVar(20.0);
        energy_storage.set_p_max_in_MW(102.0);
        energy_storage.set_p_min_in_MW(-102.0);
        energy_storage.set_q_max_in_MVar(102.0);
        energy_storage.set_q_min_in_MVar(-102.0);
        psdb.append_energy_storage(energy_storage);

        energy_storage.set_energy_storage_bus(2);
        energy_storage.set_identifier("#1");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(20.0);
        energy_storage.set_q_generation_in_MVar(10.0);
        energy_storage.set_p_max_in_MW(201.0);
        energy_storage.set_p_min_in_MW(-201.0);
        energy_storage.set_q_max_in_MVar(201.0);
        energy_storage.set_q_min_in_MVar(-201.0);
        psdb.append_energy_storage(energy_storage);

        energy_storage.set_identifier("#2");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(20.0);
        energy_storage.set_q_generation_in_MVar(20.0);
        energy_storage.set_p_max_in_MW(202.0);
        energy_storage.set_p_min_in_MW(-202.0);
        energy_storage.set_q_max_in_MVar(202.0);
        energy_storage.set_q_min_in_MVar(-202.0);
        psdb.append_energy_storage(energy_storage);

        energy_storage.set_energy_storage_bus(3);
        energy_storage.set_identifier("#1");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(30.0);
        energy_storage.set_q_generation_in_MVar(10.0);
        energy_storage.set_p_max_in_MW(301.0);
        energy_storage.set_p_min_in_MW(-301.0);
        energy_storage.set_q_max_in_MVar(301.0);
        energy_storage.set_q_min_in_MVar(-301.0);
        psdb.append_energy_storage(energy_storage);

        energy_storage.set_identifier("#2");
        energy_storage.set_status(true);
        energy_storage.set_p_generation_in_MW(30.0);
        energy_storage.set_q_generation_in_MVar(20.0);
        energy_storage.set_p_max_in_MW(302.0);
        energy_storage.set_p_min_in_MW(-302.0);
        energy_storage.set_q_max_in_MVar(302.0);
        energy_storage.set_q_min_in_MVar(-302.0);
        psdb.append_energy_storage(energy_storage);
    }

    {
        LOAD load;
        load.set_toolkit(default_toolkit);

        load.set_load_bus(1);
        load.set_identifier("#1");
        load.set_status(true);
        load.set_area_number(1);
        load.set_zone_number(1);
        load.set_nominal_constant_power_load_in_MVA(complex<double>(10.0, 10.0));
        psdb.append_load(load);

        load.set_identifier("#2");
        load.set_nominal_constant_power_load_in_MVA(complex<double>(10.0, 20.0));
        psdb.append_load(load);

        load.set_load_bus(2);
        load.set_identifier("#1");
        load.set_area_number(2);
        load.set_zone_number(2);
        load.set_nominal_constant_power_load_in_MVA(complex<double>(20.0, 10.0));
        psdb.append_load(load);

        load.set_identifier("#2");
        load.set_nominal_constant_power_load_in_MVA(complex<double>(20.0, 20.0));
        psdb.append_load(load);

        load.set_load_bus(3);
        load.set_identifier("#1");
        load.set_area_number(3);
        load.set_zone_number(3);
        load.set_nominal_constant_power_load_in_MVA(complex<double>(30.0, 10.0));
        psdb.append_load(load);

        load.set_identifier("#2");
        load.set_nominal_constant_power_load_in_MVA(complex<double>(30.0, 20.0));
        psdb.append_load(load);
    }

    {
        FIXED_SHUNT shunt;
        shunt.set_toolkit(default_toolkit);

        shunt.set_shunt_bus(1);
        shunt.set_identifier("#1");
        psdb.append_fixed_shunt(shunt);

        shunt.set_identifier("#2");
        psdb.append_fixed_shunt(shunt);

        shunt.set_shunt_bus(2);
        shunt.set_identifier("#1");
        psdb.append_fixed_shunt(shunt);

        shunt.set_identifier("#2");
        psdb.append_fixed_shunt(shunt);

        shunt.set_shunt_bus(3);
        shunt.set_identifier("#1");
        psdb.append_fixed_shunt(shunt);

        shunt.set_identifier("#2");
        psdb.append_fixed_shunt(shunt);
    }

    {
        LINE line;
        line.set_toolkit(default_toolkit);

        line.set_sending_side_bus(1);
        line.set_receiving_side_bus(2);
        line.set_identifier("#1");
        psdb.append_line(line);

        line.set_identifier("#2");
        psdb.append_line(line);

        line.set_sending_side_bus(2);
        line.set_receiving_side_bus(3);
        line.set_identifier("#1");
        psdb.append_line(line);

        line.set_identifier("#2");
        psdb.append_line(line);

        line.set_sending_side_bus(1);
        line.set_receiving_side_bus(3);
        line.set_identifier("#1");
        psdb.append_line(line);

        line.set_identifier("#2");
        psdb.append_line(line);
    }

    {
        TRANSFORMER transformer;
        transformer.set_toolkit(default_toolkit);

        transformer.set_winding_bus(PRIMARY_SIDE, 1);
        transformer.set_winding_bus(SECONDARY_SIDE, 2);
        transformer.set_identifier("#1");
        psdb.append_transformer(transformer);

        transformer.set_identifier("#2");
        psdb.append_transformer(transformer);

        transformer.set_winding_bus(PRIMARY_SIDE, 2);
        transformer.set_winding_bus(SECONDARY_SIDE, 3);
        transformer.set_identifier("#1");
        psdb.append_transformer(transformer);
        transformer.set_identifier("#2");
        psdb.append_transformer(transformer);

        transformer.set_winding_bus(PRIMARY_SIDE, 1);
        transformer.set_winding_bus(SECONDARY_SIDE, 3);
        transformer.set_identifier("#1");
        psdb.append_transformer(transformer);
        transformer.set_identifier("#2");
        psdb.append_transformer(transformer);

        transformer.set_winding_bus(PRIMARY_SIDE, 1);
        transformer.set_winding_bus(SECONDARY_SIDE, 2);
        transformer.set_winding_bus(TERTIARY_SIDE, 3);
        transformer.set_identifier("#1");
        psdb.append_transformer(transformer);
        transformer.set_identifier("#2");
        psdb.append_transformer(transformer);
    }

    {
        HVDC hvdc;
        hvdc.set_toolkit(default_toolkit);

        hvdc.set_converter_bus(RECTIFIER, 1);
        hvdc.set_converter_bus(INVERTER, 2);
        hvdc.set_identifier("#1");
        hvdc.set_name("HVDC A");
        psdb.append_hvdc(hvdc);

        hvdc.set_identifier("#2");
        hvdc.set_name("HVDC B");
        psdb.append_hvdc(hvdc);

        hvdc.set_converter_bus(RECTIFIER, 2);
        hvdc.set_converter_bus(INVERTER, 3);
        hvdc.set_identifier("#1");
        hvdc.set_name("HVDC C");
        psdb.append_hvdc(hvdc);

        hvdc.set_identifier("#2");
        hvdc.set_name("HVDC D");
        psdb.append_hvdc(hvdc);

        hvdc.set_converter_bus(RECTIFIER, 1);
        hvdc.set_converter_bus(INVERTER, 3);
        hvdc.set_identifier("#1");
        hvdc.set_name("HVDC E");
        psdb.append_hvdc(hvdc);

        hvdc.set_identifier("#2");
        hvdc.set_name("HVDC F");
        psdb.append_hvdc(hvdc);
    }

    {
        EQUIVALENT_DEVICE edevice;
        edevice.set_toolkit(default_toolkit);

        edevice.set_equivalent_device_bus(1);
        edevice.set_identifier("#1");
        edevice.set_status(true);
        psdb.append_equivalent_device(edevice);

        edevice.set_identifier("#2");
        edevice.set_status(false);
        psdb.append_equivalent_device(edevice);

        edevice.set_equivalent_device_bus(2);
        edevice.set_identifier("#1");
        edevice.set_status(true);
        psdb.append_equivalent_device(edevice);

        edevice.set_identifier("#2");
        edevice.set_status(false);
        psdb.append_equivalent_device(edevice);

        edevice.set_equivalent_device_bus(3);
        edevice.set_identifier("#1");
        edevice.set_status(true);
        psdb.append_equivalent_device(edevice);

        edevice.set_identifier("#2");
        edevice.set_status(false);
        psdb.append_equivalent_device(edevice);
    }


    {
        AREA area;
        area.set_toolkit(default_toolkit);

        area.set_area_number(1);
        area.set_area_name("AREA A");
        area.set_area_swing_bus(1);
        psdb.append_area(area);

        area.set_area_number(2);
        area.set_area_name("AREA B");
        //area.set_area_swing_bus(0);
        psdb.append_area(area);

        area.set_area_number(3);
        area.set_area_name("AREA C");
        area.set_area_swing_bus(3);
        psdb.append_area(area);
    }

    {
        ZONE zone;
        zone.set_toolkit(default_toolkit);

        zone.set_zone_number(1);
        zone.set_zone_name("ZONE A");
        psdb.append_zone(zone);

        zone.set_zone_number(2);
        zone.set_zone_name("ZONE B");
        psdb.append_zone(zone);

        zone.set_zone_number(3);
        zone.set_zone_name("ZONE C");
        psdb.append_zone(zone);
    }

    {
        OWNER owner;
        owner.set_toolkit(default_toolkit);

        owner.set_owner_number(1);
        owner.set_owner_name("OWNER A");
        psdb.append_owner(owner);

        owner.set_owner_number(2);
        owner.set_owner_name("OWNER B");
        psdb.append_owner(owner);

        owner.set_owner_number(3);
        owner.set_owner_name("OWNER C");
        psdb.append_owner(owner);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_zero_impedance_threshold()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_zero_impedance_threshold_in_pu(0.01);
    TEST_ASSERT(fabs(psdb.get_zero_impedance_threshold_in_pu()-0.01)<FLOAT_EPSILON);
    psdb.set_zero_impedance_threshold_in_pu(0.001);
    TEST_ASSERT(fabs(psdb.get_zero_impedance_threshold_in_pu()-0.001)<FLOAT_EPSILON);
    psdb.set_zero_impedance_threshold_in_pu(0.0001);
    TEST_ASSERT(fabs(psdb.get_zero_impedance_threshold_in_pu()-0.0001)<FLOAT_EPSILON);
}


void POWER_SYSTEM_DATABASE_TEST::test_set_get_bus_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_bus_capacity(10000);
    TEST_ASSERT(psdb.get_bus_capacity()==10000);

    psdb.set_bus_capacity(1000);
    TEST_ASSERT(psdb.get_bus_capacity()==10000);

    psdb.set_bus_capacity(20000);
    TEST_ASSERT(psdb.get_bus_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_generator_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_generator_capacity(10000);
    TEST_ASSERT(psdb.get_generator_capacity()==10000);

    psdb.set_generator_capacity(1000);
    TEST_ASSERT(psdb.get_generator_capacity()==10000);

    psdb.set_generator_capacity(20000);
    TEST_ASSERT(psdb.get_generator_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_wt_generator_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_wt_generator_capacity(10000);
    TEST_ASSERT(psdb.get_wt_generator_capacity()==10000);

    psdb.set_wt_generator_capacity(1000);
    TEST_ASSERT(psdb.get_wt_generator_capacity()==10000);

    psdb.set_wt_generator_capacity(20000);
    TEST_ASSERT(psdb.get_wt_generator_capacity()==20000);
}


void POWER_SYSTEM_DATABASE_TEST::test_set_get_pv_unit_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_pv_unit_capacity(10000);
    TEST_ASSERT(psdb.get_pv_unit_capacity()==10000);

    psdb.set_pv_unit_capacity(1000);
    TEST_ASSERT(psdb.get_pv_unit_capacity()==10000);

    psdb.set_pv_unit_capacity(20000);
    TEST_ASSERT(psdb.get_pv_unit_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_load_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_load_capacity(10000);
    TEST_ASSERT(psdb.get_load_capacity()==10000);

    psdb.set_load_capacity(1000);
    TEST_ASSERT(psdb.get_load_capacity()==10000);

    psdb.set_load_capacity(20000);
    TEST_ASSERT(psdb.get_load_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_fixed_shunt_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_fixed_shunt_capacity(10000);
    TEST_ASSERT(psdb.get_fixed_shunt_capacity()==10000);

    psdb.set_fixed_shunt_capacity(1000);
    TEST_ASSERT(psdb.get_fixed_shunt_capacity()==10000);

    psdb.set_fixed_shunt_capacity(20000);
    TEST_ASSERT(psdb.get_fixed_shunt_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_line_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_line_capacity(10000);
    TEST_ASSERT(psdb.get_line_capacity()==10000);

    psdb.set_line_capacity(1000);
    TEST_ASSERT(psdb.get_line_capacity()==10000);

    psdb.set_line_capacity(60000);
    TEST_ASSERT(psdb.get_line_capacity()==60000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_transformer_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_transformer_capacity(10000);
    TEST_ASSERT(psdb.get_transformer_capacity()==10000);

    psdb.set_transformer_capacity(1000);
    TEST_ASSERT(psdb.get_transformer_capacity()==10000);

    psdb.set_transformer_capacity(60000);
    TEST_ASSERT(psdb.get_transformer_capacity()==60000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_hvdc_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_hvdc_capacity(10000);
    TEST_ASSERT(psdb.get_hvdc_capacity()==10000);

    psdb.set_hvdc_capacity(1000);
    TEST_ASSERT(psdb.get_hvdc_capacity()==10000);

    psdb.set_hvdc_capacity(20000);
    TEST_ASSERT(psdb.get_hvdc_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_equivalent_device_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_equivalent_device_capacity(10000);
    TEST_ASSERT(psdb.get_equivalent_device_capacity()==10000);

    psdb.set_equivalent_device_capacity(1000);
    TEST_ASSERT(psdb.get_equivalent_device_capacity()==10000);

    psdb.set_equivalent_device_capacity(20000);
    TEST_ASSERT(psdb.get_equivalent_device_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_energy_storage_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_energy_storage_capacity(10000);
    TEST_ASSERT(psdb.get_energy_storage_capacity()==10000);

    psdb.set_energy_storage_capacity(1000);
    TEST_ASSERT(psdb.get_energy_storage_capacity()==10000);

    psdb.set_energy_storage_capacity(20000);
    TEST_ASSERT(psdb.get_energy_storage_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_area_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_area_capacity(10000);
    TEST_ASSERT(psdb.get_area_capacity()==10000);

    psdb.set_area_capacity(1000);
    TEST_ASSERT(psdb.get_area_capacity()==10000);

    psdb.set_area_capacity(20000);
    TEST_ASSERT(psdb.get_area_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_zone_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_zone_capacity(10000);
    TEST_ASSERT(psdb.get_zone_capacity()==10000);

    psdb.set_zone_capacity(1000);
    TEST_ASSERT(psdb.get_zone_capacity()==10000);

    psdb.set_zone_capacity(20000);
    TEST_ASSERT(psdb.get_zone_capacity()==20000);
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_owner_capacity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_owner_capacity(10000);
    TEST_ASSERT(psdb.get_owner_capacity()==10000);

    psdb.set_owner_capacity(1000);
    TEST_ASSERT(psdb.get_owner_capacity()==10000);

    psdb.set_owner_capacity(20000);
    TEST_ASSERT(psdb.get_owner_capacity()==20000);
}


void POWER_SYSTEM_DATABASE_TEST::test_set_get_system_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_system_name("System Test");
    TEST_ASSERT(psdb.get_system_name()=="System Test");
}

void POWER_SYSTEM_DATABASE_TEST::test_set_get_allowed_max_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(1000);
    TEST_ASSERT(psdb.get_allowed_max_bus_number()==1000);
    psdb.set_allowed_max_bus_number(10000);
    TEST_ASSERT(psdb.get_allowed_max_bus_number()==10000);
}

void POWER_SYSTEM_DATABASE_TEST::test_is_bus_in_allowed_range()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);
    TEST_ASSERT(psdb.is_bus_in_allowed_range(0)==false);
    TEST_ASSERT(psdb.is_bus_in_allowed_range(1)==true);
    TEST_ASSERT(psdb.is_bus_in_allowed_range(100)==true);
    TEST_ASSERT(psdb.is_bus_in_allowed_range(101)==false);
}


void POWER_SYSTEM_DATABASE_TEST::test_set_get_system_base_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_system_base_power_in_MVA(100.0);
    TEST_ASSERT(fabs(psdb.get_system_base_power_in_MVA()-100.0)<FLOAT_EPSILON);
    psdb.set_system_base_power_in_MVA(1000.0);
    TEST_ASSERT(fabs(psdb.get_system_base_power_in_MVA()-1000.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;

    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_rad(0.5);

    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(220.0);
    bus.set_positive_sequence_voltage_in_pu(0.95);
    bus.set_positive_sequence_angle_in_rad(0.25);
    psdb.append_bus(bus);

    TEST_ASSERT(psdb.get_bus_count()==2);

    BUS* pbus;

    pbus = psdb.get_bus(1);
    TEST_ASSERT(pbus!=NULL);
    TEST_ASSERT(pbus->get_bus_number()==1);
    TEST_ASSERT(fabs(pbus->get_base_voltage_in_kV()-110.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_voltage_in_pu()-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_angle_in_rad()-0.5)<FLOAT_EPSILON);

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    pbus = psdb.get_bus(did);
    TEST_ASSERT(pbus!=NULL);
    TEST_ASSERT(pbus->get_bus_number()==1);
    TEST_ASSERT(fabs(pbus->get_base_voltage_in_kV()-110.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_voltage_in_pu()-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_angle_in_rad()-0.5)<FLOAT_EPSILON);

    pbus = psdb.get_bus(2);
    TEST_ASSERT(pbus!=NULL);
    TEST_ASSERT(pbus->get_bus_number()==2);
    TEST_ASSERT(fabs(pbus->get_base_voltage_in_kV()-220.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_voltage_in_pu()-0.95)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_angle_in_rad()-0.25)<FLOAT_EPSILON);

    did.set_device_type("BUS");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    pbus = psdb.get_bus(did);
    TEST_ASSERT(pbus!=NULL);
    TEST_ASSERT(pbus->get_bus_number()==2);
    TEST_ASSERT(fabs(pbus->get_base_voltage_in_kV()-220.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_voltage_in_pu()-0.95)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(pbus->get_positive_sequence_angle_in_rad()-0.25)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(18.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    psdb.append_bus(bus);

    GENERATOR generator;
    generator.set_toolkit(default_toolkit);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    generator.set_status(true);

    psdb.append_generator(generator);

    generator.set_generator_bus(1);
    generator.set_identifier("#2");
    generator.set_status(false);

    psdb.append_generator(generator);

    TEST_ASSERT(psdb.get_generator_count()==2);

    GENERATOR* pgenerator;
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pgenerator = psdb.get_generator(did);

    TEST_ASSERT(pgenerator!=NULL);
    TEST_ASSERT(pgenerator->get_generator_bus()==1);
    TEST_ASSERT(pgenerator->get_identifier()=="#1");
    TEST_ASSERT(pgenerator->get_status()==true);

    did.set_device_identifier("#2");

    pgenerator = psdb.get_generator(did);

    TEST_ASSERT(pgenerator!=NULL);
    TEST_ASSERT(pgenerator->get_generator_bus()==1);
    TEST_ASSERT(pgenerator->get_identifier()=="#2");
    TEST_ASSERT(pgenerator->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_wt_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(18.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    psdb.append_bus(bus);

    WT_GENERATOR wt_generator;
    wt_generator.set_toolkit(default_toolkit);

    wt_generator.set_source_bus(1);
    wt_generator.set_identifier("#1");
    wt_generator.set_status(true);

    psdb.append_wt_generator(wt_generator);

    wt_generator.set_source_bus(1);
    wt_generator.set_identifier("#2");
    wt_generator.set_status(false);

    psdb.append_wt_generator(wt_generator);

    TEST_ASSERT(psdb.get_wt_generator_count()==2);

    WT_GENERATOR* pwt_generator;
    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pwt_generator = psdb.get_wt_generator(did);

    TEST_ASSERT(pwt_generator!=NULL);
    TEST_ASSERT(pwt_generator->get_source_bus()==1);
    TEST_ASSERT(pwt_generator->get_identifier()=="#1");
    TEST_ASSERT(pwt_generator->get_status()==true);

    did.set_device_identifier("#2");

    pwt_generator = psdb.get_wt_generator(did);

    TEST_ASSERT(pwt_generator!=NULL);
    TEST_ASSERT(pwt_generator->get_source_bus()==1);
    TEST_ASSERT(pwt_generator->get_identifier()=="#2");
    TEST_ASSERT(pwt_generator->get_status()==false);
}


void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_pv_unit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(18.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    psdb.append_bus(bus);

    PV_UNIT pv_unit;
    pv_unit.set_toolkit(default_toolkit);

    pv_unit.set_unit_bus(1);
    pv_unit.set_identifier("#1");
    pv_unit.set_status(true);

    psdb.append_pv_unit(pv_unit);

    pv_unit.set_unit_bus(1);
    pv_unit.set_identifier("#2");
    pv_unit.set_status(false);

    psdb.append_pv_unit(pv_unit);

    TEST_ASSERT(psdb.get_pv_unit_count()==2);

    PV_UNIT* ppv_unit;
    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    ppv_unit = psdb.get_pv_unit(did);

    TEST_ASSERT(ppv_unit!=NULL);
    TEST_ASSERT(ppv_unit->get_source_bus()==1);
    TEST_ASSERT(ppv_unit->get_identifier()=="#1");
    TEST_ASSERT(ppv_unit->get_status()==true);

    did.set_device_identifier("#2");

    ppv_unit = psdb.get_pv_unit(did);

    TEST_ASSERT(ppv_unit!=NULL);
    TEST_ASSERT(ppv_unit->get_source_bus()==1);
    TEST_ASSERT(ppv_unit->get_identifier()=="#2");
    TEST_ASSERT(ppv_unit->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(35.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    psdb.append_bus(bus);

    LOAD load;
    load.set_toolkit(default_toolkit);

    load.set_load_bus(1);
    load.set_identifier("#1");
    load.set_status(true);

    psdb.append_load(load);

    load.set_load_bus(1);
    load.set_identifier("#2");
    load.set_status(false);

    psdb.append_load(load);

    TEST_ASSERT(psdb.get_load_count()==2);

    LOAD* pload;
    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pload = psdb.get_load(did);

    TEST_ASSERT(pload!=NULL);
    TEST_ASSERT(pload->get_load_bus()==1);
    TEST_ASSERT(pload->get_identifier()=="#1");
    TEST_ASSERT(pload->get_status()==true);

    did.set_device_identifier("#2");

    pload = psdb.get_load(did);

    TEST_ASSERT(pload!=NULL);
    TEST_ASSERT(pload->get_load_bus()==1);
    TEST_ASSERT(pload->get_identifier()=="#2");
    TEST_ASSERT(pload->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_line()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(110.0);
    psdb.append_bus(bus);

    LINE line;
    line.set_toolkit(default_toolkit);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("#1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);

    psdb.append_line(line);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("#2");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(false);

    psdb.append_line(line);

    TEST_ASSERT(psdb.get_line_count()==2);

    LINE* pline;
    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pline = psdb.get_line(did);

    TEST_ASSERT(pline!=NULL);
    TEST_ASSERT(pline->get_sending_side_bus()==1);
    TEST_ASSERT(pline->get_receiving_side_bus()==2);
    TEST_ASSERT(pline->get_identifier()=="#1");
    TEST_ASSERT(pline->get_sending_side_breaker_status()==true);
    TEST_ASSERT(pline->get_receiving_side_breaker_status()==true);

    did.set_device_identifier("#2");

    pline = psdb.get_line(did);

    TEST_ASSERT(pline!=NULL);
    TEST_ASSERT(pline->get_sending_side_bus()==1);
    TEST_ASSERT(pline->get_receiving_side_bus()==2);
    TEST_ASSERT(pline->get_identifier()=="#2");
    TEST_ASSERT(pline->get_sending_side_breaker_status()==true);
    TEST_ASSERT(pline->get_receiving_side_breaker_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_transformer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(525.0);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(230.0);
    psdb.append_bus(bus);

    bus.set_bus_number(3);
    bus.set_base_voltage_in_kV(35.0);
    psdb.append_bus(bus);

    TRANSFORMER transformer;
    transformer.set_toolkit(default_toolkit);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 0);
    transformer.set_identifier("#1");
    transformer.set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer.set_winding_breaker_status(SECONDARY_SIDE, true);

    psdb.append_transformer(transformer);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 3);
    transformer.set_identifier("#2");
    transformer.set_winding_breaker_status(PRIMARY_SIDE, true);
    transformer.set_winding_breaker_status(SECONDARY_SIDE, true);
    transformer.set_winding_breaker_status(TERTIARY_SIDE, true);

    psdb.append_transformer(transformer);

    TEST_ASSERT(psdb.get_transformer_count()==2);

    TRANSFORMER* ptransformer;
    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    ptransformer = psdb.get_transformer(did);

    TEST_ASSERT(ptransformer!=NULL);
    TEST_ASSERT(ptransformer->is_two_winding_transformer()==true);
    TEST_ASSERT(ptransformer->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(ptransformer->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(ptransformer->get_identifier()=="#1");
    TEST_ASSERT(ptransformer->get_winding_breaker_status(PRIMARY_SIDE)==true);
    TEST_ASSERT(ptransformer->get_winding_breaker_status(SECONDARY_SIDE)==true);

    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    ptransformer = psdb.get_transformer(did);

    TEST_ASSERT(ptransformer!=NULL);
    TEST_ASSERT(ptransformer->is_three_winding_transformer()==true);
    TEST_ASSERT(ptransformer->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(ptransformer->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(ptransformer->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(ptransformer->get_identifier()=="#2");
    TEST_ASSERT(ptransformer->get_winding_breaker_status(PRIMARY_SIDE)==true);
    TEST_ASSERT(ptransformer->get_winding_breaker_status(SECONDARY_SIDE)==true);
    TEST_ASSERT(ptransformer->get_winding_breaker_status(TERTIARY_SIDE)==true);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_fixed_shunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    psdb.append_bus(bus);

    FIXED_SHUNT shunt;
    shunt.set_toolkit(default_toolkit);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("#1");
    shunt.set_status(true);

    psdb.append_fixed_shunt(shunt);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("#2");
    shunt.set_status(false);

    psdb.append_fixed_shunt(shunt);

    TEST_ASSERT(psdb.get_fixed_shunt_count()==2);

    FIXED_SHUNT* pshunt;
    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pshunt = psdb.get_fixed_shunt(did);

    TEST_ASSERT(pshunt!=NULL);
    TEST_ASSERT(pshunt->get_shunt_bus()==1);
    TEST_ASSERT(pshunt->get_identifier()=="#1");
    TEST_ASSERT(pshunt->get_status()==true);

    did.set_device_identifier("#2");

    pshunt = psdb.get_fixed_shunt(did);

    TEST_ASSERT(pshunt!=NULL);
    TEST_ASSERT(pshunt->get_shunt_bus()==1);
    TEST_ASSERT(pshunt->get_identifier()=="#2");
    TEST_ASSERT(pshunt->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(525.0);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(525.0);
    psdb.append_bus(bus);

    HVDC hvdc;
    hvdc.set_toolkit(default_toolkit);

    hvdc.set_name("DC1");
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("#1");
    hvdc.set_status(true);

    psdb.append_hvdc(hvdc);

    hvdc.set_name("DC2");
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("#2");
    hvdc.set_status(false);

    psdb.append_hvdc(hvdc);

    TEST_ASSERT(psdb.get_hvdc_count()==2);

    HVDC* phvdc;
    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    phvdc = psdb.get_hvdc(did);

    TEST_ASSERT(phvdc!=NULL);
    TEST_ASSERT(phvdc->get_name()=="DC1");
    TEST_ASSERT(phvdc->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(phvdc->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(phvdc->get_identifier()=="#1");
    TEST_ASSERT(phvdc->get_status()==true);

    did.set_device_identifier("#2");
    phvdc = psdb.get_hvdc(did);

    TEST_ASSERT(phvdc!=NULL);
    TEST_ASSERT(phvdc->get_name()=="DC2");
    TEST_ASSERT(phvdc->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(phvdc->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(phvdc->get_identifier()=="#2");
    TEST_ASSERT(phvdc->get_status()==false);
}


void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_equivalent_device()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(525.0);
    psdb.append_bus(bus);

    EQUIVALENT_DEVICE edevice;
    edevice.set_toolkit(default_toolkit);

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("#1");
    edevice.set_status(true);

    psdb.append_equivalent_device(edevice);

    edevice.set_identifier("#2");
    edevice.set_status(false);

    psdb.append_equivalent_device(edevice);

    TEST_ASSERT(psdb.get_equivalent_device_count()==2);

    EQUIVALENT_DEVICE* pedevice;
    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pedevice = psdb.get_equivalent_device(did);

    TEST_ASSERT(pedevice!=NULL);
    TEST_ASSERT(pedevice->get_equivalent_device_bus()==1);
    TEST_ASSERT(pedevice->get_identifier()=="#1");
    TEST_ASSERT(pedevice->get_status()==true);

    did.set_device_identifier("#2");
    pedevice = psdb.get_equivalent_device(did);

    TEST_ASSERT(pedevice!=NULL);
    TEST_ASSERT(pedevice->get_equivalent_device_bus()==1);
    TEST_ASSERT(pedevice->get_identifier()=="#2");
    TEST_ASSERT(pedevice->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_energy_storage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(525.0);
    psdb.append_bus(bus);

    ENERGY_STORAGE estorage;
    estorage.set_toolkit(default_toolkit);

    estorage.set_energy_storage_bus(1);
    estorage.set_identifier("#1");
    estorage.set_status(true);

    psdb.append_energy_storage(estorage);

    estorage.set_identifier("#2");
    estorage.set_status(false);

    psdb.append_energy_storage(estorage);

    TEST_ASSERT(psdb.get_energy_storage_count()==2);

    ENERGY_STORAGE* pestorage;
    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pestorage = psdb.get_energy_storage(did);

    TEST_ASSERT(pestorage!=NULL);
    TEST_ASSERT(pestorage->get_energy_storage_bus()==1);
    TEST_ASSERT(pestorage->get_identifier()=="#1");
    TEST_ASSERT(pestorage->get_status()==true);

    did.set_device_identifier("#2");
    pestorage = psdb.get_energy_storage(did);

    TEST_ASSERT(pestorage!=NULL);
    TEST_ASSERT(pestorage->get_energy_storage_bus()==1);
    TEST_ASSERT(pestorage->get_identifier()=="#2");
    TEST_ASSERT(pestorage->get_status()==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_area_number(1);
    bus.set_base_voltage_in_kV(525.0);
    bus.set_bus_type(SLACK_TYPE);
    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_area_number(2);
    bus.set_base_voltage_in_kV(525.0);
    bus.set_bus_type(SLACK_TYPE);
    psdb.append_bus(bus);

    AREA area;
    area.set_toolkit(default_toolkit);

    area.set_area_number(1);
    area.set_area_name("area 1");
    area.set_area_swing_bus(1);
    area.set_expected_power_leaving_area_in_MW(20.0);
    psdb.append_area(area);

    TEST_ASSERT(psdb.get_area_count()==1);

    AREA* parea;

    parea = psdb.get_area(1);

    TEST_ASSERT(parea!=NULL);
    TEST_ASSERT(parea->get_area_number()==1);
    TEST_ASSERT(parea->get_area_name()=="area 1");
    TEST_ASSERT(parea->get_area_swing_bus()==1);
    TEST_ASSERT(fabs(parea->get_expected_power_leaving_area_in_MW()-20.0)<FLOAT_EPSILON);


    area.set_area_number(2);
    area.set_area_name("area 2");
    area.set_area_swing_bus(2);
    area.set_expected_power_leaving_area_in_MW(20.0);
    psdb.append_area(area);

    TEST_ASSERT(psdb.get_area_count()==2);

    parea = psdb.get_area(2);

    TEST_ASSERT(parea!=NULL);
    TEST_ASSERT(parea->get_area_number()==2);
    TEST_ASSERT(parea->get_area_name()=="area 2");
    TEST_ASSERT(parea->get_area_swing_bus()==2);
    TEST_ASSERT(fabs(parea->get_expected_power_leaving_area_in_MW()-20.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    ZONE zone;
    zone.set_toolkit(default_toolkit);

    zone.set_zone_number(1);
    zone.set_zone_name("zone 1");
    psdb.append_zone(zone);

    TEST_ASSERT(psdb.get_zone_count()==1);

    ZONE* pzone;

    pzone = psdb.get_zone(1);

    TEST_ASSERT(pzone!=NULL);
    TEST_ASSERT(pzone->get_zone_number()==1);
    TEST_ASSERT(pzone->get_zone_name()=="zone 1");

    zone.set_zone_number(2);
    zone.set_zone_name("zone 2");
    psdb.append_zone(zone);

    TEST_ASSERT(psdb.get_zone_count()==2);

    pzone = psdb.get_zone(2);

    TEST_ASSERT(pzone!=NULL);
    TEST_ASSERT(pzone->get_zone_number()==2);
    TEST_ASSERT(pzone->get_zone_name()=="zone 2");
}

void POWER_SYSTEM_DATABASE_TEST::test_append_and_get_owner()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(100);

    OWNER owner;
    owner.set_toolkit(default_toolkit);

    owner.set_owner_number(1);
    owner.set_owner_name("owner 1");
    psdb.append_owner(owner);

    TEST_ASSERT(psdb.get_owner_count()==1);

    OWNER* powner;

    powner = psdb.get_owner(1);

    TEST_ASSERT(powner!=NULL);
    TEST_ASSERT(powner->get_owner_number()==1);
    TEST_ASSERT(powner->get_owner_name()=="owner 1");

    owner.set_owner_number(2);
    owner.set_owner_name("owner 2");
    psdb.append_owner(owner);

    TEST_ASSERT(psdb.get_owner_count()==2);

    powner = psdb.get_owner(2);

    TEST_ASSERT(powner!=NULL);
    TEST_ASSERT(powner->get_owner_number()==2);
    TEST_ASSERT(powner->get_owner_name()=="owner 2");
}


void POWER_SYSTEM_DATABASE_TEST::test_is_bus_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.is_bus_exist(1)==true);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.is_bus_exist(4)==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_is_generator_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_generator_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_generator_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_generator_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_wt_generator_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");
        TEST_ASSERT(psdb.is_wt_generator_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_wt_generator_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_is_pv_unit_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");
        TEST_ASSERT(psdb.is_pv_unit_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_pv_unit_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_load_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_load_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_load_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_load_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_line_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_line_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_line_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_line_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_transformer_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");
        TEST_ASSERT(psdb.is_transformer_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_transformer_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    }

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==true);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.is_transformer_exist(did)==true);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_is_fixed_shunt_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    for(size_t i=1; i!=4; i++)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_hvdc_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_hvdc_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_hvdc_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_equivalent_device_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_equivalent_device_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_equivalent_device_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_energy_storage_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(psdb.is_energy_storage_exist(did)==true);

        did.set_device_identifier("#2");
        TEST_ASSERT(psdb.is_energy_storage_exist(did)==true);

        did.set_device_identifier("#3");
        TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_is_area_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    for(size_t i=1; i!=4; ++i)
        TEST_ASSERT(psdb.is_area_exist(i)==true);

    TEST_ASSERT(psdb.is_area_exist(4)==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_is_zone_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    for(size_t i=1; i!=4; ++i)
        TEST_ASSERT(psdb.is_zone_exist(i)==true);
    TEST_ASSERT(psdb.is_zone_exist(4)==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_is_owner_exist()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    for(size_t i=1; i!=4; ++i)
        TEST_ASSERT(psdb.is_owner_exist(i)==true);
    TEST_ASSERT(psdb.is_owner_exist(4)==false);
}

void POWER_SYSTEM_DATABASE_TEST::test_change_bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;
    osstream<<"Known bug of change_bus_number(): only terminal bus can be changed. buses for additional control, i.e., generator control bus is not changed.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    prepare_database_for_test();

    TEST_ASSERT(psdb.is_bus_exist(1)==true);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.is_bus_exist(4)==false);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_generator_exist(did)==true);

    did.set_device_type("LOAD");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_load_exist(did)==true);

    did.set_device_type("FIXED SHUNT");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==true);

    did.set_device_type("LINE");
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_line_exist(did)==true);

    did.set_device_type("HVDC");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_hvdc_exist(did)==true);

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==true);

    psdb.change_bus_number(1, 4);

    TEST_ASSERT(psdb.is_bus_exist(1)==false);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.is_bus_exist(4)==true);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_generator_exist(did)==false);

    did.set_device_type("LOAD");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_load_exist(did)==false);

    did.set_device_type("FIXED SHUNT");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);

    did.set_device_type("LINE");
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_line_exist(did)==false);

    did.set_device_type("HVDC");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(4);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_generator_exist(did)==true);

    did.set_device_type("LOAD");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_load_exist(did)==true);

    did.set_device_type("FIXED SHUNT");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==true);

    did.set_device_type("LINE");
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_line_exist(did)==true);

    did.set_device_type("HVDC");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_hvdc_exist(did)==true);

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==true);

    psdb.change_bus_number(2, 1);

    TEST_ASSERT(psdb.is_bus_exist(1)==true);
    TEST_ASSERT(psdb.is_bus_exist(2)==false);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.is_bus_exist(4)==true);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_generator_exist(did)==false);

    did.set_device_type("LOAD");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_load_exist(did)==false);

    did.set_device_type("FIXED SHUNT");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);

    did.set_device_type("LINE");
    terminal.append_bus(4);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_line_exist(did)==false);

    did.set_device_type("HVDC");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_generator_exist(did)==true);

    did.set_device_type("LOAD");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_load_exist(did)==true);

    did.set_device_type("FIXED SHUNT");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==true);

    did.set_device_type("LINE");
    terminal.append_bus(4);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_line_exist(did)==true);

    did.set_device_type("HVDC");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_hvdc_exist(did)==true);

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.is_transformer_exist(did)==true);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_generators_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> device;

    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_generators_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_generator_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_generator_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_generators_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_wt_generators_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_source_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_source_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_wt_generators_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_pv_units_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_unit_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_source_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_pv_units_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_sources_connecting_to_bus(i);

        TEST_ASSERT(device.size()==8);

        TEST_ASSERT(device[0]->get_source_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_source_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
        TEST_ASSERT(device[2]->get_source_bus()==i);
        TEST_ASSERT(device[2]->get_identifier()=="#1");
        TEST_ASSERT(device[3]->get_source_bus()==i);
        TEST_ASSERT(device[3]->get_identifier()=="#2");
        TEST_ASSERT(device[4]->get_source_bus()==i);
        TEST_ASSERT(device[4]->get_identifier()=="#1");
        TEST_ASSERT(device[5]->get_source_bus()==i);
        TEST_ASSERT(device[5]->get_identifier()=="#2");
        TEST_ASSERT(device[6]->get_source_bus()==i);
        TEST_ASSERT(device[6]->get_identifier()=="#1");
        TEST_ASSERT(device[7]->get_source_bus()==i);
        TEST_ASSERT(device[7]->get_identifier()=="#2");
    }

    device = psdb.get_sources_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_loads_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_load_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_load_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_loads_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LINE*> device = psdb.get_lines_connecting_to_bus(1);

    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_sending_side_bus()==1);
    TEST_ASSERT(device[0]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_sending_side_bus()==1);
    TEST_ASSERT(device[1]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_sending_side_bus()==1);
    TEST_ASSERT(device[2]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_sending_side_bus()==1);
    TEST_ASSERT(device[3]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_lines_connecting_to_bus(2);

    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_sending_side_bus()==1);
    TEST_ASSERT(device[0]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_sending_side_bus()==1);
    TEST_ASSERT(device[1]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_sending_side_bus()==2);
    TEST_ASSERT(device[2]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_sending_side_bus()==2);
    TEST_ASSERT(device[3]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_lines_connecting_to_bus(3);
    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_sending_side_bus()==2);
    TEST_ASSERT(device[0]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_sending_side_bus()==2);
    TEST_ASSERT(device[1]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_sending_side_bus()==1);
    TEST_ASSERT(device[2]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_sending_side_bus()==1);
    TEST_ASSERT(device[3]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_lines_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<TRANSFORMER*> device = psdb.get_transformers_connecting_to_bus(1);

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[0]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[0]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[1]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[1]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[2]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[2]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[3]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[3]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[4]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[4]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[4]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[5]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[5]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[5]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");

    device = psdb.get_transformers_connecting_to_bus(2);

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[0]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[0]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[1]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[1]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[2]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[2]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[3]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[3]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[4]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[4]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[4]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[5]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[5]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[5]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");

    device = psdb.get_transformers_connecting_to_bus(3);

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[0]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[0]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[1]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[1]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[2]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[2]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[3]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[3]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[4]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[4]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[4]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[5]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[5]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[5]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");

    device = psdb.get_transformers_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<FIXED_SHUNT*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_fixed_shunts_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_shunt_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_shunt_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_fixed_shunts_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<HVDC*> device = psdb.get_hvdcs_connecting_to_bus(1);

    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[0]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[1]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[2]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[3]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_hvdcs_connecting_to_bus(2);

    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[0]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[1]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[2]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[3]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_hvdcs_connecting_to_bus(3);

    TEST_ASSERT(device.size()==4);

    TEST_ASSERT(device[0]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[0]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[1]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[2]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[3]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");

    device = psdb.get_hvdcs_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<EQUIVALENT_DEVICE*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_equivalent_devices_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_equivalent_device_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_equivalent_device_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_equivalent_devices_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ENERGY_STORAGE*> device;
    for(size_t i=1; i!=4; ++i)
    {
        device = psdb.get_energy_storages_connecting_to_bus(i);

        TEST_ASSERT(device.size()==2);

        TEST_ASSERT(device[0]->get_energy_storage_bus()==i);
        TEST_ASSERT(device[0]->get_identifier()=="#1");
        TEST_ASSERT(device[1]->get_energy_storage_bus()==i);
        TEST_ASSERT(device[1]->get_identifier()=="#2");
    }

    device = psdb.get_energy_storages_connecting_to_bus(4);
    TEST_ASSERT(device.size()==0);
}



void POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_generators_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="GENERATOR");
            TEST_ASSERT(psdb.get_generator(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_wt_generators_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="WT GENERATOR");
            TEST_ASSERT(psdb.get_wt_generator(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_pv_units_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="PV UNIT");
            TEST_ASSERT(psdb.get_pv_unit(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_sources_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==8);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="GENERATOR" or dids[j].get_device_type()=="WT GENERATOR"
                        or dids[j].get_device_type()=="PV UNIT" or dids[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_source(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_loads_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="LOAD");
            TEST_ASSERT(psdb.get_load(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_lines_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="LINE");
            TEST_ASSERT(psdb.get_line(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_transformers_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==6);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="TRANSFORMER");
            TEST_ASSERT(psdb.get_transformer(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_fixed_shunts_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="FIXED SHUNT");
            TEST_ASSERT(psdb.get_fixed_shunt(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_hvdcs_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="HVDC");
            TEST_ASSERT(psdb.get_hvdc(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_equivalent_devices_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="EQUIVALENT DEVICE");
            TEST_ASSERT(psdb.get_equivalent_device(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> dids;

    for(size_t i=1; i!=4; ++i)
    {
        dids = psdb.get_energy_storages_device_id_connecting_to_bus(i);
        TEST_ASSERT(dids.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(dids[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_energy_storage(dids[j])->is_connected_to_bus(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_buses_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<BUS*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_buses_in_area(i);
        TEST_ASSERT(devices.size()==1);
        for(size_t j=0; j!=1; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="BUS");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generators_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_generators_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="GENERATOR");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_wt_generators_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="WT GENERATOR");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_pv_units_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="PV UNIT");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_sources_in_area(i);
        TEST_ASSERT(devices.size()==8);
        for(size_t j=0; j!=8; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="GENERATOR"
                        or devices[j]->get_device_id().get_device_type()=="WT GENERATOR"
                        or devices[j]->get_device_id().get_device_type()=="PV UNIT"
                        or devices[j]->get_device_id().get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_loads_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="LOAD");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LINE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_lines_in_area(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="LINE");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<TRANSFORMER*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_transformers_in_area(i);
        TEST_ASSERT(devices.size()==6);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="TRANSFORMER");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<FIXED_SHUNT*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_fixed_shunts_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="FIXED SHUNT");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<HVDC*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_hvdcs_in_area(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="HVDC");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<EQUIVALENT_DEVICE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_equivalent_devices_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="EQUIVALENT DEVICE");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ENERGY_STORAGE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_energy_storages_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(devices[j]->is_in_area(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_buses_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_buses_device_id_in_area(i);
        TEST_ASSERT(devices.size()==1);
        for(size_t j=0; j!=1; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="BUS");
            TEST_ASSERT(psdb.get_bus(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_generators_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="GENERATOR");
            TEST_ASSERT(psdb.get_generator(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_wt_generators_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="WT GENERATOR");
            TEST_ASSERT(psdb.get_wt_generator(devices[j])->is_in_area(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_pv_units_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="PV UNIT");
            TEST_ASSERT(psdb.get_pv_unit(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_sources_device_id_in_area(i);
        TEST_ASSERT(devices.size()==8);
        for(size_t j=0; j!=8; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="GENERATOR"
                        or devices[j].get_device_type()=="WT GENERATOR"
                        or devices[j].get_device_type()=="PV UNIT"
                        or devices[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_source(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_loads_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="LOAD");
            TEST_ASSERT(psdb.get_load(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_lines_device_id_in_area(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="LINE");
            TEST_ASSERT(psdb.get_line(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_transformers_device_id_in_area(i);
        TEST_ASSERT(devices.size()==6);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="TRANSFORMER");
            TEST_ASSERT(psdb.get_transformer(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_fixed_shunts_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="FIXED SHUNT");
            TEST_ASSERT(psdb.get_fixed_shunt(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_hvdcs_device_id_in_area(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="HVDC");
            TEST_ASSERT(psdb.get_hvdc(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_equivalent_devices_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="EQUIVALENT DEVICE");
            TEST_ASSERT(psdb.get_equivalent_device(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_energy_storages_device_id_in_area(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_energy_storage(devices[j])->is_in_area(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_buses_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<BUS*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_buses_in_zone(i);
        TEST_ASSERT(devices.size()==1);
        for(size_t j=0; j!=1; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="BUS");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generators_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_generators_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="GENERATOR");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_wt_generators_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="WT GENERATOR");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_pv_units_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="PV UNIT");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_sources_in_zone(i);
        TEST_ASSERT(devices.size()==8);
        for(size_t j=0; j!=8; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="GENERATOR"
                        or devices[j]->get_device_id().get_device_type()=="WT GENERATOR"
                        or devices[j]->get_device_id().get_device_type()=="PV UNIT"
                        or devices[j]->get_device_id().get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_loads_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="LOAD");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LINE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_lines_in_zone(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="LINE");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<TRANSFORMER*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_transformers_in_zone(i);
        TEST_ASSERT(devices.size()==6);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="TRANSFORMER");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<FIXED_SHUNT*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_fixed_shunts_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="FIXED SHUNT");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<HVDC*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_hvdcs_in_zone(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="HVDC");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<EQUIVALENT_DEVICE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_equivalent_devices_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="EQUIVALENT DEVICE");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ENERGY_STORAGE*> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_energy_storages_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j]->get_device_id().get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(devices[j]->is_in_zone(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_buses_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_buses_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==1);
        for(size_t j=0; j!=1; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="BUS");
            TEST_ASSERT(psdb.get_bus(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generators_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_generators_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="GENERATOR");
            TEST_ASSERT(psdb.get_generator(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generators_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_wt_generators_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="WT GENERATOR");
            TEST_ASSERT(psdb.get_wt_generator(devices[j])->is_in_zone(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_units_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_pv_units_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="PV UNIT");
            TEST_ASSERT(psdb.get_pv_unit(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_sources_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_sources_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==8);
        for(size_t j=0; j!=8; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="GENERATOR"
                        or devices[j].get_device_type()=="WT GENERATOR"
                        or devices[j].get_device_type()=="PV UNIT"
                        or devices[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_source(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_loads_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_loads_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="LOAD");
            TEST_ASSERT(psdb.get_load(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_lines_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_lines_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="LINE");
            TEST_ASSERT(psdb.get_line(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformers_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_transformers_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==6);
        for(size_t j=0; j!=6; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="TRANSFORMER");
            TEST_ASSERT(psdb.get_transformer(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunts_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_fixed_shunts_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="FIXED SHUNT");
            TEST_ASSERT(psdb.get_fixed_shunt(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdcs_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_hvdcs_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==4);
        for(size_t j=0; j!=4; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="HVDC");
            TEST_ASSERT(psdb.get_hvdc(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_devices_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_equivalent_devices_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="EQUIVALENT DEVICE");
            TEST_ASSERT(psdb.get_equivalent_device(devices[j])->is_in_zone(i)==true);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storages_device_id_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> devices;

    for(size_t i=1; i!=3; ++i)
    {
        devices = psdb.get_energy_storages_device_id_in_zone(i);
        TEST_ASSERT(devices.size()==2);
        for(size_t j=0; j!=2; ++j)
        {
            TEST_ASSERT(devices[j].get_device_type()=="ENERGY STORAGE");
            TEST_ASSERT(psdb.get_energy_storage(devices[j])->is_in_zone(i)==true);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_all_devices()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE*> device;
    device = psdb.get_all_devices();

    TEST_ASSERT(device.size()==3+6*9+8);

    // all devices in the test database are:
    // Bus: 1, 2, 3
    // Generator: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // WT generator: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // PV UNIT: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Load: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Fixed shunt: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Line: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2,
    // Transformer: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2, 1-2-3-#1, 1-2-3-#2
    // Hvdc: 1-2-#1, 1-2-#2, 2-3-#1, 2-3-#2, 1-3-#1, 1-3-#2,
    // Equivalent device: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Energy storage: 1-#1, 1-#2, 2-#1, 2-#2, 3-#1, 3-#2
    // Area: 1, 2, 3
    // Zone: 1, 2, 3
    // Owner: 1, 2, 3
}


void POWER_SYSTEM_DATABASE_TEST::test_get_all_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<BUS*> device = psdb.get_all_buses();

    TEST_ASSERT(device.size()==3);
    TEST_ASSERT(device[0]->get_bus_number()==1);
    TEST_ASSERT(device[1]->get_bus_number()==2);
    TEST_ASSERT(device[2]->get_bus_number()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_buses_with_constraints()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<BUS*> device = psdb.get_buses_with_constraints(100.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(device.size()==3);
    TEST_ASSERT(device[0]->get_bus_number()==1);
    TEST_ASSERT(device[1]->get_bus_number()==2);
    TEST_ASSERT(device[2]->get_bus_number()==3);

    device = psdb.get_buses_with_constraints(200.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(device.size()==2);
    TEST_ASSERT(device[0]->get_bus_number()==2);
    TEST_ASSERT(device[1]->get_bus_number()==3);

    device = psdb.get_buses_with_constraints(300.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(device.size()==1);
    TEST_ASSERT(device[0]->get_bus_number()==3);

    for(size_t area=1; area!=4; ++area)
    {
        size_t bus = area;
        device = psdb.get_buses_with_constraints(100.0, 400.0, 0.9, 1.1, area, 0, 0);

        TEST_ASSERT(device.size()==1);
        TEST_ASSERT(device[0]->get_bus_number()==bus);

        size_t zone = area;
        device = psdb.get_buses_with_constraints(100.0, 400.0, 0.9, 1.1, area, zone, 0);

        TEST_ASSERT(device.size()==1);
        TEST_ASSERT(device[0]->get_bus_number()==bus);

        size_t owner = area;
        device = psdb.get_buses_with_constraints(100.0, 400.0, 0.9, 1.1, area, zone, owner);

        TEST_ASSERT(device.size()==1);
        TEST_ASSERT(device[0]->get_bus_number()==bus);
    }
    device = psdb.get_buses_with_constraints(100.0, 400.0, 0.9, 1.1, 1, 2, 0);

    TEST_ASSERT(device.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_in_service_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<BUS*> device = psdb.get_all_in_service_buses();

    TEST_ASSERT(device.size()==3);
    TEST_ASSERT(device[0]->get_bus_number()==1);
    TEST_ASSERT(device[1]->get_bus_number()==2);
    TEST_ASSERT(device[2]->get_bus_number()==3);

    BUS* bus = psdb.get_bus(2);
    bus->set_bus_type(OUT_OF_SERVICE);

    device = psdb.get_all_in_service_buses();

    TEST_ASSERT(device.size()==2);
    TEST_ASSERT(device[0]->get_bus_number()==1);
    TEST_ASSERT(device[1]->get_bus_number()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_generators()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> device = psdb.get_all_generators();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_generator_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_generator_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_generator_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_generator_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_generator_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_generator_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_wt_generators()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> device = psdb.get_all_wt_generators();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_source_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_source_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_source_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_source_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_source_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_source_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}


void POWER_SYSTEM_DATABASE_TEST::test_get_all_pv_units()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> device = psdb.get_all_pv_units();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_source_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_source_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_source_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_source_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_source_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_source_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_sources()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> device = psdb.get_all_sources();

    TEST_ASSERT(device.size()==24);

    TEST_ASSERT(device[0]->get_source_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_source_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_source_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_source_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_source_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_source_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
    TEST_ASSERT(device[6]->get_source_bus()==1);
    TEST_ASSERT(device[6]->get_identifier()=="#1");
    TEST_ASSERT(device[7]->get_source_bus()==1);
    TEST_ASSERT(device[7]->get_identifier()=="#2");
    TEST_ASSERT(device[8]->get_source_bus()==2);
    TEST_ASSERT(device[8]->get_identifier()=="#1");
    TEST_ASSERT(device[9]->get_source_bus()==2);
    TEST_ASSERT(device[9]->get_identifier()=="#2");
    TEST_ASSERT(device[10]->get_source_bus()==3);
    TEST_ASSERT(device[10]->get_identifier()=="#1");
    TEST_ASSERT(device[11]->get_source_bus()==3);
    TEST_ASSERT(device[11]->get_identifier()=="#2");
    TEST_ASSERT(device[12]->get_source_bus()==1);
    TEST_ASSERT(device[12]->get_identifier()=="#1");
    TEST_ASSERT(device[13]->get_source_bus()==1);
    TEST_ASSERT(device[13]->get_identifier()=="#2");
    TEST_ASSERT(device[14]->get_source_bus()==2);
    TEST_ASSERT(device[14]->get_identifier()=="#1");
    TEST_ASSERT(device[15]->get_source_bus()==2);
    TEST_ASSERT(device[15]->get_identifier()=="#2");
    TEST_ASSERT(device[16]->get_source_bus()==3);
    TEST_ASSERT(device[16]->get_identifier()=="#1");
    TEST_ASSERT(device[17]->get_source_bus()==3);
    TEST_ASSERT(device[17]->get_identifier()=="#2");
    TEST_ASSERT(device[18]->get_source_bus()==1);
    TEST_ASSERT(device[18]->get_identifier()=="#1");
    TEST_ASSERT(device[19]->get_source_bus()==1);
    TEST_ASSERT(device[19]->get_identifier()=="#2");
    TEST_ASSERT(device[20]->get_source_bus()==2);
    TEST_ASSERT(device[20]->get_identifier()=="#1");
    TEST_ASSERT(device[21]->get_source_bus()==2);
    TEST_ASSERT(device[21]->get_identifier()=="#2");
    TEST_ASSERT(device[22]->get_source_bus()==3);
    TEST_ASSERT(device[22]->get_identifier()=="#1");
    TEST_ASSERT(device[23]->get_source_bus()==3);
    TEST_ASSERT(device[23]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_loads()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> device = psdb.get_all_loads();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_load_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_load_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_load_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_load_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_load_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_load_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_lines()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LINE*> device = psdb.get_all_lines();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_sending_side_bus()==1);
    TEST_ASSERT(device[0]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_sending_side_bus()==1);
    TEST_ASSERT(device[1]->get_receiving_side_bus()==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_sending_side_bus()==2);
    TEST_ASSERT(device[2]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_sending_side_bus()==2);
    TEST_ASSERT(device[3]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_sending_side_bus()==1);
    TEST_ASSERT(device[4]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_sending_side_bus()==1);
    TEST_ASSERT(device[5]->get_receiving_side_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_transformers()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<TRANSFORMER*> device = psdb.get_all_transformers();

    TEST_ASSERT(device.size()==8);

    TEST_ASSERT(device[0]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[0]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[0]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[1]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[1]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[2]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[2]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[3]->get_winding_bus(PRIMARY_SIDE)==2);
    TEST_ASSERT(device[3]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[4]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[4]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->is_two_winding_transformer()==true);
    TEST_ASSERT(device[5]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[5]->get_winding_bus(SECONDARY_SIDE)==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
    TEST_ASSERT(device[6]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[6]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[6]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[6]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[6]->get_identifier()=="#1");
    TEST_ASSERT(device[7]->is_three_winding_transformer()==true);
    TEST_ASSERT(device[7]->get_winding_bus(PRIMARY_SIDE)==1);
    TEST_ASSERT(device[7]->get_winding_bus(SECONDARY_SIDE)==2);
    TEST_ASSERT(device[7]->get_winding_bus(TERTIARY_SIDE)==3);
    TEST_ASSERT(device[7]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_fixed_shunts()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<FIXED_SHUNT*> device = psdb.get_all_fixed_shunts();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_shunt_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_shunt_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_shunt_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_shunt_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_shunt_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_shunt_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_hvdcs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<HVDC*> device = psdb.get_all_hvdcs();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[0]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[1]->get_converter_bus(INVERTER)==2);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[2]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_converter_bus(RECTIFIER)==2);
    TEST_ASSERT(device[3]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[4]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_converter_bus(RECTIFIER)==1);
    TEST_ASSERT(device[5]->get_converter_bus(INVERTER)==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_equivalent_devices()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<EQUIVALENT_DEVICE*> device = psdb.get_all_equivalent_devices();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_equivalent_device_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_equivalent_device_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_equivalent_device_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_equivalent_device_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_equivalent_device_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_equivalent_device_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_energy_storages()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ENERGY_STORAGE*> device = psdb.get_all_energy_storages();

    TEST_ASSERT(device.size()==6);

    TEST_ASSERT(device[0]->get_energy_storage_bus()==1);
    TEST_ASSERT(device[0]->get_identifier()=="#1");
    TEST_ASSERT(device[1]->get_energy_storage_bus()==1);
    TEST_ASSERT(device[1]->get_identifier()=="#2");
    TEST_ASSERT(device[2]->get_energy_storage_bus()==2);
    TEST_ASSERT(device[2]->get_identifier()=="#1");
    TEST_ASSERT(device[3]->get_energy_storage_bus()==2);
    TEST_ASSERT(device[3]->get_identifier()=="#2");
    TEST_ASSERT(device[4]->get_energy_storage_bus()==3);
    TEST_ASSERT(device[4]->get_identifier()=="#1");
    TEST_ASSERT(device[5]->get_energy_storage_bus()==3);
    TEST_ASSERT(device[5]->get_identifier()=="#2");
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_areas()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<AREA*> areas = psdb.get_all_areas();

    TEST_ASSERT(areas.size()==3);

    TEST_ASSERT(areas[0]->get_area_number()==1);
    TEST_ASSERT(areas[1]->get_area_number()==2);
    TEST_ASSERT(areas[2]->get_area_number()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_zones()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ZONE*> zones = psdb.get_all_zones();

    TEST_ASSERT(zones.size()==3);

    TEST_ASSERT(zones[0]->get_zone_number()==1);
    TEST_ASSERT(zones[1]->get_zone_number()==2);
    TEST_ASSERT(zones[2]->get_zone_number()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_owners()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<OWNER*> owners = psdb.get_all_owners();

    TEST_ASSERT(owners.size()==3);

    TEST_ASSERT(owners[0]->get_owner_number()==1);
    TEST_ASSERT(owners[1]->get_owner_number()==2);
    TEST_ASSERT(owners[2]->get_owner_number()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_buses_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> buses = psdb.get_all_buses_number();

    TEST_ASSERT(buses.size()==3);
    TEST_ASSERT(buses[0]==1);
    TEST_ASSERT(buses[1]==2);
    TEST_ASSERT(buses[2]==3);
}



void POWER_SYSTEM_DATABASE_TEST::test_get_buses_number_with_constraints()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> buses = psdb.get_buses_number_with_constraints(100.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(buses.size()==3);
    TEST_ASSERT(buses[0]==1);
    TEST_ASSERT(buses[1]==2);
    TEST_ASSERT(buses[2]==3);

    buses = psdb.get_buses_number_with_constraints(200.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(buses.size()==2);
    TEST_ASSERT(buses[0]==2);
    TEST_ASSERT(buses[1]==3);

    buses = psdb.get_buses_number_with_constraints(300.0, 400.0, 0.9, 1.1, 0, 0, 0);

    TEST_ASSERT(buses.size()==1);
    TEST_ASSERT(buses[0]==3);

    for(size_t area=1; area!=4; ++area)
    {
        size_t bus = area;
        buses = psdb.get_buses_number_with_constraints(100.0, 400.0, 0.9, 1.1, area, 0, 0);

        TEST_ASSERT(buses.size()==1);
        TEST_ASSERT(buses[0]==bus);

        size_t zone = area;
        buses = psdb.get_buses_number_with_constraints(100.0, 400.0, 0.9, 1.1, area, zone, 0);

        TEST_ASSERT(buses.size()==1);
        TEST_ASSERT(buses[0]==bus);

        size_t owner = area;
        buses = psdb.get_buses_number_with_constraints(100.0, 400.0, 0.9, 1.1, area, zone, owner);

        TEST_ASSERT(buses.size()==1);
        TEST_ASSERT(buses[0]==bus);
    }
    buses = psdb.get_buses_number_with_constraints(100.0, 400.0, 0.9, 1.1, 1, 2, 0);

    TEST_ASSERT(buses.size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_in_service_buses_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> buses = psdb.get_all_in_service_buses_number();

    TEST_ASSERT(buses.size()==3);
    TEST_ASSERT(buses[0]==1);
    TEST_ASSERT(buses[1]==2);
    TEST_ASSERT(buses[2]==3);

    BUS* bus = psdb.get_bus(2);
    bus->set_bus_type(OUT_OF_SERVICE);

    buses = psdb.get_all_in_service_buses_number();

    TEST_ASSERT(buses.size()==2);
    TEST_ASSERT(buses[0]==1);
    TEST_ASSERT(buses[1]==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_generators_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_generators_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_wt_generators_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_wt_generators_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_get_all_pv_units_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_pv_units_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_sources_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_sources_device_id();

    TEST_ASSERT(device.size()==24);

    DEVICE_ID did;
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        did.set_device_type("GENERATOR");
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);

        did.set_device_type("WT GENERATOR");
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[6+2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[6+2*(i-1)+1]==did);

        did.set_device_type("PV UNIT");
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[12+2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[12+2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_loads_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_loads_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_lines_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_lines_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_transformers_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_transformers_device_id();

    TEST_ASSERT(device.size()==8);

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(device[6]==did);
    did.set_device_identifier("#2");
    TEST_ASSERT(device[7]==did);

}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_fixed_shunts_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_fixed_shunts_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_hvdcs_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_hvdcs_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        if(i==1)
        {
            terminal.append_bus(1);
            terminal.append_bus(2);
        }
        if(i==2)
        {
            terminal.append_bus(2);
            terminal.append_bus(3);
        }
        if(i==3)
        {
            terminal.append_bus(1);
            terminal.append_bus(3);
        }
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_equivalent_devices_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_equivalent_devices_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_energy_storages_device_id()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<DEVICE_ID> device = psdb.get_all_energy_storages_device_id();

    TEST_ASSERT(device.size()==6);

    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    for(size_t i=1; i!=4; ++i)
    {
        terminal.clear();
        terminal.append_bus(i);
        did.set_device_terminal(terminal);
        did.set_device_identifier("#1");

        TEST_ASSERT(device[2*(i-1)]==did);

        did.set_device_identifier("#2");
        TEST_ASSERT(device[2*(i-1)+1]==did);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_areas_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> areas = psdb.get_all_areas_number();

    TEST_ASSERT(areas.size()==3);

    TEST_ASSERT(areas[0]==1);
    TEST_ASSERT(areas[1]==2);
    TEST_ASSERT(areas[2]==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_zones_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> zones = psdb.get_all_zones_number();

    TEST_ASSERT(zones.size()==3);

    TEST_ASSERT(zones[0]==1);
    TEST_ASSERT(zones[1]==2);
    TEST_ASSERT(zones[2]==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_all_owners_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<size_t> owners = psdb.get_all_owners_number();

    TEST_ASSERT(owners.size()==3);

    TEST_ASSERT(owners[0]==1);
    TEST_ASSERT(owners[1]==2);
    TEST_ASSERT(owners[2]==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_bus_count()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_in_service_bus_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_in_service_bus_count()==3);

    psdb.trip_bus(2);
    TEST_ASSERT(psdb.get_in_service_bus_count()==2);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_overshadowed_bus_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_zero_impedance_threshold_in_pu(0.001);

    psdb.set_allowed_max_bus_number(100);

    {
        BUS bus;
        bus.set_toolkit(default_toolkit);

        bus.set_bus_number(1);
        bus.set_bus_name("BUS A");
        bus.set_base_voltage_in_kV(110.0);
        bus.set_base_frequency_in_Hz(50.0);
        bus.set_bus_type(SLACK_TYPE);
        bus.set_area_number(1);
        bus.set_zone_number(1);
        bus.set_owner_number(1);
        psdb.append_bus(bus);

        bus.set_bus_number(2);
        bus.set_bus_name("BUS B");
        bus.set_base_voltage_in_kV(220.0);
        bus.set_bus_type(PV_TYPE);
        bus.set_area_number(2);
        bus.set_zone_number(2);
        bus.set_owner_number(2);
        psdb.append_bus(bus);

        bus.set_bus_number(3);
        bus.set_bus_name("BUS C");
        bus.set_base_voltage_in_kV(330.0);
        bus.set_bus_type(PQ_TYPE);
        bus.set_area_number(3);
        bus.set_zone_number(3);
        bus.set_owner_number(3);
        psdb.append_bus(bus);

        bus.set_bus_number(4);
        bus.set_bus_name("BUS D");
        bus.set_base_voltage_in_kV(330.0);
        bus.set_bus_type(PQ_TYPE);
        bus.set_area_number(3);
        bus.set_zone_number(3);
        bus.set_owner_number(3);
        psdb.append_bus(bus);

        bus.set_bus_number(5);
        bus.set_bus_name("BUS E");
        bus.set_base_voltage_in_kV(330.0);
        bus.set_bus_type(PQ_TYPE);
        bus.set_area_number(3);
        bus.set_zone_number(3);
        bus.set_owner_number(3);
        psdb.append_bus(bus);
    }

    {
        //1-2-3-5-4-1
        LINE line;
        line.set_toolkit(default_toolkit);

        line.set_sending_side_bus(1);
        line.set_receiving_side_bus(2);
        line.set_identifier("#1");
        line.set_sending_side_breaker_status(true);
        line.set_receiving_side_breaker_status(true);
        psdb.append_line(line);

        line.set_sending_side_bus(2);
        line.set_receiving_side_bus(3);
        line.set_identifier("#1");
        psdb.append_line(line);

        line.set_sending_side_bus(3);
        line.set_receiving_side_bus(5);
        line.set_identifier("#1");
        psdb.append_line(line);


        line.set_sending_side_bus(5);
        line.set_receiving_side_bus(4);
        line.set_identifier("#1");
        psdb.append_line(line);

        line.set_sending_side_bus(4);
        line.set_receiving_side_bus(1);
        line.set_identifier("#1");
        psdb.append_line(line);
    }
    DEVICE_ID did;
    did.set_device_type("LINE");
    did.set_device_identifier("#1");

    TERMINAL terminal;

    //1-2-3-5-4-1: zero loop
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    LINE* lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.001));

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.001));

    terminal.clear();
    terminal.append_bus(3);
    terminal.append_bus(5);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.001));

    terminal.clear();
    terminal.append_bus(5);
    terminal.append_bus(4);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.001));

    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.001));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==4);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==1);

    //1-2-3-5-4-1: zero loop 4-1 not zero
    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.01));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==4);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==1);

    //1-2 3-5-4 1:  4-1 2-3 not zero
    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.01));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==3);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==3);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==3);

    //1-2 3 5-4 1:  4-1 2-3 5-3 not zero
    terminal.clear();
    terminal.append_bus(5);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.01));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==2);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==4);

    //1-2 3 5 4 1:  4-1 2-3 5-3 5-4 not zero
    terminal.clear();
    terminal.append_bus(5);
    terminal.append_bus(4);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.01));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==1);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==0);

    //1 2 3 5 4 1:  all not zero
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    lineptr = psdb.get_line(did);
    lineptr->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.01));

    psdb.update_overshadowed_bus_count();
    TEST_ASSERT(psdb.get_overshadowed_bus_count()==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(1)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(2)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(3)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(4)==0);
    TEST_ASSERT(psdb.get_equivalent_bus_of_bus(5)==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generator_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_generator_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generator_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_wt_generator_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_pv_unit_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_pv_unit_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_source_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_source_count()==24);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_load_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_load_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_line_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_line_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformer_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_transformer_count()==8);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunt_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_fixed_shunt_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdc_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_hvdc_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_device_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_equivalent_device_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storage_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_energy_storage_count()==6);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_area_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_area_count()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_zone_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_zone_count()==3);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_owner_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_owner_count()==3);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_bus_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_bus_index(1)==0);
    TEST_ASSERT(psdb.get_bus_index(2)==1);
    TEST_ASSERT(psdb.get_bus_index(3)==2);
    TEST_ASSERT(psdb.get_bus_index(4)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_generator_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    TEST_ASSERT(psdb.get_generator_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_generator_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    TEST_ASSERT(psdb.get_generator_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_generator_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    TEST_ASSERT(psdb.get_generator_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_generator_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_generator_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_wt_generator_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_wt_generator_index(did)==INDEX_NOT_EXIST);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_pv_unit_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_pv_unit_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_load_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_load_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_load_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_load_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_load_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_load_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_load_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_load_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_line_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_line_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_line_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_line_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_line_index(did)==3);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_line_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_line_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_line_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_transformer_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_transformer_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_transformer_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_transformer_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_transformer_index(did)==3);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_transformer_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_transformer_index(did)==5);

    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_transformer_index(did)==6);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_transformer_index(did)==7);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_transformer_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_fixed_shunt_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_fixed_shunt_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_hvdc_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_hvdc_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_hvdc_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_hvdc_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_hvdc_index(did)==3);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_hvdc_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_hvdc_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_hvdc_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_equivalent_device_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_equivalent_device_index(did)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_energy_storage_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==0);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==1);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==2);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==3);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==4);

    did.set_device_identifier("#2");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==5);

    did.set_device_identifier("#3");
    TEST_ASSERT(psdb.get_energy_storage_index(did)==INDEX_NOT_EXIST);
}


void POWER_SYSTEM_DATABASE_TEST::test_get_area_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_area_index(1)==0);
    TEST_ASSERT(psdb.get_area_index(2)==1);
    TEST_ASSERT(psdb.get_area_index(3)==2);
    TEST_ASSERT(psdb.get_area_index(4)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_zone_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_zone_index(1)==0);
    TEST_ASSERT(psdb.get_zone_index(2)==1);
    TEST_ASSERT(psdb.get_zone_index(3)==2);
    TEST_ASSERT(psdb.get_zone_index(4)==INDEX_NOT_EXIST);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_owner_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_owner_index(1)==0);
    TEST_ASSERT(psdb.get_owner_index(2)==1);
    TEST_ASSERT(psdb.get_owner_index(3)==2);
    TEST_ASSERT(psdb.get_owner_index(4)==INDEX_NOT_EXIST);
}


void POWER_SYSTEM_DATABASE_TEST::test_bus_name2bus_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.bus_name2bus_number("BUS A")==1);
    TEST_ASSERT(psdb.bus_name2bus_number("BUS B")==2);
    TEST_ASSERT(psdb.bus_name2bus_number("BUS C")==3);
    TEST_ASSERT(psdb.bus_name2bus_number("BUS D")==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_area_name2area_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.area_name2area_number("AREA A")==1);
    TEST_ASSERT(psdb.area_name2area_number("AREA B")==2);
    TEST_ASSERT(psdb.area_name2area_number("AREA C")==3);
    TEST_ASSERT(psdb.area_name2area_number("AREA D")==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_zone_name2zone_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.zone_name2zone_number("ZONE A")==1);
    TEST_ASSERT(psdb.zone_name2zone_number("ZONE B")==2);
    TEST_ASSERT(psdb.zone_name2zone_number("ZONE C")==3);
    TEST_ASSERT(psdb.zone_name2zone_number("ZONE D")==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_owner_name2owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.owner_name2owner_number("OWNER A")==1);
    TEST_ASSERT(psdb.owner_name2owner_number("OWNER B")==2);
    TEST_ASSERT(psdb.owner_name2owner_number("OWNER C")==3);
    TEST_ASSERT(psdb.owner_name2owner_number("OWNER D")==0);

}

void POWER_SYSTEM_DATABASE_TEST::test_bus_number2bus_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.bus_number2bus_name(1)=="BUS A");
    TEST_ASSERT(psdb.bus_number2bus_name(2)=="BUS B");
    TEST_ASSERT(psdb.bus_number2bus_name(3)=="BUS C");
    TEST_ASSERT(psdb.bus_number2bus_name(4)=="");
}


void POWER_SYSTEM_DATABASE_TEST::test_area_number2area_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.area_number2area_name(1)=="AREA A");
    TEST_ASSERT(psdb.area_number2area_name(2)=="AREA B");
    TEST_ASSERT(psdb.area_number2area_name(3)=="AREA C");
    TEST_ASSERT(psdb.area_number2area_name(4)=="");
}

void POWER_SYSTEM_DATABASE_TEST::test_zone_number2zone_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.zone_number2zone_name(1)=="ZONE A");
    TEST_ASSERT(psdb.zone_number2zone_name(2)=="ZONE B");
    TEST_ASSERT(psdb.zone_number2zone_name(3)=="ZONE C");
    TEST_ASSERT(psdb.zone_number2zone_name(4)=="");
}

void POWER_SYSTEM_DATABASE_TEST::test_owner_number2owner_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.owner_number2owner_name(1)=="OWNER A");
    TEST_ASSERT(psdb.owner_number2owner_name(2)=="OWNER B");
    TEST_ASSERT(psdb.owner_number2owner_name(3)=="OWNER C");
    TEST_ASSERT(psdb.owner_number2owner_name(4)=="");
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_load_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    LOAD* load;
    complex<double> S;

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;

    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    load = psdb.get_load(did);
    S = load->get_nominal_constant_power_load_in_MVA();

    psdb.scale_load_power(did, 0.1);

    TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S*(1.0+0.1))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    load = psdb.get_load(did);
    S = load->get_nominal_constant_power_load_in_MVA();

    psdb.scale_load_power(did, -0.1);

    TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_all_load_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> loads = psdb.get_all_loads();
    vector< complex<double> > S0_P, S0_I, S0_Z;
    size_t n=loads.size();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        S0_P.push_back(load->get_nominal_constant_power_load_in_MVA());
        S0_I.push_back(load->get_nominal_constant_current_load_in_MVA());
        S0_Z.push_back(load->get_nominal_constant_impedance_load_in_MVA());
    }

    psdb.scale_all_loads_power(0.1);

    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i]*1.1)<FLOAT_EPSILON);
        TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i]*1.1)<FLOAT_EPSILON);
        TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i]*1.1)<FLOAT_EPSILON);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_at_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> loads = psdb.get_all_loads();
    vector< complex<double> > S0_P, S0_I, S0_Z;
    size_t n=loads.size();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        S0_P.push_back(load->get_nominal_constant_power_load_in_MVA());
        S0_I.push_back(load->get_nominal_constant_current_load_in_MVA());
        S0_Z.push_back(load->get_nominal_constant_impedance_load_in_MVA());
    }

    psdb.scale_loads_power_at_bus(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        if(load->is_connected_to_bus(1))
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> loads = psdb.get_all_loads();
    vector< complex<double> > S0_P, S0_I, S0_Z;
    size_t n=loads.size();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        S0_P.push_back(load->get_nominal_constant_power_load_in_MVA());
        S0_I.push_back(load->get_nominal_constant_current_load_in_MVA());
        S0_Z.push_back(load->get_nominal_constant_impedance_load_in_MVA());
    }

    psdb.scale_loads_power_in_area(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        if(load->is_in_area(1))
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_load_power_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<LOAD*> loads = psdb.get_all_loads();
    vector< complex<double> > S0_P, S0_I, S0_Z;
    size_t n=loads.size();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        S0_P.push_back(load->get_nominal_constant_power_load_in_MVA());
        S0_I.push_back(load->get_nominal_constant_current_load_in_MVA());
        S0_Z.push_back(load->get_nominal_constant_impedance_load_in_MVA());
    }

    psdb.scale_loads_power_in_zone(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        if(load->is_in_zone(1))
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i]*1.1)<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(load->get_nominal_constant_power_load_in_MVA()-S0_P[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_current_load_in_MVA()-S0_I[i])<FLOAT_EPSILON);
            TEST_ASSERT(abs(load->get_nominal_constant_impedance_load_in_MVA()-S0_Z[i])<FLOAT_EPSILON);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_scale_source_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    SOURCE* source;
    complex<double> S;

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    source = psdb.get_generator(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_source_power(did, 0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0+0.1))<FLOAT_EPSILON);

    did.set_device_type("WT GENERATOR");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    source = psdb.get_wt_generator(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_source_power(did, -0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);

    did.set_device_type("PV UNIT");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    source = psdb.get_pv_unit(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_source_power(did, -0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);

    did.set_device_type("ENERGY STORAGE");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    source = psdb.get_energy_storage(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_source_power(did, -0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_all_source_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> sources = psdb.get_all_sources();
    vector< complex<double> > S0;
    size_t n=sources.size();
    SOURCE* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_all_sources_power(0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_at_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> sources = psdb.get_all_sources();
    vector< complex<double> > S0;
    size_t n=sources.size();
    SOURCE* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_sources_power_at_bus(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_connected_to_bus(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> sources = psdb.get_all_sources();
    vector< complex<double> > S0;
    size_t n=sources.size();
    SOURCE* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_sources_power_in_area(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_area(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }

}

void POWER_SYSTEM_DATABASE_TEST::test_scale_source_power_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<SOURCE*> sources = psdb.get_all_sources();
    vector< complex<double> > S0;
    size_t n=sources.size();
    SOURCE* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_sources_power_in_zone(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_zone(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    GENERATOR* gen;
    complex<double> S;

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    gen = psdb.get_generator(did);
    S = gen->get_complex_generation_in_MVA();

    psdb.scale_generator_power(did, 0.1);

    TEST_ASSERT(abs(gen->get_complex_generation_in_MVA()-S*(1.0+0.1))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    gen = psdb.get_generator(did);
    S = gen->get_complex_generation_in_MVA();

    psdb.scale_generator_power(did, -0.1);

    TEST_ASSERT(abs(gen->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_all_generator_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> sources = psdb.get_all_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_all_generators_power(0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_at_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> sources = psdb.get_all_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_generators_power_at_bus(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_connected_to_bus(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> sources = psdb.get_all_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_generators_power_in_area(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_area(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_generator_power_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<GENERATOR*> sources = psdb.get_all_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_generators_power_in_zone(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_zone(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    WT_GENERATOR* source;
    complex<double> S;

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;

    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    source = psdb.get_wt_generator(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_wt_generator_power(did, 0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0+0.1))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    source = psdb.get_wt_generator(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_wt_generator_power(did, -0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_all_wt_generator_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    WT_GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_all_wt_generators_power(0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_at_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    WT_GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_wt_generators_power_at_bus(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_connected_to_bus(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }

}

void POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    WT_GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_wt_generators_power_in_area(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_area(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }

}

void POWER_SYSTEM_DATABASE_TEST::test_scale_wt_generator_power_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
    vector< complex<double> > S0;
    size_t n=sources.size();
    WT_GENERATOR* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_wt_generators_power_in_zone(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_zone(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}


void POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    PV_UNIT* source;
    complex<double> S;

    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;

    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    source = psdb.get_pv_unit(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_pv_unit_power(did, 0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0+0.1))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#2");

    source = psdb.get_pv_unit(did);
    S = source->get_complex_generation_in_MVA();

    psdb.scale_pv_unit_power(did, -0.1);

    TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S*(1.0-0.1))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_all_pv_unit_power()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> sources = psdb.get_all_pv_units();
    vector< complex<double> > S0;
    size_t n=sources.size();
    PV_UNIT* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_all_pv_units_power(0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_at_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> sources = psdb.get_all_pv_units();
    vector< complex<double> > S0;
    size_t n=sources.size();
    PV_UNIT* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_pv_units_power_at_bus(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_connected_to_bus(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }

}

void POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_in_area()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> sources = psdb.get_all_pv_units();
    vector< complex<double> > S0;
    size_t n=sources.size();
    PV_UNIT* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_pv_units_power_in_area(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_area(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }

}

void POWER_SYSTEM_DATABASE_TEST::test_scale_pv_unit_power_in_zone()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<PV_UNIT*> sources = psdb.get_all_pv_units();
    vector< complex<double> > S0;
    size_t n=sources.size();
    PV_UNIT* source;
    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        S0.push_back(source->get_complex_generation_in_MVA());
    }

    psdb.scale_pv_units_power_in_zone(1, 0.1);

    for(size_t i=0; i!=n; ++i)
    {
        source = sources[i];
        if(source->is_in_zone(1))
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i]*1.1)<FLOAT_EPSILON);
        }
        else
        {
            TEST_ASSERT(abs(source->get_complex_generation_in_MVA()-S0[i])<FLOAT_EPSILON);
        }
    }
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_bus_count()==3);
    TEST_ASSERT(psdb.is_bus_exist(1)==true);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==4);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==4);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==4);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==6);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==6);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==6);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==4);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==4);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==4);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==2);


    psdb.clear_bus(1);
    TEST_ASSERT(psdb.get_bus_count()==2);
    TEST_ASSERT(psdb.is_bus_exist(1)==false);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==true);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==2);


    psdb.clear_bus(3);
    TEST_ASSERT(psdb.get_bus_count()==1);
    TEST_ASSERT(psdb.is_bus_exist(1)==false);
    TEST_ASSERT(psdb.is_bus_exist(2)==true);
    TEST_ASSERT(psdb.is_bus_exist(3)==false);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==0);

    psdb.clear_bus(2);
    TEST_ASSERT(psdb.get_bus_count()==0);
    TEST_ASSERT(psdb.is_bus_exist(1)==false);
    TEST_ASSERT(psdb.is_bus_exist(2)==false);
    TEST_ASSERT(psdb.is_bus_exist(3)==false);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear();

    TEST_ASSERT(psdb.get_bus_count()==0);
    TEST_ASSERT(psdb.get_generator_count()==0);
    TEST_ASSERT(psdb.get_wt_generator_count()==0);
    TEST_ASSERT(psdb.get_pv_unit_count()==0);
    TEST_ASSERT(psdb.get_load_count()==0);
    TEST_ASSERT(psdb.get_line_count()==0);
    TEST_ASSERT(psdb.get_transformer_count()==0);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==0);
    TEST_ASSERT(psdb.get_hvdc_count()==0);
    TEST_ASSERT(psdb.get_equivalent_device_count()==0);
    TEST_ASSERT(psdb.get_area_count()==0);
    TEST_ASSERT(psdb.get_zone_count()==0);
    TEST_ASSERT(psdb.get_owner_count()==0);
}


void POWER_SYSTEM_DATABASE_TEST::test_clear_all_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_buses();

    TEST_ASSERT(psdb.get_bus_count()==0);
    TEST_ASSERT(psdb.get_generator_count()==0);
    TEST_ASSERT(psdb.get_wt_generator_count()==0);
    TEST_ASSERT(psdb.get_pv_unit_count()==0);
    TEST_ASSERT(psdb.get_load_count()==0);
    TEST_ASSERT(psdb.get_line_count()==0);
    TEST_ASSERT(psdb.get_transformer_count()==0);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==0);
    TEST_ASSERT(psdb.get_hvdc_count()==0);
    TEST_ASSERT(psdb.get_equivalent_device_count()==0);
    TEST_ASSERT(psdb.get_area_count()==0);
    TEST_ASSERT(psdb.get_zone_count()==0);
    TEST_ASSERT(psdb.get_owner_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_generator_count()==6);

    vector<GENERATOR*> gens;

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==5);

    did.set_device_identifier("#2");
    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==3);

    did.set_device_identifier("#2");
    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==1);

    did.set_device_identifier("#2");
    psdb.clear_generator(did);
    TEST_ASSERT(psdb.is_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_generator_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_generators_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_generators_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_generator_count()==4);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==2);

    psdb.clear_generators_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_generator_count()==2);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==2);

    psdb.clear_generators_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_generator_count()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_generators_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_generators()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_generators();

    TEST_ASSERT(psdb.get_generator_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_wt_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_wt_generator_count()==6);

    vector<WT_GENERATOR*> wt_generators;

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_wt_generator(did);
    TEST_ASSERT(psdb.is_wt_generator_exist(did)==false);
    TEST_ASSERT(psdb.get_wt_generator_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_wt_generators_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_wt_generators_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==2);

    psdb.clear_wt_generators_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==0);

    psdb.clear_wt_generators_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_wt_generators_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_wt_generators()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_wt_generators();

    TEST_ASSERT(psdb.get_wt_generator_count()==0);
}


void POWER_SYSTEM_DATABASE_TEST::test_clear_pv_unit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_pv_unit_count()==6);

    vector<PV_UNIT*> pv_units;

    DEVICE_ID did;
    did.set_device_type("PV UNIT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_pv_unit(did);
    TEST_ASSERT(psdb.is_pv_unit_exist(did)==false);
    TEST_ASSERT(psdb.get_pv_unit_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_pv_units_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_pv_units_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==2);

    psdb.clear_pv_units_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==0);

    psdb.clear_pv_units_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_pv_units_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_pv_units()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_pv_units();

    TEST_ASSERT(psdb.get_pv_unit_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_sources_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_source_count()==24);

    psdb.clear_sources_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_sources_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(2).size()==8);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(3).size()==8);

    psdb.clear_sources_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_sources_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(3).size()==8);

    psdb.clear_sources_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_sources_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_sources_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_sources()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_sources();
    TEST_ASSERT(psdb.get_source_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_load(did);

    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==5);

    did.set_device_identifier("#2");
    psdb.clear_load(did);

    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_load(did);
    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_load(did);
    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_load(did);
    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_load(did);
    TEST_ASSERT(psdb.is_load_exist(did)==false);
    TEST_ASSERT(psdb.get_load_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_loads_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_loads_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==2);

    psdb.clear_loads_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==2);

    psdb.clear_loads_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_loads_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_loads_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_loads()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_loads();

    TEST_ASSERT(psdb.get_load_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_line()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(psdb.get_line_count()==6);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==2);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_line(did);
    TEST_ASSERT(psdb.is_line_exist(did)==false);
    TEST_ASSERT(psdb.get_line_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_lines_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_lines_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==2);

    psdb.clear_lines_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==0);

    psdb.clear_lines_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_lines_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_lines_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_lines()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_lines();

    TEST_ASSERT(psdb.get_line_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_transformer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==7);

    did.set_device_identifier("#2");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==6);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==4);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==2);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_transformer(did);
    TEST_ASSERT(psdb.is_transformer_exist(did)==false);
    TEST_ASSERT(psdb.get_transformer_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_transformers_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_transformers_connecting_to_bus(1);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==2);

    psdb.clear_transformers_connecting_to_bus(2);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==0);

    psdb.clear_transformers_connecting_to_bus(3);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_transformers_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_transformers()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_transformers();

    TEST_ASSERT(psdb.get_transformer_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_fixed_shunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    /*ostringstream osstream;
    osstream<<"ALEEEEEEEEEEERT! clear_fixed_shunt is not functioning if using the vector::erase function.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);*/

    prepare_database_for_test();

    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_fixed_shunt(did);
    TEST_ASSERT(psdb.is_fixed_shunt_exist(did)==false);
    TEST_ASSERT(psdb.get_fixed_shunt_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_fixed_shunts_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_fixed_shunts_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==2);

    psdb.clear_fixed_shunts_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==2);

    psdb.clear_fixed_shunts_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_fixed_shunts_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_fixed_shunts()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_fixed_shunts();

    TEST_ASSERT(psdb.get_fixed_shunt_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<HVDC*> hvdcs;

    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==2);

    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_hvdc(did);
    TEST_ASSERT(psdb.is_hvdc_exist(did)==false);
    TEST_ASSERT(psdb.get_hvdc_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_hvdcs_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_hvdcs_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==2);

    psdb.clear_hvdcs_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==0);

    psdb.clear_hvdcs_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_hvdcs_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_hvdcs()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_hvdcs();

    TEST_ASSERT(psdb.get_hvdc_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_equivalent_device()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<EQUIVALENT_DEVICE*> edevices;

    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_equivalent_device(did);
    TEST_ASSERT(psdb.is_equivalent_device_exist(did)==false);
    TEST_ASSERT(psdb.get_equivalent_device_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_equivalent_devices_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_equivalent_devices_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==2);

    psdb.clear_equivalent_devices_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==2);

    psdb.clear_equivalent_devices_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_equivalent_devices_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_equivalent_devices()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_equivalent_devices();

    TEST_ASSERT(psdb.get_equivalent_device_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_energy_storage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    vector<ENERGY_STORAGE*> edevices;

    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==5);

    did.set_device_identifier("#2");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==4);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==3);

    did.set_device_identifier("#2");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==2);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==1);

    did.set_device_identifier("#2");

    psdb.clear_energy_storage(did);
    TEST_ASSERT(psdb.is_energy_storage_exist(did)==false);
    TEST_ASSERT(psdb.get_energy_storage_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_energy_storages_connecting_to_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_energy_storages_connecting_to_bus(1);

    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(2).size()==2);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(3).size()==2);

    psdb.clear_energy_storages_connecting_to_bus(2);

    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(3).size()==2);

    psdb.clear_energy_storages_connecting_to_bus(3);

    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(1).size()==0);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(2).size()==0);
    TEST_ASSERT(psdb.get_energy_storages_connecting_to_bus(3).size()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_energy_storages()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_energy_storages();

    TEST_ASSERT(psdb.get_energy_storage_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_areas()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_areas();

    TEST_ASSERT(psdb.get_area_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_zones()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_zones();

    TEST_ASSERT(psdb.get_zone_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_clear_all_owners()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    psdb.clear_all_owners();

    TEST_ASSERT(psdb.get_owner_count()==0);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_complex_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_voltage_in_pu(1.05);
    bus->set_positive_sequence_angle_in_rad(0.5);

    complex<double> V(1.05*cos(0.5), 1.05*sin(0.5));
    TEST_ASSERT(abs(psdb.get_bus_positive_sequence_complex_voltage_in_pu(1)-V)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_base_frequency()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(fabs(psdb.get_bus_base_frequency_in_Hz(1)-50.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_base_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    TEST_ASSERT(fabs(psdb.get_bus_base_voltage_in_kV(1)-110.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_voltage_in_pu(1.05);


    TEST_ASSERT(fabs(psdb.get_bus_positive_sequence_voltage_in_pu(1)-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_bus_positive_sequence_voltage_in_kV(1)-1.05*110.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_bus_angle()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_angle_in_rad(0.5);

    TEST_ASSERT(fabs(psdb.get_bus_positive_sequence_angle_in_rad(1)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_bus_positive_sequence_angle_in_deg(1)-(0.5*ONE_OVER_PI*180.0))<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_voltage_to_regulate_of_physical_bus_in_pu()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");


    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();

    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_voltage_in_pu(1.05);
    bus->set_voltage_to_regulate_in_pu(1.05);

    TEST_ASSERT(fabs(psdb.get_voltage_to_regulate_of_physical_bus_in_pu(1)-1.05)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_p_max_at_physical_bus_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                       2                     3
    // VA                      PV                    PQ
    // 110/-110   120/-120     210/-210 220/-220     310/-310 320/-320
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302

    TEST_ASSERT(fabs(psdb.get_regulatable_p_max_at_physical_bus_in_MW(1)-433.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_p_max_at_physical_bus_in_MW(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_p_max_at_physical_bus_in_MW(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_p_min_at_physical_bus_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                       2                     3
    // VA                      PV                    PQ
    // 110/-110   120/-120     210/-210 220/-220     310/-310 320/-320
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302

    TEST_ASSERT(fabs(psdb.get_regulatable_p_min_at_physical_bus_in_MW(1)-(-433.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_p_min_at_physical_bus_in_MW(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_p_min_at_physical_bus_in_MW(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_q_max_at_physical_bus_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                       2                     3
    // VA                      PV                    PQ
    // 110/-110   120/-120     210/-210 220/-220     310/-310 320/-320
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302

    TEST_ASSERT(fabs(psdb.get_regulatable_q_max_at_physical_bus_in_MVar(1)-839.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_q_max_at_physical_bus_in_MVar(2)-1639.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_q_max_at_physical_bus_in_MVar(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_regulatable_q_min_at_physical_bus_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                       2                     3
    // VA                      PV                    PQ
    // 110/-110   120/-120     210/-210 220/-220     310/-310 320/-320
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302
    // 101/-101   102/-102     201/-201 202/-202     301/-301 302/-302

    TEST_ASSERT(fabs(psdb.get_regulatable_q_min_at_physical_bus_in_MVar(1)-(-839.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_q_min_at_physical_bus_in_MVar(2)-(-1639.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_regulatable_q_min_at_physical_bus_in_MVar(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_total_regulating_p_generation_at_physical_bus_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                 2                  3
    // VA                PV                 PQ
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20

    TEST_ASSERT(fabs(psdb.get_total_regulating_p_generation_at_physical_bus_in_MW(1)-80.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_regulating_p_generation_at_physical_bus_in_MW(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_regulating_p_generation_at_physical_bus_in_MW(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_total_regulating_q_generation_at_physical_bus_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                 2                  3
    // VA                PV                 PQ
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20

    TEST_ASSERT(fabs(psdb.get_total_regulating_q_generation_at_physical_bus_in_MVar(1)-120.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_regulating_q_generation_at_physical_bus_in_MVar(2)-120.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_regulating_q_generation_at_physical_bus_in_MVar(3)-0.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_total_p_generation_of_sources_at_physical_bus_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                 2                  3
    // VA                PV                 PQ
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20

    TEST_ASSERT(fabs(psdb.get_total_p_generation_at_physical_bus_in_MW(1)-80.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_p_generation_at_physical_bus_in_MW(2)-160.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_p_generation_at_physical_bus_in_MW(3)-240.0)<FLOAT_EPSILON);
}

void POWER_SYSTEM_DATABASE_TEST::test_get_total_q_generation_of_sources_at_physical_bus_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWER_SYSTEM_DATABASE_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_database_for_test();
    // 1                 2                  3
    // VA                PV                 PQ
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    // 10/10   10/20     20/10   20/20     30/10   30/20
    TEST_ASSERT(fabs(psdb.get_total_q_generation_at_physical_bus_in_MVar(1)-120.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_q_generation_at_physical_bus_in_MVar(2)-120.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(psdb.get_total_q_generation_at_physical_bus_in_MVar(3)-120.0)<FLOAT_EPSILON);
}
#endif
