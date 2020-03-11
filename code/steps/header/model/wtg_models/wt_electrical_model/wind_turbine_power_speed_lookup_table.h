#ifndef WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H
#define WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H

#include "header/basic/constants.h"
#include <vector>

using namespace std;
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
        const vector<vector<double> > get_wind_turbine_power_speed_table() const;
        double get_power_of_record(unsigned int i) const;
        double get_speed_of_record(unsigned int i) const;
    private:
        void copy_from_const_table(const WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE& table);
        unsigned int get_valid_record_size() const;
        void reorder_records();
        bool is_power_record_exist(double power) const;
        unsigned int get_the_last_record_with_power_less_than_or_equal_to(double power);

        double power_speed_table[STEPS_MAX_WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_SIZE][2];
};

#endif // WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE_H
