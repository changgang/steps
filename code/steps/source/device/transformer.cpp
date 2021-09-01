#include "header/device/transformer.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>


using namespace std;


TRANSFORMER::TRANSFORMER(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

TRANSFORMER::~TRANSFORMER()
{
    ;
}


string TRANSFORMER::get_winding_name(TRANSFORMER_WINDING_SIDE winding) const
{
    switch(winding)
    {
        case PRIMARY_SIDE:
            return "PRIMARY";
        case SECONDARY_SIDE:
            return "SECONDARY";
        default:
            return "TERTIARY";
    }
}

void TRANSFORMER::set_identifier(string trans_id)
{
    trans_id = trim_string(trans_id);
    add_string_to_str_int_map(trans_id);
    this->identifier_index = get_index_of_string(trans_id);
}

void TRANSFORMER::set_name(string trans_name)
{
    trans_name = trim_string(trans_name);
    add_string_to_str_int_map(trans_name);
    this->name_index = get_index_of_string(trans_name);
}

void TRANSFORMER::set_non_metered_end_bus(unsigned int bus)
{
    if(bus==0)
    {
        non_metered_end_bus = get_winding_bus(SECONDARY_SIDE);
        return;
    }

    STEPS& toolkit = get_toolkit();

    if(bus==get_winding_bus(PRIMARY_SIDE) or bus==get_winding_bus(SECONDARY_SIDE) or bus==get_winding_bus(TERTIARY_SIDE))
        non_metered_end_bus = bus;
    else
    {
        ostringstream osstream;
        if(get_winding_bus(TERTIARY_SIDE)==0)
        {
            osstream<<"Bus "<<bus<<" is not among the winding buses ("<<get_winding_bus(PRIMARY_SIDE)<<", "<<get_winding_bus(SECONDARY_SIDE)<<") for setting up non-metered end bus of transformer."<<endl
              <<"Secondary winding bus ("<<get_winding_bus(SECONDARY_SIDE)<<") will be set automatically.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            osstream<<"Bus "<<bus<<" is not among the winding buses ("<<get_winding_bus(PRIMARY_SIDE)<<", "<<get_winding_bus(SECONDARY_SIDE)<<", "<<get_winding_bus(TERTIARY_SIDE)<<") for setting up non-metered end bus of transformer."<<endl
              <<"Secondary winding bus ("<<get_winding_bus(SECONDARY_SIDE)<<") will be set automatically.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        non_metered_end_bus = get_winding_bus(SECONDARY_SIDE);
    }
}


void TRANSFORMER::set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double> y)
{
    y_magnetizing_in_pu = y;
}

void TRANSFORMER::set_winding_bus(TRANSFORMER_WINDING_SIDE winding, unsigned int bus)
{
    ostringstream osstream;
    if(bus==0 and winding==TERTIARY_SIDE)
    {
        winding_bus[winding] = 0;
        return;
    }

    STEPS& toolkit = get_toolkit();
    if(bus==0 and winding!=TERTIARY_SIDE)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<get_winding_name(winding)<<" winding bus of transformer."<<endl
          <<"0 will be set to indicate invalid transformer.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        winding_bus[winding] = 0;
        return;
    }

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_bus_exist(bus))
    {
        osstream<<"Bus "<<bus<<" does not exist in power system database '"<<psdb.get_system_name()<<"' for setting up "<<get_winding_name(winding)<<" winding bus of transformer."<<endl
          <<"0 will be set to indicate invalid transformer.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        winding_bus[winding] = 0;
        return;
    }
    winding_bus[winding] = bus;
    //winding_busptr[winding] = psdb.get_bus(bus);
}

void TRANSFORMER::set_winding_breaker_status(TRANSFORMER_WINDING_SIDE winding, bool status)
{
    winding_breaker_status[winding] = status;
}

void TRANSFORMER::set_winding_connection_type(TRANSFORMER_WINDING_SIDE winding, TRANSFORMER_WINDING_CONNECTION_TYPE wctype)
{
    winding_connection_type[winding] = wctype;
}

void TRANSFORMER::set_winding_nominal_voltage_in_kV(TRANSFORMER_WINDING_SIDE winding, double v)
{
    unsigned int bus = get_winding_bus(winding);
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(v==0.0)
        v = psdb.get_bus_base_voltage_in_kV(bus);
    if(v==0.0)
    {
        ostringstream osstream;
        DEVICE_ID did = get_device_id();
        string device = did.get_compound_device_name();
        osstream<<"Error. Zero (0.0kV) nominal voltage is not allowed for setting up "<<get_winding_name(winding)<<" winding nominal voltage of "<<device<<"."<<endl
          <<"0.0 will be set to indicate invalid transformer";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    winding_nominal_voltage_in_kV[winding] = v;
}

void TRANSFORMER::set_winding_nominal_capacity_in_MVA(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, double s)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(winding1!=winding2)
    {
        if(winding1>winding2)
        {
            TRANSFORMER_WINDING_SIDE temp_winding;
            temp_winding = winding1;
            winding1 = winding2;
            winding2 = temp_winding;
        }

        if(s==0.0)
        {
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            s = psdb.get_system_base_power_in_MVA();
        }
        if(winding1==PRIMARY_SIDE and winding2==TERTIARY_SIDE)
            winding_nominal_capacity_in_MVA[TERTIARY_SIDE]=s;
        else
            winding_nominal_capacity_in_MVA[winding1]=s;
    }
    else
    {
        osstream<<"Warning. The same windings ("<<get_winding_name(winding1)<<") are not allowed to set winding nominal capacity.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void TRANSFORMER::set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, complex<double> z)
{
    ostringstream osstream;
    if(winding1!=winding2)
    {
        if(winding1>winding2)
        {
            TRANSFORMER_WINDING_SIDE temp_winding;
            temp_winding = winding1;
            winding1 = winding2;
            winding2 = temp_winding;
        }
        if(z==0.0)
        {
            STEPS& toolkit = get_toolkit();
            osstream<<"Warning. The leakage impedance between "<<get_winding_name(winding1)<<" and "<<get_winding_name(winding2)<<" windings is zero for "<<get_compound_device_name()<<endl
                    <<"Correction is required. Check original data.\n"
                    <<"Impedance will be set as 0.001.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            z = complex<double>(0.0, 0.001);
        }

        if(winding1==PRIMARY_SIDE and winding2==SECONDARY_SIDE)
            zl_primary2secondary_in_pu = z;
        else
        {
            if(winding1==SECONDARY_SIDE and winding2==TERTIARY_SIDE)
                zl_secondary2tertiary_in_pu = z;
            else
                zl_primary2tertiary_in_pu = z;
        }
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. The same windings ("<<get_winding_name(winding1)<<") are not allowed to set winding leakage impedance.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void TRANSFORMER::set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio)
{
    winding_turn_ratio_in_pu[winding] = turn_ratio;
}

void TRANSFORMER::set_winding_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle)
{
    winding_angle_shift_in_deg[winding] = angle;
}

void TRANSFORMER::set_winding_rating_in_MVA(TRANSFORMER_WINDING_SIDE winding, RATING rate)
{
    winding_rating_in_MVA[winding] = rate;
}

void TRANSFORMER::set_winding_number_of_taps(TRANSFORMER_WINDING_SIDE winding, unsigned int n)
{
    winding_number_of_taps[winding] = n;
}

void TRANSFORMER::set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio_max)
{
    winding_max_turn_ratio_in_pu[winding] = turn_ratio_max;
}

void TRANSFORMER::set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio_min)
{
    winding_min_turn_ratio_in_pu[winding] = turn_ratio_min;
}

