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
    es_soc_model = NULL;
    es_auxiliary_model = NULL;
    es_primary_model = NULL;
    es_rotor_model = NULL;

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

        if(model->get_model_type()=="ES SOC")
        {
            set_es_soc_model((ES_SOC_MODEL*) model);
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

        if(model->get_model_type()=="ES AUXILIARY")
        {
            set_es_auxiliary_model((ES_AUXILIARY_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES ROTOR")
        {
            set_es_rotor_model((ES_ROTOR_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="ES PRIMARY")
        {
            set_es_primary_model((ES_PRIMARY_MODEL*) model);
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
    if(model_type=="ES SOC")
        return get_es_soc_model();
    if(model_type=="ES ELECTRICAL")
        return get_es_electrical_model();
    if(model_type=="ES VRT")
        return get_es_vrt_model();
    if(model_type=="ES RELAY")
        return get_es_relay_model();
    if(model_type=="ES AUXILIARY")
        return get_es_auxiliary_model();
    if(model_type=="ES ROTOR")
        return get_es_rotor_model();
    if(model_type=="ES PRIMARY")
        return get_es_primary_model();
    return NULL;
}

void ENERGY_STORAGE::set_es_converter_model(ES_CONVERTER_MODEL* model)
{
    if(model!=NULL)
        es_converter_model = model;
}

void ENERGY_STORAGE::set_es_soc_model(ES_SOC_MODEL* model)
{
    if(model!=NULL)
        es_soc_model = model;
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

void ENERGY_STORAGE::set_es_auxiliary_model(ES_AUXILIARY_MODEL* model)
{
    if(model!=NULL)
        es_auxiliary_model = model;
}

void ENERGY_STORAGE::set_es_rotor_model(ES_ROTOR_MODEL* model)
{
    if(model!=NULL)
        es_rotor_model = model;
}

void ENERGY_STORAGE::set_es_primary_model(ES_PRIMARY_MODEL* model)
{
    if(model!=NULL)
        es_primary_model = model;
}

ES_CONVERTER_MODEL* ENERGY_STORAGE::get_es_converter_model() const
{
    return es_converter_model;
}

ES_SOC_MODEL* ENERGY_STORAGE::get_es_soc_model() const
{
    return es_soc_model;
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

ES_AUXILIARY_MODEL* ENERGY_STORAGE::get_es_auxiliary_model() const
{
    return es_auxiliary_model;
}

ES_ROTOR_MODEL* ENERGY_STORAGE::get_es_rotor_model() const
{
    return es_rotor_model;
}

ES_PRIMARY_MODEL* ENERGY_STORAGE::get_es_primary_model() const
{
    return es_primary_model;
}

void ENERGY_STORAGE::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    if(get_status()==true)
    {
        ES_CONVERTER_MODEL* conv = get_es_converter_model();
        ES_ELECTRICAL_MODEL* elec = get_es_electrical_model();
        ES_SOC_MODEL* soc = get_es_soc_model();
        ES_VRT_MODEL* vrt = get_es_vrt_model();
        ES_RELAY_MODEL* relay = get_es_relay_model();
        ES_AUXILIARY_MODEL* auxiliary = get_es_auxiliary_model();
        ES_ROTOR_MODEL* rotor = get_es_rotor_model();
        ES_PRIMARY_MODEL* primary = get_es_primary_model();

        switch(mode)
        {
            case DYNAMIC_INITIALIZE_MODE:
            {

                initialize_energy_storage_mode();
                initialize_manual_power_in_pu();

                if(conv!=NULL and conv->is_model_active())
                    conv->initialize();
                else
                {
                    osstream<<"Error. No ES_CONVERTER_model is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }

                if(conv->is_simplified_model())
                {
                    unsigned int i = 0;
                    osstream<<"Warning. Simplified ES_CONVERTER_MODEL '"<<conv->get_model_name()<<"' is provided for "<<get_compound_device_name()<<"."<<endl
                            <<"The following models will be passed for dynamic simulation:"<<endl;
                    if(auxiliary!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_AUXILIARY_MODEL '"<<auxiliary->get_model_name()<<"'\n";
                    }
                    if(primary!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_PRIMARY_MODEL '"<<primary->get_model_name()<<"'\n";
                    }
                    if(rotor!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_ROTOR_MODEL '"<<rotor->get_model_name()<<"'\n";
                    }
                    if(elec!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_ELECTRICAL_MODEL '"<<elec->get_model_name()<<"'\n";
                    }
                    if(vrt!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_VRT_MODEL '"<<vrt->get_model_name()<<"'\n";
                    }
                    if(soc!=NULL)
                    {
                        ++i; osstream<<"("<<i<<") ES_SOC_MODEL '"<<soc->get_model_name()<<"'\n";
                    }
                    osstream<<"You can remove these models with no hesitation.";
                    if(i!=0)
                        toolkit.show_information_with_leading_time_stamp(osstream);
                }

                if(auxiliary!=NULL and auxiliary->is_model_active())
                {
                    auxiliary->initialize();
                    if(primary!=NULL and primary->get_model_name()=="ESB0")//battery storage
                    {
                        osstream<<"Warning. ES AUXILIARY model is not need exit for "<<get_compound_device_name()<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }

                }
                else
                {
                    if(primary!=NULL and primary->get_model_name()!="ESB0")//pumped storage
                    {
                        osstream<<"Warning. No ES AUXILIARY model is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }

                if(rotor!=NULL and rotor->is_model_active())
                {
                    rotor->initialize();
                    if(primary!=NULL and primary->get_model_name()=="ESB0")//battery storage
                    {
                        osstream<<"Warning. ES ROTOR model is not need exit for "<<get_compound_device_name()<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(primary!=NULL and primary->get_model_name()!="ESB0")//pumped storage
                    {
                        osstream<<"Error. No ES ROTOR model is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }

                if(elec!=NULL and elec->is_model_active())
                    elec->initialize();

                if(primary!=NULL and primary->is_model_active())
                    primary->initialize();

                if(vrt!=NULL and vrt->is_model_active())
                    vrt->initialize();

                if(relay!=NULL and relay->is_model_active())
                    relay->initialize();

                if(soc!=NULL and soc->is_model_active())
                    soc->initialize();

                break;
            }
            case DYNAMIC_INTEGRATE_MODE:
            case DYNAMIC_UPDATE_MODE:
            case DYNAMIC_UPDATE_TIME_STEP_MODE:
            {
                if(not conv->is_simplified_model())
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

                    if(auxiliary!=NULL and auxiliary->is_model_active())
                        auxiliary->run(mode);

                    if(primary!=NULL and primary->is_model_active())
                        primary->run(mode);

                    if(rotor!=NULL and rotor->is_model_active())
                        rotor->run(mode);

                    if(elec!=NULL and elec->is_model_active() and (not elec->is_model_bypassed()))
                        elec->run(mode);
                }

                if(conv!=NULL and conv->is_model_active())
                    conv->run(mode);

                if(soc!=NULL and soc->is_model_active())
                    soc->run(mode);

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
            <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar, "
            <<"Qmax_Nominal = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_nominal_in_MVar()<<" MVar, "
            <<"Qmin_Nominal = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_nominal_in_MVar()<<" MVar"<<endl;
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
    set_q_max_nominal_in_MVar(estorage.get_q_max_nominal_in_MVar());
    set_q_min_nominal_in_MVar(estorage.get_q_min_nominal_in_MVar());
    set_source_impedance_in_pu(estorage.get_source_impedance_in_pu());
    set_bus_to_regulate(estorage.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(estorage.get_voltage_to_regulate_in_pu());
    set_energy_storage_mode(estorage.get_energy_storage_mode());
    set_manual_power_in_pu(estorage.get_manual_power_in_pu());

    set_model(estorage.get_es_converter_model());
    set_model(estorage.get_es_electrical_model());
    set_model(estorage.get_es_vrt_model());
    set_model(estorage.get_es_relay_model());
    set_model(estorage.get_es_soc_model());
    set_model(estorage.get_es_auxiliary_model());
    set_model(estorage.get_es_primary_model());
    set_model(estorage.get_es_rotor_model());
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

void ENERGY_STORAGE::set_energy_storage_mode(ES_MODE m)
{
    this->mode = m;
}

ES_MODE ENERGY_STORAGE::get_energy_storage_mode() const
{
    return this->mode;
}

void ENERGY_STORAGE::set_manual_power_in_pu(double p)
{
    this->manual_power = p;
}

double ENERGY_STORAGE::get_manual_power_in_pu() const
{
    return this->manual_power;
}

void ENERGY_STORAGE::initialize_energy_storage_mode()
{
    double P = get_p_generation_in_MW();
    if(P>DOUBLE_EPSILON)
        set_energy_storage_mode(ES_DISCHARGE_MODE);
    else
    {
        if(P<-DOUBLE_EPSILON)
            set_energy_storage_mode(ES_CHARGE_MODE);
        else
            set_energy_storage_mode(ES_OUT_SERVICE_MODE);
    }
}

void ENERGY_STORAGE::initialize_manual_power_in_pu()
{
    double P = get_p_generation_in_MW();
    double mbase = get_mbase_in_MVA();
    set_manual_power_in_pu(P/mbase);
}
