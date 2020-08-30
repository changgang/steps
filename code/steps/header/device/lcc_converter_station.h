#ifndef LCC_CONVERTER_STATION_H
#define LCC_CONVERTER_STATION_H

#include "header/device/nonbus_device.h"
#include "header/device/converter_station.h"
#include "header/device/vsc.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"

#include <complex>
#include <string>

using namespace std;

class BUS;

class LCC_CONVERTER_STATION : public CONVERTER_STATION
{
    public:
        LCC_CONVERTER_STATION();
        virtual ~LCC_CONVERTER_STATION();
        void clear();

        virtual LCC_CONVERTER_STATION& operator=(const LCC_CONVERTER_STATION& station);

        void set_number_of_converters(unsigned int n);
        void set_converter(unsigned int converter_index, LCC& converter);

        unsigned int get_number_of_converters() const;
        const LCC* get_converter(unsigned int converter_index) const;
        LCC* get_mutable_converter(unsigned int converter_index);
        unsigned int get_total_power_percent_of_converter_station() const;

        void solve_with_desired_dc_voltage_and_current(double Vdc, double Idc);
        //void solve_transformer_tap_and_angle(double Vdc, double Idc);

        double get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle() const;
        double get_dc_voltage_drop_multiple_in_kV_per_A() const;

        void report() const;

        bool is_connected_to_bus(unsigned int bus) const;
        bool is_in_area(unsigned int area) const;
        bool is_in_zone(unsigned int zone) const;
    private:
        LCC* converters;
        unsigned int nconverters;
};
#endif // HVDC_H
