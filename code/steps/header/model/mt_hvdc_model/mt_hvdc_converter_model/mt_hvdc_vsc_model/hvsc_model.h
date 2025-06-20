#ifndef HVSC_MODEL_H
#define HVSC_MODEL_H

#include "header/model/mt_hvdc_model/mt_hvdc_model.h"

class HVSC;

class HVSC_MODEL : public MT_HVDC_MODEL
{
    public:
        HVSC_MODEL(STEPS& toolkit);
        ~HVSC_MODEL();

        void set_hvsc_pointer(HVSC* vsc);

        HVSC* get_hvsc_pointer_with_name(string name) const;
        HVSC* get_hvsc_pointer() const;
    public: // model type
        virtual string get_model_type() const;
    public: // specific model
        virtual string get_model_name() const = 0;

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
        virtual void update_converters() = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        void set_model_data_with_index(unsigned int index, double value);
        double get_model_data_with_index(unsigned int index);

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;
        double get_model_internal_variable_with_index(unsigned int index);

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;


    public://input for initialize
        void set_model_converter_name(const string name);
        string get_model_converter_name();

    public:
        //
        virtual double get_dynamic_pll_angle_in_deg() const;
        virtual double get_dynamic_pll_angle_in_rad() const;

        //output
        virtual double get_vsc_converter_ueq_in_kV() const = 0;
        virtual double get_vsc_converter_req_ohm() const = 0;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base() const = 0;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_converter_base() const = 0;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_system_base() const = 0;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA() const = 0;
        virtual complex<double> get_converter_dynamic_ac_complex_power() const = 0;
        virtual double get_converter_dynamic_active_power_in_MW() const = 0;
        virtual double get_converter_dynamic_reactive_power_in_Mvar() const = 0;

        virtual double get_converter_dynamic_current_from_converter_to_ceq_in_kA() const = 0;

        virtual void set_dc_voltage_block_ceq_in_uF(const double ceq) = 0;
        virtual double get_dc_voltage_block_ceq_in_uF() const  = 0;




    private:
        HVSC* vsc_ptr;
        string hvsc_name;

};

#endif // HLCC_MODEL_H
