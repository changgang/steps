#include "header/steps_namespace.h"

STR_INT_MAP toolkit_str_int_map;
vector<STEPS_DEVICE_TYPE> STEPS_DEVICE_TYPE_LIST = {STEPS_BUS,
                                                    STEPS_GENERATOR, STEPS_WT_GENERATOR, STEPS_PV_UNIT, STEPS_ENERGY_STORAGE,
                                                    STEPS_LOAD,
                                                    STEPS_FIXED_SHUNT,
                                                    STEPS_AC_LINE, STEPS_TRANSFORMER,
                                                    STEPS_LCC_HVDC2T, STEPS_VSC_HVDC,
                                                    STEPS_EQUIVALENT_DEVICE};
vector<STEPS_DC_DEVICE_TYPE> STEPS_DC_DEVICE_TYPE_LIST = {STEPS_DC_BUS, STEPS_DC_LINE};

STEPS default_toolkit;
STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];
std::mutex mtx;

bool use_steps_fast_math = false;

