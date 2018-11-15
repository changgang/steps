#include "header/meter/meter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <map>
#include <vector>
#include <istream>
#include <iostream>

using namespace std;

vector<string> bus_meters{ "VOLTAGE IN PU",             "VOLTAGE IN KV",
                            "ANGLE IN DEG",              "ANGLE IN RAD",
                            "FREQUENCY IN PU",           "FREQUENCY IN HZ",
                            "FREQUENCY DEVIATION IN PU", "FREQUENCY DEVIATION IN HZ",
                            "ROCOV IN PU/S",             "ROCOV IN KV/S",
                            "ROCOF IN PU/S",             "ROCOF IN HZ/S"};


vector<string> line_meters{"CURRENT AT SENDING SIDE IN KA",                    "CURRENT AT RECEIVING SIDE IN KA",
                            "ACTIVE POWER AT SENDING SIDE IN MW",               "ACTIVE POWER AT RECEIVING SIDE IN MW",
                            "REACTIVE POWER AT SENDING SIDE IN MVAR",           "REACTIVE POWER AT RECEIVING SIDE IN MVAR",
                            "APPARENT IMPEDANCE AT SENDING SIDE IN OHM",        "APPARENT IMPEDANCE AT RECEIVING SIDE IN OHM",
                            "APPARENT IMPEDANCE ANGLE AT SENDING SIDE IN DEG",  "APPARENT IMPEDANCE ANGLE AT RECEIVING SIDE IN DEG",
                            "CURRENT IN KA",
                            "ACTIVE POWER IN MW",
                            "REACTIVE POWER IN MVAR",
                            "APPARENT IMPEDANCE IN OHM",
                            "APPARENT IMPEDANCE ANGLE IN DEG",
                            "CURRENT IN PU",
                            "ACTIVE POWER IN PU",
                            "REACTIVE POWER IN PU",
                            "APPARENT IMPEDANCE IN PU",
                            "APPARENT IMPEDANCE ANGLE IN RAD"};


vector<string> transformer_meters{"CURRENT AT PRIMARY WINDING IN KA",  "CURRENT AT SECONDARY WINDING IN KA",
                                   "CURRENT AT TERTIARY WINDING IN KA",
                                   "ACTIVE POWER AT PRIMARY WINDING IN MW",    "ACTIVE POWER AT SECONDARY WINDING IN MW",
                                   "ACTIVE POWER AT TERTIARY WINDING IN MW",
                                   "REACTIVE POWER AT PRIMARY WINDING IN MVAR", "REACTIVE POWER AT SECONDARY WINDING IN MVAR",
                                   "REACTIVE POWER AT TERTIARY WINDING IN MVAR",
                                   "CURRENT IN KA",
                                   "ACTIVE POWER IN MW",
                                   "REACTIVE POWER IN MVAR",
                                   "CURRENT IN PU",
                                   "ACTIVE POWER IN PU",
                                   "REACTIVE POWER IN PU"};


vector<string> load_meters{ "CURRENT IN KA",
                            "CURRENT IN PU",
                            "ACTIVE POWER IN MW", "REACTIVE POWER IN MVAR",
                            "ACTIVE POWER IN PU", "REACTIVE POWER IN PU",
                            "TOTAL SCALE IN PU",
                            "MANUALLY SCALE IN PU",
                            "RELAY SHED SCALE IN PU",
                            "LOAD MODEL INTERNAL VARIABLE",
                            "FREQUENCY RELAY MODEL INTERNAL VARIABLE",
                            "VOLTAGE RELAY MODEL INTERNAL VARIABLE"};

vector<string> generator_meters{"ROTOR ANGLE IN DEG",
                                "ROTOR SPEED IN PU",               "ROTOR SPEED IN HZ",
                                "ROTOR SPEED DEVIATION IN PU",     "ROTOR SPEED DEVIATION IN HZ",
                                "INTERNAL VOLTAGE IN PU",
                                "TERMINAL CURRENT IN PU ON MBASE", "TERMINAL CURRENT IN PU ON SBASE",
                                "TERMINAL CURRENT IN KA",
                                "TERMINAL ACTIVE POWER IN PU ON MBASE", "TERMINAL ACTIVE POWER IN PU ON SBASE",
                                "TERMINAL ACTIVE POWER IN MW",
                                "TERMINAL REACTIVE POWER IN PU ON MBASE", "TERMINAL REACTIVE POWER IN PU ON SBASE",
                                "TERMINAL REACTIVE POWER IN MVAR",
                                "TERMINAL APPRAENT POWER IN PU ON MBASE", "TERMINAL APPRAENT POWER IN PU ON SBASE",
                                "TERMINAL APPRAENT POWER IN MVA",
                                "AIRGAP POWER IN PU ON MBASE", "AIRGAP POWER IN PU ON SBASE",
                                "AIRGAP POWER IN MW",
                                "ACCELERATING POWER IN PU ON MBASE", "ACCELERATING POWER IN PU ON SBASE",
                                "ACCELERATING POWER IN MW",
                                "MECHANICAL POWER IN PU ON MBASE",  "MECHANICAL POWER IN PU ON SBASE",
                                "MECHANICAL POWER IN MW",
                                "MECHANICAL POWER REFERENCE IN PU ON MBASE",  "MECHANICAL POWER REFERENCE IN PU ON SBASE",
                                "MECHANICAL POWER REFERENCE IN MW",
                                "COMPENSATED VOLTAGE IN PU",
                                "VOLTAGE REFERENCE IN PU",
                                "STABILIZING SIGNAL IN PU",
                                "EXCITATION VOLTAGE IN PU",
                                "SYNC GENERATOR MODEL INTERNAL VARIABLE",
                                "COMPENSATOR MODEL INTERNAL VARIABLE",
                                "EXCITER MODEL INTERNAL VARIABLE",
                                "STABILIZER MODEL INTERNAL VARIABLE",
                                "TURBINE GOVERNOR MODEL INTERNAL VARIABLE"};

vector<string> wt_generator_meters{ "TERMINAL CURRENT IN PU ON MBASE",  "TERMINAL CURRENT IN PU ON SBASE"
                                    "TERMINAL CURRENT IN KA",
                                    "TERMINAL ACTIVE POWER IN PU ON MBASE", "TERMINAL ACTIVE POWER IN PU ON SBASE",
                                    "TERMINAL ACTIVE POWER IN MW",
                                    "TERMINAL REACTIVE POWER IN PU ON MBASE", "TERMINAL REACTIVE POWER IN PU ON SBASE",
                                    "TERMINAL REACTIVE POWER IN MVAR",
                                    "TERMINAL APPRAENT POWER IN PU ON MBASE", "TERMINAL APPRAENT POWER IN PU ON SBASE",
                                    "TERMINAL APPRAENT POWER IN MVA",
                                    "MECHANICAL POWER IN PU ON MBASE", "MECHANICAL POWER IN PU ON SBASE",
                                    "MECHANICAL POWER IN MW",
                                    "MAX AVAILABLE MECHANICAL POWER IN PU ON MBASE", "MAX AVAILABLE MECHANICAL POWER IN PU ON SBASE",
                                    "MAX AVAILABLE MECHANICAL POWER IN MW",
                                    "SPEED REFERENCE IN PU", "SPEED REFERENCE IN RAD/S",
                                    "TURBINE SPEED DEVIATION IN PU", "TURBINE SPEED DEVIATION IN HZ",
                                    "TURBINE SPEED IN PU",           "TURBINE SPEED IN HZ",
                                    "ROTOR SPEED DEVIATION IN PU", "ROTOR SPEED DEVIATION IN HZ",
                                    "ROTOR SPEED IN PU",           "ROTOR SPEED IN HZ",
                                    "ROTOR ANGLE IN DEG",           "ROTOR ANGLE IN RAD",
                                    "ACTIVE CURRENT COMMAND IN PU",
                                    "REACTIVE CURRENT COMMAND IN PU",
                                    "ACTIVE POWER COMMAND IN PU",
                                    "REACTIVE POWER COMMAND IN PU",
                                    "REACTIVE VOLTAGE COMMAND IN PU",
                                    "PITCH ANGLE IN DEG",
                                    "WIND SPEED IN PU", "WIND SPEED IN MPS",
                                    "WT GENERATOR MODEL INTERNAL VARIABLE",
                                    "WT AERODYNAMIC MODEL INTERNAL VARIABLE",
                                    "WT TURBINE MODEL INTERNAL VARIABLE",
                                    "WT ELECTRICAL MODEL INTERNAL VARIABLE",
                                    "WT PITCH MODEL INTERNAL VARIABLE",
                                    "WIND SPEED MODEL INTERNAL VARIABLE"};

