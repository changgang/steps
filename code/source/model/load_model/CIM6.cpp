#include "header/model/load_model/CIM6.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <iostream>

class STEPS_SPARSE_MATRIX;
using namespace std;
CIM6::CIM6(STEPS& toolkit) : LOAD_MODEL(toolkit),
                             speed_block(toolkit),
                             transient_block_x_axis(toolkit),
                             subtransient_block_x_axis(toolkit),
                             transient_block_y_axis(toolkit),
                             subtransient_block_y_axis(toolkit)
{
    clear();
}

CIM6::CIM6(const CIM6& model) : LOAD_MODEL(model.get_toolkit()),
                                speed_block(model.get_toolkit()),
                                transient_block_x_axis(model.get_toolkit()),
                                subtransient_block_x_axis(model.get_toolkit()),
                                transient_block_y_axis(model.get_toolkit()),
                                subtransient_block_y_axis(model.get_toolkit())
{
    copy_from_const_model(model);
}

CIM6& CIM6::operator=(const CIM6& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void CIM6::copy_from_const_model(const CIM6& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    speed_block.set_toolkit(toolkit);
    transient_block_x_axis.set_toolkit(toolkit);
    subtransient_block_x_axis.set_toolkit(toolkit);
    transient_block_y_axis.set_toolkit(toolkit);
    subtransient_block_y_axis.set_toolkit(toolkit);

    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->set_subsystem_type(model.get_subsystem_type());

    this->set_motor_type(model.get_motor_type());
    this->set_Ra_in_pu(model.get_Ra_in_pu());
    this->set_Xa_in_pu(model.get_Xa_in_pu());
    this->set_Xm_in_pu(model.get_Xm_in_pu());
    this->set_R1_in_pu(model.get_R1_in_pu());
    this->set_X1_in_pu(model.get_X1_in_pu());
    this->set_R2_in_pu(model.get_R2_in_pu());
    this->set_X2_in_pu(model.get_X2_in_pu());
    this->set_E1_in_pu(model.get_E1_in_pu());
    this->set_SE1_in_pu(model.get_SE1_in_pu());
    this->set_E2_in_pu(model.get_E2_in_pu());
    this->set_SE2_in_pu(model.get_SE2_in_pu());
    this->set_Mbase_in_MVA(model.get_Mbase_in_MVA());
    this->set_Pmult(model.get_Pmult());
    this->set_H_in_s(model.get_H_in_s());
    this->set_VI_in_pu(model.get_VI_in_pu());
    this->set_TI_in_cycles(model.get_TI_in_cycles());
    this->set_TB_in_cycles(model.get_TB_in_cycles());
    this->set_A(model.get_A());
    this->set_B(model.get_B());
    this->set_C(model.get_C());
    this->set_D(model.get_D());
    this->set_E(model.get_E());
    this->set_Tnom_in_pu(model.get_Tnom_in_pu());
}

CIM6::~CIM6()
{
}

void CIM6::clear()
{
    set_voltage_source_flag(true);

    set_model_float_parameter_count(23);

    saturation.set_saturation_type(QUADRATIC_SATURATION_TYPE);
    set_Mbase_in_MVA(0.0);
}

string CIM6::get_model_name() const
{
    return "CIM6";
}

void CIM6::set_motor_type(unsigned int t)
{
    if(t==1)
        Motor_type = 1;
    else
        Motor_type = 2;
}

void CIM6::set_Ra_in_pu(double r)
{
    Ra = r;
}

void CIM6::set_Xa_in_pu(double x)
{
    Xa = x;
}

void CIM6::set_Xm_in_pu(double x)
{
    Xm = x;
}

void CIM6::set_R1_in_pu(double r)
{
    R1 = r;
}

void CIM6::set_X1_in_pu(double x)
{
    X1 = x;
}

void CIM6::set_R2_in_pu(double r)
{
    R2 = r;
}

void CIM6::set_X2_in_pu(double x)
{
    X2 = x;
}

void CIM6::set_E1_in_pu(double e)
{
    saturation.set_V1(e);
}

void CIM6::set_SE1_in_pu(double s)
{
    saturation.set_S1(s);
}

void CIM6::set_E2_in_pu(double e)
{
    saturation.set_V2(e);
}

void CIM6::set_SE2_in_pu(double s)
{
    saturation.set_S2(s);
}

void CIM6::set_Mbase_in_MVA(double mbase)
{
    Mbase = mbase;
}

void CIM6::set_Pmult(double pmult)
{
    Pmult = pmult;
}

void CIM6::set_H_in_s(double h)
{
    H = h;
}

void CIM6::set_VI_in_pu(double v)
{
    VI = v;
}

void CIM6::set_TI_in_cycles(double t)
{
    TI = t;
}

void CIM6::set_TB_in_cycles(double t)
{
    TB = t;
}

void CIM6::set_A(double a)
{
    A = a;
}

void CIM6::set_B(double b)
{
    B = b;
}

void CIM6::set_C(double c)
{
    C = c;
}

void CIM6::set_D(double d)
{
    D = d;
}

void CIM6::set_E(double e)
{
    E = e;
}

void CIM6::set_Tnom_in_pu(double t)
{
    Tnominal = t;
}

unsigned int CIM6::get_motor_type() const
{
    return Motor_type;
}

double CIM6::get_Ra_in_pu() const
{
    return Ra;
}

double CIM6::get_Xa_in_pu() const
{
    return Xa;
}

double CIM6::get_Xm_in_pu() const
{
    return Xm;
}

double CIM6::get_R1_in_pu() const
{
    return R1;
}

double CIM6::get_X1_in_pu() const
{
    return X1;
}

double CIM6::get_R2_in_pu() const
{
    return R2;
}

double CIM6::get_X2_in_pu() const
{
    return X2;
}

double CIM6::get_E1_in_pu() const
{
    return saturation.get_V1();
}

double CIM6::get_SE1_in_pu() const
{
    return saturation.get_S1();
}

double CIM6::get_E2_in_pu() const
{
    return saturation.get_V2();
}

double CIM6::get_SE2_in_pu() const
{
    return saturation.get_S2();
}

double CIM6::get_Mbase_in_MVA() const
{
    return Mbase;
}

double CIM6::get_Pmult() const
{
    return Pmult;
}

double CIM6::get_H_in_s() const
{
    return H;
}

double CIM6::get_VI_in_pu() const
{
    return VI;
}

double CIM6::get_TI_in_cycles() const
{
    return TI;
}

double CIM6::get_TB_in_cycles() const
{
    return TB;
}

double CIM6::get_A() const
{
    return A;
}

double CIM6::get_B() const
{
    return B;
}

double CIM6::get_C() const
{
    return C;
}

double CIM6::get_D() const
{
    return D;
}

double CIM6::get_E() const
{
    return E;
}

double CIM6::get_Tnom_in_pu() const
{
    return Tnominal;
}

double CIM6::get_slip_in_pu()
{
    return speed_block.get_output()-get_bus_frequency_deviation_in_pu();
}

double CIM6::get_slip_in_Hz()
{
    return get_slip_in_pu()*W0;
}

bool CIM6::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=27)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name=="CIM6AL" or model_name=="CIM6BL" or model_name=="CIM6AR" or model_name=="CIM6ZN" or model_name=="CIM6OW")
        {
            unsigned int motor_type;
            double ra, xa, xm, r1, x1, r2, x2, e1, se1, e2, se2, mbase, pmult, h, vi, ti, tb, a, b, c, d, e, tnom;

            unsigned int i=3;
            motor_type = get_integer_data(data[i],"1"); i++;
            ra = get_double_data(data[i],"0.0"); i++;
            xa = get_double_data(data[i],"0.0"); i++;
            xm = get_double_data(data[i],"0.0"); i++;
            r1 = get_double_data(data[i],"0.0"); i++;
            x1 = get_double_data(data[i],"0.0"); i++;
            r2 = get_double_data(data[i],"0.0"); i++;
            x2 = get_double_data(data[i],"0.0"); i++;
            e1 = get_double_data(data[i],"0.0"); i++;
            se1 = get_double_data(data[i],"0.0"); i++;
            e2 = get_double_data(data[i],"0.0"); i++;
            se2 = get_double_data(data[i],"0.0"); i++;
            mbase = get_double_data(data[i],"0.0"); i++;
            pmult = get_double_data(data[i],"0.0"); i++;
            h = get_double_data(data[i],"0.0"); i++;
            vi = get_double_data(data[i],"0.0"); i++;
            ti = get_double_data(data[i],"0.0"); i++;
            tb = get_double_data(data[i],"0.0"); i++;
            a = get_double_data(data[i],"0.0"); i++;
            b = get_double_data(data[i],"0.0"); i++;
            d = get_double_data(data[i],"0.0"); i++;
            e = get_double_data(data[i],"0.0"); i++;
            c = get_double_data(data[i],"0.0"); i++;
            tnom = get_double_data(data[i],"0.0"); i++;


            set_motor_type(motor_type);
            set_Ra_in_pu(ra);
            set_Xa_in_pu(xa);
            set_Xm_in_pu(xm);
            set_R1_in_pu(r1);
            set_X1_in_pu(x1);
            set_R2_in_pu(r2);
            set_X2_in_pu(x2);
            set_E1_in_pu(e1);
            set_SE1_in_pu(se1);
            set_E2_in_pu(e2);
            set_SE2_in_pu(se2);
            set_Mbase_in_MVA(mbase);
            set_Pmult(pmult);
            set_H_in_s(h);
            set_VI_in_pu(vi);
            set_TI_in_cycles(ti);
            set_TB_in_cycles(tb);
            set_A(a);
            set_B(b);
            set_C(c);
            set_D(d);
            set_E(e);
            set_Tnom_in_pu(tnom);

            if(model_name=="CIM6AL")
                set_subsystem_type(ALL_SYSTEM_TYPE);
            else
            {
                if(model_name=="CIM6AR")
                    set_subsystem_type(AREA_SUBSYSTEM_TYPE);
                else
                {
                    if(model_name=="CIM6ZN")
                        set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
                    else
                    {
                        //CIM6BL
                        set_subsystem_type(BUS_SUBSYSTEM_TYPE);
                    }
                }
            }
            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool CIM6::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool CIM6::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

complex<double> CIM6::get_dynamic_source_admittance_in_pu_based_on_SBASE()
{
    complex<double> y = 1.0/Zsource;
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = Mbase;
    return y*mbase*one_over_sbase;
}

complex<double> CIM6::get_additional_admittance_in_pu_based_on_SBASE()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return complex<double>(0.0, nominal_shunt_in_MVar*one_over_sbase);
}

