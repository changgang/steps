#include "header/model/sg_models/exciter_model/CSEET2.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
CSEET2::CSEET2(STEPS& toolkit) : EXCITER_MODEL(toolkit),
                                 sensor(toolkit),
                                 serial_tuner1_lead_lag(toolkit),
                                 serial_tuner1_pi(toolkit),
                                 serial_tuner2(toolkit),
                                 parallel_integral(toolkit),
                                 parallel_differential(toolkit),
                                 regulator(toolkit),
                                 feedbacker(toolkit)
{
    clear();
}

void CSEET2::clear()
{
    set_stabilizer_slot(AT_VOLTAGE_ERROR);
    set_excitation_source(SEPARATE_EXCITATION);
    set_tuner_type(SERIAL_TUNER);

    sensor.set_limiter_type(NO_LIMITER);
    sensor.set_K(1.0);

    set_serial_tuner_KV(true);

    serial_tuner1_lead_lag.set_limiter_type(NON_WINDUP_LIMITER);
    serial_tuner1_lead_lag.set_upper_limit(999.0);
    serial_tuner1_lead_lag.set_lower_limit(-999.0);

    serial_tuner1_pi.set_limiter_type(NON_WINDUP_LIMITER);
    serial_tuner1_pi.set_upper_limit(999.0);
    serial_tuner1_pi.set_lower_limit(-999.0);

    serial_tuner2.set_limiter_type(NO_LIMITER);

    parallel_integral.set_limiter_type(NON_WINDUP_LIMITER);
    parallel_integral.set_upper_limit(999.0);
    parallel_integral.set_lower_limit(-999.0);

    parallel_differential.set_limiter_type(NON_WINDUP_LIMITER);
    parallel_differential.set_upper_limit(999.0);
    parallel_differential.set_lower_limit(-999.0);

    regulator.set_limiter_type(NON_WINDUP_LIMITER);

    feedbacker.set_limiter_type(NO_LIMITER);

    set_VRmax_in_pu(0.0);
    set_VRmin_in_pu(0.0);

    set_KC(0.0);
}
void CSEET2::copy_from_const_model(const CSEET2& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    sensor.set_toolkit(toolkit);
    serial_tuner1_lead_lag.set_toolkit(toolkit);
    serial_tuner1_pi.set_toolkit(toolkit);
    serial_tuner2.set_toolkit(toolkit);
    parallel_integral.set_toolkit(toolkit);
    parallel_differential.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    feedbacker.set_toolkit(toolkit);

    clear();

    this->set_excitation_source(model.get_excitation_source());
    this->set_stabilizer_slot(model.get_stabilizer_slot());
    this->set_tuner_type(model.get_tuner_type());

    this->set_TR_in_s(model.get_TR_in_s());
    if(get_tuner_type()==SERIAL_TUNER)
    {
        this->set_serial_tuner_KV(model.get_serial_tuner_KV());
        this->set_serial_tuner_K(model.get_serial_tuner_K());
        this->set_serial_tuner_T1_in_s(model.get_serial_tuner_T1_in_s());
        this->set_serial_tuner_T2_in_s(model.get_serial_tuner_T2_in_s());
        this->set_serial_tuner_VA1max_in_pu(model.get_serial_tuner_VA1max_in_pu());
        this->set_serial_tuner_VA1min_in_pu(model.get_serial_tuner_VA1min_in_pu());
        this->set_serial_tuner_T3_in_s(model.get_serial_tuner_T3_in_s());
        this->set_serial_tuner_T4_in_s(model.get_serial_tuner_T4_in_s());
    }
    else
    {
        this->set_parallel_tuner_KP(model.get_parallel_tuner_KP());
        this->set_parallel_tuner_KI(model.get_parallel_tuner_KI());
        this->set_parallel_tuner_VImax_in_pu(model.get_parallel_tuner_VImax_in_pu());
        this->set_parallel_tuner_VImin_in_pu(model.get_parallel_tuner_VImin_in_pu());
        this->set_parallel_tuner_KD(model.get_parallel_tuner_KD());
        this->set_parallel_tuner_TD_in_s(model.get_parallel_tuner_TD_in_s());
        this->set_parallel_tuner_VDmax_in_pu(model.get_parallel_tuner_VDmax_in_pu());
        this->set_parallel_tuner_VDmin_in_pu(model.get_parallel_tuner_VDmin_in_pu());
    }
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_VAmax_in_pu(model.get_VAmax_in_pu());
    this->set_VAmin_in_pu(model.get_VAmin_in_pu());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
    this->set_VRmax_in_pu(model.get_VRmax_in_pu());
    this->set_VRmin_in_pu(model.get_VRmin_in_pu());
    this->set_KC(model.get_KC());
}

