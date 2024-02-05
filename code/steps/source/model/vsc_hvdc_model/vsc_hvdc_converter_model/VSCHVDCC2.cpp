#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/VSCHVDCC2.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
VSCHVDCC2::VSCHVDCC2(STEPS& toolkit):VSC_HVDC_CONVERTER_MODEL(toolkit),
                                    udc_block(toolkit),
                                    p_sensor(toolkit), f_sensor(toolkit),
                                    omega_block(toolkit), angle_block(toolkit),
                                    dc_voltage_response_block(toolkit),
                                    frequency_response_block(toolkit), frequency_tuner(toolkit),
                                    active_current_reducer(toolkit),
                                    q_sensor(toolkit), uac_sensor(toolkit),
                                    voltage_block(toolkit),
                                    voltage_response_block(toolkit), voltage_tuner(toolkit),
                                    reactive_current_reducer(toolkit)
{
    clear();
}

VSCHVDCC2::VSCHVDCC2(const VSCHVDCC2& model):VSC_HVDC_CONVERTER_MODEL(model.get_toolkit()),
                                    udc_block(model.get_toolkit()),
                                    p_sensor(model.get_toolkit()), f_sensor(model.get_toolkit()),
                                    omega_block(model.get_toolkit()), angle_block(model.get_toolkit()),
                                    dc_voltage_response_block(model.get_toolkit()),
                                    frequency_response_block(model.get_toolkit()), frequency_tuner(model.get_toolkit()),
                                    active_current_reducer(model.get_toolkit()),
                                    q_sensor(model.get_toolkit()), uac_sensor(model.get_toolkit()),
                                    voltage_block(model.get_toolkit()),
                                    voltage_response_block(model.get_toolkit()), voltage_tuner(model.get_toolkit()),
                                    reactive_current_reducer(model.get_toolkit())
{
    clear();
    copy_from_const_model(model);
}

VSCHVDCC2::~VSCHVDCC2()
{
    ;
}

VSCHVDCC2& VSCHVDCC2::operator=(const VSCHVDCC2& model)
{
    if(this != &model)
        copy_from_const_model(model);
    return *this;
}

void VSCHVDCC2::copy_from_const_model(const VSCHVDCC2& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    clear();

    this->set_converter_name(model.get_converter_name());

    set_dc_voltage_block_ceq_in_uF(model.get_dc_voltage_block_ceq_in_uF());

    set_active_power_sensor_T_in_s(model.get_active_power_sensor_T_in_s());
    set_reactive_power_sensor_T_in_s(model.get_reactive_power_sensor_T_in_s());
    set_frequency_sensor_T_in_s(model.get_frequency_sensor_T_in_s());
    set_ac_voltage_sensor_T_in_s(model.get_ac_voltage_sensor_T_in_s());

    set_frequency_tuner_T1_in_s(model.get_frequency_tuner_T1_in_s());
    set_frequency_tuner_T2_in_s(model.get_frequency_tuner_T2_in_s());
    set_frequency_responser_Kp(model.get_frequency_responser_Kp());
    set_frequency_responser_Ki(model.get_frequency_responser_Ki());
    set_omega_block_Tj_in_s(model.get_omega_block_Tj_in_s());
    set_omega_block_D_in_pu(model.get_omega_block_D_in_pu());
    set_active_current_reducer_T_in_s(model.get_active_current_reducer_T_in_s());

    set_voltage_tuner_T1_in_s(model.get_voltage_tuner_T1_in_s());
    set_voltage_tuner_T2_in_s(model.get_voltage_tuner_T2_in_s());
    set_voltage_responser_Kp(model.get_voltage_responser_Kp());
    set_voltage_responser_Ki(model.get_voltage_responser_Ki());
    set_voltage_block_Tv_in_s(model.get_voltage_block_Tv_in_s());
    set_voltage_block_D_in_pu(model.get_voltage_block_D_in_pu());
    set_reactive_current_reducer_T_in_s(model.get_reactive_current_reducer_T_in_s());

    set_dc_voltage_Ku(model.get_dc_voltage_Ku());
    set_frequency_Kf(model.get_frequency_Kf());

    udc_block.set_toolkit(toolkit);

    p_sensor.set_toolkit(toolkit);
    q_sensor.set_toolkit(toolkit);
    f_sensor.set_toolkit(toolkit);
    uac_sensor.set_toolkit(toolkit);

    frequency_tuner.set_toolkit(toolkit);
    frequency_response_block.set_toolkit(toolkit);
    omega_block.set_toolkit(toolkit);
    angle_block.set_toolkit(toolkit);
    active_current_reducer.set_toolkit(toolkit);

    voltage_tuner.set_toolkit(toolkit);
    voltage_response_block.set_toolkit(toolkit);
    voltage_block.set_toolkit(toolkit);
    reactive_current_reducer.set_toolkit(toolkit);
}

