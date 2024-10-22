#ifndef ESCVX_H
#define ESCVX_H

#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/vsg0.h"


class ESCVX : public ES_CONVERTER_MODEL
{
    public:
        ESCVX(STEPS& toolkit);
        ESCVX(const ESCVX& model);
        virtual ~ESCVX();
        virtual ESCVX& operator=(const ESCVX&);

        void set_Tj_in_s(double T);
        void set_D(double D);
        void set_Ku(double Ku);
        void set_Te_in_s(double T);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        double get_Tj_in_s() const;
        double get_D() const;
        double get_Ku() const;
        double get_Te_in_s() const;
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
        void copy_from_const_model(const ESCVX& model);
        VSG0 vsg;
};

#endif // ESCVX_H
