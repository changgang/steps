#ifndef CONVCOL1_H
#define CONVCOL1_H

#include "header/model/common_model/common_model.h"

enum CURRENT_LIMIT_MODE // Current Order Limiter
{
    DQ_PROPORTION_LIMIT = 0,    //Current magnitude will be limited at Imax but its phase angle will remain unchanged.
    SPECIFIED_MAXIMUN_DQ_COMPONENT_OF_I_LIMITED = 1,
    SPECIFIED_I_LIMITED = 2
};

enum DQ_ORIENTED_FLAG // Current Limite mode
{
    ACL_ORIENTED = 0,  //The d-axis phase is equal to the active power control loop(ACL) output phase.
    PLL_ORIENTED = 1   //The d-axis phase is equal to the PLL output phase.
};

class CONVCOL1:public COMMON_MODEL
{
    public:
        CONVCOL1(STEPS& toolkit);
        ~CONVCOL1();
        string get_model_name() const;

        void set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode);
        void set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag);
        void set_phi_i_in_deg(double phi_i_in_deg);
        void set_phi_i_in_rad(double phi_i_in_rad);
        void set_Imax(double I);

        CURRENT_LIMIT_MODE get_current_limit_mode() const;
        DQ_ORIENTED_FLAG get_dq_oriented_flag() const;
        double get_phi_i_in_deg() const;
        double get_phi_i_in_rad() const;
        double get_Imax() const;

        complex<double> get_limited_xy_current_order(complex<double> Ixy_guess, double virtual_angle_in_rad, double pll_angle_in_rad) const;
    private:
        CURRENT_LIMIT_MODE current_limit_mode;
        DQ_ORIENTED_FLAG dq_oriented_flag;
        double phi_i_in_rad;
        double Imax;
};

#endif // CONVCOL1_H
