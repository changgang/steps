#include "header/device/load.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include "header/model/load_model/load_models.h"
#include "header/model/load_relay_model/load_frequency_relay_models.h"
#include "header/model/load_relay_model/load_voltage_relay_models.h"
#include <istream>
#include <iostream>

using namespace std;

double LOAD::voltage_threshold_of_constant_power_load_in_pu = 0.7;

LOAD::LOAD()
{
    clear();

    load_model = NULL;
    load_voltage_relay_model = NULL;
    load_frequency_relay_model = NULL;
}

LOAD::~LOAD()
{
    clear();

    if(load_model != NULL)
        delete load_model;

    if(load_voltage_relay_model != NULL)
        delete load_voltage_relay_model;

    if(load_frequency_relay_model != NULL)
        delete load_frequency_relay_model;
}

void LOAD::set_load_bus(size_t load_bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    if(load_bus!=0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(not psdb.is_bus_exist(load_bus))
        {
            osstream<<"Bus "<<load_bus<<" does not exist for setting up load."<<endl
              <<"0 will be set to indicate invalid load.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            this->bus = 0;
            return;
        }
        this->bus = load_bus;
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up load bus."<<endl
          <<"0 will be set to indicate invalid load.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bus = load_bus;
        return;
    }
}

void LOAD::set_identifier(string load_id)
{
    this->identifier = load_id;
}

void LOAD::set_status(bool status)
{
    this->status = status;
}

void LOAD::set_nominal_constant_power_load_in_MVA(complex<double> s)
{
    s_constant_power_in_MVA = s;
}

void LOAD::set_nominal_constant_current_load_in_MVA(complex<double> s)
{
    s_constant_current_in_MVA = s;
}

void LOAD::set_nominal_constant_impedance_load_in_MVA(complex<double> s)
{
    s_constant_impedance_in_MVA = s;
}

void LOAD::set_area_number(size_t num)
{
    area_number = num;
}

void LOAD::set_zone_number(size_t num)
{
    zone_number = num;
}

void LOAD::set_owner_number(size_t num)
{
    owner_number = num;
}

void LOAD::set_flag_interruptable(bool flag)
{
    interruptable = flag;
}

void LOAD::set_load_manually_scale_factor_in_pu(double scale)
{
    manually_scale_in_pu = scale;
}

size_t LOAD::get_load_bus() const
{
    return bus;
}

string LOAD::get_identifier() const
{
    return identifier;
}

bool LOAD::get_status() const
{
    return status;
}

complex<double> LOAD::get_nominal_constant_power_load_in_MVA() const
{
    return s_constant_power_in_MVA;
}

complex<double> LOAD::get_nominal_constant_current_load_in_MVA() const
{
    return s_constant_current_in_MVA;
}

complex<double> LOAD::get_nominal_constant_impedance_load_in_MVA() const
{
    return s_constant_impedance_in_MVA;
}

size_t LOAD::get_area_number() const
{
    return area_number;
}

size_t LOAD::get_zone_number() const
{
    return zone_number;
}

size_t LOAD::get_owner_number() const
{
    return owner_number;
}

bool LOAD::get_flag_interruptable() const
{
    return interruptable;
}

bool LOAD::is_valid() const
{
    if(get_load_bus()!=0)
        return true;
    else
        return false;
}

void LOAD::check()
{
    ;
}

void LOAD::clear()
{
    bus = 0;
    set_identifier("");
    set_status(false);
    set_nominal_constant_power_load_in_MVA(0.0);
    set_nominal_constant_current_load_in_MVA(0.0);
    set_nominal_constant_impedance_load_in_MVA(0.0);
    set_area_number(0);
    set_zone_number(0);
    set_owner_number(0);
    set_flag_interruptable(false);
    set_load_manually_scale_factor_in_pu(0.0);
}

bool LOAD::is_connected_to_bus(size_t target_bus) const
{
    if(target_bus==get_load_bus()) return true;
    else return false;
}

