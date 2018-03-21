#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef PI
    #define PI 3.141592653589793
#endif // PI

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

#ifndef MAX_DATABASE_COUNT
    #define MAX_DATABASE_COUNT 10
#endif // MAX_DATABASE_COUNT

#ifndef MAX_LOAD_RELAY_STAGE
    #define MAX_LOAD_RELAY_STAGE 10
#endif // MAX_LOAD_RELAY_STAGE

#ifndef MAX_STABILIZER_INPUT_SIGNAL_SLOT
    #define MAX_STABILIZER_INPUT_SIGNAL_SLOT 10
#endif // MAX_STABILIZER_INPUT_SIGNAL_SLOT

#ifndef MAX_HISTORY_METER_BUFFER
    #define MAX_HISTORY_METER_BUFFER 100
#endif // MAX_HISTORY_METER_BUFFER

#endif // CONSTANTS_H
