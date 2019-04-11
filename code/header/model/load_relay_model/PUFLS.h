#ifndef PUFLS_H
#define PUFLS_H

#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/block/first_order_block.h"
#include "header/meter/continuous_buffer.h"
#include "header/basic/timer.h"
#include "header/basic/constants.h"
#include "header/basic/steps_enum.h"

class PUFLS : public LOAD_FREQUENCY_RELAY_MODEL
{
    public:
        PUFLS();
        PUFLS(const PUFLS& model);
        virtual ~PUFLS();
        virtual PUFLS& operator=(const PUFLS& model);

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
        void set_frequency_sensor_time_in_s(double t);
        void set_continuous_frequency_threshold_in_Hz(double f);
        void set_scale_K_in_pu_per_Hz(double K);
        void set_time_delay_in_s(double t);
        void set_maximum_continuous_shed_scale_in_pu(double scale);
        void set_additional_stage_trigger_signal(UFLS_TRIGGER_SIGNAL signal);
        void set_additional_stage_frequency_threshold_in_Hz(double f);
        void set_additional_stage_time_delay_in_s(double t);
        void set_additional_stage_shed_scale_in_pu(double scale);
        void set_discrete_stage_time_delay_in_s(double t);
        void set_discrete_stage_shed_scale_in_pu(size_t stage, double scale);

        double get_frequency_sensor_time_in_s() const;
        double get_continuous_frequency_threshold_in_Hz() const;
        double get_scale_K_in_pu_per_Hz() const;
        double get_time_delay_in_s() const;
        double get_maximum_continuous_shed_scale_in_pu() const;
        UFLS_TRIGGER_SIGNAL get_additional_stage_trigger_signal() const;
        double get_additional_stage_frequency_threshold_in_Hz() const;
        double get_additional_stage_time_delay_in_s() const;
        double get_additional_stage_shed_scale_in_pu() const;
        double get_discrete_stage_time_delay_in_s() const;
        double get_discrete_stage_shed_scale_in_pu(size_t stage) const;
    private:
        void copy_from_const_model(const PUFLS& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        void append_new_minimum_frequency();

        void update_continuous_shed_command();
        double get_continuous_shed_command_in_pu() const;

        size_t get_number_of_discrete_stage_to_meet_total_continuous_shed_scale() const;
        double get_sum_of_shed_scale_of_first_n_discrete_stages_in_pu(size_t n) const;
        double get_total_shed_scale_of_all_discrete_stages_in_pu() const;

        bool is_discrete_stage_tipped(size_t stage) const;
        bool is_discrete_stage_timer_timed_out(size_t stage);
        bool is_discrete_stage_timer_started(size_t stage) const;
        void trip_discrete_stage(size_t stage);
        void start_discrete_stage_timer_of_stage(size_t stage);
        void reset_discrete_stage_timer_of_stage(size_t stage);

        void try_to_shed_additional_stage();
        bool is_additional_stage_set() const;
        bool is_additional_stage_tripped() const;
        bool is_additional_stage_timer_timed_out() const;
        bool is_additional_stage_timer_started() const;
        void trip_additional_stage();
        void try_to_start_additional_stage_timer();
        void start_additional_stage_timer();
        void try_to_reset_additional_stage_timer();
        void reset_additional_stage_timer();
        bool is_frequency_recovering_beyond_current_minimum_frequency() const;
        bool is_minimum_frequency_declining() const;
        bool is_minimum_frequency_not_changing() const;

        double get_total_discrete_shed_scale_in_pu() const;


        FIRST_ORDER_BLOCK frequency_sensor;
        double continuous_frequency_threshold_in_Hz;
        double K;
        double t_delay;
        double max_continuous_shed_scale;

        UFLS_TRIGGER_SIGNAL trigger_signal;
        double additional_stage_frequency_threshold_in_Hz;
        TIMER additional_stage_timer;
        double additional_shed_scale;
        bool flag_additional_stage_is_tripped;

        double discrete_stage_shed_scale_in_pu[MAX_LOAD_RELAY_STAGE];
        TIMER discrete_stage_timer[MAX_LOAD_RELAY_STAGE];
        bool flag_discrete_stage_is_tripped[MAX_LOAD_RELAY_STAGE];

        CONTINUOUS_BUFFER history_minimum_frequency_buffer;

        double current_continuous_shed_command_in_pu;
};

#endif // PUFLS_H
