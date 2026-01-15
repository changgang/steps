#include "header/model/energy_storage_model/es_converter_model/escvxg1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

ESCVXG1::ESCVXG1(STEPS& toolkit) : ES_CONVERTER_MODEL(toolkit),
                               general_gfm1(toolkit)
{
    clear();
}

ESCVXG1::~ESCVXG1()
{
}

void ESCVXG1::clear()
{
    set_model_float_parameter_count(24);
    set_current_source_flag(false);
    set_control_mode(VSG_CONTROL_MODE);
    general_gfm1.clear();
}

void ESCVXG1::copy_from_const_model(const ESCVXG1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    general_gfm1.set_toolkit(toolkit);

    clear();

    set_control_mode(model.get_control_mode());
    set_voltage_flag(model.get_voltage_flag());
    set_Kp(model.get_Kp());
    set_Tp_in_s(model.get_Tpf_in_s());
    set_Tpf_in_s(model.get_Tpf_in_s());
    set_Tqf_in_s(model.get_Tqf_in_s());
    set_Tvf_in_s(model.get_Tvf_in_s());
    set_mq(model.get_mq());
    set_mv(model.get_mv());
    set_Kpv(model.get_Kpv());
    set_Kiv(model.get_Kiv());
    set_Emax(model.get_Emax());
    set_Emin(model.get_Emin());
    set_current_limit_mode(model.get_current_limit_mode());
    set_dq_oriented_flag(model.get_dq_oriented_flag());
    set_phi_i_in_deg(model.get_phi_i_in_deg());
    set_Imax(model.get_Imax());
    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

ESCVXG1::ESCVXG1(const ESCVXG1& model):ES_CONVERTER_MODEL(model.get_toolkit()),
                                 general_gfm1(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESCVXG1& ESCVXG1::operator=(const ESCVXG1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void ESCVXG1::set_control_mode(CONTROL_MODE mode)
{
    this->control_mode = mode;
}

void ESCVXG1::set_voltage_flag(unsigned int flag)
{
    general_gfm1.set_voltage_flag(flag);
}

void ESCVXG1::set_Kp(double Kp)
{
    general_gfm1.set_Kp(Kp);
}

void ESCVXG1::set_Tp_in_s(double Tp)
{
    general_gfm1.set_Tp_in_s(Tp);
}

void ESCVXG1::set_Tpf_in_s(double Tpf)
{
    general_gfm1.set_Tpf_in_s(Tpf);
}

void ESCVXG1::set_Tqf_in_s(double Tqf)
{
    general_gfm1.set_Tqf_in_s(Tqf);
}

void ESCVXG1::set_Tvf_in_s(double Tvf)
{
    general_gfm1.set_Tvf_in_s(Tvf);
}

void ESCVXG1::set_mq(double mq)
{
    general_gfm1.set_mq(mq);
}

void ESCVXG1::set_mv(double mv)
{
    general_gfm1.set_mv(mv);
}

void ESCVXG1::set_Kpv(double Kpv)
{
    general_gfm1.set_Kpv(Kpv);
}

void ESCVXG1::set_Kiv(double Kiv)
{
    general_gfm1.set_Kiv(Kiv);
}

void ESCVXG1::set_Emax(double Emax)
{
    general_gfm1.set_Emax(Emax);
}

void ESCVXG1::set_Emin(double Emin)
{
    general_gfm1.set_Emin(Emin);
}

void ESCVXG1::set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode)
{
    general_gfm1.set_current_limit_mode(current_limit_mode);
}

void ESCVXG1::set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag)
{
    general_gfm1.set_dq_oriented_flag(dq_oriented_flag);
}

void ESCVXG1::set_phi_i_in_deg(double phi_i_in_deg)
{
    general_gfm1.set_phi_i_in_deg(phi_i_in_deg);
}

void ESCVXG1::set_Imax(double Imax)
{
    general_gfm1.set_Imax(Imax);
}

void ESCVXG1::set_KPLL(double Kp)
{
    general_gfm1.set_KPLL(Kp);
}

void ESCVXG1::set_KIPLL(double Ki)
{
    general_gfm1.set_KIPLL(Ki);
}

void ESCVXG1::set_PLLmax(double pmax)
{
    general_gfm1.set_PLLmax(pmax);
}

void ESCVXG1::set_PLLmin(double pmin)
{
    general_gfm1.set_PLLmin(pmin);
}

CONTROL_MODE ESCVXG1::get_control_mode() const
{
    return control_mode;
}

unsigned int ESCVXG1::get_voltage_flag() const
{
    return general_gfm1.get_voltage_flag();
}

double ESCVXG1::get_Kp() const
{
    return general_gfm1.get_Kp();
}

double ESCVXG1::get_Tp_in_s() const
{
    return general_gfm1.get_Tp_in_s();
}

double ESCVXG1::get_Tpf_in_s() const
{
    return general_gfm1.get_Tpf_in_s();
}

double ESCVXG1::get_Tqf_in_s() const
{
    return general_gfm1.get_Tqf_in_s();
}

double ESCVXG1::get_Tvf_in_s() const
{
    return general_gfm1.get_Tvf_in_s();
}

double ESCVXG1::get_mq() const
{
    return general_gfm1.get_mq();
}

double ESCVXG1::get_mv() const
{
    return general_gfm1.get_mv();
}

double ESCVXG1::get_Kpv() const
{
    return general_gfm1.get_Kpv();
}

double ESCVXG1::get_Kiv() const
{
    return general_gfm1.get_Kiv();
}

double ESCVXG1::get_Emax() const
{
    return general_gfm1.get_Emax();
}

double ESCVXG1::get_Emin() const
{
    return general_gfm1.get_Emin();
}

CURRENT_LIMIT_MODE ESCVXG1::get_current_limit_mode() const
{
    return general_gfm1.get_current_limit_mode();
}

DQ_ORIENTED_FLAG ESCVXG1::get_dq_oriented_flag() const
{
    return general_gfm1.get_dq_oriented_flag();
}

double ESCVXG1::get_phi_i_in_deg() const
{
    return general_gfm1.get_phi_i_in_deg();
}

double ESCVXG1::get_phi_i_in_rad() const
{
    return general_gfm1.get_phi_i_in_rad();
}

double ESCVXG1::get_Imax() const
{
    return general_gfm1.get_Imax();
}

double ESCVXG1::get_KPLL() const
{
    return general_gfm1.get_KPLL();
}

double ESCVXG1::get_KIPLL() const
{
    return general_gfm1.get_KIPLL();
}

double ESCVXG1::get_PLLmax() const
{
    return general_gfm1.get_PLLmax();
}

double ESCVXG1::get_PLLmin() const
{
    return general_gfm1.get_PLLmin();
}

string ESCVXG1::get_model_name() const
{
    return "ESCVXG1";
}

bool ESCVXG1::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=24)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            unsigned int control_mode, voltage_flag, current_limit_mode, dq_oriented_flag;
            double num1, num2, Tpf, Tqf, Tvf, mq, mv, Kpv, Kiv, Emax, Emin;
            double phi_i_in_deg, imax, kpll, kipll, pllmax, pllmin;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            control_mode = (unsigned int)(get_integer_data(data[i],"0.0")); i++;
            voltage_flag = (unsigned int)(get_integer_data(data[i],"0.0")); i++;
            num1 = get_double_data(data[i],"0.0"); i++;
            num2 = get_double_data(data[i],"0.0"); i++;
            Tpf = get_double_data(data[i],"0.0"); i++;
            Tqf = get_double_data(data[i],"0.0"); i++;
            Tvf = get_double_data(data[i],"0.0"); i++;
            mq = get_double_data(data[i],"0.0"); i++;
            mv = get_double_data(data[i],"0.0"); i++;
            Kpv = get_double_data(data[i],"0.0"); i++;
            Kiv = get_double_data(data[i],"0.0"); i++;
            Emax = get_double_data(data[i],"0.0"); i++;
            Emin = get_double_data(data[i],"0.0"); i++;
            current_limit_mode = (unsigned int)(get_integer_data(data[i],"0.0")); i++;
            dq_oriented_flag = (unsigned int)(get_integer_data(data[i],"0.0")); i++;
            phi_i_in_deg = get_double_data(data[i],"0.0");i++;
            imax = get_double_data(data[i],"0.0");i++;
            kpll = get_double_data(data[i],"0.0"); i++;
            kipll = get_double_data(data[i],"0.0"); i++;
            pllmax = get_double_data(data[i],"0.0"); i++;
            pllmin = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_pointer_to_energy_storage(did);
            if(es==NULL)
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such energy storage exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }

            switch(control_mode)
            {
                case 1:
                    {
                        //vsg control : control_mode, num1, num2
                        //                         1,   Tj,   D
                        set_control_mode(VSG_CONTROL_MODE);
                        set_Kp(1 / num2);
                        set_Tp_in_s(num1 / num2);
                        break;
                    }
                case 0:
                default:
                    {
                        //droop control : control_mode, num1, num2
                        //                           0,  0.0,  mp
                        set_control_mode(DROOP_CONTROL_MODE);
                        set_Kp(num2);
                        set_Tp_in_s(0.0);
                        break;
                    }
            }
            set_voltage_flag(voltage_flag);
            set_Tpf_in_s(Tpf);
            set_Tqf_in_s(Tqf);
            set_Tvf_in_s(Tvf);
            set_mq(mq);
            set_mv(mv);
            set_Kpv(Kpv);
            set_Kiv(Kiv);
            set_Emax(Emax);
            set_Emin(Emin);
            switch(current_limit_mode)
            {
                case 0:
                {
                    set_current_limit_mode(DQ_PROPORTION_LIMIT);
                    break;
                }
                case 1:
                {
                    set_current_limit_mode(SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED);
                    break;
                }
                case 2:
                default:
                {
                    set_current_limit_mode(SPECIFIED_I_LIMITED);
                    break;
                }
            }
            switch(dq_oriented_flag)
            {
                case 0:
                {
                    set_dq_oriented_flag(ACL_ORIENTED);
                    break;
                }
                case 1:
                default:
                {
                    set_dq_oriented_flag(PLL_ORIENTED);
                    break;
                }
            }
            set_phi_i_in_deg(phi_i_in_deg);
            set_Imax(imax);
            set_KPLL(kpll);
            set_KIPLL(kipll);
            set_PLLmax(pllmax);
            set_PLLmin(pllmin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool ESCVXG1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESCVXG1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESCVXG1::setup_block_toolkit_and_parameters()
{
    general_gfm1.set_bus_pointer(get_bus_pointer());
    general_gfm1.set_device_pointer(get_device_pointer());
}

void ESCVXG1::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        ENERGY_STORAGE* es = get_energy_storage_pointer();
        setup_block_toolkit_and_parameters();

        double P = es->get_p_generation_in_MW();
        double Q = es->get_q_generation_in_MVar();

        double mbase = get_mbase_in_MVA();
        P /= mbase;
        Q /= mbase;

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

        general_gfm1.set_initial_P_generation_in_pu(P);
        general_gfm1.set_initial_Q_generation_in_pu(Q);
        general_gfm1.set_source_impedance_in_pu_based_on_mbase(Zsource);
        general_gfm1.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active power command(Pref) = "<<get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive power command(Qref) = "<<get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command(Vref) = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    active_power_sensor state: "<<general_gfm1.get_active_power_sensor_state()<<endl
                    <<"    active_power_regulator_block state: "<<general_gfm1.get_active_power_regulator_block_state()<<endl
                    <<"    virtual_angle_block state: "<<general_gfm1.get_virtual_angle_block_state()<<endl
                    <<"    reactive_power_sensor state: "<<general_gfm1.get_reactive_power_sensor_state()<<endl
                    <<"    terminal_voltage_sensor state: "<<general_gfm1.get_terminal_voltage_sensor_state()<<endl
                    <<"    reactive_power_regulator_block state: "<<general_gfm1.get_reactive_power_regulator_block_state()<<endl
                    <<"    PLL_frequency_integrator state: "<<general_gfm1.get_PLL_frequency_integrator_state()<<endl
                    <<"    PLL_angle_integrator state: "<<general_gfm1.get_PLL_angle_integrator_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu"<<endl
                    <<"(8) terminal voltage :"<<get_terminal_voltage_in_pu()<<"pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESCVXG1::check_limits_in_initialization()
{
    return;
}

void ESCVXG1::run(DYNAMIC_MODE mode)
{
    double Pref = get_active_power_command_in_pu_based_on_mbase();
    double Qref = get_reactive_power_command_in_pu_based_on_mbase();
    double Vref = get_reactive_voltage_command_in_pu();

    general_gfm1.set_Pref_in_pu_based_on_mbase(Pref);
    general_gfm1.set_Qref_in_pu_based_on_mbase(Qref);
    general_gfm1.set_Vref_in_pu(Vref);
    general_gfm1.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> ESCVXG1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return general_gfm1.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double ESCVXG1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    double Pe = general_gfm1.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();

    ES_MODE mode = get_energy_storage_mode();
    if(mode==ES_START_DISCHARGE_MODE or mode==ES_STOP_DISCHARGE_MODE or mode==ES_DISCHARGE_MODE)
    {
        if(Pe<0.0)
            Pe = 0.0;
    }
    else
    {
        if(mode==ES_START_CHARGE_MODE or mode==ES_STOP_CHARGE_MODE or mode==ES_CHARGE_MODE)
        {
            if(Pe>0.0)
                Pe = 0.0;
        }
        else
        {
            Pe = 0.0;
        }
    }
    return Pe;
}

complex<double> ESCVXG1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return general_gfm1.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> ESCVXG1::get_internal_voltage_in_pu_in_xy_axis()
{
    return general_gfm1.get_internal_voltage_in_pu_in_xy_axis();
}

double ESCVXG1::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return general_gfm1.get_initial_active_current_command_in_pu_based_on_mbase();
}