vector<string> pv_unit_meters{ "TERMINAL CURRENT IN PU", "TERMINAL CURRENT IN KA",
                               "TERMINAL ACTIVE POWER IN MW",
                               "TERMINAL REACTIVE POWER IN MVAR",
                               "ACTIVE CURRENT COMMAND IN PU",
                               "REACTIVE CURRENT COMMAND IN PU",
                               "ACTIVE POWER COMMAND IN PU",
                               "REACTIVE POWER COMMAND IN PU",
                               "REACTIVE VOLTAGE COMMAND IN PU",
                               "PV PANEL MODEL INTERNAL VARIABLE",
                               "PV CONVERTER MODEL INTERNAL VARIABLE",
                               "PV ELECTRICAL MODEL INTERNAL VARIABLE",
                               "PV IRRADIANCE MODEL INTERNAL VARIABLE"};

vector<string> energy_storage_meters{"STATE OF ENERGY IN PU",
                                      "ACTIVE POWER IN MW",
                                      "ACTIVE POWER IN PU",
                                      "REACTIVE POWER IN MVAR",
                                      "REACTIVE POWER IN PU",
                                      "TERMINAL CURRENT IN KA",
                                      "TERMINAL CURRENT IN PU",
                                      "ENERGY STORAGE MODEL INTERNAL VARIABLE"};

vector<string> hvdc_meters{ "DC CURRENT IN KA",
                            "RECTIFIER DC CURRENT IN KA",   "INVERTER DC CURRENT IN KA",
                            "RECTIFIER AC CURRENT IN KA",   "INVERTER AC CURRENT IN KA",
                            "RECTIFIER ALPHA IN DEG",       "INVERTER GAMMA IN DEG",
                            "RECTIFIER MU IN DEG",          "INVERTER MU IN DEG",
                            "RECTIFIER DC VOLTAGE IN KV",   "INVERTER DC VOLTAGE IN KV",
                            "RECTIFIER AC VOLTAGE IN PU",   "INVERTER AC VOLTAGE IN PU",
                            "RECTIFIER DC POWER IN MW",     "INVERTER DC POWER IN MW",
                            "RECTIFIER AC ACTIVE POWER IN MW","INVERTER AC ACTIVE POWER IN MW",
                            "RECTIFIER AC REACTIVE POWER IN MVAR","INVERTER AC REACTIVE POWER IN MVAR",
                            "HVDC MODEL INTERNAL VARIABLE"};

vector<string> equivalent_device_meters{"VOLTAGE SOURCE VOLTAGE IN PU",
                                         "VOLTAGE SOURCE VOLTAGE ANGLE IN DEG",
                                         "VOLTAGE SOURCE RESISTANCE IN PU",
                                         "VOLTAGE SOURCE REACTANCE IN PU",
                                         "ACTIVE CONSTANT POWER LOAD IN MW",
                                         "REACTIVE CONSTANT POWER LOAD IN MVAR",
                                         "ACTIVE CONSTANT CURRENT LOAD IN MW",
                                         "REACTIVE CONSTANT CURRENT LOAD IN MVAR",
                                         "ACTIVE CONSTANT IMPEDANCE LOAD IN MW",
                                         "REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR",
                                         "ACTIVE POWER GENERATION IN MW",
                                         "REACTIVE POWER GENERATION IN MVAR",
                                         "ACTIVE POWER LOAD IN MW",
                                         "REACTIVE POWER LOAD IN MVAR",
                                         "ACTIVE POWER NET LOAD IN MW",
                                         "REACTIVE POWER NET LOAD IN MVAR",
                                         "ACTIVE CONSTANT POWER LOAD IN PU",
                                         "REACTIVE CONSTANT POWER LOAD IN PU",
                                         "ACTIVE CONSTANT CURRENT LOAD IN PU",
                                         "REACTIVE CONSTANT CURRENT LOAD IN PU",
                                         "ACTIVE CONSTANT IMPEDANCE LOAD IN PU",
                                         "REACTIVE CONSTANT IMPEDANCE LOAD IN PU",
                                         "ACTIVE POWER GENERATION IN PU",
                                         "REACTIVE POWER GENERATION IN PU",
                                         "ACTIVE POWER LOAD IN PU",
                                         "REACTIVE POWER LOAD IN PU",
                                         "ACTIVE POWER NET LOAD IN PU",
                                         "REACTIVE POWER NET LOAD IN PU"};

map<string, vector<string>> SUPPORTED_METERS{ {"BUS",         bus_meters},
                                                {"LINE",        line_meters},
                                                {"TRANSFORMER", transformer_meters},
                                                {"LOAD",        load_meters},
                                                {"GENERATOR",   generator_meters},
                                                {"WT GENERATOR",wt_generator_meters},
                                                {"PV UNIT",     pv_unit_meters},
                                                {"ENERGY STORAGE", energy_storage_meters},
                                                {"HVDC", hvdc_meters},
                                                {"EQUIVALENT DEVICE", equivalent_device_meters}};
METER::METER()
{
    clear();
}

void METER::copy_from_const_meter(const METER& meter)
{
    clear();

    set_buffer_size(meter.get_buffer_size());
    set_device_id(meter.get_device_id());
    set_meter_type(meter.get_meter_type());
    if(meter_type.find("INTERNAL VARIABLE") != string::npos)
        set_internal_variable_name(meter.get_internal_variable_name());
    set_meter_side_bus(meter.get_meter_side_bus());

    if(meter.get_device_pointer()!=NULL)
        set_device_pointer();
}

METER::METER(const METER& meter)
{
    copy_from_const_meter(meter);
}

METER& METER::operator=(const METER& meter)
{
    if(this==&meter)
        return *this;

    copy_from_const_meter(meter);

    return *this;
}


METER::~METER()
{
    ;
}

void METER::set_device_id(const DEVICE_ID& did)
{
    if(not did.is_valid())
    {
        ostringstream osstream;
        osstream<<"Warning. Device id is invalid for setting up meter.";
        show_information_with_leading_time_stamp(osstream);
    }
    this->device_id =did;
    set_device_pointer();
}

