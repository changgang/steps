#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/VSCHVDCC0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
VSCHVDCC0::VSCHVDCC0(STEPS& toolkit):VSC_HVDC_CONVERTER_MODEL(toolkit),
                                     p_udc_block(toolkit),
                                     p_pac_block(toolkit),
                                     p_f_block(toolkit),
                                     pmax_block(toolkit),
                                     pmin_block(toolkit),
                                     q_uac_block(toolkit),
                                     q_qac_block(toolkit),
                                     qmax_block(toolkit),
                                     qmin_block(toolkit),
                                     ceq_block(toolkit)
{
    clear();
}

VSCHVDCC0::VSCHVDCC0(const VSCHVDCC0& model):VSC_HVDC_CONVERTER_MODEL(model.get_toolkit()),
                                             p_udc_block(model.get_toolkit()),
                                             p_pac_block(model.get_toolkit()),
                                             p_f_block(model.get_toolkit()),
                                             pmax_block(model.get_toolkit()),
                                             pmin_block(model.get_toolkit()),
                                             q_uac_block(model.get_toolkit()),
                                             q_qac_block(model.get_toolkit()),
                                             qmax_block(model.get_toolkit()),
                                             qmin_block(model.get_toolkit()),
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
    p_udc_block.set_toolkit(toolkit);
    p_pac_block.set_toolkit(toolkit);
    q_uac_block.set_toolkit(toolkit);
    q_qac_block.set_toolkit(toolkit);
    ceq_block.set_toolkit(toolkit);
    clear();

    converter_bus = model.get_converter_bus();
    VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode = model.get_converter_active_control_mode();
    VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode = model.get_converter_reactive_control_mode();
    this->set_converter_bus(converter_bus);
    this->set_converter_active_control_mode(active_control_mode);
    this->set_converter_reactive_control_mode(reactive_control_mode);
    if(active_control_mode==DY_VSC_DC_VOLTAGE_CONTORL)  //1-dc_voltage_control  2-active_power_control
    {
        if(reactive_control_mode==DY_VSC_AC_VOLTAGE_CONTROL)  //1-ac_voltage_control 2-reactive_power_control
        {
            double kp_ud = get_converter_dc_voltage_kpud();
            double Ti_ud = get_converter_dc_voltage_control_T_in_s();
            double max_ud = get_converter_dc_voltage_Umax();
            double min_ud = get_converter_dc_voltage_Umin();
            double kp_uc = get_converter_ac_voltage_kpuc();
            double Ti_uc = get_converter_ac_voltage_control_T_in_s();
            double max_uc = get_converter_ac_voltage_Umax();
            double min_uc = get_converter_dc_voltage_Umin();
            double ceq = get_dc_voltage_ceq();
            this->set_converter_dc_voltage_kpud(kp_ud);
            this->set_converter_dc_voltage_control_T_in_s(Ti_ud);
            this->set_converter_dc_voltage_Umax(max_ud);
            this->set_converter_dc_voltage_Umin(min_ud);
            this->set_converter_ac_voltage_kpuc(kp_uc);
            this->set_converter_ac_voltage_control_T_in_s(Ti_uc);
            this->set_converter_ac_voltage_Umax(max_uc);
            this->set_converter_dc_voltage_Umin(min_uc);
            this->set_dc_voltage_ceq(ceq);
        }
        if(reactive_control_mode==DY_VSC_AC_REACTIVE_POWER_CONTROL)
        {
            double kp_ud = get_converter_dc_voltage_kpud();
            double Ti_ud = get_converter_dc_voltage_control_T_in_s();
            double max_ud = get_converter_dc_voltage_Umax();
            double min_ud = get_converter_dc_voltage_Umin();
            double kp_Q = get_converter_reactive_power_kpq();
            double Ti_Q = get_converter_reactive_power_control_T_in_s();
            double max_Q = get_converter_reactive_power_Qmax_in_pu();
            double min_Q = get_converter_reactive_power_Qmin_in_pu();
            double ceq = get_dc_voltage_ceq();
            this->set_converter_dc_voltage_kpud(kp_ud);
            this->set_converter_dc_voltage_control_T_in_s(Ti_ud);
            this->set_converter_dc_voltage_Umax(max_ud);
            this->set_converter_dc_voltage_Umin(min_ud);
            this->set_converter_reactive_power_kpq(kp_Q);
            this->set_converter_reactive_power_control_T_in_s(Ti_Q);
            this->set_converter_reactive_power_Qmax_in_pu(max_Q);
            this->set_converter_reactive_power_Qmin_in_pu(min_Q);
            this->set_dc_voltage_ceq(ceq);
        }
    }
    if(active_control_mode==DY_VSC_AC_ACTIVE_POWER_CONTORL)
    {
        if(reactive_control_mode==DY_VSC_AC_VOLTAGE_CONTROL)
        {
            double kp_P = get_converter_active_power_kpp();
            double Ti_P = get_converter_active_power_control_T_in_s();
            double max_P = get_converter_active_power_Pmax_in_pu();
            double min_P = get_converter_active_power_Pmin_in_pu();
            double kp_Q = get_converter_ac_voltage_kpuc();
            double Ti_Q = get_converter_ac_voltage_control_T_in_s();
            double max_Q = get_converter_ac_voltage_Umax();
            double min_Q = get_converter_dc_voltage_Umin();
            double ceq = get_dc_voltage_ceq();
            this->set_converter_active_power_kpp(kp_P);
            this->set_converter_active_power_control_T_in_s(Ti_P);
            this->set_converter_active_power_Pmax_in_pu(max_P);
            this->set_converter_active_power_Pmin_in_pu(min_P);
            this->set_converter_ac_voltage_kpuc(kp_Q);
            this->set_converter_ac_voltage_control_T_in_s(Ti_Q);
            this->set_converter_ac_voltage_Umax(max_Q);
            this->set_converter_ac_voltage_Umin(min_Q);
            this->set_dc_voltage_ceq(ceq);
        }
        if(reactive_control_mode==DY_VSC_AC_REACTIVE_POWER_CONTROL)
        {
            double kp_P = get_converter_active_power_kpp();
            double Ti_P = get_converter_active_power_control_T_in_s();
            double max_P = get_converter_active_power_Pmax_in_pu();
            double min_P = get_converter_active_power_Pmin_in_pu();
            double kp_Q = get_converter_reactive_power_kpq();
            double Ti_Q = get_converter_reactive_power_control_T_in_s();
            double max_Q = get_converter_reactive_power_Qmax_in_pu();
            double min_Q = get_converter_reactive_power_Qmin_in_pu();
            double ceq = get_dc_voltage_ceq();
            this->set_converter_active_power_kpp(kp_P);
            this->set_converter_active_power_control_T_in_s(Ti_P);
            this->set_converter_active_power_Pmax_in_pu(max_P);
            this->set_converter_active_power_Pmin_in_pu(min_P);
            this->set_converter_reactive_power_kpq(kp_Q);
            this->set_converter_reactive_power_control_T_in_s(Ti_Q);
            this->set_converter_reactive_power_Qmax_in_pu(max_Q);
            this->set_converter_reactive_power_Qmin_in_pu(min_Q);
            this->set_dc_voltage_ceq(ceq);

        }
    }

}

