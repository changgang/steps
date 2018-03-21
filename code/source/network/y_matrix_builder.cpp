#include "header/network/y_matrix_builder.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <iostream>

using namespace std;

Y_MATRIX_BUILDER::Y_MATRIX_BUILDER()
{
    network_Y_matrix.clear();
    network_BP_matrix.clear();
    network_BQ_matrix.clear();
}

Y_MATRIX_BUILDER::~Y_MATRIX_BUILDER()
{
    network_Y_matrix.clear();
    network_BP_matrix.clear();
    network_BQ_matrix.clear();
}

SPARSE_MATRIX Y_MATRIX_BUILDER::build_network_matrix()
{
    network_Y_matrix.clear();

    add_lines_to_network();
    add_transformers_to_network();
    add_fixed_shunts_to_network();

    network_Y_matrix.compress_and_merge_duplicate_entries();

    return network_Y_matrix;
}


void Y_MATRIX_BUILDER::add_lines_to_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<LINE*> lines = db->get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i<n; i++)
        add_line_to_network(*(lines[i]));
}


void Y_MATRIX_BUILDER::add_line_to_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding line '%s' between bus %u and %u to network Y matrix.", line.get_identifier().c_str(),
                  line.get_sending_side_bus(), line.get_receiving_side_bus());
    show_information_with_leading_time_stamp(info);
    */
    size_t sending_bus = line.get_sending_side_bus();
    size_t receiving_bus = line.get_receiving_side_bus();

    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
    size_t j = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

    complex<double> Zline = line.get_line_positive_sequence_z_in_pu();
    complex<double> Yline = line.get_line_positive_sequence_y_in_pu();
    complex<double> Yshunt_sending = line.get_shunt_positive_sequence_y_at_sending_side_in_pu();
    complex<double> Yshunt_receiving = line.get_shunt_positive_sequence_y_at_receiving_side_in_pu();

    complex<double> Y, Z;
    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==true)
    {
        Y = 1.0/Zline;
        network_Y_matrix.add_entry(i,j, -Y);
        network_Y_matrix.add_entry(j,i, -Y);
        network_Y_matrix.add_entry(i,i, Y+0.5*Yline+Yshunt_sending);
        network_Y_matrix.add_entry(j,j, Y+0.5*Yline+Yshunt_receiving);
        return;
    }

    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==false)
    {
        Y = 0.5*Yline+Yshunt_receiving;
        Z = Zline + 1.0/Y;
        Y = 1.0/Z + 0.5*Yline+Yshunt_sending;
        network_Y_matrix.add_entry(i,i, Y);
        return;
    }

    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==true)
    {
        Y = 0.5*Yline+Yshunt_sending;
        Z = Zline + 1.0/Y;
        Y = 1.0/Z + 0.5*Yline+Yshunt_receiving;
        network_Y_matrix.add_entry(j,j, Y);
        return;
    }
}

void Y_MATRIX_BUILDER::add_transformers_to_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<TRANSFORMER*> transformers = db->get_all_transformers();

    size_t n = transformers.size();

    for(size_t i = 0; i<n; i++)
        add_transformer_to_network(*(transformers[i]));
}

void Y_MATRIX_BUILDER::add_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.is_two_winding_transformer())
        add_two_winding_transformer_to_network_v2(trans);
    else
        add_three_winding_transformer_to_network(trans);
}