void METER::set_meter_type(string meter_type)
{
    ostringstream osstream;
    if(not device_id.is_valid())
    {
        osstream<<"Warning. Device id is invalid for setting up meter type. Set up proper device id first.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    meter_type = string2upper(meter_type);

    if(is_valid_meter_type(meter_type))
    {
        this->meter_type = meter_type;
    }
    else
    {
        osstream<<"Warning, Invalid meter type '"<<meter_type<<"' is not supported for setting up meter type of "<<device_id.get_device_name()<<".";
        show_information_with_leading_time_stamp(osstream);
        this->meter_type = "";
    }
}

void METER::set_internal_variable_name(string name)
{
    ostringstream osstream;
    if(not device_id.is_valid())
    {
        osstream<<"Warning. Device id is invalid for setting up meter internal variable name. Set up proper device id first.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(meter_type.find("INTERNAL VARIABLE") != string::npos)
    {
        if(is_internal_variable_name_valid(name))
            this->internal_variable_name = name;
        else
        {
            osstream<<"Warning, Internal variable name '"<<name<<"' is not valid. No internal variable name will be set.";
            show_information_with_leading_time_stamp(osstream);
            this->internal_variable_name = "";
        }
    }
    else
    {
        osstream<<"Warning, Meter type '"<<meter_type<<"' is not valid Internal Variable Meter. No internal variable name will be set.";
        show_information_with_leading_time_stamp(osstream);
        this->internal_variable_name = "";
    }
}

bool METER::is_internal_variable_name_valid(string name) const
{
    if(get_device_pointer()==NULL)
        return false;

    name = string2upper(name);
    string meter_type = get_meter_type();
    MODEL* model=NULL;
    if(get_device_type()=="LOAD")
    {
        LOAD* ptr = (LOAD*)get_device_pointer();
        if(meter_type=="LOAD MODEL INTERNAL VARIABLE")
            model = ptr->get_load_model();
        if(meter_type=="FREQUENCY RELAY MODEL INTERNAL VARIABLE")
            model = ptr->get_load_frequency_relay_model();
        if(meter_type=="VOLTAGE RELAY MODEL INTERNAL VARIABLE")
            model = ptr->get_load_voltage_relay_model();
    }
    if(get_device_type()=="GENERATOR")
    {
        GENERATOR* ptr = (GENERATOR*)get_device_pointer();
        if(meter_type=="SYNC GENERATOR MODEL INTERNAL VARIABLE")
            model = ptr->get_sync_generator_model();
        if(meter_type=="COMPENSATOR MODEL INTERNAL VARIABLE")
            model = ptr->get_compensator_model();
        if(meter_type=="EXCITER MODEL INTERNAL VARIABLE")
            model = ptr->get_exciter_model();
        if(meter_type=="STABILIZER MODEL INTERNAL VARIABLE")
            model = ptr->get_stabilizer_model();
        if(meter_type=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE")
            model = ptr->get_turbine_governor_model();
    }
    if(get_device_type()=="WT GENERATOR")
    {
        WT_GENERATOR* ptr = (WT_GENERATOR*)get_device_pointer();
        if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
            model = ptr->get_wt_generator_model();
        if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
            model = ptr->get_wt_aerodynamic_model();
        if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
            model = ptr->get_wt_turbine_model();
        if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
            model = ptr->get_wt_electrical_model();
        if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
            model = ptr->get_wt_pitch_model();
        if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
            model = ptr->get_wind_speed_model();
    }
    if(get_device_type()=="PV UNIT")
    {
        PV_UNIT* ptr = (PV_UNIT*)get_device_pointer();
        if(meter_type=="PV PANEL MODEL INTERNAL VARIABLE")
            model = ptr->get_pv_panel_model();
        if(meter_type=="PV CONVERTER MODEL INTERNAL VARIABLE")
            model = ptr->get_pv_converter_model();
        if(meter_type=="PV ELECTRICAL MODEL INTERNAL VARIABLE")
            model = ptr->get_pv_electrical_model();
        if(meter_type=="PV IRRADIANCE MODEL INTERNAL VARIABLE")
            model = ptr->get_pv_irradiance_model();
    }
    if(get_device_type()=="ENERGY STORAGE")
    {
        ENERGY_STORAGE* ptr = (ENERGY_STORAGE*)get_device_pointer();
        if(meter_type=="ENERGY STORAGE MODEL INTERNAL VARIABLE")
            model = ptr->get_energy_storage_model();
    }
    if(get_device_type()=="HVDC")
    {
        HVDC* ptr = (HVDC*)get_device_pointer();
        if(meter_type=="HVDC MODEL INTERNAL VARIABLE")
            model = ptr->get_hvdc_model();
    }
    if(model!=NULL)
        return model->is_model_variable_exist(name);
    else
    {
        ostringstream osstream;
        osstream<<"Warning. No dynamic model is found when check if internal variable name is valid.";
        show_information_with_leading_time_stamp(osstream);
        return false;
    }
}

void METER::set_meter_side_bus(size_t meter_side)
{
    DEVICE* device = get_device_pointer();
    if(device->is_connected_to_bus(meter_side))
        meter_side_bus = meter_side;
    else
        meter_side_bus = 0;
}


void METER::change_device_id(DEVICE_ID did)
{
    set_device_id(did);
}

void METER::change_meter_type(string meter_type)
{
    this->meter_type = meter_type;
}

void METER::check()
{
    ;// method to disable BASE::check()
}
bool METER::is_valid_meter_type(string meter_type) const
{
    if(not device_id.is_valid())
        return false;

    string device_type = device_id.get_device_type();
    meter_type = string2upper(meter_type);

    return is_valid_meter_type_of_device(meter_type, device_type);
}

bool METER::is_valid_meter_type_of_device(string meter_type, string device_type) const
{
    ostringstream osstream;

    device_type = string2upper(device_type);
    map<string, vector<string> >::const_iterator it = SUPPORTED_METERS.find(device_type);
    if(it==SUPPORTED_METERS.end())
    {
        osstream<<"Warning. Device type "<<device_type<<" is not supported for setting up meter.";
        show_information_with_leading_time_stamp(osstream);
        return false;
    }
    else
    {
        vector<string> supported_meters = it->second;
        size_t n = supported_meters.size();
        bool flag_valid = false;
        for(size_t i=0; i!=n; ++i)
        {
            if(meter_type == supported_meters[i])
            {
                flag_valid = true;
                break;
            }
        }
        return flag_valid;
    }
}

DEVICE_ID METER::get_device_id() const
{
    return device_id;
}

string METER::get_meter_type() const
{
    return meter_type;
}

size_t METER::get_meter_side_bus() const
{
    return meter_side_bus;
}

string METER::get_device_type() const
{
    return device_id.get_device_type();
}

string METER::get_internal_variable_name() const
{
    return internal_variable_name;
}

DEVICE* METER::get_device_pointer() const
{
    return device_pointer;
}

string METER::get_meter_name() const
{
    string name;
    string meter_type = get_meter_type();
    if(meter_type=="")
        return "";
    else
    {
        name = get_meter_type();
        if(meter_type.find("INTERNAL VARIABLE")!=string::npos)
            name += " " + get_internal_variable_name();

        //name += " OF "+get_device_id().get_device_name()+" IN PS "+get_default_power_system_database()->get_system_name();
        name += " @ "+get_device_id().get_device_name();

        string device_type = get_device_type();
        if(device_type=="LINE" or device_type=="TRANSFORMER" )
            name += " @ SIDE "+num2str(get_meter_side_bus());

        name = trim_string(name);
        return name;
    }
}

bool METER::is_valid() const
{
    if(device_id.is_valid() and meter_type!="")
    {
        string device_type = device_id.get_device_type();
        if(device_type=="LINE" or device_type=="TRANSFORMER")
        {
            if(get_meter_side_bus()!=0)
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}

void METER::clear()
{
    device_id.clear();
    device_pointer = NULL;
    meter_type = "";
    meter_side_bus = 0;
    internal_variable_name = "";

    set_buffer_size(1);
    buffer.clear();
}


bool METER::operator==(const METER& meter)
{
    if(this->get_device_id() == meter.get_device_id() and
       this->get_meter_type() == meter.get_meter_type())
    {
        if(this->get_meter_type().find("INTERNAL VARIABLE") == string::npos)
            return true;
        else
        {
            if(this->get_internal_variable_name() == meter.get_internal_variable_name())
                return true;
            else
                return false;
        }
    }
    else
        return false;
}

bool METER::operator!=(const METER& meter)
{
    return not ((*this)==meter);
}

void METER::set_device_pointer()
{
    ostringstream osstream;

    if(not device_id.is_valid())
    {
        device_pointer = NULL;
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    DEVICE* deviceptr = NULL;
    if(device_type=="BUS")
    {
        TERMINAL terminal = device_id.get_device_terminal();
        vector<size_t> tbuses = terminal.get_buses();
        deviceptr = (DEVICE*) psdb.get_bus(tbuses[0]);
    }
    if(device_type=="LINE")
        deviceptr = (DEVICE*) psdb.get_line(device_id);

    if(device_type=="TRANSFORMER")
        deviceptr = (DEVICE*) psdb.get_transformer(device_id);

    if(device_type=="LOAD")
        deviceptr = (DEVICE*) psdb.get_load(device_id);

    if(device_type=="GENERATOR")
        deviceptr = (DEVICE*) psdb.get_generator(device_id);

    if(device_type=="WT GENERATOR")
        deviceptr = (DEVICE*) psdb.get_wt_generator(device_id);

    if(device_type=="PV UNIT")
        deviceptr = (DEVICE*) psdb.get_pv_unit(device_id);

    if(device_type=="ENERGY STORAGE")
        deviceptr = (DEVICE*) psdb.get_energy_storage(device_id);

    if(device_type=="HVDC")
        deviceptr = (DEVICE*) psdb.get_hvdc(device_id);

    if(device_type=="EQUIVALENT DEVICE")
        deviceptr = (DEVICE*) psdb.get_equivalent_device(device_id);

    this->device_pointer = deviceptr;
    if(deviceptr==NULL)
    {
        ostringstream osstream;
        osstream<<"Device cannot be found in database for setting meter. Device is "<<device_id.get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void METER::set_buffer_size(size_t bsize)
{
    buffer.set_buffer_size(bsize);
}

size_t METER::get_buffer_size() const
{
    return buffer.get_buffer_size();
}

void METER::initialize_meter_buffer()
{
    if(get_device_pointer()==NULL)
        set_device_pointer();

    double current_time = get_dynamic_simulation_time_in_s();

    double value = get_meter_value();
    buffer.initialize_buffer(current_time, value);
}

void METER::update_meter_buffer()
{
    if(get_device_pointer()==NULL)
        set_device_pointer();

    double current_time = get_dynamic_simulation_time_in_s();

    buffer.append_data(current_time, get_meter_value());
}


double METER::get_meter_value_from_buffer_with_delay(size_t index) const
{
    return buffer.get_buffer_value_at_delay_index(index);
}

double METER::get_meter_value_from_buffer_at_time(double time) const
{
    return buffer.get_buffer_value_at_time(time);
}

size_t METER::get_delay_index_of_time(double time) const
{
    return buffer.get_delay_index_of_time(time);
}

double METER::get_meter_value() const
{
    if(get_device_pointer()==NULL)
        return 0.0;

    string device_type = get_device_type();
    if(device_type=="BUS")
        return get_meter_value_as_a_bus();
    if(device_type=="LINE")
        return get_meter_value_as_a_line();
    if(device_type=="TRANSFORMER")
        return get_meter_value_as_a_transformer();
    if(device_type=="LOAD")
        return get_meter_value_as_a_load();
    if(device_type=="GENERATOR")
        return get_meter_value_as_a_generator();
    if(device_type=="WT GENERATOR")
        return get_meter_value_as_a_wt_generator();
    if(device_type=="HVDC")
        return get_meter_value_as_an_hvdc();
    if(device_type=="EQUIVALENT DEVICE")
        return get_meter_value_as_an_equivalent_device();

    return 0.0;
}


double METER::get_meter_value_as_a_bus() const
{
    BUS* bus = (BUS*) get_device_pointer();
    if(bus->get_bus_type()==OUT_OF_SERVICE)
        return 0.0;

    if(meter_type=="VOLTAGE IN PU")
        return bus->get_voltage_in_pu();
    if(meter_type=="VOLTAGE IN KV")
        return bus->get_voltage_in_pu();
    if(meter_type=="ANGLE IN DEG")
        return bus->get_angle_in_deg();
    if(meter_type=="FREQUENCY DEVIATION IN PU")
        return bus->get_frequency_deviation_in_pu();
    if(meter_type=="FREQUENCY DEVIATION IN HZ")
        return bus->get_frequency_deviation_in_Hz();
    if(meter_type=="FREQUENCY IN PU")
        return 1.0+bus->get_frequency_deviation_in_pu();
    if(meter_type=="FREQUENCY IN HZ")
        return bus->get_base_frequency_in_Hz()+bus->get_frequency_deviation_in_Hz();

    return 0.0;
}

double METER::get_meter_value_as_a_line() const
{
    LINE* line = (LINE*) get_device_pointer();
    if(line == NULL)
        return 0.0;

    if(line->get_sending_side_breaker_status()==false and line->get_receiving_side_breaker_status()==false)
        return 0.0;

    size_t metered_bus = get_meter_side_bus();
    if(meter_type=="CURRENT IN KA")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_kA());
        else
            return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_kA());
    }
    if(meter_type=="CURRENT IN PU")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_pu());
        else
            return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_pu());
    }

    if(meter_type=="ACTIVE POWER IN MW")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return (line->get_line_complex_power_at_sending_side_in_MVA()).real();
        else
            return (line->get_line_complex_power_at_receiving_side_in_MVA()).real();
    }

    if(meter_type=="ACTIVE POWER IN PU")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return (line->get_line_complex_power_at_sending_side_in_pu()).real();
        else
            return (line->get_line_complex_power_at_receiving_side_in_pu()).real();
    }

    if(meter_type=="REACTIVE POWER IN MVAR")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return (line->get_line_complex_power_at_sending_side_in_MVA()).imag();
        else
            return (line->get_line_complex_power_at_receiving_side_in_MVA()).imag();
    }

    if(meter_type=="REACTIVE POWER IN PU")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return (line->get_line_complex_power_at_sending_side_in_pu()).imag();
        else
            return (line->get_line_complex_power_at_receiving_side_in_pu()).imag();
    }

    if(meter_type=="APPARENT IMPEDANCE IN OHM")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
        else
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
    }

    if(meter_type=="APPARENT IMPEDANCE IN PU")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_pu());
        else
            return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_pu());
    }

    if(meter_type=="APPARENT IMPEDANCE ANGLE IN DEG")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm()));
        else
            return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm()));
    }

    if(meter_type=="APPARENT IMPEDANCE ANGLE IN RAD")
    {
        if(metered_bus!=line->get_receiving_side_bus())
            return steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
        else
            return steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
    }


    if(meter_type=="CURRENT AT SENDING SIDE IN KA")
        return steps_fast_complex_abs(line->get_line_complex_current_at_sending_side_in_kA());
    if(meter_type=="ACTIVE POWER AT SENDING SIDE IN MW")
        return (line->get_line_complex_power_at_sending_side_in_MVA()).real();
    if(meter_type=="REACTIVE POWER AT SENDING SIDE IN MVAR")
        return (line->get_line_complex_power_at_sending_side_in_MVA()).imag();
    if(meter_type=="CURRENT AT RECEIVING SIDE IN KA")
        return steps_fast_complex_abs(line->get_line_complex_current_at_receiving_side_in_kA());
    if(meter_type=="ACTIVE POWER AT RECEIVING SIDE IN MW")
        return (line->get_line_complex_power_at_receiving_side_in_MVA()).real();
    if(meter_type=="REACTIVE POWER AT RECEIVING SIDE IN MVAR")
        return (line->get_line_complex_power_at_receiving_side_in_MVA()).imag();

    if(meter_type=="APPARENT IMPEDANCE AT SENDING SIDE IN OHM")
        return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm());
    if(meter_type=="APPARENT IMPEDANCE ANGLE AT SENDING SIDE IN DEG")
        return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_sending_side_in_ohm()));
    if(meter_type=="APPARENT IMPEDANCE AT RECEIVING SIDE IN OHM")
        return steps_fast_complex_abs(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm());
    if(meter_type=="APPARENT IMPEDANCE ANGLE AT RECEIVING SIDE IN DEG")
        return rad2deg(steps_fast_complex_arg(line->get_line_complex_apparent_impedance_at_receiving_side_in_ohm()));

    return 0.0;
}

