#include "header/device/line.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;
LINE::LINE(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

LINE::~LINE()
{
    ;
}

void LINE::set_sending_side_bus(unsigned int bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(bus!=0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        if(psdb.is_bus_exist(bus))
        {
            sending_side_bus = bus;
            sending_side_busptr = psdb.get_bus(bus);
        }
        else
        {
            osstream<<"Warning. Bus "<<bus<<" does not exist for setting up sending side bus of line."<<endl
              <<"0 will be set to indicate invalid transmission line.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            sending_side_bus = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up sending side bus of line."<<endl
          <<"0 will be set to indicate invalid transmission line.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        sending_side_bus = 0;
    }
}

void LINE::set_receiving_side_bus(unsigned int bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(bus!=0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        if(psdb.is_bus_exist(bus))
        {
            receiving_side_bus = bus;
            receiving_side_busptr = psdb.get_bus(bus);
        }
        else
        {
            osstream<<"Warning. Bus "<<bus<<" does not exist for setting up receiving side bus of line."<<endl
                    <<"0 will be set to indicate invalid transmission line.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            receiving_side_bus = 0;
            return;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up receiving side bus of line."<<endl
                <<"0 will be set to indicate invalid transmission line.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        receiving_side_bus = 0;
    }
}

void LINE::set_identifier(string line_id)
{
    line_id = trim_string(line_id);
    add_string_to_str_int_map(line_id);
    this->identifier_index = get_index_of_string(line_id);
}

void LINE::set_name(string name)
{
    name = trim_string(name);
    add_string_to_str_int_map(name);
    this->name_index = get_index_of_string(name);
}

void LINE::set_sending_side_breaker_status(bool status)
{
    this->sending_side_breaker_status = status;
}

void LINE::set_receiving_side_breaker_status(bool status)
{
    this->receiving_side_breaker_status = status;
}

void LINE::set_line_positive_sequence_z_in_pu(const complex<double>& z)
{
    line_z_12 = z;
}

void LINE::set_line_positive_sequence_y_in_pu(const complex<double>& y)
{
    line_y_12 = y;
}

void LINE::set_shunt_positive_sequence_y_at_sending_side_in_pu(const complex<double>& y)
{
    shunt_y_12_sending_side = y;
}

void LINE::set_shunt_positive_sequence_y_at_receiving_side_in_pu(const complex<double>& y)
{
    shunt_y_12_receiving_side = y;
}

void LINE::set_line_zero_sequence_z_in_pu(const complex<double>& z)
{
    line_z_0 = z;
}

void LINE::set_line_zero_sequence_y_in_pu(const complex<double>& y)
{
    line_y_0 = y;
}

void LINE::set_shunt_zero_sequence_y_at_sending_side_in_pu(const complex<double>& y)
{
    shunt_y_0_sending_side = y;
}

void LINE::set_shunt_zero_sequence_y_at_receiving_side_in_pu(const complex<double>& y)
{
    shunt_y_0_receiving_side = y;
}

void LINE::set_rating(RATING line_rating)
{
    this->rating = line_rating;
}

void LINE::set_meter_end_bus(unsigned int meter_bus)
{
    if(meter_bus == get_receiving_side_bus()) meter_end_bus = get_receiving_side_bus();
    else                                      meter_end_bus = get_sending_side_bus();
}

void LINE::set_length(double line_length)
{
    if(line_length>=0.0) this->length = line_length;
}

void LINE::set_mutual_admittances(vector<complex<double>> Y)
{
    Ymutual.assign(Y.begin(), Y.end());
}

void LINE::set_line_pointers_corresponding_to_mutual_admittances(vector<LINE*> lineptrs)
{
    lineptrs_of_Ymutual.assign(lineptrs.begin(),lineptrs.end());
}

void LINE::set_is_mutual_logic(bool b)
{
    is_mutual_line = b;
}

unsigned int LINE::get_sending_side_bus() const
{
    return sending_side_bus;
}

unsigned int LINE::get_receiving_side_bus() const
{
    return receiving_side_bus;
}

BUS* LINE::get_sending_side_bus_pointer() const
{
    return sending_side_busptr;
}

BUS* LINE::get_receiving_side_bus_pointer() const
{
    return receiving_side_busptr;
}

string LINE::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

string LINE::get_name() const
{
    return get_string_of_index(name_index);
}

unsigned int LINE::get_identifier_index() const
{
    return identifier_index;
}

unsigned int LINE::get_name_index() const
{
    return name_index;
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

unsigned int LINE::get_meter_end_bus() const
{
    return meter_end_bus;
}

double LINE::get_length() const
{
    return length;
}

bool LINE::is_zero_impedance_line() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double z_th = psdb.get_zero_impedance_threshold_in_pu();
    double y = abs(get_line_positive_sequence_y_in_pu());
    double z = abs(get_line_positive_sequence_z_in_pu());
    if(y<DOUBLE_EPSILON and (z<=z_th or (z-z_th)<DOUBLE_EPSILON))
        return true;
    else
        return false;
}

void LINE::set_fault(unsigned int to_bus, double location, const FAULT& fault)
{
    if(is_connected_to_bus(to_bus))
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        if(location<0.0 or location>1.0)
        {
            osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<get_compound_device_name()<<"."<<endl
                    <<"No fault will be set.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }

        if(not fault.is_faulted())
        {
            osstream<<"Warning. Given fault is not faulted for "<<get_compound_device_name()<<"."<<endl
                   <<"No fault will be set.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }

        string fault_type = fault.get_fault_type_string();

        complex<double> y = fault.get_fault_shunt_in_pu();
        osstream<<fault_type<<" fault is set for "<<get_compound_device_name()<<"."<<endl
               <<"Fault shunt is "<<y<<" pu at location "<<location<<" to bus "<<to_bus;

        if(to_bus == get_receiving_side_bus())
            location = 1.0-location;

        map<double,FAULT>::iterator iter;
        for(iter=faults.begin(); iter!=faults.end(); ++iter)
        {
            if(fabs(iter->first-location)<FLOAT_EPSILON)
                break;
        }
        if(iter!=faults.end())
        {
            osstream<<"Fault at location "<<location<<" (to sending side bus) already exist for "<<get_compound_device_name()<<"."<<endl
                  <<"Setting fault at this location will remove the previous one";
            iter->second = fault;
        }
        else
        {
            faults.insert(pair<double,FAULT>(location, fault));
        }
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

double LINE::get_fault_location_of_fault(unsigned int index) const
{
    if(index<get_fault_count())
    {
        map<double,FAULT>::const_iterator iter=faults.begin();

        for(unsigned int i=0; i!=index; ++i)
            ++iter;

        return iter->first;
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Invalid index ("<<index<<") was given to get fault location. [0, "<<faults.size()<<") is allowed.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

FAULT LINE::get_fault_at_location(unsigned int to_bus, double location) const
{
    FAULT nofault;
    if(is_connected_to_bus(to_bus) and (location>=0.0 and location<=1.0))
    {
        if(to_bus == get_receiving_side_bus())
            location = 1.0-location;

        map<double,FAULT>::const_iterator iter;
        for(iter=faults.begin(); iter!=faults.end(); ++iter)
        {
            if(fabs(iter->first-location)<FLOAT_EPSILON)
                break;
        }
        if(iter!=faults.end())
            return iter->second;
        else
            return nofault;
    }
    else
        return nofault;
}

void LINE::clear_all_faults()
{
    faults.clear();
}

void LINE::clear_fault_at_location(unsigned int to_bus, double location)
{
    if(is_connected_to_bus(to_bus) and (location>=0.0 and location<=1.0))
    {
        STEPS& toolkit = get_toolkit();

        if(to_bus == get_receiving_side_bus())
            location = 1.0-location;

        map<double,FAULT>::iterator iter;
        for(iter=faults.begin(); iter!=faults.end(); ++iter)
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
                osstream<<fault_type_str<<" is cleared for "<<get_compound_device_name()<<endl
                  <<"Fault shunt "<<y<<" pu at location "<<location<<" to bus "<<get_sending_side_bus()<<".";
            else
                osstream<<fault_type_str<<" is cleared for "<<get_compound_device_name()<<endl
                  <<"Fault shunt "<<y<<" pu at location "<<1.0-location<<" to bus "<<get_receiving_side_bus()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
            faults.erase(iter);
        }
    }
}

unsigned int LINE::get_fault_count() const
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

bool LINE::is_mutual() const
{
    return is_mutual_line;
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
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int ibus = get_sending_side_bus();
    unsigned int jbus = get_receiving_side_bus();
    double ivbase = psdb.get_bus_base_voltage_in_kV(ibus);
    double jvbase = psdb.get_bus_base_voltage_in_kV(jbus);

    osstream<<"Warning detected when checking "<<ivbase<<"kV "<<get_compound_device_name()<<"["
            <<psdb.bus_number2bus_name(get_sending_side_bus())<<"-"
            <<psdb.bus_number2bus_name(get_receiving_side_bus())<<"]:\n";

    unsigned int error_count = 0;

    if( ivbase!= jvbase)
    {
        error_count++;
        osstream<<"("<<error_count<<") Base voltage at sending and receiving sides are different: "
                <<"Vbase["<<ibus<<"] = "<<ivbase<<"kV. "
                <<"Vbase["<<jbus<<"] = "<<jvbase<<"kV.\n";
    }
    complex<double> z = get_line_positive_sequence_z_in_pu();
    double r = z.real(), x = z.imag();
    if(r<0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence R<0: "
                <<"R = "<<r<<"pu.\n";
    }
    /*if(fabs(r)>10.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence R is too great: "
                <<"R = "<<r<<"pu.\n";
    }
    if(fabs(x)>10.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence X is too great: "
                <<"X = "<<x<<"pu.\n";
    }*/
    if(fabs(x)<0.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence X<0: "
                <<"X = "<<x<<"pu.\n";
    }
    if(x!=0.0 and fabs(r/x)>2.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence |R/X|>2.0: "
                <<"R = "<<r<<"pu, X = "<<x<<"pu, R/X = "<<r/x<<".\n";
    }
    complex<double> y = get_line_positive_sequence_y_in_pu();
    double g = y.real(), b = y.imag();
    if(fabs(g)>50.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence G is too great: "
                <<"G = "<<g<<"pu.\n";
    }
    if(fabs(b)>50.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence B is too great: "
                <<"B = "<<b<<"pu.\n";
    }
    if(error_count>0)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void LINE::clear()
{
    sending_side_bus = 0;
    receiving_side_bus = 0;
    sending_side_busptr = NULL;
    receiving_side_busptr = NULL;
    set_identifier("");
    set_name("");
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
    set_is_mutual_logic(false);

    set_sequence_parameter_import_flag(false);
}

bool LINE::is_connected_to_bus(unsigned int bus) const
{
    if(get_sending_side_bus()==bus or get_receiving_side_bus()==bus) return true;
    else                                                             return false;
}

bool LINE::is_in_area(unsigned int area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr_send = psdb.get_bus(get_sending_side_bus());
    BUS* busptr_rec = psdb.get_bus(get_receiving_side_bus());
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

bool LINE::is_in_zone(unsigned int zone) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr_send = psdb.get_bus(get_sending_side_bus());
    BUS* busptr_rec = psdb.get_bus(get_receiving_side_bus());
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

void LINE::report() const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_compound_device_name()<<": "<<((get_sending_side_breaker_status()==true and get_receiving_side_breaker_status()==true)?"in service":"out of service")<<", "
      <<"line R+jX = "<<setw(8)<<setprecision(4)<<fixed<<get_line_positive_sequence_z_in_pu()<<" pu, "
      <<"line G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_line_positive_sequence_y_in_pu()<<" pu, "
      <<"sending shunt G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_shunt_positive_sequence_y_at_sending_side_in_pu()<<" pu, "
      <<"receiving shunt G+jB = "<<setw(8)<<setprecision(4)<<fixed<<get_shunt_positive_sequence_y_at_receiving_side_in_pu()<<" pu";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void LINE::save() const
{
    ;
}

void LINE::set_model(MODEL* model)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"LINE::"<<__FUNCTION__<<"() has not been implemented yet. Input model name is:"<<(model==NULL?"":model->get_model_name());
    toolkit.show_information_with_leading_time_stamp(osstream);
}

MODEL* LINE::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="LINE RELAY")
        return NULL;
    return NULL;
}

LINE& LINE::operator=(const LINE& line)
{
    if(this==(&line)) return (*this);

    set_toolkit(line.get_toolkit());
    clear();

    set_sending_side_bus(line.get_sending_side_bus());
    set_receiving_side_bus(line.get_receiving_side_bus());
    set_identifier(line.get_identifier());
    set_name(line.get_name());
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
        unsigned int nfault = line.get_fault_count();
        for(unsigned int i=0; i!=nfault; ++i)
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
    did.set_device_type(STEPS_AC_LINE);

    TERMINAL terminal;
    terminal.append_bus(get_sending_side_bus());
    terminal.append_bus(get_receiving_side_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

double LINE::get_line_base_voltage_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return psdb.get_bus_base_voltage_in_kV(get_sending_side_bus());
}

vector<complex<double>> LINE::get_mutual_admittances() const
{
    return Ymutual;
}

vector<LINE*> LINE::get_line_pointers_corresponding_to_mutual_admittances() const
{
    return lineptrs_of_Ymutual;
}


complex<double> LINE::get_line_complex_voltage_at_sending_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_sending_side_bus());
}

complex<double> LINE::get_line_complex_voltage_at_receiving_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_receiving_side_bus());
}

complex<double> LINE::get_line_complex_voltage_at_sending_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_sending_side_bus());
}

complex<double> LINE::get_line_complex_voltage_at_receiving_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_receiving_side_bus());
}

