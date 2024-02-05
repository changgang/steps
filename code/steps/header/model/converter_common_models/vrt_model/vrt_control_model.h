#ifndef VRT_CONTROL_MODEL_H
#define VRT_CONTROL_MODEL_H

#include "header/STEPS.h"
#include "header/basic/constants.h"
#include "header/basic/multi_point_line.h"

class BUS;

class VRT_CONTROL_MODEL
{
    public:
        VRT_CONTROL_MODEL(STEPS& toolkit);
        ~VRT_CONTROL_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;
        double get_bus_voltage();

        void set_vrt_voltage_threshold(double v);
        void set_P0_pu(double P);
        void set_Q0_pu(double Q);
        void set_Ip0_pu(double I);
        void set_Iq0_pu(double I);

        double get_vrt_voltage_threshold() const;
        double get_P0_pu() const;
        double get_Q0_pu() const;
        double get_Ip0_pu() const;
        double get_Iq0_pu() const;

        void set_vrt_status(VRT_STATUS status);
        double get_vrt_activation_time() const;
        double get_vrt_start2recover_time() const;
        VRT_STATUS get_vrt_status() const;

        double get_vrt_P_command();
        double get_vrt_Q_command();
        double get_vrt_Ip_command();
        double get_vrt_Iq_command();
    public:
        void  copy_from_const_model(const VRT_CONTROL_MODEL& model);
        virtual void check_vrt_status() = 0;
    private:
        STEPS* toolkit;
        BUS* bus_ptr;

        double vrt_voltage_threshold;
        double t0_vrt_activated;
        double t0_vrt_start2recover;
        VRT_STATUS status;

        double P0, Q0;
        double Ip0, Iq0;
        double K, Kp, K;
};

#endif // VRT_CONTROL_MODEL_H
