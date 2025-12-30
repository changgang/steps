#include "header/meter/meter_builder.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <algorithm>
using namespace std;

METER_BUILDER::METER_BUILDER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

METER_BUILDER::~METER_BUILDER()
{
    ;
}

STEPS& METER_BUILDER::get_toolkit() const
{
    return *toolkit;
}

vector<METER> METER_BUILDER::build_all_meters()
{
    vector<METER> all_meters;
    vector<METER> meters;

    meters = build_all_buses_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_generators_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_wt_generators_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_pv_units_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_energy_storages_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_sv_sources_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_loads_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_ac_lines_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_transformers_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_lcc_hvdc2ts_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_vsc_hvdcs_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_mt_hvdcs_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_equivalent_devices_complete_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    return all_meters;
}

vector<METER> METER_BUILDER::build_all_buses_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_buses_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(bus_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_generators_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_generators_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(sg_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_wt_generators_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_wt_generators_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(wtg_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_pv_units_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_pv_units_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(pvu_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_energy_storages_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_energy_storages_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(es_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_sv_sources_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_sv_sources_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(sv_source_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_loads_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_loads_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(load_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_ac_lines_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_ac_lines_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(ac_line_meter_map);
    return build_meters_with_multi_ac_lines_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_transformers_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_transformers_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(transformer_meter_map);
    return build_meters_with_multi_transformers_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_lcc_hvdc2ts_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_lcc_hvdc2ts_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(lcc_hvdc2t_meter_map);
    return build_meters_with_multi_lcc_hvdc2t_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_vsc_hvdcs_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_vsc_hvdcs_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(vsc_hvdc_meter_map);
    return build_meters_with_multi_vsc_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_mt_hvdcs_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_mt_hvdcs_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(mt_hvdc_meter_map);
    return build_meters_with_multi_mt_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_equivalent_devices_complete_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_equivalent_devices_device_id();
    vector<string> keys = get_sorted_keys_from_meter_map(equivalent_device_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_common_meters()
{
    vector<METER> all_meters;
    vector<METER> meters;

    meters = build_all_buses_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_generators_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_wt_generators_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_pv_units_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_energy_storages_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_sv_sources_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_loads_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_ac_lines_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_transformers_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_lcc_hvdc2ts_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_vsc_hvdcs_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_mt_hvdcs_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    meters = build_all_equivalent_devices_common_meters();
    all_meters.insert(all_meters.end(), meters.begin(), meters.end());

    return all_meters;
}

vector<METER> METER_BUILDER::build_all_buses_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_buses_device_id();
    vector<string> keys = {"VOLTAGE IN PU", "ANGLE IN DEG", "FREQUENCY IN HZ"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_generators_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_generators_device_id();
    vector<string> keys = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                           "MECHANICAL POWER IN MW",      "EXCITATION VOLTAGE IN PU",
                           "STABILIZING SIGNAL IN PU",
                           "ROTOR ANGLE IN DEG",          "ROTOR SPEED DEVIATION IN PU"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_wt_generators_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_wt_generators_device_id();
    vector<string> keys = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                           "MECHANICAL POWER IN MW",
                           "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU",
                           "TURBINE SPEED IN PU",         "PITCH ANGLE IN DEG"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_pv_units_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_pv_units_device_id();
    vector<string> keys = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                           "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_energy_storages_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_energy_storages_device_id();
    vector<string> keys = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                           "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_sv_sources_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_sv_sources_device_id();
    vector<string> keys = {"TERMINAL REACTIVE POWER IN MVAR", "TERMINAL CURRENT IN PU ON SBASE"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_loads_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_loads_device_id();
    vector<string> keys = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "TOTAL SCALE IN PU"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_ac_lines_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_ac_lines_device_id();
    vector<string> keys = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "CURRENT IN KA"};
    return build_meters_with_multi_ac_lines_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_transformers_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_transformers_device_id();
    vector<string> keys = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "CURRENT IN KA"};
    return build_meters_with_multi_transformers_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_lcc_hvdc2ts_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_lcc_hvdc2ts_device_id();
    vector<string> keys = {"DC VOLTAGE IN KV",     "DC CURRENT IN KA", "DC POWER IN MW",
                           "ANGLE IN DEG",         "MU IN DEG",
                           "AC VOLTAGE IN PU",     "AC CURRENT IN KA",
                           "AC ACTIVE POWER IN MW","AC REACTIVE POWER IN MVAR"};
    return build_meters_with_multi_lcc_hvdc2t_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_vsc_hvdcs_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_vsc_hvdcs_device_id();
    vector<string> keys = {"CONVERTER AC VOLTAGE IN PU",  "CONVERTER AC CURRENT IN KA",
                           "CONVERTER DC VOLTAGE IN KV",  "DC BUS VOLTAGE IN KV",
                           "CONVERTER DC CURRENT IN KA",  "CONVERTER DC POWER IN MW",
                           "CONVERTER AC ACTIVE POWER IN MW", "CONVERTER AC REACTIVE POWER IN MVAR",
                           "DC LINE CURRENT IN KA", "DC LINE POWER IN MW"};
    return build_meters_with_multi_vsc_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_mt_hvdcs_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_mt_hvdcs_device_id();
    vector<string> keys = {"CONVERTER AC VOLTAGE IN PU",  "CONVERTER AC CURRENT IN KA",
                                         "CONVERTER DC VOLTAGE IN KV",  "DC BUS VOLTAGE IN KV",
                                         "CONVERTER DC CURRENT IN KA",  "CONVERTER DC POWER IN MW",
                                         "CONVERTER AC ACTIVE POWER IN MW", "CONVERTER AC REACTIVE POWER IN MVAR",
                                         "DC LINE CURRENT IN KA", "DC LINE POWER IN MW"};
    return build_meters_with_multi_mt_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_all_equivalent_devices_common_meters()
{
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_equivalent_devices_device_id();
    vector<string> keys = {"VOLTAGE SOURCE VOLTAGE IN PU",         "VOLTAGE SOURCE VOLTAGE ANGLE IN DEG",
                           "VOLTAGE SOURCE RESISTANCE IN PU",      "VOLTAGE SOURCE REACTANCE IN PU",
                           "ACTIVE CONSTANT POWER LOAD IN MW",     "REACTIVE CONSTANT POWER LOAD IN MVAR",
                           "ACTIVE CONSTANT CURRENT LOAD IN MW",   "REACTIVE CONSTANT CURRENT LOAD IN MVAR",
                           "ACTIVE CONSTANT IMPEDANCE LOAD IN MW", "REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR",
                           "ACTIVE POWER GENERATION IN MW",        "REACTIVE POWER GENERATION IN MVAR",
                           "ACTIVE POWER LOAD IN MW",              "REACTIVE POWER LOAD IN MVAR",
                           "ACTIVE POWER NET LOAD IN MW",          "REACTIVE POWER NET LOAD IN MVAR"};
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_bus_complete_meters(unsigned int bus)
{
    DEVICE_ID did = get_bus_device_id(bus);
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(bus_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_generator_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(sg_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_wt_generator_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(wtg_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_pv_unit_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(pvu_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_energy_storage_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(es_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_sv_source_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(sv_source_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_load_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(load_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_ac_line_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(ac_line_meter_map);
    return build_meters_with_multi_ac_lines_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_transformer_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(transformer_meter_map);
    return build_meters_with_multi_transformers_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_lcc_hvdc2t_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(lcc_hvdc2t_meter_map);
    return build_meters_with_multi_lcc_hvdc2t_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_vsc_hvdc_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(vsc_hvdc_meter_map);
    return build_meters_with_multi_vsc_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_mt_hvdc_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(vsc_hvdc_meter_map);
    return build_meters_with_multi_mt_hvdcs_and_meter_types(dids, keys);
}

vector<METER> METER_BUILDER::build_one_equivalent_device_complete_meters(const DEVICE_ID& did)
{
    vector<DEVICE_ID> dids = {did};
    vector<string> keys = get_sorted_keys_from_meter_map(equivalent_device_meter_map);
    return build_meters_with_multi_devices_and_meter_types(dids, keys);
}

METER METER_BUILDER::build_one_bus_related_meter(unsigned int bus, string meter_type)
{
    DEVICE_ID did = get_bus_device_id(bus);
    return build_one_meter(did, meter_type);
}

METER METER_BUILDER::build_one_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_sv_source_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_ac_line_exist(did))
    {
        side = string2upper(side);

        AC_LINE* line = psdb.get_ac_line(did);
        unsigned int bus = 0;
        if(side=="SENDING" or side=="S")
            bus = line->get_sending_side_bus();
        if(side=="RECEIVING" or side=="R")
            bus = line->get_receiving_side_bus();

        meter = build_one_ac_line_related_meter(did, meter_type, bus, var_name);
    }
    return meter;
}

METER METER_BUILDER::build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, unsigned int bus, string var_name)
{
    return build_one_meter(did, meter_type, bus, var_name);
}

METER METER_BUILDER::build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_transformer_exist(did))
    {
        side = string2upper(side);

        TRANSFORMER* trans = psdb.get_transformer(did);
        unsigned int bus = trans->get_winding_bus(transformer_winding_string2winding_side(side));

        meter = build_one_transformer_related_meter(did, meter_type, bus, var_name);
    }
    return meter;
}

METER METER_BUILDER::build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name)
{
    return build_one_meter(did, meter_type, side_bus, var_name);
}

METER METER_BUILDER::build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_lcc_hvdc2t_exist(did))
    {
        side = string2upper(side);

        LCC_HVDC2T* hvdc = psdb.get_lcc_hvdc2t(did);
        unsigned int bus = hvdc->get_converter_bus(converter_string2side(side));

        meter = build_one_lcc_hvdc2t_related_meter(did, meter_type, bus, var_name);
    }
    return meter;

}

METER METER_BUILDER::build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_lcc_hvdc2t_exist(did))
        meter = build_one_meter(did, meter_type, side_bus, var_name);
    return meter;
}

METER METER_BUILDER::build_one_vsc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side, string var_name)
{
    return build_one_meter(did, meter_type, side, var_name);
}

METER METER_BUILDER::build_one_vsc_hvdc_related_meter(string vsc_name, string meter_type, unsigned int side, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    if(vsc_hvdc != NULL)
        meter = build_one_vsc_hvdc_related_meter(vsc_hvdc->get_device_id(), meter_type, side, var_name);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. VSC HVDC with name "<<vsc_name<<" does not exist when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    return meter;
}

METER METER_BUILDER::build_one_vsc_hvdc_related_dc_line_meter(const DEVICE_ID& did, string meter_type, const DC_DEVICE_ID dc_did, unsigned int side, string var_name)
{
    METER meter(*toolkit);
    meter = build_one_vsc_hvdc_related_meter(did, meter_type, side, var_name);
    meter.set_sub_dc_device_id(dc_did);
    return meter;
}

METER METER_BUILDER::build_one_mt_hvdc_related_meter(const DEVICE_ID& did, string sub_device_name, string meter_type, string var_name)
 {
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_mt_hvdc_exist(did))
        meter = build_one_meter_with_sub_device_name(did, sub_device_name, meter_type, 0, var_name);
    return meter;
 }

METER METER_BUILDER::build_one_mt_hvdc_related_meter(string mtdc_name, string sub_device_name, string meter_type, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    if(mtdc!=NULL)
        meter = build_one_mt_hvdc_related_meter(mtdc->get_device_id(), sub_device_name, meter_type, var_name);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Meter of MTDC project:"<<mtdc_name<<" cannot be set since the given project name does not exist in the power system database";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    return meter;
}

METER METER_BUILDER::build_one_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, 0, var_name);
}

METER METER_BUILDER::build_one_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name)
{
    METER meter(*toolkit);
    meter_type = string2upper(meter_type);

    meter.set_device_id(did);
    meter.set_meter_type(meter_type);
    if(side_bus != 0)
        meter.set_metered_side_bus(side_bus);
    if(meter_type.find("INTERNAL VARIABLE") != std::string::npos)
        meter.set_internal_variable_name(string2upper(var_name));
    if(not meter.is_valid())
    {
        ostringstream osstream;
        osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    return meter;
}

METER METER_BUILDER::build_one_meter_with_sub_device_name(const DEVICE_ID& did, string sub_device_name, string meter_type, unsigned int side_bus, string var_name)
{
    METER meter = build_one_meter(did, meter_type, side_bus, var_name);
    meter.set_sub_device_name(sub_device_name);
    return meter;
}

METER METER_BUILDER::build_one_meter_with_sub_dc_device_id(const DEVICE_ID& did, DC_DEVICE_ID& sub_dc_did, string meter_type, unsigned int side_bus, string var_name)
{
    METER meter = build_one_meter(did, meter_type, side_bus, var_name);
    meter.set_sub_dc_device_id(sub_dc_did);
    return meter;
}

vector<METER> METER_BUILDER::build_meters_with_multi_devices_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    vector<METER> meters;
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for(unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                METER meter = build_one_meter(dids[j], keys[i]);
                if(meter.is_valid())
                    meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> METER_BUILDER::build_meters_with_multi_ac_lines_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    vector<METER> meters;
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                METER meter = build_one_ac_line_related_meter(dids[j], keys[i], "SENDING", "");
                if(meter.is_valid())
                    meters.push_back(meter);

                meter = build_one_ac_line_related_meter(dids[j], keys[i], "RECEIVING", "");
                if(meter.is_valid())
                    meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> METER_BUILDER::build_meters_with_multi_transformers_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    POWER_SYSTEM_DATABASE& psdb = get_toolkit().get_power_system_database();
    vector<METER> meters;
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                METER meter = build_one_transformer_related_meter(dids[j], keys[i], "PRIMARY", "");
                if(meter.is_valid())
                    meters.push_back(meter);

                meter = build_one_transformer_related_meter(dids[j], keys[i], "SECONDARY", "");
                if(meter.is_valid())
                    meters.push_back(meter);

                TRANSFORMER* trans = psdb.get_transformer(dids[j]);
                if(trans->is_three_winding_transformer())
                {
                    meter = build_one_transformer_related_meter(dids[j], keys[i], "TERTIARY", "");
                    if(meter.is_valid())
                        meters.push_back(meter);
                }
            }
        }
    }
    return meters;
}

