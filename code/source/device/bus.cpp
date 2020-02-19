#include "header/device/bus.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

BUS::BUS(STEPS& toolkit) : DEVICE(toolkit), bus_frequency_model(toolkit)
{
    clear();

    bus_frequency_model.set_bus_pointer(this);
}

BUS::BUS(const BUS& bus) : DEVICE(bus.get_toolkit()), bus_frequency_model(bus.get_toolkit())
{
    copy_from_const_bus(bus);
    bus_frequency_model.set_bus_pointer(this);
}

BUS::~BUS()
{
    ;
}

void BUS::set_bus_number(unsigned int number)
{
    if(number!=0)
        bus_number = number;
    else
    {
        ostringstream osstream;
        osstream<<"0 is not allowed for setting up a bus number. 0 will be set to indicate invalid bus.";
        bus_number = 0;
    }
}

void BUS::set_bus_name(string name)
{
    bus_name = trim_string(name);
}

void BUS::set_base_voltage_in_kV(double voltage)
{
    if(voltage>0.0)
        base_voltage_in_kV = voltage;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive kV ("<<voltage<<" kV) is not allowed for setting up bus base voltage."<<endl
          <<"0 will be set to indicate invalid bus.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        base_voltage_in_kV = 0.0;
    }
}

void BUS::set_bus_type(BUS_TYPE type)
{
    bus_type = type;
}

void BUS::set_area_number(unsigned int number)
{
    area_number = number;
}

void BUS::set_zone_number(unsigned int number)
{
    zone_number = number;
}

void BUS::set_owner_number(unsigned int number)
{
    owner_number = number;
}

void BUS::set_positive_sequence_voltage_in_pu(double voltage)
{
    positive_sequence_voltage_in_pu = voltage;
    return;
    if(voltage>=0.0)
        positive_sequence_voltage_in_pu = voltage;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Warning. Non-positive voltage ("<<voltage<<" pu) is not allowed for setting up voltage for bus "
               <<get_bus_number()<<" ("<<get_bus_name()<<")."<<endl
               <<"0.0 will be set automatically. [current dynamic simulation time is: "<<toolkit.get_dynamic_simulation_time_in_s()<<"s]";
        toolkit.show_information_with_leading_time_stamp(osstream);
        positive_sequence_voltage_in_pu = 0.0;
        if(isnan(voltage))
        {
            osstream<<"STEPS will exit abnormally.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            exit(1);
        }
    }
}

void BUS::set_positive_sequence_voltage_in_kV(double voltage)
{
    if(get_base_voltage_in_kV()!=0.0)
        set_positive_sequence_voltage_in_pu(voltage/get_base_voltage_in_kV());
    else
    {
        ostringstream osstream;
        osstream<<"Invalid to set bus positive sequence voltage ("<<voltage<<" kV) with zero base voltage for bus "
               <<get_bus_number()<<" ("<<get_bus_name()<<")."<<endl
               <<"0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_positive_sequence_voltage_in_pu(0.0);
    }
}

void BUS::set_positive_sequence_angle_in_rad(double angle)
{
    positive_sequence_angle_in_rad = angle;
    positive_sequence_Euler_complex_number = complex<double>(steps_cos(positive_sequence_angle_in_rad), steps_sin(positive_sequence_angle_in_rad));
}

void BUS::set_positive_sequence_angle_in_deg(double angle)
{
    angle = deg2rad(angle);
    set_positive_sequence_angle_in_rad(angle);
}

void BUS::set_negative_sequence_voltage_in_pu(double voltage)
{
    negative_sequence_voltage_in_pu = voltage;
}

void BUS::set_negative_sequence_voltage_in_kV(double voltage)
{
    if(get_base_voltage_in_kV()!=0.0)
        set_negative_sequence_voltage_in_pu(voltage/get_base_voltage_in_kV());
    else
    {
        ostringstream osstream;
        osstream<<"Invalid to set bus negative sequence voltage ("<<voltage<<" kV) with zero base voltage for bus "
               <<get_bus_number()<<" ("<<get_bus_name()<<")."<<endl
               <<"0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_negative_sequence_voltage_in_pu(0.0);
    }
}

void BUS::set_negative_sequence_angle_in_rad(double angle)
{
    negative_sequence_angle_in_rad = angle;
    negative_sequence_Euler_complex_number = complex<double>(steps_cos(negative_sequence_angle_in_rad), steps_sin(negative_sequence_angle_in_rad));
}

void BUS::set_negative_sequence_angle_in_deg(double angle)
{
    angle = deg2rad(angle);
    set_negative_sequence_angle_in_rad(angle);
}

void BUS::set_zero_sequence_voltage_in_pu(double voltage)
{
    zero_sequence_voltage_in_pu = voltage;
}

