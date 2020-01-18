#include "header/steps_namespace.h"

STEPS default_toolkit;
STEPS* toolkits[STEPS_MAX_TOOLKIT_SIZE];
std::mutex mtx;
