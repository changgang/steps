#ifndef LINE_H
#define LINE_H

#include "header/device/nonbus_device.h"
#include "header/basic/device_id.h"
#include "header/basic/rating.h"
#include "header/basic/fault.h"

#include <complex>
#include <string>

using namespace std;

class BUS;
class LINE : public NONBUS_DEVICE
{
    public:
        LINE(STEPS& toolkit);
        virtual ~LINE();

        void set_sending_side_bus(unsigned int bus);
        void set_receiving_side_bus(unsigned int bus);
        void set_identifier(string identifier);
        void set_name(string name);
        void set_sending_side_breaker_status(bool status);
        void set_receiving_side_breaker_status(bool status);

        void set_line_positive_sequence_z_in_pu(const complex<double>& z);
        void set_line_positive_sequence_y_in_pu(const complex<double>& y);
        void set_shunt_positive_sequence_y_at_sending_side_in_pu(const complex<double>& y);
        void set_shunt_positive_sequence_y_at_receiving_side_in_pu(const complex<double>& y);

        void set_line_zero_sequence_z_in_pu(const complex<double>& z);
        void set_line_zero_sequence_y_in_pu(const complex<double>& y);
        void set_shunt_zero_sequence_y_at_sending_side_in_pu(const complex<double>& y);
        void set_shunt_zero_sequence_y_at_receiving_side_in_pu(const complex<double>& y);

        void set_rating(RATING rating);
        void set_meter_end_bus(unsigned int meter_bus);
        void set_length(double length);

        unsigned int get_sending_side_bus() const;
        unsigned int get_receiving_side_bus() const;
        BUS* get_sending_side_bus_pointer() const;
        BUS* get_receiving_side_bus_pointer() const;

        string get_identifier() const;
        string get_name() const;
        bool get_sending_side_breaker_status() const;
        bool get_receiving_side_breaker_status() const;

        complex<double> get_line_positive_sequence_z_in_pu() const;
        complex<double> get_line_zero_sequence_z_in_pu() const;
        complex<double> get_line_positive_sequence_y_in_pu() const;
        complex<double> get_line_zero_sequence_y_in_pu() const;
        complex<double> get_shunt_positive_sequence_y_at_sending_side_in_pu() const;
        complex<double> get_shunt_zero_sequence_y_at_sending_side_in_pu() const;
        complex<double> get_shunt_positive_sequence_y_at_receiving_side_in_pu() const;
        complex<double> get_shunt_zero_sequence_y_at_receiving_side_in_pu() const;

        RATING get_rating() const;
        unsigned int get_meter_end_bus() const;
        double get_length() const;

        bool is_zero_impedance_line() const;

        void set_fault(unsigned int to_bus, double location, const FAULT& fault);
        double get_fault_location_of_fault(unsigned int index) const;
        FAULT get_fault_at_location(unsigned int to_bus, double location) const;
        void clear_all_faults();
        void clear_fault_at_location(unsigned int to_bus, double location);
        unsigned int get_fault_count() const;
        bool is_faulted() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        virtual LINE& operator=(const LINE& line);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        double get_line_base_voltage_in_kV() const;

        complex<double> get_line_complex_voltage_at_sending_side_in_pu() const;
        complex<double> get_line_complex_voltage_at_receiving_side_in_pu() const;
        complex<double> get_line_complex_voltage_at_sending_side_in_kV() const;
        complex<double> get_line_complex_voltage_at_receiving_side_in_kV() const;
        complex<double> get_line_complex_current_at_sending_side_in_pu() const;
        complex<double> get_line_complex_current_at_receiving_side_in_pu() const;
        complex<double> get_line_complex_current_at_sending_side_in_kA() const;
        complex<double> get_line_complex_current_at_receiving_side_in_kA() const;

        complex<double> get_line_complex_power_at_sending_side_in_pu() const;
        complex<double> get_line_complex_power_at_receiving_side_in_pu() const;
        complex<double> get_line_complex_power_at_sending_side_in_MVA() const;
        complex<double> get_line_complex_power_at_receiving_side_in_MVA() const;
        complex<double> get_line_complex_apparent_impedance_at_sending_side_in_pu() const;
        complex<double> get_line_complex_apparent_impedance_at_receiving_side_in_pu() const;
        complex<double> get_line_complex_apparent_impedance_at_sending_side_in_ohm() const;
        complex<double> get_line_complex_apparent_impedance_at_receiving_side_in_ohm() const;
    private:
        //map<double,FAULT>::iterator get_iterator_of_fault_at_location_to_sending_side(double location);
        unsigned int sending_side_bus, receiving_side_bus;
        BUS* sending_side_busptr, *receiving_side_busptr;
        string   identifier;
        string   name;
        bool sending_side_breaker_status, receiving_side_breaker_status;
        complex<double> line_z_12, line_z_0,
                       line_y_12, line_y_0,
                       shunt_y_12_sending_side, shunt_y_0_sending_side,
                       shunt_y_12_receiving_side, shunt_y_0_receiving_side;
        RATING rating;
        unsigned int meter_end_bus;
        double length;

        map<double,FAULT> faults;
};

#endif // LINE_H
