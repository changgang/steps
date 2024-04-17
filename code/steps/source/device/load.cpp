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
double LOAD::one_over_voltage_threshold_of_constant_power_load_in_pu = 1.0/0.7;
double LOAD::one_over_voltage_threshold_of_constant_current_load_in_pu = 2.0;

LOAD::LOAD(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
    Aptr = NULL;
    Bptr = NULL;
    Cptr = NULL;
    Dptr = NULL;
}

LOAD::~LOAD()
{
    if(Aptr != NULL)
    {
        delete Aptr;
        Aptr = NULL;
    }
    if(Bptr != NULL)
    {
        delete Bptr;
        Bptr = NULL;
    }
    if(Cptr != NULL)
    {
        delete Cptr;
        Cptr = NULL;
    }
    if(Dptr != NULL)
    {
        delete Dptr;
        Dptr = NULL;
    }
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
    load_id = trim_string(load_id);
    add_string_to_str_int_map(load_id);
    this->identifier_index = get_index_of_string(load_id);
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


void LOAD::set_negative_sequence_load_in_MVA(const complex<double>& s)
{
    s_negative_sequence_in_MVA = s;
}

void LOAD::set_zero_sequence_load_in_MVA(const complex<double>& s)
{
    s_zero_sequence_in_MVA = s;
}

void LOAD::set_grounding_flag(const bool flag)
{
    grounding_flag = flag;
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
    return get_string_of_index(identifier_index);
}

unsigned int LOAD::get_identifier_index() const
{
    return identifier_index;
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

complex<double> LOAD::get_negative_sequence_load_in_MVA() const
{
    return s_negative_sequence_in_MVA;
}

complex<double> LOAD::get_zero_sequence_load_in_MVA() const
{
    return s_zero_sequence_in_MVA;
}

bool LOAD::get_grounding_flag() const
{
    return grounding_flag;
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

    s_negative_sequence_in_MVA = 0.0;
    s_zero_sequence_in_MVA = 0.0;

    set_grounding_flag(false);
    set_sequence_parameter_import_flag(false);
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
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
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

    set_model(load.get_load_model());
    set_model(load.get_load_voltage_relay_model());
    set_model(load.get_load_frequency_relay_model());

    set_negative_sequence_load_in_MVA(load.get_negative_sequence_load_in_MVA());
    set_zero_sequence_load_in_MVA(load.get_zero_sequence_load_in_MVA());
    set_grounding_flag(load.get_grounding_flag());

    return *this;
}

DEVICE_ID LOAD::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);

    TERMINAL terminal;
    terminal.append_bus(get_load_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

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

        //return S0*get_load_scale_with_voltage(0.0, v, LOAD_ELLIPTICAL_CV);
        return S0*get_load_scale_with_voltage(0.0, v, LOAD_LINEAR_CV);
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

        //return S0*get_load_scale_with_voltage(1.0, v, LOAD_ELLIPTICAL_CV);
        return S0*get_load_scale_with_voltage(1.0, v, LOAD_LINEAR_CV);
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
    if(exp==0.0) // constant power
    {
        double vth = get_voltage_threshold_of_constant_power_load_in_pu();
        if(v>=vth)
            return 1.0;
        else
        {
            double Imax = get_one_over_voltage_threshold_of_constant_power_load_in_pu();
            return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
        }
    }
    else
    {
        if(exp==1.0) // constant current
        {
            double vth = get_voltage_threshold_of_constant_current_load_in_pu();
            if(v>vth)
                return v;
            else
            {
                double Imax = 1.0;
                return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
            }
        }
        else
        {
            if(exp==2.0) // constant power
                return v*v;
            else
            {
                if(exp>2.0) // higher than constant power
                    return steps_pow(v, exp);
                else // otherwise, treat as constant current
                {
                    double vth = get_voltage_threshold_of_constant_current_load_in_pu();
                    if(v>=vth)
                        return steps_pow(v, exp);
                    else
                    {
                        double Imax = 1.0/steps_pow(vth, exp-1.0);
                        return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
                    }
                }
            }
        }
    }

    /*
    if(exp>=2.0)// higher than constant impedance
        return steps_pow(v, exp);
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
            if(exp==1.0)
            {
                if(v>vth)
                    return v;
                else
                {
                    double Imax = 1.0;
                    return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
                }
            }
            else
            {
                if(v>=vth)
                    return steps_pow(v, exp);
                else
                {
                    double Imax = 1.0/steps_pow(vth, exp-1.0);
                    return get_load_scale_with_Imax_and_voltage(Imax, v, vth, cv_type);
                }
            }
        }
    }
    */
}

double LOAD::get_load_scale_with_Imax_and_voltage(double Imax, double v, double vth, LOAD_CURRENT_VOLTAGE_REDUCE_TYPE cv_type)
{
    switch(cv_type)
    {
        case LOAD_LINEAR_CV:
        {
            double I = Imax*v/vth;
            return v*I;
        }
        case LOAD_ELLIPTICAL_CV:
        {
            // (v-vth)^2/vth^2+I^2/Imax^2=1
            //double vscale = v/vth-1.0;
            double vscale = v/vth-1.0;

            double I = steps_sqrt(1.0-vscale*vscale)*Imax;
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
    {
        voltage_threshold_of_constant_power_load_in_pu = v;
        one_over_voltage_threshold_of_constant_power_load_in_pu = 1.0/v;
    }
}

void LOAD::set_voltage_threshold_of_constant_current_load_in_pu(double v)
{
    if(v>0.0)
    {
        voltage_threshold_of_constant_current_load_in_pu = v;
        one_over_voltage_threshold_of_constant_current_load_in_pu = 1.0/v;
    }
}

double LOAD::get_voltage_threshold_of_constant_power_load_in_pu()
{
    return voltage_threshold_of_constant_power_load_in_pu;
}

double LOAD::get_voltage_threshold_of_constant_current_load_in_pu()
{
    return voltage_threshold_of_constant_current_load_in_pu;
}

double LOAD::get_one_over_voltage_threshold_of_constant_power_load_in_pu()
{
    return one_over_voltage_threshold_of_constant_power_load_in_pu;
}

double LOAD::get_one_over_voltage_threshold_of_constant_current_load_in_pu()
{
    return one_over_voltage_threshold_of_constant_current_load_in_pu;
}

void LOAD::set_model(MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type(STEPS_LOAD))
    {
        model->set_device_id(get_device_id());
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

MODEL* LOAD::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="LOAD CHARACTERISTICS")
        return get_load_model();
    if(model_type=="LOAD VOLTAGE RELAY")
        return get_load_voltage_relay_model();
    if(model_type=="LOAD FREQUENCY RELAY")
        return get_load_frequency_relay_model();
    return NULL;
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
        LOAD_VOLTAGE_RELAY_MODEL* uvls = get_load_voltage_relay_model();
        LOAD_FREQUENCY_RELAY_MODEL* ufls = get_load_frequency_relay_model();
        LOAD_MODEL* load = get_load_model();
        switch(mode)
        {
            case DYNAMIC_INITIALIZE_MODE:
            {
                set_load_manually_scale_factor_in_pu(0.0);

                if(load != NULL and load->is_model_active())
                {
                    load->initialize();

                    if(uvls!=NULL and uvls->is_model_active())
                        uvls->initialize();

                    if(ufls!=NULL and ufls->is_model_active())
                        ufls->initialize();
                }
                break;
            }
            case DYNAMIC_INTEGRATE_MODE:
            case DYNAMIC_UPDATE_MODE:
            case DYNAMIC_UPDATE_TIME_STEP_MODE:
            {
                if(uvls!=NULL and uvls->is_model_active())
                    uvls->run(mode);

                if(ufls!=NULL and ufls->is_model_active())
                    ufls->run(mode);

                if(load!=NULL and load->is_model_active())
                    load->run(mode);
                break;
            }
            case DYNAMIC_RELAY_MODE:
            {
                if(uvls!=NULL and uvls->is_model_active())
                    uvls->run(mode);

                if(ufls!=NULL and ufls->is_model_active())
                    ufls->run(mode);
                break;
            }
            default:
                break;
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

complex<double> LOAD::get_positive_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    BUS* bus = get_bus_pointer();
    complex<double> U1 = bus->get_positive_sequence_complex_voltage_in_pu();

    complex<double> S1 = get_nominal_constant_power_load_in_MVA();
    complex<double> Y1 = conj(S1)/ sbase;
    complex<double> I1 = Y1 * U1;

    return I1;
}

complex<double> LOAD::get_negative_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    BUS* bus = get_bus_pointer();
    complex<double> U2 = bus->get_negative_sequence_complex_voltage_in_pu();

    complex<double> S2 = get_negative_sequence_load_in_MVA();
    complex<double> Y2 = S2 / sbase;
    complex<double> I2 = Y2 * U2;

    return I2;
}

complex<double> LOAD::get_zero_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    BUS* bus = get_bus_pointer();
    complex<double> U0 = bus->get_zero_sequence_complex_voltage_in_pu();

    complex<double> S0 = get_zero_sequence_load_in_MVA();
    complex<double> Y0 = S0 / sbase;
    complex<double> I0 = Y0 * U0;

    return I0;
}

complex<double> LOAD::get_positive_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_positive_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_load_bus()));
}