double ESCVXG1::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return general_gfm1.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double ESCVXG1::get_initial_reactive_voltage_command_in_pu() const
{
    return general_gfm1.get_initial_reactive_voltage_command_in_pu();
}

double ESCVXG1::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return general_gfm1.get_initial_active_power_command_in_pu_based_on_mbase();
}

double ESCVXG1::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return general_gfm1.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void ESCVXG1::check()
{
    ;
}

void ESCVXG1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESCVXG1::save()
{
    ;
}

string ESCVXG1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    double num1, num2;
    switch(control_mode)
    {
        case VSG_CONTROL_MODE:
            {
                num1 = get_Tp_in_s() / get_Kp();
                num2 = 1 / get_Kp();
                break;
            }
        case DROOP_CONTROL_MODE:
        default:
            {
                num1 = 0.0;
                num2 = get_Kp();
                break;
            }
    }
    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_control_mode()<<", "
            <<setw(8)<<setprecision(6)<<get_voltage_flag()<<", "
            <<setw(8)<<setprecision(6)<<num1<<", "
            <<setw(8)<<setprecision(6)<<num2<<", "
            <<setw(8)<<setprecision(6)<<get_Tpf_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Tqf_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Tvf_in_s()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_mq()<<", "
            <<setw(8)<<setprecision(6)<<get_mv()<<", "
            <<setw(8)<<setprecision(6)<<get_Kpv()<<", "
            <<setw(8)<<setprecision(6)<<get_Kiv()<<", "
            <<setw(8)<<setprecision(6)<<get_Emax()<<", "
            <<setw(8)<<setprecision(6)<<get_Emin()<<", "
            <<setw(8)<<setprecision(6)<<get_current_limit_mode()<<", "
            <<setw(8)<<setprecision(6)<<get_dq_oriented_flag()<<", "
            <<setw(8)<<setprecision(6)<<get_phi_i_in_deg()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Imax()<<", "
            <<setw(8)<<setprecision(6)<<get_KPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_KIPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_PLLmax()<<", "
            <<setw(8)<<setprecision(6)<<get_PLLmin()<<" / ";
    return osstream.str();
}

