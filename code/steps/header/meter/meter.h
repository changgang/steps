#ifndef METER_H
#define METER_H

#include "header/basic/constants.h"
#include "header/basic/device_id.h"
#include "header/basic/dc_device_id.h"
#include "header/device/device.h"

class METER
{
    public:
        METER(STEPS& toolkit);
        METER(const METER& meter);
        METER& operator=(const METER& meter);
        STEPS& get_toolkit() const;
        ~METER();

        void set_device_id(const DEVICE_ID& device_id);
        void set_sub_dc_device_id(DC_DEVICE_ID did);
        void set_meter_type(string meter_type);
        void set_internal_variable_name(string name);
        void set_meter_side_bus(unsigned int meter_side);

        void change_device_id(DEVICE_ID did);
        void change_meter_type(const string& meter_type);
        void change_meter_internal_variable_name(const string& name);


        DEVICE_ID get_device_id() const;
        STEPS_DEVICE_TYPE get_device_type() const;
        DC_DEVICE_ID get_sub_dc_device_id() const;
        string get_meter_type() const;
        unsigned int get_meter_side_bus() const;
        string get_internal_variable_name() const;

        string get_meter_name() const;

        void clear();
        bool is_valid() const;
        bool operator==(const METER& meter);
        bool operator!=(const METER& meter);

        DEVICE* get_device_pointer() const;

        double get_meter_value() const;
    private:
        void set_device_pointer(DEVICE_ID device_id);
        void copy_from_const_meter(const METER& meter);

        bool is_valid_meter_type(string& meter_type) const;
        bool is_valid_meter_type_of_device(const string& meter_type, STEPS_DEVICE_TYPE device_type) const;

        bool is_internal_variable_name_valid(string& name) const;

        double get_meter_value_as_a_bus() const;
        double get_meter_value_as_a_line() const;
        double get_meter_value_as_a_transformer() const;
        double get_meter_value_as_a_load() const;
        double get_meter_value_as_a_generator() const;
        double get_meter_value_as_a_wt_generator() const;
        double get_meter_value_as_a_pv_unit() const;
        double get_meter_value_as_an_hvdc() const;
        double get_meter_value_as_a_vsc_hvdc() const;
        double get_meter_value_as_an_equivalent_device() const;
        double get_meter_value_as_an_energy_storage() const;

        STEPS* toolkit;
        DEVICE* device_pointer;
        DC_DEVICE_ID dc_did;
        unsigned int meter_type_string_index;
        unsigned int meter_side_bus;
        unsigned int internal_variable_name_index;
};


extern vector<string> bus_meters;
extern vector<string> line_meters;
extern vector<string> transformer_meters;
extern vector<string> generator_meters;
extern vector<string> load_meters;
extern vector<string> wt_generator_meters;
extern vector<string> pv_unit_meters;
extern vector<string> energy_storage_meters;
extern vector<string> hvdc_meters;
extern vector<string> vsc_hvdc_meters;
extern vector<string> equivalent_device_meters;
extern map<STEPS_DEVICE_TYPE, vector<string>> SUPPORTED_METERS;

#endif // METER_H
