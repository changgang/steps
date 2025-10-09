#include "header/model/wtg_models/wt_generator_model/wt3gx2.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3GX2::WT3GX2(STEPS& toolkit) : WT_GENERATOR_MODEL(toolkit),
                               dvoc(toolkit)
{
    clear();
}

WT3GX2::~WT3GX2()
{
}

void WT3GX2::clear()
{
    set_model_float_parameter_count(6);

    set_current_source_flag(false);

    dvoc.clear();
}

void WT3GX2::copy_from_const_model(const WT3GX2& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    dvoc.set_toolkit(toolkit);

    clear();
    set_eta(model.get_eta());
    set_alpha(model.get_alpha());
    set_deltafmax_in_pu(model.get_deltafmax_in_pu());
    set_deltafmin_in_pu(model.get_deltafmin_in_pu());
    set_pq_priority_flag(model.get_pq_priority_flag());
    set_Imax(model.get_Imax());
}

WT3GX2::WT3GX2(const WT3GX2& model):WT_GENERATOR_MODEL(model.get_toolkit()),
                                 dvoc(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3GX2& WT3GX2::operator=(const WT3GX2& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void WT3GX2::set_eta(double eta)
{
    dvoc.set_eta(eta);
}

void WT3GX2::set_alpha(double alpha)
{
    dvoc.set_alpha(alpha);
}

void WT3GX2::set_deltafmax_in_pu(double deltafmax)
{
    dvoc.set_deltafmax_in_pu(deltafmax);
}

void WT3GX2::set_deltafmin_in_pu(double deltafmin)
{
    dvoc.set_deltafmin_in_pu(deltafmin);
}

void WT3GX2::set_pq_priority_flag(COL_PQ_PRIORITY flag)
{
    dvoc.set_pq_priority_flag(flag);
}

void WT3GX2::set_Imax(double Imax)
{
    dvoc.set_Imax(Imax);
}

double WT3GX2::get_eta() const
{
    return dvoc.get_eta();
}

double WT3GX2::get_alpha() const
{
    return dvoc.get_alpha();
}

double WT3GX2::get_deltafmax_in_pu() const
{
    return dvoc.get_deltafmax_in_pu();
}

double WT3GX2::get_deltafmin_in_pu() const
{
    return dvoc.get_deltafmin_in_pu();
}

COL_PQ_PRIORITY WT3GX2::get_pq_priority_flag() const
{
    return dvoc.get_pq_priority_flag();
}

double WT3GX2::get_Imax() const
{
    return dvoc.get_Imax();
}

string WT3GX2::get_model_name() const
{
    return "WT3GX2";
}

bool WT3GX2::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=10)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus, n_lumped_turbine;
            string id;
            double prate, eta, alpha, deltafmax_pu, deltafmin_pu, imax;
            unsigned int pqflag;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;

            prate = get_double_data(data[i],"0.0"); i++;
            eta = get_double_data(data[i],"0.0"); i++;
            alpha = get_double_data(data[i],"0.0"); i++;
            deltafmax_pu = get_double_data(data[i],"0.0"); i++;
            deltafmin_pu = get_double_data(data[i],"0.0"); i++;
            pqflag = (unsigned int)(get_integer_data(data[i],"0")); i++;
            imax = get_double_data(data[i],"0.0");

            DEVICE_ID did = get_wt_generator_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            WT_GENERATOR* gen = psdb.get_wt_generator(did);

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

            set_eta(eta);
            set_alpha(alpha);
            set_deltafmax_in_pu(deltafmax_pu);
            set_deltafmin_in_pu(deltafmin_pu);
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

bool WT3GX2::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3GX2::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3GX2::setup_block_toolkit_and_parameters()
{
    dvoc.set_bus_pointer(get_bus_pointer());
    dvoc.set_device_pointer(get_device_pointer());
}

void WT3GX2::initialize()
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

        dvoc.set_initial_P_generation_in_pu(P);
        dvoc.set_initial_Q_generation_in_pu(Q);
        dvoc.set_source_impedance_in_pu_based_on_mbase(Zsource);
        dvoc.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active power command = "<<get_initial_active_power_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive power command = "<<get_initial_reactive_power_command_in_pu_based_on_mbase()<<endl
                    <<"(3) Initial reactive voltage command = "<<get_initial_reactive_voltage_command_in_pu()<<endl
                    <<"(4) States of blocks"<<endl
                    <<"    virtual_angle_block state: "<<dvoc.get_virtual_angle_block_state()<<endl
                    <<"    virtual_voltage_block state: "<<dvoc.get_virtual_voltage_block_state()<<endl
                    <<"(5) active power generation :"<<get_terminal_active_power_in_pu_based_on_mbase()<<endl
                    <<"(6) reactive power generation :"<<get_terminal_reactive_power_in_pu_based_on_mbase()<<endl
                    <<"(7) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<" pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3GX2::check_limits_in_initialization()
{
    return;
}

void WT3GX2::run(DYNAMIC_MODE mode)
{
    double Pref = get_active_power_command_in_pu_based_on_mbase();
    double Qref = get_reactive_power_command_in_pu_based_on_mbase();
    double Vref = get_reactive_voltage_command_in_pu();

    dvoc.set_Pref_in_pu_based_on_mbase(Pref);
    dvoc.set_Qref_in_pu_based_on_mbase(Qref);
    dvoc.set_Vref_in_pu(Vref);
    dvoc.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3GX2::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return dvoc.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double WT3GX2::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return dvoc.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> WT3GX2::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return dvoc.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> WT3GX2::get_internal_voltage_in_pu_in_xy_axis()
{
    return dvoc.get_internal_voltage_in_pu_in_xy_axis();
}

double WT3GX2::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return dvoc.get_initial_active_current_command_in_pu_based_on_mbase();
}

double WT3GX2::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return dvoc.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double WT3GX2::get_initial_reactive_voltage_command_in_pu() const
{
    return dvoc.get_initial_reactive_voltage_command_in_pu();
}

double WT3GX2::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return dvoc.get_initial_active_power_command_in_pu_based_on_mbase();
}

