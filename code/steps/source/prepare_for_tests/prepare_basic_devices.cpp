#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/model/all_supported_models.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;

void prepare_basic_buses()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.clear();

    psdb.set_allowed_max_bus_number(10);

    BUS bus(default_toolkit);

    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(123.0);

    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    bus.set_base_voltage_in_kV(120.0);

    psdb.append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_name("bus 3");
    bus.set_base_voltage_in_kV(120.0);

    psdb.append_bus(bus);
}

void prepare_basic_generators()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    GENERATOR generator(default_toolkit);
    generator.set_generator_bus(1);
    generator.set_identifier("1#");
    generator.set_status(true);
    generator.set_p_generation_in_MW(100.0);
    generator.set_q_generation_in_MVar(50.0);
    generator.set_p_max_in_MW(200.0);
    generator.set_p_min_in_MW(50.0);
    generator.set_q_max_in_MVar(100.0);
    generator.set_q_min_in_MVar(-100.0);
    psdb.append_generator(generator);

    DEVICE_ID did = generator.get_device_id();

    GENCLS gen_model(default_toolkit);
    IEEET1 ex_model(default_toolkit);
    COMP comp_model(default_toolkit);
    IEE2ST pss_model(default_toolkit);
    IEEEG1 turbine_model(default_toolkit);
    LCFB1 turbine_load_controller_model(default_toolkit);

    gen_model.set_device_id(did);
    ex_model.set_device_id(did);
    comp_model.set_device_id(did);
    pss_model.set_device_id(did);
    turbine_model.set_device_id(did);
    turbine_load_controller_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&ex_model);
    dmdb.add_model(&comp_model);
    dmdb.add_model(&pss_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&turbine_load_controller_model);

    generator.set_generator_bus(2);
    psdb.append_generator(generator);
    did = generator.get_device_id();

    gen_model.set_device_id(did);
    ex_model.set_device_id(did);
    comp_model.set_device_id(did);
    pss_model.set_device_id(did);
    turbine_model.set_device_id(did);
    turbine_load_controller_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&ex_model);
    dmdb.add_model(&comp_model);
    dmdb.add_model(&pss_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&turbine_load_controller_model);

    generator.set_generator_bus(3);
    psdb.append_generator(generator);
    did = generator.get_device_id();

    gen_model.set_device_id(did);
    ex_model.set_device_id(did);
    comp_model.set_device_id(did);
    pss_model.set_device_id(did);
    turbine_model.set_device_id(did);
    turbine_load_controller_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&ex_model);
    dmdb.add_model(&comp_model);
    dmdb.add_model(&pss_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&turbine_load_controller_model);

    generator.set_generator_bus(1);
    generator.set_identifier("2#");
    psdb.append_generator(generator);
    did = generator.get_device_id();

    gen_model.set_device_id(did);
    ex_model.set_device_id(did);
    comp_model.set_device_id(did);
    pss_model.set_device_id(did);
    turbine_model.set_device_id(did);
    turbine_load_controller_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&ex_model);
    dmdb.add_model(&comp_model);
    dmdb.add_model(&pss_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&turbine_load_controller_model);

    generator.set_identifier("3#");
    psdb.append_generator(generator);
    did = generator.get_device_id();

    gen_model.set_device_id(did);
    ex_model.set_device_id(did);
    comp_model.set_device_id(did);
    pss_model.set_device_id(did);
    turbine_model.set_device_id(did);
    turbine_load_controller_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&ex_model);
    dmdb.add_model(&comp_model);
    dmdb.add_model(&pss_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&turbine_load_controller_model);
}

