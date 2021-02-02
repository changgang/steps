#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/data_imexporter/psse_imexporter_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

PSSE_IMEXPORTER_TEST::PSSE_IMEXPORTER_TEST() : importer(default_toolkit)
{
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_case_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_bus_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_load_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_fixed_shunt_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_generator_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_wt_generator_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_line_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_transformer_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_area_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_hvdc_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_zone_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_owner_data);
    TEST_ADD(PSSE_IMEXPORTER_TEST::test_export_powerflow_data_imported_from_psse);

    TEST_ADD(PSSE_IMEXPORTER_TEST::test_load_dynamic_data);
}

void PSSE_IMEXPORTER_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100000);

    importer.load_powerflow_data("../../../bench/sample.raw");
}

void PSSE_IMEXPORTER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

void PSSE_IMEXPORTER_TEST::test_load_case_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    TEST_ASSERT(psdb.get_system_base_power_in_MVA()==100.0);
}

void PSSE_IMEXPORTER_TEST::test_load_bus_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<BUS*> buses = psdb.get_all_buses();
    unsigned int n = buses.size();

    TEST_ASSERT(n==42);

    BUS* bus;
    bus = psdb.get_bus(101);
    TEST_ASSERT(bus->get_bus_number()==101);
    TEST_ASSERT(bus->get_bus_name()=="NUC-A");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-21.6)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_voltage_in_pu()-1.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_angle_in_deg()-(-10.4286))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PV_TYPE);
    TEST_ASSERT(bus->get_area_number()==1);
    TEST_ASSERT(bus->get_zone_number()==1);
    TEST_ASSERT(bus->get_owner_number()==1);

    bus = psdb.get_bus(201);
    TEST_ASSERT(bus->get_bus_number()==201);
    TEST_ASSERT(bus->get_bus_name()=="HYDRO");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_voltage_in_pu()-0.99007)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_angle_in_deg()-(-18.7119))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PQ_TYPE);
    TEST_ASSERT(bus->get_area_number()==2);
    TEST_ASSERT(bus->get_zone_number()==7);
    TEST_ASSERT(bus->get_owner_number()==2);

    bus = psdb.get_bus(3001);
    TEST_ASSERT(bus->get_bus_number()==3001);
    TEST_ASSERT(bus->get_bus_name()=="MINE");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-230.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_voltage_in_pu()-1.0237)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_angle_in_deg()-(-4.0297))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PQ_TYPE);
    TEST_ASSERT(bus->get_area_number()==5);
    TEST_ASSERT(bus->get_zone_number()==6);
    TEST_ASSERT(bus->get_owner_number()==5);

    bus = psdb.get_bus(93002);
    TEST_ASSERT(bus->get_bus_number()==93002);
    TEST_ASSERT(bus->get_bus_name()=="WINDBUS3");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-0.69)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_voltage_in_pu()-0.99622)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_positive_sequence_angle_in_deg()-1.4362)<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PV_TYPE);
    TEST_ASSERT(bus->get_area_number()==5);
    TEST_ASSERT(bus->get_zone_number()==6);
    TEST_ASSERT(bus->get_owner_number()==5);
}

