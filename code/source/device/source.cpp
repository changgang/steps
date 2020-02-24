#include "header/device/source.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/STEPS.h"

#include <istream>
#include <iostream>

using namespace std;

SOURCE::SOURCE(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

SOURCE::~SOURCE()
{
    ;
}

void SOURCE::set_source_bus(unsigned int bus)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    if(bus==0)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up source bus."<<endl
          <<"0 will be set to indicate invalid source.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        source_bus = 0;
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_bus_exist(bus))
    {
        osstream<<"Bus "<<bus<<" does not exist for setting up power source."<<endl
          <<"0 will be set to indicate invalid power source.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        source_bus = 0;
        return;
    }
    source_bus = bus;

    busptr = psdb.get_bus(source_bus);
}

void SOURCE::set_identifier(string id)
{
    source_identifier = trim_string(id);
}

void SOURCE::set_status(bool status)
{
    this->status = status;
}

void SOURCE::set_mbase_in_MVA(double mbase)
{
    if(mbase>0.0)
    {
        this->mbase_MVA = mbase;
        one_over_mbase = 1.0/mbase;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Negative MBASE ("<<mbase<<" MVA) is not allowed for setting up power source '"<<get_identifier()<<"' at bus "<<get_source_bus()<<"."<<endl
          <<"Source MBASE will not be changed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    return;
}

void SOURCE::set_p_generation_in_MW(double p_MW)
{
    p_generation_MW = p_MW;
}

void SOURCE::set_q_generation_in_MVar(double q_MVar)
{
    q_generation_MVar = q_MVar;
}

void SOURCE::set_p_max_in_MW(double p_max)
{
    this->p_max_MW = p_max;
}

void SOURCE::set_p_min_in_MW(double p_min)
{
    this->p_min_MW = p_min;
}

void SOURCE::set_q_max_in_MVar(double q_max)
{
    this->q_max_MVar = q_max;
}

void SOURCE::set_q_min_in_MVar(double q_min)
{
    this->q_min_MVar = q_min;
}

void SOURCE::set_voltage_to_regulate_in_pu(double v_pu)
{
    if(v_pu<=0.0)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Non-positive voltage ("<<v_pu<<") is not supported for voltage to regulate when setting up power source '"<<get_identifier()<<"' at bus "<<get_source_bus()<<"."<<endl
          <<"1.0 p.u. will set automatically.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        v_pu = 1.0;
    }
    voltage_to_regulate_pu = v_pu;
}

void SOURCE::set_bus_to_regulate(unsigned int bus)
{
    if(bus==0 or bus==get_source_bus())
        bus_to_regulate = get_source_bus();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Currently generators are not supposed to regulate voltage at bus different from its terminal bus.\n"
                <<"Terminal bus "<<get_source_bus()<<" will be set to regulate. New bus "<<bus<<" is discarded.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        bus_to_regulate = get_source_bus();
    }
}

void SOURCE::set_source_impedance_in_pu(const complex<double>& z_pu)
{
    source_Z_pu = z_pu;
}


unsigned int SOURCE::get_source_bus() const
{
    return source_bus;
}

BUS* SOURCE::get_bus_pointer() const
{
    return busptr;
}

string SOURCE::get_identifier() const
{
    return source_identifier;
}

bool SOURCE::get_status() const
{
    return status;
}

double SOURCE::get_mbase_in_MVA() const
{
    return mbase_MVA;
}

double SOURCE::get_one_over_mbase_in_one_over_MVA() const
{
    return one_over_mbase;
}

double SOURCE::get_p_generation_in_MW() const
{
    return p_generation_MW;
}

double SOURCE::get_q_generation_in_MVar() const
{
    return q_generation_MVar;
}

complex<double> SOURCE::get_complex_generation_in_MVA() const
{
    return complex<double>(get_p_generation_in_MW(), get_q_generation_in_MVar());
}

double SOURCE::get_p_max_in_MW() const
{
    return p_max_MW;
}

double SOURCE::get_p_min_in_MW() const
{
    return p_min_MW;
}

double SOURCE::get_q_max_in_MVar() const
{
    return q_max_MVar;
}

double SOURCE::get_q_min_in_MVar() const
{
    return q_min_MVar;
}

double SOURCE::get_voltage_to_regulate_in_pu() const
{
    return voltage_to_regulate_pu;
}

unsigned int SOURCE::get_bus_to_regulate() const
{
    if(bus_to_regulate!=0)
        return bus_to_regulate;
    else
        return get_source_bus();
}

complex<double> SOURCE::get_source_impedance_in_pu() const
{
    return source_Z_pu;
}

bool SOURCE::is_valid() const
{
    if(get_source_bus()!=0)
        return true;
    else
        return false;
}

void SOURCE::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    string error_leading_string = "Error detected when checking "+get_device_name()+": ";

    unsigned int bus = get_source_bus();
    double qmax = get_q_max_in_MVar();
    double qmin = get_q_min_in_MVar();
    if(fabs(qmax-qmin)<DOUBLE_EPSILON)
    {
        BUS_TYPE btype = psdb.get_bus_type(bus);
        if(btype==SLACK_TYPE)
        {
            osstream<<error_leading_string<<"Qmax and Qmin are identical for SLACK source. NaN may be detected when distributing reactive power to generators.\n"
                    <<"Qmax = "<<qmax<<"MVar. Qmin = "<<qmin<<"MVar. ";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void SOURCE::clear()
{
    source_bus = 0;
    busptr = NULL;

    set_identifier("");
    set_status(false);

    set_mbase_in_MVA(100.0);

    set_p_generation_in_MW(0.0);
    set_q_generation_in_MVar(0.0);
    set_p_max_in_MW(0.0);
    set_p_min_in_MW(0.0);
    set_q_max_in_MVar(0.0);
    set_q_min_in_MVar(0.0);
    set_voltage_to_regulate_in_pu(1.0);
    set_bus_to_regulate(0);
    set_source_impedance_in_pu(0.0);
}

bool SOURCE::is_connected_to_bus(unsigned int bus) const
{
    if(get_source_bus()==bus) return true;
    else                      return false;
}

bool SOURCE::is_in_area(unsigned int area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(get_source_bus());
    if(busptr!=NULL)
    {
        return busptr->is_in_area(area);
    }
    else
        return false;
}

bool SOURCE::is_in_zone(unsigned int zone) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(get_source_bus());
    if(busptr!=NULL)
    {
        return busptr->is_in_zone(zone);
    }
    else
        return false;
}

