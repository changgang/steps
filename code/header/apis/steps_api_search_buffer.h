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
        unsigned int bus_pointer;
        vector<GENERATOR*> generators;
        unsigned int generator_pointer;
        vector<WT_GENERATOR*> wt_generators;
        unsigned int wt_generator_pointer;
        vector<PV_UNIT*> pv_units;
        unsigned int pv_unit_pointer;
        vector<LOAD*> loads;
        unsigned int load_pointer;
        vector<FIXED_SHUNT*> fixed_shunts;
        unsigned int fixed_shunt_pointer;
        vector<LINE*> lines;
        unsigned int line_pointer;
        vector<TRANSFORMER*> transformers;
        unsigned int transformer_pointer;
        vector<HVDC*> hvdcs;
        unsigned int hvdc_pointer;
        vector<EQUIVALENT_DEVICE*> equivalent_devices;
        unsigned int equivalent_device_pointer;
        vector<ENERGY_STORAGE*> energy_storages;
        unsigned int energy_storage_pointer;
        vector<AREA*> areas;
        unsigned int area_pointer;
        vector<ZONE*> zones;
        unsigned int zone_pointer;
        vector<OWNER*> owners;
        unsigned int owner_pointer;
};
#endif // STEPS_API_SEARCH_BUFFER_H
