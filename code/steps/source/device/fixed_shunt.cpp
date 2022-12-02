#include "header/device/fixed_shunt.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

FIXED_SHUNT::FIXED_SHUNT(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

FIXED_SHUNT::~FIXED_SHUNT()
{
    ;
}

void FIXED_SHUNT::set_shunt_bus(unsigned int shunt_bus)
{
    ostringstream osstream;

    if(shunt_bus!=0)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        if(psdb.is_bus_exist(shunt_bus))
        {
            this->bus = shunt_bus;
            busptr = psdb.get_bus(shunt_bus);
        }
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
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bus = 0;
        return;
    }
}

void FIXED_SHUNT::set_identifier(string shunt_id)
{
    shunt_id = trim_string(shunt_id);
    add_string_to_str_int_map(shunt_id);
    this->identifier_index = get_index_of_string(shunt_id);
}

void FIXED_SHUNT::set_name(string name)
{
    name = trim_string(name);
    add_string_to_str_int_map(name);
    this->name_index = get_index_of_string(name);
}

void FIXED_SHUNT::set_status(bool status)
{
    this->status = status;
}

void FIXED_SHUNT::set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double> s)
{
    nominal_positive_sequence_impedance_shunt_in_MVA = s;
}

void FIXED_SHUNT::set_nominal_zero_sequence_impedance_shunt_in_MVA(complex<double> s)
{
    nominal_zero_squence_impedance_shunt_in_MVA = s;
}

unsigned int FIXED_SHUNT::get_shunt_bus() const
{
    return bus;
}

BUS* FIXED_SHUNT::get_bus_pointer() const
{
    return busptr;
}

string FIXED_SHUNT::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

string FIXED_SHUNT::get_name() const
{
    return get_string_of_index(name_index);
}

unsigned int FIXED_SHUNT::get_identifier_index() const
{
    return identifier_index;
}

unsigned int FIXED_SHUNT::get_name_index() const
{
    return name_index;
}


bool FIXED_SHUNT::get_status() const
{
    return status;
}

complex<double> FIXED_SHUNT::get_nominal_positive_sequence_impedance_shunt_in_MVA() const
{
    return nominal_positive_sequence_impedance_shunt_in_MVA;
}

complex<double> FIXED_SHUNT::get_nominal_impedance_shunt_in_pu() const
{
	if (get_nominal_positive_sequence_impedance_shunt_in_MVA() == 0.0)
		return complex<double>(0.0, INFINITE_THRESHOLD);

    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    return conj(sbase/get_nominal_positive_sequence_impedance_shunt_in_MVA());
}

complex<double> FIXED_SHUNT::get_nominal_admittance_shunt_in_pu() const
{
	if (get_nominal_positive_sequence_impedance_shunt_in_MVA() == 0.0)
		return 0.0;
    else
		return 1.0/get_nominal_impedance_shunt_in_pu();
}

complex<double> FIXED_SHUNT::get_nominal_zero_sequence_impedance_shunt_in_MVA() const
{
    return nominal_zero_squence_impedance_shunt_in_MVA;
}

complex<double> FIXED_SHUNT::get_nominal_zero_sequence_impedance_shunt_in_pu() const
{
	if (get_nominal_zero_sequence_impedance_shunt_in_MVA() == 0.0)
		return complex<double>(0.0, INFINITE_THRESHOLD);

    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    return conj(sbase/get_nominal_zero_sequence_impedance_shunt_in_MVA());
}

complex<double> FIXED_SHUNT::get_nominal_zero_sequence_admittance_shunt_in_pu() const
{
	if (get_nominal_zero_sequence_impedance_shunt_in_MVA() == 0.0)
		return 0.0;
    else
		return 1.0/get_nominal_zero_sequence_impedance_shunt_in_pu();
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
    busptr = NULL;
    set_identifier("");
    set_name("");
    set_status(false);
    set_nominal_positive_sequence_impedance_shunt_in_MVA(0.0);
    set_nominal_zero_sequence_impedance_shunt_in_MVA(0.0);

    sequence_parameter_import_flag = false;
}

