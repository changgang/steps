#include "header/model/load_model/CIM6.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
CIM6::CIM6()
{
    clear();
}

CIM6::~CIM6()
{
}

void CIM6::clear()
{
    set_model_float_parameter_count(23);
    prepare_model_data_table();
    prepare_model_internal_variable_table();
    saturation.set_saturation_type(QUADRATIC_SATURATION_TYPE);
}

void CIM6::copy_from_const_model(const CIM6& model)
{
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

CIM6::CIM6(const CIM6& model) : LOAD_MODEL()
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

string CIM6::get_model_name() const
{
    return "CIM6";
}

void CIM6::set_motor_type(size_t t)
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

size_t CIM6::get_motor_type() const
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


bool CIM6::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=27)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name=="CIM6AL" or model_name=="CIM6BL" or model_name=="CIM6AR" or model_name=="CIM6ZN" or model_name=="CIM6OW")
        {
            size_t motor_type;
            double ra, xa, xm, r1, x1, r2, x2, e1, se1, e2, se2, mbase, pmult, h, vi, ti, tb, a, b, c, d, e, tnom;

            size_t i=3;
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


complex<double> CIM6::get_dynamic_source_admittance_in_pu_based_on_SBASE()
{
    setup_model_dynamic_parameters();
    complex<double> y = 1.0/Zsource;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = Mbase;
    return y*(mbase/sbase);
}

void CIM6::setup_block_toolkit_and_parameters()
{
    setup_model_dynamic_parameters();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    speed_block.set_toolkit(toolkit);
    angle_block.set_toolkit(toolkit);
    transient_block_d_axis.set_toolkit(toolkit);
    subtransient_block_d_axis.set_toolkit(toolkit);
    transient_block_q_axis.set_toolkit(toolkit);
    subtransient_block_q_axis.set_toolkit(toolkit);

    saturation.set_saturation_type(QUADRATIC_SATURATION_TYPE);

    transient_block_d_axis.set_T_in_s(Tp);
    transient_block_q_axis.set_T_in_s(Tp);
    if(not is_single_cage)
    {
        subtransient_block_d_axis.set_T_in_s(Tpp);
        subtransient_block_q_axis.set_T_in_s(Tpp);
    }
}

void CIM6::setup_model_dynamic_parameters()
{
    if(get_R2_in_pu()==0.0 or get_X2_in_pu()==0.0)// single cage
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
        }
    }

    if(is_single_cage)
        Zsource = complex<double>(Ra, Xp);
    else
        Zsource = complex<double>(Ra, Xpp);
}

void CIM6::initialize()
{
    setup_block_toolkit_and_parameters();

    LOAD* load = get_load_pointer();

    complex<double> S = load->get_actual_total_load_in_MVA();
    double V = get_bus_positive_sequence_voltage_in_pu();

    complex<double> I = conj(S/(Mbase*V));
    complex<double> Esource = V-I*Zsource;

    complex<double> Ep, Epp;

    if(is_single_cage)
        Ep = Esource;
    else
    {
        complex<double> Fpp = Esource*complex<double>(0, 1);
        double speed = speed_block.get_output();
        double A = -W0*speed*Tpp;
        complex<double> complex_1_A(1.0, A);
        complex<double> temp = Xp_minum_Xpp_over_Xp_minum_Xleakage+(1.0-Xp_minum_Xpp_over_Xp_minum_Xleakage)*complex_1_A;
        complex<double> unit_imag(0.0, 1.0);
        Ep = (Esource*complex_1_A+unit_imag*I*Xp_minum_Xpp)/temp;
        Epp = (Ep-unit_imag*I*Xp_minum_Xleakage)/complex_1_A;
    }

    transient_block_d_axis.set_output(Ep.real());
    transient_block_q_axis.set_output(Ep.imag());
    if(not is_single_cage)
    {
        subtransient_block_d_axis.set_output(Epp.real());
        subtransient_block_q_axis.set_output(Epp.imag());
    }

    set_flag_model_initialized_as_true();
}

void CIM6::run(DYNAMIC_MODE mode)
{
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> CIM6::get_load_power_in_MVA()
{
    complex<double> E = get_internal_voltage_in_pu();
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    return -V*conj((E-V)/Zsource)*Mbase;
}

complex<double> CIM6::get_internal_voltage_in_pu() const
{
    if(is_single_cage)
    {
        return 0.0;
    }
    else
    {
        return 0.0;
    }
}

complex<double> CIM6::get_load_current_in_pu_based_on_SBASE()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    complex<double> E = get_internal_voltage_in_pu();
    complex<double> I = E/Zsource;

    return -I*(Mbase*one_over_sbase);
}

void CIM6::check()
{
    ;
}
void CIM6::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
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
    size_t bus = load->get_load_bus();
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
    size_t i=0;
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

void CIM6::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;

    add_model_inernal_variable_name_and_index_pair("TOTAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("TOTAL REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_inernal_variable_name_and_index_pair("INITIAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("INITIAL REACTIVE POWER LOAD IN MVAR", i); i++;
}

double CIM6::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TOTAL ACTIVE POWER LOAD IN MW")
        return get_load_power_in_MVA().real();
    if(var_name == "TOTAL REACTIVE POWER LOAD IN MVAR")
        return get_load_power_in_MVA().imag();
    if(var_name == "INITIAL ACTIVE POWER LOAD IN MW")
        return get_initial_load_power_in_MVA().real();
    if(var_name == "INITIAL REACTIVE POWER LOAD IN MVAR")
        return get_initial_load_power_in_MVA().imag();

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