bool LOAD::is_in_area(size_t area) const
{
    return get_area_number()==area;
}

bool LOAD::is_in_zone(size_t zone) const
{
    return get_zone_number()==zone;
}

void LOAD::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"P+jQ[P part] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_constant_power_load_in_MVA()<<" MVA, "
      <<"P+jQ[I part] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_constant_current_load_in_MVA()<<" MVA, "
      <<"P+jQ[Z part] = "<<setw(6)<<setprecision(2)<<fixed<<get_nominal_constant_impedance_load_in_MVA()<<" MVA.";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void LOAD::save() const
{
    ;
}


LOAD& LOAD::operator=(const LOAD& load)
{
    if(this==(&load)) return *this;

    clear();

    set_toolkit(load.get_toolkit(__PRETTY_FUNCTION__));
    set_load_bus(load.get_load_bus());
    set_identifier(load.get_identifier());
    set_status(load.get_status());
    set_nominal_constant_power_load_in_MVA(load.get_nominal_constant_power_load_in_MVA());
    set_nominal_constant_current_load_in_MVA(load.get_nominal_constant_current_load_in_MVA());
    set_nominal_constant_impedance_load_in_MVA(load.get_nominal_constant_impedance_load_in_MVA());
    set_area_number(load.get_area_number());
    set_zone_number(load.get_zone_number());
    set_owner_number(load.get_owner_number());

    return *this;
}

DEVICE_ID LOAD::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("LOAD");

    TERMINAL terminal;
    terminal.append_bus(get_load_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string LOAD::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

complex<double> LOAD::get_nominal_total_load_in_MVA() const
{
    return get_nominal_constant_power_load_in_MVA()+get_nominal_constant_current_load_in_MVA()+
           get_nominal_constant_impedance_load_in_MVA();
}

complex<double> LOAD::get_actual_total_load_in_MVA() const
{
    return get_actual_constant_power_load_in_MVA()+get_actual_constant_current_load_in_MVA()+
           get_actual_constant_impedance_load_in_MVA();
}

complex<double> LOAD::get_actual_constant_power_load_in_MVA() const
{
    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_constant_power_load_in_MVA();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        BUS* busptr = psdb.get_bus(get_load_bus());
        if(busptr!=NULL)
        {
            double v = busptr->get_voltage_in_pu();

            complex<double> s;
            if(v>=get_voltage_threshold_of_constant_power_load_in_pu())
                s = S0;
            else
                s = S0/get_voltage_threshold_of_constant_power_load_in_pu()*v;

            return s;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Bus "<<get_load_bus()<<" is not found in power system database '"<<psdb.get_system_name()<<"'."<<endl
              <<get_device_name()<<" actual constant power load will be returned as it nominal power.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return S0;
        }
    }
    else
        return 0.0;
}

complex<double> LOAD::get_actual_constant_current_load_in_MVA() const
{
    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_constant_current_load_in_MVA();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        BUS* busptr = psdb.get_bus(get_load_bus());
        if(busptr!=NULL)
        {
            double v = busptr->get_voltage_in_pu();

            complex<double> s = S0*v;
            return s;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Bus "<<get_load_bus()<<" is not found in power system database '"<<psdb.get_system_name()<<"'."<<endl
              <<get_device_name()<<" actual constant current load will be returned as it nominal power.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return S0;
        }
    }
    else
        return 0.0;
}

complex<double> LOAD::get_actual_constant_impedance_load_in_MVA() const
{
    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_constant_impedance_load_in_MVA();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        BUS* busptr = psdb.get_bus(get_load_bus());
        if(busptr!=NULL)
        {
            double v = busptr->get_voltage_in_pu();

            complex<double> s = S0*v*v;
            return s;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Bus "<<get_load_bus()<<" is not found in power system database '"<<psdb.get_system_name()<<"'."<<endl
              <<get_device_name()<<" actual constant impedance load will be returned as it nominal power.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return S0;
        }
    }
    else
        return 0.0;
}


