#include "header/device/fixed_shunt.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

FIXED_SHUNT::FIXED_SHUNT()
{
    clear();
}

FIXED_SHUNT::~FIXED_SHUNT()
{
    ;
}

void FIXED_SHUNT::set_shunt_bus(size_t shunt_bus)
{
    ostringstream osstream;

    if(shunt_bus!=0)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        if(psdb.is_bus_exist(shunt_bus))
            this->bus = shunt_bus;
        else
        {
            osstream<<"Bus "<<shunt_bus<<" does not exist in the power system database '"<<psdb.get_system_name()<<"' for setting up fixed shunt."<<endl
                    <<"0 will be set to indicate invalid fixed shunt.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            this->bus = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up fixed shunt bus."<<endl
                <<"0 will be set to indicate invalid fixed shunt.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bus = 0;
        return;
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
	if (get_nominal_impedance_shunt_in_MVA() == 0.0)
		return complex<double>(0.0, INFINITE_THRESHOLD);

    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double mvabase = psdb.get_system_base_power_in_MVA();
    return conj(mvabase/get_nominal_impedance_shunt_in_MVA());
}

complex<double> FIXED_SHUNT::get_nominal_admittance_shunt_in_pu() const
{
	if (get_nominal_impedance_shunt_in_MVA() == 0.0)
		return 0.0;
    else
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
    else                            return false;
}

bool FIXED_SHUNT::is_in_area(size_t area) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(get_shunt_bus());
    if(busptr!=NULL)
    {
        return busptr->is_in_area(area);
    }
    else
        return false;
}

bool FIXED_SHUNT::is_in_zone(size_t zone) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(get_shunt_bus());
    if(busptr!=NULL)
    {
        return busptr->is_in_zone(zone);
    }
    else
        return false;
}

void FIXED_SHUNT::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
            <<"P+jQ[Z] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_impedance_shunt_in_MVA()<<" MVA.";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void FIXED_SHUNT::save() const
{
    ;
}

void FIXED_SHUNT::set_model(const MODEL* model)
{
    ostringstream osstream;
    osstream<<"TRANSFORMER::"<<__FUNCTION__<<"() has not been implemented yet. Input model name is:"<<(model==NULL?"":model->get_model_name());
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

FIXED_SHUNT& FIXED_SHUNT::operator=(const FIXED_SHUNT& fixed_shunt)
{
    if(this==(&fixed_shunt)) return *this;

    clear();

    set_toolkit(fixed_shunt.get_toolkit(__PRETTY_FUNCTION__));

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
    ostringstream osstream;

    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_impedance_shunt_in_MVA();

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        BUS* busptr = psdb.get_bus(get_shunt_bus());

        if(busptr!=NULL)
        {
            double v = busptr->get_voltage_in_pu();
            return S0*v*v;
        }
        else
        {
            osstream<<"Bus "<<get_shunt_bus()<<" is not found when getting the actual impedance shunt of "<<get_device_name()<<". Nominal impedance shunt in MVA will be returned.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return S0;
        }
    }
    else
        return 0.0;
}