complex<double> LOAD::get_negative_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_negative_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_load_bus()));
}

complex<double> LOAD::get_zero_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_zero_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_load_bus()));
}






void LOAD::set_ratio_of_motor_active_power(double r)
{
    ratio_of_motor_P = r;
}

void LOAD::set_motor_rated_voltage_in_kV(double v)
{
    motor_rated_voltge = v;
}

void LOAD::set_mbase_code(MBASE_CODE c)
{
    mbase_code = c;
}

void LOAD::set_mbase_in_MVA(double m)
{
    mbase = m;
}

void LOAD::set_motor_Ra_in_pu(double r)
{
    motor_Ra = r;
}

void LOAD::set_motor_Xa_in_pu(double x)
{
    motor_Xa = x;
}

void LOAD::set_motor_Xm_in_pu(double x)
{
    motor_Xm = x;
}

void LOAD::set_motor_R1_in_pu(double r)
{
    motor_R1 = r;
}

void LOAD::set_motor_X1_in_pu(double x)
{
    motor_X1 = x;
}

void LOAD::set_motor_R2_in_pu(double r)
{
    motor_R2 = r;
}

void LOAD::set_motor_X2_in_pu(double x)
{
    motor_X2 = x;
}

void LOAD::set_motor_internal_voltage_in_pu(complex<double> v)
{
    motor_internal_voltage = v;
}

