#ifndef VSC_HVDC_CONVERTER_MODEL_H_INCLUDED
#define VSC_HVDC_CONVERTER_MODEL_H_INCLUDED

#include "header/model/vsc_hvdc_model/vsc_hvdc_model.h"

class VSC_HVDC_CONVERTER_MODEL: public VSC_HVDC_MODEL
{
    public:
        VSC_HVDC_CONVERTER_MODEL(STEPS& toolkit);
        virtual ~VSC_HVDC_CONVERTER_MODEL();
    public: // model type
        virtual string get_model_type() const;

        void set_converter_name(string name);
        string get_converter_name() const;
        unsigned int get_converter_name_index() const;
        unsigned int get_converter_index() const;
        unsigned int get_converter_bus() const;

        void set_as_voltage_source();
        void set_as_current_source();
        bool is_voltage_source() const;

        void set_converter_active_control_mode(VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode);
        void set_converter_reactive_control_mode(VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode);

        VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE get_converter_active_control_mode() const;
        VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE get_converter_reactive_control_mode() const;

        BUS* get_converter_ac_bus_pointer() const;
        double get_converter_capacity_in_MVA() const;
        double get_converter_base_voltage_in_kV() const;
        double get_converter_ac_bus_base_voltage_in_kV() const;
        double get_converter_ac_bus_angle_in_deg() const;
        double get_converter_ac_bus_angle_in_rad() const;
        complex<double> get_converter_ac_bus_complex_voltage_in_pu() const;
        complex<double> get_converter_ac_bus_complex_voltage_in_kV() const;

        double get_converter_initial_P_to_AC_bus_in_MW() const;
        double get_converter_initial_Q_to_AC_bus_in_MVar() const;
        double get_converter_initial_ac_angle_at_converter_side_in_rad() const;
        double get_converter_initial_ac_angle_at_converter_side_in_deg() const;
        complex<double> get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_kA() const;
        complex<double> get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_system_base() const;
        complex<double> get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_converter_base() const;
        complex<double> get_converter_initial_current_from_converter_to_ac_bus_in_dq_axis_in_pu_on_converter_base() const;

        double get_converter_dynamic_P_from_converter_to_AC_bus_in_MW() const;
        double get_converter_dynamic_Q_from_converter_to_AC_bus_in_MVar() const;
        double get_converter_dynamic_ac_angle_at_converter_side_in_rad() const;
        double get_converter_dynamic_ac_angle_at_converter_side_in_deg() const;

        double get_converter_ac_angle_at_ac_bus_side_in_rad() const;
        double get_converter_ac_angle_at_ac_bus_side_in_deg() const;

        complex<double> get_converter_current_from_converter_to_ac_bus_in_xy_axis_in_kA() const;
        complex<double> get_converter_current_from_converter_to_ac_bus_in_xy_axis_in_pu_based_on_converter_base() const;

    public:
        virtual complex<double> get_converter_voltage_in_dq_axis_in_pu() const = 0;
        virtual complex<double> get_converter_current_from_converter_to_ac_network_in_dq_axis_in_pu_on_converter_base() const = 0;


        complex<double> get_converter_voltage_in_xy_axis_in_pu();
        complex<double> get_converter_current_from_converter_to_ac_network_in_xy_axis_in_pu_on_converter_base();
        complex<double> get_converter_current_from_converter_to_ac_network_in_xy_axis_in_pu_on_system_base();

        complex<double> get_converter_current_from_converter_to_ac_network_in_dq_axis_in_pu_on_system_base();
        complex<double> get_converter_Norton_current_in_xy_axis_in_pu_based_on_system_base();

    public:
        // specific model
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
        unsigned int converter_index;
        unsigned int converter_name_index;
        bool voltage_source_flag;

        BUS* converter_ac_busptr;

        unsigned int control_mode; // change to enum, dynamic version.
        VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_power_control_mode;
        VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_power_control_mode;
};

#endif // VSC_HVDC_CONVERTER_MODEL_H_INCLUDED
