#include "header/model/sg_models/compensator_model/IEEEVC.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;
IEEEVC::IEEEVC()
{
    clear();
}

IEEEVC::~IEEEVC()
{
}

void IEEEVC::clear()
{
}

void IEEEVC::copy_from_const_model(const IEEEVC& model)
{
    clear();
    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());
    this->set_Rc(model.get_Rc());
    this->set_Xc(model.get_Xc());
}

IEEEVC::IEEEVC(const IEEEVC& model) : COMPENSATOR_MODEL()
{
    copy_from_const_model(model);
}

IEEEVC& IEEEVC::operator=(const IEEEVC& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEVC::get_model_name() const
{
    return "IEEEVC";
}

void IEEEVC::set_Rc(double rc)
{
    this->Rc = rc;
}

void IEEEVC::set_Xc(double xc)
{
    this->Xc = xc;
}

double IEEEVC::get_Rc() const
{
    return Rc;
}

double IEEEVC::get_Xc() const
{
    return Xc;
}

bool IEEEVC::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=5)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int i=3;

            set_Rc(get_double_data(data[i],"0.0")); ++i;
            set_Xc(get_double_data(data[i],"0.0"));

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEEVC::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEEVC::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


void IEEEVC::setup_block_toolkit_and_parameters()
{
    ;
}

void IEEEVC::initialize()
{
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        set_flag_model_initialized_as_false();
    }
}

void IEEEVC::run(DYNAMIC_MODE mode)
{
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEVC::get_compensated_voltage_in_pu()
{
    complex<double> Vt = get_generator_terminal_complex_voltage_in_pu();
    complex<double> It = get_generator_terminal_complex_current_in_pu();
    double rc = get_Rc();
    double xc = get_Xc();
    complex<double> Z(rc,xc);
    complex<double> Vct = Vt+Z*It;
    return steps_fast_complex_abs(Vct);
}

void IEEEVC::check()
{
    ;
}
void IEEEVC::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEEEVC::save()
{
    ;
}
string IEEEVC::get_standard_psse_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(8)<<get_Rc()
            <<setw(8)<<setprecision(8)<<get_Xc()<<" /";
    return osstream.str();
}

void IEEEVC::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("RC", i); i++;
    add_model_data_name_and_index_pair("XC", i); i++;
}

double IEEEVC::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="RC") return get_Rc();
        if(par_name=="XC") return get_Xc();
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void IEEEVC::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    {
        if(par_name=="RC") return set_Rc(value);
        if(par_name=="XC") return set_Xc(value);
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

void IEEEVC::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
}

double IEEEVC::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;
}


string IEEEVC::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEVC::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEVC::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
