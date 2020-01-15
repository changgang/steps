#include "header/model/sg_models/stabilizer_model/PSASPS3.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS3::PSASPS3()
{
    clear();
}

PSASPS3::~PSASPS3()
{
}

void PSASPS3::clear()
{
    set_model_float_parameter_count(18);
}

void PSASPS3::copy_from_const_model(const PSASPS3& model)
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

    this->set_Macc(model.get_Macc());
    this->set_Iacc(model.get_Iacc());
    this->set_Nacc(model.get_Nacc());
    this->set_dedc_1_flag(model.get_dedc_1_flag());
    this->set_Tacc_in_s(model.get_Tacc_in_s());
    this->set_TD_in_s(model.get_TD_in_s());
    this->set_Ti1_in_s(model.get_Ti1_in_s());
    this->set_Ti2_in_s(model.get_Ti2_in_s());
    this->set_Ti3_in_s(model.get_Ti3_in_s());
    this->set_Ti4_in_s(model.get_Ti4_in_s());
    this->set_K0(model.get_K0());
    this->set_K1(model.get_K1());
    this->set_K2(model.get_K2());
    this->set_K3(model.get_K3());
    this->set_K4(model.get_K4());
    this->set_KP(model.get_KP());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
}
PSASPS3::PSASPS3(const PSASPS3& model) : STABILIZER_MODEL()
{
    copy_from_const_model(model);
}

PSASPS3& PSASPS3::operator=(const PSASPS3& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS3::get_model_name() const
{
    return "PSASPS3";
}

void PSASPS3::set_Macc(double M)
{
    Macc = M;
}

void PSASPS3::set_Iacc(double I)
{
    Iacc = I;
}

void PSASPS3::set_Nacc(double N)
{
    Nacc = N;
}

void PSASPS3::set_dedc_1_flag(bool flag)
{
    dedc_block_1_flag = flag;
}

void PSASPS3::set_Tacc_in_s(double T)
{
    dedc_block_1.set_T_in_s(T);
    dedc_block_1.set_K(T);
}

void PSASPS3::set_TD_in_s(double T)
{
    dedc_block_2.set_T_in_s(T);
    dedc_block_2.set_K(T);
}

void PSASPS3::set_Ti1_in_s(double T)
{
    phase_tuner_1.set_T_in_s(T);
}

void PSASPS3::set_Ti2_in_s(double T)
{
    phase_tuner_2.set_T_in_s(T);
}

void PSASPS3::set_Ti3_in_s(double T)
{
    phase_tuner_3.set_T_in_s(T);
}

void PSASPS3::set_Ti4_in_s(double T)
{
    phase_tuner_4.set_T_in_s(T);
}

void PSASPS3::set_K0(double K)
{
    K0 = K;
}

void PSASPS3::set_K1(double K)
{
    K1 = K;
}

void PSASPS3::set_K2(double K)
{
    K2 = K;
}

void PSASPS3::set_K3(double K)
{
    K3 = K;
}

void PSASPS3::set_K4(double K)
{
    K4 = K;
}

void PSASPS3::set_KP(double K)
{
    KP = K;
}

void PSASPS3::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}

void PSASPS3::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS3::get_Macc() const
{
    return Macc;
}

double PSASPS3::get_Iacc() const
{
    return Iacc;
}

double PSASPS3::get_Nacc() const
{
    return Nacc;
}

bool PSASPS3::get_dedc_1_flag() const
{
    return dedc_block_1_flag;
}

double PSASPS3::get_Tacc_in_s() const
{
    return dedc_block_1.get_T_in_s();
}

double PSASPS3::get_TD_in_s() const
{
    return dedc_block_2.get_T_in_s();
}

double PSASPS3::get_Ti1_in_s() const
{
    return phase_tuner_1.get_T_in_s();
}

double PSASPS3::get_Ti2_in_s() const
{
    return phase_tuner_2.get_T_in_s();
}

