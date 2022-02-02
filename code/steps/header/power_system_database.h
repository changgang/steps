#ifndef POWER_SYSTEM_DATABASE_H
#define POWER_SYSTEM_DATABASE_H

#include "header/device/bus.h"
#include "header/device/generator.h"
#include "header/device/wt_generator.h"
#include "header/device/pv_unit.h"
#include "header/device/energy_storage.h"
#include "header/device/load.h"
#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"
#include "header/device/hvdc.h"
#include "header/device/vsc_hvdc.h"
#include "header/device/equivalent_device.h"
#include "header/device/source.h"
#include "header/device/lcc_hvdc.h"
#include "header/basic/area.h"
#include "header/basic/zone.h"
#include "header/basic/owner.h"

#include "header/basic/bus_index.h"
#include "header/basic/device_index_map.h"

#include <string>

using namespace std;

class MODEL;
class SOURCE;

class POWER_SYSTEM_DATABASE
{
    public:
        POWER_SYSTEM_DATABASE(STEPS& toolkit);
        ~POWER_SYSTEM_DATABASE();

        STEPS& get_toolkit() const;

        void check_powerflow_data();

        void check();
        void clear();

        void set_zero_impedance_threshold_in_pu(double z);
        double get_zero_impedance_threshold_in_pu() const;

        void set_database_capacity();
        unsigned int get_bus_capacity() const;
        unsigned int get_generator_capacity() const;
        unsigned int get_wt_generator_capacity() const;
        unsigned int get_pv_unit_capacity() const;
        unsigned int get_load_capacity() const;
        unsigned int get_fixed_shunt_capacity() const;
        unsigned int get_line_capacity() const;
        unsigned int get_transformer_capacity() const;
        unsigned int get_hvdc_capacity() const;
        unsigned int get_vsc_hvdc_capacity() const;
        unsigned int get_equivalent_device_capacity() const;
        unsigned int get_energy_storage_capacity() const;
        unsigned int get_lcc_hvdc_capacity() const;
        unsigned int get_area_capacity() const;
        unsigned int get_zone_capacity() const;
        unsigned int get_owner_capacity() const;

        void set_bus_capacity(unsigned int n);
        void set_generator_capacity(unsigned int n);
        void set_wt_generator_capacity(unsigned int n);
        void set_pv_unit_capacity(unsigned int n);
        void set_load_capacity(unsigned int n);
        void set_fixed_shunt_capacity(unsigned int n);
        void set_line_capacity(unsigned int n);
        void set_transformer_capacity(unsigned int n);
        void set_hvdc_capacity(unsigned int n);
        void set_vsc_hvdc_capacity(unsigned int n);
        void set_equivalent_device_capacity(unsigned int n);
        void set_energy_storage_capacity(unsigned int n);
        void set_lcc_hvdc_capacity(unsigned int n);
        void set_area_capacity(unsigned int n);
        void set_zone_capacity(unsigned int n);
        void set_owner_capacity(unsigned int n);

        void set_system_name(const string& name);
        string get_system_name() const;

        void set_allowed_max_bus_number(unsigned int max_bus_num);
        unsigned int get_allowed_max_bus_number() const;
        bool is_bus_in_allowed_range(unsigned int bus) const;

        void set_system_base_power_in_MVA(const double s);
        double get_system_base_power_in_MVA() const;
        double get_one_over_system_base_power_in_one_over_MVA() const;

        void set_case_information(const string& title);
        void set_case_additional_information(const string& title);

        string get_case_information() const;
        string get_case_additional_information() const;

        void append_bus(BUS& bus);
        void append_generator(GENERATOR& generator);
        void append_wt_generator(WT_GENERATOR& wt_generator);
        void append_pv_unit(PV_UNIT& pv_unit);
        void append_energy_storage(ENERGY_STORAGE& estorage);
        void append_load(LOAD& load);
        void append_line(LINE& line);
        void append_transformer(TRANSFORMER& transformer);
        void append_fixed_shunt(FIXED_SHUNT& shunt);
        void append_hvdc(HVDC& hvdc);
        void append_vsc_hvdc(VSC_HVDC& vsc_hvdc);
        void append_equivalent_device(EQUIVALENT_DEVICE& edevice);
        void append_lcc_hvdc(LCC_HVDC& hvdc);
        void append_area(AREA& area);
        void append_zone(ZONE& zone);
        void append_owner(OWNER& owner);

