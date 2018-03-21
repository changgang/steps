#include "header/device/fixed_shunt.h"
#include "header/basic/utility.h"
#include <iostream>

using namespace std;

FIXED_SHUNT::FIXED_SHUNT(POWER_SYSTEM_DATABASE* psdb)
{
    ostringstream sstream;
    if(psdb==NULL)
    {
        sstream<<"Error. FIXED_SHUNT object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(sstream);
    }
    set_power_system_database(psdb);
    clear();
}

FIXED_SHUNT::~FIXED_SHUNT()
{
    ;
}

void FIXED_SHUNT::set_shunt_bus(size_t shunt_bus)
{
    ostringstream sstream;

    if(shunt_bus==0)
    {
        sstream<<"Warning. Zero bus number (0) is not allowed for setting up fixed shunt bus."<<endl
          <<"0 will be set to indicate invalid fixed shunt.";
        show_information_with_leading_time_stamp(sstream);
        this->bus = 0;
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
        this->bus = shunt_bus;
    else
    {
        if(psdb->is_bus_exist(shunt_bus))
            this->bus = shunt_bus;
        else
        {
            sstream<<"Bus "<<shunt_bus<<" does not exist in the power system database '"<<psdb->get_system_name()<<"' for setting up fixed shunt."<<endl
              <<"0 will be set to indicate invalid fixed shunt.";
            show_information_with_leading_time_stamp(sstream);
            this->bus = 0;
        }
    }
}

void FIXED_SHUNT::set_identifier(string shunt_id)
{
    this->identifier = shunt_id;
}

void FIXED_SHUNT::set_status(bool status)
{
    this->status = status;
}

void FIXED_SHUNT::set_nominal_impedance_shunt_in_MVA(complex<double> s)
{
    nominal_impedance_shunt_in_MVA = s;
}

size_t FIXED_SHUNT::get_shunt_bus() const
{
    return bus;
}

string FIXED_SHUNT::get_identifier() const
{
    return identifier;
}

bool FIXED_SHUNT::get_status() const
{
    return status;
}

complex<double> FIXED_SHUNT::get_nominal_impedance_shunt_in_MVA() const
{
    return nominal_impedance_shunt_in_MVA;
}

complex<double> FIXED_SHUNT::get_nominal_impedance_shunt_in_pu() const
{
    ostringstream sstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;
    if(psdb==NULL)
    {
        sstream<<get_device_name()<<" is not assigned to any power system database. 100MVA will be used to get pu shunt impedance.";
        show_information_with_leading_time_stamp(sstream);
    }
    else
        mvabase = psdb->get_system_base_power_in_MVA();

    return conj(mvabase/get_nominal_impedance_shunt_in_MVA());
}

complex<double> FIXED_SHUNT::get_nominal_admittance_shunt_in_pu() const
{
    return 1.0/get_nominal_impedance_shunt_in_pu();
}

bool FIXED_SHUNT::is_valid() const
{
    if(get_shunt_bus()!=0)
        return true;
    else
        return false;
}

void FIXED_SHUNT::check()
{
    ;
}

void FIXED_SHUNT::clear()
{
    bus = 0;
    set_identifier("");
    set_status(false);
    set_nominal_impedance_shunt_in_MVA(0.0);
}

bool FIXED_SHUNT::is_connected_to_bus(size_t target_bus) const
{
    if(target_bus==get_shunt_bus()) return true;
    else return false;
}

bool FIXED_SHUNT::is_in_area(size_t area) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr = psdb->get_bus(get_shunt_bus());
        if(busptr!=NULL)
        {
            return busptr->is_in_area(area);
        }
        else
            return false;
    }
    else
        return false;
}

bool FIXED_SHUNT::is_in_zone(size_t zone) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr = psdb->get_bus(get_shunt_bus());
        if(busptr!=NULL)
        {
            return busptr->is_in_zone(zone);
        }
        else
            return false;
    }
    else
        return false;
}

void FIXED_SHUNT::report() const
{
    ostringstream sstream;
    sstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"P+jQ[Z] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_impedance_shunt_in_MVA()<<" MVA.";
    show_information_with_leading_time_stamp(sstream);
}

void FIXED_SHUNT::save() const
{
    ;
}

void FIXED_SHUNT::set_model(const MODEL* model)
{
    ;
}

FIXED_SHUNT& FIXED_SHUNT::operator=(const FIXED_SHUNT& fixed_shunt)
{
    if(this==(&fixed_shunt)) return *this;

    clear();

    set_power_system_database(fixed_shunt.get_power_system_database());
    set_shunt_bus(fixed_shunt.get_shunt_bus());
    set_identifier(fixed_shunt.get_identifier());
    set_status(fixed_shunt.get_status());
    set_nominal_impedance_shunt_in_MVA(fixed_shunt.get_nominal_impedance_shunt_in_MVA());

    return *this;
}


DEVICE_ID FIXED_SHUNT::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");

    TERMINAL terminal;
    terminal.append_bus(get_shunt_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string FIXED_SHUNT::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

complex<double> FIXED_SHUNT::get_actual_impedance_shunt_in_MVA() const
{
    ostringstream sstream;

    if(get_status() == false)
        return 0.0;

    complex<double> S0 = get_nominal_impedance_shunt_in_MVA();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb==NULL)
    {
        sstream<<get_device_name()<<" is not assigned to any power system database. Nominal impedance shunt in MVA will be returned.";
        show_information_with_leading_time_stamp(sstream);
        return S0;
    }
    else
    {
        BUS* busptr = psdb->get_bus(get_shunt_bus());

        if(busptr==NULL)
        {
            sstream<<"Bus "<<get_shunt_bus()<<" is not found when getting the actual impedance shunt of "<<get_device_name()<<". Nominal impedance shunt in MVA will be returned.";
            show_information_with_leading_time_stamp(sstream);
            return S0;
        }
        double v = busptr->get_voltage_in_pu();
        return S0*v*v;
    }
}



