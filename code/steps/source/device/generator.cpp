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
    clear();
    Aptr = NULL;
    Bptr = NULL;
    Cptr = NULL;
    Dptr = NULL;
}

GENERATOR::~GENERATOR()
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
    R1 = r;
    if(get_negative_sequence_resistance_in_pu()!=0.0)
        set_negative_sequence_resistance_in_pu(r);
    if(get_zero_sequence_resistance_in_pu()!=0.0)
        set_zero_sequence_resistance_in_pu(r);
}

void GENERATOR::set_positive_sequence_syncronous_reactance_in_pu(double x)
{
    X1_sync = x;
}

void GENERATOR::set_positive_sequence_transient_reactance_in_pu(double x)
{
    X1_transient = x;
}

void GENERATOR::set_positive_sequence_subtransient_reactance_in_pu(double x)
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

void GENERATOR::set_negative_sequence_resistance_in_pu(double r)
{
    R2 = r;
}

void GENERATOR::set_negative_sequence_reactance_in_pu(double x)
{
    X2 = x;
}

void GENERATOR::set_zero_sequence_resistance_in_pu(double r)
{
    R0 = r;
}

void GENERATOR::set_zero_sequence_reactance_in_pu(double x)
{
    X0 = x;
}

void GENERATOR::set_grounding_resistance_in_pu(double r)
{
    Rground = r;
}

void GENERATOR::set_grounding_reactance_in_pu(double x)
{
    Xground = x;
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
    return R1;
}

double GENERATOR::get_positive_sequence_syncronous_reactance_in_pu() const
{
    return X1_sync;
}

double GENERATOR::get_positive_sequence_transient_reactance_in_pu() const
{
    return X1_transient;
}

double GENERATOR::get_positive_sequence_subtransient_reactance_in_pu() const
{
    return X1_subtransient;
}

double GENERATOR::get_negative_sequence_resistance_in_pu() const
{
    return R2;
}

double GENERATOR::get_negative_sequence_reactance_in_pu() const
{
    return X2;
}

double GENERATOR::get_zero_sequence_resistance_in_pu() const
{
    return R0;
}

double GENERATOR::get_zero_sequence_reactance_in_pu() const
{
    return X0;
}

double GENERATOR::get_grounding_resistance_in_pu() const
{
    return Rground;
}

double GENERATOR::get_grounding_reactance_in_pu() const
{
    return Xground;
}

void GENERATOR::set_generator_reactance_option(GENERATOR_REACTANCE_OPTION option)
{
    gen_X_option = option;
}

GENERATOR_REACTANCE_OPTION GENERATOR::get_generator_reactance_option() const
{
    return gen_X_option;
}

void GENERATOR::update_internal_voltage_for_short_circuit_solver()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR_REACTANCE_OPTION option = get_generator_reactance_option();

    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_source_bus());

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

    internal_voltage_for_short_circuit = V + Z*I;
}

complex<double> GENERATOR::get_complex_internal_voltage_for_short_circuit_solver_in_pu()
{
    return internal_voltage_for_short_circuit;
}

complex<double> GENERATOR::get_positive_sequence_complex_current_in_pu()
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

complex<double> GENERATOR::get_negative_sequence_complex_current_in_pu()
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

complex<double> GENERATOR::get_zero_sequence_complex_current_in_pu()
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

complex<double> GENERATOR::get_positive_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_positive_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> GENERATOR::get_negative_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_negative_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> GENERATOR::get_zero_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_zero_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
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

    set_generator_reactance_option(SUBTRANSIENT_REACTANCE);
    set_sequence_parameter_import_flag(false);
}