void PSSE_IMEXPORTER_TEST::test_load_load_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<LOAD*> loads = psdb.get_all_loads();
    unsigned int n = loads.size();

    TEST_ASSERT(n==20);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(152);

    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    LOAD* load = psdb.get_load(did);
    TEST_ASSERT(load->get_load_bus()==152);
    TEST_ASSERT(load->get_identifier()=="1");
    TEST_ASSERT(load->get_status()==true);
    TEST_ASSERT(load->get_nominal_constant_power_load_in_MVA()==complex<double>(1200.0, 360.0));
    TEST_ASSERT(load->get_nominal_constant_current_load_in_MVA()==complex<double>(868.34, 360.502));
    TEST_ASSERT(load->get_nominal_constant_impedance_load_in_MVA()==complex<double>(837.794, 351.338));

    terminal.clear();
    terminal.append_bus(215);
    did.set_device_terminal(terminal);
    did.set_device_identifier("U1");
    load = psdb.get_load(did);
    TEST_ASSERT(load->get_load_bus()==215);
    TEST_ASSERT(load->get_identifier()=="U1");
    TEST_ASSERT(load->get_status()==true);
    TEST_ASSERT(load->get_nominal_constant_power_load_in_MVA()==complex<double>(0.0, 140.0));
    TEST_ASSERT(load->get_nominal_constant_current_load_in_MVA()==complex<double>(0.0,0.0));
    TEST_ASSERT(load->get_nominal_constant_impedance_load_in_MVA()==complex<double>(0.0, 0.0));

    terminal.clear();
    terminal.append_bus(3010);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    load = psdb.get_load(did);
    TEST_ASSERT(load->get_load_bus()==3010);
    TEST_ASSERT(load->get_identifier()=="1");
    TEST_ASSERT(load->get_status()==true);
    TEST_ASSERT(load->get_nominal_constant_power_load_in_MVA()==complex<double>(12.0, 5.0));
    TEST_ASSERT(load->get_nominal_constant_current_load_in_MVA()==complex<double>(0.0,0.0));
    TEST_ASSERT(load->get_nominal_constant_impedance_load_in_MVA()==complex<double>(0.0, 0.0));
    TEST_ASSERT(load->get_area_number()==5);
    TEST_ASSERT(load->get_zone_number()==4);
    TEST_ASSERT(load->get_owner_number()==5);
}

void PSSE_IMEXPORTER_TEST::test_load_fixed_shunt_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<FIXED_SHUNT*> shunts = psdb.get_all_fixed_shunts();
    unsigned int n = shunts.size();

    TEST_ASSERT(n==13);

    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");
    TERMINAL terminal;
    terminal.append_bus(151);

    did.set_device_terminal(terminal);
    did.set_device_identifier("F1");

    FIXED_SHUNT* shunt = psdb.get_fixed_shunt(did);
    TEST_ASSERT(shunt->get_shunt_bus()==151);
    TEST_ASSERT(shunt->get_identifier()=="F1");
    TEST_ASSERT(shunt->get_status()==true);
    TEST_ASSERT(shunt->get_nominal_positive_sequence_impedance_shunt_in_MVA()==complex<double>(5.0, 400.0));

    terminal.clear();
    terminal.append_bus(201);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    shunt = psdb.get_fixed_shunt(did);
    TEST_ASSERT(shunt->get_shunt_bus()==201);
    TEST_ASSERT(shunt->get_identifier()=="1");
    TEST_ASSERT(shunt->get_status()==true);
    TEST_ASSERT(shunt->get_nominal_positive_sequence_impedance_shunt_in_MVA()==complex<double>(3.67, 500.0));

    terminal.clear();
    terminal.append_bus(3022);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    shunt = psdb.get_fixed_shunt(did);
    TEST_ASSERT(shunt->get_shunt_bus()==3022);
    TEST_ASSERT(shunt->get_identifier()=="1");
    TEST_ASSERT(shunt->get_status()==true);
    TEST_ASSERT(shunt->get_nominal_positive_sequence_impedance_shunt_in_MVA()==complex<double>(0.0, -1080.0));
}

void PSSE_IMEXPORTER_TEST::test_load_generator_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    unsigned int n = generators.size();

    TEST_ASSERT(n==12);

    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(101);

    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    GENERATOR* gen = psdb.get_generator(did);
    TEST_ASSERT(gen->get_generator_bus()==101);
    TEST_ASSERT(gen->get_identifier()=="1");
    TEST_ASSERT(gen->get_status()==true);
    TEST_ASSERT(gen->get_mbase_in_MVA()==900.0);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-750.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-125.648)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_max_in_MW()-800.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_min_in_MW()-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_max_in_MVar()-400.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_min_in_MVar()-(-100.0))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(301);
    did.set_device_terminal(terminal);
    did.set_device_identifier("2");
    gen = psdb.get_generator(did);
    TEST_ASSERT(gen->get_generator_bus()==301);
    TEST_ASSERT(gen->get_identifier()=="2");
    TEST_ASSERT(gen->get_status()==true);
    TEST_ASSERT(gen->get_mbase_in_MVA()==1070.0);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-996.883)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-299.542)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_max_in_MW()-1011.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_min_in_MW()-321.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_max_in_MVar()-710.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_min_in_MVar()-(-600.0))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(3018);
    did.set_device_terminal(terminal);
    did.set_device_identifier("2");
    gen = psdb.get_generator(did);
    TEST_ASSERT(gen->get_generator_bus()==3018);
    TEST_ASSERT(gen->get_identifier()=="2");
    TEST_ASSERT(gen->get_status()==true);
    TEST_ASSERT(gen->get_mbase_in_MVA()==120.0);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-(-0.157))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_max_in_MW()-110.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_p_min_in_MW()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_max_in_MVar()-75.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(gen->get_q_min_in_MVar()-(-75.0))<FLOAT_EPSILON);
}

