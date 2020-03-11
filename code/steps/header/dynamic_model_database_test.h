#ifndef POWER_SYSTEM_DATABASE_TEST_H
#define POWER_SYSTEM_DATABASE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class POWER_SYSTEM_DATABASE_TEST : public Test::Suite
{
    public:
        POWER_SYSTEM_DATABASE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_set_get_zero_impedance_threshold();
        void test_set_get_bus_capacity();
        void test_set_get_generator_capacity();
        void test_set_get_wt_generator_capacity();
        void test_set_get_pv_unit_capacity();
        void test_set_get_load_capacity();
        void test_set_get_fixed_shunt_capacity();
        void test_set_get_line_capacity();
        void test_set_get_transformer_capacity();
        void test_set_get_hvdc_capacity();
        void test_set_get_equivalent_device_capacity();
        void test_set_get_energy_storage_capacity();
        void test_set_get_area_capacity();
        void test_set_get_zone_capacity();
        void test_set_get_owner_capacity();

        void test_set_get_system_name();
        void test_set_get_allowed_max_bus_number();
        void test_is_bus_in_allowed_range();

        void test_set_get_system_base_power();

        void test_append_and_get_bus();
        void test_append_and_get_generator();
        void test_append_and_get_wt_generator();
        void test_append_and_get_pv_unit();
        void test_append_and_get_load();
        void test_append_and_get_line();
        void test_append_and_get_transformer();
        void test_append_and_get_fixed_shunt();
        void test_append_and_get_hvdc();
        void test_append_and_get_equivalent_device();
        void test_append_and_get_energy_storage();
        void test_append_and_get_area();
        void test_append_and_get_zone();
        void test_append_and_get_owner();

        void test_is_bus_exist();
        void test_is_generator_exist();
        void test_is_wt_generator_exist();
        void test_is_pv_unit_exist();
        void test_is_load_exist();
        void test_is_line_exist();
        void test_is_transformer_exist();
        void test_is_fixed_shunt_exist();
        void test_is_hvdc_exist();
        void test_is_equivalent_device_exist();
        void test_is_energy_storage_exist();
        void test_is_area_exist();
        void test_is_zone_exist();
        void test_is_owner_exist();

        void test_change_bus_number();

        void test_get_bus();
        void test_get_generator();
        void test_get_wt_generator();
        void test_get_pv_unit();
        void test_get_load();
        void test_get_line();
        void test_get_transformer();
        void test_get_fixed_shunt();
        void test_get_hvdc();
        void test_get_equivalent_device();
        void test_get_energy_storage();
        void test_get_area();
        void test_get_zone();
        void test_get_owner();
        void test_get_device();

        void test_get_generators_connecting_to_bus();
        void test_get_wt_generators_connecting_to_bus();
        void test_get_pv_units_connecting_to_bus();
        void test_get_sources_connecting_to_bus();
        void test_get_loads_connecting_to_bus();
        void test_get_lines_connecting_to_bus();
        void test_get_transformers_connecting_to_bus();
        void test_get_fixed_shunts_connecting_to_bus();
        void test_get_hvdcs_connecting_to_bus();
        void test_get_equivalent_devices_connecting_to_bus();
        void test_get_energy_storages_connecting_to_bus();

        void test_get_generators_device_id_connecting_to_bus();
        void test_get_wt_generators_device_id_connecting_to_bus();
        void test_get_pv_units_device_id_connecting_to_bus();
        void test_get_sources_device_id_connecting_to_bus();
        void test_get_loads_device_id_connecting_to_bus();
        void test_get_lines_device_id_connecting_to_bus();
        void test_get_transformers_device_id_connecting_to_bus();
        void test_get_fixed_shunts_device_id_connecting_to_bus();
        void test_get_hvdcs_device_id_connecting_to_bus();
        void test_get_equivalent_devices_device_id_connecting_to_bus();
        void test_get_energy_storages_device_id_connecting_to_bus();

        void test_get_buses_in_area();
        void test_get_generators_in_area();
        void test_get_wt_generators_in_area();
        void test_get_pv_units_in_area();
        void test_get_sources_in_area();
        void test_get_loads_in_area();
        void test_get_lines_in_area();
        void test_get_transformers_in_area();
        void test_get_fixed_shunts_in_area();
        void test_get_hvdcs_in_area();
        void test_get_equivalent_devices_in_area();
        void test_get_energy_storages_in_area();

        void test_get_buses_device_id_in_area();
        void test_get_generators_device_id_in_area();
        void test_get_wt_generators_device_id_in_area();
        void test_get_pv_units_device_id_in_area();
        void test_get_sources_device_id_in_area();
        void test_get_loads_device_id_in_area();
        void test_get_lines_device_id_in_area();
        void test_get_transformers_device_id_in_area();
        void test_get_fixed_shunts_device_id_in_area();
        void test_get_hvdcs_device_id_in_area();
        void test_get_equivalent_devices_device_id_in_area();
        void test_get_energy_storages_device_id_in_area();

        void test_get_buses_in_zone();
        void test_get_generators_in_zone();
        void test_get_wt_generators_in_zone();
        void test_get_pv_units_in_zone();
        void test_get_sources_in_zone();
        void test_get_loads_in_zone();
        void test_get_lines_in_zone();
        void test_get_transformers_in_zone();
        void test_get_fixed_shunts_in_zone();
        void test_get_hvdcs_in_zone();
        void test_get_equivalent_devices_in_zone();
        void test_get_energy_storages_in_zone();

        void test_get_buses_device_id_in_zone();
        void test_get_generators_device_id_in_zone();
        void test_get_wt_generators_device_id_in_zone();
        void test_get_pv_units_device_id_in_zone();
        void test_get_sources_device_id_in_zone();
        void test_get_loads_device_id_in_zone();
        void test_get_lines_device_id_in_zone();
        void test_get_transformers_device_id_in_zone();
        void test_get_fixed_shunts_device_id_in_zone();
        void test_get_hvdcs_device_id_in_zone();
        void test_get_equivalent_devices_device_id_in_zone();
        void test_get_energy_storages_device_id_in_zone();

        void test_get_all_devices();
        void test_get_all_buses();
        void test_get_buses_with_constraints();
        void test_get_all_in_service_buses();
        void test_get_all_generators();
        void test_get_all_wt_generators();
        void test_get_all_pv_units();
        void test_get_all_sources();
        void test_get_all_loads();
        void test_get_all_lines();
        void test_get_all_transformers();
        void test_get_all_fixed_shunts();
        void test_get_all_hvdcs();
        void test_get_all_equivalent_devices();
        void test_get_all_energy_storages();
        void test_get_all_areas();
        void test_get_all_zones();
        void test_get_all_owners();

        void test_get_all_buses_number();
        void test_get_buses_number_with_constraints();
        void test_get_all_in_service_buses_number();
        void test_get_all_generators_device_id();
        void test_get_all_wt_generators_device_id();
        void test_get_all_pv_units_device_id();
        void test_get_all_sources_device_id();
        void test_get_all_loads_device_id();
        void test_get_all_lines_device_id();
        void test_get_all_transformers_device_id();
        void test_get_all_fixed_shunts_device_id();
        void test_get_all_hvdcs_device_id();
        void test_get_all_equivalent_devices_device_id();
        void test_get_all_energy_storages_device_id();
        void test_get_all_areas_number();
        void test_get_all_zones_number();
        void test_get_all_owners_number();

        void test_get_bus_count();
        void test_get_in_service_bus_count();
        void test_get_overshadowed_bus_count();
        void test_get_generator_count();
        void test_get_wt_generator_count();
        void test_get_pv_unit_count();
        void test_get_source_count();
        void test_get_load_count();
        void test_get_line_count();
        void test_get_transformer_count();
        void test_get_fixed_shunt_count();
        void test_get_hvdc_count();
        void test_get_equivalent_device_count();
        void test_get_energy_storage_count();
        void test_get_area_count();
        void test_get_zone_count();
        void test_get_owner_count();

        void test_get_bus_index();
        void test_get_generator_index();
        void test_get_wt_generator_index();
        void test_get_pv_unit_index();
        void test_get_load_index();
        void test_get_line_index();
        void test_get_transformer_index();
        void test_get_fixed_shunt_index();
        void test_get_hvdc_index();
        void test_get_equivalent_device_index();
        void test_get_energy_storage_index();
        void test_get_area_index();
        void test_get_zone_index();
        void test_get_owner_index();

        void test_bus_name2bus_number();
        void test_area_name2area_number();
        void test_zone_name2zone_number();
        void test_owner_name2owner_number();
        void test_bus_number2bus_name();
        void test_area_number2area_name();
        void test_zone_number2zone_name();
        void test_owner_number2owner_name();

        void test_scale_load_power();
        void test_scale_all_load_power();
        void test_scale_load_power_at_bus();
        void test_scale_load_power_in_area();
        void test_scale_load_power_in_zone();

        void test_scale_source_power();
        void test_scale_all_source_power();
        void test_scale_source_power_at_bus();
        void test_scale_source_power_in_area();
        void test_scale_source_power_in_zone();

        void test_scale_generator_power();
        void test_scale_all_generator_power();
        void test_scale_generator_power_at_bus();
        void test_scale_generator_power_in_area();
        void test_scale_generator_power_in_zone();

        void test_scale_wt_generator_power();
        void test_scale_all_wt_generator_power();
        void test_scale_wt_generator_power_at_bus();
        void test_scale_wt_generator_power_in_area();
        void test_scale_wt_generator_power_in_zone();

        void test_scale_pv_unit_power();
        void test_scale_all_pv_unit_power();
        void test_scale_pv_unit_power_at_bus();
        void test_scale_pv_unit_power_in_area();
        void test_scale_pv_unit_power_in_zone();

        void test_clear();
        void test_clear_bus();
        void test_clear_all_buses();
        void test_clear_generator();
        void test_clear_generators_connecting_to_bus();
        void test_clear_all_generators();
        void test_clear_wt_generator();
        void test_clear_wt_generators_connecting_to_bus();
        void test_clear_all_wt_generators();
        void test_clear_pv_unit();
        void test_clear_pv_units_connecting_to_bus();
        void test_clear_all_pv_units();
        void test_clear_sources_connecting_to_bus();
        void test_clear_all_sources();
        void test_clear_load();
        void test_clear_loads_connecting_to_bus();
        void test_clear_all_loads();
        void test_clear_line();
        void test_clear_lines_connecting_to_bus();
        void test_clear_all_lines();
        void test_clear_transformer();
        void test_clear_transformers_connecting_to_bus();
        void test_clear_all_transformers();
        void test_clear_fixed_shunt();
        void test_clear_fixed_shunts_connecting_to_bus();
        void test_clear_all_fixed_shunts();
        void test_clear_hvdc();
        void test_clear_hvdcs_connecting_to_bus();
        void test_clear_all_hvdcs();
        void test_clear_equivalent_device();
        void test_clear_equivalent_devices_connecting_to_bus();
        void test_clear_all_equivalent_devices();
        void test_clear_energy_storage();
        void test_clear_energy_storages_connecting_to_bus();
        void test_clear_all_energy_storages();
        void test_clear_all_areas();
        void test_clear_all_zones();
        void test_clear_all_owners();

        void test_get_bus_complex_voltage();
        void test_get_bus_base_frequency();
        void test_get_bus_base_voltage();
        void test_get_bus_voltage();
        void test_get_bus_angle();

        void test_get_voltage_to_regulate_of_physical_bus_in_pu();
        void test_get_regulatable_p_max_at_physical_bus_in_MW();
        void test_get_regulatable_p_min_at_physical_bus_in_MW();
        void test_get_regulatable_q_max_at_physical_bus_in_MVar();
        void test_get_regulatable_q_min_at_physical_bus_in_MVar();
        void test_get_total_regulating_p_generation_at_physical_bus_in_MW();
        void test_get_total_regulating_q_generation_at_physical_bus_in_MVar();
        void test_get_total_p_generation_of_sources_at_physical_bus_in_MW();
        void test_get_total_q_generation_of_sources_at_physical_bus_in_MVar();
    private:
        void prepare_database_for_test();
};

#endif//POWER_SYSTEM_DATABASE_TEST_H
