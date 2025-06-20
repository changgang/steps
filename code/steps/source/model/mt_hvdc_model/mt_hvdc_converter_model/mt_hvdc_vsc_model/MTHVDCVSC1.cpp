#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_vsc_model/MTHVDCVSC1.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

MTHVDCVSC1::MTHVDCVSC1(STEPS& toolkit) : HVSC_MODEL(toolkit),
                                         p_block(toolkit),
                                         q_block(toolkit),
                                         p_sensor(toolkit),
                                         q_sensor(toolkit),
                                         udc_block(toolkit)
 {
     clear();
 }

MTHVDCVSC1::~MTHVDCVSC1()
 {
     ;
 }

MTHVDCVSC1::MTHVDCVSC1(const MTHVDCVSC1& model) : HVSC_MODEL(model.get_toolkit()),
                                                   p_block(model.get_toolkit()),
                                                   q_block(model.get_toolkit()),
                                                   p_sensor(model.get_toolkit()),
                                                   q_sensor(model.get_toolkit()),
                                                   udc_block(model.get_toolkit())
 {
     clear();
     copy_from_const_model(model);
 }

MTHVDCVSC1& MTHVDCVSC1::operator=(const MTHVDCVSC1& model)
 {
    if(this != &model)
        copy_from_const_model(model);
    return *this;
 }

void MTHVDCVSC1::copy_from_const_model(const MTHVDCVSC1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    p_block.set_toolkit(toolkit);
    q_block.set_toolkit(toolkit);
    p_sensor.set_toolkit(toolkit);
    q_sensor.set_toolkit(toolkit);
    udc_block.set_toolkit(toolkit);
    clear();

    set_active_power_block_Kp(model.get_active_power_block_Kp());
    set_active_power_block_Ki(model.get_active_power_block_Ki());
    set_active_power_block_Kd(model.get_active_power_block_Kd());
    set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
    set_active_power_block_pmax(model.get_active_power_block_pmax());
    set_active_power_block_pmin(model.get_active_power_block_pmin());

    set_reactive_power_block_Kp(model.get_reactive_power_block_Kp());
    set_reactive_power_block_Ki(model.get_reactive_power_block_Ki());
    set_reactive_power_block_Kd(model.get_reactive_power_block_Kd());
    set_reactive_power_block_Td_in_s(model.get_reactive_power_block_Td_in_s());
    set_reactive_power_block_qmax(model.get_reactive_power_block_qmax());
    set_reactive_power_block_qmin(model.get_reactive_power_block_qmin());

    set_dc_voltage_block_ceq_in_uF(model.get_dc_voltage_block_ceq_in_uF());
}

string MTHVDCVSC1::get_model_name() const
{
    return "MTHVDCVSC1";
}

void MTHVDCVSC1::set_active_power_block_Kp(double kp)
{
    p_block.set_Kp(kp);
}

void MTHVDCVSC1::set_active_power_block_Ki(double ki)
{
    p_block.set_Ki(ki);
}

void MTHVDCVSC1::set_active_power_block_Kd(double kd)
{
    p_block.set_Kd(kd);
}

void MTHVDCVSC1::set_active_power_block_Td_in_s(double td)
{
    if(td==0.0)
        td = 0.01;
    p_block.set_Td_in_s(td);
}

void MTHVDCVSC1::set_active_power_block_pmax(double pmax)
{
    p_block.set_upper_limit(pmax);
}

void MTHVDCVSC1::set_active_power_block_pmin(double pmin)
{
    p_block.set_lower_limit(pmin);
}

void MTHVDCVSC1::set_reactive_power_block_Kp(double kp)
{
    q_block.set_Kp(kp);
}

void MTHVDCVSC1::set_reactive_power_block_Ki(double ki)
{
    q_block.set_Ki(ki);
}

void MTHVDCVSC1::set_reactive_power_block_Kd(double kd)
{
    q_block.set_Kd(kd);
}

void MTHVDCVSC1::set_reactive_power_block_Td_in_s(double td)
{
    if(td==0.0)
        td = 0.01;
    q_block.set_Td_in_s(td);
}

void MTHVDCVSC1::set_reactive_power_block_qmax(double qmax)
{
    q_block.set_upper_limit(qmax);
}

