#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/VSCHVDCC0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VSCHVDCC0::VSCHVDCC0(STEPS& toolkit):VSC_HVDC_CONVERTER_MODEL(toolkit),
                                     p_block(toolkit),
                                     q_block(toolkit),
                                     udc_block(toolkit),
                                     p_sensor(toolkit),
                                     q_sensor(toolkit)
{
    clear();
}

VSCHVDCC0::VSCHVDCC0(const VSCHVDCC0& model):VSC_HVDC_CONVERTER_MODEL(model.get_toolkit()),
                                             p_block(model.get_toolkit()),
                                             q_block(model.get_toolkit()),
                                             udc_block(model.get_toolkit()),
                                             p_sensor(model.get_toolkit()),
                                             q_sensor(model.get_toolkit())
{
    clear();
    copy_from_const_model(model);
}

VSCHVDCC0::~VSCHVDCC0()
{
    ;
}

VSCHVDCC0& VSCHVDCC0::operator=(const VSCHVDCC0& model)
{
    if(this != &model)
        copy_from_const_model(model);
    return *this;
}

void VSCHVDCC0::copy_from_const_model(const VSCHVDCC0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    clear();

    this->set_converter_name(model.get_converter_name());

    set_converter_active_control_mode(model.get_converter_active_control_mode());
    switch(model.get_converter_active_control_mode())
    {
        case DY_VSC_AC_ACTIVE_POWER_CONTORL:
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
        case DY_VSC_DC_VOLTAGE_CONTORL:
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
        case DY_VSC_FREQUENCY_CONTROL:
            set_frequency_control_kf(model.get_frequency_control_kf());
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
        case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
            set_active_voltage_control_ku(model.get_active_voltage_control_ku());
            set_active_voltage_control_kp(model.get_active_voltage_control_kp());
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
        case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
            set_active_voltage_control_ku(model.get_active_voltage_control_ku());
            set_frequency_control_kf(model.get_frequency_control_kf());
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
        default:
            set_active_power_block_Kp(model.get_active_power_block_Kp());
            set_active_power_block_Ki(model.get_active_power_block_Ki());
            set_active_power_block_Kd(model.get_active_power_block_Kd());
            set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
            set_active_power_block_pmax(model.get_active_power_block_pmax());
            set_active_power_block_pmin(model.get_active_power_block_pmin());
            break;
    }

    set_converter_reactive_control_mode(model.get_converter_reactive_control_mode());
    set_reactive_power_block_Kp(model.get_reactive_power_block_Kp());
    set_reactive_power_block_Ki(model.get_reactive_power_block_Ki());
    set_reactive_power_block_Kd(model.get_reactive_power_block_Kd());
    set_reactive_power_block_Td_in_s(model.get_reactive_power_block_Td_in_s());
    set_reactive_power_block_qmax(model.get_reactive_power_block_qmax());
    set_reactive_power_block_qmin(model.get_reactive_power_block_qmin());

    set_dc_voltage_block_ceq_in_uF(model.get_dc_voltage_block_ceq_in_uF());

    p_block.set_toolkit(toolkit);
    q_block.set_toolkit(toolkit);
    udc_block.set_toolkit(toolkit);
    p_sensor.set_toolkit(toolkit);
    q_sensor.set_toolkit(toolkit);
}

string VSCHVDCC0::get_model_name() const
{
    return "VSCHVDCC0";
}

void VSCHVDCC0::set_active_voltage_control_ku(double ku)
{
    p_ku = ku;
}

void VSCHVDCC0::set_active_voltage_control_kp(double kp)
{
    p_kp = kp;
}

void VSCHVDCC0::set_frequency_control_kf(double kf)
{
    p_kf = kf;
}

void VSCHVDCC0::set_active_power_block_Kp(double kp)
{
    p_block.set_Kp(kp);
}

void VSCHVDCC0::set_active_power_block_Ki(double ki)
{
    p_block.set_Ki(ki);
}

void VSCHVDCC0::set_active_power_block_Kd(double kd)
{
    p_block.set_Kd(kd);
}

void VSCHVDCC0::set_active_power_block_Td_in_s(double td)
{
    if(td==0.0)
        td = 0.01;
    p_block.set_Td_in_s(td);
}

