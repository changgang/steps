#include "header/model/sg_models/exciter_model/PSASPE1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
PSASPE1::PSASPE1(STEPS& toolkit) : EXCITER_MODEL(toolkit),
                                   sensor(toolkit),
                                   regulator(toolkit),
                                   feedbacker(toolkit),
                                   exciter(toolkit)
{
    clear();
}

PSASPE1::~PSASPE1()
{
    ;
}
void PSASPE1::clear()
{
    sensor.set_limiter_type(NO_LIMITER);
    regulator.set_limiter_type(NO_LIMITER);
    exciter.set_limiter_type(WINDUP_LIMITER);
    feedbacker.set_limiter_type(NO_LIMITER);
}
void PSASPE1::copy_from_const_model(const PSASPE1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    sensor.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    feedbacker.set_toolkit(toolkit);
    exciter.set_toolkit(toolkit);

    clear();
    this->set_KR(model.get_KR());
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    this->set_Efdmin_in_pu(model.get_Efdmin_in_pu());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
}

PSASPE1::PSASPE1(const PSASPE1& model) : EXCITER_MODEL(model.get_toolkit()),
                                         sensor(model.get_toolkit()),
                                         regulator(model.get_toolkit()),
                                         feedbacker(model.get_toolkit()),
                                         exciter(model.get_toolkit())
{
    copy_from_const_model(model);
}

PSASPE1& PSASPE1::operator=(const PSASPE1& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE1::get_model_name() const
{
    return "PSASPE1";
}

void PSASPE1::set_KR(double K)
{
    sensor.set_K(K);
}

void PSASPE1::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE1::set_KA(double K)
{
    regulator.set_K(K);
}

void PSASPE1::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE1::set_VAmax_in_pu(double vmax)
{
    this->VAmax = vmax;
}

void PSASPE1::set_VAmin_in_pu(double vmin)
{
    this->VAmin = vmin;
}

void PSASPE1::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}
void PSASPE1::set_Efdmax_in_pu(double emax)
{
    exciter.set_upper_limit(emax);
    set_VAmax_in_pu(emax);
    set_VAmin_in_pu(-emax);
}

void PSASPE1::set_Efdmin_in_pu(double emin)
{
    exciter.set_lower_limit(emin);
}

void PSASPE1::set_KF(double K)
{
    feedbacker.set_K(K);
}

void PSASPE1::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

double PSASPE1::get_KR() const
{
    return sensor.get_K();
}

double PSASPE1::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPE1::get_KA() const
{
    return regulator.get_K();
}

double PSASPE1::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE1::get_VAmax_in_pu() const
{
    return get_Efdmax_in_pu();
}

double PSASPE1::get_VAmin_in_pu() const
{
    return -get_Efdmax_in_pu();
}

double PSASPE1::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double PSASPE1::get_Efdmax_in_pu() const
{
    return exciter.get_upper_limit();
}

double PSASPE1::get_Efdmin_in_pu() const
{
    return exciter.get_lower_limit();
}

double PSASPE1::get_KF() const
{
    return feedbacker.get_K();
}

double PSASPE1::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

bool PSASPE1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=12)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kr, tr, ka, ta, te, kf, tf, efdmax, efdmin;

            unsigned int i=3;
            kr = get_double_data(data[i],"1.0"); i++;
            tr = get_double_data(data[i],"0.0"); i++;
            ka = get_double_data(data[i],"0.0"); i++;
            ta = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            kf = get_double_data(data[i],"0.0"); i++;
            tf = get_double_data(data[i],"0.0"); i++;
            efdmax = get_double_data(data[i],"0.0"); i++;
            efdmin = get_double_data(data[i],"0.0"); i++;

            set_KR(kr);
            set_TR_in_s(tr);
            set_KA(ka);
            set_TA_in_s(ta);
            set_TE_in_s(te);
            set_KF(kf);
            set_TF_in_s(tf);
            set_Efdmax_in_pu(efdmax);
            set_Efdmin_in_pu(efdmin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PSASPE1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPE1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPE1::setup_block_toolkit_and_parameters()
{
}

void PSASPE1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        STEPS& toolkit = get_toolkit();
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            double Ecomp = get_compensated_voltage_in_pu();

            set_voltage_reference_in_pu(Ecomp);

            sensor.set_output(0.0);
            sensor.initialize();

            double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
            Efd0 = Efd;

            if(Efd>get_Efdmax_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                        <<"Efd is "<<Efd<<", and Efdmax is "<<get_Efdmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Efd<get_Efdmin_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                        <<"Efd is "<<Efd<<", and Efdmin is "<<get_Efdmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(Efd>get_VAmax_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                        <<"Efd is "<<Efd<<", and VAmax is "<<get_VAmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Efd<get_VAmin_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                        <<"Efd is "<<Efd<<", and VAmin is "<<get_VAmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            exciter.set_output(Efd);
            exciter.initialize();

            feedbacker.set_input(Efd);
            feedbacker.initialize();

            regulator.set_output(0.0);
            regulator.initialize();

            set_flag_model_initialized_as_true();
        }
    }
}

