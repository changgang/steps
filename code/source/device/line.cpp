#include "header/device/line.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <iostream>

using namespace std;
LINE::LINE(POWER_SYSTEM_DATABASE* psdb)
{
    ostringstream osstream;
    if(psdb==NULL)
    {
        osstream<<"Error. LINE object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(osstream);
    }
    set_power_system_database(psdb);
    clear();
}

LINE::~LINE()
{
    ;
}

void LINE::set_sending_side_bus(size_t bus)
{
    ostringstream osstream;

    if(bus==0)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up sending side bus of line."<<endl
          <<"0 will be set to indicate invalid transmission line.";
        show_information_with_leading_time_stamp(osstream);
        sending_side_bus = 0;
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb==NULL)
        sending_side_bus = bus;
    else
    {
        if(not psdb->is_bus_exist(bus))
        {
            osstream<<"Warning. Bus "<<bus<<" does not exist for setting up sending side bus of line."<<endl
              <<"0 will be set to indicate invalid transmission line.";
            show_information_with_leading_time_stamp(osstream);
            sending_side_bus = 0;
            return;
        }
        sending_side_bus = bus;
    }
}

void LINE::set_receiving_side_bus(size_t bus)
{
    ostringstream osstream;

    if(bus==0)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up receiving side bus of line."<<endl
          <<"0 will be set to indicate invalid transmission line.";
        show_information_with_leading_time_stamp(osstream);
        receiving_side_bus = 0;
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb==NULL)
        receiving_side_bus = bus;
    else
    {
        if(not psdb->is_bus_exist(bus))
        {
            osstream<<"Warning. Bus "<<bus<<" does not exist for setting up receiving side bus of line."<<endl
              <<"0 will be set to indicate invalid transmission line.";
            show_information_with_leading_time_stamp(osstream);
            receiving_side_bus = 0;
            return;
        }
        receiving_side_bus = bus;
    }
}

void LINE::set_identifier(string line_id)
{
    this->identifier = line_id;
}

void LINE::set_sending_side_breaker_status(bool status)
{
    this->sending_side_breaker_status = status;
}

void LINE::set_receiving_side_breaker_status(bool status)
{
    this->receiving_side_breaker_status = status;
}

void LINE::set_line_positive_sequence_z_in_pu(complex<double> z)
{
    line_z_12 = z;
}

void LINE::set_line_positive_sequence_y_in_pu(complex<double> y)
{
    line_y_12 = y;
}

void LINE::set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double> y)
{
    shunt_y_12_sending_side = y;
}

void LINE::set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double> y)
{
    shunt_y_12_receiving_side = y;
}

void LINE::set_line_zero_sequence_z_in_pu(complex<double> z)
{
    line_z_0 = z;
}

void LINE::set_line_zero_sequence_y_in_pu(complex<double> y)
{
    line_y_0 = y;
}

void LINE::set_shunt_zero_sequence_y_at_sending_side_in_pu(complex<double> y)
{
    shunt_y_0_sending_side = y;
}

void LINE::set_shunt_zero_sequence_y_at_receiving_side_in_pu(complex<double> y)
{
    shunt_y_0_receiving_side = y;
}

void LINE::set_rating(RATING line_rating)
{
    this->rating = line_rating;
}

void LINE::set_meter_end_bus(size_t meter_bus)
{
    if(meter_bus == get_receiving_side_bus()) meter_end_bus = get_receiving_side_bus();
    else                                 meter_end_bus = get_sending_side_bus();
}

void LINE::set_length(double line_length)
{
    if(line_length>=0.0) this->length = line_length;
}

size_t LINE::get_sending_side_bus() const
{
    return sending_side_bus;
}

size_t LINE::get_receiving_side_bus() const
{
    return receiving_side_bus;
}

string LINE::get_identifier() const
{
    return identifier;
}

bool LINE::get_sending_side_breaker_status() const
{
    return sending_side_breaker_status;
}

bool LINE::get_receiving_side_breaker_status() const
{
    return receiving_side_breaker_status;
}

complex<double> LINE::get_line_positive_sequence_z_in_pu() const
{
    return line_z_12;
}

