#include "header/basic/test_macro.h"
#include "header/model/pvu_models/pv_converter_model/pv_converter_model_test.h"
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

PV_CONVERTER_MODEL_TEST::PV_CONVERTER_MODEL_TEST()
{
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_mbase_in_MVA);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_terminal_complex_voltage_in_pu);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_number_of_lumped_pv_units);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_rated_power_per_pv_unit_in_MW);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_nominal_frequency_in_Hz);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_source_impedance_in_pu_based_on_mbase);

    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_model_type);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_model_name);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_set_get_parameters);

    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_set_get_initial_active_current_command);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_set_get_initial_reactive_current_command);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_get_standard_psse_string);

    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_active_current_step_response_of_pv_converter_model);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_reactive_current_step_response_of_pv_converter_model);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_bus_magnitude_step_response_of_pv_converter_model);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_bus_angle_step_response_of_pv_converter_model);
    TEST_ADD(PV_CONVERTER_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response);
}



void PV_CONVERTER_MODEL_TEST::setup()
{
    PVU_MODEL_TEST::setup();
}

void PV_CONVERTER_MODEL_TEST::tear_down()
{
    PVU_MODEL_TEST::tear_down();
}

void PV_CONVERTER_MODEL_TEST::test_get_mbase_in_MVA()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    TEST_ASSERT(fabs(model->get_mbase_in_MVA()-pv_unit->get_mbase_in_MVA())<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_terminal_complex_voltage_in_pu()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    size_t bus = pv_unit->get_unit_bus();
    TEST_ASSERT(abs(model->get_terminal_complex_voltage_in_pu()-psdb.get_bus_complex_voltage_in_pu(bus))<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_number_of_lumped_pv_units()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    TEST_ASSERT(fabs(model->get_number_of_lumped_pv_units()-pv_unit->get_number_of_lumped_pv_units())<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_rated_power_per_pv_unit_in_MW()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    TEST_ASSERT(fabs(model->get_rated_power_per_pv_unit_in_MW()-pv_unit->get_rated_power_per_pv_unit_in_MW())<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_nominal_frequency_in_Hz()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    TEST_ASSERT(fabs(model->get_bus_base_frequency_in_Hz()-psdb.get_bus_base_frequency_in_Hz(pv_unit->get_unit_bus()))<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_source_impedance_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_test_pv_unit();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    TEST_ASSERT(abs(model->get_source_impedance_in_pu_based_on_mbase()-pv_unit->get_source_impedance_in_pu())<FLOAT_EPSILON);
}

void PV_CONVERTER_MODEL_TEST::test_get_model_type()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        TEST_ASSERT(model->get_model_type()=="PV CONVERTER");
    }
    else
        TEST_ASSERT(false);
}

void PV_CONVERTER_MODEL_TEST::test_set_get_initial_active_current_command()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
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

void PV_CONVERTER_MODEL_TEST::test_set_get_initial_reactive_current_command()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
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

void PV_CONVERTER_MODEL_TEST::run_a_step()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
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

void PV_CONVERTER_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVTERM\tBUSANGLE\tPLL_FREQ\tPLL_ANGLE\tIPCMD\tIQCMD\tEQCMD\tPELEC\tQELEC";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void PV_CONVERTER_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();

    complex<double> Vxy = model->get_terminal_complex_voltage_in_pu();
    double vterm = steps_fast_complex_abs(Vxy);
    double busangle = rad2deg(atan2(Vxy.imag(), Vxy.real()));
    double freq = model->get_pll_frequency_deviation_in_pu();
    double angle = model->get_pll_angle_in_deg();
    double ipcmd = model->get_active_current_command_in_pu_based_on_mbase();
    double iqcmd = model->get_reactive_current_command_in_pu_based_on_mbase();
    double pelec = model->get_terminal_active_power_in_MW();
    double qelec = model->get_terminal_reactive_power_in_MVar();

    osstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<vterm<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<busangle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<angle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<ipcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<iqcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<qelec;

    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void PV_CONVERTER_MODEL_TEST::test_active_current_step_response_of_pv_converter_model()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

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
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void PV_CONVERTER_MODEL_TEST::test_reactive_current_step_response_of_pv_converter_model()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

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
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void PV_CONVERTER_MODEL_TEST::test_bus_magnitude_step_response_of_pv_converter_model()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

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

        BUS* bus = psdb.get_bus(get_test_pv_unit()->get_unit_bus());
        double vterm = bus->get_positive_sequence_voltage_in_pu();
        bus->set_positive_sequence_voltage_in_pu(vterm*0.99);

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
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void PV_CONVERTER_MODEL_TEST::test_bus_angle_step_response_of_pv_converter_model()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

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

        BUS* bus = psdb.get_bus(get_test_pv_unit()->get_unit_bus());
        double angle = bus->get_positive_sequence_angle_in_deg();
        bus->set_positive_sequence_angle_in_deg(angle+10.0);

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
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}


void PV_CONVERTER_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

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
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
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
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
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

        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void PV_CONVERTER_MODEL_TEST::test_get_standard_psse_string()
{
    PV_CONVERTER_MODEL* model = get_test_pv_converter_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        //model->get_standard_psse_string();
    }
    else
        TEST_ASSERT(false);
}

#endif
