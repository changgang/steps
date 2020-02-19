#include "header/model/sg_models/stabilizer_model/PSASPS8.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS8::PSASPS8(STEPS& toolkit) : STABILIZER_MODEL(toolkit),
                                   sensor(toolkit),
                                   phase_tuner_1(toolkit),
                                   phase_tuner_2(toolkit),
                                   phase_tuner_3(toolkit)
{
    clear();
}

PSASPS8::~PSASPS8()
{
}

void PSASPS8::clear()
{
}

void PSASPS8::copy_from_const_model(const PSASPS8& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    sensor.set_toolkit(toolkit);
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

    this->set_Kqv(model.get_Kqv());
    this->set_Tqv_in_s(model.get_Tqv_in_s());
    this->set_Tq1_in_s(model.get_Tq1_in_s());
    this->set_Tq1p_in_s(model.get_Tq1p_in_s());
    this->set_Tq2_in_s(model.get_Tq2_in_s());
    this->set_Tq2p_in_s(model.get_Tq2p_in_s());
    this->set_Tq3_in_s(model.get_Tq3_in_s());
    this->set_Tq3p_in_s(model.get_Tq3p_in_s());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
}
PSASPS8::PSASPS8(const PSASPS8& model) : STABILIZER_MODEL(model.get_toolkit()),
                                         sensor(model.get_toolkit()),
                                         phase_tuner_1(model.get_toolkit()),
                                         phase_tuner_2(model.get_toolkit()),
                                         phase_tuner_3(model.get_toolkit())
{
    copy_from_const_model(model);
}

PSASPS8& PSASPS8::operator=(const PSASPS8& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS8::get_model_name() const
{
    return "PSASPS8";
}

void PSASPS8::set_Kqv(double K)
{
    sensor.set_K(K);
}

void PSASPS8::set_Tqv_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPS8::set_Tq1_in_s(double T)
{
    phase_tuner_1.set_T2_in_s(T);
}

void PSASPS8::set_Tq1p_in_s(double T)
{
    phase_tuner_1.set_T1_in_s(T);
}

void PSASPS8::set_Tq2_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}

void PSASPS8::set_Tq2p_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}

void PSASPS8::set_Tq3_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}

void PSASPS8::set_Tq3p_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}

void PSASPS8::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void PSASPS8::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS8::get_Kqv() const
{
    return sensor.get_K();
}

double PSASPS8::get_Tqv_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPS8::get_Tq1_in_s() const
{
    return phase_tuner_1.get_T2_in_s();
}

double PSASPS8::get_Tq1p_in_s() const
{
    return phase_tuner_1.get_T1_in_s();
}

double PSASPS8::get_Tq2_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}

double PSASPS8::get_Tq2p_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}

double PSASPS8::get_Tq3_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double PSASPS8::get_Tq3p_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}

double PSASPS8::get_Vsmax() const
{
    return Vsmax;
}

double PSASPS8::get_Vsmin() const
{
    return Vsmin;
}

bool PSASPS8::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=13)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kqv, tqv, tq1, tq1p, tq2, tq2p, tq3, tq3p, vsmax, vsmin;

            unsigned int i=3;
            kqv = get_double_data(data[i],"0.0"); i++;
            tqv= get_double_data(data[i],"0.0"); i++;
            tq1= get_double_data(data[i],"0.0"); i++;
            tq1p= get_double_data(data[i],"0.0"); i++;
            tq2= get_double_data(data[i],"0.0"); i++;
            tq2p= get_double_data(data[i],"0.0"); i++;
            tq3= get_double_data(data[i],"0.0"); i++;
            tq3p= get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            set_Kqv(kqv);
            set_Tqv_in_s(tqv);
            set_Tq1_in_s(tq1);
            set_Tq1p_in_s(tq1p);
            set_Tq2_in_s(tq2);
            set_Tq2p_in_s(tq2p);
            set_Tq3_in_s(tq3);
            set_Tq3p_in_s(tq3p);
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

bool PSASPS8::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS8::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS8::setup_block_toolkit_and_parameters()
{
}