void LOAD::set_motor_positive_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z1 = z;
}

void LOAD::set_motor_negative_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z2 = z;
}

void LOAD::set_motor_zero_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z0 = z;
}

void LOAD::set_motor_power_in_MVA(complex<double> s)
{
    motor_power_in_MVA = s;
}

double LOAD::get_ratio_of_motor_active_power() const
{
    return ratio_of_motor_P;
}

double LOAD::get_motor_rated_voltage_in_kV() const
{
    return motor_rated_voltge;
}

MBASE_CODE LOAD::get_motor_mbase_code() const
{
    return mbase_code;
}

double LOAD::get_motor_mbase_in_MVA() const
{
    return mbase;
}

double LOAD::get_motor_Ra_in_pu() const
{
    return motor_Ra;
}

double LOAD::get_motor_Xa_in_pu() const
{
    return motor_Xa;
}

double LOAD::get_motor_Xm_in_pu() const
{
    return motor_Xm;
}

double LOAD::get_motor_R1_in_pu() const
{
    return motor_R1;
}

double LOAD::get_motor_X1_in_pu() const
{
    return motor_X1;
}

double LOAD::get_motor_R2_in_pu() const
{
    return motor_R2;
}

double LOAD::get_motor_X2_in_pu() const
{
    return motor_X2;
}

