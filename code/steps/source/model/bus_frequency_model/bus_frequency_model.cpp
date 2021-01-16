#include "header/model/bus_frequency_model/bus_frequency_model.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

BUS_FREQUENCY_MODEL::BUS_FREQUENCY_MODEL(STEPS& toolkit) : frequency_block(toolkit)
{
    fbase_Hz = 0.0;
    tbase_s = 0.0;

    frequency_block.set_K(ONE_OVER_DOUBLE_PI);
    bus_ptr = NULL;
}

BUS_FREQUENCY_MODEL::~BUS_FREQUENCY_MODEL()
{
    ;
}

STEPS& BUS_FREQUENCY_MODEL::get_toolkit() const
{
    return frequency_block.get_toolkit();
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

void BUS_FREQUENCY_MODEL::set_base_frequency_in_Hz(double fn)
{
    if(fn<0.0)
        fn = -fn;
    if(fn==0.0)
        fn = 50.0;
    fbase_Hz = fn;
    tbase_s = 1.0/fbase_Hz;
}

double BUS_FREQUENCY_MODEL::get_base_frequency_in_Hz() const
{
    return fbase_Hz;
}

void BUS_FREQUENCY_MODEL::initialize()
{
    if(get_base_frequency_in_Hz()==0.0)
        set_base_frequency_in_Hz(50.0);

    double DELT = get_toolkit().get_dynamic_simulation_time_step_in_s();
    frequency_block.set_T_in_s(DELT*4.0);

    frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());

    frequency_block.initialize();
}

void BUS_FREQUENCY_MODEL::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE or mode==UPDATE_MODE)
    {
        frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());
        frequency_block.run(mode);
    }
    else
    {
        if(mode==INITIALIZE_MODE)
            initialize();
    }
}

void BUS_FREQUENCY_MODEL::update_for_applying_event()
{
    double temp = frequency_block.get_output();

    frequency_block.set_input(bus_ptr->get_positive_sequence_angle_in_rad());
    double input = frequency_block.get_input();
    double K = frequency_block.get_K();
    double T = frequency_block.get_T_in_s();

    frequency_block.set_state_WITH_CAUTION(K/T*input-temp);
    frequency_block.run(UPDATE_MODE);

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
    return frequency_block.get_output();
}

double BUS_FREQUENCY_MODEL::get_frequency_in_pu() const
{
    return 1.0+get_frequency_deviation_in_pu();
}

double BUS_FREQUENCY_MODEL::get_frequency_in_Hz() const
{
    return fbase_Hz+get_frequency_deviation_in_Hz();
}
