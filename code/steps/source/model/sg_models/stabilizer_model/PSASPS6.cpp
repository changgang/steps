#include "header/model/sg_models/stabilizer_model/PSASPS6.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
#include <iostream>

using namespace std;

/***
It should be noted that, the first Dae-DC block of Pelec is a FIRST_ORDER_BLOCK in manual of PSASP V7.3, July 2017.
However, it is modeled as a DIFFERENTIAL_BLOCK in STEPS.
Further check is necessary.
***/
PSASPS6::PSASPS6(STEPS& toolkit) : STABILIZER_MODEL(toolkit),
                                   speed_sensor(toolkit),
                                   pelec_sensor(toolkit),
                                   speed_dedc_block_1(toolkit),
                                   speed_dedc_block_2(toolkit),
                                   pelec_dedc_block_1(toolkit),
                                   pelec_dedc_block_2(toolkit),
                                   phase_tuner_1(toolkit),
                                   phase_tuner_2(toolkit),
                                   phase_tuner_3(toolkit)
{
    clear();
}

PSASPS6::~PSASPS6()
{
}

void PSASPS6::clear()
{
}

void PSASPS6::copy_from_const_model(const PSASPS6& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    speed_sensor.set_toolkit(toolkit);
    pelec_sensor.set_toolkit(toolkit);
    speed_dedc_block_1.set_toolkit(toolkit);
    speed_dedc_block_2.set_toolkit(toolkit);
    pelec_dedc_block_1.set_toolkit(toolkit);
    pelec_dedc_block_2.set_toolkit(toolkit);
    phase_tuner_1.set_toolkit(toolkit);
    phase_tuner_2.set_toolkit(toolkit);
    phase_tuner_3.set_toolkit(toolkit);

    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    for(unsigned int i=0; i!=STEPS_MAX_STABILIZER_INPUT_SIGNAL_SLOT; ++i)
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
PSASPS6::PSASPS6(const PSASPS6& model) : STABILIZER_MODEL(model.get_toolkit()),
                                         speed_sensor(model.get_toolkit()),
                                         pelec_sensor(model.get_toolkit()),
                                         speed_dedc_block_1(model.get_toolkit()),
                                         speed_dedc_block_2(model.get_toolkit()),
                                         pelec_dedc_block_1(model.get_toolkit()),
                                         pelec_dedc_block_2(model.get_toolkit()),
                                         phase_tuner_1(model.get_toolkit()),
                                         phase_tuner_2(model.get_toolkit()),
                                         phase_tuner_3(model.get_toolkit())
{
    copy_from_const_model(model);
}

PSASPS6& PSASPS6::operator=(const PSASPS6& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS6::get_model_name() const
{
    return "PSASPS6";
}

void PSASPS6::set_Kw(double K)
{
    speed_sensor.set_K(K);
}

void PSASPS6::set_Trw_in_s(double T)
{
    speed_sensor.set_T_in_s(T);
}

void PSASPS6::set_T5_in_s(double T)
{
    speed_dedc_block_1.set_K(T);
}

void PSASPS6::set_T6_in_s(double T)
{
    speed_dedc_block_1.set_T_in_s(T);
}

void PSASPS6::set_T7_in_s(double T)
{
    speed_dedc_block_2.set_K(T);
    speed_dedc_block_2.set_T_in_s(T);
}

void PSASPS6::set_Kr(double K)
{
    pelec_sensor.set_K(K);
}

void PSASPS6::set_Trp_in_s(double T)
{
    pelec_sensor.set_T_in_s(T);
}

void PSASPS6::set_Tw_in_s(double T)
{
    pelec_dedc_block_1.set_K(T);
}

void PSASPS6::set_Tw1_in_s(double T)
{
    pelec_dedc_block_1.set_T_in_s(T);
}

void PSASPS6::set_Tw2_in_s(double T)
{
    pelec_dedc_block_2.set_K(T);
    pelec_dedc_block_2.set_T_in_s(T);
}

void PSASPS6::set_Ks(double K)
{
    Ks = K;
}

void PSASPS6::set_Kp(double K)
{
    phase_tuner_1.set_K(K);
}

void PSASPS6::set_T1_in_s(double T)
{
    phase_tuner_1.set_T1_in_s(T);
}

void PSASPS6::set_T2_in_s(double T)
{
    phase_tuner_1.set_T2_in_s(T);
}

void PSASPS6::set_T13_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}

void PSASPS6::set_T14_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}

void PSASPS6::set_T3_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}

void PSASPS6::set_T4_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}

void PSASPS6::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void PSASPS6::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS6::get_Kw() const
{
    return speed_sensor.get_K();
}

double PSASPS6::get_Trw_in_s() const
{
    return speed_sensor.get_T_in_s();
}

double PSASPS6::get_T5_in_s() const
{
    return speed_dedc_block_1.get_K();
}

double PSASPS6::get_T6_in_s() const
{
    return speed_dedc_block_1.get_T_in_s();
}

double PSASPS6::get_T7_in_s() const
{
    return speed_dedc_block_2.get_T_in_s();
}

