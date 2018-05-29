#include "header/device/wt_generator.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/wtg_models/wt_generator_model/wt_generator_models.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_models.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_models.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_models.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_models.h"

#include <iostream>

using namespace std;

WT_GENERATOR::WT_GENERATOR(POWER_SYSTEM_DATABASE* psdb) : SOURCE(psdb)
{
    ostringstream sstream;
    if(psdb==NULL)
    {
        sstream<<"Error. WT_GENERATOR object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(sstream);
    }
    set_power_system_database(psdb);
    clear();
    set_regulating_mode(REGULATING_PV);

    wt_generator_model = NULL;
    wt_aerodynamic_model = NULL;
    wt_turbine_model = NULL;
    wt_electrical_model = NULL;
    wt_pitch_model = NULL;
    wind_speed_model = NULL;
}

WT_GENERATOR::~WT_GENERATOR()
{
    ;
}

void WT_GENERATOR::clear()
{
    SOURCE::clear();
    set_number_of_lumped_wt_generators(1);
    set_rated_power_per_wt_generator_in_MW(0.0);
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


void WT_GENERATOR::set_generator_bus(size_t bus)
{
    set_source_bus(bus);
}

size_t WT_GENERATOR::get_generator_bus() const
{
    return get_source_bus();
}

void WT_GENERATOR::set_number_of_lumped_wt_generators(size_t n)
{
    if(n==0)
        n = 1;
    number_of_lumped_wt_generators = n;
}

void WT_GENERATOR::set_rated_power_per_wt_generator_in_MW(double P)
{
    rated_power_per_wt_generator_in_MW = P;
}

size_t WT_GENERATOR::get_number_of_lumped_wt_generators() const
{
    return number_of_lumped_wt_generators;
}

double WT_GENERATOR::get_rated_power_per_wt_generator_in_MW() const
{
    return rated_power_per_wt_generator_in_MW;
}

void WT_GENERATOR::run(DYNAMIC_MODE mode)
{
    ostringstream sstream;

    if(get_status()==false)
        return;

    WT_GENERATOR_MODEL* gen = get_wt_generator_model();
    WT_AERODYNAMIC_MODEL* aero = get_wt_aerodynamic_model();
    WT_TURBINE_MODEL* turbine = get_wt_turbine_model();
    WT_ELECTRICAL_MODEL* elec = get_wt_electrical_model();
    WT_PITCH_MODEL* pitch = get_wt_pitch_model();
    WIND_SPEED_MODEL* wind = get_wind_speed_model();

    switch(mode)
    {
        case INITIALIZE_MODE:
        {
            if(gen==NULL)
            {
                sstream<<"Error. No WT_GENERATOR_MODEL is provided for "<<get_device_name()<<" for dynamic initialization.";
                show_information_with_leading_time_stamp(sstream);
                return;
            }
            gen->initialize();

            if(aero==NULL)
            {
                sstream<<"Error. No WT_AERO_DYNAMIC_MODEL is provided for "<<get_device_name()<<" for dynamic initialization.";
                show_information_with_leading_time_stamp(sstream);
                return;
            }
            aero->initialize();

            if(turbine==NULL)
            {
                sstream<<"Error. No WT_TURBINE_MODEL is provided for "<<get_device_name()<<" for dynamic initialization.";
                show_information_with_leading_time_stamp(sstream);
                return;
            }
            turbine->initialize();


            if(elec!=NULL)
                elec->initialize();

            if(pitch!=NULL)
                pitch->initialize();

            if(wind!=NULL)
                wind->initialize();

            break;
        }
        default:
        {
            if(pitch!=NULL)
                pitch->run(mode);

            if(wind!=NULL)
                wind->run(mode);

            if(turbine!=NULL)
                turbine->run(mode);

            if(elec!=NULL)
                elec->run(mode);

            if(turbine!=NULL)
                turbine->run(mode);

            if(aero!=NULL)
                aero->run(mode);

            if(gen!=NULL)
                gen->run(mode);
            break;
        }
    }
}


void WT_GENERATOR::report() const
{
    ostringstream sstream;
    string regulating_mode;
    switch(get_regulating_mode())
    {
        case REGULATING_PQ:
            regulating_mode = "regulating P and Q";
            break;
        case REGULATING_PV:
            regulating_mode = "regulating P and V";
            break;
        case REGULATING_VA:
            regulating_mode = "regulating V and Angle";
            break;
        default:
            regulating_mode = "not set(ERROR)";
    }
    sstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"regulating mode: "<<regulating_mode<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar"<<endl
      <<"Zsource = "<<setw(8)<<setprecision(6)<<fixed<<get_source_impedance_in_pu();
    show_information_with_leading_time_stamp(sstream);
}

void WT_GENERATOR::save() const
{
    ;
}

void WT_GENERATOR::set_model(const MODEL* model)
{
    if(model == NULL)
        return;

    if(model->get_allowed_device_type()!="WT GENERATOR")
        return;

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

    ostringstream sstream;
    sstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up wind-turbine generator-related model.";
    show_information_with_leading_time_stamp(sstream);
}

void WT_GENERATOR::set_wt_generator_model(const WT_GENERATOR_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WT GENERATOR")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wt generator model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WT_GENERATOR_MODEL* oldmodel = get_wt_generator_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wt_generator_model = NULL;
    }

    WT_GENERATOR_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="WT3G2")
    {
        WT3G2* smodel = (WT3G2*) (model);
        new_model = (WT_GENERATOR_MODEL*) new WT3G2(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wt_generator_model = new_model;

        set_number_of_lumped_wt_generators(new_model->get_number_of_lumped_wt_generators());
        set_rated_power_per_wt_generator_in_MW(new_model->get_rated_power_per_wt_generator_in_MW());
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wt generator model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}

void WT_GENERATOR::set_wt_aerodynamic_model(const WT_AERODYNAMIC_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WT AERODYNAMIC")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wt aerodynamic model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WT_AERODYNAMIC_MODEL* oldmodel = get_wt_aerodynamic_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wt_aerodynamic_model = NULL;
    }

    WT_AERODYNAMIC_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="AERD0")
    {
        AERD0* smodel = (AERD0*) (model);
        new_model = (WT_AERODYNAMIC_MODEL*) new AERD0(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wt_aerodynamic_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wt aerodynamic model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}

void WT_GENERATOR::set_wt_turbine_model(const WT_TURBINE_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WT TURBINE")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wt turbine model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WT_TURBINE_MODEL* oldmodel = get_wt_turbine_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wt_turbine_model = NULL;
    }

    WT_TURBINE_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="WT3T0")
    {
        WT3T0* smodel = (WT3T0*) (model);
        new_model = (WT_TURBINE_MODEL*) new WT3T0(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wt_turbine_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wt turbine model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}


void WT_GENERATOR::set_wt_electrical_model(const WT_ELECTRICAL_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WT ELECTRICAL")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wt electrical model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WT_ELECTRICAL_MODEL* oldmodel = get_wt_electrical_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wt_electrical_model = NULL;
    }

    WT_ELECTRICAL_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="WT3E0")
    {
        WT3E0* smodel = (WT3E0*) (model);
        new_model = (WT_ELECTRICAL_MODEL*) new WT3E0(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wt_electrical_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wt electrical model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}

void WT_GENERATOR::set_wt_pitch_model(const WT_PITCH_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WT PITCH")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wt pitch model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WT_PITCH_MODEL* oldmodel = get_wt_pitch_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wt_pitch_model = NULL;
    }

    WT_PITCH_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="WT3P1")
    {
        WT3P1* smodel = (WT3P1*) (model);
        new_model = (WT_PITCH_MODEL*) new WT3P1(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wt_pitch_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wt pitch model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}

void WT_GENERATOR::set_wind_speed_model(const WIND_SPEED_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="WIND SPEED")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up wind speed model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    WIND_SPEED_MODEL* oldmodel = get_wind_speed_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        wind_speed_model = NULL;
    }

    WIND_SPEED_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    /*if(model_name=="WBLINE")
    {
        WBLINE* smodel = (WBLINE*) (model);
        new_model = (WIND_SPEED_MODEL*) new WBLINE(*smodel);
    }*/

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        wind_speed_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append wind speed model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
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

