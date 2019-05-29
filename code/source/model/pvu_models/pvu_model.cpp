#include "header/model/pvu_models/pvu_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
PVU_MODEL::PVU_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("PV UNIT");
}

PVU_MODEL::~PVU_MODEL()
{
    ;
}

PV_UNIT* PVU_MODEL::get_pv_unit_pointer() const
{
    return (PV_UNIT*) get_device_pointer();
}

double PVU_MODEL::get_mbase_in_MVA() const
{
    PV_UNIT* gen = get_pv_unit_pointer();
    if(gen!=NULL)
        return gen->get_mbase_in_MVA();
    else
        return 0.0;
}

complex<double> PVU_MODEL::get_terminal_complex_voltage_in_pu() const
{
    PV_UNIT* gen = get_pv_unit_pointer();
    if(gen==NULL)
        return 0.0;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t bus = gen->get_unit_bus();
    complex<double> Vxy = psdb.get_bus_complex_voltage_in_pu(bus);
    return Vxy;
}

size_t PVU_MODEL::get_number_of_lumped_pv_units() const
{
    PV_UNIT* gen = get_pv_unit_pointer();
    if(gen==NULL)
        return 0;
    return gen->get_number_of_lumped_pv_units();
}

double PVU_MODEL::get_rated_power_per_pv_unit_in_MW() const
{
    PV_UNIT* gen = get_pv_unit_pointer();
    if(gen==NULL)
        return 0.0;
    return gen->get_rated_power_per_pv_unit_in_MW();
}

double PVU_MODEL::get_bus_base_frequency_in_Hz() const
{
    PV_UNIT* pvu = get_pv_unit_pointer();
    if(pvu==NULL)
        return 0.0;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return psdb.get_bus_base_frequency_in_Hz(pvu->get_unit_bus());
}

complex<double> PVU_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit==NULL)
        return 0.0;
    else
        return pv_unit->get_source_impedance_in_pu();
}
