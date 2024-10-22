#ifndef WT3GXG_H
#define WT3GXG_H

#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/general_gfm.h"


class WT3GXG : public WT_GENERATOR_MODEL
{
    public:
        WT3GXG(STEPS& toolkit);
        WT3GXG(const WT3GXG& model);
        virtual ~WT3GXG();
        virtual WT3GXG& operator=(const WT3GXG&);

        void set_gfm_control_mode(GFM_CONTROL_MODE mode);
        void set_Kp(double Kp);
        void set_Tp_in_s(double Tp);
        void set_Kq(double Kq);
        void set_Tq_in_s(double Tq);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        GFM_CONTROL_MODE get_gfm_control_mode() const;
        double get_Kp() const;
        double get_Tp_in_s() const;
        double get_Kq() const;
        double get_Tq_in_s() const;
        COL_PQ_PRIORITY get_pq_priority_flag() const;
        double get_Imax() const;

        virtual string get_model_name() const;

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
        virtual double get_initial_active_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_voltage_command_in_pu() const;
        virtual double get_initial_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_power_command_in_pu_based_on_mbase() const;

        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WT3GXG& model);

        GFM_CONTROL_MODE control_mode;
        GENERAL_GFM general_gfm;
};

#endif // WT3GXG_H
