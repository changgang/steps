#ifndef POWER_SYSTEM_DATABASE_H
#define POWER_SYSTEM_DATABASE_H

#include "header/device/bus.h"
#include "header/device/generator.h"
#include "header/device/wt_generator.h"
#include "header/device/pv_unit.h"
#include "header/device/load.h"
#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"
#include "header/device/hvdc.h"
#include "header/device/equivalent_device.h"
#include "header/device/energy_storage.h"
#include "header/basic/area.h"
#include "header/basic/zone.h"
#include "header/basic/owner.h"

#include "header/basic/bus_index.h"
#include "header/basic/device_index_map.h"

#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"

#include <string>

using namespace std;

class DYNAMICS_SIMULATOR;

class POWER_SYSTEM_DATABASE
{
    public:
        POWER_SYSTEM_DATABASE();
        ~POWER_SYSTEM_DATABASE();
        void clear_database();
        void set_zero_impedance_threshold_in_pu(double z);
        double get_zero_impedance_threshold_in_pu() const;

        size_t get_bus_capacity() const;
        size_t get_generator_capacity() const;
        size_t get_wt_generator_capacity() const;
        size_t get_pv_unit_capacity() const;
        size_t get_load_capacity() const;
        size_t get_fixed_shunt_capacity() const;
        size_t get_line_capacity() const;
        size_t get_transformer_capacity() const;
        size_t get_hvdc_capacity() const;
        size_t get_equivalent_device_capacity() const;
        size_t get_energy_storage_capacity() const;
        size_t get_area_capacity() const;
        size_t get_zone_capacity() const;
        size_t get_owner_capacity() const;

        void set_bus_capacity(size_t n);
        void set_generator_capacity(size_t n);
        void set_wt_generator_capacity(size_t n);
        void set_pv_unit_capacity(size_t n);
        void set_load_capacity(size_t n);
        void set_fixed_shunt_capacity(size_t n);
        void set_line_capacity(size_t n);
        void set_transformer_capacity(size_t n);
        void set_hvdc_capacity(size_t n);
        void set_equivalent_device_capacity(size_t n);
        void set_energy_storage_capacity(size_t n);
        void set_area_capacity(size_t n);
        void set_zone_capacity(size_t n);
        void set_owner_capacity(size_t n);

        void set_system_name(const string& name);
        string get_system_name() const;

        void set_allowed_max_bus_number(size_t max_bus_num);
        size_t get_allowed_max_bus_number() const;
        bool is_bus_in_allowed_range(size_t bus) const;

        void set_system_base_power_in_MVA(const double s);
        double get_system_base_power_in_MVA() const;

        void set_case_information(const string& title);
        void set_case_additional_information(const string& title);

        string get_case_information() const;
        string get_case_additional_information() const;

        void append_bus(const BUS& bus);
        void append_generator(const GENERATOR& generator);
        void append_wt_generator(const WT_GENERATOR& wt_generator);
        void append_pv_unit(const PV_UNIT& pv_unit);
        void append_energy_storage(const ENERGY_STORAGE& estorage);
        void append_load(const LOAD& load);
        void append_line(const LINE& line);
        void append_transformer(const TRANSFORMER& transformer);
        void append_fixed_shunt(const FIXED_SHUNT& shunt);
        void append_hvdc(const HVDC& hvdc);
        void append_equivalent_device(const EQUIVALENT_DEVICE& edevice);
        void append_area(const AREA& area);
        void append_zone(const ZONE& zone);
        void append_owner(const OWNER& owner);

        void append_dynamic_model(const DEVICE_ID& did, const MODEL* model);

        void update_all_bus_base_frequency(double fbase_Hz);

        bool is_bus_exist(size_t bus) const;
        bool is_generator_exist(const DEVICE_ID& device_id) const;
        bool is_wt_generator_exist(const DEVICE_ID& device_id) const;
        bool is_pv_unit_exist(const DEVICE_ID& device_id) const;
        bool is_load_exist(const DEVICE_ID& device_id) const;
        bool is_line_exist(const DEVICE_ID& device_id) const;
        bool is_transformer_exist(const DEVICE_ID& device_id) const;
        bool is_fixed_shunt_exist(const DEVICE_ID& device_id) const;
        bool is_hvdc_exist(const DEVICE_ID& device_id) const;
        bool is_equivalent_device_exist(const DEVICE_ID& device_id) const;
        bool is_energy_storage_exist(const DEVICE_ID& device_id) const;
        bool is_area_exist(const size_t no) const;
        bool is_zone_exist(const size_t no) const;
        bool is_owner_exist(const size_t no) const;

