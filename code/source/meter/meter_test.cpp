#include "header/meter/meter_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/meter/meter.h"
#include "header/model/all_supported_models.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>


using namespace std;

METER_TEST::METER_TEST()
{
    TEST_ADD(METER_TEST::test_constructor);
    TEST_ADD(METER_TEST::test_set_get_device_id_and_type);
    TEST_ADD(METER_TEST::test_set_get_buffer_size);
    TEST_ADD(METER_TEST::test_set_get_bus_meter_type);
    TEST_ADD(METER_TEST::test_set_get_line_meter_type);
    TEST_ADD(METER_TEST::test_set_get_transformer_meter_type);
    TEST_ADD(METER_TEST::test_set_get_generator_meter_type);
    TEST_ADD(METER_TEST::test_set_get_load_meter_type);
    TEST_ADD(METER_TEST::test_set_get_hvdc_meter_type);
    TEST_ADD(METER_TEST::test_set_get_wt_generator_meter_type);
    TEST_ADD(METER_TEST::test_set_get_equivalent_device_meter_type);
    TEST_ADD(METER_TEST::test_set_get_energy_storage_meter_type);
    TEST_ADD(METER_TEST::test_set_get_meter_internal_variable_name);
    TEST_ADD(METER_TEST::test_set_get_device_pointer);
    TEST_ADD(METER_TEST::test_clear);
    TEST_ADD(METER_TEST::test_is_valid);
    TEST_ADD(METER_TEST::test_copy_with_operator_equal);
    TEST_ADD(METER_TEST::test_equal);

    TEST_ADD(METER_TEST::test_get_bus_meter_value);
    TEST_ADD(METER_TEST::test_get_equivalent_device_meter_value);
}

void METER_TEST::setup()
{
    db = get_default_power_system_database();

    BUS bus;
    bus.set_bus_number(1);
    bus.set_base_voltage_in_kV(110.0);
    bus.set_bus_type(PQ_TYPE);
    db->append_bus(bus);
    bus.set_bus_number(2);
    bus.set_base_voltage_in_kV(110.0);
    db->append_bus(bus);
    bus.set_bus_number(3);
    bus.set_base_voltage_in_kV(110.0);
    db->append_bus(bus);

    LINE line(db);
    line.set_sending_side_bus(1);
    line.set_receiving_side_bus(2);
    line.set_identifier("#1");
    db->append_line(line);

    TRANSFORMER transformer(db);
    transformer.set_winding_bus(PRIMARY_SIDE, 1);
    transformer.set_winding_bus(SECONDARY_SIDE, 2);
    transformer.set_identifier("#1");
    db->append_transformer(transformer);

    transformer.set_winding_bus(TERTIARY_SIDE, 3);
    db->append_transformer(transformer);

    GENERATOR generator(db);
    generator.set_generator_bus(1);
    generator.set_identifier("#1");
    db->append_generator(generator);
    GENCLS model;
    model.set_H_in_s(3.0);
    db->append_dynamic_model(get_generator_device_id(1,"#1"),&model);

    LOAD load(db);
    load.set_load_bus(1);
    load.set_identifier("#1");
    db->append_load(load);

    WT_GENERATOR wt_generator(db);
    wt_generator.set_source_bus(1);
    wt_generator.set_identifier("#1");
    db->append_wt_generator(wt_generator);

    HVDC hvdc(db);
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("#1");
    db->append_hvdc(hvdc);

    EQUIVALENT_DEVICE edevice(db);
    edevice.set_equivalent_device_bus(1);
    edevice.set_identifier("#1");
    edevice.set_status(true);
    db->append_equivalent_device(edevice);


    ENERGY_STORAGE estorage(db);
    estorage.set_energy_storage_bus(1);
    estorage.set_identifier("#1");
    estorage.set_status(true);
    db->append_energy_storage(estorage);


    meter = new METER(db);
}

void METER_TEST::tear_down()
{
    delete meter;
    meter = NULL;

    db->clear_database();

    show_test_end_information();
}

void METER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    TEST_ASSERT(meter->get_device_type()=="NONE");
    TEST_ASSERT(meter->get_meter_type()=="");
    TEST_ASSERT(meter->get_device_pointer()==NULL);
    TEST_ASSERT(meter->is_valid()==false);
}