        void update_device_id(const DEVICE_ID& did_old, const DEVICE_ID& did_new);

        void update_all_bus_base_frequency(double fbase_Hz);

        bool is_bus_exist(unsigned int bus) const;
        bool is_generator_exist(const DEVICE_ID& device_id) const;
        bool is_wt_generator_exist(const DEVICE_ID& device_id) const;
        bool is_pv_unit_exist(const DEVICE_ID& device_id) const;
        bool is_load_exist(const DEVICE_ID& device_id) const;
        bool is_line_exist(const DEVICE_ID& device_id) const;
        bool is_transformer_exist(const DEVICE_ID& device_id) const;
        bool is_fixed_shunt_exist(const DEVICE_ID& device_id) const;
        bool is_hvdc_exist(const DEVICE_ID& device_id) const;
        bool is_vsc_hvdc_exist(const DEVICE_ID& device_id) const;
        bool is_equivalent_device_exist(const DEVICE_ID& device_id) const;
        bool is_energy_storage_exist(const DEVICE_ID& device_id) const;
        bool is_lcc_hvdc_exist(const DEVICE_ID& device_id) const;
        bool is_area_exist(const unsigned int no) const;
        bool is_zone_exist(const unsigned int no) const;
        bool is_owner_exist(const unsigned int no) const;

        void change_bus_number(unsigned int original_bus_number, unsigned int new_bus_number);
        void change_bus_number_with_file(string file);

        DEVICE* get_device(const DEVICE_ID& device_id);
        NONBUS_DEVICE* get_nonbus_device(const DEVICE_ID& device_id);
        BUS* get_bus(unsigned int bus);
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
        VSC_HVDC* get_vsc_hvdc(const DEVICE_ID & device_id);
        VSC_HVDC* get_vsc_hvdc(string name);
        EQUIVALENT_DEVICE* get_equivalent_device(const DEVICE_ID & device_id);
        ENERGY_STORAGE* get_energy_storage(const DEVICE_ID & device_id);
        LCC_HVDC* get_lcc_hvdc(const DEVICE_ID & device_id);
        AREA* get_area(const unsigned int no);
        ZONE* get_zone(const unsigned int no);
        OWNER* get_owner(const unsigned int no);

        vector<DEVICE*> get_all_devices_connecting_to_bus(const unsigned int bus);
        vector<GENERATOR*> get_generators_connecting_to_bus(const unsigned int bus);
        vector<WT_GENERATOR*> get_wt_generators_connecting_to_bus(const unsigned int bus);
        vector<PV_UNIT*> get_pv_units_connecting_to_bus(const unsigned int bus);
        vector<SOURCE*> get_sources_connecting_to_bus(const unsigned int bus);
        vector<LOAD*> get_loads_connecting_to_bus(const unsigned int bus);
        vector<LINE*> get_lines_connecting_to_bus(const unsigned int bus);
        vector<TRANSFORMER*> get_transformers_connecting_to_bus(const unsigned int bus);
        vector<FIXED_SHUNT*> get_fixed_shunts_connecting_to_bus(const unsigned int bus);
        vector<HVDC*> get_hvdcs_connecting_to_bus(const unsigned int bus);
        vector<VSC_HVDC*> get_vsc_hvdcs_connecting_to_bus(const unsigned int bus);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_connecting_to_bus(const unsigned int bus);
        vector<ENERGY_STORAGE*> get_energy_storages_connecting_to_bus(const unsigned int bus);
        vector<LCC_HVDC*> get_lcc_hvdcs_connecting_to_bus(const unsigned int bus);

        vector<DEVICE_ID> get_all_devices_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_generators_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_wt_generators_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_pv_units_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_sources_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_loads_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_lines_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_transformers_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_fixed_shunts_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_hvdcs_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_vsc_hvdcs_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_equivalent_devices_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_energy_storages_device_id_connecting_to_bus(const unsigned int bus);
        vector<DEVICE_ID> get_lcc_hvdcs_device_id_connecting_to_bus(const unsigned int bus);