bool FIXED_SHUNT::is_connected_to_bus(unsigned int target_bus) const
{
    if(target_bus==get_shunt_bus()) return true;
    else                            return false;
}

bool FIXED_SHUNT::is_in_area(unsigned int area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(get_shunt_bus());
    if(busptr!=NULL)
    {
        return busptr->is_in_area(area);
    }
    else
        return false;
}

bool FIXED_SHUNT::is_in_zone(unsigned int zone) const
{
    STEPS& toolkit = get_toolkit();
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
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
            <<"P+jQ[Z] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_positive_sequence_impedance_shunt_in_MVA()<<" MVA.";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void FIXED_SHUNT::save() const
{
    ;
}

void FIXED_SHUNT::set_model(MODEL* model)
{
    ostringstream osstream;
    osstream<<"FIXED_SHUNT::"<<__FUNCTION__<<"() has not been implemented yet. Input model name is:"<<(model==NULL?"":model->get_model_name());
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

MODEL* FIXED_SHUNT::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="FIXED SHUNT")
        return NULL;
    else
        return NULL;
}

FIXED_SHUNT& FIXED_SHUNT::operator=(const FIXED_SHUNT& fixed_shunt)
{
    if(this==(&fixed_shunt)) return *this;

    set_toolkit(fixed_shunt.get_toolkit());
    clear();

    set_shunt_bus(fixed_shunt.get_shunt_bus());
    set_identifier(fixed_shunt.get_identifier());
    set_name(fixed_shunt.get_name());
    set_status(fixed_shunt.get_status());
    set_nominal_positive_sequence_impedance_shunt_in_MVA(fixed_shunt.get_nominal_positive_sequence_impedance_shunt_in_MVA());
    set_nominal_zero_sequence_impedance_shunt_in_MVA(fixed_shunt.get_nominal_zero_sequence_impedance_shunt_in_MVA());

    return *this;
}


DEVICE_ID FIXED_SHUNT::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_FIXED_SHUNT);

    TERMINAL terminal;
    terminal.append_bus(get_shunt_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

complex<double> FIXED_SHUNT::get_actual_impedance_shunt_in_MVA() const
{
    ostringstream osstream;

    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_positive_sequence_impedance_shunt_in_MVA();

        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        BUS* busptr = psdb.get_bus(get_shunt_bus());

        if(busptr!=NULL)
        {
            double v = busptr->get_positive_sequence_voltage_in_pu();
            return S0*v*v;
        }
        else
        {
            osstream<<"Bus "<<get_shunt_bus()<<" is not found when getting the actual impedance shunt of "<<get_compound_device_name()<<". Nominal impedance shunt in MVA will be returned.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return S0;
        }
    }
    else
        return 0.0;
}

complex<double> FIXED_SHUNT::get_positive_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    complex<double> y = get_nominal_admittance_shunt_in_pu();
    complex<double> v = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_shunt_bus());
    complex<double> I = y*v;

    return I;
}

complex<double> FIXED_SHUNT::get_negative_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    complex<double> y = get_nominal_admittance_shunt_in_pu();
    complex<double> v = psdb.get_bus_negative_sequence_complex_voltage_in_pu(get_shunt_bus());
    complex<double> I = y*v;

    return I;
}

complex<double> FIXED_SHUNT::get_zero_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    complex<double> y = get_nominal_zero_sequence_admittance_shunt_in_pu();
    complex<double> v = psdb.get_bus_zero_sequence_complex_voltage_in_pu(get_shunt_bus());
    complex<double> I = y*v;

    return I;
}

complex<double> FIXED_SHUNT::get_positive_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_positive_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_shunt_bus()));
}

complex<double> FIXED_SHUNT::get_negative_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_negative_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_shunt_bus()));
}

complex<double> FIXED_SHUNT::get_zero_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_zero_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_shunt_bus()));
}

void FIXED_SHUNT::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool FIXED_SHUNT::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}