void MTHVDCVSC1::set_reactive_power_block_qmin(double qmin)
{
    q_block.set_lower_limit(qmin);
}

void MTHVDCVSC1::set_dc_voltage_block_ceq_in_uF(const double ceq)
{
    this->ceq_in_uF = ceq;
    udc_block.set_T_in_s(ceq*1e-6);
}


double MTHVDCVSC1::get_active_power_block_Kp() const
{
    return p_block.get_Kp();
}

double MTHVDCVSC1::get_active_power_block_Ki() const
{
    return p_block.get_Ki();
}

double MTHVDCVSC1::get_active_power_block_Kd() const
{
    return p_block.get_Kd();
}

double MTHVDCVSC1::get_active_power_block_Td_in_s() const
{
    return p_block.get_Td_in_s();
}

double MTHVDCVSC1::get_active_power_block_pmax() const
{
    return p_block.get_upper_limit();
}

double MTHVDCVSC1::get_active_power_block_pmin() const
{
    return p_block.get_lower_limit();
}

double MTHVDCVSC1::get_reactive_power_block_Kp() const
{
    return q_block.get_Kp();
}

double MTHVDCVSC1::get_reactive_power_block_Ki() const
{
    return q_block.get_Ki();
}

double MTHVDCVSC1::get_reactive_power_block_Kd() const
{
    return q_block.get_Kd();
}

double MTHVDCVSC1::get_reactive_power_block_Td_in_s() const
{
    return q_block.get_Td_in_s();
}

double MTHVDCVSC1::get_reactive_power_block_qmax() const
{
    return q_block.get_upper_limit();
}

double MTHVDCVSC1::get_reactive_power_block_qmin() const
{
    return q_block.get_lower_limit();
}

double MTHVDCVSC1::get_dc_voltage_block_ceq_in_uF() const
{
    return ceq_in_uF;
}


void MTHVDCVSC1::set_active_power_block_Pref(double pref)
{
    this->p_ref = pref;
}

void MTHVDCVSC1::set_active_power_block_Udcref(double udcref)
{
    this->udc_ref = udcref;
}

void MTHVDCVSC1::set_reactive_power_block_Qref(double qref)
{
    this->q_ref = qref;
}

void MTHVDCVSC1::set_reactive_power_block_Uacref(double uacref)
{
    this->uac_ref = uacref;
}

double MTHVDCVSC1::get_active_power_block_Pref() const
{
    return p_ref;
}

double MTHVDCVSC1::get_active_power_block_Udcref() const
{
    return udc_ref;
}

double MTHVDCVSC1::get_reactive_power_block_Qref() const
{
    return q_ref;
}

double MTHVDCVSC1::get_reactive_power_block_Uacref() const
{
    return uac_ref;
}



//
bool MTHVDCVSC1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=16)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            string converter_name = get_string_data(data[data_index],""); data_index++;
            set_model_converter_name(converter_name);

            double ceq = get_double_data(data[data_index],"0.0"); data_index++;
            set_dc_voltage_block_ceq_in_uF(ceq);

            double kp_d = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_Kp(kp_d);

            double ki_d = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_Ki(ki_d);

            double kd_d = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_Kd(kd_d);

            double Td_d = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_Td_in_s(Td_d);

            double pmax = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_pmax(pmax);

            double pmin = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_pmin(pmin);

            double kp_q = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_Kd(kp_q);

            double ki_q = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_Ki(ki_q);

            double kd_q = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_Kd(kd_q);

            double Td_q = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_Td_in_s(Td_q);

            double qmax = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_qmax(qmax);

            double qmin = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_qmin(qmin);

            is_successful = true;
        }
        return is_successful;
    }
    return is_successful;
}

bool MTHVDCVSC1::setup_model_with_psse_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}
bool MTHVDCVSC1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void MTHVDCVSC1::setup_block_toolkit_and_parameters()
{
    double ceq = get_dc_voltage_block_ceq_in_uF();
    if(ceq == 0.0)
    {
        udc_block.set_T_in_s(0.01);
    }
    else
    {
        udc_block.set_T_in_s(ceq*1e-6);
    }
    p_sensor.set_T_in_s(0.01);
    q_sensor.set_T_in_s(0.01);
}

