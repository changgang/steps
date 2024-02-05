#ifndef MULTI_POINT_LINE_H
#define MULTI_POINT_LINE_H

#include <string>
#include <vector>
using namespace std;

class MULTI_POINT_LINE
{
    public:
        MULTI_POINT_LINE();
        MULTI_POINT_LINE(const MULTI_POINT_LINE& line);
        virtual MULTI_POINT_LINE& operator=(const MULTI_POINT_LINE& line);
        virtual ~MULTI_POINT_LINE();

        void set_point_number(unsigned int n);
        void set_point(unsigned int index, double x, double y);

        unsigned int get_point_number() const;
        double get_point_x(unsigned int index) const;
        double get_point_y(unsigned int index) const;

        double get_max_x() const;
        double get_min_x() const;
        double get_max_y() const;
        double get_min_y() const;
        double interpolate_y_at_x(double x) const;

        void sort_in_ascending_order();

        void clear();
    private:
        void copy_from_const(const MULTI_POINT_LINE& line);
        unsigned int get_greatest_point_index_with_x_less_than(double x) const;
        unsigned int get_least_point_index_with_x_greater_than(double x) const;
        vector<double> x;
        vector<double> y;
};


#endif // MULTI_POINT_LINE_H
