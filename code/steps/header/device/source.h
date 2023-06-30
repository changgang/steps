#ifndef SOURCE_H
#define SOURCE_H

#include <string>
#include <complex>
#include "header/device/nonbus_device.h"
#include "header/basic/steps_enum.h"

using namespace std;

class BUS;

class SOURCE : public NONBUS_DEVICE
{
    public:
        SOURCE(STEPS& toolkit);
        virtual ~SOURCE();

        void set_source_bus(unsigned int bus);
        void set_identifier(string id);
        void set_status(bool status);
        void set_mbase_in_MVA(double mbase_MVA);
        void set_p_generation_in_MW(double p_MW);
        void set_q_generation_in_MVar(double q_MVar);
        void set_p_max_in_MW(double p_max_MW);
        void set_p_min_in_MW(double p_min_MW);
        void set_q_max_in_MVar(double q_max_MVar);
        void set_q_min_in_MVar(double q_min_MVar);
        void set_voltage_to_regulate_in_pu(double v_pu);
        void set_bus_to_regulate(unsigned int bus);
        void set_source_impedance_in_pu(const complex<double>& z_pu);
        void set_support_coefficient_of_reactive_current_during_LVRT(double K);
        void set_max_voltage_of_LVRT_strategy_in_pu(double v);
        void set_min_voltage_of_LVRT_strategy_in_pu(double v);
        void set_max_short_circuit_current_of_inverter_in_pu(double I);


        unsigned int get_source_bus() const;
        BUS* get_bus_pointer() const;
        string get_identifier() const;
        unsigned int get_identifier_index() const;
        bool get_status() const;
        double get_mbase_in_MVA() const;
        double get_one_over_mbase_in_one_over_MVA() const;
        double get_p_generation_in_MW() const;
        double get_q_generation_in_MVar() const;
        complex<double> get_complex_generation_in_MVA() const;
        double get_p_max_in_MW() const;
        double get_p_min_in_MW() const;
        double get_q_max_in_MVar() const;
        double get_q_min_in_MVar() const;
        double get_voltage_to_regulate_in_pu() const;
        unsigned int get_bus_to_regulate() const;
        complex<double> get_source_impedance_in_pu() const;
        double get_support_coefficient_of_reactive_current_during_LVRT() const;
        double get_max_voltage_of_LVRT_strategy_in_pu() const;
        double get_min_voltage_of_LVRT_strategy_in_pu() const;
        double get_max_short_circuit_current_of_inverter_in_pu() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;


        virtual DEVICE_ID get_device_id() const = 0;
        //virtual string get_compound_device_name() const = 0;

        virtual void report() const = 0;
        virtual void save() const = 0;

        virtual void set_model(MODEL* model) = 0;
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0) = 0;

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const = 0;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power() = 0;

        complex<double> get_complex_current_in_pu_during_LVRT();
    private:
        unsigned int source_identifier_index;
        double p_generation_MW, q_generation_MVar;
        BUS* busptr;

        unsigned int source_bus;
        unsigned int bus_to_regulate;

        complex<double> source_Z_pu;
        double mbase_MVA;
        double one_over_mbase;

        double p_max_MW, p_min_MW, q_max_MVar, q_min_MVar;
        double voltage_to_regulate_pu;
        bool status;

        double support_coefficient_of_reactive_current_during_LVRT,
                max_voltage_of_LVRT_strategy_in_pu,
                min_voltage_of_LVRT_strategy_in_pu,
                max_short_circuit_current_of_inverter_in_pu;
};
#endif // SOURCE_H