void prepare_basic_wt_generators()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    WT_GENERATOR wt_generator(default_toolkit);
    wt_generator.set_generator_bus(1);
    wt_generator.set_identifier("1#");

    wt_generator.set_status(true);
    wt_generator.set_p_generation_in_MW(100.0);
    wt_generator.set_q_generation_in_MVar(50.0);
    wt_generator.set_p_max_in_MW(200.0);
    wt_generator.set_p_min_in_MW(50.0);
    wt_generator.set_q_max_in_MVar(100.0);
    wt_generator.set_q_min_in_MVar(-100.0);

    psdb.append_wt_generator(wt_generator);
    DEVICE_ID did = wt_generator.get_device_id();

    WT3G0 gen_model(default_toolkit);
    AERD0 aerd_model(default_toolkit);
    WT3T0 turbine_model(default_toolkit);
    WT3E0 elec_model(default_toolkit);
    WT3P0 pitch_model(default_toolkit);
    FILEWIND wind_model(default_toolkit);

    gen_model.set_device_id(did);
    aerd_model.set_device_id(did);
    turbine_model.set_device_id(did);
    elec_model.set_device_id(did);
    pitch_model.set_device_id(did);
    wind_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&aerd_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&elec_model);
    dmdb.add_model(&pitch_model);
    dmdb.add_model(&wind_model);

    wt_generator.set_generator_bus(2);
    psdb.append_wt_generator(wt_generator);

    did = wt_generator.get_device_id();

    gen_model.set_device_id(did);
    aerd_model.set_device_id(did);
    turbine_model.set_device_id(did);
    elec_model.set_device_id(did);
    pitch_model.set_device_id(did);
    wind_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&aerd_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&elec_model);
    dmdb.add_model(&pitch_model);
    dmdb.add_model(&wind_model);

    wt_generator.set_generator_bus(3);
    psdb.append_wt_generator(wt_generator);
    did = wt_generator.get_device_id();

    gen_model.set_device_id(did);
    aerd_model.set_device_id(did);
    turbine_model.set_device_id(did);
    elec_model.set_device_id(did);
    pitch_model.set_device_id(did);
    wind_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&aerd_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&elec_model);
    dmdb.add_model(&pitch_model);
    dmdb.add_model(&wind_model);

    wt_generator.set_generator_bus(1);
    wt_generator.set_identifier("2#");
    psdb.append_wt_generator(wt_generator);
    did = wt_generator.get_device_id();

    gen_model.set_device_id(did);
    aerd_model.set_device_id(did);
    turbine_model.set_device_id(did);
    elec_model.set_device_id(did);
    pitch_model.set_device_id(did);
    wind_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&aerd_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&elec_model);
    dmdb.add_model(&pitch_model);
    dmdb.add_model(&wind_model);

    wt_generator.set_identifier("3#");
    psdb.append_wt_generator(wt_generator);
    did = wt_generator.get_device_id();

    gen_model.set_device_id(did);
    aerd_model.set_device_id(did);
    turbine_model.set_device_id(did);
    elec_model.set_device_id(did);
    pitch_model.set_device_id(did);
    wind_model.set_device_id(did);

    dmdb.add_model(&gen_model);
    dmdb.add_model(&aerd_model);
    dmdb.add_model(&turbine_model);
    dmdb.add_model(&elec_model);
    dmdb.add_model(&pitch_model);
    dmdb.add_model(&wind_model);
}

void prepare_basic_pv_units()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    PV_UNIT pv_unit(default_toolkit);

    pv_unit.set_unit_bus(1);
    pv_unit.set_identifier("1#");

    pv_unit.set_status(true);
    pv_unit.set_p_generation_in_MW(100.0);
    pv_unit.set_q_generation_in_MVar(50.0);
    pv_unit.set_p_max_in_MW(200.0);
    pv_unit.set_p_min_in_MW(50.0);
    pv_unit.set_q_max_in_MVar(100.0);
    pv_unit.set_q_min_in_MVar(-100.0);

    psdb.append_pv_unit(pv_unit);

    pv_unit.set_unit_bus(2);
    psdb.append_pv_unit(pv_unit);

    pv_unit.set_unit_bus(3);
    psdb.append_pv_unit(pv_unit);

    pv_unit.set_unit_bus(1);
    pv_unit.set_identifier("2#");
    psdb.append_pv_unit(pv_unit);

    pv_unit.set_identifier("3#");
    psdb.append_pv_unit(pv_unit);
}

void prepare_basic_energy_strorages()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    ENERGY_STORAGE estorage(default_toolkit);
    estorage.set_energy_storage_bus(1);
    estorage.set_identifier("#1");
    psdb.append_energy_storage(estorage);

    DEVICE_ID did = estorage.get_device_id();

    ESTR0 model(default_toolkit);
    model.set_device_id(did);

    dmdb.add_model(&model);

    estorage.set_energy_storage_bus(2);
    estorage.set_identifier("#2");
    psdb.append_energy_storage(estorage);
    did = estorage.get_device_id();
    model.set_device_id(did);
    dmdb.add_model(&model);

    estorage.set_energy_storage_bus(3);
    estorage.set_identifier("#3");
    psdb.append_energy_storage(estorage);
    did = estorage.get_device_id();
    model.set_device_id(did);
    dmdb.add_model(&model);
}

