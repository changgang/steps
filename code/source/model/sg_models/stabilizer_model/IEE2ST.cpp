#include "header/model/sg_models/stabilizer_model/IEE2ST.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
IEE2ST::IEE2ST()
{
    clear();
}

IEE2ST::~IEE2ST()
{
    clear();
}

void IEE2ST::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void IEE2ST::copy_from_const_model(const IEE2ST& model)
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

    this->set_K1(model.get_K1());
    this->set_T1_in_s(model.get_T1_in_s());
    this->set_K2(model.get_K2());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_T7_in_s(model.get_T7_in_s());
    this->set_T8_in_s(model.get_T8_in_s());
    this->set_T9_in_s(model.get_T9_in_s());
    this->set_T10_in_s(model.get_T10_in_s());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
    this->set_Vcmax(model.get_Vcmax());
    this->set_Vcmin(model.get_Vcmin());

}
IEE2ST::IEE2ST(const IEE2ST& model) : STABILIZER_MODEL()
{
    copy_from_const_model(model);
}

IEE2ST& IEE2ST::operator=(const IEE2ST& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string IEE2ST::get_model_name() const
{
    return "IEE2ST";
}

void IEE2ST::set_K1(double K)
{
    sensor_1.set_K(K);
}

void IEE2ST::set_T1_in_s(double T)
{
    sensor_1.set_T_in_s(T);
}

void IEE2ST::set_K2(double K)
{
    sensor_2.set_K(K);
}

void IEE2ST::set_T2_in_s(double T)
{
    sensor_2.set_T_in_s(T);
}

void IEE2ST::set_T3_in_s(double T)
{
    filter.set_K(T);
    if(T<=0.0)
        alternative_filter.set_K(1.0);
}

void IEE2ST::set_T4_in_s(double T)
{
    filter.set_T_in_s(T);
    alternative_filter.set_T_in_s(T);
}

void IEE2ST::set_T5_in_s(double T)
{
    phase_tuner_1.set_T1_in_s(T);
}

void IEE2ST::set_T6_in_s(double T)
{
    phase_tuner_1.set_T2_in_s(T);
}

void IEE2ST::set_T7_in_s(double T)
{
    phase_tuner_2.set_T1_in_s(T);
}
void IEE2ST::set_T8_in_s(double T)
{
    phase_tuner_2.set_T2_in_s(T);
}
void IEE2ST::set_T9_in_s(double T)
{
    phase_tuner_3.set_T1_in_s(T);
}
void IEE2ST::set_T10_in_s(double T)
{
    phase_tuner_3.set_T2_in_s(T);
}
void IEE2ST::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}
void IEE2ST::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

void IEE2ST::set_Vcmax(double vmax)
{
    Vcmax = vmax;
}

void IEE2ST::set_Vcmin(double vmin)
{
    Vcmin = vmin;
}


double IEE2ST::get_K1() const
{
    return sensor_1.get_K();
}

double IEE2ST::get_T1_in_s() const
{
    return sensor_1.get_T_in_s();
}

double IEE2ST::get_K2() const
{
    return sensor_2.get_K();
}

double IEE2ST::get_T2_in_s() const
{
    return sensor_2.get_T_in_s();
}

double IEE2ST::get_T3_in_s() const
{
    return filter.get_K();
}

double IEE2ST::get_T4_in_s() const
{
    return filter.get_T_in_s();
}
double IEE2ST::get_T5_in_s() const
{
    return phase_tuner_1.get_T1_in_s();
}
double IEE2ST::get_T6_in_s() const
{
    return phase_tuner_1.get_T2_in_s();
}
double IEE2ST::get_T7_in_s() const
{
    return phase_tuner_2.get_T1_in_s();
}
double IEE2ST::get_T8_in_s() const
{
    return phase_tuner_2.get_T2_in_s();
}
double IEE2ST::get_T9_in_s() const
{
    return phase_tuner_3.get_T1_in_s();
}
double IEE2ST::get_T10_in_s() const
{
    return phase_tuner_3.get_T2_in_s();
}

double IEE2ST::get_Vsmax() const
{
    return Vsmax;
}
double IEE2ST::get_Vsmin() const
{
    return Vsmin;
}
double IEE2ST::get_Vcmax() const
{
    return Vcmax;
}
double IEE2ST::get_Vcmin() const
{
    return Vcmin;
}


