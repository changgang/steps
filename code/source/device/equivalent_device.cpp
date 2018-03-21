#include "header/device/device.h"
#include "header/basic/utility.h"
#include "header/model/equivalent_model/equivalent_model.h"
#include "header/model/equivalent_model/equivalent_models.h"
#include <iostream>

using namespace std;

EQUIVALENT_DEVICE::EQUIVALENT_DEVICE(POWER_SYSTEM_DATABASE* psdb)
{
    ostringstream sstream;
    if(psdb==NULL)
    {
        sstream<<"Error. EQUIVALENT_DEVICE object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(sstream);
    }
    set_power_system_database(psdb);
    clear();

    equivalent_model = NULL;
}

EQUIVALENT_DEVICE::~EQUIVALENT_DEVICE()
{
    if(equivalent_model != NULL)
    {
        delete equivalent_model;
    }

    clear();
}

void EQUIVALENT_DEVICE::set_equivalent_device_bus(size_t device_bus)
{
    ostringstream sstream;

    if(device_bus==0)
    {
        sstream<<"Warning. Zero bus number (0) is not allowed for setting up equivalent device bus."<<endl
          <<"0 will be set to indicate invalid equivalent device.";
        show_information_with_leading_time_stamp(sstream);
        this->bus = device_bus;
        return;
    }

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb==NULL)
        this->bus = device_bus;
    else
    {
        if(not psdb->is_bus_exist(device_bus))
        {
            sstream<<"Bus "<<device_bus<<" does not exist for setting up equivalent device."<<endl
              <<"0 will be set to indicate invalid equivalent device.";
            show_information_with_leading_time_stamp(sstream);
            this->bus = 0;
            return;
        }
        this->bus = device_bus;
    }
}

void EQUIVALENT_DEVICE::set_identifier(string identifier)
{
    this->identifier = identifier;
}

void EQUIVALENT_DEVICE::set_status(bool status)
{
    this->status = status;
}

void EQUIVALENT_DEVICE::set_equivalent_voltage_source_status(bool status)
{
    equivalent_voltage_source_status = status;
}

void EQUIVALENT_DEVICE::set_equivalent_load_status(bool status)
{
    equivalent_load_status = status;
}

void EQUIVALENT_DEVICE::set_equivalent_voltage_source_voltage_in_pu(complex<double> v)
{
    equivalent_voltage_source_voltage_in_pu = v;
}

void EQUIVALENT_DEVICE::set_equivalent_voltage_source_impedance_in_pu(complex<double> z)
{
    equivalent_voltage_source_impedance_in_pu = z;
}

void EQUIVALENT_DEVICE::set_equivalent_nominal_constant_power_load_in_MVA(complex<double> s)
{
    equivalent_load_s_constant_power_in_MVA = s;
}

void EQUIVALENT_DEVICE::set_equivalent_nominal_constant_current_load_in_MVA(complex<double> s)
{
    equivalent_load_s_constant_current_in_MVA = s;
}

void EQUIVALENT_DEVICE::set_equivalent_nominal_constant_impedance_load_in_MVA(complex<double> s)
{
    equivalent_load_s_constant_impedance_in_MVA = s;
}

size_t EQUIVALENT_DEVICE::get_equivalent_device_bus() const
{
    return bus;
}

string EQUIVALENT_DEVICE::get_identifier() const
{
    return identifier;
}

bool EQUIVALENT_DEVICE::get_status() const
{
    return status;
}

bool EQUIVALENT_DEVICE::get_equivalent_voltage_source_status() const
{
    return equivalent_voltage_source_status;
}

bool EQUIVALENT_DEVICE::get_equivalent_load_status() const
{
    return equivalent_load_status;
}

void EQUIVALENT_DEVICE::update_meter_buffer()
{
    EQUIVALENT_MODEL* model = get_equivalent_model();
    if(model!=NULL)
        model->update_meters_buffer();
}