void TRANSFORMER::set_winding_max_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle_max)
{
    winding_max_angle_shift_in_deg[winding] = angle_max;
}

void TRANSFORMER::set_winding_min_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle_min)
{
    winding_min_angle_shift_in_deg[winding] = angle_min;
}

void TRANSFORMER::set_winding_control_mode(TRANSFORMER_WINDING_SIDE winding, TRANSFORMER_WINDING_CONTROL_MODE mode)
{
    winding_control_mode[winding] = mode;
}

void TRANSFORMER::set_winding_controlled_bus(TRANSFORMER_WINDING_SIDE winding, unsigned int bus)
{
    ostringstream osstream;

    unsigned int winding_bus = get_winding_bus(winding);

    if(bus==0 or bus==winding_bus)
        winding_controlled_bus[winding] = winding_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        if(psdb.is_bus_exist(bus))
            winding_controlled_bus[winding] = bus;
        else
        {
            osstream<<"Bus "<<bus<<" does not exist in power system database '"<<psdb.get_system_name()<<"' for setting up controlled bus by "
              <<get_winding_name(winding)<<" winding of "<<get_compound_device_name()<<endl
              <<get_winding_name(winding)<<" winding controlled bus will be set automatically as "<<get_winding_name(winding)<<" winding bus "<<winding_bus<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
            winding_controlled_bus[winding] = winding_bus;
        }
    }
}

void TRANSFORMER::set_winding_controlled_max_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double v)
{
    winding_controlled_max_bus_voltage_in_pu[winding] = v;
}

void TRANSFORMER::set_winding_controlled_min_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double v)
{
    winding_controlled_min_bus_voltage_in_pu[winding] = v;
}

void TRANSFORMER::set_controlled_max_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding, double q)
{
    controlled_max_reactive_power_into_winding_in_MVar[winding] = q;
}

void TRANSFORMER::set_controlled_min_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding, double q)
{
    controlled_min_reactive_power_into_winding_in_MVar[winding] = q;
}

void TRANSFORMER::set_controlled_max_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding, double p)
{
    controlled_max_active_power_into_winding_in_MW[winding] = p;
}

void TRANSFORMER::set_controlled_min_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding, double p)
{
    controlled_min_active_power_into_winding_in_MW[winding] = p;
}

void TRANSFORMER::set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding, complex<double> z)
{
    z0_winding_ground_in_pu[winding] = z;
}

void TRANSFORMER::set_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu(complex<double> z)
{
    z0_common_nutral_ground_in_pu = z;
}
void TRANSFORMER::set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, complex<double> z)
{
    if((winding1==PRIMARY_SIDE and winding2==SECONDARY_SIDE) or
       (winding1==SECONDARY_SIDE and winding2==PRIMARY_SIDE))
    {
        z0_primary2secondary_in_pu = z;
        return;
    }
    if((winding1==PRIMARY_SIDE and winding2==TERTIARY_SIDE) or
       (winding1==TERTIARY_SIDE and winding2==PRIMARY_SIDE))
    {
        z0_primary2tertiary_in_pu = z;
        return;
    }
    if((winding1==TERTIARY_SIDE and winding2==SECONDARY_SIDE) or
       (winding1==SECONDARY_SIDE and winding2==TERTIARY_SIDE))
    {
        z0_secondary2tertiary_in_pu = z;
        return;
    }
}

string TRANSFORMER::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

string TRANSFORMER::get_name() const
{
    return get_string_of_index(name_index);
}

unsigned int TRANSFORMER::get_non_metered_end_bus() const
{
    return non_metered_end_bus;
}

complex<double> TRANSFORMER::get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu() const
{
    return y_magnetizing_in_pu;
}

unsigned int TRANSFORMER::get_winding_bus(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_bus[winding];
}

/*BUS* TRANSFORMER::get_winding_bus_pointer(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_busptr[winding];
}*/

bool TRANSFORMER::get_winding_breaker_status(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_breaker_status[winding];
}

TRANSFORMER_WINDING_CONNECTION_TYPE TRANSFORMER::get_winding_connection_type(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_connection_type[winding];
}

double TRANSFORMER::get_winding_nominal_voltage_in_kV(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_nominal_voltage_in_kV[winding];
}

double TRANSFORMER::get_winding_nominal_capacity_in_MVA(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const
{
    ostringstream osstream;
    if(winding1!=winding2)
    {
        if(winding1>winding2)
        {
            TRANSFORMER_WINDING_SIDE temp_winding;
            temp_winding = winding1;
            winding1 = winding2;
            winding2 = temp_winding;
        }
        if(winding1==PRIMARY_SIDE and winding2==TERTIARY_SIDE)
            return winding_nominal_capacity_in_MVA[TERTIARY_SIDE];
        else
            return winding_nominal_capacity_in_MVA[winding1];
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. The same windings ("<<get_winding_name(winding1)<<") are not allowed to get winding nominal capacity. Zero will be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

complex<double> TRANSFORMER::get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const
{
    ostringstream osstream;
    if(winding1!=winding2)
    {
        if(winding1>winding2)
        {
            TRANSFORMER_WINDING_SIDE temp_winding;
            temp_winding = winding1;
            winding1 = winding2;
            winding2 = temp_winding;
        }
        if(winding1==PRIMARY_SIDE and winding2==TERTIARY_SIDE)
            return zl_primary2tertiary_in_pu;
        else
        {
            if(winding1==PRIMARY_SIDE)
                return zl_primary2secondary_in_pu;
            else
                return zl_secondary2tertiary_in_pu;
        }
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        osstream<<"Warning. The same windings ("<<get_winding_name(winding1)<<") are not allowed to get winding nominal leakage impedance. Zero will be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}
double TRANSFORMER::get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_turn_ratio_in_pu[winding];
}

double TRANSFORMER::get_winding_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_angle_shift_in_deg[winding];
}

RATING TRANSFORMER::get_winding_rating_in_MVA(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_rating_in_MVA[winding];
}

unsigned int TRANSFORMER::get_winding_number_of_taps(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_number_of_taps[winding];
}

double TRANSFORMER::get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_max_turn_ratio_in_pu[winding];
}

double TRANSFORMER::get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_min_turn_ratio_in_pu[winding];
}

double TRANSFORMER::get_winding_max_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_max_angle_shift_in_deg[winding];
}

double TRANSFORMER::get_winding_min_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_min_angle_shift_in_deg[winding];
}

TRANSFORMER_WINDING_CONTROL_MODE TRANSFORMER::get_winding_control_mode(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_control_mode[winding];
}

unsigned int TRANSFORMER::get_winding_controlled_bus(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_controlled_bus[winding];
}

double TRANSFORMER::get_winding_controlled_max_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_controlled_max_bus_voltage_in_pu[winding];
}

double TRANSFORMER::get_winding_controlled_min_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return winding_controlled_min_bus_voltage_in_pu[winding];
}

double TRANSFORMER::get_controlled_max_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding) const
{
    return controlled_max_reactive_power_into_winding_in_MVar[winding];
}

