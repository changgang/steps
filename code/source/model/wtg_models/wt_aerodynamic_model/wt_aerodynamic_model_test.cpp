#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

WT_AERODYNAMIC_MODEL_TEST::WT_AERODYNAMIC_MODEL_TEST()
{
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_mbase_in_MVA);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_terminal_complex_voltage_in_pu);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_number_of_lumped_wt_generators);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_rated_power_per_wt_generator_in_MW);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_nominal_frequency_in_Hz);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_source_impedance_in_pu_based_on_mbase);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_model_type);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_model_name);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_parameters);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_active_current_command);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_reactive_current_command);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_get_standard_model_string);

    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_active_current_step_response_of_wt_generator_model);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_reactive_current_step_response_of_wt_generator_model);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_bus_magnitude_step_response_of_wt_generator_model);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_bus_angle_step_response_of_wt_generator_model);
    TEST_ADD(WT_AERODYNAMIC_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response);
}



void WT_AERODYNAMIC_MODEL_TEST::setup()
{
    WTG_MODEL_TEST::setup();
}

void WT_AERODYNAMIC_MODEL_TEST::tear_down()
{
    WTG_MODEL_TEST::tear_down();
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_mbase_in_MVA()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    TEST_ASSERT(fabs(model->get_mbase_in_MVA()-wt_gen->get_mbase_in_MVA())<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_terminal_complex_voltage_in_pu()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    POWER_SYSTEM_DATABASE* psdb = wt_gen->get_power_system_database();
    size_t bus = wt_gen->get_generator_bus();
    TEST_ASSERT(abs(model->get_terminal_complex_voltage_in_pu()-psdb->get_bus_complex_voltage_in_pu(bus))<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_number_of_lumped_wt_generators()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    TEST_ASSERT(fabs(model->get_number_of_lumped_wt_generators()-wt_gen->get_number_of_lumped_wt_generators())<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_rated_power_per_wt_generator_in_MW()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    TEST_ASSERT(fabs(model->get_rated_power_per_wt_generator_in_MW()-wt_gen->get_rated_power_per_wt_generator_in_MW())<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_nominal_frequency_in_Hz()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    POWER_SYSTEM_DATABASE* psdb = wt_gen->get_power_system_database();
    TEST_ASSERT(fabs(model->get_nominal_frequency_in_Hz()-psdb->get_system_base_frequency_in_Hz())<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_source_impedance_in_pu_based_on_mbase()
{
    WT_GENERATOR* wt_gen = get_test_wt_generator();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    TEST_ASSERT(fabs(model->get_source_impedance_in_pu_based_on_mbase()-wt_gen->get_source_impedance_in_pu())<FLOAT_EPSILON);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_model_type()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        TEST_ASSERT(model->get_model_type()=="WT GENERATOR");
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_active_current_command()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_active_current_command_in_pu_based_on_mbase(1.0);
        TEST_ASSERT(fabs(model->get_initial_active_current_command_in_pu_based_on_mbase()-1.0)<FLOAT_EPSILON);
        model->set_initial_active_current_command_in_pu_based_on_mbase(2.0);
        TEST_ASSERT(fabs(model->get_initial_active_current_command_in_pu_based_on_mbase()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_set_get_initial_reactive_current_command()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_reactive_current_command_in_pu_based_on_mbase(1.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_current_command_in_pu_based_on_mbase()-1.0)<FLOAT_EPSILON);
        model->set_initial_reactive_current_command_in_pu_based_on_mbase(2.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_current_command_in_pu_based_on_mbase()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::run_a_step()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
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

void WT_AERODYNAMIC_MODEL_TEST::export_meter_title()
{
    ostringstream sstream;
    sstream<<"TIME\tVTERM\tBUSANGLE\tPLL_FREQ\tPLL_ANGLE\tIPCMD\tIQCMD\tEQCMD\tPELEC\tQELEC";
    show_information_with_leading_time_stamp(sstream);
}

void WT_AERODYNAMIC_MODEL_TEST::export_meter_values(double time)
{
    ostringstream sstream;

    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();

    complex<double> Vxy = model->get_terminal_complex_voltage_in_pu();
    double vterm = abs(Vxy);
    double busangle = rad2deg(atan2(Vxy.imag(), Vxy.real()));
    double freq = model->get_pll_frequency_deviation_in_pu();
    double angle = model->get_pll_angle_in_deg();
    double ipcmd = model->get_active_current_command_in_pu_based_on_mbase();
    double iqcmd = model->get_reactive_current_command_in_pu_based_on_mbase();
    double pelec = model->get_terminal_active_power_in_MW();
    double qelec = model->get_terminal_reactive_power_in_MVar();

    sstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<vterm<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<busangle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<angle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<ipcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<iqcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<qelec;

    show_information_with_leading_time_stamp(sstream);
}

void WT_AERODYNAMIC_MODEL_TEST::test_active_current_step_response_of_wt_generator_model()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double ipcmd = model->get_initial_active_current_command_in_pu_based_on_mbase();
        model->set_initial_active_current_command_in_pu_based_on_mbase(ipcmd*0.99);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_reactive_current_step_response_of_wt_generator_model()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double iqcmd = model->get_initial_reactive_current_command_in_pu_based_on_mbase();
        model->set_initial_reactive_current_command_in_pu_based_on_mbase(iqcmd*0.99);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_bus_magnitude_step_response_of_wt_generator_model()
{
    POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        BUS* bus = psdb->get_bus(get_test_wt_generator()->get_generator_bus());
        double vterm = bus->get_voltage_in_pu();
        bus->set_voltage_in_pu(vterm*0.99);

        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_bus_angle_step_response_of_wt_generator_model()
{
    POWER_SYSTEM_DATABASE* psdb = get_test_power_system_database();
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        BUS* bus = psdb->get_bus(get_test_wt_generator()->get_generator_bus());
        double angle = bus->get_angle_in_deg();
        bus->set_angle_in_deg(angle+10.0);

        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}


void WT_AERODYNAMIC_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double ipcmd = model->get_initial_active_current_command_in_pu_based_on_mbase();
        model->set_initial_active_current_command_in_pu_based_on_mbase(ipcmd*0.9);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }

        delt *= 2.0;
        set_dynamic_simulation_time_step_in_s(delt);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>11.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }

        delt *= 2.0;
        set_dynamic_simulation_time_step_in_s(delt);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>16.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }

        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void WT_AERODYNAMIC_MODEL_TEST::test_get_standard_model_string()
{
    WT_GENERATOR_MODEL* model = get_test_wt_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        //model->get_standard_model_string();
    }
    else
        TEST_ASSERT(false);
}
