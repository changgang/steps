#include "header/model/energy_storage_model/es_relay_model/esvrtrly1.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;

ESVRTRLY1::ESVRTRLY1(STEPS& toolkit) : ES_RELAY_MODEL(toolkit),
                                   lvrt(toolkit),
                                   hvrt(toolkit)
{
    clear();
}

ESVRTRLY1::~ESVRTRLY1()
{
}

void ESVRTRLY1::clear()
{
    lvrt.clear();
    hvrt.clear();
}

void ESVRTRLY1::copy_from_const_model(const ESVRTRLY1& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    set_vrt_trip_time_delay_in_s(model.get_vrt_trip_time_delay_in_s());
    this->lvrt = model.lvrt;
    this->hvrt = model.hvrt;
}

ESVRTRLY1::ESVRTRLY1(const ESVRTRLY1& model) : ES_RELAY_MODEL(model.get_toolkit()),
                                         lvrt(model.get_toolkit()),
                                         hvrt(model.get_toolkit())
{
    clear();
    copy_from_const_model(model);
}

ESVRTRLY1& ESVRTRLY1::operator=(const ESVRTRLY1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESVRTRLY1::get_model_name() const
{
    return "ESVRTRLY1";
}

void ESVRTRLY1::set_vrt_trip_time_delay_in_s(double t)
{
    lvrt.set_vrt_trip_time_delay_in_s(t);
    hvrt.set_vrt_trip_time_delay_in_s(t);
}

void ESVRTRLY1::add_lvrt_time_volt_threshold_pair(double t, double v)
{
    lvrt.add_vrt_time_volt_threshold_pair(t, v);
}

void ESVRTRLY1::set_lvrt_trip_scale(double scale)
{
    lvrt.set_vrt_trip_scale(scale);
}

void ESVRTRLY1::add_hvrt_time_volt_threshold_pair(double t, double v)
{
    hvrt.add_vrt_time_volt_threshold_pair(t, v);
}

void ESVRTRLY1::set_hvrt_trip_scale(double scale)
{
    hvrt.set_vrt_trip_scale(scale);
}

double ESVRTRLY1::get_vrt_trip_time_delay_in_s() const
{
    return lvrt.get_vrt_trip_time_delay_in_s();
}

unsigned int ESVRTRLY1::get_lvrt_point_number() const
{
    return lvrt.get_vrt_point_number();
}

double ESVRTRLY1::get_lvrt_time_threshold(unsigned int index) const
{
    return lvrt.get_vrt_time_threshold(index);
}

double ESVRTRLY1::get_lvrt_volt_threshold(unsigned int index) const
{
    return lvrt.get_vrt_volt_threshold(index);
}

double ESVRTRLY1::get_lvrt_trip_scale() const
{
    return lvrt.get_vrt_trip_scale();
}

unsigned int ESVRTRLY1::get_hvrt_point_number() const
{
    return hvrt.get_vrt_point_number();
}

double ESVRTRLY1::get_hvrt_time_threshold(unsigned int index) const
{
    return hvrt.get_vrt_time_threshold(index);
}

double ESVRTRLY1::get_hvrt_volt_threshold(unsigned int index) const
{
    return hvrt.get_vrt_volt_threshold(index);
}

double ESVRTRLY1::get_hvrt_trip_scale() const
{
    return hvrt.get_vrt_trip_scale();
}

bool ESVRTRLY1::setup_model_with_steps_string_vector(vector<string>& data)
{
    /*        ESVRTRLY1  bus  ickt  t_trip
              nlvrt  t1  v1  t2  v2  t3  v3  tn  vn  lvrt_trip_scale
              nhvrt  t1  v1  t2  v2  t3  v3  tm  vm  hvrt_trip_scale /
    */
    bool is_successful = false;
    if(data.size()>=12)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int n_lvrt, n_hvrt;
            double t, v, s;

            unsigned int i=3;
            t = get_double_data(data[i], "0"); ++i;
            set_vrt_trip_time_delay_in_s(t);

            n_lvrt = get_integer_data(data[i], "0"); ++i;
            for(unsigned int k=0; k<n_lvrt; ++k)
            {
                t = get_double_data(data[i], "0.0"); ++i;
                v = get_double_data(data[i], "0.0"); ++i;
                add_lvrt_time_volt_threshold_pair(t, v);
            }
            s = get_double_data(data[i], "0"); ++i;
            set_lvrt_trip_scale(s);

            n_hvrt = get_integer_data(data[i], "0"); ++i;
            for(unsigned int k=0; k<n_hvrt; ++k)
            {
                t = get_double_data(data[i], "0.0"); ++i;
                v = get_double_data(data[i], "0.0"); ++i;
                add_hvrt_time_volt_threshold_pair(t, v);
            }
            s = get_double_data(data[i], "0"); ++i;
            set_hvrt_trip_scale(s);

            set_model_float_parameter_count(2*n_lvrt+2*n_hvrt+8);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool ESVRTRLY1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESVRTRLY1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESVRTRLY1::setup_block_toolkit_and_parameters()
{
    lvrt.set_device_pointer(get_device_pointer());
    hvrt.set_device_pointer(get_device_pointer());

    lvrt.set_bus_pointer(get_bus_pointer());
    hvrt.set_bus_pointer(get_bus_pointer());
}

void ESVRTRLY1::initialize()
{
    setup_block_toolkit_and_parameters();
    lvrt.initialize_vrt_relay();
    hvrt.initialize_vrt_relay();
}

void ESVRTRLY1::check_limits_in_initialization()
{
    return;
}

void ESVRTRLY1::run(DYNAMIC_MODE mode)
{
    if(mode==DYNAMIC_RELAY_MODE)
    {
        lvrt.check_vrt_relay();
        hvrt.check_vrt_relay();
    }
}

void ESVRTRLY1::check()
{
    ;
}

void ESVRTRLY1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESVRTRLY1::save()
{
    ;
}
string ESVRTRLY1::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(4)<<get_vrt_trip_time_delay_in_s()<<", ";

    unsigned int n=get_lvrt_point_number();

    osstream<<setw(8)<<n<<", ";

    unsigned int number = 5;
    for(unsigned int i=0; i<n; ++i)
    {
        osstream<<setw(8)<<setprecision(4)<<get_lvrt_time_threshold(i)<<", ";
        number++;
        if(number == 10 or ((number-10)%9 == 0 and number>10))
            osstream<<"\n"<<setw(10)<<"";
        osstream<<setw(8)<<setprecision(4)<<get_lvrt_volt_threshold(i)<<", ";
        number++;
        if(number == 10 or ((number-10)%9 == 0 and number>10))
            osstream<<"\n"<<setw(10)<<"";
    }
    osstream<<setw(8)<<setprecision(4)<<get_lvrt_trip_scale()<<", ";
    number++;
    if(number == 10 or ((number-10)%9 == 0 and number>10))
        osstream<<"\n"<<setw(10)<<"";

    n=get_hvrt_point_number();

    osstream<<setw(8)<<n<<", ";
    number++;
    if(number == 10 or ((number-10)%9 == 0 and number>10))
        osstream<<"\n"<<setw(10)<<"";

    for(unsigned int i=0; i<n; ++i)
    {
        osstream<<setw(8)<<setprecision(4)<<get_hvrt_time_threshold(i)<<", ";
        number++;
        if(number == 10 or ((number-10)%9 == 0 and number>10))
            osstream<<"\n"<<setw(10)<<"";
        osstream<<setw(8)<<setprecision(4)<<get_hvrt_volt_threshold(i)<<", ";
        number++;
        if(number == 10 or ((number-10)%9 == 0 and number>10))
            osstream<<"\n"<<setw(10)<<"";
    }
    osstream<<setw(8)<<setprecision(4)<<get_hvrt_trip_scale()<<" /";

    return osstream.str();
}

void ESVRTRLY1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TRIP TIME DELAY", i); i++;
    add_model_data_name_and_index_pair("N LVRT TABLE", i); i++;
    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "V LVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
    }
    add_model_data_name_and_index_pair("LVRT SHED SCALE", i); i++;

    add_model_data_name_and_index_pair("N HVRT TABLE", i); i++;
    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "V HVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
    }
    add_model_data_name_and_index_pair("HVRT SHED SCALE", i); i++;
}