void VSCHVDCC0::set_active_power_block_pmax(double pmax)
{
    p_block.set_upper_limit(pmax);
}

void VSCHVDCC0::set_active_power_block_pmin(double pmin)
{
    p_block.set_lower_limit(pmin);
}

void VSCHVDCC0::set_reactive_power_block_Kp(double kp)
{
    q_block.set_Kp(kp);
}

void VSCHVDCC0::set_reactive_power_block_Ki(double ki)
{
    q_block.set_Ki(ki);
}

void VSCHVDCC0::set_reactive_power_block_Kd(double kd)
{
    q_block.set_Kd(kd);
}

void VSCHVDCC0::set_reactive_power_block_Td_in_s(double td)
{
    if(td==0.0)
        td = 0.01;
    q_block.set_Td_in_s(td);
}

void VSCHVDCC0::set_reactive_power_block_qmax(double qmax)
{
    q_block.set_upper_limit(qmax);
}

void VSCHVDCC0::set_reactive_power_block_qmin(double qmin)
{
    q_block.set_lower_limit(qmin);
}

void VSCHVDCC0::set_dc_voltage_block_ceq_in_uF(const double ceq)
{
    udc_block.set_T_in_s(ceq*1e-6);
}


double VSCHVDCC0::get_active_voltage_control_ku() const
{
    return p_ku;
}

double VSCHVDCC0::get_active_voltage_control_kp() const
{
    return p_kp;
}

double VSCHVDCC0::get_frequency_control_kf() const
{
    return p_kf;
}

double VSCHVDCC0::get_active_power_block_Kp() const
{
    return p_block.get_Kp();
}

double VSCHVDCC0::get_active_power_block_Ki() const
{
    return p_block.get_Ki();
}

double VSCHVDCC0::get_active_power_block_Kd() const
{
    return p_block.get_Kd();
}

double VSCHVDCC0::get_active_power_block_Td_in_s() const
{
    return p_block.get_Td_in_s();
}

double VSCHVDCC0::get_active_power_block_pmax() const
{
    return p_block.get_upper_limit();
}

double VSCHVDCC0::get_active_power_block_pmin() const
{
    return p_block.get_lower_limit();
}

double VSCHVDCC0::get_reactive_power_block_Kp() const
{
    return q_block.get_Kp();
}

double VSCHVDCC0::get_reactive_power_block_Ki() const
{
    return q_block.get_Ki();
}

double VSCHVDCC0::get_reactive_power_block_Kd() const
{
    return q_block.get_Kd();
}

double VSCHVDCC0::get_reactive_power_block_Td_in_s() const
{
    return q_block.get_Td_in_s();
}

double VSCHVDCC0::get_reactive_power_block_qmax() const
{
    return q_block.get_upper_limit();
}

double VSCHVDCC0::get_reactive_power_block_qmin() const
{
    return q_block.get_lower_limit();
}

double VSCHVDCC0::get_dc_voltage_block_ceq_in_uF() const
{
    return udc_block.get_T_in_s()*1e6;
}

void VSCHVDCC0::set_active_power_block_Pref(double pref)
{
    this->p_ref = pref;
}

void VSCHVDCC0::set_active_power_block_Udcref(double udcref)
{
    this->p_ref = udcref;
}

void VSCHVDCC0::set_reactive_power_block_Qref(double qref)
{
    this->q_ref = qref;
}

void VSCHVDCC0::set_reactive_power_block_Uacref(double uacref)
{
    this->q_ref = uacref;
}

double VSCHVDCC0::get_active_power_block_Pref() const
{
    return p_ref;
}

double VSCHVDCC0::get_active_power_block_Udcref() const
{
    return get_active_power_block_Pref();
}

double VSCHVDCC0::get_reactive_power_block_Qref() const
{
    return q_ref;
}

double VSCHVDCC0::get_reactive_power_block_Uacref() const
{
    return get_reactive_power_block_Qref();
}

double VSCHVDCC0::get_dynamic_dc_voltage_in_kV() const
{
    return udc_block.get_output();
}

