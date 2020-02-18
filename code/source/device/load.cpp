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
double LOAD::voltage_threshold_of_constant_current_load_in_pu = 0.5;

LOAD::LOAD(STEPS& toolkit) : DEVICE(toolkit)
{
    clear();
}

LOAD::~LOAD()
{
}

void LOAD::set_load_bus(unsigned int load_bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

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

        busptr = psdb.get_bus(load_bus);
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
    this->identifier = trim_string(load_id);
}

void LOAD::set_status(bool status)
{
    this->status = status;
}

void LOAD::set_nominal_constant_power_load_in_MVA(const complex<double>& s)
{
    s_constant_power_in_MVA = s;
}

void LOAD::set_nominal_constant_current_load_in_MVA(const complex<double>& s)
{
    s_constant_current_in_MVA = s;
}

void LOAD::set_nominal_constant_impedance_load_in_MVA(const complex<double>& s)
{
    s_constant_impedance_in_MVA = s;
}

void LOAD::set_area_number(unsigned int num)
{
    area_number = num;
}

void LOAD::set_zone_number(unsigned int num)
{
    zone_number = num;
}

void LOAD::set_owner_number(unsigned int num)
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

unsigned int LOAD::get_load_bus() const
{
    return bus;
}

BUS* LOAD::get_bus_pointer() const
{
    return busptr;
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

unsigned int LOAD::get_area_number() const
{
    return area_number;
}

unsigned int LOAD::get_zone_number() const
{
    return zone_number;
}

unsigned int LOAD::get_owner_number() const
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
    busptr = NULL;
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

    load_model = NULL;
    load_voltage_relay_model = NULL;
    load_frequency_relay_model = NULL;
}

bool LOAD::is_connected_to_bus(unsigned int target_bus) const
{
    if(target_bus==get_load_bus()) return true;
    else return false;
}

bool LOAD::is_in_area(unsigned int area) const
{
    return get_area_number()==area;
}

bool LOAD::is_in_zone(unsigned int zone) const
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
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void LOAD::save() const
{
    ;
}


LOAD& LOAD::operator=(const LOAD& load)
{
    if(this==(&load)) return *this;

    set_toolkit(load.get_toolkit());
    clear();

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

        double v = busptr->get_positive_sequence_voltage_in_pu();

        return S0*get_load_scale_with_voltage(0.0, v, LOAD_ELLIPTICAL_CV);
    }
    else
        return 0.0;
}

complex<double> LOAD::get_actual_constant_current_load_in_MVA() const
{
    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_constant_current_load_in_MVA();
        double v = busptr->get_positive_sequence_voltage_in_pu();

        return S0*get_load_scale_with_voltage(1.0, v, LOAD_ELLIPTICAL_CV);
    }
    else
        return 0.0;
}

complex<double> LOAD::get_actual_constant_impedance_load_in_MVA() const
{
    if(get_status() == true)
    {
        complex<double> S0 = get_nominal_constant_impedance_load_in_MVA();
        double v = busptr->get_positive_sequence_voltage_in_pu();
        complex<double> s = S0*v*v;
        return s;
    }
    else
        return 0.0;
}

double LOAD::get_load_scale_with_voltage(double exp, double v, LOAD_CURRENT_VOLTAGE_REDUCE_TYPE cv_type)
{
    if(exp>=2.0)// higher than constant impedance
        return pow(v, exp);
    else
    {
        if(exp==0.0) // constant power
        {
            double vth = get_voltage_threshold_of_constant_power_load_in_pu();
            if(v>=vth)
                return 1.0;
            else
            {
                double Imax = 1.0/vth;
                return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
            }
        }
        else // otherwise, including constant current
        {
            double vth = get_voltage_threshold_of_constant_current_load_in_pu();
            if(v>=vth)
                return pow(v, exp);
            else
            {
                double Imax = 1.0/pow(vth, exp-1.0);
                return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
            }
        }
    }
}