void CIM6::setup_block_toolkit_and_parameters()
{
    setup_model_dynamic_parameters();

    saturation.set_saturation_type(QUADRATIC_SATURATION_TYPE);

    transient_block_x_axis.set_T_in_s(Tp);
    transient_block_y_axis.set_T_in_s(Tp);
    if(not is_single_cage)
    {
        subtransient_block_x_axis.set_T_in_s(Tpp);
        subtransient_block_y_axis.set_T_in_s(Tpp);
    }
    speed_block.set_T_in_s(2.0*get_H_in_s());
}

void CIM6::setup_model_dynamic_parameters()
{
    if(get_R2_in_pu()==0.0 or get_X2_in_pu()==0.0)
        is_single_cage = true;
    else
        is_single_cage = false;

    if(Mbase == 0.0)
    {
        LOAD* load = get_load_pointer();
        complex<double> S = load->get_actual_total_load_in_MVA();
        double P = S.real();

        Mbase = P*Pmult;
    }

    W0 = PI2 * get_bus_base_frequency_in_Hz();
    if(is_single_cage)
    {
        Tp = (Xm+X1)/(W0*R1);
        Xs = Xa+Xm;
        Xp = Xa+(Xm*X1)/(Xm+X1);
        Xleakage = Xa;
        Xs_minum_Xp = Xs-Xp;
        Xp_minum_Xleakage = Xp-Xleakage;
    }
    else
    {
        if(Motor_type==1)
        {
            Tp = (Xm+X1)/(W0*R1);
            Xs = Xa+Xm;
            double Xm_X1_parallel = Xm*X1/(Xm+X1);
            Xp = Xa + Xm_X1_parallel;
            Xleakage = Xa;
            Tpp = (X2+Xm_X1_parallel)/(W0*R2);
            double Xm_X1_X2_parallel = Xm_X1_parallel*X2/(Xm_X1_parallel+X2);
            Xpp = Xa + Xm_X1_X2_parallel;

            Xs_minum_Xp = Xs-Xp;
            Xp_minum_Xleakage = Xp-Xleakage;
            Xp_minum_Xpp = Xp-Xpp;
            Xp_minum_Xpp_over_Xp_minum_Xleakage = Xp_minum_Xpp/Xp_minum_Xleakage;
            Xpp_minum_Xleakage_over_Xp_minum_Xleakage = 1.0-Xp_minum_Xpp_over_Xp_minum_Xleakage;
            Xp_minum_Xpp_over_Xp_minum_Xleakage_square = Xp_minum_Xpp_over_Xp_minum_Xleakage/Xp_minum_Xleakage;
        }
        else
        {
            Tp = (Xm+X1+X2)/(W0*R2);
            Xs = Xa+Xm;
            double Xm_X1_plus_X2_parallel = Xm*(X1+X2)/(Xm+X1+X2);
            Xp = Xa + Xm_X1_plus_X2_parallel;
            double Xm_plus_X1_X2_parallel = (Xm+X1)*X2/(Xm+X1+X2);
            Tpp = Xm_plus_X1_X2_parallel/(W0*R1);
            double Xm_X1_parallel = Xm*X1/(Xm+X1);
            Xpp = Xa + Xm_X1_parallel;
            Xleakage = Xpp;

            Xs_minum_Xp = Xs-Xp;
            Xp_minum_Xleakage = Xp-Xleakage;
            Xp_minum_Xpp = Xp-Xpp;
            Xp_minum_Xpp_over_Xp_minum_Xleakage = Xp_minum_Xpp/Xp_minum_Xleakage;
            Xpp_minum_Xleakage_over_Xp_minum_Xleakage = 1.0-Xp_minum_Xpp_over_Xp_minum_Xleakage;
            Xp_minum_Xpp_over_Xp_minum_Xleakage_square = Xp_minum_Xpp_over_Xp_minum_Xleakage/Xp_minum_Xleakage;
        }
    }

    if(is_single_cage)
        Zsource = complex<double>(Ra, Xp);
    else
        Zsource = complex<double>(Ra, Xpp);

    cout<<"Xs = "<<Xs<<", Xp = "<<Xp<<", Xpp = "<<Xpp<<", Xl = "<<Xleakage<<", Tp = "<<Tp<<", Tpp = "<<Tpp<<endl;
}