double VSCHVDCC0::get_dynamic_dc_voltage_in_pu() const
{
    return get_dynamic_dc_voltage_in_kV()/get_converter_dc_base_voltage_in_kV();
}

bool VSCHVDCC0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=18)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            string converter_name = get_string_data(data[data_index],""); data_index++;
            set_converter_name(converter_name);

            double ceq = get_double_data(data[data_index],"0.0"); data_index++;
            set_dc_voltage_block_ceq_in_uF(ceq);

            string mode=get_string_data(data[data_index],""); data_index++;
            VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode = DY_VSC_INVALID_ACTIVE_POWER_CONTORL_MODE;
            if(mode=="Vdc_control")
                active_control_mode = DY_VSC_DC_VOLTAGE_CONTORL;
            if(mode=="Pac_control")
                active_control_mode = DY_VSC_AC_ACTIVE_POWER_CONTORL;
            if(mode=="f_control")
                active_control_mode = DY_VSC_FREQUENCY_CONTROL;
            if(mode=="VPdc_control")
                active_control_mode = DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL;
            if(mode=="Vf_control")
                active_control_mode = DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL;

            set_converter_active_control_mode(active_control_mode);

            double ku = 0.0;
            double kp = 0.0;
            double kf = 0.0;
            double p_kp = 0.0;
            double p_ki = 0.0;
            double p_kd = 0.0;
            double p_td = 0.0;
            double p_pmax = 0.0;
            double p_pmin = 0.0;
            switch(active_control_mode)
            {
                case DY_VSC_DC_VOLTAGE_CONTORL:
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
                case DY_VSC_AC_ACTIVE_POWER_CONTORL:
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
                case DY_VSC_FREQUENCY_CONTROL:
                    kf = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
                case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                    ku = get_double_data(data[data_index],"0.0"); data_index++;
                    kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
                case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
                    ku = get_double_data(data[data_index],"0.0"); data_index++;
                    kf = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
                default:
                    p_kp = get_double_data(data[data_index],"0.0"); data_index++;
                    p_ki = get_double_data(data[data_index],"0.0"); data_index++;
                    p_kd = get_double_data(data[data_index],"0.0"); data_index++;
                    p_td = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
                    p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
                    break;
            }

            set_active_voltage_control_ku(ku);
            set_active_voltage_control_kp(kp);
            set_frequency_control_kf(kf);

            set_active_power_block_Kp(p_kp);
            set_active_power_block_Ki(p_ki);
            set_active_power_block_Kd(p_kd);
            set_active_power_block_Td_in_s(p_td);
            set_active_power_block_pmax(p_pmax);
            set_active_power_block_pmin(p_pmin);

            mode=get_string_data(data[data_index],""); data_index++;
            VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode = DY_VSC_INVALID_REACTIVE_POWER_CONTORL_MODE;
            if(mode=="Vac_control")
                reactive_control_mode = DY_VSC_AC_VOLTAGE_CONTROL;
            if(mode=="Qac_control")
                reactive_control_mode = DY_VSC_AC_REACTIVE_POWER_CONTROL;

            set_converter_reactive_control_mode(reactive_control_mode);

            double q_kp = get_double_data(data[data_index],"0.0"); data_index++;
            double q_ki = get_double_data(data[data_index],"0.0"); data_index++;
            double q_kd = get_double_data(data[data_index],"0.0"); data_index++;
            double q_td = get_double_data(data[data_index],"0.0"); data_index++;
            double q_qmax = get_double_data(data[data_index],"0.0"); data_index++;
            double q_qmin = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_block_Kp(q_kp);
            set_reactive_power_block_Ki(q_ki);
            set_reactive_power_block_Kd(q_kd);
            set_reactive_power_block_Td_in_s(q_td);
            set_reactive_power_block_qmax(q_qmax);
            set_reactive_power_block_qmin(q_qmin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool VSCHVDCC0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSCHVDCC0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSCHVDCC0::setup_block_toolkit_and_parameters()
{
    p_sensor.set_T_in_s(0.01);
    q_sensor.set_T_in_s(0.01);
}

void VSCHVDCC0::initialize()
{
    if(not is_model_initialized())
    {
        VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
        setup_block_toolkit_and_parameters();

        unsigned int converter_index=get_converter_index();
        complex<double> S = get_converter_initial_S_from_converter_to_AC_bus_in_MVA();
        double P0 = S.real()/get_converter_capacity_in_MVA();

        complex<double> Idq = get_converter_initial_current_from_converter_to_ac_bus_in_dq_axis_in_pu_on_converter_base();
        double Id = Idq.real();
        double Iq = Idq.imag();
        VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_power_control_mode = get_converter_active_control_mode();
        VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_power_control_mode = get_converter_reactive_control_mode();

        /*
        STEPS_SHOW_FILE_FUNCTION_AND_LINE_INFO
        cout<<"initializing converter "<<get_converter_index()<<": Idq(pu on converter base) = "<<Idq<<endl
            <<"        Ixy(pu on converter base) = "<<get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_converter_base()<<endl
            <<"        Ixy(pu on system base   ) = "<<get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_system_base()<<endl
            <<"        Ixy(kA                  ) = "<<get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_kA()<<endl
            <<"        S(MVA                   ) = "<<get_converter_initial_S_from_converter_to_AC_bus_in_MVA()<<endl
            <<"        Vac(pu                  ) = "<<get_converter_ac_bus_complex_voltage_in_pu()<<endl
            <<"        Vac(pu                  ) = "<<get_converter_ac_bus_voltage_in_pu()<<endl
            <<"        Vac(pu                  ) = "<<get_converter_ac_bus_angle_in_deg()<<endl;
        */
        if(active_power_control_mode==DY_VSC_FREQUENCY_CONTROL)
            Id = 0;
        p_block.set_output(Id);
        p_block.initialize();

        q_block.set_output(Iq);
        q_block.initialize();

        double Udc_in_kV = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(converter_index));
        udc_block.set_output(Udc_in_kV);
        udc_block.initialize();

        double pref = 0.0;
        double qref = 0.0;
        double udcref = 0.0;
        double uacref = 0.0;
        double fref = 0.0;
        double Vpref = 0.0;
        switch(active_power_control_mode)
        {
            case DY_VSC_DC_VOLTAGE_CONTORL:
                udcref = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()));
                udcref /= vsc_hvdc->get_dc_network_base_voltage_in_kV();
                set_active_power_block_Pref(udcref);
                break;
            case DY_VSC_AC_ACTIVE_POWER_CONTORL:
                pref = vsc_hvdc->get_converter_P_to_AC_bus_in_MW(get_converter_index());
                pref /= get_converter_capacity_in_MVA();
                set_active_power_block_Pref(pref);
                break;
            case DY_VSC_FREQUENCY_CONTROL:
                set_active_power_block_Pref(P0);
                break;
            case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
                udcref = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()));
                udcref /= vsc_hvdc->get_dc_network_base_voltage_in_kV();
                pref = vsc_hvdc->get_converter_P_to_AC_bus_in_MW(get_converter_index());
                pref /= get_converter_capacity_in_MVA();
                set_active_power_block_Pref(udcref);
                set_active_power_block_Pref(pref);
            case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
                udcref = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()));
                udcref /= vsc_hvdc->get_dc_network_base_voltage_in_kV();
                set_active_power_block_Pref(udcref);
            default:
                break;
        }

        p_sensor.set_output(0.0);
        p_sensor.initialize();

        switch(reactive_power_control_mode)
        {
            case DY_VSC_AC_VOLTAGE_CONTROL:
                uacref = get_converter_ac_bus_voltage_in_pu();
                set_reactive_power_block_Qref(uacref);
                break;
            case DY_VSC_AC_REACTIVE_POWER_CONTROL:
                qref = vsc_hvdc->get_converter_Q_to_AC_bus_in_MVar(get_converter_index());
                qref /= get_converter_capacity_in_MVA();
                set_reactive_power_block_Qref(qref);
                break;
            default:
                break;
        }
        q_sensor.set_output(0.0);
        q_sensor.initialize();
    }
}

