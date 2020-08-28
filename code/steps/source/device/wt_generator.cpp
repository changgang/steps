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
}

DEVICE_ID WT_GENERATOR::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");

    TERMINAL terminal;
    terminal.append_bus(get_generator_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

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

void WT_GENERATOR::set_model(const MODEL* model)
{
    if(model != NULL)
    {
        if(model->has_allowed_device_type("WT GENERATOR"))
        {
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
}

MODEL* WT_GENERATOR::get_model_of_type(string model_type)
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


WT_GENERATOR_MODEL* WT_GENERATOR::get_wt_generator_model()
{
    return wt_generator_model;
}

WT_AERODYNAMIC_MODEL* WT_GENERATOR::get_wt_aerodynamic_model()
{
    return wt_aerodynamic_model;
}

WT_TURBINE_MODEL* WT_GENERATOR::get_wt_turbine_model()
{
    return wt_turbine_model;
}

WT_ELECTRICAL_MODEL* WT_GENERATOR::get_wt_electrical_model()
{
    return wt_electrical_model;
}

WT_PITCH_MODEL* WT_GENERATOR::get_wt_pitch_model()
{
    return wt_pitch_model;
}

WIND_SPEED_MODEL* WT_GENERATOR::get_wind_speed_model()
{
    return wind_speed_model;
}

WT_RELAY_MODEL* WT_GENERATOR::get_wt_relay_model()
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



