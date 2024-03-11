#include "header/basic/test_macro.h"
#include "header/model/pvu_models/pv_electrical_model/pv_electrical_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;

PV_ELECTRICAL_MODEL_TEST::PV_ELECTRICAL_MODEL_TEST()
{
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_get_model_type);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_get_pv_unit_terminal_generation);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_voltage);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_frequency);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_bus_to_regulate);

    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_frequency_reference);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_active_power_reference);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_reactive_power_reference);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_power_factor_reference);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_set_get_var_control_mode);

    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_get_standard_psse_string);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_step_response_with_voltage_drop);
    TEST_ADD(PV_ELECTRICAL_MODEL_TEST::test_step_response_with_frequency_drop);
}

void PV_ELECTRICAL_MODEL_TEST::setup()
{
    PVU_MODEL_TEST::setup();
}

void PV_ELECTRICAL_MODEL_TEST::tear_down()
{
    PVU_MODEL_TEST::tear_down();
    show_test_end_information();
}

void PV_ELECTRICAL_MODEL_TEST::test_get_model_type()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="PV ELECTRICAL");
}

void PV_ELECTRICAL_MODEL_TEST::test_get_pv_unit_terminal_generation()
{
    PV_CONVERTER_MODEL* pvc_model = get_test_pv_converter_model();
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    pvc_model->initialize();

    TEST_ASSERT(abs(model->get_pv_unit_terminal_generation_in_MVA()-pvc_model->get_terminal_complex_power_in_MVA())<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_voltage()
{
    PV_UNIT* pvuptr = get_test_pv_unit();
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    unsigned int bus = pvuptr->get_unit_bus();

    TEST_ASSERT(fabs(model->get_terminal_bus_voltage_in_pu()-psdb.get_bus_positive_sequence_voltage_in_pu(bus))<FLOAT_EPSILON);
    BUS* busptr = psdb.get_bus(bus);
    busptr->set_positive_sequence_voltage_in_pu(1.1);
    TEST_ASSERT(fabs(model->get_terminal_bus_voltage_in_pu()-1.1)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_frequency()
{
    PV_UNIT* pvuptr = get_test_pv_unit();
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    unsigned int bus = pvuptr->get_unit_bus();

    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_deviation_in_pu()-psdb.get_bus_frequency_deviation_in_pu(bus))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_in_pu()-psdb.get_bus_frequency_in_pu(bus))<FLOAT_EPSILON);
    BUS* busptr = psdb.get_bus(bus);
    BUS_FREQUENCY_MODEL* freqmodel = busptr->get_bus_frequency_model();
    freqmodel->set_frequency_deviation_in_pu(0.1);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_deviation_in_pu()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_in_pu()-1.1)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_bus_to_regulate()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_bus_to_regulate(1);
    TEST_ASSERT(model->get_bus_to_regulate()==1);
    model->set_bus_to_regulate(2);
    TEST_ASSERT(model->get_bus_to_regulate()==2);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_voltage_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_frequency_reference()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_frequency_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_frequency_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_active_power_reference()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_active_power_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_active_power_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_reactive_power_reference()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_reactive_power_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_reactive_power_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_power_factor_reference()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_power_factor_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_power_factor_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void PV_ELECTRICAL_MODEL_TEST::test_set_get_var_control_mode()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_var_control_mode(CONSTANT_POWER_FACTOR_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_POWER_FACTOR_MODE);

    model->set_var_control_mode(CONSTANT_VOLTAGE_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_VOLTAGE_MODE);

    model->set_var_control_mode(CONSTANT_VAR_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_VAR_MODE);
}


void PV_ELECTRICAL_MODEL_TEST::test_step_response_with_voltage_drop()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    initialize_models();
    run_to_time(1.0);
    apply_voltage_drop_of_10_percent();
    run_to_time(6.0);

    default_toolkit.close_log_file();
}

void PV_ELECTRICAL_MODEL_TEST::test_step_response_with_frequency_drop()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    initialize_models();
    run_to_time(1.0);
    apply_voltage_drop_of_10_percent();
    run_to_time(6.0);

    default_toolkit.close_log_file();
}

void PV_ELECTRICAL_MODEL_TEST::initialize_models()
{
    PV_CONVERTER_MODEL* pvc_model = get_test_pv_converter_model();
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
    default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);
    pvc_model->initialize();
    model->initialize();
    export_meter_title();
    export_meter_values();
}

void PV_ELECTRICAL_MODEL_TEST::run_to_time(double tend)
{
    ostringstream osstream;

    PV_CONVERTER_MODEL* pvc_model = get_test_pv_converter_model();
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();

    double delt =default_toolkit.get_dynamic_simulation_time_step_in_s();
    while(true)
    {
        default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()+delt);
        if(default_toolkit.get_dynamic_simulation_time_in_s()>tend)
        {
            default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-delt);
            break;
        }
        double ip=0.0, iq=0.0;
        unsigned int iter_count = 0;
        while(true)
        {
            model->run(INTEGRATE_MODE);
            pvc_model->run(INTEGRATE_MODE);
            if(fabs(ip-model->get_active_current_command_in_pu_based_on_mbase())<1e-6 and
                fabs(iq-model->get_reactive_current_command_in_pu_based_on_mbase())<1e-6)
                break;

            ip = model->get_active_current_command_in_pu_based_on_mbase();
            iq = model->get_reactive_current_command_in_pu_based_on_mbase();
            iter_count++;
        }
        //osstream<<"Integration at time "<<default_toolkit.get_dynamic_simulation_time_in_s()<<", takes "<<iter_count<<" iterations.";
        //show_information_with_leading_time_stamp(osstream);
        model->run(UPDATE_MODE);
        pvc_model->run(UPDATE_MODE);
        export_meter_values();
   }
}

void PV_ELECTRICAL_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT\tFREQ\tIPCMD\tIQCMD\tEQCMD\tPELEC\tQELEC";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void PV_ELECTRICAL_MODEL_TEST::export_meter_values()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);

    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    PV_CONVERTER_MODEL* pvc_model = get_test_pv_converter_model();

    double voltage = bus->get_positive_sequence_voltage_in_pu();
    double freq = bus->get_frequency_deviation_in_pu();

    ostringstream osstream;
    osstream<<setw(10)<<setprecision(6)<<fixed<<default_toolkit.get_dynamic_simulation_time_in_s()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<voltage<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_active_current_command_in_pu_based_on_mbase()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_reactive_current_command_in_pu_based_on_mbase()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_reactive_voltage_command_in_pu_based_on_mbase()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<pvc_model->get_terminal_active_power_in_MW()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<pvc_model->get_terminal_reactive_power_in_MVar();
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void PV_ELECTRICAL_MODEL_TEST::apply_voltage_drop_of_10_percent()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);
    bus->set_positive_sequence_voltage_in_pu(bus->get_positive_sequence_voltage_in_pu()-0.1);
}

void PV_ELECTRICAL_MODEL_TEST::apply_frequency_drop_of_5_percent()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);
    BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
    model->set_frequency_deviation_in_pu(0.05);
}

void PV_ELECTRICAL_MODEL_TEST::test_get_standard_psse_string()
{
    PV_ELECTRICAL_MODEL* model = get_test_pv_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.show_information_with_leading_time_stamp(model->get_standard_psse_string());
}

#endif