void VSCHVDCC0::run(DYNAMIC_MODE mode)
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int converter_index = get_converter_index();
    complex<double> Is = get_converter_dynamic_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_converter_base();
    complex<double> Us = get_converter_ac_bus_complex_voltage_in_pu();
    complex<double> S = Us*conj(Is);

    double Ps = S.real();
    double Qs = S.imag();

    double input = 0.0;
    double Pdc_from_Ceq_to_dc_network = vsc_hvdc->get_converter_Pdc_from_Ceq_to_DC_network_in_MW(converter_index);
    double Pac_from_ac_network_to_Ceq = get_converter_dc_power_from_converter_to_Ceq_in_MW();
    double Udc = get_dynamic_dc_voltage_in_kV();

    input = (-Pdc_from_Ceq_to_dc_network+Pac_from_ac_network_to_Ceq)/Udc;
    udc_block.set_input(input);
    udc_block.run(mode);

    vsc_hvdc->set_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()),
                                   get_dynamic_dc_voltage_in_kV());

    VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_power_control_mode = get_converter_active_control_mode();
    VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_power_control_mode = get_converter_reactive_control_mode();

    input = 0.0;
    switch(active_power_control_mode)
    {
        case DY_VSC_DC_VOLTAGE_CONTORL:
            input = get_dynamic_dc_voltage_in_pu() - get_active_power_block_Udcref();
            break;
        case DY_VSC_AC_ACTIVE_POWER_CONTORL:
            input = get_active_power_block_Pref() - Ps;
            break;
        case DY_VSC_FREQUENCY_CONTROL:
            input = get_converter_ac_bus_frequency_deviation_in_pu();
            break;
        case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
            input = get_active_voltage_control_ku()*(get_dynamic_dc_voltage_in_pu() - get_active_power_block_Udcref())+ get_active_voltage_control_kp()*(get_active_power_block_Pref() - Ps);
        case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
            //if(get_active_voltage_control_ku()*(get_dynamic_dc_voltage_in_pu()>get_active_power_block_Udcref())- get_frequency_control_kf()*get_converter_ac_bus_frequency_deviation_in_pu())：
            //    input = get_active_voltage_control_ku()*(get_dynamic_dc_voltage_in_pu() - get_active_power_block_Udcref());
            //else:
            //    input = get_frequency_control_kf()*get_converter_ac_bus_frequency_deviation_in_pu();
            input = get_active_voltage_control_ku()*(get_dynamic_dc_voltage_in_pu() - get_active_power_block_Udcref()) + get_frequency_control_kf()*get_converter_ac_bus_frequency_deviation_in_pu();
            //input = - get_frequency_control_kf()*get_converter_ac_bus_frequency_deviation_in_pu();
        default:
            break;
    }

    p_sensor.set_input(input);
    p_sensor.run(mode);

    input = p_sensor.get_output();
    p_block.set_input(input);
    p_block.run(mode);

    input = 0.0;
    switch(reactive_power_control_mode)
    {
        case DY_VSC_AC_VOLTAGE_CONTROL:
            input = abs(Us) - get_reactive_power_block_Uacref();
            break;
        case DY_VSC_AC_REACTIVE_POWER_CONTROL:
            input = Qs - get_reactive_power_block_Qref();
            break;
        default:
            break;
    }
    q_sensor.set_input(input);
    q_sensor.run(mode);

    input = q_sensor.get_output();
    q_block.set_input(input);
    q_block.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void VSCHVDCC0::check()
{

}

