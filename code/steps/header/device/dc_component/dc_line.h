#ifndef DC_LINE_H
#define DC_LINE_H

#include "header/device/nonbus_device.h"
#include "header/basic/device_id.h"
#include "header/basic/rating.h"
#include "header/basic/fault.h"
#include "header/device/dc_component/dc_bus.h"

#include <complex>
#include <string>

using namespace std;

class DC_BUS;
class DC_LINE
{
    public:
        DC_LINE(STEPS& toolkit);
        virtual ~DC_LINE();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit();
        virtual bool is_connected_to_bus(unsigned int bus) const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();

        virtual void report() const;
        virtual void save() const;

        virtual DEVICE_ID get_device_id() const;

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        //set
        void set_sending_side_bus(const unsigned int bus);
        void set_receiving_side_bus(const unsigned int bus);
        void set_dc_line_name(const string name);
        void set_status(const bool status);

        void set_dc_network_simulation_time_step(const double t_dc);
        void set_line_R_in_ohm(const double r);
        void set_line_L_in_H(const double l);
        void set_line_C_in_uF(const double c);


        void set_Udc_ibus_in_kV(const double udc);
        void set_Udc_jbus_in_kV(const double udc);
        void set_sending_side_power(const double psend);
        void set_receiving_side_power(const double prec);
        void set_dc_line_current(const double idc);

        void set_i_RL_in_kA_on_sending_side(const double i1);
        void set_i_RL_in_kA_on_receiving_side(const double i2);
        void set_ic_in_kA_on_sending_side(const double ic1);
        void set_ic_in_kA_on_receiving_side(const double ic2);

        void set_dc_line_history_Ui_in_kV(const double Ui);
        void set_dc_line_history_Uj_in_kV(const double Uj);
        void set_dc_line_history_Uv_in_kV(const double Uv);

        //get
        unsigned int get_sending_side_bus_number() const;
        unsigned int get_receiving_side_bus_number() const;
        string get_dc_line_name() const;
        bool get_status() const;


        double get_dc_network_simulation_time_step() const;
        double get_line_R_in_ohm() const;
        double get_line_L_in_H() const;
        double get_line_C_in_uF() const;

        double get_Udc_ibus_in_kV() const;
        double get_Udc_jbus_in_kV() const;
        double get_sending_side_power() const;
        double get_receiving_side_power() const;

        //dynamic
        double get_i_RL_in_kA_on_sending_side();
        double get_i_RL_in_kA_on_receiving_side();
        double get_dc_line_current() const;
        double get_ic_in_kA_on_sending_side();
        double get_ic_in_kA_on_receiving_side();

        double get_dc_line_history_Ui_in_kV();
        double get_dc_line_history_Uj_in_kV();
        double get_dc_line_history_Uv_in_kV();

        double get_virtual_bus_voltage();
        double get_virtual_bus_current();

        //dynamic
        void set_dc_line_fault_location(double location);
        void set_dc_line_fault_R_in_ohm(double R);
        void set_dc_line_fault_L_in_H(double L);
        void set_dc_line_fault_C_in_uF(double C);

        double get_line_dynamic_y_of_RL_in_S_in_implicit_trapezoid_method(double m);
        double get_line_dynamic_injection_current_of_RL_in_kA_in_implicit_trapezoid_method(double m, double Ui, double Uj, double I_L);

        double get_line_dynamic_y_of_C_in_S_in_implicit_trapezoid_method();
        double get_line_dynamic_injection_current_of_C_in_kA_in_implicit_trapezoid_method(double Ui, double Ic);

        double get_line_dynamic_y_of_RL_in_S_in_implicit_euler_method(double m);
        double get_line_dynamic_injection_current_of_RL_in_kA_in_implicit_euler_method(double m, double Ui, double Uj, double I_L);

        double get_line_dynamic_y_of_C_in_S_in_implicit_euler_method();
        double get_line_dynamic_injection_current_of_C_in_kA_in_implicit_euler_method(double Ui, double Ic);

        DC_BUS* get_dc_line_virtual_bus();
        double get_dc_line_fault_location();
        double get_dc_line_fault_R_in_ohm();
        double get_dc_line_fault_L_in_H();
        double get_dc_line_fault_C_in_uF();

    private:
        STEPS* toolkit;

        unsigned int sending_side_bus;
        unsigned int receiving_side_bus;
        string name;
        bool status;
        double line_R_in_ohm;
        double line_L_in_H;
        double line_C_in_uF;

        double Udc_ibus, Udc_jbus;
        double idc;
        double P_send, P_rec;

        DC_BUS virtual_bus;
        double dc_simulation_time_step;
        double i_RL_sending_side,i_RL_receiving_side;
        double ic_sending_side,ic_reciving_side;
        double history_Ui,history_Uv,history_Uj;

        double dynamic_y_of_RL;
        double dynamic_y_of_C;

        double fault_location;
        double fault_R;
        double fault_L;
        double fault_C;

};

#endif // LINE_H
