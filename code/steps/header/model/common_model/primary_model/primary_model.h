#ifndef PRIMARY_MODEL_H
#define PRIMARY_MODEL_H

#include "header/model/common_model/common_model.h"
#include "header/block/first_order_block.h"

class PRIMARY_MODEL:public COMMON_MODEL
{
    public:
        PRIMARY_MODEL(STEPS& toolkit);
        virtual ~PRIMARY_MODEL();
        void clear();
    public:

        void set_speed_in_pu(double s);
        void set_active_power_reference_in_pu(double p);
        void set_speed_reference_in_pu(double s);
        void set_valve_reference_in_pu(double v);
        void set_initial_active_power_reference_in_pu(double p);
        void set_initial_speed_in_pu(double s);
        void set_initial_mechanical_power_in_pu(double p);
        void set_initial_valve_reference_in_pu(double v);
        void set_Ka(double K);
        void set_Eb(double K);
        void set_Bp(double K);

        double get_speed_in_pu() const;
        double get_active_power_reference_in_pu() const;
        double get_speed_reference_in_pu() const;
        double get_valve_reference_in_pu() const;
        double get_initial_active_power_reference_in_pu() const;
        double get_initial_speed_in_pu() const;
        double get_initial_mechanical_power_in_pu() const;
        double get_initial_valve_reference_in_pu() const;
        double get_Ka() const;
        double get_Eb() const;
        double get_Bp() const;

    public:

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

    private:
        //input from outside model
        double speed_in_pu;

        //reference
        double active_power_reference_in_pu;
        double speed_reference_in_pu;
        double valve_reference_in_pu;

        //initial value
        double initial_Pref;
        double initial_speed;
        double initial_Pmech;
        double initial_valve_reference;

        //parameters
        double Ka;
        double Eb;
        double Bp;


};

#endif // PRIMARY_MODEL_H

