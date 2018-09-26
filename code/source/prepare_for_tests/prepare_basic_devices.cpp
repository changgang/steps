#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/model/all_supported_models.h"
#include "header/basic/utility.h"

void prepare_basic_buses()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    psdb.clear_database();

    psdb.set_allowed_max_bus_number(10);

    BUS bus;
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
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    GENERATOR generator;
    GENCLS gen_model;
    IEEET1 ex_model;
    COMP comp_model;
    IEE2ST pss_model;
    IEEEG1 turbine_model;

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

    DEVICE_ID did = get_generator_device_id(1, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &ex_model);
    psdb.append_dynamic_model(did, &comp_model);
    psdb.append_dynamic_model(did, &pss_model);
    psdb.append_dynamic_model(did, &turbine_model);

    generator.set_generator_bus(2);
    psdb.append_generator(generator);
    did = get_generator_device_id(2, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &ex_model);
    psdb.append_dynamic_model(did, &comp_model);
    psdb.append_dynamic_model(did, &pss_model);
    psdb.append_dynamic_model(did, &turbine_model);

    generator.set_generator_bus(3);
    psdb.append_generator(generator);
    did = get_generator_device_id(3, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &ex_model);
    psdb.append_dynamic_model(did, &comp_model);
    psdb.append_dynamic_model(did, &pss_model);
    psdb.append_dynamic_model(did, &turbine_model);

    generator.set_generator_bus(1);
    generator.set_identifier("2#");
    psdb.append_generator(generator);
    did = get_generator_device_id(1, "2#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &ex_model);
    psdb.append_dynamic_model(did, &comp_model);
    psdb.append_dynamic_model(did, &pss_model);
    psdb.append_dynamic_model(did, &turbine_model);

    generator.set_identifier("3#");
    psdb.append_generator(generator);
    did = get_generator_device_id(1, "3#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &ex_model);
    psdb.append_dynamic_model(did, &comp_model);
    psdb.append_dynamic_model(did, &pss_model);
    psdb.append_dynamic_model(did, &turbine_model);
}

void prepare_basic_wt_generators()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    WT_GENERATOR wt_generator;
    WT3G2 gen_model;
    AERD0 aerd_model;
    WT3T0 turbine_model;
    WT3E0 elec_model;
    WT3P0 pitch_model;
    FILEWIND wind_model;

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
    DEVICE_ID did = get_wt_generator_device_id(1, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &aerd_model);
    psdb.append_dynamic_model(did, &turbine_model);
    psdb.append_dynamic_model(did, &elec_model);
    psdb.append_dynamic_model(did, &pitch_model);
    psdb.append_dynamic_model(did, &wind_model);

    wt_generator.set_generator_bus(2);
    psdb.append_wt_generator(wt_generator);
    did = get_wt_generator_device_id(2, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &aerd_model);
    psdb.append_dynamic_model(did, &turbine_model);
    psdb.append_dynamic_model(did, &elec_model);
    psdb.append_dynamic_model(did, &pitch_model);
    psdb.append_dynamic_model(did, &wind_model);

    wt_generator.set_generator_bus(3);
    psdb.append_wt_generator(wt_generator);
    did = get_wt_generator_device_id(3, "1#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &aerd_model);
    psdb.append_dynamic_model(did, &turbine_model);
    psdb.append_dynamic_model(did, &elec_model);
    psdb.append_dynamic_model(did, &pitch_model);
    psdb.append_dynamic_model(did, &wind_model);

    wt_generator.set_generator_bus(1);
    wt_generator.set_identifier("2#");
    psdb.append_wt_generator(wt_generator);
    did = get_wt_generator_device_id(1, "2#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &aerd_model);
    psdb.append_dynamic_model(did, &turbine_model);
    psdb.append_dynamic_model(did, &elec_model);
    psdb.append_dynamic_model(did, &pitch_model);
    psdb.append_dynamic_model(did, &wind_model);

    wt_generator.set_identifier("3#");
    psdb.append_wt_generator(wt_generator);
    did = get_wt_generator_device_id(1, "3#");
    psdb.append_dynamic_model(did, &gen_model);
    psdb.append_dynamic_model(did, &aerd_model);
    psdb.append_dynamic_model(did, &turbine_model);
    psdb.append_dynamic_model(did, &elec_model);
    psdb.append_dynamic_model(did, &pitch_model);
    psdb.append_dynamic_model(did, &wind_model);
}

void prepare_basic_pv_units()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    PV_UNIT pv_unit;

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
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    ENERGY_STORAGE estorage;
    ESTR0 model;

    estorage.set_energy_storage_bus(1);
    estorage.set_identifier("#1");
    psdb.append_energy_storage(estorage);
    psdb.append_dynamic_model(get_energy_storage_device_id(1, "#1"), &model);

    estorage.set_energy_storage_bus(2);
    estorage.set_identifier("#2");
    psdb.append_energy_storage(estorage);
    psdb.append_dynamic_model(get_energy_storage_device_id(2, "#2"), &model);

    estorage.set_energy_storage_bus(3);
    estorage.set_identifier("#3");
    psdb.append_energy_storage(estorage);
    psdb.append_dynamic_model(get_energy_storage_device_id(3, "#3"), &model);
}

void prepare_basic_loads()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    LOAD load;
    IEEL load_model;
    load_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);
    UFLS freq_model;
    UVLS volt_model;

    load.set_load_bus(1);
    load.set_identifier("1#");
    psdb.append_load(load);
    psdb.append_dynamic_model(get_load_device_id(1,"1#"), &load_model);
    psdb.append_dynamic_model(get_load_device_id(1,"1#"), &freq_model);
    psdb.append_dynamic_model(get_load_device_id(1,"1#"), &volt_model);

    load.set_load_bus(2);
    psdb.append_load(load);
    psdb.append_dynamic_model(get_load_device_id(2,"1#"), &load_model);
    psdb.append_dynamic_model(get_load_device_id(2,"1#"), &freq_model);
    psdb.append_dynamic_model(get_load_device_id(2,"1#"), &volt_model);

    load.set_load_bus(3);
    psdb.append_load(load);
    psdb.append_dynamic_model(get_load_device_id(3,"1#"), &load_model);
    psdb.append_dynamic_model(get_load_device_id(3,"1#"), &freq_model);
    psdb.append_dynamic_model(get_load_device_id(3,"1#"), &volt_model);

    load.set_load_bus(1);
    load.set_identifier("2#");
    psdb.append_load(load);
    psdb.append_dynamic_model(get_load_device_id(1,"2#"), &load_model);
    psdb.append_dynamic_model(get_load_device_id(1,"2#"), &freq_model);
    psdb.append_dynamic_model(get_load_device_id(1,"2#"), &volt_model);

    load.set_identifier("3#");
    psdb.append_load(load);
    psdb.append_dynamic_model(get_load_device_id(1,"3#"), &load_model);
    psdb.append_dynamic_model(get_load_device_id(1,"3#"), &freq_model);
    psdb.append_dynamic_model(get_load_device_id(1,"3#"), &volt_model);
}

void prepare_basic_lines()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    LINE line;

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
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    TRANSFORMER transformer;

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
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    FIXED_SHUNT shunt;

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
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    HVDC hvdc;
    CDC4T model;

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("P");
    psdb.append_hvdc(hvdc);
    psdb.append_dynamic_model(get_hvdc_device_id(1, 2, "P"), &model);

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("N");
    psdb.append_hvdc(hvdc);
    psdb.append_dynamic_model(get_hvdc_device_id(1, 2, "N"), &model);

    hvdc.set_converter_bus(RECTIFIER, 2);
    hvdc.set_converter_bus(INVERTER, 3);
    hvdc.set_identifier("P");
    psdb.append_hvdc(hvdc);
    psdb.append_dynamic_model(get_hvdc_device_id(2, 3, "P"), &model);
}

void prepare_basic_equivalent_devices()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(psdb.get_bus_count()==0)
        prepare_basic_buses();

    EQUIVALENT_DEVICE edevice;

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