complex<double> LINE::get_line_zero_sequence_z_in_pu() const
{
    return line_z_0;
}

complex<double> LINE::get_line_positive_sequence_y_in_pu() const
{
    return line_y_12;
}

complex<double> LINE::get_line_zero_sequence_y_in_pu() const
{
    return line_y_0;
}

complex<double> LINE::get_shunt_positive_sequence_y_at_sending_side_in_pu() const
{
    return shunt_y_12_sending_side;
}

complex<double> LINE::get_shunt_zero_sequence_y_at_sending_side_in_pu() const
{
    return shunt_y_0_sending_side;
}

complex<double> LINE::get_shunt_positive_sequence_y_at_receiving_side_in_pu() const
{
    return shunt_y_12_receiving_side;
}

complex<double> LINE::get_shunt_zero_sequence_y_at_receiving_side_in_pu() const
{
    return shunt_y_0_receiving_side;
}

RATING LINE::get_rating() const
{
    return rating;
}

size_t LINE::get_meter_end_bus() const
{
    return meter_end_bus;
}

double LINE::get_length() const
{
    return length;
}


void LINE::set_fault(size_t to_bus, double location, FAULT& fault)
{
    ostringstream osstream;
    if(not is_connected_to_bus(to_bus))
        return;

    if(location<0.0 or location>1.0)
    {
        osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<get_device_name()<<"."<<endl
               <<"No fault will be set.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(not fault.is_faulted())
    {
        osstream<<"Warning. Given fault is not faulted for "<<get_device_name()<<"."<<endl
               <<"No fault will be set.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    string fault_type = fault.get_fault_type_string();

    complex<double> y = fault.get_fault_shunt_in_pu();
    osstream<<fault_type<<" fault is set for "<<get_device_name()<<"."<<endl
           <<"Fault shunt is "<<y<<" pu at location "<<location<<" to bus "<<to_bus;

    if(to_bus == get_receiving_side_bus())
        location = 1.0-location;

    map<double,FAULT>::iterator iter;
    for(iter=faults.begin(); iter!=faults.end(); iter++)
    {
        if(fabs(iter->first-location)<FLOAT_EPSILON)
            break;
    }
    if(iter!=faults.end())
    {
        osstream<<"Fault at location "<<location<<" (to sending side bus) already exist for "<<get_device_name()<<"."<<endl
              <<"Setting fault at this location will remove the previous one";
        iter->second = fault;
    }
    else
    {
        faults.insert(pair<double,FAULT>(location, fault));
    }
    show_information_with_leading_time_stamp(osstream);
}

double LINE::get_fault_location_of_fault(size_t index) const
{
    ostringstream osstream;
    if(index>=get_fault_count())
    {
        osstream<<"Invalid index ("<<index<<") was given to get fault location. [0, "<<faults.size()<<") is allowed.";
        show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
    map<double,FAULT>::const_iterator iter=faults.begin();

    for(size_t i=0; i!=index; ++i)
        iter++;

    return iter->first;
}

FAULT LINE::get_fault_at_location(size_t to_bus, double location) const
{
    FAULT nofault;
    if(not is_connected_to_bus(to_bus))
        return nofault;

    if(location<0.0 or location>1.0)
        return nofault;

    if(to_bus == get_receiving_side_bus())
        location = 1.0-location;

    map<double,FAULT>::const_iterator iter;
    for(iter=faults.begin(); iter!=faults.end(); iter++)
    {
        if(fabs(iter->first-location)<FLOAT_EPSILON)
            break;
    }
    if(iter!=faults.end())
        return iter->second;
    else
        return nofault;
}

void LINE::clear_all_faults()
{
    faults.clear();
}

void LINE::clear_fault_at_location(size_t to_bus, double location)
{
    if(not is_connected_to_bus(to_bus))
        return;

    if(location<0.0 or location>1.0)
        return;

    if(to_bus == get_receiving_side_bus())
        location = 1.0-location;

    map<double,FAULT>::iterator iter;
    for(iter=faults.begin(); iter!=faults.end(); iter++)
    {
        if(fabs(iter->first-location)<FLOAT_EPSILON)
            break;
    }
    if(iter!=faults.end())
    {
        ostringstream osstream;
        FAULT fault = iter->second;
        string fault_type_str = fault.get_fault_type_string();
        complex<double> y = fault.get_fault_shunt_in_pu();

        if(to_bus==get_sending_side_bus())
            osstream<<fault_type_str<<" is cleared for "<<get_device_name()<<endl
              <<"Fault shunt "<<y<<" pu at location "<<location<<" to bus "<<get_sending_side_bus()<<".";
        else
            osstream<<fault_type_str<<" is cleared for "<<get_device_name()<<endl
              <<"Fault shunt "<<y<<" pu at location "<<1.0-location<<" to bus "<<get_receiving_side_bus()<<".";
        show_information_with_leading_time_stamp(osstream);
        faults.erase(iter);
    }
}

size_t LINE::get_fault_count() const
{
    return faults.size();
}

bool LINE::is_faulted() const
{
    if(get_fault_count()>0)
        return true;
    else
        return false;
}

bool LINE::is_valid() const
{
    if(get_sending_side_bus()!=0 and get_receiving_side_bus()!=0)
        return true;
    else
        return false;
}

void LINE::check()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb==NULL)
    {
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Line base voltage will not be checked.";
        show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        if(psdb->get_bus_base_voltage_in_kV(get_sending_side_bus()) !=
           psdb->get_bus_base_voltage_in_kV(get_receiving_side_bus()))
        {
            osstream<<"Error. base voltage at sending and receiving sides are different.";
            show_information_with_leading_time_stamp(osstream);
        }
    }

}

void LINE::clear()
{
    sending_side_bus = 0;
    receiving_side_bus = 0;
    set_identifier("");
    set_sending_side_breaker_status(false);
    set_receiving_side_breaker_status(false);
    set_line_positive_sequence_z_in_pu(0.0);
    set_line_positive_sequence_y_in_pu(0.0);
    set_shunt_positive_sequence_y_at_sending_side_in_pu(0.0);
    set_shunt_positive_sequence_y_at_receiving_side_in_pu(0.0);
    set_line_zero_sequence_z_in_pu(0.0);
    set_line_zero_sequence_y_in_pu(0.0);
    set_shunt_zero_sequence_y_at_sending_side_in_pu(0.0);
    set_shunt_zero_sequence_y_at_receiving_side_in_pu(0.0);
    RATING linerating;
    linerating.set_rating_A_MVA(0.0);
    linerating.set_rating_B_MVA(0.0);
    linerating.set_rating_C_MVA(0.0);
    set_rating(linerating);
    set_meter_end_bus(0);
    set_length(0.0);
    clear_all_faults();
}

bool LINE::is_connected_to_bus(size_t bus) const
{
    if(get_sending_side_bus()==bus || get_receiving_side_bus()==bus) return true;
    else return false;
}

bool LINE::is_in_area(size_t area) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr_send = psdb->get_bus(get_sending_side_bus());
        BUS* busptr_rec = psdb->get_bus(get_receiving_side_bus());
        if(busptr_send!=NULL or busptr_rec!=NULL)
        {
            bool in_area = false;
            if(busptr_send!=NULL)
                in_area = in_area or busptr_send->is_in_area(area);
            if(busptr_rec!=NULL)
                in_area = in_area or busptr_rec->is_in_area(area);
            return in_area;
        }
        else
            return false;
    }
    else
        return false;
}