double TRANSFORMER::get_controlled_min_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding) const
{
    return controlled_min_reactive_power_into_winding_in_MVar[winding];
}

double TRANSFORMER::get_controlled_max_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding) const
{
    return controlled_max_active_power_into_winding_in_MW[winding];
}

double TRANSFORMER::get_controlled_min_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding) const
{
    return controlled_min_active_power_into_winding_in_MW[winding];
}

complex<double> TRANSFORMER::get_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding) const
{
    return z0_winding_ground_in_pu[winding];
}

complex<double> TRANSFORMER::get_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu() const
{
    return z0_common_nutral_ground_in_pu;
}

complex<double> TRANSFORMER::get_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const
{
    if((winding1==PRIMARY_SIDE and winding2==SECONDARY_SIDE) or
       (winding1==SECONDARY_SIDE and winding2==PRIMARY_SIDE))
        return z0_primary2secondary_in_pu;
    if((winding1==PRIMARY_SIDE and winding2==TERTIARY_SIDE) or
       (winding1==TERTIARY_SIDE and winding2==PRIMARY_SIDE))
        return z0_primary2tertiary_in_pu;
    if((winding1==TERTIARY_SIDE and winding2==SECONDARY_SIDE) or
       (winding1==SECONDARY_SIDE and winding2==TERTIARY_SIDE))
        return z0_secondary2tertiary_in_pu;
    return 0.0;
}

bool TRANSFORMER::is_valid() const
{
    if(get_winding_bus(PRIMARY_SIDE)!=0 and get_winding_bus(SECONDARY_SIDE)!=0)
        return true;
    else
        return false;
}

void TRANSFORMER::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int ibus = get_winding_bus(PRIMARY_SIDE);
    unsigned int jbus = get_winding_bus(SECONDARY_SIDE);
    unsigned int kbus = get_winding_bus(TERTIARY_SIDE);

    if(is_two_winding_transformer())
        osstream<<"Warning detected when checking "<<get_compound_device_name()+"["
                <<psdb.bus_number2bus_name(ibus)<<"-"
                <<psdb.bus_number2bus_name(jbus)<<"]: \n";
    else
        osstream<<"Warning detected when checking "<<get_compound_device_name()+"["
                <<psdb.bus_number2bus_name(ibus)<<"-"
                <<psdb.bus_number2bus_name(jbus)<<"-"
                <<psdb.bus_number2bus_name(kbus)<<"]: \n";

    unsigned int error_count = 0;

    complex<double> zij = get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE,SECONDARY_SIDE);
    double x = zij.imag();
    /*if(fabs(r)>10.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence R between primary and secondary side is too great: "
                <<"Rij = "<<r<<"pu\n";
    }*/
    if(fabs(x)>10.0)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence X between primary and secondary side is too great: "
                <<"Xij = "<<x<<"pu.\n";
    }
    if(fabs(x)<0.0001)
    {
        error_count++;
        osstream<<"("<<error_count<<") Positive sequence X between primary and secondary side is too little: "
                <<"Xij = "<<x<<"pu.\n";
    }
    if(is_three_winding_transformer())
    {
        complex<double> zjk = get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> zki = get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE);
        x = zjk.imag();
        /*if(fabs(r)>10.0)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence R between secondary and tertiary side is too great: "
                    <<"Rjk = "<<r<<"pu.\n";
        }*/
        if(fabs(x)>10.0)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence X between secondary and tertiary side is too great: "
                    <<"Xjk = "<<x<<"pu.\n";
        }
        if(fabs(x)<0.0001)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence X between secondary and tertiary side is too little: "
                    <<"Xjk = "<<x<<"pu.\n";
        }
        x = zki.imag();
        /*if(fabs(r)>10.0)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence R between tertiary and primary side is too great: "
                    <<"Rki = "<<r<<"pu.\n";
        }*/
        if(fabs(x)>10.0)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence X between tertiary and primary side is too great: "
                    <<"Xki = "<<x<<"pu.\n";
        }
        if(fabs(x)<0.0001)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence X between tertiary and primary side is too little: "
                    <<"Xki = "<<x<<"pu.\n";
        }
        if(zij==zjk)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence leakage impedance between primary and secondary side is equal to that between secondary and tertiary side: "
                    <<"Zij = "<<zij<<"pu, "<<"Zjk = "<<zjk<<"pu.\n";
        }
        if(zjk==zki)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence leakage impedance between secondary and tertiary side is equal to that between tertiary and primary side: "
                    <<"Zjk = "<<zjk<<"pu, "<<"Zki = "<<zki<<"pu.\n";
        }
        if(zki==zij)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence leakage impedance between tertiary and primary side is equal to that between primary and secondary side: "
                    <<"Zki = "<<zki<<"pu, "<<"Zij = "<<zij<<"pu.\n";
        }
        double sij = get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);
        double sjk = get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE);
        double ski = get_winding_nominal_capacity_in_MVA(TERTIARY_SIDE,PRIMARY_SIDE);
        double sbase = psdb.get_system_base_power_in_MVA();
        zij = zij/sij*sbase;
        zjk = zjk/sjk*sbase;
        zki = zki/ski*sbase;
        if(abs(zij+zki-zjk)<1e-6 or abs(zij+zjk-zki)<1e-6 or abs(zki+zjk-zij)<1e-6)
        {
            error_count++;
            osstream<<"("<<error_count<<") Positive sequence leakage impedance of at least one winding is or nearly zero: "
                    <<"Zij = "<<zij<<"pu, "<<"Zjk = "<<zjk<<"pu, "<<"Zki = "<<zki<<"pu. All based on system base power ("<<sbase<<"MVA).\n";
        }
    }
    if(error_count>0)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void TRANSFORMER::clear()
{
    winding_bus[PRIMARY_SIDE] = 0;
    winding_bus[SECONDARY_SIDE] = 0;
    winding_bus[TERTIARY_SIDE] = 0;
    /*winding_busptr[PRIMARY_SIDE] = NULL;
    winding_busptr[SECONDARY_SIDE] = NULL;
    winding_busptr[TERTIARY_SIDE] = NULL;*/
    set_identifier("");
    set_name("");
    set_winding_breaker_status(PRIMARY_SIDE, false);
    set_winding_breaker_status(SECONDARY_SIDE, false);
    set_winding_breaker_status(TERTIARY_SIDE, false);
    non_metered_end_bus = 0;

    winding_connection_type[PRIMARY_SIDE] = WYE_CONNECTION;
    winding_connection_type[SECONDARY_SIDE] = WYE_CONNECTION;
    winding_connection_type[TERTIARY_SIDE] = WYE_CONNECTION;

    winding_nominal_voltage_in_kV[PRIMARY_SIDE] = 0.0;
    winding_nominal_voltage_in_kV[SECONDARY_SIDE] = 0.0;
    winding_nominal_voltage_in_kV[TERTIARY_SIDE] = 0.0;

    winding_nominal_capacity_in_MVA[PRIMARY_SIDE] = 0.0;
    winding_nominal_capacity_in_MVA[SECONDARY_SIDE] = 0.0;
    winding_nominal_capacity_in_MVA[TERTIARY_SIDE] = 0.0;

    set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, complex<double>(0.0, 0.001));
    set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, complex<double>(0.0, 0.001));
    set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE, complex<double>(0.0, 0.001));

    set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(0.0);

    set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.0);
    set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.0);
    set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.0);

    set_winding_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    set_winding_angle_shift_in_deg(SECONDARY_SIDE, 0.0);
    set_winding_angle_shift_in_deg(TERTIARY_SIDE, 0.0);

    RATING rate;
    set_winding_rating_in_MVA(PRIMARY_SIDE, rate);
    set_winding_rating_in_MVA(SECONDARY_SIDE, rate);
    set_winding_rating_in_MVA(TERTIARY_SIDE, rate);

    set_winding_number_of_taps(PRIMARY_SIDE, DEFAULT_WINDING_NUMBER_OF_TAPS);
    set_winding_number_of_taps(SECONDARY_SIDE, DEFAULT_WINDING_NUMBER_OF_TAPS);
    set_winding_number_of_taps(TERTIARY_SIDE, DEFAULT_WINDING_NUMBER_OF_TAPS);

    set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 1.1);
    set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE, 0.9);
    set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 1.1);
    set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE, 0.9);
    set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 1.1);
    set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE, 0.9);

    set_winding_max_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    set_winding_min_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    set_winding_max_angle_shift_in_deg(SECONDARY_SIDE, 0.0);
    set_winding_min_angle_shift_in_deg(SECONDARY_SIDE, 0.0);
    set_winding_max_angle_shift_in_deg(TERTIARY_SIDE, 0.0);
    set_winding_min_angle_shift_in_deg(TERTIARY_SIDE, 0.0);

    set_winding_control_mode(PRIMARY_SIDE, TRANSFORMER_TAP_NO_CONTROL);
    set_winding_control_mode(SECONDARY_SIDE, TRANSFORMER_TAP_NO_CONTROL);
    set_winding_control_mode(TERTIARY_SIDE, TRANSFORMER_TAP_NO_CONTROL);

    winding_controlled_bus[PRIMARY_SIDE] = 0;
    winding_controlled_bus[SECONDARY_SIDE] = 0;
    winding_controlled_bus[TERTIARY_SIDE] = 0;

    set_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE, 1.1);
    set_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE, 0.9);
    set_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE, 1.1);
    set_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE, 0.9);
    set_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE, 1.1);
    set_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE, 0.9);

    set_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, 0.0);
    set_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, 0.0);
    set_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, 0.0);
    set_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, 0.0);
    set_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, 0.0);
    set_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, 0.0);

    set_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE, 0.0);
    set_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE, 0.0);
    set_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE, 0.0);
    set_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE, 0.0);
    set_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE, 0.0);
    set_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE, 0.0);

    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(PRIMARY_SIDE, 0.0);
    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(SECONDARY_SIDE, 0.0);
    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TERTIARY_SIDE, 0.0);

    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, 0.0);
    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, 0.0);
    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE, 0.0);

    set_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu(0.0);
}

