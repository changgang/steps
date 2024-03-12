#include "header/apis/steps_api_search_buffer.h"
#include <iostream>
using namespace std;

STEPS_API_SEARCH_BUFFER::STEPS_API_SEARCH_BUFFER()
{
    buses.clear();
    bus_pointer = 0;
    generators.clear();
    generator_pointer = 0;
    wt_generators.clear();
    wt_generator_pointer = 0;
    pv_units.clear();
    pv_unit_pointer = 0;
    energy_storages.clear();
    energy_storage_pointer = 0;
    loads.clear();
    load_pointer = 0;
    fixed_shunts.clear();
    fixed_shunt_pointer = 0;
    ac_lines.clear();
    ac_line_pointer = 0;
    transformers.clear();
    transformer_pointer = 0;
    hvdcs.clear();
    hvdc_pointer = 0;
    equivalent_devices.clear();
    equivalent_device_pointer = 0;
    areas.clear();
    area_pointer = 0;
    zones.clear();
    zone_pointer = 0;
    owners.clear();
    owner_pointer = 0;
}

STEPS_API_SEARCH_BUFFER::~STEPS_API_SEARCH_BUFFER()
{
    ;
}