void MTHVDCVSC1::initialize()
{
    if(not is_model_initialized())
    {
        HVSC* vsc = get_hvsc_pointer();
        setup_block_toolkit_and_parameters();

        double Ps = vsc->get_converter_P_to_AC_bus_MW();
        double Qs = vsc->get_converter_Q_to_AC_bus_Mvar();
        double Udc = vsc->get_udc_in_kV();
        double Idc = vsc->get_idc_in_kA();
        double Uac = vsc->get_converter_AC_bus_voltage_in_pu();

        double Sconverter_base = vsc->get_converter_rated_capacity_in_MVA();
        double Vdc_base = vsc->get_converter_base_voltage_in_kV();

        vsc->set_converter_Pdc_command_in_MW(Udc*Idc);

        HVSC_D_CONTROL_MODE dmode = vsc->get_converter_d_axis_control_mode();
        switch(dmode)
        {
            case VSC_CONSTANT_AC_ACTIVE_POWER_MODE:
                set_active_power_block_Pref(Ps/Sconverter_base);
                p_sensor.set_output(Ps/Sconverter_base);
                p_sensor.initialize();
                break;
            case VSC_CONSTANT_DC_VOLTAGE_MODE:
                set_active_power_block_Udcref(Udc/Vdc_base);
                p_sensor.set_output(Udc/Vdc_base);
                p_sensor.initialize();
                break;
            default:
                break;
        }

        HVSC_Q_CONTROL_MODE qmode = vsc->get_converter_q_axis_control_mode();
        switch(qmode)
        {
            case VSC_CONSTANT_AC_REACTIVE_POWER_MODE:
                set_reactive_power_block_Qref(Qs/Sconverter_base);
                q_sensor.set_output(Qs/Sconverter_base);
                q_sensor.initialize();
                break;
            case VSC_CONSTANT_AC_VOLTAGE_MODE:
                set_reactive_power_block_Uacref(Uac);
                q_sensor.set_output(Uac);
                q_sensor.initialize();
                break;
            default:
                break;
        }

        complex<double> Idq = vsc->get_converter_initial_ac_current_in_pu_on_converter_base_in_dq_axis();
        double Id = Idq.real();
        double Iq = Idq.imag();
        p_block.set_output(Id);
        p_block.initialize();
        q_block.set_output(Iq);
        q_block.initialize();

        udc_block.set_output(Udc);
        udc_block.initialize();
        /*
        ostringstream osstream;
        osstream<<" INITIALIZE RESULT OF VSC:: " <<vsc->get_converter_name()<<endl
                <<"Sensor"<<endl
                <<"P snesor output: "<<Ps/Sconverter_base<<" OR "<<Udc/Vdc_base<<" ' "
                <<"Q snesor output: "<<Qs/Sconverter_base<<" OR "<<Uac<<endl
                <<"Control block"<<endl
                <<"P block output: "<<Id<<" ' "
                <<"Q block output: "<<Iq;
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        */
        set_flag_model_initialized_as_true();
    }
}

