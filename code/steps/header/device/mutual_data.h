#ifndef MUTUAL_DATA_H
#define MUTUAL_DATA_H

#include "header/device/device.h"

#include <complex>
#include <string>

using namespace std;

class AC_LINE;
class MUTUAL_DATA : public DEVICE
{
    public:
        MUTUAL_DATA(STEPS& toolkit);
        virtual ~MUTUAL_DATA();

        virtual void clear();

        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual bool is_valid() const;
        virtual void check();
        virtual void report() const;
        virtual DEVICE_ID get_device_id() const;

        void set_first_ac_line_pointer(AC_LINE* lineptr);
        void set_second_ac_line_pointer(AC_LINE* lineptr);
        void set_mutual_impedance(complex<double> z);
        void set_starting_location_of_first_ac_line(double location);
        void set_ending_location_of_first_ac_line(double location);
        void set_starting_location_of_second_ac_line(double location);
        void set_ending_location_of_second_ac_line(double location);

        AC_LINE* get_first_ac_line_pointer() const;
        AC_LINE* get_second_ac_line_pointer() const;
        AC_LINE* get_mutual_ac_line(AC_LINE* lineptr) const;
        complex<double> get_mutual_impedance() const;
        double get_starting_location_of_ac_line(AC_LINE* lineptr) const;
        double get_ending_location_of_ac_line(AC_LINE* lineptr) const;

        bool is_related_to_ac_line(unsigned int ibus, unsigned int jbus,const string& identifier);

    private:
        AC_LINE* first_ac_lineptr;
        AC_LINE* second_ac_lineptr;
        complex<double> Zm;
        double starting_location_of_first_ac_line, ending_location_of_first_ac_line;
        double starting_location_of_second_ac_line, ending_location_of_second_ac_line;
};

#endif // MUTUAL_DATA_H
