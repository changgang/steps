#include "header/model/wtg_models/wt_electrical_model/wt3e1.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"

WT3E1::WT3E1(STEPS& toolkit) : WT_ELECTRICAL_MODEL(toolkit),
                               wind_turbine_speed_reference_sensor(toolkit),
                               torque_PI_regulator(toolkit),
                               conelec1(toolkit)
{
    clear();
}

WT3E1::WT3E1(const WT3E1& model) : WT_ELECTRICAL_MODEL(model.get_toolkit()),
                                   wind_turbine_speed_reference_sensor(model.get_toolkit()),
                                   torque_PI_regulator(model.get_toolkit()),
                                   conelec1(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3E1::~WT3E1()
{
    ;
}

WT3E1& WT3E1::operator=(const WT3E1& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void WT3E1::clear()
{
    set_model_float_parameter_count(36);

    set_transformer_from_bus(0);
    set_transformer_to_bus(0);
    set_transformer_id("");
    conelec1.clear();
    power_speed_table.clear();
}

void WT3E1::copy_from_const_model(const WT3E1& model)
{
    if(model.is_model_bypassed()) bypass_model();

    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    wind_turbine_speed_reference_sensor.set_toolkit(toolkit);
    torque_PI_regulator.set_toolkit(toolkit);

    clear();

    set_bus_to_regulate(model.get_bus_to_regulate());
    set_var_control_mode(model.get_var_control_mode());

    set_Xcomp_in_pu(model.get_Xcomp_in_pu());
    set_TRV_in_s(model.get_TRV_in_s());
    set_Fn(model.get_Fn());
    set_KIV(model.get_KIV());
    set_Qmax_in_pu(model.get_Qmax_in_pu());
    set_Qmin_in_pu(model.get_Qmin_in_pu());
    set_KPV(model.get_KPV());
    set_TV_in_s(model.get_TV_in_s());
    set_TFV_in_s(model.get_TFV_in_s());
    set_TP_in_s(model.get_TP_in_s());
    set_KQI(model.get_KQI());
    set_Vmax_in_pu(model.get_Vmax_in_pu());
    set_Vmin_in_pu(model.get_Vmin_in_pu());
    set_voltage_flag(model.get_voltage_flag());
    set_KQV(model.get_KQV());
    set_EQmax_in_pu(model.get_EQmax_in_pu());
    set_EQmin_in_pu(model.get_EQmin_in_pu());
    set_Tspeed_in_s(model.get_Tspeed_in_s());
    set_KPP(model.get_KPP());
    set_KIP(model.get_KIP());
    set_rPmax_in_pu(model.get_rPmax_in_pu());
    set_rPmin_in_pu(model.get_rPmin_in_pu());
    set_TFP_in_s(model.get_TFP_in_s());
    set_Pmax_in_pu(model.get_Pmax_in_pu());
    set_Pmin_in_pu(model.get_Pmin_in_pu());
    set_IPmax_in_pu(model.get_IPmax_in_pu());
}

string WT3E1::get_model_name() const
{
    return "WT3E1";
}

void WT3E1::set_bus_to_regulate(unsigned int bus)
{
    conelec1.set_bus_to_regulate(bus);
}

void WT3E1::set_var_control_mode(REACTIVE_POWER_CONTROL_MODE mode)
{
    conelec1.set_var_control_mode(mode);
}

void WT3E1::set_transformer_from_bus(unsigned int bus)
{
    transformer_from_bus = bus;
}

void WT3E1::set_transformer_to_bus(unsigned int bus)
{
    transformer_to_bus = bus;
}

void WT3E1::set_transformer_id(string id)
{
    transformer_id = id;
}

void WT3E1::set_Xcomp_in_pu(double Xc)
{
    conelec1.set_Xcomp_in_pu(Xc);
}

void WT3E1::set_TRV_in_s(double T)
{
    conelec1.set_TRV_in_s(T);
}

void WT3E1::set_Fn(double Fn)
{
    conelec1.set_Fn(Fn);
}

void WT3E1::set_KIV(double K)
{
    conelec1.set_KIV(K);
}

void WT3E1::set_Qmax_in_pu(double q)
{
    conelec1.set_Qmax_in_pu(q);
}

void WT3E1::set_Qmin_in_pu(double q)
{
    conelec1.set_Qmin_in_pu(q);
}

void WT3E1::set_KPV(double K)
{
    conelec1.set_KPV(K);
}

void WT3E1::set_TV_in_s(double T)
{
    conelec1.set_TV_in_s(T);
}

void WT3E1::set_TFV_in_s(double T)
{
    conelec1.set_TFV_in_s(T);
}

void WT3E1::set_TP_in_s(double T)
{
    conelec1.set_TP_in_s(T);
}

void WT3E1::set_KQI(double K)
{
    conelec1.set_KQI(K);
}

void WT3E1::set_Vmax_in_pu(double v)
{
    conelec1.set_Vmax_in_pu(v);
}

void WT3E1::set_Vmin_in_pu(double v)
{
    conelec1.set_Vmin_in_pu(v);
}

void WT3E1::set_voltage_flag(unsigned int flag)
{
    conelec1.set_voltage_flag(flag);
}

void WT3E1::set_KQV(double K)
{
    conelec1.set_KQV(K);
}

void WT3E1::set_EQmax_in_pu(double I)
{
    conelec1.set_EQmax_in_pu(I);
}

void WT3E1::set_EQmin_in_pu(double I)
{
    conelec1.set_EQmin_in_pu(I);
}

void WT3E1::set_Tspeed_in_s(double T)
{
    wind_turbine_speed_reference_sensor.set_T_in_s(T);
}

void WT3E1::set_KPP(double K)
{
    torque_PI_regulator.set_Kp(K);
}

void WT3E1::set_KIP(double K)
{
    torque_PI_regulator.set_Ki(K);
}

void WT3E1::set_rPmax_in_pu(double r)
{
    conelec1.set_rPmax_in_pu(r);
}

void WT3E1::set_rPmin_in_pu(double r)
{
    conelec1.set_rPmin_in_pu(r);
}

void WT3E1::set_TFP_in_s(double T)
{
    conelec1.set_TFP_in_s(T);
}

void WT3E1::set_Pmax_in_pu(double p)
{
    conelec1.set_Pmax_in_pu(p);
}

void WT3E1::set_Pmin_in_pu(double p)
{
    conelec1.set_Pmin_in_pu(p);
}

void WT3E1::set_IPmax_in_pu(double I)
{
    conelec1.set_IPmax_in_pu(I);
}

unsigned int WT3E1::get_bus_to_regulate()  const
{
    return conelec1.get_bus_to_regulate();
}

REACTIVE_POWER_CONTROL_MODE WT3E1::get_var_control_mode() const
{
    return conelec1.get_var_control_mode();
}

unsigned int WT3E1::get_transformer_from_bus() const
{
    return transformer_from_bus;
}

unsigned int WT3E1::get_transformer_to_bus() const
{
    return transformer_to_bus;
}

string WT3E1::get_transformer_id() const
{
    return transformer_id;
}

double WT3E1::get_Xcomp_in_pu() const
{
    return conelec1.get_Xcomp_in_pu();
}

double WT3E1::get_TRV_in_s() const
{
    return conelec1.get_TRV_in_s();
}

double WT3E1::get_Fn() const
{
    return conelec1.get_Fn();
}

double WT3E1::get_KIV() const
{
    return conelec1.get_KIV();
}

double WT3E1::get_Qmax_in_pu() const
{
    return conelec1.get_Qmax_in_pu();
}

double WT3E1::get_Qmin_in_pu() const
{
    return conelec1.get_Qmin_in_pu();
}

double WT3E1::get_KPV() const
{
    return conelec1.get_KPV();
}

double WT3E1::get_TV_in_s() const
{
    return conelec1.get_TV_in_s();
}

double WT3E1::get_TFV_in_s() const
{
    return conelec1.get_TFV_in_s();
}

double WT3E1::get_TP_in_s() const
{
    return conelec1.get_TP_in_s();
}

double WT3E1::get_KQI() const
{
    return conelec1.get_KQI();
}

double WT3E1::get_Vmax_in_pu() const
{
    return conelec1.get_Vmax_in_pu();
}

double WT3E1::get_Vmin_in_pu() const
{
    return conelec1.get_Vmin_in_pu();
}

unsigned int WT3E1::get_voltage_flag() const
{
    return conelec1.get_voltage_flag();
}

double WT3E1::get_KQV() const
{
    return conelec1.get_KQV();
}

double WT3E1::get_EQmax_in_pu() const
{
    return conelec1.get_EQmax_in_pu();
}

double WT3E1::get_EQmin_in_pu() const
{
    return conelec1.get_EQmin_in_pu();
}

double WT3E1::get_Tspeed_in_s() const
{
    return wind_turbine_speed_reference_sensor.get_T_in_s();
}

double WT3E1::get_KPP() const
{
    return torque_PI_regulator.get_Kp();
}

double WT3E1::get_KIP() const
{
    return torque_PI_regulator.get_Ki();
}

double WT3E1::get_rPmax_in_pu() const
{
    return conelec1.get_rPmax_in_pu();
}

double WT3E1::get_rPmin_in_pu() const
{
    return conelec1.get_rPmin_in_pu();
}

double WT3E1::get_TFP_in_s() const
{
    return conelec1.get_TFP_in_s();
}

double WT3E1::get_Pmax_in_pu() const
{
    return conelec1.get_Pmax_in_pu();
}

double WT3E1::get_Pmin_in_pu() const
{
    return conelec1.get_Pmin_in_pu();
}

double WT3E1::get_IPmax_in_pu() const
{
    return conelec1.get_IPmax_in_pu();
}

void WT3E1::add_wind_turbine_power_and_speed_pair_in_pu(double power, double speed)
{
    power_speed_table.add_x_and_y_pair(power, speed);
}

double WT3E1::get_wind_turbine_reference_speed_with_power_in_pu(double power)
{
    return power_speed_table.get_y_at_x(power);
}

bool WT3E1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=40)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int bus, var_control_flag, voltage_flag, trans_from_bus, trans_to_bus;
            string trans_id;

            double tfv, kpv, kiv, xc, tfp, kpp, kip, pmax, pmin, qmax, qmin,
                   ipmax, trv, rpmax, rpmin, tspeed, kqi, vmax, vmin,
                   kqv, eqmax, eqmin, tv, tp, fn,
                   wmin, wp20, wp40, wp60, pmin_at_wmin, wp100;

            unsigned int i=3;
            bus = get_integer_data(data[i],"0"); i++;
            var_control_flag = get_integer_data(data[i],"0"); i++;
            voltage_flag = get_integer_data(data[i],"0"); i++;
            trans_from_bus = get_integer_data(data[i],"0"); i++;
            trans_to_bus = get_integer_data(data[i],"0"); i++;
            trans_id = get_string_data(data[i],""); i++;
            tfv = get_double_data(data[i],"0.0"); i++;
            kpv = get_double_data(data[i],"0.0"); i++;
            kiv = get_double_data(data[i],"0.0"); i++;
            xc = get_double_data(data[i],"0.0"); i++;
            tfp = get_double_data(data[i],"0.0"); i++;
            kpp = get_double_data(data[i],"0.0"); i++;
            kip = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            qmax = get_double_data(data[i],"0.0"); i++;
            qmin = get_double_data(data[i],"0.0"); i++;
            ipmax = get_double_data(data[i],"0.0"); i++;
            trv = get_double_data(data[i],"0.0"); i++;
            rpmax = get_double_data(data[i],"0.0"); i++;
            rpmin = get_double_data(data[i],"0.0"); i++;
            tspeed = get_double_data(data[i],"0.0"); i++;
            kqi = get_double_data(data[i],"0.0"); i++;
            vmax = get_double_data(data[i],"0.0"); i++;
            vmin = get_double_data(data[i],"0.0"); i++;
            kqv = get_double_data(data[i],"0.0"); i++;
            eqmin = get_double_data(data[i],"0.0"); i++;
            eqmax = get_double_data(data[i],"0.0"); i++;
            tv = get_double_data(data[i],"0.0"); i++;
            tp = get_double_data(data[i],"0.0"); i++;
            fn = get_double_data(data[i],"0.0"); i++;
            wmin = get_double_data(data[i],"0.0"); i++;
            wp20 = get_double_data(data[i],"0.0"); i++;
            wp40 = get_double_data(data[i],"0.0"); i++;
            wp60 = get_double_data(data[i],"0.0"); i++;
            pmin_at_wmin = get_double_data(data[i],"0.0"); i++;
            wp100 = get_double_data(data[i],"0.0");

            set_bus_to_regulate(bus);
            REACTIVE_POWER_CONTROL_MODE mode;
            switch(var_control_flag)
            {
                case 0:
                {
                    mode = CONSTANT_VAR_MODE;
                    break;
                }
                case 1:
                {
                    mode = CONSTANT_POWER_FACTOR_MODE;
                    break;
                }
                case 2:
                {
                    mode = CONSTANT_VOLTAGE_MODE;
                    break;
                }
                default:
                {
                    mode = CONSTANT_VAR_MODE;
                    break;
                }
            }
            set_var_control_mode(mode);
            set_transformer_from_bus(trans_from_bus);
            set_transformer_to_bus(trans_to_bus);
            set_transformer_id(trans_id);
            set_Xcomp_in_pu(xc);
            set_TRV_in_s(trv);
            set_Fn(fn);
            set_KIV(kiv);
            set_Qmax_in_pu(qmax);
            set_Qmin_in_pu(qmin);
            set_KPV(kpv);
            set_TV_in_s(tv);
            set_TFV_in_s(tfv);
            set_TP_in_s(tp);
            set_KQI(kqi);
            set_Vmax_in_pu(vmax);
            set_Vmin_in_pu(vmin);

            if(voltage_flag>2)
                voltage_flag = 2;
            set_voltage_flag(voltage_flag);
            set_KQV(kqv);
            set_EQmax_in_pu(eqmax);
            set_EQmin_in_pu(eqmin);
            set_Tspeed_in_s(tspeed);
            set_KPP(kpp);
            set_KIP(kip);
            set_rPmax_in_pu(rpmax);
            set_rPmin_in_pu(rpmin);
            set_TFP_in_s(tfp);
            set_Pmax_in_pu(pmax);
            set_Pmin_in_pu(pmin);
            set_IPmax_in_pu(ipmax);

            add_wind_turbine_power_and_speed_pair_in_pu(pmin_at_wmin, wmin);
            add_wind_turbine_power_and_speed_pair_in_pu(0.2, wp20);
            add_wind_turbine_power_and_speed_pair_in_pu(0.4, wp40);
            add_wind_turbine_power_and_speed_pair_in_pu(0.6, wp60);
            add_wind_turbine_power_and_speed_pair_in_pu(1.0, wp100);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool WT3E1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3E1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3E1::setup_block_toolkit_and_parameters()
{
    conelec1.set_bus_pointer(get_bus_pointer());
    conelec1.set_device_pointer(get_device_pointer());
}

void WT3E1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        WT_GENERATOR_MODEL* gen_model = wt_generator->get_wt_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            double iterm = gen_model->get_terminal_current_in_pu_based_on_mbase();

            complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
            double pelec = selec.real();
            double qelec = selec.imag();
            double speed = get_wind_turbine_reference_speed_with_power_in_pu(pelec);

            double ipcmd = gen_model->get_initial_active_current_command_in_pu_based_on_mbase();
            double iqcmd = gen_model->get_initial_reactive_current_command_in_pu_based_on_mbase();
            double pcmd = gen_model->get_initial_active_power_command_in_pu_based_on_mbase();
            double qcmd = gen_model->get_initial_reactive_power_command_in_pu_based_on_mbase();
            double eqcmd = gen_model->get_initial_reactive_voltage_command_in_pu();
            double xsource = get_source_impedance_in_pu_based_on_mbase().imag();

            conelec1.set_terminal_current_in_pu_based_on_mbase(iterm);
            conelec1.set_active_power_generation_in_pu_based_on_mbase(pelec);
            conelec1.set_reactive_power_generation_in_pu_based_on_mbase(qelec);
            conelec1.set_initial_active_current_command_in_pu_based_on_mbase(ipcmd);
            conelec1.set_initial_active_power_command_in_pu_based_on_mbase(pcmd);
            conelec1.set_initial_reactive_current_command_in_pu_based_on_mbase(iqcmd);
            conelec1.set_initial_reactive_power_command_in_pu_based_on_mbase(qcmd);
            conelec1.set_initial_reactive_voltage_command_in_pu(eqcmd);

            conelec1.set_Xsource(xsource);

            conelec1.initialize();

            double torque =  pcmd/(1.0+speed);
            torque_PI_regulator.set_output(torque);
            torque_PI_regulator.initialize();

            wind_turbine_speed_reference_sensor.set_output(speed);
            wind_turbine_speed_reference_sensor.initialize();

            set_flag_model_initialized_as_true();
        }
    }
}

