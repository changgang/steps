#include "header/model/sg_models/exciter_model/IEEET1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
IEEET1::IEEET1()
{
    clear();
}

IEEET1::~IEEET1()
{
    ;
}

void IEEET1::clear()
{
    regulator.set_limiter_type(WINDUP_LIMITER);
    saturation_block.set_saturation_type(QUADRATIC_SATURATION_TYPE);
}

void IEEET1::copy_from_const_model(const IEEET1& model)
{
    clear();
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_VRmax_in_pu(model.get_VRmax_in_pu());
    this->set_VRmin_in_pu(model.get_VRmin_in_pu());
    this->set_KE(model.get_KE());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
    this->set_E1_in_pu(model.get_E1_in_pu());
    this->set_SE1_in_pu(model.get_SE1_in_pu());
    this->set_E2_in_pu(model.get_E2_in_pu());
    this->set_SE2_in_pu(model.get_SE2_in_pu());

}
IEEET1::IEEET1(const IEEET1& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

IEEET1& IEEET1::operator=(const IEEET1& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEET1::get_model_name() const
{
    return "IEEET1";
}

void IEEET1::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void IEEET1::set_KA(double K)
{
    regulator.set_K(K);
}

void IEEET1::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void IEEET1::set_VRmax_in_pu(double vr)
{
    regulator.set_upper_limit(vr);
}

void IEEET1::set_VRmin_in_pu(double vr)
{
    regulator.set_lower_limit(vr);
}

void IEEET1::set_KE(double K)
{
    this->KE = K;
}

void IEEET1::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}

void IEEET1::set_KF(double K)
{
    feedbacker.set_K(K);
}

void IEEET1::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void IEEET1::set_E1_in_pu(double E)
{
    saturation_block.set_V1(E);
}

void IEEET1::set_SE1_in_pu(double S)
{
    saturation_block.set_S1(S);
}

void IEEET1::set_E2_in_pu(double E)
{
    saturation_block.set_V2(E);
}

void IEEET1::set_SE2_in_pu(double S)
{
    saturation_block.set_S2(S);
}

double IEEET1::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

double IEEET1::get_KA() const
{
    return regulator.get_K();
}

double IEEET1::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double IEEET1::get_VRmax_in_pu() const
{
    return regulator.get_upper_limit();
}

double IEEET1::get_VRmin_in_pu() const
{
    return regulator.get_lower_limit();
}

double IEEET1::get_KE() const
{
    return this->KE;
}

double IEEET1::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double IEEET1::get_KF() const
{
    return feedbacker.get_K();
}

double IEEET1::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

double IEEET1::get_E1_in_pu() const
{
    return saturation_block.get_V1();
}

double IEEET1::get_SE1_in_pu() const
{
    return saturation_block.get_S1();
}

double IEEET1::get_E2_in_pu() const
{
    return saturation_block.get_V2();
}

double IEEET1::get_SE2_in_pu() const
{
    return saturation_block.get_S2();
}


bool IEEET1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=17)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double tr, ka, ta, vrmax, vrmin, ke, te, kf, tf, e1, se1, e2, se2;

            unsigned int i=3;
            tr = get_double_data(data[i],"0.0"); i++;
            ka = get_double_data(data[i],"0.0"); i++;
            ta = get_double_data(data[i],"0.0"); i++;
            vrmax = get_double_data(data[i],"0.0"); i++;
            vrmin = get_double_data(data[i],"0.0"); i++;
            ke = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            kf = get_double_data(data[i],"0.0"); i++;
            tf = get_double_data(data[i],"0.0"); i++;
            /*double temp = get_double_data(data[i],"0.0");*/ i++;
            e1 = get_double_data(data[i],"0.0"); i++;
            se1 = get_double_data(data[i],"0.0"); i++;
            e2 = get_double_data(data[i],"0.0"); i++;
            se2 = get_double_data(data[i],"0.0");

            set_TR_in_s(tr);
            set_KA(ka);
            set_TA_in_s(ta);
            set_VRmax_in_pu(vrmax);
            set_VRmin_in_pu(vrmin);
            set_KE(ke);
            set_TE_in_s(te);
            set_KF(kf);
            set_TF_in_s(tf);
            set_E1_in_pu(e1);
            set_SE1_in_pu(se1);
            set_E2_in_pu(e2);
            set_SE2_in_pu(se2);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEET1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEET1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input data is provided: "<<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEEET1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    sensor.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    feedbacker.set_toolkit(toolkit);
    exciter.set_toolkit(toolkit);
}

