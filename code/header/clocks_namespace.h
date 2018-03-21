#ifndef STEPS_NAMESPACE_H
#define STEPS_NAMESPACE_H

#include "header/basic/bus_index.h"
#include "header/basic/device_index_map.h"
#include "header/basic/inphno.h"
#include "header/device/bus.h"
#include "header/device/source.h"
#include "header/device/load.h"
#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/device/fixed_shunt.h"
#include "header/device/hvdc.h"

#include "header/basic/sparse_matrix.h"

#include <string>
#include <ctime>

enum TRANSIENT_SIMULATION_MODE
{
    TRANSIENT_INITIALIZATION_MODE = 0,
    TRANSIENT_INTEGRATION_MODE = 1,
    TRANSIENT_UPDATE_MODE = 2
};

using namespace std;

namespace STEPS
{
    extern time_t clock_when_system_started;
    extern double system_base_power_in_MVA;
    extern double system_base_fequency_in_Hz;

    extern vector<AREA> Area;
    extern vector<ZONE> Zone;
    extern vector<OWNER> Owner;
    extern vector<BUS> Bus;
    extern vector<SOURCE> Source;
    extern vector<LOAD> Load;
    extern vector<FIXED_SHUNT> Fixed_shunt;
    extern vector<LINE> Line;
    extern vector<TRANSFORMER> Transformer;
    extern vector<HVDC> Hvdc;

    extern BUS_INDEX bus_index;
    extern DEVICE_INDEX_MAP source_index, load_index, fixed_shunt_index, switched_shunt_index,
                            line_index, transformer_index, hvdc_index;
    extern INPHNO inphno;

    extern SPARSE_MATRIX network_Y_matrix;


    extern TRANSIENT_SIMULATION_MODE transient_simulation_mode;
    extern double transient_simulation_step_in_s;
}
#endif // STEPS_NAMESPACE_H
