#include "header/network/network_matrix.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <fstream>
#include <iostream>
#include <set>
using namespace std;

class SUBLINE
{
    public:
        SUBLINE()
        {
            from_loc = 0.0;
            to_loc = 1.0;
        }
        double from_loc;
        double to_loc;
};


NETWORK_MATRIX::NETWORK_MATRIX()
{
    clear();
}


NETWORK_MATRIX::~NETWORK_MATRIX()
{
}

void NETWORK_MATRIX::clear()
{
    network_Y_matrix.clear();
    network_BP_matrix.clear();
    network_BQ_matrix.clear();
    network_DC_B_matrix.clear();
    inphno.clear();
}

void NETWORK_MATRIX::build_network_matrix()
{
    if(inphno.empty())
        initialize_physical_internal_bus_pair();

    network_Y_matrix.clear();

    add_lines_to_network();
    add_transformers_to_network();
    add_fixed_shunts_to_network();

    network_Y_matrix.compress_and_merge_duplicate_entries();
}

STEPS_SPARSE_MATRIX& NETWORK_MATRIX::get_network_matrix()
{
    if(network_Y_matrix.matrix_in_triplet_form())
        build_network_matrix();

    return network_Y_matrix;
}

void NETWORK_MATRIX::build_decoupled_network_matrix()
{
    if(inphno.empty())
        initialize_physical_internal_bus_pair();

    network_BP_matrix.clear();
    network_BQ_matrix.clear();

    add_lines_to_decoupled_network();
    add_transformers_to_decoupled_network();
    add_fixed_shunts_to_decoupled_network();

    network_BP_matrix.compress_and_merge_duplicate_entries();
    network_BQ_matrix.compress_and_merge_duplicate_entries();

}

STEPS_SPARSE_MATRIX& NETWORK_MATRIX::get_decoupled_network_BP_matrix()
{
    if(network_BP_matrix.matrix_in_triplet_form())
        build_decoupled_network_matrix();

    return network_BP_matrix;
}

STEPS_SPARSE_MATRIX& NETWORK_MATRIX::get_decoupled_network_BQ_matrix()
{
    if(network_BP_matrix.matrix_in_triplet_form())
        build_decoupled_network_matrix();

    return network_BQ_matrix;
}

void NETWORK_MATRIX::build_dc_network_matrix()
{
    if(inphno.empty())
        initialize_physical_internal_bus_pair();

    network_DC_B_matrix.clear();

    add_lines_to_dc_network();
    add_transformers_to_dc_network();

    network_DC_B_matrix.compress_and_merge_duplicate_entries();
}

STEPS_SPARSE_MATRIX& NETWORK_MATRIX::get_dc_network_matrix()
{
    if(network_DC_B_matrix.matrix_in_triplet_form())
        build_dc_network_matrix();

    return network_DC_B_matrix;
}


void NETWORK_MATRIX::build_dynamic_network_matrix()
{
    if(inphno.empty())
        initialize_physical_internal_bus_pair();

    network_Y_matrix.clear();

    add_bus_fault_to_dynamic_network();
    add_lines_to_dynamic_network();
    add_transformers_to_network();
    add_fixed_shunts_to_network();
    add_generators_to_dynamic_network();
    add_wt_generators_to_dynamic_network();

    network_Y_matrix.compress_and_merge_duplicate_entries();
}

STEPS_SPARSE_MATRIX& NETWORK_MATRIX::get_dynamic_network_matrix()
{
    if(network_Y_matrix.matrix_in_triplet_form())
        build_dynamic_network_matrix();

    return network_Y_matrix;
}

void NETWORK_MATRIX::add_lines_to_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i!=n; ++i)
        add_line_to_network(*(lines[i]));
}


void NETWORK_MATRIX::add_line_to_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==true or line.get_receiving_side_breaker_status()==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding line '%s' connecting to bus %u and %u to network Y matrix.", (line.get_identifier()).c_str(),
                      line.get_sending_side_bus(), line.get_receiving_side_bus());
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t sending_bus = line.get_sending_side_bus();
        size_t receiving_bus = line.get_receiving_side_bus();

        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
        size_t j = inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

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
}

void NETWORK_MATRIX::add_transformers_to_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();

    size_t n = transformers.size();

    for(size_t i = 0; i!=n; ++i)
        add_transformer_to_network(*(transformers[i]));
}

void NETWORK_MATRIX::add_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.is_two_winding_transformer())
        add_two_winding_transformer_to_network_v2(trans);
    else
        add_three_winding_transformer_to_network(trans);
}