bool IEE2ST::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<23)
        is_successful = false;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;

    size_t signal_type[2], bus[2];
    double k1, t1, k2, t2, t3, t4, t5, t6, t7, t8, t9, t10,
           vsmax, vsmin, vcl, vcu;

    size_t i=3;
    signal_type[0] = get_integer_data(data[i],"0"); i++;
    bus[0] = get_integer_data(data[i],"0"); i++;
    signal_type[1] = get_integer_data(data[i],"0"); i++;
    bus[1] = get_integer_data(data[i],"0"); i++;

    for(size_t i=0; i<2; ++i)
    {
        SIGNAL signal = prepare_signal_with_signal_type_and_bus(signal_type[i], bus[i]);
        if(signal.is_valid())
            set_input_signal_at_slot(i, signal);
    }

    k1 = get_double_data(data[i],"0.0"); i++;
    k2 = get_double_data(data[i],"0.0"); i++;
    t1 = get_double_data(data[i],"0.0"); i++;
    t2 = get_double_data(data[i],"0.0"); i++;
    t3 = get_double_data(data[i],"0.0"); i++;
    t4 = get_double_data(data[i],"0.0"); i++;
    t5 = get_double_data(data[i],"0.0"); i++;
    t6 = get_double_data(data[i],"0.0"); i++;
    t7 = get_double_data(data[i],"0.0"); i++;
    t8 = get_double_data(data[i],"0.0"); i++;
    t9 = get_double_data(data[i],"0.0"); i++;
    t10 = get_double_data(data[i],"0.0"); i++;
    vsmax = get_double_data(data[i],"0.0"); i++;
    vsmin = get_double_data(data[i],"0.0"); i++;
    vcu = get_double_data(data[i],"0.0"); i++;
    vcl = get_double_data(data[i],"0.0"); i++;

    set_K1(k1);
    set_K2(k2);
    set_T1_in_s(t1);
    set_T2_in_s(t2);
    set_T3_in_s(t3);
    set_T4_in_s(t4);
    set_T5_in_s(t5);
    set_T6_in_s(t6);
    set_T7_in_s(t7);
    set_T8_in_s(t8);
    set_T9_in_s(t9);
    set_T10_in_s(t10);
    set_Vsmax(vsmax);
    set_Vsmin(vsmin);
    set_Vcmax(vcu);
    set_Vcmin(vcl);

    is_successful = true;

    return is_successful;
}

bool IEE2ST::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEE2ST::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEE2ST::set_block_toolkit()
{
    STEPS& toolkit = get_toolkit();
    sensor_1.set_toolkit(toolkit);
    sensor_2.set_toolkit(toolkit);
    filter.set_toolkit(toolkit);
    alternative_filter.set_toolkit(toolkit);
    phase_tuner_1.set_toolkit(toolkit);
    phase_tuner_2.set_toolkit(toolkit);
    phase_tuner_3.set_toolkit(toolkit);
}

void IEE2ST::initialize()
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

    set_block_toolkit();

    phase_tuner_3.set_output(0.0);
    phase_tuner_3.initialize();

    phase_tuner_2.set_output(0.0);
    phase_tuner_2.initialize();

    phase_tuner_1.set_output(0.0);
    phase_tuner_1.initialize();

    double value_slot_1 = get_signal_value_of_slot(0);
    double value_slot_2 = get_signal_value_of_slot(1);
    double value_sensor_1 = value_slot_1*get_K1();
    double value_sensor_2 = value_slot_2*get_K2();

    sensor_1.set_output(value_sensor_1);
    sensor_1.initialize();

    sensor_2.set_output(value_sensor_2);
    sensor_2.initialize();

    double value_sum = value_sensor_1 + value_sensor_2;

    double T3 = get_T3_in_s();
    if(T3>0.0)
    {
        filter.set_input(value_sum);
        filter.initialize();
    }
    else
    {
        osstream<<"Initialization failed. T3<=0.0 in model "<<get_model_name()<<" of "
          <<get_generator_pointer()->get_device_name()<<" requires input signals with 0 steady state value."<<endl
          <<"However, "<<value_sum<<" is given. Check input signals in slots.";
        alternative_filter.set_output(0.0);
        alternative_filter.initialize();
    }
    set_flag_model_initialized_as_true();
}

