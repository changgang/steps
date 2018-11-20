#ifndef WT3P1_H
#define WT3P1_H

#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"

class WT3P1 : public WT_PITCH_MODEL
{
    public:
        WT3P1();
        WT3P1(const WT3P1&model);
        virtual ~WT3P1();
        virtual WT3P1& operator=(const WT3P1& model);
    public: // specific model level
        virtual string get_model_name() const;
        virtual double get_model_data_with_index(size_t index) const;
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_index(size_t index, double value);
        virtual void set_model_data_with_name(string par_name, double value);

        void set_Tp_in_s(double T);
        void set_Kp_speed_controller(double K);
        void set_Ki_speed_controller(double K);
        void set_Kp_power_controller(double K);
        void set_Ki_power_controller(double K);
        void set_Pitchmax_in_deg(double P);
        void set_Pitchmin_in_deg(double P);
        void set_ratePitchmax_in_deg_per_s(double rP);

        double get_Tp_in_s() const;
        double get_Kp_speed_controller() const;
        double get_Ki_speed_controller() const;
        double get_Kp_power_controller() const;
        double get_Ki_power_controller() const;
        double get_Pitchmax_in_deg() const;
        double get_Pitchmin_in_deg() const;
        double get_ratePitchmax_in_deg_per_s() const;
    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_pitch_angle_in_deg() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WT3P1& model);
        virtual void prepare_model_internal_variable_table();
        PI_BLOCK speed_controller;
        PI_BLOCK power_controller;
        INTEGRAL_BLOCK pitch_integrator;
        double ratePitchmax;
};

#endif // WT3P1_H
