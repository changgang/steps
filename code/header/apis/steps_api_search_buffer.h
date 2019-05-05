#ifndef STEPS_API_SEARCH_BUFFER_H
#define STEPS_API_SEARCH_BUFFER_H

#include "header/power_system_database.h"
#include "header/basic/constants.h"
#include <ctime>
#include <string>

using namespace std;

class STEPS_API_SEARCH_BUFFER
{
    public:
        STEPS_API_SEARCH_BUFFER();
        ~STEPS_API_SEARCH_BUFFER();

        vector<BUS*> buses;
        size_t bus_pointer;
        vector<GENERATOR*> generators;
        size_t generator_pointer;
        vector<WT_GENERATOR*> wt_generators;
        size_t wt_generator_pointer;
        vector<PV_UNIT*> pv_units;
        size_t pv_unit_pointer;
        vector<LOAD*> loads;
        size_t load_pointer;
        vector<FIXED_SHUNT*> fixed_shunts;
        size_t fixed_shunt_pointer;
        vector<LINE*> lines;
        size_t line_pointer;
        vector<TRANSFORMER*> transformers;
        size_t transformer_pointer;
        vector<HVDC*> hvdcs;
        size_t hvdc_pointer;
        vector<EQUIVALENT_DEVICE*> equivalent_devices;
        size_t equivalent_device_pointer;
        vector<ENERGY_STORAGE*> energy_storages;
        size_t energy_storage_pointer;
        vector<AREA*> areas;
        size_t area_pointer;
        vector<ZONE*> zones;
        size_t zone_pointer;
        vector<OWNER*> owners;
        size_t owner_pointer;
};
#endif // STEPS_API_SEARCH_BUFFER_H