complex<double> LINE::get_line_positive_sequence_complex_voltage_at_sending_side_in_pu() const
{
    return get_line_complex_voltage_at_sending_side_in_pu();
}

complex<double> LINE::get_line_positive_sequence_complex_voltage_at_receiving_side_in_pu() const
{
    return get_line_complex_voltage_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_positive_sequence_complex_voltage_at_sending_side_in_kV() const
{
    return get_line_complex_voltage_at_sending_side_in_kV();
}

complex<double> LINE::get_line_positive_sequence_complex_voltage_at_receiving_side_in_kV() const
{
    return get_line_complex_voltage_at_receiving_side_in_kV();
}

complex<double> LINE::get_line_negative_sequence_complex_voltage_at_sending_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_negative_sequence_complex_voltage_in_pu(get_sending_side_bus());
}

complex<double> LINE::get_line_negative_sequence_complex_voltage_at_receiving_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_negative_sequence_complex_voltage_in_pu(get_receiving_side_bus());
}

complex<double> LINE::get_line_negative_sequence_complex_voltage_at_sending_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_negative_sequence_complex_voltage_in_kV(get_sending_side_bus());
}

complex<double> LINE::get_line_negative_sequence_complex_voltage_at_receiving_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_negative_sequence_complex_voltage_in_kV(get_receiving_side_bus());
}

