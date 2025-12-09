#include "header/steps_namespace.h"

STR_INT_MAP toolkit_str_int_map;
const vector<STEPS_DEVICE_TYPE> STEPS_DEVICE_TYPE_LIST = {STEPS_BUS,
                                                          STEPS_GENERATOR, STEPS_WT_GENERATOR, STEPS_PV_UNIT,
                                                          STEPS_ENERGY_STORAGE, STEPS_SV_SOURCE,
                                                          STEPS_LOAD,
                                                          STEPS_FIXED_SHUNT,
                                                          STEPS_AC_LINE, STEPS_TRANSFORMER,
                                                          STEPS_LCC_HVDC2T, STEPS_VSC_HVDC,
                                                          STEPS_EQUIVALENT_DEVICE};
const vector<STEPS_DC_DEVICE_TYPE> STEPS_DC_DEVICE_TYPE_LIST = {STEPS_DC_BUS, STEPS_DC_LINE};


unordered_map<string, function<double(METER_PARAMETER)>> bus_meter_map,
                                                         sg_meter_map, wtg_meter_map, pvu_meter_map, es_meter_map, sv_source_meter_map,
                                                         load_meter_map, fixed_shunt_meter_map,
                                                         ac_line_meter_map, transformer_meter_map,
                                                         lcc_hvdc2t_meter_map, vsc_hvdc_meter_map, mt_hvdc_meter_map,
                                                         equivalent_device_meter_map;

STEPS default_toolkit;
STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];
std::mutex mtx;

bool use_steps_fast_math = false;
bool package_initialized = false;

