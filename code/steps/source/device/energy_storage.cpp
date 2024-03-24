#include "header/device/energy_storage.h"
#include "header/basic/utility.h"

#include "header/STEPS.h"

#include <istream>
#include <iostream>

using namespace std;

ENERGY_STORAGE::ENERGY_STORAGE(STEPS& toolkit) : SOURCE(toolkit)
{
    clear();
}

ENERGY_STORAGE::~ENERGY_STORAGE()
{
    ;
}

void ENERGY_STORAGE::set_energy_storage_bus(unsigned int bus)
{
    set_source_bus(bus);
}

void ENERGY_STORAGE::set_energy_storage_impedance_in_pu(const complex<double>& z_pu)
{
    set_source_impedance_in_pu(z_pu);
}

unsigned int ENERGY_STORAGE::get_energy_storage_bus() const
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

    es_converter_model = NULL;
    es_electrical_model = NULL;
    es_vrt_model = NULL;
    es_relay_model = NULL;
    es_battery_model = NULL;

    sequence_parameter_import_flag = false;
}

DEVICE_ID ENERGY_STORAGE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_ENERGY_STORAGE);

    TERMINAL terminal;
    terminal.append_bus(get_energy_storage_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

void ENERGY_STORAGE::set_model(MODEL* model)
{
    if(model!=NULL and model->has_allowed_device_type(STEPS_ENERGY_STORAGE))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="ES CONVERTER")
        {
            set_es_converter_model((ES_CONVERTER_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES BATTERY")
        {
            set_es_battery_model((ES_BATTERY_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES ELECTRICAL")
        {
            set_es_electrical_model((ES_ELECTRICAL_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES VRT")
        {
            set_es_vrt_model((ES_VRT_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES RELAY")
        {
            set_es_relay_model((ES_RELAY_MODEL*) model);
            return;
        }

        ostringstream osstream;
        osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up energy storage-related model.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* ENERGY_STORAGE::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="ES CONVERTER")
        return get_es_converter_model();
    if(model_type=="ES BATTERY")
        return get_es_battery_model();
    if(model_type=="ES ELECTRICAL")
        return get_es_electrical_model();
    if(model_type=="ES VRT")
        return get_es_vrt_model();
    if(model_type=="ES RELAY")
        return get_es_relay_model();
    return NULL;
}

void ENERGY_STORAGE::set_es_converter_model(ES_CONVERTER_MODEL* model)
{
    if(model!=NULL)
        es_converter_model = model;
}

void ENERGY_STORAGE::set_es_battery_model(ES_BATTERY_MODEL* model)
{
    if(model!=NULL)
        es_battery_model = model;
}

void ENERGY_STORAGE::set_es_electrical_model(ES_ELECTRICAL_MODEL* model)
{
    if(model!=NULL)
        es_electrical_model = model;
}

void ENERGY_STORAGE::set_es_vrt_model(ES_VRT_MODEL* model)
{
    if(model!=NULL)
        es_vrt_model = model;
}

void ENERGY_STORAGE::set_es_relay_model(ES_RELAY_MODEL* model)
{
    if(model!=NULL)
        es_relay_model = model;
}

ES_CONVERTER_MODEL* ENERGY_STORAGE::get_es_converter_model() const
{
    return es_converter_model;
}

ES_BATTERY_MODEL* ENERGY_STORAGE::get_es_battery_model() const
{
    return es_battery_model;
}

ES_ELECTRICAL_MODEL* ENERGY_STORAGE::get_es_electrical_model() const
{
    return es_electrical_model;
}

ES_VRT_MODEL* ENERGY_STORAGE::get_es_vrt_model() const
{
    return es_vrt_model;
}

ES_RELAY_MODEL* ENERGY_STORAGE::get_es_relay_model() const
{
    return es_relay_model;
}

void ENERGY_STORAGE::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    if(get_status()==true)
    {
        ES_CONVERTER_MODEL* conv = get_es_converter_model();
        ES_ELECTRICAL_MODEL* elec = get_es_electrical_model();
        ES_BATTERY_MODEL* battery = get_es_battery_model();
        ES_VRT_MODEL* vrt = get_es_vrt_model();
        ES_RELAY_MODEL* relay = get_es_relay_model();

        switch(mode)
        {
            case DYNAMIC_INITIALIZE_MODE:
            {
                if(conv!=NULL and conv->is_model_active())
                    conv->initialize();
                else
                {
                    osstream<<"Error. No ES CONVERTER model is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }

                if(elec!=NULL and elec->is_model_active())
                    elec->initialize();

                if(battery!=NULL and battery->is_model_active())
                    battery->initialize();

                if(vrt!=NULL and vrt->is_model_active())
                    vrt->initialize();

                if(relay!=NULL and relay->is_model_active())
                    relay->initialize();

                break;
            }
            case DYNAMIC_INTEGRATE_MODE:
            case DYNAMIC_UPDATE_MODE:
            case DYNAMIC_UPDATE_TIME_STEP_MODE:
            {
                if(vrt!=NULL and vrt->is_model_active())
                {
                    vrt->run(mode);
                    if(elec!=NULL and elec->is_model_active())
                    {
                        if(elec->is_model_bypassed()
                            and (not vrt->is_in_vrt_status()))
                            elec->unbypass_model();
                        else
                        {
                            if(not elec->is_model_bypassed()
                                and (vrt->is_in_vrt_status()))
                                elec->bypass_model();
                        }
                    }
                }

                if(battery!=NULL and battery->is_model_active())
                    battery->run(mode);

                if(elec!=NULL and elec->is_model_active() and (not elec->is_model_bypassed()))
                    elec->run(mode);

                if(conv!=NULL and conv->is_model_active())
                    conv->run(mode);
                break;
            }
            case DYNAMIC_RELAY_MODE:
            {
                if(relay!=NULL and relay->is_model_active())
                    relay->run(mode);
                break;
            }
            default:
                break;
        }
    }
}

void ENERGY_STORAGE::report() const
{
    ostringstream osstream;
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
            <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
            <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
            <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
            <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
            <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
            <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
            <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ENERGY_STORAGE::save() const
{
    ;
}

ENERGY_STORAGE& ENERGY_STORAGE::operator=(const ENERGY_STORAGE& estorage)
{
    if(this==(&estorage)) return *this;

    set_toolkit(estorage.get_toolkit());
    clear();

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

    set_model(estorage.get_es_converter_model());
    set_model(estorage.get_es_electrical_model());
    set_model(estorage.get_es_vrt_model());
    set_model(estorage.get_es_relay_model());
    set_model(estorage.get_es_vrt_model());
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

void ENERGY_STORAGE::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool ENERGY_STORAGE::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}
