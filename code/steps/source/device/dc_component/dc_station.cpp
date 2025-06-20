#include "header/device/dc_component/dc_station.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;


DC_STATION::DC_STATION(STEPS& toolkit) :    udc_block(toolkit)
{
    set_toolkit(toolkit);

    clear();
}

DC_STATION::~DC_STATION()
{
    ;
}

void DC_STATION::set_toolkit(STEPS& toolkit)
{
    this->toolkit =(&toolkit);
}
STEPS& DC_STATION::get_toolkit() const
{
    return *toolkit;
}


void DC_STATION::clear()
{
    station_number = 0;
    terminal_number = 0;
    status = false;

    station_structure = LCC_CASCADE;
    station_mode = DC_P_STATION;

    n_cascade_lcc = 0;
    n_parallel_vsc = 0;

    //lccs.clear();
    //vscs.clear();
    vsc_station_dynamic_udc_in_kV = 0.0;


    C = 0.0;
    Udc_input = 0.0;
    Udc_initial_value = 0.0;

}

 DEVICE_ID DC_STATION::get_device_id() const
 {
     ;
 }

 //SET
void DC_STATION::set_station_name(const string name)
{
    this->station_name = name;
}

void DC_STATION::set_station_number(const unsigned int station_number)
{
    this->station_number = station_number;
}

void DC_STATION::set_terminal_number(const unsigned int terminal_number)
{
    this->terminal_number = terminal_number;
}

void DC_STATION::set_status(const bool status)
{
    this->status = status;
}

void DC_STATION::set_station_structure(const DC_STATION_STRUCTURE structure)
{
    this->station_structure = structure;
}

void DC_STATION::set_station_mode(const DC_STATION_MODE mode)
{
    this->station_mode = mode;
}

void DC_STATION::set_station_cascade_lcc_number(const unsigned int n_lcc)
{
    this->n_cascade_lcc = n_lcc;
}

void DC_STATION::set_station_parallel_vsc_number(const unsigned int n_vsc)
{
    this->n_parallel_vsc = n_vsc;
}

void DC_STATION::set_vsc_station_dynamic_udc_in_kV(const double udc)
{
    this->vsc_station_dynamic_udc_in_kV = udc;
}


//GET
string DC_STATION::get_station_name() const
{
    return station_name;
}

unsigned int DC_STATION::get_station_number() const
{
    return station_number;
}

unsigned int DC_STATION::get_terminal_number() const
{
    return terminal_number;
}
bool DC_STATION::get_status() const
{
    return status;
}

DC_STATION_STRUCTURE DC_STATION::get_station_structure() const
{
    return station_structure;
}

DC_STATION_MODE DC_STATION::get_station_mode() const
{
    return station_mode;
}

unsigned int DC_STATION::get_station_cascade_lcc_number() const
{
    return n_cascade_lcc;
}

unsigned int DC_STATION::get_station_parallel_vsc_number() const
{
    return n_parallel_vsc;
}

/*
vector<HLCC*> DC_STATION::get_lcc_converters_in_dc_station_with_lcc_cascade()
{
    vector<HLCC*> lccs_station;
    unsigned int n = lccs.size();
    for(unsigned int i=0;i!=n;i++)
    {
        lccs_station.push_back(&(lccs[i]));
    }
    return lccs_station;
}
*/

/*
vector<HVSC*> DC_STATION::get_vsc_converters_in_dc_station_with_vsc_parallel()
{
    vector<HVSC*> vscs_station;
    unsigned int n = vscs.size();
    for(unsigned int i=0;i!=n;i++)
    {
        vscs_station.push_back(&(vscs[i]));
    }
    return vscs_station;
}
*/

double DC_STATION::get_vsc_station_dynamic_udc_in_kV()
{
    return udc_block.get_output();
}

/*
void DC_STATION::initialize_dc_station_converters_with_lcc_cascade(vector<HLCC*> lccs_station)
{
    lccs.clear();
    unsigned int n_lcc = lccs_station.size();
    lccs.reserve(n_lcc);
    for(unsigned int i=0;i!=n_lcc;i++)
    {
        HLCC lcc = *lccs_station[i];
        lccs.push_back(lcc);
    }
}
*/
/*
void DC_STATION::initialize_dc_station_converters_with_vsc_parallel(vector<HVSC*> vscs_station)
{
    vscs.clear();
    unsigned int n_vsc = vscs_station.size();
    vscs.reserve(n_vsc);
    for(unsigned int i=0;i!=n_vsc;i++)
    {
        HVSC vsc = *vscs_station[i];
        vscs.push_back(vsc);
    }

}
*/

void DC_STATION::set_udc_block_ceq_in_F(double C)
{
    if(get_station_structure()==VSC_PARALLEL)
        this->C = C;
}

void DC_STATION::set_udc_block_input(double input)
{
    if(get_station_structure()==VSC_PARALLEL)
        this->Udc_input = input;
}

void DC_STATION::set_udc_block_initial_value(double value)
{
    if(get_station_structure()==VSC_PARALLEL)
        this->Udc_initial_value = value;
}


double DC_STATION::get_udc_block_c_in_F()
{
    return C;
}

double DC_STATION::get_udc_block_input()
{
    return Udc_input;
}

double DC_STATION::get_udc_block_initial_value()
{
    return Udc_initial_value;
}

void DC_STATION::initialize()
{
    double C = get_udc_block_c_in_F();
    udc_block.set_T_in_s(C);
    udc_block.set_output(get_udc_block_initial_value());
    udc_block.initialize();
    /*
    ostringstream osstream;
    osstream<<" UDC BLOCK INITIALZE WITH VALUE: "<<get_udc_block_initial_value()<<"  AND C is: "<<udc_block.get_T_in_s()<<"  and udc is: "<<udc_block.get_output();
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */
}

void DC_STATION::run(DYNAMIC_MODE mode)
{
    double input = get_udc_block_input();

    udc_block.set_input(input);
    udc_block.run(mode);
    /*
    ostringstream osstream;
    osstream<<" UDC station:"<<get_station_number()<<" RUN WITH INPUT: "<<input<<"  AND OUTPUT: "<<udc_block.get_output();
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    */

}






