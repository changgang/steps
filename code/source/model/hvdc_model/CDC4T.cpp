#include "header/model/hvdc_model/CDC4T.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
CDC4T::CDC4T()
{
    clear();
}

CDC4T::~CDC4T()
{
    ;
}

void CDC4T::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_converter_dynamic_max_alpha_or_gamma_in_deg(RECTIFIER, 90.0);
    set_converter_dynamic_max_alpha_or_gamma_in_deg(INVERTER, 90.0);
    inverter_dc_voltage_sensor.set_limiter_type(NO_LIMITER);
    inverter_dc_voltage_sensor.set_K(1.0);
    dc_current_sensor.set_limiter_type(NO_LIMITER);
    dc_current_sensor.set_K(1.0);
}

void CDC4T::copy_from_const_model(const CDC4T& model)
{
    clear();
    this->set_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER,model.get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER));
    this->set_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER,model.get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER));
    this->set_inverter_dc_voltage_sensor_T_in_s(model.get_inverter_dc_voltage_sensor_T_in_s());
    this->set_dc_current_sensor_T_in_s(model.get_dc_current_sensor_T_in_s());
    this->set_rectifier_ac_instantaneous_blocking_voltage_in_pu(model.get_rectifier_ac_instantaneous_blocking_voltage_in_pu());
    this->set_rectifier_ac_instantaneous_unblocking_voltage_in_pu(model.get_rectifier_ac_instantaneous_unblocking_voltage_in_pu());
    this->set_mininum_blocking_time_in_s(model.get_mininum_blocking_time_in_s());
    this->set_inverter_dc_instantaneous_bypassing_voltage_in_kV(model.get_inverter_dc_instantaneous_bypassing_voltage_in_kV());
    this->set_inverter_ac_instantaneous_unbypassing_voltage_in_pu(model.get_inverter_ac_instantaneous_unbypassing_voltage_in_pu());
    this->set_mininum_bypassing_time_in_s(model.get_mininum_bypassing_time_in_s());
    this->set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(model.get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing());
    this->set_minimum_dc_current_in_kA_following_unblocking(model.get_minimum_dc_current_in_kA_following_unblocking());
    this->set_dc_voltage_command_recovery_rate_in_pu_per_second(model.get_dc_voltage_command_recovery_rate_in_pu_per_second());
    this->set_dc_current_command_recovery_rate_in_pu_per_second(model.get_dc_current_command_recovery_rate_in_pu_per_second());
    this->set_minimum_dc_current_command_in_kA(model.get_minimum_dc_current_command_in_kA());
    this->set_VDCOL(model.get_VDCOL());
    this->set_minimum_time_in_switched_mode_in_s(model.get_minimum_time_in_switched_mode_in_s());
}

CDC4T::CDC4T(const CDC4T& model) : HVDC_MODEL()
{
    copy_from_const_model(model);
}