void BUS::set_zero_sequence_voltage_in_kV(double voltage)
{
    if(get_base_voltage_in_kV()!=0.0)
        set_zero_sequence_voltage_in_pu(voltage/get_base_voltage_in_kV());
    else
    {
        ostringstream osstream;
        osstream<<"Invalid to set bus zero sequence voltage ("<<voltage<<" kV) with zero base voltage for bus "
               <<get_bus_number()<<" ("<<get_bus_name()<<")."<<endl
               <<"0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_zero_sequence_voltage_in_pu(0.0);
    }
}

void BUS::set_zero_sequence_angle_in_rad(double angle)
{
    zero_sequence_angle_in_rad = angle;
    zero_sequence_Euler_complex_number = complex<double>(steps_cos(zero_sequence_angle_in_rad), steps_sin(zero_sequence_angle_in_rad));
}

void BUS::set_zero_sequence_angle_in_deg(double angle)
{
    angle = deg2rad(angle);
    set_zero_sequence_angle_in_rad(angle);
}

void BUS::set_normal_voltage_upper_limit_in_pu(double voltage)
{
    if(voltage==0.0)
        voltage = 1.1;

    normal_voltage_upper_limit_in_pu = voltage;
}

void BUS::set_normal_voltage_lower_limit_in_pu(double voltage)
{
    if(voltage==0.0)
        voltage = 0.9;

    normal_voltage_lower_limit_in_pu = voltage;
}

void BUS::set_emergency_voltage_upper_limit_in_pu(double voltage)
{
    if(voltage==0.0)
        voltage = 1.1;

    emergency_voltage_upper_limit_in_pu = voltage;
}

void BUS::set_emergency_voltage_lower_limit_in_pu(double voltage)
{
    if(voltage==0.0)
        voltage = 0.9;

    emergency_voltage_lower_limit_in_pu = voltage;
}

void BUS::set_voltage_upper_limit_in_pu(double voltage)
{
    set_normal_voltage_upper_limit_in_pu(voltage);
    set_emergency_voltage_upper_limit_in_pu(voltage);
}

void BUS::set_voltage_lower_limit_in_pu(double voltage)
{
    set_normal_voltage_lower_limit_in_pu(voltage);
    set_emergency_voltage_lower_limit_in_pu(voltage);
}

void BUS::set_base_frequency_in_Hz(double fn)
{
    if(fn<0.0)
        fn = -fn;
    if(fn==0.0)
        fn = 50.0;
    fn_Hz = fn;
    tn_s = 1.0/fn_Hz;
}