double WT3GX2::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return dvoc.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void WT3GX2::check()
{
    ;
}

void WT3GX2::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3GX2::save()
{
    ;
}

string WT3GX2::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<", "
            <<setw(8)<<setprecision(6)<<get_eta()<<", "
            <<setw(8)<<setprecision(6)<<get_alpha()<<", "
            <<setw(8)<<setprecision(6)<<get_deltafmax_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_deltafmin_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_pq_priority_flag()<<", "
            <<setw(8)<<setprecision(6)<<get_Imax()<<", \n";

    return osstream.str();
}

void WT3GX2::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PN", i); i++;
    add_model_data_name_and_index_pair("ETA", i); i++;
    add_model_data_name_and_index_pair("ALPHA", i); i++;
    add_model_data_name_and_index_pair("DELTAFMAX", i); i++;
    add_model_data_name_and_index_pair("DELTAFMIN", i); i++;
    add_model_data_name_and_index_pair("PQ PRIORITY FLAG", i); i++;
    add_model_data_name_and_index_pair("IMAX", i);
}

double WT3GX2::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
        return get_rated_power_per_wt_generator_in_MW();
    if(par_name=="ETA")
        return get_eta();
    if(par_name=="ALPHA")
        return get_alpha();
    if(par_name=="DELTAFMAX")
        return get_deltafmax_in_pu();
    if(par_name=="DELTAFMIN")
        return get_deltafmin_in_pu();
    if(par_name=="PQ PRIORITY FLAG")
        return get_pq_priority_flag();
    if(par_name=="IMAX")
        return get_Imax();

    return 0.0;
}

void WT3GX2::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PN")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_rated_power_per_wt_generator_in_MW(value);
    }
    if(par_name=="ETA")
        return set_eta(value);
    if(par_name=="ALPHA")
        return set_alpha(value);
    if(par_name=="DELTAFMAX")
        return set_deltafmax_in_pu(value);
    if(par_name=="DELTAFMIN")
        return set_deltafmin_in_pu(value);
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

double WT3GX2::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    return mint;
}

void WT3GX2::prepare_model_internal_variable_table()
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
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VIRTUAL VOLTAGE BLOCK", i); i++;
}

double WT3GX2::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "VIRTUAL VOLTAGE IN PU")
        return dvoc.get_virtual_voltage_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN PU")
        return dvoc.get_virtual_frequency_in_pu();
    if(var_name == "VIRTUAL FREQUENCY IN HZ")
        return dvoc.get_virtual_frequency_in_Hz();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN PU")
        return dvoc.get_virtual_frequency_deviation_in_pu();
    if(var_name == "VIRTUAL FREQUENCY DEVIATION IN HZ")
        return dvoc.get_virtual_frequency_deviation_in_Hz();
    if(var_name == "VIRTUAL ANGLE IN DEG")
        return dvoc.get_virtual_angle_in_deg();
    if(var_name == "VIRTUAL ANGLE IN RAD")
        return dvoc.get_virtual_angle_in_rad();
    if(var_name == "STATE@VIRTUAL ANGLE BLOCK")
        return dvoc.get_virtual_angle_block_state();
    if(var_name == "STATE@VIRTUAL VOLTAGE BLOCK")
        return dvoc.get_virtual_voltage_block_state();
    return 0.0;
}

string WT3GX2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3GX2::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3GX2::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
