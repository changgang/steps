#include "header/steps_namespace.h"

namespace STEPS
{
    POWER_SYSTEM_DATABASE default_power_system_db;
    POWERFLOW_SOLVER default_powerflow_solver;
    DYNAMICS_SIMULATOR default_dynamic_simulator(&default_power_system_db);

    double DELT = 0.01;
    double TIME;

    time_t clock_when_system_started=clock();

	char steps_char_buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
}


