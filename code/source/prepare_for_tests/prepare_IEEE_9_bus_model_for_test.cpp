#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"
#include "header/model/load_model/IEEL.h"

#include "header/data_imexporter/psse_imexporter.h"

#include <istream>
#include <iostream>
using namespace std;

void prepare_IEEE_9_bus_model(POWER_SYSTEM_DATABASE* db)
{
    ostringstream osstream;
    if(db==NULL)
    {
        osstream<<"NULL power system database is provided for preparing IEEE 9 bus model."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    prepare_IEEE_9_bus_model_buses(db);
    prepare_IEEE_9_bus_model_generators(db);
    prepare_IEEE_9_bus_model_loads(db);
    prepare_IEEE_9_bus_model_lines(db);
    prepare_IEEE_9_bus_model_transformers(db);
    prepare_IEEE_9_bus_model_areas(db);
    prepare_IEEE_9_bus_model_zones(db);
    prepare_IEEE_9_bus_model_owners(db);
}

void prepare_IEEE_9_bus_model_buses(POWER_SYSTEM_DATABASE* db)
{
    db->set_allowed_max_bus_number(10);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_name("BUS_1");
    bus.set_base_voltage_in_kV(16.5);
    bus.set_base_frequency_in_Hz(60.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(SLACK_TYPE);
    bus.set_voltage_in_pu(1.04);
    bus.set_angle_in_deg(0.0);

    db->append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("BUS_2");
    bus.set_base_voltage_in_kV(18.0);
    bus.set_area_number(2);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_voltage_in_pu(1.025);
    bus.set_angle_in_deg(9.280008);

    db->append_bus(bus);

    bus.set_bus_number(3);
    bus.set_bus_name("BUS_3");
    bus.set_base_voltage_in_kV(13.8);
    bus.set_area_number(3);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_voltage_in_pu(1.025);
    bus.set_angle_in_deg(4.664753);

    db->append_bus(bus);

    bus.set_bus_number(4);
    bus.set_bus_name("BUS_4");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.025788);
    bus.set_angle_in_deg(-2.216789);

    db->append_bus(bus);

    bus.set_bus_number(5);
    bus.set_bus_name("BUS_5");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(0.9956308);
    bus.set_angle_in_deg(-3.988808);

    db->append_bus(bus);

    bus.set_bus_number(6);
    bus.set_bus_name("BUS_6");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(3);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.012654);
    bus.set_angle_in_deg(-3.687398);

    db->append_bus(bus);

    bus.set_bus_number(7);
    bus.set_bus_name("BUS_7");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(2);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.025769);
    bus.set_angle_in_deg(3.719703);

    db->append_bus(bus);

    bus.set_bus_number(8);
    bus.set_bus_name("BUS_8");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(2);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.015883);
    bus.set_angle_in_deg(0.7275363);

    db->append_bus(bus);

    bus.set_bus_number(9);
    bus.set_bus_name("BUS_9");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(3);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.032353);
    bus.set_angle_in_deg(1.966717);

    db->append_bus(bus);
}
void prepare_IEEE_9_bus_model_generators(POWER_SYSTEM_DATABASE* db)
{
    GENERATOR generator(db);
    complex<double> gen_z(0.0);

    generator.set_generator_bus(1);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(71.64104);
    generator.set_q_generation_in_MVar(27.04593);
    generator.set_p_max_in_MW(137.0);
    generator.set_p_min_in_MW(6.0);
    generator.set_q_max_in_MVar(999.0);
    generator.set_q_min_in_MVar(-999.0);
    generator.set_mbase_in_MVA(247.5);
    gen_z = complex<double>(0.0, 0.15048);
    generator.set_generator_impedance_in_pu(gen_z);
    generator.set_voltage_to_regulate_in_pu(1.04);
    generator.set_bus_to_regulate(0);

    db->append_generator(generator);


    generator.set_generator_bus(2);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(163.0);
    generator.set_q_generation_in_MVar(6.6537);
    generator.set_p_max_in_MW(200.0);
    generator.set_p_min_in_MW(100.0);
    generator.set_q_max_in_MVar(999.0);
    generator.set_q_min_in_MVar(-999.0);
    generator.set_mbase_in_MVA(192.0);
    gen_z = complex<double>(0.0, 0.230016);
    generator.set_generator_impedance_in_pu(gen_z);
    generator.set_voltage_to_regulate_in_pu(1.025);
    generator.set_bus_to_regulate(0);

    db->append_generator(generator);


    generator.set_generator_bus(3);
    generator.set_identifier("1");
    generator.set_status(true);
    generator.set_p_generation_in_MW(85.0);
    generator.set_q_generation_in_MVar(-10.8597);
    generator.set_p_max_in_MW(140.0);
    generator.set_p_min_in_MW(70.0);
    generator.set_q_max_in_MVar(999.0);
    generator.set_q_min_in_MVar(-999.0);
    generator.set_mbase_in_MVA(128.0);
    gen_z = complex<double>(0.0, 0.232064);
    generator.set_generator_impedance_in_pu(gen_z);
    generator.set_voltage_to_regulate_in_pu(1.025);
    generator.set_bus_to_regulate(0);

    db->append_generator(generator);
}
void prepare_IEEE_9_bus_model_loads(POWER_SYSTEM_DATABASE* db)
{
    LOAD load(db);
    complex<double> s(0.0,0.0);

    load.set_load_bus(5);
    load.set_identifier("1");
    load.set_status(true);
    s = complex<double>(125, 50);
    load.set_nominal_constant_power_load_in_MVA(s);
    s = complex<double>(0.0, 0.0);
    load.set_nominal_constant_current_load_in_MVA(s);
    load.set_nominal_constant_impedance_load_in_MVA(s);

    db->append_load(load);

    load.set_load_bus(6);
    load.set_identifier("1");
    load.set_status(true);
    s = complex<double>(90.0, 30.0);
    load.set_nominal_constant_power_load_in_MVA(s);
    s = complex<double>(0.0, 0.0);
    load.set_nominal_constant_current_load_in_MVA(s);
    load.set_nominal_constant_impedance_load_in_MVA(s);

    db->append_load(load);

    load.set_load_bus(8);
    load.set_identifier("1");
    load.set_status(true);
    s = complex<double>(100, 35);
    load.set_nominal_constant_power_load_in_MVA(s);
    s = complex<double>(0.0, 0.0);
    load.set_nominal_constant_current_load_in_MVA(s);
    load.set_nominal_constant_impedance_load_in_MVA(s);

    db->append_load(load);
}
void prepare_IEEE_9_bus_model_lines(POWER_SYSTEM_DATABASE* db)
{
    LINE line(db);
    complex<double> z(0.0), y(0.0);

    line.set_sending_side_bus(4);
    line.set_receiving_side_bus(5);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.01, 0.085);
    y = complex<double>(0.0, 0.176);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);
    OWNERSHIP ownership;
    ownership.append_owner_and_its_fraction(1,1.0);
    line.set_ownership(ownership);

    db->append_line(line);

    line.set_sending_side_bus(4);
    line.set_receiving_side_bus(6);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.017, 0.092);
    y = complex<double>(0.0, 0.158);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    db->append_line(line);

    line.set_sending_side_bus(5);
    line.set_receiving_side_bus(7);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.032, 0.161);
    y = complex<double>(0.0, 0.306);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    db->append_line(line);

    line.set_sending_side_bus(6);
    line.set_receiving_side_bus(9);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.039, 0.17);
    y = complex<double>(0.0, 0.358);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    db->append_line(line);

    line.set_sending_side_bus(7);
    line.set_receiving_side_bus(8);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0085, 0.072);
    y = complex<double>(0.0, 0.149);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    db->append_line(line);

    line.set_sending_side_bus(8);
    line.set_receiving_side_bus(9);
    line.set_identifier("1");
    line.set_sending_side_breaker_status(true);
    line.set_receiving_side_breaker_status(true);
    line.set_meter_end_bus(line.get_sending_side_bus());
    z = complex<double>(0.0119, 0.1008);
    y = complex<double>(0.0, 0.209);
    line.set_line_positive_sequence_z_in_pu(z);
    line.set_line_positive_sequence_y_in_pu(y);

    db->append_line(line);
}