void MTHVDCVSC1::run(DYNAMIC_MODE mode)
{
    HVSC* vsc = get_hvsc_pointer();
    double Sbase_con = vsc->get_converter_rated_capacity_in_MVA();
    double Udc_base = vsc->get_converter_base_voltage_in_kV();

    complex<double> Ixy_in_kA = get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA();
    complex<double> Uac_complex = vsc->get_converter_AC_bus_compelx_voltage_in_kV();
    complex<double> Scon = SQRT3*Uac_complex*conj(Ixy_in_kA);
    double Uac = vsc->get_converter_AC_bus_voltage_in_pu();
    double Udc = vsc->get_udc_in_kV();

    double P = Scon.real();
    double Q = Scon.imag();

    ostringstream osstream;
    /*
    osstream<<"Ixy_in_kA = "<<Ixy_in_kA.real()<<" and = "<<Ixy_in_kA.imag()<<endl
            <<"Uac = "<<Uac_complex.real()<<"  and  "<<Uac_complex.imag()<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */

    //double P = vsc->get_converter_P_to_AC_bus_MW();
    //double Q = vsc->get_converter_Q_to_AC_bus_Mvar();

    double Pref = get_active_power_block_Pref();
    double Udcref = get_active_power_block_Udcref();
    double Qref = get_reactive_power_block_Qref();
    double Uacref = get_reactive_power_block_Uacref();

    double d_input = 0.0;
    HVSC_D_CONTROL_MODE dmode = vsc->get_converter_d_axis_control_mode();
    if(dmode == VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
    {
        p_sensor.set_input(P/Sbase_con);
        p_sensor.run(mode);
        double Pm = p_sensor.get_output();
        d_input = Pref-Pm;
    }
    if(dmode == VSC_CONSTANT_DC_VOLTAGE_MODE)
    {
        p_sensor.set_input(Udc/Udc_base);
        p_sensor.run(mode);
        double Udcm =  p_sensor.get_output();
        d_input = Udcref-Udcm;
    }
    p_block.set_input(d_input);
    p_block.run(mode);

    double q_input = 0.0;
    HVSC_Q_CONTROL_MODE qmode = vsc->get_converter_q_axis_control_mode();
    if(qmode == VSC_CONSTANT_AC_REACTIVE_POWER_MODE)
    {
        q_sensor.set_input(Q/Sbase_con);
        q_sensor.run(mode);
        double Qm = q_sensor.get_output();
        q_input = -Qref+Qm;
    }
    if(qmode == VSC_CONSTANT_AC_VOLTAGE_MODE)
    {
        q_sensor.set_input(Uac);
        q_sensor.run(mode);
        double Uacm = q_sensor.get_output();
        q_input = Uacref-Uacm;
    }
    q_block.set_input(q_input);
    q_block.run(mode);


    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
    /*
     osstream<<"Ps = "<<P/Sbase_con<<" Qs = "<<Q/Sbase_con<<endl
             <<"Id = "<<p_block.get_output()<<" with input = "<<d_input<<endl
             <<"Iq = "<<q_block.get_output()<<" with input = "<<q_input<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */
}

void MTHVDCVSC1::check()
{
    ;
}

void MTHVDCVSC1::clear()
{
    p_block.set_limiter_type(WINDUP_LIMITER);
    q_block.set_limiter_type(WINDUP_LIMITER);
    udc_block.set_limiter_type(NO_LIMITER);
    p_ref = 0.0;
    q_ref = 0.0;
    udc_ref = 0.0;
    uac_ref = 0.0;
    ceq_in_uF = 1000;

    p_block.set_limiter_type(WINDUP_LIMITER);
    q_block.set_limiter_type(WINDUP_LIMITER);
    udc_block.set_limiter_type(NO_LIMITER);
}


void MTHVDCVSC1::report()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void MTHVDCVSC1::save()
{
    ;
}

string MTHVDCVSC1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    HVSC* vsc = get_hvsc_pointer();

    string mtdc_name = "'"+mtdc->get_name()+"'";
    string vsc_name = "'"+vsc->get_converter_name()+"'";
    string model_name = "'"+get_model_name()+"'";
    string dmode = "";
    if(vsc->get_converter_d_axis_control_mode()==VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
        dmode = "CONSTANT PAC CONTROL";
    else if(vsc->get_converter_d_axis_control_mode()==VSC_CONSTANT_DC_VOLTAGE_MODE)
        dmode = "CONSTANT UDC CONTROL";
    else
        dmode = "INVALID D-AXIS CONTROL MODE";

    string qmode = "";
    if(vsc->get_converter_q_axis_control_mode()==VSC_CONSTANT_AC_REACTIVE_POWER_MODE)
        qmode = "CONSTANT QAC CONTROL";
    else if(vsc->get_converter_q_axis_control_mode()==VSC_CONSTANT_AC_VOLTAGE_MODE)
        qmode = "CONSTANT UAC CONTROL";
    else
        qmode ="INVALID Q-AXIS CONTROL MODE";


    osstream<<setw(16)<<mtdc_name<<", "
            <<setw(10)<<vsc_name<<","
            <<setw(10)<<model_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", \n"
            <<setw(10)<<""
            <<setw(10)<<dmode<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(4)<<get_active_power_block_pmin()<<", \n"
            <<setw(10)<<""
            <<setw(10)<<qmode<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(4)<<get_reactive_power_block_qmin()<<" / "
            <<setw(10)<<"";

    return osstream.str();
}

void MTHVDCVSC1::update_converters()
{
    run(DYNAMIC_UPDATE_MODE);
}

void MTHVDCVSC1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("CEQ", i); i++;
    add_model_data_name_and_index_pair("KP D", i); i++;
    add_model_data_name_and_index_pair("KI D", i); i++;
    add_model_data_name_and_index_pair("KD D", i); i++;
    add_model_data_name_and_index_pair("TD D", i); i++;
    add_model_data_name_and_index_pair("IMAX D", i); i++;
    add_model_data_name_and_index_pair("IMIN D", i); i++;
    add_model_data_name_and_index_pair("KP Q", i); i++;
    add_model_data_name_and_index_pair("KI Q", i); i++;
    add_model_data_name_and_index_pair("KD Q", i); i++;
    add_model_data_name_and_index_pair("TD Q", i); i++;
    add_model_data_name_and_index_pair("IMAX Q", i); i++;
    add_model_data_name_and_index_pair("IMIN Q", i); i++;
}