bool LINE::is_in_zone(size_t zone) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr_send = psdb->get_bus(get_sending_side_bus());
        BUS* busptr_rec = psdb->get_bus(get_receiving_side_bus());
        if(busptr_send!=NULL or busptr_rec!=NULL)
        {
            bool in_zone = false;
            if(busptr_send!=NULL)
                in_zone = in_zone or busptr_send->is_in_zone(zone);
            if(busptr_rec!=NULL)
                in_zone = in_zone or busptr_rec->is_in_zone(zone);
            return in_zone;
        }
        else
            return false;
    }
    else
        return false;
}

void LINE::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<((get_sending_side_breaker_status()==true and get_receiving_side_breaker_status()==true)?"in service":"out of service")<<", "
      <<"line R+jX = "<<setw(8)<<setprecision(4)<<fixed<<get_line_positive_sequence_z_in_pu()<<" pu, "
      <<"line G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_line_positive_sequence_y_in_pu()<<" pu, "
      <<"sending shunt G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_shunt_positive_sequence_y_at_sending_side_in_pu()<<" pu, "
      <<"receiving shunt G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_shunt_positive_sequence_y_at_receiving_side_in_pu()<<" pu";
    show_information_with_leading_time_stamp(osstream);
}

void LINE::save() const
{
    ;
}