string VSCHVDCC0::get_model_name() const
{
    return "VSCHVDCC0";
}

void VSCHVDCC0::set_converter_bus(const unsigned int bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(bus!=0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(bus))
        {
            converter_bus = bus;
        }
        else
        {
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<"converter_name"<<" side bus of vsc_hvdc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converter_bus = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<"converter_name"<<" bus of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        converter_bus = bus;
    }
}

void VSCHVDCC0::set_converter_active_control_mode(VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode)
{
    active_power_control_mode = active_control_mode;
}

void VSCHVDCC0::set_converter_reactive_control_mode(VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode)
{
    reactive_power_control_mode = reactive_control_mode;
}

void VSCHVDCC0::set_converter_active_power_control_T_in_s(const double t)
{
    p_pac_block.set_Ki(t);
}

void VSCHVDCC0::set_converter_active_power_Pmax_in_pu(const double Pmax)
{
    p_pac_block.set_upper_limit(Pmax);
}

void VSCHVDCC0::set_converter_active_power_Pmin_in_pu(const double Pmin)
{
    p_pac_block.set_lower_limit(Pmin);
}

void VSCHVDCC0::set_converter_active_power_kpp(const double k)
{
    p_pac_block.set_Kp(k);
}

void VSCHVDCC0::set_converter_reactive_power_control_T_in_s(const double t)
{
    q_qac_block.set_Ki(t);
}

void VSCHVDCC0::set_converter_reactive_power_Qmax_in_pu(const double Qmax)
{
    q_qac_block.set_upper_limit(Qmax);
}

