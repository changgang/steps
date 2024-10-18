#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ES_CONVERTER_MODEL::ES_CONVERTER_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{
    set_current_source_flag(true);
    set_max_active_current_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
    set_max_active_power_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
    set_min_active_current_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
    set_min_active_power_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
}

ES_CONVERTER_MODEL::~ES_CONVERTER_MODEL()
{
    ;
}

string ES_CONVERTER_MODEL::get_model_type() const
{
    return "ES CONVERTER";
}


void ES_CONVERTER_MODEL::set_current_source_flag(bool flag)
{
    current_source_flag = flag;
}

bool ES_CONVERTER_MODEL::get_current_source_flag() const
{
    return current_source_flag;
}

bool ES_CONVERTER_MODEL::is_current_source() const
{
    return current_source_flag;
}

bool ES_CONVERTER_MODEL::is_voltage_source() const
{
    return not is_current_source();
}

double ES_CONVERTER_MODEL::get_active_current_command_in_pu_based_on_mbase()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    ES_VRT_MODEL* vrt_model = energy_storage->get_es_vrt_model();
    ES_SOC_MODEL* soc_model = energy_storage->get_es_soc_model();
    double Ipcmd = 0.0;

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        Ipcmd = elec_model->get_active_current_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            Ipcmd = vrt_model->get_active_current_command_in_pu_based_on_mbase();
        else
            Ipcmd = get_initial_active_current_command_in_pu_based_on_mbase();
    }

    //SOC limit output
    if(soc_model!=NULL and soc_model->is_model_initialized())
    {
        double SOC = soc_model->get_state_of_charge_in_pu();
        double SOC_max = soc_model->get_max_SOC_in_pu();
        double SOC_min = soc_model->get_min_SOC_in_pu();

        if(SOC>SOC_min and SOC<SOC_max)
        {
            set_max_active_current_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
            set_min_active_current_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
        }
        else
        {
            if(SOC>=SOC_max)
            {
                set_min_active_current_command_in_pu_based_on_mbase(0.0);
                set_max_active_current_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
            }
            else
            {
                set_max_active_current_command_in_pu_based_on_mbase(0.0);
                set_min_active_current_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
            }
        }

        double Ipcmd_max = get_max_active_current_command_in_pu_based_on_mbase();
        double Ipcmd_min = get_min_active_current_command_in_pu_based_on_mbase();
        if(Ipcmd>=Ipcmd_min and Ipcmd<=Ipcmd_max)
            ;
        else
        {
            if(Ipcmd>Ipcmd_max)
                Ipcmd = Ipcmd_max;
            else
                Ipcmd = Ipcmd_min;
        }
    }
    return Ipcmd;
}

double ES_CONVERTER_MODEL::get_reactive_current_command_in_pu_based_on_mbase()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    ES_VRT_MODEL* vrt_model = energy_storage->get_es_vrt_model();

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_current_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_current_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_current_command_in_pu_based_on_mbase();
    }
}

double ES_CONVERTER_MODEL::get_reactive_voltage_command_in_pu()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    ES_VRT_MODEL* vrt_model = energy_storage->get_es_vrt_model();

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_voltage_command_in_pu();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_voltage_command_in_pu();
        else
            return get_initial_reactive_voltage_command_in_pu();
    }
}

