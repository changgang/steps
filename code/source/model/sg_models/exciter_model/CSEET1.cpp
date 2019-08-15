#include "header/model/sg_models/exciter_model/CSEET1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
CSEET1::CSEET1()
{
    clear();
}

void CSEET1::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_feedback_slot(AT_VOLTAGE_ERROR);
    set_excitation_source(SEPARATE_EXCITATION);
    set_exciter_brush(WITHOUT_BRUSH);
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

    parallel_tuner_integral.set_limiter_type(NON_WINDUP_LIMITER);
    parallel_tuner_integral.set_upper_limit(999.0);
    parallel_tuner_integral.set_lower_limit(-999.0);
    parallel_tuner_differential.set_limiter_type(NON_WINDUP_LIMITER);
    parallel_tuner_differential.set_upper_limit(999.0);
    parallel_tuner_differential.set_lower_limit(-999.0);

    regulator1.set_limiter_type(NON_WINDUP_LIMITER);

    feedbacker.set_limiter_type(NO_LIMITER);

    set_KH(0.0);

    regulator2.set_limiter_type(NO_LIMITER);
    set_VRmax_in_pu(0.0);
    set_VRmin_in_pu(0.0);

    exciter.set_limiter_type(WINDUP_LIMITER);
    exciter.set_lower_limit(-999.0);

    set_KE(1.0);

    saturation_block.set_saturation_type(EXPONENTIAL_SATURATION_TYPE_INPUT_AS_EXPONETIAL);
    saturation_block.set_V1(0.75);
    saturation_block.set_V2(1.0);

    set_KC(0.0);
    set_KD(0.0);

    set_Efdmax_in_pu(0.0);
}

CSEET1::~CSEET1()
{
    ;
}

