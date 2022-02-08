#ifndef VSCHVDCC0_H_INCLUDED
#define VSCHVDCC0_H_INCLUDED

#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
#include "header/block/pid_block.h"

class VSCHVDCC0: public VSC_HVDC_CONVERTER_MODEL
{
    public:
        VSCHVDCC0(STEPS& toolkit);
        VSCHVDCC0(const VSCHVDCC0& model);
        virtual ~VSCHVDCC0();
        virtual VSCHVDCC0& operator=(const VSCHVDCC0& model);
    public:
        virtual string get_model_name() const;

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

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:
        virtual complex<double> get_converter_voltage_in_dq_axis_in_pu();
        virtual complex<double> get_converter_voltage_in_xy_axis_in_pu();
        virtual complex<double> get_converter_current_in_dq_axis_in_pu();
        virtual complex<double> get_converter_current_in_xy_axis_in_pu();
        virtual complex<double> get_converter_Norton_current_in_xy_axis_in_pu_based_on_SBASE();
    private:
        void copy_from_const_model(const VSCHVDCC0& model);
    private:
        PID_BLOCK p_udc_block, p_pac_block, p_f_block, pmax_block, pmin_block;
        PID_BLOCK q_block;
};
#endif // VSCHVDCC0_H_INCLUDED
