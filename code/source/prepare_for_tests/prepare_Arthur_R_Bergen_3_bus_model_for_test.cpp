#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

void prepare_Arthur_R_Bergen_3_bus_model()
{
    prepare_Arthur_R_Bergen_3_bus_model_buses();
    prepare_Arthur_R_Bergen_3_bus_model_generators();
    prepare_Arthur_R_Bergen_3_bus_model_loads();
    prepare_Arthur_R_Bergen_3_bus_model_lines();
    prepare_Arthur_R_Bergen_3_bus_model_transformers();
    prepare_Arthur_R_Bergen_3_bus_model_areas();
    prepare_Arthur_R_Bergen_3_bus_model_zones();
    prepare_Arthur_R_Bergen_3_bus_model_owners();
}

void prepare_Arthur_R_Bergen_3_bus_model_buses()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(10);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_toolkit(default_toolkit);

    bus.set_bus_number(1);
    bus.set_bus_name("BUS_1");
    bus.set_base_voltage_in_kV(100.0);
    bus.set_base_frequency_in_Hz(60.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(SLACK_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(0.0);

    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("BUS_2");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(0.0);

    psdb.append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_name("BUS_3");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(0.0);

    psdb.append_bus(bus);
}

void prepare_Arthur_R_Bergen_3_bus_model_generators()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR generator;
    generator.set_toolkit(default_toolkit);

    complex<double> gen_z(0.0);

    generator.set_generator_bus(1);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(0.0);
    generator.set_q_generation_in_MVar(0.0);
    generator.set_p_max_in_MW(300.0);
    generator.set_p_min_in_MW(50.0);
    generator.set_q_max_in_MVar(999.0);
    generator.set_q_min_in_MVar(-999.0);
    generator.set_mbase_in_MVA(100.0);
    gen_z = complex<double>(0.0, 0.01);
    generator.set_generator_impedance_in_pu(gen_z);
    generator.set_voltage_to_regulate_in_pu(1.0);
    generator.set_bus_to_regulate(0);

    psdb.append_generator(generator);


    generator.set_generator_bus(2);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(66.61);
    generator.set_q_generation_in_MVar(0.0);
    generator.set_voltage_to_regulate_in_pu(1.05);
    generator.set_bus_to_regulate(0);

    psdb.append_generator(generator);
}

void prepare_Arthur_R_Bergen_3_bus_model_loads()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LOAD load;
    load.set_toolkit(default_toolkit);

    complex<double> s(0.0,0.0);

    load.set_load_bus(3);
    load.set_identifier("1");
    load.set_status(true);
    s = complex<double>(286.53, 122.44);
    load.set_nominal_constant_power_load_in_MVA(s);
    s = complex<double>(0.0, 0.0);
    load.set_nominal_constant_current_load_in_MVA(s);
    load.set_nominal_constant_impedance_load_in_MVA(s);

    psdb.append_load(load);
}

void prepare_Arthur_R_Bergen_3_bus_model_lines()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LINE line;
    line.set_toolkit(default_toolkit);

    complex<double> z(0.0), y(0.0);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.00, 0.1);
    y = complex<double>(0.0, 0.01);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(3);
    psdb.append_line(line);

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(3);
    psdb.append_line(line);
}

void prepare_Arthur_R_Bergen_3_bus_model_transformers()
{
    //POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    //TRANSFORMER trans;
    //psdb.append_transformer(trans);
}

void prepare_Arthur_R_Bergen_3_bus_model_areas()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    AREA area;
    area.set_toolkit(default_toolkit);

    area.set_area_number(1);
    area.set_area_swing_bus(0);
    area.set_expected_power_leaving_area_in_MW(0.0);
    area.set_area_power_mismatch_tolerance_in_MW(10.0);

    psdb.append_area(area);
}

void prepare_Arthur_R_Bergen_3_bus_model_zones()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ZONE zone;
    zone.set_toolkit(default_toolkit);

    zone.set_zone_number(1);

    psdb.append_zone(zone);
}

void prepare_Arthur_R_Bergen_3_bus_model_owners()
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    OWNER owner;
    owner.set_toolkit(default_toolkit);

    owner.set_owner_number(1);

    psdb.append_owner(owner);
}