complex<double> LOAD::get_motor_internal_voltage_in_pu() const
{
    return motor_internal_voltage;
}

complex<double> LOAD::get_motor_equivalent_injection_current_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    unsigned int bus = get_load_bus();
    double mbase = get_motor_mbase_in_MVA();
    double ratedV = get_motor_rated_voltage_in_kV();
    double bus_ratedV = psdb.get_bus_base_voltage_in_kV(bus);
    double Zmb = ratedV*ratedV/mbase;
    double Zbb = bus_ratedV*bus_ratedV/sbase;

    complex<double> Z = get_motor_positive_sequence_impedance_in_pu();

    return get_motor_internal_voltage_in_pu()/(Z*Zmb/Zbb);
}

complex<double> LOAD::get_motor_positive_sequence_impedance_in_pu() const
{
    return motor_Z1;
}

complex<double> LOAD::get_motor_negative_sequence_impedance_in_pu() const
{
    return motor_Z2;
}

complex<double> LOAD::get_motor_zero_sequence_impedance_in_pu() const
{
    return motor_Z0;
}

complex<double> LOAD::get_motor_power_in_MVA() const
{
    return motor_power_in_MVA;
}

complex<double> LOAD::get_static_load_power_in_MVA() const
{
    return get_actual_total_load_in_MVA() - get_motor_power_in_MVA();
}

bool LOAD::has_motor_load() const
{
    if(get_ratio_of_motor_active_power()>0.0)
        return true;
    else
        return false;
}