void VSCHVDCC0::set_converter_reactive_power_Qmin_in_pu(const double Qmin)
{
    q_qac_block.set_lower_limit(Qmin);
}

void VSCHVDCC0::set_converter_reactive_power_kpq(const double k)
{
    q_qac_block.set_Kp(k);
}

void VSCHVDCC0::set_converter_dc_voltage_control_T_in_s(const double t)
{
    p_udc_block.set_Ki(t);
}

void VSCHVDCC0::set_converter_dc_voltage_Umax(const double Umax)
{
    p_udc_block.set_upper_limit(Umax);
}

void VSCHVDCC0::set_converter_dc_voltage_Umin(const double Umin)
{
    p_udc_block.set_lower_limit(Umin);
}

void VSCHVDCC0::set_converter_dc_voltage_kpud(const double k)
{
    p_udc_block.set_Kp(k);
}

void VSCHVDCC0::set_converter_ac_voltage_control_T_in_s(const double t)
{
    q_uac_block.set_Ki(t);
}

void VSCHVDCC0::set_converter_ac_voltage_Umax(const double Umax)
{
    q_uac_block.set_upper_limit(Umax);
}

void VSCHVDCC0::set_converter_ac_voltage_Umin(const double Umin)
{
    q_uac_block.set_lower_limit(Umin);
}

void VSCHVDCC0::set_converter_ac_voltage_kpuc(const double k)
{
    q_uac_block.set_Kp(k);
}

void VSCHVDCC0::set_dc_voltage_ceq(const double ceq)
{
    Ceq = ceq;
}

unsigned int VSCHVDCC0::get_converter_bus() const
{
    return converter_bus;
}

VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE VSCHVDCC0::get_converter_active_control_mode() const
{
    return active_power_control_mode;
}

VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE VSCHVDCC0::get_converter_reactive_control_mode() const
{
    return reactive_power_control_mode;
}

double VSCHVDCC0::get_converter_active_power_control_T_in_s() const
{
    return p_pac_block.get_Ki();
}

double VSCHVDCC0::get_converter_active_power_Pmax_in_pu() const
{
    return p_pac_block.get_upper_limit();
}

double VSCHVDCC0::get_converter_active_power_Pmin_in_pu() const
{
    return p_pac_block.get_lower_limit();
}

double VSCHVDCC0::get_converter_active_power_kpp() const
{
    return p_pac_block.get_Kp();
}

double VSCHVDCC0::get_converter_reactive_power_control_T_in_s() const
{
    return q_qac_block.get_Ki();
}

double VSCHVDCC0::get_converter_reactive_power_Qmax_in_pu() const
{
    return q_qac_block.get_upper_limit();
}

double VSCHVDCC0::get_converter_reactive_power_Qmin_in_pu() const
{
    return q_qac_block.get_lower_limit();
}

double VSCHVDCC0::get_converter_reactive_power_kpq() const
{
    return q_qac_block.get_Kp();
}

double VSCHVDCC0::get_converter_dc_voltage_control_T_in_s() const
{
    return p_udc_block.get_Ki();
}

double VSCHVDCC0::get_converter_dc_voltage_Umax() const
{
    return p_udc_block.get_upper_limit();
}

double VSCHVDCC0::get_converter_dc_voltage_Umin() const
{
    return p_udc_block.get_lower_limit();
}

double VSCHVDCC0::get_converter_dc_voltage_kpud() const
{
    return p_udc_block.get_Kp();
}

double VSCHVDCC0::get_converter_ac_voltage_control_T_in_s() const
{
    return q_uac_block.get_Ki();
}

double VSCHVDCC0::get_converter_ac_voltage_Umax() const
{
    return q_uac_block.get_upper_limit();
}

double VSCHVDCC0::get_converter_ac_voltage_Umin() const
{
    return q_uac_block.get_lower_limit();
}

double VSCHVDCC0::get_converter_ac_voltage_kpuc() const
{
    return q_uac_block.get_Kp();
}

double VSCHVDCC0::get_dc_voltage_ceq() const
{
    return Ceq;
}

