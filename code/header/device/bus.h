#ifndef BUS_H
#define BUS_H

#include "header/device/device.h"
#include "header/basic/area.h"
#include "header/basic/zone.h"
#include "header/basic/owner.h"
#include "header/basic/steps_enum.h"
#include "header/basic/fault.h"

#include "header/model/bus_frequency_model/bus_frequency_model.h"

#include <string>
#include <complex>
#include <map>

using namespace std;

class POWER_SYSTEM_DATABASE;

class BUS : public DEVICE
{
    public:
        BUS();
        BUS(const BUS& bus);
        virtual ~BUS();

        void set_bus_number(size_t number);
        void set_bus_name(string name);
        void set_base_voltage_in_kV(double voltage);
        void set_bus_type(BUS_TYPE type);
        void set_area_number(size_t number);
        void set_zone_number(size_t number);
        void set_owner_number(size_t number);
        void set_voltage_in_pu(double voltage);
        void set_voltage_in_kV(double voltage);
        void set_angle_in_rad(double angle);
        void set_angle_in_deg(double angle);
        void set_normal_voltage_upper_limit_in_pu(double voltage);
        void set_normal_voltage_lower_limit_in_pu(double voltage);
        void set_emergency_voltage_upper_limit_in_pu(double voltage);
        void set_emergency_voltage_lower_limit_in_pu(double voltage);
        void set_voltage_upper_limit_in_pu(double voltage);
        void set_voltage_lower_limit_in_pu(double voltage);
        void set_base_frequency_in_Hz(double fn);
        void set_voltage_to_regulate_in_pu(double voltage);

        size_t get_bus_number() const;
        string get_bus_name() const;
        double get_base_voltage_in_kV() const;
        BUS_TYPE get_bus_type() const;
        size_t get_area_number() const;
        size_t get_zone_number() const;
        size_t get_owner_number() const;
        double get_voltage_in_pu() const;
        double get_voltage_in_kV() const;
        double get_angle_in_rad() const;
        double get_angle_in_deg() const;
        double get_normal_voltage_upper_limit_in_pu() const;
        double get_normal_voltage_lower_limit_in_pu() const;
        double get_emergency_voltage_upper_limit_in_pu() const;
        double get_emergency_voltage_lower_limit_in_pu() const;
        double get_voltage_upper_limit_in_pu() const;
        double get_voltage_lower_limit_in_pu() const;
        double get_base_frequency_in_Hz() const;
        double get_voltage_to_regulate_in_pu() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(size_t bus) const;
        virtual bool is_in_area(size_t area) const;
        virtual bool is_in_zone(size_t zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual bool operator<(const BUS& bus) const;
        virtual BUS& operator=(const BUS& bus);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        void set_fault(FAULT fault);
        FAULT get_fault() const;
        void clear_fault();
        bool is_faulted() const;

        BUS_FREQUENCY_MODEL* get_bus_frequency_model();
        void set_frequency_deviation_in_pu(double f);
        double get_frequency_deviation_in_pu();
        double get_frequency_deviation_in_Hz();
    private:
        size_t bus_number;
        string bus_name;
        double base_voltage_in_kV;
        BUS_TYPE bus_type;
        size_t area_number;
        size_t zone_number;
        size_t owner_number;

        double voltage_in_pu;
        double angle_in_rad;

        double normal_voltage_upper_limit_in_pu;
        double normal_voltage_lower_limit_in_pu;
        double emergency_voltage_upper_limit_in_pu;
        double emergency_voltage_lower_limit_in_pu;

        double voltage_to_regulate_in_pu;

        BUS_FREQUENCY_MODEL bus_frequency_model;

        FAULT fault;
    private:
        void copy_from_const_bus(const BUS& bus);
        virtual void set_model(const MODEL* model);
        double fn_Hz;
};

#endif // BUS_H