double PSASPS3::get_Ti3_in_s() const
{
    return phase_tuner_3.get_T_in_s();
}

double PSASPS3::get_Ti4_in_s() const
{
    return phase_tuner_4.get_T_in_s();
}

double PSASPS3::get_K0() const
{
    return K0;
}

double PSASPS3::get_K1() const
{
    return K1;
}

double PSASPS3::get_K2() const
{
    return K2;
}

double PSASPS3::get_K3() const
{
    return K3;
}

double PSASPS3::get_K4() const
{
    return K4;
}

double PSASPS3::get_KP() const
{
    return KP;
}

double PSASPS3::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS3::get_Vsmin() const
{
    return Vsmin;
}

bool PSASPS3::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=21)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            int dedc1_flag;
            double macc, iacc, nacc, tacc, td, ti1, ti2, ti3, ti4, k0, k1, k2, k3, k4, kp, vsmax, vsmin;

            size_t i=3;
            macc = get_double_data(data[i],"0.0"); i++;
            iacc = get_double_data(data[i],"0.0"); i++;
            nacc = get_double_data(data[i],"0.0"); i++;
            dedc1_flag= get_integer_data(data[i],"0"); i++;
            tacc= get_double_data(data[i],"0.0"); i++;
            td= get_double_data(data[i],"0.0"); i++;
            ti1 = get_double_data(data[i],"0.0"); i++;
            ti2 = get_double_data(data[i],"0.0"); i++;
            ti3 = get_double_data(data[i],"0.0"); i++;
            ti4 = get_double_data(data[i],"0.0"); i++;
            k0 = get_double_data(data[i],"0.0"); i++;
            k1 = get_double_data(data[i],"0.0"); i++;
            k2 = get_double_data(data[i],"0.0"); i++;
            k3 = get_double_data(data[i],"0.0"); i++;
            k4 = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            set_Macc(macc);
            set_Iacc(iacc);
            set_Nacc(nacc);
            set_dedc_1_flag((dedc1_flag==0?false:true));
            if(tacc == 0.0)
                tacc = td;
            set_Tacc_in_s(tacc);
            set_TD_in_s(td);
            set_Ti1_in_s(ti1);
            set_Ti2_in_s(ti2);
            set_Ti3_in_s(ti3);
            set_Ti4_in_s(ti4);
            set_K0(k0);
            set_K1(k1);
            set_K2(k2);
            set_K3(k3);
            set_K4(k4);
            set_KP(kp);
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

bool PSASPS3::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS3::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS3::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    dedc_block_1.set_toolkit(toolkit);
    dedc_block_2.set_toolkit(toolkit);
    phase_tuner_1.set_toolkit(toolkit);
    phase_tuner_2.set_toolkit(toolkit);
    phase_tuner_3.set_toolkit(toolkit);
    phase_tuner_4.set_toolkit(toolkit);
}