void LOAD::update_motor_load_data()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    if(has_motor_load())
    {
        double power_ratio = get_ratio_of_motor_active_power();
        complex<double> total_load_power = get_actual_total_load_in_MVA();
        double motor_p = power_ratio*total_load_power.real();

        MBASE_CODE code = get_motor_mbase_code();
        double mbase = get_motor_mbase_in_MVA();
        switch(code)
        {
            case DEFAULT_RATIO_OF_ACTUAL_MACHINE_POWER:
                set_mbase_in_MVA(1.5*motor_p);break;
            case MACHINE_BASE_POWER:
                set_mbase_in_MVA(mbase);break;
            case CUSTOM_RATIO_OF_ACTUAL_MACHINE_POWER:
                set_mbase_in_MVA(mbase*motor_p);break;
            default:
                set_mbase_in_MVA(1.5*motor_p);
        }

        mbase = get_motor_mbase_in_MVA();
        double rated_voltage = get_motor_rated_voltage_in_kV();
        double Ra = get_motor_Ra_in_pu();
        double Xa = get_motor_Xa_in_pu();
        double Xm = get_motor_Xm_in_pu();
        double R1 = get_motor_R1_in_pu();
        double X1 = get_motor_X1_in_pu();
        double R2 = get_motor_R2_in_pu();
        double X2 = get_motor_X2_in_pu();

        BUS* busptr = get_bus_pointer();
        double V = busptr->get_positive_sequence_voltage_in_kV()/rated_voltage;
        complex<double> complex_V = busptr->get_positive_sequence_complex_voltage_in_kV()/rated_voltage;
        double P = motor_p/mbase/(V*V);

        // single cage
        if((R2==0.0 and X2==0.0) or (R2==999.0 and X2==999.0))
        {
            double Xrm = X1 + Xm;
            double Xsm = Xa + Xm;
            double Xp = Xa*X1 + Xm*Xa + Xm*X1;
            double R = 0.0;

            double Ap = Ra*Ra + Xsm*Xsm;
            double Bp = 2.0*Ra*Xm*Xm;
            double Cp = Xp*Xp + Ra*Ra*Xrm*Xrm;


            double App = Ap*P - Ra;
            double Bpp = Bp*P - Xm*Xm;
            double Cpp = Cp*P - Ra*Xrm*Xrm;

            double delta = Bpp*Bpp - 4.0*App*Cpp;
            if(delta<0.0)
            {
                osstream<<"Wrong"<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }

            R = (-Bpp + sqrt(delta))/(2.0*App);
            if(R1/R>0.2 or R1/R<0.0)
            {
                R = (-Bpp - sqrt(delta))/(2.0*App);
                if(R1/R>0.2 or R1<0.0)
                {
                    osstream<<"No appropriate root"<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
            }
            double s = R1/R;

            double Q = V*V*(Xsm*R*R+Xp*Xrm)/(Ap*R*R+Bp*R+Cp);
            double motor_q = Q*mbase;

            set_motor_power_in_MVA(complex<double>(motor_p, motor_q));

            // equivalent impedance
            complex<double> Z = complex<double>(Ra,Xa+Xm*X1/(Xm+X1));
            set_motor_positive_sequence_impedance_in_pu(Z);
            set_motor_negative_sequence_impedance_in_pu(Z);

            // internal voltage
//            complex<double> motor_power = get_motor_power_in_MVA();
//            complex<double> I = motor_power/mbase/complex_V;
//            complex<double> E = complex_V - I*Z;
//            set_motor_internal_voltage_in_pu(E);


            double ratedV = get_motor_rated_voltage_in_kV();
            double bus_ratedV = busptr->get_base_voltage_in_kV();
            double Zmb = ratedV*ratedV/mbase;
            double Zbb = bus_ratedV*bus_ratedV/sbase;

            complex<double> V1 = busptr->get_positive_sequence_complex_voltage_in_pu();

            complex<double> Z_on_mbase = get_motor_positive_sequence_impedance_in_pu();
            complex<double> Z1 = Z_on_mbase*Zmb/Zbb;


            complex<double> motor_power = get_motor_power_in_MVA();
            complex<double> I = conj(motor_power/sbase/V1);

            complex<double> E = V1 - I*Z1;
            set_motor_internal_voltage_in_pu(E);


            // set_motor_internal_voltage_in_pu(complex_V);
        }
        else    //double cage
        {
            double M = R2/R1;
            double a,b,c,d,e,f,g,h,k;
            double Y4, Y3, Y2, Y1, Y0;
            double R=0.0,Q;

            a = M*Ra;
            b = -Xa*(Xm+Xm*M+X2+X1*M)-Xm*X1*M-Xm*X2;
            c = -(Xm*X1+Xm*X2+X1*X2)*Ra;
            d = Xa*M+Xm*M;
            e = Ra*(Xm+Xm*M+X2+X1*M);
            f = -Xa*(Xm*X1+Xm*X2+X1*X2)-Xm*X1*X2;
            g = M;
            h = -(Xm*X1+Xm*X2+X1*X2);
            k = Xm+Xm*M+X2+X1*M;

            Y4 = g*a-P*a*a-P*d*d;
            Y3 = g*b+k*d-2*P*a*b-2*P*d*e;
            Y2 = g*c+h*a+k*e-P*b*b-2*P*a*c-P*e*e-2*P*d*f;
            Y1 = h*b+k*f-2*P*b*c-2*P*e*f;
            Y0 = h*c-P*c*c-P*f*f;

            float* Rp = roots_quartic_equation(Y4,Y3,Y2,Y1,Y0);

            for (unsigned int t=1; t<(Rp[0]+1); t++)
            {
                if(R1/Rp[t]<0.2 and R1/Rp[t]>0.0)
                {
                    R = Rp[t];
                    break;
                }
            }

            if(R == 0.0)
            {
                osstream<<"Double cage has no root"<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
            double s = R1/R;

            Q = -V*V*(k*R*(a*R*R+b*R+c)-(g*R*R+h)*(d*R*R+e*R+f))/((a*R*R+b*R+c)*(a*R*R+b*R+c)+(d*R*R+e*R+f)*(d*R*R+e*R*f));
            double motor_q = Q*mbase;
            set_motor_power_in_MVA(complex<double>(motor_p, motor_q));

            // equivalent impedance
            complex<double> Z = complex<double>(Ra,Xa+Xm*X1*X2/(X1+X2)/(Xm+X1*X2/(X1+X2)));
            set_motor_positive_sequence_impedance_in_pu(Z);
            set_motor_negative_sequence_impedance_in_pu(Z);

            // internal voltage
            double ratedV = get_motor_rated_voltage_in_kV();
            double bus_ratedV = busptr->get_base_voltage_in_kV();
            double Zmb = ratedV*ratedV/mbase;
            double Zbb = bus_ratedV*bus_ratedV/sbase;

            complex<double> V1 = busptr->get_positive_sequence_complex_voltage_in_pu();

            complex<double> Z_on_mbase = get_motor_positive_sequence_impedance_in_pu();
            complex<double> Z1 = Z_on_mbase*Zmb/Zbb;


            complex<double> motor_power = get_motor_power_in_MVA();
            complex<double> I = conj(motor_power/sbase/V1);

            complex<double> E = V1 - I*Z1;
            set_motor_internal_voltage_in_pu(E);

        }
    }
}

void LOAD::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool LOAD::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}

