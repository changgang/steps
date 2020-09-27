#include "header/model/sg_models/stabilizer_model/PSASPS2.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS2::PSASPS2(STEPS& toolkit) : STABILIZER_MODEL(toolkit),
                                   sensor_w(toolkit),
                                   sensor_p(toolkit),
                                   sensor_t(toolkit),
                                   dedc_block_1(toolkit),
                                   dedc_block_2(toolkit),
                                   phase_tuner_1(toolkit),
                                   phase_tuner_2(toolkit),
                                   phase_tuner_3(toolkit)
{
    clear();
}

PSASPS2::~PSASPS2()
{
}

void PSASPS2::clear()
{
    set_model_float_parameter_count(14);
}

void PSASPS2::copy_from_const_model(const PSASPS2& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    sensor_w.set_toolkit(toolkit);
    sensor_p.set_toolkit(toolkit);
    sensor_t.set_toolkit(toolkit);
    dedc_block_1.set_toolkit(toolkit);
    dedc_block_2.set_toolkit(toolkit);
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
    this->set_Kp(model.get_Kp());
    this->set_Kt(model.get_Kt());
    this->set_Tr_in_s(model.get_Tr_in_s());
    this->set_Tw1_in_s(model.get_Tw1_in_s());
    this->set_Tw2_in_s(model.get_Tw2_in_s());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
}
PSASPS2::PSASPS2(const PSASPS2& model) : STABILIZER_MODEL(model.get_toolkit()),
                                         sensor_w(model.get_toolkit()),
                                         sensor_p(model.get_toolkit()),
                                         sensor_t(model.get_toolkit()),
                                         dedc_block_1(model.get_toolkit()),
                                         dedc_block_2(model.get_toolkit()),
                                         phase_tuner_1(model.get_toolkit()),
                                         phase_tuner_2(model.get_toolkit()),
                                         phase_tuner_3(model.get_toolkit())
{
    copy_from_const_model(model);
}

PSASPS2& PSASPS2::operator=(const PSASPS2& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS2::get_model_name() const
{
    return "PSASPS2";
}

void PSASPS2::set_Kw(double K)
{
    Kw = K;
}

void PSASPS2::set_Kp(double K)
{
    Kp = K;
}

void PSASPS2::set_Kt(double K)
{
    Kt = K;
}

void PSASPS2::set_Tr_in_s(double T)
{
    sensor_w.set_T_in_s(T);
    sensor_p.set_T_in_s(T);
    sensor_t.set_T_in_s(T);
}

void PSASPS2::set_Tw1_in_s(double T)
{
    dedc_block_1.set_T_in_s(T);
    dedc_block_1.set_K(T);
}

void PSASPS2::set_Tw2_in_s(double T)
{
    dedc_block_2.set_T_in_s(T);
    dedc_block_2.set_K(T);
}

void PSASPS2::set_T1_in_s(double T)
{
    phase_tuner_1.set_T1_in_s(T);
}

void PSASPS2::set_T2_in_s(double T)
{
    phase_tuner_1.set_T2_in_s(T);
}

void PSASPS2::set_T3_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}

void PSASPS2::set_T4_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}

void PSASPS2::set_T5_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}

void PSASPS2::set_T6_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}

void PSASPS2::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void PSASPS2::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS2::get_Kw() const
{
    return Kw;
}

double PSASPS2::get_Kp() const
{
    return Kp;
}

double PSASPS2::get_Kt() const
{
    return Kt;
}

double PSASPS2::get_Tr_in_s() const
{
    return sensor_w.get_T_in_s();
}

double PSASPS2::get_Tw1_in_s() const
{
    return dedc_block_1.get_T_in_s();
}

double PSASPS2::get_Tw2_in_s() const
{
    return dedc_block_2.get_T_in_s();
}

double PSASPS2::get_T1_in_s() const
{
    return phase_tuner_1.get_T1_in_s();
}

double PSASPS2::get_T2_in_s() const
{
    return phase_tuner_1.get_T2_in_s();
}

double PSASPS2::get_T3_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}

double PSASPS2::get_T4_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}

double PSASPS2::get_T5_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}

double PSASPS2::get_T6_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double PSASPS2::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS2::get_Vsmin() const
{
    return Vsmin;
}