vector<METER> METER_BUILDER::build_meters_with_multi_lcc_hvdc2t_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    vector<METER> meters;
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                METER meter = build_one_lcc_hvdc2t_related_meter(dids[j], keys[i], "RECTIFIER");
                if(meter.is_valid())
                    meters.push_back(meter);

                meter = build_one_lcc_hvdc2t_related_meter(dids[j], keys[i], "INVERTER");
                if(meter.is_valid())
                    meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> METER_BUILDER::build_meters_with_multi_vsc_hvdcs_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    vector<METER> meters;
    POWER_SYSTEM_DATABASE& psdb = get_toolkit().get_power_system_database();
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                DEVICE_ID did = dids[j];
                VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(did);

                if(vsc_hvdc != nullptr)
                {
                    if(keys[i].starts_with("CONVERTER"))
                    {
                        unsigned int n_converter = vsc_hvdc->get_converter_count();
                        for(unsigned int k=0; k<n_converter; ++k)
                        {
                            unsigned int bus = vsc_hvdc->get_converter_ac_bus(k);
                            METER meter = build_one_vsc_hvdc_related_meter(did, keys[i], bus);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                    else if(keys[i].starts_with("DC BUS"))
                    {
                        unsigned int n_dc_bus = vsc_hvdc->get_dc_bus_count();
                        for(unsigned int k=0; k<n_dc_bus; ++k)
                        {
                            unsigned int dc_bus_number = vsc_hvdc->get_dc_bus_number(k);
                            METER meter = build_one_vsc_hvdc_related_meter(did, keys[i], dc_bus_number);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                    else if(keys[i].starts_with("DC LINE"))
                    {
                        unsigned int n_dc_line = vsc_hvdc->get_dc_line_count();
                        for(unsigned int k=0; k<n_dc_line; ++k)
                        {
                            DC_DEVICE_ID dc_line_did = vsc_hvdc->get_dc_line_device_id(k);
                            unsigned int ibus = vsc_hvdc->get_dc_line_sending_side_bus(k);
                            unsigned int jbus = vsc_hvdc->get_dc_line_receiving_side_bus(k);

                            METER meter = build_one_meter_with_sub_dc_device_id(did, dc_line_did, keys[i], ibus);
                            if(meter.is_valid())
                                meters.push_back(meter);
                            meter = build_one_meter_with_sub_dc_device_id(did, dc_line_did, keys[i], jbus);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                }
            }
        }
    }
    return meters;
}

vector<METER> METER_BUILDER::build_meters_with_multi_mt_hvdcs_and_meter_types(const vector<DEVICE_ID>& dids, vector<string>& keys)
{
    vector<METER> meters;
    POWER_SYSTEM_DATABASE& psdb = get_toolkit().get_power_system_database();
    unsigned int m = keys.size();
    unsigned int n = dids.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                DEVICE_ID did = dids[j];
                MT_HVDC* mt_hvdc = psdb.get_mt_hvdc(did);

                if(mt_hvdc != nullptr)
                {
                    if(keys[i].starts_with("LCC CONVERTER"))
                    {
                        unsigned int n_converter = mt_hvdc->get_lcc_converter_count();
                        for(unsigned int k=0; k<n_converter; ++k)
                        {
                            string converter_name = mt_hvdc->get_lcc_converter_name(k);
                            METER meter = build_one_mt_hvdc_related_meter(did, converter_name, keys[i]);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                    if(keys[i].starts_with("VSC CONVERTER"))
                    {
                        unsigned int n_converter = mt_hvdc->get_vsc_converter_count();
                        for(unsigned int k=0; k<n_converter; ++k)
                        {
                            string converter_name = mt_hvdc->get_vsc_converter_name(k);
                            METER meter = build_one_mt_hvdc_related_meter(did, converter_name, keys[i]);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                    else if(keys[i].starts_with("DC BUS"))
                    {
                        unsigned int n_dc_bus = mt_hvdc->get_dc_bus_count();
                        for(unsigned int k=0; k<n_dc_bus; ++k)
                        {
                            string dc_bus_name = mt_hvdc->get_dc_bus_name(k);
                            METER meter = build_one_mt_hvdc_related_meter(did, dc_bus_name, keys[i]);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                    else if(keys[i].starts_with("DC LINE"))
                    {
                        unsigned int n_dc_line = mt_hvdc->get_dc_line_count();
                        for(unsigned int k=0; k<n_dc_line; ++k)
                        {
                            string dc_line_name = mt_hvdc->get_dc_line_name(k);
                            unsigned int ibus = mt_hvdc->get_dc_line_sending_side_bus_number(k);
                            unsigned int jbus = mt_hvdc->get_dc_line_receiving_side_bus_number(k);

                            METER meter = build_one_meter_with_sub_device_name(did, dc_line_name, keys[i], ibus);
                            if(meter.is_valid())
                                meters.push_back(meter);
                            meter = build_one_meter_with_sub_device_name(did, dc_line_name, keys[i], jbus);
                            if(meter.is_valid())
                                meters.push_back(meter);
                        }
                    }
                }
            }
        }
    }
    return meters;
}

vector<string> METER_BUILDER::get_sorted_keys_from_meter_map(unordered_map<string, function<double(METER_PARAMETER)>>& meter_map)
{
    vector<string> keys;
    keys.reserve(meter_map.size());
    for (auto iter = meter_map.begin(); iter != meter_map.end(); ++iter)
        keys.push_back(iter->first);
    std::sort(keys.begin(), keys.end());
    return keys;
}
