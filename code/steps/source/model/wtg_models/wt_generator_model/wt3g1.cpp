#include "header/model/wtg_models/wt_generator_model/wt3g1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

WT3G1::WT3G1(STEPS& toolkit) : WT_GENERATOR_MODEL(toolkit),
                               csconv1(toolkit)
{
    clear();
}

WT3G1::~WT3G1()
{
}

void WT3G1::clear()
{
    set_model_float_parameter_count(5);
    set_current_source_flag(false);
    csconv1.clear();
    csconv1.set_model_name(get_model_name());
}

void WT3G1::copy_from_const_model(const WT3G1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    csconv1.set_toolkit(toolkit);

    clear();

    set_Xeq_in_pu(model.get_Xeq_in_pu());
    set_KPLL(model.get_KPLL());
    set_KIPLL(model.get_KIPLL());
    set_PLLmax(model.get_PLLmax());
}

WT3G1::WT3G1(const WT3G1& model):WT_GENERATOR_MODEL(model.get_toolkit()),
                                 csconv1(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3G1& WT3G1::operator=(const WT3G1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void WT3G1::set_Xeq_in_pu(double xeq)
{
    csconv1.set_Xeq_in_pu(xeq);
}

void WT3G1::set_KPLL(double K)
{
    csconv1.set_KPLL(K);
}

void WT3G1::set_KIPLL(double K)
{
    csconv1.set_KIPLL(K);
}

void WT3G1::set_PLLmax(double pmax)
{
    csconv1.set_PLLmax(pmax);
}

double WT3G1::get_Xeq_in_pu() const
{
    return csconv1.get_Xeq_in_pu();
}

double WT3G1::get_KPLL() const
{
    return csconv1.get_KPLL();
}

double WT3G1::get_KIPLL() const
{
    return csconv1.get_KIPLL();
}

double WT3G1::get_PLLmax() const
{
    return csconv1.get_PLLmax();
}

string WT3G1::get_model_name() const
{
    return "WT3G1";
}

bool WT3G1::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;
    if(data.size()>=8)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            unsigned int n_lumped_turbine;
            double xeq, kpll, kipll, pllmax, prate;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            xeq = get_double_data(data[i],"0.0"); i++;
            kpll = get_double_data(data[i],"0.0"); i++;
            kipll = get_double_data(data[i],"0.0"); i++;
            pllmax = get_double_data(data[i],"0.0"); i++;
            prate = get_double_data(data[i],"0.0");

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
                gen->set_mbase_in_MVA(n_lumped_turbine*prate);
            }

            gen->set_number_of_lumped_wt_generators(n_lumped_turbine);
            gen->set_rated_power_per_wt_generator_in_MW(prate);

            set_Xeq_in_pu(xeq);
            set_KPLL(kpll);
            set_KIPLL(kipll);
            set_PLLmax(pllmax);

            complex<double> Z = gen->get_source_impedance_in_pu();
            if(fabs(Z.imag()-xeq)>DOUBLE_EPSILON)
            {
                osstream<<"Warning. The Xeq of "<<get_model_name()<<" model is different from imaginary part of ZSOURCE of "<<did.get_compound_device_name()<<endl
                       <<"Xeq = "<<xeq<<" and XSource = "<<Z.imag()<<endl
                       <<"XSource will be updated as "<<xeq;
                toolkit.show_information_with_leading_time_stamp(osstream);
                Z = complex<double>(Z.real(), xeq);
                gen->set_source_impedance_in_pu(Z);
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

bool WT3G1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3G1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3G1::setup_block_toolkit_and_parameters()
{
    csconv1.set_bus_pointer(get_bus_pointer());
    csconv1.set_device_pointer(get_device_pointer());
}

void WT3G1::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        setup_block_toolkit_and_parameters();

        unsigned int n_lumped = get_number_of_lumped_wt_generators();
        double mbase = get_mbase_in_MVA();
        mbase /= n_lumped;
        double P = wt_generator->get_p_generation_in_MW()/n_lumped;
        double Q = wt_generator->get_q_generation_in_MVar()/n_lumped;
        P /= mbase;
        Q /= mbase;

        double Xeq = get_Xeq_in_pu();

        complex<double> Zsource(0.0, Xeq);

        csconv1.set_initial_P_generation_in_pu(P);
        csconv1.set_initial_Q_generation_in_pu(Q);
        csconv1.set_source_impedance_in_pu_based_on_mbase(Zsource);
        csconv1.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial active current command = "<<get_initial_active_current_command_in_pu_based_on_mbase()<<endl
                    <<"(2) Initial reactive current command = "<<get_initial_reactive_current_command_in_pu_based_on_mbase()<<endl
                    <<"(3) States of blocks"<<endl
                    <<"    active_current_commander block state: "<<csconv1.get_active_current_commander_state()<<endl
                    <<"    reactive_voltage_commander block state: "<<csconv1.get_reactive_voltage_commander_state()<<endl
                    <<"    PLL_frequency_integrator block state: "<<csconv1.get_PLL_frequency_integrator_state()<<endl
                    <<"    PLL_angle_integrator block state: "<<csconv1.get_PLL_angle_integrator_state()<<endl
                    <<"(4) active power generation :"<<get_terminal_active_power_in_MW()<<"MW"<<endl
                    <<"(5) reactive power generation :"<<get_terminal_reactive_power_in_MVar()<<"MVar"<<endl
                    <<"(6) terminal current :"<<get_terminal_current_in_pu_based_on_mbase()<<"pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3G1::run(DYNAMIC_MODE mode)
{
    double Ipcmd = get_active_current_command_in_pu_based_on_mbase();
    double Eqcmd = get_reactive_voltage_command_in_pu();
    csconv1.set_Ipcmd_in_pu_based_on_mbase(Ipcmd);
    csconv1.set_Eqcmd_in_pu(Eqcmd);
    csconv1.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> WT3G1::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv1.get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

double WT3G1::get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()
{
    return csconv1.get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

complex<double> WT3G1::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    return csconv1.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
}

complex<double> WT3G1::get_internal_voltage_in_pu_in_xy_axis()
{
    return csconv1.get_internal_voltage_in_pu_in_xy_axis();
}

double WT3G1::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return csconv1.get_initial_active_current_command_in_pu_based_on_mbase();
}

double WT3G1::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return csconv1.get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double WT3G1::get_initial_reactive_voltage_command_in_pu() const
{
    return csconv1.get_initial_reactive_voltage_command_in_pu();
}

double WT3G1::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return csconv1.get_initial_active_power_command_in_pu_based_on_mbase();
}

double WT3G1::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return csconv1.get_initial_reactive_power_command_in_pu_based_on_mbase();
}

