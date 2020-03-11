#include "header/basic/test_macro.h"
#include "header/model/hvdc_model/hvdc_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include "header/model/hvdc_model/CDC4T.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
HVDC_MODEL_TEST::HVDC_MODEL_TEST()
{
    TEST_ADD(HVDC_MODEL_TEST::test_initialize);
    TEST_ADD(HVDC_MODEL_TEST::test_rectifier_voltage_ramp_response);
    TEST_ADD(HVDC_MODEL_TEST::test_inverter_voltage_ramp_response);
    TEST_ADD(HVDC_MODEL_TEST::test_manual_block_and_unblock);
    TEST_ADD(HVDC_MODEL_TEST::test_manual_bypass_and_unbypass);
    TEST_ADD(HVDC_MODEL_TEST::test_get_standard_psse_string);
}



void HVDC_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_name("RECBUS");
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(345.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);

    psdb.append_bus(bus);

    bus.set_bus_number(2);
    bus.set_bus_name("INVBUS");
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(525.0);
    bus.set_positive_sequence_voltage_in_pu(1.0);
    bus.set_positive_sequence_angle_in_rad(0.0);

    psdb.append_bus(bus);

    HVDC hvdc(default_toolkit);
    hvdc.set_converter_bus(RECTIFIER, 1);
    hvdc.set_converter_bus(INVERTER, 2);
    hvdc.set_identifier("DC");
    hvdc.set_name("Yin-Dong HVDC");
    hvdc.set_status(true);
    hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    hvdc.set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    hvdc.set_nominal_dc_power_per_pole_in_MW(2000.0);
    hvdc.set_nominal_dc_voltage_per_pole_in_kV(660.0);
    hvdc.set_line_resistance_in_ohm(9.5);
    hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
    hvdc.set_current_power_margin(0.1);
    hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(500.0);

    hvdc.set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 90.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 5.0);
    hvdc.set_converter_number_of_bridge(RECTIFIER, 2);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, 345.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, 250.0);
    hvdc.set_converter_transformer_impedance_in_ohm(RECTIFIER, complex<double>(0.1, 9.153));
    hvdc.set_converter_transformer_max_tap_in_pu(RECTIFIER, 1.3);
    hvdc.set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.7);
    hvdc.set_converter_transformer_number_of_taps(RECTIFIER, 25);

    hvdc.set_converter_max_alpha_or_gamma_in_deg(INVERTER, 90.0);
    hvdc.set_converter_min_alpha_or_gamma_in_deg(INVERTER, 8.0);
    hvdc.set_converter_number_of_bridge(INVERTER, 2);
    hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, 525.0);
    hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, 250.0);
    hvdc.set_converter_transformer_impedance_in_ohm(INVERTER, complex<double>(0.1, 9.885));
    hvdc.set_converter_transformer_max_tap_in_pu(INVERTER, 1.3);
    hvdc.set_converter_transformer_min_tap_in_pu(INVERTER, 0.7);
    hvdc.set_converter_transformer_number_of_taps(INVERTER, 25);

    psdb.append_hvdc(hvdc);

    HVDC* hvdcptr = get_test_hvdc();
    hvdcptr->solve_steady_state();
    hvdcptr->show_solved_hvdc_steady_state();
}

void HVDC_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}

HVDC* HVDC_MODEL_TEST::get_test_hvdc()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    DEVICE_ID did;
    did.set_device_type("HVDC");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("DC");

    return psdb.get_hvdc(did);
}

HVDC_MODEL* HVDC_MODEL_TEST::get_test_hvdc_model()
{
    HVDC* hvdc = get_test_hvdc();
    if(hvdc==NULL)
        return NULL;
    else
        return hvdc->get_hvdc_model();
}

