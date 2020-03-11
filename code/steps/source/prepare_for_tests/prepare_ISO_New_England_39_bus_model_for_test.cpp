#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

void prepare_ISO_New_England_39_bus_model()
{
    prepare_ISO_New_England_39_bus_model_buses();
    prepare_ISO_New_England_39_bus_model_generators();
    prepare_ISO_New_England_39_bus_model_loads();
    prepare_ISO_New_England_39_bus_model_lines();
    prepare_ISO_New_England_39_bus_model_transformers();
    prepare_ISO_New_England_39_bus_model_areas();
    prepare_ISO_New_England_39_bus_model_zones();
    prepare_ISO_New_England_39_bus_model_owners();
}

void prepare_ISO_New_England_39_bus_model_buses()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(50);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_name("BUS_1");
    bus.set_base_voltage_in_kV(100.0);
    bus.set_base_frequency_in_Hz(60.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0475);
    bus.set_positive_sequence_angle_in_deg(-9.57);

    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("BUS_2");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0490);
    bus.set_positive_sequence_angle_in_deg(-7.01);
    psdb.append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_name("BUS_3");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0304);
    bus.set_positive_sequence_angle_in_deg(-9.86);

    psdb.append_bus(bus);

    bus.set_bus_number(4);
    bus.set_bus_name("BUS_4");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0038);
    bus.set_positive_sequence_angle_in_deg(-10.65);

    psdb.append_bus(bus);

    bus.set_bus_number(5);
    bus.set_bus_name("BUS_5");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0050);
    bus.set_positive_sequence_angle_in_deg(-9.47);

    psdb.append_bus(bus);

    bus.set_bus_number(6);
    bus.set_bus_name("BUS_6");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0074);
    bus.set_positive_sequence_angle_in_deg(-8.77);

    psdb.append_bus(bus);

    bus.set_bus_number(7);
    bus.set_bus_name("BUS_7");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9967);
    bus.set_positive_sequence_angle_in_deg(-10.97);

    psdb.append_bus(bus);

    bus.set_bus_number(8);
    bus.set_bus_name("BUS_8");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9957);
    bus.set_positive_sequence_angle_in_deg(-11.48);

    psdb.append_bus(bus);

    bus.set_bus_number(9);
    bus.set_bus_name("BUS_9");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0281);
    bus.set_positive_sequence_angle_in_deg(-11.30);

    psdb.append_bus(bus);

    bus.set_bus_number(10);
    bus.set_bus_name("BUS_10");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0170);
    bus.set_positive_sequence_angle_in_deg(-6.38);

    psdb.append_bus(bus);

    bus.set_bus_number(11);
    bus.set_bus_name("BUS_11");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0125);
    bus.set_positive_sequence_angle_in_deg(-7.2);

    psdb.append_bus(bus);

    bus.set_bus_number(12);
    bus.set_bus_name("BUS_12");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_deg(-7.21);

    psdb.append_bus(bus);

    bus.set_bus_number(13);
    bus.set_bus_name("BUS_4");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0142);
    bus.set_positive_sequence_angle_in_deg(-7.1);

    psdb.append_bus(bus);

    bus.set_bus_number(14);
    bus.set_bus_name("BUS_14");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0117);
    bus.set_positive_sequence_angle_in_deg(-8.76);

    psdb.append_bus(bus);

    bus.set_bus_number(15);
    bus.set_bus_name("BUS_15");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0158);
    bus.set_positive_sequence_angle_in_deg(-9.18);

    psdb.append_bus(bus);

    bus.set_bus_number(16);
    bus.set_bus_name("BUS_16");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0322);
    bus.set_positive_sequence_angle_in_deg(-7.78);

    psdb.append_bus(bus);

    bus.set_bus_number(17);
    bus.set_bus_name("BUS_17");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0339);
    bus.set_positive_sequence_angle_in_deg(-8.77);

    psdb.append_bus(bus);

    bus.set_bus_number(18);
    bus.set_bus_name("BUS_18");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0313);
    bus.set_positive_sequence_angle_in_deg(-9.62);

    psdb.append_bus(bus);

    bus.set_bus_number(19);
    bus.set_bus_name("BUS_19");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(-3.15);

    psdb.append_bus(bus);

    bus.set_bus_number(20);
    bus.set_bus_name("BUS_20");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9910);
    bus.set_positive_sequence_angle_in_deg(-4.56);

    psdb.append_bus(bus);

    bus.set_bus_number(21);
    bus.set_bus_name("BUS_21");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0321);
    bus.set_positive_sequence_angle_in_deg(-5.37);

    psdb.append_bus(bus);

    bus.set_bus_number(22);
    bus.set_bus_name("BUS_22");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(-0.92);

    psdb.append_bus(bus);

    bus.set_bus_number(23);
    bus.set_bus_name("BUS_23");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.045);
    bus.set_positive_sequence_angle_in_deg(-1.12);

    psdb.append_bus(bus);

    bus.set_bus_number(24);
    bus.set_bus_name("BUS_24");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0378);
    bus.set_positive_sequence_angle_in_deg(-7.66);

    psdb.append_bus(bus);

    bus.set_bus_number(25);
    bus.set_bus_name("BUS_25");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0575);
    bus.set_positive_sequence_angle_in_deg(-5.65);

    psdb.append_bus(bus);

    bus.set_bus_number(26);
    bus.set_bus_name("BUS_26");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0522);
    bus.set_positive_sequence_angle_in_deg(-6.91);

    psdb.append_bus(bus);

    bus.set_bus_number(27);
    bus.set_bus_name("BUS_27");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0379);
    bus.set_positive_sequence_angle_in_deg(-8.92);

    psdb.append_bus(bus);

    bus.set_bus_number(28);
    bus.set_bus_name("BUS_28");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0502);
    bus.set_positive_sequence_angle_in_deg(-3.39);

    psdb.append_bus(bus);

    bus.set_bus_number(29);
    bus.set_bus_name("BUS_29");
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.05);
    bus.set_positive_sequence_angle_in_deg(-0.64);

    psdb.append_bus(bus);

    bus.set_bus_number(30);
    bus.set_bus_name("BUS_30");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0475);
    bus.set_positive_sequence_angle_in_deg(-4.59);

    psdb.append_bus(bus);

    bus.set_bus_number(31);
    bus.set_bus_name("BUS_31");
    bus.set_bus_type(SLACK_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9820);
    bus.set_positive_sequence_angle_in_deg(0.0);

    psdb.append_bus(bus);

    bus.set_bus_number(32);
    bus.set_bus_name("BUS_32");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9831);
    bus.set_positive_sequence_angle_in_deg(1.62);

    psdb.append_bus(bus);

    bus.set_bus_number(33);
    bus.set_bus_name("BUS_33");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9972);
    bus.set_positive_sequence_angle_in_deg(2.06);

    psdb.append_bus(bus);

    bus.set_bus_number(34);
    bus.set_bus_name("BUS_34");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0123);
    bus.set_positive_sequence_angle_in_deg(0.63);

    psdb.append_bus(bus);

    bus.set_bus_number(35);
    bus.set_bus_name("BUS_35");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0493);
    bus.set_positive_sequence_angle_in_deg(4.04);

    psdb.append_bus(bus);

    bus.set_bus_number(36);
    bus.set_bus_name("BUS_36");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0635);
    bus.set_positive_sequence_angle_in_deg(6.73);

    psdb.append_bus(bus);

    bus.set_bus_number(37);
    bus.set_bus_name("BUS_37");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0278);
    bus.set_positive_sequence_angle_in_deg(1.14);

    psdb.append_bus(bus);

    bus.set_bus_number(38);
    bus.set_bus_name("BUS_38");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.0265);
    bus.set_positive_sequence_angle_in_deg(6.43);

    psdb.append_bus(bus);

    bus.set_bus_number(39);
    bus.set_bus_name("BUS_39");
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.03);
    bus.set_positive_sequence_angle_in_deg(-11.11);

    psdb.append_bus(bus);
}
void prepare_ISO_New_England_39_bus_model_generators()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    GENERATOR generator(default_toolkit);
    complex<double> gen_z(0.0);

    generator.set_generator_bus(30);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(250.0);
    generator.set_q_generation_in_MVar(144.919);
    generator.set_p_max_in_MW(300.0);
    generator.set_p_min_in_MW(0.0);
    generator.set_q_max_in_MVar(1000.0);
    generator.set_q_min_in_MVar(-1000.0);
    generator.set_mbase_in_MVA(1000.0);
    gen_z = complex<double>(0.0, 0.002);
    generator.set_generator_impedance_in_pu(gen_z);
    generator.set_voltage_to_regulate_in_pu(1.0475);
    generator.set_bus_to_regulate(0);

    psdb.append_generator(generator);


    generator.set_generator_bus(31);
    generator.set_p_generation_in_MW(572.835);
    generator.set_q_generation_in_MVar(207.036);
    generator.set_p_max_in_MW(600.0);
    generator.set_voltage_to_regulate_in_pu(0.9820);

    psdb.append_generator(generator);


    generator.set_generator_bus(32);
    generator.set_p_generation_in_MW(650.0);
    generator.set_q_generation_in_MVar(205.731);
    generator.set_p_max_in_MW(700.0);
    generator.set_voltage_to_regulate_in_pu(0.9831);

    psdb.append_generator(generator);

    generator.set_generator_bus(33);
    generator.set_p_generation_in_MW(632.0);
    generator.set_q_generation_in_MVar(108.936);
    generator.set_p_max_in_MW(700.0);
    generator.set_voltage_to_regulate_in_pu(0.9972);

    psdb.append_generator(generator);

    generator.set_generator_bus(34);
    generator.set_p_generation_in_MW(508.0);
    generator.set_q_generation_in_MVar(166.986);
    generator.set_p_max_in_MW(600.0);
    generator.set_voltage_to_regulate_in_pu(1.0123);

    psdb.append_generator(generator);

    generator.set_generator_bus(35);
    generator.set_p_generation_in_MW(650.0);
    generator.set_q_generation_in_MVar(211.112);
    generator.set_p_max_in_MW(700.0);
    generator.set_voltage_to_regulate_in_pu(1.0493);

    psdb.append_generator(generator);

    generator.set_generator_bus(36);
    generator.set_p_generation_in_MW(560.0);
    generator.set_q_generation_in_MVar(100.438);
    generator.set_p_max_in_MW(600.0);
    generator.set_voltage_to_regulate_in_pu(1.0635);

    psdb.append_generator(generator);

    generator.set_generator_bus(37);
    generator.set_p_generation_in_MW(540.0);
    generator.set_q_generation_in_MVar(0.647);
    generator.set_p_max_in_MW(600.0);
    generator.set_voltage_to_regulate_in_pu(1.0278);

    psdb.append_generator(generator);

    generator.set_generator_bus(38);
    generator.set_p_generation_in_MW(830.0);
    generator.set_q_generation_in_MVar(22.659);
    generator.set_p_max_in_MW(900.0);
    generator.set_voltage_to_regulate_in_pu(1.0265);

    psdb.append_generator(generator);

    generator.set_generator_bus(39);
    generator.set_p_generation_in_MW(1000.0);
    generator.set_q_generation_in_MVar(87.883);
    generator.set_p_max_in_MW(1100.0);
    generator.set_voltage_to_regulate_in_pu(1.03);

    psdb.append_generator(generator);

}
void prepare_ISO_New_England_39_bus_model_loads()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    LOAD load(default_toolkit);
    complex<double> s(0.0,0.0);

    load.set_load_bus(3);
    load.set_identifier("1");
    load.set_status(true);
    s = complex<double>(322.0, 2.4);
    load.set_nominal_constant_power_load_in_MVA(s);
    s = complex<double>(0.0, 0.0);
    load.set_nominal_constant_current_load_in_MVA(s);
    load.set_nominal_constant_impedance_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(4);
    s = complex<double>(500.0, 184.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(7);
    s = complex<double>(233.8, 84.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(8);
    s = complex<double>(522.0, 176.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(12);
    s = complex<double>(8.5, 88.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(15);
    s = complex<double>(320.0, 153.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(16);
    s = complex<double>(329.0, 32.3);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(18);
    s = complex<double>(158.0, 30.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(20);
    s = complex<double>(680.0, 103.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(21);
    s = complex<double>(274.0, 115.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(23);
    s = complex<double>(247.5, 84.6);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(24);
    s = complex<double>(308.6, -92.2);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(25);
    s = complex<double>(224.0, 47.2);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(26);
    s = complex<double>(139.0, 17.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(27);
    s = complex<double>(281.0, 75.5);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(28);
    s = complex<double>(206.0, 27.6);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(29);
    s = complex<double>(283.5, 26.9);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(31);
    s = complex<double>(9.2, 4.6);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

    load.set_load_bus(39);
    s = complex<double>(1104.0, 250.0);
    load.set_nominal_constant_power_load_in_MVA(s);

    psdb.append_load(load);

}
void prepare_ISO_New_England_39_bus_model_lines()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    LINE line(default_toolkit);
    complex<double> z(0.0), y(0.0);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0035, 0.0411);
    y = complex<double>(0.0, 0.6987);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(39);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.001, 0.025);
    y = complex<double>(0.0, 0.75);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(3);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0013, 0.0151);
    y = complex<double>(0.0, 0.2572);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(2);
    line.set_receiving_side_bus(25);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.007, 0.0086);
    y = complex<double>(0.0, 0.146);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(3);
    line.set_receiving_side_bus(4);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0013, 0.0213);
    y = complex<double>(0.0, 0.2214);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(3);
    line.set_receiving_side_bus(18);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0011, 0.0133);
    y = complex<double>(0.0, 0.2138);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(4);
    line.set_receiving_side_bus(5);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0008, 0.0128);
    y = complex<double>(0.0, 0.1342);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(4);
    line.set_receiving_side_bus(14);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0008, 0.0129);
    y = complex<double>(0.0, 0.1382);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(5);
    line.set_receiving_side_bus(6);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0002, 0.0026);
    y = complex<double>(0.0, 0.0434);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(5);
    line.set_receiving_side_bus(8);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0008, 0.0112);
    y = complex<double>(0.0, 0.147);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(6);
    line.set_receiving_side_bus(7);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0006, 0.0092);
    y = complex<double>(0.0, 0.113);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(6);
    line.set_receiving_side_bus(11);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0007, 0.0082);
    y = complex<double>(0.0, 0.1389);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(7);
    line.set_receiving_side_bus(8);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0004, 0.0046);
    y = complex<double>(0.0, 0.078);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(8);
    line.set_receiving_side_bus(9);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0023, 0.0363);
    y = complex<double>(0.0, 0.3804);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(9);
    line.set_receiving_side_bus(39);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.001, 0.025);
    y = complex<double>(0.0, 1.2);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(10);
    line.set_receiving_side_bus(11);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0004, 0.0043);
    y = complex<double>(0.0, 0.0729);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(10);
    line.set_receiving_side_bus(13);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0004, 0.0043);
    y = complex<double>(0.0, 0.0729);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(13);
    line.set_receiving_side_bus(14);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0009, 0.0101);
    y = complex<double>(0.0, 0.1723);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(14);
    line.set_receiving_side_bus(15);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0018, 0.0217);
    y = complex<double>(0.0, 0.366);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(15);
    line.set_receiving_side_bus(16);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0009, 0.0094);
    y = complex<double>(0.0, 0.171);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(16);
    line.set_receiving_side_bus(17);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0007, 0.0089);
    y = complex<double>(0.0, 0.1342);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(16);
    line.set_receiving_side_bus(19);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0016, 0.0195);
    y = complex<double>(0.0, 0.304);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(16);
    line.set_receiving_side_bus(21);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0008, 0.0135);
    y = complex<double>(0.0, 0.2548);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(16);
    line.set_receiving_side_bus(24);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0003, 0.0059);
    y = complex<double>(0.0, 0.068);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(17);
    line.set_receiving_side_bus(18);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0007, 0.0082);
    y = complex<double>(0.0, 0.1319);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(17);
    line.set_receiving_side_bus(27);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0013, 0.0173);
    y = complex<double>(0.0, 0.3216);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(21);
    line.set_receiving_side_bus(22);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0008, 0.014);
    y = complex<double>(0.0, 0.2565);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(22);
    line.set_receiving_side_bus(23);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0006, 0.0096);
    y = complex<double>(0.0, 0.1846);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(23);
    line.set_receiving_side_bus(24);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0022, 0.035);
    y = complex<double>(0.0, 0.361);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(25);
    line.set_receiving_side_bus(26);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0032, 0.0323);
    y = complex<double>(0.0, 0.513);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(26);
    line.set_receiving_side_bus(27);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0014, 0.0147);
    y = complex<double>(0.0, 0.2396);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(26);
    line.set_receiving_side_bus(28);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0043, 0.0474);
    y = complex<double>(0.0, 0.7802);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(26);
    line.set_receiving_side_bus(29);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0057, 0.0625);
    y = complex<double>(0.0, 1.029);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);

    line.set_sending_side_bus(28);
    line.set_receiving_side_bus(29);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0014, 0.0151);
    y = complex<double>(0.0, 0.249);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    psdb.append_line(line);
}

