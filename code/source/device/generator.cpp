#include "header/device/generator.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"

#include "header/model/sg_models/sync_generator_model/sync_generator_models.h"
#include "header/model/sg_models/compensator_model/compensator_models.h"
#include "header/model/sg_models/exciter_model/exciter_models.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_models.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_models.h"
#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_models.h"

#include <istream>
#include <iostream>

using namespace std;

GENERATOR::GENERATOR() : SOURCE()
{
    clear();
}

GENERATOR::~GENERATOR()
{
}

void GENERATOR::set_generator_bus(unsigned int bus)
{
    set_source_bus(bus);
}

void GENERATOR::set_generator_impedance_in_pu(const complex<double>& z_pu)
{
    set_source_impedance_in_pu(z_pu);
}

unsigned int GENERATOR::get_generator_bus() const
{
    return get_source_bus();
}

complex<double> GENERATOR::get_generator_impedance_in_pu() const
{
    return get_source_impedance_in_pu();
}

void GENERATOR::clear()
{
    SOURCE::clear();

    sync_generator_model = NULL;
    compensator_model = NULL;
    exciter_model = NULL;
    stabilizer_model = NULL;
    turbine_governor_model = NULL;
    turbine_load_controller_model = NULL;
}

DEVICE_ID GENERATOR::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");

    TERMINAL terminal;
    terminal.append_bus(get_generator_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

void GENERATOR::set_model(const MODEL* model)
{
    if(model!=NULL and model->has_allowed_device_type("GENERATOR"))
    {
        if(model->get_model_type()=="SYNC GENERATOR")
        {
            set_sync_generator_model((SYNC_GENERATOR_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="COMPENSATOR")
        {
            set_compensator_model((COMPENSATOR_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="EXCITER")
        {
            set_exciter_model((EXCITER_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="STABILIZER")
        {
            set_stabilizer_model((STABILIZER_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="TURBINE GOVERNOR")
        {
            set_turbine_governor_model((TURBINE_GOVERNOR_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="TURBINE LOAD CONTROLLER")
        {
            set_turbine_load_controller_model((TURBINE_LOAD_CONTROLLER_MODEL*) model);
            return;
        }
        ostringstream osstream;
        osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up generator-related model.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* GENERATOR::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="SYNC GENERATOR")
        return get_sync_generator_model();
    if(model_type=="COMPENSATOR")
        return get_compensator_model();
    if(model_type=="EXCITER")
        return get_exciter_model();
    if(model_type=="STABILIZER")
        return get_stabilizer_model();
    if(model_type=="TURBINE GOVERNOR")
        return get_turbine_governor_model();
    if(model_type=="TURBINE LOAD CONTROLLER")
        return get_turbine_load_controller_model();
    return nullptr;
}

void GENERATOR::set_sync_generator_model(SYNC_GENERATOR_MODEL* model)
{
    if(model!=NULL)
        sync_generator_model = model;
}

void GENERATOR::set_compensator_model(COMPENSATOR_MODEL* model)
{
    if(model!=NULL)
        compensator_model = model;
}

void GENERATOR::set_exciter_model(EXCITER_MODEL* model)
{
    if(model!=NULL)
        exciter_model = model;
}

void GENERATOR::set_stabilizer_model(STABILIZER_MODEL* model)
{
    if(model!=NULL)
        stabilizer_model = model;
}

void GENERATOR::set_turbine_governor_model(TURBINE_GOVERNOR_MODEL* model)
{
    if(model!=NULL)
        turbine_governor_model = model;
}

void GENERATOR::set_turbine_load_controller_model(TURBINE_LOAD_CONTROLLER_MODEL* model)
{
    if(model!=NULL)
        turbine_load_controller_model = model;
}

SYNC_GENERATOR_MODEL* GENERATOR::get_sync_generator_model() const
{
    return sync_generator_model;
}

COMPENSATOR_MODEL* GENERATOR::get_compensator_model() const
{
    return compensator_model;
}

EXCITER_MODEL* GENERATOR::get_exciter_model() const
{
    return exciter_model;
}

STABILIZER_MODEL* GENERATOR::get_stabilizer_model() const
{
    return stabilizer_model;
}

TURBINE_GOVERNOR_MODEL* GENERATOR::get_turbine_governor_model() const
{
    return turbine_governor_model;
}

TURBINE_LOAD_CONTROLLER_MODEL* GENERATOR::get_turbine_load_controller_model() const
{
    return turbine_load_controller_model;
}

void GENERATOR::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    if(get_status()==true)
    {
        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                SYNC_GENERATOR_MODEL* gen = get_sync_generator_model();
                if(gen!=NULL)
                    gen->initialize();
                else
                    return;
                STEPS& toolkit = gen->get_toolkit(__PRETTY_FUNCTION__);
                POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
                COMPENSATOR_MODEL* comp = get_compensator_model();
                if(comp!=NULL)
                    comp->initialize();
                STABILIZER_MODEL* pss = get_stabilizer_model();
                if(pss!=NULL)
                {
                    if(gen->get_model_name()=="GENCLS")
                    {
                        osstream<<"Warning. Stabilizer model "<<pss->get_model_name()<<" is incompatible with generator model GENCLS for "<<get_device_name()<<"["<<psdb.bus_number2bus_name(get_generator_bus())<<"]\n"
                                <<"Stabilizer model "<<pss->get_model_name()<<" will be removed.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        pss->deactivate_model();
                    }
                    else
                        pss->initialize();
                }
                EXCITER_MODEL* exciter = get_exciter_model();
                if(exciter!=NULL)
                {
                    if(gen->get_model_name()=="GENCLS")
                    {
                        osstream<<"Warning. Exciter model "<<exciter->get_model_name()<<" is incompatible with generator model GENCLS for "<<get_device_name()<<"["<<psdb.bus_number2bus_name(get_generator_bus())<<"]\n"
                                <<"Exciter model "<<exciter->get_model_name()<<" will be removed.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        exciter->deactivate_model();
                    }
                    else
                        exciter->initialize();
                }
                TURBINE_GOVERNOR_MODEL* tg = get_turbine_governor_model();
                if(tg!=NULL)
                    tg->initialize();
                TURBINE_LOAD_CONTROLLER_MODEL* tlc = get_turbine_load_controller_model();
                if(tlc!=NULL)
                    tlc->initialize();

                break;
            }
            case INTEGRATE_MODE:
            case UPDATE_MODE:
            {
                COMPENSATOR_MODEL* comp = get_compensator_model();
                if(comp!=NULL)
                    comp->run(mode);

                STABILIZER_MODEL* pss = get_stabilizer_model();
                if(pss!=NULL)
                    pss->run(mode);

                EXCITER_MODEL* exciter = get_exciter_model();
                if(exciter!=NULL)
                    exciter->run(mode);

                TURBINE_LOAD_CONTROLLER_MODEL* tlc = get_turbine_load_controller_model();
                if(tlc!=NULL)
                    tlc->run(mode);

                TURBINE_GOVERNOR_MODEL* tg = get_turbine_governor_model();
                if(tg!=NULL)
                    tg->run(mode);

                SYNC_GENERATOR_MODEL* gen = get_sync_generator_model();
                if(gen!=NULL)
                    gen->run(mode);
                break;
            }
            case RELAY_MODE:
            {
                break;
            }
        }
    }
}

void GENERATOR::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar"<<endl
      <<"Zsource = "<<setw(8)<<setprecision(6)<<fixed<<get_generator_impedance_in_pu();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void GENERATOR::save() const
{
    ;
}

GENERATOR& GENERATOR::operator=(const GENERATOR& gen)
{
    if(this==(&gen)) return *this;

    clear();

    set_toolkit(gen.get_toolkit(__PRETTY_FUNCTION__));

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
    set_generator_impedance_in_pu(gen.get_generator_impedance_in_pu());
    set_bus_to_regulate(gen.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(gen.get_voltage_to_regulate_in_pu());

    return *this;
}

complex<double> GENERATOR::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    SYNC_GENERATOR_MODEL* generator_model = get_sync_generator_model();
    if(generator_model!=NULL)
        return generator_model->get_internal_voltage_in_pu_in_xy_axis();
    else
        return 0.0;
}


complex<double> GENERATOR::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    ostringstream osstream;

    SYNC_GENERATOR_MODEL* generator_model = get_sync_generator_model();

    if(get_status()==true and generator_model!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        double mbase = get_mbase_in_MVA();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        complex<double> I = generator_model->get_terminal_current_in_pu_based_on_mbase();
        I *= (mbase*one_over_sbase);
        return I;
    }
    else
        return 0.0;
}


