#ifndef METER_H
#define METER_H

#include "header/basic/constants.h"
#include "header/basic/device_id.h"
#include "header/device/device.h"
#include "header/basic/base.h"
#include "header/meter/continuous_buffer.h"

class METER : public BASE
{
    public:
        METER(POWER_SYSTEM_DATABASE* db);
        METER();
        METER(const METER& meter);
        METER& operator=(const METER& meter);
        ~METER();

        void set_device_id(const DEVICE_ID& device_id);
        void set_meter_type(string meter_type);
        void set_internal_variable_name(string var_name);
        void set_internal_variable_index(size_t internal_variable_index);
        void set_meter_side_bus(size_t meter_side);

        void change_device_id(DEVICE_ID did);
        void change_meter_type(string meter_type);


        DEVICE_ID get_device_id() const;
        string get_device_type() const;
        string get_meter_type() const;
        size_t get_meter_side_bus() const;
        size_t get_internal_variable_index() const;
        string get_internal_variable_name() const;

        string get_meter_name() const;

        void clear();
        bool is_valid() const;
        bool operator==(const METER& meter);
        bool operator!=(const METER& meter);

        DEVICE* get_device_pointer() const;

        void set_buffer_size(size_t bsize);
        size_t get_buffer_size() const;
        void initialize_meter_buffer();
        void update_meter_buffer();
        double get_meter_value() const;
        double get_meter_value_from_buffer_with_delay(size_t index=0) const;
        double get_meter_value_from_buffer_at_time(double time) const;
        size_t get_delay_index_of_time(double time) const;

    private:
        void set_device_pointer();
        void copy_from_const_meter(const METER& meter);

        bool is_valid_meter_type(string meter_type) const;
        bool is_valid_meter_type_of_device(string meter_type, string device_type) const;

        size_t get_internal_variable_index_with_name(string name) const;
        size_t get_internal_variable_index_with_name_as_load(string name) const;
        size_t get_internal_variable_index_with_name_as_generator(string name) const;
        size_t get_internal_variable_index_with_name_as_wt_generator(string name) const;
        size_t get_internal_variable_index_with_name_as_hvdc(string name) const;

        double get_meter_value_as_a_bus() const;
        double get_meter_value_as_a_line() const;
        double get_meter_value_as_a_transformer() const;
        double get_meter_value_as_a_load() const;
        double get_meter_value_as_a_generator() const;
        double get_meter_value_as_a_wt_generator() const;
        double get_meter_value_as_an_hvdc() const;
        double get_meter_value_as_an_equivalent_device() const;
        double get_meter_value_as_an_energy_storage() const;

        virtual void check();

        DEVICE_ID device_id;
        DEVICE* device_pointer;
        string meter_type;
        size_t meter_side_bus;
        string internal_variable_name;
        size_t internal_variable_index;

        CONTINUOUS_BUFFER buffer;
};


extern vector<string> bus_meters;
extern vector<string> line_meters;
extern vector<string> transformer_meters;
extern vector<string> generator_meters;
extern vector<string> load_meters;
extern vector<string> wt_generator_meters;
extern vector<string> hvdc_meters;
extern vector<string> equivalent_device_meters;
extern vector<string> energy_storage_meters;
extern map<string, vector<string>> SUPPORTED_METERS;

#endif // METER_H