void BUS::set_voltage_to_regulate_in_pu(double voltage)
{
    if(voltage == 0.0)
    {
        voltage_to_regulate_in_pu = 0.0;
        return;
    }

    if(get_voltage_to_regulate_in_pu()==0.0)
        voltage_to_regulate_in_pu = voltage;
    else
    {
        if(get_voltage_to_regulate_in_pu()!=voltage)
        {
            ostringstream osstream;
            osstream<<"Warning. New voltage to regulate ("<<voltage<<" pu) is not allowed to set for bus "<<get_bus_number()
                    <<" with existing voltage to regulate: "<<get_voltage_to_regulate_in_pu()<<" pu."<<endl
                    <<"Voltage to regulate will not be changed.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

unsigned int BUS::get_bus_number() const
{
    return bus_number;
}

string BUS::get_bus_name() const
{
    return bus_name;
}

double BUS::get_base_voltage_in_kV() const
{
    return base_voltage_in_kV;
}

BUS_TYPE BUS::get_bus_type() const
{
    return bus_type;
}

unsigned int BUS::get_area_number() const
{
    return area_number;
}

unsigned int BUS::get_zone_number() const
{
    return zone_number;
}

unsigned int BUS::get_owner_number() const
{
    return owner_number;
}

double BUS::get_positive_sequence_voltage_in_pu() const
{
    return positive_sequence_voltage_in_pu;
}

double BUS::get_positive_sequence_voltage_in_kV() const
{
    return get_base_voltage_in_kV()*get_positive_sequence_voltage_in_pu();
}

double BUS::get_positive_sequence_angle_in_rad() const
{
    return positive_sequence_angle_in_rad;
}

double BUS::get_positive_sequence_angle_in_deg() const
{
    return rad2deg(get_positive_sequence_angle_in_rad());
}


double BUS::get_negative_sequence_voltage_in_pu() const
{
    return negative_sequence_voltage_in_pu;
}

double BUS::get_negative_sequence_voltage_in_kV() const
{
    return get_base_voltage_in_kV()*get_negative_sequence_voltage_in_pu();
}

double BUS::get_negative_sequence_angle_in_rad() const
{
    return negative_sequence_angle_in_rad;
}

double BUS::get_negative_sequence_angle_in_deg() const
{
    return rad2deg(get_negative_sequence_angle_in_rad());
}


double BUS::get_zero_sequence_voltage_in_pu() const
{
    return zero_sequence_voltage_in_pu;
}

double BUS::get_zero_sequence_voltage_in_kV() const
{
    return get_base_voltage_in_kV()*get_zero_sequence_voltage_in_pu();
}

double BUS::get_zero_sequence_angle_in_rad() const
{
    return zero_sequence_angle_in_rad;
}

double BUS::get_zero_sequence_angle_in_deg() const
{
    return rad2deg(get_zero_sequence_angle_in_rad());
}

double BUS::get_normal_voltage_upper_limit_in_pu() const
{
    return normal_voltage_upper_limit_in_pu;
}

double BUS::get_normal_voltage_lower_limit_in_pu() const
{
    return normal_voltage_lower_limit_in_pu;
}

double BUS::get_emergency_voltage_upper_limit_in_pu() const
{
    return emergency_voltage_upper_limit_in_pu;
}

double BUS::get_emergency_voltage_lower_limit_in_pu() const
{
    return emergency_voltage_lower_limit_in_pu;
}

double BUS::get_voltage_upper_limit_in_pu() const
{
    return get_normal_voltage_upper_limit_in_pu();
}

double BUS::get_voltage_lower_limit_in_pu() const
{
    return get_normal_voltage_lower_limit_in_pu();
}

double BUS::get_base_frequency_in_Hz() const
{
    return fn_Hz;
}

double BUS::get_base_period_in_s() const
{
    return tn_s;
}

double BUS::get_voltage_to_regulate_in_pu() const
{
    return voltage_to_regulate_in_pu;
}

complex<double> BUS::get_positive_sequence_complex_voltage_in_pu() const
{
    return positive_sequence_voltage_in_pu*positive_sequence_Euler_complex_number;
}

complex<double> BUS::get_positive_sequence_complex_voltage_in_kV() const
{
    return base_voltage_in_kV*positive_sequence_voltage_in_pu*positive_sequence_Euler_complex_number;
}

complex<double> BUS::get_negative_sequence_complex_voltage_in_pu() const
{
    return negative_sequence_voltage_in_pu*negative_sequence_Euler_complex_number;
}

complex<double> BUS::get_negative_sequence_complex_voltage_in_kV() const
{
    return base_voltage_in_kV*negative_sequence_voltage_in_pu*negative_sequence_Euler_complex_number;
}

complex<double> BUS::get_zero_sequence_complex_voltage_in_pu() const
{
    return zero_sequence_voltage_in_pu*zero_sequence_Euler_complex_number;
}

complex<double> BUS::get_zero_sequence_complex_voltage_in_kV() const
{
    return base_voltage_in_kV*zero_sequence_voltage_in_pu*zero_sequence_Euler_complex_number;
}


void BUS::set_equivalent_bus_number(unsigned int number)
{
    equivalent_bus_number = number;
}

unsigned int BUS::get_equivalent_bus_number() const
{
    return equivalent_bus_number;
}

bool BUS::is_bus_overshadowed() const
{
    if(get_equivalent_bus_number()!=0 and get_equivalent_bus_number()!=get_bus_number())
        return true;
    else
        return false;
}

bool BUS::is_valid() const
{
    if(get_bus_number()!=0 and get_base_voltage_in_kV()>0.0)
        return true;
    else
        return false;
}

void BUS::check()
{
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    if(get_bus_number()==0)
    {
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Invalid bus number 0 is detected.");
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(buffer);
    }
}

void BUS::clear()
{
    bus_number = 0;
    set_bus_name("");
    base_voltage_in_kV = 0.0;
    set_base_frequency_in_Hz(50.0);
    set_bus_type(OUT_OF_SERVICE);
    set_area_number(0);
    set_zone_number(0);
    set_owner_number(0);
    set_positive_sequence_voltage_in_pu(1.0);
    set_positive_sequence_angle_in_rad(0.0);
    set_negative_sequence_voltage_in_pu(0.0);
    set_negative_sequence_angle_in_rad(0.0);
    set_zero_sequence_voltage_in_pu(0.0);
    set_zero_sequence_angle_in_rad(0.0);
    set_voltage_upper_limit_in_pu(1.1);
    set_voltage_lower_limit_in_pu(0.9);
    set_voltage_to_regulate_in_pu(0.0);

    set_equivalent_bus_number(0);

    fault.clear();
}

bool BUS::is_connected_to_bus(unsigned int bus) const
{
    if(get_bus_number()==bus) return true;
    else                      return false;
}

bool BUS::is_in_area(unsigned int area) const
{
    return get_area_number()==area;
}

bool BUS::is_in_zone(unsigned int zone) const
{
    return get_zone_number()==zone;
}

bool BUS::operator<(const BUS& bus) const
{
    if(this->get_bus_number()<bus.get_bus_number()) return true;
    else                                            return false;
}

DEVICE_ID BUS::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("BUS");

    TERMINAL terminal;
    terminal.append_bus(get_bus_number());
    did.set_device_terminal(terminal);

    return did;
}

void BUS::set_fault(const FAULT& bus_fault)
{
    ostringstream osstream;
    if(not bus_fault.is_faulted())
    {
        osstream<<"Warning. Non-faulted fault is given for bus "<<get_bus_number()<<"."<<endl
               <<"Fault will not be set.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    else
    {
        string fault_type = bus_fault.get_fault_type_string();
        complex<double> y = bus_fault.get_fault_shunt_in_pu();
        if(not this->fault.is_faulted())
            osstream<<fault_type<<" is set for bus "<<get_bus_number()<<" with fault shunt "<<y<<" pu";
        else
            osstream<<"Fault is already set for bus "<<get_bus_number()<<" with fault shunt "<<this->fault.get_fault_shunt_in_pu()<<" pu."<<endl
                   <<"New "<<fault_type<<" is set for bus "<<get_bus_number()<<" with fault shunt "<<y<<" pu";

        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->fault = bus_fault;
    }
}

FAULT BUS::get_fault() const
{
    return fault;
}

void BUS::clear_fault()
{
    ostringstream osstream;
    if(is_faulted())
    {
        osstream<<"Fault at bus "<<get_bus_number()<<" is cleared.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        fault.clear();
    }
    else
    {
        osstream<<"No fault was set for bus "<<get_bus_number()<<"."<<endl
               <<"No fault will be cleared.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

bool BUS::is_faulted() const
{
    return fault.is_faulted();
}


BUS& BUS::operator=(const BUS& bus)
{
    if(this==&bus) return *this; // to avoid self assignment

    copy_from_const_bus(bus);

    bus_frequency_model.set_bus_pointer(this);

    return *this;
}

void BUS::copy_from_const_bus(const BUS& bus)
{
    set_toolkit(bus.get_toolkit());
    bus_frequency_model.set_toolkit(bus.get_toolkit());

    clear();

    set_bus_number(bus.get_bus_number());
    set_bus_name(bus.get_bus_name());
    set_base_voltage_in_kV(bus.get_base_voltage_in_kV());
    set_base_frequency_in_Hz(bus.get_base_frequency_in_Hz());
    set_bus_type(bus.get_bus_type());
    set_area_number(bus.get_area_number());
    set_zone_number(bus.get_zone_number());
    set_owner_number(bus.get_owner_number());
    set_positive_sequence_voltage_in_pu(bus.get_positive_sequence_voltage_in_pu());
    set_positive_sequence_angle_in_rad(bus.get_positive_sequence_angle_in_rad());
    set_voltage_to_regulate_in_pu(bus.get_voltage_to_regulate_in_pu());

    if(bus.is_faulted())
    {
        set_fault(bus.get_fault());
    }
}

void BUS::report() const
{
    ostringstream osstream;
    osstream<<"Bus "
      <<setw(6)<<get_bus_number()<<" "
      <<"'"<<get_bus_name()<<"' "
      <<setprecision(3)<<get_base_voltage_in_kV()<<" kV, "<<get_base_frequency_in_Hz()<<" Hz: "
      <<(get_bus_type()==SLACK_TYPE?"Slack":(get_bus_type()==PQ_TYPE?"PQ":(get_bus_type()==OUT_OF_SERVICE?"Out of service":"PV")))<<", "
      <<setprecision(6)<<fixed<<get_positive_sequence_voltage_in_pu()<<" pu, "
      <<setprecision(6)<<fixed<<get_positive_sequence_angle_in_deg()<<" deg.";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void BUS::save() const
{
    ;
}
BUS_FREQUENCY_MODEL* BUS::get_bus_frequency_model()
{
    return &bus_frequency_model;
}

void BUS::set_frequency_deviation_in_pu(double f)
{
    return bus_frequency_model.set_frequency_deviation_in_pu(f);
}

double BUS::get_frequency_deviation_in_pu() const
{
    return bus_frequency_model.get_frequency_deviation_in_pu();
}

double BUS::get_frequency_deviation_in_Hz() const
{
    return bus_frequency_model.get_frequency_deviation_in_Hz();
}

double BUS::get_frequency_in_pu() const
{
    return get_frequency_deviation_in_pu()+1.0;
}

double BUS::get_frequency_in_Hz() const
{
    return get_frequency_deviation_in_Hz()+get_base_frequency_in_Hz();
}

void BUS::set_model(const MODEL* model)
{
    ostringstream osstream;
    osstream<<"BUS::"<<__FUNCTION__<<"() has not been implemented yet. Input model name is:"<<(model==NULL?"":model->get_model_name());
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

MODEL* BUS::get_model_of_type(string model_type)
{
    return nullptr;
}
