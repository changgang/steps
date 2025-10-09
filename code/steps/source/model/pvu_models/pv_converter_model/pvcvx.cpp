#include "header/model/pvu_models/pv_converter_model/pvcvx.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

PVCVX::PVCVX(STEPS& toolkit) : PV_CONVERTER_MODEL(toolkit),
                               vsg(toolkit)
{
    clear();
}

PVCVX::~PVCVX()
{
}

void PVCVX::clear()
{
    set_model_float_parameter_count(6);

    set_current_source_flag(false);

    vsg.clear();
}

void PVCVX::copy_from_const_model(const PVCVX& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    vsg.set_toolkit(toolkit);

    clear();

    set_Tj_in_s(model.get_Tj_in_s());
    set_D(model.get_D());
    set_Ku(model.get_Ku());
    set_Te_in_s(model.get_Te_in_s());

    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(model.get_Imax());
}

PVCVX::PVCVX(const PVCVX& model):PV_CONVERTER_MODEL(model.get_toolkit()),
                                 vsg(model.get_toolkit())
{
    copy_from_const_model(model);
}

PVCVX& PVCVX::operator=(const PVCVX& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void PVCVX::set_Tj_in_s(double T)
{
    vsg.set_Tj_in_s(T);
}

void PVCVX::set_D(double D)
{
    vsg.set_D(D);
}

void PVCVX::set_Ku(double Ku)
{
    vsg.set_Ku(Ku);
}

void PVCVX::set_Te_in_s(double T)
{
    vsg.set_Te_in_s(T);
}

void PVCVX::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    vsg.set_pq_priority_flag(flag);
}

void PVCVX::set_Imax(double Imax)
{
    vsg.set_Imax(Imax);
}

double PVCVX::get_Tj_in_s() const
{
    return vsg.get_Tj_in_s();
}

double PVCVX::get_D() const
{
    return vsg.get_D();
}

double PVCVX::get_Ku() const
{
    return vsg.get_Ku();
}

double PVCVX::get_Te_in_s() const
{
    return vsg.get_Te_in_s();
}

COL_PQ_PRIORITY PVCVX::get_pq_priority_flag() const
{
    return vsg.get_pq_priority_flag();
}

double PVCVX::get_Imax() const
{
    return vsg.get_Imax();
}

string PVCVX::get_model_name() const
{
    return "PVCVX";
}

bool PVCVX::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=10)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus, n_lumped;
            string id;
            double tj, d, ku, te, prate, imax;
            unsigned int pqflag;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;

            prate = get_double_data(data[i],"0.0"); i++;
            tj = get_double_data(data[i],"0.0"); i++;
            d = get_double_data(data[i],"0.0"); i++;
            ku = get_double_data(data[i],"0.0"); i++;
            te = get_double_data(data[i],"0.0"); i++;
            pqflag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            imax = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_pv_unit_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            PV_UNIT* pv_unit = psdb.get_pv_unit(did);

            if(pv_unit==NULL)
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such pv unit exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
            double mbase = pv_unit->get_mbase_in_MVA();
            n_lumped = round(mbase/prate);
            if(fabs(mbase-n_lumped*prate)>1e-2)
            {
                osstream<<"Warning. The MBASE of "<<did.get_compound_device_name()<<" is far way from n times of the Prate of "<<get_model_name()<<" model."<<endl
                       <<"MBASE = "<<mbase<<" MVA and Prate = "<<prate<<" MW."<<endl
                       <<"Machine MBASE will be updated as "<<n_lumped*prate<<" MVA.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            pv_unit->set_number_of_lumped_pv_units(n_lumped);
            pv_unit->set_rated_power_per_pv_unit_in_MW(prate);
            pv_unit->set_mbase_in_MVA(n_lumped*prate);


            set_Tj_in_s(tj);
            set_D(d);
            set_Ku(ku);
            set_Te_in_s(te);
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

bool PVCVX::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PVCVX::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PVCVX::setup_block_toolkit_and_parameters()
{
    vsg.set_bus_pointer(get_bus_pointer());
    vsg.set_device_pointer(get_device_pointer());
}

void PVCVX::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        PV_UNIT* pv_unit = get_pv_unit_pointer();
        setup_block_toolkit_and_parameters();

        double n_lumped = get_number_of_lumped_pv_units();
        double P = pv_unit->get_p_generation_in_MW()/n_lumped;
        double Q = pv_unit->get_q_generation_in_MVar()/n_lumped;

        double prate = get_rated_power_per_pv_unit_in_MW();
        P /= prate;
        Q /= prate;

        complex<double> Zsource = get_source_impedance_in_pu_based_on_mbase();

        vsg.set_initial_P_generation_in_pu(P);
        vsg.set_initial_Q_generation_in_pu(Q);
        vsg.set_source_impedance_in_pu_based_on_mbase(Zsource);
        vsg.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active power command = "<<get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive power command = "<<get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    virtual_frequency_deviation block state: "<<vsg.get_virtual_frequency_deviation_block_state()<<endl
                    <<"    virtual_angle block state: "<<vsg.get_virtual_angle_block_state()<<endl
                    <<"    virtual_voltage block state: "<<vsg.get_virtual_voltage_block_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu"<<endl
                    <<"(8) terminal voltage :"<<get_terminal_voltage_in_pu()<<"pu";///////////////////////////////////
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void PVCVX::check_limits_in_initialization()
{
    return;
}

void PVCVX::run(DYNAMIC_MODE mode)
{
    double Pref = get_active_power_command_in_pu_based_on_mbase();
    double Qref = get_reactive_power_command_in_pu_based_on_mbase();
    double Vref = get_reactive_voltage_command_in_pu();

    vsg.set_Pref_in_pu_based_on_mbase(Pref);
    vsg.set_Qref_in_pu_based_on_mbase(Qref);
    vsg.set_Vref_in_pu(Vref);
    vsg.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> PVCVX::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return vsg.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double PVCVX::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return vsg.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> PVCVX::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return vsg.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> PVCVX::get_internal_voltage_in_pu_in_xy_axis()
{
    return vsg.get_internal_voltage_in_pu_in_xy_axis();
}

double PVCVX::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return vsg.get_initial_active_current_command_in_pu_based_on_mbase();
}

double PVCVX::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return vsg.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double PVCVX::get_initial_reactive_voltage_command_in_pu() const
{
    return vsg.get_initial_reactive_voltage_command_in_pu();
}

double PVCVX::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return vsg.get_initial_active_power_command_in_pu_based_on_mbase();
}

