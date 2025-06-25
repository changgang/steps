#ifndef ESCVXG1_H
#define ESCVXG1_H

#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/general_gfm1.h"


class ESCVXG1 : public ES_CONVERTER_MODEL
{
    public:
        ESCVXG1(STEPS& toolkit);
        ESCVXG1(const ESCVXG1& model);
        virtual ~ESCVXG1();
        virtual ESCVXG1& operator=(const ESCVXG1&);

        void set_control_mode(CONTROL_MODE mode);
        void set_voltage_flag(unsigned int flag);
        void set_Kp(double Kp);
        void set_Tp_in_s(double Tp);
        void set_Tpf_in_s(double Tpf);
        void set_Tqf_in_s(double Tqf);
        void set_Tvf_in_s(double Tvf);
        void set_mq(double mq);
        void set_mv(double mv);
        void set_Kpv(double Kpv);
        void set_Kiv(double Kiv);
        void set_Emax(double Emax);
        void set_Emin(double Emin);
        void set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode);
        void set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag);
        void set_phi_i_in_deg(double phi_i_in_deg);
        void set_Imax(double Imax);
        void set_KPLL(double Kp);
        void set_KIPLL(double Ki);
        void set_PLLmax(double pmax);
        void set_PLLmin(double pmin);

        CONTROL_MODE get_control_mode() const;
        unsigned int get_voltage_flag() const;
        double get_Kp() const;
        double get_Tp_in_s() const;
        double get_Tpf_in_s() const;
        double get_Tqf_in_s() const;
        double get_Tvf_in_s() const;
        double get_mq() const;
        double get_mv() const;
        double get_Kpv() const;
        double get_Kiv() const;
        double get_Emax() const;
        double get_Emin() const;
        CURRENT_LIMIT_MODE get_current_limit_mode() const;
        DQ_ORIENTED_FLAG get_dq_oriented_flag() const;
        double get_phi_i_in_deg() const;
        double get_phi_i_in_rad() const;
        double get_Imax() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_PLLmin() const;

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
        void copy_from_const_model(const ESCVXG1& model);

        CONTROL_MODE control_mode;
        GENERAL_GFM1 general_gfm1;
};

#endif // ESCVXG1_H
