#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/STEPS.h"
#include "header/basic/constants.h"

#include <mutex>

extern STEPS default_toolkit;
extern STEPS* toolkits[MAX_TOOLKIT_SIZE];
extern std::mutex mtx;

#endif // STEPS_NAMESPACE_H
