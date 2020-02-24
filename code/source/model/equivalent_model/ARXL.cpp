#include "header/model/equivalent_model/ARXL.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
ARXL::ARXL(STEPS& toolkit) : EQUIVALENT_MODEL(toolkit)
{
    clear();
}

ARXL::ARXL(const ARXL& model) : EQUIVALENT_MODEL(model.get_toolkit())
{
    copy_from_constant_model(model);
}

ARXL& ARXL::operator=(const ARXL& model)
{
    if(this==&model)
        return *this;

    copy_from_constant_model(model);

    return (*this);
}

ARXL::~ARXL()
{
}

void ARXL::copy_from_constant_model(const ARXL& model)
{
    set_toolkit(model.get_toolkit());

    clear();

    p_meters = model.get_P_meters();
    p_delays = model.get_P_delays();
    p_coefficients = model.get_P_coefficients();
    q_meters = model.get_Q_meters();
    q_delays = model.get_Q_delays();
    q_coefficients = model.get_Q_coefficients();
}

string ARXL::get_model_name() const
{
    return "ARXL";
}

void ARXL::clear()
{
    p_meters.clear();
    q_meters.clear();
    p_delays.clear();
    q_delays.clear();
    p_coefficients.clear();
    q_coefficients.clear();
}

void ARXL::set_output_line(DEVICE_ID did, unsigned int meter_side)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    if(did.get_device_type()!="LINE")
    {
        osstream<<"Warning. The output device (of type "<<did.get_device_type()<<") is not a LINE when setting up output line for ARXL model.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    LINE* line = psdb.get_line(did);
    if(line==NULL)
    {
        osstream<<"Warning. "<<did.get_device_name()<<" is not found in power system database when setting up ARXL model.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(line->get_sending_side_breaker_status()==false and line->get_receiving_side_breaker_status()==false)
    {
        osstream<<"Warning. "<<did.get_device_name()<<" is out-of-service when setting up ARXL model.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    clear();

    METER_SETTER setter(toolkit);

    METER meter(get_toolkit());

    meter = setter.prepare_line_active_power_in_MW_meter(did, meter_side);
    add_P_input_item(meter, 0, 0.0);

    meter = setter.prepare_line_reactive_power_in_MVar_meter(did, meter_side);
    add_Q_input_item(meter, 0, 0.0);
}

void ARXL::add_P_input_item(METER meter, unsigned int delay, double coef)
{
    if(not is_P_meter_exist(meter))
        add_P_meter(meter);

    if(fabs(coef)<DOUBLE_EPSILON)
        return;

    unsigned int index = get_P_meter_index(meter);
    if(p_meter_buffers[index].get_buffer_size()<delay+1)
        p_meter_buffers[index].set_buffer_size(delay+1);
    p_delays[index].push_back(delay);
    p_coefficients[index].push_back(coef);
}

void ARXL::add_Q_input_item(METER meter, unsigned int delay, double coef)
{
    if(not is_Q_meter_exist(meter))
        add_Q_meter(meter);

    if(coef==0.0)
        return;

    unsigned int index = get_Q_meter_index(meter);
    if(q_meter_buffers[index].get_buffer_size()<delay+1)
        q_meter_buffers[index].set_buffer_size(delay+1);
    q_delays[index].push_back(delay);
    q_coefficients[index].push_back(coef);
}

unsigned int ARXL::get_P_meter_index(METER meter)
{
    unsigned int index = INDEX_NOT_EXIST;
    unsigned int n = p_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(p_meters[i]==meter)
        {
            index = i;
            break;
        }
    }
    return index;
}

unsigned int ARXL::get_Q_meter_index(METER meter)
{
    unsigned int index = INDEX_NOT_EXIST;
    unsigned int n = q_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(q_meters[i]==meter)
        {
            index = i;
            break;
        }
    }
    return index;
}

bool ARXL::is_P_meter_exist(METER meter)
{
    if(get_P_meter_index(meter)!=INDEX_NOT_EXIST)
        return true;
    else
        return false;
}

