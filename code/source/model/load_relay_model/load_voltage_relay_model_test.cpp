#include "header/basic/test_macro.h"
#include "header/model/load_relay_model/load_voltage_relay_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;
LOAD_VOLTAGE_RELAY_MODEL_TEST::LOAD_VOLTAGE_RELAY_MODEL_TEST() : LOAD_RELAY_MODEL_TEST()
{
    TEST_ADD(LOAD_VOLTAGE_RELAY_MODEL_TEST::test_model_type);
    TEST_ADD(LOAD_VOLTAGE_RELAY_MODEL_TEST::test_get_bus_voltage);
    TEST_ADD(LOAD_VOLTAGE_RELAY_MODEL_TEST::test_run);
}



void LOAD_VOLTAGE_RELAY_MODEL_TEST::setup()
{
    LOAD_RELAY_MODEL_TEST::setup();
}

void LOAD_VOLTAGE_RELAY_MODEL_TEST::tear_down()
{
    LOAD_RELAY_MODEL_TEST::tear_down();
}

void LOAD_VOLTAGE_RELAY_MODEL_TEST::test_model_type()
{
    LOAD* load = get_load();
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="LOAD VOLTAGE RELAY");
}

void LOAD_VOLTAGE_RELAY_MODEL_TEST::test_get_bus_voltage()
{
    LOAD* load = get_load();
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    size_t bus = load->get_load_bus();
    BUS* busptr = psdb.get_bus(bus);

    busptr->set_positive_sequence_voltage_in_pu(1.0);
    TEST_ASSERT(fabs(model->get_bus_positive_sequence_voltage_in_pu()-1)<FLOAT_EPSILON);

    busptr->set_positive_sequence_voltage_in_pu(1.1);
    TEST_ASSERT(fabs(model->get_bus_positive_sequence_voltage_in_pu()-1.1)<FLOAT_EPSILON);

    busptr->set_positive_sequence_voltage_in_pu(0.5);
    TEST_ASSERT(fabs(model->get_bus_positive_sequence_voltage_in_pu()-0.5)<FLOAT_EPSILON);
}


void LOAD_VOLTAGE_RELAY_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT\tSHED_SCALE";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void LOAD_VOLTAGE_RELAY_MODEL_TEST::export_meter_values()
{
    LOAD* load = get_load();
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();

    ostringstream osstream;
    osstream<<setprecision(6)<<fixed<<default_toolkit.get_dynamic_simulation_time_in_s()<<"\t"
      <<setprecision(6)<<fixed<<model->get_bus_positive_sequence_voltage_in_pu()<<"\t"
      <<setprecision(6)<<fixed<<model->get_total_shed_scale_factor_in_pu()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}


void LOAD_VOLTAGE_RELAY_MODEL_TEST::test_run()
{
    LOAD* load = get_load();
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();

    string outputfile = "test_log/voltage_ramp_response_of_"+model->get_model_name()+"_model.txt";
    run_model(outputfile);
}

void LOAD_VOLTAGE_RELAY_MODEL_TEST::run_model(string outputfile)
{
    LOAD* load = get_load();
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();

    default_toolkit.open_log_file(outputfile);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);

    size_t bus = load->get_load_bus();
    BUS* busptr = psdb.get_bus(bus);
    busptr->set_positive_sequence_voltage_in_pu(1.0);

    model->initialize();
    export_meter_title();
    export_meter_values();

    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);
        if(default_toolkit.get_dynamic_simulation_time_in_s()>1.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    double volt = busptr->get_positive_sequence_voltage_in_pu();
    double rate = 0.2; // pu/s

    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);

        if(default_toolkit.get_dynamic_simulation_time_in_s()>7.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        volt -= (rate*delt);
        busptr->set_positive_sequence_voltage_in_pu(volt);

        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }
    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);

        if(default_toolkit.get_dynamic_simulation_time_in_s()>8.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        volt += (rate*delt);
        busptr->set_positive_sequence_voltage_in_pu(volt);

        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }
    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);
        if(default_toolkit.get_dynamic_simulation_time_in_s()>10.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        volt -= (rate*delt);
        busptr->set_positive_sequence_voltage_in_pu(volt);

        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);
        if(default_toolkit.get_dynamic_simulation_time_in_s()>11.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);
        volt += (rate*delt);
        busptr->set_positive_sequence_voltage_in_pu(volt);

        if(default_toolkit.get_dynamic_simulation_time_in_s()>18.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            volt -= (rate*delt);
            busptr->set_positive_sequence_voltage_in_pu(volt);
            break;
        }
        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    default_toolkit.close_log_file();
}

#endif