CDC4T& CDC4T::operator=(const CDC4T& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string CDC4T::get_model_name() const
{
    return "CDC4T";
}

void CDC4T::set_inverter_dc_voltage_sensor_T_in_s(double t)
{
    inverter_dc_voltage_sensor.set_T_in_s(t);
}

void CDC4T::set_dc_current_sensor_T_in_s(double t)
{
    dc_current_sensor.set_T_in_s(t);
}

void CDC4T::set_rectifier_ac_instantaneous_blocking_voltage_in_pu(double v)
{
    rectifier_ac_instantaneous_blocking_voltage_in_pu = v;
}

void CDC4T::set_rectifier_ac_instantaneous_unblocking_voltage_in_pu(double v)
{
    rectifier_ac_instantaneous_unblocking_voltage_in_pu = v;
}

void CDC4T::set_inverter_dc_instantaneous_bypassing_voltage_in_kV(double v)
{
    inverter_dc_instantaneous_bypassing_voltage_in_kV = v;
}

void CDC4T::set_inverter_ac_instantaneous_unbypassing_voltage_in_pu(double v)
{
    inverter_ac_instantaneous_unbypassing_voltage_in_pu = v;
}


double CDC4T::get_inverter_dc_voltage_sensor_T_in_s() const
{
    return inverter_dc_voltage_sensor.get_T_in_s();
}

double CDC4T::get_dc_current_sensor_T_in_s() const
{
    return dc_current_sensor.get_T_in_s();
}

double CDC4T::get_rectifier_ac_instantaneous_blocking_voltage_in_pu() const
{
    return rectifier_ac_instantaneous_blocking_voltage_in_pu;
}

double CDC4T::get_rectifier_ac_instantaneous_unblocking_voltage_in_pu() const
{
    return rectifier_ac_instantaneous_unblocking_voltage_in_pu;
}

double CDC4T::get_inverter_dc_instantaneous_bypassing_voltage_in_kV() const
{
    return inverter_dc_instantaneous_bypassing_voltage_in_kV;
}

double CDC4T::get_inverter_ac_instantaneous_unbypassing_voltage_in_pu() const
{
    return inverter_ac_instantaneous_unbypassing_voltage_in_pu;
}


bool CDC4T::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<24)
        return is_successful;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;

    // iblcok iunblock for instantenous block or unblock
    // dblcok dunblock for delayed block or unblock
    // ibupass iunbupass for instantenous bupass or unbupass
    // dbupass dunbupass for delayed bupass or unbupass
    double min_alpha, min_gamma, tvdci, tidc, vac_iblock, vac_iunblock, t_block,
           vdc_ibypass, vac_iunbypass, t_bypass,
           vdcmin_unblock, idcmin_unblock, vdc_ramp, idc_ramp,
           c0, v1, c1, v2, c2, v3, c3,  t_switched;


    size_t i=2;
    min_alpha = get_double_data(data[i],"0.0"); i++;
    min_gamma = get_double_data(data[i],"0.0"); i++;
    tvdci = get_double_data(data[i],"0.0"); i++;
    tidc = get_double_data(data[i],"0.0"); i++;
    vac_iblock = get_double_data(data[i],"0.0"); i++;
    vac_iunblock = get_double_data(data[i],"0.0"); i++;
    t_block = get_double_data(data[i],"0.0"); i++;
    vdc_ibypass = get_double_data(data[i],"0.0"); i++;
    vac_iunbypass = get_double_data(data[i],"0.0"); i++;
    t_bypass = get_double_data(data[i],"0.0"); i++;
    vdcmin_unblock = get_double_data(data[i],"0.0"); i++;
    idcmin_unblock = get_double_data(data[i],"0.0"); i++;
    vdc_ramp = get_double_data(data[i],"0.0"); i++;
    idc_ramp = get_double_data(data[i],"0.0"); i++;
    c0 = get_double_data(data[i],"0.0"); i++;
    v1 = get_double_data(data[i],"0.0"); i++;
    c1 = get_double_data(data[i],"0.0"); i++;
    v2 = get_double_data(data[i],"0.0"); i++;
    c2 = get_double_data(data[i],"0.0"); i++;
    v3 = get_double_data(data[i],"0.0"); i++;
    c3 = get_double_data(data[i],"0.0"); i++;
    t_switched = get_double_data(data[i],"0.0");

    set_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER, min_alpha);
    set_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER, min_gamma);
    set_inverter_dc_voltage_sensor_T_in_s(tvdci);
    set_dc_current_sensor_T_in_s(tidc);
    set_rectifier_ac_instantaneous_blocking_voltage_in_pu(vac_iblock);
    set_rectifier_ac_instantaneous_unblocking_voltage_in_pu(vac_iunblock);
    set_mininum_blocking_time_in_s(t_block);
    set_inverter_dc_instantaneous_bypassing_voltage_in_kV(vdc_ibypass);
    set_inverter_ac_instantaneous_unbypassing_voltage_in_pu(vac_iunbypass);
    set_mininum_bypassing_time_in_s(t_bypass);
    set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(vdcmin_unblock);
    set_minimum_dc_current_in_kA_following_unblocking(idcmin_unblock);
    set_dc_voltage_command_recovery_rate_in_pu_per_second(vdc_ramp);
    set_dc_current_command_recovery_rate_in_pu_per_second(idc_ramp);
    set_minimum_dc_current_command_in_kA(c0*0.001);

    VDCOL limiter;
    limiter.append_vdcol_point_in_kV_kA(v1, c1*0.001);
    limiter.append_vdcol_point_in_kV_kA(v2, c2*0.001);
    limiter.append_vdcol_point_in_kV_kA(v3, c3*0.001);
    set_VDCOL(limiter);

    set_minimum_time_in_switched_mode_in_s(t_switched);

    is_successful = true;

    return is_successful;
}

