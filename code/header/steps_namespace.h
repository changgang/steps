#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/power_system_database.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/constants.h"
#include <ctime>

using namespace std;

namespace STEPS
{
    extern POWER_SYSTEM_DATABASE power_system_db[MAX_DATABASE_COUNT];

    extern POWER_SYSTEM_DATABASE default_power_system_db;
    extern POWERFLOW_SOLVER default_powerflow_solver;
    extern DYNAMICS_SIMULATOR default_dynamic_simulator;

    extern time_t clock_when_system_started;

    extern double DELT;
    extern double TIME;
}
#endif // STEPS_NAMESPACE_H