void LOAD::initialize_ABCD_matrix_for_linearization()
{
    if(Aptr == NULL)
        Aptr = new STEPS_SPARSE_MATRIX;
    if(Bptr == NULL)
        Bptr = new STEPS_SPARSE_MATRIX;
    if(Cptr == NULL)
        Cptr = new STEPS_SPARSE_MATRIX;
    if(Dptr == NULL)
        Dptr = new STEPS_SPARSE_MATRIX;
}

void LOAD::build_linearized_matrix_ABCD()
{
    initialize_ABCD_matrix_for_linearization();

    STEPS_SPARSE_MATRIX A, B, C, D;
    if(load_model != NULL)
    {
        load_model->build_linearized_matrix_ABCD();
        A = load_model->get_linearized_matrix_A();
        B = load_model->get_linearized_matrix_B();
        C = load_model->get_linearized_matrix_C();
        D = load_model->get_linearized_matrix_D();
    }
    A.compress_and_merge_duplicate_entries();
    B.compress_and_merge_duplicate_entries();
    C.compress_and_merge_duplicate_entries();
    D.compress_and_merge_duplicate_entries();

    *Aptr = A;
    *Bptr = B;
    *Cptr = C;
    *Dptr = D;
}

STEPS_SPARSE_MATRIX LOAD::get_linearized_matrix_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_matrix_A();
        case 'B':
            return get_linearized_matrix_B();
        case 'C':
            return get_linearized_matrix_C();
        case 'D':
            return get_linearized_matrix_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}
STEPS_SPARSE_MATRIX LOAD::get_linearized_matrix_A() const
{
    if(Aptr!=NULL) return *Aptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to A matrix exists in LOAD. Cannot return linearized A matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD::get_linearized_matrix_B() const
{
    if(Bptr!=NULL) return *Bptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to B matrix exists in LOAD. Cannot return linearized B matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD::get_linearized_matrix_C() const
{
    if(Cptr!=NULL) return *Cptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to C matrix exists in LOAD. Cannot return linearized C matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD::get_linearized_matrix_D() const
{
    if(Dptr!=NULL) return *Dptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to D matrix exists in LOAD. Cannot return linearized D matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX* LOAD::get_linearized_matrix_pointer_A()
{
    return Aptr;
}

STEPS_SPARSE_MATRIX* LOAD::get_linearized_matrix_pointer_B()
{
    return Bptr;
}

STEPS_SPARSE_MATRIX* LOAD::get_linearized_matrix_pointer_C()
{
    return Cptr;
}

STEPS_SPARSE_MATRIX* LOAD::get_linearized_matrix_pointer_D()
{
    return Dptr;
}