bool ARXL::is_Q_meter_exist(METER meter)
{
    if(get_Q_meter_index(meter)!=INDEX_NOT_EXIST)
        return true;
    else
        return false;
}

void ARXL::add_P_meter(METER meter)
{
    vector<unsigned int> A;
    vector<double> B;
    CONTINUOUS_BUFFER buffer;

    if(not is_P_meter_exist(meter))
    {
        p_meters.push_back(meter);
        p_meter_buffers.push_back(buffer);
        p_delays.push_back(A);
        p_coefficients.push_back(B);
    }
}

void ARXL::add_Q_meter(METER meter)
{
    vector<unsigned int> A;
    vector<double> B;
    CONTINUOUS_BUFFER buffer;

    if(not is_Q_meter_exist(meter))
    {
        q_meters.push_back(meter);
        q_meter_buffers.push_back(buffer);
        q_delays.push_back(A);
        q_coefficients.push_back(B);
    }
}

vector<METER> ARXL::get_P_meters() const
{
    return p_meters;
}

vector< vector<unsigned int> > ARXL::get_P_delays() const
{
    return p_delays;
}

vector< vector<double> > ARXL::get_P_coefficients() const
{
    return p_coefficients;
}

vector<METER> ARXL::get_Q_meters() const
{
    return q_meters;
}

vector< vector<unsigned int> > ARXL::get_Q_delays() const
{
    return q_delays;
}

vector< vector<double> > ARXL::get_Q_coefficients() const
{
    return q_coefficients;
}

bool ARXL::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<string_vector2csv(data);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

bool ARXL::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ARXL::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ARXL::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = p_meters.size();
    for(unsigned int i=0; i!=n; ++i)
        p_meter_buffers[i].set_toolkit(toolkit);
    n = q_meters.size();
    for(unsigned int i=0; i!=n; ++i)
        q_meter_buffers[i].set_toolkit(toolkit);
}

void ARXL::initialize()
{
    setup_block_toolkit_and_parameters();

    STEPS& toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    unsigned int n = p_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int m = p_delays[i].size();
        unsigned int max_delay = 0;
        for(unsigned int j=0; j!=m; ++j)
        {
            if(p_delays[i][j]>max_delay)
                max_delay = p_delays[i][j];
        }
        p_meter_buffers[i].set_buffer_size(max_delay+2);

        p_meter_buffers[i].initialize_buffer(time, p_meters[i].get_meter_value());
    }
    n = q_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int m = q_delays[i].size();
        unsigned int max_delay = 0;
        for(unsigned int j=0; j!=m; ++j)
        {
            if(q_delays[i][j]>max_delay)
                max_delay = q_delays[i][j];
        }
        q_meter_buffers[i].set_buffer_size(max_delay+2);

        q_meter_buffers[i].initialize_buffer(time, q_meters[i].get_meter_value());
    }

    set_flag_model_initialized_as_true();
}

void ARXL::run(DYNAMIC_MODE mode)
{

    if(mode==UPDATE_MODE)
    {
        update_meters_buffer();
        set_flag_model_updated_as_true();
    }
}

void ARXL::update_meters_buffer()
{
    STEPS& toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    unsigned int n = p_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        p_meter_buffers[i].append_data(time, p_meters[i].get_meter_value());
    }
    n = q_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        q_meter_buffers[i].append_data(time, q_meters[i].get_meter_value());
    }
    //show_information_with_leading_time_stamp(osstream);
}

void ARXL::update_equivalent_outputs()
{
    update_equivalent_constant_power_load();
}