DEVICE_ID GENERATOR::get_device_id() const
{
    DEVICE_ID did;

    did.set_device_type(STEPS_GENERATOR);

    TERMINAL terminal;
    terminal.append_bus(get_generator_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

void GENERATOR::set_model(MODEL* model)
{
    if(model!=NULL and model->has_allowed_device_type(STEPS_GENERATOR))
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

MODEL* GENERATOR::get_model_of_type(string model_type, unsigned int index)
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
                if(gen!=NULL and gen->is_model_active())
                    gen->initialize();
                else
                    return;

                STEPS& toolkit = gen->get_toolkit();
                POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
                if(comp!=NULL and comp->is_model_active())
                    comp->initialize();

                if(pss!=NULL and pss->is_model_active())
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

                if(exciter!=NULL and exciter->is_model_active())
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

                if(tg!=NULL and tg->is_model_active())
                    tg->initialize();

                if(tlc!=NULL and tlc->is_model_active())
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
            default:
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

    set_positive_sequence_resistance_in_pu(gen.get_positive_sequence_resistance_in_pu());
    set_positive_sequence_syncronous_reactance_in_pu(gen.get_positive_sequence_syncronous_reactance_in_pu());
    set_positive_sequence_transient_reactance_in_pu(gen.get_positive_sequence_transient_reactance_in_pu());
    set_positive_sequence_subtransient_reactance_in_pu(gen.get_positive_sequence_subtransient_reactance_in_pu());
    set_negative_sequence_resistance_in_pu(gen.get_negative_sequence_resistance_in_pu());
    set_negative_sequence_reactance_in_pu(gen.get_negative_sequence_reactance_in_pu());
    set_zero_sequence_resistance_in_pu(gen.get_zero_sequence_resistance_in_pu());
    set_zero_sequence_reactance_in_pu(gen.get_zero_sequence_reactance_in_pu());
    set_generator_reactance_option(gen.get_generator_reactance_option());
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

void GENERATOR::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}

bool GENERATOR::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}

void GENERATOR::initialize_ABCD_matrix_for_linearization()
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

void GENERATOR::build_linearized_matrix_ABCD()
{
    initialize_ABCD_matrix_for_linearization();

    STEPS_SPARSE_MATRIX Agen, Bgen, Cgen, Dgen;
    if(sync_generator_model != NULL)
    {
        sync_generator_model->build_linearized_matrix_ABCD();
        Agen = sync_generator_model->get_linearized_matrix_A();
        Bgen = sync_generator_model->get_linearized_matrix_B();
        Cgen = sync_generator_model->get_linearized_matrix_C();
        Dgen = sync_generator_model->get_linearized_matrix_D();
    }

    STEPS_SPARSE_MATRIX Aex, Bex, Cex, Dex;
    if(exciter_model != NULL)
    {
        exciter_model->build_linearized_matrix_ABCD();
        Aex = exciter_model->get_linearized_matrix_A();
        Bex = exciter_model->get_linearized_matrix_B();
        Cex = exciter_model->get_linearized_matrix_C();
        Dex = exciter_model->get_linearized_matrix_D();
    }

    STEPS_SPARSE_MATRIX Apss, Bpss, Cpss, Dpss;
    if(stabilizer_model != NULL)
    {
        stabilizer_model->build_linearized_matrix_ABCD();
        Apss = stabilizer_model->get_linearized_matrix_A();
        Bpss = stabilizer_model->get_linearized_matrix_B();
        Cpss = stabilizer_model->get_linearized_matrix_C();
        Dpss = stabilizer_model->get_linearized_matrix_D();
    }

    STEPS_SPARSE_MATRIX Agov, Bgov, Cgov, Dgov;
    if(turbine_governor_model != NULL)
    {
        turbine_governor_model->build_linearized_matrix_ABCD();
        Agov = turbine_governor_model->get_linearized_matrix_A();
        Bgov = turbine_governor_model->get_linearized_matrix_B();
        Cgov = turbine_governor_model->get_linearized_matrix_C();
        Dgov = turbine_governor_model->get_linearized_matrix_D();
    }

    STEPS_SPARSE_MATRIX Acomp, Bcomp, Ccomp, Dcomp;
    if(compensator_model != NULL)
    {
        compensator_model->build_linearized_matrix_ABCD();
        Acomp = compensator_model->get_linearized_matrix_A();
        Bcomp = compensator_model->get_linearized_matrix_B();
        Ccomp = compensator_model->get_linearized_matrix_C();
        Dcomp = compensator_model->get_linearized_matrix_D();
    }

    STEPS_SPARSE_MATRIX Atlc, Btlc, Ctlc, Dtlc;
    if(turbine_load_controller_model != NULL)
    {
        turbine_load_controller_model->build_linearized_matrix_ABCD();
        Atlc = turbine_load_controller_model->get_linearized_matrix_A();
        Btlc = turbine_load_controller_model->get_linearized_matrix_B();
        Ctlc = turbine_load_controller_model->get_linearized_matrix_C();
        Dtlc = turbine_load_controller_model->get_linearized_matrix_D();
    }

    vector<STEPS_SPARSE_MATRIX*> matrix;
    matrix.push_back(&Agen);
    matrix.push_back(&Aex);
    matrix.push_back(&Apss);
    matrix.push_back(&Agov);
    matrix.push_back(&Acomp);
    matrix.push_back(&Atlc);
    STEPS_SPARSE_MATRIX A = concatenate_matrix_diagnally(matrix);
    matrix.clear();

    matrix.push_back(&Bgen);
    matrix.push_back(&Bex);
    matrix.push_back(&Bpss);
    matrix.push_back(&Bgov);
    matrix.push_back(&Bcomp);
    matrix.push_back(&Btlc);
    STEPS_SPARSE_MATRIX B = concatenate_matrix_diagnally(matrix);
    matrix.clear();

    matrix.push_back(&Cgen);
    matrix.push_back(&Cex);
    matrix.push_back(&Cpss);
    matrix.push_back(&Cgov);
    matrix.push_back(&Ccomp);
    matrix.push_back(&Ctlc);
    STEPS_SPARSE_MATRIX C = concatenate_matrix_diagnally(matrix);
    matrix.clear();

    matrix.push_back(&Dgen);
    matrix.push_back(&Dex);
    matrix.push_back(&Dpss);
    matrix.push_back(&Dgov);
    matrix.push_back(&Dcomp);
    matrix.push_back(&Dtlc);
    STEPS_SPARSE_MATRIX D = concatenate_matrix_diagnally(matrix);
    matrix.clear();


    STEPS_SPARSE_MATRIX E, F, G, H;



    matrix.push_back(&A);
    matrix.push_back(&B);
    matrix.push_back(&C);
    matrix.push_back(&D);
    matrix.push_back(&E);
    matrix.push_back(&F);
    matrix.push_back(&G);
    matrix.push_back(&H);

    build_linearized_matrix_ABCD_with_basic_ABCD_and_EFGH(matrix);

    Aptr->report_brief();
    Bptr->report_brief();
    Cptr->report_brief();
    Dptr->report_brief();
}

void GENERATOR::build_linearized_matrix_ABCD_with_basic_ABCD_and_EFGH(vector<STEPS_SPARSE_MATRIX*> matrix)
{

}

STEPS_SPARSE_MATRIX GENERATOR::get_linearized_matrix_variable(char var) const
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
STEPS_SPARSE_MATRIX GENERATOR::get_linearized_matrix_A() const
{
    if(Aptr!=NULL) return *Aptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to A matrix exists in GENERATOR. Cannot return linearized A matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX GENERATOR::get_linearized_matrix_B() const
{
    if(Bptr!=NULL) return *Bptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to B matrix exists in GENERATOR. Cannot return linearized B matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX GENERATOR::get_linearized_matrix_C() const
{
    if(Cptr!=NULL) return *Cptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to C matrix exists in GENERATOR. Cannot return linearized C matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX GENERATOR::get_linearized_matrix_D() const
{
    if(Dptr!=NULL) return *Dptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to D matrix exists in GENERATOR. Cannot return linearized D matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX* GENERATOR::get_linearized_matrix_pointer_A()
{
    return Aptr;
}

STEPS_SPARSE_MATRIX* GENERATOR::get_linearized_matrix_pointer_B()
{
    return Bptr;
}

STEPS_SPARSE_MATRIX* GENERATOR::get_linearized_matrix_pointer_C()
{
    return Cptr;
}

STEPS_SPARSE_MATRIX* GENERATOR::get_linearized_matrix_pointer_D()
{
    return Dptr;
}
