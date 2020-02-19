#include "header/model/sg_models/stabilizer_model/PSASPS1.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS1::PSASPS1(STEPS& toolkit) : STABILIZER_MODEL(toolkit),
                                   dedc_block(toolkit),
                                   phase_tuner_1(toolkit),
                                   phase_tuner_2(toolkit),
                                   phase_tuner_3(toolkit)
{
    clear();
}

PSASPS1::~PSASPS1()
{
}

void PSASPS1::clear()
{
    set_model_float_parameter_count(11);

    dedc_block.set_K(1.0);
    phase_tuner_1.set_T1_in_s(1.0);
}

void PSASPS1::copy_from_const_model(const PSASPS1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    dedc_block.set_toolkit(toolkit);
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

    this->set_Kq1(model.get_Kq1());
    this->set_Kq2(model.get_Kq2());
    this->set_Kq3(model.get_Kq3());
    this->set_K(model.get_K());
    this->set_Tq_in_s(model.get_Tq_in_s());
    this->set_T1e_in_s(model.get_T1e_in_s());
    this->set_T2e_in_s(model.get_T2e_in_s());
    this->set_T3e_in_s(model.get_T3e_in_s());
    this->set_T4e_in_s(model.get_T4e_in_s());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
}
PSASPS1::PSASPS1(const PSASPS1& model) : STABILIZER_MODEL(model.get_toolkit()),
                                         dedc_block(model.get_toolkit()),
                                         phase_tuner_1(model.get_toolkit()),
                                         phase_tuner_2(model.get_toolkit()),
                                         phase_tuner_3(model.get_toolkit())
{
    copy_from_const_model(model);
}

PSASPS1& PSASPS1::operator=(const PSASPS1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS1::get_model_name() const
{
    return "PSASPS1";
}

void PSASPS1::set_Kq1(double K)
{
    Kq1 = K;
}

void PSASPS1::set_Kq2(double K)
{
    Kq2 = K;
}

void PSASPS1::set_Kq3(double K)
{
    Kq3 = K;
}

void PSASPS1::set_K(unsigned int K)
{
    if(K==0)
        this->K = 0;
    else
        this->K = 1;
}

void PSASPS1::set_Tq_in_s(double T)
{
    dedc_block.set_T_in_s(T);
    phase_tuner_1.set_T2_in_s(T);
}

void PSASPS1::set_T1e_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}

void PSASPS1::set_T2e_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}

void PSASPS1::set_T3e_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}

void PSASPS1::set_T4e_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}

void PSASPS1::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void PSASPS1::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS1::get_Kq1() const
{
    return Kq1;
}

double PSASPS1::get_Kq2() const
{
    return Kq2;
}

double PSASPS1::get_Kq3() const
{
    return Kq3;
}

unsigned int PSASPS1::get_K() const
{
    return K;
}

double PSASPS1::get_Tq_in_s() const
{
    return dedc_block.get_T_in_s();
}

double PSASPS1::get_T1e_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}

double PSASPS1::get_T2e_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}

double PSASPS1::get_T3e_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}

double PSASPS1::get_T4e_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double PSASPS1::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS1::get_Vsmin() const
{
    return Vsmin;
}

bool PSASPS1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=14)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int k;
            double kq1, kq2, kq3, tq, t1e, t2e, t3e, t4e, vsmax, vsmin;

            unsigned int i=3;
            kq1 = get_double_data(data[i],"0.0"); i++;
            kq2 = get_double_data(data[i],"0.0"); i++;
            kq3 = get_double_data(data[i],"0.0"); i++;
            k = get_integer_data(data[i],"0"); i++;
            tq = get_double_data(data[i],"0.0"); i++;
            t1e = get_double_data(data[i],"0.0"); i++;
            t2e = get_double_data(data[i],"0.0"); i++;
            t3e = get_double_data(data[i],"0.0"); i++;
            t4e = get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            set_Kq1(kq1);
            set_Kq2(kq2);
            set_Kq3(kq3);
            set_K(k);
            if(tq==0.0)
                tq = 0.01;
            set_Tq_in_s(tq);
            set_T1e_in_s(t1e);
            set_T2e_in_s(t2e);
            set_T3e_in_s(t3e);
            set_T4e_in_s(t4e);
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

