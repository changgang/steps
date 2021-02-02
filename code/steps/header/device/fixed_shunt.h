#ifndef FIXED_SHUNT_H
#define FIXED_SHUNT_H

#include "header/device/nonbus_device.h"
#include <string>
#include <complex>

using namespace std;

class BUS;
class FIXED_SHUNT : public NONBUS_DEVICE
{
    public:
        FIXED_SHUNT(STEPS& toolkit);
        virtual ~FIXED_SHUNT();

        void set_shunt_bus(unsigned int bus);
        void set_identifier(string identifier);
        void set_name(string name);
        void set_status(bool status);
        void set_nominal_positive_sequence_impedance_shunt_in_MVA(complex<double> s);
        void set_nominal_zero_sequence_impedance_shunt_in_MVA(complex<double> s);

        unsigned int get_shunt_bus() const;
        BUS* get_bus_pointer() const;
        string get_identifier() const;
        string get_name() const;
        bool get_status() const;
        complex<double> get_nominal_positive_sequence_impedance_shunt_in_MVA() const;
        complex<double> get_nominal_impedance_shunt_in_pu() const;
        complex<double> get_nominal_admittance_shunt_in_pu() const;
        complex<double> get_nominal_zero_sequence_impedance_shunt_in_MVA() const;
        complex<double> get_nominal_zero_sequence_impedance_shunt_in_pu() const;
        complex<double> get_nominal_zero_sequence_admittance_shunt_in_pu() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;
        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        virtual FIXED_SHUNT& operator=(const FIXED_SHUNT& load);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        complex<double> get_actual_impedance_shunt_in_MVA() const;
    private:
        unsigned int bus;
        BUS* busptr;
        string identifier;
        string name;
        bool status;
        complex<double> nominal_positive_sequence_impedance_shunt_in_MVA;
        complex<double> nominal_zero_squence_impedance_shunt_in_MVA;
};
#endif // FIXED_SHUNT_H
