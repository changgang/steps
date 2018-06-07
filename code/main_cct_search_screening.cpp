#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/toolkit/cct_searcher/cct_searcher.h"
#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
    initialize_simulator(); // this function should be called first

    set_dynamic_simulation_time_step_in_s(0.01);

    POWER_SYSTEM_DATABASE psdb; // create a new database
    psdb.set_allowed_max_bus_number(1000); // set the max bus number of the database

    PSSE_IMEXPORTER importer; // create an imexporter
    importer.set_power_system_database(&psdb); // assign database to the importer

    importer.load_powerflow_data("IEEE9_classical.raw"); // load powerflow
    vector<DEVICE_ID> lines = psdb.get_all_lines_device_id(); // the powerflow data is only used for get line device id
    size_t n_lines = lines.size();

    vector<DEVICE_ID> fault_lines;// the three vectors are used to store searching result
    vector<size_t> fault_side_buses;
    vector<double> ccts;

    for(size_t i=0; i!=n_lines; ++i)
    {
        DEVICE_ID did = lines[i];
        fault_lines.push_back(did);
        fault_lines.push_back(did);

        TERMINAL terminal = did.get_device_terminal();
        vector<size_t> buses = terminal.get_buses();
        fault_side_buses.push_back(buses[0]);
        fault_side_buses.push_back(buses[1]);

        ccts.push_back(0.0);
        ccts.push_back(0.0);
    }

    size_t n_events = n_lines*2;

    #pragma omp parallel for
    for(size_t i=0; i<n_events; ++i)
    {
        ostringstream osstream;
        CCT_SEARCHER searcher;
        searcher.set_power_system_database_maximum_bus_number(1000);
        searcher.set_search_title("");
        searcher.set_powerflow_data_filename("IEEE9_classical.raw");
        searcher.set_dynamic_data_filename("IEEE9_classical.dyr");

        DEVICE_ID did = fault_lines[i];
        searcher.set_fault_device(did);
        searcher.set_fault_side_bus(fault_side_buses[i]);
        searcher.set_fault_location_to_fault_side_bus_in_pu(0.0);
        searcher.set_fault_shunt_in_pu(complex<double>(0.0, -2e8));
        searcher.set_flag_trip_line_after_clearing_fault(true);

        osstream<<"Now go searching CCT for fault at side "<<searcher.get_fault_side_bus()<<" of "<<did.get_device_name();
        show_information_with_leading_time_stamp(osstream);
        double cct = searcher.search_cct();
        osstream<<"Now done searching CCT for fault at side "<<searcher.get_fault_side_bus()<<" of "<<did.get_device_name();
        show_information_with_leading_time_stamp(osstream);
        ccts[i] = cct;

        searcher.run_case_with_clearing_time(cct);
        searcher.run_case_with_clearing_time(cct+0.1);
    }

    ostringstream osstream;
    osstream<<"Searched CCT of all lines : "<<endl;
    for(size_t i=0; i!=n_events; ++i)
        osstream<<fault_lines[i].get_device_name()<<" at fault side "<<fault_side_buses[i]<<": "<<ccts[i]<<" s."<<endl;
    show_information_with_leading_time_stamp(osstream);

    terminate_simulator();

    return 0;
}
