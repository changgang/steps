#ifndef GOV11_H
#define GOV11_H

#include "header/model/common_model/primary_model/primary_model.h"
#include "header/block/first_order_block.h"
#include "header/block/integral_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/proportional_block.h"

class GOV11:public PRIMARY_MODEL
{
    //PSASP11 turbine governor
    public:
        GOV11(STEPS& toolkit);
        virtual ~GOV11();
        void clear();
        string get_model_name() const;
    public:

        void set_Tr_in_s(double T);
        void set_dbp(double k);
        void set_dbn(double k);
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
        double get_dbp() const;
        double get_dbn() const;
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

        double get_valve_in_pu();
        double get_delta_Y_in_pu();

    public:

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

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

#endif // GOV11_H

