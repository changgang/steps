#include "header/device/pe_source.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/source_model/pe_source_model/pe_source_models.h"

#include <iostream>

using namespace std;

PE_SOURCE::PE_SOURCE(POWER_SYSTEM_DATABASE* psdb) : SOURCE(psdb)
{
    ostringstream sstream;
    if(psdb==NULL)
    {
        sstream<<"Error. PE_SOURCE object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(sstream);
    }
    set_power_system_database(psdb);
    clear();
    set_regulating_mode(REGULATING_PV);

    pe_source_model = NULL;
}

PE_SOURCE::~PE_SOURCE()
{
    ;
}

void PE_SOURCE::clear()
{
    SOURCE::clear();
    set_source_type("");
}

DEVICE_ID PE_SOURCE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("PE SOURCE");

    TERMINAL terminal;
    terminal.append_bus(get_source_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string PE_SOURCE::get_device_name() const
{
    return get_device_id().get_device_name();
}*/


void PE_SOURCE::report() const
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

void PE_SOURCE::save() const
{
    ;
}

void PE_SOURCE::set_source_type(string type)
{
    if(get_source_type()=="")
    {
        type = string2upper(type);
        if(type=="WIND SOURCE" or type=="PV SOURCE" or type=="BATTERY SOURCE")
            source_type = type;
    }
}

string PE_SOURCE::get_source_type() const
{
    return source_type;
}

void PE_SOURCE::set_model(const MODEL* model)
{
    if(model == NULL)
        return;

    if(model->get_allowed_device_type()!="PE SOURCE")
        return;
    if(model->get_model_type()=="PE SOURCE")
    {
        set_pe_source_model((PE_SOURCE_MODEL*) model);
        return;
    }

    ostringstream sstream;
    sstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up wind-turbine generator-related model.";
    show_information_with_leading_time_stamp(sstream);
}

void PE_SOURCE::set_pe_source_model(const PE_SOURCE_MODEL* model)
{
    if(model==NULL)
        return;
    if(model->get_model_type()!="PE SOURCE")
    {
        ostringstream sstream;
        sstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up pe source model.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    PE_SOURCE_MODEL* oldmodel = get_pe_source_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        pe_source_model = NULL;
    }

    PE_SOURCE_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="WT3G2")
    {
        WT3G2* smodel = (WT3G2*) (model);
        new_model = (PE_SOURCE_MODEL*) new WT3G2(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        pe_source_model = new_model;
    }
    else
    {
        ostringstream sstream;
        sstream<<"Warning. Model '"<<model_name<<"' is not supported when append pe source model of "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(sstream);
    }
}


PE_SOURCE_MODEL* PE_SOURCE::get_pe_source_model()
{
    return pe_source_model;
}

void PE_SOURCE::clear_pe_source_model()
{
    if(pe_source_model!=NULL)
    {
        delete pe_source_model;
        pe_source_model = NULL;
    }
}

PE_SOURCE& PE_SOURCE::operator=(const PE_SOURCE& gen)
{
    if(this==(&gen)) return *this;

    clear();

    set_power_system_database(gen.get_power_system_database());

    set_source_bus(gen.get_source_bus());
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

    return *this;
}

complex<double> PE_SOURCE::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    return 0.0;
}

complex<double> PE_SOURCE::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    return 0.0;
}