void WT3E1::run(DYNAMIC_MODE mode)
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* gen_model = wt_generator->get_wt_generator_model();
    WT_DBR_MODEL* dbr_model = wt_generator->get_wt_dbr_model();

    double iterm = gen_model->get_terminal_current_in_pu_based_on_mbase();

    complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();

    if(dbr_model!=NULL and dbr_model->is_model_active())
        pelec += dbr_model->get_braking_resistor_power_in_pu_based_on_mbase();

    double speed_ref = get_wind_turbine_reference_speed_with_power_in_pu(pelec);
    double speed = speed_ref;


    wind_turbine_speed_reference_sensor.set_input(speed_ref);
    wind_turbine_speed_reference_sensor.run(mode);

    double input = speed - wind_turbine_speed_reference_sensor.get_output();

    torque_PI_regulator.set_input(input);
    torque_PI_regulator.run(mode);

    input = torque_PI_regulator.get_output()*(1.0+speed);


    conelec1.set_active_power_reference_in_pu_based_on_mbase(input);
    conelec1.set_terminal_current_in_pu_based_on_mbase(iterm);
    conelec1.set_active_power_generation_in_pu_based_on_mbase(pelec);
    conelec1.set_reactive_power_generation_in_pu_based_on_mbase(qelec);
    conelec1.run(mode);

    if(mode == DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double WT3E1::get_active_current_command_in_pu_based_on_mbase()
{
    return conelec1.get_active_current_command_in_pu_based_on_mbase();
}

double WT3E1::get_active_power_command_in_pu_based_on_mbase() const
{
    return conelec1.get_active_power_command_in_pu_based_on_mbase();
}

double WT3E1::get_reactive_current_command_in_pu_based_on_mbase()
{
    return conelec1.get_reactive_current_command_in_pu_based_on_mbase();
}

double WT3E1::get_reactive_power_command_in_pu_based_on_mbase()
{
    return conelec1.get_reactive_power_command_in_pu_based_on_mbase();
}

double WT3E1::get_reactive_voltage_command_in_pu() const
{
    return conelec1.get_reactive_voltage_command_in_pu();
}

void WT3E1::check()
{
    ;
}

void WT3E1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3E1::save()
{
    ;
}

string WT3E1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    WT_GENERATOR* source = get_wt_generator_pointer();
    unsigned int bus = source->get_source_bus();
    string identifier = "'"+source->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    unsigned int bus_reg = get_bus_to_regulate();
    REACTIVE_POWER_CONTROL_MODE mode = get_var_control_mode();
    int var_mode = (mode==CONSTANT_VAR_MODE)? 0:(mode==CONSTANT_POWER_FACTOR_MODE? -1 : 1);
    unsigned int voltage_flag = get_voltage_flag();
    unsigned int trans_from_bus = get_transformer_from_bus();
    unsigned int trans_to_bus = get_transformer_to_bus();
    string trans_id = "'"+get_transformer_id()+"'";
    double tfv = get_TFV_in_s();
    double kpv = get_KPV();
    double kiv = get_KIV();
    double xc = get_Xcomp_in_pu();
    double tfp = get_TFP_in_s();
    double kpp = get_KPP();
    double kip = get_KIP();
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    double qmax = get_Qmax_in_pu();
    double qmin = get_Qmin_in_pu();
    double ipmax = get_IPmax_in_pu();
    double trv = get_TRV_in_s();
    double rpmax = get_rPmax_in_pu();
    double rpmin = get_rPmin_in_pu();
    double tspeed = get_Tspeed_in_s();
    double kqi = get_KQI();
    double vmax = get_Vmax_in_pu();
    double vmin = get_Vmin_in_pu();
    double kqv = get_KQV();
    double eqmin = get_EQmin_in_pu();
    double eqmax = get_EQmax_in_pu();
    double tv = get_TV_in_s();
    double tp = get_TP_in_s();
    double fn = get_Fn();

    double wmin = power_speed_table.get_point_y_with_index(0);
    double wp20 = power_speed_table.get_point_y_with_index(1);
    double wp40 = power_speed_table.get_point_y_with_index(2);
    double wp60 = power_speed_table.get_point_y_with_index(3);
    double pmin_at_wmin = power_speed_table.get_point_x_with_index(0);
    double wp100 = power_speed_table.get_point_y_with_index(4);

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
    {
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;
        if(bus_reg!=0) bus_reg = network.get_internal_bus_number_of_physical_bus(bus_reg)+1;
        if(trans_from_bus!=0) trans_from_bus = network.get_internal_bus_number_of_physical_bus(trans_from_bus)+1;
        if(trans_to_bus!=0) trans_to_bus = network.get_internal_bus_number_of_physical_bus(trans_to_bus)+1;
    }


    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<bus_reg<<", "
            <<setw(8)<<var_mode<<", "
            <<setw(8)<<voltage_flag<<", "
            <<setw(8)<<trans_from_bus<<", "
            <<setw(8)<<trans_to_bus<<", "
            <<setw(8)<<trans_id<<", "
            <<setw(8)<<setprecision(6)<<tfv<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kpv<<", "
            <<setw(8)<<setprecision(6)<<kiv<<", "
            <<setw(8)<<setprecision(6)<<xc<<", "
            <<setw(8)<<setprecision(6)<<tfp<<", "
            <<setw(8)<<setprecision(6)<<kpp<<", "
            <<setw(8)<<setprecision(6)<<kip<<", "
            <<setw(8)<<setprecision(6)<<pmax<<", "
            <<setw(8)<<setprecision(6)<<pmin<<", "
            <<setw(8)<<setprecision(6)<<qmax<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<qmin<<", "
            <<setw(8)<<setprecision(6)<<ipmax<<", "
            <<setw(8)<<setprecision(6)<<trv<<", "
            <<setw(8)<<setprecision(6)<<rpmax<<", "
            <<setw(8)<<setprecision(6)<<rpmin<<", "
            <<setw(8)<<setprecision(6)<<tspeed<<", "
            <<setw(8)<<setprecision(6)<<kqi<<", "
            <<setw(8)<<setprecision(6)<<vmax<<", "
            <<setw(8)<<setprecision(6)<<vmin<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kqv<<", "
            <<setw(8)<<setprecision(6)<<eqmin<<", "
            <<setw(8)<<setprecision(6)<<eqmax<<", "
            <<setw(8)<<setprecision(6)<<tv<<", "
            <<setw(8)<<setprecision(6)<<tp<<", "
            <<setw(8)<<setprecision(6)<<fn<<", "
            <<setw(8)<<setprecision(6)<<wmin<<", "
            <<setw(8)<<setprecision(6)<<wp20<<", "
            <<setw(8)<<setprecision(6)<<wp40<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<wp60<<", "
            <<setw(8)<<setprecision(6)<<pmin_at_wmin<<", "
            <<setw(8)<<setprecision(6)<<wp100<<" /";

    return osstream.str();
}

