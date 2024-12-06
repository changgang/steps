#ifndef PRIM0_H
#define PRIM0_H

#include "header/model/common_model/primary_model/primary_model.h"
#include "header/block/first_order_block.h"
#include "header/block/integral_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/proportional_block.h"

class PRIM0:public PRIMARY_MODEL
{
    public:
        PRIM0(STEPS& toolkit);
        PRIM0(const PRIM0& model);
        virtual ~PRIM0();
        virtual PRIM0& operator=(const PRIM0& model);
        virtual void clear();
        void copy_from_const_model(const PRIM0& model);
        string get_model_name() const;
    public:

        void set_Tr_in_s(double T);
        void set_Kp(double k);
        void set_Ki(double k);
        void set_INTmax(double k);
        void set_INTmin(double k);
        void set_PImax(double k);
        void set_PImin(double k);
        void set_umax(double k);
        void set_umin(double k);
        void set_Ts_in_s(double T);
        void set_Twg_in_s(double T);

        double get_Tr_in_s() const;
        double get_Kp() const;
        double get_Ki() const;
        double get_INTmax() const;
        double get_INTmin() const;
        double get_PImax() const;
        double get_PImin() const;
        double get_umax() const;
        double get_umin() const;
        double get_Ts_in_s() const;
        double get_Twg_in_s() const;

        double get_valve_in_pu() const;
        double get_delta_Y_in_pu() const;
        double get_delta_Yref_in_pu() const;
        double get_mechanical_power_in_pu_on_mbase() const;

    public:

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void initialize_mechanical_power_as_zero();

    private:

        double dbp, dbn;
        double PImax, PImin;
        double umax, umin;

        FIRST_ORDER_BLOCK speed_sensor;
        PROPORTIONAL_BLOCK Kp;
        INTEGRAL_BLOCK governor;
        FIRST_ORDER_BLOCK servo_motor;
        LEAD_LAG_BLOCK water_turbine;


};

#endif // PRIM0_H