string VSCHVDCC2::get_model_name() const
{
    return "VSCHVDCC2";
}

void VSCHVDCC2::set_dc_voltage_block_ceq_in_uF(const double ceq)
{
    udc_block.set_T_in_s(ceq*1e-6);
}

void VSCHVDCC2::set_active_power_sensor_T_in_s(double t)
{
    p_sensor.set_T_in_s(t);
}

void VSCHVDCC2::set_reactive_power_sensor_T_in_s(double t)
{
    q_sensor.set_T_in_s(t);
}

void VSCHVDCC2::set_frequency_sensor_T_in_s(double t)
{
    f_sensor.set_T_in_s(t);
}

void VSCHVDCC2::set_ac_voltage_sensor_T_in_s(double t)
{
    uac_sensor.set_T_in_s(t);
}

void VSCHVDCC2::set_frequency_tuner_T1_in_s(double t)
{
    frequency_tuner.set_T1_in_s(t);
}

void VSCHVDCC2::set_frequency_tuner_T2_in_s(double t)
{
    frequency_tuner.set_T2_in_s(t);
}

void VSCHVDCC2::set_frequency_responser_Kp(double k)
{
    frequency_response_block.set_Kp(k);
}

void VSCHVDCC2::set_frequency_responser_Ki(double k)
{
    frequency_response_block.set_Ki(k);
}

void VSCHVDCC2::set_omega_block_Tj_in_s(double t)
{
    omega_block.set_T_in_s(t);
}

void VSCHVDCC2::set_omega_block_D_in_pu(double d)
{
    D_omega = d;
}

void VSCHVDCC2::set_active_current_reducer_T_in_s(double t)
{
    active_current_reducer.set_T_in_s(t);
}

void VSCHVDCC2::set_voltage_tuner_T1_in_s(double t)
{
    voltage_tuner.set_T1_in_s(t);
}

void VSCHVDCC2::set_voltage_tuner_T2_in_s(double t)
{
    voltage_tuner.set_T2_in_s(t);
}

void VSCHVDCC2::set_voltage_responser_Kp(double k)
{
    voltage_response_block.set_Kp(k);
}

void VSCHVDCC2::set_voltage_responser_Ki(double k)
{
    voltage_response_block.set_Ki(k);
}

void VSCHVDCC2::set_voltage_block_Tv_in_s(double t)
{
    voltage_block.set_T_in_s(t);
}

void VSCHVDCC2::set_voltage_block_D_in_pu(double d)
{
    D_voltage = d;
}

void VSCHVDCC2::set_reactive_current_reducer_T_in_s(double t)
{
    reactive_current_reducer.set_T_in_s(t);
}

void VSCHVDCC2::set_dc_voltage_Ku(double k)
{
    ku = k;
}

void VSCHVDCC2::set_frequency_Kf(double k)
{
    kf = k;
}

double VSCHVDCC2::get_dc_voltage_block_ceq_in_uF() const
{
    return udc_block.get_T_in_s()*1e6;
}

double VSCHVDCC2::get_active_power_sensor_T_in_s() const
{
    return p_sensor.get_T_in_s();
}

double VSCHVDCC2::get_reactive_power_sensor_T_in_s() const
{
    return q_sensor.get_T_in_s();
}

double VSCHVDCC2::get_frequency_sensor_T_in_s() const
{
    return f_sensor.get_T_in_s();
}

double VSCHVDCC2::get_ac_voltage_sensor_T_in_s() const
{
    return uac_sensor.get_T_in_s();
}