bool TRANSFORMER::is_connected_to_bus(unsigned int bus) const
{
    if(bus != 0)
    {
        if(bus == get_winding_bus(PRIMARY_SIDE) or
           bus == get_winding_bus(SECONDARY_SIDE) or
           bus == get_winding_bus(TERTIARY_SIDE))
            return true;
        else
            return false;
    }
    else
        return false;
}

bool TRANSFORMER::is_in_area(unsigned int area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(is_two_winding_transformer())
    {
        BUS* busptr_pri = psdb.get_bus(get_winding_bus(PRIMARY_SIDE));
        BUS* busptr_sec = psdb.get_bus(get_winding_bus(SECONDARY_SIDE));
        if(busptr_pri!=NULL or busptr_sec!=NULL)
        {
            bool in_area = false;
            if(busptr_pri!=NULL)
                in_area = in_area or busptr_pri->is_in_area(area);
            if(busptr_sec!=NULL)
                in_area = in_area or busptr_sec->is_in_area(area);
            return in_area;
        }
        else
            return false;
    }
    else
    {
        BUS* busptr_pri = psdb.get_bus(get_winding_bus(PRIMARY_SIDE));
        BUS* busptr_sec = psdb.get_bus(get_winding_bus(SECONDARY_SIDE));
        BUS* busptr_ter = psdb.get_bus(get_winding_bus(TERTIARY_SIDE));
        if(busptr_pri!=NULL or busptr_sec!=NULL or busptr_ter!=NULL)
        {
            bool in_area = false;
            if(busptr_pri!=NULL)
                in_area = in_area or busptr_pri->is_in_area(area);
            if(busptr_sec!=NULL)
                in_area = in_area or busptr_sec->is_in_area(area);
            if(busptr_ter!=NULL)
                in_area = in_area or busptr_ter->is_in_area(area);
            return in_area;
        }
        else
            return false;
    }
}

bool TRANSFORMER::is_in_zone(unsigned int zone) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(is_two_winding_transformer())
    {
        BUS* busptr_pri = psdb.get_bus(get_winding_bus(PRIMARY_SIDE));
        BUS* busptr_sec = psdb.get_bus(get_winding_bus(SECONDARY_SIDE));
        if(busptr_pri!=NULL or busptr_sec!=NULL)
        {
            bool in_zone = false;
            if(busptr_pri!=NULL)
                in_zone = in_zone or busptr_pri->is_in_zone(zone);
            if(busptr_sec!=NULL)
                in_zone = in_zone or busptr_sec->is_in_zone(zone);
            return in_zone;
        }
        else
            return false;
    }
    else
    {
        BUS* busptr_pri = psdb.get_bus(get_winding_bus(PRIMARY_SIDE));
        BUS* busptr_sec = psdb.get_bus(get_winding_bus(SECONDARY_SIDE));
        BUS* busptr_ter = psdb.get_bus(get_winding_bus(TERTIARY_SIDE));
        if(busptr_pri!=NULL or busptr_sec!=NULL or busptr_ter!=NULL)
        {
            bool in_zone = false;
            if(busptr_pri!=NULL)
                in_zone = in_zone or busptr_pri->is_in_zone(zone);
            if(busptr_sec!=NULL)
                in_zone = in_zone or busptr_sec->is_in_zone(zone);
            if(busptr_ter!=NULL)
                in_zone = in_zone or busptr_ter->is_in_zone(zone);
            return in_zone;
        }
        else
            return false;
    }
}