CSEET2::~CSEET2()
{
    ;
}

CSEET2::CSEET2(const CSEET2& model) : EXCITER_MODEL(model.get_toolkit()),
                                      sensor(model.get_toolkit()),
                                      serial_tuner1_lead_lag(model.get_toolkit()),
                                      serial_tuner1_pi(model.get_toolkit()),
                                      serial_tuner2(model.get_toolkit()),
                                      parallel_integral(model.get_toolkit()),
                                      parallel_differential(model.get_toolkit()),
                                      regulator(model.get_toolkit()),
                                      feedbacker(model.get_toolkit())
{
    copy_from_const_model(model);
}

CSEET2& CSEET2::operator=(const CSEET2& model)
{
    if(this==(&model)) return (*this);

    copy_from_const_model(model);

    return (*this);
}

string CSEET2::get_model_name() const
{
    return "CSEET2";
}

void CSEET2::set_excitation_source(AVR_EXCITATION_SOURCE source)
{
    excitation_source = source;
}

void CSEET2::set_stabilizer_slot(AVR_FEEDBACK_SLOT slot)
{
    stabilizer_slot = slot;
}

void CSEET2::set_tuner_type(AVR_TUNER_TYPE type)
{
    tuner_type = type;
}

void CSEET2::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void CSEET2::set_serial_tuner_KV(bool K)
{
    KV = K;
}

void CSEET2::set_serial_tuner_K(double K)
{
    serial_tuner1_lead_lag.set_K(K);
}

void CSEET2::set_serial_tuner_T1_in_s(double T)
{
    serial_tuner1_lead_lag.set_T1_in_s(T);
}

void CSEET2::set_serial_tuner_T2_in_s(double T)
{
    serial_tuner1_lead_lag.set_T2_in_s(T);
}

void CSEET2::set_serial_tuner_VA1max_in_pu(double vmax)
{
    serial_tuner1_lead_lag.set_upper_limit(vmax);
}

void CSEET2::set_serial_tuner_VA1min_in_pu(double vmin)
{
    serial_tuner1_lead_lag.set_lower_limit(vmin);
}

void CSEET2::set_serial_tuner_T3_in_s(double T)
{
    serial_tuner2.set_T1_in_s(T);
}

void CSEET2::set_serial_tuner_T4_in_s(double T)
{
    serial_tuner2.set_T2_in_s(T);
}

void CSEET2::set_parallel_tuner_KP(double K)
{
    KP = K;
}

void CSEET2::set_parallel_tuner_KI(double K)
{
    parallel_integral.set_T_in_s(1.0/K);
}

void CSEET2::set_parallel_tuner_VImax_in_pu(double vmax)
{
    parallel_integral.set_upper_limit(vmax);
}

void CSEET2::set_parallel_tuner_VImin_in_pu(double vmin)
{
    parallel_integral.set_lower_limit(vmin);
}

void CSEET2::set_parallel_tuner_KD(double K)
{
    parallel_differential.set_K(K);
}

void CSEET2::set_parallel_tuner_TD_in_s(double T)
{
    parallel_differential.set_T_in_s(T);
}

void CSEET2::set_parallel_tuner_VDmax_in_pu(double vmax)
{
    parallel_differential.set_upper_limit(vmax);
}

void CSEET2::set_parallel_tuner_VDmin_in_pu(double vmin)
{
    parallel_differential.set_lower_limit(vmin);
}

void CSEET2::set_KA(double K)
{
    regulator.set_K(K);
}

void CSEET2::set_TA_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void CSEET2::set_VAmax_in_pu(double vmax)
{
    regulator.set_upper_limit(vmax);
}

void CSEET2::set_VAmin_in_pu(double vmin)
{
    regulator.set_lower_limit(vmin);
}

void CSEET2::set_KF(double K)
{
    feedbacker.set_K(K);
}

void CSEET2::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void CSEET2::set_VRmax_in_pu(double vmax)
{
    VRmax = vmax;
}

void CSEET2::set_VRmin_in_pu(double vmin)
{
    VRmin = vmin;
}

void CSEET2::set_KC(double K)
{
    this->KC = K;
}