void VSCHVDCC0::clear()
{
    set_as_current_source();
    p_block.set_limiter_type(WINDUP_LIMITER);
    q_block.set_limiter_type(WINDUP_LIMITER);
    udc_block.set_limiter_type(NO_LIMITER);
}

void VSCHVDCC0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSCHVDCC0::save()
{

}

string VSCHVDCC0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    string vsc_name = "'"+vsc->get_name()+"'";

    string model_name = "'"+get_model_name()+"'";
    string converter_name = "'"+get_converter_name()+"'";

    VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE pmode = get_converter_active_control_mode();
    string pmode_name = "Unsupported";
    switch(pmode)
    {
        case DY_VSC_AC_ACTIVE_POWER_CONTORL:
            pmode_name = "'Pac_control'";
            break;
        case DY_VSC_DC_VOLTAGE_CONTORL:
            pmode_name = "'Vdc_control'";
            break;
        case DY_VSC_FREQUENCY_CONTROL:
            pmode_name = "'f_control'";
            break;
        case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
            pmode_name = "'VPdc_control'";
            break;
        case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
            pmode_name = "'Vf_control'";
            break;
        default:
            break;
    }

    VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE qmode = get_converter_reactive_control_mode();
    string qmode_name = "Unsupported";
    switch(qmode)
    {
        case DY_VSC_AC_REACTIVE_POWER_CONTROL:
            qmode_name = "'Qac_control'";
            break;
        case DY_VSC_AC_VOLTAGE_CONTROL:
            qmode_name = "'Vac_control'";
            break;
        default:
            break;
    }

    switch(pmode)
    {
        case DY_VSC_DC_VOLTAGE_CONTORL:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
        case DY_VSC_AC_ACTIVE_POWER_CONTORL:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
        case DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_active_voltage_control_ku()<<", "
            <<setw(8)<<setprecision(3)<<get_active_voltage_control_kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
        case DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_active_voltage_control_ku()<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_control_kf()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
        case DY_VSC_FREQUENCY_CONTROL:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_control_kf()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
        default:
            osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "<<endl
            <<setw(10)<<pmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmax()<<", "
            <<setw(8)<<setprecision(3)<<get_active_power_block_pmin()<<", "<<endl
            <<setw(10)<<qmode_name<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Kd()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmax()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_power_block_qmin()<<" /";
            break;
    }
    return osstream.str();
}