double VSCHVDCC2::get_frequency_tuner_T1_in_s() const
{
    return frequency_tuner.get_T1_in_s();
}

double VSCHVDCC2::get_frequency_tuner_T2_in_s() const
{
    return frequency_tuner.get_T2_in_s();
}

double VSCHVDCC2::get_frequency_responser_Kp() const
{
    return frequency_response_block.get_Kp();
}

double VSCHVDCC2::get_frequency_responser_Ki() const
{
    return frequency_response_block.get_Ki();
}

double VSCHVDCC2::get_omega_block_Tj_in_s() const
{
    return omega_block.get_T_in_s();
}

double VSCHVDCC2::get_omega_block_D_in_pu() const
{
    return D_omega;
}

double VSCHVDCC2::get_active_current_reducer_T_in_s() const
{
    return active_current_reducer.get_T_in_s();
}

double VSCHVDCC2::get_voltage_tuner_T1_in_s() const
{
    return voltage_tuner.get_T1_in_s();
}

double VSCHVDCC2::get_voltage_tuner_T2_in_s() const
{
    return voltage_tuner.get_T2_in_s();
}

double VSCHVDCC2::get_voltage_responser_Kp() const
{
    return voltage_response_block.get_Kp();
}

double VSCHVDCC2::get_voltage_responser_Ki() const
{
    return voltage_response_block.get_Ki();
}

double VSCHVDCC2::get_voltage_block_Tv_in_s() const
{
    return voltage_block.get_T_in_s();
}

double VSCHVDCC2::get_voltage_block_D_in_pu() const
{
    return D_voltage;
}

double VSCHVDCC2::get_reactive_current_reducer_T_in_s() const
{
    return reactive_current_reducer.get_T_in_s();
}

double VSCHVDCC2::get_dc_voltage_Ku() const
{
    return ku;
}

double VSCHVDCC2::get_frequency_Kf() const
{
    return kf;
}

void VSCHVDCC2::set_Pref_in_pu(double pref)
{
    p_ref = pref;
}

void VSCHVDCC2::set_Qref_in_pu(double qref)
{
    q_ref = qref;
}

void VSCHVDCC2::set_Uacref_in_pu(double uacref)
{
    uac_ref = uacref;
}

double VSCHVDCC2::get_Pref_in_pu() const
{
    return p_ref;
}

double VSCHVDCC2::get_Qref_in_pu() const
{
    return q_ref;
}

double VSCHVDCC2::get_Uacref_in_pu() const
{
    return uac_ref;
}

double VSCHVDCC2::get_Porder_in_pu() const
{
    return get_Pref_in_pu() - frequency_response_block.get_output(); //- active_current_reducer.get_output();
}

double VSCHVDCC2::get_Qorder_in_pu() const
{
    return get_Qref_in_pu() + voltage_response_block.get_output(); //- reactive_current_reducer.get_output();
}

double VSCHVDCC2::get_dynamic_dc_voltage_in_kV() const
{
    return udc_block.get_output();
}

double VSCHVDCC2::get_dynamic_dc_voltage_in_pu() const
{
    return get_dynamic_dc_voltage_in_kV()/get_converter_dc_base_voltage_in_kV();
}

