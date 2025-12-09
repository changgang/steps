#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/STEPS.h"
#include "header/basic/steps_define.h"
#include "header/basic/constants.h"
#include "header/basic/string_int_map.h"
#include "header/meter/meter_parameter_struct.h"

#include <mutex>

extern STR_INT_MAP toolkit_str_int_map;
extern const vector<STEPS_DEVICE_TYPE> STEPS_DEVICE_TYPE_LIST;
extern const vector<STEPS_DC_DEVICE_TYPE> STEPS_DC_DEVICE_TYPE_LIST;
extern unordered_map<string, function<double(METER_PARAMETER)>> bus_meter_map,
                                                                sg_meter_map, wtg_meter_map, pvu_meter_map, es_meter_map, sv_source_meter_map,
                                                                load_meter_map, fixed_shunt_meter_map,
                                                                ac_line_meter_map, transformer_meter_map,
                                                                lcc_hvdc2t_meter_map, vsc_hvdc_meter_map, mt_hvdc_meter_map,
                                                                equivalent_device_meter_map;
extern STEPS default_toolkit;
extern STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];

extern std::mutex mtx;

extern bool use_steps_fast_math;
extern bool package_initialized;

#endif // STEPS_NAMESPACE_H