bool VSCHVDCC0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=2)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=1;
            unsigned int converter_bus = get_integer_data(data[data_index],"0"); data_index++;
            VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode = DY_VSC_INVALID_DC_CONTORL;
            string type=get_string_data(data[data_index],""); data_index++;
            if(type=="Vdc_control")
                active_control_mode = DY_VSC_DC_VOLTAGE_CONTORL;
            if(type=="Pac_control")
                active_control_mode = DY_VSC_AC_ACTIVE_POWER_CONTORL;
            VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode = DY_VSC_INVALID_AC_CONTORL;
            string mode=get_string_data(data[data_index],""); data_index++;
            if(mode=="Vac_control")
                reactive_control_mode = DY_VSC_AC_VOLTAGE_CONTROL;
            if(mode=="Qac_control")
                reactive_control_mode = DY_VSC_AC_REACTIVE_POWER_CONTROL;
            set_converter_bus(converter_bus);
            set_converter_active_control_mode(active_control_mode);
            set_converter_reactive_control_mode(reactive_control_mode);
            if(active_control_mode==DY_VSC_DC_VOLTAGE_CONTORL)  //1-dc_voltage_control  2-active_power_control
            {
                if(reactive_control_mode==DY_VSC_AC_VOLTAGE_CONTROL)  //1-ac_voltage_control 2-reactive_power_control
                {
                    double kp_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double kp_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double ceq = get_double_data(data[data_index],"0.0");
                    set_converter_dc_voltage_control_T_in_s(Ti_ud);
                    set_converter_dc_voltage_Umax(max_ud);
                    set_converter_dc_voltage_Umin(min_ud);
                    set_converter_dc_voltage_kpud(kp_ud);
                    set_converter_ac_voltage_control_T_in_s(Ti_uc);
                    set_converter_ac_voltage_Umax(max_uc);
                    set_converter_ac_voltage_Umin(max_uc);
                    set_converter_ac_voltage_kpuc(kp_uc);
                    set_dc_voltage_ceq(ceq);
                }
                if(reactive_control_mode==DY_VSC_AC_REACTIVE_POWER_CONTROL)
                {
                    double kp_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_ud = get_double_data(data[data_index],"0.0"); data_index++;
                    double kp_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double ceq = get_double_data(data[data_index],"0.0");
                    set_converter_dc_voltage_control_T_in_s(Ti_ud);
                    set_converter_dc_voltage_Umax(max_ud);
                    set_converter_dc_voltage_Umin(min_ud);
                    set_converter_dc_voltage_kpud(kp_ud);
                    set_converter_reactive_power_kpq(kp_Q);
                    set_converter_reactive_power_control_T_in_s(Ti_Q);
                    set_converter_reactive_power_Qmax_in_pu(max_Q);
                    set_converter_reactive_power_Qmin_in_pu(min_Q);
                    set_dc_voltage_ceq(ceq);
                }
            }
            if(active_control_mode==DY_VSC_AC_ACTIVE_POWER_CONTORL)
            {
                if(reactive_control_mode==DY_VSC_AC_VOLTAGE_CONTROL)
                {
                    double kp_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double kp_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_uc = get_double_data(data[data_index],"0.0"); data_index++;
                    double ceq = get_double_data(data[data_index],"0.0");
                    set_converter_dc_voltage_control_T_in_s(Ti_P);
                    set_converter_dc_voltage_Umax(max_P);
                    set_converter_dc_voltage_Umin(min_P);
                    set_converter_dc_voltage_kpud(kp_P);
                    set_converter_ac_voltage_control_T_in_s(Ti_uc);
                    set_converter_ac_voltage_Umax(max_uc);
                    set_converter_ac_voltage_Umin(max_uc);
                    set_converter_ac_voltage_kpuc(kp_uc);
                    set_dc_voltage_ceq(ceq);
                }
                if(reactive_control_mode==DY_VSC_AC_REACTIVE_POWER_CONTROL)
                {
                    double kp_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_P = get_double_data(data[data_index],"0.0"); data_index++;
                    double kp_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double Ti_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double max_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double min_Q = get_double_data(data[data_index],"0.0"); data_index++;
                    double ceq = get_double_data(data[data_index],"0.0");
                    set_converter_dc_voltage_control_T_in_s(Ti_P);
                    set_converter_dc_voltage_Umax(max_P);
                    set_converter_dc_voltage_Umin(min_P);
                    set_converter_dc_voltage_kpud(kp_P);
                    set_converter_reactive_power_kpq(kp_Q);
                    set_converter_reactive_power_control_T_in_s(Ti_Q);
                    set_converter_reactive_power_Qmax_in_pu(max_Q);
                    set_converter_reactive_power_Qmin_in_pu(min_Q);
                    set_dc_voltage_ceq(ceq);
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
        if(active_power_control_mode == 1)  //1-dc_voltage_control  2-active_power_control
        {
            if(reactive_power_control_mode == 1)   //1-ac_voltage_control 2-reactive_power_control
            {
                ceq_block.set_output(0.0);
                ceq_block.initialize();
                p_udc_block.set_output(Isd0);
                p_udc_block.initialize();

                q_uac_block.set_output(Isq0);
                q_uac_block.initialize();

            }
            if(reactive_power_control_mode == 2)
            {
                ceq_block.set_output(0.0);
                ceq_block.initialize();
                p_udc_block.set_output(Isd0);
                p_udc_block.initialize();

                q_qac_block.set_output(Isq0);
                q_qac_block.initialize();
            }
        }

        if(active_power_control_mode == 2)
        {
            if(reactive_power_control_mode == 1)
            {
                p_pac_block.set_output(Isd0);
                p_pac_block.initialize();

                q_uac_block.set_output(Isq0);
                q_uac_block.initialize();
            }
            if(reactive_power_control_mode == 2)
            {
                p_pac_block.set_output(Isd0);
                p_pac_block.initialize();

                q_qac_block.set_output(Isq0);
                q_qac_block.initialize();
            }

        }

    }
}

