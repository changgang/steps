#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include<iostream>

using namespace std;

VSC_HVDC_CONVERTER_MODEL::VSC_HVDC_CONVERTER_MODEL(STEPS& toolkit):VSC_HVDC_MODEL(toolkit)
{
    converter_index = INDEX_NOT_EXIST;
    converter_name_index = INDEX_NOT_EXIST;
    converter_ac_busptr = NULL;
    set_as_current_source();
}

VSC_HVDC_CONVERTER_MODEL::~VSC_HVDC_CONVERTER_MODEL()
{
    ;
}

string VSC_HVDC_CONVERTER_MODEL::get_model_type() const
{
    return "VSC HVDC CONVERTER";
}

void VSC_HVDC_CONVERTER_MODEL::set_converter_name(string name)
{
    STEPS& toolkit = get_toolkit();
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    if(vsc!=NULL)
    {
        unsigned int index = vsc->get_converter_index_with_converter_name(name);
        if(index!=INDEX_NOT_EXIST)
        {
            converter_index = index;
            add_string_to_str_int_map(name);
            converter_name_index = get_index_of_string(name);

            unsigned int bus = vsc->get_converter_ac_bus(converter_index);
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            converter_ac_busptr = psdb.get_bus(bus);
        }
        else
        {
            ostringstream osstream;
            osstream<<"Fatal error. Converter with name ("<<name<<") cannot be found when building "
                    <<get_model_name()<<" model for VSC_HVDC "<<vsc->get_name();
            cout<<osstream.str()<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

string VSC_HVDC_CONVERTER_MODEL::get_converter_name() const
{
    return get_string_of_index(get_converter_name_index());
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_name_index() const
{
    return converter_name_index;
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_index() const
{
    return converter_index;
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_bus() const
{
    return get_vsc_hvdc_pointer()->get_converter_ac_bus(get_converter_index());
}

void VSC_HVDC_CONVERTER_MODEL::set_as_voltage_source()
{
    voltage_source_flag = true;
}

void VSC_HVDC_CONVERTER_MODEL::set_as_current_source()
{
    voltage_source_flag = false;
}

bool VSC_HVDC_CONVERTER_MODEL::is_voltage_source() const
{
    return voltage_source_flag;
}


void VSC_HVDC_CONVERTER_MODEL::set_converter_active_control_mode(VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode)
{
    active_power_control_mode = active_control_mode;
}

void VSC_HVDC_CONVERTER_MODEL::set_converter_reactive_control_mode(VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode)
{
    reactive_power_control_mode = reactive_control_mode;
}


VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE VSC_HVDC_CONVERTER_MODEL::get_converter_active_control_mode() const
{
    return active_power_control_mode;
}

VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE VSC_HVDC_CONVERTER_MODEL::get_converter_reactive_control_mode() const
{
    return reactive_power_control_mode;
}

BUS* VSC_HVDC_CONVERTER_MODEL::get_converter_ac_bus_pointer() const
{
    return converter_ac_busptr;
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_bus_base_voltage_in_kV() const
{
    BUS* bus = get_converter_ac_bus_pointer();
    if(bus!=NULL)
        return bus->get_base_voltage_in_kV();
    else
        return 0.0;
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_bus_angle_in_deg() const
{
    BUS* bus = get_converter_ac_bus_pointer();
    if(bus!=NULL)
        return bus->get_positive_sequence_angle_in_deg();
    else
        return 0.0;
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_bus_angle_in_rad() const
{
    BUS* bus = get_converter_ac_bus_pointer();
    if(bus!=NULL)
        return bus->get_positive_sequence_angle_in_rad();
    else
        return 0.0;
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_angle_at_converter_side_in_rad() const
{
    return 0.0;
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_angle_at_converter_side_in_deg() const
{
    return rad2deg(get_converter_ac_angle_at_converter_side_in_rad());
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_angle_at_ac_bus_side_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_ac_angle_at_ac_bus_side_in_deg() const
{
    return rad2deg(get_converter_ac_angle_at_ac_bus_side_in_rad());
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_capacity_in_MVA() const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    return vsc_hvdc->get_converter_rated_capacity_in_MVA(get_converter_index());
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_base_voltage_in_kV() const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    return vsc_hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(get_converter_index());
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_transformer_ac_side_base_voltage_in_kV() const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    return vsc_hvdc->get_converter_transformer_AC_side_base_voltage_in_kV(get_converter_index());
}

double VSC_HVDC_CONVERTER_MODEL::get_converter_transformer_capacity_in_MVA() const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    return vsc_hvdc->get_converter_transformer_capacity_in_MVA(get_converter_index());
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_current_from_converter_to_ac_bus_in_xy_axis_in_MVA() const
{
    return 0.0;
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_current_from_converter_to_ac_bus_in_xy_axis_in_pu_based_on_converter_bases() const
{
    complex<double> Ixy = get_converter_current_from_converter_to_ac_bus_in_xy_axis_in_MVA();
    double Sbase = get_converter_capacity_in_MVA();
    double Vbase = get_converter_base_voltage_in_kV();
    return Ixy*SQRT3*Vbase/Sbase;
}

void VSC_HVDC_CONVERTER_MODEL::initialize_current_or_voltage_source_equivalent_scale()
{
    complex<double> Yf = 0.0;
    complex<double> Zl = 0.0;
    complex<double> Zt = 0.0;
    double kt = 0.0;

    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    if(vsc_hvdc!=NULL)
    {
        unsigned int converter_index = get_converter_index();

        kt = vsc_hvdc->get_converter_transformer_off_nominal_turn_ratio(converter_index);
        Zt = vsc_hvdc->get_converter_transformer_impedance_in_pu(converter_index);
        Zl = vsc_hvdc->get_converter_commutating_impedance_in_ohm(converter_index);
        Yf = vsc_hvdc->get_converter_filter_admittance_in_siemens(converter_index);
    }
    if(is_voltage_source()==true)
        current_or_voltage_equivalent_scale = 1.0/(Zt*(1.0+Zl*Yf)+Zl)/kt;
    else
        current_or_voltage_equivalent_scale = 1.0/(1.0+Yf*Zt)/kt;


}
complex<double> VSC_HVDC_CONVERTER_MODEL::get_current_source_equivalent_scale() const
{
    return current_or_voltage_equivalent_scale;
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_voltage_source_equivalent_admittance() const
{
    return get_current_source_equivalent_scale();
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_voltage_in_xy_axis_in_pu_on_converter_base()
{
    complex<double> Vdq = get_converter_voltage_in_dq_axis_in_pu_on_converter_base();
    double angle = get_converter_ac_angle_at_converter_side_in_rad();
    return dq2xy_with_angle_in_rad(Vdq, angle);
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_current_in_xy_axis_in_pu_on_converter_base()
{
    complex<double> Idq = get_converter_current_in_dq_axis_in_pu_on_converter_base();
    double angle = get_converter_ac_angle_at_converter_side_in_rad();
    return dq2xy_with_angle_in_rad(Idq, angle);
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_voltage_in_dq_axis_in_pu_on_system_base()
{
    complex<double> Vdc = get_converter_voltage_in_dq_axis_in_pu_on_converter_base();
    double Vbase_converter = get_converter_transformer_ac_side_base_voltage_in_kV();
    double Vbase_bus = get_converter_ac_bus_base_voltage_in_kV();
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int index = get_converter_index();
    double k = vsc_hvdc->get_converter_transformer_off_nominal_turn_ratio(index);

    return Vdc*Vbase_converter/Vbase_bus*k;
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_voltage_in_xy_axis_in_pu_on_system_base()
{
    complex<double> Vdq = get_converter_voltage_in_dq_axis_in_pu_on_system_base();
    double angle = get_converter_ac_angle_at_converter_side_in_rad();
    return dq2xy_with_angle_in_rad(Vdq, angle);
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_current_in_dq_axis_in_pu_on_system_base()
{
    complex<double> Idc = get_converter_current_in_dq_axis_in_pu_on_converter_base();
    double Sbase_transformer = get_converter_transformer_capacity_in_MVA();
    double Sbase_system = get_toolkit().get_system_base_power_in_MVA();
    double Vbase_converter = get_converter_transformer_ac_side_base_voltage_in_kV();
    double Vbase_bus = get_converter_ac_bus_base_voltage_in_kV();

    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int index = get_converter_index();
    double k = vsc_hvdc->get_converter_transformer_off_nominal_turn_ratio(index);

    return Idc*(Sbase_transformer*Vbase_bus)/(Sbase_system*Vbase_converter)/(k*k);
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_current_in_xy_axis_in_pu_on_system_base()
{
    complex<double> Idq = get_converter_current_in_dq_axis_in_pu_on_system_base();
    double angle = get_converter_ac_angle_at_converter_side_in_rad();
    return dq2xy_with_angle_in_rad(Idq, angle);
}

complex<double> VSC_HVDC_CONVERTER_MODEL::get_converter_Norton_current_in_xy_axis_in_pu_based_on_system_base()
{
    if(is_voltage_source() == true)
    {
        complex<double> Vxy = get_converter_voltage_in_xy_axis_in_pu_on_system_base();
        return Vxy*get_voltage_source_equivalent_admittance();
    }
    else
    {
        complex<double> Ixy = get_converter_current_in_xy_axis_in_pu_on_system_base();
        return Ixy*get_current_source_equivalent_scale();
    }
}

/*
VSC_MODEL_VAR::VSC_MODEL_VAR(STEPS& toolkit):
               active_power_control_block(toolkit),
               reactive_power_control_block(toolkit),
               ac_voltage_control_block(toolkit),
               ud_voltage_control_block(toolkit),
               dc_voltage_block(toolkit)
{
    ;

}
VSC_MODEL_VAR::~VSC_MODEL_VAR()
{

}

VSC_MODEL_VAR::VSC_MODEL_VAR(const VSC_MODEL_VAR& var):
               active_power_control_block(var.active_power_control_block.get_toolkit()),
               reactive_power_control_block(var.reactive_power_control_block.get_toolkit()),
               ac_voltage_control_block(var.ac_voltage_control_block.get_toolkit()),
               ud_voltage_control_block(var.ud_voltage_control_block.get_toolkit()),
               dc_voltage_block(var.dc_voltage_block.get_toolkit())
{
    converter_bus = var.converter_bus;
    converter_busptr = var.converter_busptr;
    active_power_control_mode = var.active_power_control_mode;
    reactive_power_control_mode = var.reactive_power_control_mode;

}
*/