bool PSASPS2::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=17)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();

        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kw, kp, kt, tr, tw1, tw2, t1, t2, t3, t4, t5, t6, vsmax, vsmin;

            unsigned int i=3;
            kw = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            kt = get_double_data(data[i],"0.0"); i++;
            tr= get_double_data(data[i],"0.0"); i++;
            tw1= get_double_data(data[i],"0.0"); i++;
            tw2= get_double_data(data[i],"0.0"); i++;
            t1 = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            t4 = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            t6 = get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            if(tw1==0.0)
            {
                osstream<<"Error. Tw1 in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(tw2==0.0)
            {
                osstream<<"Error. Tw2 in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            set_Kw(kw);
            set_Kp(kp);
            set_Kt(kt);
            set_Tr_in_s(tr);
            set_Tw1_in_s(tw1);
            set_Tw2_in_s(tw2);
            set_T1_in_s(t1);
            set_T2_in_s(t2);
            set_T3_in_s(t3);
            set_T4_in_s(t4);
            set_T5_in_s(t5);
            set_T6_in_s(t6);
            set_Vsmax(vsmax);
            set_Vsmin(vsmin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        is_successful = false;

    return is_successful;
}

bool PSASPS2::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS2::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS2::setup_block_toolkit_and_parameters()
{
}

void PSASPS2::initialize()
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

        signal = prepare_signal_with_signal_type_and_device_id(7, did);
        if(signal.is_valid())
            set_input_signal_at_slot(2, signal);

        phase_tuner_3.set_output(0.0);
        phase_tuner_3.initialize();

        phase_tuner_2.set_output(0.0);
        phase_tuner_2.initialize();

        phase_tuner_1.set_output(0.0);
        phase_tuner_1.initialize();

        dedc_block_2.set_output(0.0);
        dedc_block_2.initialize();

        dedc_block_1.set_output(0.0);
        dedc_block_1.initialize();

        speed_deviation_ref_pu = get_signal_value_of_slot(0);
        Pe_ref_pu = get_signal_value_of_slot(1);
        Pmech_ref_pu = get_signal_value_of_slot(2);

        sensor_w.set_output(speed_deviation_ref_pu);
        sensor_w.initialize();

        sensor_p.set_output(Pe_ref_pu);
        sensor_p.initialize();

        sensor_t.set_output(Pmech_ref_pu);
        sensor_t.initialize();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS2::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        double speed_deviation_pu = get_signal_value_of_slot(0);
        double Pe_pu = get_signal_value_of_slot(1);
        double Pmech_pu = get_signal_value_of_slot(2);

        sensor_w.set_input(speed_deviation_pu);
        sensor_w.run(mode);

        sensor_p.set_input(Pe_pu);
        sensor_p.run(mode);

        sensor_t.set_input(Pmech_pu);
        sensor_t.run(mode);

        double input = Kw*(sensor_w.get_output()-speed_deviation_ref_pu)-Kp*(sensor_p.get_output()-Pe_ref_pu)-Kt*(Pmech_ref_pu-sensor_t.get_output());

        dedc_block_1.set_input(input);
        dedc_block_1.run(mode);
        input = dedc_block_1.get_output();

        dedc_block_2.set_input(input);
        dedc_block_2.run(mode);
        input = dedc_block_2.get_output();

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

double PSASPS2::get_stabilizing_signal_in_pu()
{
    double output = phase_tuner_3.get_output();
    if(output>Vsmax)
        output=Vsmax;
    if(output<Vsmin)
        output=Vsmin;

    return output;
}

void PSASPS2::check()
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
    if(get_Tw1_in_s()==0.0)
    {
        osstream<<"Tw1=0.0 was detected\n";
        error_found = true;
    }
    if(get_Tw2_in_s()==0.0)
    {
        osstream<<"Tw2=0.0 was detected\n";
        error_found = true;
    }
    /*if(get_Tw1_in_s()>20.0)
    {
        osstream<<"Tw1>20.0 was detected. Tw1="<<get_Tw1_in_s()<<"\n";
        error_found = true;
    }
    if(get_Tw2_in_s()>20.0)
    {
        osstream<<"Tw2>20.0 was detected. Tw2="<<get_Tw2_in_s()<<"\n";
        error_found = true;
    }*/
    if(get_T2_in_s()==0.0)
    {
        osstream<<"T2=0.0 was detected\n";
        error_found = true;
    }
    if(get_T4_in_s()==0.0)
    {
        osstream<<"T4=0.0 was detected\n";
        error_found = true;
    }
    if(get_T6_in_s()==0.0)
    {
        osstream<<"T6=0.0 was detected\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS2::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS2::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS2::get_standard_psse_string() const
{
    ostringstream osstream;

    double Kw = get_Kw();
    double Kp = get_Kp();
    double Kt = get_Kt();
    double Tr = get_Tr_in_s();
    double Tw1 = get_Tw1_in_s();
    double Tw2 = get_Tw2_in_s();
    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();
    double T5 = get_T5_in_s();
    double T6 = get_T6_in_s();
    double Vsmax = get_Vsmax();
    double Vsmin = get_Vsmin();

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<Kw<<", "
            <<setw(8)<<setprecision(6)<<Kp<<", "
            <<setw(8)<<setprecision(6)<<Kt<<", "
            <<setw(8)<<setprecision(6)<<Tr<<", "
            <<setw(8)<<setprecision(6)<<Tw1<<", "
            <<setw(8)<<setprecision(6)<<Tw2<<", "
            <<setw(8)<<setprecision(6)<<T1<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<T4<<", "
            <<setw(8)<<setprecision(6)<<T5<<", "
            <<setw(8)<<setprecision(6)<<T6<<", "
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS2::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KW", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KT", i); i++;
    add_model_data_name_and_index_pair("TR", i); i++;
    add_model_data_name_and_index_pair("TW1", i); i++;
    add_model_data_name_and_index_pair("TW2", i); i++;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("T4", i); i++;
    add_model_data_name_and_index_pair("T5", i); i++;
    add_model_data_name_and_index_pair("T6", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS2::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="KW")
        return get_Kw();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="KT")
        return get_Kt();
    if(par_name=="TR")
        return get_Tr_in_s();
    if(par_name=="TW1")
        return get_Tw1_in_s();
    if(par_name=="TW2")
        return get_Tw2_in_s();
    if(par_name=="T1")
        return get_T1_in_s();
    if(par_name=="T2")
        return get_T2_in_s();
    if(par_name=="T3")
        return get_T3_in_s();
    if(par_name=="T4")
        return get_T4_in_s();
    if(par_name=="T5")
        return get_T5_in_s();
    if(par_name=="T6")
        return get_T6_in_s();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS2::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KW")
        return set_Kw(value);
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="KT")
        return set_Kt(value);
    if(par_name=="TR")
        return set_Tr_in_s(value);
    if(par_name=="TW1")
        return set_Tw1_in_s(value);
    if(par_name=="TW2")
        return set_Tw2_in_s(value);
    if(par_name=="T1")
        return set_T1_in_s(value);
    if(par_name=="T2")
        return set_T2_in_s(value);
    if(par_name=="T3")
        return set_T3_in_s(value);
    if(par_name=="T4")
        return set_T4_in_s(value);
    if(par_name=="T5")
        return set_T5_in_s(value);
    if(par_name=="T6")
        return set_T6_in_s(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

double PSASPS2::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tr_in_s()!=0.0 and mint>get_Tr_in_s())
        mint = get_Tr_in_s();
    if(get_Tw1_in_s()!=0.0 and mint>get_Tw1_in_s())
        mint = get_Tw1_in_s();
    if(get_Tw2_in_s()!=0.0 and mint>get_Tw2_in_s())
        mint = get_Tw2_in_s();
    if(get_T2_in_s()!=0.0 and mint>get_T2_in_s())
        mint = get_T2_in_s();
    if(get_T4_in_s()!=0.0 and mint>get_T4_in_s())
        mint = get_T4_in_s();
    if(get_T6_in_s()!=0.0 and mint>get_T6_in_s())
        mint = get_T6_in_s();
    return mint;
}