double ESVRTRLY1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="TRIP TIME DELAY") return get_vrt_trip_time_delay_in_s();

    if(par_name=="N LVRT TABLE") return get_lvrt_point_number();

    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        if(par_name==name) return get_lvrt_time_threshold(j);
        name = "V LVRT "+num2str(j);
        if(par_name==name) return get_lvrt_volt_threshold(j);
    }
    if(par_name=="LVRT SHED SCALE") return get_lvrt_trip_scale();

    if(par_name=="N HVRT TABLE") return get_hvrt_point_number();

    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        if(par_name==name) return get_hvrt_time_threshold(j);
        name = "V HVRT "+num2str(j);
        if(par_name==name) return get_hvrt_volt_threshold(j);
    }
    if(par_name=="HVRT SHED SCALE") return get_hvrt_trip_scale();

    return 0.0;
}

void ESVRTRLY1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="TRIP TIME DELAY") return set_vrt_trip_time_delay_in_s(value);

    if(par_name=="N LVRT TABLE") return;

    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        if(par_name==name) return lvrt.set_point_time_with_index(j, value);
        name = "V LVRT "+num2str(j);
        if(par_name==name) return lvrt.set_point_voltage_with_index(j, value);
    }
    if(par_name=="LVRT SHED SCALE") return set_lvrt_trip_scale(value);

    if(par_name=="N HVRT TABLE") return;

    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        if(par_name==name) return hvrt.set_point_time_with_index(j, value);
        name = "V HVRT "+num2str(j);
        if(par_name==name) return hvrt.set_point_voltage_with_index(j, value);
    }
    if(par_name=="HVRT SHED SCALE") return set_hvrt_trip_scale(value);
}


double ESVRTRLY1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void ESVRTRLY1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("LVRT VOLTAGE TRIP BOUNDARY IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("HVRT VOLTAGE TRIP BOUNDARY IN PU", i); i++;
}

double ESVRTRLY1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="LVRT VOLTAGE TRIP BOUNDARY IN PU")
        return lvrt.get_lvrt_voltage_trip_boundary_in_pu();
    if(var_name=="HVRT VOLTAGE TRIP BOUNDARY IN PU")
        return hvrt.get_hvrt_voltage_trip_boundary_in_pu();

    return 0.0;
}

string ESVRTRLY1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESVRTRLY1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESVRTRLY1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