void HVDC_MODEL_TEST::export_meter_titles()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT_REC\tVOLT_INV\tP_REC\tQ_REC\tALPHA\tMU_REC\tP_INV\tQ_INV\tGAMMA\tMU_INV\tVDCR\tVDCI\tIDCR\tIDCR"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void HVDC_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    HVDC* hvdc = get_test_hvdc();
    HVDC_MODEL* model = hvdc->get_hvdc_model();

    double volt_rec, volt_inv;
    double p_rec, q_rec, alpha, mu_rec, p_inv, q_inv, gamma, mu_inv;
    double vdcr, vdci, idcr, idci;

    volt_rec = psdb.get_bus_positive_sequence_voltage_in_pu(1);
    volt_inv = psdb.get_bus_positive_sequence_voltage_in_pu(2);

    HVDC_CONVERTER_SIDE converter;

    converter = RECTIFIER;
    p_rec = model->get_converter_ac_complex_power_in_MVA(converter).real();
    q_rec = model->get_converter_ac_complex_power_in_MVA(converter).imag();
    alpha = model->get_converter_alpha_or_gamma_in_deg(converter);
    mu_rec = model->get_converter_commutation_overlap_angle_in_deg(converter);
    vdcr = model->get_converter_dc_voltage_in_kV(converter);
    idcr = model->get_converter_dc_current_in_kA(converter);

    converter = INVERTER;
    p_inv = model->get_converter_ac_complex_power_in_MVA(converter).real();
    q_inv = model->get_converter_ac_complex_power_in_MVA(converter).imag();
    gamma = model->get_converter_alpha_or_gamma_in_deg(converter);
    mu_inv = model->get_converter_commutation_overlap_angle_in_deg(converter);
    vdci = model->get_converter_dc_voltage_in_kV(converter);
    idci = model->get_converter_dc_current_in_kA(converter);

    osstream<<setprecision(6)<<fixed<<time<<"\t"
      <<setprecision(6)<<fixed<<volt_rec<<"\t"
      <<setprecision(6)<<fixed<<volt_inv<<"\t"
      <<setprecision(6)<<fixed<<p_rec<<"\t"
      <<setprecision(6)<<fixed<<q_rec<<"\t"
      <<setprecision(6)<<fixed<<alpha<<"\t"
      <<setprecision(6)<<fixed<<mu_rec<<"\t"
      <<setprecision(6)<<fixed<<p_inv<<"\t"
      <<setprecision(6)<<fixed<<q_inv<<"\t"
      <<setprecision(6)<<fixed<<gamma<<"\t"
      <<setprecision(6)<<fixed<<mu_inv<<"\t"
      <<setprecision(6)<<fixed<<vdcr<<"\t"
      <<setprecision(6)<<fixed<<vdci<<"\t"
      <<setprecision(6)<<fixed<<idcr<<"\t"
      <<setprecision(6)<<fixed<<idci<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}


void HVDC_MODEL_TEST::test_initialize()
{

    ostringstream osstream;

    HVDC* hvdcptr = get_test_hvdc();
    HVDC_MODEL* model = get_test_hvdc_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/test_initialize_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    model->initialize();
    osstream<<"hvdc info when initialized:"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    hvdcptr->show_solved_hvdc_steady_state();

    default_toolkit.close_log_file();
}