void PSSE_IMEXPORTER_TEST::test_load_wt_generator_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    unsigned int n = wt_generators.size();

    TEST_ASSERT(n==3);

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(9154);

    did.set_device_terminal(terminal);
    did.set_device_identifier("W1");

    WT_GENERATOR* wt_generator = psdb.get_wt_generator(did);
    TEST_ASSERT(wt_generator->get_source_bus()==9154);
    TEST_ASSERT(wt_generator->get_identifier()=="W1");
    TEST_ASSERT(wt_generator->get_status()==true);
    TEST_ASSERT(wt_generator->get_mbase_in_MVA()==12.0);
    TEST_ASSERT(fabs(wt_generator->get_p_generation_in_MW()-10.8)<FLOAT_EPSILON);
    //TEST_ASSERT(fabs(wt_generator->get_q_generation_in_MVar()-2.193034)<FLOAT_EPSILON); // this one is the constant pf Q
    TEST_ASSERT(fabs(wt_generator->get_p_max_in_MW()-10.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_max_in_MVar()-2.1930335348472467)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_min_in_MVar()-(-2.1930335348472467))<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(93002);
    did.set_device_terminal(terminal);
    did.set_device_identifier("W3");
    wt_generator = psdb.get_wt_generator(did);
    TEST_ASSERT(wt_generator->get_source_bus()==93002);
    TEST_ASSERT(wt_generator->get_identifier()=="W3");
    TEST_ASSERT(wt_generator->get_status()==true);
    TEST_ASSERT(wt_generator->get_mbase_in_MVA()==4.0);
    TEST_ASSERT(fabs(wt_generator->get_p_generation_in_MW()-3.24)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_generation_in_MVar()-(-1.475))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_max_in_MW()-3.6)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_max_in_MVar()-(-1.475))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_min_in_MVar()-(-1.475))<FLOAT_EPSILON);
}