void LOAD::set_voltage_threshold_of_constant_power_load_in_pu(double v)
{
    if(v>0.0)
        voltage_threshold_of_constant_power_load_in_pu = v;
}

double LOAD::get_voltage_threshold_of_constant_power_load_in_pu()
{
    return voltage_threshold_of_constant_power_load_in_pu;
}


void LOAD::set_model(const MODEL* model)
{
    if(model->get_allowed_device_type()=="LOAD")
    {
        if(model->get_model_type()=="LOAD CHARACTERISTICS")
        {
            set_load_model((LOAD_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="LOAD VOLTAGE RELAY")
        {
            set_load_voltage_relay_model((LOAD_VOLTAGE_RELAY_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="LOAD FREQUENCY RELAY")
        {
            set_load_frequency_relay_model((LOAD_FREQUENCY_RELAY_MODEL*) model);
            return;
        }
    }
}

void LOAD::set_load_model(const LOAD_MODEL* model)
{
    if(model!=NULL and model->get_model_type()=="LOAD CHARACTERISTICS")
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

        LOAD_MODEL* oldmodel = get_load_model();
        if(oldmodel!=NULL and oldmodel->get_subsystem_type()>=model->get_subsystem_type())
        {
            delete oldmodel;
            load_model = NULL;
        }

        LOAD_MODEL *new_model = NULL;
        string model_name = model->get_model_name();
        if(model_name=="IEEL")
        {
            IEEL* smodel = (IEEL*) (model);
            new_model = (LOAD_MODEL*) new IEEL(*smodel);
        }

        if(new_model!=NULL)
        {
            new_model->set_toolkit(toolkit);
            new_model->set_device_id(get_device_id());
            load_model = new_model;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Model '"<<model_name<<"' is not supported when append load model of "<<get_device_name()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void LOAD::set_load_frequency_relay_model(const LOAD_FREQUENCY_RELAY_MODEL* model)
{
    if(model != NULL and model->get_model_type()=="LOAD FREQUENCY RELAY")
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        LOAD_FREQUENCY_RELAY_MODEL* oldmodel = get_load_frequency_relay_model();
        if(oldmodel!=NULL and oldmodel->get_subsystem_type()>=model->get_subsystem_type())
        {
            delete oldmodel;
            load_frequency_relay_model = NULL;
        }

        LOAD_FREQUENCY_RELAY_MODEL *new_model = NULL;
        string model_name = model->get_model_name();
        if(model_name=="UFLS")
        {
            UFLS* smodel = (UFLS*) (model);
            new_model = (LOAD_FREQUENCY_RELAY_MODEL*) new UFLS(*smodel);
        }
        if(model_name=="PUFLS")
        {
            PUFLS* smodel = (PUFLS*) (model);
            new_model = (LOAD_FREQUENCY_RELAY_MODEL*) new PUFLS(*smodel);
        }

        if(new_model!=NULL)
        {
            new_model->set_toolkit(toolkit);
            new_model->set_device_id(get_device_id());
            load_frequency_relay_model = new_model;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Model '"<<model_name<<"' is not supported when append load frequency relay model of "<<get_device_name()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void LOAD::set_load_voltage_relay_model(const LOAD_VOLTAGE_RELAY_MODEL* model)
{
    if(model != NULL and model->get_model_type()=="LOAD VOLTAGE RELAY")
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        LOAD_VOLTAGE_RELAY_MODEL* oldmodel = get_load_voltage_relay_model();
        if(oldmodel!=NULL and oldmodel->get_subsystem_type()>=model->get_subsystem_type())
        {
            delete oldmodel;
            load_voltage_relay_model = NULL;
        }

        LOAD_VOLTAGE_RELAY_MODEL *new_model = NULL;
        string model_name = model->get_model_name();
        if(model_name=="UVLS")
        {
            UVLS* smodel = (UVLS*) (model);
            new_model = (LOAD_VOLTAGE_RELAY_MODEL*) new UVLS(*smodel);
        }

        if(new_model!=NULL)
        {
            new_model->set_toolkit(toolkit);
            new_model->set_device_id(get_device_id());
            load_voltage_relay_model = new_model;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Warning. Model '"<<model_name<<"' is not supported when append load voltage relay model of "<<get_device_name()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


LOAD_MODEL* LOAD::get_load_model() const
{
    return load_model;
}

LOAD_FREQUENCY_RELAY_MODEL* LOAD::get_load_frequency_relay_model() const
{
    return load_frequency_relay_model;
}

LOAD_VOLTAGE_RELAY_MODEL* LOAD::get_load_voltage_relay_model() const
{
    return load_voltage_relay_model;
}

void LOAD::run(DYNAMIC_MODE mode)
{
    if(get_status()==true)
    {
        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                LOAD_MODEL* load = get_load_model();
                if(load != NULL)
                {
                    load->initialize();

                    LOAD_VOLTAGE_RELAY_MODEL* uvls = get_load_voltage_relay_model();
                    if(uvls!=NULL)
                        uvls->initialize();

                    LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
                    if(ufls!=NULL)
                        ufls->initialize();
                }
                break;
            }
            case INTEGRATE_MODE:
            case UPDATE_MODE:
            {
                LOAD_VOLTAGE_RELAY_MODEL* uvls = get_load_voltage_relay_model();
                if(uvls!=NULL)
                    uvls->run(mode);

                LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
                if(ufls!=NULL)
                    ufls->run(mode);

                LOAD_MODEL* load = get_load_model();
                if(load!=NULL)
                    load->run(mode);
                break;
            }
            case RELAY_MODE:
            {
                LOAD_VOLTAGE_RELAY_MODEL* uvls = get_load_voltage_relay_model();
                if(uvls!=NULL)
                    uvls->run(mode);

                LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
                if(ufls!=NULL)
                    ufls->run(mode);
            }
        }
    }
}

complex<double> LOAD::get_dynamic_load_in_MVA()
{
    if(get_status()==true)
    {
        double scale = 1.0+get_load_total_scale_factor_in_pu();

        LOAD_MODEL* load_model = get_load_model();
        if(load_model!=NULL)
            return scale*(load_model->get_load_power_in_MVA());
        else
            return scale*get_actual_total_load_in_MVA();
    }
    else
        return 0.0;
}

complex<double> LOAD::get_dynamic_load_in_pu()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double sbase = toolkit.get_power_system_database().get_system_base_power_in_MVA();

    return get_dynamic_load_in_MVA()/sbase;
}

double LOAD::get_load_total_scale_factor_in_pu() const
{
    double scale = get_load_manually_scale_factor_in_pu()-get_load_relay_shed_scale_factor_in_pu();
    if(scale<-1.0)
        scale = -1.0;
    return scale;
}

double LOAD::get_load_manually_scale_factor_in_pu() const
{
    return manually_scale_in_pu;
}

double LOAD::get_load_relay_shed_scale_factor_in_pu() const
{
    if(get_status()==true)
    {
        LOAD_FREQUENCY_RELAY_MODEL* frelay = get_load_frequency_relay_model();
        LOAD_VOLTAGE_RELAY_MODEL* vrelay = get_load_voltage_relay_model();

        double total_shed = 0.0;

        if(frelay!=NULL)
            total_shed += frelay->get_total_shed_scale_factor_in_pu();

        if(vrelay!=NULL)
            total_shed += vrelay->get_total_shed_scale_factor_in_pu();

        return total_shed;
    }
    else
        return 0.0;
}


complex<double> LOAD::get_dynamics_load_current_in_pu_based_on_system_base_power()
{
    if(get_status()==true)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        complex<double> S = get_dynamic_load_in_MVA()/psdb.get_system_base_power_in_MVA();

        complex<double> V = psdb.get_bus_complex_voltage_in_pu(get_load_bus());

        return conj(S/V);
    }
    else
        return 0.0;
}



