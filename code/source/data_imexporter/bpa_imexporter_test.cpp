#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"

#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;


BPA_IMEXPORTER_TEST::BPA_IMEXPORTER_TEST()
{
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_powerflow_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_zone_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_owner_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_bus_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_area_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_line_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_generator_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_load_and_fixed_shunt_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_transformer_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_hvdc_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_convert_data_into_bpa_format);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_export_powerflow_data);
    /*TEST_ADD(BPA_IMEXPORTER_TEST::test_load_case_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_fixed_shunt_data);
    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_wt_generator_data);

    TEST_ADD(BPA_IMEXPORTER_TEST::test_load_dynamic_data);
    */
}

void BPA_IMEXPORTER_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.set_allowed_max_bus_number(100000);

    importer.load_powerflow_data("山东电网简化.dat");
}

void BPA_IMEXPORTER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();

    show_test_end_information();
}

void BPA_IMEXPORTER_TEST::test_load_powerflow_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

}

void BPA_IMEXPORTER_TEST::test_load_case_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    TEST_ASSERT(psdb.get_system_base_power_in_MVA()==100.0);
    TEST_ASSERT(importer.get_data_version()==31);
}


void BPA_IMEXPORTER_TEST::test_load_zone_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<ZONE*> zones = psdb.get_all_zones();

    osstream<<"Zone count: "<<zones.size()<<endl;
    size_t n = zones.size();
    for(size_t i=0; i!=n; ++i)
        osstream<<zones[i]->get_zone_name()<<", "<<zones[i]->get_zone_number()<<endl;

    show_information_with_leading_time_stamp(osstream);
}
void BPA_IMEXPORTER_TEST::test_load_owner_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<OWNER*> owners = psdb.get_all_owners();

    osstream<<"Owner count: "<<owners.size()<<endl;
    size_t n = owners.size();
    for(size_t i=0; i!=n; ++i)
        osstream<<owners[i]->get_owner_name()<<", "<<owners[i]->get_owner_number()<<endl;
    show_information_with_leading_time_stamp(osstream);
}
void BPA_IMEXPORTER_TEST::test_load_bus_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<BUS*> buses = psdb.get_all_buses();

    osstream<<"Bus count: "<<buses.size()<<endl;
    show_information_with_leading_time_stamp(osstream);
    size_t n = buses.size();
    for(size_t i=0; i!=n; ++i)
        buses[i]->report();
}
void BPA_IMEXPORTER_TEST::test_load_area_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<AREA*> areas = psdb.get_all_areas();

    osstream<<"Area count: "<<areas.size()<<endl;
    size_t n = areas.size();
    for(size_t i=0; i!=n; ++i)
        osstream<<areas[i]->get_area_name()<<", "<<areas[i]->get_area_number()<<endl;
    show_information_with_leading_time_stamp(osstream);
}