void LINE::set_model(const MODEL* model)
{
    ;
}

LINE& LINE::operator=(const LINE& line)
{
    if(this==(&line)) return (*this);

    clear();
    set_power_system_database(line.get_power_system_database());
    set_sending_side_bus(line.get_sending_side_bus());
    set_receiving_side_bus(line.get_receiving_side_bus());
    set_identifier(line.get_identifier());
    set_sending_side_breaker_status(line.get_sending_side_breaker_status());
    set_receiving_side_breaker_status(line.get_receiving_side_breaker_status());
    set_line_positive_sequence_z_in_pu(line.get_line_positive_sequence_z_in_pu());
    set_line_positive_sequence_y_in_pu(line.get_line_positive_sequence_y_in_pu());
    set_shunt_positive_sequence_y_at_sending_side_in_pu(line.get_shunt_positive_sequence_y_at_sending_side_in_pu());
    set_shunt_positive_sequence_y_at_receiving_side_in_pu(line.get_shunt_positive_sequence_y_at_receiving_side_in_pu());
    set_line_zero_sequence_z_in_pu(line.get_line_zero_sequence_z_in_pu());
    set_line_zero_sequence_y_in_pu(line.get_line_zero_sequence_y_in_pu());
    set_shunt_zero_sequence_y_at_sending_side_in_pu(line.get_shunt_zero_sequence_y_at_sending_side_in_pu());
    set_shunt_zero_sequence_y_at_receiving_side_in_pu(line.get_shunt_zero_sequence_y_at_receiving_side_in_pu());
    set_rating(line.get_rating());
    set_meter_end_bus(line.get_meter_end_bus());
    set_length(line.get_length());
    if(line.is_faulted())
    {
        size_t nfault = line.get_fault_count();
        for(size_t i=0; i!=nfault; ++i)
        {
            double location = line.get_fault_location_of_fault(i);
            FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(),location);
            set_fault(get_sending_side_bus(),location, fault);
        }
    }
    else
        clear_all_faults();

    return *this;
}


DEVICE_ID LINE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("LINE");

    TERMINAL terminal;
    terminal.append_bus(get_sending_side_bus());
    terminal.append_bus(get_receiving_side_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string LINE::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

double LINE::get_line_base_voltage_in_kV() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb!=NULL)
        return psdb->get_bus_base_voltage_in_kV(get_sending_side_bus());
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Line base voltage will be returned as 0.0.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}


complex<double> LINE::get_line_complex_voltage_at_sending_side_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb!=NULL)
        return psdb->get_bus_complex_voltage_in_pu(get_sending_side_bus());
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex voltage at sending side will be returned as 0.0 p.u.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

complex<double> LINE::get_line_complex_voltage_at_receiving_side_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb!=NULL)
        return psdb->get_bus_complex_voltage_in_pu(get_receiving_side_bus());
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex voltage at receiving side will be returned as 0.0 p.u.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

complex<double> LINE::get_line_complex_voltage_at_sending_side_in_kV() const
{

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
        return psdb->get_bus_complex_voltage_in_kV(get_sending_side_bus());
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex voltage at sending side will be returned as 0.0 kV.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

complex<double> LINE::get_line_complex_voltage_at_receiving_side_in_kV() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb!=NULL)
        return psdb->get_bus_complex_voltage_in_kV(get_receiving_side_bus());
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex voltage at receiving side will be returned as 0.0 kV.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

