#include "header/basic/continuous_buffer.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <algorithm>
/****
current time:T
step: t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T-6t  T-7t
if a new data is appended, it becomes
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T-6t  T+t
then
if a new data is appended, it becomes
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T+8   T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T+8   T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+9t
****/


CONTINUOUS_BUFFER::CONTINUOUS_BUFFER()
{
    index_of_buffer_head = 0;
    clear();
}

CONTINUOUS_BUFFER::CONTINUOUS_BUFFER(const CONTINUOUS_BUFFER& buffer)
{
    copy_from_constant_buffer(buffer);
}

CONTINUOUS_BUFFER& CONTINUOUS_BUFFER::operator=(const CONTINUOUS_BUFFER& buffer)
{
    if(this==&buffer)
        return *this;
    copy_from_constant_buffer(buffer);
    return *this;
}


void CONTINUOUS_BUFFER::copy_from_constant_buffer(const CONTINUOUS_BUFFER& buffer)
{
    index_of_buffer_head = 0;
    clear();

    set_buffer_size(buffer.get_buffer_size());

    for(unsigned int i=0; i<STEPS_MAX_CONTINUOUS_BUFFER_SIZE; ++i)
    {
        this->buffer[i][0] = buffer.get_buffer_time_at_delay_index(i);
        this->buffer[i][1] = buffer.get_buffer_value_at_delay_index(i);
    }
}

CONTINUOUS_BUFFER::~CONTINUOUS_BUFFER()
{
}

void CONTINUOUS_BUFFER::clear()
{
    set_buffer_size(0);
    index_of_buffer_head = 0;
    for(unsigned int i=0; i<STEPS_MAX_CONTINUOUS_BUFFER_SIZE; ++i)
    {
        buffer[i][0] = 0.0;
        buffer[i][1] = 0.0;
    }
}

bool CONTINUOUS_BUFFER::is_valid() const
{
    return true;
}

void CONTINUOUS_BUFFER::check()
{
    ;
}


void CONTINUOUS_BUFFER::set_buffer_size(unsigned int n)
{
    if(n>STEPS_MAX_CONTINUOUS_BUFFER_SIZE)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        ostringstream osstream;
        osstream<<"Warning. The continuous buffer size "<<n<<" exceeds limit "<<STEPS_MAX_CONTINUOUS_BUFFER_SIZE<<".\n"
                <<"Buffer size will be set as "<<STEPS_MAX_CONTINUOUS_BUFFER_SIZE<<".\n"
                <<"If you do need greater buffer size, try to change STEPS_MAX_CONTINUOUS_BUFFER_SIZE in header/basic/constants.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        n = STEPS_MAX_CONTINUOUS_BUFFER_SIZE;
    }
    buffer_size = n;
}

unsigned int CONTINUOUS_BUFFER::get_buffer_size() const
{
    return buffer_size;
}

void CONTINUOUS_BUFFER::initialize_buffer(double initial_time, double value)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double delt = toolkit.get_dynamic_simulation_time_step_in_s();

    index_of_buffer_head = 0;
    index_of_buffer_tail = buffer_size-1;
    for(unsigned int i=0; i<buffer_size; ++i)
    {
        buffer[i][0] = initial_time-i*delt;
        buffer[i][1] = value;
    }
}

void CONTINUOUS_BUFFER::append_data(double time, double value)
{
    if(buffer_size==1)
    {
        buffer[0][0] = time;
        buffer[0][1] = value;
        return;
    }
    else
    {
        double time_at_head = get_buffer_time_at_head();
        if(fabs(time-time_at_head)<DOUBLE_EPSILON)
            buffer[index_of_buffer_head][1] = value;
        else
        {
            if(time<time_at_head)
                return;
            else
            {
                buffer[index_of_buffer_tail][0] = time;
                buffer[index_of_buffer_tail][1] = value;

                index_of_buffer_head = index_of_buffer_tail;

                if(index_of_buffer_head>0)
                    index_of_buffer_tail = index_of_buffer_head-1;
                else
                    index_of_buffer_tail = buffer_size-1;
            }
        }
    }
}

unsigned int CONTINUOUS_BUFFER::get_index_of_buffer_head() const
{
    return index_of_buffer_head;
}