void ESCVXG1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("CONTROL MODE", i); i++;
    add_model_data_name_and_index_pair("VOLTAGE FLAG", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("TP", i); i++;
    add_model_data_name_and_index_pair("TPF", i); i++;
    add_model_data_name_and_index_pair("TQF", i); i++;
    add_model_data_name_and_index_pair("TVF", i); i++;
    add_model_data_name_and_index_pair("MQ", i); i++;
    add_model_data_name_and_index_pair("MV", i); i++;
    add_model_data_name_and_index_pair("KPV", i); i++;
    add_model_data_name_and_index_pair("KIV", i); i++;
    add_model_data_name_and_index_pair("EMAX", i); i++;
    add_model_data_name_and_index_pair("EMIN", i); i++;
    add_model_data_name_and_index_pair("CURRENT LIMIT MODE", i); i++;
    add_model_data_name_and_index_pair("DQ ORIENTED FLAG", i); i++;
    add_model_data_name_and_index_pair("PHI I IN DEG", i); i++;
    add_model_data_name_and_index_pair("IMAX", i); i++;
    add_model_data_name_and_index_pair("KPLL", i); i++;
    add_model_data_name_and_index_pair("KIPLL", i); i++;
    add_model_data_name_and_index_pair("PLLMAX", i); i++;
    add_model_data_name_and_index_pair("PLLMIN", i); i++;
}