bool CDC4T::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool CDC4T::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void CDC4T::setup_block_toolkit_and_parameters()
{
    set_common_timer_toolkit();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    inverter_dc_voltage_sensor.set_toolkit(toolkit);
    dc_current_sensor.set_toolkit(toolkit);
}

void CDC4T::initialize()
{
    if(is_model_initialized())
        return;

    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    setup_block_toolkit_and_parameters();

    set_attached_device_of_common_meters();

    double Vdci = hvdc->get_converter_dc_voltage_in_kV(INVERTER);
    inverter_dc_voltage_sensor.set_output(Vdci);
    inverter_dc_voltage_sensor.initialize();

    double Idc = hvdc->get_line_dc_current_in_kA();
    dc_current_sensor.set_output(Idc);
    dc_current_sensor.initialize();

    set_flag_model_initialized_as_true();
}

void CDC4T::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    //solve_hvdc_model_without_integration();

    double Vdci = hvdc->get_converter_dc_voltage_in_kV(INVERTER);
    inverter_dc_voltage_sensor.set_input(Vdci);
    inverter_dc_voltage_sensor.run(mode);

    double Idc = hvdc->get_line_dc_current_in_kA();
    dc_current_sensor.set_input(Idc);
    dc_current_sensor.run(mode);

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void CDC4T::solve_hvdc_model_without_integration()
{
    ostringstream osstream;

    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    check_blocking_logic();
    check_bypassing_logic();
    check_mode_switching_logic();

    if(not is_blocked())
    {
        double Vdci_measured = inverter_dc_voltage_sensor.get_output();
        double Idc_measured = dc_current_sensor.get_output();
        double Icommand = get_rectifier_dc_current_command_in_kA(Vdci_measured, Idc_measured);
        double Vcommand = get_inverter_dc_voltage_command_in_kV();
        solve_hvdc_model_without_line_dynamics(Icommand, Vcommand);
    }
}

void CDC4T::check_blocking_logic()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_manual_blocked())
        return;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double TIME = toolkit.get_dynamic_simulation_time_in_s();

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t bus_r = hvdc->get_converter_bus(RECTIFIER);
    double vac_r = psdb.get_bus_voltage_in_pu(bus_r);

    double vblock = get_rectifier_ac_instantaneous_blocking_voltage_in_pu();
    double vunblock = get_rectifier_ac_instantaneous_unblocking_voltage_in_pu();

    ostringstream osstream;
    if(not is_blocked())
    {
        if(vac_r<vblock)
        {
            osstream<<hvdc->get_device_name()<<" will be blocked at time "<<TIME<<" s due to drop of rectifier AC voltage."<<endl;
            osstream<<"Rectifier AC voltage is "<<vac_r<<" pu, and AC blocking voltage threshold is "<<vblock<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            block_hvdc();
        }
    }
    else
    {
        if(is_block_timer_timed_out() and vac_r>vunblock)
        {
            osstream<<hvdc->get_device_name()<<" block timer is timed out, and will unblock at time "<<TIME<<" s due to recovery of rectifier AC voltage."<<endl;
            osstream<<"Rectifier AC voltage is "<<vac_r<<" pu, and AC unblocking voltage threshold is "<<vunblock<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            unblock_hvdc();
        }
    }
}