void PSASPS3::initialize()
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

            size_t bus = generator->get_generator_bus();

            SIGNAL signal = prepare_signal_with_signal_type_and_device_id(1, did);
            if(signal.is_valid())
                set_input_signal_at_slot(0, signal);

            signal = prepare_signal_with_signal_type_and_device_id(3, did);
            if(signal.is_valid())
                set_input_signal_at_slot(1, signal);

            signal = prepare_signal_with_signal_type_and_device_id(7, did);
            if(signal.is_valid())
                set_input_signal_at_slot(2, signal);

            phase_tuner_4.set_output(0.0);
            phase_tuner_4.initialize();

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
        }
        else
            deactivate_model();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS3::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        double speed_deviation_pu = get_signal_value_of_slot(0);
        double Pe_pu = get_signal_value_of_slot(1);
        double Pmech_pu = get_signal_value_of_slot(2);

        double input = Macc*(speed_deviation_pu-speed_deviation_ref_pu)+Iacc*(Pmech_pu-Pmech_ref_pu)-Nacc*(Pe_pu-Pe_ref_pu);

        if(get_dedc_1_flag()==false)
        {
            dedc_block_1.set_input(input);
            dedc_block_1.run(mode);
            input = dedc_block_1.get_output();
        }

        dedc_block_2.set_input(input);
        dedc_block_2.run(mode);
        input = dedc_block_2.get_output();

        input -= (phase_tuner_1.get_output()+phase_tuner_2.get_output()+phase_tuner_3.get_output()+phase_tuner_4.get_output());

        phase_tuner_1.set_input(input);
        phase_tuner_1.run(mode);
        input = phase_tuner_1.get_output();

        phase_tuner_2.set_input(input);
        phase_tuner_2.run(mode);
        input = phase_tuner_2.get_output();

        phase_tuner_3.set_input(input);
        phase_tuner_3.run(mode);
        input = phase_tuner_3.get_output();

        phase_tuner_4.set_input(input);
        phase_tuner_4.run(mode);

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double PSASPS3::get_stabilizing_signal_in_pu()
{
    double s1 = phase_tuner_1.get_output();
    double s2 = phase_tuner_2.get_output();
    double s3 = phase_tuner_3.get_output();
    double s4 = phase_tuner_4.get_output();
    double output = K0*(dedc_block_2.get_output()-(s1+s2+s3+s4))+
                    K1*s1+
                    K2*s2+
                    K3*s3+
                    K4*s4;
    output *= KP;

    if(output>Vsmax)
        output=Vsmax;
    if(output<Vsmin)
        output=Vsmin;

    return output;
}