double ESCVXG1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="CONTROL MODE")
        return get_control_mode();
    if(par_name=="VOLTAGE FLAG")
        return get_voltage_flag();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="TP")
        return get_Tp_in_s();
    if(par_name=="TPF")
        return get_Tpf_in_s();
    if(par_name=="TQF")
        return get_Tqf_in_s();
    if(par_name=="TVF")
        return get_Tvf_in_s();
    if(par_name=="MQ")
        return get_mq();
    if(par_name=="MV")
        return get_mv();
    if(par_name=="KPV")
        return get_Kpv();
    if(par_name=="KIV")
        return get_Kiv();
    if(par_name=="EMAX")
        return get_Emax();
    if(par_name=="EMIN")
        return get_Emin();
    if(par_name=="CURRENT LIMIT MODE")
        return get_current_limit_mode();
    if(par_name=="DQ ORIENTED FLAG")
        return get_dq_oriented_flag();
    if(par_name=="PHI I IN DEG")
        return get_phi_i_in_deg();
    if(par_name=="IMAX")
        return get_Imax();
    if(par_name=="KPLL")
        return get_KPLL();
    if(par_name=="KIPLL")
        return get_KIPLL();
    if(par_name=="PLLMAX")
        return get_PLLmax();
    if(par_name=="PLLMIN")
        return get_PLLmin();
    return 0.0;
}