bool VSCHVDCC2::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=22)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            string converter_name = get_string_data(data[data_index],""); data_index++;
            set_converter_name(converter_name);

            double Ceq = get_double_data(data[data_index],"0.0"); data_index++;
            set_dc_voltage_block_ceq_in_uF(Ceq);

            double Tp = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_power_sensor_T_in_s(Tp);

            double Tq = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_power_sensor_T_in_s(Tq);

            double Tf = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_sensor_T_in_s(Tf);

            double Tuac = get_double_data(data[data_index],"0.0"); data_index++;
            set_ac_voltage_sensor_T_in_s(Tuac);

            double T1 = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_tuner_T1_in_s(T1);

            double T2 = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_tuner_T2_in_s(T2);

            double Kp = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_responser_Kp(Kp);

            double Ki = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_responser_Ki(Ki);

            double Tj = get_double_data(data[data_index],"0.0"); data_index++;
            set_omega_block_Tj_in_s(Tj);

            double D = get_double_data(data[data_index],"0.0"); data_index++;
            set_omega_block_D_in_pu(D);

            double Treduce = get_double_data(data[data_index],"0.0"); data_index++;
            set_active_current_reducer_T_in_s(Treduce);

            T1 = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_tuner_T1_in_s(T1);

            T2 = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_tuner_T2_in_s(T2);

            Kp = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_responser_Kp(Kp);

            Ki = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_responser_Ki(Ki);

            double Tv = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_block_Tv_in_s(Tv);

            D = get_double_data(data[data_index],"0.0"); data_index++;
            set_voltage_block_D_in_pu(D);

            Treduce = get_double_data(data[data_index],"0.0"); data_index++;
            set_reactive_current_reducer_T_in_s(Treduce);

            Kp = get_double_data(data[data_index],"0.0"); data_index++;
            set_dc_voltage_Ku(Kp);

            double Kf = get_double_data(data[data_index],"0.0"); data_index++;
            set_frequency_Kf(Kf);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool VSCHVDCC2::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSCHVDCC2::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSCHVDCC2::setup_block_toolkit_and_parameters()
{
    BUS* bus = get_converter_ac_bus_pointer();

    double fn = bus->get_base_frequency_in_Hz();
    double wn = DOUBLE_PI * fn;

    angle_block.set_T_in_s(1.0/wn);

    //dc_voltage_response_block.set_Kp(5);
    //dc_voltage_response_block.set_Ki(0);
}

void VSCHVDCC2::initialize()
{
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
        unsigned int converter_index=get_converter_index();

        double Udc_in_kV = vsc_hvdc->get_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(converter_index));

        udc_block.set_output(Udc_in_kV);
        udc_block.initialize();

        complex<double> S = get_converter_initial_S_from_converter_to_AC_bus_in_MVA();
        /*complex<double> I = get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_kA();
        complex<double> Z = get_converter_commutating_impedance_in_ohm();
        double Imag = abs(I);
        S += 3.0*Imag*Imag*Z;*/
        S /= get_converter_capacity_in_MVA();

        double P = S.real(), Q = S.imag();

        p_sensor.set_output(P);
        p_sensor.initialize();

        q_sensor.set_output(Q);
        q_sensor.initialize();

        f_sensor.set_output(0.0);
        f_sensor.initialize();

        double Uac = get_converter_ac_bus_voltage_in_pu();

        uac_sensor.set_output(Uac);
        uac_sensor.initialize();

        complex<double> Ipu = get_converter_initial_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_converter_base();
        complex<double> V = get_converter_ac_bus_complex_voltage_in_pu();
        complex<double> Z = get_converter_commutating_impedance_in_pu_on_converter_base();
        complex<double> E = V+Ipu*Z;

        angle_block.set_output(arg(E));
        angle_block.initialize();

        omega_block.set_output(0.0);
        omega_block.initialize();

        dc_voltage_response_block.set_output(0.0);
        dc_voltage_response_block.initialize();

        frequency_response_block.set_output(0.0);
        frequency_response_block.initialize();

        frequency_tuner.set_output(0.0);
        frequency_tuner.initialize();

        active_current_reducer.set_output(0.0);
        active_current_reducer.initialize();

        voltage_block.set_output(abs(E));
        voltage_block.initialize();

        voltage_response_block.set_output(0.0);
        voltage_response_block.initialize();

        voltage_tuner.set_output(0.0);
        voltage_tuner.initialize();

        reactive_current_reducer.set_output(0.0);
        reactive_current_reducer.initialize();

        set_Pref_in_pu(P);
        set_Qref_in_pu(Q);
        set_Uacref_in_pu(Uac);

        vdc_ref = Udc_in_kV;
    }
}