double METER::get_meter_value_as_a_transformer() const
{
    TRANSFORMER* trans = (TRANSFORMER*) get_device_pointer();
    if(trans == NULL)
        return 0.0;
    size_t metered_bus = get_meter_side_bus();
    if(trans->is_two_winding_transformer())
    {
        if(meter_type=="CURRENT IN KA")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
            else
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
        }

        if(meter_type=="CURRENT IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(PRIMARY_SIDE));
            else
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(SECONDARY_SIDE));
        }

        if(meter_type=="ACTIVE POWER IN MW")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
            else
                return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
        }

        if(meter_type=="ACTIVE POWER IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).real();
            else
                return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).real();
        }

        if(meter_type=="REACTIVE POWER IN MVAR")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
            else
                return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
        }

        if(meter_type=="REACTIVE POWER IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).imag();
            else
                return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).imag();
        }
    }
    else
    {
        if(meter_type=="CURRENT IN KA")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
                else
                    return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(TERTIARY_SIDE));
            }
        }

        if(meter_type=="CURRENT IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(PRIMARY_SIDE));
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(SECONDARY_SIDE));
                else
                    return steps_fast_complex_abs(trans->get_winding_complex_current_in_pu(TERTIARY_SIDE));
            }
        }

        if(meter_type=="ACTIVE POWER IN MW")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
                else
                    return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).real();
            }
        }

        if(meter_type=="ACTIVE POWER IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).real();
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).real();
                else
                    return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).real();
            }
        }

        if(meter_type=="REACTIVE POWER IN MVAR")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
                else
                    return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).imag();
            }
        }

        if(meter_type=="REACTIVE POWER IN PU")
        {
            if(metered_bus!=trans->get_winding_bus(SECONDARY_SIDE) and metered_bus!=trans->get_winding_bus(TERTIARY_SIDE))
                return (trans->get_winding_complex_power_in_pu(PRIMARY_SIDE)).imag();
            else
            {
                if(metered_bus==trans->get_winding_bus(SECONDARY_SIDE))
                    return (trans->get_winding_complex_power_in_pu(SECONDARY_SIDE)).imag();
                else
                    return (trans->get_winding_complex_power_in_pu(TERTIARY_SIDE)).imag();
            }
        }
    }



    if(meter_type=="CURRENT AT PRIMARY WINDING IN KA")
        return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(PRIMARY_SIDE));
    if(meter_type=="CURRENT AT SECONDARY WINDING IN KA")
        return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(SECONDARY_SIDE));
    if(meter_type=="CURRENT AT TERTIARY WINDING IN KA")
        return steps_fast_complex_abs(trans->get_winding_complex_current_in_kA(TERTIARY_SIDE));
    if(meter_type=="ACTIVE POWER AT PRIMARY WINDING IN MW")
        return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).real();
    if(meter_type=="ACTIVE POWER AT SECONDARY WINDING IN MW")
        return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).real();
    if(meter_type=="ACTIVE POWER AT TERTIARY WINDING IN MW")
        return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).real();
    if(meter_type=="REACTIVE POWER AT PRIMARY WINDING IN MVAR")
        return (trans->get_winding_complex_power_in_MVA(PRIMARY_SIDE)).imag();
    if(meter_type=="REACTIVE POWER AT SECONDARY WINDING IN MVAR")
        return (trans->get_winding_complex_power_in_MVA(SECONDARY_SIDE)).imag();
    if(meter_type=="REACTIVE POWER AT TERTIARY WINDING IN MVAR")
        return (trans->get_winding_complex_power_in_MVA(TERTIARY_SIDE)).imag();

    return 0.0;
}