void prepare_ISO_New_England_39_bus_model_transformers()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    TRANSFORMER trans(default_toolkit);
    complex<double> z(0.0), y(0.0);

    trans.set_winding_bus(PRIMARY_SIDE, 2);
    trans.set_winding_bus(SECONDARY_SIDE, 30);
    trans.set_identifier("1");
    trans.set_winding_breaker_status(PRIMARY_SIDE, true);
    trans.set_winding_breaker_status(SECONDARY_SIDE, true);
    trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 100.0);
    trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 100.0);
    trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, 100.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.025);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0, 0.0181);
    y = complex<double>(0.0, 0.0);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(y);
    trans.set_winding_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    trans.set_winding_angle_shift_in_deg(SECONDARY_SIDE, 0.0);

    psdb.append_transformer(trans);


    trans.set_winding_bus(PRIMARY_SIDE, 6);
    trans.set_winding_bus(SECONDARY_SIDE, 31);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.07);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0, 0.025);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 10);
    trans.set_winding_bus(SECONDARY_SIDE, 32);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.07);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0, 0.02);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 11);
    trans.set_winding_bus(SECONDARY_SIDE, 12);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.006);
    z = complex<double>(0.0016, 0.0435);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 12);
    trans.set_winding_bus(SECONDARY_SIDE, 13);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.006);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0016, 0.0435);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 19);
    trans.set_winding_bus(SECONDARY_SIDE, 20);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.06);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0007, 0.0138);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 19);
    trans.set_winding_bus(SECONDARY_SIDE, 33);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.07);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0007, 0.0142);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 20);
    trans.set_winding_bus(SECONDARY_SIDE, 34);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.009);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0009, 0.018);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 22);
    trans.set_winding_bus(SECONDARY_SIDE, 35);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.025);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0, 0.0143);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 23);
    trans.set_winding_bus(SECONDARY_SIDE, 36);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0005, 0.0272);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 25);
    trans.set_winding_bus(SECONDARY_SIDE, 37);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.025);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0006, 0.0232);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 29);
    trans.set_winding_bus(SECONDARY_SIDE, 38);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.025);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0008, 0.0156);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);

    psdb.append_transformer(trans);
}

void prepare_ISO_New_England_39_bus_model_areas()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    AREA area(default_toolkit);

    area.set_area_number(1);
    area.set_area_swing_bus(0);
    area.set_expected_power_leaving_area_in_MW(0.0);
    area.set_area_power_mismatch_tolerance_in_MW(10.0);

    psdb.append_area(area);
}

void prepare_ISO_New_England_39_bus_model_zones()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    ZONE zone(default_toolkit);

    zone.set_zone_number(1);

    psdb.append_zone(zone);
}

void prepare_ISO_New_England_39_bus_model_owners()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    OWNER owner(default_toolkit);

    owner.set_owner_number(1);

    psdb.append_owner(owner);
}