void ARXL::update_equivalent_constant_power_load()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    double P = 0.0;
    unsigned int n = p_meters.size();

    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int m = p_delays[i].size();
        for(unsigned int j=0; j!=m; ++j)
        {
            unsigned int delay = p_delays[i][j];
            if(delay==0)
            {
                osstream<<"fatal erro. 0 delay is detected in ARXL model.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            double coef = p_coefficients[i][j];

            double v = p_meter_buffers[i].get_buffer_value_at_delay_index(delay)-p_meter_buffers[i].get_buffer_value_at_delay_index(delay+1);

            //osstream<<"Meter "<<p_meters[i].get_meter_name()<<", delay = "<<delay<<", coefficient = "<<coef<<", difference = "<<v<<endl;
            //osstream<<"i"<<i<<"j"<<j<<"d"<<delay<<"c"<<setprecision(6)<<fixed<<coef<<"D"<<setprecision(9)<<fixed<<v;
            P += (coef*v);
        }
    }
    //show_information_with_leading_time_stamp(osstream);

    P += p_meter_buffers[0].get_buffer_value_at_delay_index(1);//meter[0] is the output meter


    double Q = 0.0;
    n = q_meters.size();

    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int m = q_delays[i].size();
        for(unsigned int j=0; j!=m; ++j)
        {
            unsigned int delay = q_delays[i][j];
            double coef = q_coefficients[i][j];

            double v = q_meter_buffers[i].get_buffer_value_at_delay_index(delay)-q_meter_buffers[i].get_buffer_value_at_delay_index(delay+1);

            Q += coef*v;
        }
    }
    Q += q_meter_buffers[0].get_buffer_value_at_delay_index(1);//meter[0] is the output meter


    equivalent_load_constant_power = complex<double>(P, Q);

    EQUIVALENT_DEVICE* device =get_equivalent_device_pointer();
    device->set_equivalent_nominal_constant_power_load_in_MVA(equivalent_load_constant_power);
}

complex<double> ARXL::get_equivalent_voltage_source_voltage_in_pu() const
{
    return 0.0;
}

complex<double> ARXL::get_equivalent_voltage_source_impedance_in_pu() const
{
    return 0.0;
}

complex<double> ARXL::get_equivalent_nominal_constant_power_load_in_MVA() const
{
    return equivalent_load_constant_power;
}

complex<double> ARXL::get_equivalent_nominal_constant_current_load_in_MVA() const
{
    return 0.0;
}

complex<double> ARXL::get_equivalent_nominal_constant_impedance_load_in_MVA() const
{
    return 0.0;
}

complex<double> ARXL::get_total_load_power_in_MVA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    EQUIVALENT_DEVICE* edevice = get_equivalent_device_pointer();
    double vbus = psdb.get_bus_positive_sequence_voltage_in_pu(edevice->get_equivalent_device_bus());

    return (get_equivalent_nominal_constant_power_load_in_MVA()+
            get_equivalent_nominal_constant_current_load_in_MVA()*vbus+
            get_equivalent_nominal_constant_impedance_load_in_MVA()*vbus*vbus);
}

double ARXL::get_P_load_power_in_MW() const
{
    return get_total_load_power_in_MVA().real();
}

double ARXL::get_Q_load_power_in_MVar() const
{
    return get_total_load_power_in_MVA().imag();
}

