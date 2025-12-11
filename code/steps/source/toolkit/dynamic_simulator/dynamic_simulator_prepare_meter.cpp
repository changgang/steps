#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include "header/meter/basic_meter_preparer.h"
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

void DYNAMICS_SIMULATOR::prepare_meters()
{
    prepare_bus_related_meters();
    prepare_generator_related_meters();
    prepare_wt_generator_related_meters();
    prepare_pv_unit_related_meters();
    prepare_energy_storage_related_meters();
    prepare_sv_source_related_meters();
    prepare_load_related_meters();
    prepare_ac_line_related_meters();
    prepare_transformer_related_meters();
    prepare_2t_lcc_hvdc_related_meters();
    prepare_vsc_hvdc_related_meters();
    prepare_mt_hvdc_related_meters();
    prepare_equivalent_device_related_meters();
}


void DYNAMICS_SIMULATOR::prepare_bus_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n = psdb.get_bus_count();
    vector<BUS*> buses = psdb.get_all_buses();
    BUS* bus;
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_voltage_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_angle_in_deg_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_in_Hz_meter(bus->get_bus_number());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_generator_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_generator_count();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_excitation_voltage_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_active_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(generator->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meters()
{
    vector<string> keys;
    keys.reserve(wtg_meter_map.size());
    for (auto iter = wtg_meter_map.begin(); iter != wtg_meter_map.end(); ++iter)
        keys.push_back(iter->first);
    std::sort(keys.begin(), keys.end());
    unsigned int m = keys.size();

    vector<DEVICE_ID> dids = toolkit->get_power_system_database().get_all_wt_generators_device_id();
    unsigned int n = dids.size();

    DYNAMICS_SIMULATOR& sim = toolkit->get_dynamic_simulator();
    for (unsigned int i=0; i<m; ++i)
        for(unsigned int j=0; j!=n; ++j)
            sim.prepare_wt_generator_related_meter(dids[j], keys[i]);
}

void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_pv_unit_count();
    vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
    PV_UNIT* pv_unit;
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_current_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_current_in_pu_on_sbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_active_power_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_active_power_in_pu_on_sbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_reactive_power_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_reactive_power_in_pu_on_sbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_apparent_power_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_apparent_power_in_pu_on_sbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_apparent_power_in_MVA_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_solar_irradiance_in_pu_on_s0_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_solar_irradiance_in_pu_on_sref_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_solar_irradiance_in_Wpm2_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_temperature_in_c_deg_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_MW_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_pu_on_sbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_panel_power_in_pu_on_mbase_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_panel_power_in_MW(pv_unit->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_energy_storage_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_energy_storage_count();
    vector<ENERGY_STORAGE*> energy_storages = psdb.get_all_energy_storages();
    ENERGY_STORAGE* energy_storage;
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_current_in_kA_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_current_in_pu_on_mbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_current_in_pu_on_sbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_active_power_in_MW_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_reactive_power_in_MVar_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_apparent_power_in_pu_on_mbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_apparent_power_in_pu_on_sbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_terminal_apparent_power_in_MVA_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_active_current_command_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_reactive_current_command_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_active_power_command_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_reactive_power_command_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_reactive_voltage_command_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_soc_power_in_pu_on_mbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_soc_power_in_pu_on_sbase_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_soc_power_in_MW_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        energy_storage = energy_storages[i];
        METER meter = setter.prepare_es_state_of_charge_in_pu_meter(energy_storage->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_sv_source_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_sv_source_count();
    vector<SV_SOURCE*> svss = psdb.get_all_sv_sources();
    SV_SOURCE* svs;

    for(unsigned int i=0; i!=n; ++i)
    {
        svs = svss[i];
        METER meter = setter.prepare_svs_terminal_current_in_pu_on_sbase_meter(svs->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        svs = svss[i];
        METER meter = setter.prepare_svs_terminal_reactive_power_in_MVar_meter(svs->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_load_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_load_count();
    vector<LOAD*> loads = psdb.get_all_loads();
    LOAD* load;
    for(unsigned int i=0; i!=n; ++i)
    {
        load = loads[i];
        METER meter = setter.prepare_load_active_power_in_MW_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_reactive_power_in_MVar_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_total_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_manually_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_relay_shed_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
    }

}

void DYNAMICS_SIMULATOR::prepare_ac_line_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_ac_line_count();
    vector<AC_LINE*> lines = psdb.get_all_ac_lines();
    AC_LINE* line;
    for(unsigned int i=0; i!=n; ++i)
    {
        line = lines[i];
        METER meter = setter.prepare_ac_line_active_power_in_MW_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_ac_line_reactive_power_in_MVar_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_ac_line_current_in_kA_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_ac_line_active_power_in_MW_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
        meter = setter.prepare_ac_line_reactive_power_in_MVar_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
        meter = setter.prepare_ac_line_current_in_kA_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_transformer_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_transformer_count();
    vector<TRANSFORMER*> transes = psdb.get_all_transformers();
    TRANSFORMER* trans;
    for(unsigned int i=0; i!=n; ++i)
    {
        trans = transes[i];
        METER meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        if(trans->is_three_winding_transformer())
        {
            meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
            meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
            meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
        }
    }
}

void DYNAMICS_SIMULATOR::prepare_2t_lcc_hvdc_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_2t_lcc_hvdc_count();
    vector<LCC_HVDC2T*> hvdcs = psdb.get_all_2t_lcc_hvdcs();
    LCC_HVDC2T* hvdc;
    for(unsigned int i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];

        METER meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_voltage_in_pu_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_ac_voltage_in_pu_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_voltage_in_kV_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_dc_voltage_in_kV_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_dc_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_alpha_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_mu_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_gamma_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_mu_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_dc_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_active_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_reactive_power_in_MVar_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_ac_active_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_ac_reactive_power_in_MVar_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_2t_lcc_hvdc_inverter_ac_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meters()
{
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
            append_meter(meter);

            meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, meter_side);
            append_meter(meter);
        }
        unsigned int n_dc_bus = vsc_hvdc->get_dc_bus_count();
        for(unsigned int j=0; j!=n_dc_bus; ++j)
        {
            unsigned int meter_side = vsc_hvdc->get_dc_bus_number(j);
            METER meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
        }
        unsigned int n_dc_line = vsc_hvdc->get_dc_line_count();
        for(unsigned int j=0; j!=n_dc_line; ++j)
        {
            DC_DEVICE_ID dc_line_did = vsc_hvdc->get_dc_line_device_id(j);

            unsigned int ibus = vsc_hvdc->get_dc_line_sending_side_bus(j);
            unsigned int jbus = vsc_hvdc->get_dc_line_receiving_side_bus(j);

            METER meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, ibus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, jbus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, ibus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, jbus);
            append_meter(meter);
        }
    }
}

void DYNAMICS_SIMULATOR::prepare_mt_hvdc_related_meters()
{

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
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_dc_voltage_in_kV_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_dc_power_in_kV_MW_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_current_in_kA_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_firing_angle_in_deg_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_active_power_in_MW_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_ac_reactive_power_in_Mvar_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cc_in_deg_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cea_in_deg_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cv_in_deg_meter(did, lcc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_lcc_converter_reference_idc_in_kA_meter(did, lcc_name);
            append_meter(meter);

        }
        //vsc
        vector<HVSC*> vscs = mtdc->get_all_vsc_converters();
        unsigned int n_vsc = vscs.size();
        for(unsigned int j=0;j!=n_vsc;++j)
        {
            string vsc_name = vscs[j]->get_converter_name();

            METER meter = setter.prepare_mt_hvdc_vsc_converter_dc_current_in_kA_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_dc_voltage_in_kV_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_dc_power_in_kV_MW_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_current_in_kA_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_active_power_in_MW_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_ac_reactive_power_in_Mvar_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_id_in_kA_meter(did, vsc_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_vsc_converter_iq_in_kA_meter(did, vsc_name);
            append_meter(meter);
        }
        //dc bus
        vector<DC_BUS*> dcbuses = mtdc->get_all_dc_buses();
        unsigned int n_dcbus = dcbuses.size();
        for(unsigned int j=0;j!=n_dcbus;++j)
        {
            string dcbus_name = dcbuses[j]->get_name();

            METER meter = setter.prepare_mt_hvdc_dc_bus_voltage_in_kV_meter(did, dcbus_name);
            append_meter(meter);
        }
        //dc line
        vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
        unsigned int n_dclines = dc_lines.size();
        for(unsigned int j=0;j!=n_dclines;++j)
        {
            string line_name = dc_lines[j]->get_dc_line_name();

            METER meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_sending_side_in_kA_meter(did, line_name);
            append_meter(meter);

            meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_receiving_side_in_kA_meter(did, line_name);
            append_meter(meter);

        }
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    EQUIVALENT_DEVICE* edevice;
    for(unsigned int i=0; i!=n; ++i)
    {
        edevice = edevices[i];

        METER meter = setter.prepare_equivalent_device_active_power_generation_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_generation_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_active_power_load_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_load_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_active_power_net_load_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_bus_related_meter(unsigned int bus, string meter_type)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(psdb.is_bus_exist(bus))
    {
        METER_SETTER setter(*toolkit);

        METER meter(*toolkit);

        meter_type = string2upper(meter_type);

        if(meter_type=="VOLTAGE IN PU")
            meter = setter.prepare_bus_voltage_in_pu_meter(bus);
        if(meter_type=="ANGLE IN DEG")
            meter = setter.prepare_bus_angle_in_deg_meter(bus);
        if(meter_type=="ANGLE IN RAD")
            meter = setter.prepare_bus_angle_in_rad_meter(bus);
        if(meter_type=="FREQUENCY DEVIATION IN PU")
            meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus);
        if(meter_type=="FREQUENCY DEVIATION IN HZ")
            meter = setter.prepare_bus_frequency_deviation_in_Hz_meter(bus);
        if(meter_type=="FREQUENCY IN PU")
            meter = setter.prepare_bus_frequency_in_pu_meter(bus);
        if(meter_type=="FREQUENCY IN HZ")
            meter = setter.prepare_bus_frequency_in_Hz_meter(bus);
        if(meter_type=="VOLTAGE IN KV")
            meter = setter.prepare_bus_voltage_in_kV_meter(bus);
        if(meter_type=="ROCOV IN PU/S")
            meter = setter.prepare_bus_ROCOV_in_pu_per_s_meter(bus);
        if(meter_type=="ROCOV IN KV/S")
            meter = setter.prepare_bus_ROCOV_in_kV_per_s_meter(bus);
        if(meter_type=="ROCOF IN PU/S")
            meter = setter.prepare_bus_ROCOF_in_pu_per_s_meter(bus);
        if(meter_type=="ROCOF IN HZ/S")
            meter = setter.prepare_bus_ROCOF_in_Hz_per_s_meter(bus);
        if(meter.is_valid())
            append_meter(meter);
        else
        {
            osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for bus "<<bus;
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. Meter of bus "<<bus<<" cannot be set since the given bus does not exist in the power system database";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_GENERATOR)
    {
        if(psdb.is_generator_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="ROTOR ANGLE IN DEG")
                meter = setter.prepare_generator_rotor_angle_in_deg_meter(did);
            if(meter_type=="ROTOR ANGLE IN RAD")
                meter = setter.prepare_generator_rotor_angle_in_rad_meter(did);
            if(meter_type=="ROTOR SPEED IN PU")
                meter = setter.prepare_generator_rotor_speed_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED IN HZ")
                meter = setter.prepare_generator_rotor_speed_in_Hz_meter(did);
            if(meter_type=="ROTOR SPEED DEVIATION IN PU")
                meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED DEVIATION IN HZ")
                meter = setter.prepare_generator_rotor_speed_deviation_in_Hz_meter(did);
            if(meter_type=="EXCITATION VOLTAGE IN PU")
                meter = setter.prepare_generator_excitation_voltage_in_pu_meter(did);
            if(meter_type=="STABILIZING SIGNAL IN PU")
                meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(did);
            if(meter_type=="TERMINAL VOLTAGE IN PU")
                meter = setter.prepare_generator_terminal_voltage_in_pu_meter(did);
            if(meter_type=="INTERNAL VOLTAGE IN PU")
                meter = setter.prepare_generator_internal_voltage_in_pu_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MECHANICAL POWER IN PU ON MBASE")
                meter = setter.prepare_generator_mechanical_power_in_pu_on_mbase_meter(did);
            if(meter_type=="MECHANICAL POWER IN PU ON SBASE")
                meter = setter.prepare_generator_mechanical_power_in_pu_on_sbase_meter(did);
            if(meter_type=="MECHANICAL POWER REFERENCE IN MW")
                meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(did);
            if(meter_type=="MECHANICAL POWER REFERENCE IN PU ON MBASE")
                meter = setter.prepare_generator_mechanical_power_reference_in_pu_on_mbase_meter(did);
            if(meter_type=="MECHANICAL POWER REFERENCE IN PU ON SBASE")
                meter = setter.prepare_generator_mechanical_power_reference_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_generator_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON MBASE")
                meter = setter.prepare_generator_terminal_current_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON SBASE")
                meter = setter.prepare_generator_terminal_current_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_generator_terminal_active_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
            if(meter_type=="AIRGAP POWER IN MW")
                meter = setter.prepare_generator_airgap_power_in_MW_meter(did);
            if(meter_type=="AIRGAP POWER IN PU ON MBASE")
                meter = setter.prepare_generator_airgap_power_in_pu_on_mbase_meter(did);
            if(meter_type=="AIRGAP POWER IN PU ON SBASE")
                meter = setter.prepare_generator_airgap_power_in_pu_on_sbase_meter(did);
            if(meter_type=="ACCELERATING POWER IN MW")
                meter = setter.prepare_generator_accelerating_power_in_MW_meter(did);
            if(meter_type=="ACCELERATING POWER IN PU ON MBASE")
                meter = setter.prepare_generator_accelerating_power_in_pu_on_mbase_meter(did);
            if(meter_type=="ACCELERATING POWER IN PU ON SBASE")
                meter = setter.prepare_generator_accelerating_power_in_pu_on_sbase_meter(did);
            if(meter_type=="COMPENSATED VOLTAGE IN PU")
                meter = setter.prepare_generator_compensated_voltage_in_pu_meter(did);
            if(meter_type=="SYNC GENERATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_sync_generator_model_internal_variable_meter(did, var_name);
            if(meter_type=="COMPENSATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_compensator_model_internal_variable_meter(did, var_name);
            if(meter_type=="EXCITER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_exciter_model_internal_variable_meter(did, var_name);
            if(meter_type=="STABILIZER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_stabilizer_model_internal_variable_meter(did, var_name);
            if(meter_type=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_turbine_governor_model_internal_variable_meter(did, var_name);
            if(meter_type=="TURBINE LOAD CONTROLLER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_turbine_load_controller_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not GENERATOR when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    METER meter(*toolkit);
    bool successful = basic_meter_preparer::prepare_wt_generator_meter(*toolkit, meter, did);
    if(successful)
    {
        meter_type = string2upper(meter_type);
        var_name = string2upper(var_name);
        meter.set_meter_type(meter_type);
        if(meter_type.find("INTERNAL VARIABLE") != std::string::npos)
            meter.set_internal_variable_name(var_name);
        if(meter.is_valid())
            append_meter(meter);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
}

void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_PV_UNIT)
    {
        if(psdb.is_pv_unit_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON MBASE")
                meter = setter.prepare_pv_unit_terminal_current_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON SBASE")
                meter = setter.prepare_pv_unit_terminal_current_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_pv_unit_terminal_active_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_pv_unit_terminal_active_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_pv_unit_terminal_reactive_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_pv_unit_terminal_reactive_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN MVA")
                meter = setter.prepare_pv_unit_terminal_apparent_power_in_MVA_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN PU ON MBASE")
                meter = setter.prepare_pv_unit_terminal_reactive_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN PU ON SBASE")
                meter = setter.prepare_pv_unit_terminal_apparent_power_in_pu_on_sbase_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="SOLAR IRRADIANCE IN PU ON S0")
                meter = setter.prepare_pv_unit_solar_irradiance_in_pu_on_s0_meter(did);
            if(meter_type=="SOLAR IRRADIANCE IN PU ON SREF")
                meter = setter.prepare_pv_unit_solar_irradiance_in_pu_on_sref_meter(did);
            if(meter_type=="SOLAR IRRADIANCE IN W/M2")
                meter = setter.prepare_pv_unit_solar_irradiance_in_Wpm2_meter(did);
            if(meter_type=="TEMPERATURE IN C DEG")
                meter = setter.prepare_pv_unit_temperature_in_c_deg_meter(did);
            if(meter_type=="MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN MW")
                meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_MW_meter(did);
            if(meter_type=="MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON MBASE")
                meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_pu_on_mbase_meter(did);
            if(meter_type=="MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON SBASE")
                meter = setter.prepare_pv_unit_maximum_available_power_from_irradiance_in_pu_on_sbase_meter(did);
            if(meter_type=="PANEL POWER IN PU ON MBASE")
                meter = setter.prepare_pv_unit_panel_power_in_pu_on_mbase_meter(did);
            if(meter_type=="PANEL POWER IN MW")
                meter = setter.prepare_pv_unit_panel_power_in_MW(did);
            if(meter_type=="PV CONVERTER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_converter_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV PANEL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_panel_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV IRRADIANCE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_irradiance_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV VRT MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_vrt_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV RELAY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_relay_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV TEMPERATURE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_temperature_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not PV UNIT when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_ENERGY_STORAGE)
    {
        if(psdb.is_energy_storage_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_es_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON SBASE")
                meter = setter.prepare_es_terminal_current_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL CURRENT IN PU ON MBASE")
                meter = setter.prepare_es_terminal_current_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_es_terminal_active_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_es_terminal_active_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_es_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_es_terminal_reactive_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_es_terminal_reactive_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_es_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN PU ON MBASE")
                meter = setter.prepare_es_terminal_apparent_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN PU ON SBASE")
                meter = setter.prepare_es_terminal_apparent_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL APPARENT POWER IN MVA")
                meter = setter.prepare_es_terminal_apparent_power_in_MVA_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_es_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_es_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_es_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_es_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_es_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="SOC POWER IN PU ON MBASE")
                meter = setter.prepare_es_soc_power_in_pu_on_mbase_meter(did);
            if(meter_type=="SOC POWER IN PU ON SBASE")
                meter = setter.prepare_es_soc_power_in_pu_on_sbase_meter(did);
            if(meter_type=="SOC POWER IN MW")
                meter = setter.prepare_es_soc_power_in_MW_meter(did);
            if(meter_type=="STATE OF CHARGE IN PU")
                meter = setter.prepare_es_state_of_charge_in_pu_meter(did);
            if(meter_type=="VALVE REFERENCE IN PU")
                meter = setter.prepare_es_valve_reference_in_pu_meter(did);
            if(meter_type=="VALVE IN PU")
                meter = setter.prepare_es_valve_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER REFERENCE IN PU ON MBASE")
                meter = setter.prepare_es_active_power_reference_in_pu_on_mbase_meter(did);
            if(meter_type=="FREQUENCY REGULATION POWER IN PU ON MBASE")
                meter = setter.prepare_es_frequency_regulation_power_in_pu_on_mbase_meter(did);
            if(meter_type=="ROTOR SPEED REFERENCE IN PU")
                meter = setter.prepare_es_rotor_speed_reference_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED IN PU")
                meter = setter.prepare_es_rotor_speed_in_pu_meter(did);
            if(meter_type=="MECHANICAL POWER IN PU ON MBASE")
                meter = setter.prepare_es_mechanical_power_in_pu_on_mbase_meter(did);
            if(meter_type=="ES CONVERTOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_converter_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES SOC MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_soc_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES VRT MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_vrt_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES RELAY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_relay_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES PRIMARY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_primary_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES ROTOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_rotor_model_internal_variable_meter(did, var_name);
            if(meter_type=="ES AUXILIARY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_es_auxiliary_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not ENERGY STORAGE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_sv_source_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_SV_SOURCE)
    {
        if(psdb.is_sv_source_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN PU ON SBASE")
                meter = setter.prepare_svs_terminal_current_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_svs_terminal_reactive_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_svs_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="SVS CONTROL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_svs_control_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not SV SOURCE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_LOAD)
    {
        if(psdb.is_load_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            if(meter_type=="CURRENT IN KA")
                meter = setter.prepare_load_current_in_kA_meter(did);
            if(meter_type=="CURRENT IN PU")
                meter = setter.prepare_load_current_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_load_active_power_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_load_reactive_power_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER IN PU")
                meter = setter.prepare_load_active_power_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER IN PU")
                meter = setter.prepare_load_reactive_power_in_pu_meter(did);
            if(meter_type=="TOTAL SCALE IN PU")
                meter = setter.prepare_load_total_scale_in_pu_meter(did);
            if(meter_type=="MANUALLY SCALE IN PU")
                meter = setter.prepare_load_manually_scale_in_pu_meter(did);
            if(meter_type=="RELAY SHED SCALE IN PU")
                meter = setter.prepare_load_relay_shed_scale_in_pu_meter(did);
            if(meter_type=="LOAD MODEL INTERNAL VARIABLE")
                meter = setter.prepare_load_load_model_internal_variable_meter(did, var_name);
            if(meter_type=="FREQUENCY RELAY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_load_frequency_relay_model_internal_variable_meter(did, var_name);
            if(meter_type=="VOLTAGE RELAY MODEL INTERNAL VARIABLE")
                meter = setter.prepare_load_voltage_relay_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not LOAD when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_ac_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_AC_LINE)
    {
        if(psdb.is_ac_line_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            AC_LINE* line = psdb.get_ac_line(did);
            unsigned int bus = 0;
            if(side=="SENDING" or side=="S")
                bus = line->get_sending_side_bus();
            if(side=="RECEIVING" or side=="R")
                bus = line->get_receiving_side_bus();
            if(bus==0)
                return;

            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_ac_line_active_power_in_MW_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_ac_line_reactive_power_in_MVar_meter(did, bus);
            if(meter_type=="CURRENT IN KA")
                meter = setter.prepare_ac_line_current_in_kA_meter(did, bus);
            if(meter_type=="APPARENT IMPEDANCE IN OHM")
                meter = setter.prepare_ac_line_apparent_impedance_in_Ohm_meter(did, bus);
            if(meter_type=="APPARENT IMPEDANCE ANGLE IN DEG")
                meter = setter.prepare_ac_line_apparent_impedance_angle_in_deg_meter(did, bus);
            if(meter_type=="CURRENT IN PU")
                meter = setter.prepare_ac_line_current_in_pu_meter(did, bus);
            if(meter_type=="ACTIVE POWER IN PU")
                meter = setter.prepare_ac_line_active_power_in_pu_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN PU")
                meter = setter.prepare_ac_line_reactive_power_in_pu_meter(did, bus);
            if(meter_type=="APPARENT IMPEDANCE IN PU")
                meter = setter.prepare_ac_line_apparent_impedance_in_pu_meter(did, bus);
            if(meter_type=="APPARENT IMPEDANCE ANGLE IN RAD")
                meter = setter.prepare_ac_line_apparent_impedance_angle_in_rad_meter(did, bus);
            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not LINE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void DYNAMICS_SIMULATOR::prepare_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_TRANSFORMER)
    {
        if(psdb.is_transformer_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            TRANSFORMER* trans = psdb.get_transformer(did);
            unsigned int bus = 0;
            if(side=="PRIMARY" or side=="P")
                bus = trans->get_winding_bus(PRIMARY_SIDE);
            if(side=="SECONDARY" or side=="S")
                bus = trans->get_winding_bus(SECONDARY_SIDE);
            if(side=="TERTIARY" or side=="T")
                bus = trans->get_winding_bus(TERTIARY_SIDE);
            if(bus==0)
                return;

            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_transformer_active_power_in_MW_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did, bus);
            if(meter_type=="CURRENT IN KA")
                meter = setter.prepare_transformer_current_in_kA_meter(did, bus);
            if(meter_type=="CURRENT IN PU")
                meter = setter.prepare_transformer_current_in_pu_meter(did, bus);
            if(meter_type=="ACTIVE POWER IN PU")
                meter = setter.prepare_transformer_active_power_in_pu_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN PU")
                meter = setter.prepare_transformer_reactive_power_in_pu_meter(did, bus);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not TRANSFORMER when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}


void DYNAMICS_SIMULATOR::prepare_2t_lcc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_LCC_HVDC2T)
    {
        if(psdb.is_2t_lcc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            if(side=="RECTIFIER" or side=="R")
            {
                if(meter_type=="AC VOLTAGE IN PU")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_voltage_in_pu_meter(did);
                if(meter_type=="DC VOLTAGE IN KV")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_voltage_in_kV_meter(did);
                if(meter_type=="DC CURRENT IN KA")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_current_in_kA_meter(did);
                if(meter_type=="ALPHA IN DEG")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_alpha_in_deg_meter(did);
                if(meter_type=="MU IN DEG")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_mu_in_deg_meter(did);
                if(meter_type=="DC POWER IN MW")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_dc_power_in_MW_meter(did);
                if(meter_type=="AC ACTIVE POWER IN MW")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_active_power_in_MW_meter(did);
                if(meter_type=="AC REACTIVE POWER IN MVAR")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_reactive_power_in_MVar_meter(did);
                if(meter_type=="AC CURRENT IN KA")
                    meter = setter.prepare_2t_lcc_hvdc_rectifier_ac_current_in_kA_meter(did);
            }

            if(side=="INVERTER" or side=="I")
            {
                if(meter_type=="AC VOLTAGE IN PU")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_ac_voltage_in_pu_meter(did);
                if(meter_type=="DC VOLTAGE IN KV")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_dc_voltage_in_kV_meter(did);
                if(meter_type=="DC CURRENT IN KA")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_dc_current_in_kA_meter(did);
                if(meter_type=="GAMMA IN DEG")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_gamma_in_deg_meter(did);
                if(meter_type=="MU IN DEG")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_mu_in_deg_meter(did);
                if(meter_type=="DC POWER IN MW")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_dc_power_in_MW_meter(did);
                if(meter_type=="AC ACTIVE POWER IN MW")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_ac_active_power_in_MW_meter(did);
                if(meter_type=="AC REACTIVE POWER IN MVAR")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_ac_reactive_power_in_MVar_meter(did);
                if(meter_type=="AC CURRENT IN KA")
                    meter = setter.prepare_2t_lcc_hvdc_inverter_ac_current_in_kA_meter(did);
            }
             if(meter_type=="2T LCC HVDC MODEL INTERNAL VARIABLE")
                    meter = setter.prepare_2t_lcc_hvdc_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not LCC_HVDC2T when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_VSC_HVDC)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN PU")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, side);
            if(meter_type=="CONVERTER DC POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC ACTIVE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER AC REACTIVE POWER IN MVAR")
                meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, side);
            if(meter_type=="DC BUS VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, side);
            if(meter_type=="VSC LCC_HVDC2T MODEL INTERNAL VARIABLE")
            {
                var_name = string2upper(var_name);
                meter = setter.prepare_vsc_hvdc_model_internal_variable_meter(did, var_name, side);
            }

            if(meter.is_valid())
                append_meter(meter);
            else
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
}

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meter(string vsc_name, string meter_type, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();

    if(vsc_hvdc != NULL)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            if(meter_type=="CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN PU")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, side);
            if(meter_type=="CONVERTER DC POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC ACTIVE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER AC REACTIVE POWER IN MVAR")
                meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, side);
            if(meter_type=="DC BUS VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, side);
            if(meter_type=="VSC LCC_HVDC2T MODEL INTERNAL VARIABLE")
            {
                var_name = string2upper(var_name);
                meter = setter.prepare_vsc_hvdc_model_internal_variable_meter(did, var_name, side);
            }

            if(meter.is_valid())
                append_meter(meter);
            else
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
}


void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_dc_line_meter(const DEVICE_ID& did, string meter_type, const DC_DEVICE_ID dc_did, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_VSC_HVDC)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="DC LINE CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_did, side);
            if(meter_type=="DC LINE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_did, side);

            if(meter.is_valid())
                append_meter(meter);
            else
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
}

void DYNAMICS_SIMULATOR::prepare_mt_hvdc_related_meter(const DEVICE_ID& did, string sub_device_name, string meter_type, string var_name)
 {
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    //MT_HVDC* mtdc = psdb.get_mt_hvdc(did);
    if(did.get_device_type()==STEPS_HYBRID_DC)
    {
        if(psdb.is_mt_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);
            METER meter(*toolkit);
            //device_type = string2upper(device_type);
            meter_type = string2upper(meter_type);

            if(meter_type=="LCC CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_mt_hvdc_lcc_converter_dc_current_in_kA_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_mt_hvdc_lcc_converter_dc_voltage_in_kV_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER DC POWER IN MW")
                meter = setter.prepare_mt_hvdc_lcc_converter_dc_power_in_kV_MW_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_mt_hvdc_lcc_converter_ac_current_in_kA_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER FIRING ANGLE IN DEG")
                meter = setter.prepare_mt_hvdc_lcc_converter_firing_angle_in_deg_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER ACTIVE POWER IN MW")
                meter = setter.prepare_mt_hvdc_lcc_converter_ac_active_power_in_MW_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER REACTIVE POWER IN MVAR")
                meter = setter.prepare_mt_hvdc_lcc_converter_ac_reactive_power_in_Mvar_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER REFERENCE ANGLE OF CC")
                meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cc_in_deg_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER REFERENCE ANGLE OF CEA")
                meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cea_in_deg_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER REFERENCE ANGLE OF CV")
                meter = setter.prepare_mt_hvdc_lcc_converter_reference_angle_of_cv_in_deg_meter(did, sub_device_name);
            if(meter_type=="LCC CONVERTER IDC REF IN KA")
                meter = setter.prepare_mt_hvdc_lcc_converter_reference_idc_in_kA_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_mt_hvdc_vsc_converter_dc_current_in_kA_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_mt_hvdc_vsc_converter_dc_voltage_in_kV_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER DC POWER IN MW")
                meter = setter.prepare_mt_hvdc_vsc_converter_dc_power_in_kV_MW_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_mt_hvdc_vsc_converter_ac_current_in_kA_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER AVTIVE POWER IN MW")
                meter = setter.prepare_mt_hvdc_vsc_converter_ac_active_power_in_MW_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER REACTIVE POWER IN MVAR")
                meter = setter.prepare_mt_hvdc_vsc_converter_ac_reactive_power_in_Mvar_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER ID IN KA")
                meter = setter.prepare_mt_hvdc_vsc_converter_id_in_kA_meter(did, sub_device_name);
            if(meter_type=="VSC CONVERTER IQ IN KA")
                meter = setter.prepare_mt_hvdc_vsc_converter_iq_in_kA_meter(did, sub_device_name);

            if(meter_type=="DC BUS VOLTAGE IN KV")
                meter = setter.prepare_mt_hvdc_dc_bus_voltage_in_kV_meter(did, sub_device_name);

            if(meter_type=="DC LINE IDC ON SENDING SIDE")
                meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_sending_side_in_kA_meter(did, sub_device_name);
            if(meter_type=="DC LINE IDC ON RECEIVING SIDE")
                meter = setter.prepare_mt_hvdc_dc_line_dc_current_on_receiving_side_in_kA_meter(did, sub_device_name);
            if(meter_type=="HLCC CONVERTER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_mt_hvdc_model_hlcc_converter_model_internal_variable_meter(did, var_name);
            if(meter_type=="HVSC CONVERTER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_mt_hvdc_model_hvsc_converter_model_internal_variable_meter(did, var_name);
            if(meter_type=="DC NETWORK MODEL INTERNAL VARIABLE")
                meter = setter.prepare_mt_hvdc_model_dc_network_model_internal_variable_meter(did, var_name);


            if(meter.is_valid())
            {
                append_meter(meter);
            }
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not MT HVDC when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
 }

void DYNAMICS_SIMULATOR::prepare_mt_hvdc_related_meter(string mtdc_name, string device_name, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    MT_HVDC* mtdc = psdb.get_mt_hvdc(mtdc_name);
    if(mtdc!=NULL)
    {
        DEVICE_ID did = mtdc->get_device_id();
        prepare_mt_hvdc_related_meter(did,device_name,meter_type,var_name);
    }
    else
    {
        osstream<<"Warning. Meter of MTDC project:"<<mtdc_name<<" cannot be set since the given project name does not exist in the power system database";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_EQUIVALENT_DEVICE)
    {
        if(psdb.is_equivalent_device_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            if(meter_type=="VOLTAGE SOURCE VOLTAGE IN PU")
                meter = setter.prepare_equivalent_device_voltage_source_voltage_in_pu_meter(did);
            if(meter_type=="VOLTAGE SOURCE VOLTAGE ANGLE IN DEG")
                meter = setter.prepare_equivalent_device_voltage_source_voltage_angle_in_deg_meter(did);
            if(meter_type=="VOLTAGE SOURCE RESISTANCE IN PU")
                meter = setter.prepare_equivalent_device_voltage_source_resistance_in_pu_meter(did);
            if(meter_type=="VOLTAGE SOURCE REACTANCE IN PU")
                meter = setter.prepare_equivalent_device_voltage_source_reactance_in_pu_meter(did);
            if(meter_type=="ACTIVE CONSTANT POWER LOAD IN MW")
                meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(did);
            if(meter_type=="REACTIVE CONSTANT POWER LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE CONSTANT CURRENT LOAD IN MW")
                meter = setter.prepare_equivalent_device_nominal_active_constant_current_load_in_MW_meter(did);
            if(meter_type=="REACTIVE CONSTANT CURRENT LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_current_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE CONSTANT IMPEDANCE LOAD IN MW")
                meter = setter.prepare_equivalent_device_nominal_active_constant_impedance_load_in_MW_meter(did);
            if(meter_type=="REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER GENERATION IN MW")
                meter = setter.prepare_equivalent_device_active_power_generation_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER GENERATION IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_generation_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER LOAD IN MW")
                meter = setter.prepare_equivalent_device_active_power_load_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER NET LOAD IN MW")
                meter = setter.prepare_equivalent_device_active_power_net_load_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER NET LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE CONSTANT POWER LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_pu_meter(did);
            if(meter_type=="REACTIVE CONSTANT POWER LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_pu_meter(did);
            if(meter_type=="ACTIVE CONSTANT CURRENT LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_active_constant_current_load_in_pu_meter(did);
            if(meter_type=="REACTIVE CONSTANT CURRENT LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_current_load_in_pu_meter(did);
            if(meter_type=="ACTIVE CONSTANT IMPEDANCE LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_active_constant_impedance_load_in_pu_meter(did);
            if(meter_type=="REACTIVE CONSTANT IMPEDANCE LOAD IN PU")
                meter = setter.prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER GENERATION IN PU")
                meter = setter.prepare_equivalent_device_active_power_generation_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER GENERATION IN PU")
                meter = setter.prepare_equivalent_device_reactive_power_generation_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER LOAD IN PU")
                meter = setter.prepare_equivalent_device_active_power_load_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER LOAD IN PU")
                meter = setter.prepare_equivalent_device_reactive_power_load_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER NET LOAD IN PU")
                meter = setter.prepare_equivalent_device_active_power_net_load_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER NET LOAD IN PU")
                meter = setter.prepare_equivalent_device_reactive_power_net_load_in_pu_meter(did);

            if(meter.is_valid())
                append_meter(meter);
            else
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
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not EQUIVALENT DEVICE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}
