#ifndef WT_PITCH_MODEL_H
#define WT_PITCH_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
class WT_PITCH_MODEL : public WTG_MODEL
{
    /*
    wind turbine mechanical model:
    inputs:
        vwind: wind speed
        pitch: pitch angle
        pelec: electrical power generation
    output:
        pmech: mechanical power
        speed: turbine generator speed
        speedRef: speed reference
        angle: turbine generator angle
    */
    public:
        WT_PITCH_MODEL();
        virtual ~WT_PITCH_MODEL();

        virtual string get_model_type() const;
        // inputs
        double get_wt_generator_speed_in_pu() const;
        double get_bus_frequency_in_pu() const;
        double get_wt_active_power_command_in_pu() const;
        //reference
        void set_speed_reference_in_pu(double speed);
        void set_frequency_reference_in_pu(double freq);
        void set_power_reference_in_pu(double power);

        double get_speed_reference_in_pu() const;
        double get_frequency_reference_in_pu() const;
        double get_power_reference_in_pu() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual double get_double_data_with_index(size_t index) const = 0;
        virtual double get_double_data_with_name(string par_name) const = 0;
        virtual void set_double_data_with_index(size_t index, double value) = 0;
        virtual void set_double_data_with_name(string par_name, double value) = 0;

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_pitch_angle_in_deg() const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual size_t get_variable_index_from_variable_name(string var_name)= 0;
        virtual string get_variable_name_from_variable_index(size_t var_index)= 0;
        virtual double get_variable_with_index(size_t var_index)= 0;
        virtual double get_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        void common_constructor();

        double speed_reference_in_pu;
        double frequency_reference_in_pu;
        double power_reference_in_pu;
};

#endif // WT_PITCH_MODEL_H
