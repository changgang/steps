#ifndef MTHVDCLCC1_H
#define MTHVDCLCC1_H


#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_lcc_model/hlcc_model.h"
#include "header/block/pi_block.h"
#include "header/block/first_order_block.h"
#include "header/model/lcc_hvdc2t_model/vdcol.h"
#include "header/basic/timer.h"

class MTHVDCLCC1: public HLCC_MODEL
{
    public:
        MTHVDCLCC1(STEPS& toolkit);
        ~MTHVDCLCC1();
        MTHVDCLCC1(const MTHVDCLCC1& model);
        virtual MTHVDCLCC1& operator=(const MTHVDCLCC1& model);
        void copy_from_const_model(const MTHVDCLCC1& model);

    public:

        virtual string get_model_name() const;

        void set_udc_sensor_block_Td_in_s(const double T);
        void set_idc_sensor_block_Td_in_s(const double T);
        void set_cc_block_kp(const double kp);
        void set_cc_block_ki(const double ki);
        void set_cea_block_kp(const double kp);
        void set_cea_block_ki(const double ki);
        void set_cv_block_kp(const double kp);
        void set_cv_block_ki(const double ki);

        void set_vdcol(const VDCOL& vdcol_limiter);
        void set_dynamic_max_firing_angle(const double max_angle);
        void set_dynamic_min_firing_angle(const double min_angle);
        void set_min_current_command_value(const double c0);
        void set_dynamic_idc_ref_in_kA(const double idc);
        void set_dynamic_angle_ref_in_deg(const double angle);
        void set_dynamic_udc_ref_in_kV(const double udc);

        void set_dynamic_min_unbypass_gamma_in_deg(const double gamma);

        double get_udc_sensor_block_Td_in_s() const;
        double get_idc_sensor_block_Td_in_s() const;
        double get_cc_block_kp() const;
        double get_cc_block_ki() const;
        double get_cea_block_kp() const;
        double get_cea_block_ki() const;
        double get_cv_block_kp() const;
        double get_cv_block_ki() const;

        VDCOL get_vdcol() const;
        double get_dynamic_max_firing_angle() const;
        double get_dynamic_min_firing_angle() const;
        double get_min_current_command_value() const;
        double get_idc_ref_in_kA() const;
        double get_idc_ref_in_kA_of_VDCOL(double Udc);
        double get_dynamic_angle_ref_in_deg() const;
        double get_dynamic_udc_ref_in_kV() const;

        double get_dynamic_min_unbypass_gamma_in_deg() const;

        double solve_inverter_gamma_in_deg_with_beta_in_rad_and_idc_in_kA(double beta,double idc);

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
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;
        virtual void update_converters();

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;


    public://input for initialize

    public://output
        double get_dynamic_command_firing_angle_in_rad();
        void check_bypass_logic();
        void check_block_logic();

        virtual double get_converter_angle_ref_in_deg_of_cc();
        virtual double get_converter_angle_ref_in_deg_of_cea();
        virtual double get_converter_angle_ref_in_deg_of_cv();
        virtual double get_converter_idc_ref_in_kA();



    private:
        //BLOCK
        FIRST_ORDER_BLOCK udc_sensor, idc_sensor;
        PI_BLOCK pi_cc;
        PI_BLOCK pi_cea;
        PI_BLOCK pi_cv;
        VDCOL vdcol_limiter;
        double time_of_last_bypass;

        double dynamic_max_angle, dynamic_min_angle;
        double dynamic_min_unbypass_gamma_in_deg;
        double min_current_command_value;
        double idc_ref_in_kA;
        double angle_ref_in_deg;
        double udc_ref_in_kV;

};

#endif // HLCC_MODEL_H