void CIM6::initialize()
{
    synchronize_bus_voltage_and_frequency();

    STEPS& toolkit = get_toolkit();

    setup_block_toolkit_and_parameters();

    LOAD* load = get_load_pointer();

    // treat model as generating power
    complex<double> S = -load->get_actual_total_load_in_MVA()/Mbase;
    double Pload0 = S.real(), Qload0 = S.imag();
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    double Vr = V.real(), Vi = V.imag();

    complex<double> I = conj(S/V); // guess
    complex<double> Esource = V+I*Zsource;

    double R = Zsource.real(), X = Zsource.imag();
    complex<double> Ep, Ek;
    unsigned int n_iteration = 0;
    if(is_single_cage)
    {
        // five vars to solve
        double Erp = Esource.real(), Eip = Esource.imag();
        double Ir = I.real(), Ii = I.imag();
        double slip = 0.0;

        vector<double> Error;
        Error.reserve(5);
        while(true)
        {
            ++n_iteration;

            Error.clear();
            Error.push_back(-Erp+Xs_minum_Xp*Ii-Eip*Tp*slip); // x-axis dEr'/dt=0
            Error.push_back(-Eip-Xs_minum_Xp*Ir+Erp*Tp*slip); // y-axis dEi'/dt=0
            Error.push_back((Erp-Vr)*R+(Eip-Vi)*X-(R*R+X*X)*Ir); // x-axis current Ir
            Error.push_back((Eip-Vi)*R-(Erp-Vr)*X-(R*R+X*X)*Ii); // y-axis current Ii
            Error.push_back(Erp*Ir+Eip*Ii-R*(Ir*Ir+Ii*Ii)-Pload0); // active power balance

            double maxerror = 0.0;
            for(unsigned int i=0; i<5; ++i)
            {
                if(maxerror<fabs(Error[i]))
                    maxerror = fabs(Error[i]);
            }
            if(maxerror<FLOAT_EPSILON or n_iteration>20)
            {
                double speed = slip/W0;
                speed_block.set_output(speed);
                speed_block.initialize();

                Esource = complex<double>(Erp, Eip);
                I = complex<double>(Ir, Ii);
                complex<double> Smotor = Esource*conj(I);
                double pelec = Smotor.real();
                speed = 1.0+speed;
                double temp = get_A()*speed*speed +
                              get_B()*speed +
                              get_C() +
                              get_D()*pow(speed, get_E());
                initial_load_torque = -pelec/(speed*temp);

                complex<double> S = V*conj(I);
                double Qshunt = Qload0-S.imag();
                nominal_shunt_in_MVar = Qshunt/(abs(V)*abs(V))*Mbase;

                Ep = Esource;

                if(n_iteration>20)
                {
                    ostringstream osstream;
                    osstream<<"Warning. Fail to initialize CIM6 model of "<<get_device_name()<<" in 20 iterations.\n"
                            <<"Max error is "<<maxerror<<" with the best estimation:\n"
                            <<"(1) Er\' ="<<Erp<<"\n"
                            <<"(2) Ei\' ="<<Eip<<"\n"
                            <<"(3) Ir  ="<<Ir<<"\n"
                            <<"(4) Ii  ="<<Ii<<"\n"
                            <<"(5) slip="<<slip/W0<<"\n"
                            <<"(6) T0  ="<<initial_load_torque<<"\n"
                            <<"(7) Q0  ="<<nominal_shunt_in_MVar<<" MVar";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                break;
            }
            STEPS_SPARSE_MATRIX J;

            J.add_entry(0,0, -1);
            J.add_entry(0,1, -Tp*slip);
            J.add_entry(0,3, Xs_minum_Xp);
            J.add_entry(0,4, -Eip*Tp);

            J.add_entry(1,0, Tp*slip);
            J.add_entry(1,1, -1);
            J.add_entry(1,2, -Xs_minum_Xp);
            J.add_entry(1,4, Erp*Tp);

            J.add_entry(2,0, R);
            J.add_entry(2,1, X);
            J.add_entry(2,2, -(R*R+X*X));

            J.add_entry(3,0, -X);
            J.add_entry(3,1, R);
            J.add_entry(3,3, -(R*R+X*X));

            J.add_entry(4,0, Ir);
            J.add_entry(4,1, Ii);
            J.add_entry(4,2, Erp-2*R*Ir);
            J.add_entry(4,3, Eip-2*R*Ii);

            J.compress_and_merge_duplicate_entries();

            vector<double> update = Error/J;

            Erp -= update[0];
            Eip -= update[1];
            Ir  -= update[2];
            Ii  -= update[3];
            slip-= update[4];
        }
    }
    else
    {
        // nine vars to solve
        double Erpp = Esource.real(), Eipp = Esource.imag();
        double Erk = Erpp, Eik = Eipp;
        double Erp = Erpp, Eip = Eipp;
        double Ir = I.real(), Ii = I.imag();
        double slip = 0.0;

        double EppMag = 0.0;
        double Sat = 0.0;
        vector<double> Error;
        Error.reserve(9);
        while(true)
        {
            ++n_iteration;

            EppMag = abs(Esource);
            Sat = saturation.get_saturation(EppMag);

            Error.clear();
            Error.push_back(-Erpp+Xp_minum_Xpp_over_Xp_minum_Xleakage*Erk+Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Erp); // x-axis Er"=xxx
            Error.push_back(-Eipp+Xp_minum_Xpp_over_Xp_minum_Xleakage*Eik+Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Eip); // y-axis Ei"=xxx
            Error.push_back(-Erk-Eik*Tpp*slip+Erp+Xp_minum_Xleakage*Ii); // x-axis dErk/dt=0
            Error.push_back(Erk*Tpp*slip-Eik+Eip-Xp_minum_Xleakage*Ir); // x-axis dErk/dt=0
            Error.push_back(Sat/EppMag*Eipp+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp*Erk-
                            (1.0+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp)*Erp-
                            Eip*Tp*slip+
                            Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Xs_minum_Xp*Ii); // x-axis dEr'/dt=0
            Error.push_back(-Sat/EppMag*Erpp+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp*Eik+
                            Erp*Tp*slip-
                            (1.0+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp)*Eip-
                            Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Xs_minum_Xp*Ir); // x-axis dEr'/dt=0
            Error.push_back((Erpp-Vr)*R+(Eipp-Vi)*X-(R*R+X*X)*Ir); // x-axis current Ir
            Error.push_back((Eipp-Vi)*R-(Erpp-Vr)*X-(R*R+X*X)*Ii); // y-axis current Ii
            Error.push_back(Erpp*Ir+Eipp*Ii-R*(Ir*Ir+Ii*Ii)-Pload0); // active power balance

            double maxerror = 0.0;
            for(unsigned int i=0; i<9; ++i)
            {
                if(maxerror<fabs(Error[i]))
                    maxerror = fabs(Error[i]);
            }
            if(maxerror<FLOAT_EPSILON or n_iteration>20)
            {
                double speed = slip/W0;
                speed_block.set_output(speed);
                speed_block.initialize();

                Esource = complex<double>(Erpp, Eipp);
                I = complex<double>(Ir, Ii);
                complex<double> Smotor = Esource*conj(I);
                double pelec = Smotor.real();
                speed = 1.0+speed;
                double temp = get_A()*speed*speed +
                              get_B()*speed +
                              get_C() +
                              get_D()*pow(speed, get_E());
                initial_load_torque = -pelec/(speed*temp);

                complex<double> S = V*conj(I);
                double Qshunt = Qload0-S.imag();
                nominal_shunt_in_MVar = Qshunt/(abs(V)*abs(V))*Mbase;

                Ek = complex<double>(Erk, Eik);
                Ep = complex<double>(Erp, Eip);

                if(n_iteration>20)
                {
                    ostringstream osstream;
                    osstream<<"Warning. Fail to initialize CIM6 model of "<<get_device_name()<<" in 20 iterations.\n"
                            <<"Max error is "<<maxerror<<" with the best estimation:\n"
                            <<"(1) Er\" ="<<Erpp<<"\n"
                            <<"(2) Ei\" ="<<Eipp<<"\n"
                            <<"(3) Erk ="<<Erk<<"\n"
                            <<"(4) Eik ="<<Eik<<"\n"
                            <<"(5) Er\' ="<<Erp<<"\n"
                            <<"(6) Ei\' ="<<Eip<<"\n"
                            <<"(7) Ir  ="<<Ir<<"\n"
                            <<"(8) Ii  ="<<Ii<<"\n"
                            <<"(9) slip="<<slip/W0<<"\n"
                            <<"(10) T0  ="<<initial_load_torque<<"\n"
                            <<"(11) Q0  ="<<nominal_shunt_in_MVar<<" MVar";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                break;
            }
            STEPS_SPARSE_MATRIX J;

            J.add_entry(0,0, -1);
            J.add_entry(0,2, Xp_minum_Xpp_over_Xp_minum_Xleakage);
            J.add_entry(0,4, Xpp_minum_Xleakage_over_Xp_minum_Xleakage);

            J.add_entry(1,1, -1);
            J.add_entry(1,3, Xp_minum_Xpp_over_Xp_minum_Xleakage);
            J.add_entry(1,5, Xpp_minum_Xleakage_over_Xp_minum_Xleakage);

            J.add_entry(2,2, -1);
            J.add_entry(2,3, -Tpp*slip);
            J.add_entry(2,4, 1);
            J.add_entry(2,7, Xp_minum_Xleakage);
            J.add_entry(2,8, -Eik*Tpp);

            J.add_entry(3,2, Tpp*slip);
            J.add_entry(3,3, -1);
            J.add_entry(3,5, 1);
            J.add_entry(3,6, -Xp_minum_Xleakage);
            J.add_entry(3,8, Erk*Tpp);

            J.add_entry(4,1, Sat/EppMag);
            J.add_entry(4,2, Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp);
            J.add_entry(4,4, -(1+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp));
            J.add_entry(4,5, -Tp*slip);
            J.add_entry(4,7, Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Xs_minum_Xp);
            J.add_entry(4,8, -Eip*Tp);

            J.add_entry(5,0, -Sat/EppMag);
            J.add_entry(5,3, Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp);
            J.add_entry(5,4, Tp*slip);
            J.add_entry(5,5, -(1+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*Xs_minum_Xp));
            J.add_entry(5,6, -Xpp_minum_Xleakage_over_Xp_minum_Xleakage*Xs_minum_Xp);
            J.add_entry(5,8, Erp*Tp);

            J.add_entry(6,0, R);
            J.add_entry(6,1, X);
            J.add_entry(6,6, -(R*R+X*X));

            J.add_entry(7,0, -X);
            J.add_entry(7,1, R);
            J.add_entry(7,7, -(R*R+X*X));

            J.add_entry(8,0, Ir);
            J.add_entry(8,1, Ii);
            J.add_entry(8,6, Erpp-2*R*Ir);
            J.add_entry(8,7, Eipp-2*R*Ii);

            J.compress_and_merge_duplicate_entries();

            vector<double> update = Error/J;

            Erpp -= update[0];
            Eipp -= update[1];
            Erk  -= update[2];
            Eik  -= update[3];
            Erp  -= update[4];
            Eip  -= update[5];
            Ir   -= update[6];
            Ii   -= update[7];
            slip -= update[8];
        }

    }

    transient_block_x_axis.set_output(Ep.real());
    transient_block_x_axis.initialize();
    transient_block_y_axis.set_output(Ep.imag());
    transient_block_y_axis.initialize();
    subtransient_block_x_axis.set_output(Ek.real());
    subtransient_block_x_axis.initialize();
    subtransient_block_y_axis.set_output(Ek.imag());
    subtransient_block_y_axis.initialize();

    set_flag_model_initialized_as_true();
}

void CIM6::initialize_to_start()
{
    if(not is_model_initialized())
    {
        synchronize_bus_voltage_and_frequency();

        setup_block_toolkit_and_parameters();

        transient_block_x_axis.set_output(0);
        transient_block_x_axis.initialize();
        transient_block_y_axis.set_output(0);
        transient_block_y_axis.initialize();

        subtransient_block_x_axis.set_output(0);
        subtransient_block_x_axis.initialize();
        subtransient_block_y_axis.set_output(0);
        subtransient_block_y_axis.initialize();

        speed_block.set_output(-1);
        speed_block.initialize();

        initial_load_torque = get_Tnom_in_pu();
        nominal_shunt_in_MVar = 0.0;

        set_flag_model_initialized_as_true();
    }
}

void CIM6::run(DYNAMIC_MODE mode)
{
    synchronize_bus_voltage_and_frequency();
    double Pelec = 0.0;
    double slip = get_slip_in_Hz();
    complex<double> Vterm = get_bus_positive_sequence_complex_voltage_in_pu();
    if(is_single_cage)
    {

        complex<double> Ep = get_internal_voltage_in_pu();
        complex<double> I = (Ep-Vterm)/Zsource;

        double Epr = transient_block_x_axis.get_output();
        double Epi = transient_block_y_axis.get_output();

        double input = 0.0;
        // x-axis
        input = -Epi*Tp*slip - Epr + Xs_minum_Xp*I.imag();
        transient_block_x_axis.set_input(input);
        transient_block_x_axis.run(mode);
        // y-axis
        input = Epr*Tp*slip - Epi - Xs_minum_Xp*I.real();
        transient_block_y_axis.set_input(input);
        transient_block_y_axis.run(mode);

        Ep = get_internal_voltage_in_pu();
        I = (Ep-Vterm)/Zsource;
        complex<double> S = Ep*conj(I);
        Pelec = S.real();
    }
    else
    {
        complex<double> Epp = get_internal_voltage_in_pu();
        complex<double> I = (Epp-Vterm)/Zsource;

        double Epr = transient_block_x_axis.get_output();
        double Epi = transient_block_y_axis.get_output();
        double Ekr = subtransient_block_x_axis.get_output();
        double Eki = subtransient_block_y_axis.get_output();

        double EppMag = abs(Epp);
        double sat = saturation.get_saturation(EppMag);

        double input = 0.0, temp = 0.0;
        // x-axis
        temp = I.imag()*Xp_minum_Xleakage+Epr-Ekr;

        input = Epp.imag()/EppMag*sat -
                Epi*Tp*slip -
                Epr +
                Xs_minum_Xp*(I.imag()-Xp_minum_Xpp_over_Xp_minum_Xleakage_square*temp);
        transient_block_x_axis.set_input(input);
        transient_block_x_axis.run(mode);

        input = temp - Eki*Tpp*slip;
        subtransient_block_x_axis.set_input(input);
        subtransient_block_x_axis.run(mode);
        // y-axis
        temp = -I.real()*Xp_minum_Xleakage+Epi-Eki;

        input = -Epp.real()/EppMag*sat +
                Epr*Tp*slip -
                Epi -
                Xs_minum_Xp*(I.real()+Xp_minum_Xpp_over_Xp_minum_Xleakage_square*temp);
        transient_block_y_axis.set_input(input);
        transient_block_y_axis.run(mode);

        input = temp + Ekr*Tpp*slip;
        subtransient_block_y_axis.set_input(input);
        subtransient_block_y_axis.run(mode);

        Epp = get_internal_voltage_in_pu();
        I = (Epp-Vterm)/Zsource;
        complex<double> S = Epp*conj(I);
        Pelec = S.real();
    }

    double speed = 1.0+speed_block.get_output();
    double temp = get_A()*speed*speed +
                  get_B()*speed +
                  get_C() +
                  get_D()*pow(speed, get_E());
    double Tload = initial_load_torque*temp;
    speed_block.set_input(-Pelec/speed - Tload);
    speed_block.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> CIM6::get_load_power_in_MVA()
{
    complex<double> E = get_internal_voltage_in_pu();
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    complex<double> I = (E-V)/Zsource;
    double V2 = abs(V)*abs(V);

    complex<double> Smotor = V*conj(I)*Mbase;
    complex<double> Sload = Smotor + complex<double>(0.0, nominal_shunt_in_MVar*V2);
    return -Sload;
}

complex<double> CIM6::get_internal_voltage_in_pu() const
{
    if(is_single_cage)
    {
        double Erp = transient_block_x_axis.get_output();
        double Eip = transient_block_y_axis.get_output();
        return complex<double>(Erp, Eip);
    }
    else
    {
        double Erpp = subtransient_block_x_axis.get_output()*Xp_minum_Xpp_over_Xp_minum_Xleakage+
                      transient_block_x_axis.get_output()*(1.0-Xp_minum_Xpp_over_Xp_minum_Xleakage);
        double Eipp = subtransient_block_y_axis.get_output()*Xp_minum_Xpp_over_Xp_minum_Xleakage+
                      transient_block_y_axis.get_output()*(1.0-Xp_minum_Xpp_over_Xp_minum_Xleakage);
        return complex<double>(Erpp, Eipp);
    }
}

complex<double> CIM6::get_load_current_in_pu_based_on_SBASE()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    complex<double> S = get_load_power_in_MVA()*one_over_sbase;
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();

    return conj(S/V);
}

complex<double> CIM6::get_norton_current_in_pu_based_on_SBASE()
{
    complex<double> E = get_internal_voltage_in_pu();
    complex<double> I = E/Zsource;

    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return I*Mbase*one_over_sbase;
}

void CIM6::check()
{
    ;
}
void CIM6::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void CIM6::save()
{
    ;
}
string CIM6::get_standard_psse_string() const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    unsigned int bus = load->get_load_bus();
    string identifier = "'"+load->get_identifier()+"'";

    string model_name = "'CIM6BL'";
    double mbase = get_Mbase_in_MVA();
    if(get_Pmult()!=0.0) mbase = 0.0;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<get_motor_type()<<", "
            <<setw(8)<<setprecision(6)<<get_Ra_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Xa_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Xm_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_R1_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_X1_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_R2_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_X2_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_E1_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_SE1_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_E2_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_SE2_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<mbase<<", "
            <<setw(8)<<setprecision(6)<<get_Pmult()<<", "
            <<setw(8)<<setprecision(6)<<get_H_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_VI_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_TI_in_cycles()<<", "
            <<setw(8)<<setprecision(6)<<get_TB_in_cycles()<<", "
            <<setw(8)<<setprecision(6)<<get_A()<<", "
            <<setw(8)<<setprecision(6)<<get_B()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_E()<<", "
            <<setw(8)<<setprecision(6)<<get_C()<<", "
            <<setw(8)<<setprecision(6)<<get_Tnom_in_pu()<<" /";

    return osstream.str();
}

void CIM6::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("RA", i); i++;
    add_model_data_name_and_index_pair("XA", i); i++;
    add_model_data_name_and_index_pair("XM", i); i++;
    add_model_data_name_and_index_pair("R1", i); i++;
    add_model_data_name_and_index_pair("X1", i); i++;
    add_model_data_name_and_index_pair("R2", i); i++;
    add_model_data_name_and_index_pair("X2", i); i++;
    add_model_data_name_and_index_pair("E1", i); i++;
    add_model_data_name_and_index_pair("SE1", i); i++;
    add_model_data_name_and_index_pair("E2", i); i++;
    add_model_data_name_and_index_pair("SE2", i); i++;
    add_model_data_name_and_index_pair("MBASE", i); i++;
    add_model_data_name_and_index_pair("PMULT", i); i++;
    add_model_data_name_and_index_pair("H", i); i++;
    add_model_data_name_and_index_pair("VI", i); i++;
    add_model_data_name_and_index_pair("TI", i); i++;
    add_model_data_name_and_index_pair("TB", i); i++;
    add_model_data_name_and_index_pair("A", i); i++;
    add_model_data_name_and_index_pair("B", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
    add_model_data_name_and_index_pair("E", i); i++;
    add_model_data_name_and_index_pair("C", i); i++;
    add_model_data_name_and_index_pair("TNOM", i); i++;
}

double CIM6::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="RA")
        return get_Ra_in_pu();
    if(par_name=="XA")
        return get_Xa_in_pu();
    if(par_name=="XM")
        return get_Xm_in_pu();
    if(par_name=="R1")
        return get_R1_in_pu();
    if(par_name=="X1")
        return get_X1_in_pu();
    if(par_name=="R2")
        return get_R2_in_pu();
    if(par_name=="X2")
        return get_X2_in_pu();
    if(par_name=="E1")
        return get_E1_in_pu();
    if(par_name=="SE1")
        return get_SE1_in_pu();
    if(par_name=="E2")
        return get_SE2_in_pu();
    if(par_name=="MBASE")
    {
        if(get_Pmult()!=0.0)
            return 0.0;
        else
            return get_Mbase_in_MVA();
    }
    if(par_name=="PMULT")
        return get_Pmult();
    if(par_name=="H")
        return get_H_in_s();
    if(par_name=="VI")
        return get_VI_in_pu();
    if(par_name=="TI")
        return get_TI_in_cycles();
    if(par_name=="TB")
        return get_TB_in_cycles();
    if(par_name=="A")
        return get_A();
    if(par_name=="B")
        return get_B();
    if(par_name=="D")
        return get_D();
    if(par_name=="E")
        return get_E();
    if(par_name=="C")
        return get_C();
    if(par_name=="TNOM")
        return get_Tnom_in_pu();

    return 0.0;
}