void VSCHVDCC0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TEST", i); i++;
}

double VSCHVDCC0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return 0.0;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void VSCHVDCC0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double VSCHVDCC0::get_minimum_nonzero_time_constant_in_s()
{
    return 0.0;
}

void VSCHVDCC0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("TEST STATE", i); i++;
}

double VSCHVDCC0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TEST STATE") return 0.0;
    return 0.0;
}

string VSCHVDCC0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSCHVDCC0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string VSCHVDCC0::get_dynamic_data_in_steps_format() const
{
    return "";
}

complex<double> VSCHVDCC0::get_converter_voltage_in_xy_axis_in_pu_as_voltage_source() const
{
    return 0.0;
}

complex<double> VSCHVDCC0::get_converter_dynamic_current_from_converter_to_ac_bus_in_dq_axis_in_pu_on_converter_base_as_current_source() const
{
    double Id = p_block.get_output();
    if(get_converter_active_control_mode()==DY_VSC_FREQUENCY_CONTROL)
    {
        double P = Id + get_active_power_block_Pref();
        Id = P/get_converter_ac_bus_voltage_in_pu();
    }

    double Iq = q_block.get_output();
    return complex<double>(Id, Iq);
}

double VSCHVDCC0::get_converter_dc_power_from_converter_to_Ceq_in_MW()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int converter_index = get_converter_index();

    complex<double> Is = get_converter_dynamic_current_from_converter_to_ac_bus_in_xy_axis_in_kA();
    complex<double> Us = get_converter_ac_bus_complex_voltage_in_kV();
    complex<double> Zc_in_ohm = vsc_hvdc->get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Es = Us+Is*Zc_in_ohm*SQRT3;
    complex<double> S = Es*conj(Is)*SQRT3;

    double Ps = S.real();
    double Qs = S.imag();

    double c_loss = vsc_hvdc->get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss = vsc_hvdc->get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss = vsc_hvdc->get_converter_loss_factor_A_in_kW(converter_index);
    double Is_mag = abs(Is)*1000;
    double P_converter_loss = (c_loss*Is_mag*Is_mag+b_loss*Is_mag+a_loss)/1000;
    double Pdc_from_dc_network_to_converter = Ps + P_converter_loss;

    return -Pdc_from_dc_network_to_converter;
}

double VSCHVDCC0::get_initial_angle_at_pll_in_rad() const
{
    return get_converter_initial_ac_angle_at_ac_bus_side_in_rad();
}

double VSCHVDCC0::get_dynamic_angle_at_pll_in_rad() const
{
    return get_converter_dynamic_ac_angle_at_ac_bus_side_in_rad();
}