AVR_EXCITATION_SOURCE CSEET2::get_excitation_source() const
{
    return excitation_source;
}

AVR_FEEDBACK_SLOT CSEET2::get_stabilizer_slot() const
{
    return stabilizer_slot;
}

AVR_TUNER_TYPE CSEET2::get_tuner_type() const
{
    return tuner_type;
}

double CSEET2::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

bool CSEET2::get_serial_tuner_KV() const
{
    return KV;
}

double CSEET2::get_serial_tuner_K() const
{
    return serial_tuner1_lead_lag.get_K();
}

double CSEET2::get_serial_tuner_T1_in_s() const
{
    return serial_tuner1_lead_lag.get_T1_in_s();
}

double CSEET2::get_serial_tuner_T2_in_s() const
{
    return serial_tuner1_lead_lag.get_T2_in_s();
}

double CSEET2::get_serial_tuner_VA1max_in_pu() const
{
    return serial_tuner1_lead_lag.get_upper_limit();
}

double CSEET2::get_serial_tuner_VA1min_in_pu() const
{
    return serial_tuner1_lead_lag.get_lower_limit();
}

double CSEET2::get_serial_tuner_T3_in_s() const
{
    return serial_tuner2.get_T1_in_s();
}

double CSEET2::get_serial_tuner_T4_in_s() const
{
    return serial_tuner2.get_T2_in_s();
}

double CSEET2::get_parallel_tuner_KP() const
{
    return KP;
}

double CSEET2::get_parallel_tuner_KI() const
{
    return 1.0/parallel_integral.get_T_in_s();
}

double CSEET2::get_parallel_tuner_VImax_in_pu() const
{
    return parallel_integral.get_upper_limit();
}

double CSEET2::get_parallel_tuner_VImin_in_pu() const
{
    return parallel_integral.get_lower_limit();
}

double CSEET2::get_parallel_tuner_KD() const
{
    return parallel_differential.get_K();
}

double CSEET2::get_parallel_tuner_TD_in_s() const
{
    return parallel_differential.get_T_in_s();
}

double CSEET2::get_parallel_tuner_VDmax_in_pu() const
{
    return parallel_differential.get_upper_limit();
}

double CSEET2::get_parallel_tuner_VDmin_in_pu() const
{
    return parallel_differential.get_lower_limit();
}

double CSEET2::get_KA() const
{
    return regulator.get_K();
}

double CSEET2::get_TA_in_s() const
{
    return regulator.get_T_in_s();
}

double CSEET2::get_VAmax_in_pu() const
{
    return regulator.get_upper_limit();
}

double CSEET2::get_VAmin_in_pu() const
{
    return regulator.get_lower_limit();
}

double CSEET2::get_KF() const
{
    return feedbacker.get_K();
}

double CSEET2::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

double CSEET2::get_VRmax_in_pu() const
{
    return VRmax;
}

double CSEET2::get_VRmin_in_pu() const
{
    return VRmin;
}

double CSEET2::get_KC() const
{
    return this->KC;
}