double MTHVDCVSC1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="CEQ")
        return get_dc_voltage_block_ceq_in_uF();
    if(par_name=="KP D")
        return get_active_power_block_Kp();
    if(par_name=="KI D")
        return get_active_power_block_Ki();
    if(par_name=="KD D")
        return get_active_power_block_Kd();
    if(par_name=="TD D")
        return get_active_power_block_Td_in_s();
    if(par_name=="IMAX D")
        return get_active_power_block_pmax();
    if(par_name=="IMIN D")
        return get_active_power_block_pmin();

    if(par_name=="KP Q")
        return get_reactive_power_block_Kp();
    if(par_name=="KI Q")
        return get_reactive_power_block_Ki();
    if(par_name=="KD Q")
        return get_reactive_power_block_Kd();
    if(par_name=="TD Q")
        return get_reactive_power_block_Td_in_s();
    if(par_name=="IMAX Q")
        return get_reactive_power_block_qmax();
    if(par_name=="IMIN Q")
        return get_reactive_power_block_qmin();
    return 0.0;
}
void MTHVDCVSC1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="CEQ")
        return set_dc_voltage_block_ceq_in_uF(value);
    if(par_name=="KP D")
        return set_active_power_block_Kp(value);
    if(par_name=="KI D")
        return set_active_power_block_Ki(value);
    if(par_name=="KD D")
        return set_active_power_block_Kd(value);
    if(par_name=="TD D")
        return set_active_power_block_Td_in_s(value);
    if(par_name=="IMAX D")
        return set_active_power_block_pmax(value);
    if(par_name=="IMIN D")
        return set_active_power_block_pmin(value);

    if(par_name=="KP Q")
        return set_reactive_power_block_Kp(value);
    if(par_name=="KI Q")
        return set_reactive_power_block_Ki(value);
    if(par_name=="KD Q")
        return set_reactive_power_block_Kd(value);
    if(par_name=="TD Q")
        return set_reactive_power_block_Td_in_s(value);
    if(par_name=="IMAX Q")
        return set_reactive_power_block_qmax(value);
    if(par_name=="IMIN Q")
        return set_reactive_power_block_qmin(value);
    return;
}
double MTHVDCVSC1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;;
}

void MTHVDCVSC1::prepare_model_internal_variable_table()
{
    ;
}

double MTHVDCVSC1::get_model_internal_variable_with_name(string var_name)
{
    ;
}

string MTHVDCVSC1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string MTHVDCVSC1::get_dynamic_data_in_bpa_format() const
{
    return "";
}
string MTHVDCVSC1::get_dynamic_data_in_steps_format() const
{
    return "";
}

double MTHVDCVSC1::get_vsc_converter_ueq_in_kV() const
{
    return udc_block.get_output();
}

double MTHVDCVSC1::get_vsc_converter_req_ohm() const
{
    return 0.0;
}

complex<double> MTHVDCVSC1::get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base() const
{
    double Id = p_block.get_output();
    double Iq = q_block.get_output();
    complex<double> Idq = complex<double>(Id,Iq);
    /*
    ostringstream osstream;
    osstream<<"Idq_in_con_pu = "<<Id<<" +j  "<<Iq<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */

    return Idq;
}

