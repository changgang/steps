#include "header/device/sv_source.h"
#include "header/basic/utility.h"

#include "header/STEPS.h"

#include <istream>
#include <iostream>

using namespace std;

SV_SOURCE::SV_SOURCE(STEPS& toolkit) : SOURCE(toolkit)
{
    clear();
}

SV_SOURCE::~SV_SOURCE()
{
    ;
}

void SV_SOURCE::set_sv_source_bus(unsigned int bus)
{
    set_source_bus(bus);
}

unsigned int SV_SOURCE::get_sv_source_bus() const
{
    return get_source_bus();
}

void SV_SOURCE::clear()
{
    SOURCE::clear();

    svs_control_model = NULL;

    sequence_parameter_import_flag = false;
}

DEVICE_ID SV_SOURCE::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_SV_SOURCE);

    TERMINAL terminal;
    terminal.append_bus(get_sv_source_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

void SV_SOURCE::set_model(MODEL* model)
{
    if(model!=NULL and model->has_allowed_device_type(STEPS_SV_SOURCE))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="SVS CONTROL")
        {
            set_svs_control_model((SVS_CONTROL_MODEL*) model);
            return;
        }

        ostringstream osstream;
        osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up sv source-related model.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* SV_SOURCE::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="SVS CONTROL")
        return get_svs_control_model();
    return NULL;
}

void SV_SOURCE::set_svs_control_model(SVS_CONTROL_MODEL* model)
{
    if(model!=NULL)
        svs_control_model = model;
}

SVS_CONTROL_MODEL* SV_SOURCE::get_svs_control_model() const
{
    return svs_control_model;
}

void SV_SOURCE::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    if(get_status()==true)
    {
        SVS_CONTROL_MODEL* svs_control_model = get_svs_control_model();

        switch(mode)
        {
            case DYNAMIC_INITIALIZE_MODE:
            {

                if(svs_control_model!=NULL and svs_control_model->is_model_active())
                    svs_control_model->initialize();
                else
                {
                    osstream<<"Error. No SVS_CONTROL_MODEL is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
                break;
            }
            case DYNAMIC_INTEGRATE_MODE:
            case DYNAMIC_UPDATE_MODE:
            case DYNAMIC_UPDATE_TIME_STEP_MODE:
            {
                if(svs_control_model!=NULL and svs_control_model->is_model_active())
                    svs_control_model->run(mode);
                break;
            }
            case DYNAMIC_RELAY_MODE:
            {
                break;
            }
            default:
                break;
        }
    }
}

void SV_SOURCE::report() const
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

void SV_SOURCE::save() const
{
    ;
}

SV_SOURCE& SV_SOURCE::operator=(const SV_SOURCE& sv_source)
{
    if(this==(&sv_source)) return *this;

    set_toolkit(sv_source.get_toolkit());
    clear();

    set_sv_source_bus(sv_source.get_sv_source_bus());
    set_identifier(sv_source.get_identifier());

    copy_basic_data_from_sv_source(sv_source);

    set_model(sv_source.get_svs_control_model());
    return *this;
}

void SV_SOURCE::copy_basic_data_from_sv_source(const SV_SOURCE& sv_source)
{
    set_status(sv_source.get_status());
    set_mbase_in_MVA(sv_source.get_mbase_in_MVA());
    set_p_generation_in_MW(sv_source.get_p_generation_in_MW());
    set_q_generation_in_MVar(sv_source.get_q_generation_in_MVar());
    set_p_max_in_MW(sv_source.get_p_max_in_MW());
    set_p_min_in_MW(sv_source.get_p_min_in_MW());
    set_q_max_in_MVar(sv_source.get_q_max_in_MVar());
    set_q_min_in_MVar(sv_source.get_q_min_in_MVar());
    set_q_max_nominal_in_MVar(sv_source.get_q_max_nominal_in_MVar());
    set_q_min_nominal_in_MVar(sv_source.get_q_min_nominal_in_MVar());
    set_source_impedance_in_pu(sv_source.get_source_impedance_in_pu());
    set_bus_to_regulate(sv_source.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(sv_source.get_voltage_to_regulate_in_pu());
}

complex<double> SV_SOURCE::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    // disabled as private
    return 0.0;
}


complex<double> SV_SOURCE::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    // disabled as private
    return 0.0;
}

void SV_SOURCE::set_sequence_parameter_import_flag(bool flag)
{
    sequence_parameter_import_flag = flag;
}
bool SV_SOURCE::get_sequence_parameter_import_flag() const
{
    return sequence_parameter_import_flag;
}