void VSCHVDCC2::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        STEPS& toolkit = get_toolkit();
        double time = toolkit.get_dynamic_simulation_time_in_s();
        VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
        unsigned int converter_index = get_converter_index();

        double Pdc_from_Ceq_to_dc_network = vsc_hvdc->get_converter_Pdc_from_Ceq_to_DC_network_in_MW(converter_index);
        double Pac_from_ac_network_to_Ceq = get_converter_dc_power_from_converter_to_Ceq_in_MW();
        double Udc = get_dynamic_dc_voltage_in_kV();

        double input;
        input = (-Pdc_from_Ceq_to_dc_network+Pac_from_ac_network_to_Ceq)/Udc;
        udc_block.set_input(input);
        udc_block.run(mode);

        vsc_hvdc->set_dc_bus_Vdc_in_kV(vsc_hvdc->get_dc_bus_index_with_converter_index(get_converter_index()),
                                       get_dynamic_dc_voltage_in_kV());

        complex<double> Is = get_converter_dynamic_current_from_converter_to_ac_bus_in_xy_axis_in_pu_on_converter_base();

        double Imag = abs(Is);

        double Imax = 0.001*vsc_hvdc->get_converter_rated_current_in_A(converter_index)*get_converter_dc_base_voltage_in_kV()/get_converter_capacity_in_MVA();
        input = Imag - Imax;
        active_current_reducer.set_input(input);
        active_current_reducer.run(mode);

        reactive_current_reducer.set_input(input);
        reactive_current_reducer.run(mode);

        complex<double> Us = get_converter_ac_bus_complex_voltage_in_pu();
        complex<double> S = Us*conj(Is);

        double Ps = S.real();
        double Qs = S.imag();

        p_sensor.set_input(Ps);
        p_sensor.run(mode);

        q_sensor.set_input(Qs);
        q_sensor.run(mode);

        double f = get_converter_ac_bus_frequency_deviation_in_pu();
        //cout<<"get_dc_voltage_Ku: "<<get_dc_voltage_Ku()<<"  get_frequency_Kf:  "<<get_frequency_Kf()<<endl;
        f = get_dc_voltage_Ku()*(Udc-vdc_ref)/get_converter_dc_base_voltage_in_kV()+get_frequency_Kf()*f;

        f_sensor.set_input(f);
        f_sensor.run(mode);

        double u = get_converter_ac_bus_voltage_in_pu();
        uac_sensor.set_input(u);
        uac_sensor.run(mode);

        input = f_sensor.get_output();
        frequency_tuner.set_input(input);
        frequency_tuner.run(mode);

        input = frequency_tuner.get_output();
        frequency_response_block.set_input(input);
        frequency_response_block.run(mode);

        double Porder = get_Porder_in_pu();
        double Pe = p_sensor.get_output();
        input = Porder - Pe - get_omega_block_D_in_pu()*omega_block.get_output();
        omega_block.set_input(input);
        omega_block.run(mode);

        input = omega_block.get_output();

        angle_block.set_input(input);
        angle_block.run(mode);

        input = get_Uacref_in_pu() - uac_sensor.get_output();
        voltage_tuner.set_input(input);
        voltage_tuner.run(mode);

        input = voltage_tuner.get_output();
        voltage_response_block.set_input(input);
        voltage_response_block.run(mode);

        double Qorder = get_Qorder_in_pu();
        double Qe = q_sensor.get_output();
        input = Qorder - Qe - get_voltage_block_D_in_pu()*voltage_block.get_output();
        voltage_block.set_input(input);
        voltage_block.run(mode);

        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

void VSCHVDCC2::check()
{

}

void VSCHVDCC2::clear()
{
    set_as_voltage_source();

    set_omega_block_D_in_pu(0.0);
    set_voltage_block_D_in_pu(0.0);

    active_current_reducer.set_limiter_type(WINDUP_LIMITER);
    active_current_reducer.set_upper_limit(INFINITE_THRESHOLD);
    active_current_reducer.set_lower_limit(0.0);

    // voltage_block.set_limiter_type(WINDUP_LIMITER);

    reactive_current_reducer.set_limiter_type(WINDUP_LIMITER);
    reactive_current_reducer.set_upper_limit(INFINITE_THRESHOLD);
    reactive_current_reducer.set_lower_limit(0.0);
}

void VSCHVDCC2::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSCHVDCC2::save()
{

}