void NETWORK_MATRIX::add_two_winding_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding two-winding transformer '%s' connecting to bus %u and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

        complex<double> Zp, Zs;
        Zp = 0.5*Zps; Zs = Zp;

        double tap_primary = trans.get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE);
        double angle_primary = trans.get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);
        double tap_secondary = trans.get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE);
        double angle_secondary = trans.get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);

        complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                        ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


        complex<double> V, I, Vstar;
        complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

        complex<double> kp_store, ks_store, Ym_store;
        kp_store = kp;
        ks_store = ks;
        Ym_store = Ym;

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
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


            kp = kp/ steps_fast_complex_abs(kp);
            ks = ks/ steps_fast_complex_abs(ks);
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

            kp = steps_fast_complex_abs(kp);
            ks = steps_fast_complex_abs(ks);
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==false)
        {
            V = 1.0;
            V = V/kp;
            Zeq = Zp + 1.0/Ym;
            I = V/Zeq;
            I /= conj(kp);
            ypp = I;

            network_Y_matrix.add_entry(p,p,ypp);

            network_BP_matrix.add_entry(p,p,0.0);

            kp = steps_fast_complex_abs(kp);

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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
        {
            V = 1.0;
            V = V/ks;
            Zeq = Zs + 1.0/Ym;
            I = V/Zeq;
            I /= conj(ks);
            yss = I;

            network_Y_matrix.add_entry(s,s,yss);

            network_BP_matrix.add_entry(s,s,0.0);

            ks = steps_fast_complex_abs(ks);

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
}

void NETWORK_MATRIX::add_three_winding_transformer_to_network(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or
       trans.get_winding_breaker_status(SECONDARY_SIDE)==true or
       trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding three-winding transformer '%s' connecting to bus %u, %u, and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE), trans.get_winding_bus(TERTIARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);
        size_t tertiary_bus = trans.get_winding_bus(TERTIARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);
        size_t t = inphno.get_internal_bus_number_of_physical_bus_number(tertiary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);
        complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

        complex<double> Zp = 0.5*(Zps+Zpt-Zst),
                        Zs = 0.5*(Zps+Zst-Zpt),
                        Zt = 0.5*(Zpt+Zst-Zps);

        double tap_primary = trans.get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE);
        double angle_primary = trans.get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);
        double tap_secondary = trans.get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE);
        double angle_secondary = trans.get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);
        double tap_tertiary = trans.get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE);
        double angle_tertiary = trans.get_winding_angle_shift_in_deg(TERTIARY_SIDE);
        angle_tertiary = deg2rad(angle_tertiary);

        complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                        ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary)),
                        kt(tap_tertiary*cos(angle_tertiary),tap_tertiary*sin(angle_tertiary));

        complex<double> V, I, Vstar;
        complex<double> ypp, yps, ypt, ysp, yss, yst, ytp, yts, ytt, Yeq, Zeq;

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
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


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
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


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
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
}

void NETWORK_MATRIX::add_two_winding_transformer_to_network_v2(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding two-winding transformer '%s' connecting to bus %u and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

        complex<double> Zp, Zs;
        Zp = 0.5*Zps; Zs = Zp;

        double tap_primary = trans.get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE);
        double angle_primary = trans.get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);
        double tap_secondary = trans.get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE);
        double angle_secondary = trans.get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);

        complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                        ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


        complex<double> V, I, Vstar;
        complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

        complex<double> kp_store, ks_store, Ym_store;
        kp_store = kp;
        ks_store = ks;
        Ym_store = Ym;

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==false)
        {
            network_Y_matrix.add_entry(p,p,Ym);
            return;
        }

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
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
}


void NETWORK_MATRIX::add_fixed_shunts_to_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<FIXED_SHUNT*> shunts = psdb.get_all_fixed_shunts();

    size_t n = shunts.size();

    for(size_t i = 0; i!=n; ++i)
        add_fixed_shunt_to_network(*(shunts[i]));
}

void NETWORK_MATRIX::add_fixed_shunt_to_network(const FIXED_SHUNT& shunt)
{
    if(shunt.get_status()==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding fixed shunt '%s' at bus %u to network Y matrix.", (shunt.get_identifier()).c_str(),
                      shunt.get_shunt_bus());
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t bus = shunt.get_shunt_bus();

        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(bus);

        complex<double> Yshunt = shunt.get_nominal_admittance_shunt_in_pu();

        network_Y_matrix.add_entry(i,i, Yshunt);
    }
}

void NETWORK_MATRIX::add_lines_to_decoupled_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i!=n; ++i)
    {
        add_line_to_decoupled_network(*(lines[i]));
    }
}

void NETWORK_MATRIX::add_line_to_decoupled_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==true or line.get_receiving_side_breaker_status()==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding line '%s' connecting to bus %u and %u to network Y matrix.", (line.get_identifier()).c_str(),
                      line.get_sending_side_bus(), line.get_receiving_side_bus());
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t sending_bus = line.get_sending_side_bus();
        size_t receiving_bus = line.get_receiving_side_bus();

        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
        size_t j = inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

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
}


void NETWORK_MATRIX::add_transformers_to_decoupled_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();

    size_t n = transformers.size();

    for(size_t i = 0; i!=n; ++i)
    {
        add_transformer_to_decoupled_network(*(transformers[i]));
    }
}

void NETWORK_MATRIX::add_transformer_to_decoupled_network(const TRANSFORMER& trans)
{
    if(trans.is_two_winding_transformer())
        add_two_winding_transformer_to_decoupled_network_v2(trans);
    else
        add_three_winding_transformer_to_decoupled_network(trans);
}