double METER::get_meter_value_as_a_load() const
{
    LOAD* load = (LOAD*) get_device_pointer();
    if(load==NULL)
        return 0.0;
    if(load->get_status()==false)
        return 0.0;

    if(meter_type=="ACTIVE POWER IN MW")
        return (load->get_dynamic_load_in_MVA()).real();

    if(meter_type=="ACTIVE POWER IN PU")
        return (load->get_dynamic_load_in_pu()).real();

    if(meter_type=="REACTIVE POWER IN MVAR")
        return (load->get_dynamic_load_in_MVA()).imag();

    if(meter_type=="REACTIVE POWER IN PU")
        return (load->get_dynamic_load_in_pu()).imag();

    if(meter_type=="CURRENT IN KA")
    {
        POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
        double sbase = psdb.get_system_base_power_in_MVA();
        double vbase = psdb.get_bus_base_voltage_in_kV(load->get_load_bus());
        double ibase = sbase/(sqrt(3.0)*vbase);
        return ibase* steps_fast_complex_abs(load->get_dynamics_load_current_in_pu_based_on_system_base_power());
    }


    if(meter_type=="CURRENT IN PU")
    {
        return steps_fast_complex_abs(load->get_dynamics_load_current_in_pu_based_on_system_base_power());
    }

    if(meter_type=="LOAD MODEL INTERNAL VARIABLE")
    {
        LOAD_MODEL* model = load->get_load_model();
        if(model==NULL)
            return 0.0;
        else
            return model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="FREQUENCY RELAY MODEL INTERNAL VARIABLE")
    {
        LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();
        if(model==NULL)
            return 0.0;
        else
            return model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="VOLTAGE RELAY MODEL INTERNAL VARIABLE")
    {
        LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();
        if(model==NULL)
            return 0.0;
        else
            return model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="TOTAL SCALE IN PU")
    {
        return load->get_load_total_scale_factor_in_pu();
    }
    if(meter_type=="MANUALLY SCALE IN PU")
    {
        return load->get_load_manually_scale_factor_in_pu();
    }
    if(meter_type=="RELAY SHED SCALE IN PU")
    {
        return load->get_load_relay_shed_scale_factor_in_pu();
    }
    return 0.0;
}
double METER::get_meter_value_as_a_generator() const
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator == NULL)
        return 0.0;

    if(generator->get_status()==false)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double fbase = psdb.get_bus_base_frequency_in_Hz(generator->get_generator_bus());
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = generator->get_mbase_in_MVA();

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    COMPENSATOR_MODEL* comp_model = generator->get_compensator_model();
    STABILIZER_MODEL* stabilizer_model = generator->get_stabilizer_model();
    EXCITER_MODEL* exciter_model = generator->get_exciter_model();
    TURBINE_GOVERNOR_MODEL* turbine_governor_model = generator->get_turbine_governor_model();

    if(meter_type=="ROTOR ANGLE IN DEG")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_rotor_angle_in_deg();
    }
    if(meter_type =="ROTOR SPEED IN PU")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_rotor_speed_in_pu();
    }
    if(meter_type =="ROTOR SPEED IN HZ")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return fbase*gen_model->get_rotor_speed_in_pu();
    }
    if(meter_type =="ROTOR SPEED DEVIATION IN PU")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_rotor_speed_deviation_in_pu();
    }
    if(meter_type =="ROTOR SPEED DEVIATION IN HZ")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return fbase*gen_model->get_rotor_speed_deviation_in_pu();
    }
    if(meter_type =="INTERNAL VOLTAGE IN PU")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return steps_fast_complex_abs(gen_model->get_internal_voltage_in_pu_in_dq_axis());
    }
    if(meter_type =="TERMINAL CURRENT IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_current_in_pu_based_on_mbase();
    }
    if(meter_type =="TERMINAL CURRENT IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_current_in_pu_based_on_sbase();
    }
    if(meter_type =="TERMINAL CURRENT IN KA")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double vbase = psdb.get_bus_base_voltage_in_kV(generator->get_generator_bus());
            double ibase = sbase/(sqrt(3.0)*vbase);
            return ibase*gen_model->get_terminal_current_in_pu_based_on_sbase();
        }
    }
    if(meter_type =="TERMINAL ACTIVE POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_MW()/mbase;
    }
    if(meter_type =="TERMINAL ACTIVE POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_MW()/sbase;
    }
    if(meter_type =="TERMINAL ACTIVE POWER IN MW")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_MW();
    }
    if(meter_type =="TERMINAL REACTIVE POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_MVar()/mbase;
    }
    if(meter_type =="TERMINAL REACTIVE POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_MVar()/sbase;
    }
    if(meter_type =="TERMINAL REACTIVE POWER IN MVAR")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_MVar();
    }
    if(meter_type =="TERMINAL APPRAENT POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_pu_based_on_mbase();
            double q = gen_model->get_terminal_reactive_power_in_pu_based_on_mbase();
            return sqrt(p*p+q*q);
        }
    }
    if(meter_type =="TERMINAL APPRAENT POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_pu_based_on_mbase();
            double q = gen_model->get_terminal_reactive_power_in_pu_based_on_mbase();
            return sqrt(p*p+q*q)*mbase/sbase;
        }
    }
    if(meter_type =="TERMINAL APPRAENT POWER IN MVA")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_pu_based_on_mbase();
            double q = gen_model->get_terminal_reactive_power_in_pu_based_on_mbase();
            return sqrt(p*p+q*q)*mbase;
        }
    }
    if(meter_type =="AIRGAP POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_air_gap_power_in_pu_based_on_mbase();
    }
    if(meter_type =="AIRGAP POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_air_gap_power_in_MW()/sbase;
    }
    if(meter_type =="AIRGAP POWER IN MW")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_air_gap_power_in_MW();
    }
    if(meter_type =="ACCELERATING POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_accelerating_power_in_pu_based_on_mbase();
    }
    if(meter_type =="ACCELERATING POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_accelerating_power_in_MW()/sbase;
    }
    if(meter_type =="ACCELERATING POWER IN MW")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_accelerating_power_in_MW();
    }
    if(meter_type =="MECHANICAL POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_mechanical_power_in_pu_based_on_mbase();
    }
    if(meter_type =="MECHANICAL POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_mechanical_power_in_MW()/sbase;
    }
    if(meter_type =="MECHANICAL POWER IN MW")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_mechanical_power_in_MW();
    }
    if(meter_type =="MECHANICAL POWER REFERENCE IN PU ON MBASE")
    {
        if(turbine_governor_model == NULL)
            return 0.0;
        else
            return turbine_governor_model->get_mechanical_power_reference_in_pu_based_on_mbase();
    }
    if(meter_type =="MECHANICAL POWER REFERENCE IN PU ON SBASE")
    {
        if(turbine_governor_model == NULL)
            return 0.0;
        else
            return turbine_governor_model->get_mechanical_power_reference_in_pu_based_on_mbase()*mbase/sbase;
    }
    if(meter_type =="MECHANICAL POWER REFERENCE IN MW")
    {
        if(turbine_governor_model == NULL)
            return 0.0;
        else
            return turbine_governor_model->get_mechanical_power_reference_in_pu_based_on_mbase()*mbase;
    }
    if(meter_type =="COMPENSATED VOLTAGE IN PU")
    {
        if(exciter_model == NULL)
            return 0.0;
        else
            return exciter_model->get_compensated_voltage_in_pu();
    }
    if(meter_type =="VOLTAGE REFERENCE IN PU")
    {
        if(exciter_model == NULL)
            return 0.0;
        else
            return exciter_model->get_voltage_reference_in_pu();
    }
    if(meter_type =="STABILIZING SIGNAL IN PU")
    {
        if(exciter_model == NULL)
            return 0.0;
        else
            return exciter_model->get_stabilizing_signal_in_pu();
    }
    if(meter_type =="EXCITATION VOLTAGE IN PU")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_excitation_voltage_in_pu();
    }
    if(meter_type=="SYNC GENERATOR MODEL INTERNAL VARIABLE")
    {
        if(gen_model==NULL)
            return 0.0;
        else
            return gen_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="COMPENSATOR MODEL INTERNAL VARIABLE")
    {
        if(comp_model==NULL)
            return 0.0;
        else
            return comp_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="STABILIZER MODEL INTERNAL VARIABLE")
    {
        if(stabilizer_model==NULL)
            return 0.0;
        else
            return stabilizer_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="EXCITER MODEL INTERNAL VARIABLE")
    {
        if(exciter_model==NULL)
            return 0.0;
        else
            return exciter_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE")
    {
        if(turbine_governor_model==NULL)
            return 0.0;
        else
            return turbine_governor_model->get_internal_variable_with_name(internal_variable_name);
    }

    return 0.0;
}

