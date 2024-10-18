#include "header/model/energy_storage_model/es_soc_model/essoc0.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

ESSOC0::ESSOC0(STEPS& toolkit) : ES_SOC_MODEL(toolkit)
{
    clear();
}

ESSOC0::ESSOC0(const ESSOC0& model):ES_SOC_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESSOC0::~ESSOC0()
{
    ;
}

void ESSOC0::clear()
{
    set_model_float_parameter_count(6);
}

ESSOC0& ESSOC0::operator=(const ESSOC0& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void ESSOC0::copy_from_const_model(const ESSOC0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    set_energy_state_block_toolkit(toolkit);

    clear();

    set_initial_SOC_in_pu(model.get_initial_SOC_in_pu());
    set_max_SOC_in_pu(model.get_max_SOC_in_pu());
    set_min_SOC_in_pu(model.get_min_SOC_in_pu());
    set_charge_efficiency(model.get_charge_efficiency());
    set_discharge_efficiency(model.get_discharge_efficiency());
    set_nominal_charge_time_in_s(model.get_nominal_charge_time_in_s());
}

string ESSOC0::get_model_name() const
{
    return "ESSOC0";
}

bool ESSOC0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;
    if(data.size()>=9)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double initial_soc, max_soc, min_soc, t_total, kin, kout;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            initial_soc = get_double_data(data[i],"0.0"); i++;
            max_soc = get_double_data(data[i],"0.0"); i++;
            min_soc = get_double_data(data[i],"0.0"); i++;
            t_total = get_double_data(data[i],"0.0"); i++;
            kin = get_double_data(data[i],"0.0");i++;
            kout = get_double_data(data[i],"0.0");i++;

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_energy_storage(did);
            if(es==NULL)
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                        <<"No such energy storage exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }

            set_initial_SOC_in_pu(initial_soc);
            set_max_SOC_in_pu(max_soc);
            set_min_SOC_in_pu(min_soc);
            set_nominal_charge_time_in_s(t_total);
            set_charge_efficiency(kin);
            set_discharge_efficiency(kout);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool ESSOC0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESSOC0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}

void ESSOC0::setup_block_toolkit_and_parameters()
{
    ;
}

void ESSOC0::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();

        if(not converter_model->is_model_initialized())
            converter_model->initialize();

        setup_block_toolkit_and_parameters();
        INTEGRAL_BLOCK* energy_state_block = get_energy_state_block();
        energy_state_block->set_output(get_initial_SOC_in_pu());
        energy_state_block->initialize();

        double mbase = get_mbase_in_MVA();
        double P = energy_storage->get_p_generation_in_MW();
        P /= mbase;

        if(P>=0)
            P /= get_discharge_efficiency();
        else
            P *= get_charge_efficiency();

        energy_state_block->set_input(P);

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial SOC = "<<get_initial_SOC_in_pu()<<" pu."<<endl
                    <<"(2) Initial battery power = "<<P<<" pu."<<endl
                    <<"(3) States of blocks"<<endl
                    <<"    energy_state_block block state: "<<energy_state_block->get_state()<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESSOC0::run(DYNAMIC_MODE mode)
{
    INTEGRAL_BLOCK* energy_state_block = get_energy_state_block();
    double Preq = get_soc_power_in_pu_based_on_mbase();
    Preq = -Preq;
    energy_state_block->set_input(Preq);
    energy_state_block->run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}


void ESSOC0::check()
{
    ;
}

void ESSOC0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESSOC0::save()
{
    ;
}

string ESSOC0::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_initial_SOC_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_max_SOC_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_min_SOC_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_nominal_charge_time_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_charge_efficiency()<<", "
            <<setw(8)<<setprecision(6)<<get_discharge_efficiency()<<" /";

    return osstream.str();
}

void ESSOC0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("INITIAL SOC IN PU", i); i++;
    add_model_data_name_and_index_pair("MAX SOC IN PU", i); i++;
    add_model_data_name_and_index_pair("MIN SOC IN PU", i); i++;
    add_model_data_name_and_index_pair("NOMINAL CHARGE TIME IN S", i); i++;
    add_model_data_name_and_index_pair("CHARGE EFFICIENCY", i); i++;
    add_model_data_name_and_index_pair("DISCHARGE EFFICIENCY", i); i++;
}

double ESSOC0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="INITIAL SOC IN PU")
        return get_initial_SOC_in_pu();
    if(par_name=="MAX SOC IN PU")
        return get_max_SOC_in_pu();
    if(par_name=="MIN SOC IN PU")
        return get_min_SOC_in_pu();
    if(par_name=="NOMINAL CHARGE TIME IN S")
        return get_nominal_charge_time_in_s();
    if(par_name=="CHARGE EFFICIENCY")
        return get_charge_efficiency();
    if(par_name=="DISCHARGE EFFICIENCY")
        return get_discharge_efficiency();

    return 0.0;
}

void ESSOC0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="INITIAL SOC IN PU")
        return set_initial_SOC_in_pu(value);
    if(par_name=="MAX SOC IN PU")
        return set_max_SOC_in_pu(value);
    if(par_name=="MIN SOC IN PU")
        return set_min_SOC_in_pu(value);
    if(par_name=="NOMINAL CHARGE TIME IN S")
        return set_nominal_charge_time_in_s(value);
    if(par_name=="CHARGE EFFICIENCY")
        return set_charge_efficiency(value);
    if(par_name=="DISCHARGE EFFICIENCY")
        return set_discharge_efficiency(value);
    return;
}

double ESSOC0::get_minimum_nonzero_time_constant_in_s()
{
    return get_nominal_charge_time_in_s();
}

void ESSOC0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@ENERGY STATE BLOCK", i); i++;
}

double ESSOC0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@ENERGY STATE BLOCK")
        return get_energy_state_block()->get_state();

    return 0.0;
}

string ESSOC0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESSOC0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string ESSOC0::get_dynamic_data_in_steps_format() const
{
    return "";
}