void PSASPE1::run(DYNAMIC_MODE mode)
{
    double Ecomp = get_compensated_voltage_in_pu();
    double Vref = get_voltage_reference_in_pu();
    double Vs = get_stabilizing_signal_in_pu();

    double input = Vref-Ecomp;
    sensor.set_input(input);
    sensor.run(mode);

    double VAmax = get_VAmax_in_pu();
    double VAmin = get_VAmin_in_pu();
    for(unsigned int i=0; i<STEPS_MODEL_FEEDBACK_LOOP_INTEGRATION_COUNT; ++i)
    {
        input = sensor.get_output()+Vs-feedbacker.get_output();
        regulator.set_input(input);
        regulator.run(mode);

        input = Efd0+regulator.get_output();
        if(input>VAmax) input = VAmax;
        if(input<VAmin) input = VAmin;

        exciter.set_input(input);
        exciter.run(mode);

        double Efd = get_excitation_voltage_in_pu();

        feedbacker.set_input(Efd);
        feedbacker.run(mode);

        if(feedbacker.get_K()!=0.0)
            continue;
        else
            break;
    }
    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE1::get_excitation_voltage_in_pu()
{
    double Efd = exciter.get_output();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    if(Efd>Efdmax) Efd = Efdmax;
    if(Efd<Efdmin) Efd = Efdmin;

    return Efd;
}
void PSASPE1::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(get_TA_in_s()==0.0)
    {
        osstream<<"TA=0 was detected\n";
        error_found = true;
    }
    if(get_TE_in_s()==0.0)
    {
        osstream<<"TE=0 was detected\n";
        error_found = true;
    }
    if(get_TF_in_s()==0.0)
    {
        osstream<<"TF=0 was detected\n";
        error_found = true;
    }
    double vamax = get_VAmax_in_pu();
    double vamin = get_VAmin_in_pu();
    if(vamax<=vamin)
    {
        osstream<<"VAmax<=VAmin was detected: VAmax="<<vamax<<", VAmin="<<vamin<<"\n";
        error_found = true;
    }
    double efdmax = get_Efdmax_in_pu();
    double efdmin = get_Efdmin_in_pu();
    if(efdmax<=efdmin)
    {
        osstream<<"Efdmax<=Efdmin was detected: Efdmax="<<efdmax<<", Efdmin="<<efdmin<<"\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE1::save()
{
    ;
}

string PSASPE1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    double KR = get_KR();
    double TR = get_TR_in_s();
    double KA = get_KA();
    double TA = get_TA_in_s();
    double Efdmax = get_Efdmax_in_pu();
    double Efdmin = get_Efdmin_in_pu();
    double TE = get_TE_in_s();
    double KF = get_KF();
    double TF = get_TF_in_s();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<KR<<", "
            <<setw(8)<<setprecision(6)<<TR<<", "
            <<setw(8)<<setprecision(6)<<KA<<", "
            <<setw(8)<<setprecision(6)<<TA<<", "
            <<setw(8)<<setprecision(6)<<TE<<", "
            <<setw(8)<<setprecision(6)<<KF<<", "
            <<setw(8)<<setprecision(6)<<TF<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Efdmax<<", "
            <<setw(8)<<setprecision(6)<<Efdmin<<" /";
    return osstream.str();
}

void PSASPE1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KA", i); i++;
}

double PSASPE1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="KA")
        return get_KA();

    return 0.0;
}

void PSASPE1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KA")
        return set_KA(value);

    return;
}

double PSASPE1::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_TR_in_s()!=0.0 and mint>get_TR_in_s())
        mint = get_TR_in_s();
    if(get_TA_in_s()!=0.0 and mint>get_TA_in_s())
        mint = get_TA_in_s();
    if(get_TE_in_s()!=0.0 and mint>get_TE_in_s())
        mint = get_TE_in_s();
    if(get_TF_in_s()!=0.0 and mint>get_TF_in_s())
        mint = get_TF_in_s();
    return mint;
}


void PSASPE1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@EXCITER", i); i++;
}

double PSASPE1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    if(var_name == "STATE@EXCITER")
        return exciter.get_state();

    return 0.0;
}


string PSASPE1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void PSASPE1::linearize()
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

void PSASPE1::build_linearized_matrix_ABCD()
{
    return;
}