bool PSASPS1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS1::setup_block_toolkit_and_parameters()
{
}

void PSASPS1::initialize()
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

            unsigned int bus = generator->get_generator_bus();

            SIGNAL signal = prepare_signal_with_signal_type_and_device_id(1, did);
            if(signal.is_valid())
                set_input_signal_at_slot(0, signal);

            signal = prepare_signal_with_signal_type_and_device_id(3, did);
            if(signal.is_valid())
                set_input_signal_at_slot(1, signal);

            DEVICE_ID bus_device;
            bus_device.set_device_type("BUS");
            TERMINAL terminal;
            terminal.append_bus(bus);
            bus_device.set_device_terminal(terminal);

            signal = prepare_signal_with_signal_type_and_device_id(5, bus_device);
            if(signal.is_valid())
                set_input_signal_at_slot(2, signal);

            phase_tuner_3.set_output(0.0);
            phase_tuner_3.initialize();

            phase_tuner_2.set_output(0.0);
            phase_tuner_2.initialize();

            phase_tuner_1.set_output(0.0);
            phase_tuner_1.initialize();

            dedc_block.set_output(0.0);
            dedc_block.initialize();

            speed_deviation_ref_pu = get_signal_value_of_slot(0);
            Pe_ref_pu = get_signal_value_of_slot(1);
            Vterminal_ref_pu = get_signal_value_of_slot(2);
        }
        else
            deactivate_model();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS1::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        double speed_deviation_pu = get_signal_value_of_slot(0);
        double Pe_pu = get_signal_value_of_slot(1);
        double Vterminal_pu = get_signal_value_of_slot(2);

        double input = Kq1*(speed_deviation_pu-speed_deviation_ref_pu)-Kq2*(Pe_pu-Pe_ref_pu)-Kq3*(Vterminal_pu-Vterminal_ref_pu);
        if(K==0)
        {
            dedc_block.set_input(input);
            dedc_block.run(mode);
            input = dedc_block.get_output();
        }
        else
        {
            phase_tuner_1.set_input(input);
            phase_tuner_1.run(mode);
            input = phase_tuner_1.get_output();
        }

        phase_tuner_2.set_input(input);
        phase_tuner_2.run(mode);
        input = phase_tuner_2.get_output();

        phase_tuner_3.set_input(input);
        phase_tuner_3.run(mode);

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double PSASPS1::get_stabilizing_signal_in_pu()
{
    double output = phase_tuner_3.get_output();
    if(output>Vsmax)
        output=Vsmax;
    if(output<Vsmin)
        output=Vsmin;

    return output;
}

