#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#define DEFAULT_WINDING_NUMBER_OF_TAPS 33

#include "header/device/nonbus_device.h"
#include "header/basic/steps_enum.h"
#include "header/basic/rating.h"
#include <string>
#include <complex>

using namespace std;

class TRANSFORMER : public NONBUS_DEVICE
{
    public:
        TRANSFORMER(STEPS& toolkit);
        virtual ~TRANSFORMER();

        void set_identifier(string identifier);
        void set_name(string name);
        void set_non_metered_end_bus(unsigned int bus);
        void set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double> y);

        void set_winding_bus(TRANSFORMER_WINDING_SIDE winding, unsigned int bus);
        void set_winding_breaker_status(TRANSFORMER_WINDING_SIDE winding, bool status);
        void set_winding_connection_type(TRANSFORMER_WINDING_SIDE winding, TRANSFORMER_WINDING_CONNECTION_TYPE wctype);
        void set_winding_nominal_voltage_in_kV(TRANSFORMER_WINDING_SIDE winding, double v);
        void set_winding_nominal_capacity_in_MVA(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, double s);
        void set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, complex<double> z);
        void set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio);
        void set_winding_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle);
        void set_winding_rating_in_MVA(TRANSFORMER_WINDING_SIDE winding, RATING rate);
        void set_winding_number_of_taps(TRANSFORMER_WINDING_SIDE winding, unsigned int n);
        void set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio_max);
        void set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double turn_ratio_min);
        void set_winding_max_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle_max);
        void set_winding_min_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding, double angle_min);
        void set_winding_control_mode(TRANSFORMER_WINDING_SIDE winding, TRANSFORMER_WINDING_CONTROL_MODE mode);
        void set_winding_controlled_bus(TRANSFORMER_WINDING_SIDE winding, unsigned int bus);
        void set_winding_controlled_max_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double v);
        void set_winding_controlled_min_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding, double v);
        void set_controlled_max_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding, double q);
        void set_controlled_min_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding, double q);
        void set_controlled_max_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding, double p);
        void set_controlled_min_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding, double p);
        void set_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding, complex<double> z);
        void set_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu(complex<double> z);
        void set_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2, complex<double> z);



        string get_identifier() const;
        string get_name() const;
        unsigned int get_non_metered_end_bus() const;
        complex<double> get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu() const;

        unsigned int get_winding_bus(TRANSFORMER_WINDING_SIDE winding) const;
        //BUS* get_winding_bus_pointer(TRANSFORMER_WINDING_SIDE winding) const;
        bool get_winding_breaker_status(TRANSFORMER_WINDING_SIDE winding) const;
        TRANSFORMER_WINDING_CONNECTION_TYPE get_winding_connection_type(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_nominal_voltage_in_kV(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_nominal_capacity_in_MVA(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const;
        complex<double> get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const;
        double get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const;
        RATING get_winding_rating_in_MVA(TRANSFORMER_WINDING_SIDE winding) const;
        unsigned int get_winding_number_of_taps(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_max_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_min_angle_shift_in_deg(TRANSFORMER_WINDING_SIDE winding) const;
        TRANSFORMER_WINDING_CONTROL_MODE get_winding_control_mode(TRANSFORMER_WINDING_SIDE winding) const;
        unsigned int get_winding_controlled_bus(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_controlled_max_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        double get_winding_controlled_min_voltage_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        double get_controlled_max_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding) const;
        double get_controlled_min_reactive_power_into_winding_in_MVar(TRANSFORMER_WINDING_SIDE winding) const;
        double get_controlled_max_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding) const;
        double get_controlled_min_active_power_into_winding_in_MW(TRANSFORMER_WINDING_SIDE winding) const;
        complex<double> get_winding_zero_sequence_impedance_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding) const;
        complex<double> get_common_zero_sequence_nutural_grounding_impedance_based_on_winding_nominals_in_pu() const;
        complex<double> get_zero_sequence_impedance_between_windings_based_on_winding_nominals_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const;


        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;
        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);
        virtual TRANSFORMER& operator=(const TRANSFORMER& transformer);

        bool is_two_winding_transformer() const;
        bool is_three_winding_transformer() const;

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        complex<double> get_leakage_impedance_between_windings_based_on_system_base_power_in_pu(TRANSFORMER_WINDING_SIDE winding1, TRANSFORMER_WINDING_SIDE winding2) const;
        complex<double> get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu() const;

        double get_winding_off_nominal_turn_ratio_in_pu(TRANSFORMER_WINDING_SIDE side) const;

        complex<double> get_star_bus_complex_voltage_in_pu() const;
        complex<double> get_star_bus_complex_voltage_in_kV_based_on_winding_nominal_voltage(TRANSFORMER_WINDING_SIDE side) const;

        complex<double> get_winding_complex_current_in_pu(TRANSFORMER_WINDING_SIDE side) const;

        complex<double> get_winding_complex_current_in_kA(TRANSFORMER_WINDING_SIDE side) const;

        complex<double> get_winding_complex_power_in_pu(TRANSFORMER_WINDING_SIDE side) const;

        complex<double> get_winding_complex_power_in_MVA(TRANSFORMER_WINDING_SIDE side) const;
    private:
        string get_winding_name(TRANSFORMER_WINDING_SIDE winding) const;

        complex<double> get_two_winding_trans_star_bus_complex_voltage_in_pu() const;
        complex<double> get_three_winding_trans_star_bus_complex_voltage_in_pu() const;

        double get_primary_winding_off_nominal_turn_ratio_in_pu() const;
        double get_secondary_winding_off_nominal_turn_ratio_in_pu() const;
        double get_tertiary_winding_off_nominal_turn_ratio_in_pu() const;

        complex<double> get_primary_winding_star_bus_complex_voltage_in_kV() const;
        complex<double> get_secondary_winding_star_bus_complex_voltage_in_kV() const;
        complex<double> get_tertiary_winding_star_bus_complex_voltage_in_kV() const;

        complex<double> get_primary_winding_complex_current_in_pu() const;
        complex<double> get_secondary_winding_complex_current_in_pu() const;
        complex<double> get_tertiary_winding_complex_current_in_pu() const;

        complex<double> get_primary_winding_complex_current_in_kA() const;
        complex<double> get_secondary_winding_complex_current_in_kA() const;
        complex<double> get_tertiary_winding_complex_current_in_kA() const;

        complex<double> get_primary_winding_complex_power_in_pu() const;
        complex<double> get_secondary_winding_complex_power_in_pu() const;
        complex<double> get_tertiary_winding_complex_power_in_pu() const;

        complex<double> get_primary_winding_complex_power_in_MVA() const;
        complex<double> get_secondary_winding_complex_power_in_MVA() const;
        complex<double> get_tertiary_winding_complex_power_in_MVA() const;

        complex<double> get_two_winding_trans_primary_winding_complex_current_in_pu() const;
        complex<double> get_two_winding_trans_secondary_winding_complex_current_in_pu() const;
        complex<double> get_three_winding_trans_primary_winding_complex_current_in_pu() const;
        complex<double> get_three_winding_trans_secondary_winding_complex_current_in_pu() const;
        complex<double> get_three_winding_trans_tertiary_winding_complex_current_in_pu() const;

        unsigned int winding_bus[3];
        //BUS* winding_busptr[3];
        unsigned int identifier_index, name_index;
        bool winding_breaker_status[3];
        unsigned int non_metered_end_bus;
        TRANSFORMER_WINDING_CONNECTION_TYPE winding_connection_type[3];
        double winding_nominal_capacity_in_MVA[3];
        double winding_nominal_voltage_in_kV[3];
        RATING winding_rating_in_MVA[3];

        complex<double> zl_primary2secondary_in_pu, zl_secondary2tertiary_in_pu,
                        zl_primary2tertiary_in_pu,  y_magnetizing_in_pu;

        double winding_turn_ratio_in_pu[3], winding_angle_shift_in_deg[3];

        unsigned int winding_number_of_taps[3];

        double winding_max_turn_ratio_in_pu[3], winding_min_turn_ratio_in_pu[3];

        double winding_max_angle_shift_in_deg[3], winding_min_angle_shift_in_deg[3];

        TRANSFORMER_WINDING_CONTROL_MODE winding_control_mode[3];

        unsigned int winding_controlled_bus[3];

        double winding_controlled_max_bus_voltage_in_pu[3], winding_controlled_min_bus_voltage_in_pu[3];

        double controlled_max_reactive_power_into_winding_in_MVar[3],
               controlled_min_reactive_power_into_winding_in_MVar[3];

        double controlled_max_active_power_into_winding_in_MW[3],
               controlled_min_active_power_into_winding_in_MW[3];

        complex<double> z0_winding_ground_in_pu[3], z0_common_nutral_ground_in_pu;
        complex<double> z0_primary2secondary_in_pu, z0_secondary2tertiary_in_pu,
                        z0_primary2tertiary_in_pu;
};
#endif // TRANSFORMER_H