double PVCVX::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return vsg.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void PVCVX::check()
{
    ;
}

void PVCVX::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PVCVX::save()
{
    ;
}

string PVCVX::get_standard_psse_string(bool export_internal_bus_number) const
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

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_rated_power_per_pv_unit_in_MW()<<", "
            <<setw(8)<<setprecision(6)<<get_Tj_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Ku()<<", "
            <<setw(8)<<setprecision(6)<<get_Te_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_pq_priority_flag()<<", "
            <<setw(8)<<setprecision(6)<<get_Imax()<<" /";

    return osstream.str();
}

void PVCVX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PN", i); i++;
    add_model_data_name_and_index_pair("TJ", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
    add_model_data_name_and_index_pair("KU", i); i++;
    add_model_data_name_and_index_pair("TE", i); i++;
    add_model_data_name_and_index_pair("PQ PRIORITY FLAG", i); i++;
    add_model_data_name_and_index_pair("IMAX", i);
}

double PVCVX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
        return get_rated_power_per_pv_unit_in_MW();
    if(par_name=="TJ")
        return get_Tj_in_s();
    if(par_name=="D")
        return get_D();
    if(par_name=="KU")
        return get_Ku();
    if(par_name=="TE")
        return get_Te_in_s();
    if(par_name=="PQ PRIORITY FLAG")
        return get_pq_priority_flag();
    if(par_name=="IMAX")
        return get_Imax();

    return 0.0;
}

void PVCVX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
    {
        PV_UNIT* pv_unit = get_pv_unit_pointer();
        return pv_unit->set_rated_power_per_pv_unit_in_MW(value);
    }
    if(par_name=="TJ")
        return set_Tj_in_s(value);
    if(par_name=="D")
        return set_D(value);
    if(par_name=="KU")
        return set_Ku(value);
    if(par_name=="TE")
        return set_Te_in_s(value);
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

double PVCVX::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Te_in_s()!=0.0 and mint>get_Te_in_s())
        mint = get_Te_in_s();
    if(get_Tj_in_s()!=0.0 and mint>get_Tj_in_s())
        mint = get_Tj_in_s();
    return mint;
}

void PVCVX::prepare_model_internal_variable_table()
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
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL FREQUENCY BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL VOLTAGE BLOCK", i); i++;
}

double PVCVX::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "VIRTUAL VOLTAGE IN PU")
        return vsg.get_virtual_voltage_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN PU")
        return vsg.get_virtual_frequency_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN HZ")
        return vsg.get_virtual_frequency_in_Hz();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN PU")
        return vsg.get_virtual_frequency_deviation_in_pu();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN HZ")
        return vsg.get_virtual_frequency_deviation_in_Hz();
    if(var_name == "VIRTUAL ANGLE IN DEG")
        return vsg.get_virtual_angle_in_deg();
    if(var_name == "VIRTUAL ANGLE IN RAD")
        return vsg.get_virtual_angle_in_rad();
    if(var_name == "STATE@VIRTUAL FREQUENCY BLOCK")
        return vsg.get_virtual_frequency_deviation_block_state();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return vsg.get_virtual_angle_block_state();
    if(var_name == "STATE@VIRTUAL VOLTAGE BLOCK")
        return vsg.get_virtual_voltage_block_state();
    return 0.0;
}

string PVCVX::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PVCVX::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PVCVX::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