void NETWORK_MATRIX::add_three_winding_transformer_to_decoupled_network(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or
       trans.get_winding_breaker_status(SECONDARY_SIDE)==true or
       trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding three-winding transformer '%s' connecting to bus %u, %u, and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE), trans.get_winding_bus(TERTIARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);
        size_t tertiary_bus = trans.get_winding_bus(TERTIARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);
        size_t t = inphno.get_internal_bus_number_of_physical_bus_number(tertiary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);
        complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

        complex<double> Zp = 0.5*(Zps+Zpt-Zst),
                        Zs = 0.5*(Zps+Zst-Zpt),
                        Zt = 0.5*(Zpt+Zst-Zps);

        double tap_primary = trans.get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE);
        double angle_primary = trans.get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);
        double tap_secondary = trans.get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE);
        double angle_secondary = trans.get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);
        double tap_tertiary = trans.get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE);
        double angle_tertiary = trans.get_winding_angle_shift_in_deg(TERTIARY_SIDE);
        angle_tertiary = deg2rad(angle_tertiary);

        complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                        ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary)),
                        kt(tap_tertiary*cos(angle_tertiary),tap_tertiary*sin(angle_tertiary));

        complex<double> V, I, Vstar;
        complex<double> ypp, yps, ypt, ysp, yss, yst, ytp, yts, ytt, Yeq, Zeq;


        complex<double> kp_store = kp, ks_store = ks, kt_store = kt;
        complex<double> Ym_store = Ym;

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            // voltage at primary side.
            kp = kp/ steps_fast_complex_abs(kp);
            ks = ks/ steps_fast_complex_abs(ks);
            kt = kt/ steps_fast_complex_abs(kt);

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

            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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

            if(isnan(ypp.real()) or isnan(ypp.imag()) or
               isnan(yps.real()) or isnan(yps.imag()) or
               isnan(ypt.real()) or isnan(ypt.imag()) or
               isnan(ysp.real()) or isnan(ysp.imag()) or
               isnan(yss.real()) or isnan(yss.imag()) or
               isnan(yst.real()) or isnan(yst.imag()) or
               isnan(ytp.real()) or isnan(ytp.imag()) or
               isnan(yts.real()) or isnan(yts.imag()) or
               isnan(ytt.real()) or isnan(ytt.imag()) )
                cout<<"NAN is detected when adding "<<trans.get_device_name()<<" to BQ"<<endl;
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


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
        {
            kp = kp/ steps_fast_complex_abs(kp);
            ks = ks/ steps_fast_complex_abs(ks);
            kt = kt/ steps_fast_complex_abs(kt);

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


            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            kp = kp/ steps_fast_complex_abs(kp);
            ks = ks/ steps_fast_complex_abs(ks);
            kt = kt/ steps_fast_complex_abs(kt);

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


            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            kp = kp/ steps_fast_complex_abs(kp);
            ks = ks/ steps_fast_complex_abs(ks);
            kt = kt/ steps_fast_complex_abs(kt);

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


            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
        {
            network_BP_matrix.add_entry(p,p,0.0);

            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
        {
            network_BP_matrix.add_entry(s,s,0.0);


            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            network_BP_matrix.add_entry(t,t,0.0);


            kp = steps_fast_complex_abs(kp_store);
            ks = steps_fast_complex_abs(ks_store);
            kt = steps_fast_complex_abs(kt_store);

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
}

void NETWORK_MATRIX::add_two_winding_transformer_to_decoupled_network_v2(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding two-winding transformer '%s' connecting to bus %u and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Ym = trans.get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

        complex<double> Zp, Zs;
        Zp = 0.5*Zps; Zs = Zp;

        double tap_primary = trans.get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE);
        double angle_primary = trans.get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);
        double tap_secondary = trans.get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE);
        double angle_secondary = trans.get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);

        complex<double> kp(tap_primary*cos(angle_primary),tap_primary*sin(angle_primary)),
                        ks(tap_secondary*cos(angle_secondary),tap_secondary*sin(angle_secondary));


        complex<double> V, I, Vstar;
        complex<double> ypp, yps, ysp, yss, Yeq, Zeq;

        complex<double> kp_store, ks_store, Ym_store;
        kp_store = kp;
        ks_store = ks;
        Ym_store = Ym;

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
        {
            kp = kp/steps_fast_complex_abs(kp);
            ks = ks/steps_fast_complex_abs(ks);
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

            kp = steps_fast_complex_abs(kp);
            ks = steps_fast_complex_abs(ks);

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

            if(isnan(ypp.real()) or isnan(ypp.imag()) or
               isnan(yps.real()) or isnan(yps.imag()) or
               isnan(ysp.real()) or isnan(ysp.imag()) or
               isnan(yss.real()) or isnan(yss.imag()) )
                cout<<"NAN is detected when adding "<<trans.get_device_name()<<" to BQ"<<endl;
            network_BQ_matrix.add_entry(p,p,ypp);
            network_BQ_matrix.add_entry(p,s,yps);
            network_BQ_matrix.add_entry(s,p,ysp);
            network_BQ_matrix.add_entry(s,s,yss);

            Ym = Ym_store;
            Ym = complex<double>(0.0, Ym.imag());
            network_BQ_matrix.add_entry(p,p,Ym);

            return;
        }

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==false)
        {
            network_BP_matrix.add_entry(p,p,0.0);

            Ym = complex<double>(0.0, Ym.imag());

            network_BQ_matrix.add_entry(p,p,Ym);
            return;
        }

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
        {
            network_BP_matrix.add_entry(s,s,0.0);

            ks = steps_fast_complex_abs(ks);

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
}

void NETWORK_MATRIX::add_fixed_shunts_to_decoupled_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<FIXED_SHUNT*> shunts = psdb.get_all_fixed_shunts();

    size_t n = shunts.size();

    for(size_t i = 0; i!=n; ++i)
        add_fixed_shunt_to_decoupled_network(*(shunts[i]));
}


void NETWORK_MATRIX::add_fixed_shunt_to_decoupled_network(const FIXED_SHUNT& shunt)
{
    if(shunt.get_status()==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding fixed shunt '%s' at bus %u to network Y matrix.", (shunt.get_identifier()).c_str(),
                      shunt.get_shunt_bus());
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t bus = shunt.get_shunt_bus();

        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(bus);

        complex<double> Yshunt = shunt.get_nominal_admittance_shunt_in_pu();

        network_BP_matrix.add_entry(i,i, 0.0);
        network_BQ_matrix.add_entry(i,i, Yshunt);
    }
}



void NETWORK_MATRIX::add_lines_to_dc_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i!=n; ++i)
        add_line_to_dc_network(*(lines[i]));
}

