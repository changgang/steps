#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

class STEPS;

vector<METER> DYNAMICS_SIMULATOR::build_all_meters()
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

vector<METER> DYNAMICS_SIMULATOR::build_all_buses_complete_meters()
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(bus_meter_map);
    unsigned int m = keys.size();

    vector<unsigned int> bus_numbers = toolkit->get_power_system_database().get_all_buses_number();
    unsigned int n = bus_numbers.size();

    for(unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            for(unsigned int j=0; j!=n; ++j)
            {
                METER meter = build_one_bus_related_meter(bus_numbers[j], keys[i]);
                if(meter.is_valid())
                    meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_meters_with_multi_devices_and_meter_typs(const vector<DEVICE_ID>& dids, vector<string>& keys)
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

vector<METER> DYNAMICS_SIMULATOR::build_all_generators_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(sg_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_generators_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_wt_generators_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(wtg_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_wt_generators_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_pv_units_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(pvu_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_pv_units_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_energy_storages_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(es_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_energy_storages_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_sv_sources_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(sv_source_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_sv_sources_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_loads_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(load_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_loads_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_ac_lines_complete_meters()
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(ac_line_meter_map);
    unsigned int m = keys.size();

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_ac_lines_device_id();
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

vector<METER> DYNAMICS_SIMULATOR::build_all_transformers_complete_meters()
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(transformer_meter_map);
    unsigned int m = keys.size();

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<DEVICE_ID> dids = psdb.get_all_transformers_device_id();
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

vector<METER> DYNAMICS_SIMULATOR::build_all_lcc_hvdc2ts_complete_meters()
{
    vector<METER> meters;
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_vsc_hvdcs_complete_meters()
{
    vector<METER> meters;
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_mt_hvdcs_complete_meters()
{
    vector<METER> meters;
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_equivalent_devices_complete_meters()
{
    vector<string> keys = get_sorted_keys_from_meter_map(equivalent_device_meter_map);
    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_equivalent_devices_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, keys);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_common_meters()
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

vector<METER> DYNAMICS_SIMULATOR::build_all_buses_common_meters()
{
    vector<string> common_meter_types = {"VOLTAGE IN PU", "ANGLE IN DEG", "FREQUENCY IN HZ"};

    vector<unsigned int> buses = toolkit->get_power_system_database().get_all_buses_number();
    unsigned int n = buses.size();

    vector<METER> meters;
    for(unsigned int i = 0; i<common_meter_types.size(); ++i)
    {
        for(unsigned int j=0; j!=n; ++j)
        {
            METER meter = build_one_bus_related_meter(buses[j], common_meter_types[i]);
            meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_generators_common_meters()
{
    vector<string> common_meter_types = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                                      "MECHANICAL POWER IN MW",      "EXCITATION VOLTAGE IN PU",
                                      "STABILIZING SIGNAL IN PU",
                                      "ROTOR ANGLE IN DEG",          "ROTOR SPEED DEVIATION IN PU"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_generators_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_wt_generators_common_meters()
{
    vector<string> common_meter_types = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                                      "MECHANICAL POWER IN MW",
                                      "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU",
                                      "TURBINE SPEED IN PU",         "PITCH ANGLE IN DEG"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_wt_generators_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_pv_units_common_meters()
{
    vector<string> common_meter_types = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                                      "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_pv_units_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_energy_storages_common_meters()
{
    vector<string> common_meter_types = {"TERMINAL ACTIVE POWER IN MW", "TERMINAL REACTIVE POWER IN MVAR",
                                      "ACTIVE CURRENT COMMAND IN PU", "REACTIVE CURRENT COMMAND IN PU"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_energy_storages_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_sv_sources_common_meters()
{
    vector<string> common_meter_types = {"TERMINAL REACTIVE POWER IN MVAR", "TERMINAL CURRENT IN PU ON SBASE"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_sv_sources_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_loads_common_meters()
{
    vector<string> common_meter_types = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "TOTAL SCALE IN PU"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_loads_device_id();
    return build_meters_with_multi_devices_and_meter_typs(dids, common_meter_types);
}

vector<METER> DYNAMICS_SIMULATOR::build_all_ac_lines_common_meters()
{
    vector<string> common_meter_types = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "CURRENT IN KA"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_ac_lines_device_id();
    unsigned int n = dids.size();

    vector<METER> meters;
    for(unsigned int i = 0; i<common_meter_types.size(); ++i)
    {
        for(unsigned int j=0; j!=n; ++j)
        {
            METER meter = build_one_ac_line_related_meter(dids[j], common_meter_types[i], "SENDING");
            meters.push_back(meter);
            meter = build_one_ac_line_related_meter(dids[j], common_meter_types[i], "RECEIVING");
            meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_transformers_common_meters()
{
    vector<string> common_meter_types = {"ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR", "CURRENT IN KA"};

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<DEVICE_ID> dids = psdb.get_all_transformers_device_id();
    unsigned int n = dids.size();

    vector<METER> meters;
    for(unsigned int i = 0; i<common_meter_types.size(); ++i)
    {
        for(unsigned int j=0; j!=n; ++j)
        {
            METER meter = build_one_transformer_related_meter(dids[j], common_meter_types[i], "PRIMARY");
            meters.push_back(meter);
            meter = build_one_transformer_related_meter(dids[j], common_meter_types[i], "SECONDARY");
            meters.push_back(meter);
            if(psdb.get_transformer(dids[j])->is_three_winding_transformer())
            {
                meter = build_one_transformer_related_meter(dids[j], common_meter_types[i], "TERTIARY");
                meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_lcc_hvdc2ts_common_meters()
{
    vector<string> common_meter_types = {"DC VOLTAGE IN KV",     "DC CURRENT IN KA", "DC POWER IN MW",
                                         "ANGLE IN DEG",         "MU IN DEG",
                                         "AC VOLTAGE IN PU",     "AC CURRENT IN KA",
                                         "AC ACTIVE POWER IN MW","AC REACTIVE POWER IN MVAR"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_lcc_hvdc2ts_device_id();
    unsigned int n = dids.size();

    vector<METER> meters;
    for(unsigned int i = 0; i<common_meter_types.size(); ++i)
    {
        for(unsigned int j=0; j!=n; ++j)
        {
            METER meter = build_one_lcc_hvdc2t_related_meter(dids[j], common_meter_types[i], "RECTIFIER");
            meters.push_back(meter);

            meter = build_one_lcc_hvdc2t_related_meter(dids[j], common_meter_types[i], "INVERTER");
            meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_vsc_hvdcs_common_meters()
{
    vector<METER> meters;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_vsc_hvdc_count();
    vector<VSC_HVDC*> vsc_hvdcs = psdb.get_all_vsc_hvdcs();
    VSC_HVDC* vsc_hvdc;
    DEVICE_ID did;
    for(unsigned int i=0; i!=n; ++i)
    {
        vsc_hvdc = vsc_hvdcs[i];
        did = vsc_hvdc->get_device_id();

        unsigned int n_converter = vsc_hvdc->get_converter_count();
        for(unsigned int j=0; j!=n_converter; ++j)
        {
            unsigned int meter_side = vsc_hvdc->get_converter_ac_bus(j);
            METER meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
        }
        unsigned int n_dc_bus = vsc_hvdc->get_dc_bus_count();
        for(unsigned int j=0; j!=n_dc_bus; ++j)
        {
            unsigned int meter_side = vsc_hvdc->get_dc_bus_number(j);
            METER meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, meter_side);
            if(meter.is_valid())
                meters.push_back(meter);
        }
        unsigned int n_dc_line = vsc_hvdc->get_dc_line_count();
        for(unsigned int j=0; j!=n_dc_line; ++j)
        {
            DC_DEVICE_ID dc_line_did = vsc_hvdc->get_dc_line_device_id(j);

            unsigned int ibus = vsc_hvdc->get_dc_line_sending_side_bus(j);
            unsigned int jbus = vsc_hvdc->get_dc_line_receiving_side_bus(j);

            METER meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, ibus);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, jbus);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, ibus);
            if(meter.is_valid())
                meters.push_back(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, jbus);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_mt_hvdcs_common_meters()
{
    vector<METER> meters;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_mt_hvdc_count();
    vector<MT_HVDC*> mtdcs = psdb.get_all_mt_hvdcs();
    MT_HVDC* mtdc;
    DEVICE_ID did;
    for(unsigned int i=0;i!=n;++i)
    {
        mtdc = mtdcs[i];
        did = mtdc->get_device_id();
        //lcc
        vector<HLCC*> lccs = mtdc->get_all_lcc_converters();
        unsigned int n_lcc = lccs.size();
        for(unsigned int j=0;j!=n_lcc;++j)
        {
            string lcc_name = lccs[j]->get_converter_name();

            METER meter = setter.prepare_mt_hvdc_lcc_converter_dc_current_in_kA_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_dc_voltage_in_kV_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_dc_power_in_kV_MW_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_current_in_kA_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_firing_angle_in_deg_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_active_power_in_MW_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_reactive_power_in_Mvar_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cc_in_deg_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cea_in_deg_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cv_in_deg_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_idc_in_kA_meter(did, lcc_name);
            if(meter.is_valid())
                meters.push_back(meter);
        }
        //vsc
        vector<HVSC*> vscs = mtdc->get_all_vsc_converters();
        unsigned int n_vsc = vscs.size();
        for(unsigned int j=0;j!=n_vsc;++j)
        {
            string vsc_name = vscs[j]->get_converter_name();

            METER meter = setter.prepare_mt_hvdc_vsc_converter_dc_current_in_kA_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_dc_voltage_in_kV_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_dc_power_in_kV_MW_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_current_in_kA_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_active_power_in_MW_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_reactive_power_in_Mvar_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_id_in_kA_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_iq_in_kA_meter(did, vsc_name);
            if(meter.is_valid())
                meters.push_back(meter);
        }
        //dc bus
        vector<DC_BUS*> dcbuses = mtdc->get_all_dc_buses();
        unsigned int n_dcbus = dcbuses.size();
        for(unsigned int j=0;j!=n_dcbus;++j)
        {
            string dcbus_name = dcbuses[j]->get_name();

            METER meter = setter.prepare_mt_hvdc_dc_bus_voltage_in_kV_meter(did, dcbus_name);
            if(meter.is_valid())
                meters.push_back(meter);
        }
        //dc line
        vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
        unsigned int n_dclines = dc_lines.size();
        for(unsigned int j=0;j!=n_dclines;++j)
        {
            string line_name = dc_lines[j]->get_dc_line_name();

            METER meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_sending_side_in_kA_meter(did, line_name);
            if(meter.is_valid())
                meters.push_back(meter);

            meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_receiving_side_in_kA_meter(did, line_name);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_all_equivalent_devices_common_meters()
{
    vector<string> common_meter_types = {"VOLTAGE SOURCE VOLTAGE IN PU",         "VOLTAGE SOURCE VOLTAGE ANGLE IN DEG",
                                      "VOLTAGE SOURCE RESISTANCE IN PU",      "VOLTAGE SOURCE REACTANCE IN PU",
                                      "ACTIVE CONSTANT POWER LOAD IN MW",     "REACTIVE CONSTANT POWER LOAD IN MVAR",
                                      "ACTIVE CONSTANT CURRENT LOAD IN MW",   "REACTIVE CONSTANT CURRENT LOAD IN MVAR",
                                      "ACTIVE CONSTANT IMPEDANCE LOAD IN MW", "REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR",
                                      "ACTIVE POWER GENERATION IN MW",        "REACTIVE POWER GENERATION IN MVAR",
                                      "ACTIVE POWER LOAD IN MW",              "REACTIVE POWER LOAD IN MVAR",
                                      "ACTIVE POWER NET LOAD IN MW",          "REACTIVE POWER NET LOAD IN MVAR"};

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_equivalent_devices_device_id();
    unsigned int n = dids.size();

    vector<METER> meters;
    for(unsigned int i = 0; i<common_meter_types.size(); ++i)
    {
        for(unsigned int j=0; j!=n; ++j)
        {
            METER meter = build_one_equivalent_device_related_meter(dids[j], common_meter_types[i]);
            meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_bus_complete_meters(unsigned int bus)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(bus_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_bus_related_meter(bus, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_generator_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(sg_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_generator_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_wt_generator_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(wtg_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_wt_generator_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_pv_unit_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(pvu_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_pv_unit_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_energy_storage_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(es_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_energy_storage_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_sv_source_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(sv_source_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_sv_source_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_load_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(load_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i = 0; i < m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_load_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_ac_line_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(ac_line_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_ac_line_related_meter(did, keys[i], "SENDING");
            if(meter.is_valid())
                meters.push_back(meter);

            meter = build_one_ac_line_related_meter(did, keys[i], "RECEIVING");
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_transformer_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(transformer_meter_map);
    unsigned int m = keys.size();

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    TRANSFORMER* trans = psdb.get_transformer(did);
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_transformer_related_meter(did, keys[i], "PRIMARY");
            if(meter.is_valid())
                meters.push_back(meter);
            meter = build_one_transformer_related_meter(did, keys[i], "SECONDARY");
            if(meter.is_valid())
                meters.push_back(meter);
            if(trans->is_three_winding_transformer())
            {
                meter = build_one_transformer_related_meter(did, keys[i], "TERTIARY");
                if(meter.is_valid())
                    meters.push_back(meter);
            }
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_lcc_hvdc2t_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(lcc_hvdc2t_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_lcc_hvdc2t_related_meter(did, keys[i], "RECTIFIER");
            if(meter.is_valid())
                meters.push_back(meter);

            meter = build_one_lcc_hvdc2t_related_meter(did, keys[i], "INVERTER");
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_vsc_hvdc_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_mt_hvdc_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    return meters;
}

vector<METER> DYNAMICS_SIMULATOR::build_one_equivalent_device_complete_meters(const DEVICE_ID& did)
{
    vector<METER> meters;
    vector<string> keys = get_sorted_keys_from_meter_map(equivalent_device_meter_map);
    unsigned int m = keys.size();
    for (unsigned int i=0; i<m; ++i)
    {
        if(keys[i].find("INTERNAL VARIABLE") == std::string::npos)
        {
            METER meter = build_one_equivalent_device_related_meter(did, keys[i]);
            if(meter.is_valid())
                meters.push_back(meter);
        }
    }
    return meters;
}

METER DYNAMICS_SIMULATOR::build_one_bus_related_meter(unsigned int bus, string meter_type)
{
    METER meter(*toolkit);

    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_BUS);
    TERMINAL terminal;
    terminal.append_bus(bus);
    device_id.set_device_terminal(terminal);

    meter.set_device_id(device_id);

    meter_type = string2upper(meter_type);
    meter.set_meter_type(meter_type);
    if(not meter.is_valid())
    {
        ostringstream osstream;
        osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for bus "<<bus;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    return meter;
}

METER DYNAMICS_SIMULATOR::build_one_meter(const DEVICE_ID& did, string meter_type, string var_name, unsigned int side_bus)
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

METER DYNAMICS_SIMULATOR::build_one_meter_with_sub_device_name(const DEVICE_ID& did, string sub_device_name, string meter_type, string var_name, unsigned int side_bus)
{
    METER meter = build_one_meter(did, meter_type, var_name, side_bus);
    meter.set_sub_device_name(sub_device_name);
    return meter;
}

METER DYNAMICS_SIMULATOR::build_one_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_sv_source_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

METER DYNAMICS_SIMULATOR::build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
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

METER DYNAMICS_SIMULATOR::build_one_ac_line_related_meter(const DEVICE_ID& did, string meter_type, unsigned int bus, string var_name)
{
    return build_one_meter(did, meter_type, var_name, bus);
}

METER DYNAMICS_SIMULATOR::build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
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

METER DYNAMICS_SIMULATOR::build_one_transformer_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name)
{
    return build_one_meter(did, meter_type, var_name, side_bus);
}

METER DYNAMICS_SIMULATOR::build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
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

METER DYNAMICS_SIMULATOR::build_one_lcc_hvdc2t_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side_bus, string var_name)
{
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_lcc_hvdc2t_exist(did))
        meter = build_one_meter(did, meter_type, var_name, side_bus);
    return meter;
}

METER DYNAMICS_SIMULATOR::build_one_vsc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side, string var_name)
{
    return build_one_meter(did, meter_type, var_name, side);
}

METER DYNAMICS_SIMULATOR::build_one_vsc_hvdc_related_meter(string vsc_name, string meter_type, unsigned int side, string var_name)
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

METER DYNAMICS_SIMULATOR::build_one_vsc_hvdc_related_dc_line_meter(const DEVICE_ID& did, string meter_type, const DC_DEVICE_ID dc_did, unsigned int side, string var_name)
{
    METER meter(*toolkit);
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_VSC_HVDC)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="DC LINE CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_did, side);
            if(meter_type=="DC LINE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_did, side);

            if(not meter.is_valid())
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not VSC LCC_HVDC2T when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    return meter;
}

METER DYNAMICS_SIMULATOR::build_one_mt_hvdc_related_meter(const DEVICE_ID& did, string sub_device_name, string meter_type, string var_name)
 {
    METER meter(*toolkit);
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.is_mt_hvdc_exist(did))
        meter = build_one_meter_with_sub_device_name(did, sub_device_name, meter_type, var_name, 0);
    return meter;
 }

METER DYNAMICS_SIMULATOR::build_one_mt_hvdc_related_meter(string mtdc_name, string sub_device_name, string meter_type, string var_name)
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

METER DYNAMICS_SIMULATOR::build_one_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    return build_one_meter(did, meter_type, var_name);
}

vector<string> DYNAMICS_SIMULATOR::get_sorted_keys_from_meter_map(unordered_map<string, function<double(METER_PARAMETER)>>& meter_map)
{
    vector<string> keys;
    keys.reserve(meter_map.size());
    for (auto iter = meter_map.begin(); iter != meter_map.end(); ++iter)
        keys.push_back(iter->first);
    std::sort(keys.begin(), keys.end());
    return keys;
}
