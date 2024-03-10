#include "header/model/sg_models/compensator_model/COMP.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;
COMP::COMP(STEPS& toolkit) : COMPENSATOR_MODEL(toolkit)
{
    clear();
}

COMP::~COMP()
{
}

void COMP::clear()
{
}

void COMP::copy_from_const_model(const COMP& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());
    this->set_Xe(model.get_Xe());
}

COMP::COMP(const COMP& model) : COMPENSATOR_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

COMP& COMP::operator=(const COMP& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string COMP::get_model_name() const
{
    return "COMP";
}

void COMP::set_Xe(double xe)
{
    this->Xe = xe;
}

double COMP::get_Xe() const
{
    return Xe;
}

bool COMP::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double xe;

            unsigned int i=3;
            xe = get_double_data(data[i],"0.0");

            set_Xe(xe);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool COMP::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool COMP::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}


void COMP::setup_block_toolkit_and_parameters()
{
    ;
}

void COMP::initialize()
{
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        set_flag_model_initialized_as_false();
    }
}

void COMP::run(DYNAMIC_MODE mode)
{
    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double COMP::get_compensated_voltage_in_pu()
{
    complex<double> Vt = get_generator_terminal_complex_voltage_in_pu();
    complex<double> It = get_generator_terminal_complex_current_in_pu();
    double xe = get_Xe();
    complex<double> j(0.0,1.0);
    complex<double> Vct = Vt-j*xe*It;
    return steps_fast_complex_abs(Vct);
}

void COMP::check()
{
    ;
}
void COMP::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void COMP::save()
{
    ;
}
string COMP::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(8)<<get_Xe()<<" /";
    return osstream.str();
}

void COMP::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("XE", i); i++;
}

double COMP::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="XE") return get_Xe();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void COMP::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    {
        if(par_name=="XE") return set_Xe(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double COMP::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void COMP::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("COMPENSATED VOLTAGE IN PU", i); i++;
}

double COMP::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;
}


string COMP::get_dynamic_data_in_psse_format() const
{
    return "";
}

string COMP::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string COMP::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void COMP::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("COMP", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("COMP-AVR", matrix);
    // do linearization
}

void COMP::build_linearized_matrix_ABCD()
{
    return;
}