void Y_MATRIX_BUILDER::add_two_winding_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding two-winding transformer '%s' between bus %u and %u to network Y matrix.", trans.get_identifier().c_str(),
                  trans.get_primary_winding_bus(), trans.get_secondary_winding_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t primary_bus = trans.get_primary_winding_bus();
    size_t secondary_bus = trans.get_secondary_winding_bus();

    size_t p = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
    size_t s = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

    complex<double> Zps = trans.get_leakage_impedance_between_primary_and_secondary_windings_based_on_system_base_power_in_pu();
    complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

    complex<double> Zp, Zs;
    Zp = 0.5*Zps; Zs = Zp;

    double tap_primary = trans.get_primary_winding_off_nominal_turn_ratio_in_pu();
    double angle_primary = trans.get_primary_winding_angle_shift_in_deg();
    angle_primary = deg2rad(angle_primary);
    double tap_secondary = trans.get_secondary_winding_off_nominal_turn_ratio_in_pu();
    double angle_secondary = trans.get_secondary_winding_angle_shift_in_deg();
    angle_secondary = deg2rad(angle_secondary);

    complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                    ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


    complex<double> V, I, Vstar;
    complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

    complex<double> kp_store, ks_store, Ym_store;
    kp_store = kp;
    ks_store = ks;
    Ym_store = Ym;

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_Y_matrix.add_entry(p,p,ypp);
        network_Y_matrix.add_entry(p,s,yps);
        network_Y_matrix.add_entry(s,p,ysp);
        network_Y_matrix.add_entry(s,s,yss);


        kp = kp/abs(kp);
        ks = ks/abs(ks);
        Ym = 0.0;

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BP_matrix.add_entry(p,p,ypp);
        network_BP_matrix.add_entry(p,s,yps);
        network_BP_matrix.add_entry(s,p,ysp);
        network_BP_matrix.add_entry(s,s,yss);


        kp = kp_store;
        ks = ks_store;

        kp = abs(kp);
        ks = abs(ks);
        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        network_BQ_matrix.add_entry(p,s,yps);
        network_BQ_matrix.add_entry(s,p,ysp);
        network_BQ_matrix.add_entry(s,s,yss);


        return;
    }

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==false)
    {
        V = 1.0;
        V = V/kp;
        Zeq = Zp + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        network_Y_matrix.add_entry(p,p,ypp);

        network_BP_matrix.add_entry(p,p,0.0);

        kp = abs(kp);

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kp;
        Zeq = Zp + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/ks;
        Zeq = Zs + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_Y_matrix.add_entry(s,s,yss);

        network_BP_matrix.add_entry(s,s,0.0);

        ks = abs(ks);

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/ks;
        Zeq = Zs + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_BQ_matrix.add_entry(s,s,yss);
        return;
    }
}

void Y_MATRIX_BUILDER::add_three_winding_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding three-winding transformer '%s' between bus %u, %u, and %u to network Y matrix.", trans.get_identifier().c_str(),
                  trans.get_primary_winding_bus(), trans.get_secondary_winding_bus(), trans.get_tertiary_winding_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t primary_bus = trans.get_primary_winding_bus();
    size_t secondary_bus = trans.get_secondary_winding_bus();
    size_t tertiary_bus = trans.get_tertiary_winding_bus();

    size_t p = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
    size_t s = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);
    size_t t = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(tertiary_bus);

    complex<double> Zps = trans.get_leakage_impedance_between_primary_and_secondary_windings_based_on_system_base_power_in_pu();
    complex<double> Zst = trans.get_leakage_impedance_between_secondary_and_tertiary_windings_based_on_system_base_power_in_pu();
    complex<double> Zpt = trans.get_leakage_impedance_between_primary_and_tertiary_windings_based_on_system_base_power_in_pu();
    complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

    complex<double> Zp = 0.5*(Zps+Zpt-Zst),
                    Zs = 0.5*(Zps+Zst-Zpt),
                    Zt = 0.5*(Zpt+Zst-Zps);

    double tap_primary = trans.get_primary_winding_off_nominal_turn_ratio_in_pu();
    double angle_primary = trans.get_primary_winding_angle_shift_in_deg();
    angle_primary = deg2rad(angle_primary);
    double tap_secondary = trans.get_secondary_winding_off_nominal_turn_ratio_in_pu();
    double angle_secondary = trans.get_secondary_winding_angle_shift_in_deg();
    angle_secondary = deg2rad(angle_secondary);
    double tap_tertiary = trans.get_tertiary_winding_off_nominal_turn_ratio_in_pu();
    double angle_tertiary = trans.get_tertiary_winding_angle_shift_in_deg();
    angle_tertiary = deg2rad(angle_tertiary);

    complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                    ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary)),
                    kt(tap_tertiary*cos(angle_tertiary),tap_tertiary*sin(angle_tertiary));

    complex<double> V, I, Vstar;
    complex<double> ypp, yps, ypt, ysp, yss, yst, ytp, yts, ytt, Yeq, Zeq;

    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        // voltage at primary side.
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        // voltage at secondary side
        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp + 1.0/Zt;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        // voltage at tertiary side
        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_Y_matrix.add_entry(p,p,ypp);
        network_Y_matrix.add_entry(p,s,yps);
        network_Y_matrix.add_entry(p,t,ypt);
        network_Y_matrix.add_entry(s,p,ysp);
        network_Y_matrix.add_entry(s,s,yss);
        network_Y_matrix.add_entry(s,t,yst);
        network_Y_matrix.add_entry(t,p,ytp);
        network_Y_matrix.add_entry(t,s,yts);
        network_Y_matrix.add_entry(t,t,ytt);
        return;
    }


    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_Y_matrix.add_entry(p,p,ypp);
        network_Y_matrix.add_entry(p,s,yps);
        network_Y_matrix.add_entry(s,p,ysp);
        network_Y_matrix.add_entry(s,s,yss);
        return;
    }


    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        network_Y_matrix.add_entry(p,p,ypp);
        network_Y_matrix.add_entry(p,t,ypt);
        network_Y_matrix.add_entry(t,p,ytp);
        network_Y_matrix.add_entry(t,t,ytt);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_Y_matrix.add_entry(s,s,yss);
        network_Y_matrix.add_entry(s,t,yst);
        network_Y_matrix.add_entry(t,s,yts);
        network_Y_matrix.add_entry(t,t,ytt);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        V = 1.0;
        V = V/kp;
        Zeq = Zp + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        network_Y_matrix.add_entry(p,p,ypp);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        V = 1.0;
        V = V/ks;
        Zeq = Zs + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_Y_matrix.add_entry(s,s,yss);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/kt;
        Zeq = Zt + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        network_Y_matrix.add_entry(t,t,ytt);
        return;
    }
}