        void change_bus_number(size_t original_bus_number, size_t new_bus_number);

        DEVICE* get_device(const DEVICE_ID& device_id);
        BUS* get_bus(size_t bus);
        BUS* get_bus(const DEVICE_ID & device_id);
        GENERATOR* get_generator(const DEVICE_ID & device_id);
        WT_GENERATOR* get_wt_generator(const DEVICE_ID & device_id);
        PV_UNIT* get_pv_unit(const DEVICE_ID & device_id);
        SOURCE* get_source(const DEVICE_ID & device_id);
        LOAD* get_load(const DEVICE_ID & device_id);
        LINE* get_line(const DEVICE_ID & device_id);
        TRANSFORMER* get_transformer(const DEVICE_ID & device_id);
        FIXED_SHUNT* get_fixed_shunt(const DEVICE_ID & device_id);
        HVDC* get_hvdc(const DEVICE_ID & device_id);
        EQUIVALENT_DEVICE* get_equivalent_device(const DEVICE_ID & device_id);
        ENERGY_STORAGE* get_energy_storage(const DEVICE_ID & device_id);
        AREA* get_area(const size_t no);
        ZONE* get_zone(const size_t no);
        OWNER* get_owner(const size_t no);

        vector<DEVICE*> get_all_devices_connecting_to_bus(const size_t bus);
        vector<GENERATOR*> get_generators_connecting_to_bus(const size_t bus);
        vector<WT_GENERATOR*> get_wt_generators_connecting_to_bus(const size_t bus);
        vector<PV_UNIT*> get_pv_units_connecting_to_bus(const size_t bus);
        vector<SOURCE*> get_sources_connecting_to_bus(const size_t bus);
        vector<LOAD*> get_loads_connecting_to_bus(const size_t bus);
        vector<LINE*> get_lines_connecting_to_bus(const size_t bus);
        vector<TRANSFORMER*> get_transformers_connecting_to_bus(const size_t bus);
        vector<FIXED_SHUNT*> get_fixed_shunts_connecting_to_bus(const size_t bus);
        vector<HVDC*> get_hvdcs_connecting_to_bus(const size_t bus);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_connecting_to_bus(const size_t bus);
        vector<ENERGY_STORAGE*> get_energy_storages_connecting_to_bus(const size_t bus);

        vector<DEVICE_ID> get_all_devices_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_generators_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_wt_generators_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_pv_units_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_sources_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_loads_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_lines_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_transformers_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_fixed_shunts_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_hvdcs_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_equivalent_devices_device_id_connecting_to_bus(const size_t bus);
        vector<DEVICE_ID> get_energy_storages_device_id_connecting_to_bus(const size_t bus);

        vector<DEVICE*> get_all_devices_in_area(const size_t area);
        vector<BUS*> get_buses_in_area(const size_t area);
        vector<GENERATOR*> get_generators_in_area(const size_t area);
        vector<WT_GENERATOR*> get_wt_generators_in_area(const size_t area);
        vector<PV_UNIT*> get_pv_units_in_area(const size_t area);
        vector<SOURCE*> get_sources_in_area(const size_t area);
        vector<LOAD*> get_loads_in_area(const size_t area);
        vector<LINE*> get_lines_in_area(const size_t area);
        vector<TRANSFORMER*> get_transformers_in_area(const size_t area);
        vector<FIXED_SHUNT*> get_fixed_shunts_in_area(const size_t area);
        vector<HVDC*> get_hvdcs_in_area(const size_t area);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_in_area(const size_t area);
        vector<ENERGY_STORAGE*> get_energy_storages_in_area(const size_t area);

        vector<DEVICE_ID> get_all_devices_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_buses_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_generators_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_wt_generators_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_pv_units_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_sources_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_loads_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_lines_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_transformers_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_fixed_shunts_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_hvdcs_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_equivalent_devices_device_id_in_area(const size_t area);
        vector<DEVICE_ID> get_energy_storages_device_id_in_area(const size_t area);

        vector<DEVICE*> get_all_devices_in_zone(const size_t zone);
        vector<BUS*> get_buses_in_zone(const size_t zone);
        vector<GENERATOR*> get_generators_in_zone(const size_t zone);
        vector<WT_GENERATOR*> get_wt_generators_in_zone(const size_t zone);
        vector<PV_UNIT*> get_pv_units_in_zone(const size_t zone);
        vector<SOURCE*> get_sources_in_zone(const size_t zone);
        vector<LOAD*> get_loads_in_zone(const size_t zone);
        vector<LINE*> get_lines_in_zone(const size_t zone);
        vector<TRANSFORMER*> get_transformers_in_zone(const size_t zone);
        vector<FIXED_SHUNT*> get_fixed_shunts_in_zone(const size_t zone);
        vector<HVDC*> get_hvdcs_in_zone(const size_t zone);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_in_zone(const size_t zone);
        vector<ENERGY_STORAGE*> get_energy_storages_in_zone(const size_t zone);