void prepare_IEEE_9_bus_model_transformers(POWER_SYSTEM_DATABASE* db)
{
    TRANSFORMER trans(db);
    complex<double> z(0.0), y(0.0);

    trans.set_winding_bus(PRIMARY_SIDE, 1);
    trans.set_winding_bus(SECONDARY_SIDE, 4);
    trans.set_identifier("1");
    trans.set_winding_breaker_status(PRIMARY_SIDE, true);
    trans.set_winding_breaker_status(SECONDARY_SIDE, true);
    trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 16.5);
    trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 230.0);
    trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, 100.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    z = complex<double>(0.0, 0.0576);
    y = complex<double>(0.0, 0.0);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(y);
    trans.set_winding_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    trans.set_winding_angle_shift_in_deg(SECONDARY_SIDE, 0.0);


    /*trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, 150.0);
    trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 200.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.1);
    trans.set_winding_angle_shift_in_deg(SECONDARY_SIDE, 5.0);
    trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 14.0);
    trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.2);
    //trans.set_winding_angle_shift_in_deg(PRIMARY_SIDE, -5.0);
    //z = z/100.0*150.0;
    //trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
*/
    db->append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 2);
    trans.set_winding_bus(SECONDARY_SIDE, 7);
    trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 18.0);
    trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 230.0);
    z = complex<double>(0.00, 0.0625);
    y = complex<double>(0.0, 0.0);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(y);

    db->append_transformer(trans);

    trans.set_winding_bus(PRIMARY_SIDE, 3);
    trans.set_winding_bus(SECONDARY_SIDE, 9);
    trans.set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, 13.8);
    trans.set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, 230.0);
    z = complex<double>(0.00, 0.0586);
    y = complex<double>(0.0, 0.0);
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, z);
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(y);

    db->append_transformer(trans);
}

