#ifndef UFLS_H
#define UFLS_H

#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/basic/constants.h"
#include "header/basic/timer.h"
#include "header/block/first_order_block.h"

class UFLS : public LOAD_FREQUENCY_RELAY_MODEL
{
    public:
        UFLS();
        UFLS(const UFLS& model);
        virtual ~UFLS();
        virtual UFLS& operator=(const UFLS& model);

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_total_shed_scale_factor_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:
        void set_frequency_sensor_time_in_s(double t);
        void set_frequency_threshold_in_Hz_of_stage(unsigned int i, double f);
        void set_time_delay_in_s_of_stage(unsigned int i, double t);
        void set_scale_in_pu_of_stage(unsigned int i, double s);
        void set_breaker_time_in_s(double t);

        double get_frequency_sensor_time_in_s() const;
        double get_frequency_threshold_in_Hz_of_stage(unsigned int i) const;
        double get_time_delay_in_s_of_stage(unsigned int i) const;
        double get_scale_in_pu_of_stage(unsigned int i) const;
        double get_breaker_time_in_s() const;
        double get_delayer_timer_in_s_of_stage(unsigned int i) const;
        double get_breaker_timer_in_s_of_stage(unsigned int i) const;
    private:
        bool is_stage_delayer_timer_started(unsigned int i) const;
        bool is_stage_breaker_timer_started(unsigned int i) const;
        void start_stage_delayer_timer(unsigned int i);
        void start_stage_breaker_timer(unsigned int i);
        void reset_stage_delayer_timer(unsigned int i);
        void reset_stage_breaker_timer(unsigned int i);
        bool is_stage_delayer_timer_timed_out(unsigned int i) const;
        bool is_stage_breaker_timer_timed_out(unsigned int i) const;

        void trip_stage(unsigned int i);
        bool is_stage_tripped(unsigned int i) const;

        bool is_stage_breaker_enabled(unsigned int i) const;
        bool is_stage_delayer_enabled(unsigned int i) const;
        void update_delayer_timer_of_stage(unsigned int i);
        void update_breaker_timer_of_stage(unsigned int i);
    private:
        void copy_from_const_model(const UFLS& model);

        FIRST_ORDER_BLOCK frequency_sensor;

        double frequency_threshold_in_Hz[STEPS_MAX_LOAD_RELAY_STAGE];
        double scale_in_pu[STEPS_MAX_LOAD_RELAY_STAGE];

        TIMER stage_timer[STEPS_MAX_LOAD_RELAY_STAGE];
        TIMER breaker_timer[STEPS_MAX_LOAD_RELAY_STAGE];
        bool flag_stage_is_tripped[STEPS_MAX_LOAD_RELAY_STAGE];
};

#endif // UFLS_H