void PSASPS8::initialize()
{
    ostringstream osstream;

    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
        {
            if(not exciter->is_model_initialized())
                exciter->initialize();

            setup_block_toolkit_and_parameters();

            unsigned int bus = generator->get_generator_bus();

            DEVICE_ID bus_device;
            bus_device.set_device_type("BUS");
            TERMINAL terminal;
            terminal.append_bus(bus);
            bus_device.set_device_terminal(terminal);

            SIGNAL signal = prepare_signal_with_signal_type_and_device_id(5, bus_device);
            if(signal.is_valid())
                set_input_signal_at_slot(0, signal);

            phase_tuner_3.set_output(0.0);
            phase_tuner_3.initialize();

            phase_tuner_2.set_output(0.0);
            phase_tuner_2.initialize();

            phase_tuner_1.set_output(0.0);
            phase_tuner_1.initialize();

            sensor.set_output(0.0);
            sensor.initialize();

            V_ref_pu = get_signal_value_of_slot(0);
        }
        else
            deactivate_model();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS8::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        double V = get_signal_value_of_slot(0);

        double input = V_ref_pu - V;
        sensor.set_input(input);
        sensor.run(mode);

        input = sensor.get_output();

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

double PSASPS8::get_stabilizing_signal_in_pu()
{
    double output = phase_tuner_3.get_output();
    if(output>Vsmax)
        output=Vsmax;
    if(output<Vsmin)
        output=Vsmin;

    return output;
}

void PSASPS8::check()
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
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS8::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS8::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS8::get_standard_psse_string() const
{
    ostringstream osstream;

    double Kqv = get_Kqv();
    double Tqv = get_Tqv_in_s();
    double Tq1 = get_Tq1_in_s();
    double Tq1p = get_Tq1p_in_s();
    double Tq2 = get_Tq2_in_s();
    double Tq2p = get_Tq2p_in_s();
    double Tq3 = get_Tq3_in_s();
    double Tq3p = get_Tq3p_in_s();
    double Vsmax = get_Vsmax();
    double Vsmin = get_Vsmin();

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<Kqv<<", "
            <<setw(8)<<setprecision(6)<<Tqv<<", "
            <<setw(8)<<setprecision(6)<<Tq1<<", "
            <<setw(8)<<setprecision(6)<<Tq1p<<", "
            <<setw(8)<<setprecision(6)<<Tq2<<", "
            <<setw(8)<<setprecision(6)<<Tq2p<<", "
            <<setw(8)<<setprecision(6)<<Tq3<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Tq3p<<", "
            <<setw(8)<<setprecision(6)<<Vsmax<<", "
            <<setw(8)<<setprecision(6)<<Vsmin<<" /";

    return osstream.str();
}

void PSASPS8::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KQV", i); i++;
    add_model_data_name_and_index_pair("TQV", i); i++;
    add_model_data_name_and_index_pair("TQ1", i); i++;
    add_model_data_name_and_index_pair("TQ1P", i); i++;
    add_model_data_name_and_index_pair("TQ2", i); i++;
    add_model_data_name_and_index_pair("TQ2P", i); i++;
    add_model_data_name_and_index_pair("TQ3", i); i++;
    add_model_data_name_and_index_pair("TQ3P", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS8::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="KQV")
        return get_Kqv();
    if(par_name=="TQV")
        return get_Tqv_in_s();
    if(par_name=="TQ1")
        return get_Tq1_in_s();
    if(par_name=="TQ1P")
        return get_Tq1p_in_s();
    if(par_name=="TQ2")
        return get_Tq2_in_s();
    if(par_name=="TQ2P")
        return get_Tq2p_in_s();
    if(par_name=="TQ3")
        return get_Tq3_in_s();
    if(par_name=="TQ3P")
        return get_Tq3p_in_s();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS8::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KQV")
        return set_Kqv(value);
    if(par_name=="TQV")
        return set_Tqv_in_s(value);
    if(par_name=="TQ1")
        return set_Tq1_in_s(value);
    if(par_name=="TQ1P")
        return set_Tq1p_in_s(value);
    if(par_name=="TQ2")
        return set_Tq2_in_s(value);
    if(par_name=="TQ2P")
        return set_Tq2p_in_s(value);
    if(par_name=="TQ3")
        return set_Tq3_in_s(value);
    if(par_name=="TQ3P")
        return set_Tq3p_in_s(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

double PSASPS8::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tqv_in_s()!=0.0 and mint>get_Tqv_in_s())
        mint = get_Tqv_in_s();
    if(get_Tq1_in_s()!=0.0 and mint>get_Tq1_in_s())
        mint = get_Tq1_in_s();
    if(get_Tq2_in_s()!=0.0 and mint>get_Tq2_in_s())
        mint = get_Tq2_in_s();
    if(get_Tq3_in_s()!=0.0 and mint>get_Tq3_in_s())
        mint = get_Tq3_in_s();
    return mint;
}

void PSASPS8::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@VOLTAGE SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@VOLTAGE SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("OUTPUT@PHASE TUNER 3", i); i++;
}

double PSASPS8::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "STATE@VOLTAGE SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@PHASE TUNER 1")
        return phase_tuner_1.get_state();

    if(var_name == "STATE@PHASE TUNER 2")
        return phase_tuner_2.get_state();

    if(var_name == "STATE@PHASE TUNER 3")
        return phase_tuner_3.get_state();

    if(var_name == "OUTPUT@VOLTAGE SENSOR")
        return sensor.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 1")
        return phase_tuner_1.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 2")
        return phase_tuner_2.get_output();

    if(var_name == "OUTPUT@PHASE TUNER 3")
        return phase_tuner_3.get_output();

    return 0.0;
}

string PSASPS8::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS8::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS8::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