string VSCHVDCC2::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    string vsc_name = "'"+vsc->get_name()+"'";

    string model_name = "'"+get_model_name()+"'";
    string converter_name = "'"+get_converter_name()+"'";

    osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<", "
            <<setw(10)<<converter_name<<", "
            <<setw(8)<<get_dc_voltage_block_ceq_in_uF()<<", "
            <<setw(8)<<get_active_power_sensor_T_in_s()<<", "
            <<setw(8)<<get_reactive_power_sensor_T_in_s()<<", "
            <<setw(8)<<get_frequency_sensor_T_in_s()<<", "
            <<setw(8)<<get_ac_voltage_sensor_T_in_s()<<endl
            <<setw(8)<<"       "
            <<setw(8)<<setprecision(3)<<get_frequency_tuner_T1_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_tuner_T2_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_responser_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_responser_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_omega_block_Tj_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_omega_block_D_in_pu()<<", "
            <<setw(8)<<setprecision(3)<<get_active_current_reducer_T_in_s()<<endl
            <<setw(8)<<"       "
            <<setw(8)<<setprecision(3)<<get_voltage_tuner_T1_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_voltage_tuner_T2_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_voltage_responser_Kp()<<", "
            <<setw(8)<<setprecision(3)<<get_voltage_responser_Ki()<<", "
            <<setw(8)<<setprecision(3)<<get_voltage_block_Tv_in_s()<<", "
            <<setw(8)<<setprecision(3)<<get_voltage_block_D_in_pu()<<", "
            <<setw(8)<<setprecision(3)<<get_reactive_current_reducer_T_in_s()<<endl
            <<setw(8)<<setprecision(3)<<get_dc_voltage_Ku()<<", "
            <<setw(8)<<setprecision(3)<<get_frequency_Kf()<<"/ ";

    return osstream.str();
}

void VSCHVDCC2::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TEST", i); i++;
}

double VSCHVDCC2::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return 0.0;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void VSCHVDCC2::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(is_model_data_exist(par_name))
    {
        if(par_name == "TEST") return;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double VSCHVDCC2::get_minimum_nonzero_time_constant_in_s()
{
    return 0.0;
}

void VSCHVDCC2::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("TEST STATE", i); i++;
}

double VSCHVDCC2::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TEST STATE") return 0.0;
    return 0.0;
}

string VSCHVDCC2::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSCHVDCC2::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string VSCHVDCC2::get_dynamic_data_in_steps_format() const
{
    return "";
}

double VSCHVDCC2::get_initial_angle_at_pll_in_rad() const
{
    return 0.0;
}

double VSCHVDCC2::get_dynamic_angle_at_pll_in_rad() const
{
    return 0.0;
}

complex<double> VSCHVDCC2::get_converter_voltage_in_xy_axis_in_pu_as_voltage_source() const
{
    double E = voltage_block.get_output();
    double angle = angle_block.get_output();

    return E * complex<double>(steps_cos(angle), steps_sin(angle));
}

complex<double> VSCHVDCC2::get_converter_dynamic_current_from_converter_to_ac_bus_in_dq_axis_in_pu_on_converter_base_as_current_source() const
{
    return 0.0;
}

double VSCHVDCC2::get_converter_dc_power_from_converter_to_Ceq_in_MW()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int converter_index = get_converter_index();

    complex<double> Is = get_converter_dynamic_current_from_converter_to_ac_bus_in_xy_axis_in_kA();
    complex<double> Us = get_converter_ac_bus_complex_voltage_in_kV();
    complex<double> Zc_in_ohm = vsc_hvdc->get_converter_commutating_impedance_in_ohm(converter_index);
    complex<double> Es = Us+Is*Zc_in_ohm*SQRT3;
    complex<double> S = Es*conj(Is)*SQRT3;

    double Ps = S.real();

    double c_loss = vsc_hvdc->get_converter_loss_factor_C_in_kW_per_amp_squard(converter_index);
    double b_loss = vsc_hvdc->get_converter_loss_factor_B_in_kW_per_amp(converter_index);
    double a_loss = vsc_hvdc->get_converter_loss_factor_A_in_kW(converter_index);
    double Is_mag = abs(Is)*1000;
    double P_converter_loss = (c_loss*Is_mag*Is_mag+b_loss*Is_mag+a_loss)/1000;
    double Pdc_from_dc_network_to_converter = Ps + P_converter_loss;

    return -Pdc_from_dc_network_to_converter;
}