void NETWORK_MATRIX::add_line_to_dc_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==true or line.get_receiving_side_breaker_status()==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding line '%s' connecting to bus %u and %u to network Y matrix.", (line.get_identifier()).c_str(),
                      line.get_sending_side_bus(), line.get_receiving_side_bus());
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t sending_bus = line.get_sending_side_bus();
        size_t receiving_bus = line.get_receiving_side_bus();

        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
        size_t j = inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

        complex<double> Zline = line.get_line_positive_sequence_z_in_pu();
        Zline = complex<double>(0.0, Zline.imag());
        complex<double> Yline = 1.0/Zline;
        if(line.get_sending_side_breaker_status()==true and line.get_receiving_side_breaker_status()==true)
        {
            network_DC_B_matrix.add_entry(i,j, -Yline);
            network_DC_B_matrix.add_entry(j,i, -Yline);
            network_DC_B_matrix.add_entry(i,i, Yline);
            network_DC_B_matrix.add_entry(j,j, Yline);
            return;
        }
    }
}


void NETWORK_MATRIX::add_transformers_to_dc_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();

    size_t n = transformers.size();

    for(size_t i = 0; i!=n; ++i)
        add_transformer_to_dc_network(*(transformers[i]));
}

void NETWORK_MATRIX::add_transformer_to_dc_network(const TRANSFORMER& trans)
{
    if(trans.is_two_winding_transformer())
        add_two_winding_transformer_to_dc_network(trans);
    else
        add_three_winding_transformer_to_dc_network(trans);
}

void NETWORK_MATRIX::add_three_winding_transformer_to_dc_network(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or
       trans.get_winding_breaker_status(SECONDARY_SIDE)==true or
       trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding three-winding transformer '%s' connecting to bus %u, %u, and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE), trans.get_winding_bus(TERTIARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);
        size_t tertiary_bus = trans.get_winding_bus(TERTIARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);
        size_t t = inphno.get_internal_bus_number_of_physical_bus_number(tertiary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);

        complex<double> Zp = 0.5*(Zps+Zpt-Zst),
                         Zs = 0.5*(Zps+Zst-Zpt),
                         Zt = 0.5*(Zpt+Zst-Zps);
        double Xp = Zp.imag();
        double Xs = Zs.imag();
        double Xt = Zt.imag();

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            double Xsigma = Xp*Xs+Xs*Xt+Xt*Xp;
            double Xps = Xsigma/Xt;
            double Xst = Xsigma/Xp;
            double Xtp = Xsigma/Xs;

            complex<double> Yps(0.0, -1.0/Xps);
            complex<double> Yst(0.0, -1.0/Xst);
            complex<double> Ytp(0.0, -1.0/Xtp);

            network_DC_B_matrix.add_entry(p,p,Yps+Ytp);
            network_DC_B_matrix.add_entry(p,s,-Yps);
            network_DC_B_matrix.add_entry(p,t,-Ytp);
            network_DC_B_matrix.add_entry(s,p,-Yps);
            network_DC_B_matrix.add_entry(s,s,Yps+Yst);
            network_DC_B_matrix.add_entry(s,t,-Yst);
            network_DC_B_matrix.add_entry(t,p,-Ytp);
            network_DC_B_matrix.add_entry(t,s,-Yst);
            network_DC_B_matrix.add_entry(t,t,Yst+Ytp);
            return;
        }


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
        {
            double Xps = Zps.imag();
            complex<double> Yps(0.0, -1.0/Xps);

            network_DC_B_matrix.add_entry(p,p,Yps);
            network_DC_B_matrix.add_entry(p,s,-Yps);
            network_DC_B_matrix.add_entry(s,p,-Yps);
            network_DC_B_matrix.add_entry(s,s,Yps);
            return;
        }


        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==false and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            double Xpt = Zpt.imag();
            complex<double> Ypt(0.0, -1.0/Xpt);

            network_DC_B_matrix.add_entry(p,p,Ypt);
            network_DC_B_matrix.add_entry(p,t,-Ypt);
            network_DC_B_matrix.add_entry(t,p,-Ypt);
            network_DC_B_matrix.add_entry(t,t,Ypt);
            return;
        }

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false and
           trans.get_winding_breaker_status(SECONDARY_SIDE)==true and
           trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
        {

            double Xst = Zst.imag();
            complex<double> Yst(0.0, -1.0/Xst);

            network_DC_B_matrix.add_entry(s,s,Yst);
            network_DC_B_matrix.add_entry(s,t,-Yst);
            network_DC_B_matrix.add_entry(t,s,-Yst);
            network_DC_B_matrix.add_entry(t,t,Yst);
            return;
        }
    }
}

void NETWORK_MATRIX::add_two_winding_transformer_to_dc_network(const TRANSFORMER& trans)
{
    if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        /*ostringstream osstream;
        osstream<<"Adding two-winding transformer '%s' connecting to bus %u and %u to network Y matrix.", (trans.get_identifier()).c_str(),
                      trans.get_winding_bus(PRIMARY_SIDE), trans.get_winding_bus(SECONDARY_SIDE));
        toolkit.show_information_with_leading_time_stamp(osstream);
        */

        size_t primary_bus = trans.get_winding_bus(PRIMARY_SIDE);
        size_t secondary_bus = trans.get_winding_bus(SECONDARY_SIDE);

        size_t p = inphno.get_internal_bus_number_of_physical_bus_number(primary_bus);
        size_t s = inphno.get_internal_bus_number_of_physical_bus_number(secondary_bus);

        complex<double> Zps = trans.get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);

        double Xps = Zps.imag();
        complex<double> Yps(0.0, -1.0/Xps);

        if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true and trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
        {
            network_DC_B_matrix.add_entry(p,p,Yps);
            network_DC_B_matrix.add_entry(p,s,-Yps);
            network_DC_B_matrix.add_entry(s,p,-Yps);
            network_DC_B_matrix.add_entry(s,s,Yps);
        }
    }
}