CSEET1::CSEET1(const CSEET1& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

CSEET1& CSEET1::operator=(const CSEET1& model)
{
    if(this==(&model)) return (*this);

    copy_from_const_model(model);

    return (*this);
}
void CSEET1::copy_from_const_model(const CSEET1& model)
{
    clear();

    this->set_excitation_source(model.get_excitation_source());
    this->set_exciter_brush(model.get_exciter_brush());
    this->set_feedback_slot(model.get_feedback_slot());
    this->set_tuner_type(model.get_tuner_type());

    this->set_TR_in_s(model.get_TR_in_s());
    if(model.get_tuner_type()==SERIAL_TUNER)
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
    else // PARALLEL_TUNER
    {
        this->set_parallel_tuner_KP(model.get_parallel_tuner_KP());
        this->set_parallel_tuner_KI(model.get_parallel_tuner_KI());
        this->set_parallel_tuner_VImax_in_pu(model.get_parallel_tuner_VImax_in_pu());
        this->set_parallel_tuner_VImin_in_pu(model.get_parallel_tuner_VImin_in_pu());
        this->set_parallel_tuner_KD(model.get_parallel_tuner_KD());
        this->set_parallel_tuner_VDmax_in_pu(model.get_parallel_tuner_VDmax_in_pu());
        this->set_parallel_tuner_VDmin_in_pu(model.get_parallel_tuner_VDmin_in_pu());
    }
    this->set_KA(model.get_KA());
    this->set_TA_in_s(model.get_TA_in_s());
    this->set_VAmax_in_pu(model.get_VAmax_in_pu());
    this->set_VAmin_in_pu(model.get_VAmin_in_pu());
    this->set_KH(model.get_KH());
    this->set_KF(model.get_KF());
    this->set_TF_in_s(model.get_TF_in_s());
    this->set_KB(model.get_KB());
    this->set_T5_in_s(model.get_T5_in_s());
    this->set_VRmax_in_pu(model.get_VRmax_in_pu());
    this->set_VRmin_in_pu(model.get_VRmin_in_pu());
    this->set_KE(model.get_KE());
    this->set_TE_in_s(model.get_TE_in_s());
    this->set_Vemax_in_pu(model.get_Vemax_in_pu());
    this->set_SE75_in_pu(model.get_SE75_in_pu());
    this->set_SE100_in_pu(model.get_SE100_in_pu());
    this->set_KC(model.get_KC());
    this->set_KD(model.get_KD());
    this->set_Efdmax_in_pu(model.get_Efdmax_in_pu());
}

string CSEET1::get_model_name() const
{
    return "CSEET1";
}

void CSEET1::set_excitation_source(AVR_EXCITATION_SOURCE source)
{
    excitation_source = source;
}

void CSEET1::set_exciter_brush(AVR_EXCITER_BRUSH brush)
{
    exciter_brush = brush;
}

void CSEET1::set_feedback_slot(AVR_FEEDBACK_SLOT slot)
{
    feedback_slot = slot;
}

void CSEET1::set_tuner_type(AVR_TUNER_TYPE type)
{
    tuner_type = type;
}

void CSEET1::set_TR_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void CSEET1::set_serial_tuner_KV(bool K)
{
    KV = K;
}

void CSEET1::set_serial_tuner_K(double K)
{
    serial_tuner1_lead_lag.set_K(K);
}

void CSEET1::set_serial_tuner_T1_in_s(double T)
{
    serial_tuner1_lead_lag.set_T1_in_s(T);
}

void CSEET1::set_serial_tuner_T2_in_s(double T)
{
    serial_tuner1_lead_lag.set_T2_in_s(T);
}

void CSEET1::set_serial_tuner_VA1max_in_pu(double vmax)
{
    serial_tuner1_lead_lag.set_upper_limit(vmax);
}

void CSEET1::set_serial_tuner_VA1min_in_pu(double vmin)
{
    serial_tuner1_lead_lag.set_lower_limit(vmin);
}

void CSEET1::set_serial_tuner_T3_in_s(double T)
{
    serial_tuner2.set_T1_in_s(T);
}

void CSEET1::set_serial_tuner_T4_in_s(double T)
{
    serial_tuner2.set_T2_in_s(T);
}

void CSEET1::set_parallel_tuner_KP(double K)
{
    KP = K;
}

void CSEET1::set_parallel_tuner_KI(double K)
{
    parallel_tuner_integral.set_T_in_s(1.0/K);
}

void CSEET1::set_parallel_tuner_VImax_in_pu(double vmax)
{
    parallel_tuner_integral.set_upper_limit(vmax);
}

void CSEET1::set_parallel_tuner_VImin_in_pu(double vmin)
{
    parallel_tuner_integral.set_lower_limit(vmin);
}

void CSEET1::set_parallel_tuner_KD(double K)
{
    parallel_tuner_differential.set_K(K);
}

void CSEET1::set_parallel_tuner_TD_in_s(double T)
{
    parallel_tuner_differential.set_T_in_s(T);
}

void CSEET1::set_parallel_tuner_VDmax_in_pu(double vmax)
{
    parallel_tuner_differential.set_upper_limit(vmax);
}

void CSEET1::set_parallel_tuner_VDmin_in_pu(double vmin)
{
    parallel_tuner_differential.set_lower_limit(vmin);
}

void CSEET1::set_KA(double K)
{
    regulator1.set_K(K);
}

void CSEET1::set_TA_in_s(double T)
{
    regulator1.set_T_in_s(T);
}

void CSEET1::set_VAmax_in_pu(double vmax)
{
    regulator1.set_upper_limit(vmax);
}

void CSEET1::set_VAmin_in_pu(double vmin)
{
    regulator1.set_lower_limit(vmin);
}

void CSEET1::set_KH(double K)
{
    this->KH = K;
}

void CSEET1::set_KF(double K)
{
    feedbacker.set_K(K);
}

void CSEET1::set_TF_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void CSEET1::set_KB(double K)
{
    regulator2.set_K(K);
}

void CSEET1::set_T5_in_s(double T)
{
    regulator2.set_T_in_s(T);
}

void CSEET1::set_VRmax_in_pu(double vmax)
{
    Vrmax = vmax;
}

void CSEET1::set_VRmin_in_pu(double vmin)
{
    Vrmin = vmin;
}

void CSEET1::set_KE(double K)
{
    this->KE = K;
}

void CSEET1::set_TE_in_s(double T)
{
    exciter.set_T_in_s(T);
}

void CSEET1::set_Vemax_in_pu(double vmax)
{
    exciter.set_upper_limit(vmax);
}

void CSEET1::set_Efdmax_in_pu(double vmax)
{
    this->Efdmax = vmax;
}

void CSEET1::set_SE75_in_pu(double S)
{
    saturation_block.set_S1(S);
}

void CSEET1::set_SE100_in_pu(double S)
{
    saturation_block.set_S2(S);
}

void CSEET1::set_KC(double K)
{
    this->KC = K;
}

void CSEET1::set_KD(double K)
{
    this->KD = K;
}

AVR_EXCITATION_SOURCE CSEET1::get_excitation_source() const
{
    return excitation_source;
}

AVR_EXCITER_BRUSH CSEET1::get_exciter_brush() const
{
    return exciter_brush;
}

AVR_FEEDBACK_SLOT CSEET1::get_feedback_slot() const
{
    return feedback_slot;
}

AVR_TUNER_TYPE CSEET1::get_tuner_type() const
{
    return tuner_type;
}

double CSEET1::get_TR_in_s() const
{
    return sensor.get_T_in_s();
}

bool CSEET1::get_serial_tuner_KV() const
{
    return KV;
}

double CSEET1::get_serial_tuner_K() const
{
    return serial_tuner1_lead_lag.get_K();
}

double CSEET1::get_serial_tuner_T1_in_s() const
{
    return serial_tuner1_lead_lag.get_T1_in_s();
}

double CSEET1::get_serial_tuner_T2_in_s() const
{
    return serial_tuner1_lead_lag.get_T2_in_s();
}

double CSEET1::get_serial_tuner_VA1max_in_pu() const
{
    return serial_tuner1_lead_lag.get_upper_limit();
}

double CSEET1::get_serial_tuner_VA1min_in_pu() const
{
    return serial_tuner1_lead_lag.get_lower_limit();
}

double CSEET1::get_serial_tuner_T3_in_s() const
{
    return serial_tuner2.get_T1_in_s();
}

double CSEET1::get_serial_tuner_T4_in_s() const
{
    return serial_tuner2.get_T2_in_s();
}

double CSEET1::get_parallel_tuner_KP() const
{
    return KP;
}

double CSEET1::get_parallel_tuner_KI() const
{
    return 1.0/parallel_tuner_integral.get_T_in_s();
}

double CSEET1::get_parallel_tuner_VImax_in_pu() const
{
    return parallel_tuner_integral.get_upper_limit();
}

double CSEET1::get_parallel_tuner_VImin_in_pu() const
{
    return parallel_tuner_integral.get_lower_limit();
}

double CSEET1::get_parallel_tuner_KD() const
{
    return parallel_tuner_differential.get_K();
}

double CSEET1::get_parallel_tuner_TD_in_s() const
{
    return parallel_tuner_differential.get_T_in_s();
}

double CSEET1::get_parallel_tuner_VDmax_in_pu() const
{
    return parallel_tuner_differential.get_upper_limit();
}

double CSEET1::get_parallel_tuner_VDmin_in_pu() const
{
    return parallel_tuner_differential.get_lower_limit();
}

double CSEET1::get_KA() const
{
    return regulator1.get_K();
}

double CSEET1::get_TA_in_s() const
{
    return regulator1.get_T_in_s();
}

double CSEET1::get_VAmax_in_pu() const
{
    return regulator1.get_upper_limit();
}

double CSEET1::get_VAmin_in_pu() const
{
    return regulator1.get_lower_limit();
}

double CSEET1::get_KH() const
{
    return this->KH;
}

double CSEET1::get_KF() const
{
    return feedbacker.get_K();
}

double CSEET1::get_TF_in_s() const
{
    return feedbacker.get_T_in_s();
}

double CSEET1::get_KB() const
{
    return regulator2.get_K();
}

double CSEET1::get_T5_in_s() const
{
    return regulator2.get_T_in_s();
}

double CSEET1::get_VRmax_in_pu() const
{
    return Vrmax;
}

double CSEET1::get_VRmin_in_pu() const
{
    return Vrmin;
}

double CSEET1::get_KE() const
{
    return this->KE;
}

double CSEET1::get_TE_in_s() const
{
    return exciter.get_T_in_s();
}

double CSEET1::get_Vemax_in_pu() const
{
    return exciter.get_upper_limit();
}

double CSEET1::get_SE75_in_pu() const
{
    return saturation_block.get_S1();
}

double CSEET1::get_SE100_in_pu() const
{
    return saturation_block.get_S2();
}

double CSEET1::get_KC() const
{
    return this->KC;
}

double CSEET1::get_KD() const
{
    return this->KD;
}

double CSEET1::get_Efdmax_in_pu() const
{
    return this->Efdmax;
}

double CSEET1::get_initial_Ve_with_Fex_function() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return 0.0;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return  0.0;

    double Efd = gen_model->get_initial_excitation_voltage_in_pu();
    double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();

    double Fex = 1.0, newFex = 0.0;
    double Ve = 0.0;
    double oldFex=0.0;

    size_t iter_count = 0;
    while(true)
    {
        ++iter_count;

        Ve = Efd/Fex;
        newFex = get_Fex(Ve, Ifd);
        oldFex = Fex;

        if(fabs(Fex-newFex)>FLOAT_EPSILON)
            Fex = newFex;
        else
            break;

        if(Fex != 0.0)
            ;
        else
        {
            osstream<<"Fatal error. Fex = 0.0 is encountered when initializing exciter CSEET1 of "<<get_device_name()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
            Ve = 0.0;
            break;
        }

        if(iter_count<=100)
            ;
        else
        {
            osstream<<"Warning. Initial Ve is not solved within 100 iterations when initializing exciter CSEET1 of "<<get_device_name()<<".\n"
                    <<"Old Fex = "<<oldFex<<", New Fex = "<<Fex;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    return Ve;
}
double CSEET1::get_Fex(double Ve, double Ifd) const
{
    double KC = get_KC();

    double In = KC*Ifd/Ve;

    double Fex = 0.0;

    if(In<=0.0)
        Fex = 1.0;
    else
    {
        if(In<=0.433)
            Fex = 1-0.577*In;
        else
        {
            if(In<0.75)
                Fex = sqrt(0.75-In*In);
            else
            {
                if(In<=1.0)
                    Fex = 1.732*(1.0-In);
                else
                    Fex = 0.0;
            }
        }
    }
    return Fex;
}


bool CSEET1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=35)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t excitation_source, exciter_brush, feedback_slot, tuner_type;
            double tr;
            // serial
            double serial_k;
            size_t serial_kv;
            double t1, t2, va1max, va1min, t3, t4;
            // parallel
            double parallel_kp, parallel_ki, vimax, vimin, parallel_kd, parallel_td, vdmax, vdmin;
            // common
            double ka, ta, vamax, vamin, kh, kf, tf, kb, t5, vrmax, vrmin, te, vemax, ke, se75, se100, kc, kd, efdmax;

            size_t i=3;
            excitation_source = get_integer_data(data[i],"0"); i++; if(excitation_source!=0) excitation_source = 1;
            exciter_brush = get_integer_data(data[i],"0"); i++; if(exciter_brush!=0) exciter_brush = 1;
            feedback_slot = get_integer_data(data[i],"0"); i++; if(feedback_slot!=0) feedback_slot = 1;
            tuner_type = get_integer_data(data[i],"0"); i++; if(tuner_type!=0) tuner_type = 1;
            tr = get_double_data(data[i],"1.0"); i++;

            if(excitation_source==0)
                set_excitation_source(SELF_EXCITATION);
            else
                set_excitation_source(SEPARATE_EXCITATION);

            if(exciter_brush==0)
                set_exciter_brush(WITHOUT_BRUSH);
            else
                set_exciter_brush(WITH_BRUSH);

            if(feedback_slot==0)
                set_feedback_slot(AT_VOLTAGE_ERROR);
            else
                set_feedback_slot(AT_REGULATOR);

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
            kh = get_double_data(data[i],"0.0"); i++;
            kf = get_double_data(data[i],"0.0"); i++;
            tf = get_double_data(data[i],"0.0"); i++;
            kb = get_double_data(data[i],"0.0"); i++;
            t5 = get_double_data(data[i],"0.0"); i++;
            vrmax = get_double_data(data[i],"0.0"); i++;
            vrmin = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            vemax = get_double_data(data[i],"0.0"); i++;
            ke = get_double_data(data[i],"0.0"); i++;
            se75 = get_double_data(data[i],"0.0"); i++;
            se100 = get_double_data(data[i],"0.0"); i++;
            kc = get_double_data(data[i],"0.0"); i++;
            kd = get_double_data(data[i],"0.0"); i++;
            efdmax = get_double_data(data[i],"0.0"); i++;

            set_KA(ka);
            set_TA_in_s(ta);
            set_VAmax_in_pu(vamax);
            set_VAmin_in_pu(vamin);
            set_KH(kh);
            set_KF(kf);
            set_TF_in_s(tf);
            set_KB(kb);
            set_T5_in_s(t5);
            set_VRmax_in_pu(vrmax);
            set_VRmin_in_pu(vrmin);
            set_TE_in_s(te);
            set_Vemax_in_pu(vemax);
            set_KE(ke);
            set_SE75_in_pu(se75);
            set_SE100_in_pu(se100);
            set_KC(kc);
            set_KD(kd);
            set_Efdmax_in_pu(efdmax);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool CSEET1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool CSEET1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void CSEET1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    sensor.set_toolkit(toolkit);
    serial_tuner1_lead_lag.set_toolkit(toolkit);
    serial_tuner1_pi.set_toolkit(toolkit);
    serial_tuner2.set_toolkit(toolkit);
    parallel_tuner_integral.set_toolkit(toolkit);
    parallel_tuner_differential.set_toolkit(toolkit);
    regulator1.set_toolkit(toolkit);
    regulator2.set_toolkit(toolkit);
    feedbacker.set_toolkit(toolkit);
    exciter.set_toolkit(toolkit);
}

