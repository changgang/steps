#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/VSCHVDCC0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
VSCHVDCC0::VSCHVDCC0(STEPS& toolkit):VSC_HVDC_CONVERTER_MODEL(toolkit),
                                     p_block(toolkit),
                                     q_block(toolkit),
                                     ceq_block(toolkit)
{
    clear();
}

VSCHVDCC0::VSCHVDCC0(const VSCHVDCC0& model):VSC_HVDC_CONVERTER_MODEL(model.get_toolkit()),
                                             p_block(model.get_toolkit()),
                                             q_block(model.get_toolkit()),
                                             ceq_block(model.get_toolkit())
{
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
    set_active_power_block_Kp(model.get_active_power_block_Kp());
    set_active_power_block_Ki(model.get_active_power_block_Ki());
    set_active_power_block_Kd(model.get_active_power_block_Kd());
    set_active_power_block_Td_in_s(model.get_active_power_block_Td_in_s());
    set_active_power_block_pmax(model.get_active_power_block_pmax());
    set_active_power_block_pmin(model.get_active_power_block_pmin());

    set_converter_reactive_control_mode(model.get_converter_reactive_control_mode());
    set_reactive_power_block_Kp(model.get_reactive_power_block_Kp());
    set_reactive_power_block_Ki(model.get_reactive_power_block_Ki());
    set_reactive_power_block_Kd(model.get_reactive_power_block_Kd());
    set_reactive_power_block_Td_in_s(model.get_reactive_power_block_Td_in_s());
    set_reactive_power_block_qmax(model.get_reactive_power_block_qmax());
    set_reactive_power_block_qmin(model.get_reactive_power_block_qmin());

    set_dc_voltage_ceq(model.get_dc_voltage_ceq());

    p_block.set_toolkit(toolkit);
    q_block.set_toolkit(toolkit);
    ceq_block.set_toolkit(toolkit);
}