void Y_MATRIX_BUILDER::add_two_winding_transformer_to_network_v2(const TRANSFORMER& trans)
{
    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding two-winding transformer '%s' between bus %u and %u to network Y matrix.", trans.get_identifier().c_str(),
                  trans.get_primary_winding_bus(), trans.get_secondary_winding_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t primary_bus = trans.get_primary_winding_bus();
    size_t secondary_bus = trans.get_secondary_winding_bus();

    size_t p = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
    size_t s = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

    complex<double> Zps = trans.get_leakage_impedance_between_primary_and_secondary_windings_based_on_system_base_power_in_pu();
    complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

    complex<double> Zp, Zs;
    Zp = 0.5*Zps; Zs = Zp;

    double tap_primary = trans.get_primary_winding_off_nominal_turn_ratio_in_pu();
    double angle_primary = trans.get_primary_winding_angle_shift_in_deg();
    angle_primary = deg2rad(angle_primary);
    double tap_secondary = trans.get_secondary_winding_off_nominal_turn_ratio_in_pu();
    double angle_secondary = trans.get_secondary_winding_angle_shift_in_deg();
    angle_secondary = deg2rad(angle_secondary);

    complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                    ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


    complex<double> V, I, Vstar;
    complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

    complex<double> kp_store, ks_store, Ym_store;
    kp_store = kp;
    ks_store = ks;
    Ym_store = Ym;

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==true)
    {
        Ym = 0.0;
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_Y_matrix.add_entry(p,p,ypp);
        network_Y_matrix.add_entry(p,s,yps);
        network_Y_matrix.add_entry(s,p,ysp);
        network_Y_matrix.add_entry(s,s,yss);

        Ym = Ym_store;
        network_Y_matrix.add_entry(p,p,Ym);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==false)
    {
        network_Y_matrix.add_entry(p,p,Ym);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==true)
    {
        V = 1.0;
        V = V/ks;
        Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();
        Zeq = Zps + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_Y_matrix.add_entry(s,s,yss);
        return;
    }
}


void Y_MATRIX_BUILDER::add_fixed_shunts_to_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<FIXED_SHUNT*> shunts = db->get_all_fixed_shunts();

    size_t n = shunts.size();

    for(size_t i = 0; i<n; i++)
        add_fixed_shunt_to_network(*(shunts[i]));
}

void Y_MATRIX_BUILDER::add_fixed_shunt_to_network(const FIXED_SHUNT& shunt)
{
    if(shunt.get_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding fixed shunt '%s' at bus %u to network Y matrix.", shunt.get_identifier().c_str(),
                  shunt.get_shunt_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t bus = shunt.get_shunt_bus();

    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(bus);

    complex<double> Yshunt = shunt.get_nominal_admittance_shunt_in_pu();

    network_Y_matrix.add_entry(i,i, Yshunt);
    return;
}


void Y_MATRIX_BUILDER::build_decoupled_network_matrix()
{
    network_BP_matrix.clear();
    network_BQ_matrix.clear();

    add_lines_to_decoupled_network();
    add_transformers_to_decoupled_network();
    add_fixed_shunts_to_decoupled_network();

    network_BP_matrix.compress_and_merge_duplicate_entries();
    network_BQ_matrix.compress_and_merge_duplicate_entries();
}

SPARSE_MATRIX Y_MATRIX_BUILDER::get_decoupled_network_BP_matrix()
{
    if(network_BP_matrix.matrix_in_triplet_form())
        build_decoupled_network_matrix();

    return network_BP_matrix;
}

SPARSE_MATRIX Y_MATRIX_BUILDER::get_decoupled_network_BQ_matrix()
{
    if(network_BP_matrix.matrix_in_triplet_form())
        build_decoupled_network_matrix();

    return network_BQ_matrix;
}

void Y_MATRIX_BUILDER::add_lines_to_decoupled_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<LINE*> lines = db->get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i<n; i++)
        add_line_to_decoupled_network(*(lines[i]));
}