void ARXL::switch_output_to_equivalent_device()
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double current_time = toolkit.get_dynamic_simulation_time_in_s();

    DEVICE_ID did = p_meters[0].get_device_id();

    LINE* line = psdb.get_line(did);
    unsigned int arxl_bus;
    //unsigned int other_bus;
    complex<double> arxl_power, other_power;

    string meter_type = p_meters[0].get_meter_type();
    arxl_bus = p_meters[0].get_meter_side_bus();
    if(arxl_bus==line->get_sending_side_bus())
        arxl_power = line->get_line_complex_power_at_sending_side_in_MVA();
    else
        arxl_power = line->get_line_complex_power_at_receiving_side_in_MVA();

    //enable equivalent device for arxl_power at arxl_bus
    vector<EQUIVALENT_DEVICE*> pedevices = psdb.get_equivalent_devices_connecting_to_bus(arxl_bus);
    if(pedevices.size()==0)
    {
        osstream<<"Warning. There is no EQUIVALENT DEVICE at bus "<<arxl_bus<<". Cannot switch output to equivalent device.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(pedevices.size()>1)
    {
        osstream<<"Warning. There are more than 1 EQUIVALENT DEVICEs at bus "<<arxl_bus<<". Cannot determine which equivalent device is to be switched to.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    EQUIVALENT_DEVICE* edevice = pedevices[0];

    edevice->set_status(true);
    edevice->set_equivalent_voltage_source_status(false);
    edevice->set_equivalent_load_status(true);
    edevice->set_equivalent_nominal_constant_power_load_in_MVA(arxl_power);
    edevice->set_equivalent_nominal_constant_current_load_in_MVA(0.0);
    edevice->set_equivalent_nominal_constant_impedance_load_in_MVA(0.0);

    osstream<<edevice->get_device_name()<<" is switched on at time "<<current_time<<" s. Desired equivalent load is "<<edevice->get_equivalent_load_in_MVA();
    toolkit.show_information_with_leading_time_stamp(osstream);


    // change 0-meters into equivalent device meters
    METER_SETTER setter(toolkit);

    DEVICE_ID  edevice_did = edevice->get_device_id();

    osstream<<"P_meters[0] was "<<p_meters[0].get_meter_name()<<endl
           <<"Q_meters[0] was "<<q_meters[0].get_meter_name()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    METER meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(edevice_did);
    p_meters[0].change_device_id(meter.get_device_id());
    p_meters[0].change_meter_type(meter.get_meter_type());

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(edevice_did);
    q_meters[0].change_device_id(meter.get_device_id());
    q_meters[0].change_meter_type(meter.get_meter_type());


    osstream<<"P_meters[0] is "<<p_meters[0].get_meter_name()<<endl
           <<"Q_meters[0] is "<<q_meters[0].get_meter_name()<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ARXL::check()
{
    ;
}
void ARXL::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void ARXL::save()
{
    ;
}
string ARXL::get_standard_psse_string() const
{
    string data;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = p_meters[0].get_device_id();
    LINE* line = psdb.get_line(did);

    data += "ARXL,"+num2str(line->get_sending_side_bus())+","+num2str(line->get_receiving_side_bus())+","+line->get_identifier()+",";
    data += num2str(p_meters[0].get_meter_side_bus())+"\n";;

    unsigned int n = p_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        vector<unsigned int> p_delay = p_delays[i];
        vector<double> p_coefficient = p_coefficients[i];
        unsigned int m = p_delay.size();
        for(unsigned int j=0; j!=m; ++j)
        {
            data += "P_INPUT,";
            data += get_meter_string(p_meters[i]);
            data += ","+num2str(p_delay[j])+","+num2str(p_coefficient[j])+"\n";
        }
    }

    n = q_meters.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        vector<unsigned int> q_delay = q_delays[i];
        vector<double> q_coefficient = q_coefficients[i];
        unsigned int m = q_delay.size();
        for(unsigned int j=0; j!=m; ++j)
        {
            data += "Q_INPUT,";
            data += get_meter_string(q_meters[i]);
            data += ","+num2str(q_delay[j])+","+num2str(q_coefficient[j])+"\n";
        }
    }
    data += "END\n";


    return data;
}

string ARXL::get_meter_string(const METER& meter) const
{
    if(meter.get_device_id().get_device_type()=="LINE")
        return get_line_meter_string(meter);

    if(meter.get_device_id().get_device_type()=="TRANSFORMER")
        return get_transformer_meter_string(meter);

    if(meter.get_device_id().get_device_type()=="BUS")
        return get_bus_meter_string(meter);

    if(meter.get_device_id().get_device_type()=="GENERATOR")
        return get_generator_meter_string(meter);

    if(meter.get_device_id().get_device_type()=="LOAD")
        return get_load_meter_string(meter);

    return "";
}

