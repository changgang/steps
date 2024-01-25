#ifndef VSGCONTROL2_H
#define VSGCONTROL2_H

#include "header/model/vsg_model/vsg_model2.h"
#include "header/block/integral_block.h"

class VSGCONTROL2 : public VSG_MODEL2
{
    public:
        VSGCONTROL2(STEPS& toolkit);
        virtual ~VSGCONTROL2();
        VSGCONTROL2(const VSGCONTROL2& model);
        virtual VSGCONTROL2& operator=(const VSGCONTROL2& model);
        void clear();
        void copy_from_const_model(const VSGCONTROL2& model);
    public:
        void set_Tj_in_s(double Tj);
        void set_D(double D);
        void set_Ku(double Ku);
        void set_Te_in_s(double Ke);

        double get_Tj_in_s() const;
        double get_D() const;
        double get_Ku() const;
        double get_Te_in_s() const;
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

#endif // VSGCONTROL2_H