void METER_TEST::test_set_get_device_id_and_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="BUS");

    did.set_device_type("LOAD");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="LOAD");

    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="LINE");

    did.set_device_type("TRANSFORMER");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="TRANSFORMER");

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="GENERATOR");

    did.set_device_type("WT GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="WT GENERATOR");

    did.set_device_type("HVDC");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");
    meter->set_device_id(did);
    TEST_ASSERT(meter->get_device_type()=="HVDC");
}

void METER_TEST::test_set_get_buffer_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    TEST_ASSERT(meter->get_buffer_size()==1);

    meter->set_buffer_size(10);
    TEST_ASSERT(meter->get_buffer_size()==10);
}

void METER_TEST::test_set_get_bus_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("BUS");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    size_t n = bus_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = bus_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_line_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("LINE");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = line_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = line_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_transformer_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = transformer_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = transformer_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }

    terminal.append_bus(3);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    for(size_t i=0; i!=n; ++i)
    {
        meter_type = transformer_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_generator_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("GENERATOR");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = generator_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = generator_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_load_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("LOAD");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = load_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = load_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_hvdc_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("HVDC");
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = hvdc_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = hvdc_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}


void METER_TEST::test_set_get_wt_generator_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("WT GENERATOR");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = wt_generator_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = wt_generator_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_equivalent_device_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("EQUIVALENT DEVICE");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = equivalent_device_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = equivalent_device_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}


void METER_TEST::test_set_get_energy_storage_meter_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    TERMINAL terminal;
    string meter_type;

    did.set_device_type("ENERGY STORAGE");
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    size_t n = energy_storage_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        meter_type = energy_storage_meters[i];
        meter->set_meter_type(meter_type);
        TEST_ASSERT(meter->get_meter_type()==meter_type);
    }
}

void METER_TEST::test_set_get_meter_internal_variable_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);
    meter->set_meter_type("VOLTAGE IN PU");

    TEST_ASSERT(meter->get_internal_variable_name()=="");

    meter->set_meter_type("VOLTAGE IN PU");
    //meter->set_internal_variable_name("abc");

    TEST_ASSERT(meter->get_internal_variable_name()=="");

    did.set_device_type("GENERATOR");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);
    meter->set_meter_type("ROTOR ANGLE IN DEG");
    TEST_ASSERT(meter->get_internal_variable_name()=="");
    meter->set_meter_type("SYNC GENERATOR MODEL INTERNAL VARIABLE");
    meter->set_internal_variable_name("ROTOR ANGLE IN DEG");
    TEST_ASSERT(meter->get_internal_variable_name()=="ROTOR ANGLE IN DEG");
    meter->set_internal_variable_name("STATE@ROTOR ANGLE BLOCK");
    TEST_ASSERT(meter->get_internal_variable_name()=="STATE@ROTOR ANGLE BLOCK");

}

void METER_TEST::test_set_get_device_pointer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");


    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);



    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);

    meter->clear();
    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);


    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);

    meter->clear();
    did.set_device_type("TRANSFORMER");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);

    meter->clear();
    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);

    meter->clear();
    did.set_device_type("WT GENERATOR");
    terminal.clear();
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);

    meter->clear();
    did.set_device_type("HVDC");
    terminal.clear();
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);

    TEST_ASSERT(meter->get_device_pointer()!=NULL);
    TEST_ASSERT(meter->get_device_pointer()->get_device_id()==did);
}

void METER_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    meter->set_meter_type("VOLTAGE IN PU");

    TEST_ASSERT(meter->get_meter_type()=="VOLTAGE IN PU");

    meter->clear();

    TEST_ASSERT(meter->get_device_pointer()==NULL);
    TEST_ASSERT(meter->get_meter_type()=="");
}

void METER_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    meter->set_meter_type("VOLTAGE IN PU");

    TEST_ASSERT(meter->is_valid()==true);
    meter->set_meter_type("");
    TEST_ASSERT(meter->is_valid()==false);
}

void METER_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    meter->set_meter_type("VOLTAGE IN PU");

    METER newmeter = (*meter);

    TEST_ASSERT(newmeter.get_device_id()==did);
    TEST_ASSERT(newmeter.get_device_type()=="BUS");
    TEST_ASSERT(newmeter.get_device_pointer()==(DEVICE*) db->get_bus(1));
    TEST_ASSERT(newmeter.get_meter_type()=="VOLTAGE IN PU");
}

