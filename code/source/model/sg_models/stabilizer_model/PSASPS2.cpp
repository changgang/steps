#include "header/model/sg_models/stabilizer_model/PSASPS2.h"
#include "header/basic/constants.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS2::PSASPS2()
{
    clear();
}

PSASPS2::~PSASPS2()
{
    clear();
}

void PSASPS2::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void PSASPS2::copy_from_const_model(const PSASPS2& model)
{
    clear();

    //this->set_power_system_database(model.get_default_power_system_database());
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
    this->set_Kp(model.get_Kp());
    this->set_Kt(model.get_Kt());
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
PSASPS2::PSASPS2(const PSASPS2& model) : STABILIZER_MODEL()
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
    if(data.size()<16)
        is_successful = false;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;

    double kw, kp, kt, tw1, tw2, t1, t2, t3, t4, t5, t6, vsmax, vsmin;

    size_t i=3;
    kw = get_double_data(data[i],"0.0"); i++;
    kp = get_double_data(data[i],"0.0"); i++;
    kt = get_double_data(data[i],"0.0"); i++;
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

    set_Kw(kw);
    set_Kp(kp);
    set_Kt(kt);
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
    show_information_with_leading_time_stamp(osstream);
    return false;
}


void PSASPS2::initialize()
{
    ostringstream osstream;

    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
    {
        deactivate_model();
        set_flag_model_initialized_as_true();
        return;
    }

    EXCITER_MODEL* exciter = generator->get_exciter_model();
    if(exciter==NULL)
    {
        deactivate_model();
        set_flag_model_initialized_as_true();
        return;
    }
    if(not exciter->is_model_initialized())
        exciter->initialize();

    size_t bus = generator->get_generator_bus();

    SIGNAL signal = prepare_signal_with_signal_type_and_bus(1, bus);
    if(signal.is_valid())
        set_input_signal_at_slot(0, signal);

    signal = prepare_signal_with_signal_type_and_bus(3, bus);
    if(signal.is_valid())
        set_input_signal_at_slot(1, signal);

    signal = prepare_signal_with_signal_type_and_bus(7, bus);
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

    set_flag_model_initialized_as_true();
}

void PSASPS2::run(DYNAMIC_MODE mode)
{
    if(not is_model_active())
        return;

    double speed_deviation_pu = get_signal_value_of_slot(0);
    double Pe_pu = get_signal_value_of_slot(1);
    double Pmech_pu = get_signal_value_of_slot(2);

    double input = Kw*(speed_deviation_pu-speed_deviation_ref_pu)-Kp*(Pe_pu-Pe_ref_pu)-Kt*(Pmech_ref_pu-Pmech_pu);

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

double PSASPS2::get_stabilizing_signal_in_pu() const
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
    ;
}

void PSASPS2::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void PSASPS2::save()
{
    string model = get_standard_model_string();
    return;
}

string PSASPS2::get_standard_model_string() const
{
    ostringstream osstream;

    double Kw = get_Kw();
    double Kp = get_Kp();
    double Kt = get_Kt();
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
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<Kw<<", "
      <<setw(8)<<setprecision(6)<<Kp<<", "
      <<setw(8)<<setprecision(6)<<Kt<<", "
      <<setw(8)<<setprecision(6)<<Tw1<<", "
      <<setw(8)<<setprecision(6)<<Tw2<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
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
    size_t i=0;
    add_model_data_name_and_index_pair("KW", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KT", i); i++;
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

void PSASPS2::prepare_model_internal_variable_table()
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