void Y_MATRIX_BUILDER::add_line_to_decoupled_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding line '%s' between bus %u and %u to network Y matrix.", line.get_identifier().c_str(),
                  line.get_sending_side_bus(), line.get_receiving_side_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t sending_bus = line.get_sending_side_bus();
    size_t receiving_bus = line.get_receiving_side_bus();

    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
    size_t j = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

    complex<double> Zline = line.get_line_positive_sequence_z_in_pu();
    complex<double> Yline = line.get_line_positive_sequence_y_in_pu();
    complex<double> Yshunt_sending = line.get_shunt_positive_sequence_y_at_sending_side_in_pu();
    complex<double> Yshunt_receiving = line.get_shunt_positive_sequence_y_at_receiving_side_in_pu();

    complex<double> Y, Z;
    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==true)
    {
        Y = 1.0/Zline;
        network_BP_matrix.add_entry(i,j, -Y);
        network_BP_matrix.add_entry(j,i, -Y);
        network_BP_matrix.add_entry(i,i, Y);
        network_BP_matrix.add_entry(j,j, Y);

        Zline = complex<double>(0.0, Zline.imag());
        Y = 1.0/Zline;
        Yline = complex<double>(0.0, Yline.imag());
        Yshunt_sending = complex<double>(0.0, Yshunt_sending.imag());
        Yshunt_receiving = complex<double>(0.0, Yshunt_receiving.imag());

        network_BQ_matrix.add_entry(i,j, -Y);
        network_BQ_matrix.add_entry(j,i, -Y);
        network_BQ_matrix.add_entry(i,i, Y+0.5*Yline+Yshunt_sending);
        network_BQ_matrix.add_entry(j,j, Y+0.5*Yline+Yshunt_receiving);

        return;
    }

    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==false)
    {
        network_BP_matrix.add_entry(i,i, 0.0);

        Zline = complex<double>(0.0, Zline.imag());
        Yline = complex<double>(0.0, Yline.imag());
        Yshunt_sending = complex<double>(0.0, Yshunt_sending.imag());
        Yshunt_receiving = complex<double>(0.0, Yshunt_receiving.imag());

        Y = 0.5*Yline+Yshunt_receiving;
        Z = Zline + 1.0/Y;
        Y = 1.0/Z + 0.5*Yline+Yshunt_sending;

        network_BQ_matrix.add_entry(i,i, Y);
        return;
    }

    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==true)
    {
        network_BP_matrix.add_entry(j,j, 0.0);

        Zline = complex<double>(0.0, Zline.imag());
        Yline = complex<double>(0.0, Yline.imag());
        Yshunt_sending = complex<double>(0.0, Yshunt_sending.imag());
        Yshunt_receiving = complex<double>(0.0, Yshunt_receiving.imag());

        Y = 0.5*Yline+Yshunt_sending;
        Z = Zline + 1.0/Y;
        Y = 1.0/Z + 0.5*Yline+Yshunt_receiving;
        network_Y_matrix.add_entry(j,j, Y);

        network_BQ_matrix.add_entry(j,j, Y);
        return;
    }
}


void Y_MATRIX_BUILDER::add_transformers_to_decoupled_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<TRANSFORMER*> transformers = db->get_all_transformers();

    size_t n = transformers.size();

    for(size_t i = 0; i<n; i++)
        add_transformer_to_decoupled_network(*(transformers[i]));
}

void Y_MATRIX_BUILDER::add_transformer_to_decoupled_network(const TRANSFORMER& trans)
{
    if(trans.is_two_winding_transformer())
        add_two_winding_transformer_to_decoupled_network_v2(trans);
    else
        add_three_winding_transformer_to_decoupled_network(trans);
}

