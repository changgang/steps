#ifndef VRTRLY1_H
#define VRTRLY1_H

#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"
#include "header/model/converter_common_models/vrt_model/lvrt_relay.h"
#include "header/model/converter_common_models/vrt_model/hvrt_relay.h"
#include "header/basic/timer.h"
#include "header/basic/constants.h"
#include "header/basic/multi_point_line.h"

class VRTRLY1 : public WT_RELAY_MODEL
{
    public:
        VRTRLY1(STEPS& toolkit);
        VRTRLY1(const VRTRLY1& model);
        virtual ~VRTRLY1();
        virtual VRTRLY1& operator=(const VRTRLY1& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_vrt_trip_time_delay_in_s(double t);
        void set_lvrt_point_number(unsigned int n);
        void set_lvrt_time_volt_threshold_pair(unsigned int index, double t, double v);
        void set_lvrt_trip_scale(double scale);
        void set_hvrt_point_number(unsigned int n);
        void set_hvrt_time_volt_threshold_pair(unsigned int index, double t, double v);
        void set_hvrt_trip_scale(double scale);

        double get_vrt_trip_time_delay_in_s() const;
        unsigned int get_lvrt_point_number() const;
        double get_lvrt_time_threshold(unsigned int index) const;
        double get_lvrt_volt_threshold(unsigned int index) const;
        double get_lvrt_trip_scale() const;
        unsigned int get_hvrt_point_number() const;
        double get_hvrt_time_threshold(unsigned int index) const;
        double get_hvrt_volt_threshold(unsigned int index) const;
        double get_hvrt_trip_scale() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const VRTRLY1& model);

        void check_lvrt_relay();
        void check_hvrt_relay();

        LVRT_RELAY lvrt;
        HVRT_RELAY hvrt;
};

#endif // VRTRLY1_H