complex<double> LINE::get_line_zero_sequence_complex_voltage_at_sending_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_zero_sequence_complex_voltage_in_pu(get_sending_side_bus());
}

complex<double> LINE::get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_zero_sequence_complex_voltage_in_pu(get_receiving_side_bus());
}

complex<double> LINE::get_line_zero_sequence_complex_voltage_at_sending_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_zero_sequence_complex_voltage_in_kV(get_sending_side_bus());
}

complex<double> LINE::get_line_zero_sequence_complex_voltage_at_receiving_side_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_zero_sequence_complex_voltage_in_kV(get_receiving_side_bus());
}


complex<double> LINE::get_line_complex_current_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==true)
    {
        complex<double> Z = get_line_positive_sequence_z_in_pu();
        complex<double> Y = get_line_positive_sequence_y_in_pu();
        complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
        complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();

        complex<double> Vs = get_line_complex_voltage_at_sending_side_in_pu();

        if(get_receiving_side_breaker_status()==true)
        {
            complex<double> Vr = get_line_complex_voltage_at_receiving_side_in_pu();
            return (Vs-Vr)/Z+Vs*Ys;
        }
        else
        {
            complex<double> Yeq = Ys+1.0/(Z+1.0/Yr);
            return Vs*Yeq;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_complex_current_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==true)
    {
        complex<double> Z = get_line_positive_sequence_z_in_pu();
        complex<double> Y = get_line_positive_sequence_y_in_pu();
        complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
        complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();


        complex<double> Vr = get_line_complex_voltage_at_receiving_side_in_pu();
        if(get_sending_side_breaker_status()==true)
        {
            complex<double> Vs = get_line_complex_voltage_at_sending_side_in_pu();
            return (Vr-Vs)/Z+Vr*Yr;
        }
        else
        {
            complex<double> Yeq = Yr+1.0/(Z+1.0/Ys);
            return Vr*Yeq;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_complex_current_at_sending_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());

    return Ibase_kA*get_line_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_complex_current_at_receiving_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());

    return Ibase_kA*get_line_complex_current_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_positive_sequence_complex_current_at_sending_side_in_pu() const
{
    return get_line_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_positive_sequence_complex_current_at_receiving_side_in_pu() const
{
    return get_line_complex_current_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_positive_sequence_complex_current_at_sending_side_in_kA() const
{
    return get_line_complex_current_at_sending_side_in_kA();
}

complex<double> LINE::get_line_positive_sequence_complex_current_at_receiving_side_in_kA() const
{
    return get_line_complex_current_at_receiving_side_in_kA();
}

complex<double> LINE::get_line_negative_sequence_complex_current_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==true)
    {
        complex<double> Z = get_line_positive_sequence_z_in_pu();
        complex<double> Y = get_line_positive_sequence_y_in_pu();
        complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
        complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();

        complex<double> Vs = get_line_negative_sequence_complex_voltage_at_sending_side_in_pu();
        if(get_receiving_side_breaker_status()==true)
        {
            complex<double> Vr = get_line_negative_sequence_complex_voltage_at_receiving_side_in_pu();
            return (Vs-Vr)/Z+Vs*Ys;
        }
        else
        {
            complex<double> Yeq = Ys+1.0/(Z+1.0/Yr);
            return Vs*Yeq;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_negative_sequence_complex_current_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==true)
    {
        complex<double> Z = get_line_positive_sequence_z_in_pu();
        complex<double> Y = get_line_positive_sequence_y_in_pu();
        complex<double> Ys = 0.5*Y + get_shunt_positive_sequence_y_at_sending_side_in_pu();
        complex<double> Yr = 0.5*Y + get_shunt_positive_sequence_y_at_receiving_side_in_pu();

        complex<double> Vr = get_line_negative_sequence_complex_voltage_at_receiving_side_in_pu();
        if(get_sending_side_breaker_status()==true)
        {
            complex<double> Vs = get_line_negative_sequence_complex_voltage_at_sending_side_in_pu();
            return (Vr-Vs)/Z+Vr*Yr;
        }
        else
        {
            complex<double> Yeq = Yr+1.0/(Z+1.0/Ys);
            return Vr*Yeq;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_negative_sequence_complex_current_at_sending_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());
    return Ibase_kA*get_line_negative_sequence_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_negative_sequence_complex_current_at_receiving_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());
    return Ibase_kA*get_line_negative_sequence_complex_current_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_zero_sequence_complex_current_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==true)
    {
        if(not is_mutual())
        {
            complex<double> Z = get_line_zero_sequence_z_in_pu();
            complex<double> Y = get_line_zero_sequence_y_in_pu();
            complex<double> Ys = 0.5*Y + get_shunt_zero_sequence_y_at_sending_side_in_pu();
            complex<double> Yr = 0.5*Y + get_shunt_zero_sequence_y_at_receiving_side_in_pu();

            complex<double> Vs = get_line_zero_sequence_complex_voltage_at_sending_side_in_pu();
            if(get_receiving_side_breaker_status()==true)
            {
                complex<double> Vr = get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu();
                return (Vs-Vr)/Z+Vs*Ys;
            }
            else
            {
                complex<double> Yeq = Ys+1.0/(Z+1.0/Yr);
                return Vs*Yeq;
            }
        }
        else
        {
            vector<complex<double> > Y_mutual = get_mutual_admittances();
            vector<LINE*> lineptrs_of_Y_mutual = get_line_pointers_corresponding_to_mutual_admittances();
            complex<double> Y = get_line_zero_sequence_y_in_pu();
            complex<double> Ys = 0.5*Y + get_shunt_zero_sequence_y_at_sending_side_in_pu();
            complex<double> Yr = 0.5*Y + get_shunt_zero_sequence_y_at_receiving_side_in_pu();
            complex<double> Vs = get_line_zero_sequence_complex_voltage_at_sending_side_in_pu();

            complex<double> I = 0.0;
            for(unsigned int i=0; i<Y_mutual.size(); i++)
            {
                LINE* lineptr = lineptrs_of_Y_mutual[i];
                complex<double> V_s = lineptr->get_line_zero_sequence_complex_voltage_at_sending_side_in_pu();
                complex<double> V_r = lineptr->get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu();
                I = I + Y_mutual[i]*(V_s-V_r);
            }
            I = I + Vs*Ys;
            return I;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_zero_sequence_complex_current_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==true)
    {
        if(not is_mutual())
        {
            complex<double> Z = get_line_zero_sequence_z_in_pu();
            complex<double> Y = get_line_zero_sequence_y_in_pu();
            complex<double> Ys = 0.5*Y + get_shunt_zero_sequence_y_at_sending_side_in_pu();
            complex<double> Yr = 0.5*Y + get_shunt_zero_sequence_y_at_receiving_side_in_pu();

            complex<double> Vr = get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu();
            if(get_sending_side_breaker_status()==true)
            {
                complex<double> Vs = get_line_zero_sequence_complex_voltage_at_sending_side_in_pu();
                return (Vr-Vs)/Z+Vr*Yr;
            }
            else
            {
                complex<double> Yeq = Yr+1.0/(Z+1.0/Ys);
                return Vr*Yeq;
            }
        }
        else
        {
            vector<complex<double> > Y_mutual = get_mutual_admittances();
            vector<LINE*> lineptrs_of_Y_mutual = get_line_pointers_corresponding_to_mutual_admittances();
            complex<double> Y = get_line_zero_sequence_y_in_pu();
            complex<double> Ys = 0.5*Y + get_shunt_zero_sequence_y_at_sending_side_in_pu();
            complex<double> Yr = 0.5*Y + get_shunt_zero_sequence_y_at_receiving_side_in_pu();
            complex<double> Vr = get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu();

            complex<double> I = 0.0;
            for(unsigned int i=0; i<Y_mutual.size(); i++)
            {
                LINE* lineptr = lineptrs_of_Y_mutual[i];
                complex<double> V_s = lineptr->get_line_zero_sequence_complex_voltage_at_sending_side_in_pu();
                complex<double> V_r = lineptr->get_line_zero_sequence_complex_voltage_at_receiving_side_in_pu();
                I = I + Y_mutual[i]*(V_r-V_s);
            }
            I = I + Vr*Yr;
            return I;
        }
    }
    else
        return 0.0;
}

complex<double> LINE::get_line_zero_sequence_complex_current_at_sending_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());
    return Ibase_kA*get_line_zero_sequence_complex_current_at_sending_side_in_pu();
}

