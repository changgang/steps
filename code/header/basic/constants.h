#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef PI
    #define PI 3.141592653589793
#endif // PI

#ifndef PI2
    #define PI2 PI*2.0
#endif // PI2

#ifndef HALF_PI
    #define HALF_PI PI*0.5
#endif // HALF_PI

#ifndef QUARTER_PI
    #define QUARTER_PI PI*0.25
#endif // QUARTER_PI

#ifndef THIRD_PI
    #define THIRD_PI PI/3.0
#endif // THIRD_PI

#ifndef FLOAT_EPSILON
    #define FLOAT_EPSILON 1e-10
#endif // FLOAT_EPSILON

#ifndef INDEX_NOT_EXIST
    #define INDEX_NOT_EXIST 1000000000
#endif // INDEX_NOT_EXIST

#ifndef INFINITE_THRESHOLD
    #define INFINITE_THRESHOLD 1e10
#endif // INFINITE_THRESHOLD

#ifndef get_variable_name
    #define get_variable_name(x) #x
#endif // get_variable_name(x)

#ifndef MAX_TEMP_CHAR_BUFFER_SIZE
#define MAX_TEMP_CHAR_BUFFER_SIZE 500
#endif // MAX_TEMP_CHAR_BUFFER_SIZE

#ifndef MAX_LOAD_RELAY_STAGE
    #define MAX_LOAD_RELAY_STAGE 50
#endif // MAX_LOAD_RELAY_STAGE

#ifndef MAX_RELAY_COUNT
    #define MAX_RELAY_COUNT 5
#endif // MAX_RELAY_COUNT


#ifndef MAX_STABILIZER_INPUT_SIGNAL_SLOT
    #define MAX_STABILIZER_INPUT_SIGNAL_SLOT 10
#endif // MAX_STABILIZER_INPUT_SIGNAL_SLOT

#ifndef MAX_HISTORY_METER_BUFFER
    #define MAX_HISTORY_METER_BUFFER 100
#endif // MAX_HISTORY_METER_BUFFER


#ifndef STEPS_MAGIC1
    #define STEPS_MAGIC1 621877636
#endif // STEPS_MAGIC1

#ifndef STEPS_MAGIC2
    #define STEPS_MAGIC2 40966531
#endif // STEPS_MAGIC2

#ifndef STEPS_MAGIC3
    #define STEPS_MAGIC3 20389910
#endif // STEPS_MAGIC3

#ifndef STEPS_MAGIC4
    #define STEPS_MAGIC4 7807000
#endif // STEPS_MAGIC4

#endif // CONSTANTS_H
