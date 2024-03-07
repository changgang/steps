#ifndef MULTI_POINT_LINE_H
#define MULTI_POINT_LINE_H

#include "header/basic/constants.h"
#include <vector>

using namespace std;
class MULTI_POINT_LINE
{
    public:
        MULTI_POINT_LINE();
        virtual ~MULTI_POINT_LINE();
        MULTI_POINT_LINE(const MULTI_POINT_LINE& m);
        virtual MULTI_POINT_LINE& operator=(const MULTI_POINT_LINE& m);
    public:
        void clear();
        void add_x_and_y_pair(double x, double y);
        double get_y_at_x(double x);
        const vector<vector<double>> get_line() const;
        double get_point_x_with_index(unsigned int i) const;
        double get_point_y_with_index(unsigned int i) const;
        void set_point_y_with_index(unsigned int i, double value);
        void set_point_x_with_index(unsigned int i, double value);
        unsigned int get_valid_point_size() const;
        double get_max_y() const;
        double get_min_y() const;
    private:
        void copy_from_const_line(const MULTI_POINT_LINE& m);
        void reorder_points();
        bool is_point_exist(double x, double y) const;
        unsigned int get_the_last_point_with_x_less_than_or_equal_to(double x);
        double line[STEPS_MAX_MULTI_POINT_LINE_SIZE][2];
};

#endif // MULTI_POINT_LINE_H