string ARXL::get_line_meter_string(const METER& meter) const
{
    string data;

    data += "LINE,";
    if(meter.get_meter_type().find("REACTIVE POWER IN MVAR")!=string::npos)
        data += "REACTIVE_POWER_MVAR,";
    else
        if(meter.get_meter_type().find("ACTIVE POWER IN MW")!=string::npos)
            data += "ACTIVE_POWER_MW,";
        else
            if(meter.get_meter_type().find("REACTIVE POWER IN PU")!=string::npos)
                data += "REACTIVE_POWER_PU,";
            else
                if(meter.get_meter_type().find("ACTIVE POWER IN PU")!=string::npos)
                    data += "ACTIVE_POWER_PU,";

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    LINE* line = psdb.get_line(did);
    data += num2str(line->get_sending_side_bus())+",";
    data += num2str(line->get_receiving_side_bus())+",";
    data += "\""+line->get_identifier()+"\",";

    data += num2str(meter.get_meter_side_bus());

    return data;
}
string ARXL::get_transformer_meter_string(const METER& meter) const
{
    string data;

    data += "TRANSFORMER,";
    if(meter.get_meter_type().find("REACTIVE POWER IN MVAR")!=string::npos)
        data += "REACTIVE_POWER_MVAR,";
    else
        if(meter.get_meter_type().find("ACTIVE POWER IN MW")!=string::npos)
            data += "ACTIVE_POWER_MW,";
        else
            if(meter.get_meter_type().find("REACTIVE POWER IN PU")!=string::npos)
                data += "REACTIVE_POWER_PU,";
            else
                if(meter.get_meter_type().find("ACTIVE POWER IN PU")!=string::npos)
                    data += "ACTIVE_POWER_PU,";

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    TRANSFORMER* trans = psdb.get_transformer(did);
    data += num2str(trans->get_winding_bus(PRIMARY_SIDE))+",";
    data += num2str(trans->get_winding_bus(SECONDARY_SIDE))+",";
    data += "\""+trans->get_identifier()+"\",";

    data += num2str(meter.get_meter_side_bus());

    return data;
}
string ARXL::get_bus_meter_string(const METER& meter) const
{
    string data;

    data += "BUS,";
    if(meter.get_meter_type().find("VOLTAGE")!=string::npos)
        data += "VOLTAGE_PU,";
    else
    {
        if(meter.get_meter_type().find("ANGLE")!=string::npos)
            data += "ANGLE_DEG,";
        else
            data += "FREQUENCY_DEVIATION_PU,";
    }

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    BUS* bus = psdb.get_bus(did);
    data += num2str(bus->get_bus_number());

    return data;
}
string ARXL::get_generator_meter_string(const METER& meter) const
{
    string data;

    data += "GENERATOR,";
    if(meter.get_meter_type().find("REACTIVE POWER IN MVAR")!=string::npos)
        data += "REACTIVE_POWER_MVAR,";
    else
        if(meter.get_meter_type().find("ACTIVE POWER IN MW")!=string::npos)
            data += "ACTIVE_POWER_MW,";
        else
            if(meter.get_meter_type().find("EXCITATION VOLTAGE IN PU")!=string::npos)
                data += "EXCITATION_VOLTAGE_PU,";
            else
                if(meter.get_meter_type().find("MECHANICAL POWER IN MW")!=string::npos)
                    data += "MECHANICAL_POWER_MW,";


    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    GENERATOR* gen = psdb.get_generator(did);
    data += num2str(gen->get_generator_bus())+",";
    data += "\""+gen->get_identifier()+"\",";

    return data;
}
string ARXL::get_load_meter_string(const METER& meter) const
{
    string data;

    data += "LOAD,";
    if(meter.get_meter_type().find("REACTIVE POWER IN MVAR")!=string::npos)
        data += "REACTIVE_POWER_MVAR,";
    else
        if(meter.get_meter_type().find("ACTIVE POWER IN MW")!=string::npos)
            data += "ACTIVE_POWER_MW,";


    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    LOAD* load = psdb.get_load(did);
    data += num2str(load->get_load_bus())+",";
    data += "\""+load->get_identifier()+"\",";

    return data;
}

void ARXL::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double ARXL::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void ARXL::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double ARXL::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void ARXL::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("TOTAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_inernal_variable_name_and_index_pair("TOTAL REACTIVE POWER LOAD IN MVAR", i); i++;
}

double ARXL::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TOTAL ACTIVE POWER LOAD IN MW")
        return get_P_load_power_in_MW();
    if(var_name == "TOTAL REACTIVE POWER LOAD IN MVAR")
        return get_Q_load_power_in_MVar();

    return 0.0;
}


string ARXL::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ARXL::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ARXL::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