void CIM6::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="RA")
        return set_Ra_in_pu(value);
    if(par_name=="XA")
        return set_Xa_in_pu(value);
    if(par_name=="XM")
        return set_Xm_in_pu(value);
    if(par_name=="R1")
        return set_R1_in_pu(value);
    if(par_name=="X1")
        return set_X1_in_pu(value);
    if(par_name=="R2")
        return set_R2_in_pu(value);
    if(par_name=="X2")
        return set_X2_in_pu(value);
    if(par_name=="E1")
        return set_E1_in_pu(value);
    if(par_name=="SE1")
        return set_SE1_in_pu(value);
    if(par_name=="E2")
        return set_SE2_in_pu(value);
    if(par_name=="MBASE")
        return set_Mbase_in_MVA(value);
    if(par_name=="PMULT")
        return set_Pmult(value);
    if(par_name=="H")
        return set_H_in_s(value);
    if(par_name=="VI")
        return set_VI_in_pu(value);
    if(par_name=="TI")
        return set_TI_in_cycles(value);
    if(par_name=="TB")
        return set_TB_in_cycles(value);
    if(par_name=="A")
        return set_A(value);
    if(par_name=="B")
        return set_B(value);
    if(par_name=="D")
        return set_D(value);
    if(par_name=="E")
        return set_E(value);
    if(par_name=="C")
        return set_C(value);
    if(par_name=="TNOM")
        return set_Tnom_in_pu(value);

    return;
}