double LOAD::get_load_scale_with_Imax_and_voltage(double Imax, double v, double vth, LOAD_CURRENT_VOLTAGE_REDUCE_TYPE cv_type)
{
    switch(cv_type)
    {
        case LOAD_ELLIPTICAL_CV:
        {
            // (v-vth)^2/vth^2+I^2/Imax^2=1
            //double vscale = v/vth-1.0;
            double vscale = v/vth-1.0;

            double I = steps_sqrt(1.0-vscale*vscale)*Imax;
            return v*I;
        }
        case LOAD_LINEAR_CV:
        {
            double I = Imax*v/vth;
            return v*I;
        }
        default: // LOAD_CONSTANT_CV
        {
            return v*Imax;
        }
    }
}

void LOAD::set_voltage_threshold_of_constant_power_load_in_pu(double v)
{
    if(v>0.0)
        voltage_threshold_of_constant_power_load_in_pu = v;
}

void LOAD::set_voltage_threshold_of_constant_current_load_in_pu(double v)
{
    if(v>0.0)
        voltage_threshold_of_constant_current_load_in_pu = v;
}

double LOAD::get_voltage_threshold_of_constant_power_load_in_pu()
{
    return voltage_threshold_of_constant_power_load_in_pu;
}

double LOAD::get_voltage_threshold_of_constant_current_load_in_pu()
{
    return voltage_threshold_of_constant_current_load_in_pu;
}

void LOAD::set_model(const MODEL* model)
{
    if(model->has_allowed_device_type("LOAD"))
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

MODEL* LOAD::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="LOAD CHARACTERISTICS")
        return get_load_model();
    if(model_type=="LOAD VOLTAGE RELAY")
        return get_load_voltage_relay_model();
    if(model_type=="LOAD FREQUENCY RELAY")
        return get_load_frequency_relay_model();
    return nullptr;
}

void LOAD::set_load_model(LOAD_MODEL* model)
{
    if(model!=NULL)
        load_model = model;
}

void LOAD::set_load_frequency_relay_model(LOAD_FREQUENCY_RELAY_MODEL* model)
{
    if(model != NULL)
        load_frequency_relay_model = model;
}

void LOAD::set_load_voltage_relay_model(LOAD_VOLTAGE_RELAY_MODEL* model)
{
    if(model != NULL)
        load_voltage_relay_model = model;
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
                set_load_manually_scale_factor_in_pu(0.0);

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
                if(uvls!=NULL and uvls->is_model_active())
                    uvls->run(mode);

                LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
                if(ufls!=NULL and ufls->is_model_active())
                    ufls->run(mode);

                LOAD_MODEL* load = get_load_model();
                if(load!=NULL and load->is_model_active())
                    load->run(mode);
                break;
            }
            case RELAY_MODE:
            {
                LOAD_VOLTAGE_RELAY_MODEL* uvls = get_load_voltage_relay_model();
                if(uvls!=NULL and uvls->is_model_active())
                    uvls->run(mode);

                LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
                if(ufls!=NULL and ufls->is_model_active())
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
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return get_dynamic_load_in_MVA()*one_over_sbase;
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
        double scale = 1.0+get_load_total_scale_factor_in_pu();

        LOAD_MODEL* load_model = get_load_model();
        if(load_model!=NULL)
            return scale*(load_model->get_load_current_in_pu_based_on_SBASE());
        else
        {
            STEPS& toolkit = get_toolkit();
            double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
            complex<double> V = get_bus_pointer()->get_positive_sequence_complex_voltage_in_pu();
            return (scale*one_over_sbase)*conj(get_actual_total_load_in_MVA()/V);
        }
        /*STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        complex<double> S = get_dynamic_load_in_pu();

        complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_load_bus());

        return conj(S/V);*/
    }
    else
        return 0.0;
}

complex<double> LOAD::get_dynamics_load_norton_current_in_pu_based_on_system_base_power()
{
    LOAD_MODEL* load_model = get_load_model();
    if(get_status()==true and load_model!=NULL)
    {
        double scale = 1.0+get_load_total_scale_factor_in_pu();
        return scale*load_model->get_norton_current_in_pu_based_on_SBASE();
    }
    else
        return 0.0;
}