complex<double> LINE::get_line_zero_sequence_complex_current_at_receiving_side_in_kA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();
    double Ibase_kA = sbase/(SQRT3*get_line_base_voltage_in_kV());
    return Ibase_kA*get_line_zero_sequence_complex_current_at_receiving_side_in_pu();
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
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    return sbase*get_line_complex_power_at_sending_side_in_pu();
}

complex<double> LINE::get_line_complex_power_at_receiving_side_in_MVA() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    return sbase*get_line_complex_power_at_receiving_side_in_pu();
}

complex<double> LINE::get_line_complex_apparent_impedance_at_sending_side_in_pu() const
{
    if(get_sending_side_breaker_status()==true)
        return get_line_complex_voltage_at_sending_side_in_pu()/get_line_complex_current_at_sending_side_in_pu();
    else
        return INFINITE_THRESHOLD;
}

complex<double> LINE::get_line_complex_apparent_impedance_at_receiving_side_in_pu() const
{
    if(get_receiving_side_breaker_status()==true)
        return get_line_complex_voltage_at_receiving_side_in_pu()/get_line_complex_current_at_receiving_side_in_pu();
    else
        return INFINITE_THRESHOLD;
}

complex<double> LINE::get_line_complex_apparent_impedance_at_sending_side_in_ohm() const
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    if(get_sending_side_breaker_status()==true)
    {
        double Vbase_kV = get_line_base_voltage_in_kV();
        double Zbase_ohm = Vbase_kV*Vbase_kV*one_over_sbase;
        return get_line_complex_apparent_impedance_at_sending_side_in_pu()*Zbase_ohm;
    }
    else
        return INDEX_NOT_EXIST;
}

complex<double> LINE::get_line_complex_apparent_impedance_at_receiving_side_in_ohm() const
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    if(get_receiving_side_breaker_status()==true)
    {
        double Vbase_kV = get_line_base_voltage_in_kV();
        double Zbase_ohm = Vbase_kV*Vbase_kV*one_over_sbase;
        return get_line_complex_apparent_impedance_at_receiving_side_in_pu()*Zbase_ohm;
    }
    else
        return INFINITE_THRESHOLD;
}

void LINE::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool LINE::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}
