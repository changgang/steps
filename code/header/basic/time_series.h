#ifndef TIME_SERIES_H
#define TIME_SERIES_H

#include <string>
#include <vector>
using namespace std;

class TIME_SERIES
{
    public:
        TIME_SERIES();
        TIME_SERIES(const TIME_SERIES& ts);
        TIME_SERIES& operator=(const TIME_SERIES& ts);
        void copy_from_const_time_series(const TIME_SERIES& ts);
        virtual ~TIME_SERIES();

        void load_time_series_from_file(string file);
        void clear();
        vector<double> get_values_at_time(double t);
        double get_value_of_name_at_time(string vname, double t);
        vector<double> get_time_vector() const;
        vector< vector<double> > get_value_vector() const;
        vector<string> get_value_name_vector() const;
    private:
        vector<double> time;
        vector<string> value_name;
        vector< vector<double> > value;
        size_t current_time_index;
};
#endif // TIME_SERIES_H
