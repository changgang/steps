#ifndef HLCC_MODEL_H
#define HLCC_MODEL_H

#include "header/model/mt_hvdc_model/mt_hvdc_model.h"


class HLCC;

class HLCC_MODEL : public MT_HVDC_MODEL
{
    public:
        HLCC_MODEL(STEPS& toolkit);
        ~HLCC_MODEL();

        void set_hlcc_pointer(HLCC* lcc);
        HLCC* get_hlcc_pointer() const;
        HLCC* get_hlcc_pointer_with_name(string name) const;
    public: // model type
        virtual string get_model_type() const;
    public: // specific model
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
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

        void set_model_data_with_index(unsigned int index, double value);
        double get_model_data_with_index(unsigned int index);

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;
        double get_model_internal_variable_with_index(unsigned int index);

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

        virtual void update_converters() = 0;

    public:
        virtual void set_bypass_flag(const bool flag);
        virtual void set_block_flag(const bool flag);
        virtual bool get_bypass_flag();
        virtual bool get_block_flag();
        virtual void block_lcc();
        virtual void bypass_lcc();
        virtual void unblock_lcc();
        virtual void unbypass_lcc();



    public://input for initialize
        void set_model_converter_name(const string name);
        string get_model_converter_name() const;


    public://output


    public:
        virtual double get_lcc_converter_ueq_in_kV() const;
        virtual double get_lcc_converter_req_ohm() const;

        virtual double get_converter_dc_poewr_in_MW();
        virtual complex<double> get_converter_ac_complex_power_in_MVA();
        virtual double get_converter_ac_power_factor();
        virtual complex<double> get_converter_ac_current_in_kA();
        virtual complex<double> get_converter_ac_current_in_pu_on_system_base();
        virtual double get_converter_angle_ref_in_deg_of_cc() = 0;
        virtual double get_converter_angle_ref_in_deg_of_cea() = 0;
        virtual double get_converter_angle_ref_in_deg_of_cv() = 0;
        virtual double get_converter_idc_ref_in_kA() = 0;


    private:
        HLCC* hlcc_ptr;
        string hlcc_name;

        bool is_blocked, is_bypassed;
        bool is_unblocking, is_unbypassing;



};

#endif // HLCC_MODEL_H
