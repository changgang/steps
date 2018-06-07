#include "header/model/bus_frequency_model/bus_frequency_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <iostream>

using namespace std;

BUS_FREQUENCY_MODEL::BUS_FREQUENCY_MODEL()
{
    frequency_block.set_K(1.0/(2.0*PI));
}

BUS_FREQUENCY_MODEL::~BUS_FREQUENCY_MODEL()
{
    ;
}

void BUS_FREQUENCY_MODEL::set_bus(size_t bus_number)
{
    this->bus = bus_number;
}

void BUS_FREQUENCY_MODEL::initialize()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = this->get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. Power system database is not set for BUS_FREQUENCY MODEL of bus "<<bus<<" when initializing bus frequency model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    bus_ptr = psdb->get_bus(bus);
    if(bus_ptr==NULL)
    {
        osstream<<"Warning. Bus "<<bus<<" cannot be found in power system database "<<psdb->get_system_name()<<" when initializing bus frequency model.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }


    double DELT = get_dynamic_simulation_time_step_in_s();
    frequency_block.set_T_in_s(DELT*4.0);

    frequency_block.set_input(bus_ptr->get_angle_in_rad());

    frequency_block.initialize();
}

void BUS_FREQUENCY_MODEL::run(DYNAMIC_MODE mode)
{
    frequency_block.set_input(bus_ptr->get_angle_in_rad());
    frequency_block.run(mode);
}

void BUS_FREQUENCY_MODEL::update_for_applying_event()
{
    double temp = frequency_block.get_output();

    frequency_block.set_input(bus_ptr->get_angle_in_rad());
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
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    double fbase = psdb->get_system_base_frequency_in_Hz();
    frequency_block.set_output(f*fbase);
}

double BUS_FREQUENCY_MODEL::get_frequency_deviation_in_pu() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = this->get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"NULL power system database is detected. Failed to call BUS_FREQUENCY_MODEL::"<<__FUNCTION__<<"().";
        show_information_with_leading_time_stamp(osstream);
    }
    double fbase = psdb->get_system_base_frequency_in_Hz();
    return get_frequency_deviation_in_Hz()/fbase;
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
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = this->get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"NULL power system database is detected. Failed to call BUS_FREQUENCY_MODEL::"<<__FUNCTION__<<"().";
        show_information_with_leading_time_stamp(osstream);
    }
    double fbase = psdb->get_system_base_frequency_in_Hz();
    return fbase+get_frequency_deviation_in_Hz();
}


bool BUS_FREQUENCY_MODEL::is_valid() const
{
    return true; // function is disabled
}
void BUS_FREQUENCY_MODEL::check()
{
    ; // function is disabled
}
void BUS_FREQUENCY_MODEL::clear()
{
    ; // function is disabled
}
