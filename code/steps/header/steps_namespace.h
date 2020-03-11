#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/STEPS.h"
#include "header/basic/constants.h"

#include <mutex>

extern STEPS default_toolkit;
extern STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];
extern std::mutex mtx;

extern bool use_steps_fast_math;

#endif // STEPS_NAMESPACE_H
