#include "header/model/equivalent_model/ARXL.h"
#include "header/basic/utility.h"
#include "header/meter/meter_setter.h"
#include <cstdio>

static vector<string> MODEL_VARIABLE_TABLE{ "TOTAL ACTIVE POWER LOAD IN MW",      //0
                                            "TOTAL REACTIVE POWER LOAD IN MVAR",      //1
                                            };


ARXL::ARXL() : EQUIVALENT_MODEL()
{
    clear();
}

ARXL::~ARXL()
{
    ;
}

void ARXL::copy_from_constant_model(const ARXL& model)
{
    clear();

    set_power_system_database(model.get_power_system_database());

    p_meters = model.get_P_meters();
    p_delays = model.get_P_delays();
    p_coefficients = model.get_P_coefficients();
    q_meters = model.get_Q_meters();
    q_delays = model.get_Q_delays();
    q_coefficients = model.get_Q_coefficients();
}

ARXL::ARXL(const ARXL& model) : EQUIVALENT_MODEL()
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

void ARXL::set_output_line(DEVICE_ID did, size_t meter_side)
{
    ostringstream sstream;

    if(did.get_device_type()!="LINE")
    {
        sstream<<"Warning. The output device (of type "<<did.get_device_type()<<") is not a LINE when setting up output line for ARXL model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        sstream<<"Warning. Power system database is not set when setting up ARXL model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    LINE* line = psdb->get_line(did);
    if(line==NULL)
    {
        sstream<<"Warning. "<<did.get_device_name()<<" is not found in power system database when setting up ARXL model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    if(line->get_sending_side_breaker_status()==false and line->get_receiving_side_breaker_status()==false)
    {
        sstream<<"Warning. "<<did.get_device_name()<<" is out-of-service when setting up ARXL model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    clear();

    METER_SETTER setter;
    METER meter;

    setter.set_power_system_database(psdb);

    meter = setter.prepare_line_active_power_in_MW_meter(did, meter_side);
    add_P_input_item(meter, 0, 0.0);

    meter = setter.prepare_line_reactive_power_in_MVar_meter(did, meter_side);
    add_Q_input_item(meter, 0, 0.0);
}

void ARXL::add_P_input_item(METER meter, size_t delay, double coef)
{
    if(not is_P_meter_exist(meter))
        add_P_meter(meter);

    if(fabs(coef)<FLOAT_EPSILON)
        return;

    size_t index = get_P_meter_index(meter);
    if(p_meters[index].get_buffer_size()<delay+1)
        p_meters[index].set_buffer_size(delay+1);
    p_delays[index].push_back(delay);
    p_coefficients[index].push_back(coef);
}

void ARXL::add_Q_input_item(METER meter, size_t delay, double coef)
{
    if(not is_Q_meter_exist(meter))
        add_Q_meter(meter);

    if(coef==0.0)
        return;

    size_t index = get_Q_meter_index(meter);
    if(q_meters[index].get_buffer_size()<delay+1)
        q_meters[index].set_buffer_size(delay+1);
    q_delays[index].push_back(delay);
    q_coefficients[index].push_back(coef);
}

size_t ARXL::get_P_meter_index(METER meter)
{
    size_t index = INDEX_NOT_EXIST;
    size_t n = p_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(p_meters[i]==meter)
        {
            index = i;
            break;
        }
    }
    return index;
}

size_t ARXL::get_Q_meter_index(METER meter)
{
    size_t index = INDEX_NOT_EXIST;
    size_t n = q_meters.size();
    for(size_t i=0; i!=n; ++i)
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
    vector<size_t> A;
    vector<double> B;

    if(not is_P_meter_exist(meter))
    {
        p_meters.push_back(meter);
        p_delays.push_back(A);
        p_coefficients.push_back(B);
    }
}

void ARXL::add_Q_meter(METER meter)
{
    vector<size_t> A;
    vector<double> B;

    if(not is_Q_meter_exist(meter))
    {
        q_meters.push_back(meter);
        q_delays.push_back(A);
        q_coefficients.push_back(B);
    }
}

vector<METER> ARXL::get_P_meters() const
{
    return p_meters;
}

vector< vector<size_t> > ARXL::get_P_delays() const
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

vector< vector<size_t> > ARXL::get_Q_delays() const
{
    return q_delays;
}

vector< vector<double> > ARXL::get_Q_coefficients() const
{
    return q_coefficients;
}

bool ARXL::setup_model_with_steps_string(string data)
{
    return false;
}

bool ARXL::setup_model_with_psse_string(string data)
{
    return false;
}

bool ARXL::setup_model_with_bpa_string(string data)
{
    return false;
}

void ARXL::initialize()
{
    size_t n = p_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        size_t m = p_delays[i].size();
        size_t max_delay = 0;
        for(size_t j=0; j!=m; ++j)
        {
            if(p_delays[i][j]>max_delay)
                max_delay = p_delays[i][j];
        }
        p_meters[i].set_buffer_size(max_delay+2);

        p_meters[i].initialize_meter_buffer();
    }
    n = q_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        size_t m = q_delays[i].size();
        size_t max_delay = 0;
        for(size_t j=0; j!=m; ++j)
        {
            if(q_delays[i][j]>max_delay)
                max_delay = q_delays[i][j];
        }
        q_meters[i].set_buffer_size(max_delay+2);

        q_meters[i].initialize_meter_buffer();
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
    size_t n = p_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        p_meters[i].update_meter_buffer();
        /*sstream<<"P_INPUT meter "<<p_meters[i].get_meter_name()<<" buffer:"<<endl;
        for(size_t j=0; j!=10; j++)
            sstream<<p_meters[i].get_meter_value_from_buffer_with_delay(j)<<", ";
        sstream<<endl;*/
    }
    n = q_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        q_meters[i].update_meter_buffer();
        /*sstream<<"Q_INPUT meter "<<q_meters[i].get_meter_name()<<" buffer:"<<endl;
        for(size_t j=0; j!=MAX_HISTORY_METER_BUFFER; j++)
            sstream<<q_meters[i].get_meter_value_from_buffer_with_delay(j)<<", ";
        sstream<<endl;*/
    }
    //show_information_with_leading_time_stamp(sstream);
}

