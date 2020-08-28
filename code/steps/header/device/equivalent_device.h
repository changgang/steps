#ifndef EQUIVALENT_DEVICE_H
#define EQUIVALENT_DEVICE_H

#include "header/device/nonbus_device.h"
#include "header/model/equivalent_model/equivalent_model.h"
#include <string>
#include <complex>

using namespace std;

class EQUIVALENT_DEVICE : public NONBUS_DEVICE
{
    public:
        EQUIVALENT_DEVICE(STEPS& toolkit);
        virtual ~EQUIVALENT_DEVICE();

        void set_equivalent_device_bus(unsigned int bus);
        void set_identifier(string identifier);
        void set_status(bool status);

        void set_equivalent_voltage_source_status(bool status);
        void set_equivalent_load_status(bool status);

        void set_equivalent_voltage_source_voltage_in_pu(complex<double> v);
        void set_equivalent_voltage_source_impedance_in_pu(complex<double> z);

        void set_equivalent_nominal_constant_impedance_load_in_MVA(complex<double> s);
        void set_equivalent_nominal_constant_current_load_in_MVA(complex<double> s);
        void set_equivalent_nominal_constant_power_load_in_MVA(complex<double> s);


        unsigned int get_equivalent_device_bus() const;
        string get_identifier() const;
        bool get_status() const;

        bool get_equivalent_voltage_source_status() const;
        bool get_equivalent_load_status() const;

        void update_meter_buffer();
        void update_equivalent_outputs();

        complex<double> get_equivalent_voltage_source_voltage_in_pu() const;
        complex<double> get_equivalent_voltage_source_impedance_in_pu() const;

        complex<double> get_equivalent_nominal_constant_impedance_load_in_MVA() const;
        complex<double> get_equivalent_nominal_constant_current_load_in_MVA() const;
        complex<double> get_equivalent_nominal_constant_power_load_in_MVA() const;

        complex<double> get_total_equivalent_power_as_load_in_MVA() const;
        complex<double> get_equivalent_generation_in_MVA() const;
        complex<double> get_equivalent_load_in_MVA() const;

        complex<double> get_equivalent_nominal_constant_impedance_load_in_pu() const;
        complex<double> get_equivalent_nominal_constant_current_load_in_pu() const;
        complex<double> get_equivalent_nominal_constant_power_load_in_pu() const;

        complex<double> get_total_equivalent_power_as_load_in_pu() const;
        complex<double> get_equivalent_generation_in_pu() const;
        complex<double> get_equivalent_load_in_pu() const;

        void switch_on();

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;


        virtual EQUIVALENT_DEVICE& operator=(const EQUIVALENT_DEVICE& load);

        virtual DEVICE_ID get_device_id() const;
        ////virtual string get_compound_device_name() const;

        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_equivalent_model(EQUIVALENT_MODEL* model);

        EQUIVALENT_MODEL* get_equivalent_model() const;

        virtual void run(DYNAMIC_MODE mode);
    private:
    private:
        unsigned int bus;
        string identifier;
        bool status;

        bool equivalent_voltage_source_status;
        bool equivalent_load_status;

        complex<double> equivalent_voltage_source_voltage_in_pu,
                        equivalent_voltage_source_impedance_in_pu;

        complex<double> equivalent_load_s_constant_power_in_MVA,
                        equivalent_load_s_constant_current_in_MVA,
                        equivalent_load_s_constant_impedance_in_MVA;

        EQUIVALENT_MODEL* equivalent_model;
};
#endif // EQUIVALENT_DEVICE_H
