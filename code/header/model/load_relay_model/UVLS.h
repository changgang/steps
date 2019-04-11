#ifndef UVLS_H
#define UVLS_H

#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/basic/constants.h"
#include "header/basic/timer.h"
#include "header/block/first_order_block.h"

class UVLS : public LOAD_VOLTAGE_RELAY_MODEL
{
    public:
        UVLS();
        UVLS(const UVLS& model);
        virtual ~UVLS();
        virtual UVLS& operator=(const UVLS& model);

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_total_shed_scale_factor_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:
        void set_voltage_sensor_time_in_s(double t);
        void set_voltage_threshold_in_pu_of_stage(size_t i, double f);
        void set_time_delay_in_s_of_stage(size_t i, double t);
        void set_scale_in_pu_of_stage(size_t i, double s);
        void set_breaker_time_in_s(double t);

        double get_voltage_sensor_time_in_s() const;
        double get_voltage_threshold_in_pu_of_stage(size_t i) const;
        double get_time_delay_in_s_of_stage(size_t i) const;
        double get_scale_in_pu_of_stage(size_t i) const;
        double get_breaker_time_in_s() const;
        double get_delayer_timer_in_s_of_stage(size_t i) const;
        double get_breaker_timer_in_s_of_stage(size_t i) const;
    private:

        bool is_stage_delayer_timer_started(size_t i) const;
        bool is_stage_breaker_timer_started(size_t i) const;
        void start_stage_delayer_timer(size_t i);
        void start_stage_breaker_timer(size_t i);
        void reset_stage_delayer_timer(size_t i);
        void reset_stage_breaker_timer(size_t i);
        bool is_stage_delayer_timer_timed_out(size_t i) const;
        bool is_stage_breaker_timer_timed_out(size_t i) const;

        void trip_stage(size_t i);
        bool is_stage_tripped(size_t i) const;

        bool is_stage_breaker_enabled(size_t i) const;
        bool is_stage_delayer_enabled(size_t i) const;
        void update_delayer_timer_of_stage(size_t i);
        void update_breaker_timer_of_stage(size_t i);


    private:
        void copy_from_const_model(const UVLS& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK voltage_sensor;

        double voltage_threshold_in_pu[MAX_LOAD_RELAY_STAGE];
        double scale_in_pu[MAX_LOAD_RELAY_STAGE];

        TIMER stage_timer[MAX_LOAD_RELAY_STAGE];
        TIMER breaker_timer[MAX_LOAD_RELAY_STAGE];
        bool flag_stage_is_tripped[MAX_LOAD_RELAY_STAGE];
};

#endif // UVLS_H
