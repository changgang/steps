#ifndef AREA_H
#define AREA_H

#include <string>
using namespace std;

class STEPS;

class AREA
{
    public:
        AREA(STEPS& toolkit);
        virtual ~AREA();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_area_number(unsigned int area_number);
        void set_area_name(string area_name);
        void set_area_swing_bus(unsigned int bus);
        void set_expected_power_leaving_area_in_MW(double P);
        void set_area_power_mismatch_tolerance_in_MW(double P);

        unsigned int get_area_number() const;
        string get_area_name() const;
        unsigned int get_area_swing_bus() const;
        double get_expected_power_leaving_area_in_MW() const;
        double get_area_power_mismatch_tolerance_in_MW() const;

        bool is_valid() const;
        void check();
        void clear();
        void report() const;

        virtual AREA& operator=(const AREA& area);
        //double get_actual_power_leaving_area_in_MW() const;
    private:
        void set_area_swing_bus_with_zero_input();
        void set_area_swing_bus_with_existing_bus(unsigned int bus);

        STEPS* toolkit;

        unsigned int area_number;
        string area_name;
        unsigned int area_swing_bus;
        float expected_power_leaving_area_in_MW;
        float area_power_mismatch_tolerance_in_MW;
};
#endif // AREA_H