void Y_MATRIX_BUILDER::add_three_winding_transformer_to_decoupled_network(const TRANSFORMER& trans)
{
    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding three-winding transformer '%s' between bus %u, %u, and %u to network Y matrix.", trans.get_identifier().c_str(),
                  trans.get_primary_winding_bus(), trans.get_secondary_winding_bus(), trans.get_tertiary_winding_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t primary_bus = trans.get_primary_winding_bus();
    size_t secondary_bus = trans.get_secondary_winding_bus();
    size_t tertiary_bus = trans.get_tertiary_winding_bus();

    size_t p = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
    size_t s = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);
    size_t t = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(tertiary_bus);

    complex<double> Zps = trans.get_leakage_impedance_between_primary_and_secondary_windings_based_on_system_base_power_in_pu();
    complex<double> Zst = trans.get_leakage_impedance_between_secondary_and_tertiary_windings_based_on_system_base_power_in_pu();
    complex<double> Zpt = trans.get_leakage_impedance_between_primary_and_tertiary_windings_based_on_system_base_power_in_pu();
    complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

    complex<double> Zp = 0.5*(Zps+Zpt-Zst),
                    Zs = 0.5*(Zps+Zst-Zpt),
                    Zt = 0.5*(Zpt+Zst-Zps);

    double tap_primary = trans.get_primary_winding_off_nominal_turn_ratio_in_pu();
    double angle_primary = trans.get_primary_winding_angle_shift_in_deg();
    angle_primary = deg2rad(angle_primary);
    double tap_secondary = trans.get_secondary_winding_off_nominal_turn_ratio_in_pu();
    double angle_secondary = trans.get_secondary_winding_angle_shift_in_deg();
    angle_secondary = deg2rad(angle_secondary);
    double tap_tertiary = trans.get_tertiary_winding_off_nominal_turn_ratio_in_pu();
    double angle_tertiary = trans.get_tertiary_winding_angle_shift_in_deg();
    angle_tertiary = deg2rad(angle_tertiary);

    complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                    ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary)),
                    kt(tap_tertiary*cos(angle_tertiary),tap_tertiary*sin(angle_tertiary));

    complex<double> V, I, Vstar;
    complex<double> ypp, yps, ypt, ysp, yss, yst, ytp, yts, ytt, Yeq, Zeq;


    complex<double> kp_store = kp, ks_store = ks, kt_store = kt;
    complex<double> Ym_store = Ym;

    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        // voltage at primary side.
        kp = kp/abs(kp);
        ks = ks/abs(ks);
        kt = kt/abs(kt);

        Ym = 0.0;

        // voltage at primary side.
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        // voltage at secondary side
        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp + 1.0/Zt;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        // voltage at tertiary side
        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_BP_matrix.add_entry(p,p,ypp);
        network_BP_matrix.add_entry(p,s,yps);
        network_BP_matrix.add_entry(p,t,ypt);
        network_BP_matrix.add_entry(s,p,ysp);
        network_BP_matrix.add_entry(s,s,yss);
        network_BP_matrix.add_entry(s,t,yst);
        network_BP_matrix.add_entry(t,p,ytp);
        network_BP_matrix.add_entry(t,s,yts);
        network_BP_matrix.add_entry(t,t,ytt);

        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        // voltage at primary side.
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        // voltage at secondary side
        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp + 1.0/Zt;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        // voltage at tertiary side
        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        network_BQ_matrix.add_entry(p,s,yps);
        network_BQ_matrix.add_entry(p,t,ypt);
        network_BQ_matrix.add_entry(s,p,ysp);
        network_BQ_matrix.add_entry(s,s,yss);
        network_BQ_matrix.add_entry(s,t,yst);
        network_BQ_matrix.add_entry(t,p,ytp);
        network_BQ_matrix.add_entry(t,s,yts);
        network_BQ_matrix.add_entry(t,t,ytt);

        return;
    }


    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        kp = kp/abs(kp);
        ks = ks/abs(ks);
        kt = kt/abs(kt);

        Ym = 0.0;

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BP_matrix.add_entry(p,p,ypp);
        network_BP_matrix.add_entry(p,s,yps);
        network_BP_matrix.add_entry(s,p,ysp);
        network_BP_matrix.add_entry(s,s,yss);


        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        network_BQ_matrix.add_entry(p,s,yps);
        network_BQ_matrix.add_entry(s,p,ysp);
        network_BQ_matrix.add_entry(s,s,yss);
        return;
    }


    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        kp = kp/abs(kp);
        ks = ks/abs(ks);
        kt = kt/abs(kt);

        Ym = 0.0;

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        network_BP_matrix.add_entry(p,p,ypp);
        network_BP_matrix.add_entry(p,t,ypt);
        network_BP_matrix.add_entry(t,p,ytp);
        network_BP_matrix.add_entry(t,t,ytt);


        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        ytp = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        ypt = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        network_BQ_matrix.add_entry(p,t,ypt);
        network_BQ_matrix.add_entry(t,p,ytp);
        network_BQ_matrix.add_entry(t,t,ytt);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        kp = kp/abs(kp);
        ks = ks/abs(ks);
        kt = kt/abs(kt);

        Ym = 0.0;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_BP_matrix.add_entry(s,s,yss);
        network_BP_matrix.add_entry(s,t,yst);
        network_BP_matrix.add_entry(t,s,yts);
        network_BP_matrix.add_entry(t,t,ytt);


        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zt;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zt;
        I /= conj(kt);
        yts = I;

        V = 1.0;
        V = V/kt;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zt + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        yst = I;

        network_BQ_matrix.add_entry(s,s,yss);
        network_BQ_matrix.add_entry(s,t,yst);
        network_BQ_matrix.add_entry(t,s,yts);
        network_BQ_matrix.add_entry(t,t,ytt);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==true and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        network_BP_matrix.add_entry(p,p,0.0);

        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kp;
        Zeq = Zp + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        network_BQ_matrix.add_entry(p,p,ypp);


        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==true and
       trans.get_tertiary_winding_breaker_status()==false)
    {
        network_BP_matrix.add_entry(s,s,0.0);


        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/ks;
        Zeq = Zs + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_BQ_matrix.add_entry(s,s,yss);

        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and
       trans.get_secondary_winding_breaker_status()==false and
       trans.get_tertiary_winding_breaker_status()==true)
    {
        network_BP_matrix.add_entry(t,t,0.0);


        kp = abs(kp_store);
        ks = abs(ks_store);
        kt = abs(kt_store);

        Ym = Ym_store;

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());
        Zt = complex<double>(0.0, Zt.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/kt;
        Zeq = Zt + 1.0/Ym;
        I = V/Zeq;
        I /= conj(kt);
        ytt = I;

        network_BQ_matrix.add_entry(t,t,ytt);
        return;
    }
}

