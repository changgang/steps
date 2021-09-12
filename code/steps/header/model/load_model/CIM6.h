#ifndef CIM6_H
#define CIM6_H

#include "header/model/load_model/load_model.h"
#include "header/block/saturation_block.h"
#include "header/block/integral_block.h"
#include <complex>

class CIM6 : public LOAD_MODEL
{
    public:
        CIM6(STEPS& toolkit);
        CIM6(const CIM6& model);
        virtual ~CIM6();
        virtual CIM6& operator=(const CIM6& model);

        virtual string get_model_name() const;

        void set_motor_type(unsigned int t);
        void set_Ra_in_pu(double r);
        void set_Xa_in_pu(double x);
        void set_Xm_in_pu(double x);
        void set_R1_in_pu(double r);
        void set_X1_in_pu(double x);
        void set_R2_in_pu(double r);
        void set_X2_in_pu(double x);
        void set_E1_in_pu(double e);
        void set_SE1_in_pu(double s);
        void set_E2_in_pu(double e);
        void set_SE2_in_pu(double s);
        void set_Mbase_in_MVA(double mbase);
        void set_Pmult(double pmult);
        void set_H_in_s(double h);
        void set_VI_in_pu(double v);
        void set_TI_in_cycles(double t);
        void set_TB_in_cycles(double t);
        void set_A(double a);
        void set_B(double b);
        void set_C(double c);
        void set_D(double d);
        void set_E(double e);
        void set_Tnom_in_pu(double t);

        unsigned int get_motor_type() const;
        double get_Ra_in_pu() const;
        double get_Xa_in_pu() const;
        double get_Xm_in_pu() const;
        double get_R1_in_pu() const;
        double get_X1_in_pu() const;
        double get_R2_in_pu() const;
        double get_X2_in_pu() const;
        double get_E1_in_pu() const;
        double get_SE1_in_pu() const;
        double get_E2_in_pu() const;
        double get_SE2_in_pu() const;
        double get_Mbase_in_MVA() const;
        double get_Pmult() const;
        double get_H_in_s() const;
        double get_VI_in_pu() const;
        double get_TI_in_cycles() const;
        double get_TB_in_cycles() const;
        double get_A() const;
        double get_B() const;
        double get_C() const;
        double get_D() const;
        double get_E() const;
        double get_Tnom_in_pu() const;
    public:
        double get_slip_in_pu();
        double get_slip_in_Hz();

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual complex<double> get_dynamic_source_admittance_in_pu_based_on_SBASE();
        virtual complex<double> get_additional_admittance_in_pu_based_on_SBASE();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void initialize_to_start();
        virtual void run(DYNAMIC_MODE mode);
        complex<double> get_internal_voltage_in_pu() const;
        virtual complex<double> get_load_power_in_MVA();
        virtual complex<double> get_load_current_in_pu_based_on_SBASE();
        virtual complex<double> get_norton_current_in_pu_based_on_SBASE();
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        complex<double> get_initial_load_power_in_MVA() const;

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const CIM6& model);
        void setup_model_dynamic_parameters();

        double P0, Q0;

        unsigned int Motor_type;
        double Ra, Xa, Xm, R1, X1, R2, X2;
        SATURATION_BLOCK saturation;
        double Mbase;
        double Pmult;
        double H;
        double VI, TI, TB;
        double A, B, C, D, E;
        double Tnominal;

        INTEGRAL_BLOCK speed_block;
        INTEGRAL_BLOCK transient_block_x_axis, subtransient_block_x_axis;
        INTEGRAL_BLOCK transient_block_y_axis, subtransient_block_y_axis;

        bool is_single_cage;
        double Xs, Xp, Xpp, Xleakage, Tp, Tpp;
        double W0;
        double Xs_minum_Xp, Xp_minum_Xleakage, Xp_minum_Xpp,
               Xp_minum_Xpp_over_Xp_minum_Xleakage,
               Xpp_minum_Xleakage_over_Xp_minum_Xleakage,
               Xp_minum_Xpp_over_Xp_minum_Xleakage_square;
        complex<double> Zsource;
        double initial_load_torque;
        double nominal_shunt_in_MVar;
};
#endif // CIM6_H
