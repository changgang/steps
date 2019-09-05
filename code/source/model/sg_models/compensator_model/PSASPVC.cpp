#include "header/model/sg_models/compensator_model/PSASPVC.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;
PSASPVC::PSASPVC()
{
    clear();
}

PSASPVC::~PSASPVC()
{
    clear();
}

void PSASPVC::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void PSASPVC::copy_from_const_model(const PSASPVC& model)
{
    clear();
    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());
    this->set_Xc(model.get_Xc());
}

PSASPVC::PSASPVC(const PSASPVC& model) : COMPENSATOR_MODEL()
{
    copy_from_const_model(model);
}

PSASPVC& PSASPVC::operator=(const PSASPVC& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPVC::get_model_name() const
{
    return "PSASPVC";
}

void PSASPVC::set_Xc(double xc)
{
    this->Xc = xc;
}

double PSASPVC::get_Xc() const
{
    return Xc;
}

bool PSASPVC::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double xc;

            size_t i=3;
            xc = get_double_data(data[i],"0.0");

            set_Xc(xc);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PSASPVC::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPVC::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


void PSASPVC::setup_block_toolkit_and_parameters()
{
    ;
}

void PSASPVC::initialize()
{
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        GENERATOR* gen = get_generator_pointer();
        double P = gen->get_p_generation_in_MW();
        double Q = gen->get_q_generation_in_MVar();

        sin_phi = Q/sqrt(P*P+Q*Q);

        set_flag_model_initialized_as_false();
    }
}

void PSASPVC::run(DYNAMIC_MODE mode)
{
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPVC::get_compensated_voltage_in_pu() const
{
    double Vt = get_generator_terminal_voltage_in_pu();
    double It = get_generator_terminal_current_in_pu();
    double xc = get_Xc();
    return Vt+It*xc*sin_phi;
}

void PSASPVC::check()
{
    ;
}
void PSASPVC::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void PSASPVC::save()
{
    ;
}
string PSASPVC::get_standard_psse_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(8)<<get_Xc()<<" /";
    return osstream.str();
}

void PSASPVC::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("XC", i); i++;
}

double PSASPVC::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="XC") return get_Xc();
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void PSASPVC::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    {
        if(par_name=="XC") return set_Xc(value);
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

void PSASPVC::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
}

double PSASPVC::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;
}


string PSASPVC::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPVC::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPVC::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