void CSEET1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        if(generator!=NULL)
        {
            STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
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
                        double VA1max = get_serial_tuner_VA1max_in_pu();
                        double VA1min = get_serial_tuner_VA1min_in_pu();

                        serial_tuner1_pi.set_Kp(K*T1/T2);
                        serial_tuner1_pi.set_Ki(K/T2);
                        serial_tuner1_pi.set_upper_limit(VA1max);
                        serial_tuner1_pi.set_lower_limit(VA1min);
                    }
                }

                double Efd = get_initial_excitation_voltage_in_pu_from_sync_generator_model();
                if(Efd>get_Efdmax_in_pu())
                {
                    osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Efd is "<<Efd<<", and Efdmax is "<<get_Efdmax_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();
                double Vt = psdb.get_bus_voltage_in_pu(generator->get_generator_bus());

                double Ve = get_initial_Ve_with_Fex_function();
                if(Ve>get_Vemax_in_pu())
                {
                    osstream<<"Initialization error. Output of exciter of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Ve is "<<Ve<<", and Vemax is "<<get_Vemax_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                exciter.set_output(Ve);
                exciter.initialize();

                double SE = saturation_block.get_saturation(Ve);

                double KE = get_KE();
                double KD = get_KD();

                double Vfe = (SE+KE)*Ve+Ifd*KD;
                double Vr = Vfe;
                regulator2.set_output(Vr);
                regulator2.initialize();

                if(get_excitation_source()==SEPARATE_EXCITATION)
                {
                    if(Vr>get_VRmax_in_pu())
                    {
                        osstream<<"Initialization error. Output of regulator 2 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                          <<"Vr is "<<Vr<<", and VRmax is "<<get_VRmax_in_pu()<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    if(Vr<get_VRmin_in_pu())
                    {
                        osstream<<"Initialization error. Output of regulator 2 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                          <<"Vr is "<<Vr<<", and VRmin is "<<get_VRmin_in_pu()<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(Vr>get_VRmax_in_pu()*Vt)
                    {
                        osstream<<"Initialization error. Output of regulator 2 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                          <<"Vr is "<<Vr<<", and VRmax*Vt is "<<get_VRmax_in_pu()*Vt<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    if(Vr<get_VRmin_in_pu()*Vt)
                    {
                        osstream<<"Initialization error. Output of regulator 2 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                          <<"Vr is "<<Vr<<", and VRmin*Vt is "<<get_VRmin_in_pu()*Vt<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }

                double V_input_to_feedbacker = 0.0;

                if(get_exciter_brush()==WITHOUT_BRUSH)
                    V_input_to_feedbacker = Vfe;
                else
                    V_input_to_feedbacker = Efd;

                feedbacker.set_input(V_input_to_feedbacker);
                feedbacker.initialize();

                double KH = get_KH();
                double V_regulator1 = regulator2.get_input()+KH*V_input_to_feedbacker;
                if(V_regulator1>get_VAmax_in_pu())
                {
                    osstream<<"Initialization error. Output of regulator 1 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Va is "<<V_regulator1<<", and VAmax is "<<get_VAmax_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(V_regulator1<get_VAmin_in_pu())
                {
                    osstream<<"Initialization error. Output of regulator 1 of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Va is "<<V_regulator1<<", and VAmin is "<<get_VAmin_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                regulator1.set_output(V_regulator1);
                regulator1.initialize();

                double V_input_to_tuner;
                if(get_tuner_type()==SERIAL_TUNER)
                {
                    serial_tuner2.set_output(regulator1.get_input());
                    serial_tuner2.initialize();


                    if(get_serial_tuner_KV()==true)
                    {
                        serial_tuner1_lead_lag.set_output(serial_tuner2.get_input());
                        serial_tuner1_lead_lag.initialize();
                        V_input_to_tuner = serial_tuner1_lead_lag.get_input();
                    }
                    else
                    {
                        serial_tuner1_pi.set_output(serial_tuner2.get_input());
                        serial_tuner1_pi.initialize();
                        V_input_to_tuner = serial_tuner1_pi.get_input();
                    }
                }
                else
                {
                    parallel_tuner_integral.set_output(regulator1.get_input());
                    parallel_tuner_integral.initialize();

                    V_input_to_tuner = 0.0;
                    parallel_tuner_differential.set_input(V_input_to_tuner);
                    parallel_tuner_differential.initialize();
                }

                double Ecomp = get_compensated_voltage_in_pu();

                sensor.set_output(Ecomp);
                sensor.initialize();

                double Vref = V_input_to_tuner+Ecomp;
                set_voltage_reference_in_pu(Vref);

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void CSEET1::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            double Ecomp = get_compensated_voltage_in_pu();
            double Vref = get_voltage_reference_in_pu();
            double Vs = get_stabilizing_signal_in_pu();
            double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();

            double Ve = exciter.get_output();
            double Vemax = get_Vemax_in_pu();
            if(Ve>Vemax)
                Ve = Vemax;

            double Efd = get_excitation_voltage_in_pu();

            double SE = saturation_block.get_saturation(Ve);
            double KE = get_KE();
            double KD = get_KD();

            double Vfe = (SE+KE)*Ve+KD*Ifd;
            double V_to_feedback = Vfe;
            if(get_exciter_brush()==WITH_BRUSH)
                V_to_feedback = Efd;

            feedbacker.set_input(V_to_feedback);
            feedbacker.run(mode);

            sensor.set_input(Ecomp);
            sensor.run(mode);

            double input;
            input = Vref+Vs-sensor.get_output();

            if(get_feedback_slot()==AT_VOLTAGE_ERROR)
                input -= feedbacker.get_output();

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
            else
            {
                parallel_tuner_integral.set_input(input);
                parallel_tuner_integral.run(mode);
                parallel_tuner_differential.set_input(input);
                parallel_tuner_differential.run(mode);

                input = input*get_parallel_tuner_KP()+parallel_tuner_integral.get_output()+parallel_tuner_differential.get_output();
            }

            if(get_feedback_slot()==AT_REGULATOR)
                input -= feedbacker.get_output();

            regulator1.set_input(input);
            regulator1.run(mode);
            input = regulator1.get_output();

            double KH = get_KH();
            input = input-KH*V_to_feedback;

            regulator2.set_input(input);
            regulator2.run(mode);

            double Vr = regulator2.get_output();
            double Vrmax = get_VRmax_in_pu();
            double Vrmin = get_VRmin_in_pu();

            STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            size_t bus = generator->get_generator_bus();
            double Vt = psdb.get_bus_voltage_in_pu(bus);

            if(get_excitation_source()==SELF_EXCITATION)
            {
                Vrmax *= Vt;
                Vrmin *= Vt;
            }

            if(Vr>Vrmax)
                Vr = Vrmax;

            if(Vr<Vrmin)
                Vr = Vrmin;

            input = Vr-Vfe;
            exciter.set_input(input);
            exciter.run(mode);

            //cout<<"Ecomp="<<Ecomp<<", Vref="<<Vref<<", Vs="<<Vs<<", Efd="<<exciter.get_output()<<endl;

            if(mode == UPDATE_MODE)
                set_flag_model_updated_as_true();
        }
    }
}

double CSEET1::get_excitation_voltage_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            double Ifd = gen_model->get_field_current_in_pu_based_on_mbase();

            double Ve = exciter.get_output();
            double Vemax = get_Vemax_in_pu();
            if(Ve>Vemax)
                Ve = Vemax;

            double Fex = get_Fex(Ve, Ifd);
            double Efd = Ve*Fex;
            double Efdmax = get_Efdmax_in_pu();
            if(Efd>Efdmax)
                Efd = Efdmax;

            return Efd;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

void CSEET1::check()
{
    ;
}

void CSEET1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void CSEET1::save()
{
    ;
}

string CSEET1::get_standard_psse_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    size_t source_index = 0;
    switch(get_excitation_source())
    {
        case SELF_EXCITATION:
            source_index = 0;
            break;
        default:
            source_index = 1;
            break;
    }
    size_t brush_index = 0;
    switch(get_exciter_brush())
    {
        case WITHOUT_BRUSH:
            brush_index = 0;
            break;
        default:
            brush_index = 1;
            break;
    }
    size_t feedback_index = 0;
    switch(get_feedback_slot())
    {
        case AT_VOLTAGE_ERROR:
            feedback_index = 0;
            break;
        default:
            feedback_index = 1;
            break;
    }
    size_t tuner_type = 0;
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

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<source_index<<", "
            <<setw(8)<<brush_index<<", "
            <<setw(8)<<feedback_index<<", "
            <<setw(8)<<tuner_type<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<TR<<", ";

    bool tuner_KV; size_t tuner_selector; double tuner_K, tuner_T1, tuner_T2, tuner_VA1max, tuner_VA1min, tuner_T3, tuner_T4;
    double tuner_KP, tuner_KI, tuner_VImax, tuner_VImin, tuner_KD, tuner_TD, tuner_VDmax, tuner_VDmin;

    if(get_tuner_type()==SERIAL_TUNER)
    {
        tuner_KV = get_serial_tuner_KV();
        tuner_selector = 0;
        if(tuner_KV) tuner_selector = 1;
        tuner_K = get_serial_tuner_K();
        tuner_T1 = get_serial_tuner_T1_in_s();
        tuner_T2 = get_serial_tuner_T2_in_s();
        tuner_VA1max = get_serial_tuner_VA1max_in_pu();
        tuner_VA1min = get_serial_tuner_VA1min_in_pu();
        tuner_T3 = get_serial_tuner_T3_in_s();
        tuner_T4 = get_serial_tuner_T4_in_s();

        osstream<<setw(8)<<setprecision(6)<<tuner_K<<", "
                <<setw(8)<<tuner_selector<<", "
                <<setw(8)<<setprecision(6)<<tuner_T1<<", "
                <<setw(8)<<setprecision(6)<<tuner_T2<<", "
                <<setw(8)<<setprecision(6)<<tuner_VA1max<<", "
                <<setw(8)<<setprecision(6)<<tuner_VA1min<<", "
                <<setw(8)<<setprecision(6)<<tuner_T3<<", "
                <<setw(8)<<setprecision(6)<<tuner_T4<<", \n"
                <<setw(10)<<"";
    }
    else
    {
        tuner_KP = get_parallel_tuner_KP();
        tuner_KI = get_parallel_tuner_KI();
        tuner_VImax = get_parallel_tuner_VImax_in_pu();
        tuner_VImin = get_parallel_tuner_VImin_in_pu();
        tuner_KD = get_parallel_tuner_KD();
        tuner_TD = get_parallel_tuner_TD_in_s();
        tuner_VDmax = get_parallel_tuner_VDmax_in_pu();
        tuner_VDmin = get_parallel_tuner_VDmin_in_pu();

        osstream<<setw(8)<<setprecision(6)<<tuner_KP<<", "
                <<setw(8)<<setprecision(6)<<tuner_KI<<", "
                <<setw(8)<<setprecision(6)<<tuner_VImax<<", "
                <<setw(8)<<setprecision(6)<<tuner_VImin<<", "
                <<setw(8)<<setprecision(6)<<tuner_KD<<", "
                <<setw(8)<<setprecision(6)<<tuner_TD<<", "
                <<setw(8)<<setprecision(6)<<tuner_VDmax<<", "
                <<setw(8)<<setprecision(6)<<tuner_VDmin<<", \n"
                <<setw(10)<<"";
    }

    double KA = get_KA();
    double TA = get_TA_in_s();
    double VAmax = get_VAmax_in_pu();
    double VAmin = get_VAmin_in_pu();
    double KH = get_KH();
    double KF = get_KF();
    double TF = get_TF_in_s();
    double KB = get_KB();
    double T5 = get_T5_in_s();
    double VRmax = get_VRmax_in_pu();
    double VRmin = get_VRmin_in_pu();
    double KE = get_KE();
    double TE = get_TE_in_s();
    double Vemax = get_Vemax_in_pu();
    double SE75 = get_SE75_in_pu();
    double SE100 = get_SE100_in_pu();
    double KC = get_KC();
    double KD = get_KD();
    double Efdmax = get_Efdmax_in_pu();


    osstream<<setw(8)<<setprecision(6)<<KA<<", "
            <<setw(8)<<setprecision(6)<<TA<<", "
            <<setw(8)<<setprecision(6)<<VAmax<<", "
            <<setw(8)<<setprecision(6)<<VAmin<<", "
            <<setw(8)<<setprecision(6)<<KH<<", "
            <<setw(8)<<setprecision(6)<<KF<<", "
            <<setw(8)<<setprecision(6)<<TF<<", "
            <<setw(8)<<setprecision(6)<<KB<<", "
            <<setw(8)<<setprecision(6)<<T5<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<VRmax<<", "
            <<setw(8)<<setprecision(6)<<VRmin<<", "
            <<setw(8)<<setprecision(6)<<TE<<", "
            <<setw(8)<<setprecision(6)<<Vemax<<", "
            <<setw(8)<<setprecision(6)<<KE<<", "
            <<setw(8)<<SE75<<", "
            <<setw(8)<<SE100<<", "
            <<setw(8)<<setprecision(6)<<KC<<", "
            <<setw(8)<<setprecision(6)<<KD<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<Efdmax<<" /";

    return osstream.str();
}

void CSEET1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double CSEET1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void CSEET1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void CSEET1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TUNER1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TUNER2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@EXCITER", i); i++;
}

double CSEET1::get_model_internal_variable_with_name(string var_name)
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
            return parallel_tuner_integral.get_state();
    }

    if(var_name == "STATE@TUNER2")
    {
        if(get_tuner_type()==SERIAL_TUNER)
            return serial_tuner2.get_state();
        else
            return parallel_tuner_differential.get_state();
    }

    if(var_name == "STATE@REGULATOR1")
        return regulator1.get_state();

    if(var_name == "STATE@REGULATOR2")
        return regulator2.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    if(var_name == "STATE@EXCITER")
        return exciter.get_state();

    return 0.0;
}



string CSEET1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string CSEET1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string CSEET1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