void CDC4T::check_bypassing_logic()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    if(is_manual_bypassed())
        return;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double TIME = toolkit.get_dynamic_simulation_time_in_s();

    if(not is_bypassed())
    {
        double t_unblock = get_unblocking_time();
        if(fabs(t_unblock-INFINITE_THRESHOLD)>FLOAT_EPSILON)
            return;

        double t_unbypass = get_unbypassing_time();
        if(t_unbypass==INFINITE_THRESHOLD) // not unbypassing
        {
            double vdc_i = get_converter_dc_voltage_in_kV(INVERTER);
            //cout<<"VDCI = "<<vdc_i<<" at line "<<__LINE__<<" of file "<<__FILE__<<endl;
            double vbypass = get_inverter_dc_instantaneous_bypassing_voltage_in_kV();

            if(vdc_i<vbypass)
            {
                ostringstream osstream;
                osstream<<hvdc->get_device_name()<<" will be bypassed at time "<<TIME<<" s due to drop of inverter DC voltage."<<endl;
                osstream<<"Inverter DC voltage is "<<vdc_i<<" kV, and DC bypassing voltage threshold is "<<vbypass<<" kV.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                bypass_hvdc();
            }
        }
    }
    else
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        size_t bus_i = hvdc->get_converter_bus(INVERTER);
        double vac_i = psdb.get_bus_voltage_in_pu(bus_i);

        double vunbypass = get_inverter_ac_instantaneous_unbypassing_voltage_in_pu();
        if(is_bypass_timer_timed_out() and vac_i>vunbypass)
        {
            ostringstream osstream;
            osstream<<hvdc->get_device_name()<<" bypass timer is timed out, and will unbypass at time "<<TIME<<" s due to recovery of inverter AC voltage."<<endl;
            osstream<<"Inverter AC voltage is "<<vac_i<<" pu, and AC unbypassing voltage threshold is "<<vunbypass<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            unbypass_hvdc();
        }
    }
}

void CDC4T::check_mode_switching_logic()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked() or is_bypassed())
        return;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double TIME = toolkit.get_dynamic_simulation_time_in_s();

    double t_unblock = get_unblocking_time();
    double t_unbypass = get_unbypassing_time();
    if(fabs(t_unblock-INFINITE_THRESHOLD)>FLOAT_EPSILON or fabs(t_unbypass-INFINITE_THRESHOLD)>FLOAT_EPSILON) // unblocking or unbypassing
        return;

    double vdc_i = hvdc->get_converter_dc_voltage_in_kV(INVERTER);
    double vmode = hvdc->get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV();
    if(not is_mode_switched())
    {
        if(hvdc->get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER)
        {
            if(vdc_i<vmode)
            {
                ostringstream osstream;
                osstream<<hvdc->get_device_name()<<" will switch mode from holding DC power to holding DC current at time "<<TIME<<" s due to drop of inverter DC voltage."<<endl;
                osstream<<"Inverter DC voltage is "<<vdc_i<<" kV, and DC mode switch voltage threshold is "<<vmode<<" kV.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                switch_hvdc_mode();
            }
        }
    }
    else
    {
        if(is_mode_switch_timer_timed_out() and vdc_i> vmode)
        {
            ostringstream osstream;
            osstream<<hvdc->get_device_name()<<" mode switch timer is timed out, and will switch back to DC power control mode at time "<<TIME<<" s due to recovery of inverter DC voltage."<<endl;
            osstream<<"Inverter DC voltage is "<<vdc_i<<" pu, andDC mode switch voltage threshold is "<<vmode<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            switch_hvdc_mode_back();
        }
    }
}


void CDC4T::check()
{
    ;
}

void CDC4T::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void CDC4T::save()
{
    ;
}

string CDC4T::get_standard_model_string() const
{
    ostringstream osstream;

    HVDC* hvdc = get_hvdc_pointer();
    //size_t rbus = hvdc->get_converter_bus(RECTIFIER);
    //size_t ibus = hvdc->get_converter_bus(INVERTER);
    string dcname = hvdc->get_name();

    osstream<<"  \""<<dcname<<"\", \"CDC4T\", ";
    osstream<<setprecision(4)<<this->get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER)<<", "
      <<setprecision(4)<<this->get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER)<<", "
      <<setprecision(4)<<this->get_inverter_dc_voltage_sensor_T_in_s()<<", "
      <<setprecision(4)<<this->get_dc_current_sensor_T_in_s()<<", "
      <<" /";

    return osstream.str();
}

void CDC4T::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double CDC4T::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void CDC4T::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

void CDC4T::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@INVERTER DC VOLTAGE SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DC CURRENT SENSOR", i); i++;
}

double CDC4T::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;
    if(var_name == "STATE@INVERTER DC VOLTAGE SENSOR")
        return inverter_dc_voltage_sensor.get_state();

    if(var_name == "STATE@DC CURRENT SENSOR")
        return dc_current_sensor.get_state();
}



string CDC4T::get_dynamic_data_in_psse_format() const
{
    return "";
}

string CDC4T::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string CDC4T::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
