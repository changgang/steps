#include "header/device/device.h"
#include "header/basic/utility.h"
#include "header/model/equivalent_model/equivalent_model.h"
#include "header/model/equivalent_model/equivalent_models.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

EQUIVALENT_DEVICE::EQUIVALENT_DEVICE()
{
    clear();

    equivalent_model = NULL;
}

EQUIVALENT_DEVICE::~EQUIVALENT_DEVICE()
{
    if(equivalent_model != NULL) delete equivalent_model;
}

void EQUIVALENT_DEVICE::set_equivalent_device_bus(size_t device_bus)
{
    ostringstream osstream;

    if(device_bus==0)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up equivalent device bus."<<endl
          <<"0 will be set to indicate invalid equivalent device.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bus = device_bus;
        return;
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(not psdb.is_bus_exist(device_bus))
    {
        osstream<<"Bus "<<device_bus<<" does not exist for setting up equivalent device."<<endl
          <<"0 will be set to indicate invalid equivalent device.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bus = 0;
        return;
    }
    this->bus = device_bus;
}

void EQUIVALENT_DEVICE::set_identifier(string identifier)
{
    this->identifier = trim_string(identifier);
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

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(bus);

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

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    double V = psdb.get_bus_positive_sequence_voltage_in_pu(bus);

    return SP+SI*V+SZ*V*V;
}


complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_power_load_in_pu() const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return equivalent_load_s_constant_power_in_MVA*one_over_sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_current_load_in_pu() const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return equivalent_load_s_constant_current_in_MVA*one_over_sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_nominal_constant_impedance_load_in_pu() const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return equivalent_load_s_constant_impedance_in_MVA*one_over_sbase;
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

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return S*one_over_sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_generation_in_pu() const
{
    if(get_equivalent_voltage_source_status()==false or get_status()==false)
        return 0.0;

    complex<double> E = get_equivalent_voltage_source_voltage_in_pu();
    complex<double> Z = get_equivalent_voltage_source_impedance_in_pu();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(bus);

    complex<double> I = (E-V)/Z;

    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return V*conj(I)*one_over_sbase;
}

complex<double> EQUIVALENT_DEVICE::get_equivalent_load_in_pu() const
{
    if(get_equivalent_load_status()==false or get_status()==false)
        return 0.0;

    complex<double> SP = get_equivalent_nominal_constant_power_load_in_MVA();
    complex<double> SI = get_equivalent_nominal_constant_current_load_in_MVA();
    complex<double> SZ = get_equivalent_nominal_constant_impedance_load_in_MVA();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus = get_equivalent_device_bus();
    double V = psdb.get_bus_positive_sequence_voltage_in_pu(bus);

    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return (SP+SI*V+SZ*V*V)*one_over_sbase;
}

void EQUIVALENT_DEVICE::switch_on()
{
    EQUIVALENT_MODEL* model = get_equivalent_model();
    if(model!=NULL)
    {
        ostringstream osstream;
        osstream<<"Equivalent model is found :"<<model->get_model_name();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(get_equivalent_device_bus());
    if(busptr!=NULL)
        return busptr->is_in_area(area);
    else
        return false;
}

bool EQUIVALENT_DEVICE::is_in_zone(size_t zone) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(get_equivalent_device_bus());
    if(busptr!=NULL)
        return busptr->is_in_zone(zone);
    else
        return false;
}

void EQUIVALENT_DEVICE::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<endl
            <<"equivalent voltage source: "<<(get_equivalent_voltage_source_status()==true?"in service":"out of service")<<", "
            <<"V = "<<setw(8)<<setprecision(6)<<fixed<< steps_fast_complex_abs(get_equivalent_voltage_source_voltage_in_pu())<<" pu, "
            <<"Angle = "<<setw(8)<<setprecision(6)<<fixed<<rad2deg(steps_fast_complex_arg(get_equivalent_voltage_source_voltage_in_pu()))<<" deg, "
            <<"Z = "<<setw(8)<<setprecision(6)<<fixed<<get_equivalent_voltage_source_impedance_in_pu()<<" pu"<<endl
            <<"equivalent load: "<<(get_equivalent_load_status()==true?"in service":"out of service")<<", "
            <<"P+jQ[P part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_power_load_in_MVA()<<"MVA, "
            <<"P+jQ[I part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_current_load_in_MVA()<<"MVA, "
            <<"P+jQ[Z part] = "<<setw(6)<<setprecision(2)<<fixed<<get_equivalent_nominal_constant_impedance_load_in_MVA()<<"MVA.";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void EQUIVALENT_DEVICE::save() const
{
    ;
}


EQUIVALENT_DEVICE& EQUIVALENT_DEVICE::operator=(const EQUIVALENT_DEVICE& device)
{
    if(this==(&device)) return *this;

    clear();

    set_toolkit(device.get_toolkit(__PRETTY_FUNCTION__));

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

void EQUIVALENT_DEVICE::set_model(const MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type("EQUIVALENT DEVICE"))
    {
        if(model->get_model_type()=="EQUIVALENT MODEL")
        {
            set_equivalent_model((EQUIVALENT_MODEL*) model);
            return;
        }
    }
}

void EQUIVALENT_DEVICE::set_equivalent_model(const EQUIVALENT_MODEL* model)
{
    if(model!=NULL and model->get_model_type()=="EQUIVALENT MODEL")
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

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
            new_model->set_toolkit(toolkit);
            new_model->set_device_id(get_device_id());
            equivalent_model = new_model;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Model '"<<model_name<<"' is not supported when append equivalent model of "<<get_device_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
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
            if(model!=NULL)
                model->initialize();
            else
                return;

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