/*
void BPA_IMEXPORTER_TEST::test_load_bus_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    vector<BUS*> buses = psdb.get_all_buses();
    size_t n = buses.size();

    TEST_ASSERT(n==42);

    BUS* bus;
    bus = psdb.get_bus(101);
    TEST_ASSERT(bus->get_bus_number()==101);
    TEST_ASSERT(bus->get_bus_name()=="NUC-A");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-21.6)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.01)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-10.4286))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PV_TYPE);
    TEST_ASSERT(bus->get_area_number()==1);
    TEST_ASSERT(bus->get_zone_number()==1);
    TEST_ASSERT(bus->get_owner_number()==1);

    bus = psdb.get_bus(201);
    TEST_ASSERT(bus->get_bus_number()==201);
    TEST_ASSERT(bus->get_bus_name()=="HYDRO");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-500.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.99007)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-18.7119))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PQ_TYPE);
    TEST_ASSERT(bus->get_area_number()==2);
    TEST_ASSERT(bus->get_zone_number()==7);
    TEST_ASSERT(bus->get_owner_number()==2);

    bus = psdb.get_bus(3001);
    TEST_ASSERT(bus->get_bus_number()==3001);
    TEST_ASSERT(bus->get_bus_name()=="MINE");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-230.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0237)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-4.0297))<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PQ_TYPE);
    TEST_ASSERT(bus->get_area_number()==5);
    TEST_ASSERT(bus->get_zone_number()==6);
    TEST_ASSERT(bus->get_owner_number()==5);

    bus = psdb.get_bus(93002);
    TEST_ASSERT(bus->get_bus_number()==93002);
    TEST_ASSERT(bus->get_bus_name()=="WINDBUS3");
    TEST_ASSERT(fabs(bus->get_base_voltage_in_kV()-0.69)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.99622)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-1.4362)<FLOAT_EPSILON);
    TEST_ASSERT(bus->get_bus_type()==PV_TYPE);
    TEST_ASSERT(bus->get_area_number()==5);
    TEST_ASSERT(bus->get_zone_number()==6);
    TEST_ASSERT(bus->get_owner_number()==5);
}
*/
void BPA_IMEXPORTER_TEST::test_load_load_and_fixed_shunt_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<LOAD*> loads = psdb.get_all_loads();

    osstream<<"Load count: "<<loads.size()<<endl;
    show_information_with_leading_time_stamp(osstream);
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
        loads[i]->report();

    vector<FIXED_SHUNT*> fshunts = psdb.get_all_fixed_shunts();

    osstream<<"Fixed shunt count: "<<fshunts.size()<<endl;
    show_information_with_leading_time_stamp(osstream);
    n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
        fshunts[i]->report();
}

void BPA_IMEXPORTER_TEST::test_load_fixed_shunt_data()
{
    ;
}

void BPA_IMEXPORTER_TEST::test_load_generator_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<GENERATOR*> generators = psdb.get_all_generators();

    osstream<<"Generator count: "<<generators.size()<<endl;
    show_information_with_leading_time_stamp(osstream);

    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
        generators[i]->report();

    return;

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

void BPA_IMEXPORTER_TEST::test_load_wt_generator_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    size_t n = wt_generators.size();

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
    //TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-2.193034)<FLOAT_EPSILON); // this one is the constant pf Q
    TEST_ASSERT(fabs(wt_generator->get_p_max_in_MW()-10.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_p_min_in_MW()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_max_in_MVar()-2.193)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(wt_generator->get_q_min_in_MVar()-(-2.193))<FLOAT_EPSILON);

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

void BPA_IMEXPORTER_TEST::test_load_line_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();

    osstream<<"Line count: "<<lines.size()<<endl;
    show_information_with_leading_time_stamp(osstream);
    size_t n = lines.size();
    for(size_t i=0; i!=n; ++i)
        lines[i]->report();

    return;

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

void BPA_IMEXPORTER_TEST::test_load_transformer_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<TRANSFORMER*> trans = psdb.get_all_transformers();

    osstream<<"Transformer count: "<<trans.size()<<endl;
    show_information_with_leading_time_stamp(osstream);
    size_t n = trans.size();
    for(size_t i=0; i!=n; ++i)
        trans[i]->report();
}

void BPA_IMEXPORTER_TEST::test_load_hvdc_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t n = hvdcs.size();

    //TEST_ASSERT(n==2);

    for(size_t i=0; i!=n; ++i)
        hvdcs[i]->report();
}