void PSASPS3::check()
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
    if(get_Tacc_in_s()==0.0)
    {
        osstream<<"Tacc=0.0 was detected\n";
        error_found = true;
    }
    if(get_TD_in_s()==0.0)
    {
        osstream<<"TD=0.0 was detected\n";
        error_found = true;
    }
    if(get_Ti1_in_s()==0.0)
    {
        osstream<<"Ti1=0.0 was detected\n";
        error_found = true;
    }
    if(get_Ti2_in_s()==0.0)
    {
        osstream<<"Ti2=0.0 was detected\n";
        error_found = true;
    }
    if(get_Ti3_in_s()==0.0)
    {
        osstream<<"Ti3=0.0 was detected\n";
        error_found = true;
    }
    if(get_Ti4_in_s()==0.0)
    {
        osstream<<"Ti4=0.0 was detected\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS3::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS3::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS3::get_standard_psse_string() const
{
    ostringstream osstream;

    double Macc = get_Macc();
    double Iacc = get_Iacc();
    double Nacc = get_Nacc();
    size_t flag = (get_dedc_1_flag()==true?1:0);
    double Tacc = get_Tacc_in_s();
    double TD = get_TD_in_s();
    double Ti1 = get_Ti1_in_s();
    double Ti2 = get_Ti2_in_s();
    double Ti3 = get_Ti3_in_s();
    double Ti4 = get_Ti4_in_s();
    double K0 = get_K0();
    double K1 = get_K1();
    double K2 = get_K2();
    double K3 = get_K3();
    double K4 = get_K4();
    double KP = get_KP();
    double Vsmax = get_Vsmax();
    double Vsmin = get_Vsmin();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<Macc<<", "
            <<setw(8)<<setprecision(6)<<Iacc<<", "
            <<setw(8)<<setprecision(6)<<Nacc<<", "
            <<setw(8)<<flag<<", "
            <<setw(8)<<setprecision(6)<<Tacc<<", "
            <<setw(8)<<setprecision(6)<<TD<<", "
            <<setw(8)<<setprecision(6)<<Ti1<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Ti2<<", "
            <<setw(8)<<setprecision(6)<<Ti3<<", "
            <<setw(8)<<setprecision(6)<<Ti4<<", "
            <<setw(8)<<setprecision(6)<<K0<<", "
            <<setw(8)<<setprecision(6)<<K1<<", "
            <<setw(8)<<setprecision(6)<<K2<<", "
            <<setw(8)<<setprecision(6)<<K3<<", "
            <<setw(8)<<setprecision(6)<<K4<<", "
            <<setw(8)<<setprecision(6)<<KP<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS3::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("MACC", i); i++;
    add_model_data_name_and_index_pair("IACC", i); i++;
    add_model_data_name_and_index_pair("NACC", i); i++;
    add_model_data_name_and_index_pair("FLAG", i); i++;
    add_model_data_name_and_index_pair("TACC", i); i++;
    add_model_data_name_and_index_pair("TD", i); i++;
    add_model_data_name_and_index_pair("TI1", i); i++;
    add_model_data_name_and_index_pair("TI2", i); i++;
    add_model_data_name_and_index_pair("TI3", i); i++;
    add_model_data_name_and_index_pair("TI4", i); i++;
    add_model_data_name_and_index_pair("K0", i); i++;
    add_model_data_name_and_index_pair("K1", i); i++;
    add_model_data_name_and_index_pair("K2", i); i++;
    add_model_data_name_and_index_pair("K3", i); i++;
    add_model_data_name_and_index_pair("K4", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS3::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="MACC")
        return get_Macc();
    if(par_name=="IACC")
        return get_Iacc();
    if(par_name=="NACC")
        return get_Nacc();
    if(par_name=="FLAG")
        return (get_dedc_1_flag()?1:0);
    if(par_name=="TACC")
        return get_Tacc_in_s();
    if(par_name=="TD")
        return get_TD_in_s();
    if(par_name=="TI1")
        return get_Ti1_in_s();
    if(par_name=="TI2")
        return get_Ti2_in_s();
    if(par_name=="TI3")
        return get_Ti3_in_s();
    if(par_name=="TI4")
        return get_Ti4_in_s();
    if(par_name=="K0")
        return get_K0();
    if(par_name=="K1")
        return get_K1();
    if(par_name=="K2")
        return get_K2();
    if(par_name=="K3")
        return get_K3();
    if(par_name=="K4")
        return get_K4();
    if(par_name=="KP")
        return get_KP();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS3::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="MACC")
        return set_Macc(value);
    if(par_name=="IACC")
        return set_Iacc(value);
    if(par_name=="NACC")
        return set_Nacc(value);
    if(par_name=="FLAG")
        return set_dedc_1_flag(value);
    if(par_name=="TACC")
        return set_Tacc_in_s(value);
    if(par_name=="TD")
        return set_TD_in_s(value);
    if(par_name=="TI1")
        return set_Ti1_in_s(value);
    if(par_name=="TI2")
        return set_Ti2_in_s(value);
    if(par_name=="TI3")
        return set_Ti3_in_s(value);
    if(par_name=="TI4")
        return set_Ti4_in_s(value);
    if(par_name=="K0")
        return set_K0(value);
    if(par_name=="K1")
        return set_K1(value);
    if(par_name=="K2")
        return set_K2(value);
    if(par_name=="K3")
        return set_K3(value);
    if(par_name=="K4")
        return set_K4(value);
    if(par_name=="KP")
        return set_KP(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

void PSASPS3::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 4", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@DEDC BLOCK 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@DEDC BLOCK 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 4", i); i++;
}

double PSASPS3::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "SIGNAL@SLOT 2")
        return get_signal_value_of_slot(1);

    if(var_name == "SIGNAL@SLOT 3")
        return get_signal_value_of_slot(2);

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

    if(var_name == "STATE@PHASE TUNER 4")
        return phase_tuner_4.get_state();

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

    if(var_name == "OUTPUT@PHASE TUNER 4")
        return phase_tuner_4.get_output();

    return 0.0;
}



string PSASPS3::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS3::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS3::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