void PSASPS2::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PELEC SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PMECH SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@SPEED SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PELEC SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PMECH SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
}

double PSASPS2::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "SIGNAL@SLOT 2")
        return get_signal_value_of_slot(1);

    if(var_name == "SIGNAL@SLOT 3")
        return get_signal_value_of_slot(2);

    if(var_name == "STATE@SPEED SENSOR")
        return sensor_w.get_state();

    if(var_name == "STATE@PELEC SENSOR")
        return sensor_p.get_state();

    if(var_name == "STATE@PMECH SENSOR")
        return sensor_t.get_state();

    if(var_name == "STATE@DEDC BLOCK 1")
        return dedc_block_1.get_state();

    if(var_name == "STATE@DEDC BLOCK 2")
        return dedc_block_2.get_state();

    if(var_name == "STATE@PHASE TUNER 1")
        return phase_tuner_1.get_state();

    if(var_name == "STATE@PHASE TUNER 2")
        return phase_tuner_2.get_state();

    if(var_name == "STATE@PHASE TUNER 3")
        return phase_tuner_3.get_state();

    if(var_name == "OUTPUT@SPEED SENSOR")
        return sensor_w.get_output();

    if(var_name == "OUTPUT@PELEC SENSOR")
        return sensor_p.get_output();

    if(var_name == "OUTPUT@PMECH SENSOR")
        return sensor_t.get_output();

    if(var_name == "OUTPUT@DEDC BLOCK 1")
        return dedc_block_1.get_output();

    if(var_name == "OUTPUT@DEDC BLOCK 2")
        return dedc_block_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 1")
        return phase_tuner_1.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 2")
        return phase_tuner_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 3")
        return phase_tuner_3.get_output();

    return 0.0;
}



string PSASPS2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS2::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS2::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