void IEEET1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        if(generator!=NULL)
        {
            SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
            if(gen_model!=NULL)
            {
                if(not gen_model->is_model_initialized())
                    gen_model->initialize();

                setup_block_toolkit_and_parameters();

                STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
                double Ecomp = get_compensated_voltage_in_pu();

                sensor.set_output(Ecomp);
                sensor.initialize();

                double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();

                exciter.set_output(Efd);
                exciter.initialize();

                feedbacker.set_input(Efd);
                feedbacker.initialize();

                double KE = get_KE();
                double SE = saturation_block.get_saturation(Efd);
                double input = (KE+SE)*Efd;

                if(input>get_VRmax_in_pu())
                {
                    osstream<<"Initialization error. VR of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"VR is "<<input<<", and VRmax is "<<get_VRmax_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(input<get_VRmin_in_pu())
                {
                    osstream<<"Initialization error. VR of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"VR is "<<input<<", and VRmin is "<<get_VRmin_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                regulator.set_output(input);
                regulator.initialize();

                input = regulator.get_input();

                double Vref = input+Ecomp;
                set_voltage_reference_in_pu(Vref);

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void IEEET1::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        double Ecomp = get_compensated_voltage_in_pu();
        double Vref = get_voltage_reference_in_pu();
        double Vs = get_stabilizing_signal_in_pu();

        sensor.set_input(Ecomp);
        sensor.run(mode);

        double Efd = exciter.get_output();
        feedbacker.set_input(Efd);
        feedbacker.run(mode);

        double input = Vref-sensor.get_output()+Vs-feedbacker.get_output();

        regulator.set_input(input);
        regulator.run(mode);

        double KE = get_KE();
        double SE = saturation_block.get_saturation(Efd);
        input = regulator.get_output()-(KE+SE)*Efd;

        exciter.set_input(input);
        exciter.run(mode);

        //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

        if(mode == UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double IEEET1::get_excitation_voltage_in_pu()
{
    return exciter.get_output();
}
void IEEET1::check()
{
    ;
}

void IEEET1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void IEEET1::save()
{
    ;
}

string IEEET1::get_standard_psse_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    double TR = get_TR_in_s();
    double KA = get_KA();
    double TA = get_TA_in_s();
    double VRmax = get_VRmax_in_pu();
    double VRmin = get_VRmin_in_pu();
    double KE = get_KE();
    double TE = get_TE_in_s();
    double KF = get_KF();
    double TF = get_TF_in_s();
    double E1 = get_E1_in_pu();
    double S1 = get_SE1_in_pu();
    double E2 = get_E2_in_pu();
    double S2 = get_SE2_in_pu();


    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<TR<<", "
            <<setw(8)<<setprecision(6)<<KA<<", "
            <<setw(8)<<setprecision(6)<<TA<<", "
            <<setw(8)<<setprecision(6)<<VRmax<<", "
            <<setw(8)<<setprecision(6)<<VRmin<<", "
            <<setw(8)<<setprecision(6)<<KE<<", "
            <<setw(8)<<setprecision(6)<<TE<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<KF<<", "
            <<setw(8)<<setprecision(6)<<TF<<", "
            <<setw(8)<<setprecision(0)<<0<<", "
            <<setw(8)<<setprecision(6)<<E1<<", "
            <<setw(8)<<setprecision(6)<<S1<<", "
            <<setw(8)<<setprecision(6)<<E2<<", "
            <<setw(8)<<setprecision(6)<<S2<<" /";

    return osstream.str();
}

void IEEET1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double IEEET1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void IEEET1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void IEEET1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@EXCITER", i); i++;
}

double IEEET1::get_model_internal_variable_with_name(string var_name)
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



string IEEET1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEET1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEET1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
