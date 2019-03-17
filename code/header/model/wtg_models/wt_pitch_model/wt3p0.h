#ifndef WT3P0_H
#define WT3P0_H

#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class WT3P0 : public WT_PITCH_MODEL
{
    public:
        WT3P0();
        WT3P0(const WT3P0&model);
        virtual ~WT3P0();
        virtual WT3P0& operator=(const WT3P0& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_Tspeed_in_s(double T);
        void set_Kp_speed_controller(double K);
        void set_Ki_speed_controller(double K);
        void set_Tfrequency_in_s(double T);
        void set_Kp_frequency_controller(double K);
        void set_Ki_frequency_controller(double K);
        void set_Kd_frequency_controller(double K);
        void set_Td_frequency_controller_in_s(double T);
        void set_Pitchmax_in_deg(double P);
        void set_Pitchmin_in_deg(double P);
        void set_ratePitchmax_in_deg_per_s(double rP);
        void set_Tp_in_s(double T);

        double get_Tspeed_in_s() const;
        double get_Kp_speed_controller() const;
        double get_Ki_speed_controller() const;
        double get_Tfrequency_in_s() const;
        double get_Kp_frequency_controller() const;
        double get_Ki_frequency_controller() const;
        double get_Kd_frequency_controller() const;
        double get_Td_frequency_controller_in_s() const;
        double get_Pitchmax_in_deg() const;
        double get_Pitchmin_in_deg() const;
        double get_ratePitchmax_in_deg_per_s() const;
        double get_Tp_in_s() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
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

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WT3P0& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK speed_reference_sensor;
        PI_BLOCK speed_controller;
        FIRST_ORDER_BLOCK frequency_sensor;
        PID_BLOCK frequency_controller;
        INTEGRAL_BLOCK pitch_integrator;
        double ratePitchmax;
};

#endif // WT3P0_H
