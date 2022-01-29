#include "header/model/vsc_hvdc_model/vsc_hvdc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
class VSC_HVDC;

VSC_HVDC_MODEL::VSC_HVDC_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_VSC_HVDC);

}


VSC_HVDC_MODEL::~VSC_HVDC_MODEL()
{
}

VSC_HVDC* VSC_HVDC_MODEL::get_vsc_hvdc_pointer() const
{
    return (VSC_HVDC*) get_device_pointer();
}

string VSC_HVDC_MODEL::get_model_type() const
{
    return "VSC HVDC";
}

complex<double> VSC_HVDC_MODEL::get_converter_ac_current_in_pu(unsigned int converter_index) const
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    complex<double> S = get_converter_ac_complex_power_in_MVA(converter_index)*one_over_sbase;

    complex<double> V = get_converter_ac_complex_voltage_in_pu(converter_index);

    return conj(S/V);
}

complex<double> VSC_HVDC_MODEL::get_converter_ac_complex_power_in_MVA(unsigned int converter_index) const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    double P = vsc_hvdc->get_converter_P_to_AC_bus_in_MW(converter_index);
    double Q = vsc_hvdc->get_converter_Q_to_AC_bus_in_MVar(converter_index);
    return complex<double> (P,Q);
}

complex<double> VSC_HVDC_MODEL::get_converter_ac_complex_voltage_in_pu(unsigned int converter_index) const
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    BUS* bus = vsc_hvdc->get_converter_ac_bus_pointer(converter_index);
    return bus->get_positive_sequence_complex_voltage_in_pu();
}
