#ifndef STEPS_ENUM_H
#define STEPS_ENUM_H

#include <iso646.h>

enum STEPS_DEVICE_TYPE
{
    STEPS_BUS = 0,
    // devices connecting to single bus
    STEPS_GENERATOR =  1,
    STEPS_WT_GENERATOR = 2,
    STEPS_PV_UNIT = 3,
    STEPS_ENERGY_STORAGE = 4,
    STEPS_LOAD = 5,
    STEPS_FIXED_SHUNT = 6,
    STEPS_SWITCHED_SHUNT = 7,
    STEPS_EQUIVALENT_DEVICE = 10,
    // devices connecting to multi buses
    STEPS_AC_LINE = 11,
    STEPS_TRANSFORMER = 12,
    STEPS_LCC_HVDC2T = 13,
    STEPS_VSC_HVDC = 14,
    STEPS_FACTS = 15,
    STEPS_LCC_HVDC = 16,
    STEPS_HYBRID_DC = 17,

    STEPS_GENERAL_DEVICE = 102,

    STEPS_INVALID_DEVICE = 9999,
};

enum STEPS_DC_DEVICE_TYPE
{
    STEPS_DC_BUS = 0,
    STEPS_DC_LINE = 1,
    STEPS_DC_SOURCE = 2,
    STEPS_DC_LOAD = 3,

    STEPS_GENERAL_DC_DEVICE = 102,

    STEPS_INVALID_DC_DEVICE = 9999,
};

// bus enum
enum BUS_TYPE
{
    OUT_OF_SERVICE=0,
    PQ_TYPE=1,
    PV_TYPE=2,
    SLACK_TYPE=3,

    PV_TO_PQ_TYPE_1 = 4,
    PV_TO_PQ_TYPE_2 = 5,
    PV_TO_PQ_TYPE_3 = 6,
    PV_TO_PQ_TYPE_4 = 7,
};

// source enum
enum SOURCE_TYPE
{
    SYNC_GENERATOR_SOURCE = 0,
    WT_GENERATOR_SOURCE = 1,
    PV_UNIT_SOURCE = 2,
    ENERGY_STORAGE_SOURCE = 3
};

enum LOAD_CURRENT_VOLTAGE_REDUCE_TYPE
{
    LOAD_ELLIPTICAL_CV = 0,
    LOAD_LINEAR_CV = 1,
    LOAD_CONSTANT_CV = 2
};

// line enums
enum LINE_SIDE
{
    SENDING_SIDE = 0,
    RECEIVING_SIDE = 1
};

// transformer enums
enum TRANSFORMER_WINDING_SIDE
{
    PRIMARY_SIDE = 0,
    SECONDARY_SIDE = 1,
    TERTIARY_SIDE = 2,
};

enum TRANSFORMER_WINDING_CONNECTION_TYPE
{
    WYE_CONNECTION = 1,
    DELTA_CONNECTION = 2,
    WYE_N_CONNECTION = 3
};

enum TRANSFORMER_WINDING_TAP_CODE
{
    TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE = 1,
    TAP_WINDING_VOLTAGE_IN_KV = 2,
    TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE = 3 // default
};

enum TRANSFORMER_IMPEDANCE_CODE
{
    IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE = 1,
    IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE = 2,  // default
    IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE = 3,
};

enum TRANSFORMER_ADMITTANCE_CODE
{
    ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE = 1, // default
    ADMITTANCE_LOSS_IN_WATT_AND_CURRENT_IN_PU_ON_PRIMARY_SECONDARY_WINDINGS_POWER_AND_PRIMARY_WINDING_NOMINAL_VOLTAGE = 2
};

enum TRANSFORMER_WINDING_CONTROL_MODE
{
    TRANSFORMER_TAP_NO_CONTROL = 0,
    TRANSFORMER_TAP_VOLTAGE_CONTROL = 1,
    TRANSFORMER_TAP_REACTIVE_POWER_CONTROL = 2,
    TRANSFORMER_TAP_ACTIVE_POWER_CONTROL = 3,
    TRANSFORMER_TAP_HVDC_LINE_CONTROL = 4,
    TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL = 5
};

// hvdc enum
enum CONVERTER_SIDE
{
    RECTIFIER = 0,
    INVERTER = 1
};

enum HVDC_OPERATION_MODE
{
    RECTIFIER_CONSTANT_POWER = 1,
    RECTIFIER_CONSTANT_CURRENT = 2,
    INVERTER_CONSTANT_VOLTAGE = 3,
    INVERTER_CONSTANT_GAMMA = 4
};

