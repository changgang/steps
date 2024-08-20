#ifndef VRT_CONTROL_MODEL_H
#define VRT_CONTROL_MODEL_H

#include "header/STEPS.h"
#include "header/basic/constants.h"
#include "header/basic/timer.h"

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

        void set_device_pointer(NONBUS_DEVICE* device);
        NONBUS_DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_compound_device_name() const;

        double get_bus_voltage();

        void set_vrt_voltage_threshold(double v);
        void set_vrt_prepare2recover_delay_time_in_s(double t);

        double get_vrt_voltage_threshold() const;
        double get_vrt_prepare2recover_delay_time_in_s() const;

        void reset_vrt_prepare2recover_timer();
        void start_vrt_prepare2recover_timer();
        bool is_vrt_prepare2recover_timer_timed_out();


        void set_vrt_status(VRT_STATUS status);
        VRT_STATUS get_vrt_status() const;

        void set_current_Ip_command(double Ip);
        void set_current_Iq_command(double Iq);
        void set_current_P_command(double P);
        void set_current_Q_command(double Q);

        double get_current_Ip_command() const;
        double get_current_Iq_command() const;
        double get_current_P_command() const;
        double get_current_Q_command() const;

        void set_time_when_vrt_enter_during_status(double t);
        void set_time_when_vrt_enter_prepare2recover_status(double t);
        void set_time_when_vrt_enter_recover_status(double t);
        void set_Ip0_vrt_activated_in_pu(double I);
        void set_Iq0_vrt_activated_in_pu(double I);
        void set_P0_vrt_activated_in_pu_based_on_mbase(double P);
        void set_Q0_vrt_activated_in_pu_based_on_mbase(double Q);
        void set_Ip0_vrt_prepare2recover_activated_in_pu(double I);
        void set_Iq0_vrt_prepare2recover_activated_in_pu(double I);
        void set_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase(double P);
        void set_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase(double Q);
        void set_Ip0_vrt_recover_activated_in_pu(double I);
        void set_Iq0_vrt_recover_activated_in_pu(double I);
        void set_P0_vrt_recover_activated_in_pu_based_on_mbase(double P);
        void set_Q0_vrt_recover_activated_in_pu_based_on_mbase(double Q);

        double get_time_when_vrt_enter_during_status() const;
        double get_time_when_vrt_enter_prepare2recover_status() const;
        double get_time_when_vrt_enter_recover_status() const;
        double get_Ip0_vrt_activated_in_pu() const;
        double get_Ip0_vrt_prepare2recover_activated_in_pu() const;
        double get_Ip0_vrt_recover_activated_in_pu() const;
        double get_Iq0_vrt_activated_in_pu() const;
        double get_Iq0_vrt_prepare2recover_activated_in_pu() const;
        double get_Iq0_vrt_recover_activated_in_pu() const;
        double get_P0_vrt_activated_in_pu_based_on_mbase() const;
        double get_P0_vrt_prepare2recover_activated_in_pu_based_on_mbase() const;
        double get_P0_vrt_recover_activated_in_pu_based_on_mbase() const;
        double get_Q0_vrt_activated_in_pu_based_on_mbase() const;
        double get_Q0_vrt_prepare2recover_activated_in_pu_based_on_mbase() const;
        double get_Q0_vrt_recover_activated_in_pu_based_on_mbase() const;

        void update_PQI0_in_vrt_during_status(double P0, double Q0, double Ip0, double Iq0);
        void update_PQI0_in_vrt_prepare2recover_status(double P0, double Q0, double Ip0, double Iq0);
        void update_PQI0_in_vrt_recover_status(double P0, double Q0, double Ip0, double Iq0);
    public:
        void  copy_from_const_model(const VRT_CONTROL_MODEL& model);
    public:
        virtual void check_vrt_status() = 0;
        virtual double get_vrt_P_command() = 0;
        virtual double get_vrt_Q_command() = 0;

        double get_vrt_Ip_command();
        double get_vrt_Iq_command();
    private:
        STEPS* toolkit;
        BUS* bus_ptr;
        NONBUS_DEVICE* device;

        double vrt_voltage_threshold;
        TIMER prepare2recover_timer;

        VRT_STATUS status;
        double Ipcmd_now, Iqcmd_now, Pcmd_now, Qcmd_now;

        double Ip0_vrt_activated, Ip0_vrt_prepare2recover_activated, Ip0_vrt_recover_activated;
        double Iq0_vrt_activated, Iq0_vrt_prepare2recover_activated, Iq0_vrt_recover_activated;
        double P0_vrt_activated,  P0_vrt_prepare2recover_activated,  P0_vrt_recover_activated;
        double Q0_vrt_activated,  Q0_vrt_prepare2recover_activated,  Q0_vrt_recover_activated;

        double t0_vrt_activated,  t0_vrt_prepare2recover_activated,  t0_vrt_recover_activated;
};

#endif // VRT_CONTROL_MODEL_H
