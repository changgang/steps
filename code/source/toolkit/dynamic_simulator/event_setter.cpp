#include "header/toolkit/event_setter.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include "header/network/y_matrix_builder.h"
#include "header/basic/fault.h"
EVENT_SETTER::EVENT_SETTER(DYNAMICS_SIMULATOR& simulator)
{
    this->simulator = (&simulator);
}

EVENT_SETTER::~EVENT_SETTER()
{
    ;
}

void EVENT_SETTER::change_dynamic_simulator_time_step(double newDELT)
{
    simulator->set_simulation_time_step_in_s(newDELT);
    simulator->update();
}

void EVENT_SETTER::set_bus_fault(size_t bus, complex<double> fault_shunt)
{
    size_t index = get_bus_index(bus);
    if(index == INDEX_NOT_EXIST)
        return;
    FAULT fault;
    fault.set_fault_type(THREE_PHASES_FAULT);
    fault.set_fault_shunt_in_pu(fault_shunt);
    STEPS::Bus[index].set_fault(fault);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}
void EVENT_SETTER::clear_bus_fault(size_t bus)
{
    size_t index = get_bus_index(bus);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Bus[index].clear_fault();

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::set_line_fault(DEVICE_ID device_id, size_t side_bus, double location, complex<double> fault_shunt)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    FAULT fault;
    fault.set_fault_type(THREE_PHASES_FAULT);
    fault.set_fault_shunt_in_pu(fault_shunt);

    STEPS::Line[index].set_fault(side_bus, location, fault);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::clear_line_fault(DEVICE_ID device_id)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;
    STEPS::Line[index].clear_all_faults();

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::trip_line(DEVICE_ID device_id)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Line[index].set_sending_side_breaker_status(false);
    STEPS::Line[index].set_receiving_side_breaker_status(false);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}
void EVENT_SETTER::trip_line_breaker(DEVICE_ID device_id, size_t side_bus)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    size_t sending_bus = STEPS::Line[index].get_sending_side_bus();
    if(STEPS::Line[index].is_connected_to_bus(side_bus))
    {
        if(side_bus == sending_bus)
            STEPS::Line[index].set_sending_side_breaker_status(false);
        else
            STEPS::Line[index].set_receiving_side_breaker_status(false);

        Y_MATRIX_BUILDER builder;
        builder.build_dynamic_network_matrix();

        simulator->update();
    }
}

void EVENT_SETTER::close_line(DEVICE_ID device_id)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Line[index].set_sending_side_breaker_status(true);
    STEPS::Line[index].set_receiving_side_breaker_status(true);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::close_line_breaker(DEVICE_ID device_id, size_t side_bus)
{
    size_t index = get_line_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    size_t sending_bus = STEPS::Line[index].get_sending_side_bus();
    size_t receiving_bus = STEPS::Line[index].get_receiving_side_bus();
    if(side_bus == sending_bus or side_bus == receiving_bus)
    {
        if(side_bus == sending_bus)
            STEPS::Line[index].set_sending_side_breaker_status(true);
        else
            STEPS::Line[index].set_receiving_side_breaker_status(true);

        Y_MATRIX_BUILDER builder;
        builder.build_dynamic_network_matrix();

        simulator->update();
    }
}


void EVENT_SETTER::trip_generator(DEVICE_ID device_id)
{
    size_t index = get_generator_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Generator[index].set_status(false);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::shed_generator(DEVICE_ID device_id,double percent)
{
    size_t index = get_generator_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    double mbase = STEPS::Generator[index].get_mbase_in_MVA();
    mbase *= (1.0-percent);
    STEPS::Generator[index].set_mbase_in_MVA(mbase);

    Y_MATRIX_BUILDER builder;
    builder.build_dynamic_network_matrix();

    simulator->update();
}

void EVENT_SETTER::trip_load(DEVICE_ID device_id)
{
    size_t index = get_load_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Load[index].set_status(false);
    simulator->update();
}

void EVENT_SETTER::scale_load(DEVICE_ID device_id, double percent)
{
    size_t index = get_load_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    double scale_factor = STEPS::Load[index].get_scale_factor();
    STEPS::Load[index].set_scale_factor(scale_factor+percent);
    simulator->update();
}

void EVENT_SETTER::scale_all_load(double percent)
{
    size_t n=get_load_count();
    double scale_factor;
    for(size_t i=0; i<n; i++)
    {
        scale_factor = STEPS::Load[i].get_scale_factor();
        STEPS::Load[i].set_scale_factor(scale_factor+percent);
    }
    simulator->update();
}

void EVENT_SETTER::bypass_hvdc(DEVICE_ID device_id)
{
    ;
}
void EVENT_SETTER::block_hvdc(DEVICE_ID device_id)
{
    size_t index = get_hvdc_index(device_id);
    if(index == INDEX_NOT_EXIST)
        return;

    STEPS::Hvdc[index].set_status(false);
    simulator->update();
}

void EVENT_SETTER::change_generator_voltage_reference_in_pu(DEVICE_ID device_id, double vref)
{
    ;
}
void EVENT_SETTER::change_generator_power_reference_in_MW(DEVICE_ID device_id, double Pref)
{
    ;
}
