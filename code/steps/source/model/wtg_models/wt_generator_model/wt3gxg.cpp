#include "header/model/wtg_models/wt_generator_model/wt3gxg.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3GXG::WT3GXG(STEPS& toolkit) : WT_GENERATOR_MODEL(toolkit),
                               general_gfm(toolkit)
{
    clear();
}

WT3GXG::~WT3GXG()
{
}

void WT3GXG::clear()
{
    set_model_float_parameter_count(11);
    set_current_source_flag(false);
    set_gfm_control_mode(VSG_CONTROL);
    general_gfm.clear();
}

void WT3GXG::copy_from_const_model(const WT3GXG& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    general_gfm.set_toolkit(toolkit);

    clear();

    set_gfm_control_mode(model.get_gfm_control_mode());
    set_Kp(model.get_Kp());
    set_Tp_in_s(model.get_Tp_in_s());
    set_Kq(model.get_Kq());
    set_Tq_in_s(model.get_Tq_in_s());
    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(model.get_Imax());
}

WT3GXG::WT3GXG(const WT3GXG& model):WT_GENERATOR_MODEL(model.get_toolkit()),
                                 general_gfm(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3GXG& WT3GXG::operator=(const WT3GXG& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void WT3GXG::set_gfm_control_mode(GFM_CONTROL_MODE mode)
{
    this->control_mode = mode;
}

void WT3GXG::set_Kp(double Kp)
{
    general_gfm.set_Kp(Kp);
}

void WT3GXG::set_Tp_in_s(double Tp)
{
    general_gfm.set_Tp_in_s(Tp);
}

void WT3GXG::set_Kq(double Kq)
{
    general_gfm.set_Kq(Kq);
}

void WT3GXG::set_Tq_in_s(double Tq)
{
    general_gfm.set_Tq_in_s(Tq);
}

void WT3GXG::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    general_gfm.set_pq_priority_flag(flag);
}

void WT3GXG::set_Imax(double Imax)
{
    general_gfm.set_Imax(Imax);
}

GFM_CONTROL_MODE WT3GXG::get_gfm_control_mode() const
{
    return control_mode;
}

double WT3GXG::get_Kp() const
{
    return general_gfm.get_Kp();
}

double WT3GXG::get_Tp_in_s() const
{
    return general_gfm.get_Tp_in_s();
}

double WT3GXG::get_Kq() const
{
    return general_gfm.get_Kq();
}

double WT3GXG::get_Tq_in_s() const
{
    return general_gfm.get_Tq_in_s();
}

COL_PQ_PRIORITY WT3GXG::get_pq_priority_flag() const
{
    return general_gfm.get_pq_priority_flag();
}

double WT3GXG::get_Imax() const
{
    return general_gfm.get_Imax();
}

string WT3GXG::get_model_name() const
{
    return "WT3GXG";
}

bool WT3GXG::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=11)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus, n_lumped_turbine;
            string id;
            double prate, num1, num2, num3, num4, imax;
            unsigned int pqflag, mode;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            prate = get_double_data(data[i],"0.0"); i++;
            mode = (unsigned int)(get_integer_data(data[i],"0.0")); i++;
            num1 = get_double_data(data[i],"0.0"); i++;
            num2 = get_double_data(data[i],"0.0"); i++;
            num3 = get_double_data(data[i],"0.0"); i++;
            num4 = get_double_data(data[i],"0.0"); i++;
            pqflag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            imax = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_wt_generator_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            WT_GENERATOR* gen = psdb.get_pointer_to_wt_generator(did);

            if(gen==NULL)
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
            double mbase = gen->get_mbase_in_MVA();
            n_lumped_turbine = round(mbase/prate);
            if(fabs(mbase-n_lumped_turbine*prate)>1e-2)
            {
                osstream<<"Warning. The MBASE of "<<did.get_compound_device_name()<<" is far way from n times of the Prate of "<<get_model_name()<<" model."<<endl
                       <<"MBASE = "<<mbase<<" MVA and Prate = "<<prate<<" MW."<<endl
                       <<"Machine MBASE will be updated as "<<n_lumped_turbine*prate<<" MVA.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            gen->set_number_of_lumped_wt_generators(n_lumped_turbine);
            gen->set_rated_power_per_wt_generator_in_MW(prate);
            gen->set_mbase_in_MVA(n_lumped_turbine*prate);

            switch(mode)
            {
                case 2:
                    {
                        //vsg control : mode, num1, num2, num3, num4
                        //                 2,   Tj,    D,   Ku,   Te
                        set_gfm_control_mode(VSG_CONTROL);
                        set_Kp(1 / num2);
                        set_Kq(1 / num3);
                        set_Tp_in_s(num1 / num2);
                        set_Tq_in_s(num4 / num3);
                        break;
                    }
                case 1:
                    {
                        //traditional droop with filter control : mode, num1, num2, num3, num4
                        //                                           1,   Kp,   Kq,   wp,   wq
                        set_gfm_control_mode(DROOP_WITH_FILTER_CONTROL);
                        set_Kp(num1);
                        set_Kq(num2);
                        set_Tp_in_s(1 / num3);
                        set_Tq_in_s(1 / num4);
                        break;
                    }
                case 0:
                default:
                    {
                        //traditional droop control : mode, num1, num2, num3, num4
                        //                               0,   Kp,   Kq,  0.0,  0.0
                        set_gfm_control_mode(TRADITIONAL_DROOP_CONTROL);
                        set_Kp(num1);
                        set_Kq(num2);
                        set_Tp_in_s(0.0);
                        set_Tq_in_s(0.0);
                        break;
                    }

            }
            switch(pqflag)
            {
                case 1:
                    set_pq_priority_flag(Q_FIRST);
                    break;
                case 0:
                default:
                    set_pq_priority_flag(P_FIRST);
                    break;
            }
            set_Imax(imax);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool WT3GXG::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3GXG::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3GXG::setup_block_toolkit_and_parameters()
{
    general_gfm.set_bus_pointer(get_bus_pointer());
    general_gfm.set_device_pointer(get_device_pointer());
}

void WT3GXG::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        setup_block_toolkit_and_parameters();

        double n_lumped = get_number_of_lumped_wt_generators();
        double P = wt_generator->get_p_generation_in_MW()/n_lumped;
        double Q = wt_generator->get_q_generation_in_MVar()/n_lumped;

        double prate = get_rated_power_per_wt_generator_in_MW();
        P /= prate;
        Q /= prate;

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

        general_gfm.set_initial_P_generation_in_pu(P);
        general_gfm.set_initial_Q_generation_in_pu(Q);
        general_gfm.set_source_impedance_in_pu_based_on_mbase(Zsource);
        general_gfm.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active power command = "<<get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive power command = "<<get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    active_power_regulator_block state: "<<general_gfm.get_active_power_regulator_block_state()<<endl
                    <<"    reactive_power_regulator_block state: "<<general_gfm.get_reactive_power_regulator_block_state()<<endl
                    <<"    virtual_angle_block state: "<<general_gfm.get_virtual_angle_block_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu"<<endl
                    <<"(8) terminal voltage :"<<get_terminal_voltage_in_pu()<<"pu"<<endl
                    <<"(9) Kp :"<<get_Kp()<<endl
                    <<"(10) Tp :"<<get_Tp_in_s()<<endl
                    <<"(11) Kq :"<<get_Kq()<<endl
                    <<"(12) Tq :"<<get_Tq_in_s();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3GXG::check_limits_in_initialization()
{
    return;
}

void WT3GXG::run(DYNAMIC_MODE mode)
{
    double Pref = get_active_power_command_in_pu_based_on_mbase();
    double Qref = get_reactive_power_command_in_pu_based_on_mbase();

    general_gfm.set_Pref_in_pu_based_on_mbase(Pref);
    general_gfm.set_Qref_in_pu_based_on_mbase(Qref);
    general_gfm.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3GXG::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return general_gfm.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double WT3GXG::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return general_gfm.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> WT3GXG::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return general_gfm.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> WT3GXG::get_internal_voltage_in_pu_in_xy_axis()
{
    return general_gfm.get_internal_voltage_in_pu_in_xy_axis();
}

double WT3GXG::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return general_gfm.get_initial_active_current_command_in_pu_based_on_mbase();
}

double WT3GXG::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return general_gfm.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double WT3GXG::get_initial_reactive_voltage_command_in_pu() const
{
    return general_gfm.get_initial_reactive_voltage_command_in_pu();
}

double WT3GXG::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return general_gfm.get_initial_active_power_command_in_pu_based_on_mbase();
}

