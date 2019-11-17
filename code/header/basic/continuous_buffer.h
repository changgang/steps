#ifndef CONTINUOUS_BUFFER_H
#define CONTINUOUS_BUFFER_H

#include "header/device/device.h"

#include <cstdlib>

class CONTINUOUS_BUFFER : public BASE
{
    public:
        CONTINUOUS_BUFFER();
        CONTINUOUS_BUFFER(const CONTINUOUS_BUFFER& buffer);
        CONTINUOUS_BUFFER& operator=(const CONTINUOUS_BUFFER& buffer);
        ~CONTINUOUS_BUFFER();

        virtual void clear();

        void set_buffer_size(size_t buffer_size);
        size_t get_buffer_size() const;

        void initialize_buffer(double initial_time, double value);
        void append_data(double time, double value);

        size_t get_index_of_buffer_head() const;

        double get_buffer_time_at_head() const;
        double get_buffer_value_at_head() const;
        double get_buffer_time_at_delay_index(size_t index) const;
        double get_buffer_value_at_delay_index(size_t index) const;
        double get_buffer_value_at_time(double time) const;
        size_t get_delay_index_of_time(double time) const;

        void show_buffer() const;
    private:
        void copy_from_constant_buffer(const CONTINUOUS_BUFFER& buffer);
        size_t get_storage_index_of_delay_index(size_t index) const;

        size_t buffer_size;

        size_t index_of_buffer_head;

        vector<double> time_buffer;
        vector<double> value_buffer;

        virtual bool is_valid() const;
        virtual void check();
};

#endif // CONTINUOUS_BUFFER_H
