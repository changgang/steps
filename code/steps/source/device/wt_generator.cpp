#include "header/device/wt_generator.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/STEPS.h"
#include "header/model/wtg_models/wt_generator_model/wt_generator_models.h"
#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_models.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_models.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_models.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_models.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_models.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_models.h"

#include <istream>
#include <iostream>

using namespace std;

WT_GENERATOR::WT_GENERATOR(STEPS& toolkit) : SOURCE(toolkit)
{
    clear();
}

WT_GENERATOR::~WT_GENERATOR()
{
}

void WT_GENERATOR::clear()
{
    SOURCE::clear();
    set_number_of_lumped_wt_generators(1);
    set_rated_power_per_wt_generator_in_MW(0.0);

    wt_generator_model = NULL;
    wt_aerodynamic_model = NULL;
    wt_turbine_model = NULL;
    wt_electrical_model = NULL;
    wt_pitch_model = NULL;
    wind_speed_model = NULL;
    wt_relay_model = NULL;

    set_positive_sequence_resistance_in_pu(0.0);
    set_positive_sequence_syncronous_reactance_in_pu(0.0);
    set_positive_sequence_transient_reactance_in_pu(0.0);
    set_positive_sequence_subtransient_reactance_in_pu(0.0);
    set_negative_sequence_resistance_in_pu(0.0);
    set_negative_sequence_reactance_in_pu(0.0);
    set_zero_sequence_resistance_in_pu(0.0);
    set_zero_sequence_reactance_in_pu(0.0);
    set_grounding_resistance_in_pu(0.0);
    set_grounding_reactance_in_pu(0.0);
    set_generator_reactance_option(SUBTRANSIENT_REACTANCE);
    set_sequence_parameter_import_flag(false);
}

DEVICE_ID WT_GENERATOR::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);

    TERMINAL terminal;
    terminal.append_bus(get_generator_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}


void WT_GENERATOR::set_generator_bus(unsigned int bus)
{
    set_source_bus(bus);
}

unsigned int WT_GENERATOR::get_generator_bus() const
{
    return get_source_bus();
}

void WT_GENERATOR::set_number_of_lumped_wt_generators(unsigned int n)
{
    if(n==0)
        n = 1;
    number_of_lumped_wt_generators = n;
}

void WT_GENERATOR::set_rated_power_per_wt_generator_in_MW(double P)
{
    rated_power_per_wt_generator_in_MW = P;
}

unsigned int WT_GENERATOR::get_number_of_lumped_wt_generators() const
{
    return number_of_lumped_wt_generators;
}

double WT_GENERATOR::get_rated_power_per_wt_generator_in_MW() const
{
    return rated_power_per_wt_generator_in_MW;
}

void WT_GENERATOR::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    if(get_status()==true)
    {
        WT_GENERATOR_MODEL* gen = get_wt_generator_model();
        WT_TURBINE_MODEL* turbine = get_wt_turbine_model();
        WT_ELECTRICAL_MODEL* elec = get_wt_electrical_model();
        WT_PITCH_MODEL* pitch = get_wt_pitch_model();
        WT_RELAY_MODEL* relay = get_wt_relay_model();

        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                STEPS& toolkit = get_toolkit();
                WT_AERODYNAMIC_MODEL* aero = get_wt_aerodynamic_model();
                WIND_SPEED_MODEL* wind = get_wind_speed_model();
                if(gen!=NULL)
                    gen->initialize();
                else
                {
                    osstream<<"Error. No WT_GENERATOR_MODEL is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }

                if(wind!=NULL)
                    wind->initialize();

                if(aero!=NULL)
                    aero->initialize();
                else
                {
                    osstream<<"Error. No WT_AERO_DYNAMIC_MODEL is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }

                if(turbine!=NULL)
                    turbine->initialize();
                else
                {
                    osstream<<"Error. No WT_TURBINE_MODEL is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }


                if(elec!=NULL)
                    elec->initialize();

                if(pitch!=NULL)
                    pitch->initialize();

                if(relay!=NULL)
                    relay->initialize();

                break;
            }
            case INTEGRATE_MODE:
            case UPDATE_MODE:
            {
                if(relay!=NULL and relay->is_model_active())
                    relay->run(mode);

                if(pitch!=NULL and pitch->is_model_active())
                    pitch->run(mode);

                //if(wind!=NULL and wind->is_model_active())
                //    wind->run(mode);

                if(turbine!=NULL and turbine->is_model_active())
                    turbine->run(mode);

                if(elec!=NULL and elec->is_model_active())
                    elec->run(mode);

                if(turbine!=NULL and turbine->is_model_active())
                    turbine->run(mode);

                //if(aero!=NULL)
                //    aero->run(mode);

                if(gen!=NULL and gen->is_model_active())
                    gen->run(mode);
                break;
            }
            case RELAY_MODE:
            {
                if(relay!=NULL)
                    relay->run(mode);
            }
        }
    }
}