void PSSE_IMEXPORTER_TEST::test_load_line_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();
    unsigned int n = lines.size();

    TEST_ASSERT(n==30);

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(151);
    terminal.append_bus(152);

    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    LINE* line = psdb.get_line(did);
    TEST_ASSERT(line->get_sending_side_bus()==151);
    TEST_ASSERT(line->get_receiving_side_bus()==152);
    TEST_ASSERT(line->get_identifier()=="1");
    TEST_ASSERT(line->get_sending_side_breaker_status()==true);
    TEST_ASSERT(line->get_receiving_side_breaker_status()==true);
    TEST_ASSERT(line->get_line_positive_sequence_z_in_pu()==complex<double>(0.0026, 0.046));
    TEST_ASSERT(line->get_line_positive_sequence_y_in_pu()==complex<double>(0.0, 3.5));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_sending_side_in_pu()==complex<double>(0.01, -0.25));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_receiving_side_in_pu()==complex<double>(0.011, -0.15));
    TEST_ASSERT(line->get_length()==150.0);

    terminal.clear();
    terminal.append_bus(213);
    terminal.append_bus(214);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    line = psdb.get_line(did);
    TEST_ASSERT(line->get_sending_side_bus()==213);
    TEST_ASSERT(line->get_receiving_side_bus()==214);
    TEST_ASSERT(line->get_identifier()=="1");
    TEST_ASSERT(line->get_sending_side_breaker_status()==true);
    TEST_ASSERT(line->get_receiving_side_breaker_status()==true);
    TEST_ASSERT(line->get_line_positive_sequence_z_in_pu()==complex<double>(0.0, 0.01));
    TEST_ASSERT(line->get_line_positive_sequence_y_in_pu()==complex<double>(0.0, 0.0));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_sending_side_in_pu()==complex<double>(0.0, 0.0));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_receiving_side_in_pu()==complex<double>(0.0, 0.0));
    TEST_ASSERT(line->get_length()==0.5);

    terminal.clear();
    terminal.append_bus(3008);
    terminal.append_bus(3009);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    line = psdb.get_line(did);
    TEST_ASSERT(line->get_sending_side_bus()==3008);
    TEST_ASSERT(line->get_receiving_side_bus()==3009);
    TEST_ASSERT(line->get_identifier()=="1");
    TEST_ASSERT(line->get_sending_side_breaker_status()==true);
    TEST_ASSERT(line->get_receiving_side_breaker_status()==true);
    TEST_ASSERT(line->get_line_positive_sequence_z_in_pu()==complex<double>(0.003, 0.025));
    TEST_ASSERT(line->get_line_positive_sequence_y_in_pu()==complex<double>(0.0, 0.06));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_sending_side_in_pu()==complex<double>(0.0, 0.0));
    TEST_ASSERT(line->get_shunt_positive_sequence_y_at_receiving_side_in_pu()==complex<double>(0.0, 0.0));
    TEST_ASSERT(line->get_length()==60.0);
}

void PSSE_IMEXPORTER_TEST::test_load_transformer_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<TRANSFORMER*> trans = psdb.get_all_transformers();
    unsigned int n = trans.size();

    TEST_ASSERT(n==18);

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");

    TERMINAL terminal;
    terminal.append_bus(101);
    terminal.append_bus(151);

    did.set_device_terminal(terminal);
    did.set_device_identifier("T1");

    TRANSFORMER* transptr = psdb.get_transformer(did);
    TEST_ASSERT(transptr!=NULL);
    TEST_ASSERT(fabs(transptr->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)-1200.0)<FLOAT_EPSILON);

    terminal.clear();
    terminal.append_bus(102);
    terminal.append_bus(151);

    did.set_device_terminal(terminal);
    did.set_device_identifier("T2");

    transptr = psdb.get_transformer(did);
    TEST_ASSERT(transptr!=NULL);
    TEST_ASSERT(fabs(transptr->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)-1210.0)<FLOAT_EPSILON);


    for(unsigned int i=0; i!=n; ++i)
        trans[i]->report();
}

