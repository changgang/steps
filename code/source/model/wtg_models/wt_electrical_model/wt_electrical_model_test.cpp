#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/wtg_models/wt_generator_model/wt3g0.h"
#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include "header/model/wtg_models/wt_turbine_model/wt3t0.h"

using namespace std;
WT_ELECTRICAL_MODEL_TEST::WT_ELECTRICAL_MODEL_TEST()
{
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_terminal_generation);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_voltage);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_frequency);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_bus_to_regulate);

    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_frequency_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_active_power_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_reactive_power_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_power_factor_reference);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_set_get_var_control_mode);

    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_speed);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_speed_reference);

    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_get_standard_psse_string);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_step_response_with_voltage_drop);
    TEST_ADD(WT_ELECTRICAL_MODEL_TEST::test_step_response_with_frequency_drop);
}

void WT_ELECTRICAL_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();
    wt_gen->set_p_generation_in_MW(20.0);
    wt_gen->set_q_generation_in_MVar(5.0);
    wt_gen->set_rated_power_per_wt_generator_in_MW(1.5);
    wt_gen->set_number_of_lumped_wt_generators(20);

    WT3G0 genmodel;
    genmodel.set_converter_activer_current_command_T_in_s(0.2);
    genmodel.set_converter_reactiver_voltage_command_T_in_s(0.2);
    genmodel.set_KPLL(20.0);
    genmodel.set_KIPLL(10.0);
    genmodel.set_PLLmax(0.1);
    LVPL lvpl;
    lvpl.set_low_voltage_in_pu(0.5);
    lvpl.set_high_voltage_in_pu(0.8);
    lvpl.set_gain_at_high_voltage(20.0);
    genmodel.set_LVPL(lvpl);
    genmodel.set_HVRC_voltage_in_pu(0.8);
    genmodel.set_HVRC_current_in_pu(20.0);
    genmodel.set_LVPL_max_rate_of_active_current_change(0.2);
    genmodel.set_LVPL_voltage_sensor_T_in_s(0.1);

    wt_gen->set_model(&genmodel);

    AERD0 aeromodel;
    aeromodel.set_number_of_pole_pairs(2);
    aeromodel.set_generator_to_turbine_gear_ratio(100.0);
    aeromodel.set_gear_efficiency(1.0);
    aeromodel.set_turbine_blade_radius_in_m(25.0);
    aeromodel.set_nominal_wind_speed_in_mps(13.0);
    aeromodel.set_nominal_air_density_in_kgpm3(1.25);
    aeromodel.set_air_density_in_kgpm3(1.25);
    aeromodel.set_turbine_speed_mode(WT_UNDERSPEED_MODE);

    aeromodel.set_C1(0.22);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(12.5);
    aeromodel.set_C6(0.0);

    aeromodel.set_C1(0.5176);
    aeromodel.set_C2(116.0);
    aeromodel.set_C3(0.4);
    aeromodel.set_C4(5.0);
    aeromodel.set_C5(21.0);
    aeromodel.set_C6(0.0068);

    wt_gen->set_model(&aeromodel);

    WT3T0 model;
    model.set_Hturbine_in_s(5.0);
    model.set_Hgenerator_in_s(3.0);
    model.set_Kshaft_in_pu(20.0);
    model.set_damping_in_pu(0.01);
    model.set_Dshaft_in_pu(1.0);

    wt_gen->set_model(&model);
}

void WT_ELECTRICAL_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
    show_test_end_information();
}

void WT_ELECTRICAL_MODEL_TEST::test_get_model_type()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    TEST_ASSERT(model->get_model_type()=="WT ELECTRICAL");
}

void WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_terminal_generation()
{
    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    wtgenmodel->initialize();

    TEST_ASSERT(abs(model->get_wt_generator_terminal_generation_in_MVA()-wtgenmodel->get_terminal_complex_power_in_MVA())<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_voltage()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    size_t bus = genptr->get_generator_bus();

    TEST_ASSERT(fabs(model->get_terminal_bus_voltage_in_pu()-psdb.get_bus_voltage_in_pu(bus))<FLOAT_EPSILON);
    BUS* busptr = psdb.get_bus(bus);
    busptr->set_voltage_in_pu(1.1);
    TEST_ASSERT(fabs(model->get_terminal_bus_voltage_in_pu()-1.1)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_get_terminal_bus_frequency()
{
    WT_GENERATOR* genptr = get_test_wt_generator();
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    size_t bus = genptr->get_generator_bus();

    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_deviation_in_pu()-psdb.get_bus_frequency_deviation_in_pu(bus))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_in_pu()-psdb.get_bus_frequency_in_pu(bus))<FLOAT_EPSILON);
    BUS* busptr = psdb.get_bus(bus);
    BUS_FREQUENCY_MODEL* freqmodel = busptr->get_bus_frequency_model();
    freqmodel->set_frequency_deviation_in_pu(0.1);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_deviation_in_pu()-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_terminal_bus_frequency_in_pu()-1.1)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_bus_to_regulate()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_bus_to_regulate(1);
    TEST_ASSERT(model->get_bus_to_regulate()==1);
    model->set_bus_to_regulate(2);
    TEST_ASSERT(model->get_bus_to_regulate()==2);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_voltage_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_voltage_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_voltage_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_frequency_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_frequency_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_frequency_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_active_power_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_active_power_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_active_power_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_reactive_power_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_reactive_power_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_reactive_power_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_power_factor_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_power_factor_reference_in_pu(1.5);
    TEST_ASSERT(fabs(model->get_power_factor_reference_in_pu()-1.5)<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_set_get_var_control_mode()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    model->set_var_control_mode(CONSTANT_POWER_FACTOR_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_POWER_FACTOR_MODE);

    model->set_var_control_mode(CONSTANT_VOLTAGE_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_VOLTAGE_MODE);

    model->set_var_control_mode(CONSTANT_VAR_MODE);
    TEST_ASSERT(model->get_var_control_mode()==CONSTANT_VAR_MODE);
}

void WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_speed()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    WT_TURBINE_MODEL* turbine = get_test_wt_turbine_model();

    TEST_ASSERT(fabs(model->get_wt_generator_speed_in_pu()-turbine->get_generator_speed_in_pu())<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_get_wt_generator_speed_reference()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    WT_AERODYNAMIC_MODEL* aerd = get_test_wt_aerodynamic_model();

    TEST_ASSERT(fabs(model->get_wt_generator_speed_referance_in_pu()-aerd->get_turbine_reference_speed_in_pu())<FLOAT_EPSILON);
}

void WT_ELECTRICAL_MODEL_TEST::test_step_response_with_voltage_drop()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    initialize_models();
    run_to_time(1.0);
    apply_voltage_drop_of_10_percent();
    run_to_time(6.0);

    default_toolkit.close_log_file();
}

void WT_ELECTRICAL_MODEL_TEST::test_step_response_with_frequency_drop()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();

    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/step_response_of_"+model->get_model_name()+"_model.txt");

    initialize_models();
    run_to_time(1.0);
    apply_voltage_drop_of_10_percent();
    run_to_time(6.0);

    default_toolkit.close_log_file();
}

void WT_ELECTRICAL_MODEL_TEST::initialize_models()
{
    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
    default_toolkit.set_dynamic_simulation_time_in_s(default_toolkit.get_dynamic_simulation_time_in_s()-2.0*delt);
    wtgenmodel->initialize();
    model->initialize();
    export_meter_title();
    export_meter_values();
}

void WT_ELECTRICAL_MODEL_TEST::run_to_time(double tend)
{
    ostringstream oosstream;

    WT_GENERATOR_MODEL* wtgenmodel = get_test_wt_generator_model();
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();

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
        size_t iter_count = 0;
        while(true)
        {
            model->run(INTEGRATE_MODE);
            wtgenmodel->run(INTEGRATE_MODE);
            if(fabs(ip-model->get_active_current_command_in_pu_based_on_mbase())<1e-6 and
                fabs(iq-model->get_reactive_current_command_in_pu_based_on_mbase())<1e-6)
                break;

            ip = model->get_active_current_command_in_pu_based_on_mbase();
            iq = model->get_reactive_current_command_in_pu_based_on_mbase();
            iter_count++;
        }
        //oosstream<<"Integration at time "<<default_toolkit.get_dynamic_simulation_time_in_s()<<", takes "<<iter_count<<" iterations.";
        //show_information_with_leading_time_stamp(oosstream);
        model->run(UPDATE_MODE);
        wtgenmodel->run(UPDATE_MODE);
        export_meter_values();
   }
}

void WT_ELECTRICAL_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT\tFREQ\tIPCMD\tIQCMD\tEQCMD\tPELEC\tQELEC";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT_ELECTRICAL_MODEL_TEST::export_meter_values()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);

    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    WT_GENERATOR_MODEL* genmodel = get_test_wt_generator_model();

    double voltage = bus->get_voltage_in_pu();
    double freq = bus->get_frequency_deviation_in_pu();

    ostringstream oosstream;
    oosstream<<setw(10)<<setprecision(6)<<fixed<<default_toolkit.get_dynamic_simulation_time_in_s()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<voltage<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_active_current_command_in_pu_based_on_mbase()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_reactive_current_command_in_pu_based_on_mbase()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<model->get_reactive_voltage_command_in_pu()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<genmodel->get_terminal_active_power_in_MW()<<"\t"
            <<setw(10)<<setprecision(6)<<fixed<<genmodel->get_terminal_reactive_power_in_MVar();
    default_toolkit.show_information_with_leading_time_stamp(oosstream);
}

void WT_ELECTRICAL_MODEL_TEST::apply_voltage_drop_of_10_percent()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);
    bus->set_voltage_in_pu(bus->get_voltage_in_pu()-0.1);
}

void WT_ELECTRICAL_MODEL_TEST::apply_frequency_drop_of_5_percent()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(1);
    BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
    model->set_frequency_deviation_in_pu(0.05);
}

void WT_ELECTRICAL_MODEL_TEST::test_get_standard_psse_string()
{
    WT_ELECTRICAL_MODEL* model = get_test_wt_electrical_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.show_information_with_leading_time_stamp(model->get_standard_psse_string());
}
