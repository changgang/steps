#ifndef ES_SOC_MODEL_H
#define ES_SOC_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include "header/block/integral_block.h"
#include <cstdlib>

class ES_SOC_MODEL : public ES_MODEL
{
    public:
        ES_SOC_MODEL(STEPS& toolkit);
        virtual ~ES_SOC_MODEL();
    public:
        virtual string get_model_type() const;
        void set_energy_state_block_toolkit(STEPS& toolkit);
    public:
        virtual string get_model_name() const = 0;

        void set_max_SOC_in_pu(double s);
        void set_min_SOC_in_pu(double s);
        void set_initial_SOC_in_pu(double s);
        void set_nominal_charge_time_in_s(double t);
        void set_charge_efficiency(double u);
        void set_discharge_efficiency(double u);


        double get_max_SOC_in_pu() const;
        double get_min_SOC_in_pu() const;
        double get_initial_SOC_in_pu() const;
        double get_nominal_charge_time_in_s() const;
        double get_charge_efficiency() const;
        double get_discharge_efficiency() const;


        double get_terminal_active_power_in_pu_based_on_mbase() const;
        double get_battery_power_in_pu_based_on_mbase();
        double get_battery_power_in_MW();
        double get_state_of_charge_in_pu() const;

        INTEGRAL_BLOCK* get_energy_state_block();

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        double SOC_max, SOC_min, SOC_initial;
        double Kin, Kout;
        INTEGRAL_BLOCK energy_state_block;
};

#endif // ES_SOC_MODEL_H