void PSSE_IMEXPORTER_TEST::test_load_area_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<AREA*> areas = psdb.get_all_areas();
    unsigned int n = areas.size();

    TEST_ASSERT(n==6);

    AREA* area;

    area = psdb.get_area(1);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==1);
    TEST_ASSERT(area->get_area_swing_bus()==101);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(-2800.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="CENTRAL");
    cout<<"Area 1 name is :'"<<area->get_area_name()<<"'"<<endl;

    area = psdb.get_area(2);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==2);
    TEST_ASSERT(area->get_area_swing_bus()==206);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(-1600.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="EAST");

    area = psdb.get_area(3);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==3);
    TEST_ASSERT(area->get_area_swing_bus()==301);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(2900.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-55.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="CENTRAL_DC");

    area = psdb.get_area(4);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==4);
    TEST_ASSERT(area->get_area_swing_bus()==401);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(300.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-15.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="EAST_COGEN1");

    area = psdb.get_area(5);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==5);
    TEST_ASSERT(area->get_area_swing_bus()==3011);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(900.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="WEST");

    area = psdb.get_area(6);
    TEST_ASSERT(area!=NULL);
    TEST_ASSERT(area->get_area_number()==6);
    TEST_ASSERT(area->get_area_swing_bus()==402);
    TEST_ASSERT(fabs(area->get_expected_power_leaving_area_in_MW()-(300.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(area->get_area_power_mismatch_tolerance_in_MW()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(area->get_area_name()=="EAST_COGEN2");

    for(unsigned int i=0; i!=n; ++i)
        areas[i]->report();
}

void PSSE_IMEXPORTER_TEST::test_load_hvdc_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    unsigned int n = hvdcs.size();

    TEST_ASSERT(n==2);

    for(unsigned int i=0; i!=n; ++i)
        hvdcs[i]->report();
}

void PSSE_IMEXPORTER_TEST::test_load_zone_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<ZONE*> zones = psdb.get_all_zones();
    unsigned int n = zones.size();

    TEST_ASSERT(n==9);

    ZONE* zone;

    zone = psdb.get_zone(1);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==1);
    TEST_ASSERT(zone->get_zone_name()=="NORTH_A1");

    zone = psdb.get_zone(2);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==2);
    TEST_ASSERT(zone->get_zone_name()=="MID_A1_A2_A5");

    zone = psdb.get_zone(3);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==3);
    TEST_ASSERT(zone->get_zone_name()=="DISCNT_IN_A1");

    zone = psdb.get_zone(4);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==4);
    TEST_ASSERT(zone->get_zone_name()=="SOUTH_A1_A5");

    zone = psdb.get_zone(5);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==5);
    TEST_ASSERT(zone->get_zone_name()=="ALL_A3");

    zone = psdb.get_zone(6);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==6);
    TEST_ASSERT(zone->get_zone_name()=="NORTH_A5");

    zone = psdb.get_zone(7);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==7);
    TEST_ASSERT(zone->get_zone_name()=="NORTH_A2");

    zone = psdb.get_zone(8);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==8);
    TEST_ASSERT(zone->get_zone_name()=="SOUTH_A2");

    zone = psdb.get_zone(9);
    TEST_ASSERT(zone!=NULL);
    TEST_ASSERT(zone->get_zone_number()==9);
    TEST_ASSERT(zone->get_zone_name()=="ALL_A4_A6");

    for(unsigned int i=0; i!=n; ++i)
        zones[i]->report();
}

void PSSE_IMEXPORTER_TEST::test_load_owner_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    vector<OWNER*> owners = psdb.get_all_owners();
    unsigned int n = owners.size();

    TEST_ASSERT(n==5);

    OWNER* owner;

    owner = psdb.get_owner(1);
    TEST_ASSERT(owner!=NULL);
    TEST_ASSERT(owner->get_owner_number()==1);
    TEST_ASSERT(owner->get_owner_name()=="OWNER 1");

    owner = psdb.get_owner(2);
    TEST_ASSERT(owner!=NULL);
    TEST_ASSERT(owner->get_owner_number()==2);
    TEST_ASSERT(owner->get_owner_name()=="OWNER 2");

    owner = psdb.get_owner(3);
    TEST_ASSERT(owner!=NULL);
    TEST_ASSERT(owner->get_owner_number()==3);
    TEST_ASSERT(owner->get_owner_name()=="OWNER 3");

    owner = psdb.get_owner(4);
    TEST_ASSERT(owner!=NULL);
    TEST_ASSERT(owner->get_owner_number()==4);
    TEST_ASSERT(owner->get_owner_name()=="OWNER 4");

    owner = psdb.get_owner(5);
    TEST_ASSERT(owner!=NULL);
    TEST_ASSERT(owner->get_owner_number()==5);
    TEST_ASSERT(owner->get_owner_name()=="OWNER 5");

    for(unsigned int i=0; i!=n; ++i)
        owners[i]->report();
}

void PSSE_IMEXPORTER_TEST::test_export_powerflow_data_imported_from_psse()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    importer.export_powerflow_data("export_sample_model_with_PSSE_IMEXPORTER_data_imported_with_PSSE_IMEXPORTER.raw");
}

void PSSE_IMEXPORTER_TEST::test_export_powerflow_data_imported_from_bpa()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    importer.export_powerflow_data("export_sample_model_with_PSSE_IMEXPORTER_data_imported_with_BPA_IMEXPORTER.raw");
}

void PSSE_IMEXPORTER_TEST::test_load_dynamic_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"PSSE_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
    importer.load_powerflow_data("../../../bench/ieee9.raw");
    importer.load_dynamic_data("../../../bench/ieee9.dyr");
}


#endif
