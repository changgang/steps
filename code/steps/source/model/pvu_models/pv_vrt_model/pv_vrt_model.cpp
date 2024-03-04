#include "header/model/pvu_models/pv_vrt_model/pv_vrt_model.h"
#include "header/basic/utility.h"
#include "header/device/pv_unit.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

PV_VRT_MODEL::PV_VRT_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
    ;
}

PV_VRT_MODEL::~PV_VRT_MODEL()
{
    ;
}

string PV_VRT_MODEL::get_model_type() const
{
    return "PV VRT";
}

complex<double> PV_VRT_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double PV_VRT_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return steps_sqrt(x*x + y * y);
}

complex<double> PV_VRT_MODEL::get_pv_unit_terminal_generation_in_MVA() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if(model!=NULL)
    {
        if(not model->is_model_initialized())
            model->initialize();

        return model->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> PV_VRT_MODEL::get_pv_unit_terminal_generation_in_pu_based_on_mbase() const
{
    return get_pv_unit_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> PV_VRT_MODEL::get_pv_unit_terminal_complex_current_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double PV_VRT_MODEL::get_pv_unit_terminal_current_in_pu() const
{
	complex<double> I =get_pv_unit_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return steps_sqrt(x*x+y*y);
}
