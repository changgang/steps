#include "header/model/load_relay_model/load_frequency_relay_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
LOAD_FREQUENCY_RELAY_MODEL_TEST::LOAD_FREQUENCY_RELAY_MODEL_TEST() : LOAD_RELAY_MODEL_TEST()
{
    TEST_ADD(LOAD_FREQUENCY_RELAY_MODEL_TEST::test_model_type);
    TEST_ADD(LOAD_FREQUENCY_RELAY_MODEL_TEST::test_get_bus_frequency);
    TEST_ADD(LOAD_FREQUENCY_RELAY_MODEL_TEST::test_run);
}



void LOAD_FREQUENCY_RELAY_MODEL_TEST::setup()
{
    LOAD_RELAY_MODEL_TEST::setup();
}

void LOAD_FREQUENCY_RELAY_MODEL_TEST::tear_down()
{
    LOAD_RELAY_MODEL_TEST::tear_down();
}

void LOAD_FREQUENCY_RELAY_MODEL_TEST::test_model_type()
{
    LOAD* load = get_load();
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="LOAD FREQUENCY RELAY");
}

void LOAD_FREQUENCY_RELAY_MODEL_TEST::test_get_bus_frequency()
{
    LOAD* load = get_load();
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    size_t bus = load->get_load_bus();
    BUS* busptr = psdb.get_bus(bus);

    BUS_FREQUENCY_MODEL* bus_model = busptr->get_bus_frequency_model();

    bus_model->set_frequency_deviation_in_pu(0.0);
    TEST_ASSERT(fabs(model->get_bus_frequency_in_Hz()-50.0*(1.0-0.0))<FLOAT_EPSILON);

    bus_model->set_frequency_deviation_in_pu(-0.01);
    TEST_ASSERT(fabs(model->get_bus_frequency_in_Hz()-50.0*(1.0-0.01))<FLOAT_EPSILON);

    bus_model->set_frequency_deviation_in_pu(-0.05);
    TEST_ASSERT(fabs(model->get_bus_frequency_in_Hz()-50.0*(1.0-0.05))<FLOAT_EPSILON);

    bus_model->set_frequency_deviation_in_pu(0.05);
    TEST_ASSERT(fabs(model->get_bus_frequency_in_Hz()-50.0*(1.0+0.05))<FLOAT_EPSILON);
}


void LOAD_FREQUENCY_RELAY_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tFREQ\tSHED_SCALE";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void LOAD_FREQUENCY_RELAY_MODEL_TEST::export_meter_values()
{
    LOAD* load = get_load();
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();

    ostringstream osstream;
    osstream<<setprecision(6)<<fixed<<default_toolkit.get_dynamic_simulation_time_in_s()<<"\t"
      <<setprecision(6)<<fixed<<model->get_bus_frequency_in_Hz()<<"\t"
      <<setprecision(6)<<fixed<<model->get_total_shed_scale_factor_in_pu()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}


void LOAD_FREQUENCY_RELAY_MODEL_TEST::test_run()
{
    LOAD* load = get_load();
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();

    string outputfile = "test_log/frequency_ramp_response_of_"+model->get_model_name()+"_model.txt";
    run_model(outputfile);
}

void LOAD_FREQUENCY_RELAY_MODEL_TEST::run_model(string outputfile)
{
    LOAD* load = get_load();
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();

    default_toolkit.redirect_stdout_to_file(outputfile);

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    double fbase = psdb.get_bus_base_frequency_in_Hz(load->get_load_bus());

    ostringstream osstream;

    osstream<<"Model:"<<model->get_standard_model_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);

    size_t bus = load->get_load_bus();
    BUS* busptr = psdb.get_bus(bus);
    BUS_FREQUENCY_MODEL* freq_model = busptr->get_bus_frequency_model();

    freq_model->initialize();

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

    double freq = freq_model->get_frequency_in_Hz();
    double rate = 0.5; // Hz/s

    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);

        if(default_toolkit.get_dynamic_simulation_time_in_s()>7.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        freq -= (rate*delt);
        freq_model->set_frequency_deviation_in_pu(freq/fbase-1.0);

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
        freq += (rate*delt);
        freq_model->set_frequency_deviation_in_pu(freq/fbase-1.0);

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
        freq -= (rate*delt);
        freq_model->set_frequency_deviation_in_pu(freq/fbase-1.0);

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
        freq += (rate*delt);
        freq_model->set_frequency_deviation_in_pu(freq/fbase-1.0);

        if(default_toolkit.get_dynamic_simulation_time_in_s()>18.0+FLOAT_EPSILON)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            freq -= (rate*delt);
            freq_model->set_frequency_deviation_in_pu(freq/fbase-1.0);
            break;
        }
        model->run(INTEGRATE_MODE);
        model->run(UPDATE_MODE);

        export_meter_values();
    }

    default_toolkit.recover_stdout();
}