double CIM6::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void CIM6::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;

    add_model_inernal_variable_name_and_index_pair("TOTAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("TOTAL REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_inernal_variable_name_and_index_pair("MOTOR ACTIVE POWER LOAD IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("MOTOR REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_inernal_variable_name_and_index_pair("SHUNT REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_inernal_variable_name_and_index_pair("TOTAL CURRENT IN KA", i); i++;
    add_model_inernal_variable_name_and_index_pair("MOTOR CURRENT IN KA", i); i++;
    add_model_inernal_variable_name_and_index_pair("MOTOR SPEED DEVIATION IN PU", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TRANSIENT BLOCK OF X AXIS", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@TRANSIENT BLOCK OF Y AXIS", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SUBTRANSIENT BLOCK OF X AXIS", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SUBTRANSIENT BLOCK OF Y AXIS", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED BLOCK", i); i++;
}

double CIM6::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TOTAL ACTIVE POWER LOAD IN MW")
        return get_load_power_in_MVA().real();
    if(var_name == "TOTAL REACTIVE POWER LOAD IN MVAR")
        return get_load_power_in_MVA().imag();
    if(var_name == "MOTOR ACTIVE POWER LOAD IN MW")
        return get_load_power_in_MVA().real();
    if(var_name == "MOTOR REACTIVE POWER LOAD IN MVAR")
    {
        complex<double> E = get_internal_voltage_in_pu();
        complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
        complex<double> I = (E-V)/Zsource;
        complex<double> S = V*conj(I)*Mbase;
        return -S.imag();
    }
    if(var_name == "SHUNT REACTIVE POWER LOAD IN MVAR")
    {
        double V = get_bus_positive_sequence_voltage_in_pu();
        double V2 = V*V;
        double Q = nominal_shunt_in_MVar*V2;
        return -Q;
    }
    if(var_name == "TOTAL CURRENT IN KA")
    {
        double S = abs(get_load_power_in_MVA());
        double V = get_bus_positive_sequence_voltage_in_kV();
        return S/(SQRT3*V);
    }
    if(var_name == "MOTOR CURRENT IN KA")
    {
        complex<double> E = get_internal_voltage_in_pu();
        complex<double> Vcomplex = get_bus_positive_sequence_complex_voltage_in_pu();
        complex<double> I = (E-Vcomplex)/Zsource;
        complex<double> S = Vcomplex*conj(I)*Mbase;
        double V = get_bus_positive_sequence_voltage_in_kV();
        return abs(S)/(SQRT3*V);
    }
    if(var_name == "MOTOR SPEED DEVIATION IN PU")
        return speed_block.get_output();
    if(var_name == "STATE@TRANSIENT BLOCK OF X AXIS")
        return transient_block_x_axis.get_state();
    if(var_name == "STATE@TRANSIENT BLOCK OF Y AXIS")
        return transient_block_y_axis.get_state();
    if(var_name == "STATE@SUBTRANSIENT BLOCK OF X AXIS")
        return subtransient_block_x_axis.get_state();
    if(var_name == "STATE@SUBTRANSIENT BLOCK OF Y AXIS")
        return subtransient_block_y_axis.get_state();
    if(var_name == "STATE@SPEED BLOCK")
        return speed_block.get_state();

    return 0.0;
}

complex<double> CIM6::get_initial_load_power_in_MVA() const
{
    return complex<double>(P0, Q0);
}


string CIM6::get_dynamic_data_in_psse_format() const
{
    return "";
}

string CIM6::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string CIM6::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