void HVDC_MODEL_TEST::test_rectifier_voltage_ramp_response()
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    HVDC* hvdcptr = get_test_hvdc();
    HVDC_MODEL* model = get_test_hvdc_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/test_rectifier_voltage_ramp_response_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    model->initialize();
    osstream<<"hvdc info when initialized:"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    hvdcptr->show_solved_hvdc_steady_state();

    double TIME = -delt*2.0;
    double vdcr;
    HVDC_CONVERTER_SIDE converter;

    export_meter_titles();
    export_meter_values(TIME);

    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    double Vac_rec = psdb.get_bus_positive_sequence_voltage_in_pu(1);
    BUS* bus = psdb.get_bus(1);
    double rate = 0.2;

    while(true)
    {
        TIME += delt;
        Vac_rec -= (rate*delt);
        bus->set_positive_sequence_voltage_in_pu(Vac_rec);

        if(TIME>5.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            Vac_rec += (rate*delt);
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;

        if(TIME>7.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;
        Vac_rec += (rate*delt);
        bus->set_positive_sequence_voltage_in_pu(Vac_rec);

        if(TIME>11.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            Vac_rec -= (rate*delt);
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;

        if(TIME>15.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    default_toolkit.close_log_file();
}


void HVDC_MODEL_TEST::test_inverter_voltage_ramp_response()
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    HVDC* hvdcptr = get_test_hvdc();
    HVDC_MODEL* model = get_test_hvdc_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/test_inverter_voltage_ramp_response_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    model->initialize();
    osstream<<"hvdc info when initialized:"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    hvdcptr->show_solved_hvdc_steady_state();

    double TIME = -delt*2.0;
    double vdcr;
    HVDC_CONVERTER_SIDE converter;

    export_meter_titles();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    double Vac_inv = psdb.get_bus_positive_sequence_voltage_in_pu(2);
    BUS* bus = psdb.get_bus(2);
    double rate = 0.2;

    while(true)
    {
        TIME += delt;
        Vac_inv -= (rate*delt);
        bus->set_positive_sequence_voltage_in_pu(Vac_inv);

        if(TIME>5.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            Vac_inv += (rate*delt);
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;

        if(TIME>7.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;
        Vac_inv += (rate*delt);
        bus->set_positive_sequence_voltage_in_pu(Vac_inv);

        if(TIME>11.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            Vac_inv -= (rate*delt);
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;

        if(TIME>15.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    default_toolkit.close_log_file();
}


void HVDC_MODEL_TEST::test_manual_block_and_unblock()
{
    ostringstream osstream;

    HVDC* hvdcptr = get_test_hvdc();
    HVDC_MODEL* model = get_test_hvdc_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/test_manual_block_and_unblock_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    model->initialize();
    osstream<<"hvdc info when initialized:"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    hvdcptr->show_solved_hvdc_steady_state();

    double TIME = -delt*2.0;
    double vdcr;
    HVDC_CONVERTER_SIDE converter;

    export_meter_titles();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    //cout<<"now go blocking HVDC manually"<<endl;
    model->manual_block_hvdc();
    //cout<<"now successfully blocked HVDC manually"<<endl;
    model->run(UPDATE_MODE);
    export_meter_values(TIME);


    while(true)
    {
        TIME += delt;

        if(TIME>5.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }
    model->manual_unblock_hvdc();
    model->run(UPDATE_MODE);
    export_meter_values(TIME);


    while(true)
    {
        TIME += delt;

        if(TIME>10.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    default_toolkit.close_log_file();
}

void HVDC_MODEL_TEST::test_manual_bypass_and_unbypass()
{
    ostringstream osstream;

    HVDC* hvdcptr = get_test_hvdc();
    HVDC_MODEL* model = get_test_hvdc_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    default_toolkit.open_log_file("test_log/test_manual_bypass_and_unbypass_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    double delt = 0.001;
    default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

    model->initialize();
    osstream<<"hvdc info when initialized:"<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    hvdcptr->show_solved_hvdc_steady_state();

    double TIME = -delt*2.0;
    double vdcr;
    HVDC_CONVERTER_SIDE converter;

    export_meter_titles();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    model->manual_bypass_hvdc();
    model->run(UPDATE_MODE);
    export_meter_values(TIME);


    while(true)
    {
        TIME += delt;

        if(TIME>5.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }
    model->manual_unbypass_hvdc();
    model->run(UPDATE_MODE);
    export_meter_values(TIME);


    while(true)
    {
        TIME += delt;

        if(TIME>10.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }

        converter = RECTIFIER;
        vdcr = model->get_converter_dc_voltage_in_kV(converter);
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(vdcr-model->get_converter_dc_voltage_in_kV(RECTIFIER))>1e-6)
                vdcr = model->get_converter_dc_voltage_in_kV(RECTIFIER);
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    default_toolkit.close_log_file();
}

void HVDC_MODEL_TEST::test_get_standard_psse_string()
{
    ostringstream osstream;

    HVDC_MODEL* model = get_test_hvdc_model();

    default_toolkit.open_log_file("test_log/test_get_standard_psse_string_"+model->get_model_name()+".txt");

    osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    default_toolkit.close_log_file();
}

void HVDC_MODEL_TEST::test_get_auxiliary_signal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"HVDC_MODEL_TEST");
}

#endif
