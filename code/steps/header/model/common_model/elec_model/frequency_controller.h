#ifndef FREQUENCY_CONTROLLER_H
#define FREQUENCY_CONTROLLER_H

#include "header/model/common_model/elec_model/frequency_controller.h"
#include "header/model/common_model/common_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"

class FREQUENCY_CONTROLLER: public COMMON_MODEL
{
    public:
        FREQUENCY_CONTROLLER(STEPS& toolkit);
        FREQUENCY_CONTROLLER(const FREQUENCY_CONTROLLER& model);
         ~FREQUENCY_CONTROLLER();
        FREQUENCY_CONTROLLER& operator=(const FREQUENCY_CONTROLLER& model);
        void clear();
        void copy_from_const_model(const FREQUENCY_CONTROLLER& model);
    public:

        void set_frequency_deviation_upper_deadband_in_pu(double f);
        void set_frequency_deviation_lower_deadband_in_pu(double f);
        void set_Kfint(double K);
        void set_Kvi(double K);
        void set_Tvi_in_s(double T);
        void set_Kdroop(double K);
        void set_Tdroop_in_s(double T);

        inline double get_frequency_deviation_upper_deadband_in_pu() const{return f_upper_pu;}
        inline double get_frequency_deviation_lower_deadband_in_pu() const{return f_lower_pu;}
        double get_Kfint() const;
        double get_Kvi() const;
        double get_Tvi_in_s() const;
        double get_Kdroop() const;
        double get_Tdroop_in_s() const;

        double get_virtual_inertia_controller_state() const;
        double get_frequency_droop_controller_state() const;
        double get_frequency_integral_controller_state() const;

        double get_virtual_inertia_power_deviation_in_pu() const;
        double get_frequency_droop_power_deviation_in_pu() const;
        double get_frequency_integral_power_deviation_in_pu() const;

        double get_frequency_regulation_power_deviation_in_pu() const;
    public:

        void initialize();
        void run(DYNAMIC_MODE mode);

    private:

        double f_upper_pu, f_lower_pu;
        DIFFERENTIAL_BLOCK virtual_inertia_controller;
        FIRST_ORDER_BLOCK frequency_droop_controller;
        INTEGRAL_BLOCK frequency_integral_controller;
};

#endif // FREQUENCY_CONTROLLER_H

