#include "header/model/sg_models/stabilizer_model/PSASPS5.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
PSASPS5::PSASPS5()
{
    clear();
}

PSASPS5::~PSASPS5()
{
    //clear();
}

void PSASPS5::clear()
{
    set_model_float_parameter_count(13);
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void PSASPS5::copy_from_const_model(const PSASPS5& model)
{
    clear();

    this->set_T1_in_s(model.get_T1_in_s());
    this->set_T2_in_s(model.get_T2_in_s());
    this->set_T3_in_s(model.get_T3_in_s());
    this->set_T4_in_s(model.get_T4_in_s());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_K1(model.get_K1());
    this->set_T6_in_s(model.get_T6_in_s());
    this->set_K2(model.get_K2());
    this->set_a(model.get_a());
    this->set_p(model.get_p());
    this->set_K(model.get_K());
    this->set_Vsmax(model.get_Vsmax());
    this->set_Vsmin(model.get_Vsmin());
}

PSASPS5::PSASPS5(const PSASPS5& model) : STABILIZER_MODEL()
{
    copy_from_const_model(model);
}

PSASPS5& PSASPS5::operator=(const PSASPS5& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string PSASPS5::get_model_name() const
{
    return "PSASPS5";
}

void PSASPS5::set_T1_in_s(double T)
{
    sensor_1.set_T_in_s(T);
}

void PSASPS5::set_T2_in_s(double T)
{
    sensor_2.set_T_in_s(T);
}

void PSASPS5::set_T3_in_s(double T)
{
    dedc_block_1.set_K(T);
    dedc_block_1.set_T_in_s(T);
}

void PSASPS5::set_T4_in_s(double T)
{
    dedc_block_2.set_K(T);
    dedc_block_2.set_T_in_s(T);
}

void PSASPS5::set_T5_in_s(double T)
{
    dedc_block_3.set_K(T);
    dedc_block_3.set_T_in_s(T);
}

void PSASPS5::set_K1(double K)
{
    speed_amplifier.set_K(K);
}

void PSASPS5::set_T6_in_s(double T)
{
    speed_amplifier.set_T_in_s(T);
}

void PSASPS5::set_K2(double K)
{
    K2 = K;
}

void PSASPS5::set_a(double a)
{
    this->a = a;
}

void PSASPS5::set_p(double p)
{
    this->p = p;
}

void PSASPS5::set_K(double K)
{
    this->K = K;
}

void PSASPS5::set_Vsmax(double vmax)
{
    Vsmax = vmax;
}

void PSASPS5::set_Vsmin(double vmin)
{
    Vsmin = vmin;
}

double PSASPS5::get_T1_in_s() const
{
    return sensor_1.get_T_in_s();
}

double PSASPS5::get_T2_in_s() const
{
    return sensor_2.get_T_in_s();
}

double PSASPS5::get_T3_in_s() const
{
    return dedc_block_1.get_T_in_s();
}

double PSASPS5::get_T4_in_s() const
{
    return dedc_block_2.get_T_in_s();
}

double PSASPS5::get_T5_in_s() const
{
    return dedc_block_3.get_T_in_s();
}

double PSASPS5::get_K1() const
{
    return speed_amplifier.get_K();
}

double PSASPS5::get_T6_in_s() const
{
    return speed_amplifier.get_T_in_s();
}

double PSASPS5::get_K2() const
{
    return K2;
}

double PSASPS5::get_a() const
{
    return a;
}

double PSASPS5::get_p() const
{
    return p;
}

double PSASPS5::get_K() const
{
    return K;
}

double PSASPS5::get_Vsmax() const
{
    return Vsmax;
}
double PSASPS5::get_Vsmin() const
{
    return Vsmin;
}

bool PSASPS5::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=16)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double t1, t2, t3, t4, t5, k1, t6, k2, a, p, k,vsmax, vsmin;

            size_t i=3;
            t1 = get_double_data(data[i],"0.0"); i++;
            t2 = get_double_data(data[i],"0.0"); i++;
            t3 = get_double_data(data[i],"0.0"); i++;
            t4 = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            k1 = get_double_data(data[i],"0.0"); i++;
            t6 = get_double_data(data[i],"0.0"); i++;
            k2 = get_double_data(data[i],"0.0"); i++;
            a = get_double_data(data[i],"0.0"); i++;
            p = get_double_data(data[i],"0.0"); i++;
            k = get_double_data(data[i],"0.0"); i++;
            vsmax = get_double_data(data[i],"0.0"); i++;
            vsmin = get_double_data(data[i],"0.0"); i++;

            set_T1_in_s(t1);
            set_T2_in_s(t2);
            set_T3_in_s(t3);
            set_T4_in_s(t4);
            set_T5_in_s(t5);
            set_K1(k1);
            set_T6_in_s(t6);
            set_K2(k2);
            set_a(a);
            set_p(p);
            set_K(k);
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

bool PSASPS5::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPS5::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPS5::setup_block_toolkit_and_parameters()
{
    ;
}

void PSASPS5::initialize()
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
            return;

        }
        else
            deactivate_model();
    }
    else
        deactivate_model();

    set_flag_model_initialized_as_true();
}

