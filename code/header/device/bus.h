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

        void set_bus_number(unsigned int number);
        void set_bus_name(string name);
        void set_base_voltage_in_kV(double voltage);
        void set_bus_type(BUS_TYPE type);
        void set_area_number(unsigned int number);
        void set_zone_number(unsigned int number);
        void set_owner_number(unsigned int number);
        void set_positive_sequence_voltage_in_pu(double voltage);
        void set_positive_sequence_voltage_in_kV(double voltage);
        void set_positive_sequence_angle_in_rad(double angle);
        void set_positive_sequence_angle_in_deg(double angle);
        void set_negative_sequence_voltage_in_pu(double voltage);
        void set_negative_sequence_voltage_in_kV(double voltage);
        void set_negative_sequence_angle_in_rad(double angle);
        void set_negative_sequence_angle_in_deg(double angle);
        void set_zero_sequence_voltage_in_pu(double voltage);
        void set_zero_sequence_voltage_in_kV(double voltage);
        void set_zero_sequence_angle_in_rad(double angle);
        void set_zero_sequence_angle_in_deg(double angle);
        void set_normal_voltage_upper_limit_in_pu(double voltage);
        void set_normal_voltage_lower_limit_in_pu(double voltage);
        void set_emergency_voltage_upper_limit_in_pu(double voltage);
        void set_emergency_voltage_lower_limit_in_pu(double voltage);
        void set_voltage_upper_limit_in_pu(double voltage);
        void set_voltage_lower_limit_in_pu(double voltage);
        void set_base_frequency_in_Hz(double fn);
        void set_voltage_to_regulate_in_pu(double voltage);

        unsigned int get_bus_number() const;
        string get_bus_name() const;
        double get_base_voltage_in_kV() const;
        BUS_TYPE get_bus_type() const;
        unsigned int get_area_number() const;
        unsigned int get_zone_number() const;
        unsigned int get_owner_number() const;
        double get_positive_sequence_voltage_in_pu() const;
        double get_positive_sequence_voltage_in_kV() const;
        double get_positive_sequence_angle_in_rad() const;
        double get_positive_sequence_angle_in_deg() const;
        double get_negative_sequence_voltage_in_pu() const;
        double get_negative_sequence_voltage_in_kV() const;
        double get_negative_sequence_angle_in_rad() const;
        double get_negative_sequence_angle_in_deg() const;
        double get_zero_sequence_voltage_in_pu() const;
        double get_zero_sequence_voltage_in_kV() const;
        double get_zero_sequence_angle_in_rad() const;
        double get_zero_sequence_angle_in_deg() const;
        double get_normal_voltage_upper_limit_in_pu() const;
        double get_normal_voltage_lower_limit_in_pu() const;
        double get_emergency_voltage_upper_limit_in_pu() const;
        double get_emergency_voltage_lower_limit_in_pu() const;
        double get_voltage_upper_limit_in_pu() const;
        double get_voltage_lower_limit_in_pu() const;
        double get_base_frequency_in_Hz() const;
        double get_base_period_in_s() const;
        double get_voltage_to_regulate_in_pu() const;
        complex<double> get_positive_sequence_complex_voltage_in_pu() const;
        complex<double> get_positive_sequence_complex_voltage_in_kV() const;
        complex<double> get_negative_sequence_complex_voltage_in_pu() const;
        complex<double> get_negative_sequence_complex_voltage_in_kV() const;
        complex<double> get_zero_sequence_complex_voltage_in_pu() const;
        complex<double> get_zero_sequence_complex_voltage_in_kV() const;

        void set_equivalent_bus_number(unsigned int number);
        unsigned int get_equivalent_bus_number() const;

        bool is_bus_overshadowed() const;


        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual bool operator<(const BUS& bus) const;
        virtual BUS& operator=(const BUS& bus);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        void set_fault(const FAULT& fault);
        FAULT get_fault() const;
        void clear_fault();
        bool is_faulted() const;

        BUS_FREQUENCY_MODEL* get_bus_frequency_model();
        void set_frequency_deviation_in_pu(double f);
        double get_frequency_deviation_in_pu() const;
        double get_frequency_deviation_in_Hz() const;
        double get_frequency_in_pu() const;
        double get_frequency_in_Hz() const;
    private:
        unsigned int bus_number;
        string bus_name;
        double base_voltage_in_kV;
        double fn_Hz, tn_s;
        BUS_TYPE bus_type;
        unsigned int area_number;
        unsigned int zone_number;
        unsigned int owner_number;

        double positive_sequence_voltage_in_pu;
        double positive_sequence_angle_in_rad;
        double negative_sequence_voltage_in_pu;
        double negative_sequence_angle_in_rad;
        double zero_sequence_voltage_in_pu;
        double zero_sequence_angle_in_rad;

        complex<double> positive_sequence_Euler_complex_number;
        complex<double> negative_sequence_Euler_complex_number;
        complex<double> zero_sequence_Euler_complex_number;

        double normal_voltage_upper_limit_in_pu;
        double normal_voltage_lower_limit_in_pu;
        double emergency_voltage_upper_limit_in_pu;
        double emergency_voltage_lower_limit_in_pu;

        double voltage_to_regulate_in_pu;

        unsigned int equivalent_bus_number;

        BUS_FREQUENCY_MODEL bus_frequency_model;

        FAULT fault;
    private:
        void copy_from_const_bus(const BUS& bus);
        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);
};

#endif // BUS_H