unsigned int CONTINUOUS_BUFFER::get_index_of_buffer_tail() const
{
    return index_of_buffer_tail;
}

unsigned int CONTINUOUS_BUFFER::get_storage_index_of_delay_index(unsigned int index) const
{
    index = index_of_buffer_head+index;
    while(index>=buffer_size)
        index -= buffer_size;
    return index;
}

double CONTINUOUS_BUFFER::get_buffer_time_at_head() const
{
    return buffer[index_of_buffer_head][0];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_head() const
{
    return buffer[index_of_buffer_head][1];
}

double CONTINUOUS_BUFFER::get_buffer_time_at_tail() const
{
    return buffer[index_of_buffer_tail][0];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_tail() const
{
    return buffer[index_of_buffer_tail][1];
}

double CONTINUOUS_BUFFER::get_buffer_time_at_delay_index(unsigned int index) const
{
    index = get_storage_index_of_delay_index(index);
    return buffer[index][0];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_delay_index(unsigned int index) const
{
    index = get_storage_index_of_delay_index(index);
    return buffer[index][1];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_time(double time) const
{
    unsigned int index = get_delay_index_of_time(time);
    if(index==INDEX_NOT_EXIST)
        return 0.0;
    else
        return get_buffer_value_at_delay_index(index);
}

unsigned int CONTINUOUS_BUFFER::get_delay_index_of_time(double time) const
{
    double time_at_head = get_buffer_time_at_head();
    double time_at_tail = buffer[index_of_buffer_tail][0];

    if(fabs(time_at_head-time)<DOUBLE_EPSILON)
        return 0;
    if(fabs(time_at_tail-time)<DOUBLE_EPSILON)
        return buffer_size-1;

    if(time<time_at_tail or time>time_at_head)
        return INDEX_NOT_EXIST;

    double time_error = fabs(time-get_buffer_time_at_head());
    unsigned int index = 0;
    for(unsigned int i=1; i!=buffer_size; ++i)
    {
        double delayed_time = get_buffer_time_at_delay_index(i);
        if(time_error>fabs(time-delayed_time))
        {
            index = i;
            time_error = fabs(time-delayed_time);
        }
        else
            break;
    }
    return index;
}

double CONTINUOUS_BUFFER::get_buffer_max_value() const
{
    if(buffer_size!=1)
    {
        double max_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size; ++i)
        {
            double value = get_buffer_value_at_delay_index(i);
            if(value>max_value)
                max_value = value;
        }
        return max_value;
    }
    else
        return get_buffer_value_at_head();
}

double CONTINUOUS_BUFFER::get_buffer_min_value() const
{
    if(buffer_size!=1)
    {
        double min_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size; ++i)
        {
            double value = get_buffer_value_at_delay_index(i);
            if(value<min_value)
                min_value = value;
        }
        return min_value;
    }
    else
        return get_buffer_value_at_head();
}

double CONTINUOUS_BUFFER::get_buffer_range() const
{
    if(buffer_size!=1)
    {
        double max_value = get_buffer_max_value();
        double min_value = get_buffer_min_value();
        return max_value - min_value;
    }
    else
        return 0;
}

double CONTINUOUS_BUFFER::get_buffer_max_change() const
{
    if(buffer_size!=1)
    {
        double max_change = 0;
        double current_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size-1; ++i)
        {
            double next_value = get_buffer_value_at_delay_index(i);
            double change = fabs(current_value-next_value);
            if(change>max_change)
                max_change = change;
        }
        return max_change;
    }
    else
        return 0;
}

double CONTINUOUS_BUFFER::get_buffer_max_value_in_latest_seconds(double t) const
{
    if(buffer_size!=1)
    {
        double thead = get_buffer_time_at_delay_index(0);
        double max_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size; ++i)
        {
            double time = get_buffer_time_at_delay_index(i);
            if(thead-time<t)
            {
                double value = get_buffer_value_at_delay_index(i);
                if(value>max_value)
                    max_value = value;
            }
            else
                break;
        }
        return max_value;
    }
    else
        return get_buffer_value_at_head();
}