void EQUIVALENT_DEVICE::update_equivalent_outputs()
{
    EQUIVALENT_MODEL* model = get_equivalent_model();
    if(model!=NULL)
        model->update_equivalent_outputs();
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_voltage_source_voltage_in_pu() const
{
    return equivalent_voltage_source_voltage_in_pu;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_voltage_source_impedance_in_pu() const
{
    return equivalent_voltage_source_impedance_in_pu;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_power_load_in_MVA() const
{
    return equivalent_load_s_constant_power_in_MVA;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_current_load_in_MVA() const
{
    return equivalent_load_s_constant_current_in_MVA;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_impedance_load_in_MVA() const
{
    return equivalent_load_s_constant_impedance_in_MVA;
}

complex<double> EQUIVALENT_DEVICE::get_total_equivalent_power_as_load_in_MVA() const
{
    if(get_status()==false)
        return 0.0;

    complex<double> S=0.0;
    if(get_equivalent_load_status()==true)
        S += get_equivalent_load_in_MVA();

    if(get_equivalent_voltage_source_status()==true)
        S -= get_equivalent_generation_in_MVA();

    return S;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_generation_in_MVA() const
{
    if(get_equivalent_voltage_source_status()==false or get_status()==false)
        return 0.0;

    complex<double> E = get_equivalent_voltage_source_voltage_in_pu();
    complex<double> Z = get_equivalent_voltage_source_impedance_in_pu();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    complex<double> V = psdb->get_bus_complex_voltage_in_pu(bus);

    complex<double> I = (E-V)/Z;

    return V*conj(I);
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_load_in_MVA() const
{
    if(get_equivalent_load_status()==false or get_status()==false)
        return 0.0;

    complex<double> SP = get_equivalent_nominal_constant_power_load_in_MVA();
    complex<double> SI = get_equivalent_nominal_constant_current_load_in_MVA();
    complex<double> SZ = get_equivalent_nominal_constant_impedance_load_in_MVA();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    double V = psdb->get_bus_voltage_in_pu(bus);

    return SP+SI*V+SZ*V*V;
}


complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_power_load_in_pu() const
{
    double sbase = get_power_system_database()->get_system_base_power_in_MVA();
    return equivalent_load_s_constant_power_in_MVA/sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_current_load_in_pu() const
{
    double sbase = get_power_system_database()->get_system_base_power_in_MVA();
    return equivalent_load_s_constant_current_in_MVA/sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_impedance_load_in_pu() const
{
    double sbase = get_power_system_database()->get_system_base_power_in_MVA();
    return equivalent_load_s_constant_impedance_in_MVA/sbase;
}

complex<double> EQUIVALENT_DEVICE::get_total_equivalent_power_as_load_in_pu() const
{
    if(get_status()==false)
        return 0.0;

    complex<double> S=0.0;
    if(get_equivalent_load_status()==true)
        S += get_equivalent_load_in_MVA();

    if(get_equivalent_voltage_source_status()==true)
        S -= get_equivalent_generation_in_MVA();

    double sbase = get_power_system_database()->get_system_base_power_in_MVA();

    return S/sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_generation_in_pu() const
{
    if(get_equivalent_voltage_source_status()==false or get_status()==false)
        return 0.0;

    complex<double> E = get_equivalent_voltage_source_voltage_in_pu();
    complex<double> Z = get_equivalent_voltage_source_impedance_in_pu();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    complex<double> V = psdb->get_bus_complex_voltage_in_pu(bus);

    complex<double> I = (E-V)/Z;

    double sbase = psdb->get_system_base_power_in_MVA();

    return V*conj(I)/sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_load_in_pu() const
{
    if(get_equivalent_load_status()==false or get_status()==false)
        return 0.0;

    complex<double> SP = get_equivalent_nominal_constant_power_load_in_MVA();
    complex<double> SI = get_equivalent_nominal_constant_current_load_in_MVA();
    complex<double> SZ = get_equivalent_nominal_constant_impedance_load_in_MVA();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    double V = psdb->get_bus_voltage_in_pu(bus);

    double sbase = psdb->get_system_base_power_in_MVA();

    return (SP+SI*V+SZ*V*V)/sbase;
}

void EQUIVALENT_DEVICE::switch_on()
{
    EQUIVALENT_MODEL* model = get_equivalent_model();
    if(model!=NULL)
    {
        ostringstream sstream;
        sstream<<"Equivalent model is found :"<<model->get_model_name();
        show_information_with_leading_time_stamp(sstream);
        model->switch_output_to_equivalent_device();
    }
}

bool EQUIVALENT_DEVICE::is_valid() const
{
    if(get_equivalent_device_bus()!=0)
        return true;
    else
        return false;
}

void EQUIVALENT_DEVICE::check()
{
    ;
}

void EQUIVALENT_DEVICE::clear()
{
    bus = 0;
    set_identifier("");
    set_status(false);
    set_equivalent_voltage_source_status(false);
    set_equivalent_load_status(false);
    set_equivalent_voltage_source_voltage_in_pu(0.0);
    set_equivalent_voltage_source_impedance_in_pu(0.0);
    set_equivalent_nominal_constant_power_load_in_MVA(0.0);
    set_equivalent_nominal_constant_current_load_in_MVA(0.0);
    set_equivalent_nominal_constant_impedance_load_in_MVA(0.0);
}

bool EQUIVALENT_DEVICE::is_connected_to_bus(size_t target_bus) const
{
    if(target_bus==get_equivalent_device_bus()) return true;
    else return false;
}

bool EQUIVALENT_DEVICE::is_in_area(size_t area) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr = psdb->get_bus(get_equivalent_device_bus());
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

bool EQUIVALENT_DEVICE::is_in_zone(size_t zone) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
    {
        BUS* busptr = psdb->get_bus(get_equivalent_device_bus());
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

void EQUIVALENT_DEVICE::report() const
{
    ostringstream sstream;
    sstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<endl
      <<"equivalent voltage source: "<<(get_equivalent_voltage_source_status()==true?"in service":"out of service")<<", "
      <<"V = "<<setw(8)<<setprecision(6)<<fixed<<abs(get_equivalent_voltage_source_voltage_in_pu())<<" pu, "
      <<"Angle = "<<setw(8)<<setprecision(6)<<fixed<<rad2deg(arg(get_equivalent_voltage_source_voltage_in_pu()))<<" deg, "
      <<"Z = "<<setw(8)<<setprecision(6)<<fixed<<get_equivalent_voltage_source_impedance_in_pu()<<" pu"<<endl
      <<"equivalent load: "<<(get_equivalent_load_status()==true?"in service":"out of service")<<", "
      <<"P+jQ[P part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_power_load_in_MVA()<<"MVA, "
      <<"P+jQ[I part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_current_load_in_MVA()<<"MVA, "
      <<"P+jQ[Z part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_impedance_load_in_MVA()<<"MVA.";
    show_information_with_leading_time_stamp(sstream);
}

void EQUIVALENT_DEVICE::save() const
{
    ;
}


EQUIVALENT_DEVICE& EQUIVALENT_DEVICE::operator=(const EQUIVALENT_DEVICE& device)
{
    if(this==(&device)) return *this;

    clear();
    set_power_system_database(device.get_power_system_database());
    set_equivalent_device_bus(device.get_equivalent_device_bus());
    set_identifier(device.get_identifier());
    set_status(device.get_status());
    set_equivalent_voltage_source_status(device.get_equivalent_voltage_source_status());
    set_equivalent_load_status(device.get_equivalent_load_status());
    set_equivalent_voltage_source_voltage_in_pu(device.get_equivalent_voltage_source_voltage_in_pu());
    set_equivalent_voltage_source_impedance_in_pu(device.get_equivalent_voltage_source_impedance_in_pu());
    set_equivalent_nominal_constant_power_load_in_MVA(device.get_equivalent_nominal_constant_power_load_in_MVA());
    set_equivalent_nominal_constant_current_load_in_MVA(device.get_equivalent_nominal_constant_current_load_in_MVA());
    set_equivalent_nominal_constant_impedance_load_in_MVA(device.get_equivalent_nominal_constant_impedance_load_in_MVA());

    return *this;
}

DEVICE_ID EQUIVALENT_DEVICE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");

    TERMINAL terminal;
    terminal.append_bus(get_equivalent_device_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string EQUIVALENT_DEVICE::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

void EQUIVALENT_DEVICE::set_model(const MODEL* model)
{
    if(model->get_allowed_device_type()!="EQUIVALENT DEVICE")
        return;

    if(model->get_model_type()=="EQUIVALENT MODEL")
    {
        set_equivalent_model((EQUIVALENT_MODEL*) model);
        return;
    }
}

void EQUIVALENT_DEVICE::set_equivalent_model(const EQUIVALENT_MODEL* model)
{
    if(model->get_model_type()!="EQUIVALENT MODEL")
        return;

    EQUIVALENT_MODEL* oldmodel = get_equivalent_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        equivalent_model = NULL;
    }

    EQUIVALENT_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="ARXL")
    {
        ARXL* smodel = (ARXL*) (model);
        new_model = (EQUIVALENT_MODEL*) new ARXL(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        equivalent_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append equivalent model of "<<get_device_name();
        show_information_with_leading_time_stamp(sstream);
    }
}


EQUIVALENT_MODEL* EQUIVALENT_DEVICE::get_equivalent_model() const
{
    return equivalent_model;
}

void EQUIVALENT_DEVICE::run(DYNAMIC_MODE mode)
{
    switch(mode)
    {
        case INITIALIZE_MODE:
        {
            EQUIVALENT_MODEL* model = get_equivalent_model();
            if(model==NULL)
                return;
            model->initialize();

            break;
        }
        case INTEGRATE_MODE:
        {
            EQUIVALENT_MODEL* model = get_equivalent_model();
            if(model!=NULL)
            {
                model->run(mode);

                set_equivalent_voltage_source_voltage_in_pu(model->get_equivalent_voltage_source_voltage_in_pu());
                set_equivalent_voltage_source_impedance_in_pu(model->get_equivalent_voltage_source_impedance_in_pu());

                set_equivalent_nominal_constant_power_load_in_MVA(model->get_equivalent_nominal_constant_power_load_in_MVA());
                set_equivalent_nominal_constant_current_load_in_MVA(model->get_equivalent_nominal_constant_current_load_in_MVA());
                set_equivalent_nominal_constant_impedance_load_in_MVA(model->get_equivalent_nominal_constant_impedance_load_in_MVA());
            }
        }
        case UPDATE_MODE:
        {
            EQUIVALENT_MODEL* model = get_equivalent_model();
            if(model!=NULL)
            {
                model->run(mode);

                set_equivalent_voltage_source_voltage_in_pu(model->get_equivalent_voltage_source_voltage_in_pu());
                set_equivalent_voltage_source_impedance_in_pu(model->get_equivalent_voltage_source_impedance_in_pu());

                set_equivalent_nominal_constant_power_load_in_MVA(model->get_equivalent_nominal_constant_power_load_in_MVA());
                set_equivalent_nominal_constant_current_load_in_MVA(model->get_equivalent_nominal_constant_current_load_in_MVA());
                set_equivalent_nominal_constant_impedance_load_in_MVA(model->get_equivalent_nominal_constant_impedance_load_in_MVA());
            }
            break;
        }
        default:
            break;
    }
}