double PSASPS6::get_Kr() const
{
    return pelec_sensor.get_K();
}

double PSASPS6::get_Trp_in_s() const
{
    return pelec_sensor.get_T_in_s();
}

double PSASPS6::get_Tw_in_s() const
{
    return pelec_dedc_block_1.get_K();
}

double PSASPS6::get_Tw1_in_s() const
{
    return pelec_dedc_block_1.get_T_in_s();
}

double PSASPS6::get_Tw2_in_s() const
{
    return pelec_dedc_block_2.get_T_in_s();
}

double PSASPS6::get_Ks() const
{
    return Ks;
}

double PSASPS6::get_Kp() const
{
    return phase_tuner_1.get_K();
}

double PSASPS6::get_T1_in_s() const
{
    return phase_tuner_1.get_T1_in_s();
}

double PSASPS6::get_T2_in_s() const
{
    return phase_tuner_1.get_T2_in_s();
}

double PSASPS6::get_T13_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}

double PSASPS6::get_T14_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}

double PSASPS6::get_T3_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}

double PSASPS6::get_T4_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double PSASPS6::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS6::get_Vsmin() const
{
    return Vsmin;
}


bool PSASPS6::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=23)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kw, trw, t5, t6, t7, kr, trp, tw, tw1, tw2, ks, kp, t1, t2, t13, t14, t3, t4,
                   vsmax, vsmin;

            unsigned int i=3;
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

bool PSASPS6::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS6::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS6::setup_block_toolkit_and_parameters()
{
}

void PSASPS6::initialize()
{
    ostringstream osstream;

    GENERATOR* generator = get_generator_pointer();
    DEVICE_ID did = generator->get_device_id();

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

    set_flag_model_initialized_as_true();
}

void PSASPS6::run(DYNAMIC_MODE mode)
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

        input = speed_dedc_block_2.get_output()+(Ks-1)*pelec_dedc_block_2.get_output();

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

double PSASPS6::get_stabilizing_signal_in_pu()
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

void PSASPS6::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    double vsmax = get_Vsmax();
    double vsmin = get_Vsmin();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(vsmax<=vsmin)
    {
        osstream<<"Vsmax<=Vsmin was detected: Vsmax="<<vsmax<<", Vsmin="<<vsmin<<"\n";
        error_found = true;
    }
    if(vsmax<=0.0)
    {
        osstream<<"Vsmax<=0.0 was detected: Vsmax="<<vsmax<<"\n";
        error_found = true;
    }
    if(vsmin>=0.0)
    {
        osstream<<"Vsmin>=0.0 was detected: Vsmin="<<vsmin<<"\n";
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
    if(get_Kp()>20.0)
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

void PSASPS6::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS6::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS6::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
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
    double Kp = get_Kp();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T13 = get_T13_in_s();
    double T14 = get_T14_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

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
            <<setw(8)<<setprecision(6)<<Kp<<", "
            <<setw(8)<<setprecision(6)<<T1<<", "
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T13<<", "
            <<setw(8)<<setprecision(6)<<T14<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<T4<<", "
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS6::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double PSASPS6::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void PSASPS6::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double PSASPS6::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Trw_in_s()!=0.0 and mint>get_Trw_in_s())
        mint = get_Trw_in_s();
    if(get_T6_in_s()!=0.0 and mint>get_T6_in_s())
        mint = get_T6_in_s();
    if(get_T7_in_s()!=0.0 and mint>get_T7_in_s())
        mint = get_T7_in_s();
    if(get_Trp_in_s()!=0.0 and mint>get_Trp_in_s())
        mint = get_Trp_in_s();
    if(get_Tw1_in_s()!=0.0 and mint>get_Tw1_in_s())
        mint = get_Tw1_in_s();
    if(get_Tw2_in_s()!=0.0 and mint>get_Tw2_in_s())
        mint = get_Tw2_in_s();
    if(get_T2_in_s()!=0.0 and mint>get_T2_in_s())
        mint = get_T2_in_s();
    if(get_T14_in_s()!=0.0 and mint>get_T14_in_s())
        mint = get_T14_in_s();
    if(get_T4_in_s()!=0.0 and mint>get_T4_in_s())
        mint = get_T4_in_s();
    return mint;
}

void PSASPS6::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_internal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SPEED SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SPEED DEDC BLOCK 1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SPEED DEDC BLOCK 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PELEC SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PELEC DEDC BLOCK 1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PELEC DEDC BLOCK 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@SPEED SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@SPEED DEDC BLOCK 1", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@SPEED DEDC BLOCK 2", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PELEC SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PELEC DEDC BLOCK 1", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PELEC DEDC BLOCK 2", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_internal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
}

double PSASPS6::get_model_internal_variable_with_name(string var_name)
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

    if(var_name == "OUTPUT@PHASE TUNER 1")
        return phase_tuner_1.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 2")
        return phase_tuner_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 3")
        return phase_tuner_3.get_output();

    return 0.0;
}



string PSASPS6::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS6::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS6::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void PSASPS6::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("PSS", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("PSS-AVR", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("PSS-GEN", matrix);
    // do linearization
}
