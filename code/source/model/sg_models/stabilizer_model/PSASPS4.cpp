#include "header/model/sg_models/stabilizer_model/PSASPS4.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
#include <iostream>

using namespace std;

/***
It should be noted that, the first De-DC block of Pelec is a FIRST_ORDER_BLOCK in manual of PSASP V7.3, July 2017.
However, it is modeled as a DIFFERENTIAL_BLOCK in STEPS.
Further check is necessary.
***/
PSASPS4::PSASPS4()
{
    clear();
}

PSASPS4::~PSASPS4()
{
    //clear();
}

void PSASPS4::clear()
{
    set_model_float_parameter_count(23);
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void PSASPS4::copy_from_const_model(const PSASPS4& model)
{
    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    for(size_t i=0; i!=MAX_STABILIZER_INPUT_SIGNAL_SLOT; ++i)
    {
        if(model.is_slot_valid(i))
        {
            SIGNAL signal =  model.get_input_signal_at_slot(i);
            this->set_input_signal_at_slot(i, signal);
        }
    }

    this->set_Kw(model.get_Kw());
    this->set_Trw_in_s(model.get_Trw_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_T7_in_s(model.get_T7_in_s());
    this->set_Kp(model.get_Kp());
    this->set_Trp_in_s(model.get_Trp_in_s());
    this->set_Tw_in_s(model.get_Tw_in_s());
    this->set_Tw1_in_s(model.get_Tw1_in_s());
    this->set_Tw2_in_s(model.get_Tw2_in_s());
    this->set_Ks(model.get_Ks());
    this->set_T9_in_s(model.get_T9_in_s());
    this->set_T10_in_s(model.get_T10_in_s());
    this->set_T12_in_s(model.get_T12_in_s());
    this->set_Kp(model.get_Kp());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T13_in_s(model.get_T13_in_s());
    this->set_T14_in_s(model.get_T14_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());

}
PSASPS4::PSASPS4(const PSASPS4& model) : STABILIZER_MODEL()
{
    copy_from_const_model(model);
}

PSASPS4& PSASPS4::operator=(const PSASPS4& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS4::get_model_name() const
{
    return "PSASPS4";
}

void PSASPS4::set_Kw(double K)
{
    speed_sensor.set_K(K);
}

void PSASPS4::set_Trw_in_s(double T)
{
    speed_sensor.set_T_in_s(T);
}

void PSASPS4::set_T5_in_s(double T)
{
    speed_dedc_block_1.set_K(T);
}

void PSASPS4::set_T6_in_s(double T)
{
    speed_dedc_block_1.set_T_in_s(T);
}

void PSASPS4::set_T7_in_s(double T)
{
    speed_dedc_block_2.set_K(T);
    speed_dedc_block_2.set_T_in_s(T);
}

void PSASPS4::set_Kr(double K)
{
    pelec_sensor.set_K(K);
}

void PSASPS4::set_Trp_in_s(double T)
{
    pelec_sensor.set_T_in_s(T);
}

void PSASPS4::set_Tw_in_s(double T)
{
    pelec_dedc_block_1.set_K(T);
}

void PSASPS4::set_Tw1_in_s(double T)
{
    pelec_dedc_block_1.set_T_in_s(T);
}

void PSASPS4::set_Tw2_in_s(double T)
{
    pelec_dedc_block_2.set_K(T);
    pelec_dedc_block_2.set_T_in_s(T);
}

void PSASPS4::set_Ks(double K)
{
    Ks = K;
}

void PSASPS4::set_T9_in_s(double T)
{
    notch_filter_phase_tuner.set_T1_in_s(T);
}

void PSASPS4::set_T10_in_s(double T)
{
    notch_filter_phase_tuner.set_T2_in_s(T);
}

void PSASPS4::set_T12_in_s(double T)
{
    notch_filter_first_order_block_1.set_K(1.0);
    notch_filter_first_order_block_1.set_T_in_s(T);
    notch_filter_first_order_block_2.set_K(1.0);
    notch_filter_first_order_block_2.set_T_in_s(T);
    notch_filter_first_order_block_3.set_K(1.0);
    notch_filter_first_order_block_3.set_T_in_s(T);
    notch_filter_first_order_block_4.set_K(1.0);
    notch_filter_first_order_block_4.set_T_in_s(T);
}

void PSASPS4::set_Kp(double K)
{
    phase_tuner_1.set_K(K);
}

void PSASPS4::set_T1_in_s(double T)
{
    phase_tuner_1.set_T1_in_s(T);
}

void PSASPS4::set_T2_in_s(double T)
{
    phase_tuner_1.set_T2_in_s(T);
}

void PSASPS4::set_T13_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}

void PSASPS4::set_T14_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}

void PSASPS4::set_T3_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}

