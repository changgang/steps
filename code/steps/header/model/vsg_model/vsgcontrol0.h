#ifndef VSG0_H
#define VSG0_H

#include "header/model/vsg_model/vsg_model.h"
#include "header/block/integral_block.h"
#include <complex>

class VSGCONTROL0 : public VSG_MODEL
{
    public:
        VSGCONTROL0(STEPS& toolkit);
        VSGCONTROL0(const VSGCONTROL0& model);
        virtual ~VSGCONTROL0();
        virtual VSGCONTROL0& operator=(const VSGCONTROL0& model);

        void set_Tj_in_s(double Tj);
        void set_D(double D);
        void set_Ku(double Ku);
        void set_Te_in_s(double Ke);

        double get_Tj_in_s() const;
        double get_D() const;
        double get_Ku() const;
        double get_Te_in_s() const;

    public: // specific model level
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

        virtual double get_virtual_speed_deviation_in_pu() const;
        virtual double get_virtual_angle_in_rad() const;
        virtual double get_virtual_voltage_in_pu() const;
    private:
        void copy_from_const_model(const VSGCONTROL0& model);
    private:
        double D, Ku;
        INTEGRAL_BLOCK virtual_speed_deviation_block, virtual_angle_block, virtual_voltage_block;

};
#endif // VSG0_H
