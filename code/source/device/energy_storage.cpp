#include "header/device/energy_storage.h"
#include "header/basic/utility.h"

#include "header/model/energy_storage_model/energy_storage_models.h"

#include <istream>
#include <iostream>

using namespace std;

ENERGY_STORAGE::ENERGY_STORAGE(POWER_SYSTEM_DATABASE* psdb) : SOURCE(psdb)
{
    ostringstream osstream;
    if(psdb==NULL)
    {
        osstream<<"Error. ENERGY STORAGE object cannot be constructed since NULL power system database is given."<<endl
          <<"Operations on the object is unpredictable.";
        show_information_with_leading_time_stamp(osstream);
    }
    clear();
    energy_storage_model = NULL;
}

ENERGY_STORAGE::~ENERGY_STORAGE()
{
    clear();
    if(energy_storage_model!=NULL)
        delete energy_storage_model;
}

void ENERGY_STORAGE::set_energy_storage_bus(size_t bus)
{
    set_source_bus(bus);
}

void ENERGY_STORAGE::set_energy_storage_impedance_in_pu(complex<double> z_pu)
{
    set_source_impedance_in_pu(z_pu);
}

size_t ENERGY_STORAGE::get_energy_storage_bus() const
{
    return get_source_bus();
}

complex<double> ENERGY_STORAGE::get_energy_storage_impedance_in_pu() const
{
    return get_source_impedance_in_pu();
}

void ENERGY_STORAGE::clear()
{
    SOURCE::clear();
}

DEVICE_ID ENERGY_STORAGE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");

    TERMINAL terminal;
    terminal.append_bus(get_energy_storage_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string ENERGY_STORAGE::get_device_name() const
{
    return get_device_id().get_device_name();
}*/

void ENERGY_STORAGE::set_model(const MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_allowed_device_type()!="ENERGY STORAGE")
        return;

    if(model->get_model_type()=="ENERGY STORAGE")
    {
        set_energy_storage_model((ENERGY_STORAGE_MODEL*) model);
        return;
    }
    ostringstream osstream;
    osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up energy storage-related model.";
    show_information_with_leading_time_stamp(osstream);
}

void ENERGY_STORAGE::set_energy_storage_model(const ENERGY_STORAGE_MODEL* model)
{
    if(model==NULL)
        return;

    if(model->get_model_type()!="ENERGY STORAGE")
    {
        ostringstream osstream;
        osstream<<"Warning. Model of type '"<<model->get_model_type()<<"' is not allowed when setting up energy storage model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    ENERGY_STORAGE_MODEL* oldmodel = get_energy_storage_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        energy_storage_model = NULL;
    }

    ENERGY_STORAGE_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="ESTR0")
    {
        ESTR0* smodel = (ESTR0*) (model);
        new_model = (ENERGY_STORAGE_MODEL*) new ESTR0(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_power_system_database(get_power_system_database());
        new_model->set_device_id(get_device_id());
        energy_storage_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append energy storage model of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
    }
}

void ENERGY_STORAGE::clear_energy_storage_model()
{
    if(energy_storage_model!=NULL)
    {
        delete energy_storage_model;
        energy_storage_model = NULL;
    }
}

ENERGY_STORAGE_MODEL* ENERGY_STORAGE::get_energy_storage_model() const
{
    return energy_storage_model;
}

void ENERGY_STORAGE::run(DYNAMIC_MODE mode)
{
    if(get_status()==false)
        return;

    switch(mode)
    {
        case INITIALIZE_MODE:
        {
            ENERGY_STORAGE_MODEL* estorage = get_energy_storage_model();
            if(estorage==NULL)
                return;
            estorage->initialize();

            break;
        }
        default:
        {
            ENERGY_STORAGE_MODEL* estorage = get_energy_storage_model();
            if(estorage!=NULL)
                estorage->run(mode);
            break;
        }
    }
}

void ENERGY_STORAGE::report() const
{
    ostringstream osstream;
    osstream<<get_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar";
    show_information_with_leading_time_stamp(osstream);
}

void ENERGY_STORAGE::save() const
{
    ;
}

ENERGY_STORAGE& ENERGY_STORAGE::operator=(const ENERGY_STORAGE& estorage)
{
    if(this==(&estorage)) return *this;

    clear();
    set_power_system_database(estorage.get_power_system_database());
    set_energy_storage_bus(estorage.get_energy_storage_bus());
    set_identifier(estorage.get_identifier());
    set_status(estorage.get_status());
    set_mbase_in_MVA(estorage.get_mbase_in_MVA());
    set_p_generation_in_MW(estorage.get_p_generation_in_MW());
    set_q_generation_in_MVar(estorage.get_q_generation_in_MVar());
    set_p_max_in_MW(estorage.get_p_max_in_MW());
    set_p_min_in_MW(estorage.get_p_min_in_MW());
    set_q_max_in_MVar(estorage.get_q_max_in_MVar());
    set_q_min_in_MVar(estorage.get_q_min_in_MVar());
    set_source_impedance_in_pu(estorage.get_source_impedance_in_pu());
    set_bus_to_regulate(estorage.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(estorage.get_voltage_to_regulate_in_pu());

    return *this;
}

complex<double> ENERGY_STORAGE::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    // disabled as private
    return 0.0;
}


complex<double> ENERGY_STORAGE::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    // disabled as private
    return 0.0;
}