void ESCVXG1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="GFM CONTROL MODE")
    {
        int ivalue = int(value);
        CONTROL_MODE mode = VSG_CONTROL_MODE;
        switch(ivalue)
        {
            case 1:
                mode = VSG_CONTROL_MODE;
                break;
            case 0:
            default:
                mode = DROOP_CONTROL_MODE;
                break;
        }
        return set_control_mode(mode);
    }
    if(par_name == "VOLTAGE FLAG")
    {
        return set_voltage_flag((unsigned int)(value));
    }
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="TP")
        return set_Tp_in_s(value);
    if(par_name=="TPF")
        return set_Tpf_in_s(value);
    if(par_name=="TQF")
        return set_Tqf_in_s(value);
    if(par_name=="TVF")
        return set_Tvf_in_s(value);
    if(par_name=="MQ")
        return set_mq(value);
    if(par_name=="MV")
        return set_mv(value);
    if(par_name=="KPV")
        return set_Kpv(value);
    if(par_name=="KIV")
        return set_Kiv(value);
    if(par_name=="EMAX")
        return set_Emax(value);
    if(par_name=="EMIN")
        return set_Emin(value);
    if(par_name=="CURRENT LIMIT MODE")
    {
        int ivalue = int(value);
        CURRENT_LIMIT_MODE mode = SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED;
        switch(ivalue)
        {
            case 0:
                mode = DQ_PROPORTION_LIMIT;
                break;
            case 1:
                mode = SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED;
                break;
            case 2:
            default:
                mode = SPECIFIED_I_LIMITED;
                break;
        }
        return set_current_limit_mode(mode);
    }
    if(par_name=="DQ ORIENTED FLAG")
    {
        int ivalue = int(value);
        DQ_ORIENTED_FLAG flag = ACL_ORIENTED;
        switch(ivalue)
        {
            case 0:
                flag = ACL_ORIENTED;
                break;
            case 1:
            default:
                flag = PLL_ORIENTED;
                break;
        }
        return set_dq_oriented_flag(flag);
    }
    if(par_name=="PHI I IN DEG")
        return set_phi_i_in_deg(value);
    if(par_name=="IMAX")
        return set_Imax(value);
    if(par_name=="KPLL")
        return set_KPLL(value);
    if(par_name=="KIPLL")
        return set_KIPLL(value);
    if(par_name=="PLLMAX")
        return set_PLLmax(value);
    if(par_name=="PLLMIN")
        return set_PLLmin(value);

    return;
}

double ESCVXG1::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tp_in_s()!=0.0 and mint>get_Tp_in_s())
        mint = get_Tp_in_s();
    if(get_Tpf_in_s()!=0.0 and mint>get_Tpf_in_s())
        mint = get_Tpf_in_s();
    if(get_Tqf_in_s()!=0.0 and mint>get_Tqf_in_s())
        mint = get_Tqf_in_s();
    if(get_Tvf_in_s()!=0.0 and mint>get_Tvf_in_s())
        mint = get_Tvf_in_s();
    return mint;
}

