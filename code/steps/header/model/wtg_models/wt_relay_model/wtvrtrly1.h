#ifndef WTVRTRLY1_H
#define WTVRTRLY1_H

#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"
#include "header/model/converter_common_models/vrt_model/lvrt_relay.h"
#include "header/model/converter_common_models/vrt_model/hvrt_relay.h"
#include "header/basic/timer.h"
#include "header/basic/constants.h"
#include "header/basic/multi_point_line.h"

class WTVRTRLY1 : public WT_RELAY_MODEL
{
    public:
        WTVRTRLY1(STEPS& toolkit);
        WTVRTRLY1(const WTVRTRLY1& model);
        virtual ~WTVRTRLY1();
        virtual WTVRTRLY1& operator=(const WTVRTRLY1& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_vrt_trip_time_delay_in_s(double t);
        void add_lvrt_time_volt_threshold_pair(double t, double v);
        void set_lvrt_trip_scale(double scale);
        void add_hvrt_time_volt_threshold_pair(double t, double v);
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
        void copy_from_const_model(const WTVRTRLY1& model);
        LVRT_RELAY lvrt;
        HVRT_RELAY hvrt;
};

#endif // WTVRTRLY1_H
