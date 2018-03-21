#include "header/network/network_optimize_network_orderingr.h"
#include "header/basic/utility.h"
#include "header/network/y_matrix_builder.h"
#include "header/steps_namespace.h"

#include <iostream>

using namespace std;

NETWORK_OPTIMIZER::NETWORK_OPTIMIZER()
{
    ;
}

NETWORK_OPTIMIZER::~NETWORK_OPTIMIZER()
{
    ;
}

void NETWORK_OPTIMIZER::optimize_network_ordering()
{
    initialize_physical_internal_bus_pair();

    Y_MATRIX_BUILDER builder;
    network_Y_matrix = builder.build_network_matrix();

    reorder_physical_internal_bus_pair();
}

bool NETWORK_OPTIMIZER::is_condition_ok() const
{
    return true;
}

void NETWORK_OPTIMIZER::initialize_physical_internal_bus_pair()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<BUS*> buses = db->get_all_buses();

    size_t nbus = buses.size();

    if(nbus==0)
        return;

    STEPS::inphno.clear();

    size_t bus_number;
    for(size_t i=0; i<nbus; i++)
    {
        bus_number = buses[i]->get_bus_number();
        STEPS::inphno.set_physical_internal_bus_number_pair(bus_number, i);
    }
}

void NETWORK_OPTIMIZER::reorder_physical_internal_bus_pair()
{
    vector<size_t> permutation = network_Y_matrix.get_reorder_permutation();
    STEPS::inphno.update_with_new_internal_bus_permutation(permutation);

    char info[256];
    sprintf(info, "Network internal bus numbers are optimized.");
    show_information_with_leading_time_stamp(info);

    /*sprintf(info,"After optimizing the network with network optimizer, the internal buses are listed as follows");
    show_information_with_leading_time_stamp(info);

    sprintf(info,"internal   physical   storage");
    show_information_with_leading_time_stamp(info);

    size_t bus, index, nbus;

    nbus = get_bus_count();

    for(size_t i=0; i<nbus; i++)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        index = get_bus_index(bus);
        sprintf(info,"%-10u %-10u %-10u",i, bus, index);
        show_information_with_leading_time_stamp(info);
    }*/

    /*sprintf(info,"Permutation for reorder internal bus:");
    show_information_with_leading_time_stamp(info);

    for(size_t i=0; i<permutation.size(); i++)
    {
        sprintf(info,"%u ---> %u",i,permutation[i]);
        show_information_with_leading_time_stamp(info);
    }
    */

}

void NETWORK_OPTIMIZER::check_newtork_connectivity()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    if(not STEPS::inphno.is_table_full())
        optimize_network_ordering();

    Y_MATRIX_BUILDER builder;
    network_Y_matrix = builder.build_network_matrix();

    vector< vector<size_t> > islands;

    size_t nbus = db->get_bus_count();

    vector<bool> bus_searched_flag;
    bus_searched_flag.reserve(nbus);

    for(size_t i=0; i<nbus; i++)
        bus_searched_flag.push_back(false);

    while(true)
    {
        bool new_island_found = false;
        size_t first_bus_in_new_island;

        for(size_t i=0; i<nbus; i++)
        {
            if(bus_searched_flag[i]==false)
            {
                new_island_found = true;
                first_bus_in_new_island = i;
                break;
            }
        }

        if(new_island_found==false)
            break;

        vector<size_t> this_island;
        this_island.reserve(nbus);

        this_island.push_back(first_bus_in_new_island);
        bus_searched_flag[first_bus_in_new_island] = true;

        int searching_bus=0;
        for(size_t i=0; i<this_island.size(); i++)
        {
            searching_bus = this_island[i];
            int k_start = network_Y_matrix.get_starting_index_of_column(searching_bus);
            int k_end = network_Y_matrix.get_starting_index_of_column(searching_bus+1);
            int row_bus;
            for(int k=k_start; k<k_end; k++)
            {
                row_bus = network_Y_matrix.get_row_number_of_entry_index(k);
                if(row_bus!=searching_bus)
                {
                    if(bus_searched_flag[row_bus]==false)
                    {
                        this_island.push_back(row_bus);
                        bus_searched_flag[row_bus] = true;
                    }
                }
            }
        }
        islands.push_back(this_island);
    }

    char info[256];
    sprintf(info,"There are %u islands.", islands.size());
    show_information_with_leading_time_stamp(info);

    size_t physical_bus;
    bool ther_is_slack_bus_in_island;
    BUS* bus;
    for(size_t i=0; i<islands.size(); i++)
    {
        sprintf(info, "Physical buses in island %u:", i);
        show_information_with_leading_time_stamp(info);
        ther_is_slack_bus_in_island = false;
        for(size_t j=0; j<islands[i].size(); j++)
        {
            physical_bus = get_physical_bus_number_of_internal_bus(islands[i][j]);
            bus = db->get_bus(physical_bus);
            if(bus->get_bus_type()==SLACK_TYPE)
            {
                sprintf(info, "%u (Slack bus)", physical_bus);
                show_information_with_leading_time_stamp(info);
                ther_is_slack_bus_in_island = true;
            }
            else
            {
                sprintf(info, "%u", physical_bus);
                show_information_with_leading_time_stamp(info);
            }
        }
        if(not ther_is_slack_bus_in_island)
        {
            sprintf(info, "No slack bus is found in island %u.", i);
            show_information_with_leading_time_stamp(info);
        }
    }
}