void ESCVXG1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("VIRTUAL VOLTAGE IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL FREQUENCY IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL FREQUENCY IN HZ", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL FREQUENCY DEVIATION IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL FREQUENCY DEVIATION IN HZ", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("VIRTUAL ANGLE IN RAD", i); i++;

    add_model_internal_variable_name_and_index_pair("LIMITED INTERNAL VOLTAGE AMPLITUDE IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("LIMITED INTERNAL VOLTAGE ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("LIMITED INTERNAL VOLTAGE ANGLE IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("UNLIMITED CURRENT AMPLITUDE IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("UNLIMITED CURRENT ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("UNLIMITED CURRENT ANGLE IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("LIMITED CURRENT AMPLITUDE IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("LIMITED CURRENT ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("LIMITED CURRENT ANGLE IN RAD", i); i++;

    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE POWER SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE POWER REGULATOR BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE POWER SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@TERMINAL VOLTAGE SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE POWER REGULATOR BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL FREQUENCY INTEGRATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL ANGLE INTEGRATOR", i); i++;

}

double ESCVXG1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "VIRTUAL VOLTAGE IN PU")
        return general_gfm1.get_virtual_voltage_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN PU")
        return general_gfm1.get_virtual_frequency_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN HZ")
        return general_gfm1.get_virtual_frequency_in_Hz();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN PU")
        return general_gfm1.get_virtual_frequency_deviation_in_pu();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN HZ")
        return general_gfm1.get_virtual_frequency_deviation_in_Hz();
    if(var_name == "VIRTUAL ANGLE IN DEG")
        return general_gfm1.get_virtual_angle_in_deg();
    if(var_name == "VIRTUAL ANGLE IN RAD")
        return general_gfm1.get_virtual_angle_in_rad();

    if(var_name == "LIMITED INTERNAL VOLTAGE AMPLITUDE IN PU")
        return steps_fast_complex_abs(general_gfm1.get_internal_voltage_in_pu_in_xy_axis());
    if(var_name == "LIMITED INTERNAL VOLTAGE ANGLE IN DEG")
        return rad2deg(steps_fast_complex_arg(general_gfm1.get_internal_voltage_in_pu_in_xy_axis()));
    if(var_name == "LIMITED INTERNAL VOLTAGE ANGLE IN RAD")
        return steps_fast_complex_arg(general_gfm1.get_internal_voltage_in_pu_in_xy_axis());
    if(var_name == "UNLIMITED CURRENT AMPLITUDE IN PU")
        return steps_fast_complex_abs(general_gfm1.get_unlimited_xy_current_order());
    if(var_name == "UNLIMITED CURRENT ANGLE IN DEG")
        return rad2deg(steps_fast_complex_arg(general_gfm1.get_unlimited_xy_current_order()));
    if(var_name == "UNLIMITED CURRENT ANGLE IN RAD")
        return steps_fast_complex_arg(general_gfm1.get_unlimited_xy_current_order());
    if(var_name == "LIMITED CURRENT AMPLITUDE IN PU")
        return steps_fast_complex_abs(general_gfm1.get_limited_xy_current_order());
    if(var_name == "LIMITED CURRENT ANGLE IN DEG")
        return rad2deg(steps_fast_complex_arg(general_gfm1.get_limited_xy_current_order()));
    if(var_name == "LIMITED CURRENT ANGLE IN RAD")
        return steps_fast_complex_arg(general_gfm1.get_limited_xy_current_order());

    if(var_name == "STATE@ACTIVE POWER SENSOR")
        return general_gfm1.get_active_power_sensor_state();
    if(var_name == "STATE@ACTIVE POWER REGULATOR BLOCK")
        return general_gfm1.get_active_power_regulator_block_state();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return general_gfm1.get_virtual_angle_block_state();
    if(var_name == "STATE@REACTIVE POWER SENSOR")
        return general_gfm1.get_reactive_power_sensor_state();
    if(var_name == "STATE@TERMINAL VOLTAGE SENSOR")
        return general_gfm1.get_terminal_voltage_sensor_state();
    if(var_name == "STATE@REACTIVE POWER REGULATOR BLOCK")
        return general_gfm1.get_reactive_power_regulator_block_state();
    if(var_name == "STATE@PLL FREQUENCY INTEGRATOR")
        return general_gfm1.get_PLL_frequency_integrator_state();
    if(var_name == "STATE@PLL ANGLE INTEGRATOR")
        return general_gfm1.get_PLL_angle_integrator_state();

    return 0.0;
}

string ESCVXG1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESCVXG1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESCVXG1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