void VSCHVDCC0::run(DYNAMIC_MODE mode)
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    setup_block_toolkit_and_parameters();
    unsigned int bus_number=get_converter_bus();
    unsigned int i=vsc_hvdc->get_converter_index_with_ac_bus(bus_number);
    complex<double> Is = vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(bus_number);
    complex<double> Us = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(bus_number);
    complex<double> j(0.0,1.0);
    double Ps = (Us * conj(Is)).real();
    double Qs = (Us * conj(Is)).imag();
    double Isd0 = Is.real();
    double Isq0 = Is.imag();

    if(active_power_control_mode == DY_VSC_DC_VOLTAGE_CONTORL)  //1-dc_voltage_control  2-active_power_control
    {
        if(reactive_power_control_mode == DY_VSC_AC_VOLTAGE_CONTROL)   //1-ac_voltage_control 2-reactive_power_control
        {
            double udcref = vsc_hvdc->get_converter_initial_dc_voltage_reference_in_kV(i);
            unsigned int dc_bus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(i);
            double udc = vsc_hvdc->get_dc_bus_Vdc_in_kV(dc_bus_index);
            double p_input = udcref-udc;
            p_udc_block.set_input(p_input);
            p_udc_block.run(mode);

            double Usref = vsc_hvdc->get_converter_nominal_ac_voltage_command_in_kV(i);
            double q_input = Usref - Us.real();
            q_uac_block.set_input(q_input);
            q_uac_block.run(mode);

        }
        if(reactive_power_control_mode == DY_VSC_AC_REACTIVE_POWER_CONTROL)
        {
            double udcref = vsc_hvdc->get_converter_initial_dc_voltage_reference_in_kV(i);
            unsigned int dc_bus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(i);
            double udc = vsc_hvdc->get_dc_bus_Vdc_in_kV(dc_bus_index);
            double p_input = udcref-udc;
            p_udc_block.set_input(p_input);
            p_udc_block.run(mode);

            double Qsref = vsc_hvdc->get_converter_nominal_reactive_power_command_in_pu(i);
            double q_input = Qsref-Qs;
            q_qac_block.set_input(q_input);
            q_qac_block.run(mode);
        }
    }

    if(active_power_control_mode == DY_VSC_AC_ACTIVE_POWER_CONTORL)
    {
        if(reactive_power_control_mode == DY_VSC_AC_VOLTAGE_CONTROL)
        {
            double Psref = vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(i);
            double p_input = Ps - Psref;
            p_pac_block.set_input(p_input);
            p_pac_block.run(mode);

            double Usref = vsc_hvdc->get_converter_nominal_ac_voltage_command_in_kV(i);
            double q_input = Usref - Us.real();
            q_uac_block.set_input(q_input);
            q_uac_block.run(mode);
        }
        if(reactive_power_control_mode == DY_VSC_AC_REACTIVE_POWER_CONTROL)
        {
            double Psref = vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(i);
            double p_input = Ps - Psref;
            p_pac_block.set_input(p_input);
            p_pac_block.run(mode);

            double Qsref = vsc_hvdc->get_converter_nominal_ac_reactive_power_command_in_Mvar(i);
            double q_input = Qs-Qsref;
            q_qac_block.set_input(q_input);
            q_qac_block.run(mode);
        }
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
