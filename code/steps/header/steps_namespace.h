#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/STEPS.h"
#include "header/basic/constants.h"
#include "header/basic/string_int_map.h"

#include <mutex>

extern STR_INT_MAP toolkit_str_int_map;
extern vector<STEPS_DEVICE_TYPE> STEPS_DEVICE_TYPE_LIST;
extern vector<STEPS_DC_DEVICE_TYPE> STEPS_DC_DEVICE_TYPE_LIST;
extern STEPS default_toolkit;
extern STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];
extern std::mutex mtx;

extern bool use_steps_fast_math;

#endif // STEPS_NAMESPACE_H