string VSCHVDCC0::get_model_name() const
{
    return "VSCHVDCC0";
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

void VSCHVDCC0::set_dc_voltage_block_ceq(const double ceq)
{
    udc_block.set_T_in_s(ceq);
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

double VSCHVDCC0::get_dc_voltage_block_ceq() const
{
    return udc_block.get_T_in_s();
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

bool VSCHVDCC0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=2)
    {
        string model_name = get_string_data(data[1],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            string converter_name = get_string_data(data[data_index],""); data_index++;
            set_converter_name(converter_name);

            double ceq = get_double_data(data[data_index],"0.0"); data_index++;
            set_dc_voltage_ceq(ceq);

            string mode=get_string_data(data[data_index],""); data_index++;
            VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode = DY_VSC_INVALID_DC_CONTORL;
            if(mode=="Vdc_control")
                active_control_mode = DY_VSC_DC_VOLTAGE_CONTORL;
            if(mode=="Pac_control")
                active_control_mode = DY_VSC_AC_ACTIVE_POWER_CONTORL;

            set_converter_active_control_mode(active_control_mode);

            double p_kp = get_double_data(data[data_index],"0.0"); data_index++;
            double p_ki = get_double_data(data[data_index],"0.0"); data_index++;
            double p_kd = get_double_data(data[data_index],"0.0"); data_index++;
            double p_td = get_double_data(data[data_index],"0.0"); data_index++;
            double p_pmax = get_double_data(data[data_index],"0.0"); data_index++;
            double p_pmin = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_block_Kp(p_kp);
            set_active_power_block_Ki(p_ki);
            set_active_power_block_Kd(p_kd);
            set_active_power_block_Td_in_s(p_td);
            set_active_power_block_pmax(p_pmax);
            set_active_power_block_pmin(p_pmin);

            mode=get_string_data(data[data_index],""); data_index++;
            VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode = DY_VSC_INVALID_AC_CONTORL;
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

}

void VSCHVDCC0::initialize()
{
    if(not is_model_initialized())
    {
        VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
        setup_block_toolkit_and_parameters();

        unsigned int bus_number=get_converter_bus();

        complex<double> Is = vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(bus_number);
        complex<double> Us = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(bus_number);
        double angle = arg(Us);

        double Isd0 = Is.real()*cos(angle)+Is.imag()*sin(angle);
        double Isq0 = -Is.real()*sin(angle)+Is.imag()*cos(angle);

        VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_power_control_mode = get_converter_active_control_mode();
        VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_power_control_mode = get_converter_reactive_control_mode();

        p_block.set_output(Isd0);
        p_block.initialize();

        q_block.set_output(Isq0);
        q_block.initialize();

        double Udc = vsc_hvdc->get_dc_bus_Vdc_in_kV_with_dc_bus_number(bus_number);// here wrong
        ceq_block.set_output(Udc); // change to initial Udc
        ceq_block.initialize();

        double pref = 0.0;
        double qref = 0.0;
        double udcref = 0.0;
        double uacref = 0.0;
        switch(active_power_control_mode)
        {
            case DY_VSC_DC_VOLTAGE_CONTORL:
                udcref = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()));
                set_active_power_block_Pref(udcref);
                break;
            case DY_VSC_AC_ACTIVE_POWER_CONTORL:
                pref = vsc_hvdc->get_converter_P_to_AC_bus_in_MW(get_converter_index());
                set_active_power_block_Pref(pref);
                break;
            default:
                break;
        }

        switch(reactive_power_control_mode)
        {
            case DY_VSC_AC_VOLTAGE_CONTROL:
                uacref = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(get_converter_bus());
                set_reactive_power_block_Qref(uacref);
                break;
            case DY_VSC_AC_REACTIVE_POWER_CONTROL:
                qref = vsc_hvdc->get_converter_Q_to_AC_bus_in_MVar(get_converter_index());
                set_reactive_power_block_Qref(qref);
                break;
            default:
                break;
        }

    }
}

void VSCHVDCC0::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
        unsigned int bus_number=get_converter_bus();
        unsigned int converter_index=vsc_hvdc->get_converter_index_with_ac_bus(bus_number);
        complex<double> Is = vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(bus_number);
        complex<double> Us = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(bus_number);
        complex<double> j(0.0,1.0);
        double Ps = (Us * conj(Is)).real();
        double Qs = (Us * conj(Is)).imag();
        double Isd0 = Is.real();
        double Isq0 = Is.imag();

        get dc power into Ceq
        get Pac = Ps + Ploss

        double input = (Pdc-Pdc)/Udc;
        udc_block.set_input(input);
        udc_block.run(mode);

        VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_power_control_mode = get_converter_active_control_mode();
        VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_power_control_mode = get_converter_reactive_control_mode();

        double input = 0.0;
        unsigned int dc_bus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(converter_index);
        switch(active_power_control_mode)
        {
            case DY_VSC_DC_VOLTAGE_CONTORL:
                input = get_active_power_block_Udcref() - vsc_hvdc->get_dc_bus_Vdc_in_kV(dc_bus_index);
                break;
            case DY_VSC_AC_ACTIVE_POWER_CONTORL:
                input = get_active_power_block_Pref() - Ps;
                break;
            default:
                break;
        }

        p_block.set_input(input);
        p_block.run(mode);

        input = 0.0;
        switch(reactive_power_control_mode)
        {
            case DY_VSC_AC_VOLTAGE_CONTROL:
                input = get_reactive_power_block_Uacref() - abs(Us);
                break;
            case DY_VSC_AC_REACTIVE_POWER_CONTROL:
                input = get_reactive_power_block_Qref() - Qs;
                break;
            default:
                break;
        }
        q_block.set_input(input);
        q_block.run(mode);

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

void VSCHVDCC0::check()
{

}

void VSCHVDCC0::clear()
{

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


    osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<" /";
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

complex<double> VSCHVDCC0::get_converter_voltage_in_dq_axis_in_pu()
{
    ;
}

complex<double> VSCHVDCC0::get_converter_voltage_in_xy_axis_in_pu()
{
    ;
}

complex<double> VSCHVDCC0::get_converter_current_in_dq_axis_in_pu()
{
    ;
}

complex<double> VSCHVDCC0::get_converter_current_in_xy_axis_in_pu()
{
    ;
}

complex<double> VSCHVDCC0::get_converter_Norton_current_in_xy_axis_in_pu_based_on_SBASE()
{
    ;
}
