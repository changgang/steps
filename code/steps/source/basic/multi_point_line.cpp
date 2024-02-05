#include "header/basic/multi_point_line.h"
#include "header/basic/utility.h"

#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

MULTI_POINT_LINE::MULTI_POINT_LINE()
{
    clear();
}

MULTI_POINT_LINE::MULTI_POINT_LINE(const MULTI_POINT_LINE& line)
{
    clear();
    copy_from_const(line);
}

MULTI_POINT_LINE& MULTI_POINT_LINE::operator=(const MULTI_POINT_LINE& line)
{
    if(this==(&line)) return *this;
    copy_from_const(line);
    sort_in_ascending_order();
    return *this;
}

void MULTI_POINT_LINE::clear()
{
    x.clear();
    y.clear();
}

void MULTI_POINT_LINE::copy_from_const(const MULTI_POINT_LINE& line)
{
    set_point_number(line.get_point_number());
    unsigned int n = get_point_number();
    for(unsigned int i=0; i<n; ++i)
    {
        double x = line.get_point_x(i);
        double y = line.get_point_y(i);
        set_point(i, x, y);
    }
}
MULTI_POINT_LINE::~MULTI_POINT_LINE()
{
}

void MULTI_POINT_LINE::set_point_number(unsigned int n)
{
    x.clear();
    y.clear();
    x.reserve(n);
    y.reserve(n);
    for(unsigned int i=0; i<n; ++i)
    {
        x.push_back(0);
        y.push_back(0);
    }
}

void MULTI_POINT_LINE::set_point(unsigned int index, double x, double y)
{
    unsigned int n = this->x.size();
    if(index<n)
    {
        this->x[index] = x;
        this->y[index] = y;
    }
    else
    {
        cout<<"Warning. Index ("<<index<<") exceeding line point count ("<<n
            <<"). No data will be added to MULTI_POINT_LINE";
    }
}

unsigned int MULTI_POINT_LINE::get_point_number() const
{
    return x.size();
}

double MULTI_POINT_LINE::get_point_x(unsigned int index) const
{
    unsigned int n = x.size();
    if(index<n)
        return x[index];
    else
        return 0;
}

double MULTI_POINT_LINE::get_point_y(unsigned int index) const
{
    unsigned int n = x.size();
    if(index<n)
        return y[index];
    else
        return 0;
}

double MULTI_POINT_LINE::interpolate_y_at_x(double x) const
{
    double imin = get_greatest_point_index_with_x_less_than(x);
    double imax = get_least_point_index_with_x_greater_than(x);
    if(imin==imax)
        return get_point_y(imin);
    else
    {
        double xmin = get_point_x(imin);
        double ymin = get_point_y(imin);
        double xmax = get_point_x(imax);
        double ymax = get_point_y(imax);
        return ymin+(ymax-ymin)/(xmax-xmin)*(x-xmin);
    }
}

unsigned int MULTI_POINT_LINE::get_greatest_point_index_with_x_less_than(double x) const
{
    /*     x0/xmin  x1  x2  x3  x4  x5    xn-2  xn-1/xmax*/
    unsigned int n = get_point_number();
    double xmin = get_min_x();
    double xmax = get_max_x();
    if(x<xmin) return 0;
    if(x>xmax) return n-1;
    else // xmin <= x <= xmax
    {
        unsigned int n = get_point_number();
        unsigned int index = 0;
        for(unsigned int i=0; i<n; ++i)
        {
            double xi = get_point_x(i);
            if(fabs(xi-x)<FLOAT_EPSILON*0.1)
            {
                index = i;
                break;
            }
            else
            {
                if(xi>x)
                {
                    index = i-1;
                    break;
                }
            }
        }
        return index;
    }
}

unsigned int MULTI_POINT_LINE::get_least_point_index_with_x_greater_than(double x) const
{
    /*     x0/xmin  x1  x2  x3  x4  x5    xn-2  xn-1/xmax*/
    unsigned int n = get_point_number();
    double xmin = get_min_x();
    double xmax = get_max_x();
    if(x<xmin) return 0;
    if(x>xmax) return n-1;
    else // xmin <= x <= xmax
    {
        int n = get_point_number();
        unsigned int index = 0;
        for(int i=n-1; i>=0; --i)
        {
            double xi = get_point_x(i);
            if(fabs(xi-x)<FLOAT_EPSILON*0.1)
            {
                index = i;
                break;
            }
            else
            {
                if(xi<x)
                {
                    index = i+1;
                    break;
                }
            }
        }
        return index;
    }
}

double MULTI_POINT_LINE::get_max_x() const
{
    return get_point_x(get_point_number()-1);
}

double MULTI_POINT_LINE::get_min_x() const
{
    return get_point_x(0);
}

double MULTI_POINT_LINE::get_max_y() const
{
    unsigned int n = get_point_number();
    double ymax = get_point_y(0);
    for(unsigned int i=1; i!=n; ++i)
    {
        double y = get_point_y(i);
        if(y>ymax)
            ymax = y;
    }
    return ymax;
}
double MULTI_POINT_LINE::get_min_y() const
{
    unsigned int n = get_point_number();
    double ymin = get_point_y(0);
    for(unsigned int i=1; i!=n; ++i)
    {
        double y = get_point_y(i);
        if(y<ymin)
            ymin = y;
    }
    return ymin;
}

void MULTI_POINT_LINE::sort_in_ascending_order()
{
    unsigned int n = get_point_number();
    for(unsigned int i=0; i<n-1; ++i)
    {
        double xmin = get_point_x(i);
        unsigned int element_to_swap = i;
        for(unsigned int j=i+1; j<n; ++j)
        {
            double xj = get_point_x(j);
            if(xj<xmin)
            {
                element_to_swap = j;
                xmin = xj;
            }
        }
        if(element_to_swap != i)
        {
            unsigned int j = element_to_swap;
            double xi = get_point_x(i);
            double yi = get_point_y(i);
            double xj = get_point_x(j);
            double yj = get_point_y(j);
            set_point(i, xj, yj);
            set_point(j, xi, yi);
        }
        else
            break;
    }
}