void IEE2ST::run(DYNAMIC_MODE mode)
{
    if(not is_model_active())
        return;

    double value_slot_1 = get_signal_value_of_slot(0);
    sensor_1.set_input(value_slot_1);
    sensor_1.run(mode);
    double output1 = sensor_1.get_output();

    double value_slot_2 = get_signal_value_of_slot(1);
    sensor_2.set_input(value_slot_2);
    sensor_2.run(mode);
    double output2 = sensor_2.get_output();

    double input = output1+output2;
    double T3 = get_T3_in_s();
    if(T3>0.0)
    {
        filter.set_input(input);
        filter.run(mode);
        input = filter.get_output();
    }
    else
    {
        alternative_filter.set_input(input);
        alternative_filter.run(mode);
        input = alternative_filter.get_output();
    }

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

double IEE2ST::get_stabilizing_signal_in_pu() const
{
    double output = phase_tuner_3.get_output();
    double vssmax = get_Vsmax();
    double vssmin = get_Vsmin();
    if(output>vssmax)
        output=vssmax;
    if(output<vssmin)
        output=vssmin;

    double vcmax = get_Vcmax();
    double vcmin = get_Vcmin();

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = get_device_id().get_device_terminal()[0];
    double terminal_voltage = psdb.get_bus_voltage_in_pu(bus);
    if(vcmax!=0.0 and terminal_voltage>vcmax)
        output=0.0;
    if(vcmin!=0.0 and terminal_voltage<vcmin)
        output=0.0;
    return output;
}

void IEE2ST::check()
{
    ;
}

void IEE2ST::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void IEE2ST::save()
{
    string model = get_standard_model_string();
    return;
}

string IEE2ST::get_standard_model_string() const
{
    ostringstream osstream;

    double K1 = get_K1();
    double T1 = get_T1_in_s();
    double K2 = get_K2();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();
    double T5 = get_T5_in_s();
    double T6 = get_T6_in_s();
    double T7 = get_T7_in_s();
    double T8 = get_T8_in_s();
    double T9 = get_T9_in_s();
    double T10 = get_T10_in_s();
    double Vsmax = get_Vsmax();
    double Vsmin = get_Vsmin();
    double Vcmax = get_Vcmax();
    double Vcmin = get_Vcmin();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    SIGNAL signal1 = get_input_signal_at_slot(0);
    SIGNAL signal2 = get_input_signal_at_slot(1);

    DEVICE_ID did1 = signal1.get_device_id();
    DEVICE_ID did2 = signal2.get_device_id();

    string signal1_type = signal1.get_meter_type();
    string signal2_type = signal2.get_meter_type();

    size_t signal1_type_number = convert_signal_type_string_to_number(signal1_type);
    size_t signal2_type_number = convert_signal_type_string_to_number(signal2_type);

    size_t signal1_bus = did1.get_device_terminal().get_buses()[0];
    size_t signal2_bus = did2.get_device_terminal().get_buses()[0];

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(4)<<signal1_type_number<<", "
      <<setw(8)<<signal1_bus<<", "
      <<setw(4)<<signal2_type_number<<", "
      <<setw(8)<<signal2_bus<<", "
      <<setw(8)<<setprecision(6)<<K1<<", "
      <<setw(8)<<setprecision(6)<<K2<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<T4<<", "
      <<setw(8)<<setprecision(6)<<T5<<", "
      <<setw(8)<<setprecision(6)<<T6<<", "
      <<setw(8)<<setprecision(6)<<T7<<", "
      <<setw(8)<<setprecision(6)<<T8<<", "
      <<setw(8)<<setprecision(6)<<T9<<", "
      <<setw(8)<<setprecision(6)<<T10<<", "
      <<setw(8)<<setprecision(6)<<Vsmax<<", "
      <<setw(8)<<setprecision(6)<<Vsmin<<", "
      <<setw(8)<<setprecision(6)<<Vcmax<<", "
      <<setw(8)<<setprecision(6)<<Vcmin<<" /";


    return osstream.str();
}

void IEE2ST::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double IEE2ST::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void IEE2ST::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void IEE2ST::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("SIGNAL@SLOT 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FILTER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PHASE TUNER 3", i); i++;
}

double IEE2ST::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "SIGNAL@SLOT 1")
        return get_signal_value_of_slot(0);

    if(var_name == "SIGNAL@SLOT 2")
        return get_signal_value_of_slot(1);

    if(var_name == "STATE@SENSOR 1")
        return sensor_1.get_state();

    if(var_name == "STATE@SENSOR 2")
        return sensor_2.get_state();

    if(var_name == "STATE@FILTER")
    {
        double T3 = get_T3_in_s();
        if(T3!=0.0)
            return filter.get_state();
        else
            return alternative_filter.get_state();
    }

    if(var_name == "STATE@PHASE TUNER 1")
        return phase_tuner_1.get_state();

    if(var_name == "STATE@PHASE TUNER 2")
        return phase_tuner_2.get_state();

    if(var_name == "STATE@PHASE TUNER 3")
        return phase_tuner_3.get_state();

    return 0.0;
}



string IEE2ST::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEE2ST::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEE2ST::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