void METER_TEST::test_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);
    meter->set_meter_type("VOLTAGE IN PU");

    METER newmeter = (*meter);

    TEST_ASSERT(newmeter==(*meter));

    did.set_device_type("GENERATOR");
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    meter->set_device_id(did);
    meter->set_meter_type("ROTOR ANGLE IN DEG");

    newmeter = (*meter);
    TEST_ASSERT(newmeter==(*meter));

    meter->set_meter_type("SYNC GENERATOR MODEL INTERNAL VARIABLE");
    meter->set_internal_variable_name("STATE@ROTOR ANGLE BLOCK");

    newmeter = (*meter);
    TEST_ASSERT(newmeter==(*meter));

    meter->set_meter_type("SYNC GENERATOR MODEL INTERNAL VARIABLE");
    meter->set_internal_variable_name("STATE@ROTOR SPEED BLOCK");
    TEST_ASSERT(newmeter!=(*meter));
}

void METER_TEST::test_get_bus_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("BUS");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);

    meter->set_device_id(did);

    meter->set_meter_type("VOLTAGE IN PU");

    BUS* busptr = db->get_bus(1);
    busptr->set_voltage_in_pu(1.05);
    TEST_ASSERT(fabs(meter->get_meter_value()-1.05)<FLOAT_EPSILON);
}

void METER_TEST::test_get_line_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}
void METER_TEST::test_get_transformer_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}

void METER_TEST::test_get_load_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}

void METER_TEST::test_get_generator_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}

void METER_TEST::test_get_wt_generator_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}

void METER_TEST::test_get_hvdc_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");
}

void METER_TEST::test_get_equivalent_device_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    EQUIVALENT_DEVICE* edevice = db->get_equivalent_device(did);
    TEST_ASSERT(edevice!=NULL);

    edevice->set_equivalent_voltage_source_status(true);
    edevice->set_equivalent_load_status(true);

    edevice->set_equivalent_voltage_source_voltage_in_pu(1.0);
    edevice->set_equivalent_voltage_source_impedance_in_pu(complex<double>(1.0, 5.0));
    edevice->set_equivalent_nominal_constant_power_load_in_MVA(complex<double>(1.0, 5.0));
    edevice->set_equivalent_nominal_constant_current_load_in_MVA(complex<double>(10.0, 50.0));
    edevice->set_equivalent_nominal_constant_impedance_load_in_MVA(complex<double>(100.0, 500.0));

    meter->set_device_id(did);


    meter->set_meter_type("VOLTAGE SOURCE VOLTAGE IN PU");
    TEST_ASSERT(fabs(meter->get_meter_value()-abs(edevice->get_equivalent_voltage_source_voltage_in_pu()))<FLOAT_EPSILON);

    meter->set_meter_type("VOLTAGE SOURCE VOLTAGE ANGLE IN DEG");
    TEST_ASSERT(fabs(meter->get_meter_value()-rad2deg(arg(edevice->get_equivalent_voltage_source_voltage_in_pu())))<FLOAT_EPSILON);

    meter->set_meter_type("VOLTAGE SOURCE RESISTANCE IN PU");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_voltage_source_impedance_in_pu().real())<FLOAT_EPSILON);

    meter->set_meter_type("VOLTAGE SOURCE REACTANCE IN PU");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_voltage_source_impedance_in_pu().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE CONSTANT POWER LOAD IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_power_load_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE CONSTANT POWER LOAD IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_power_load_in_MVA().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE CONSTANT CURRENT LOAD IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_current_load_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE CONSTANT CURRENT LOAD IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_current_load_in_MVA().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE CONSTANT IMPEDANCE LOAD IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE POWER GENERATION IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_generation_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE POWER GENERATION IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_generation_in_MVA().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE POWER LOAD IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_load_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE POWER LOAD IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_equivalent_load_in_MVA().imag())<FLOAT_EPSILON);

    meter->set_meter_type("ACTIVE POWER NET LOAD IN MW");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_total_equivalent_power_as_load_in_MVA().real())<FLOAT_EPSILON);

    meter->set_meter_type("REACTIVE POWER NET LOAD IN MVAR");
    TEST_ASSERT(fabs(meter->get_meter_value()-edevice->get_total_equivalent_power_as_load_in_MVA().imag())<FLOAT_EPSILON);
}


void METER_TEST::test_get_energy_storage_meter_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"METER_TEST");

}