void WT3E1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("BUS TO REGULATE", i); i++;
    add_model_data_name_and_index_pair("VAR CONTROL FLAG", i); i++;
    add_model_data_name_and_index_pair("VOLTAGE FLAG", i); i++;
    add_model_data_name_and_index_pair("XCOMP IN PU", i); i++;
    add_model_data_name_and_index_pair("T VOLTAGE SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("FN", i); i++;
    add_model_data_name_and_index_pair("KP VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("TP VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("KI VOLTAGE ERROR", i); i++;
    add_model_data_name_and_index_pair("QMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("QMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("T REACTIVE POWER FILTER IN S", i); i++;
    add_model_data_name_and_index_pair("T ACTIVE POWER SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("KI REACTIVE POWER ERROR", i); i++;
    add_model_data_name_and_index_pair("VMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("VMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("KI VOLTAGE COMMAND", i); i++;
    add_model_data_name_and_index_pair("EQMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("EQMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("T SPEED SENSOR IN S", i); i++;
    add_model_data_name_and_index_pair("KP SPEED", i); i++;
    add_model_data_name_and_index_pair("KI SPEED", i); i++;
    add_model_data_name_and_index_pair("K VIRTUAL INERTIA", i); i++;
    add_model_data_name_and_index_pair("T VIRTUAL INERTIA IN S", i); i++;
    add_model_data_name_and_index_pair("K FREQUENCY DROOP", i); i++;
    add_model_data_name_and_index_pair("T FREQUENCY DROOP IN S", i); i++;
    add_model_data_name_and_index_pair("F LOWER IN PU", i); i++;
    add_model_data_name_and_index_pair("F UPPER IN PU", i); i++;
    add_model_data_name_and_index_pair("K SECONDARY FREQUENCY REGULATION", i); i++;
    add_model_data_name_and_index_pair("P RATE MIN IN PU/S", i); i++;
    add_model_data_name_and_index_pair("P RATE MAX IN PU/S", i); i++;
    add_model_data_name_and_index_pair("T ACTIVE POWER COMMAND IN S", i); i++;
    add_model_data_name_and_index_pair("PMIN IN PU", i); i++;
    add_model_data_name_and_index_pair("PMAX IN PU", i); i++;
    add_model_data_name_and_index_pair("ACTIVE CURRENT MAX IN PU", i); i++;
}

double WT3E1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0.0;

    return 0.0;
}

void WT3E1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return;

    return;
}

double WT3E1::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_TRV_in_s()!=0.0 and mint>get_TRV_in_s())
        mint = get_TRV_in_s();
    if(get_TV_in_s()!=0.0 and mint>get_TV_in_s())
        mint = get_TV_in_s();
    if(get_TFV_in_s()!=0.0 and mint>get_TFV_in_s())
        mint = get_TFV_in_s();
    if(get_TP_in_s()!=0.0 and mint>get_TP_in_s())
        mint = get_TP_in_s();
    if(get_Tspeed_in_s()!=0.0 and mint>get_Tspeed_in_s())
        mint = get_Tspeed_in_s();
    if(get_TFP_in_s()!=0.0 and mint>get_TFP_in_s())
        mint = get_TFP_in_s();
    return mint;
}

void WT3E1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
}

double WT3E1::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string WT3E1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3E1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3E1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
