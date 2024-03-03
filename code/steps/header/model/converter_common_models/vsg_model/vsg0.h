#ifndef VSG0_H
#define VSG0_H

#include "header/model/converter_common_models/vsg_model/vsg_model.h"
#include "header/block/integral_block.h"

class VSG0 : public VSG_MODEL
{
    public:
        VSG0(STEPS& toolkit);
        virtual ~VSG0();
        VSG0(const VSG0& model);
        virtual VSG0& operator=(const VSG0& model);
        void clear();
        void copy_from_const_model(const VSG0& model);
    public:
        void set_Tj_in_s(double Tj);
        void set_D(double D);
        void set_Ku(double Ku);
        void set_Te_in_s(double Ke);

        double get_Tj_in_s() const;
        double get_D() const;
        double get_Ku() const;
        double get_Te_in_s() const;

        double get_virtual_frequency_deviation_block_state() const;
        double get_virtual_angle_block_state() const;
        double get_virtual_voltage_block_state() const;
    public:
        virtual string get_model_name() const;
        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual double get_virtual_frequency_deviation_in_Hz() const;
        virtual double get_virtual_angle_in_rad() const;
        virtual double get_virtual_voltage_in_pu() const;
        virtual string get_standard_psse_string() const;
    private:
        double D, Ku;
        INTEGRAL_BLOCK virtual_frequency_deviation_block, virtual_angle_block, virtual_voltage_block;
};

#endif // VSG0_H