double WT3GXG::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return general_gfm.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void WT3GXG::check()
{
    ;
}

void WT3GXG::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3GXG::save()
{
    ;
}

string WT3GXG::get_standard_psse_string(bool export_internal_bus_number) const
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

    double num1, num2, num3, num4;
    switch(control_mode)
    {
        case VSG_CONTROL:
            {
                num1 = get_Tp_in_s() / get_Kp();
                num2 = 1 / get_Kp();
                num3 = 1 / get_Kq();
                num4 = get_Tq_in_s() / get_Kq();
                break;
            }
        case DROOP_WITH_FILTER_CONTROL:
            {
                num1 = get_Kp();
                num2 = get_Kq();
                num3 = 1 / get_Tp_in_s();
                num4 = 1 / get_Tq_in_s();
                break;
            }
        case TRADITIONAL_DROOP_CONTROL:
        default:
            {
                num1 = get_Kp();
                num2 = get_Kq();
                num3 = 0.0;
                num4 = 0.0;
                break;
            }
    }
    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<", "
            <<setw(8)<<setprecision(6)<<get_gfm_control_mode()<<", "
            <<setw(8)<<setprecision(6)<<num1<<", "
            <<setw(8)<<setprecision(6)<<num2<<", "
            <<setw(8)<<setprecision(6)<<num3<<", "
            <<setw(8)<<setprecision(6)<<num4<<", "
            <<setw(8)<<setprecision(6)<<get_pq_priority_flag()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Imax()<<" /";

    return osstream.str();
}