void NETWORK_MATRIX::add_bus_fault_to_dynamic_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<BUS*> buses = psdb.get_all_buses();

    size_t n = buses.size();

    BUS* bus;

    for(size_t i = 0; i!=n; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()!=OUT_OF_SERVICE)
        {
            if(not bus->is_faulted())
                continue;
            else
            {
                FAULT fault = bus->get_fault();
                complex<double> y = fault.get_fault_shunt_in_pu();
                size_t j = inphno.get_internal_bus_number_of_physical_bus_number(bus->get_bus_number());
                network_Y_matrix.add_entry(j,j, y);
            }
        }
    }
}


void NETWORK_MATRIX::add_lines_to_dynamic_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<LINE*> lines = psdb.get_all_lines();

    size_t n = lines.size();

    for(size_t i = 0; i!=n; ++i)
    {
        if(not lines[i]->is_faulted())
           add_line_to_network(*(lines[i]));
        else
           add_faulted_line_to_dynamic_network(*(lines[i]));
    }
}

void NETWORK_MATRIX::add_faulted_line_to_dynamic_network(const LINE& line)
{
    if(line.get_sending_side_breaker_status()==true or line.get_receiving_side_breaker_status()==true)
    {
        if(line.is_faulted())
        {
            /*ostringstream osstream;
            osstream<<"Adding line '%s' connecting to bus %u and %u to network Y matrix.", (line.get_identifier()).c_str(),
                          line.get_sending_side_bus(), line.get_receiving_side_bus());
            toolkit.show_information_with_leading_time_stamp(osstream);*/

            size_t sending_bus = line.get_sending_side_bus();
            size_t receiving_bus = line.get_receiving_side_bus();

            size_t i = inphno.get_internal_bus_number_of_physical_bus_number(sending_bus);
            size_t j = inphno.get_internal_bus_number_of_physical_bus_number(receiving_bus);

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

            for(size_t k=0; k!=nfault; ++k)
            {
                double this_fault_location = line.get_fault_location_of_fault(k);
                if(this_fault_location != 0.0 and this_fault_location != 1.0)
                {
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
                for(int k=n-2; k>=0; --k)
                {
                    double fault_location = sublines[k].to_loc;
                    line_length = sublines[k].to_loc - sublines[k].from_loc;
                    fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
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
                for(size_t k=0; k!=n; ++k)
                {
                    double fault_location = sublines[k].to_loc;
                    line_length = sublines[k].to_loc - sublines[k].from_loc;
                    y = Yline*line_length; z = Zline*line_length;
                    I = I-V*y*0.5;
                    V = V-I*z;
                    if(fault_location!=1.0)
                    {
                        I = I - V*y*0.5;
                        fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
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
                for(size_t k=1; k!=n; ++k)
                {
                    double fault_location = sublines[k].from_loc;
                    line_length = sublines[k].to_loc - sublines[k].from_loc;
                    y = Yline*line_length; z = Zline*line_length;
                    fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
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
                for(int k=n-1; k>=0; --k)
                {
                    double fault_location = sublines[k].from_loc;
                    double line_length = sublines[k].to_loc - sublines[k].from_loc;
                    complex<double> yfault = 0.0;
                    if(fault_location!=0.0)
                    {
                        fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
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
                int nint = int(n);
                for(int k=0; k!=nint; ++k)
                {
                    double fault_location = sublines[k].to_loc;
                    double line_length = sublines[k].to_loc - sublines[k].from_loc;
                    complex<double> yfault = 0.0;
                    if(fault_location!=1.0)
                    {
                        fault = line.get_fault_at_location(line.get_sending_side_bus(), fault_location);
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
        else // line is not faulted
            add_line_to_network(line);
    }
}

void NETWORK_MATRIX::add_generators_to_dynamic_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<GENERATOR*> generators = psdb.get_all_generators();

    size_t n= generators.size();

    for(size_t i=0; i!=n; ++i)
        add_generator_to_dynamic_network(*(generators[i]));
}

void NETWORK_MATRIX::add_generator_to_dynamic_network(const GENERATOR& gen)
{
    if(gen.get_status()==true)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        complex<double> Z = gen.get_source_impedance_in_pu();
        double one_over_mbase = gen.get_one_over_mbase_in_one_over_MVA();
        double sbase = psdb.get_system_base_power_in_MVA();
        Z *= (one_over_mbase*sbase);

        size_t bus = gen.get_generator_bus();
        size_t i = inphno.get_internal_bus_number_of_physical_bus_number(bus);
        network_Y_matrix.add_entry(i,i,1.0/Z);
    }
}

void NETWORK_MATRIX::add_wt_generators_to_dynamic_network()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<WT_GENERATOR*> generators = psdb.get_all_wt_generators();

    size_t n= generators.size();

    for(size_t i=0; i!=n; ++i)
        add_wt_generator_to_dynamic_network(*(generators[i]));
}

void NETWORK_MATRIX::add_wt_generator_to_dynamic_network(WT_GENERATOR& gen)
{
    if(gen.get_status()==true)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        WT_GENERATOR_MODEL* genmodel = gen.get_wt_generator_model();
        if(genmodel!=NULL)
        {
            if(genmodel->is_voltage_source())
            {
                POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
                complex<double> Z = gen.get_source_impedance_in_pu();
                double one_over_mbase = gen.get_one_over_mbase_in_one_over_MVA();
                double sbase = psdb.get_system_base_power_in_MVA();
                Z *= (one_over_mbase*sbase);

                size_t bus = gen.get_generator_bus();
                size_t i = inphno.get_internal_bus_number_of_physical_bus_number(bus);
                network_Y_matrix.add_entry(i,i,1.0/Z);
            }
            //else // is current source
            //    return;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Error. No WT_GENERATOR_MODEL is provided for "<<gen.get_device_name()<<endl
                    <<"Its source impedance will not be added to network matrix.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }
    }
}

void NETWORK_MATRIX::optimize_network_ordering()
{
    initialize_physical_internal_bus_pair();
    build_network_matrix();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    if(toolkit.is_optimize_network_enabled())
    {
        reorder_physical_internal_bus_pair();
        /*for(size_t i=0; i<1; ++i)
        {
            reorder_physical_internal_bus_pair();
            build_network_matrix();
        }*/
    }
}

bool NETWORK_MATRIX::is_condition_ok() const
{
    return true;
}

void NETWORK_MATRIX::initialize_physical_internal_bus_pair()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.check_device_status_for_out_of_service_buses();
    vector<BUS*> buses = psdb.get_all_buses();

    size_t nbus = buses.size();

    if(nbus!=0)
    {
        inphno.clear();

        size_t bus_number;
        size_t internal_bus_number = 0;
        for(size_t i=0; i!=nbus; ++i)
        {
            if(buses[i]->get_bus_type()!=OUT_OF_SERVICE)
            {
                bus_number = buses[i]->get_bus_number();
                inphno.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                internal_bus_number ++;
            }
        }
    }
}

void NETWORK_MATRIX::reorder_physical_internal_bus_pair()
{
    vector<size_t> permutation = network_Y_matrix.get_reorder_permutation();
    inphno.update_with_new_internal_bus_permutation(permutation);
    ostringstream osstream;
    osstream<<"Network internal bus numbers are optimized.";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    /*os<<"After optimizing the network with network optimizer, the internal buses are listed as follows");
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"internal   physical   storage");
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t bus, index, nbus;

    nbus = get_in_service_bus_count();

    for(size_t i=0; i!=nbus; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        index = get_bus_index(bus);
        osstream<<"%-10u %-10u %-10u",i, bus, index);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }*/

    /*os<<"Permutation for reorder internal bus:");
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t nperm = permutation.size();
    for(size_t i=0; i!=nperm; ++i)
    {
        osstream<<"%u ---> %u",i,permutation[i]);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    */

}

void NETWORK_MATRIX::check_network_connectivity(bool remove_void_island)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector< vector<size_t> > islands = get_islands_with_physical_bus_number();

    size_t nislands = islands.size();

    ostringstream osstream;
    osstream<<"There are "<<nislands<<" islands.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t physical_bus;
    bool there_is_slack_bus_in_island;
    BUS* bus;
    for(size_t i=0; i!=nislands; ++i)
    {
        osstream<<"Island "<<i<<":";
        toolkit.show_information_with_leading_time_stamp(osstream);

        osstream<<"Areas in island "<<i<<":\n";
        size_t nbus = islands[i].size();
        set<size_t> areas_in_island;
        for(size_t j=0; j!=nbus; ++j)
        {
            physical_bus = islands[i][j];
            bus = psdb.get_bus(physical_bus);
            size_t area = bus->get_area_number();
            areas_in_island.insert(area);
        }
        set<size_t>::iterator iter;
        size_t area_count = 0;
        for(iter = areas_in_island.begin() ; iter != areas_in_island.end() ; ++iter)
        {
            area_count++;
            size_t area = *iter;
            osstream<<"("<<setw(3)<<area_count<<") "<<area<<" ["<<psdb.area_number2area_name(area)<<"]\n";
        }
        toolkit.show_information_with_leading_time_stamp(osstream);


        osstream<<"Physical buses in island "<<i<<":";
        toolkit.show_information_with_leading_time_stamp(osstream);
        there_is_slack_bus_in_island = false;
        for(size_t j=0; j!=nbus; ++j)
        {
            physical_bus = islands[i][j];
            bus = psdb.get_bus(physical_bus);
            size_t area = bus->get_area_number();
            osstream<<physical_bus<<" ["<<bus->get_bus_name()<<"  "<<bus->get_base_voltage_in_kV()<<"kV] AREA "<<area<<"["<<psdb.area_number2area_name(area)<<"]";
            if(bus->get_bus_type()==SLACK_TYPE)
            {
                osstream<<" (Slack bus)";
                there_is_slack_bus_in_island = true;
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        if(not there_is_slack_bus_in_island)
        {
            osstream<<"No slack bus is found in island "<<i<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
            if(remove_void_island==true)
            {
                for(size_t j=0; j!=nbus; ++j)
                {
                    physical_bus = islands[i][j];
                    bus = psdb.get_bus(physical_bus);
                    bus->set_bus_type(OUT_OF_SERVICE);
                }
                osstream<<"All buses in island "<<i<<" are set as OUT OF SERVICE.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

vector< vector<size_t> > NETWORK_MATRIX::get_islands_with_internal_bus_number()
{
    vector< vector<size_t> > islands;

    if(inphno.empty())
        initialize_physical_internal_bus_pair();

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //build_network_matrix();
    //network_Y_matrix.report_brief();

    size_t nbus = psdb.get_in_service_bus_count();

    vector<bool> bus_searched_flag;
    bus_searched_flag.reserve(nbus);

    for(size_t i=0; i!=nbus; ++i)
    {
        size_t physical_bus = get_physical_bus_number_of_internal_bus(i);
        if(physical_bus!=INDEX_NOT_EXIST)
            bus_searched_flag.push_back(false);
        else
            bus_searched_flag.push_back(true);
    }

    while(true)
    {
        bool new_island_found = false;
        size_t first_bus_in_new_island;

        for(size_t i=0; i!=nbus; ++i)
        {
            if(bus_searched_flag[i]==false)
            {
                new_island_found = true;
                first_bus_in_new_island = i;
                break;
            }
        }
        if(new_island_found==true)
        {
            vector<size_t> this_island;
            this_island.reserve(nbus);

            this_island.push_back(first_bus_in_new_island);
            bus_searched_flag[first_bus_in_new_island] = true;

            int searching_bus=0;
            size_t nbus_in_island = this_island.size();
            for(size_t i=0; i!=nbus_in_island; ++i)
            {
                searching_bus = this_island[i];
                int k_start = network_Y_matrix.get_starting_index_of_column(searching_bus);
                int k_end = network_Y_matrix.get_starting_index_of_column(searching_bus+1);
                int row_bus;
                for(int k=k_start; k!=k_end; ++k)
                {
                    row_bus = network_Y_matrix.get_row_number_of_entry_index(k);
                    if(row_bus!=searching_bus)
                    {
                        if(bus_searched_flag[row_bus]==false)
                        {
                            this_island.push_back(row_bus);
                            nbus_in_island = this_island.size();
                            bus_searched_flag[row_bus] = true;
                        }
                    }
                }
            }
            islands.push_back(this_island);
        }
        else
            break;
    }
    return islands;
}


vector< vector<size_t> > NETWORK_MATRIX::get_islands_with_physical_bus_number()
{
    vector< vector<size_t> > islands = get_islands_with_internal_bus_number();

    size_t nislands = islands.size();
    for(size_t i=0; i!= nislands; ++i)
    {
        size_t nbus_in_island = islands[i].size();
        for(size_t j=0; j!=nbus_in_island; ++j)
        {
            islands[i][j] = get_physical_bus_number_of_internal_bus(islands[i][j]);
        }
    }

    return islands;
}

size_t NETWORK_MATRIX::get_internal_bus_number_of_physical_bus(size_t bus) const
{
    return inphno.get_internal_bus_number_of_physical_bus_number(bus);
}

size_t NETWORK_MATRIX::get_physical_bus_number_of_internal_bus(size_t bus) const
{
    return inphno.get_physical_bus_number_of_internal_bus_number(bus);
}

void NETWORK_MATRIX::report_network_matrix() const
{
    ostringstream osstream;

    osstream<<"Network Y matrix lists begin:";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    report_network_matrix_common();
}

void NETWORK_MATRIX::report_decoupled_network_matrix() const
{
    ostringstream osstream;

    osstream<<"Network decoupled B matrix lists begin:";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"row   [  bus  ]  column[  bus  ]     BP       BQ";
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t i, ibus, jbus;
    size_t n = network_BP_matrix.get_matrix_size();
    int k_starting, k_ending;
    k_starting = 0;
    complex<double> y;
    double bp, bq;
    for(size_t j=0; j!=n; ++j)
    {
        k_ending = network_BP_matrix.get_starting_index_of_column(j+1);
        for(int k=k_starting; k!=k_ending; ++k)
        {
            y = network_BP_matrix.get_entry_value(k);
            bp = y.imag();
            y = network_BQ_matrix.get_entry_value(k);
            bq = y.imag();
            i = network_BP_matrix.get_row_number_of_entry_index(k);
            ibus = get_physical_bus_number_of_internal_bus(i);
            jbus = get_physical_bus_number_of_internal_bus(j);

            osstream<<setw(6)<<i<<"["
              <<setw(7)<<ibus<<"]  "
              <<setw(6)<<j<<"["
              <<setw(7)<<jbus<<"]  "
              <<setw(10)<<setprecision(6)<<fixed<<bp<<", "
              <<setw(10)<<setprecision(6)<<fixed<<bq<<endl;

            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        k_starting = k_ending;
    }
    osstream<<"Network decoupled B matrix lists finished.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void NETWORK_MATRIX::report_dc_network_matrix() const
{
    ostringstream osstream;

    osstream<<"DC network B matrix lists begin:";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"row   [  bus  ]  column[  bus  ]     B";
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t i, ibus, jbus;
    size_t n = network_DC_B_matrix.get_matrix_size();
    int k_starting, k_ending;
    k_starting = 0;
    complex<double> y;
    double b;
    for(size_t j=0; j!=n; ++j)
    {
        k_ending = network_DC_B_matrix.get_starting_index_of_column(j+1);
        for(int k=k_starting; k!=k_ending; ++k)
        {
            y = network_DC_B_matrix.get_entry_value(k);
            b = y.imag();
            i = network_DC_B_matrix.get_row_number_of_entry_index(k);
            ibus = get_physical_bus_number_of_internal_bus(i);
            jbus = get_physical_bus_number_of_internal_bus(j);

            osstream<<setw(6)<<i<<"["
              <<setw(7)<<ibus<<"]  "
              <<setw(6)<<j<<"["
              <<setw(7)<<jbus<<"]  "
              <<setw(10)<<setprecision(6)<<fixed<<b<<endl;

            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        k_starting = k_ending;
    }
    osstream<<"DC network B matrix lists finished.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void NETWORK_MATRIX::report_dynamic_network_matrix() const
{
    ostringstream osstream;

    osstream<<"Network dynamic Y matrix lists begin:";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

	report_network_matrix_common();
}

void NETWORK_MATRIX::report_network_matrix_common() const
{
    ostringstream osstream;

    osstream<<"row   [  bus  ]  column[  bus  ]     real       imaginary";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    size_t i, ibus, jbus;
    size_t n = network_Y_matrix.get_matrix_size();
    int k_starting, k_ending;
    k_starting = 0;
    complex<double> y;
    for(size_t j=0; j!=n; ++j)
    {
        k_ending = network_Y_matrix.get_starting_index_of_column(j+1);
        for(int k=k_starting; k!=k_ending; ++k)
        {
            y = network_Y_matrix.get_entry_value(k);
            i = network_Y_matrix.get_row_number_of_entry_index(k);
            ibus = get_physical_bus_number_of_internal_bus(i);
            jbus = get_physical_bus_number_of_internal_bus(j);

            osstream<<setw(6)<<i<<"["
              <<setw(7)<<ibus<<"]  "
              <<setw(6)<<j<<"["
              <<setw(7)<<jbus<<"]  "
              <<setw(10)<<setprecision(6)<<fixed<<y.real()<<", "
              <<setw(10)<<setprecision(6)<<fixed<<y.imag()<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        k_starting = k_ending;
    }
    osstream<<"Network matrix lists finished.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}


void NETWORK_MATRIX::save_network_matrix_to_file(const string& filename) const
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ofstream file(filename);
    if(file.is_open())
    {
        save_network_matrix_common(file);
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving network matrix to file."<<endl
          <<"No network matrix will be exported.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void NETWORK_MATRIX::save_decoupled_network_matrix_to_file(const string& filename) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    ofstream file(filename);
    if(file.is_open())
    {
        file<<"ROW,ROW_BUS,COLUMN,COLUMN_BUS,BP,BQ"<<endl;

        size_t i, ibus, jbus;
        size_t n = network_BP_matrix.get_matrix_size();
        int k_starting, k_ending;
        k_starting = 0;
        complex<double> yp, yq;
        char buffer[1000];
        for(size_t j=0; j!=n; ++j)
        {
            k_ending = network_BP_matrix.get_starting_index_of_column(j+1);
            for(int k=k_starting; k!=k_ending; ++k)
            {
                yp = network_BP_matrix.get_entry_value(k);
                yq = network_BQ_matrix.get_entry_value(k);
                i = network_BP_matrix.get_row_number_of_entry_index(k);
                ibus = get_physical_bus_number_of_internal_bus(i);
                jbus = get_physical_bus_number_of_internal_bus(j);

                snprintf(buffer, 1000, "%lu,%lu,%lu,%lu,%.14f,%.14f",i, ibus, j, jbus, yp.imag(), yq.imag());
                file<<buffer<<endl;
                /*file<<i<<","<<ibus<<","<<j<<","<<jbus<<","
                    <<setprecision(14)<<fixed<<yp.imag()<<","
                    <<setprecision(14)<<fixed<<yq.imag()<<endl;*/
            }
            k_starting = k_ending;
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving decoupled network matrix to file."<<endl
          <<"No decoupled network matrix will be exported.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void NETWORK_MATRIX::save_dc_network_matrix_to_file(const string& filename) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    ofstream file(filename);
    if(file.is_open())
    {
        file<<"ROW,ROW_BUS,COLUMN,COLUMN_BUS,B"<<endl;

        size_t i, ibus, jbus;
        size_t n = network_DC_B_matrix.get_matrix_size();
        int k_starting, k_ending;
        k_starting = 0;
        complex<double> y;
        char buffer[1000];
        for(size_t j=0; j!=n; ++j)
        {
            k_ending = network_DC_B_matrix.get_starting_index_of_column(j+1);
            for(int k=k_starting; k!=k_ending; ++k)
            {
                y = network_DC_B_matrix.get_entry_value(k);
                i = network_DC_B_matrix.get_row_number_of_entry_index(k);
                ibus = get_physical_bus_number_of_internal_bus(i);
                jbus = get_physical_bus_number_of_internal_bus(j);

                snprintf(buffer, 1000, "%lu,%lu,%lu,%lu,%.14f",i, ibus, j, jbus, y.imag());
                file<<buffer<<endl;
                /*file<<i<<","<<ibus<<","<<j<<","<<jbus<<","
                    <<setprecision(14)<<fixed<<y.imag()<<endl;*/
            }
            k_starting = k_ending;
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving DC network matrix to file."<<endl
          <<"No DC network matrix will be exported.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void NETWORK_MATRIX::save_dynamic_network_matrix_to_file(const string& filename) const
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    ofstream file(filename);
    if(file.is_open())
    {
        save_network_matrix_common(file);
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving dynamic network matrix to file."<<endl
          <<"No dynamic network matrix will be exported.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void NETWORK_MATRIX::save_network_matrix_common(ofstream& file) const
{
    file<<"ROW,ROW_BUS,COLUMN,COLUMN_BUS,REAL,IMAGINARY"<<endl;

    size_t i, ibus, jbus;
    size_t n = network_Y_matrix.get_matrix_size();
    int k_starting, k_ending;
    k_starting = 0;
    complex<double> y;
    char buffer[1000];
    for(size_t j=0; j!=n; ++j)
    {
        k_ending = network_Y_matrix.get_starting_index_of_column(j+1);
        for(int k=k_starting; k!=k_ending; ++k)
        {
            y = network_Y_matrix.get_entry_value(k);
            i = network_Y_matrix.get_row_number_of_entry_index(k);
            ibus = get_physical_bus_number_of_internal_bus(i);
            jbus = get_physical_bus_number_of_internal_bus(j);

            snprintf(buffer, 1000, "%lu,%lu,%lu,%lu,%.14f,%.14f",i, ibus, j, jbus, y.real(), y.imag());
            file<<buffer<<endl;
            /*file<<i<<","<<ibus<<","<<j<<","<<jbus<<","
                <<setprecision(14)<<fixed<<y.real()<<","
                <<setprecision(14)<<fixed<<y.imag()<<endl;*/
        }
        k_starting = k_ending;
    }
}


void NETWORK_MATRIX::report_physical_internal_bus_number_pair() const
{
    inphno.report();
}




bool NETWORK_MATRIX::is_valid() const
{
    return true; // disabled.
}
void NETWORK_MATRIX::check()
{
    ;// disabled.
}
