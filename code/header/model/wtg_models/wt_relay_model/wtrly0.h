#ifndef WTRLY0_H
#define WTRLY0_H

#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"
#include "header/basic/timer.h"
#include "header/basic/constants.h"

class WTRLY0 : public WT_RELAY_MODEL
{
    public:
        WTRLY0();
        WTRLY0(const WTRLY0& model);
        virtual ~WTRLY0();
        virtual WTRLY0& operator=(const WTRLY0& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_wind_spped_relay_pair_in_pu_s(size_t index, double vwind, double tdelay);
        void set_rotor_speed_relay_pair_in_pu_s(size_t index, double speed, double tdelay);
        void set_bus_frequency_relay_pair_in_pu_s(size_t index, double freq, double tdelay);
        void set_bus_voltage_relay_pair_in_pu_s(size_t index, double volt, double tdelay);

        double get_wind_speed_relay_threshold_in_pu(size_t index) const;
        double get_wind_speed_relay_delay_in_s(size_t index) const;
        double get_rotor_speed_relay_threshold_in_pu(size_t index) const;
        double get_rotor_speed_relay_delay_in_s(size_t index) const;
        double get_bus_frequency_relay_threshold_in_pu(size_t index) const;
        double get_bus_frequency_relay_delay_in_s(size_t index) const;
        double get_bus_voltage_relay_threshold_in_pu(size_t index) const;
        double get_bus_voltage_relay_delay_in_s(size_t index) const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WTRLY0& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        void check_wind_speed_relay();
        void check_rotor_speed_relay();
        void check_bus_frequency_relay();
        void check_bus_voltage_relay();

        //  threshold, time delay, start time
        double vwind_th[MAX_RELAY_COUNT];
        double speed_th[MAX_RELAY_COUNT];
        double freq_th[MAX_RELAY_COUNT];
        double volt_th[MAX_RELAY_COUNT];

        TIMER vwind_relay_timer[MAX_RELAY_COUNT], speed_relay_timer[MAX_RELAY_COUNT],
              freq_relay_timer[MAX_RELAY_COUNT], volt_relay_timer[MAX_RELAY_COUNT];
};

#endif // WTRLY0_H
