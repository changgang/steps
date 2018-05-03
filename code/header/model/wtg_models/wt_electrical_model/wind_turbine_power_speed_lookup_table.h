#ifndef WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H
#define WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H

#include "header/block/integral_block.h"
class WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE
{
    /*
    Lookup table for wind turbine generator speed based on wind power generation, or vice verse
    */
    public:
        WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE();
        virtual ~WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE();
        WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table);
        virtual WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& operator=(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table);
    public:
        void clear();
        void add_wind_turbine_power_and_speed_pair_in_pu(double power, double speed);
        double get_reference_speed_with_power_in_pu(double power);
        double get_reference_power_with_speed_in_pu(double speed);
        vector<double> get_wind_turbine_power_table() const;
        vector<double> get_wind_turbine_speed_table() const;
    private:
        void copy_from_const_table(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table);
        vector<double> power_table;
        vector<double> speed_table;
};

#endif // WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H