complex<double> LINE::get_line_complex_current_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==false)
        return 0.0;

    complex<double> Z = get_line_positive_sequence_z_in_pu();
    complex<double> Y = get_line_positive_sequence_y_in_pu();
    complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
    complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();

    complex<double> Vs = get_line_complex_voltage_at_sending_side_in_pu();

    if(get_receiving_side_breaker_status()==false)
    {
        complex<double> Yeq = Ys+1.0/(Z+1.0/Yr);
        return Vs*Yeq;
    }
    else
    {
        complex<double> Vr = get_line_complex_voltage_at_receiving_side_in_pu();
        return (Vs-Vr)/Z+Vs*Ys;
    }
}

complex<double> LINE::get_line_complex_current_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==false)
        return 0.0;

    complex<double> Z = get_line_positive_sequence_z_in_pu();
    complex<double> Y = get_line_positive_sequence_y_in_pu();
    complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
    complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();


    complex<double> Vr = get_line_complex_voltage_at_receiving_side_in_pu();
    if(get_sending_side_breaker_status()==false)
    {
        complex<double> Yeq = Yr+1.0/(Z+1.0/Ys);
        return Vr*Yeq;
    }
    else
    {
        complex<double> Vs = get_line_complex_voltage_at_sending_side_in_pu();
        return (Vr-Vs)/Z+Vr*Yr;
    }
}

complex<double> LINE::get_line_complex_current_at_sending_side_in_kA() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex current at sending side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    double Ibase_kA = mvabase/sqrt(3.0)/get_line_base_voltage_in_kV();

    return Ibase_kA*get_line_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_complex_current_at_receiving_side_in_kA() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex current at receiving side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    double Ibase_kA = mvabase/sqrt(3.0)/get_line_base_voltage_in_kV();

    return Ibase_kA*get_line_complex_current_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_complex_power_at_sending_side_in_pu() const
{
    return get_line_complex_voltage_at_sending_side_in_pu()*conj(get_line_complex_current_at_sending_side_in_pu());
}

complex<double> LINE::get_line_complex_power_at_receiving_side_in_pu() const
{
    return get_line_complex_voltage_at_receiving_side_in_pu()*conj(get_line_complex_current_at_receiving_side_in_pu());
}

complex<double> LINE::get_line_complex_power_at_sending_side_in_MVA() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex power at sending side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    return mvabase*get_line_complex_power_at_sending_side_in_pu();
}

complex<double> LINE::get_line_complex_power_at_receiving_side_in_MVA() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex power at receiving side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    return mvabase*get_line_complex_power_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_complex_apparent_impedance_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==false)
        return INFINITE_THRESHOLD;
    else
        return get_line_complex_voltage_at_sending_side_in_pu()/get_line_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_complex_apparent_impedance_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==false)
        return INFINITE_THRESHOLD;
    else
        return get_line_complex_voltage_at_receiving_side_in_pu()/get_line_complex_current_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_complex_apparent_impedance_at_sending_side_in_ohm() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex apparent impedance at sending side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    if(get_sending_side_breaker_status()==false)
        return INDEX_NOT_EXIST;
    else
    {
        double Vbase_kV = get_line_base_voltage_in_kV();
        double Zbase_ohm = Vbase_kV*Vbase_kV/mvabase;
        return get_line_complex_apparent_impedance_at_sending_side_in_pu()*Zbase_ohm;
    }
}

complex<double> LINE::get_line_complex_apparent_impedance_at_receiving_side_in_ohm() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double mvabase = 100.0;

    if(psdb!=NULL)
        mvabase = psdb->get_system_base_power_in_MVA();
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<get_device_name()<<" is not assigned to any power system database."<<endl
          <<"Complex apparent impedance at receiving side will be returned with MVABASE=100MVA.";
        show_information_with_leading_time_stamp(osstream);
    }

    if(get_receiving_side_breaker_status()==false)
        return INFINITE_THRESHOLD;
    else
    {
        double Vbase_kV = get_line_base_voltage_in_kV();
        double Zbase_ohm = Vbase_kV*Vbase_kV/mvabase;
        return get_line_complex_apparent_impedance_at_receiving_side_in_pu()*Zbase_ohm;
    }
}