void WT_GENERATOR::clear_wt_generator_model()
{
    if(wt_generator_model!=NULL)
    {
        delete wt_generator_model;
        wt_generator_model = NULL;
    }
}

void WT_GENERATOR::clear_wt_aerodynamic_model()
{
    if(wt_aerodynamic_model!=NULL)
    {
        delete wt_aerodynamic_model;
        wt_aerodynamic_model = NULL;
    }
}

void WT_GENERATOR::clear_wt_turbine_model()
{
    if(wt_turbine_model!=NULL)
    {
        delete wt_turbine_model;
        wt_turbine_model = NULL;
    }
}

void WT_GENERATOR::clear_wt_electrical_model()
{
    if(wt_electrical_model!=NULL)
    {
        delete wt_electrical_model;
        wt_electrical_model = NULL;
    }
}

void WT_GENERATOR::clear_wt_pitch_model()
{
    if(wt_pitch_model!=NULL)
    {
        delete wt_pitch_model;
        wt_pitch_model = NULL;
    }
}

void WT_GENERATOR::clear_wind_speed_model()
{
    if(wind_speed_model!=NULL)
    {
        delete wind_speed_model;
        wind_speed_model = NULL;
    }
}

WT_GENERATOR& WT_GENERATOR::operator=(const WT_GENERATOR& gen)
{
    if(this==(&gen)) return *this;

    clear();

    set_power_system_database(gen.get_power_system_database());

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



