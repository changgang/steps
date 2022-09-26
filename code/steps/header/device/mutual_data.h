#ifndef MUTUAL_DATA_H
#define MUTUAL_DATA_H

#include "header/device/device.h"

#include <complex>
#include <string>

using namespace std;

class LINE;
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

        void set_first_line_pointer(LINE* lineptr);
        void set_second_line_pointer(LINE* lineptr);
        void set_mutual_impedance(complex<double> z);
        void set_starting_location_of_first_line(double location);
        void set_ending_location_of_first_line(double location);
        void set_starting_location_of_second_line(double location);
        void set_ending_location_of_second_line(double location);

        LINE* get_first_line_pointer() const;
        LINE* get_second_line_pointer() const;
        LINE* get_mutual_line(LINE* lineptr) const;
        complex<double> get_mutual_impedance() const;
        double get_starting_location_of_line(LINE* lineptr) const;
        double get_ending_location_of_line(LINE* lineptr) const;

        bool is_related_to_line(unsigned int ibus, unsigned int jbus,const string& identifier);

    private:
        LINE* first_lineptr;
        LINE* second_lineptr;
        complex<double> Zm;
        double starting_location_of_first_line, ending_location_of_first_line;
        double starting_location_of_second_line, ending_location_of_second_line;
};

#endif // MUTUAL_DATA_H
