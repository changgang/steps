#include "header/model/wtg_models/wt_dbr_model/wtdbr1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"

WTDBR1::WTDBR1(STEPS& toolkit) : WT_DBR_MODEL(toolkit),
                             resistor_energy_integrator(toolkit)
{
    clear();
}

WTDBR1::~WTDBR1()
{
}

void WTDBR1::clear()
{
    set_model_float_parameter_count(3);
    resistor_energy_integrator.set_T_in_s(1.0);
    resistor_energy_integrator.set_limiter_type(NO_LIMITER);
}

void WTDBR1::copy_from_const_model(const WTDBR1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    resistor_energy_integrator.set_toolkit(toolkit);

    clear();

    set_initial_braking_resistor_energy_in_pu(model.get_initial_braking_resistor_energy_in_pu());
    set_braking_resistor_energy_threshold_in_pu(model.get_braking_resistor_energy_threshold_in_pu());
    set_Kdbr(model.get_Kdbr());
}

WTDBR1::WTDBR1(const WTDBR1& model) : WT_DBR_MODEL(model.get_toolkit()),
                                 resistor_energy_integrator(model.get_toolkit())
{
    copy_from_const_model(model);
}

WTDBR1& WTDBR1::operator=(const WTDBR1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string WTDBR1::get_model_name() const
{
    return "WTDBR1";
}

void WTDBR1::set_Kdbr(double K)
{
    Kdbr = K;
}

double WTDBR1::get_Kdbr() const
{
    return Kdbr;
}


bool WTDBR1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=6)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double kdbr, edbr, ebst;

            unsigned int i=3;
            kdbr = get_double_data(data[i],"0.0"); i++;
            edbr = get_double_data(data[i],"0.0"); i++;
            ebst = get_double_data(data[i],"0.0");

            set_Kdbr(kdbr);
            set_initial_braking_resistor_energy_in_pu(edbr);
            set_braking_resistor_energy_threshold_in_pu(ebst);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;

    }
    else
        return is_successful;
}

bool WTDBR1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WTDBR1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WTDBR1::setup_block_toolkit_and_parameters()
{
    ;
}

void WTDBR1::initialize()
{
    setup_block_toolkit_and_parameters();

    resistor_energy_integrator.set_output(get_initial_braking_resistor_energy_in_pu());
    resistor_energy_integrator.initialize();

    set_flag_model_initialized_as_true();

    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(toolkit.is_detailed_log_enabled())
    {
        osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                <<"(1) initial braking resistor energy is "<<get_initial_braking_resistor_energy_in_pu()<<" pu"<<endl
                <<"(2) braking resistor energy threshold is "<<get_braking_resistor_energy_threshold_in_pu()<<" pu";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void WTDBR1::run(DYNAMIC_MODE mode)
{
    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    double pelec = get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    double ebst = get_braking_resistor_energy_threshold_in_pu();
    double input = pcmd - pelec;

    for(unsigned int i=0; i<STEPS_MODEL_FEEDBACK_LOOP_INTEGRATION_COUNT; ++i)
    {
        double error = resistor_energy_integrator.get_output() - ebst;
        if(error<0.0)
            error = 0.0;
        double pd = get_Kdbr()*error;
        input -= pd;
        if(input>1.0)
            input = 1.0;
        if(input<0.0)
            input = 0.0;
        resistor_energy_integrator.set_input(input);
        resistor_energy_integrator.run(mode);
    }

    if(mode==DYNAMIC_UPDATE_MODE)
    {
        set_flag_model_updated_as_true();
    }
}

double WTDBR1::get_braking_resistor_power_in_pu_based_on_mbase()
{
    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    double pelec = get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    double input = pcmd - pelec;
    double edbr = resistor_energy_integrator.get_output();
    double ebst = get_braking_resistor_energy_threshold_in_pu();
    double error = edbr - ebst;
    if(error<0.0)
        error = 0.0;
    double pd = get_Kdbr()*error;
    input -= pd;
    if(input>1.0)
        input = 1.0;
    if(input<0.0)
        input = 0.0;
    return input;
}

void WTDBR1::check()
{
    ;
}

void WTDBR1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WTDBR1::save()
{
    ;
}

string WTDBR1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    double kdbr = get_Kdbr();
    double edbr = get_initial_braking_resistor_energy_in_pu();
    double ebst = get_braking_resistor_energy_threshold_in_pu();

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
            <<setw(8)<<setprecision(6)<<kdbr<<", "
            <<setw(8)<<setprecision(6)<<edbr<<", "
            <<setw(8)<<setprecision(6)<<ebst<<" /";
    return osstream.str();
}

void WTDBR1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KDBR", i); i++;
    add_model_data_name_and_index_pair("INITIAL BRAKING RESISTOR ENERGY", i); i++;
    add_model_data_name_and_index_pair("EBST THRESHOLD", i); i++;
}

double WTDBR1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="KDBR") return get_Kdbr();
        if(par_name=="INITIAL BRAKING RESISTOR ENERGY") return get_initial_braking_resistor_energy_in_pu();
        if(par_name=="EBST THRESHOLD") return get_braking_resistor_energy_threshold_in_pu();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void WTDBR1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="KDBR") return set_Kdbr(value);
        if(par_name=="INITIAL BRAKING RESISTOR ENERGY ") return set_initial_braking_resistor_energy_in_pu(value);
        if(par_name=="EBST THRESHOLD") return set_braking_resistor_energy_threshold_in_pu(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double WTDBR1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void WTDBR1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@RESISTOR ENERGY INTEGRATOR", i);i++;
    add_model_internal_variable_name_and_index_pair("BRAKING RESISTOR ENERGY", i);

}

double WTDBR1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="STATE@RESISTOR ENERGY INTEGRATOR") return resistor_energy_integrator.get_state();
    if(var_name=="BRAKING RESISTOR ENERGY") return resistor_energy_integrator.get_output();

    return 0.0;
}

string WTDBR1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WTDBR1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WTDBR1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