void WT_GENERATOR::report() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar"<<endl
      <<"Zsource = "<<setw(8)<<setprecision(6)<<fixed<<get_source_impedance_in_pu();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT_GENERATOR::save() const
{
    ;
}

void WT_GENERATOR::set_model(MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type(STEPS_WT_GENERATOR))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="WT GENERATOR")
        {
            set_wt_generator_model((WT_GENERATOR_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WT AERODYNAMIC")
        {
            set_wt_aerodynamic_model((WT_AERODYNAMIC_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WT TURBINE")
        {
            set_wt_turbine_model((WT_TURBINE_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WT ELECTRICAL")
        {
            set_wt_electrical_model((WT_ELECTRICAL_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WT PITCH")
        {
            set_wt_pitch_model((WT_PITCH_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WIND SPEED")
        {
            set_wind_speed_model((WIND_SPEED_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="WT RELAY")
        {
            set_wt_relay_model((WT_RELAY_MODEL*) model);
            return;
        }

        ostringstream osstream;
        osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up wind-turbine generator-related model.";

        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* WT_GENERATOR::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="WT GENERATOR")
        return get_wt_generator_model();
    if(model_type=="WT AERODYNAMIC")
        return get_wt_aerodynamic_model();
    if(model_type=="WT TURBINE")
        return get_wt_turbine_model();
    if(model_type=="WT ELECTRICAL")
        return get_wt_electrical_model();
    if(model_type=="WT PITCH")
        return get_wt_pitch_model();
    if(model_type=="WIND SPEED")
        return get_wind_speed_model();
    if(model_type=="WT RELAY")
        return get_wt_relay_model();
    return NULL;
}

void WT_GENERATOR::set_wt_generator_model(WT_GENERATOR_MODEL* model)
{
    if(model!=NULL)
        wt_generator_model = model;
}

void WT_GENERATOR::set_wt_aerodynamic_model(WT_AERODYNAMIC_MODEL* model)
{
    if(model!=NULL)
        wt_aerodynamic_model = model;
}

void WT_GENERATOR::set_wt_turbine_model(WT_TURBINE_MODEL* model)
{
    if(model!=NULL)
        wt_turbine_model = model;
}

void WT_GENERATOR::set_wt_electrical_model(WT_ELECTRICAL_MODEL* model)
{
    if(model!=NULL)
        wt_electrical_model = model;
}

void WT_GENERATOR::set_wt_pitch_model(WT_PITCH_MODEL* model)
{
    if(model!=NULL)
        wt_pitch_model = model;
}

void WT_GENERATOR::set_wind_speed_model(WIND_SPEED_MODEL* model)
{
    if(model!=NULL)
        wind_speed_model = model;
}

void WT_GENERATOR::set_wt_relay_model(WT_RELAY_MODEL* model)
{
    if(model!=NULL)
        wt_relay_model = model;
}


WT_GENERATOR_MODEL* WT_GENERATOR::get_wt_generator_model() const
{
    return wt_generator_model;
}

WT_AERODYNAMIC_MODEL* WT_GENERATOR::get_wt_aerodynamic_model() const
{
    return wt_aerodynamic_model;
}

WT_TURBINE_MODEL* WT_GENERATOR::get_wt_turbine_model() const
{
    return wt_turbine_model;
}

WT_ELECTRICAL_MODEL* WT_GENERATOR::get_wt_electrical_model() const
{
    return wt_electrical_model;
}

WT_PITCH_MODEL* WT_GENERATOR::get_wt_pitch_model() const
{
    return wt_pitch_model;
}

WIND_SPEED_MODEL* WT_GENERATOR::get_wind_speed_model() const
{
    return wind_speed_model;
}

WT_RELAY_MODEL* WT_GENERATOR::get_wt_relay_model() const
{
    return wt_relay_model;
}

WT_GENERATOR& WT_GENERATOR::operator=(const WT_GENERATOR& gen)
{
    if(this==(&gen)) return *this;

    set_toolkit(gen.get_toolkit());
    clear();

    set_generator_bus(gen.get_generator_bus());
    set_identifier(gen.get_identifier());
    set_status(gen.get_status());
    set_mbase_in_MVA(gen.get_mbase_in_MVA());
    set_p_generation_in_MW(gen.get_p_generation_in_MW());
    set_q_generation_in_MVar(gen.get_q_generation_in_MVar());
    set_p_max_in_MW(gen.get_p_max_in_MW());
    set_p_min_in_MW(gen.get_p_min_in_MW());
    set_q_max_in_MVar(gen.get_q_max_in_MVar());
    set_q_min_in_MVar(gen.get_q_min_in_MVar());
    set_source_impedance_in_pu(gen.get_source_impedance_in_pu());
    set_bus_to_regulate(gen.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(gen.get_voltage_to_regulate_in_pu());

    set_number_of_lumped_wt_generators(gen.get_number_of_lumped_wt_generators());
    set_rated_power_per_wt_generator_in_MW(gen.get_rated_power_per_wt_generator_in_MW());

    set_model(gen.get_wt_generator_model());
    set_model(gen.get_wt_aerodynamic_model());
    set_model(gen.get_wt_electrical_model());
    set_model(gen.get_wt_turbine_model());
    set_model(gen.get_wt_pitch_model());
    set_model(gen.get_wind_speed_model());
    set_model(gen.get_wt_relay_model());

    set_positive_sequence_resistance_in_pu(gen.get_positive_sequence_resistance_in_pu());
    set_positive_sequence_syncronous_reactance_in_pu(gen.get_positive_sequence_syncronous_reactance_in_pu());
    set_positive_sequence_transient_reactance_in_pu(gen.get_positive_sequence_transient_reactance_in_pu());
    set_positive_sequence_subtransient_reactance_in_pu(gen.get_positive_sequence_subtransient_reactance_in_pu());
    set_negative_sequence_resistance_in_pu(gen.get_negative_sequence_resistance_in_pu());
    set_negative_sequence_reactance_in_pu(gen.get_negative_sequence_reactance_in_pu());
    set_zero_sequence_resistance_in_pu(gen.get_zero_sequence_resistance_in_pu());
    set_zero_sequence_reactance_in_pu(gen.get_zero_sequence_reactance_in_pu());
    set_grounding_resistance_in_pu(gen.get_grounding_resistance_in_pu());
    set_grounding_reactance_in_pu(gen.get_grounding_reactance_in_pu());
    set_generator_reactance_option(gen.get_generator_reactance_option());
    return *this;
}

complex<double> WT_GENERATOR::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    return 0.0;
}

complex<double> WT_GENERATOR::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    return 0.0;
}

void WT_GENERATOR::set_positive_sequence_resistance_in_pu(double r)
{
    R1 = r;
    if(get_negative_sequence_resistance_in_pu()!=0.0)
        set_negative_sequence_resistance_in_pu(r);
    if(get_zero_sequence_resistance_in_pu()!=0.0)
        set_zero_sequence_resistance_in_pu(r);
}

void WT_GENERATOR::set_positive_sequence_syncronous_reactance_in_pu(double x)
{
    X1_sync = x;
}

void WT_GENERATOR::set_positive_sequence_transient_reactance_in_pu(double x)
{
    X1_transient = x;
}

void WT_GENERATOR::set_positive_sequence_subtransient_reactance_in_pu(double x)
{
    X1_subtransient = x;
    if(get_positive_sequence_syncronous_reactance_in_pu()==0.0)
        set_positive_sequence_syncronous_reactance_in_pu(x);
    if(get_positive_sequence_transient_reactance_in_pu()==0.0)
        set_positive_sequence_transient_reactance_in_pu(x);
    if(get_negative_sequence_reactance_in_pu()==0.0)
        set_negative_sequence_reactance_in_pu(x);
    if(get_zero_sequence_reactance_in_pu()==0.0)
        set_zero_sequence_reactance_in_pu(x);
}

void WT_GENERATOR::set_negative_sequence_resistance_in_pu(double r)
{
    R2 = r;
}

void WT_GENERATOR::set_negative_sequence_reactance_in_pu(double x)
{
    X2 = x;
}

void WT_GENERATOR::set_zero_sequence_resistance_in_pu(double r)
{
    R0 = r;
}

void WT_GENERATOR::set_zero_sequence_reactance_in_pu(double x)
{
    X0 = x;
}

void WT_GENERATOR::set_grounding_resistance_in_pu(double r)
{
    Rground = r;
}

void WT_GENERATOR::set_grounding_reactance_in_pu(double x)
{
    Xground = x;
}

double WT_GENERATOR::get_positive_sequence_resistance_in_pu() const
{
    return R1;
}

double WT_GENERATOR::get_positive_sequence_syncronous_reactance_in_pu() const
{
    return X1_sync;
}

double WT_GENERATOR::get_positive_sequence_transient_reactance_in_pu() const
{
    return X1_transient;
}

double WT_GENERATOR::get_positive_sequence_subtransient_reactance_in_pu() const
{
    return X1_subtransient;
}

double WT_GENERATOR::get_negative_sequence_resistance_in_pu() const
{
    return R2;
}

double WT_GENERATOR::get_negative_sequence_reactance_in_pu() const
{
    return X2;
}

double WT_GENERATOR::get_zero_sequence_resistance_in_pu() const
{
    return R0;
}

double WT_GENERATOR::get_zero_sequence_reactance_in_pu() const
{
    return X0;
}

double WT_GENERATOR::get_grounding_resistance_in_pu() const
{
    return Rground;
}

double WT_GENERATOR::get_grounding_reactance_in_pu() const
{
    return Xground;
}

void WT_GENERATOR::set_generator_reactance_option(GENERATOR_REACTANCE_OPTION option)
{
    gen_X_option = option;
}

GENERATOR_REACTANCE_OPTION WT_GENERATOR::get_generator_reactance_option() const
{
    return gen_X_option;
}

void WT_GENERATOR::update_internal_voltage_for_short_circuit_solver()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR_REACTANCE_OPTION option = get_generator_reactance_option();

    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_generator_bus());
    complex<double> sbase = psdb.get_system_base_power_in_MVA();
    complex<double> s = get_complex_generation_in_MVA();
    complex<double> I = conj(s/sbase/V);

    double R = get_positive_sequence_resistance_in_pu();
    double X;
    switch(option)
    {
        case SUBTRANSIENT_REACTANCE:
            X = get_positive_sequence_subtransient_reactance_in_pu();
            break;
        case TRANSIENT_REACTANCE:
            X = get_positive_sequence_transient_reactance_in_pu();
            break;
        case SYNCHRONOUS_REACTANCE:
            X = get_positive_sequence_syncronous_reactance_in_pu();
            break;
    }
    complex<double> mbase = get_mbase_in_MVA();
    complex<double> Z = complex<double>(R,X)/mbase*sbase;

    internal_voltage_for_short_circuit_solver = V + Z*I;
}

complex<double> WT_GENERATOR::get_complex_internal_voltage_for_short_circuit_solver_in_pu()
{
    return internal_voltage_for_short_circuit_solver;
}

complex<double> WT_GENERATOR::get_positive_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR_REACTANCE_OPTION option = get_generator_reactance_option();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R = get_positive_sequence_resistance_in_pu();
    double X;
    switch(option)
    {
        case SUBTRANSIENT_REACTANCE:
            X = get_positive_sequence_subtransient_reactance_in_pu();
            break;
        case TRANSIENT_REACTANCE:
            X = get_positive_sequence_transient_reactance_in_pu();
            break;
        case SYNCHRONOUS_REACTANCE:
            X = get_positive_sequence_syncronous_reactance_in_pu();
            break;
    }
    complex<double> Z = complex<double>(R, X) * one_over_mbase * sbase;
    complex<double> E = get_complex_internal_voltage_for_short_circuit_solver_in_pu();
    complex<double> U1 = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = (U1-E)/Z;

    return I;
}

complex<double> WT_GENERATOR::get_negative_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R2 = get_negative_sequence_resistance_in_pu();
    double X2 = get_negative_sequence_reactance_in_pu();
    complex<double> Z2 = complex<double>(R2, X2)* one_over_mbase * sbase;
    complex<double> U2 = psdb.get_bus_negative_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = U2/Z2;

    return I;
}

complex<double> WT_GENERATOR::get_zero_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R0 = get_zero_sequence_resistance_in_pu();
    double X0 = get_zero_sequence_reactance_in_pu();
    complex<double> Z0 = complex<double>(R0, X0) * one_over_mbase * sbase;
    complex<double> U0 = psdb.get_bus_zero_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = U0/Z0;

    return I;
}

complex<double> WT_GENERATOR::get_positive_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_positive_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> WT_GENERATOR::get_negative_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_negative_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> WT_GENERATOR::get_zero_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_zero_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

void WT_GENERATOR::set_wt_generator_type(WT_GENERATOR_TYPE type)
{
    wtg_type = type;
}

WT_GENERATOR_TYPE WT_GENERATOR::get_wt_generator_type() const
{
    return wtg_type;
}

void WT_GENERATOR::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool WT_GENERATOR::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}









void WT_GENERATOR::set_motor_rated_voltage_in_kV(double v)
{
    motor_rated_voltge = v;
}

void WT_GENERATOR::set_mbase_in_MVA(double m)
{
    mbase = m;
}

void WT_GENERATOR::set_motor_Ra_in_pu(double r)
{
    motor_Ra = r;
}

void WT_GENERATOR::set_motor_Xa_in_pu(double x)
{
    motor_Xa = x;
}

void WT_GENERATOR::set_motor_Xm_in_pu(double x)
{
    motor_Xm = x;
}

void WT_GENERATOR::set_motor_R1_in_pu(double r)
{
    motor_R1 = r;
}

void WT_GENERATOR::set_motor_X1_in_pu(double x)
{
    motor_X1 = x;
}

void WT_GENERATOR::set_motor_R2_in_pu(double r)
{
    motor_R2 = r;
}

void WT_GENERATOR::set_motor_X2_in_pu(double x)
{
    motor_X2 = x;
}

void WT_GENERATOR::set_motor_internal_voltage_in_pu(complex<double> v)
{
    motor_internal_voltage = v;
}

void WT_GENERATOR::set_motor_positive_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z1 = z;
}

void WT_GENERATOR::set_motor_negative_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z2 = z;
}

void WT_GENERATOR::set_motor_zero_sequence_impedance_in_pu(complex<double> z)
{
    motor_Z0 = z;
}

void WT_GENERATOR::set_motor_power_in_MVA(complex<double> s)
{
    motor_power_in_MVA = s;
}


double WT_GENERATOR::get_motor_rated_voltage_in_kV() const
{
    return motor_rated_voltge;
}

double WT_GENERATOR::get_motor_mbase_in_MVA() const
{
    return mbase;
}

double WT_GENERATOR::get_motor_Ra_in_pu() const
{
    return motor_Ra;
}

double WT_GENERATOR::get_motor_Xa_in_pu() const
{
    return motor_Xa;
}

double WT_GENERATOR::get_motor_Xm_in_pu() const
{
    return motor_Xm;
}

double WT_GENERATOR::get_motor_R1_in_pu() const
{
    return motor_R1;
}

double WT_GENERATOR::get_motor_X1_in_pu() const
{
    return motor_X1;
}

double WT_GENERATOR::get_motor_R2_in_pu() const
{
    return motor_R2;
}

double WT_GENERATOR::get_motor_X2_in_pu() const
{
    return motor_X2;
}

complex<double> WT_GENERATOR::get_motor_internal_voltage_in_pu() const
{
    return motor_internal_voltage;
}

complex<double> WT_GENERATOR::get_motor_equivalent_injection_current_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    unsigned int bus = get_generator_bus();
    double mbase = get_motor_mbase_in_MVA();
    double ratedV = get_motor_rated_voltage_in_kV();
    double bus_ratedV = psdb.get_bus_base_voltage_in_kV(bus);
    double Zmb = ratedV*ratedV/mbase;
    double Zbb = bus_ratedV*bus_ratedV/sbase;

    complex<double> Z = get_motor_positive_sequence_impedance_in_pu();

    return get_motor_internal_voltage_in_pu()/(Z*Zmb/Zbb);
}

complex<double> WT_GENERATOR::get_motor_positive_sequence_impedance_in_pu() const
{
    return motor_Z1;
}

complex<double> WT_GENERATOR::get_motor_negative_sequence_impedance_in_pu() const
{
    return motor_Z2;
}

complex<double> WT_GENERATOR::get_motor_zero_sequence_impedance_in_pu() const
{
    return motor_Z0;
}

complex<double> WT_GENERATOR::get_motor_power_in_MVA() const
{
    return motor_power_in_MVA;
}

complex<double> WT_GENERATOR::get_static_power_in_MVA() const
{
    return get_complex_generation_in_MVA() - get_motor_power_in_MVA();
}


void WT_GENERATOR::update_motor_data()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    if(get_sequence_parameter_import_flag()==false)
        return;

    double motor_p = get_p_generation_in_MW();
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
            osstream<<"Can not get slip of wt induction machine."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }

        R = (-Bpp + sqrt(delta))/(2.0*App);
        if(R1/R>0.2 or R1/R<0.0)
        {
            R = (-Bpp - sqrt(delta))/(2.0*App);
            if(R1/R>0.2 or R1<0.0)
            {
                osstream<<"No appropriate root with wt induction machine"<<endl;
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
//        complex<double> motor_power = get_motor_power_in_MVA();
//        complex<double> I = motor_power/mbase/complex_V;
//        complex<double> E = complex_V - I*Z;
//        set_motor_internal_voltage_in_pu(E);
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
            osstream<<"Double cage has no root of wt generator."<<endl;
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
//        complex<double> motor_power = get_motor_power_in_MVA();
//        complex<double> I = motor_power/mbase/complex_V;
//        complex<double> E = complex_V - I*Z;
//        set_motor_internal_voltage_in_pu(E);
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