enum LCC_HVDC_OPERATION_MODE
{
    RECTIFIER_CONSTANT_POWER_AND_CURRENT = 1,
    RECTIFIER_CONSTANT_POWER_INVERTER_CONSTANT_VOLTAGE = 2,
    RECTIFIER_CONSTANT_POWER_AND_VOLTAGE = 3,
};


enum HVDC_POLE
{
    SINGLE_POLE = 1,
    DOUBLE_POLE = 2
};

enum VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE
{
    VSC_DC_VOLTAGE_CONTORL = 1,
    VSC_AC_ACTIVE_POWER_CONTORL = 2,
    VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL = 3,
    VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL = 4,
    VSC_AC_VOLTAGE_ANGLE_CONTROL = 5,
    VSC_INVALID_DC_CONTORL = 999
};

enum VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE
{
    VSC_AC_VOLTAGE_CONTROL = 1,
    VSC_AC_REACTIVE_POWER_CONTROL = 2,
    VSC_INVALID_AC_CONTORL = 999
};

enum VSC_HVDC_CONVERTER_CONTROL_MODE
{
    CURRENT_VECTOR_CONTROL = 1,
    VIRTUAL_SYNCHRONOUS_GENERATOR_CONTROL = 2
};

enum POWERFLOW_DATA_SAVE_MODE
{
    SAVE_TO_KEEP_ORIGINAL_BUS_ORDER = 0,
    SAVE_TO_ORDER_BUS_WITH_BUS_NUMBER = 1,
    SAVE_TO_ORDER_BUS_WITH_BUS_NAME = 2,
    SAVE_TO_ORDER_BUS_WITH_DYNAMIC_DEVICE_ORDER = 3,
};

enum LIMITER_TYPE
{
    NO_LIMITER = 0,
    WINDUP_LIMITER = 1,
    NON_WINDUP_LIMITER = 2
};

enum DYNAMIC_MODE
{
    DYNAMIC_INITIALIZE_MODE = 1,
    DYNAMIC_INTEGRATE_MODE = 2,
    DYNAMIC_UPDATE_MODE = 3,
    DYNAMIC_RELAY_MODE = 4,
    DYNAMIC_UPDATE_TIME_STEP_MODE = 5
};

enum BLOCK_INTEGRATION_TIME_STEP_MODE
{
    NORMAL_INTEGRATION_TIME_STEP_MODE = 0,
    SMALL_INTEGRATION_TIME_STEP_MODE = 1,
    LARGE_INTEGRATION_TIME_STEP_MODE = 2
};

enum DYNAMIC_EVENT_TYPE
{
    FAULT_OR_OPERATION_EVENT = 0,
    TIME_STEP_CHANGE_EVENT = 1,
};

enum FAULT_TYPE
{
    SINGLE_PHASE_GROUNDED_FAULT = 10,
    DOUBLE_PHASES_FAULT = 2,
    DOUBLE_PHASES_GROUNDED_FAULT = 20,
    THREE_PHASES_FAULT = 3,
};

enum UFLS_TRIGGER_SIGNAL
{
    REALTIME_FREQUENCY = 0,
    MINIMUM_FREQUENCY =1,
};

enum SUBSYSTEM_TYPE
{
    BUS_SUBSYSTEM_TYPE = 0,
    OWNER_SUBSYSTEM_TYPE = 1,
    ZONE_SUBSYSTEM_TYPE = 2,
    AREA_SUBSYSTEM_TYPE = 3,
    ALL_SYSTEM_TYPE = 4
};


enum SATURATION_TYPE
{
    QUADRATIC_SATURATION_TYPE = 0,
    EXPONENTIAL_SATURATION_TYPE_INPUT_AS_BASE = 1,
    EXPONENTIAL_SATURATION_TYPE_INPUT_AS_EXPONETIAL = 2
};

enum DOUBLE_CAGE_MOTOR_TYPE
{
    PARALLEL_TYPE = 1,
    SERIES_TYPE = 2
};


enum AVR_FEEDBACK_SLOT
{
    AT_VOLTAGE_ERROR = 0,
    AT_REGULATOR = 1
};

enum AVR_EXCITATION_SOURCE
{
    SELF_EXCITATION = 0,
    SEPARATE_EXCITATION = 1
};

enum AVR_EXCITER_BRUSH
{
    WITHOUT_BRUSH = 0,
    WITH_BRUSH = 1
};

