#ifndef ESB0_H
#define ESB0_H

#include "header/model/energy_storage_model/es_primary_model/es_primary_model.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"
#include "header/block/proportional_block.h"

#include <cstdlib>

class ESB0 : public ES_PRIMARY_MODEL
{
    //battery model
    public:
        ESB0(STEPS& toolkit);
        ESB0(const ESB0& model);
        virtual ~ESB0();
        virtual ESB0& operator=(const ESB0&);
        string get_model_name() const;
    public:

        void set_converter_number(unsigned int n);
        void set_Nse_per_converter(unsigned int n);
        void set_Nsh_per_converter(unsigned int n);
        void set_Eoc_per_cell_in_V(double v);
        void set_Rb_per_cell_in_mOhm(double r);
        void set_Rp_per_cell_in_mOhm(double r);
        void set_Cp_per_cell_in_mF(double c);
        void set_capacitor_nominal_dc_voltage_in_kV(double v);
        void set_C_per_converter_in_F(double c);
        void set_Kpdc_per_converter(double k);
        void set_Kidc_per_converter(double k);

        unsigned int get_converter_number() const;
        unsigned int get_Nse_per_converter() const;
        unsigned int get_Nsh_per_converter() const;
        double get_Eoc_per_cell_in_V() const;
        double get_Rb_per_cell_in_mOhm() const;
        double get_Rp_per_cell_in_mOhm() const;
        double get_Cp_per_cell_in_mF() const;
        double get_capacitor_nominal_dc_voltage_in_kV() const;
        double get_C_per_converter_in_F() const;
        double get_Kpdc_per_converter() const;
        double get_Kidc_per_converter() const;

        double get_battery_dc_power_per_cell_in_W() const;
        double get_battery_dc_current_per_cell_in_A() const;
        double get_battery_dc_voltage_per_cell_in_V() const;

        virtual double get_valve_in_pu() const;
        virtual double get_delta_Y_in_pu() const;
        virtual double get_delta_Yref_in_pu() const;
        virtual double get_mechanical_power_in_pu_on_mbase() const;
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void initialize_Pmech_as_zero();

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
    private:
        void copy_from_const_model(const ESB0& model);
        double Eoc_per_cell_in_V;
        double Rp_per_cell_in_mOhm, Cp_per_cell_in_mF;
        double VdcN_in_kV;
        unsigned int Nse_per_converter, Nsh_per_converter;
        unsigned int Converter_number;
        unsigned int stage_number;

        PROPORTIONAL_BLOCK Rb_block;
        FIRST_ORDER_BLOCK battery_delay_block;
        PI_BLOCK Vdc_controller;
        INTEGRAL_BLOCK Capacitor;//T is C in F per converter
};

#endif // ESB0_H
