#include "header/model/bus_model/bus_frequency_model/bus_frequency_model.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

BUS_FREQUENCY_MODEL::BUS_FREQUENCY_MODEL(STEPS& toolkit) : frequency_block(toolkit)
{
    set_toolkit(toolkit);
    frequency_block.set_K(ONE_OVER_DOUBLE_PI);
    bus_ptr = NULL;
}

BUS_FREQUENCY_MODEL::~BUS_FREQUENCY_MODEL()
{
    ;
}

void BUS_FREQUENCY_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& BUS_FREQUENCY_MODEL::get_toolkit() const
{
    return *toolkit;
}

void BUS_FREQUENCY_MODEL::set_bus_pointer(BUS* bus)
{
    bus_ptr = bus;
}

BUS* BUS_FREQUENCY_MODEL::get_bus_pointer() const
{
    return bus_ptr;
}

unsigned int BUS_FREQUENCY_MODEL::get_bus() const
{
    return bus_ptr->get_bus_number();
}

void BUS_FREQUENCY_MODEL::initialize()
{
    fbase_Hz = bus_ptr->get_base_frequency_in_Hz();
    tbase_s = bus_ptr->get_base_period_in_s();

    double DELT = toolkit->get_dynamic_simulation_time_step_in_s();
    frequency_block.set_T_in_s(DELT*4.0);

    frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());

    frequency_block.initialize();

    frequency_block_output_old = frequency_block.get_output();
    is_model_updated = true;
}

void BUS_FREQUENCY_MODEL::run(DYNAMIC_MODE mode)
{
    if(mode==DYNAMIC_INTEGRATE_MODE or mode==DYNAMIC_UPDATE_MODE)
    {
        if(mode==DYNAMIC_INTEGRATE_MODE and is_model_updated==true)
        {
            // this is the very first time for bus frequency block to be integrated for the next time step.
            //need to store the previous frequency output temporarily for calculating the frequency when simulation time step is very large.
            frequency_block_output_old = frequency_block.get_output();
            is_model_updated = false;
        }
        if(mode==DYNAMIC_UPDATE_MODE)
            is_model_updated = true;

        frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());
        frequency_block.run(mode);
    }
    else
    {
        if(mode==DYNAMIC_UPDATE_TIME_STEP_MODE)
        {
            double DELT= toolkit->get_dynamic_simulation_time_step_in_s();
            frequency_block.set_T_in_s(DELT*4.0);
            frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());
            frequency_block.run(mode);
        }
        else
        {
            if(mode==DYNAMIC_INITIALIZE_MODE)
                initialize();
        }
    }
}

void BUS_FREQUENCY_MODEL::update_for_applying_event(DYNAMIC_EVENT_TYPE type)
{
    double temp = frequency_block.get_output();

    frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());
    double input = frequency_block.get_input();
    double K = frequency_block.get_K();
    double T = frequency_block.get_T_in_s();

    switch(type)
    {
        case FAULT_OR_OPERATION_EVENT:
        {
            frequency_block.set_state_WITH_CAUTION(K/T*input-temp);
            frequency_block.run(DYNAMIC_UPDATE_MODE);
            break;
        }
        case TIME_STEP_CHANGE_EVENT:
        {
            double new_DELT= toolkit->get_dynamic_simulation_time_step_in_s();
            frequency_block.set_T_in_s(new_DELT*4.0);
            T = frequency_block.get_T_in_s();
            frequency_block.set_state_WITH_CAUTION(K/T*input-temp);
            frequency_block.run(DYNAMIC_UPDATE_TIME_STEP_MODE);
        }
    }
    //cout<<"before updating bus frequency model of bus "<<bus_ptr->get_bus_number()<<": output = "<<temp<<endl;
    //cout<<"after updating bus frequency model of bus "<<bus_ptr->get_bus_number()<<": input="<<rad2deg(input)<<", output = "<<frequency_block.get_output()<<endl;
}

void BUS_FREQUENCY_MODEL::set_frequency_deviation_in_pu(double f)
{
    frequency_block.set_output(f*fbase_Hz);
}

double BUS_FREQUENCY_MODEL::get_frequency_deviation_in_pu() const
{
    return get_frequency_deviation_in_Hz()*tbase_s;
}

double BUS_FREQUENCY_MODEL::get_frequency_deviation_in_Hz() const
{
    double DELT= toolkit->get_dynamic_simulation_time_step_in_s();
    DYNAMICS_SIMULATOR& ds = toolkit->get_dynamic_simulator();
    double h_th = ds.get_time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled_in_s();
    if(DELT<h_th)
        return frequency_block.get_output();
    else
    {
        // assuming an additional (1+s*k*DELT) block is added following the frequency_block
        double k = ds.get_k_for_leading_part_of_bus_frequency_model();
        double f = frequency_block.get_output();
        return f + (f-frequency_block_output_old)*k;
    }
}

double BUS_FREQUENCY_MODEL::get_frequency_in_pu() const
{
    return 1.0+get_frequency_deviation_in_pu();
}

double BUS_FREQUENCY_MODEL::get_frequency_in_Hz() const
{
    return fbase_Hz+get_frequency_deviation_in_Hz();
}
