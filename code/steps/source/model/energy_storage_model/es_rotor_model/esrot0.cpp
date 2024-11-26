#include "header/model/energy_storage_model/es_rotor_model/esrot0.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ESROT0::ESROT0(STEPS& toolkit) : ES_ROTOR_MODEL(toolkit)
{
    clear();
}

ESROT0::~ESROT0()
{
    ;
}

void ESROT0::clear()
{
    set_model_float_parameter_count(1);
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    rotor_speed_block->set_limiter_type(NO_LIMITER);
}

void ESROT0::copy_from_const_model(const ESROT0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();
    set_H_in_s(model.get_H_in_s());

}

ESROT0::ESROT0(const ESROT0& model):ES_ROTOR_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESROT0& ESROT0::operator=(const ESROT0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESROT0::get_model_name() const
{
    return "ESROT0";
}

void ESROT0::set_rotor_unlock_power_threshold_in_pu(double p)
{
    this->rotor_unlock_power_threshold_in_pu = p;
}

double ESROT0::get_rotor_unlock_power_threshold_in_pu() const
{
    return this->rotor_unlock_power_threshold_in_pu;
}

bool ESROT0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double H;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            H = get_double_data(data[i],"0.0"); i++;

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_energy_storage(did);
            if(es!=NULL)
            {
                set_H_in_s(H);
                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
        }
        else
        {
            return is_successful;
        }
    }
    else
    {
        return is_successful;
    }
}

bool ESROT0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESROT0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESROT0::setup_block_toolkit_and_parameters()
{

}

void ESROT0::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
        double pe = converter_model->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        double speed = get_rotor_speed_reference_in_pu();

        set_initial_mechanical_power_in_pu_on_mbase(pe);
        set_initial_rotor_speed_reference_in_pu(speed);

        ES_MODE mode = get_energy_storage_mode();

        if(mode==ES_OUT_SERVICE_MODE)
            lock_rotor();

        if(mode==ES_CHARGE_MODE or mode==ES_DISCHARGE_MODE)
            unlock_rotor();

        INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
        rotor_speed_block->set_output(speed);
        rotor_speed_block->initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial speedref = "<<get_initial_rotor_speed_reference_in_pu()<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESROT0::run(DYNAMIC_MODE mode)
{
    if(not is_rotor_locked())
    {
        double Pmech = get_mechanical_power_in_pu_on_mbase();
        double Pe = get_active_power_including_stator_loss_in_pu_on_mbase();
        double speed = get_rotor_speed_in_pu();

        double input = (Pmech-Pe)/speed;

        ENERGY_STORAGE* es = get_energy_storage_pointer();
        ES_AUXILIARY_MODEL* au_model = es->get_es_auxiliary_model();
        if(au_model!=NULL and au_model->is_rotor_braking_activated())
        {
            double D = au_model->get_rotor_braking_Damp();
            input = - D*speed;
        }

        INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
        rotor_speed_block->set_input(input);
        rotor_speed_block->run(mode);
    }

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double ESROT0::get_rotor_speed_in_pu()
{
    if(is_rotor_locked())
    {
        return 0.0;
    }

    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    double speed =  rotor_speed_block->get_output();

    ES_MODE mode = get_energy_storage_mode();
    if(mode==ES_START_DISCHARGE_MODE and speed<=MINIMUM_ROTOR_SPEED_THRESHOLD)
    {
        speed = MINIMUM_ROTOR_SPEED_THRESHOLD;
    }

    if(mode==ES_START_CHARGE_MODE and speed>=-MINIMUM_ROTOR_SPEED_THRESHOLD)
    {
        speed = -MINIMUM_ROTOR_SPEED_THRESHOLD;
    }

    return speed;
}

void ESROT0::check()
{
    ;
}

void ESROT0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESROT0::save()
{
    ;
}

string ESROT0::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(4)<<get_H_in_s()<<" /";

    return osstream.str();
}

void ESROT0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double ESROT0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0;

    return 0.0;
}

void ESROT0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return ;

    return;
}

double ESROT0::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    return mint;
}

void ESROT0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("AA", i); i++;
}

double ESROT0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "A")
        return 0.0;

    return 0.0;
}

string ESROT0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESROT0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESROT0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