void prepare_basic_loads()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    LOAD load(default_toolkit);
    load.set_load_bus(1);
    load.set_identifier("1#");
    psdb.append_load(load);

    DEVICE_ID did = load.get_device_id();

    IEEL load_model(default_toolkit);
    load_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    UFLS freq_model(default_toolkit);
    freq_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    UVLS volt_model(default_toolkit);
    volt_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    load_model.set_device_id(did);
    freq_model.set_device_id(did);
    volt_model.set_device_id(did);

    dmdb.add_model(&load_model);
    dmdb.add_model(&freq_model);
    dmdb.add_model(&volt_model);

    load.set_load_bus(2);
    psdb.append_load(load);

    did = load.get_device_id();
    load_model.set_device_id(did);
    freq_model.set_device_id(did);
    volt_model.set_device_id(did);

    dmdb.add_model(&load_model);
    dmdb.add_model(&freq_model);
    dmdb.add_model(&volt_model);

    load.set_load_bus(3);
    psdb.append_load(load);

    did = load.get_device_id();
    load_model.set_device_id(did);
    freq_model.set_device_id(did);
    volt_model.set_device_id(did);

    dmdb.add_model(&load_model);
    dmdb.add_model(&freq_model);
    dmdb.add_model(&volt_model);

    load.set_load_bus(1);
    load.set_identifier("2#");
    psdb.append_load(load);

    did = load.get_device_id();
    load_model.set_device_id(did);
    freq_model.set_device_id(did);
    volt_model.set_device_id(did);

    dmdb.add_model(&load_model);
    dmdb.add_model(&freq_model);
    dmdb.add_model(&volt_model);

    load.set_identifier("3#");
    psdb.append_load(load);

    did = load.get_device_id();
    load_model.set_device_id(did);
    freq_model.set_device_id(did);
    volt_model.set_device_id(did);

    dmdb.add_model(&load_model);
    dmdb.add_model(&freq_model);
    dmdb.add_model(&volt_model);
}

void prepare_basic_lines()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    LINE line(default_toolkit);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");
    psdb.append_line(line);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("2#");
    psdb.append_line(line);

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(3);
    line.set_identifier("1#");
    psdb.append_line(line);
}
void prepare_basic_transformers()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    TRANSFORMER transformer(default_toolkit);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 0);
    transformer.set_identifier("1#");
    psdb.append_transformer(transformer);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 3);
    transformer.set_identifier("1#");
    psdb.append_transformer(transformer);

    transformer.set_winding_bus(PRIMARY_SIDE, 2);
    transformer.set_winding_bus(SECONDARY_SIDE, 3);
    transformer.set_winding_bus(TERTIARY_SIDE, 0);
    transformer.set_identifier("1#");
    psdb.append_transformer(transformer);
}

void prepare_basic_fixed_shunts()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    FIXED_SHUNT shunt(default_toolkit);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("1#");
    psdb.append_fixed_shunt(shunt);

    shunt.set_shunt_bus(2);
    psdb.append_fixed_shunt(shunt);

    shunt.set_shunt_bus(3);
    psdb.append_fixed_shunt(shunt);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("2#");
    psdb.append_fixed_shunt(shunt);

    shunt.set_identifier("3#");
    psdb.append_fixed_shunt(shunt);
}

void prepare_basic_hvdcs()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    HVDC hvdc(default_toolkit);
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("P");
    psdb.append_hvdc(hvdc);

    DEVICE_ID did = hvdc.get_device_id();

    CDC4T model(default_toolkit);
    model.set_device_id(did);

    dmdb.add_model(&model);

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("N");
    psdb.append_hvdc(hvdc);
    did = hvdc.get_device_id();
    model.set_device_id(did);
    dmdb.add_model(&model);

    hvdc.set_converter_bus(RECTIFIER, 2);
    hvdc.set_converter_bus(INVERTER, 3);
    hvdc.set_identifier("P");
    psdb.append_hvdc(hvdc);
    did = hvdc.get_device_id();
    model.set_device_id(did);
    dmdb.add_model(&model);
}

void prepare_basic_vsc_hvdcs()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();
    VSC_HVDC vsc_hvdc(default_toolkit);
    vsc_hvdc.set_converter_count(3);
    vsc_hvdc.set_dc_bus_count(3);
    vsc_hvdc.set_dc_line_count(3);
    vsc_hvdc.set_dc_network_base_voltage_in_kV(200.0);
    vsc_hvdc.set_identifier("#1");
    psdb.append_vsc_hvdc(vsc_hvdc);

    DEVICE_ID did=vsc_hvdc.get_device_id();
    VSCHVDC1 model(default_toolkit);
    model.set_device_id(did);
    dmdb.add_model(&model);

}

void prepare_basic_equivalent_devices()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    EQUIVALENT_DEVICE edevice(default_toolkit);

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("E1");
    psdb.append_equivalent_device(edevice);

    edevice.set_equivalent_device_bus(2);
    edevice.set_identifier("E2");
    psdb.append_equivalent_device(edevice);

    edevice.set_equivalent_device_bus(3);
    edevice.set_identifier("E3");
    psdb.append_equivalent_device(edevice);
}