void Y_MATRIX_BUILDER::add_two_winding_transformer_to_decoupled_network_v2(const TRANSFORMER& trans)
{
    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding two-winding transformer '%s' between bus %u and %u to network Y matrix.", trans.get_identifier().c_str(),
                  trans.get_primary_winding_bus(), trans.get_secondary_winding_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t primary_bus = trans.get_primary_winding_bus();
    size_t secondary_bus = trans.get_secondary_winding_bus();

    size_t p = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
    size_t s = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

    complex<double> Zps = trans.get_leakage_impedance_between_primary_and_secondary_windings_based_on_system_base_power_in_pu();
    complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

    complex<double> Zp, Zs;
    Zp = 0.5*Zps; Zs = Zp;

    double tap_primary = trans.get_primary_winding_off_nominal_turn_ratio_in_pu();
    double angle_primary = trans.get_primary_winding_angle_shift_in_deg();
    angle_primary = deg2rad(angle_primary);
    double tap_secondary = trans.get_secondary_winding_off_nominal_turn_ratio_in_pu();
    double angle_secondary = trans.get_secondary_winding_angle_shift_in_deg();
    angle_secondary = deg2rad(angle_secondary);

    complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                    ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


    complex<double> V, I, Vstar;
    complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

    complex<double> kp_store, ks_store, Ym_store;
    kp_store = kp;
    ks_store = ks;
    Ym_store = Ym;

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==true)
    {
        kp = kp/abs(kp);
        ks = ks/abs(ks);
        Ym = 0.0;

        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BP_matrix.add_entry(p,p,ypp);
        network_BP_matrix.add_entry(p,s,yps);
        network_BP_matrix.add_entry(s,p,ysp);
        network_BP_matrix.add_entry(s,s,yss);


        kp = kp_store;
        ks = ks_store;

        kp = abs(kp);
        ks = abs(ks);

        Zp = complex<double>(0.0, Zp.imag());
        Zs = complex<double>(0.0, Zs.imag());

        Ym = 0.0;
        V = 1.0;
        V = V/kp;
        Yeq = Ym + 1.0/Zs;
        Zeq = Zp + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(kp);
        ypp = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zs;
        I /= conj(ks);
        ysp = I;

        V = 1.0;
        V = V/ks;
        Yeq = Ym + 1.0/Zp;
        Zeq = Zs + 1.0/Yeq;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        Vstar = V*(1.0/Yeq)/Zeq;
        I = -Vstar/Zp;
        I /= conj(kp);
        yps = I;

        network_BQ_matrix.add_entry(p,p,ypp);
        network_BQ_matrix.add_entry(p,s,yps);
        network_BQ_matrix.add_entry(s,p,ysp);
        network_BQ_matrix.add_entry(s,s,yss);

        Ym = Ym_store;
        Ym = complex<double>(0.0, Ym.imag());
        network_BQ_matrix.add_entry(p,p,Ym);

        return;
    }

    if(trans.get_primary_winding_breaker_status()==true and trans.get_secondary_winding_breaker_status()==false)
    {
        network_BP_matrix.add_entry(p,p,0.0);

        Ym = complex<double>(0.0, Ym.imag());

        network_BQ_matrix.add_entry(p,p,Ym);
        return;
    }

    if(trans.get_primary_winding_breaker_status()==false and trans.get_secondary_winding_breaker_status()==true)
    {
        network_BP_matrix.add_entry(s,s,0.0);

        ks = abs(ks);

        Zps = complex<double>(0.0, Zps.imag());
        Ym = complex<double>(0.0, Ym.imag());

        V = 1.0;
        V = V/ks;
        Zeq = Zps + 1.0/Ym;
        I = V/Zeq;
        I /= conj(ks);
        yss = I;

        network_BQ_matrix.add_entry(s,s,yss);
        return;
    }
}