complex<double> MTHVDCVSC1::get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_converter_base() const
{
    complex<double> Idq = get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base();
    double angle = get_dynamic_pll_angle_in_rad();
    complex<double> Ixy = dq2ab_with_angle_in_rad(Idq,angle);
    /*
    ostringstream osstream;
    osstream<<"Ixy_in_con_pu = "<<Ixy.real()<<" +j  "<<Ixy.imag()<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */
    return Ixy;
}

complex<double> MTHVDCVSC1::get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_system_base() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVSC* vsc = get_hvsc_pointer();
    double Scon_base = vsc->get_converter_rated_capacity_in_MVA();
    double Sac_base = psdb.get_system_base_power_in_MVA();

    complex<double> Ixy_on_converter_base = get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_converter_base();
    complex<double> Ixy_on_system_base = Ixy_on_converter_base*Scon_base/Sac_base;
    /*
    ostringstream osstream;
    osstream<<"Ixy_in_system_pu = "<<Ixy_on_system_base.real()<<" +j  "<<Ixy_on_system_base.imag()<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */

    return Ixy_on_system_base;
}

complex<double> MTHVDCVSC1::get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVSC* vsc = get_hvsc_pointer();
    BUS* bus = psdb.get_bus(vsc->get_converter_ac_bus());

    double Sbase = psdb.get_system_base_power_in_MVA();
    double Vbase = bus->get_base_voltage_in_kV();

    complex<double> Ixy_pu = get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_system_base();
    double Ibase = Sbase/SQRT3/Vbase;
    complex<double> Ixy_kA = Ixy_pu*Ibase;
    /*
    ostringstream osstream;
    osstream<<"Ixy_in_kA = "<<Ixy_kA.real()<<" +j  "<<Ixy_kA.imag()<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */
    return Ixy_kA;

}

complex<double> MTHVDCVSC1::get_converter_dynamic_ac_complex_power() const
{
    HVSC* vsc = get_hvsc_pointer();
    complex<double> ixy = get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA();
    complex<double> uac = vsc->get_converter_AC_bus_compelx_voltage_in_kV();
    complex<double> S = SQRT3*uac*conj(ixy);
    return S;

}

double MTHVDCVSC1::get_converter_dynamic_active_power_in_MW() const
{
    complex<double> S =get_converter_dynamic_ac_complex_power();
    return S.real();
}

double MTHVDCVSC1::get_converter_dynamic_reactive_power_in_Mvar() const
{
    complex<double> S =get_converter_dynamic_ac_complex_power();
    return S.imag();
}

double MTHVDCVSC1::get_converter_dynamic_current_from_converter_to_ceq_in_kA() const
{
    ostringstream osstream;
    HVSC* vsc = get_hvsc_pointer();
    double Udc = vsc->get_udc_in_kV();
    double Pdc = get_converter_dynamic_power_of_converter_to_ceq();
    double idc = Pdc/Udc;
    /*
    osstream<<"VSC CONVERTER:"<<vsc->get_converter_name()<<" idc to ceq is :";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    cout << fixed << setprecision(12) << idc<<endl;
    */

    return idc;
}

double MTHVDCVSC1::get_converter_dynamic_power_of_converter_to_ceq() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVSC* vsc = get_hvsc_pointer();
    ostringstream osstream;

    double Pdc = 0.0;
    complex<double> Vac = psdb.get_bus_positive_sequence_complex_voltage_in_kV(vsc->get_converter_ac_bus());
    complex<double> It = get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA();
    complex<double> Z = vsc->get_converter_commutating_impedance();
    complex<double> Uc = Vac - It*Z*SQRT3;
    double Pc = (SQRT3*Uc*conj(It)).real();

    double a = vsc->get_converter_loss_coefficient_A_in_kW();
    double b = vsc->get_converter_loss_coefficient_B_in_kW_per_amp();
    double c = vsc->get_converter_loss_coefficient_C_in_kW_per_amp_squard();
    double I = abs(It)*1000;
    double ploss = (a+b*I+c*I*I)/1000;
    double ploss_min = vsc->get_converter_min_loss_in_kW()/1000;

    if(ploss<ploss_min)
    {
        ploss = ploss_min;
    }
    Pdc = Pc-ploss;

    return Pdc;
}

