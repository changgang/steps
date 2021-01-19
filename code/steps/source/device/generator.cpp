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

GENERATOR::GENERATOR(STEPS& toolkit) : SOURCE(toolkit)
{
    other_gen_vars = new GENERATOR_VAR();
    clear();
}

GENERATOR::GENERATOR(const GENERATOR& gen) : SOURCE(gen.get_toolkit())
{
    other_gen_vars = new GENERATOR_VAR();
    copy_from_const_generator(gen);
}

GENERATOR::~GENERATOR()
{
    delete other_gen_vars;
}

void GENERATOR::set_generator_bus(unsigned int bus)
{
    set_source_bus(bus);
}

void GENERATOR::set_generator_impedance_in_pu(const complex<double>& z_pu)
{
    set_source_impedance_in_pu(z_pu);
    set_positive_sequence_resistance_in_pu(z_pu.real());
    set_positive_sequence_subtransient_reactance_in_pu(z_pu.imag());
}

void GENERATOR::set_positive_sequence_resistance_in_pu(double r)
{
    other_gen_vars->R1 = r;
    if(get_negative_sequence_resistance_in_pu()!=0.0)
        set_negative_sequence_resistance_in_pu(r);
    if(get_zero_sequence_resistance_in_pu()!=0.0)
        set_zero_sequence_resistance_in_pu(r);
}

void GENERATOR::set_positive_sequence_syncronous_reactance_in_pu(double x)
{
    other_gen_vars->X1_sync = x;
}

void GENERATOR::set_positive_sequence_transient_reactance_in_pu(double x)
{
    other_gen_vars->X1_transient = x;
}

void GENERATOR::set_positive_sequence_subtransient_reactance_in_pu(double x)
{
    other_gen_vars->X1_subtransient = x;
    if(get_positive_sequence_syncronous_reactance_in_pu()==0.0)
        set_positive_sequence_syncronous_reactance_in_pu(x);
    if(get_positive_sequence_transient_reactance_in_pu()==0.0)
        set_positive_sequence_transient_reactance_in_pu(x);
    if(get_negative_sequence_reactance_in_pu()==0.0)
        set_negative_sequence_reactance_in_pu(x);
    if(get_zero_sequence_reactance_in_pu()==0.0)
        set_zero_sequence_reactance_in_pu(x);
}

void GENERATOR::set_negative_sequence_resistance_in_pu(double r)
{
    other_gen_vars->R2 = r;
}

void GENERATOR::set_negative_sequence_reactance_in_pu(double x)
{
    other_gen_vars->X2 = x;
}

void GENERATOR::set_zero_sequence_resistance_in_pu(double r)
{
    other_gen_vars->R0 = r;
}

void GENERATOR::set_zero_sequence_reactance_in_pu(double x)
{
    other_gen_vars->X0 = x;
}

void GENERATOR::set_grounding_resistance_in_pu(double r)
{
    other_gen_vars->Rground = r;
}

void GENERATOR::set_grounding_reactance_in_pu(double x)
{
    other_gen_vars->Xground = x;
}

unsigned int GENERATOR::get_generator_bus() const
{
    return get_source_bus();
}

complex<double> GENERATOR::get_generator_impedance_in_pu() const
{
    return get_source_impedance_in_pu();
}

double GENERATOR::get_positive_sequence_resistance_in_pu() const
{
    return other_gen_vars->R1;
}

double GENERATOR::get_positive_sequence_syncronous_reactance_in_pu() const
{
    return other_gen_vars->X1_sync;
}

double GENERATOR::get_positive_sequence_transient_reactance_in_pu() const
{
    return other_gen_vars->X1_transient;
}

double GENERATOR::get_positive_sequence_subtransient_reactance_in_pu() const
{
    return other_gen_vars->X1_subtransient;
}

double GENERATOR::get_negative_sequence_resistance_in_pu() const
{
    return other_gen_vars->R2;
}

double GENERATOR::get_negative_sequence_reactance_in_pu() const
{
    return other_gen_vars->X2;
}

double GENERATOR::get_zero_sequence_resistance_in_pu() const
{
    return other_gen_vars->R0;
}

double GENERATOR::get_zero_sequence_reactance_in_pu() const
{
    return other_gen_vars->X0;
}

double GENERATOR::get_grounding_resistance_in_pu() const
{
    return other_gen_vars->Rground;
}

double GENERATOR::get_grounding_reactance_in_pu() const
{
    return other_gen_vars->Xground;
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

    sync_generator_model_disabled = false;
    compensator_model_disabled = false;
    exciter_model_disabled = false;
    stabilizer_model_disabled = false;
    turbine_governor_model_disabled = false;
    turbine_load_controller_model_disabled = false;

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

void GENERATOR::set_model(MODEL* model)
{
    if(model!=NULL and model->has_allowed_device_type("GENERATOR"))
    {
        model->set_device_id(get_device_id());
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
        STEPS& toolkit = get_toolkit();
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
    return NULL;
}

void GENERATOR::disable_mode_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="SYNC GENERATOR")
    {
        sync_generator_model_disabled = true;
        return;
    }
    if(model_type=="COMPENSATOR")
    {
        compensator_model_disabled = true;
        return;
    }
    if(model_type=="EXCITER")
    {
        exciter_model_disabled = true;
        return;
    }
    if(model_type=="STABILIZER")
    {
        stabilizer_model_disabled = true;
        return;
    }
    if(model_type=="TURBINE GOVERNOR")
    {
        turbine_governor_model_disabled = true;
        return;
    }
    if(model_type=="TURBINE LOAD CONTROLLER")
    {
        turbine_load_controller_model_disabled = true;
        return;
    }
}