double CONTINUOUS_BUFFER::get_buffer_min_value_in_latest_seconds(double t) const
{
    if(buffer_size!=1)
    {
        double thead = get_buffer_time_at_delay_index(0);
        double min_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size; ++i)
        {
            double time = get_buffer_time_at_delay_index(i);
            if(thead-time<t)
            {
                double value = get_buffer_value_at_delay_index(i);
                if(value<min_value)
                    min_value = value;
            }
            else
                break;
        }
        return min_value;
    }
    else
        return get_buffer_value_at_head();
}

double CONTINUOUS_BUFFER::get_buffer_range_in_latest_seconds(double t) const
{
    if(buffer_size!=1)
    {
        double max_value = get_buffer_max_value_in_latest_seconds(t);
        double min_value = get_buffer_min_value_in_latest_seconds(t);
        return max_value - min_value;
    }
    else
        return 0;
}

double CONTINUOUS_BUFFER::get_buffer_max_change_in_latest_seconds(double t) const
{
    if(buffer_size!=1)
    {
        double thead = get_buffer_time_at_delay_index(0);
        double max_change = 0;
        double current_value = get_buffer_value_at_delay_index(0);
        for(unsigned int i=1; i<buffer_size-1; ++i)
        {
            double time = get_buffer_time_at_delay_index(i);
            if(thead-time<t)
            {
                double next_value = get_buffer_value_at_delay_index(i);
                double change = fabs(current_value-next_value);
                if(change>max_change)
                    max_change = change;
            }
            else
                break;
        }
        return max_change;
    }
    else
        return 0;
}


double CONTINUOUS_BUFFER::get_buffer_latest_change() const
{
    if(buffer_size!=1)
    {
        double now_value = get_buffer_value_at_delay_index(0);
        double hist_value = get_buffer_value_at_delay_index(1);
        return fabs(now_value-hist_value);
    }
    else
        return 0.0;
}

void CONTINUOUS_BUFFER::update_buffer_value_at_delay_index(unsigned int index, double value)
{
    index = get_storage_index_of_delay_index(index);
    buffer[index][1] = value;
}

void CONTINUOUS_BUFFER::show_buffer() const
{
    ostringstream osstream;
    osstream<<"Buffer conttens:"<<endl;
    osstream<<"TIME,VALUE"<<endl;
    for(unsigned int i=0; i!=buffer_size; ++i)
    {
        osstream<<get_buffer_time_at_delay_index(i)<<","<<get_buffer_value_at_delay_index(i)<<endl;
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

bool operator<(CONTINUOUS_BUFFER&A, CONTINUOUS_BUFFER& B)
{
    unsigned int nA = A.get_buffer_size();
    unsigned int nB = B.get_buffer_size();
    if(nA==nB)
    {
        for(unsigned int index = nA; index!=0; --index)
        {
            double xA = A.get_buffer_value_at_delay_index(index-1);
            double xB = B.get_buffer_value_at_delay_index(index-1);
            if(xA<xB-FLOAT_EPSILON)
                return true;
        }
        return false;
    }
    else
        return false;
}

bool operator>(CONTINUOUS_BUFFER&A, CONTINUOUS_BUFFER& B)
{
    unsigned int nA = A.get_buffer_size();
    unsigned int nB = B.get_buffer_size();
    if(nA==nB)
    {
        for(unsigned int index = nA; index!=0; --index)
        {
            double xA = A.get_buffer_value_at_delay_index(index-1);
            double xB = B.get_buffer_value_at_delay_index(index-1);
            if(xA>xB+FLOAT_EPSILON)
                return true;
        }
        return false;
    }
    else
        return false;
}

bool operator==(CONTINUOUS_BUFFER&A, CONTINUOUS_BUFFER& B)
{
    unsigned int nA = A.get_buffer_size();
    unsigned int nB = B.get_buffer_size();
    if(nA==nB)
    {
        for(unsigned int index = nA; index!=0; --index)
        {
            double xA = A.get_buffer_value_at_delay_index(index-1);
            double xB = B.get_buffer_value_at_delay_index(index-1);
            if(abs(xA-xB)>FLOAT_EPSILON)
                return false;
        }
        return true;
    }
    else
        return false;
}
