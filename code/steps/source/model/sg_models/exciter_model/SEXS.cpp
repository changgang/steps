#include "header/model/sg_models/exciter_model/SEXS.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
SEXS::SEXS(STEPS& toolkit) : EXCITER_MODEL(toolkit),
                             phase_tuner(toolkit),
                             exciter(toolkit)
{
    clear();
}

SEXS::~SEXS()
{
    ;
}

void SEXS::clear()
{
    set_model_float_parameter_count(6);

    exciter.set_limiter_type(WINDUP_LIMITER);
}

void SEXS::copy_from_const_model(const SEXS& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    phase_tuner.set_toolkit(toolkit);
    exciter.set_toolkit(toolkit);

    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->set_TA_in_s(model.get_TA_in_s());
    this->set_TB_in_s(model.get_TB_in_s());
    this->set_K(model.get_K());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());

}
SEXS::SEXS(const SEXS& model) : EXCITER_MODEL(model.get_toolkit()),
                                phase_tuner(model.get_toolkit()),
                                exciter(model.get_toolkit())
{
    copy_from_const_model(model);
}

SEXS& SEXS::operator=(const SEXS& model)
{
    if(this == &model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string SEXS::get_model_name() const
{
    return "SEXS";
}

void SEXS::set_TA_in_s(double T)
{
    phase_tuner.set_T1_in_s(T);
}

void SEXS::set_TB_in_s(double T)
{
    phase_tuner.set_T2_in_s(T);
}

void SEXS::set_K(double K)
{
    exciter.set_K(K);
}

void SEXS::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}

void SEXS::set_Efdmax_in_pu(double efd)
{
    exciter.set_upper_limit(efd);
}

void SEXS::set_Efdmin_in_pu(double efd)
{
    exciter.set_lower_limit(efd);
}

double SEXS::get_TA_in_s() const
{
    return phase_tuner.get_T1_in_s();
}

double SEXS::get_TB_in_s() const
{
    return phase_tuner.get_T2_in_s();
}

double SEXS::get_K() const
{
    return exciter.get_K();
}

double SEXS::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double SEXS::get_Efdmax_in_pu() const
{
    return exciter.get_upper_limit();
}

double SEXS::get_Efdmin_in_pu() const
{
    return exciter.get_lower_limit();
}

bool SEXS::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=9)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double ta_over_tb, ta, tb, k, te, emax, emin;

            unsigned int i=3;
            ta_over_tb = get_double_data(data[i],"0.0"); i++;
            tb = get_double_data(data[i],"0.0"); i++;
            k = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            emin = get_double_data(data[i],"0.0"); i++;
            emax = get_double_data(data[i],"0.0"); i++;
            ta = ta_over_tb*tb;

            set_TA_in_s(ta);
            set_TB_in_s(tb);
            set_K(k);
            set_TE_in_s(te);
            set_Efdmin_in_pu(emin);
            set_Efdmax_in_pu(emax);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool SEXS::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool SEXS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void SEXS::setup_block_toolkit_and_parameters()
{
}

void SEXS::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            STEPS& toolkit = get_toolkit();

            double Ecomp = get_compensated_voltage_in_pu();

            double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();

            if(Efd>get_Efdmax_in_pu())
            {
                osstream<<"Initialization error. Efd of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                  <<"Efd is "<<Efd<<", and Efdmax is "<<get_Efdmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Efd<get_Efdmin_in_pu())
            {
                osstream<<"Initialization error. Efd of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                  <<"Efd is "<<Efd<<", and Efdmin is "<<get_Efdmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            exciter.set_output(Efd);
            exciter.initialize();

            double input = exciter.get_input();
            phase_tuner.set_output(input);
            phase_tuner.initialize();

            input = phase_tuner.get_input();

            double Vref = input+Ecomp;
            set_voltage_reference_in_pu(Vref);

            set_flag_model_initialized_as_true();
        }
    }
}

void SEXS::run(DYNAMIC_MODE mode)
{
    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    double input = Vref-Ecomp+Vs;

    phase_tuner.set_input(input);
    phase_tuner.run(mode);

    input = phase_tuner.get_output();
    exciter.set_input(input);
    exciter.run(mode);

    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double SEXS::get_excitation_voltage_in_pu()
{
    return exciter.get_output();
}
void SEXS::check()
{
    ;
}

void SEXS::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void SEXS::save()
{
    ;
}

string SEXS::get_standard_psse_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    double TA = get_TA_in_s();
    double TB = get_TB_in_s();
    double K = get_K();
    double TE = get_TE_in_s();
    double Efdmin = get_Efdmin_in_pu();
    double Efdmax = get_Efdmax_in_pu();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<TA/TB<<", "
            <<setw(8)<<setprecision(6)<<TB<<", "
            <<setw(8)<<setprecision(6)<<K<<", "
            <<setw(8)<<setprecision(6)<<TE<<", "
            <<setw(8)<<setprecision(6)<<Efdmin<<", "
            <<setw(8)<<setprecision(6)<<Efdmax<<" /";
    return osstream.str();
}

void SEXS::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=1;
    add_model_data_name_and_index_pair("TA", i); i++;
    add_model_data_name_and_index_pair("TB", i); i++;
    add_model_data_name_and_index_pair("K", i); i++;
    add_model_data_name_and_index_pair("TE", i); i++;
    add_model_data_name_and_index_pair("EFDMAX", i); i++;
    add_model_data_name_and_index_pair("EFDMIN", i);
}

double SEXS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="TA") return get_TA_in_s();
        if(par_name=="TB") return get_TB_in_s();
        if(par_name=="K") return get_K();
        if(par_name=="TE") return get_TE_in_s();
        if(par_name=="EFDMAX") return get_Efdmax_in_pu();
        if(par_name=="EFDMIN") return get_Efdmin_in_pu();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void SEXS::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="TA") return set_TA_in_s(value);
        if(par_name=="TB") return set_TB_in_s(value);
        if(par_name=="K") return set_K(value);
        if(par_name=="TE") return set_TE_in_s(value);
        if(par_name=="EFDMAX") return set_Efdmax_in_pu(value);
        if(par_name=="EFDMIN") return set_Efdmin_in_pu(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double SEXS::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_TB_in_s()!=0.0 and mint>get_TB_in_s())
        mint = get_TB_in_s();
    if(get_TE_in_s()!=0.0 and mint>get_TE_in_s())
        mint = get_TE_in_s();
    return mint;
}

void SEXS::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@TUNER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@EXCITER", i); i++;
}

double SEXS::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(is_model_internal_variable_exist(var_name))
    {
        if(var_name == "STATE@TUNER") return phase_tuner.get_state();
        if(var_name == "STATE@EXCITER") return exciter.get_state();
    }
    return 0.0;
}


string SEXS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string SEXS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string SEXS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void SEXS::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-GEN", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-PSS", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-COMP", matrix);
    // do linearization
}
