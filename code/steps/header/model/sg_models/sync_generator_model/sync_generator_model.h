#ifndef SYNC_GENERATOR_MODEL_H
#define SYNC_GENERATOR_MODEL_H

#include "header/model/sg_models/sg_model.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"
#include "header/block/saturation_block.h"


class SYNC_GENERATOR_MODEL : public SG_MODEL
{
    /*
    Model of sync generator
    Inputs:
        Efd: excitation voltage
        Pmech: mechanical power
    Output:
        Isource: current injection to network
    */

    public:
        SYNC_GENERATOR_MODEL(STEPS& toolkit);
        virtual ~SYNC_GENERATOR_MODEL();
        void set_blocks_toolkit(STEPS& toolkit);

    public: // sync generator common
        virtual string get_model_type() const;

        void set_Rs(double Rs);
        void set_Xd(double Xd);
        void set_Xq(double Xq);
        void set_Xdp(double Xdp);
        void set_Xqp(double Xqp);
        void set_Xdpp(double Xdpp);
        void set_Xqpp(double Xqpp);
        void set_Xpp(double Xpp);
        void set_Xl(double Xl);
        void set_Td0p_in_s(double T);
        void set_Tq0p_in_s(double T);
        void set_Td0pp_in_s(double T);
        void set_Tq0pp_in_s(double T);
        void set_H_in_s(double H);
        void set_Tj_in_s(double Tj);
        void set_D(double D);
        void set_saturation_at_1(double s);
        void set_saturation_at_1p2(double s);
        void set_saturation_type(SATURATION_TYPE sat_type);

        double get_Rs() const;
        double get_Xd() const;
        double get_Xq() const;
        double get_Xdp() const;
        double get_Xqp() const;
        double get_Xdpp() const;
        double get_Xqpp() const;
        double get_Xpp() const;
        double get_Xl() const;
        double get_Td0p_in_s() const;
        double get_Tq0p_in_s() const;
        double get_Td0pp_in_s() const;
        double get_Tq0pp_in_s() const;
        double get_H_in_s() const;
        double get_Tj_in_s() const;
        double get_D() const;
        double get_saturation_at_1() const;
        double get_saturation_at_1p2() const;
        SATURATION_TYPE get_saturation_type() const;

        INTEGRAL_BLOCK* get_rotor_speed_block();
        INTEGRAL_BLOCK* get_rotor_angle_block();
        FIRST_ORDER_BLOCK* get_d_axis_transient_block();
        FIRST_ORDER_BLOCK* get_d_axis_subtransient_block();
        FIRST_ORDER_BLOCK* get_q_axis_transient_block();
        FIRST_ORDER_BLOCK* get_q_axis_subtransient_block();

        bool is_saturation_considered() const;
        double get_saturation_with_flux(double flux);
    public:
        // set initial efd and pmech for initialization
        void set_initial_mechanical_power_in_pu_based_on_mbase(double pmech);
        double get_initial_mechanical_power_in_pu_based_on_mbase() const;

        void set_initial_excitation_voltage_in_pu(double efd);
        double get_initial_excitation_voltage_in_pu() const;
        // get inputs for dynamics run
        double get_mechanical_power_in_pu_based_on_mbase() const;
        double get_mechanical_power_in_MW() const;
        double get_excitation_voltage_in_pu();
    public:
        virtual STEPS_SPARSE_MATRIX* get_linearized_matrix(string matrix_type);
        virtual void set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix);
    public: // specific sync generator model
        virtual string get_model_name() const = 0;

        virtual void update_source_impedance() = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize() = 0;
        virtual void initialize_rotor_angle() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_dq_axis_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase() = 0;
        virtual double get_terminal_current_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_current_in_pu_based_on_sbase() = 0;

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

        virtual double get_air_gap_power_in_pu_based_on_mbase() = 0;
        virtual double get_air_gap_power_in_MW() = 0;
        virtual double get_air_gap_torque_in_pu_based_on_mbase() = 0;
        virtual double get_accelerating_power_in_pu_based_on_mbase() = 0;
        virtual double get_accelerating_power_in_MW() = 0;
        virtual complex<double> get_terminal_complex_power_in_pu_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_power_in_MVA() = 0;
        virtual double get_terminal_active_power_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_active_power_in_MW() = 0;
        virtual double get_terminal_reactive_power_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_reactive_power_in_MVar() = 0;
        virtual double get_field_current_in_pu_based_on_mbase() = 0;

        virtual double get_rotor_angle_in_rad() = 0;
        virtual double get_rotor_angle_in_deg() = 0;
        virtual double get_rotor_speed_deviation_in_pu() = 0;
        virtual double get_rotor_speed_in_pu() = 0;
        virtual complex<double> get_internal_voltage_in_pu_in_dq_axis() = 0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() = 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

        virtual void linearize() = 0;
    public:
        // the following two functions are used to model GENCLS as ideal voltage source
        // with set_excitation_voltage_in_pu(), GENCLS is user controllable.
        void set_rotor_angle_in_deg(double angle);
        void set_rotor_speed_deviation_in_pu(double speed);

    private:
        void common_constructor();
        //double get_exponential_saturation_with_flux(double flux) const;
        //double get_quadratic_saturation_with_flux(double flux) const;
        //unsigned int generator_index;
        double Rs, Xd, Xq, Xdp, Xqp, Xdpp, Xqpp, Xl;
        double D;
        FIRST_ORDER_BLOCK transient_block_d_axis, subtransient_block_d_axis;
        FIRST_ORDER_BLOCK transient_block_q_axis, subtransient_block_q_axis;
        INTEGRAL_BLOCK rotor_angle_block, rotor_speed_block;
        SATURATION_BLOCK saturation_block;
        SATURATION_TYPE saturation_type;

        double Pmech0, Efd0;

        STEPS_SPARSE_MATRIX *gen_matrix;
        STEPS_SPARSE_MATRIX *gen_avr_matrix;
        STEPS_SPARSE_MATRIX *gen_gov_matrix;
};

#endif // SYNC_GENERATOR_MODEL_H