        vector<DEVICE_ID> get_all_devices_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_buses_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_generators_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_wt_generators_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_pv_units_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_sources_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_loads_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_lines_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_transformers_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_fixed_shunts_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_hvdcs_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_equivalent_devices_device_id_in_zone(const size_t zone);
        vector<DEVICE_ID> get_energy_storages_device_id_in_zone(const size_t zone);

        vector<DEVICE*> get_all_devices();
        vector<BUS*> get_all_buses();
        vector<BUS*> get_buses_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner);
        vector<BUS*> get_all_in_service_buses();
        vector<GENERATOR*> get_all_generators();
        vector<WT_GENERATOR*> get_all_wt_generators();
        vector<PV_UNIT*> get_all_pv_units();
        vector<SOURCE*> get_all_sources();
        vector<LOAD*> get_all_loads();
        vector<LINE*> get_all_lines();
        vector<TRANSFORMER*> get_all_transformers();
        vector<FIXED_SHUNT*> get_all_fixed_shunts();
        vector<HVDC*> get_all_hvdcs();
        vector<EQUIVALENT_DEVICE*> get_all_equivalent_devices();
        vector<ENERGY_STORAGE*> get_all_energy_storages();
        vector<AREA*> get_all_areas();
        vector<ZONE*> get_all_zones();
        vector<OWNER*> get_all_owners();

        vector<size_t> get_all_buses_number();
        vector<size_t> get_buses_number_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, size_t area, size_t zone, size_t owner);
        vector<size_t> get_all_in_service_buses_number();
        vector<DEVICE_ID> get_all_generators_device_id();
        vector<DEVICE_ID> get_all_wt_generators_device_id();
        vector<DEVICE_ID> get_all_pv_units_device_id();
        vector<DEVICE_ID> get_all_sources_device_id();
        vector<DEVICE_ID> get_all_loads_device_id();
        vector<DEVICE_ID> get_all_lines_device_id();
        vector<DEVICE_ID> get_all_transformers_device_id();
        vector<DEVICE_ID> get_all_fixed_shunts_device_id();
        vector<DEVICE_ID> get_all_hvdcs_device_id();
        vector<DEVICE_ID> get_all_equivalent_devices_device_id();
        vector<DEVICE_ID> get_all_energy_storages_device_id();
        vector<size_t> get_all_areas_number();
        vector<size_t> get_all_zones_number();
        vector<size_t> get_all_owners_number();

        size_t get_bus_count() const;
        size_t get_in_service_bus_count() const;
        size_t get_overshadowed_bus_count() const;
        size_t get_generator_count() const;
        size_t get_wt_generator_count() const;
        size_t get_pv_unit_count() const;
        size_t get_source_count() const;
        size_t get_load_count() const;
        size_t get_line_count() const;
        size_t get_transformer_count() const;
        size_t get_fixed_shunt_count() const;
        size_t get_hvdc_count() const;
        size_t get_switched_shunt_count() const;
        size_t get_equivalent_device_count() const;
        size_t get_energy_storage_count() const;
        size_t get_area_count() const;
        size_t get_zone_count() const;
        size_t get_owner_count() const;

        size_t get_bus_index(size_t bus) const;
        size_t get_generator_index(const DEVICE_ID & device_id) const;
        size_t get_wt_generator_index(const DEVICE_ID & device_id) const;
        size_t get_pv_unit_index(const DEVICE_ID & device_id) const;
        size_t get_load_index(const DEVICE_ID & device_id) const;
        size_t get_line_index(const DEVICE_ID & device_id) const;
        size_t get_transformer_index(const DEVICE_ID & device_id) const;
        size_t get_fixed_shunt_index(const DEVICE_ID & device_id) const;
        size_t get_hvdc_index(const DEVICE_ID & device_id) const;
        size_t get_equivalent_device_index(const DEVICE_ID & device_id) const;
        size_t get_energy_storage_index(const DEVICE_ID & device_id) const;
        size_t get_area_index(const size_t no) const;
        size_t get_zone_index(const size_t no) const;
        size_t get_owner_index(const size_t no) const;

        size_t bus_name2bus_number(const string& name) const;
        size_t area_name2area_number(const string& name) const;
        size_t zone_name2zone_number(const string& name) const;
        size_t owner_name2owner_number(const string& name) const;
        string bus_number2bus_name(size_t number);
        string area_number2area_name(size_t number);
        string zone_number2zone_name(size_t number);
        string owner_number2owner_name(size_t number);

        void check_database();
        void check_all_devices();
        void check_all_buses();
        void check_all_sources();
        void check_all_generators();
        void check_all_wt_generators();
        void check_all_pv_units();
        void check_all_loads();
        void check_all_fixed_shunts();
        void check_all_lines();
        void check_all_transformers();
        void check_all_hvdcs();
        void check_all_equivalent_devices();
        void check_all_energy_storages();
        void check_all_areas();
        void check_all_zones();
        void check_all_owners();

        void check_dynamic_data();
        void check_generator_related_dynamic_data();
        void check_wt_generator_related_dynamic_data();
        void check_pv_unit_related_dynamic_data();
        void check_load_related_dynamic_data();
        void check_hvdc_related_dynamic_data();
        void check_energy_storage_related_dynamic_data();
        void check_equivalent_device_related_dynamic_data();


        void scale_load_power(const DEVICE_ID& did, double scale);
        void scale_all_loads_power(double scale);
        void scale_loads_power_at_bus(size_t bus, double scale);
        void scale_loads_power_in_area(size_t area_number, double scale);
        void scale_loads_power_in_zone(size_t zone_number, double scale);

        void scale_source_power(const DEVICE_ID& did, double scale);
        void scale_all_sources_power(double scale);
        void scale_sources_power_at_bus(size_t bus, double scale);
        void scale_sources_power_in_area(size_t area_number, double scale);
        void scale_sources_power_in_zone(size_t zone_number, double scale);

        void scale_generator_power(const DEVICE_ID& did, double scale);
        void scale_all_generators_power(double scale);
        void scale_generators_power_at_bus(size_t bus, double scale);
        void scale_generators_power_in_area(size_t area_number, double scale);
        void scale_generators_power_in_zone(size_t zone_number, double scale);

        void scale_wt_generator_power(const DEVICE_ID& did, double scale);
        void scale_all_wt_generators_power(double scale);
        void scale_wt_generators_power_at_bus(size_t bus, double scale);
        void scale_wt_generators_power_in_area(size_t area_number, double scale);
        void scale_wt_generators_power_in_zone(size_t zone_number, double scale);

        void scale_pv_unit_power(const DEVICE_ID& did, double scale);
        void scale_all_pv_units_power(double scale);
        void scale_pv_units_power_at_bus(size_t bus, double scale);
        void scale_pv_units_power_in_area(size_t area_number, double scale);
        void scale_pv_units_power_in_zone(size_t zone_number, double scale);

        void scale_energy_storage_power(const DEVICE_ID& did, double scale);
        void scale_all_energy_storages_power(double scale);
        void scale_energy_storages_power_at_bus(size_t bus, double scale);
        void scale_energy_storages_power_in_area(size_t area_number, double scale);
        void scale_energy_storages_power_in_zone(size_t zone_number, double scale);

        void clear_bus(size_t bus);
        void clear_all_buses();

        void clear_generator(const DEVICE_ID& device_id);
        void clear_generators_connecting_to_bus(const size_t bus);
        void clear_all_generators();

        void clear_wt_generator(const DEVICE_ID& device_id);
        void clear_wt_generators_connecting_to_bus(const size_t bus);
        void clear_all_wt_generators();

        void clear_pv_unit(const DEVICE_ID& device_id);
        void clear_pv_units_connecting_to_bus(const size_t bus);
        void clear_all_pv_units();

        void clear_sources_connecting_to_bus(const size_t bus);
        void clear_all_sources();

        void clear_load(const DEVICE_ID& device_id);
        void clear_loads_connecting_to_bus(const size_t bus);
        void clear_all_loads();

        void clear_line(const DEVICE_ID& device_id);
        void clear_lines_connecting_to_bus(const size_t bus);
        void clear_all_lines();

        void clear_transformer(const DEVICE_ID& device_id);
        void clear_transformers_connecting_to_bus(const size_t bus);
        void clear_all_transformers();

        void clear_fixed_shunt(const DEVICE_ID& device_id);
        void clear_fixed_shunts_connecting_to_bus(const size_t bus);
        void clear_all_fixed_shunts();

        void clear_hvdc(const DEVICE_ID& device_id);
        void clear_hvdcs_connecting_to_bus(const size_t bus);
        void clear_all_hvdcs();

        void clear_equivalent_device(const DEVICE_ID& device_id);
        void clear_equivalent_devices_connecting_to_bus(const size_t bus);
        void clear_all_equivalent_devices();

        void clear_energy_storage(const DEVICE_ID& device_id);
        void clear_energy_storages_connecting_to_bus(const size_t bus);
        void clear_all_energy_storages();

        void clear_area(size_t area);
        void clear_all_areas();

        void clear_zone(size_t zone);
        void clear_all_zones();

        void clear_owner(size_t owner);
        void clear_all_owners();

        void trip_bus(size_t bus);

        void check_device_status_for_out_of_service_buses();
        void calibrate_sources_pmax_and_pmin_with_dynamic_model();
        void calibrate_generators_pmax_and_pmin_with_turbine_governor_model();

        complex<double> get_bus_complex_voltage_in_pu(size_t bus);
        complex<double> get_bus_complex_voltage_in_kV(size_t bus);
        double get_bus_base_frequency_in_Hz(size_t bus);
        double get_bus_base_voltage_in_kV(size_t bus);
        double get_bus_voltage_in_pu(size_t bus);
        double get_bus_voltage_in_kV(size_t bus);
        double get_bus_angle_in_deg(size_t bus);
        double get_bus_angle_in_rad(size_t bus);
        double get_bus_frequency_deviation_in_pu(size_t bus);
        double get_bus_frequency_deviation_in_Hz(size_t bus);
        double get_bus_frequency_in_Hz(size_t bus);
        double get_bus_frequency_in_pu(size_t bus);

        double get_voltage_to_regulate_of_physical_bus_in_pu(size_t bus);
        double get_regulatable_p_max_at_physical_bus_in_MW(size_t bus);
        double get_regulatable_p_min_at_physical_bus_in_MW(size_t bus);
        double get_regulatable_q_max_at_physical_bus_in_MVar(size_t bus);
        double get_regulatable_q_min_at_physical_bus_in_MVar(size_t bus);
        double get_total_regulating_p_generation_at_physical_bus_in_MW(size_t bus);
        double get_total_regulating_q_generation_at_physical_bus_in_MVar(size_t bus);
        double get_total_p_generation_at_physical_bus_in_MW(size_t bus);
        double get_total_q_generation_at_physical_bus_in_MVar(size_t bus);

        complex<double> get_total_load_power_in_MVA();
        complex<double> get_total_generation_power_in_MVA();
        complex<double> get_total_loss_power_in_MVA();


        void update_in_service_bus_count();
        void update_overshadowed_bus_count();
        size_t get_equivalent_bus_of_bus(size_t bus);
    private:
        void set_database_capacity();

        void append_load_related_model(const DEVICE_ID& did, const MODEL* model);
        void append_generator_related_model(const DEVICE_ID& did, const MODEL* model);
        void append_wt_generator_related_model(const DEVICE_ID& did, const MODEL* model);
        void append_pv_unit_related_model(const DEVICE_ID& did, const MODEL* model);
        void append_energy_storage_related_model(const DEVICE_ID& did, const MODEL* model);
        void append_hvdc_related_model(const DEVICE_ID& did, const MODEL* model);

        void check_source_status_for_out_of_service_bus(size_t bus);
        void check_load_status_for_out_of_service_bus(size_t bus);
        void check_fixed_shunt_status_for_out_of_service_bus(size_t bus);
        void check_line_status_for_out_of_service_bus(size_t bus);
        void check_transformer_status_for_out_of_service_bus(size_t bus);
        void check_hvdc_status_for_out_of_service_bus(size_t bus);
        void check_energy_storage_status_for_out_of_service_bus(size_t bus);

        string system_name;
        double system_base_power_in_MVA;

        string case_information, case_additional_information;

        vector<BUS> Bus;
        vector<GENERATOR> Generator;
        vector<WT_GENERATOR> WT_Generator;
        vector<PV_UNIT> PV_Unit;
        vector<LOAD> Load;
        vector<LINE> Line;
        vector<TRANSFORMER> Transformer;
        vector<FIXED_SHUNT> Fixed_shunt;
        vector<HVDC> Hvdc;
        vector<EQUIVALENT_DEVICE> Equivalent_device;
        vector<ENERGY_STORAGE> Energy_storage;
        vector<AREA> Area;
        vector<ZONE> Zone;
        vector<OWNER> Owner;

        double zero_impedance_threshold;

        size_t in_service_bus_count;
        size_t overshadowed_bus_count;

        BUS_INDEX bus_index;
        DEVICE_INDEX_MAP generator_index, wt_generator_index, pv_unit_index, load_index, fixed_shunt_index, switched_shunt_index,
                            line_index, transformer_index, hvdc_index, equivalent_device_index, energy_storage_index;
        map<size_t,  size_t> area_index, zone_index, owner_index;
};
#endif // POWER_SYSTEM_DATABASE_H