void TRANSFORMER::report() const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(is_two_winding_transformer())
        osstream<<"Two winding transformer '"<<get_identifier()<<"' connecting to bus "
          <<get_winding_bus(PRIMARY_SIDE)<<"("<<get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)<<" kV) and "
          <<get_winding_bus(SECONDARY_SIDE)<<"("<<get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)<<" kV): "
          <<((get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==true)?"in service":"out of service")<<", "<<endl
          <<"R+jXl = "<<setw(6)<<setprecision(4)<<fixed<<get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE)<<" pu, "<<endl
          <<"Gm+jBm = "<<setw(6)<<setprecision(4)<<fixed<<get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu()<<" pu"<<endl
          <<"Primary-Secondary capacity = "<<setw(6)<<setprecision(4)<<fixed<<get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<" MVA";
    else
        osstream<<"Three winding transformer '"<<get_identifier()<<"' connecting to bus "
          <<get_winding_bus(PRIMARY_SIDE)<<"("<<get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)<<" kV), "
          <<get_winding_bus(SECONDARY_SIDE)<<"("<<get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)<<" kV) and "
          <<get_winding_bus(TERTIARY_SIDE)<<"("<<get_winding_nominal_voltage_in_kV(TERTIARY_SIDE)<<" kV): "
          <<((get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==true and get_winding_breaker_status(TERTIARY_SIDE)==true)?"in service":"out of service")<<", "<<endl
          <<"Primary to secondary R+jXl = "<<setw(6)<<setprecision(4)<<fixed<<get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE)<<" pu, "<<endl
          <<"Secondary to tertiary R+jXl = "<<setw(6)<<setprecision(4)<<fixed<<get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE)<<" pu, "<<endl
          <<"Primary to secondary R+jXl = "<<setw(6)<<setprecision(4)<<fixed<<get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, SECONDARY_SIDE)<<" pu, "<<endl
          <<"Gm+jBm = "<<setw(6)<<setprecision(4)<<fixed<<get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu()<<" pu"<<endl
          <<"Primary-Secondary capacity = "<<setw(6)<<setprecision(4)<<fixed<<get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<" MVA"<<endl
          <<"Secondary-Tertiary capacity = "<<setw(6)<<setprecision(4)<<fixed<<get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)<<" MVA"<<endl
          <<"Tertiary-Primary capacity = "<<setw(6)<<setprecision(4)<<fixed<<get_winding_nominal_capacity_in_MVA(TERTIARY_SIDE, PRIMARY_SIDE)<<" MVA"<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void TRANSFORMER::save() const
{
    ;
}

void TRANSFORMER::set_model(MODEL* model)
{
    ostringstream osstream;
    osstream<<"TRANSFORMER::"<<__FUNCTION__<<"() has not been implemented yet. Input model name is:"<<(model==NULL?"":model->get_model_name());
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

MODEL* TRANSFORMER::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="")
        return NULL;
    return NULL;
}

TRANSFORMER& TRANSFORMER::operator=(const TRANSFORMER& transformer)
{
    if(this == (&transformer)) return *this;

    set_toolkit(transformer.get_toolkit());
    clear();


    if(transformer.get_winding_bus(PRIMARY_SIDE)!=0)
        set_winding_bus(PRIMARY_SIDE, transformer.get_winding_bus(PRIMARY_SIDE));
    if(transformer.get_winding_bus(SECONDARY_SIDE)!=0)
        set_winding_bus(SECONDARY_SIDE, transformer.get_winding_bus(SECONDARY_SIDE));
    set_winding_bus(TERTIARY_SIDE, transformer.get_winding_bus(TERTIARY_SIDE));
    set_identifier(transformer.get_identifier());
    set_name(transformer.get_name());
    set_winding_breaker_status(PRIMARY_SIDE, transformer.get_winding_breaker_status(PRIMARY_SIDE));
    set_winding_breaker_status(SECONDARY_SIDE, transformer.get_winding_breaker_status(SECONDARY_SIDE));
    set_winding_breaker_status(TERTIARY_SIDE, transformer.get_winding_breaker_status(TERTIARY_SIDE));
    if(transformer.get_non_metered_end_bus()!=0)
        set_non_metered_end_bus(transformer.get_non_metered_end_bus());

    set_winding_connection_type(PRIMARY_SIDE, transformer.get_winding_connection_type(PRIMARY_SIDE));
    set_winding_connection_type(SECONDARY_SIDE, transformer.get_winding_connection_type(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_connection_type(TERTIARY_SIDE, transformer.get_winding_connection_type(TERTIARY_SIDE));

    set_winding_nominal_voltage_in_kV(PRIMARY_SIDE, transformer.get_winding_nominal_voltage_in_kV(PRIMARY_SIDE));
    set_winding_nominal_voltage_in_kV(SECONDARY_SIDE, transformer.get_winding_nominal_voltage_in_kV(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_nominal_voltage_in_kV(TERTIARY_SIDE, transformer.get_winding_nominal_voltage_in_kV(TERTIARY_SIDE));

    set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, transformer.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE, transformer.get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE, transformer.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE));

    set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE,
            transformer.get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE,
            transformer.get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE,
            transformer.get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE));

    set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(
            transformer.get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu());

    set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE,
            transformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE));
    set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE,
            transformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE,
            transformer.get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE));

    set_winding_angle_shift_in_deg(PRIMARY_SIDE, transformer.get_winding_angle_shift_in_deg(PRIMARY_SIDE));
    set_winding_angle_shift_in_deg(SECONDARY_SIDE, transformer.get_winding_angle_shift_in_deg(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_angle_shift_in_deg(TERTIARY_SIDE, transformer.get_winding_angle_shift_in_deg(TERTIARY_SIDE));

    set_winding_number_of_taps(PRIMARY_SIDE, transformer.get_winding_number_of_taps(PRIMARY_SIDE));
    set_winding_number_of_taps(SECONDARY_SIDE, transformer.get_winding_number_of_taps(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_number_of_taps(TERTIARY_SIDE, transformer.get_winding_number_of_taps(TERTIARY_SIDE));

    set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE,
            transformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE));
    set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE,
            transformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE));
    set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE,
            transformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE));
    set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE,
            transformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE,
            transformer.get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE,
            transformer.get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE));

    set_winding_max_angle_shift_in_deg(PRIMARY_SIDE, transformer.get_winding_max_angle_shift_in_deg(PRIMARY_SIDE));
    set_winding_min_angle_shift_in_deg(PRIMARY_SIDE, transformer.get_winding_min_angle_shift_in_deg(PRIMARY_SIDE));
    set_winding_max_angle_shift_in_deg(SECONDARY_SIDE, transformer.get_winding_max_angle_shift_in_deg(SECONDARY_SIDE));
    set_winding_min_angle_shift_in_deg(SECONDARY_SIDE, transformer.get_winding_min_angle_shift_in_deg(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_max_angle_shift_in_deg(TERTIARY_SIDE, transformer.get_winding_max_angle_shift_in_deg(TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_min_angle_shift_in_deg(TERTIARY_SIDE, transformer.get_winding_min_angle_shift_in_deg(TERTIARY_SIDE));

    set_winding_control_mode(PRIMARY_SIDE, transformer.get_winding_control_mode(PRIMARY_SIDE));
    set_winding_control_mode(SECONDARY_SIDE, transformer.get_winding_control_mode(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_control_mode(TERTIARY_SIDE, transformer.get_winding_control_mode(TERTIARY_SIDE));

    if(transformer.get_winding_controlled_bus(PRIMARY_SIDE)!=0)
        set_winding_controlled_bus(PRIMARY_SIDE, transformer.get_winding_controlled_bus(PRIMARY_SIDE));
    if(transformer.get_winding_controlled_bus(SECONDARY_SIDE)!=0)
        set_winding_controlled_bus(SECONDARY_SIDE, transformer.get_winding_controlled_bus(SECONDARY_SIDE));
    if(transformer.get_winding_controlled_bus(TERTIARY_SIDE)!=0)
        set_winding_controlled_bus(TERTIARY_SIDE, transformer.get_winding_controlled_bus(TERTIARY_SIDE));

    set_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE, transformer.get_winding_controlled_max_voltage_in_pu(PRIMARY_SIDE));
    set_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE, transformer.get_winding_controlled_min_voltage_in_pu(PRIMARY_SIDE));
    set_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE, transformer.get_winding_controlled_max_voltage_in_pu(SECONDARY_SIDE));
    set_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE, transformer.get_winding_controlled_min_voltage_in_pu(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE, transformer.get_winding_controlled_max_voltage_in_pu(TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE, transformer.get_winding_controlled_min_voltage_in_pu(TERTIARY_SIDE));

    set_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, transformer.get_controlled_max_reactive_power_into_winding_in_MVar(PRIMARY_SIDE));
    set_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE, transformer.get_controlled_min_reactive_power_into_winding_in_MVar(PRIMARY_SIDE));
    set_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, transformer.get_controlled_max_reactive_power_into_winding_in_MVar(SECONDARY_SIDE));
    set_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE, transformer.get_controlled_min_reactive_power_into_winding_in_MVar(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, transformer.get_controlled_max_reactive_power_into_winding_in_MVar(TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE, transformer.get_controlled_min_reactive_power_into_winding_in_MVar(TERTIARY_SIDE));

    set_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE, transformer.get_controlled_max_active_power_into_winding_in_MW(PRIMARY_SIDE));
    set_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE, transformer.get_controlled_min_active_power_into_winding_in_MW(PRIMARY_SIDE));
    set_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE, transformer.get_controlled_max_active_power_into_winding_in_MW(SECONDARY_SIDE));
    set_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE, transformer.get_controlled_min_active_power_into_winding_in_MW(SECONDARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE, transformer.get_controlled_max_active_power_into_winding_in_MW(TERTIARY_SIDE));
    if(transformer.is_three_winding_transformer())
        set_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE, transformer.get_controlled_min_active_power_into_winding_in_MW(TERTIARY_SIDE));

    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE,
        transformer.get_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE));
    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE,
        transformer.get_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE));
    set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE,
        transformer.get_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TERTIARY_SIDE, PRIMARY_SIDE));

    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(PRIMARY_SIDE,
        transformer.get_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(PRIMARY_SIDE));
    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(SECONDARY_SIDE,
        transformer.get_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(SECONDARY_SIDE));
    set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TERTIARY_SIDE,
        transformer.get_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TERTIARY_SIDE));

    set_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu(transformer.get_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu());
    return *this;
}