void PSASPS4::set_T4_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}

void PSASPS4::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void PSASPS4::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS4::get_Kw() const
{
    return speed_sensor.get_K();
}

double PSASPS4::get_Trw_in_s() const
{
    return speed_sensor.get_T_in_s();
}

double PSASPS4::get_T5_in_s() const
{
    return speed_dedc_block_1.get_K();
}

double PSASPS4::get_T6_in_s() const
{
    return speed_dedc_block_1.get_T_in_s();
}

double PSASPS4::get_T7_in_s() const
{
    return speed_dedc_block_2.get_T_in_s();
}

double PSASPS4::get_Kr() const
{
    return pelec_sensor.get_K();
}

double PSASPS4::get_Trp_in_s() const
{
    return pelec_sensor.get_T_in_s();
}

double PSASPS4::get_Tw_in_s() const
{
    return pelec_dedc_block_1.get_K();
}

double PSASPS4::get_Tw1_in_s() const
{
    return pelec_dedc_block_1.get_T_in_s();
}

double PSASPS4::get_Tw2_in_s() const
{
    return pelec_dedc_block_2.get_T_in_s();
}

double PSASPS4::get_Ks() const
{
    return Ks;
}

double PSASPS4::get_T9_in_s() const
{
    return notch_filter_phase_tuner.get_T1_in_s();
}

double PSASPS4::get_T10_in_s() const
{
    return notch_filter_phase_tuner.get_T2_in_s();
}

double PSASPS4::get_T12_in_s() const
{
    return notch_filter_first_order_block_1.get_T_in_s();
}

double PSASPS4::get_Kp() const
{
    return phase_tuner_1.get_K();
}

double PSASPS4::get_T1_in_s() const
{
    return phase_tuner_1.get_T1_in_s();
}

double PSASPS4::get_T2_in_s() const
{
    return phase_tuner_1.get_T2_in_s();
}

double PSASPS4::get_T13_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}

double PSASPS4::get_T14_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}

double PSASPS4::get_T3_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}

double PSASPS4::get_T4_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double PSASPS4::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS4::get_Vsmin() const
{
    return Vsmin;
}


bool PSASPS4::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=26)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kw, trw, t5, t6, t7, kr, trp, tw, tw1, tw2, ks, t9, t10, t12, kp, t1, t2, t13, t14, t3, t4,
                   vsmax, vsmin;

            size_t i=3;
            kw = get_double_data(data[i],"0.0"); i++;
            trw = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            t6 = get_double_data(data[i],"0.0"); i++;
            t7 = get_double_data(data[i],"0.0"); i++;
            kr= get_double_data(data[i],"0.0"); i++;
            trp = get_double_data(data[i],"0.0"); i++;
            tw = get_double_data(data[i],"0.0"); i++;
            tw1 = get_double_data(data[i],"0.0"); i++;
            tw2 = get_double_data(data[i],"0.0"); i++;
            ks = get_double_data(data[i],"0.0"); i++;
            t9 = get_double_data(data[i],"0.0"); i++;
            t10 = get_double_data(data[i],"0.0"); i++;
            t12 = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            t1 = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t13 = get_double_data(data[i],"0.0"); i++;
            t14 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            t4 = get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            set_Kw(kw);
            set_Trw_in_s(trw);
            set_T5_in_s(t5);
            set_T6_in_s(t6);
            set_T7_in_s(t7);
            set_Kr(kr);
            set_Trp_in_s(trp);
            set_Tw_in_s(tw);
            set_Tw1_in_s(tw1);
            set_Tw2_in_s(tw2);
            set_Ks(ks);
            set_T9_in_s(t9);
            set_T10_in_s(t10);
            set_T12_in_s(t12);
            set_Kp(kp);
            set_T1_in_s(t1);
            set_T2_in_s(t2);
            set_T13_in_s(t13);
            set_T14_in_s(t14);
            set_T3_in_s(t3);
            set_T4_in_s(t4);
            set_Vsmax(vsmax);
            set_Vsmin(vsmin);

            is_successful = true;
            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PSASPS4::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS4::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS4::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    speed_sensor.set_toolkit(toolkit);
    pelec_sensor.set_toolkit(toolkit);
    speed_dedc_block_1.set_toolkit(toolkit);
    speed_dedc_block_2.set_toolkit(toolkit);
    pelec_dedc_block_1.set_toolkit(toolkit);
    pelec_dedc_block_2.set_toolkit(toolkit);
    notch_filter_phase_tuner.set_toolkit(toolkit);
    notch_filter_first_order_block_1.set_toolkit(toolkit);
    notch_filter_first_order_block_2.set_toolkit(toolkit);
    notch_filter_first_order_block_3.set_toolkit(toolkit);
    notch_filter_first_order_block_4.set_toolkit(toolkit);
    phase_tuner_1.set_toolkit(toolkit);
    phase_tuner_2.set_toolkit(toolkit);
    phase_tuner_3.set_toolkit(toolkit);
}