void GENERATOR::enable_mode_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="SYNC GENERATOR")
    {
        sync_generator_model_disabled = false;
        return;
    }
    if(model_type=="COMPENSATOR")
    {
        compensator_model_disabled = false;
        return;
    }
    if(model_type=="EXCITER")
    {
        exciter_model_disabled = false;
        return;
    }
    if(model_type=="STABILIZER")
    {
        stabilizer_model_disabled = false;
        return;
    }
    if(model_type=="TURBINE GOVERNOR")
    {
        turbine_governor_model_disabled = false;
        return;
    }
    if(model_type=="TURBINE LOAD CONTROLLER")
    {
        turbine_load_controller_model_disabled = false;
        return;
    }
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
    if(not sync_generator_model_disabled)
        return sync_generator_model;
    else
        return NULL;
}

COMPENSATOR_MODEL* GENERATOR::get_compensator_model() const
{
    if(not compensator_model_disabled)
        return compensator_model;
    else
        return NULL;
}

EXCITER_MODEL* GENERATOR::get_exciter_model() const
{
    if(not exciter_model_disabled)
        return exciter_model;
    else
        return NULL;
}

STABILIZER_MODEL* GENERATOR::get_stabilizer_model() const
{
    if(not stabilizer_model_disabled)
        return stabilizer_model;
    else
        return NULL;
}

TURBINE_GOVERNOR_MODEL* GENERATOR::get_turbine_governor_model() const
{
    if(not turbine_governor_model_disabled)
        return turbine_governor_model;
    else
        return NULL;
}

TURBINE_LOAD_CONTROLLER_MODEL* GENERATOR::get_turbine_load_controller_model() const
{
    if(not turbine_load_controller_model_disabled)
        return turbine_load_controller_model;
    else
        return NULL;
}

void GENERATOR::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    if(get_status()==true)
    {
        COMPENSATOR_MODEL* comp = get_compensator_model();
        STABILIZER_MODEL* pss = get_stabilizer_model();
        EXCITER_MODEL* exciter = get_exciter_model();
        TURBINE_LOAD_CONTROLLER_MODEL* tlc = get_turbine_load_controller_model();
        TURBINE_GOVERNOR_MODEL* tg = get_turbine_governor_model();
        SYNC_GENERATOR_MODEL* gen = get_sync_generator_model();
        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                if(gen!=NULL)
                    gen->initialize();
                else
                    return;

                STEPS& toolkit = gen->get_toolkit();
                POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
                if(comp!=NULL)
                    comp->initialize();

                if(pss!=NULL)
                {
                    if(gen->get_model_name()=="GENCLS")
                    {
                        osstream<<"Warning. Stabilizer model "<<pss->get_model_name()<<" is incompatible with generator model GENCLS for "<<get_compound_device_name()<<"["<<psdb.bus_number2bus_name(get_generator_bus())<<"]\n"
                                <<"Stabilizer model "<<pss->get_model_name()<<" will be removed.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        pss->deactivate_model();
                    }
                    else
                    {
                        pss->initialize();
                    }
                }

                if(exciter!=NULL)
                {
                    if(gen->get_model_name()=="GENCLS")
                    {
                        osstream<<"Warning. Exciter model "<<exciter->get_model_name()<<" is incompatible with generator model GENCLS for "<<get_compound_device_name()<<"["<<psdb.bus_number2bus_name(get_generator_bus())<<"]\n"
                                <<"Exciter model "<<exciter->get_model_name()<<" will be removed.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        exciter->deactivate_model();
                    }
                    else
                        exciter->initialize();
                }

                if(tg!=NULL)
                    tg->initialize();

                if(tlc!=NULL)
                    tlc->initialize();

                break;
            }
            case INTEGRATE_MODE:
            case UPDATE_MODE:
            {
                if(comp!=NULL and comp->is_model_active())
                    comp->run(mode);

                if(pss!=NULL and pss->is_model_active())
                    pss->run(mode);

                if(exciter!=NULL and exciter->is_model_active())
                    exciter->run(mode);

                if(tlc!=NULL and tlc->is_model_active())
                    tlc->run(mode);

                if(tg!=NULL and tg->is_model_active())
                    tg->run(mode);

                if(gen!=NULL and gen->is_model_active())
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
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar"<<endl
      <<"Zsource = "<<setw(8)<<setprecision(6)<<fixed<<get_generator_impedance_in_pu();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void GENERATOR::save() const
{
    ;
}

GENERATOR& GENERATOR::operator=(const GENERATOR& gen)
{
    if(this==(&gen)) return *this;

    SOURCE::operator=(gen);

    other_gen_vars = new GENERATOR_VAR();

    clear();
    copy_from_const_generator(gen);

    return *this;
}

void GENERATOR::copy_from_const_generator(const GENERATOR& gen)
{
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
    set_generator_impedance_in_pu(gen.get_generator_impedance_in_pu());
    set_bus_to_regulate(gen.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(gen.get_voltage_to_regulate_in_pu());

    set_model(gen.get_sync_generator_model());
    set_model(gen.get_compensator_model());
    set_model(gen.get_exciter_model());
    set_model(gen.get_stabilizer_model());
    set_model(gen.get_turbine_governor_model());
    set_model(gen.get_turbine_load_controller_model());
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
        STEPS& toolkit = get_toolkit();
        double mbase = get_mbase_in_MVA();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        complex<double> I = generator_model->get_terminal_current_in_pu_based_on_mbase();
        I *= (mbase*one_over_sbase);
        return I;
    }
    else
        return 0.0;
}