bool TRANSFORMER::is_two_winding_transformer() const
{
    if(get_winding_bus(TERTIARY_SIDE)==0) return true;
    else                                  return false;
}

bool TRANSFORMER::is_three_winding_transformer() const
{
    return not is_two_winding_transformer();
}

DEVICE_ID TRANSFORMER::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");

    TERMINAL terminal;
    terminal.append_bus(get_winding_bus(PRIMARY_SIDE));
    terminal.append_bus(get_winding_bus(SECONDARY_SIDE));
    if(is_three_winding_transformer())
        terminal.append_bus(get_winding_bus(TERTIARY_SIDE));

    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

complex<double> TRANSFORMER::get_star_bus_complex_voltage_in_pu() const
{
    if(is_two_winding_transformer())
        return get_two_winding_trans_star_bus_complex_voltage_in_pu();
    else
        return get_three_winding_trans_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_two_winding_trans_star_bus_complex_voltage_in_pu() const
{
    if(is_two_winding_transformer())
    {
        if(get_winding_breaker_status(PRIMARY_SIDE)==true or get_winding_breaker_status(SECONDARY_SIDE)==true)
        {
            ostringstream osstream;
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

            complex<double> Zps_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
            get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);

            complex<double> Zp, Zs;
            Zp = 0.5*Zps_original;
            Zs = Zp;

            complex<double> Y = get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

            complex<double> V_primary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(PRIMARY_SIDE));
            complex<double> V_secondary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(SECONDARY_SIDE));

            double tap_primary = get_primary_winding_off_nominal_turn_ratio_in_pu();
            double angle_primary = get_winding_angle_shift_in_deg(PRIMARY_SIDE);
            angle_primary = deg2rad(angle_primary);
            double tap_secondary = get_secondary_winding_off_nominal_turn_ratio_in_pu();
            double angle_secondary = get_winding_angle_shift_in_deg(SECONDARY_SIDE);
            angle_secondary = deg2rad(angle_secondary);

            complex<double> k_primary(tap_primary*steps_cos(angle_primary),tap_primary*steps_sin(angle_primary));
            complex<double> k_secondary(tap_secondary*steps_cos(angle_secondary),tap_secondary*steps_sin(angle_secondary));

            complex<double> Vp = V_primary/k_primary, Vs = V_secondary/k_secondary;

            complex<double> Yeq, Vstar, Istar;
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zp + 1.0/Zs;
                Istar = Vp/Zp + Vs/Zs;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==false)
            {
                Yeq = Y + 1.0/Zp;
                Istar = Vp/Zp;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==false and get_winding_breaker_status(SECONDARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zs;
                Istar = Vs/Zs;
            }
            Vstar = Istar/Yeq;
            return Vstar;
        }
        else
            return 0.0;
    }
    else
        return get_three_winding_trans_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_three_winding_trans_star_bus_complex_voltage_in_pu() const
{
    if(is_three_winding_transformer())
    {
        if(get_winding_breaker_status(PRIMARY_SIDE)==true or get_winding_breaker_status(SECONDARY_SIDE)==true or get_winding_breaker_status(TERTIARY_SIDE)==true)
        {
            ostringstream osstream;
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

            complex<double> Zps_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
            complex<double> Zst_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
            complex<double> Zpt_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);

            complex<double> Zp, Zs, Zt;
            Zp = 0.5*(Zps_original+Zpt_original-Zst_original);
            Zs = 0.5*(Zps_original+Zst_original-Zpt_original);
            Zt = 0.5*(Zpt_original+Zst_original-Zps_original);

            complex<double> Y = get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu();

            complex<double> V_primary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(PRIMARY_SIDE));
            complex<double> V_secondary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(SECONDARY_SIDE));
            complex<double> V_tertiary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(TERTIARY_SIDE));

            double tap_primary = get_primary_winding_off_nominal_turn_ratio_in_pu();
            double angle_primary = get_winding_angle_shift_in_deg(PRIMARY_SIDE);
            angle_primary = deg2rad(angle_primary);
            double tap_secondary = get_secondary_winding_off_nominal_turn_ratio_in_pu();
            double angle_secondary = get_winding_angle_shift_in_deg(SECONDARY_SIDE);
            angle_secondary = deg2rad(angle_secondary);
            double tap_tertiary = get_tertiary_winding_off_nominal_turn_ratio_in_pu();
            double angle_tertiary = get_winding_angle_shift_in_deg(TERTIARY_SIDE);
            angle_tertiary = deg2rad(angle_tertiary);

            complex<double> k_primary(tap_primary*steps_cos(angle_primary),tap_primary*steps_sin(angle_primary));
            complex<double> k_secondary(tap_secondary*steps_cos(angle_secondary),tap_secondary*steps_sin(angle_secondary));
            complex<double> k_tertiary(tap_tertiary*steps_cos(angle_tertiary),tap_tertiary*steps_sin(angle_tertiary));

            complex<double> Vp = V_primary/k_primary, Vs = V_secondary/k_secondary, Vt = V_tertiary/k_tertiary;

            complex<double> Yeq, Vstar, Istar;
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==true and get_winding_breaker_status(TERTIARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zp + 1.0/Zs + 1.0/Zt;
                Istar = Vp/Zp + Vs/Zs + Vt/Zt;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==true and get_winding_breaker_status(TERTIARY_SIDE)==false)
            {
                Yeq = Y + 1.0/Zp + 1.0/Zs;
                Istar = Vp/Zp + Vs/Zs;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==false and get_winding_breaker_status(TERTIARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zp + 1.0/Zt;
                Istar = Vp/Zp + Vt/Zt;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==true and get_winding_breaker_status(SECONDARY_SIDE)==false and get_winding_breaker_status(TERTIARY_SIDE)==false)
            {
                Yeq = Y + 1.0/Zp;
                Istar = Vp/Zp;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==false and get_winding_breaker_status(SECONDARY_SIDE)==true and get_winding_breaker_status(TERTIARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zs + 1.0/Zt;
                Istar = Vs/Zs + Vt/Zt;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==false and get_winding_breaker_status(SECONDARY_SIDE)==true and get_winding_breaker_status(TERTIARY_SIDE)==false)
            {
                Yeq = Y + 1.0/Zs;
                Istar = Vs/Zs;
            }
            if(get_winding_breaker_status(PRIMARY_SIDE)==false and get_winding_breaker_status(SECONDARY_SIDE)==false and get_winding_breaker_status(TERTIARY_SIDE)==true)
            {
                Yeq = Y + 1.0/Zt;
                Istar = Vt/Zt;
            }
            Vstar = Istar/Yeq;
            return Vstar;
        }
        else
            return 0.0;
    }
    else
        return get_two_winding_trans_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_star_bus_complex_voltage_in_kV_based_on_winding_nominal_voltage(TRANSFORMER_WINDING_SIDE side) const
{
    return get_winding_nominal_voltage_in_kV(side)*get_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_primary_winding_star_bus_complex_voltage_in_kV() const
{
    return get_winding_nominal_voltage_in_kV(PRIMARY_SIDE)*get_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_secondary_winding_star_bus_complex_voltage_in_kV() const
{
    return get_winding_nominal_voltage_in_kV(SECONDARY_SIDE)*get_star_bus_complex_voltage_in_pu();
}

complex<double> TRANSFORMER::get_tertiary_winding_star_bus_complex_voltage_in_kV() const
{
    if(is_three_winding_transformer())
        return get_winding_nominal_voltage_in_kV(TERTIARY_SIDE)*get_star_bus_complex_voltage_in_pu();
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_winding_complex_current_in_pu(TRANSFORMER_WINDING_SIDE side) const
{
    if(is_two_winding_transformer())
    {
        switch(side)
        {
            case PRIMARY_SIDE:
                return get_two_winding_trans_primary_winding_complex_current_in_pu();
            case SECONDARY_SIDE:
                return get_two_winding_trans_secondary_winding_complex_current_in_pu();
            default:
                return 0.0;
        }
    }
    else
    {
        switch(side)
        {
            case PRIMARY_SIDE:
                return get_three_winding_trans_primary_winding_complex_current_in_pu();
            case SECONDARY_SIDE:
                return get_three_winding_trans_secondary_winding_complex_current_in_pu();
            default:
                return get_three_winding_trans_tertiary_winding_complex_current_in_pu();
        }
    }
}

complex<double> TRANSFORMER::get_primary_winding_complex_current_in_pu() const
{
    if(is_two_winding_transformer())
        return get_two_winding_trans_primary_winding_complex_current_in_pu();
    else
        return get_three_winding_trans_primary_winding_complex_current_in_pu();
}

complex<double> TRANSFORMER::get_secondary_winding_complex_current_in_pu() const
{
    if(is_two_winding_transformer())
        return get_two_winding_trans_secondary_winding_complex_current_in_pu();
    else
        return get_three_winding_trans_secondary_winding_complex_current_in_pu();
}


complex<double> TRANSFORMER::get_tertiary_winding_complex_current_in_pu() const
{

    if(is_three_winding_transformer())
        return get_three_winding_trans_tertiary_winding_complex_current_in_pu();
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_two_winding_trans_primary_winding_complex_current_in_pu() const
{
    if(get_winding_breaker_status(PRIMARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> Z = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);

        Z *= 0.5;

        double tap_primary = get_primary_winding_off_nominal_turn_ratio_in_pu();
        double angle_primary = get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);

        complex<double> k_primary(tap_primary*steps_cos(angle_primary),tap_primary*steps_sin(angle_primary));

        complex<double> V_primary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(PRIMARY_SIDE));

        complex<double> Vp = V_primary/k_primary;

        complex<double> Vstar = get_two_winding_trans_star_bus_complex_voltage_in_pu();

        complex<double> I = (Vp-Vstar)/Z;

        return I/conj(k_primary);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_two_winding_trans_secondary_winding_complex_current_in_pu() const
{
    if(get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> Z = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);

        Z *= 0.5;

        double tap_secondary = get_secondary_winding_off_nominal_turn_ratio_in_pu();
        double angle_secondary = get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);

        complex<double> k_secondary(tap_secondary*steps_cos(angle_secondary),tap_secondary*steps_sin(angle_secondary));

        complex<double> V_secondary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(SECONDARY_SIDE));

        complex<double> Vs = V_secondary/k_secondary;

        complex<double> Vstar = get_two_winding_trans_star_bus_complex_voltage_in_pu();

        complex<double> I = (Vs-Vstar)/Z;

        return I/conj(k_secondary);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_three_winding_trans_primary_winding_complex_current_in_pu() const
{
    if(get_winding_breaker_status(PRIMARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> Zps_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);

        complex<double> Zp = 0.5*(Zps_original+Zpt_original-Zst_original);

        complex<double> V_primary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(PRIMARY_SIDE));

        double tap_primary = get_primary_winding_off_nominal_turn_ratio_in_pu();
        double angle_primary = get_winding_angle_shift_in_deg(PRIMARY_SIDE);
        angle_primary = deg2rad(angle_primary);

        complex<double> k_primary(tap_primary*steps_cos(angle_primary),tap_primary*steps_sin(angle_primary));

        complex<double> Vp = V_primary/k_primary;

        complex<double> Vstar = get_three_winding_trans_star_bus_complex_voltage_in_pu();

        complex<double> I = (Vp-Vstar)/Zp;

        return I/conj(k_primary);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_three_winding_trans_secondary_winding_complex_current_in_pu() const
{
    if(get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> Zps_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);

        complex<double> Zs = 0.5*(Zps_original+Zst_original-Zpt_original);

        complex<double> V_secondary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(SECONDARY_SIDE));

        double tap_secondary = get_secondary_winding_off_nominal_turn_ratio_in_pu();
        double angle_secondary = get_winding_angle_shift_in_deg(SECONDARY_SIDE);
        angle_secondary = deg2rad(angle_secondary);

        complex<double> k_secondary(tap_secondary*steps_cos(angle_secondary),tap_secondary*steps_sin(angle_secondary));

        complex<double> Vs = V_secondary/k_secondary;

        complex<double> Vstar = get_three_winding_trans_star_bus_complex_voltage_in_pu();

        complex<double> I = (Vs-Vstar)/Zs;

        return I/conj(k_secondary);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_three_winding_trans_tertiary_winding_complex_current_in_pu() const
{
    if(get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> Zps_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, SECONDARY_SIDE);
        complex<double> Zst_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(SECONDARY_SIDE, TERTIARY_SIDE);
        complex<double> Zpt_original = get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(PRIMARY_SIDE, TERTIARY_SIDE);

        complex<double> Zt = 0.5*(Zpt_original+Zst_original-Zps_original);

        complex<double> V_tertiary = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(TERTIARY_SIDE));

        double tap_tertiary = get_tertiary_winding_off_nominal_turn_ratio_in_pu();
        double angle_tertiary = get_winding_angle_shift_in_deg(TERTIARY_SIDE);
        angle_tertiary = deg2rad(angle_tertiary);

        complex<double> k_tertiary(tap_tertiary*steps_cos(angle_tertiary),tap_tertiary*steps_sin(angle_tertiary));

        complex<double> Vt = V_tertiary/k_tertiary;

        complex<double> Vstar = get_three_winding_trans_star_bus_complex_voltage_in_pu();

        complex<double> I = (Vt-Vstar)/Zt;

        return I/conj(k_tertiary);
    }
    else
        return 0.0;
}


complex<double> TRANSFORMER::get_winding_complex_current_in_kA(TRANSFORMER_WINDING_SIDE side) const
{
    switch(side)
    {
        case PRIMARY_SIDE:
            return get_primary_winding_complex_current_in_kA();
        case SECONDARY_SIDE:
            return get_secondary_winding_complex_current_in_kA();
        default:
            return get_tertiary_winding_complex_current_in_kA();
    }
}

complex<double> TRANSFORMER::get_primary_winding_complex_current_in_kA() const
{
    if(get_winding_breaker_status(PRIMARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(PRIMARY_SIDE));
        double Ibase = psdb.get_system_base_power_in_MVA()/(SQRT3*Vbase);

        return get_primary_winding_complex_current_in_pu()*Ibase;
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_secondary_winding_complex_current_in_kA() const
{
    if(get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(SECONDARY_SIDE));
        double Ibase = psdb.get_system_base_power_in_MVA()/(SQRT3*Vbase);

        return get_secondary_winding_complex_current_in_pu()*Ibase;
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_tertiary_winding_complex_current_in_kA() const
{
    if(is_three_winding_transformer() and get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(TERTIARY_SIDE));
        double Ibase = psdb.get_system_base_power_in_MVA()/(SQRT3*Vbase);

        return get_tertiary_winding_complex_current_in_pu()*Ibase;
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_winding_complex_power_in_pu(TRANSFORMER_WINDING_SIDE side) const
{
    switch(side)
    {
        case PRIMARY_SIDE:
            return get_primary_winding_complex_power_in_pu();
        case SECONDARY_SIDE:
            return get_secondary_winding_complex_power_in_pu();
        default:
            return get_tertiary_winding_complex_power_in_pu();
    }
}


complex<double> TRANSFORMER::get_primary_winding_complex_power_in_pu() const
{
    if(get_winding_breaker_status(PRIMARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> V, I;
        V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(PRIMARY_SIDE));
        I = get_primary_winding_complex_current_in_pu();
        return V*conj(I);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_secondary_winding_complex_power_in_pu() const
{
    if(get_winding_breaker_status(SECONDARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> V, I;
        V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(SECONDARY_SIDE));
        I = get_secondary_winding_complex_current_in_pu();
        return V*conj(I);
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_tertiary_winding_complex_power_in_pu() const
{
    if(is_three_winding_transformer() and get_winding_breaker_status(TERTIARY_SIDE)==true)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        complex<double> V, I;
        V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_winding_bus(TERTIARY_SIDE));
        I = get_tertiary_winding_complex_current_in_pu();
        return V*conj(I);
    }
    else
        return 0.0;
}


complex<double> TRANSFORMER::get_winding_complex_power_in_MVA(TRANSFORMER_WINDING_SIDE side) const
{
    switch(side)
    {
        case PRIMARY_SIDE:
            return get_primary_winding_complex_power_in_MVA();
        case SECONDARY_SIDE:
            return get_secondary_winding_complex_power_in_MVA();
        default:
            return get_tertiary_winding_complex_power_in_MVA();
    }
}

complex<double> TRANSFORMER::get_primary_winding_complex_power_in_MVA() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return get_primary_winding_complex_power_in_pu()*psdb.get_system_base_power_in_MVA();
}

complex<double> TRANSFORMER::get_secondary_winding_complex_power_in_MVA() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return get_secondary_winding_complex_power_in_pu()*psdb.get_system_base_power_in_MVA();
}

complex<double> TRANSFORMER::get_tertiary_winding_complex_power_in_MVA() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    return get_tertiary_winding_complex_power_in_pu()*psdb.get_system_base_power_in_MVA();
}

complex<double> TRANSFORMER::get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(winding1!=winding2)
    {
        if(winding1>winding2)
        {
            TRANSFORMER_WINDING_SIDE temp_winding;
            temp_winding = winding1;
            winding1 = winding2;
            winding2 = temp_winding;
        }
        double Sbase = 100.0;
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        Sbase = psdb.get_system_base_power_in_MVA();

        double Sbase_winding = get_winding_nominal_capacity_in_MVA(winding1, winding2);

        complex<double> Z = get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(winding1, winding2);

        return Z*Sbase/Sbase_winding;
    }
    else
    {
        osstream<<"Warning. The same windings ("<<get_winding_name(winding1)<<") are not allowed to get winding leakage impedance based on system base. Zero will be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
}

double TRANSFORMER::get_winding_off_nominal_turn_ratio_in_pu(TRANSFORMER_WINDING_SIDE side) const
{
    switch(side)
    {
        case PRIMARY_SIDE:
            return get_primary_winding_off_nominal_turn_ratio_in_pu();
        case SECONDARY_SIDE:
            return get_secondary_winding_off_nominal_turn_ratio_in_pu();
        default:
            return get_tertiary_winding_off_nominal_turn_ratio_in_pu();
    }
}


double TRANSFORMER::get_primary_winding_off_nominal_turn_ratio_in_pu() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Tr = get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(PRIMARY_SIDE);
    double Vnom = get_winding_nominal_voltage_in_kV(PRIMARY_SIDE);
    double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(PRIMARY_SIDE));

    return Tr*Vnom/Vbase;
}

double TRANSFORMER::get_secondary_winding_off_nominal_turn_ratio_in_pu() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Tr = get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(SECONDARY_SIDE);
    double Vnom = get_winding_nominal_voltage_in_kV(SECONDARY_SIDE);
    double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(SECONDARY_SIDE));

    return Tr*Vnom/Vbase;
}

double TRANSFORMER::get_tertiary_winding_off_nominal_turn_ratio_in_pu() const
{
    if(is_three_winding_transformer())
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        double Tr = get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TERTIARY_SIDE);
        double Vnom = get_winding_nominal_voltage_in_kV(TERTIARY_SIDE);
        double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(TERTIARY_SIDE));

        return Tr*Vnom/Vbase;
    }
    else
        return 0.0;
}

complex<double> TRANSFORMER::get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu() const
{
    complex<double> Y = get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu();

    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Vnom = get_winding_nominal_voltage_in_kV(PRIMARY_SIDE);
    double Vbase = psdb.get_bus_base_voltage_in_kV(get_winding_bus(PRIMARY_SIDE));

    return Y/(Vbase*Vbase)*(Vnom*Vnom);
}