void PSASPS1::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    double vsmax = get_Vsmax();
    double vsmin = get_Vsmin();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_device_name()<<".\n";
    bool error_found = false;
    if(vsmax<=vsmin)
    {
        osstream<<"Vsmax<=Vsmin was detected: Vsmax="<<vsmax<<", Vsmin="<<vsmin<<"\n";
        error_found = true;
    }
    if(get_Tq_in_s()==0.0)
    {
        osstream<<"Tq=0.0 was detected\n";
        error_found = true;
    }
    if(get_T2e_in_s()==0.0)
    {
        osstream<<"T2e=0.0 was detected\n";
        error_found = true;
    }
    if(get_T4e_in_s()==0.0)
    {
        osstream<<"T4e=0.0 was detected\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS1::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS1::get_standard_psse_string() const
{
    ostringstream osstream;

    double Kq1 = get_Kq1();
    double Kq2 = get_Kq2();
    double Kq3 = get_Kq3();
    unsigned int K = get_K();
    double Tq = get_Tq_in_s();
    double T1e = get_T1e_in_s();
    double T2e = get_T2e_in_s();
    double T3e = get_T3e_in_s();
    double T4e = get_T4e_in_s();
    double Vsmax = get_Vsmax();
    double Vsmin = get_Vsmin();

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<Kq1<<", "
            <<setw(8)<<setprecision(6)<<Kq2<<", "
            <<setw(8)<<setprecision(6)<<Kq3<<", "
            <<setw(8)<<K<<", "
            <<setw(8)<<setprecision(6)<<Tq<<", "
            <<setw(8)<<setprecision(6)<<T1e<<", "
            <<setw(8)<<setprecision(6)<<T2e<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<T3e<<", "
            <<setw(8)<<setprecision(6)<<T4e<<", "
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KQ1", i); i++;
    add_model_data_name_and_index_pair("KQ2", i); i++;
    add_model_data_name_and_index_pair("KQ3", i); i++;
    add_model_data_name_and_index_pair("K", i); i++;
    add_model_data_name_and_index_pair("TQ", i); i++;
    add_model_data_name_and_index_pair("T1E", i); i++;
    add_model_data_name_and_index_pair("T2E", i); i++;
    add_model_data_name_and_index_pair("T3E", i); i++;
    add_model_data_name_and_index_pair("T4E", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="KQ1")
        return get_Kq1();
    if(par_name=="KQ2")
        return get_Kq2();
    if(par_name=="KQ3")
        return get_Kq3();
    if(par_name=="K")
        return get_K();
    if(par_name=="TQ")
        return get_Tq_in_s();
    if(par_name=="T1E")
        return get_T1e_in_s();
    if(par_name=="T2E")
        return get_T2e_in_s();
    if(par_name=="T3E")
        return get_T3e_in_s();
    if(par_name=="T4E")
        return get_T4e_in_s();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KQ1")
        return set_Kq1(value);
    if(par_name=="KQ2")
        return set_Kq2(value);
    if(par_name=="KQ3")
        return set_Kq3(value);
    if(par_name=="K")
        return set_K((unsigned int)(value));
    if(par_name=="TQ")
        return set_Tq_in_s(value);
    if(par_name=="T1E")
        return set_T1e_in_s(value);
    if(par_name=="T2E")
        return set_T2e_in_s(value);
    if(par_name=="T3E")
        return set_T3e_in_s(value);
    if(par_name=="T4E")
        return set_T4e_in_s(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

double PSASPS1::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tq_in_s()!=0.0 and mint>get_Tq_in_s())
        mint = get_Tq_in_s();
    if(get_T2e_in_s()!=0.0 and mint>get_T2e_in_s())
        mint = get_T2e_in_s();
    if(get_T4e_in_s()!=0.0 and mint>get_T4e_in_s())
        mint = get_T4e_in_s();
    return mint;
}

void PSASPS1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DEDC BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@DEDC BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
}

double PSASPS1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "SIGNAL@SLOT 2")
        return get_signal_value_of_slot(1);

    if(var_name == "SIGNAL@SLOT 3")
        return get_signal_value_of_slot(2);

    if(var_name == "STATE@DEDC BLOCK")
        return dedc_block.get_state();

    if(var_name == "STATE@PHASE TUNER 1")
        return phase_tuner_1.get_state();

    if(var_name == "STATE@PHASE TUNER 2")
        return phase_tuner_2.get_state();

    if(var_name == "STATE@PHASE TUNER 3")
        return phase_tuner_3.get_state();

    if(var_name == "OUTPUT@DEDC BLOCK")
        return dedc_block.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 1")
        return phase_tuner_1.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 2")
        return phase_tuner_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 3")
        return phase_tuner_3.get_output();

    return 0.0;
}



string PSASPS1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
