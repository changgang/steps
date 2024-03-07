#include "header/basic/multi_point_line.h"
#include "header/basic/utility.h"
MULTI_POINT_LINE::MULTI_POINT_LINE()
{
    clear();
}

MULTI_POINT_LINE::~MULTI_POINT_LINE()
{
    ;
}

MULTI_POINT_LINE::MULTI_POINT_LINE(const MULTI_POINT_LINE& m)
{
    copy_from_const_line(m);
}

MULTI_POINT_LINE& MULTI_POINT_LINE::operator=(const MULTI_POINT_LINE& m)
{
    if(this==(&m)) return *this;

    copy_from_const_line(m);

    return *this;
}

void MULTI_POINT_LINE::copy_from_const_line(const MULTI_POINT_LINE& m)
{
    clear();
    vector<vector<double> > old_line = m.get_line();
    for(unsigned int i=0; i<STEPS_MAX_MULTI_POINT_LINE_SIZE; ++i)
    {
        line[i][0] = old_line[i][0];
        line[i][1] = old_line[i][1];
    }
}

void MULTI_POINT_LINE::clear()
{
    for(unsigned int i=0; i<STEPS_MAX_MULTI_POINT_LINE_SIZE; ++i)
    {
        line[i][0] = INFINITE_THRESHOLD;
        line[i][1] = INFINITE_THRESHOLD;
    }
}

void MULTI_POINT_LINE::add_x_and_y_pair(double x, double y)
{
    unsigned int n = get_valid_point_size();
    if(n>=STEPS_MAX_MULTI_POINT_LINE_SIZE)
    {
        ostringstream osstream;
        osstream<<"Warning. Multi point line is full (size "<<STEPS_MAX_MULTI_POINT_LINE_SIZE<<").\n"
                <<"No more point will be added. \n"
                <<"If more points are needed, increase STEPS_MAX_MULTI_POINT_LINE_SIZE in basic/constants.h.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }

    if(is_point_exist(x,y))
    {
        ostringstream osstream;
        osstream<<"Warning. Duplicate point is identified with the same point ("<<x<<" , "<<y<<").\n"
                <<"Input x-y point ("<<x<<" , "<<y<<") will not be added.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }

    line[n][0] = x;
    line[n][1] = y;

    reorder_points();
}

unsigned int MULTI_POINT_LINE::get_valid_point_size() const
{
    unsigned int valid_point_count = STEPS_MAX_MULTI_POINT_LINE_SIZE;
    for(unsigned int i=0; i<STEPS_MAX_MULTI_POINT_LINE_SIZE; ++i)
    {
        if(fabs(line[i][0]-INFINITE_THRESHOLD)<DOUBLE_EPSILON and fabs(line[i][1]-INFINITE_THRESHOLD)<DOUBLE_EPSILON)
        {
            valid_point_count = i;
            break;
        }
    }
    return valid_point_count;
}

double MULTI_POINT_LINE::get_max_y() const
{
    unsigned int n = get_valid_point_size();
    double max_y = get_point_y_with_index(0);
    for(unsigned int i=0; i<n; ++i)
    {
        double y = get_point_y_with_index(i);
        if(y>max_y)
            max_y = y;
    }
    return max_y;
}

double MULTI_POINT_LINE::get_min_y() const
{
    unsigned int n = get_valid_point_size();
    double min_y = get_point_y_with_index(0);
    for(unsigned int i=0; i<n; ++i)
    {
        double y = get_point_y_with_index(i);
        if(y<min_y)
            min_y = y;
    }
    return min_y;
}

bool MULTI_POINT_LINE::is_point_exist(double x, double y) const
{
    unsigned int n = get_valid_point_size();
    bool point_exist = false;
    for(unsigned int i=0; i<n; ++i)
    {
        if(fabs(line[i][0]-x)<DOUBLE_EPSILON and fabs(line[i][1]-y)<DOUBLE_EPSILON)
        {
            point_exist = true;
            break;
        }
    }
    return point_exist;
}

void MULTI_POINT_LINE::reorder_points()
{
    unsigned int n = get_valid_point_size();
    if(n<=1)
        return;
    else
    {
        for(unsigned int k=0; k<n-1; ++k)
        {
            bool swapped = false;
            for(unsigned int i=0; i<n-1; ++i)
            {
                if(line[i][0]>line[i+1][0])
                {
                    double temp_x = line[i][0];
                    double temp_y = line[i][1];
                    line[i][0] = line[i+1][0];
                    line[i][1] = line[i+1][1];
                    line[i+1][0] = temp_x;
                    line[i+1][1] = temp_y;
                    swapped = true;
                }
            }
            if(swapped==false)
                break;
        }
    }
}

double MULTI_POINT_LINE::get_y_at_x(double x)
{
    unsigned int n = get_valid_point_size();
    if(n<1)
        return 0.0;
    if(n==1)
        return get_point_y_with_index(0);

    unsigned int i_less = get_the_last_point_with_x_less_than_or_equal_to(x);
    if(i_less==INDEX_NOT_EXIST)
        return get_point_y_with_index(0);
    if(i_less == n-1)
        return get_point_y_with_index(n-1);

    unsigned int i_greater = i_less + 1;

    double x_less = get_point_x_with_index(i_less);
    double y_less = get_point_y_with_index(i_less);
    double x_greater = get_point_x_with_index(i_greater);
    double y_greater = get_point_y_with_index(i_greater);

    double slope = (y_greater-y_less)/(x_greater-x_less);
    return y_less + slope*(x-x_less);
}


unsigned int MULTI_POINT_LINE::get_the_last_point_with_x_less_than_or_equal_to(double x)
{
    unsigned int n = get_valid_point_size();
    unsigned int index = INDEX_NOT_EXIST;
    for(unsigned int i=0; i<n; ++i)
    {
        if(get_point_x_with_index(i)<=x)
            index = i;
        else
            break;
    }
    return index;
}

const vector<vector<double> > MULTI_POINT_LINE::get_line() const
{
    vector< vector<double> > new_line;
    new_line.reserve(STEPS_MAX_MULTI_POINT_LINE_SIZE);
    for(unsigned int i=0; i<STEPS_MAX_MULTI_POINT_LINE_SIZE; ++i)
    {
        vector<double> point;
        point.push_back(line[i][0]);
        point.push_back(line[i][1]);
        new_line.push_back(point);
    }
    return new_line;
}

double MULTI_POINT_LINE::get_point_x_with_index(unsigned int i) const
{
    if(i<STEPS_MAX_MULTI_POINT_LINE_SIZE)
        return line[i][0];
    else
        return 0.0;
}
double MULTI_POINT_LINE::get_point_y_with_index(unsigned int i) const
{
    if(i<STEPS_MAX_MULTI_POINT_LINE_SIZE)
        return line[i][1];
    else
        return 0.0;
}

void MULTI_POINT_LINE::set_point_x_with_index(unsigned int i, double value)
{
    if(i<STEPS_MAX_MULTI_POINT_LINE_SIZE)
        line[i][0] = value;
}

void MULTI_POINT_LINE::set_point_y_with_index(unsigned int i, double value)
{
    if(i<STEPS_MAX_MULTI_POINT_LINE_SIZE)
        line[i][1] = value;
}
