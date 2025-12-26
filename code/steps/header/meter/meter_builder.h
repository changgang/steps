#ifndef METER_BUILDER_H
#define METER_BUILDER_H

#include "header/meter/meter.h"

class STEPS;

class METER_BUILDER
{
    public:
        METER_BUILDER(STEPS& toolkit);
        STEPS& get_toolkit() const;
        ~METER_BUILDER();

        vector<METER> build_all_meters();
        vector<METER> build_all_buses_complete_meters();
        vector<METER> build_all_generators_complete_meters();
        vector<METER> build_all_wt_generators_complete_meters();
        vector<METER> build_all_pv_units_complete_meters();
        vector<METER> build_all_energy_storages_complete_meters();
        vector<METER> build_all_sv_sources_complete_meters();
        vector<METER> build_all_loads_complete_meters();
        vector<METER> build_all_ac_lines_complete_meters();
        vector<METER> build_all_transformers_complete_meters();
        vector<METER> build_all_lcc_hvdc2ts_complete_meters();
        vector<METER> build_all_vsc_hvdcs_complete_meters();
        vector<METER> build_all_mt_hvdcs_complete_meters();
        vector<METER> build_all_equivalent_devices_complete_meters();

        vector<METER> build_all_common_meters();
        vector<METER> build_all_buses_common_meters();
        vector<METER> build_all_generators_common_meters();
        vector<METER> build_all_wt_generators_common_meters();
        vector<METER> build_all_pv_units_common_meters();
        vector<METER> build_all_energy_storages_common_meters();
        vector<METER> build_all_sv_sources_common_meters();
        vector<METER> build_all_loads_common_meters();
        vector<METER> build_all_ac_lines_common_meters();
        vector<METER> build_all_transformers_common_meters();
        vector<METER> build_all_lcc_hvdc2ts_common_meters();
        vector<METER> build_all_vsc_hvdcs_common_meters();
        vector<METER> build_all_mt_hvdcs_common_meters();
        vector<METER> build_all_equivalent_devices_common_meters();

        vector<METER> build_one_bus_complete_meters(unsigned int bus);
        vector<METER> build_one_generator_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_wt_generator_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_pv_unit_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_energy_storage_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_sv_source_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_load_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_ac_line_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_transformer_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_lcc_hvdc2t_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_vsc_hvdc_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_mt_hvdc_complete_meters(const DEVICE_ID& did);
        vector<METER> build_one_equivalent_device_complete_meters(const DEVICE_ID& did);

        METER build_one_bus_related_meter(unsigned int bus, string meter_type);
        METER build_one_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_sv_source_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        METER build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        METER build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name="");
        METER build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        METER build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name="");
        METER build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        METER build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name="");
        METER build_one_vsc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name="");
        METER build_one_vsc_hvdc_related_meter(string vsc_name, string meter_type, unsigned int side_bus, string var_name="");
        METER build_one_vsc_hvdc_related_dc_line_meter(const DEVICE_ID& did, string meter_type, const DC_DEVICE_ID dc_did, unsigned int side_bus, string var_name="");
        METER build_one_mt_hvdc_related_meter(const DEVICE_ID& did, string sub_device_name, string meter_type, string var_name="");
        METER build_one_mt_hvdc_related_meter(string mtdc_name, string sub_device_name, string meter_type, string var_name="");
        METER build_one_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
    private:
        METER build_one_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus = 0, string var_name="");
        METER build_one_meter_with_sub_device_name(const DEVICE_ID& did, string sub_device_name, string meter_type, unsigned int side_bus = 0, string var_name="");
        METER build_one_meter_with_sub_dc_device_id(const DEVICE_ID& did, DC_DEVICE_ID& sub_dc_did, string meter_type, unsigned int side_bus = 0, string var_name="");

        vector<METER> build_meters_with_multi_devices_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);
        vector<METER> build_meters_with_multi_ac_lines_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);
        vector<METER> build_meters_with_multi_transformers_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);
        vector<METER> build_meters_with_multi_lcc_hvdc2t_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);
        vector<METER> build_meters_with_multi_vsc_hvdcs_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);
        vector<METER> build_meters_with_multi_mt_hvdcs_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys);

        vector<string> get_sorted_keys_from_meter_map(unordered_map<string, function<double(METER_PARAMETER)>>& meter_map);
    private:
        STEPS* toolkit;
};
#endif // METER_BUILDER_H