void Y_MATRIX_BUILDER::add_fixed_shunts_to_decoupled_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<FIXED_SHUNT*> shunts = db->get_all_fixed_shunts();

    size_t n = shunts.size();

    for(size_t i = 0; i<n; i++)
        add_fixed_shunt_to_decoupled_network(*(shunts[i]));
}


void Y_MATRIX_BUILDER::add_fixed_shunt_to_decoupled_network(const FIXED_SHUNT& shunt)
{
    if(shunt.get_status()==false)
        return;

    /*char info[256];
    sprintf(info,"Adding fixed shunt '%s' at bus %u to network Y matrix.", shunt.get_identifier().c_str(),
                  shunt.get_shunt_bus());
    show_information_with_leading_time_stamp(info);
    */

    size_t bus = shunt.get_shunt_bus();

    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(bus);

    complex<double> Yshunt = shunt.get_nominal_admittance_shunt_in_pu();

    network_BP_matrix.add_entry(i,i, 0.0);
    network_BQ_matrix.add_entry(i,i, Yshunt);
    return;
}


SPARSE_MATRIX Y_MATRIX_BUILDER::build_dynamic_network_matrix()
{
    network_Y_matrix.clear();

    add_lines_to_dynamic_network();
    add_transformers_to_network();
    add_fixed_shunts_to_network();
    add_generators_to_dynamic_network();

    network_Y_matrix.compress_and_merge_duplicate_entries();

    return network_Y_matrix;
}

void Y_MATRIX_BUILDER::add_lines_to_dynamic_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<LINE*> lines = db->get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i<n; i++)
    {
        if(not lines[i]->is_faulted())
           add_line_to_network(*(lines[i]));
        else
           add_faulted_line_to_dynamic_network(*(lines[i]));
    }
}