void PSASPS4::initialize()
{
    ostringstream osstream;

    GENERATOR* generator = get_generator_pointer();
    DEVICE_ID did = generator->get_device_id();
    if(generator!=NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
        {
            if(not exciter->is_model_initialized())
                exciter->initialize();

            setup_block_toolkit_and_parameters();

            SIGNAL signal = prepare_signal_with_signal_type_and_device_id(1, did);
            if(signal.is_valid())
                set_input_signal_at_slot(0, signal);

            signal = prepare_signal_with_signal_type_and_device_id(3, did);
            if(signal.is_valid())
                set_input_signal_at_slot(1, signal);

            phase_tuner_3.set_output(0.0);
            phase_tuner_3.initialize();

            phase_tuner_2.set_output(0.0);
            phase_tuner_2.initialize();

            phase_tuner_1.set_output(0.0);
            phase_tuner_1.initialize();

            notch_filter_first_order_block_4.set_output(0.0);
            notch_filter_first_order_block_4.initialize();
            notch_filter_first_order_block_3.set_output(0.0);
            notch_filter_first_order_block_3.initialize();
            notch_filter_first_order_block_2.set_output(0.0);
            notch_filter_first_order_block_2.initialize();
            notch_filter_first_order_block_1.set_output(0.0);
            notch_filter_first_order_block_1.initialize();

            notch_filter_phase_tuner.set_output(0.0);
            notch_filter_phase_tuner.initialize();

            pelec_dedc_block_2.set_output(0.0);
            pelec_dedc_block_2.initialize();
            pelec_dedc_block_1.set_output(0.0);
            pelec_dedc_block_1.initialize();
            pelec_sensor.set_output(0.0);
            pelec_sensor.initialize();

            speed_dedc_block_2.set_output(0.0);
            speed_dedc_block_2.initialize();
            speed_dedc_block_1.set_output(0.0);
            speed_dedc_block_1.initialize();
            speed_sensor.set_output(0.0);
            speed_sensor.initialize();

            speed_deviation_ref_pu = get_signal_value_of_slot(0);
            Pe_ref_pu = get_signal_value_of_slot(1);
        }
        else
            deactivate_model();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS4::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        double value_slot_1 = get_signal_value_of_slot(0);
        speed_sensor.set_input(value_slot_1-speed_deviation_ref_pu);
        speed_sensor.run(mode);

        double input = speed_sensor.get_output();
        speed_dedc_block_1.set_input(input);
        speed_dedc_block_1.run(mode);

        input = speed_dedc_block_1.get_output();
        speed_dedc_block_2.set_input(input);
        speed_dedc_block_2.run(mode);

        double value_slot_2 = get_signal_value_of_slot(1);
        pelec_sensor.set_input(value_slot_2-Pe_ref_pu);
        pelec_sensor.run(mode);

        input = pelec_sensor.get_output();
        pelec_dedc_block_1.set_input(input);
        pelec_dedc_block_1.run(mode);

        input = pelec_dedc_block_1.get_output();
        pelec_dedc_block_2.set_input(input);
        pelec_dedc_block_2.run(mode);

        input = speed_dedc_block_2.get_output()+Ks*pelec_dedc_block_2.get_output();
        notch_filter_phase_tuner.set_input(input);
        notch_filter_phase_tuner.run(mode);

        input = notch_filter_phase_tuner.get_output();
        notch_filter_first_order_block_1.set_input(input);
        notch_filter_first_order_block_1.run(mode);

        input = notch_filter_first_order_block_1.get_output();
        notch_filter_first_order_block_2.set_input(input);
        notch_filter_first_order_block_2.run(mode);

        input = notch_filter_first_order_block_2.get_output();
        notch_filter_first_order_block_3.set_input(input);
        notch_filter_first_order_block_3.run(mode);

        input = notch_filter_first_order_block_3.get_output();
        notch_filter_first_order_block_4.set_input(input);
        notch_filter_first_order_block_4.run(mode);

        input = notch_filter_first_order_block_4.get_output()-pelec_dedc_block_2.get_output();
        phase_tuner_1.set_input(input);
        phase_tuner_1.run(mode);

        input = phase_tuner_1.get_output();
        phase_tuner_2.set_input(input);
        phase_tuner_2.run(mode);

        input = phase_tuner_2.get_output();
        phase_tuner_3.set_input(input);
        phase_tuner_3.run(mode);

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double PSASPS4::get_stabilizing_signal_in_pu() const
{
    double output = phase_tuner_3.get_output();
    double vsmax = get_Vsmax();
    double vsmin = get_Vsmin();
    if(output>vsmax)
        output=vsmax;
    if(output<vsmin)
        output=vsmin;

    return output;
}

void PSASPS4::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double vsmax = get_Vsmax();
    double vsmin = get_Vsmin();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_device_name()<<".\n";
    bool error_found = false;
    if(vsmax<=vsmin)
    {
        osstream<<"Vsmax<=Vsmin was detected: Vsmax="<<vsmax<<", Vsmin="<<vsmin<<"\n";
        error_found = true;
    }
    if(get_Trw_in_s()==0.0)
    {
        osstream<<"Trw=0.0 was detected\n";
        error_found = true;
    }
    if(get_T5_in_s()==0.0)
    {
        osstream<<"T5=0.0 was detected\n";
        error_found = true;
    }
    if(get_T6_in_s()==0.0)
    {
        osstream<<"T6=0.0 was detected\n";
        error_found = true;
    }
    if(get_T7_in_s()==0.0)
    {
        osstream<<"T7=0.0 was detected\n";
        error_found = true;
    }
    if(get_Trp_in_s()==0.0)
    {
        osstream<<"Trp=0.0 was detected\n";
        error_found = true;
    }
    if(get_Tw_in_s()==0.0)
    {
        osstream<<"Tw=0.0 was detected\n";
        error_found = true;
    }
    if(get_Tw1_in_s()==0.0)
    {
        osstream<<"Tw1=0.0 was detected\n";
        error_found = true;
    }
    if(get_Tw_in_s()==0.0)
    {
        osstream<<"Tw2=0.0 was detected\n";
        error_found = true;
    }
    if(get_T10_in_s()==0.0 and get_T9_in_s()!=0.0)
    {
        osstream<<"T10=0.0 was detected when T9!=0.0. T9="<<get_T9_in_s()<<"\n";
        error_found = true;
    }
    if(get_T12_in_s()==0.0)
    {
        //osstream<<"T12=0.0 was detected\n";
        //error_found = true;
    }
    if(get_Kp()>10.0)
    {
        osstream<<"Kp>10.0 was detected. Kp="<<get_Kp()<<"\n";
        error_found = true;
    }
    if(get_T2_in_s()==0.0)
    {
        osstream<<"T2=0.0 was detected\n";
        error_found = true;
    }
    if(get_T14_in_s()==0.0)
    {
        osstream<<"T14=0.0 was detected\n";
        error_found = true;
    }
    if(get_T4_in_s()==0.0)
    {
        osstream<<"T4=0.0 was detected\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS4::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS4::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS4::get_standard_psse_string() const
{
    ostringstream osstream;

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    double Kw = get_Kw();
    double Trw = get_Trw_in_s();
    double T5 = get_T5_in_s();
    double T6 = get_T6_in_s();
    double T7 = get_T7_in_s();
    double Kr = get_Kr();
    double Trp = get_Trp_in_s();
    double Tw = get_Tw_in_s();
    double Tw1 = get_Tw1_in_s();
    double Tw2 = get_Tw2_in_s();
    double Ks = get_Ks();
    double T9 = get_T9_in_s();
    double T10 = get_T10_in_s();
    double T12 = get_T12_in_s();
    double Kp = get_Kp();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T13 = get_T13_in_s();
    double T14 = get_T14_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<Kw<<", "
            <<setw(8)<<setprecision(6)<<Trw<<", "
            <<setw(8)<<setprecision(6)<<T5<<", "
            <<setw(8)<<setprecision(6)<<T6<<", "
            <<setw(8)<<setprecision(6)<<T7<<", "
            <<setw(8)<<setprecision(6)<<Kr<<", "
            <<setw(8)<<setprecision(6)<<Trp<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Tw<<", "
            <<setw(8)<<setprecision(6)<<Tw1<<", "
            <<setw(8)<<setprecision(6)<<Tw2<<", "
            <<setw(8)<<setprecision(6)<<Ks<<", "
            <<setw(8)<<setprecision(6)<<T9<<", "
            <<setw(8)<<setprecision(6)<<T10<<", "
            <<setw(8)<<setprecision(6)<<T12<<", "
            <<setw(8)<<setprecision(6)<<Kp<<", "
            <<setw(8)<<setprecision(6)<<T1<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T13<<", "
            <<setw(8)<<setprecision(6)<<T14<<", "
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<T4<<", "
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS4::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("KW", i); i++;
    add_model_data_name_and_index_pair("TRW", i); i++;
    add_model_data_name_and_index_pair("T5", i); i++;
    add_model_data_name_and_index_pair("T6", i); i++;
    add_model_data_name_and_index_pair("T7", i); i++;
    add_model_data_name_and_index_pair("KR", i); i++;
    add_model_data_name_and_index_pair("TRP", i); i++;
    add_model_data_name_and_index_pair("TW", i); i++;
    add_model_data_name_and_index_pair("TW1", i); i++;
    add_model_data_name_and_index_pair("TW2", i); i++;
    add_model_data_name_and_index_pair("KS", i); i++;
    add_model_data_name_and_index_pair("T9", i); i++;
    add_model_data_name_and_index_pair("T10", i); i++;
    add_model_data_name_and_index_pair("T12", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T13", i); i++;
    add_model_data_name_and_index_pair("T14", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("T4", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS4::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="KW")
        return get_Kw();
    if(par_name=="TRW")
        return get_Trw_in_s();
    if(par_name=="T5")
        return get_T5_in_s();
    if(par_name=="T6")
        return get_T6_in_s();
    if(par_name=="T7")
        return get_T7_in_s();
    if(par_name=="KR")
        return get_Kr();
    if(par_name=="TRP")
        return get_Trp_in_s();
    if(par_name=="TW")
        return get_Tw_in_s();
    if(par_name=="TW1")
        return get_Tw1_in_s();
    if(par_name=="TW2")
        return get_Tw2_in_s();
    if(par_name=="KS")
        return get_Ks();
    if(par_name=="T9")
        return get_T9_in_s();
    if(par_name=="T10")
        return get_T10_in_s();
    if(par_name=="T12")
        return get_T12_in_s();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="T1")
        return get_T1_in_s();
    if(par_name=="T2")
        return get_T2_in_s();
    if(par_name=="T13")
        return get_T13_in_s();
    if(par_name=="T14")
        return get_T14_in_s();
    if(par_name=="T3")
        return get_T3_in_s();
    if(par_name=="T4")
        return get_T4_in_s();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS4::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KW")
        return set_Kw(value);
    if(par_name=="TRW")
        return set_Trw_in_s(value);
    if(par_name=="T5")
        return set_T5_in_s(value);
    if(par_name=="T6")
        return set_T6_in_s(value);
    if(par_name=="T7")
        return set_T7_in_s(value);
    if(par_name=="KR")
        return set_Kr(value);
    if(par_name=="TRP")
        return set_Trp_in_s(value);
    if(par_name=="TW")
        return set_Tw_in_s(value);
    if(par_name=="TW1")
        return set_Tw1_in_s(value);
    if(par_name=="TW2")
        return set_Tw2_in_s(value);
    if(par_name=="KS")
        return set_Ks(value);
    if(par_name=="T9")
        return set_T9_in_s(value);
    if(par_name=="T10")
        return set_T10_in_s(value);
    if(par_name=="T12")
        return set_T12_in_s(value);
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="T1")
        return set_T1_in_s(value);
    if(par_name=="T2")
        return set_T2_in_s(value);
    if(par_name=="T13")
        return set_T13_in_s(value);
    if(par_name=="T14")
        return set_T14_in_s(value);
    if(par_name=="T3")
        return set_T3_in_s(value);
    if(par_name=="T4")
        return set_T4_in_s(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

void PSASPS4::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PELEC SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PELEC DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PELEC DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@NOTCH FILTER PHASE TUNER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@NOTCH FILTER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@NOTCH FILTER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@NOTCH FILTER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@NOTCH FILTER 4", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@SPEED SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@SPEED DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@SPEED DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PELEC SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PELEC DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PELEC DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@NOTCH FILTER PHASE TUNER", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@NOTCH FILTER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@NOTCH FILTER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@NOTCH FILTER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@NOTCH FILTER 4", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
}

double PSASPS4::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "SIGNAL@SLOT 2")
        return get_signal_value_of_slot(1);

    if(var_name == "STATE@SPEED SENSOR")
        return speed_sensor.get_state();

    if(var_name == "STATE@SPEED DEDC BLOCK 1")
        return speed_dedc_block_1.get_state();

    if(var_name == "STATE@SPEED DEDC BLOCK 2")
        return speed_dedc_block_2.get_state();

    if(var_name == "STATE@PELEC SENSOR")
        return pelec_sensor.get_state();

    if(var_name == "STATE@PELEC DEDC BLOCK 1")
        return pelec_dedc_block_1.get_state();

    if(var_name == "STATE@PELEC DEDC BLOCK 2")
        return pelec_dedc_block_2.get_state();

    if(var_name == "STATE@NOTCH FILTER PHASE TUNER")
        return notch_filter_phase_tuner.get_state();

    if(var_name == "STATE@NOTCH FILTER 1")
        return notch_filter_first_order_block_1.get_state();

    if(var_name == "STATE@NOTCH FILTER 2")
        return notch_filter_first_order_block_2.get_state();

    if(var_name == "STATE@NOTCH FILTER 3")
        return notch_filter_first_order_block_3.get_state();

    if(var_name == "STATE@NOTCH FILTER 4")
        return notch_filter_first_order_block_4.get_state();

    if(var_name == "STATE@PHASE TUNER 1")
        return phase_tuner_1.get_state();

    if(var_name == "STATE@PHASE TUNER 2")
        return phase_tuner_2.get_state();

    if(var_name == "STATE@PHASE TUNER 3")
        return phase_tuner_3.get_state();

    if(var_name == "OUTPUT@SPEED SENSOR")
        return speed_sensor.get_output();

    if(var_name == "OUTPUT@SPEED DEDC BLOCK 1")
        return speed_dedc_block_1.get_output();

    if(var_name == "OUTPUT@SPEED DEDC BLOCK 2")
        return speed_dedc_block_2.get_output();

    if(var_name == "OUTPUT@PELEC SENSOR")
        return pelec_sensor.get_output();

    if(var_name == "OUTPUT@PELEC DEDC BLOCK 1")
        return pelec_dedc_block_1.get_output();

    if(var_name == "OUTPUT@PELEC DEDC BLOCK 2")
        return pelec_dedc_block_2.get_output();

    if(var_name == "OUTPUT@NOTCH FILTER PHASE TUNER")
        return notch_filter_phase_tuner.get_output();

    if(var_name == "OUTPUT@NOTCH FILTER 1")
        return notch_filter_first_order_block_1.get_output();

    if(var_name == "OUTPUT@NOTCH FILTER 2")
        return notch_filter_first_order_block_2.get_output();

    if(var_name == "OUTPUT@NOTCH FILTER 3")
        return notch_filter_first_order_block_3.get_output();

    if(var_name == "OUTPUT@NOTCH FILTER 4")
        return notch_filter_first_order_block_4.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 1")
        return phase_tuner_1.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 2")
        return phase_tuner_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 3")
        return phase_tuner_3.get_output();

    return 0.0;
}



string PSASPS4::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS4::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS4::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