void PSASPS5::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        return;

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double PSASPS5::get_stabilizing_signal_in_pu() const
{
    return 0.0;
}

void PSASPS5::check()
{
    ;
}

void PSASPS5::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPS5::save()
{
    string model = get_standard_psse_string();
    return;
}

string PSASPS5::get_standard_psse_string() const
{
    ostringstream osstream;

    double T1 = get_T1_in_s();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double T4 = get_T4_in_s();
    double T5 = get_T5_in_s();
    double K1 = get_K1();
    double T6 = get_T6_in_s();
    double K2 = get_K2();
    double a = get_a();
    double p = get_p();
    double K = get_K();
    double vsmax = get_Vsmax();
    double vsmin = get_Vsmin();;

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];

    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<T1<<", "
            <<setw(8)<<setprecision(6)<<T2<<", "
            <<setw(8)<<setprecision(6)<<T3<<", "
            <<setw(8)<<setprecision(6)<<T4<<", "
            <<setw(8)<<setprecision(6)<<T5<<", "
            <<setw(8)<<setprecision(6)<<K1<<", "
            <<setw(8)<<setprecision(6)<<T6<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<K2<<", "
            <<setw(8)<<setprecision(6)<<a<<", "
            <<setw(8)<<setprecision(6)<<p<<", "
            <<setw(8)<<setprecision(6)<<K<<", "
            <<setw(8)<<setprecision(6)<<vsmax<<", "
            <<setw(8)<<setprecision(6)<<vsmin<<" /";

    return osstream.str();
}

void PSASPS5::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("T1", i); i++;
    add_model_data_name_and_index_pair("T2", i); i++;
    add_model_data_name_and_index_pair("T3", i); i++;
    add_model_data_name_and_index_pair("T4", i); i++;
    add_model_data_name_and_index_pair("T5", i); i++;
    add_model_data_name_and_index_pair("K1", i); i++;
    add_model_data_name_and_index_pair("T6", i); i++;
    add_model_data_name_and_index_pair("K2", i); i++;
    add_model_data_name_and_index_pair("A", i); i++;
    add_model_data_name_and_index_pair("P", i); i++;
    add_model_data_name_and_index_pair("K", i); i++;
    add_model_data_name_and_index_pair("VSMAX", i); i++;
    add_model_data_name_and_index_pair("VSMIN", i); i++;
}

double PSASPS5::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

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
    if(par_name=="K1")
        return get_K1();
    if(par_name=="T6")
        return get_T6_in_s();
    if(par_name=="K2")
        return get_K2();
    if(par_name=="A")
        return get_a();
    if(par_name=="P")
        return get_p();
    if(par_name=="K")
        return get_K();
    if(par_name=="VSMAX")
        return get_Vsmax();
    if(par_name=="VSMIN")
        return get_Vsmin();

    return 0.0;
}

void PSASPS5::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

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
    if(par_name=="K1")
        return set_K1(value);
    if(par_name=="T6")
        return set_T6_in_s(value);
    if(par_name=="K2")
        return set_K2(value);
    if(par_name=="A")
        return set_a(value);
    if(par_name=="P")
        return set_p(value);
    if(par_name=="K")
        return set_K(value);
    if(par_name=="VSMAX")
        return set_Vsmax(value);
    if(par_name=="VSMIN")
        return set_Vsmin(value);

    return;
}

void PSASPS5::prepare_model_internal_variable_table()
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

double PSASPS5::get_model_internal_variable_with_name(string var_name)
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
    /*
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
    */
    return 0.0;
}



string PSASPS5::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPS5::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPS5::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
