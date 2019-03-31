#include "header/device/generator.h"
#include "header/basic/utility.h"

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

    sync_generator_model = NULL;
    compensator_model = NULL;
    exciter_model = NULL;
    stabilizer_model = NULL;
    turbine_governor_model = NULL;
    turbine_load_controller_model = NULL;
}

GENERATOR::~GENERATOR()
{
    clear();
    if(sync_generator_model!=NULL)
        delete sync_generator_model;

    if(compensator_model!=NULL)
        delete compensator_model;

    if(exciter_model!=NULL)
        delete exciter_model;

    if(stabilizer_model!=NULL)
        delete stabilizer_model;

    if(turbine_governor_model!=NULL)
        delete turbine_governor_model;

    if(turbine_load_controller_model!=NULL)
        delete turbine_load_controller_model;
}

void GENERATOR::set_generator_bus(size_t bus)
{
    set_source_bus(bus);
}

void GENERATOR::set_generator_impedance_in_pu(complex<double> z_pu)
{
    set_source_impedance_in_pu(z_pu);
}

size_t GENERATOR::get_generator_bus() const
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

/*string GENERATOR::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

void GENERATOR::set_model(const MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_allowed_device_type()!="GENERATOR")
        return;

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
    show_information_with_leading_time_stamp(osstream);
}

void GENERATOR::set_sync_generator_model(const SYNC_GENERATOR_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="SYNC GENERATOR")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up sync generator model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    SYNC_GENERATOR_MODEL* oldmodel = get_sync_generator_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        sync_generator_model = NULL;
    }

    SYNC_GENERATOR_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="GENCLS")
    {
        GENCLS* smodel = (GENCLS*) (model);
        new_model = (SYNC_GENERATOR_MODEL*) new GENCLS(*smodel);
    }
    if(model_name=="GENROU")
    {
        GENROU* smodel = (GENROU*) (model);
        new_model = (SYNC_GENERATOR_MODEL*) new GENROU(*smodel);
    }
    if(model_name=="GENSAL")
    {
        GENSAL* smodel = (GENSAL*) (model);
        new_model = (SYNC_GENERATOR_MODEL*) new GENSAL(*smodel);
    }

    if(new_model!=NULL)
    {

        new_model->set_device_id(get_device_id());
        sync_generator_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append sync generator model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void GENERATOR::set_compensator_model(const COMPENSATOR_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="COMPENSATOR")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up compensator model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    COMPENSATOR_MODEL* oldmodel = get_compensator_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        compensator_model=NULL;
    }

    COMPENSATOR_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="COMP")
    {
        COMP* smodel = (COMP*) (model);
        new_model = (COMPENSATOR_MODEL*) new COMP(*smodel);
    }

    if(new_model!=NULL)
    {

        new_model->set_device_id(get_device_id());
        compensator_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append compensator model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void GENERATOR::set_exciter_model(const EXCITER_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="EXCITER")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up exciter model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    EXCITER_MODEL* oldmodel = get_exciter_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        exciter_model=NULL;
    }

    EXCITER_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="SEXS")
    {
        SEXS* smodel = (SEXS*) (model);
        new_model = (EXCITER_MODEL*) new SEXS(*smodel);
    }
    if(model_name=="IEEET1")
    {
        IEEET1* smodel = (IEEET1*) (model);
        new_model = (EXCITER_MODEL*) new IEEET1(*smodel);
    }
    if(model_name=="PSASPE1")
    {
        PSASPE1* smodel = (PSASPE1*) (model);
        new_model = (EXCITER_MODEL*) new PSASPE1(*smodel);
    }
    if(model_name=="PSASPE2")
    {
        PSASPE2* smodel = (PSASPE2*) (model);
        new_model = (EXCITER_MODEL*) new PSASPE2(*smodel);
    }
    if(model_name=="PSASPE13")
    {
        PSASPE13* smodel = (PSASPE13*) (model);
        new_model = (EXCITER_MODEL*) new PSASPE13(*smodel);
    }
    if(model_name=="CSEET1")
    {
        CSEET1* smodel = (CSEET1*) (model);
        new_model = (EXCITER_MODEL*) new CSEET1(*smodel);
    }
    if(model_name=="CSEET2")
    {
        CSEET2* smodel = (CSEET2*) (model);
        new_model = (EXCITER_MODEL*) new CSEET2(*smodel);
    }

    if(new_model!=NULL)
    {

        new_model->set_device_id(get_device_id());
        exciter_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append exciter model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void GENERATOR::set_stabilizer_model(const STABILIZER_MODEL* model)
{
    ostringstream osstream;
    if(model==NULL)
        return;

    if(model->get_model_type()!="STABILIZER")
    {
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up stabilizer model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    STABILIZER_MODEL* oldmodel = get_stabilizer_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        stabilizer_model=NULL;
    }

    STABILIZER_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="IEE2ST")
    {
        IEE2ST* smodel = (IEE2ST*) (model);
        new_model = (STABILIZER_MODEL*) new IEE2ST(*smodel);
    }

    if(new_model!=NULL)
    {

        new_model->set_device_id(get_device_id());
        stabilizer_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append stabilizer model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void GENERATOR::set_turbine_governor_model(const TURBINE_GOVERNOR_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="TURBINE GOVERNOR")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up turbine governor model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    TURBINE_GOVERNOR_MODEL* oldmodel = get_turbine_governor_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        turbine_governor_model=NULL;
    }

    TURBINE_GOVERNOR_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="TGOV1")
    {
        TGOV1* smodel = (TGOV1*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new TGOV1(*smodel);
    }
    if(model_name=="IEEEG1")
    {
        IEEEG1* smodel = (IEEEG1*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new IEEEG1(*smodel);
    }
    if(model_name=="IEEEG2")
    {
        IEEEG2* smodel = (IEEEG2*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new IEEEG2(*smodel);
    }
    if(model_name=="IEEEG3")
    {
        IEEEG3* smodel = (IEEEG3*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new IEEEG3(*smodel);
    }
    if(model_name=="IEESGO")
    {
        IEESGO* smodel = (IEESGO*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new IEESGO(*smodel);
    }
    if(model_name=="IEEEG1SB")
    {
        IEEEG1SB* smodel = (IEEEG1SB*) (model);
        new_model = (TURBINE_GOVERNOR_MODEL*) new IEEEG1SB(*smodel);
    }

    if(new_model!=NULL)
    {

        new_model->set_device_id(get_device_id());
        turbine_governor_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append turbine governor model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void GENERATOR::set_turbine_load_controller_model(const TURBINE_LOAD_CONTROLLER_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="TURBINE LOAD CONTROLLER")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up turbine load controller model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    TURBINE_LOAD_CONTROLLER_MODEL* oldmodel = get_turbine_load_controller_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        turbine_load_controller_model=NULL;
    }

    TURBINE_LOAD_CONTROLLER_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="LCFB1")
    {
        LCFB1* smodel = (LCFB1*) (model);
        new_model = (TURBINE_LOAD_CONTROLLER_MODEL*) new LCFB1(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_device_id(get_device_id());
        turbine_load_controller_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append turbine load controller model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}


void GENERATOR::clear_sync_generator_model()
{
    if(sync_generator_model!=NULL)
    {
        delete sync_generator_model;
        sync_generator_model = NULL;
    }
}
void GENERATOR::clear_compensator_model()
{
    if(compensator_model!=NULL)
    {
        delete compensator_model;
        compensator_model = NULL;
    }
}
void GENERATOR::clear_exciter_model()
{
    if(exciter_model!=NULL)
    {
        delete exciter_model;
        exciter_model = NULL;
    }
}
void GENERATOR::clear_stabilizer_model()
{
    if(stabilizer_model!=NULL)
    {
        delete stabilizer_model;
        stabilizer_model = NULL;
    }
}
void GENERATOR::clear_turbine_governor_model()
{
    if(turbine_governor_model!=NULL)
    {
        delete turbine_governor_model;
        turbine_governor_model = NULL;
    }
}
void GENERATOR::clear_turbine_load_controller_model()
{
    if(turbine_load_controller_model!=NULL)
    {
        delete turbine_load_controller_model;
        turbine_load_controller_model = NULL;
    }
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
    if(get_status()==false)
        return;

    switch(mode)
    {
        case INITIALIZE_MODE:
        {
            SYNC_GENERATOR_MODEL* gen = get_sync_generator_model();
            if(gen==NULL)
                return;
            gen->initialize();

            COMPENSATOR_MODEL* comp = get_compensator_model();
            if(comp!=NULL)
                comp->initialize();

            STABILIZER_MODEL* pss = get_stabilizer_model();
            if(pss!=NULL)
                pss->initialize();

            EXCITER_MODEL* exciter = get_exciter_model();
            if(exciter!=NULL)
                exciter->initialize();

            TURBINE_GOVERNOR_MODEL* tg = get_turbine_governor_model();
            if(tg!=NULL)
                tg->initialize();

            TURBINE_LOAD_CONTROLLER_MODEL* tlc = get_turbine_load_controller_model();
            if(tlc!=NULL)
                tlc->initialize();

            break;
        }
        default:
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
    show_information_with_leading_time_stamp(osstream);
}

void GENERATOR::save() const
{
    ;
}

GENERATOR& GENERATOR::operator=(const GENERATOR& gen)
{
    if(this==(&gen)) return *this;

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

    if(get_status()==false or generator_model==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    complex<double> I = generator_model->get_terminal_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();
    double sbase = psdb.get_system_base_power_in_MVA();
    I *= (mbase/sbase);
    return I;
}


