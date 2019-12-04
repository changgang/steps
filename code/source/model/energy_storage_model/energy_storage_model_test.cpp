#include "header/basic/test_macro.h"
#include "header/model/energy_storage_model/energy_storage_model_test.h"
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

ENERGY_STORAGE_MODEL_TEST::ENERGY_STORAGE_MODEL_TEST()
{
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_get_model_type);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_set_get_max_power);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_set_get_inital_energy);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_set_get_nominal_energy);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_set_get_initial_Pref);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_set_get_initial_Vref);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_frequency_step_response);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_voltage_step_response);
    TEST_ADD(ENERGY_STORAGE_MODEL_TEST::test_get_standard_psse_string);
}



void ENERGY_STORAGE_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);

    psdb.append_bus(bus);

    ENERGY_STORAGE estorage;
    estorage.set_energy_storage_bus(1);
    estorage.set_identifier("#1");
    estorage.set_status(true);
    estorage.set_mbase_in_MVA(100.0);
    estorage.set_source_impedance_in_pu(complex<double>(0.01, 0.1));
    estorage.set_p_generation_in_MW(10.0);
    estorage.set_q_generation_in_MVar(0.0);

    psdb.append_energy_storage(estorage);
}

void ENERGY_STORAGE_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}


ENERGY_STORAGE* ENERGY_STORAGE_MODEL_TEST::get_test_energy_storage()
{
    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    return psdb.get_energy_storage(did);
}

ENERGY_STORAGE_MODEL* ENERGY_STORAGE_MODEL_TEST::get_test_energy_storage_model()
{
    ENERGY_STORAGE* esptr = get_test_energy_storage();
    if(esptr!=NULL)
        return esptr->get_energy_storage_model();
    else
        return NULL;
}


void ENERGY_STORAGE_MODEL_TEST::test_get_model_type()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        TEST_ASSERT(model->get_model_type()=="ENERGY STORAGE");
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_set_get_max_power()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_Pmax_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_Pmax_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_Pmax_in_pu(2.0);
        TEST_ASSERT(fabs(model->get_Pmax_in_pu()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_set_get_inital_energy()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_E0_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_E0_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_E0_in_pu(0.5);
        TEST_ASSERT(fabs(model->get_E0_in_pu()-0.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_set_get_nominal_energy()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_En_in_MWh(10.0);
        TEST_ASSERT(fabs(model->get_En_in_MWh()-10.0)<FLOAT_EPSILON);
        model->set_En_in_MWh(20.0);
        TEST_ASSERT(fabs(model->get_En_in_MWh()-20.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_set_get_initial_Pref()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_Pref_in_pu(0.0);
        TEST_ASSERT(fabs(model->get_Pref_in_pu()-0.0)<FLOAT_EPSILON);
        model->set_Pref_in_pu(0.5);
        TEST_ASSERT(fabs(model->get_Pref_in_pu()-0.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_set_get_initial_Vref()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_Vref_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_Vref_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_Vref_in_pu(0.5);
        TEST_ASSERT(fabs(model->get_Vref_in_pu()-0.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}


void ENERGY_STORAGE_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tFREQ\tVOLT\tPELEC\tQELEC\tENERGY";
    osstream<<osstream.str()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void ENERGY_STORAGE_MODEL_TEST::export_meter_values()
{
    ostringstream osstream;

    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();

    double freq = model->get_terminal_bus_frequency_deviation_in_pu();
    double volt = model->get_terminal_bus_voltage_in_pu();
    double pelec = model->get_terminal_active_power_in_MW();
    double qelec = model->get_terminal_reactive_power_in_MVar();
    double soc = model->get_energy_state_in_pu();

    osstream<<setw(6)<<setprecision(3)<<fixed<<default_toolkit.get_dynamic_simulation_time_in_s()<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<volt<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<qelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<soc;

    osstream<<osstream.str()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

}

void ENERGY_STORAGE_MODEL_TEST::test_frequency_step_response()
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);

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
            run_a_step();
            export_meter_values();
        }

        BUS* bus = psdb.get_bus(1);
        bus->set_frequency_deviation_in_pu(-0.02);
        model->run(UPDATE_MODE);

        export_meter_values();

        while(true)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);

            if(default_toolkit.get_dynamic_simulation_time_in_s()>6.0+FLOAT_EPSILON)
            {
                default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
                break;
            }

            run_a_step();
            export_meter_values();
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void ENERGY_STORAGE_MODEL_TEST::test_voltage_step_response()
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);

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
            run_a_step();
            export_meter_values();
        }

        BUS* bus = psdb.get_bus(1);
        bus->set_positive_sequence_voltage_in_pu(bus->get_positive_sequence_voltage_in_pu()-0.1);
        model->run(UPDATE_MODE);

        export_meter_values();

        while(true)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);

            if(default_toolkit.get_dynamic_simulation_time_in_s()>6.0+FLOAT_EPSILON)
            {
                default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
                break;
            }

            run_a_step();
            export_meter_values();
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}


void ENERGY_STORAGE_MODEL_TEST::run_a_step()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    double pelec = model->get_terminal_active_power_in_MW();
    while(true)
    {
        model->run(INTEGRATE_MODE);
        if(fabs(pelec-model->get_terminal_active_power_in_MW())>1e-6)
            pelec = model->get_terminal_active_power_in_MW();
        else
            break;
    }
    model->run(UPDATE_MODE);
}


void ENERGY_STORAGE_MODEL_TEST::test_get_standard_psse_string()
{
    ENERGY_STORAGE_MODEL* model = get_test_energy_storage_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        cout<<model->get_standard_psse_string()<<endl;
    }
    else
        TEST_ASSERT(false);
}

#endif
