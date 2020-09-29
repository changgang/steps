#ifndef LOAD_H
#define LOAD_H

#include "header/device/nonbus_device.h"
#include "header/model/load_model/load_model.h"
#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include <string>
#include <complex>

using namespace std;

class BUS;
class LOAD : public NONBUS_DEVICE
{
    public:
        LOAD(STEPS& toolkit);
        virtual ~LOAD();

        void set_load_bus(unsigned int bus);
        void set_identifier(string identifier);
        void set_status(bool status);
        void set_nominal_constant_power_load_in_MVA(const complex<double>& s);
        void set_nominal_constant_current_load_in_MVA(const complex<double>& s);
        void set_nominal_constant_impedance_load_in_MVA(const complex<double>& s);
        void set_area_number(unsigned int num);
        void set_zone_number(unsigned int num);
        void set_owner_number(unsigned int num);
        void set_flag_interruptable(bool flag);
        void set_load_manually_scale_factor_in_pu(double scale);

        unsigned int get_load_bus() const;
        BUS* get_bus_pointer() const;

        string get_identifier() const;
        bool get_status() const;
        complex<double> get_nominal_constant_power_load_in_MVA() const;
        complex<double> get_nominal_constant_current_load_in_MVA() const;
        complex<double> get_nominal_constant_impedance_load_in_MVA() const;
        unsigned int get_area_number() const;
        unsigned int get_zone_number() const;
        unsigned int get_owner_number() const;
        bool get_flag_interruptable() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;


        virtual LOAD& operator=(const LOAD& load);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        complex<double> get_nominal_total_load_in_MVA() const;
        complex<double> get_actual_total_load_in_MVA() const;
        complex<double> get_actual_constant_power_load_in_MVA() const;
        complex<double> get_actual_constant_current_load_in_MVA() const;
        complex<double> get_actual_constant_impedance_load_in_MVA() const;
        static double get_load_scale_with_voltage(double exp, double v, LOAD_CURRENT_VOLTAGE_REDUCE_TYPE cv_type = LOAD_ELLIPTICAL_CV);
        static double get_load_scale_with_Imax_and_voltage(double Imax, double v, double vth, LOAD_CURRENT_VOLTAGE_REDUCE_TYPE cv_type);

        static void set_voltage_threshold_of_constant_power_load_in_pu(double v);
        static void set_voltage_threshold_of_constant_current_load_in_pu(double v);
        static double get_voltage_threshold_of_constant_power_load_in_pu();
        static double get_voltage_threshold_of_constant_current_load_in_pu();
        static double get_one_over_voltage_threshold_of_constant_power_load_in_pu();
        static double get_one_over_voltage_threshold_of_constant_current_load_in_pu();

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_load_model(LOAD_MODEL* model);
        void set_load_frequency_relay_model(LOAD_FREQUENCY_RELAY_MODEL* model);
        void set_load_voltage_relay_model(LOAD_VOLTAGE_RELAY_MODEL* model);

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
        complex<double> get_dynamics_load_norton_current_in_pu_based_on_system_base_power();
    private:
        unsigned int bus;
        BUS* busptr;

        string identifier;
        bool status;
        complex<double> s_constant_power_in_MVA,
                       s_constant_current_in_MVA,
                       s_constant_impedance_in_MVA;
        unsigned int area_number, zone_number, owner_number;
        bool interruptable;

        static double voltage_threshold_of_constant_power_load_in_pu;
        static double voltage_threshold_of_constant_current_load_in_pu;
        static double one_over_voltage_threshold_of_constant_power_load_in_pu;
        static double one_over_voltage_threshold_of_constant_current_load_in_pu;

        double manually_scale_in_pu;

        LOAD_MODEL* load_model;
        LOAD_VOLTAGE_RELAY_MODEL* load_voltage_relay_model;
        LOAD_FREQUENCY_RELAY_MODEL* load_frequency_relay_model;
};
#endif // LOAD_H