double METER::get_meter_value_as_a_wt_generator() const
{
    ostringstream osstream;

    WT_GENERATOR* generator = (WT_GENERATOR*) get_device_pointer();
    if(generator == NULL)
        return 0.0;

    if(generator->get_status()==false)
        return 0.0;

    size_t bus = generator->get_generator_bus();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    double fbase = psdb.get_bus_base_frequency_in_Hz(generator->get_generator_bus());
    double sbase = psdb.get_system_base_power_in_MVA();
    double mbase = generator->get_mbase_in_MVA();

    WT_GENERATOR_MODEL* gen_model = generator->get_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aerd_model = generator->get_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine_model = generator->get_wt_turbine_model();
    WT_ELECTRICAL_MODEL* electrical_model = generator->get_wt_electrical_model();
    WT_PITCH_MODEL* pitch_model = generator->get_wt_pitch_model();
    WIND_SPEED_MODEL* windspeed_model = generator->get_wind_speed_model();

    if(meter_type=="TERMINAL CURRENT IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_current_in_pu_based_on_mbase();
    }
    if(meter_type=="TERMINAL CURRENT IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_current_in_pu_based_on_mbase()*mbase/sbase;
    }

    if(meter_type=="TERMINAL CURRENT IN KA")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double vbase = psdb.get_bus_base_voltage_in_kV(bus);
            double ibase = mbase/(sqrt(3.0)*vbase);
            return gen_model->get_terminal_current_in_pu_based_on_mbase()*ibase;
        }
    }
    if(meter_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_pu_based_on_mbase();
    }
    if(meter_type=="TERMINAL ACTIVE POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_MW()/sbase;
    }
    if(meter_type=="TERMINAL ACTIVE POWER IN MW")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_active_power_in_MW();
    }
    if(meter_type=="TERMINAL REACTIVE POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_pu_based_on_mbase();
    }
    if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_MVar()/sbase;
    }
    if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
    {
        if(gen_model == NULL)
            return 0.0;
        else
            return gen_model->get_terminal_reactive_power_in_MVar();
    }
    if(meter_type=="TERMINAL APPARENT POWER IN PU ON MBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_pu_based_on_mbase();
            double q = gen_model->get_terminal_reactive_power_in_pu_based_on_mbase();
            return sqrt(p*p+q*q);
        }
    }
    if(meter_type=="TERMINAL APPARENT POWER IN PU ON SBASE")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_MW()/sbase;
            double q = gen_model->get_terminal_reactive_power_in_MVar()/sbase;
            return sqrt(p*p+q*q);
        }
    }
    if(meter_type=="TERMINAL APPARENT POWER IN MVAR")
    {
        if(gen_model == NULL)
            return 0.0;
        else
        {
            double p = gen_model->get_terminal_active_power_in_MW();
            double q = gen_model->get_terminal_reactive_power_in_MVar();
            return sqrt(p*p+q*q);
        }
    }
    if(meter_type=="MECHANICAL POWER IN PU ON MBASE")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
            return aerd_model->get_turbine_mechanical_power_in_MW()/mbase;
    }
    if(meter_type=="MECHANICAL POWER IN PU ON SBASE")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
            return aerd_model->get_turbine_mechanical_power_in_MW()/sbase;
    }
    if(meter_type=="MECHANICAL POWER IN MW")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
            return aerd_model->get_turbine_mechanical_power_in_MW();
    }
    if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN PU ON MBASE")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
        {
            double vwind = aerd_model->get_wind_speed_in_mps();
            double pmax = aerd_model->get_maximum_available_mechanical_power_per_wt_generator_in_MW(vwind);
            size_t n = generator->get_number_of_lumped_wt_generators();
            return pmax*n/mbase;
        }
    }
    if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN PU ON SBASE")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
        {
            double vwind = aerd_model->get_wind_speed_in_mps();
            double pmax = aerd_model->get_maximum_available_mechanical_power_per_wt_generator_in_MW(vwind);
            size_t n = generator->get_number_of_lumped_wt_generators();
            return pmax*n/sbase;
        }
    }
    if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN MW")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
        {
            double vwind = aerd_model->get_wind_speed_in_mps();
            double pmax = aerd_model->get_maximum_available_mechanical_power_per_wt_generator_in_MW(vwind);
            size_t n = generator->get_number_of_lumped_wt_generators();
            return pmax*n;
        }
    }
    if(meter_type=="SPEED REFERENCE IN PU")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
            return aerd_model->get_turbine_reference_speed_in_pu();
    }
    if(meter_type=="SPEED REFERENCE IN RAD/S")
    {
        if(aerd_model == NULL)
            return 0.0;
        else
            return aerd_model->get_turbine_reference_speed_in_rad_per_s();
    }
    if(meter_type=="TURBINE SPEED DEVIATION IN PU")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_turbine_speed_in_pu()-1.0;
    }
    if(meter_type=="TURBINE SPEED DEVIATION IN HZ")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
        {
            double fn = aerd_model->get_nominal_turbine_speed_in_rad_per_s()/(2.0*PI);
            return (turbine_model->get_turbine_speed_in_pu()-1.0)*fn;
        }
    }
    if(meter_type=="TURBINE SPEED IN PU")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_turbine_speed_in_pu();
    }
    if(meter_type=="TURBINE SPEED IN HZ")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
        {
            double wn = aerd_model->get_nominal_turbine_speed_in_rad_per_s()/(2.0*PI);
            return turbine_model->get_turbine_speed_in_pu()*wn;
        }
    }
    if(meter_type=="ROTOR SPEED DEVIATION IN PU")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_generator_speed_in_pu()-1.0;
    }
    if(meter_type=="ROTOR SPEED DEVIATION IN HZ")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
        {
            double fn = fbase/aerd_model->get_number_of_pole_pairs();
            return (turbine_model->get_generator_speed_in_pu()-1.0)*fn;
        }
    }
    if(meter_type=="ROTOR SPEED IN PU")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_generator_speed_in_pu();
    }
    if(meter_type=="ROTOR SPEED IN HZ")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
        {
            double fn = fbase/aerd_model->get_number_of_pole_pairs();
            return turbine_model->get_generator_speed_in_pu()*fn;
        }
    }
    if(meter_type=="ROTOR ANGLE IN DEG")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_rotor_angle_in_deg();
    }
    if(meter_type=="ROTOR ANGLE IN RAD")
    {
        if(turbine_model == NULL)
            return 0.0;
        else
            return turbine_model->get_rotor_angle_in_rad();
    }
    if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
    {
        if(electrical_model == NULL)
            return gen_model->get_active_current_command_in_pu_based_on_mbase();
        else
            return electrical_model->get_active_current_command_in_pu_based_on_mbase();
    }
    if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
    {
        if(electrical_model == NULL)
            return gen_model->get_reactive_current_command_in_pu_based_on_mbase();
        else
            return electrical_model->get_reactive_current_command_in_pu_based_on_mbase();
    }
    if(meter_type=="ACTIVE POWER COMMAND IN PU")
    {
        if(electrical_model == NULL)
            return 0.0;
        else
            return electrical_model->get_active_power_command_in_pu_based_on_mbase();
    }
    if(meter_type=="REACTIVE POWER COMMAND IN PU")
    {
        if(electrical_model == NULL)
            return 0.0;
        else
            return electrical_model->get_reactive_power_command_in_pu_based_on_mbase();
    }
    if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
    {
        if(electrical_model == NULL)
            return 0.0;
        else
            return electrical_model->get_reactive_voltage_command_in_pu_based_on_mbase();
    }
    if(meter_type=="PITCH ANGLE IN DEG")
    {
        if(pitch_model == NULL)
            return aerd_model->get_initial_pitch_angle_in_deg();
        else
            return pitch_model->get_pitch_angle_in_deg();
    }
    if(meter_type=="WIND SPEED IN PU")
    {
        if(windspeed_model == NULL)
            return aerd_model->get_wind_speed_in_mps()/aerd_model->get_nominal_wind_speed_in_mps();
        else
            return windspeed_model->get_wind_speed_in_pu();
    }
    if(meter_type=="WIND SPEED IN MPS")
    {
        if(windspeed_model == NULL)
            return aerd_model->get_wind_speed_in_mps();
        else
            return windspeed_model->get_wind_speed_in_mps();
    }
    if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
    {
        if(gen_model==NULL)
            return 0.0;
        else
            return gen_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
    {
        if(aerd_model==NULL)
            return 0.0;
        else
            return aerd_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
    {
        if(turbine_model==NULL)
            return 0.0;
        else
            return turbine_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
    {
        if(electrical_model==NULL)
            return 0.0;
        else
            return electrical_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
    {
        if(pitch_model==NULL)
            return 0.0;
        else
            return pitch_model->get_internal_variable_with_name(internal_variable_name);
    }
    if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
    {
        if(windspeed_model==NULL)
            return 0.0;
        else
            return windspeed_model->get_internal_variable_with_name(internal_variable_name);
    }
    return 0.0;
}

double METER::get_meter_value_as_an_hvdc() const
{
    HVDC* hvdc = (HVDC*) get_device_pointer();
    if(hvdc == NULL)
        return 0.0;

    if(hvdc->get_status()==false)
        return 0.0;

    HVDC_MODEL* hvdc_model = hvdc->get_hvdc_model();

    if(meter_type=="DC CURRENT IN KA")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_current_in_kA(RECTIFIER);
    }

    if(meter_type=="RECTIFIER DC CURRENT IN KA")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_current_in_kA(RECTIFIER);
    }

    if(meter_type=="RECTIFIER AC CURRENT IN KA")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return steps_fast_complex_abs(hvdc_model->get_converter_ac_current_in_kA(RECTIFIER));
    }

    if(meter_type=="INVERTER DC CURRENT IN KA")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_current_in_kA(INVERTER);
    }
    if(meter_type=="INVERTER AC CURRENT IN KA")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return steps_fast_complex_abs(hvdc_model->get_converter_ac_current_in_kA(INVERTER));
    }

    if(meter_type=="RECTIFIER ALPHA IN DEG")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_alpha_or_gamma_in_deg(RECTIFIER);
    }

    if(meter_type=="INVERTER GAMMA IN DEG")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_alpha_or_gamma_in_deg(INVERTER);
    }

    if(meter_type=="RECTIFIER MU IN DEG")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_commutation_overlap_angle_in_deg(RECTIFIER);
    }

    if(meter_type=="INVERTER MU IN DEG")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_commutation_overlap_angle_in_deg(INVERTER);
    }

    if(meter_type=="RECTIFIER DC VOLTAGE IN KV")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_voltage_in_kV(RECTIFIER);
    }

    if(meter_type=="INVERTER DC VOLTAGE IN KV")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_voltage_in_kV(INVERTER);
    }

    if(meter_type=="RECTIFIER AC VOLTAGE IN PU")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_voltage_in_pu(RECTIFIER);
    }

    if(meter_type=="INVERTER AC VOLTAGE IN PU")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_voltage_in_pu(INVERTER);
    }

    if(meter_type=="RECTIFIER DC POWER IN MW")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_power_in_MW(RECTIFIER);
    }

    if(meter_type=="INVERTER DC POWER IN MW")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_dc_power_in_MW(INVERTER);
    }

    if(meter_type=="RECTIFIER AC ACTIVE POWER IN MW")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_complex_power_in_MVA(RECTIFIER).real();
    }

    if(meter_type=="INVERTER AC ACTIVE POWER IN MW")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_complex_power_in_MVA(INVERTER).real();
    }

    if(meter_type=="RECTIFIER AC REACTIVE POWER IN MVAR")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_complex_power_in_MVA(RECTIFIER).imag();
    }

    if(meter_type=="INVERTER AC REACTIVE POWER IN MVAR")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_converter_ac_complex_power_in_MVA(INVERTER).imag();
    }

    if(meter_type=="HVDC MODEL INTERNAL VARIABLE")
    {
        if(hvdc_model == NULL)
            return 0.0;
        else
            return hvdc_model->get_internal_variable_with_name(internal_variable_name);
    }

    return 0.0;
}