        vector<DEVICE*> get_all_devices_in_area(const unsigned int area);
        vector<BUS*> get_buses_in_area(const unsigned int area);
        vector<GENERATOR*> get_generators_in_area(const unsigned int area);
        vector<WT_GENERATOR*> get_wt_generators_in_area(const unsigned int area);
        vector<PV_UNIT*> get_pv_units_in_area(const unsigned int area);
        vector<SOURCE*> get_sources_in_area(const unsigned int area);
        vector<LOAD*> get_loads_in_area(const unsigned int area);
        vector<LINE*> get_lines_in_area(const unsigned int area);
        vector<TRANSFORMER*> get_transformers_in_area(const unsigned int area);
        vector<FIXED_SHUNT*> get_fixed_shunts_in_area(const unsigned int area);
        vector<HVDC*> get_hvdcs_in_area(const unsigned int area);
        vector<VSC_HVDC*> get_vsc_hvdcs_in_area(const unsigned int area);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_in_area(const unsigned int area);
        vector<ENERGY_STORAGE*> get_energy_storages_in_area(const unsigned int area);
        vector<LCC_HVDC*> get_lcc_hvdcs_in_area(const unsigned int area);

        vector<DEVICE_ID> get_all_devices_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_buses_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_generators_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_wt_generators_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_pv_units_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_sources_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_loads_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_lines_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_transformers_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_fixed_shunts_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_hvdcs_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_vsc_hvdcs_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_equivalent_devices_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_energy_storages_device_id_in_area(const unsigned int area);
        vector<DEVICE_ID> get_lcc_hvdcs_device_id_in_area(const unsigned int area);

        vector<DEVICE*> get_all_devices_in_zone(const unsigned int zone);
        vector<BUS*> get_buses_in_zone(const unsigned int zone);
        vector<GENERATOR*> get_generators_in_zone(const unsigned int zone);
        vector<WT_GENERATOR*> get_wt_generators_in_zone(const unsigned int zone);
        vector<PV_UNIT*> get_pv_units_in_zone(const unsigned int zone);
        vector<SOURCE*> get_sources_in_zone(const unsigned int zone);
        vector<LOAD*> get_loads_in_zone(const unsigned int zone);
        vector<LINE*> get_lines_in_zone(const unsigned int zone);
        vector<TRANSFORMER*> get_transformers_in_zone(const unsigned int zone);
        vector<FIXED_SHUNT*> get_fixed_shunts_in_zone(const unsigned int zone);
        vector<HVDC*> get_hvdcs_in_zone(const unsigned int zone);
        vector<VSC_HVDC*> get_vsc_hvdcs_in_zone(const unsigned int zone);
        vector<EQUIVALENT_DEVICE*> get_equivalent_devices_in_zone(const unsigned int zone);
        vector<ENERGY_STORAGE*> get_energy_storages_in_zone(const unsigned int zone);
        vector<LCC_HVDC*> get_lcc_hvdcs_in_zone(const unsigned int zone);

        vector<DEVICE_ID> get_all_devices_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_buses_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_generators_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_wt_generators_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_pv_units_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_sources_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_loads_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_lines_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_transformers_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_fixed_shunts_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_hvdcs_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_vsc_hvdcs_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_equivalent_devices_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_energy_storages_device_id_in_zone(const unsigned int zone);
        vector<DEVICE_ID> get_lcc_hvdcs_device_id_in_zone(const unsigned int zone);

