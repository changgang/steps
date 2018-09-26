#ifndef LOAD_H
#define LOAD_H

#include "header/device/device.h"
#include "header/model/load_model/load_model.h"
#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include <string>
#include <complex>

using namespace std;

class LOAD : public DEVICE
{
    public:
        LOAD();
        virtual ~LOAD();

        void set_load_bus(size_t bus);
        void set_identifier(string identifier);
        void set_status(bool status);
        void set_nominal_constant_power_load_in_MVA(complex<double> s);
        void set_nominal_constant_current_load_in_MVA(complex<double> s);
        void set_nominal_constant_impedance_load_in_MVA(complex<double> s);
        void set_area_number(size_t num);
        void set_zone_number(size_t num);
        void set_owner_number(size_t num);
        void set_flag_interruptable(bool flag);
        void set_load_manually_scale_factor_in_pu(double scale);

        size_t get_load_bus() const;
        string get_identifier() const;
        bool get_status() const;
        complex<double> get_nominal_constant_power_load_in_MVA() const;
        complex<double> get_nominal_constant_current_load_in_MVA() const;
        complex<double> get_nominal_constant_impedance_load_in_MVA() const;
        size_t get_area_number() const;
        size_t get_zone_number() const;
        size_t get_owner_number() const;
        bool get_flag_interruptable() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(size_t bus) const;
        virtual bool is_in_area(size_t area) const;
        virtual bool is_in_zone(size_t zone) const;
        virtual void report() const;
        virtual void save() const;


        LOAD& operator=(const LOAD& load);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        complex<double> get_nominal_total_load_in_MVA() const;
        complex<double> get_actual_total_load_in_MVA() const;
        complex<double> get_actual_constant_power_load_in_MVA() const;
        complex<double> get_actual_constant_current_load_in_MVA() const;
        complex<double> get_actual_constant_impedance_load_in_MVA() const;

        static void set_voltage_threshold_of_constant_power_load_in_pu(double v);
        static double get_voltage_threshold_of_constant_power_load_in_pu();

        virtual void set_model(const MODEL* model);

        void set_load_model(const LOAD_MODEL* model);
        void set_load_frequency_relay_model(const LOAD_FREQUENCY_RELAY_MODEL* model);
        void set_load_voltage_relay_model(const LOAD_VOLTAGE_RELAY_MODEL* model);

        LOAD_MODEL* get_load_model() const;
        LOAD_FREQUENCY_RELAY_MODEL* get_load_frequency_relay_model() const;
        LOAD_VOLTAGE_RELAY_MODEL* get_load_voltage_relay_model() const;

        virtual void run(DYNAMIC_MODE mode);
        complex<double> get_dynamic_load_in_MVA();
        complex<double> get_dynamic_load_in_pu();
        double get_load_total_scale_factor_in_pu() const;
        double get_load_manually_scale_factor_in_pu() const;
        double get_load_relay_shed_scale_factor_in_pu() const;
        complex<double> get_dynamics_load_current_in_pu_based_on_system_base_power();
    private:
        size_t bus;
        string identifier;
        bool status;
        complex<double> s_constant_power_in_MVA,
                        s_constant_current_in_MVA,
                        s_constant_impedance_in_MVA;
        size_t area_number, zone_number, owner_number;
        bool interruptable;

        static double voltage_threshold_of_constant_power_load_in_pu;

        double manually_scale_in_pu;

        LOAD_MODEL* load_model;
        LOAD_VOLTAGE_RELAY_MODEL* load_voltage_relay_model;
        LOAD_FREQUENCY_RELAY_MODEL* load_frequency_relay_model;
};
#endif // LOAD_H