double ES_CONVERTER_MODEL::get_active_power_command_in_pu_based_on_mbase()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    ES_VRT_MODEL* vrt_model = energy_storage->get_es_vrt_model();
    ES_SOC_MODEL* soc_model = energy_storage->get_es_soc_model();
    double Pcmd = 0.0;

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        Pcmd = elec_model->get_active_power_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            Pcmd = vrt_model->get_active_power_command_in_pu_based_on_mbase();
        else
            Pcmd = get_initial_active_power_command_in_pu_based_on_mbase();
    }

    //SOC limit output
    if(soc_model!=NULL and soc_model->is_model_initialized())
    {
        double SOC = soc_model->get_state_of_charge_in_pu();
        double SOC_max = soc_model->get_max_SOC_in_pu();
        double SOC_min = soc_model->get_min_SOC_in_pu();

        if(SOC>SOC_min and SOC<SOC_max)
        {
            set_max_active_power_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
            set_min_active_power_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
        }
        else
        {
            if(SOC>=SOC_max)
            {
                set_min_active_power_command_in_pu_based_on_mbase(0.0);
                set_max_active_power_command_in_pu_based_on_mbase(INFINITE_THRESHOLD);
            }
            else
            {
                set_max_active_power_command_in_pu_based_on_mbase(0.0);
                set_min_active_power_command_in_pu_based_on_mbase(-INFINITE_THRESHOLD);
            }
        }

        double Pcmd_max = get_max_active_power_command_in_pu_based_on_mbase();
        double Pcmd_min = get_min_active_power_command_in_pu_based_on_mbase();
        if(Pcmd>=Pcmd_min and Pcmd<=Pcmd_max)
            ;
        else
        {
            if(Pcmd>Pcmd_max)
                Pcmd = Pcmd_max;
            else
                Pcmd = Pcmd_min;
        }
    }

    return Pcmd;

}

double ES_CONVERTER_MODEL::get_reactive_power_command_in_pu_based_on_mbase()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    ES_VRT_MODEL* vrt_model = energy_storage->get_es_vrt_model();

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_power_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_power_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_power_command_in_pu_based_on_mbase();
    }
}

void ES_CONVERTER_MODEL::set_max_active_current_command_in_pu_based_on_mbase(double Ipcmd)
{
    this->max_active_current_command_in_pu_based_on_mbase = Ipcmd;
}

void ES_CONVERTER_MODEL::set_min_active_current_command_in_pu_based_on_mbase(double Ipcmd)
{
    this->min_active_current_command_in_pu_based_on_mbase = Ipcmd;
}

void ES_CONVERTER_MODEL::set_max_active_power_command_in_pu_based_on_mbase(double Pcmd)
{
    this->max_active_power_command_in_pu_based_on_mbase = Pcmd;
}

void ES_CONVERTER_MODEL::set_min_active_power_command_in_pu_based_on_mbase(double Pcmd)
{
    this->min_active_power_command_in_pu_based_on_mbase = Pcmd;
}

double ES_CONVERTER_MODEL::get_max_active_current_command_in_pu_based_on_mbase() const
{
    return this->max_active_current_command_in_pu_based_on_mbase;
}

double ES_CONVERTER_MODEL::get_min_active_current_command_in_pu_based_on_mbase() const
{
    return this->min_active_current_command_in_pu_based_on_mbase;
}

double ES_CONVERTER_MODEL::get_max_active_power_command_in_pu_based_on_mbase() const
{
    return this->max_active_power_command_in_pu_based_on_mbase;
}

double ES_CONVERTER_MODEL::get_min_active_power_command_in_pu_based_on_mbase() const
{
    return this->min_active_power_command_in_pu_based_on_mbase;
}

complex<double> ES_CONVERTER_MODEL::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> ES_CONVERTER_MODEL::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double ES_CONVERTER_MODEL::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double ES_CONVERTER_MODEL::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double ES_CONVERTER_MODEL::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double ES_CONVERTER_MODEL::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

complex<double> ES_CONVERTER_MODEL::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return Ixy*(mbase*one_over_sbase);
}

double ES_CONVERTER_MODEL::get_terminal_current_in_pu_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    double I = get_terminal_current_in_pu_based_on_mbase();
    return I*(mbase*one_over_sbase);
}

double ES_CONVERTER_MODEL::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double ES_CONVERTER_MODEL::get_active_power_generation_including_stator_loss_in_MW()
{
    return get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

complex<double> ES_CONVERTER_MODEL::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> I = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return I*(mbase*one_over_sbase);
}