void ARXL::update_equivalent_outputs()
{
    update_equivalent_constant_power_load();
}

void ARXL::update_equivalent_constant_power_load()
{
    double time = get_power_system_database()->get_dynamic_simulator_time_in_s();

    ostringstream sstream;

    double P = 0.0;
    size_t n = p_meters.size();

    for(size_t i=0; i!=n; ++i)
    {
        size_t m = p_delays[i].size();
        for(size_t j=0; j!=m; ++j)
        {
            size_t delay = p_delays[i][j];
            if(delay==0)
            {
                sstream<<"fatal erro. 0 delay is detected in ARXL model.";
                show_information_with_leading_time_stamp(sstream);
            }
            double coef = p_coefficients[i][j];

            double v = p_meters[i].get_meter_value_from_buffer_with_delay(delay)-p_meters[i].get_meter_value_from_buffer_with_delay(delay+1);

            //sstream<<"Meter "<<p_meters[i].get_meter_name()<<", delay = "<<delay<<", coefficient = "<<coef<<", difference = "<<v<<endl;
            //sstream<<"i"<<i<<"j"<<j<<"d"<<delay<<"c"<<setprecision(6)<<fixed<<coef<<"D"<<setprecision(9)<<fixed<<v;
            P += (coef*v);
        }
    }
    //show_information_with_leading_time_stamp(sstream);

    P += p_meters[0].get_meter_value_from_buffer_with_delay(1);//meter[0] is the output meter


    double Q = 0.0;
    n = q_meters.size();

    for(size_t i=0; i!=n; ++i)
    {
        size_t m = q_delays[i].size();
        for(size_t j=0; j!=m; ++j)
        {
            size_t delay = q_delays[i][j];
            double coef = q_coefficients[i][j];

            double v = q_meters[i].get_meter_value_from_buffer_with_delay(delay)-q_meters[i].get_meter_value_from_buffer_with_delay(delay+1);

            Q += coef*v;
        }
    }
    Q += q_meters[0].get_meter_value_from_buffer_with_delay(1);//meter[0] is the output meter


    equivalent_load_constant_power = complex<double>(P, Q);

    EQUIVALENT_DEVICE* device = (EQUIVALENT_DEVICE*)get_device_pointer();
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
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) get_device_pointer();
    double vbus = psdb->get_bus_voltage_in_pu(edevice->get_equivalent_device_bus());

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
    ostringstream sstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double current_time = psdb->get_dynamic_simulator_time_in_s();

    DEVICE_ID did = p_meters[0].get_device_id();

    LINE* line = psdb->get_line(did);
    size_t arxl_bus, other_bus;
    complex<double> arxl_power, other_power;

    string meter_type = p_meters[0].get_meter_type();
    arxl_bus = p_meters[0].get_meter_side_bus();
    if(arxl_bus==line->get_sending_side_bus())
        arxl_power = line->get_line_complex_power_at_sending_side_in_MVA();
    else
        arxl_power = line->get_line_complex_power_at_receiving_side_in_MVA();

    //enable equivalent device for arxl_power at arxl_bus
    vector<EQUIVALENT_DEVICE*> pedevices = psdb->get_equivalent_devices_connecting_to_bus(arxl_bus);
    if(pedevices.size()==0)
    {
        sstream<<"Warning. There is no EQUIVALENT DEVICE at bus "<<arxl_bus<<". Cannot switch output to equivalent device.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }
    if(pedevices.size()>1)
    {
        sstream<<"Warning. There are more than 1 EQUIVALENT DEVICEs at bus "<<arxl_bus<<". Cannot determine which equivalent device is to be switched to.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    EQUIVALENT_DEVICE* edevice = pedevices[0];

    edevice->set_status(true);
    edevice->set_equivalent_voltage_source_status(false);
    edevice->set_equivalent_load_status(true);
    edevice->set_equivalent_nominal_constant_power_load_in_MVA(arxl_power);
    edevice->set_equivalent_nominal_constant_current_load_in_MVA(0.0);
    edevice->set_equivalent_nominal_constant_impedance_load_in_MVA(0.0);

    sstream<<edevice->get_device_name()<<" is switched on at time "<<current_time<<" s. Desired equivalent load is "<<edevice->get_equivalent_load_in_MVA();
    show_information_with_leading_time_stamp(sstream);


    // change 0-meters into equivalent device meters
    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    DEVICE_ID  edevice_did = edevice->get_device_id();

    sstream<<"P_meters[0] was "<<p_meters[0].get_meter_name()<<endl
           <<"Q_meters[0] was "<<q_meters[0].get_meter_name()<<endl;
    show_information_with_leading_time_stamp(sstream);

    METER meter = setter.prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(edevice_did);
    p_meters[0].change_device_id(meter.get_device_id());
    p_meters[0].change_meter_type(meter.get_meter_type());

    meter = setter.prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(edevice_did);
    q_meters[0].change_device_id(meter.get_device_id());
    q_meters[0].change_meter_type(meter.get_meter_type());


    sstream<<"P_meters[0] is "<<p_meters[0].get_meter_name()<<endl
           <<"Q_meters[0] is "<<q_meters[0].get_meter_name()<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void ARXL::check()
{
    ;
}
void ARXL::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}
void ARXL::save()
{
    ;
}
string ARXL::get_standard_model_string() const
{
    string data;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = p_meters[0].get_device_id();
    LINE* line = psdb->get_line(did);

    data += "ARXL,"+num2str(line->get_sending_side_bus())+","+num2str(line->get_receiving_side_bus())+","+line->get_identifier()+",";
    data += num2str(p_meters[0].get_meter_side_bus())+"\n";;

    size_t n = p_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        vector<size_t> p_delay = p_delays[i];
        vector<double> p_coefficient = p_coefficients[i];
        size_t m = p_delay.size();
        for(size_t j=0; j!=m; ++j)
        {
            data += "P_INPUT,";
            data += get_meter_string(p_meters[i]);
            data += ","+num2str(p_delay[j])+","+num2str(p_coefficient[j])+"\n";
        }
    }

    n = q_meters.size();
    for(size_t i=0; i!=n; ++i)
    {
        vector<size_t> q_delay = q_delays[i];
        vector<double> q_coefficient = q_coefficients[i];
        size_t m = q_delay.size();
        for(size_t j=0; j!=m; ++j)
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

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    LINE* line = psdb->get_line(did);
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

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    TRANSFORMER* trans = psdb->get_transformer(did);
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

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    BUS* bus = psdb->get_bus(did);
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


    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    GENERATOR* gen = psdb->get_generator(did);
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


    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    DEVICE_ID did = meter.get_device_id();
    LOAD* load = psdb->get_load(did);
    data += num2str(load->get_load_bus())+",";
    data += "\""+load->get_identifier()+"\",";

    return data;
}

size_t ARXL::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string ARXL::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double ARXL::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double ARXL::get_variable_with_name(string var_name)
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