void prepare_IEEE_9_bus_model_areas(POWER_SYSTEM_DATABASE* db)
{
    AREA area(db);

    area.set_area_number(1);
    area.set_area_swing_bus(0);
    area.set_expected_power_leaving_area_in_MW(0.0);
    area.set_area_power_mismatch_tolerance_in_MW(10.0);

    db->append_area(area);

    area.set_area_number(2);
    db->append_area(area);

    area.set_area_number(3);
    db->append_area(area);
}

void prepare_IEEE_9_bus_model_zones(POWER_SYSTEM_DATABASE* db)
{
    ZONE zone(db);

    zone.set_zone_number(1);

    db->append_zone(zone);
}

void prepare_IEEE_9_bus_model_owners(POWER_SYSTEM_DATABASE* db)
{
    OWNER owner(db);

    owner.set_owner_number(1);

    db->append_owner(owner);
}

void prepare_IEEE_9_bus_model_dynamic_model(POWER_SYSTEM_DATABASE* db)
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    GENERATOR* generator;

    GENCLS gen1_model;
    gen1_model.set_H_in_s(9.5515);

    generator = db->get_generator(did);
    generator->set_model(&gen1_model);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    GENCLS gen2_model;
    gen2_model.set_H_in_s(3.3333);

    generator = db->get_generator(did);
    generator->set_model(&gen2_model);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    GENCLS gen3_model;
    gen3_model.set_H_in_s(2.3516);

    generator = db->get_generator(did);
    generator->set_model(&gen3_model);

    did.set_device_type("LOAD");


    IEEL load_model;
    load_model.set_P_alpha_1(1.0);
    load_model.set_P_alpha_2(0.0);
    load_model.set_P_alpha_3(0.0);
    load_model.set_P_n_power_1(2.0);
    load_model.set_P_n_power_2(1.0);
    load_model.set_P_n_power_3(0.0);
    load_model.set_P_Kf(0.0);

    load_model.set_Q_alpha_1(1.0);
    load_model.set_Q_alpha_2(0.0);
    load_model.set_Q_alpha_3(0.0);
    load_model.set_Q_n_power_1(2.0);
    load_model.set_Q_n_power_2(1.0);
    load_model.set_Q_n_power_3(0.0);
    load_model.set_Q_Kf(0.0);
    load_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    vector<LOAD*> loads = db->get_all_loads();
    size_t n = db->get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        loads[i]->set_model(&load_model);
    }
}


void prepare_IEEE_9_bus_model_classical_dynamic_model(POWER_SYSTEM_DATABASE* db)
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    GENERATOR* generator;

    GENCLS gen1_model;
    gen1_model.set_H_in_s(9.5515);

    generator = db->get_generator(did);
    generator->set_model(&gen1_model);

    complex<double> gen_z(0.0, 0.15048);
    generator->set_generator_impedance_in_pu(gen_z);


    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);

    GENCLS gen2_model;
    gen2_model.set_H_in_s(3.3333);

    generator = db->get_generator(did);
    generator->set_model(&gen2_model);

    gen_z = complex<double>(0.0, 0.230016);
    generator->set_generator_impedance_in_pu(gen_z);


    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    GENCLS gen3_model;
    gen3_model.set_H_in_s(2.3516);

    generator = db->get_generator(did);
    generator->set_model(&gen3_model);

    gen_z = complex<double>(0.0, 0.232064);
    generator->set_generator_impedance_in_pu(gen_z);

    did.set_device_type("LOAD");

    IEEL load_model;
    load_model.set_P_alpha_1(1.0);
    load_model.set_P_alpha_2(0.0);
    load_model.set_P_alpha_3(0.0);
    load_model.set_P_n_power_1(2.0);
    load_model.set_P_n_power_2(1.0);
    load_model.set_P_n_power_3(0.0);
    load_model.set_P_Kf(0.0);

    load_model.set_Q_alpha_1(1.0);
    load_model.set_Q_alpha_2(0.0);
    load_model.set_Q_alpha_3(0.0);
    load_model.set_Q_n_power_1(2.0);
    load_model.set_Q_n_power_2(1.0);
    load_model.set_Q_n_power_3(0.0);
    load_model.set_Q_Kf(0.0);
    load_model.set_subsystem_type(BUS_SUBSYSTEM_TYPE);

    vector<LOAD*> loads = db->get_all_loads();
    size_t n = db->get_load_count();
    for(size_t i=0; i!=n; ++i)
    {
        loads[i]->set_model(&load_model);
    }
}


void prepare_IEEE_9_bus_model_complete_dynamic_model(POWER_SYSTEM_DATABASE* db)
{
    PSSE_IMEXPORTER importer;
    importer.set_power_system_database(db);
    importer.load_dynamic_data("ieee9.dyr");
}