bool CSEET2::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=24)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int excitation_source, stabilizer_slot, tuner_type;
            double tr;
            // serial
            double serial_k;
            unsigned int serial_kv;
            double t1, t2, va1max, va1min, t3, t4;
            // parallel
            double parallel_kp, parallel_ki, vimax, vimin, parallel_kd, parallel_td, vdmax, vdmin;
            // common
            double ka, ta, vamax, vamin, kf, tf, vrmax, vrmin, kc;

            unsigned int i=3;
            excitation_source = get_integer_data(data[i],"0"); i++; if(excitation_source!=0) excitation_source = 1;
            stabilizer_slot = get_integer_data(data[i],"0"); i++; if(stabilizer_slot!=0) stabilizer_slot = 1;
            tuner_type = get_integer_data(data[i],"0"); i++; if(tuner_type!=0) tuner_type = 1;
            tr = get_double_data(data[i],"1.0"); i++;

            if(excitation_source==0)
                set_excitation_source(SELF_EXCITATION);
            else
                set_excitation_source(SEPARATE_EXCITATION);

            if(stabilizer_slot==0)
                set_stabilizer_slot(AT_VOLTAGE_ERROR);
            else
                set_stabilizer_slot(AT_REGULATOR);

            if(tuner_type==0)
                set_tuner_type(SERIAL_TUNER);
            else
                set_tuner_type(PARALLEL_TUNER);

            set_TR_in_s(tr);


            if(tuner_type==0) // serial
            {
                serial_k = get_double_data(data[i],"1.0"); i++;
                serial_kv = get_integer_data(data[i],"1"); i++;
                t1 = get_double_data(data[i],"1.0"); i++;
                t2 = get_double_data(data[i],"1.0"); i++;
                va1max = get_double_data(data[i],"999.0"); i++;
                va1min = get_double_data(data[i],"-999.0"); i++;
                t3 = get_double_data(data[i],"1.0"); i++;
                t4 = get_double_data(data[i],"1.0"); i++;

                if(t2==0.0)
                {
                    osstream<<"Error. T2 in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(t4==0.0)
                {
                    osstream<<"Error. T4 in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                set_serial_tuner_K(serial_k);
                set_serial_tuner_KV(serial_kv);
                set_serial_tuner_T1_in_s(t1);
                set_serial_tuner_T2_in_s(t2);
                set_serial_tuner_VA1max_in_pu(va1max);
                set_serial_tuner_VA1min_in_pu(va1min);
                set_serial_tuner_T3_in_s(t3);
                set_serial_tuner_T4_in_s(t4);
            }
            else
            {
                parallel_kp = get_double_data(data[i],"1.0"); i++;
                parallel_ki = get_double_data(data[i],"1.0"); i++;
                vimax = get_double_data(data[i],"999.0"); i++;
                vimin = get_double_data(data[i],"-999.0"); i++;
                parallel_kd = get_double_data(data[i],"1.0"); i++;
                parallel_td = get_double_data(data[i],"1.0"); i++;
                vdmax = get_double_data(data[i],"999.0"); i++;
                vdmin = get_double_data(data[i],"-999.0"); i++;

                if(parallel_td==0.0)
                {
                    osstream<<"Error. Td in parallel mode in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                set_parallel_tuner_KP(parallel_kp);
                set_parallel_tuner_KI(parallel_ki);
                set_parallel_tuner_VImax_in_pu(vimax);
                set_parallel_tuner_VImin_in_pu(vimin);
                set_parallel_tuner_KD(parallel_kd);
                set_parallel_tuner_TD_in_s(parallel_td);
                set_parallel_tuner_VDmax_in_pu(vdmax);
                set_parallel_tuner_VDmin_in_pu(vdmin);
            }

            ka = get_double_data(data[i],"1.0"); i++;
            ta = get_double_data(data[i],"0.0"); i++;
            vamax = get_double_data(data[i],"0.0"); i++;
            vamin = get_double_data(data[i],"0.0"); i++;
            kf = get_double_data(data[i],"0.0"); i++;
            tf = get_double_data(data[i],"0.0"); i++;
            vrmax = get_double_data(data[i],"0.0"); i++;
            vrmin = get_double_data(data[i],"0.0"); i++;
            kc = get_double_data(data[i],"0.0"); i++;

            if(tf==0.0)
            {
                osstream<<"Error. Tf in "<<get_model_name()<<" is zero for"<<get_compound_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            set_KA(ka);
            set_TA_in_s(ta);
            set_VAmax_in_pu(vamax);
            set_VAmin_in_pu(vamin);
            set_KF(kf);
            set_TF_in_s(tf);
            set_VRmax_in_pu(vrmax);
            set_VRmin_in_pu(vrmin);
            set_KC(kc);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool CSEET2::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool CSEET2::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void CSEET2::setup_block_toolkit_and_parameters()
{
}

void CSEET2::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        STEPS& toolkit = get_toolkit();
        //osstream<<"now go initialize "<<get_model_name()<<" for "<<get_compound_device_name();
        //toolkit.show_information_with_leading_time_stamp(osstream);
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            if(get_tuner_type()==SERIAL_TUNER)
            {
                if(get_serial_tuner_KV()==false)
                {
                    double K = get_serial_tuner_K();
                    double T1 = get_serial_tuner_T1_in_s();
                    double T2 = get_serial_tuner_T2_in_s();

                    serial_tuner1_pi.set_Kp(K*T1/T2);
                    serial_tuner1_pi.set_Ki(K/T2);
                }
            }

            double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
            double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();
            double Kc = get_KC();

            double Vr = Efd + Kc*Ifd;
            if(Vr>get_VRmax_in_pu())
            {
                osstream<<"Initialization error. Regulating excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                  <<"Vr is "<<Vr<<", and Vrmax is "<<get_VRmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Vr<get_VRmin_in_pu())
            {
                osstream<<"Initialization error. Regulating excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                  <<"Vr is "<<Vr<<", and Vrmin is "<<get_VRmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Vr>get_VAmax_in_pu())
            {
                osstream<<"Initialization error. Regulating excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                  <<"Va is "<<Vr<<", and VAmax is "<<get_VAmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Vr<get_VAmin_in_pu())
            {
                osstream<<"Initialization error. Regulating excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                  <<"Va is "<<Vr<<", and VAmin is "<<get_VAmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            regulator.set_output(Vr);
            regulator.initialize();

            feedbacker.set_input(Vr);
            feedbacker.initialize();

            double output = regulator.get_input();

            if(get_tuner_type()==SERIAL_TUNER)
            {
                serial_tuner2.set_output(output);
                serial_tuner2.initialize();

                output = serial_tuner2.get_input();
                if(get_serial_tuner_KV()==true)
                {
                    serial_tuner1_lead_lag.set_output(output);
                    serial_tuner1_lead_lag.initialize();
                    output = serial_tuner1_lead_lag.get_input();
                }
                else
                {
                    //osstream<<"serial tuner output :"<<output;
                    //toolkit.show_information_with_leading_time_stamp(osstream);
                    serial_tuner1_pi.set_output(output);
                    serial_tuner1_pi.initialize();
                    output = serial_tuner1_pi.get_input();
                }
            }
            else // parallel tuner
            {
                parallel_integral.set_output(output);
                parallel_integral.initialize();

                parallel_differential.set_input(0.0);
                parallel_differential.initialize();

                output = 0.0;
            }

            double Ecomp = get_compensated_voltage_in_pu();
            sensor.set_output(Ecomp);
            sensor.initialize();

            double Vref = Ecomp+output;
            set_voltage_reference_in_pu(Vref);

            set_flag_model_initialized_as_true();
        }
    }
}

void CSEET2::run(DYNAMIC_MODE mode)
{
    sensor.set_input(get_compensated_voltage_in_pu());
    sensor.run(mode);

    double Vs = get_stabilizing_signal_in_pu();
    double input = get_voltage_reference_in_pu()-sensor.get_output();
    if(get_stabilizer_slot()==AT_VOLTAGE_ERROR)
        input += Vs;

    if(get_tuner_type()==SERIAL_TUNER)
    {
        if(get_serial_tuner_KV()==true)
        {
            serial_tuner1_lead_lag.set_input(input);
            serial_tuner1_lead_lag.run(mode);
            input = serial_tuner1_lead_lag.get_output();
        }
        else
        {
            serial_tuner1_pi.set_input(input);
            serial_tuner1_pi.run(mode);
            input = serial_tuner1_pi.get_output();
        }
        serial_tuner2.set_input(input);
        serial_tuner2.run(mode);
        input = serial_tuner2.get_output();
    }
    else // parallel tuner
    {
        parallel_integral.set_input(input);
        parallel_integral.run(mode);

        parallel_differential.set_input(input);
        parallel_differential.run(mode);

        input = get_parallel_tuner_KP()*input+parallel_integral.get_output()+parallel_differential.get_output();
    }

    for(unsigned int i=0; i<STEPS_MODEL_FEEDBACK_LOOP_INTEGRATION_COUNT; ++i)
    {
        double input2 = input - feedbacker.get_output();
        if(get_stabilizer_slot()==AT_REGULATOR)
            input2 += Vs;

        regulator.set_input(input2);
        regulator.run(mode);
        input2 = regulator.get_output();

        feedbacker.set_input(input2);
        feedbacker.run(mode);

        if(feedbacker.get_K()==0.0)
            break;
    }

    //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

    if(mode == DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double CSEET2::get_excitation_voltage_in_pu()
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();

    double Efd = regulator.get_output();
    double Vrmax = get_VRmax_in_pu();
    double Vrmin = get_VRmin_in_pu();

    //unsigned int bus = generator->get_generator_bus();
    //double Vt = psdb.get_bus_positive_sequence_voltage_in_pu(bus);

    if(get_excitation_source()==SELF_EXCITATION)
    {
        double Vt = get_terminal_voltage_in_pu();
        Vrmax *= Vt;
        Vrmin *= Vt;
    }

    if(Efd>Vrmax)
        Efd = Vrmax;

    if(Efd<Vrmin)
        Efd = Vrmin;

    Efd -= get_KC()*gen_model->get_field_current_in_pu_based_on_mbase();

    return Efd;
}
void CSEET2::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(get_tuner_type()==SERIAL_TUNER)
    {
        if(get_serial_tuner_T2_in_s()==0.0)
        {
            osstream<<"Serial tuner T2=0 was detected\n";
            error_found = true;
        }
        if(get_serial_tuner_T4_in_s()==0.0)
        {
            osstream<<"Serial tuner T4=0 was detected\n";
            error_found = true;
        }
    }
    else//PARALLEL_TUNER
    {
        if(get_parallel_tuner_TD_in_s()==0.0)
        {
            osstream<<"Parallel tuner TD=0 was detected\n";
            error_found = true;
        }
        double vimax = get_parallel_tuner_VImax_in_pu();
        double vimin = get_parallel_tuner_VImin_in_pu();
        if(vimax<=0.0)
        {
            osstream<<"VImax<=0.0 was detected for parallel tuner: VImax="<<vimax<<"\n";
            error_found = true;
        }
        if(vimin>=0.0)
        {
            osstream<<"VImin>=0.0 was detected for parallel tuner: VImin="<<vimin<<"\n";
            error_found = true;
        }
        if(vimax<=vimin)
        {
            osstream<<"VImax<=VImin was detected for parallel tuner: VImax="<<vimax<<", VImin="<<vimin<<"\n";
            error_found = true;
        }

        double vdmax = get_parallel_tuner_VDmax_in_pu();
        double vdmin = get_parallel_tuner_VDmin_in_pu();
        if(vdmax<=0.0)
        {
            osstream<<"VDmax<=0.0 was detected for parallel tuner: VDmax="<<vdmax<<"\n";
            error_found = true;
        }
        if(vdmin>=0.0)
        {
            osstream<<"VDmin>=0.0 was detected for parallel tuner: VDmin="<<vdmin<<"\n";
            error_found = true;
        }
        if(vdmax<=vdmin)
        {
            osstream<<"VDmax<=VDmin was detected for parallel tuner: VDmax="<<vdmax<<", VDmin="<<vdmin<<"\n";
            error_found = true;
        }
    }
    if(get_TA_in_s()==0.0)
    {
        osstream<<"TA=0 was detected\n";
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
    if(vamax<=0.0)
    {
        osstream<<"VAmax<=0.0 was detected: VAmax="<<vamax<<"\n";
        error_found = true;
    }
    double vrmax = get_VRmax_in_pu();
    double vrmin = get_VRmin_in_pu();
    if(vrmax<=vrmin)
    {
        osstream<<"VRmax<=VRmin was detected: VRmax="<<vrmax<<", VRmin="<<vrmin<<"\n";
        error_found = true;
    }
    if(vrmax<=0.0)
    {
        osstream<<"VRmax<=0.0 was detected: VRmax="<<vrmax<<"\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void CSEET2::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void CSEET2::save()
{
    ;
}

string CSEET2::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    unsigned int source_index = 0;
    switch(get_excitation_source())
    {
        case SELF_EXCITATION:
            source_index = 0;
            break;
        default:
            source_index = 1;
            break;
    }
    unsigned int stabilizer_index = 0;
    switch(get_stabilizer_slot())
    {
        case AT_VOLTAGE_ERROR:
            stabilizer_index = 0;
            break;
        default:
            stabilizer_index = 1;
            break;
    }

    unsigned int tuner_type = 0;
    switch(get_tuner_type())
    {
        case SERIAL_TUNER:
            tuner_type = 0;
            break;
        default:
            tuner_type = 1;
            break;
    }
    double TR = get_TR_in_s();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<source_index<<", "
            <<setw(8)<<stabilizer_index<<", "
            <<setw(8)<<tuner_type<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<TR<<", ";

    if(get_tuner_type()==SERIAL_TUNER)
    {
        double K = get_serial_tuner_K();
        bool KV = get_serial_tuner_KV();
        unsigned int selector = 0;
        if(KV) selector = 1;
        double T1 = get_serial_tuner_T1_in_s();
        double T2 = get_serial_tuner_T2_in_s();
        double VA1max = get_serial_tuner_VA1max_in_pu();
        double VA1min = get_serial_tuner_VA1min_in_pu();
        double T3 = get_serial_tuner_T3_in_s();
        double T4 = get_serial_tuner_T4_in_s();

        osstream<<setw(8)<<setprecision(6)<<K<<", "
                <<setw(8)<<selector<<", "
                <<setw(8)<<setprecision(6)<<T1<<", "
                <<setw(8)<<setprecision(6)<<T2<<", "
                <<setw(8)<<setprecision(6)<<VA1max<<", "
                <<setw(8)<<setprecision(6)<<VA1min<<", "
                <<setw(8)<<setprecision(6)<<T3<<", "
                <<setw(8)<<setprecision(6)<<T4<<", \n"
                <<setw(10)<<"";
    }
    else
    {
        double KP = get_parallel_tuner_KP();
        double KI = get_parallel_tuner_KI();
        double Vimax = get_parallel_tuner_VImax_in_pu();
        double Vimin = get_parallel_tuner_VImin_in_pu();
        double KD = get_parallel_tuner_KD();
        double TD = get_parallel_tuner_TD_in_s();
        double Vdmax = get_parallel_tuner_VDmax_in_pu();
        double Vdmin = get_parallel_tuner_VDmin_in_pu();

        osstream<<setw(8)<<setprecision(6)<<KP<<", "
                <<setw(8)<<setprecision(6)<<KI<<", "
                <<setw(8)<<setprecision(6)<<Vimax<<", "
                <<setw(8)<<setprecision(6)<<Vimin<<", "
                <<setw(8)<<setprecision(6)<<KD<<", "
                <<setw(8)<<setprecision(6)<<TD<<", "
                <<setw(8)<<setprecision(6)<<Vdmax<<", "
                <<setw(8)<<setprecision(6)<<Vdmin<<", \n"
                <<setw(10)<<"";
    }
    double KA = get_KA();
    double TA = get_TA_in_s();
    double VAmax = get_VAmax_in_pu();
    double VAmin = get_VAmin_in_pu();
    double KF = get_KF();
    double TF = get_TF_in_s();
    double VRmax = get_VRmax_in_pu();
    double VRmin = get_VRmin_in_pu();
    double KC = get_KC();

    osstream<<setw(8)<<setprecision(6)<<KA<<", "
            <<setw(8)<<setprecision(6)<<TA<<", "
            <<setw(8)<<setprecision(6)<<VAmax<<", "
            <<setw(8)<<setprecision(6)<<VAmin<<", "
            <<setw(8)<<setprecision(6)<<KF<<", "
            <<setw(8)<<setprecision(6)<<TF<<", "
            <<setw(8)<<setprecision(6)<<VRmax<<", "
            <<setw(8)<<setprecision(6)<<VRmin<<", "
            <<setw(8)<<setprecision(6)<<KC<<" /";

    return osstream.str();
}

void CSEET2::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double CSEET2::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void CSEET2::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double CSEET2::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_tuner_type()==SERIAL_TUNER)
    {
        if(get_serial_tuner_T2_in_s()!=0.0 and mint>get_serial_tuner_T2_in_s())
            mint = get_serial_tuner_T2_in_s();
        if(get_serial_tuner_T4_in_s()!=0.0 and mint>get_serial_tuner_T4_in_s())
            mint = get_serial_tuner_T4_in_s();
    }
    else
    {
        if(get_parallel_tuner_TD_in_s()!=0.0 and mint>get_parallel_tuner_TD_in_s())
            mint = get_parallel_tuner_TD_in_s();
    }
    if(get_TA_in_s()!=0.0 and mint>get_TA_in_s())
        mint = get_TA_in_s();
    if(get_TF_in_s()!=0.0 and mint>get_TF_in_s())
        mint = get_TF_in_s();
    return mint;
}

void CSEET2::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@TUNER1", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@TUNER2", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
}

double CSEET2::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@TUNER1")
    {
        if(get_tuner_type()==SERIAL_TUNER)
        {
            if(get_serial_tuner_KV()==true)
                return serial_tuner1_lead_lag.get_state();
            else
                return serial_tuner1_pi.get_state();
        }
        else
            return parallel_integral.get_state();
    }

    if(var_name == "STATE@TUNER2")
    {
        if(get_tuner_type()==SERIAL_TUNER)
            return serial_tuner2.get_state();
        else
            return parallel_differential.get_state();
    }

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    return 0.0;
}



string CSEET2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string CSEET2::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string CSEET2::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void CSEET2::linearize()
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

void CSEET2::build_linearized_matrix_ABCD()
{
    return;
}