double METER::get_meter_value_as_an_equivalent_device() const
{
    EQUIVALENT_DEVICE* edevice = (EQUIVALENT_DEVICE*) get_device_pointer();
    if(edevice == NULL)
        return 0.0;

    if(meter_type=="VOLTAGE SOURCE VOLTAGE IN PU")
    {
        return steps_fast_complex_abs(edevice->get_equivalent_voltage_source_voltage_in_pu());
        if(edevice->get_equivalent_voltage_source_status()==false)
            return 0.0;
        else
            return steps_fast_complex_abs(edevice->get_equivalent_voltage_source_voltage_in_pu());
    }

    if(meter_type=="VOLTAGE SOURCE VOLTAGE ANGLE IN DEG")
    {
        return rad2deg(steps_fast_complex_arg(edevice->get_equivalent_voltage_source_voltage_in_pu()));
        if(edevice->get_equivalent_voltage_source_status()==false)
            return 0.0;
        else
            return rad2deg(steps_fast_complex_arg(edevice->get_equivalent_voltage_source_voltage_in_pu()));
    }

    if(meter_type=="VOLTAGE SOURCE RESISTANCE IN PU")
    {
        return edevice->get_equivalent_voltage_source_impedance_in_pu().real();
        if(edevice->get_equivalent_voltage_source_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_voltage_source_impedance_in_pu().real();
    }

    if(meter_type=="VOLTAGE SOURCE REACTANCE IN PU")
    {
        return edevice->get_equivalent_voltage_source_impedance_in_pu().imag();
        if(edevice->get_equivalent_voltage_source_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_voltage_source_impedance_in_pu().imag();
    }

    if(meter_type=="ACTIVE CONSTANT POWER LOAD IN MW")
    {
        return edevice->get_equivalent_nominal_constant_power_load_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_power_load_in_MVA().real();
    }

    if(meter_type=="REACTIVE CONSTANT POWER LOAD IN MVAR")
    {
        return edevice->get_equivalent_nominal_constant_power_load_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_power_load_in_MVA().imag();
    }

    if(meter_type=="ACTIVE CONSTANT CURRENT LOAD IN MW")
    {
        return edevice->get_equivalent_nominal_constant_current_load_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_current_load_in_MVA().real();
    }

    if(meter_type=="REACTIVE CONSTANT CURRENT LOAD IN MVAR")
    {
        return edevice->get_equivalent_nominal_constant_current_load_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_current_load_in_MVA().imag();
    }

    if(meter_type=="ACTIVE CONSTANT IMPEDANCE LOAD IN MW")
    {
        return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().real();
    }

    if(meter_type=="REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR")
    {
        return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_impedance_load_in_MVA().imag();
    }

    if(meter_type=="ACTIVE POWER GENERATION IN MW")
    {
        return edevice->get_equivalent_generation_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_generation_in_MVA().real();
    }
    if(meter_type=="REACTIVE POWER GENERATION IN MVAR")
    {
        return edevice->get_equivalent_generation_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_generation_in_MVA().imag();
    }
    if(meter_type=="ACTIVE POWER LOAD IN MW")
    {
        return edevice->get_equivalent_load_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_load_in_MVA().real();
    }
    if(meter_type=="REACTIVE POWER LOAD IN MVAR")
    {
        return edevice->get_equivalent_load_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_load_in_MVA().imag();
    }
    if(meter_type=="ACTIVE POWER NET LOAD IN MW")
    {
        return edevice->get_total_equivalent_power_as_load_in_MVA().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_total_equivalent_power_as_load_in_MVA().real();
    }
    if(meter_type=="REACTIVE POWER NET LOAD IN MVAR")
    {
        return edevice->get_total_equivalent_power_as_load_in_MVA().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_total_equivalent_power_as_load_in_MVA().imag();
    }

    if(meter_type=="ACTIVE CONSTANT POWER LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_power_load_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_power_load_in_pu().real();
    }

    if(meter_type=="REACTIVE CONSTANT POWER LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_power_load_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_power_load_in_pu().imag();
    }

    if(meter_type=="ACTIVE CONSTANT CURRENT LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_current_load_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_current_load_in_pu().real();
    }

    if(meter_type=="REACTIVE CONSTANT CURRENT LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_current_load_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_current_load_in_pu().imag();
    }

    if(meter_type=="ACTIVE CONSTANT IMPEDANCE LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().real();
    }

    if(meter_type=="REACTIVE CONSTANT IMPEDANCE LOAD IN PU")
    {
        return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_nominal_constant_impedance_load_in_pu().imag();
    }

    if(meter_type=="ACTIVE POWER GENERATION IN PU")
    {
        return edevice->get_equivalent_generation_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_generation_in_pu().real();
    }
    if(meter_type=="REACTIVE POWER GENERATION IN PU")
    {
        return edevice->get_equivalent_generation_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_generation_in_pu().imag();
    }
    if(meter_type=="ACTIVE POWER LOAD IN PU")
    {
        return edevice->get_equivalent_load_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_load_in_pu().real();
    }
    if(meter_type=="REACTIVE POWER LOAD IN PU")
    {
        return edevice->get_equivalent_load_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_equivalent_load_in_pu().imag();
    }
    if(meter_type=="ACTIVE POWER NET LOAD IN PU")
    {
        return edevice->get_total_equivalent_power_as_load_in_pu().real();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_total_equivalent_power_as_load_in_pu().real();
    }
    if(meter_type=="REACTIVE POWER NET LOAD IN PU")
    {
        return edevice->get_total_equivalent_power_as_load_in_pu().imag();
        if(edevice->get_equivalent_load_status()==false)
            return 0.0;
        else
            return edevice->get_total_equivalent_power_as_load_in_pu().imag();
    }

    return 0.0;
}


double METER::get_meter_value_as_an_energy_storage() const
{
    ENERGY_STORAGE* estorage = (ENERGY_STORAGE*) get_device_pointer();
    if(estorage == NULL)
        return 0.0;
    ENERGY_STORAGE_MODEL* model = estorage->get_energy_storage_model();
    if(model==NULL)
        return 0.0;

    if(meter_type=="STATE OF ENERGY IN PU")
        return model->get_energy_state_in_pu();

    if(meter_type=="ACTIVE POWER IN MW")
        return model->get_terminal_active_power_in_MW();

    if(meter_type=="ACTIVE POWER IN PU")
        return model->get_terminal_active_power_in_pu_based_on_mbase();

    if(meter_type=="REACTIVE POWER IN MVAR")
        return model->get_terminal_reactive_power_in_MVar();

    if(meter_type=="REACTIVE POWER IN PU")
        return model->get_terminal_reactive_power_in_pu_based_on_mbase();

    if(meter_type=="TERMINAL CURRENT IN KA")
        return model->get_terminal_current_in_kA();

    if(meter_type=="TERMINAL CURRENT IN PU")
        return model->get_terminal_current_in_pu_based_on_mbase();

    if(meter_type=="ENERGY STORAGE MODEL INTERNAL VARIABLE")
        return model->get_internal_variable_with_name(internal_variable_name);


    return 0.0;
}