void WT3GXG::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PN", i); i++;
    add_model_data_name_and_index_pair("GFM CONTROL MODE", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("TP", i); i++;
    add_model_data_name_and_index_pair("KQ", i); i++;
    add_model_data_name_and_index_pair("TQ", i); i++;
    add_model_data_name_and_index_pair("PQ PRIORITY FLAG", i); i++;
    add_model_data_name_and_index_pair("IMAX", i);
}

double WT3GXG::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
        return get_rated_power_per_wt_generator_in_MW();
    if(par_name=="GFM CONTROL MODE")
        return get_gfm_control_mode();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="TP")
        return get_Tp_in_s();
    if(par_name=="KQ")
        return get_Kq();
    if(par_name=="TQ")
        return get_Tq_in_s();
    if(par_name=="PQ PRIORITY FLAG")
        return get_pq_priority_flag();
    if(par_name=="IMAX")
        return get_Imax();

    return 0.0;
}

void WT3GXG::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_rated_power_per_wt_generator_in_MW(value);
    }
    if(par_name=="GFM CONTROL MODE")
    {
        int ivalue = int(value);
        GFM_CONTROL_MODE mode = VSG_CONTROL;
        switch(ivalue)
        {
            case 2:
                mode = VSG_CONTROL;
                break;
            case 1:
                mode = DROOP_WITH_FILTER_CONTROL;
                break;
            case 0:
            default:
                mode = TRADITIONAL_DROOP_CONTROL;
                break;
        }
        return set_gfm_control_mode(mode);
    }
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="TP")
        return set_Tp_in_s(value);
    if(par_name=="KQ")
        return set_Kq(value);
    if(par_name=="TQ")
        return set_Tq_in_s(value);
    if(par_name=="PQ PRIORITY FLAG")
    {
        int ivalue = int(value);
        COL_PQ_PRIORITY flag = P_FIRST;
        switch(ivalue)
        {
            case 1:
                flag = Q_FIRST;
                break;
            case 0:
            default:
                flag = P_FIRST;
                break;
        }
        return set_pq_priority_flag(flag);
    }
    if(par_name=="IMAX")
        return set_Imax(value);

    return;
}

double WT3GXG::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tp_in_s()!=0.0 and mint>get_Tp_in_s())
        mint = get_Tp_in_s();
    if(get_Tq_in_s()!=0.0 and mint>get_Tq_in_s())
        mint = get_Tq_in_s();
    return mint;
}

void WT3GXG::prepare_model_internal_variable_table()
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
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE POWER REGULATOR BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE POWER REGULATOR BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
}

double WT3GXG::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "VIRTUAL VOLTAGE IN PU")
        return general_gfm.get_virtual_voltage_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN PU")
        return general_gfm.get_virtual_frequency_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN HZ")
        return general_gfm.get_virtual_frequency_in_Hz();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN PU")
        return general_gfm.get_virtual_frequency_deviation_in_pu();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN HZ")
        return general_gfm.get_virtual_frequency_deviation_in_Hz();
    if(var_name == "VIRTUAL ANGLE IN DEG")
        return general_gfm.get_virtual_angle_in_deg();
    if(var_name == "VIRTUAL ANGLE IN RAD")
        return general_gfm.get_virtual_angle_in_rad();
    if(var_name == "STATE@ACTIVE POWER REGULATOR BLOCK")
        return general_gfm.get_active_power_regulator_block_state();
    if(var_name == "STATE@REACTIVE POWER REGULATOR BLOCK")
        return general_gfm.get_reactive_power_regulator_block_state();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return general_gfm.get_virtual_angle_block_state();
    return 0.0;
}

string WT3GXG::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3GXG::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3GXG::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