void WT3G1::check()
{
    ;
}

void WT3G1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3G1::save()
{
    ;
}

string WT3G1::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_Xeq_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_KPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_KIPLL()<<", "
            <<setw(8)<<setprecision(6)<<get_PLLmax()<<", "
            <<setw(8)<<setprecision(6)<<get_rated_power_per_wt_generator_in_MW()<<" /";

    return osstream.str();
}

void WT3G1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("XEQ", i); i++;
    add_model_data_name_and_index_pair("KPLL", i); i++;
    add_model_data_name_and_index_pair("KIPLL", i); i++;
    add_model_data_name_and_index_pair("PLLMAX", i); i++;
    add_model_data_name_and_index_pair("PN", i); i++;
}

double WT3G1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="XEQ")
        return get_Xeq_in_pu();
    if(par_name=="KPLL")
        return get_KPLL();
    if(par_name=="KIPLL")
        return get_KIPLL();
    if(par_name=="PLLMAX")
        return get_PLLmax();
    if(par_name=="PN")
        return get_rated_power_per_wt_generator_in_MW();

    return 0.0;
}

void WT3G1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="N")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_number_of_lumped_wt_generators((unsigned int)(value));
    }
    if(par_name=="KPLL")
        return set_KPLL(value);
    if(par_name=="KIPLL")
        return set_KIPLL(value);
    if(par_name=="PLLMAX")
        return set_PLLmax(value);
    if(par_name=="PN")
    {
        WT_GENERATOR* gen = get_wt_generator_pointer();
        return gen->set_rated_power_per_wt_generator_in_MW(value);
    }

    return;
}

double WT3G1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void WT3G1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ACTIVE CURRENT COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REACTIVE VOLTAGE COMMAND BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL FREQUENCY BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PLL ANGLE BLOCK", i); i++;
}

double WT3G1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PLL ANGLE IN DEG")
        return get_pll_angle_in_deg();
    if(var_name == "PLL ANGLE IN RAD")
        return get_pll_angle_in_rad();
    if(var_name == "STATE@ACTIVE CURRENT COMMAND BLOCK")
        return csconv1.get_active_current_commander_state();
    if(var_name == "STATE@REACTIVE VOLTAGE COMMAND BLOCK")
        return csconv1.get_reactive_voltage_commander_state();
    if(var_name == "STATE@PLL FREQUENCY BLOCK")
        return csconv1.get_PLL_frequency_integrator_state();
    if(var_name == "STATE@PLL ANGLE BLOCK")
        return csconv1.get_PLL_angle_integrator_state();

    return 0.0;
}

double WT3G1::get_pll_angle_in_rad()
{
    return csconv1.get_pll_angle_in_rad();
}

double WT3G1::get_pll_angle_in_deg()
{
    return csconv1.get_pll_angle_in_deg();
}

double WT3G1::get_pll_frequency_deviation_in_pu()
{
    return csconv1.get_pll_frequency_deviation_in_pu();
}

double WT3G1::get_pll_frequency_deviation_in_Hz()
{
    return csconv1.get_pll_frequency_deviation_in_Hz();
}

double WT3G1::get_pll_frequency_in_pu()
{
    return csconv1.get_pll_frequency_deviation_in_pu();
}

double WT3G1::get_pll_frequency_in_Hz()
{
    return csconv1.get_pll_frequency_in_Hz();
}


string WT3G1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3G1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3G1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
