#ifndef GREATEST_POWER_CURRENT_MISMATCH_STRUCT_H
#define GREATEST_POWER_CURRENT_MISMATCH_STRUCT_H

struct GREATEST_POWER_CURRENT_MISMATCH_STRUCT
{
    double greatest_power_mismatch_in_MVA;
    unsigned int bus_with_greatest_power_mismatch;
    double greatest_current_mismatch_in_pu;
    unsigned int bus_with_greatest_current_mismatch;
};

#endif // GREATEST_POWER_CURRENT_MISMATCH_STRUCT_H
