#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/basic/utility.h"

void prepare_basic_buses(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic buse."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    db->clear_database();

    db->set_allowed_max_bus_number(10);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_name("bus 1");
    bus.set_base_voltage_in_kV(123.0);

    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("bus 2");
    bus.set_base_voltage_in_kV(120.0);

    db->append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_name("bus 3");
    bus.set_base_voltage_in_kV(120.0);

    db->append_bus(bus);
}

void prepare_basic_generators(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic generators."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    GENERATOR generator(db);

    generator.set_generator_bus(1);
    generator.set_identifier("1#");
    generator.set_status(true);
    generator.set_p_generation_in_MW(100.0);
    generator.set_q_generation_in_MVar(50.0);
    generator.set_p_max_in_MW(200.0);
    generator.set_p_min_in_MW(50.0);
    generator.set_q_max_in_MVar(100.0);
    generator.set_q_min_in_MVar(-100.0);
    db->append_generator(generator);

    generator.set_generator_bus(2);
    db->append_generator(generator);

    generator.set_generator_bus(3);
    db->append_generator(generator);

    generator.set_generator_bus(1);
    generator.set_identifier("2#");
    db->append_generator(generator);

    generator.set_identifier("3#");
    db->append_generator(generator);
}
void prepare_basic_wt_generators(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic power-electronic sources."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    WT_GENERATOR wt_generator(db);

    wt_generator.set_source_bus(1);
    wt_generator.set_identifier("1#");

    wt_generator.set_status(true);
    wt_generator.set_p_generation_in_MW(100.0);
    wt_generator.set_q_generation_in_MVar(50.0);
    wt_generator.set_p_max_in_MW(200.0);
    wt_generator.set_p_min_in_MW(50.0);
    wt_generator.set_q_max_in_MVar(100.0);
    wt_generator.set_q_min_in_MVar(-100.0);

    db->append_wt_generator(wt_generator);

    wt_generator.set_source_bus(2);
    db->append_wt_generator(wt_generator);

    wt_generator.set_source_bus(3);
    db->append_wt_generator(wt_generator);

    wt_generator.set_source_bus(1);
    wt_generator.set_identifier("2#");
    db->append_wt_generator(wt_generator);

    wt_generator.set_identifier("3#");
    db->append_wt_generator(wt_generator);
}

void prepare_basic_loads(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic loads."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    LOAD load(db);

    load.set_load_bus(1);
    load.set_identifier("1#");
    db->append_load(load);

    load.set_load_bus(2);
    db->append_load(load);

    load.set_load_bus(3);
    db->append_load(load);

    load.set_load_bus(1);
    load.set_identifier("2#");
    db->append_load(load);

    load.set_identifier("3#");
    db->append_load(load);
}

void prepare_basic_lines(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic lines."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    LINE line(db);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1#");
    db->append_line(line);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("2#");
    db->append_line(line);

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(3);
    line.set_identifier("1#");
    db->append_line(line);
}
void prepare_basic_transformers(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic transformers."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    TRANSFORMER transformer(db);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 0);
    transformer.set_identifier("1#");
    db->append_transformer(transformer);

    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_winding_bus(TERTIARY_SIDE, 3);
    transformer.set_identifier("1#");
    db->append_transformer(transformer);

    transformer.set_winding_bus(PRIMARY_SIDE, 2);
    transformer.set_winding_bus(SECONDARY_SIDE, 3);
    transformer.set_winding_bus(TERTIARY_SIDE, 0);
    transformer.set_identifier("1#");
    db->append_transformer(transformer);
}

void prepare_basic_fixed_shunts(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic fixed shunts."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    FIXED_SHUNT shunt(db);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("1#");
    db->append_fixed_shunt(shunt);

    shunt.set_shunt_bus(2);
    db->append_fixed_shunt(shunt);

    shunt.set_shunt_bus(3);
    db->append_fixed_shunt(shunt);

    shunt.set_shunt_bus(1);
    shunt.set_identifier("2#");
    db->append_fixed_shunt(shunt);

    shunt.set_identifier("3#");
    db->append_fixed_shunt(shunt);
}

void prepare_basic_hvdcs(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic hvdcs."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    HVDC hvdc(db);

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("P");
    db->append_hvdc(hvdc);

    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("N");
    db->append_hvdc(hvdc);

    hvdc.set_converter_bus(RECTIFIER, 2);
    hvdc.set_converter_bus(INVERTER, 3);
    hvdc.set_identifier("P");
    db->append_hvdc(hvdc);
}

void prepare_basic_equivalent_devices(POWER_SYSTEM_DATABASE* db)
{
    ostringstream sstream;
    if(db==NULL)
    {
        sstream<<"NULL power system database is provided for preparing basic equivalent devices."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(db->get_bus_count()==0)
        prepare_basic_buses(db);

    EQUIVALENT_DEVICE edevice(db);

    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("E1");
    db->append_equivalent_device(edevice);

    edevice.set_equivalent_device_bus(2);
    edevice.set_identifier("E2");
    db->append_equivalent_device(edevice);

    edevice.set_equivalent_device_bus(3);
    edevice.set_identifier("E3");
    db->append_equivalent_device(edevice);
}