enum AVR_TUNER_TYPE
{
    SERIAL_TUNER = 0,
    PARALLEL_TUNER = 1
};

enum PE_VAR_CONTROL_MODE
{
    CONSTANT_VAR_MODE = 0,
    CONSTANT_VOLTAGE_MODE =1,
    CONSTANT_POWER_FACTOR_MODE = -1
};



enum VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE
{
    DY_VSC_DC_VOLTAGE_CONTORL = 1,
    DY_VSC_AC_ACTIVE_POWER_CONTORL = 2,
    DY_VSC_DC_ACTIVE_POWER_VOLTAGE_DROOP_CONTROL = 3,
    DY_VSC_DC_CURRENT_VOLTAGE_DROOP_CONTROL = 4,
    DY_VSC_FREQUENCY_CONTROL = 5,
    DY_VSC_DC_VOLTAGE_FREQUENCY_CONTROL = 6,
    DY_VSC_INVALID_ACTIVE_POWER_CONTORL_MODE = 999
};

enum VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE
{
    DY_VSC_AC_VOLTAGE_CONTROL = 1,
    DY_VSC_AC_REACTIVE_POWER_CONTROL = 2,
    DY_VSC_INVALID_REACTIVE_POWER_CONTORL_MODE = 999
};

enum STEPS_DYNAMIC_MODEL_TYPE
{
    STEPS_SG_SYNC_GENERATOR,
    STEPS_SG_EXCITER,
    STEPS_SG_STABILIZER,
    STEPS_SG_COMPENSATOR,
    STEPS_SG_TURBINE_GOVERNOR,
    STEPS_SG_TURBINE_LOAD_CONTROLLER,

    STEPS_WTG_GENERATOR,
    STEPS_WTG_ELECTRICAL,

    STEPS_VSC_HVDC_PROJECT,
    STEPS_VSC_HVDC_CONVERTER
};

enum GENERATOR_REACTANCE_OPTION
{
    SUBTRANSIENT_REACTANCE = 0,
    TRANSIENT_REACTANCE = 1,
    SYNCHRONOUS_REACTANCE = 2
};

enum WT_GENERATOR_TYPE
{
    CONSTANT_SPEED_WT_GENERATOR = 1,
    DOUBLY_FED_WT_GENERATOR = 2,
    DIRECT_DRIVEN_WT_GENERATOR = 3
};

enum UNITS_OPTION
{
    PU = 0,
    PHYSICAL = 1
};
enum COORDINATES_OPTION
{
    RECTANGULAR = 0,
    POLAR = 1
};

enum MBASE_CODE
{
    DEFAULT_RATIO_OF_ACTUAL_MACHINE_POWER = 0,
    MACHINE_BASE_POWER = 1,
    CUSTOM_RATIO_OF_ACTUAL_MACHINE_POWER = 2

};

enum DC_LINES_OPTION
{
    BLOCK_AND_IGNORE = 0,
    CONVERT_TO_CONSTANT_ADMITTANCE_LOAD = 1
};

enum COL_PQ_PRIORITY // Current Order Limiter
{
    P_FIRST = 0,
    Q_FIRST = 1
};

enum VRT_STATUS // voltage ride through status
{
    VRT_NORMAL_STATUS = 0,
    VRT_DURING_STATUS = 1,
    VRT_PREPARE2RECOVER_STATUS = 2,
    VRT_RECOVER_STATUS = 3
};

enum VRT_DURING_CONTROL_MODE
{
    VRT_DURING_POWER_CONTROL_MODE = 0,
    VRT_DURING_V_DEPEND_CURRENT_CONTROL_MODE = 1,
    VRT_DURING_CONSTANT_CURRENT_CONTROL_MODE = 2,
    VRT_DURING_LVPL_CURRENT_CONTROL_MODE = 3, // only used for active power
};

enum VRT_PREPARE2RECOVER_CONTROL_MODE
{
    VRT_PREPARE2RECOVER_INITIAL_CURRENT_CONTROL_MODE = 0,
    VRT_PREPARE2RECOVER_FAULTED_CURRENT_CONTROL_MODE = 1,
    VRT_PREPARE2RECOVER_FAULTED_POWER_CONTROL_MODE = 2
};

enum VRT_RECOVER_CONTROL_MODE
{
    VRT_RECOVER_LINEAR_CONTROL_MODE = 0,
    VRT_RECOVER_EXPOENTIAL_CONTROL_MODE = 1
};

#endif // STEPS_ENUM_H