void Y_MATRIX_BUILDER::add_faulted_line_to_dynamic_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==false)
        return;

    if(not line.is_faulted())
    {
        add_line_to_network(line);
        return;
    }

    /*char info[256];
    sprintf(info,"Adding line '%s' between bus %u and %u to network Y matrix.", line.get_identifier().c_str(),
                  line.get_sending_side_bus(), line.get_receiving_side_bus());
    show_information_with_leading_time_stamp(info);*/

    size_t sending_bus = line.get_sending_side_bus();
    size_t receiving_bus = line.get_receiving_side_bus();

    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
    size_t j = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

    complex<double> Zline = line.get_line_positive_sequence_z_in_pu();
    complex<double> Yline = line.get_line_positive_sequence_y_in_pu();
    complex<double> Yshunt_sending = line.get_shunt_positive_sequence_y_at_sending_side_in_pu();
    complex<double> Yshunt_receiving = line.get_shunt_positive_sequence_y_at_receiving_side_in_pu();

    size_t nfault = line.get_fault_count();
    // take care of fault at two ends
    FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), 0.0);
    Yshunt_sending += fault.get_fault_shunt_in_pu();

    fault = line.get_fault_at_location(line.get_sending_side_bus(), 1.0);
    Yshunt_receiving += fault.get_fault_shunt_in_pu();

    vector<SUBLINE> sublines;

    double starting_fault_location;
    size_t n;

    for(size_t i=0; i<nfault; i++)
    {
        double this_fault_location = line.get_fault_location_of_fault(i);
        if(this_fault_location == 0.0 or this_fault_location == 1.0)
            continue;

        n = sublines.size();
        if(n==0)
            starting_fault_location = 0.0;
        else
            starting_fault_location = sublines[n-1].to_loc;

        SUBLINE sl;
        sl.from_loc = starting_fault_location;
        sl.to_loc = this_fault_location;
        sublines.push_back(sl);
    }
    n = sublines.size();
    if(n==0)
        starting_fault_location = 0.0;
    else
        starting_fault_location = sublines[n-1].to_loc;

    SUBLINE sl;
    sl.from_loc = starting_fault_location;
    sl.to_loc = 1.0;
    sublines.push_back(sl);

    complex<double> V, Y, Z, y, z, I;
    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==true)
    {
        V = 1.0; // source at sending side, receiving side shorted
        Y = 0.0; Z = 0;
        n = sublines.size();
        double line_length = sublines[n-1].to_loc - sublines[n-1].from_loc;
        y = Yline*line_length; z = Zline*line_length;
        Z = z; Y = 1.0/Z+y*0.5;
        for(int i=n-2; i>=0; i--)
        {
            double fault_location = sublines[i].to_loc;
            double line_length = sublines[i].to_loc - sublines[i].from_loc;
            FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
            complex<double> yfault = fault.get_fault_shunt_in_pu();
            y = Yline*line_length; z = Zline*line_length;
            Y = Y+y*0.5+yfault; Z = z+1.0/Y;
            Y = 1.0/Z+y*0.5;
        }
        Y = Y+Yshunt_sending;
        I = V*Y;
        network_Y_matrix.add_entry(i,i, I);
        Y = Yshunt_sending;
        I = I-V*Y;
        for(size_t i=0; i<n; i++)
        {
            double fault_location = sublines[i].to_loc;
            double line_length = sublines[i].to_loc - sublines[i].from_loc;
            y = Yline*line_length; z = Zline*line_length;
            I = I-V*y*0.5;
            V = V-I*z;
            if(fault_location!=1.0)
            {
                I = I - V*y*0.5;
                FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
                complex<double> yfault = fault.get_fault_shunt_in_pu();
                I = I - V*yfault;
            }
        }
        network_Y_matrix.add_entry(j,i, -I);
        network_Y_matrix.add_entry(i,j, -I);

        V = 1.0; // source at receiving side, sending side shorted

        n = sublines.size();
        line_length = sublines[0].to_loc - sublines[0].from_loc;
        y = Yline*line_length; z = Zline*line_length;
        Z = z; Y = 1.0/Z+y*0.5;
        for(size_t i=1; i<n; i++)
        {
            double fault_location = sublines[i].from_loc;
            double line_length = sublines[i].to_loc - sublines[i].from_loc;
            y = Yline*line_length; z = Zline*line_length;
            FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
            complex<double> yfault = fault.get_fault_shunt_in_pu();
            Y = Y+y*0.5+yfault; Z = z+1.0/Y;
            Y = 1.0/Z+y*0.5;
        }
        Y = Y+Yshunt_receiving;
        I = V*Y;
        network_Y_matrix.add_entry(j,j, I);
        return;
    }

    if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==false)
    {
        V = 1.0; // source at sending side
        Y = Yshunt_receiving; Z = 0;
        n = sublines.size();
        for(int i=n-1; i>=0; i--)
        {
            double fault_location = sublines[i].from_loc;
            double line_length = sublines[i].to_loc - sublines[i].from_loc;
            complex<double> yfault = 0.0;
            if(fault_location!=0.0)
            {
                FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
                yfault = fault.get_fault_shunt_in_pu();
            }
            y = Yline*line_length; z = Zline*line_length;
            Y = Y+y*0.5; Z = z+1.0/Y;
            Y = 1.0/Z+y*0.5+yfault;
        }
        Y = Y+Yshunt_sending;
        I = V*Y;
        network_Y_matrix.add_entry(i,i, I);
        return;
    }

    if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==true)
    {
        V = 1.0; // source at receiving side
        Y = Yshunt_sending; Z = 0;
        n = sublines.size();
        for(int i=0; i<int(n); i++)
        {
            double fault_location = sublines[i].to_loc;
            double line_length = sublines[i].to_loc - sublines[i].from_loc;
            complex<double> yfault = 0.0;
            if(fault_location!=1.0)
            {
                FAULT fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
                yfault = fault.get_fault_shunt_in_pu();
            }
            y = Yline*line_length; z = Zline*line_length;
            Y = Y+y*0.5; Z = z+1.0/Y;
            Y = 1.0/Z+y*0.5+yfault;
        }
        Y = Y+Yshunt_receiving;
        I = V*Y;
        network_Y_matrix.add_entry(j,j, I);
        return;
    }

}

void Y_MATRIX_BUILDER::add_generators_to_dynamic_network()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<GENERATOR*> generators = db->get_all_generators();

    size_t n= generators.size();

    for(size_t i=0; i<n; i++)
        add_generator_to_dynamic_network(*(generators[i]));
}

void Y_MATRIX_BUILDER::add_generator_to_dynamic_network(const GENERATOR& gen)
{
    if(gen.get_status()==false)
        return;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    complex<double> Z = gen.get_source_impedance_in_pu();
    double mbase = gen.get_mbase_in_MVA();
    double mvabase = db->get_system_base_power_in_MVA();
    Z = Z/mbase*mvabase;

    size_t bus = gen.get_generator_bus();
    size_t i = STEPS::inphno.get_internal_bus_number_of_physical_bus_number(bus);
    network_Y_matrix.add_entry(i,i,1.0/Z);
}

