#ifndef DC_BUS_H
#define DC_BUS_H

#include "header/device/device.h"
#include "header/basic/area.h"
#include "header/basic/zone.h"
#include "header/basic/owner.h"
#include "header/basic/steps_enum.h"




using namespace std;


class DC_BUS
{
    public:
        DC_BUS(STEPS& toolkit);
        ~DC_BUS();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit();

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual void report() const;

        //set
        void set_status(const bool status);
        void set_name(const string name);
        void set_nominal_voltage(const double vn);
        void set_dc_bus_number(const unsigned int number);
        void set_dc_bus_voltage_in_kV(const double vdc);
        void set_dc_bus_current_in_kA(const double idc);
        void set_dc_bus_power_in_MW(const double pdc);
        void set_dc_bus_area(const unsigned int area);
        void set_dc_bus_zone(const unsigned int zone);
        void set_dc_bus_type(const DC_BUS_TYPE type);
        void set_dc_bus_dynamic_type(const DYNAMIC_DC_BUS_TYPE dy_type);


        //get
        bool get_status() const;
        string get_name() const;
        unsigned int get_dc_bus_number() const;
        double get_nominal_voltage() const;
        unsigned int get_converter_index() const;
        double get_dc_bus_voltage_in_kV() const;
        double get_dc_bus_current_in_kA() const;
        double get_dc_bus_power_in_MW() const;
        unsigned int get_dc_bus_area() const;
        unsigned int get_dc_bus_zone() const;
        DC_BUS_TYPE get_dc_bus_type() const;

        DYNAMIC_DC_BUS_TYPE get_dc_bus_dynamic_type() const;


    private:
        STEPS* toolkit;
        bool status;
        unsigned int dc_bus_number;
        string name;
        double nominal_voltage;
        double Vdc_kV, Idc_kA, Pdc_MW;
        unsigned int dc_bus_area;
        unsigned int dc_bus_zone;

        DC_BUS_TYPE dc_bus_type;
        DYNAMIC_DC_BUS_TYPE dy_dc_bus_type;

        //dp = pc+pg-pl-f(u) = 0

};

#endif // DC_BUS_H