void BPA_IMEXPORTER_TEST::test_convert_data_into_bpa_format()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    TEST_ASSERT(importer.convert_data_into_bpa_format("ABC", "A3")=="ABC");
    TEST_ASSERT(importer.convert_data_into_bpa_format("AB", "A3")=="AB ");
    TEST_ASSERT(importer.convert_data_into_bpa_format("ABCD", "A3")=="ABC");

    TEST_ASSERT(importer.convert_data_into_bpa_format(size_t(1), "I3")=="  1");
    TEST_ASSERT(importer.convert_data_into_bpa_format(size_t(10), "I3")==" 10");
    TEST_ASSERT(importer.convert_data_into_bpa_format(size_t(999), "I3")=="999");

    TEST_ASSERT(importer.convert_data_into_bpa_format(int(1), "I3")=="  1");
    TEST_ASSERT(importer.convert_data_into_bpa_format(int(10), "I3")==" 10");
    TEST_ASSERT(importer.convert_data_into_bpa_format(int(999), "I3")=="999");
    TEST_ASSERT(importer.convert_data_into_bpa_format(int(-1), "I3")==" -1");
    TEST_ASSERT(importer.convert_data_into_bpa_format(int(-10), "I3")=="-10");
    TEST_ASSERT(importer.convert_data_into_bpa_format(int(-99), "I3")=="-99");

    TEST_ASSERT(importer.convert_data_into_bpa_format(1.23, "F5.0")==" 1.23");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1.23, "F5.1")==" 1.23");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1.23, "F5.2")==" 1.23");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1.23, "F5.3")==" 1.23");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1.23, "F5.4")==" 1.23");

    TEST_ASSERT(importer.convert_data_into_bpa_format(12.3456, "F5.0")=="12.34");
    TEST_ASSERT(importer.convert_data_into_bpa_format(12.3456, "F5.1")=="12.34");
    TEST_ASSERT(importer.convert_data_into_bpa_format(12.3456, "F5.2")=="12.34");
    TEST_ASSERT(importer.convert_data_into_bpa_format(12.3456, "F5.3")=="12.34");
    TEST_ASSERT(importer.convert_data_into_bpa_format(12.3456, "F5.4")=="12.34");

    TEST_ASSERT(importer.convert_data_into_bpa_format(123.456, "F5.0")=="123.4");
    TEST_ASSERT(importer.convert_data_into_bpa_format(123.456, "F5.1")=="123.4");
    TEST_ASSERT(importer.convert_data_into_bpa_format(123.456, "F5.2")=="123.4");
    TEST_ASSERT(importer.convert_data_into_bpa_format(123.456, "F5.3")=="123.4");
    TEST_ASSERT(importer.convert_data_into_bpa_format(123.456, "F5.4")=="123.4");

    TEST_ASSERT(importer.convert_data_into_bpa_format(1234.56, "F5.0")=="1234.");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1234.56, "F5.1")=="1234.");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1234.56, "F5.2")=="1234.");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1234.56, "F5.3")=="1234.");
    TEST_ASSERT(importer.convert_data_into_bpa_format(1234.56, "F5.4")=="1234.");

    TEST_ASSERT(importer.convert_data_into_bpa_format(12345.6, "F5.0")=="12345");
}


void BPA_IMEXPORTER_TEST::test_export_powerflow_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.clear_database();
    //importer.load_powerflow_data("")
    PSSE_IMEXPORTER psse_importer;
    psse_importer.load_powerflow_data("bench_yunnan.raw");

    //psse_importer.load_powerflow_data("sample.raw");

    //psse_importer.export_powerflow_data("export_sample_model_with_PSSE_IMEXPORTER.raw");

    importer.export_powerflow_data("test_log/export_sd_model_with_BPA_IMEXPORTER.dat");
    psse_importer.export_powerflow_data("test_log/export_sd_model_with_PSSE_IMEXPORTER.raw");

    psdb.clear_database();
    importer.load_powerflow_data("云南网500kV简化网络.dat");
    importer.export_powerflow_data("test_log/export_yn_model_with_BPA_IMEXPORTER.dat");
    psse_importer.export_powerflow_data("test_log/export_yn_model_with_PSSE_IMEXPORTER.raw");

}

void BPA_IMEXPORTER_TEST::test_export_powerflow_data_imported_from_psse()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    importer.export_powerflow_data("test_log/export_sample_model_with_BPA_IMEXPORTER.raw");
}

void BPA_IMEXPORTER_TEST::test_export_powerflow_data_imported_from_bpa()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

    importer.export_powerflow_data("test_log/export_sample_model_with_BPA_IMEXPORTER.raw");
}

void BPA_IMEXPORTER_TEST::test_load_dynamic_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"BPA_IMEXPORTER_TEST");

}