        vector<DEVICE*> get_all_devices();
        vector<BUS*> get_all_buses();
        vector<BUS*> get_buses_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner);
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
        vector<VSC_HVDC*> get_all_vsc_hvdcs();
        vector<EQUIVALENT_DEVICE*> get_all_equivalent_devices();
        vector<ENERGY_STORAGE*> get_all_energy_storages();
        vector<LCC_HVDC*> get_all_lcc_hvdcs();
        vector<AREA*> get_all_areas();
        vector<ZONE*> get_all_zones();
        vector<OWNER*> get_all_owners();

        vector<unsigned int> get_all_buses_number();
        vector<unsigned int> get_buses_number_with_constraints(double vbase_kV_min, double vbase_kV_max, double v_pu_min, double v_pu_max, unsigned int area, unsigned int zone, unsigned int owner);
        vector<unsigned int> get_all_in_service_buses_number();
        vector<DEVICE_ID> get_all_generators_device_id();
        vector<DEVICE_ID> get_all_wt_generators_device_id();
        vector<DEVICE_ID> get_all_pv_units_device_id();
        vector<DEVICE_ID> get_all_sources_device_id();
        vector<DEVICE_ID> get_all_loads_device_id();
        vector<DEVICE_ID> get_all_lines_device_id();
        vector<DEVICE_ID> get_all_transformers_device_id();
        vector<DEVICE_ID> get_all_fixed_shunts_device_id();
        vector<DEVICE_ID> get_all_hvdcs_device_id();
        vector<DEVICE_ID> get_all_vsc_hvdcs_device_id();
        vector<DEVICE_ID> get_all_equivalent_devices_device_id();
        vector<DEVICE_ID> get_all_energy_storages_device_id();
        vector<DEVICE_ID> get_all_lcc_hvdcs_device_id();
        vector<unsigned int> get_all_areas_number();
        vector<unsigned int> get_all_zones_number();
        vector<unsigned int> get_all_owners_number();

        unsigned int get_bus_count() const;
        unsigned int get_in_service_bus_count() const;
        unsigned int get_overshadowed_bus_count() const;
        unsigned int get_generator_count() const;
        unsigned int get_wt_generator_count() const;
        unsigned int get_pv_unit_count() const;
        unsigned int get_source_count() const;
        unsigned int get_load_count() const;
        unsigned int get_line_count() const;
        unsigned int get_transformer_count() const;
        unsigned int get_fixed_shunt_count() const;
        unsigned int get_hvdc_count() const;
        unsigned int get_vsc_hvdc_count() const;
        unsigned int get_switched_shunt_count() const;
        unsigned int get_equivalent_device_count() const;
        unsigned int get_energy_storage_count() const;
        unsigned int get_lcc_hvdc_count() const;
        unsigned int get_area_count() const;
        unsigned int get_zone_count() const;
        unsigned int get_owner_count() const;

        unsigned int get_bus_index(unsigned int bus) const;
        unsigned int get_generator_index(const DEVICE_ID & device_id) const;
        unsigned int get_wt_generator_index(const DEVICE_ID & device_id) const;
        unsigned int get_pv_unit_index(const DEVICE_ID & device_id) const;
        unsigned int get_load_index(const DEVICE_ID & device_id) const;
        unsigned int get_line_index(const DEVICE_ID & device_id) const;
        unsigned int get_transformer_index(const DEVICE_ID & device_id) const;
        unsigned int get_fixed_shunt_index(const DEVICE_ID & device_id) const;
        unsigned int get_hvdc_index(const DEVICE_ID & device_id) const;
        unsigned int get_vsc_hvdc_index(const DEVICE_ID & device_id) const;
        unsigned int get_equivalent_device_index(const DEVICE_ID & device_id) const;
        unsigned int get_energy_storage_index(const DEVICE_ID & device_id) const;
        unsigned int get_lcc_hvdc_index(const DEVICE_ID & device_id) const;
        unsigned int get_area_index(const unsigned int no) const;
        unsigned int get_zone_index(const unsigned int no) const;
        unsigned int get_owner_index(const unsigned int no) const;

        unsigned int bus_name2bus_number(const string& name, double vbase=0) const;
        unsigned int area_name2area_number(const string& name) const;
        unsigned int zone_name2zone_number(const string& name) const;
        unsigned int owner_name2owner_number(const string& name) const;
        string bus_number2bus_name(unsigned int number);
        string area_number2area_name(unsigned int number);
        string zone_number2zone_name(unsigned int number);
        string owner_number2owner_name(unsigned int number);

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
        void check_all_vsc_hvdcs();
        void check_all_equivalent_devices();
        void check_all_energy_storages();
        void check_all_lcc_hvdcs();
        void check_all_areas();
        void check_all_zones();
        void check_all_owners();

        void check_dynamic_data();
        void check_generator_related_dynamic_data();
        void check_wt_generator_related_dynamic_data();
        void check_pv_unit_related_dynamic_data();
        void check_energy_storage_related_dynamic_data();
        void check_load_related_dynamic_data();
        void check_line_related_dynamic_data();
        void check_hvdc_related_dynamic_data();
        void check_vsc_hvdc_related_dynamic_data();
        void check_equivalent_device_related_dynamic_data();
        void check_lcc_hvdc_related_dynamic_data();

        void check_missing_models();
        void check_missing_generator_related_model();
        void check_missing_wt_generator_related_model();
        void check_missing_pv_unit_related_model();
        void check_missing_energy_storage_related_model();
        void check_missing_load_related_model();
        void check_missing_line_related_model();
        void check_missing_hvdc_related_model();
        void check_missing_vsc_hvdc_related_model();
        void check_missing_equivalent_device_related_model();
        void check_missing_lcc_hvdc_related_model();


        void scale_load_power(const DEVICE_ID& did, double scale);
        void scale_all_loads_power(double scale);
        void scale_loads_power_at_bus(unsigned int bus, double scale);
        void scale_loads_power_in_area(unsigned int area_number, double scale);
        void scale_loads_power_in_zone(unsigned int zone_number, double scale);

        void scale_source_power(const DEVICE_ID& did, double scale);
        void scale_all_sources_power(double scale);
        void scale_sources_power_at_bus(unsigned int bus, double scale);
        void scale_sources_power_in_area(unsigned int area_number, double scale);
        void scale_sources_power_in_zone(unsigned int zone_number, double scale);

        void scale_generator_power(const DEVICE_ID& did, double scale);
        void scale_all_generators_power(double scale);
        void scale_generators_power_at_bus(unsigned int bus, double scale);
        void scale_generators_power_in_area(unsigned int area_number, double scale);
        void scale_generators_power_in_zone(unsigned int zone_number, double scale);

        void scale_wt_generator_power(const DEVICE_ID& did, double scale);
        void scale_all_wt_generators_power(double scale);
        void scale_wt_generators_power_at_bus(unsigned int bus, double scale);
        void scale_wt_generators_power_in_area(unsigned int area_number, double scale);
        void scale_wt_generators_power_in_zone(unsigned int zone_number, double scale);

        void scale_pv_unit_power(const DEVICE_ID& did, double scale);
        void scale_all_pv_units_power(double scale);
        void scale_pv_units_power_at_bus(unsigned int bus, double scale);
        void scale_pv_units_power_in_area(unsigned int area_number, double scale);
        void scale_pv_units_power_in_zone(unsigned int zone_number, double scale);

        void scale_energy_storage_power(const DEVICE_ID& did, double scale);
        void scale_all_energy_storages_power(double scale);
        void scale_energy_storages_power_at_bus(unsigned int bus, double scale);
        void scale_energy_storages_power_in_area(unsigned int area_number, double scale);
        void scale_energy_storages_power_in_zone(unsigned int zone_number, double scale);

        void clear_bus(unsigned int bus);
        void clear_all_buses();

        void clear_generator(const DEVICE_ID& device_id);
        void clear_generators_connecting_to_bus(const unsigned int bus);
        void clear_all_generators();

        void clear_wt_generator(const DEVICE_ID& device_id);
        void clear_wt_generators_connecting_to_bus(const unsigned int bus);
        void clear_all_wt_generators();

        void clear_pv_unit(const DEVICE_ID& device_id);
        void clear_pv_units_connecting_to_bus(const unsigned int bus);
        void clear_all_pv_units();

        void clear_sources_connecting_to_bus(const unsigned int bus);
        void clear_all_sources();

        void clear_load(const DEVICE_ID& device_id);
        void clear_loads_connecting_to_bus(const unsigned int bus);
        void clear_all_loads();

        void clear_line(const DEVICE_ID& device_id);
        void clear_lines_connecting_to_bus(const unsigned int bus);
        void clear_all_lines();

        void clear_transformer(const DEVICE_ID& device_id);
        void clear_transformers_connecting_to_bus(const unsigned int bus);
        void clear_all_transformers();

        void clear_fixed_shunt(const DEVICE_ID& device_id);
        void clear_fixed_shunts_connecting_to_bus(const unsigned int bus);
        void clear_all_fixed_shunts();

        void clear_hvdc(const DEVICE_ID& device_id);
        void clear_hvdcs_connecting_to_bus(const unsigned int bus);
        void clear_all_hvdcs();

        void clear_vsc_hvdc(const DEVICE_ID& device_id);
        void clear_vsc_hvdcs_connecting_to_bus(const unsigned int bus);
        void clear_all_vsc_hvdcs();

        void clear_equivalent_device(const DEVICE_ID& device_id);
        void clear_equivalent_devices_connecting_to_bus(const unsigned int bus);
        void clear_all_equivalent_devices();

        void clear_energy_storage(const DEVICE_ID& device_id);
        void clear_energy_storages_connecting_to_bus(const unsigned int bus);
        void clear_all_energy_storages();

        void clear_lcc_hvdc(const DEVICE_ID& device_id);
        void clear_lcc_hvdcs_connecting_to_bus(const unsigned int bus);
        void clear_all_lcc_hvdcs();

        void clear_area(unsigned int area);
        void clear_all_areas();

        void clear_zone(unsigned int zone);
        void clear_all_zones();

        void clear_owner(unsigned int owner);
        void clear_all_owners();

        void trip_bus(unsigned int bus);

        void check_device_status_for_out_of_service_buses();
        void calibrate_sources_pmax_and_pmin_with_dynamic_model();
        void calibrate_generators_pmax_and_pmin_with_turbine_governor_model();

        BUS_TYPE get_bus_type(unsigned int bus);
        double get_bus_base_frequency_in_Hz(unsigned int bus);
        double get_bus_base_voltage_in_kV(unsigned int bus);
        double get_bus_positive_sequence_voltage_in_pu(unsigned int bus);
        double get_bus_positive_sequence_voltage_in_kV(unsigned int bus);
        double get_bus_positive_sequence_angle_in_deg(unsigned int bus);
        double get_bus_positive_sequence_angle_in_rad(unsigned int bus);
        complex<double> get_bus_positive_sequence_complex_voltage_in_pu(unsigned int bus);
        complex<double> get_bus_positive_sequence_complex_voltage_in_kV(unsigned int bus);
        double get_bus_frequency_deviation_in_pu(unsigned int bus);
        double get_bus_frequency_deviation_in_Hz(unsigned int bus);
        double get_bus_frequency_in_Hz(unsigned int bus);
        double get_bus_frequency_in_pu(unsigned int bus);

        double get_voltage_to_regulate_of_physical_bus_in_pu(unsigned int bus);
        double get_regulatable_p_max_at_physical_bus_in_MW(unsigned int bus);
        double get_regulatable_p_min_at_physical_bus_in_MW(unsigned int bus);
        double get_regulatable_q_max_at_physical_bus_in_MVar(unsigned int bus);
        double get_regulatable_q_min_at_physical_bus_in_MVar(unsigned int bus);
        double get_total_regulating_p_generation_at_physical_bus_in_MW(unsigned int bus);
        double get_total_regulating_q_generation_at_physical_bus_in_MVar(unsigned int bus);
        double get_total_p_generation_at_physical_bus_in_MW(unsigned int bus);
        double get_total_q_generation_at_physical_bus_in_MVar(unsigned int bus);

        complex<double> get_total_load_power_in_MVA();
        complex<double> get_total_generation_power_in_MVA();
        complex<double> get_total_loss_power_in_MVA();


        void update_in_service_bus_count();
        void set_all_buses_un_overshadowed();
        void update_overshadowed_bus_count();
        unsigned int get_equivalent_bus_of_bus(unsigned int bus);

        unsigned int get_memory_usage_in_bytes();
    private:
        void check_source_status_for_out_of_service_bus(unsigned int bus);
        void check_load_status_for_out_of_service_bus(unsigned int bus);
        void check_fixed_shunt_status_for_out_of_service_bus(unsigned int bus);
        void check_line_status_for_out_of_service_bus(unsigned int bus);
        void check_transformer_status_for_out_of_service_bus(unsigned int bus);
        void check_hvdc_status_for_out_of_service_bus(unsigned int bus);
        void check_vsc_hvdc_status_for_out_of_service_bus(unsigned int bus);
        void check_lcc_hvdc_status_for_out_of_service_bus(unsigned int bus);

        STEPS* toolkit;

        string system_name;
        double system_base_power_in_MVA, one_over_system_base_power;

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
        vector<VSC_HVDC> Vsc_hvdc;
        vector<EQUIVALENT_DEVICE> Equivalent_device;
        vector<ENERGY_STORAGE> Energy_storage;
        vector<LCC_HVDC> Lcc_hvdc;
        vector<AREA> Area;
        vector<ZONE> Zone;
        vector<OWNER> Owner;

        double zero_impedance_threshold;

        unsigned int in_service_bus_count;
        unsigned int overshadowed_bus_count;

        BUS_INDEX bus_index;
        DEVICE_INDEX_MAP generator_index, wt_generator_index, pv_unit_index, load_index, fixed_shunt_index, switched_shunt_index,
                            line_index, transformer_index, hvdc_index, vsc_hvdc_index, equivalent_device_index, energy_storage_index,
                            lcc_hvdc_index;
        map<unsigned int,  unsigned int> area_index, zone_index, owner_index;
};
#endif // POWER_SYSTEM_DATABASE_H
