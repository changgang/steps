#ifndef FIXED_SHUNT_H
#define FIXED_SHUNT_H

#include "header/device/device.h"
#include <string>
#include <complex>

using namespace std;

class BUS;
class FIXED_SHUNT : public DEVICE
{
    public:
        FIXED_SHUNT();
        virtual ~FIXED_SHUNT();

        void set_shunt_bus(size_t bus);
        void set_identifier(string identifier);
        void set_status(bool status);
        void set_nominal_impedance_shunt_in_MVA(complex<double> s);

        size_t get_shunt_bus() const;
        BUS* get_bus_pointer() const;
        string get_identifier() const;
        bool get_status() const;
        complex<double> get_nominal_impedance_shunt_in_MVA() const;
        complex<double> get_nominal_impedance_shunt_in_pu() const;
        complex<double> get_nominal_admittance_shunt_in_pu() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(size_t bus) const;
        virtual bool is_in_area(size_t area) const;
        virtual bool is_in_zone(size_t zone) const;
        virtual void report() const;
        virtual void save() const;
        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        virtual FIXED_SHUNT& operator=(const FIXED_SHUNT& load);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        complex<double> get_actual_impedance_shunt_in_MVA() const;
    private:
        size_t bus;
        BUS* busptr;
        string identifier;
        bool status;
        complex<double> nominal_impedance_shunt_in_MVA;
};
#endif // FIXED_SHUNT_H
