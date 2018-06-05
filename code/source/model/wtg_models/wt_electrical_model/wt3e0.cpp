#include "header/model/wtg_models/wt_electrical_model/wt3e0.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

WT3E0::WT3E0()
{
    clear();
}

WT3E0::WT3E0(const WT3E0& model)
{
    copy_from_const_model(model);
}

WT3E0::~WT3E0()
{
    ;
}

WT3E0& WT3E0::operator=(const WT3E0& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}

void WT3E0::copy_from_const_model(const WT3E0& model)
{
    clear();

    set_bus_to_regulate(model.get_bus_to_regulate());
    set_var_control_mode(model.get_var_control_mode());
    set_wind_turbine_power_speed_lookup_table(model.get_wind_turbine_power_speed_lookup_table());

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
    set_Kvi(model.get_Kvi());
    set_Tvi_in_s(model.get_Tvi_in_s());
    set_Kdroop(model.get_Kdroop());
    set_Tdroop_in_s(model.get_Tdroop_in_s());
    set_rPmax_in_pu(model.get_rPmax_in_pu());
    set_rPmin_in_pu(model.get_rPmin_in_pu());
    set_TFP_in_s(model.get_TFP_in_s());
    set_Pmax_in_pu(model.get_Pmax_in_pu());
    set_Pmin_in_pu(model.get_Pmin_in_pu());
    set_IPmax_in_pu(model.get_IPmax_in_pu());
}

string WT3E0::get_model_name() const
{
    return "WT3E0";
}

void WT3E0::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void WT3E0::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void WT3E0::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void WT3E0::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void WT3E0::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void WT3E0::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void WT3E0::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void WT3E0::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void WT3E0::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void WT3E0::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void WT3E0::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void WT3E0::set_voltage_flag(size_t flag)
{
    if(flag<3)
        Voltage_Flag = flag;
    else
    {
        ostringstream osstream;
        osstream<<"Error. "<<flag<<" is not allowed to set up voltage flag for "<<get_model_name()<<" model. 0, 1, or 2 is allowed.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void WT3E0::set_KQV(double K)
{
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void WT3E0::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void WT3E0::set_EQmin_in_pu(double I)
{
    EQmin = I;
}

void WT3E0::set_Tspeed_in_s(double T)
{
    wind_turbine_speed_reference_sensor.set_T_in_s(T);
}

void WT3E0::set_KPP(double K)
{
    torque_PI_regulator.set_Kp(K);
}

void WT3E0::set_KIP(double K)
{
    torque_PI_regulator.set_Ki(K);
}

void WT3E0::set_Kvi(double K)
{
    virtual_inertia_emulator.set_K(K);
}

void WT3E0::set_Tvi_in_s(double T)
{
    virtual_inertia_emulator.set_T_in_s(T);
}

void WT3E0::set_Kdroop(double K)
{
    frequency_droop_controller.set_K(K);
}

void WT3E0::set_Tdroop_in_s(double T)
{
    frequency_droop_controller.set_T_in_s(T);
}

void WT3E0::set_rPmax_in_pu(double r)
{
    max_torque_rate = r;
}

void WT3E0::set_rPmin_in_pu(double r)
{
    min_torque_rate = r;
}

void WT3E0::set_TFP_in_s(double T)
{
    power_order_integrator.set_T_in_s(T);
}

void WT3E0::set_Pmax_in_pu(double p)
{
    power_order_integrator.set_upper_limit(p);
}

void WT3E0::set_Pmin_in_pu(double p)
{
    power_order_integrator.set_lower_limit(p);
}

void WT3E0::set_IPmax_in_pu(double I)
{
    IPmax = I;
}

double WT3E0::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double WT3E0::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double WT3E0::get_Fn() const
{
    return Fn;
}

double WT3E0::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double WT3E0::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double WT3E0::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double WT3E0::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double WT3E0::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double WT3E0::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double WT3E0::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double WT3E0::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double WT3E0::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double WT3E0::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

size_t WT3E0::get_voltage_flag() const
{
    return Voltage_Flag;
}

double WT3E0::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double WT3E0::get_EQmax_in_pu() const
{
    return EQmax;
}

double WT3E0::get_EQmin_in_pu() const
{
    return EQmin;
}

double WT3E0::get_Tspeed_in_s() const
{
    return wind_turbine_speed_reference_sensor.get_T_in_s();
}

double WT3E0::get_KPP() const
{
    return torque_PI_regulator.get_Kp();
}

double WT3E0::get_KIP() const
{
    return torque_PI_regulator.get_Ki();
}

double WT3E0::get_Kvi() const
{
    return virtual_inertia_emulator.get_K();
}

double WT3E0::get_Tvi_in_s() const
{
    return virtual_inertia_emulator.get_T_in_s();
}

double WT3E0::get_Kdroop() const
{
    return frequency_droop_controller.get_K();
}

double WT3E0::get_Tdroop_in_s() const
{
    return frequency_droop_controller.get_T_in_s();
}

double WT3E0::get_rPmax_in_pu() const
{
    return max_torque_rate;
}

double WT3E0::get_rPmin_in_pu() const
{
    return min_torque_rate;
}

double WT3E0::get_TFP_in_s() const
{
    return power_order_integrator.get_T_in_s();
}

double WT3E0::get_Pmax_in_pu() const
{
    return power_order_integrator.get_upper_limit();
}

double WT3E0::get_Pmin_in_pu() const
{
    return power_order_integrator.get_lower_limit();
}

double WT3E0::get_IPmax_in_pu() const
{
    return IPmax;
}

void WT3E0::set_speed_reference_bias_in_pu(double bias)
{
    speedref_bias =  bias;
}

double WT3E0::get_speed_reference_bias_in_pu() const
{
    return speedref_bias;
}

double WT3E0::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double WT3E0::get_double_data_with_name(string par_name) const
{
    return 0.0;
}

void WT3E0::set_double_data_with_index(size_t index, double value)
{
    ;
}

void WT3E0::set_double_data_with_name(string par_name, double value)
{
    ;
}

bool WT3E0::setup_model_with_steps_string(string data)
{
    return false;
}

bool WT3E0::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<35)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    size_t bus, voltage_flag;
    int var_control_flag;
    double tfv, kpv, kiv, xc, tfp, kpp, kip, pmax, pmin, qmax, qmin,
           ipmax, trv, rpmax, rpmin, tspeed, kqi, vmax, vmin,
           kqv, eqmax, eqmin, tv, tp, fn,
           kvi, tvi, kdroop, tdroop;

    size_t i=3;
    bus = get_integer_data(dyrdata[i],"0"); i++;
    var_control_flag = get_integer_data(dyrdata[i],"0"); i++;
    voltage_flag = size_t(get_integer_data(dyrdata[i],"0")); i++;
    xc = get_double_data(dyrdata[i],"0.0"); i++;
    trv = get_double_data(dyrdata[i],"0.0"); i++;
    fn = get_double_data(dyrdata[i],"0.0"); i++;
    kpv = get_double_data(dyrdata[i],"0.0"); i++;
    tv = get_double_data(dyrdata[i],"0.0"); i++;
    kiv = get_double_data(dyrdata[i],"0.0"); i++;
    qmin = get_double_data(dyrdata[i],"0.0"); i++;
    qmax = get_double_data(dyrdata[i],"0.0"); i++;
    tfv = get_double_data(dyrdata[i],"0.0"); i++;
    tp = get_double_data(dyrdata[i],"0.0"); i++;
    kqi = get_double_data(dyrdata[i],"0.0"); i++;
    vmin = get_double_data(dyrdata[i],"0.0"); i++;
    vmax = get_double_data(dyrdata[i],"0.0"); i++;
    kqv = get_double_data(dyrdata[i],"0.0"); i++;
    eqmin = get_double_data(dyrdata[i],"0.0"); i++;
    eqmax = get_double_data(dyrdata[i],"0.0"); i++;
    tspeed = get_double_data(dyrdata[i],"0.0"); i++;
    kpp = get_double_data(dyrdata[i],"0.0"); i++;
    kip = get_double_data(dyrdata[i],"0.0"); i++;
    kvi = get_double_data(dyrdata[i],"0.0"); i++;
    tvi = get_double_data(dyrdata[i],"0.0"); i++;
    kdroop = get_double_data(dyrdata[i],"0.0"); i++;
    tdroop = get_double_data(dyrdata[i],"0.0"); i++;
    rpmin = get_double_data(dyrdata[i],"0.0"); i++;
    rpmax = get_double_data(dyrdata[i],"0.0"); i++;
    tfp = get_double_data(dyrdata[i],"0.0"); i++;
    pmin = get_double_data(dyrdata[i],"0.0"); i++;
    pmax = get_double_data(dyrdata[i],"0.0"); i++;
    ipmax = get_double_data(dyrdata[i],"0.0");

    set_bus_to_regulate(bus);
    PE_VAR_CONTROL_MODE mode;
    switch(var_control_flag)
    {
        case 0:
        {
            mode = CONSTANT_VAR_MODE;
            break;
        }
        case 1:
        {
            mode = CONSTANT_VOLTAGE_MODE;
            break;
        }
        case -1:
        {
            mode = CONSTANT_POWER_FACTOR_MODE;
            break;
        }
        default:
        {
            mode = CONSTANT_VAR_MODE;
            break;
        }
    }

    set_var_control_mode(mode);
    set_Xcomp_in_pu(xc);
    set_TRV_in_s(trv);
    set_Fn(fn);
    set_KPV(kpv);
    set_TV_in_s(tv);
    set_KIV(kiv);
    set_Qmin_in_pu(qmin);
    set_Qmax_in_pu(qmax);
    set_TFV_in_s(tfv);
    set_TP_in_s(tp);
    set_KQI(kqi);
    set_Vmin_in_pu(vmin);
    set_Vmax_in_pu(vmax);

    if(voltage_flag>2)
        voltage_flag = 2;
    set_voltage_flag(voltage_flag);
    set_KQV(kqv);
    set_EQmin_in_pu(eqmin);
    set_EQmax_in_pu(eqmax);
    set_Tspeed_in_s(tspeed);
    set_KPP(kpp);
    set_KIP(kip);
    set_Kvi(kvi);
    set_Tvi_in_s(tvi);
    set_Kdroop(kdroop);
    set_Tdroop_in_s(tdroop);
    set_rPmin_in_pu(rpmin);
    set_rPmax_in_pu(rpmax);
    set_TFP_in_s(tfp);
    set_Pmin_in_pu(pmin);
    set_Pmax_in_pu(pmax);
    set_IPmax_in_pu(ipmax);

    is_successful = true;

    return is_successful;
}

bool WT3E0::setup_model_with_bpa_string(string data)
{
    return false;
}

void WT3E0::initialize()
{
    ostringstream osstream;
    if(is_model_initialized())
        return;

    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator==NULL)
        return;

    WT_GENERATOR_MODEL* wtgenmodel = wt_generator->get_wt_generator_model();
    if(wtgenmodel==NULL)
        return;

    if(not wtgenmodel->is_model_initialized())
        wtgenmodel->initialize();

    WT_TURBINE_MODEL* turbine_model = wt_generator->get_wt_turbine_model();
    if(turbine_model==NULL)
        return;

    if(not turbine_model->is_model_initialized())
        turbine_model->initialize();

    double vterm = get_terminal_bus_voltage_in_pu();
    double iterm = get_wt_generator_terminal_current_in_pu();
    double freq = get_terminal_bus_frequency_deviation_in_pu();
    double mbase = get_mbase_in_MVA();
    complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();
    double speed = get_wt_generator_speed_in_pu();

    double ipcmd = wtgenmodel->get_initial_active_current_command_in_pu_based_on_mbase();
    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
    {
        osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    double porder = ipcmd*vterm;
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    power_order_integrator.set_output(porder);
    if(ipcmd>ipmax)
    {
        osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Porder is "<<porder<<", and Pmax is "<<pmax<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    power_order_integrator.initialize();

    double torque =  porder/(1.0+speed);
    torque_PI_regulator.set_output(torque);
    torque_PI_regulator.initialize();

    virtual_inertia_emulator.set_input(0.0);
    virtual_inertia_emulator.initialize();

    frequency_droop_controller.set_output(freq);
    frequency_droop_controller.initialize();

    set_frequency_reference_in_pu(freq);

    double speedref = get_wt_generator_speed_referance_in_pu();
    //set_speed_reference_bias_in_pu(speedref-speed);
    wind_turbine_speed_reference_sensor.set_output(speedref);
    wind_turbine_speed_reference_sensor.initialize();


    double iqcmd = wtgenmodel->get_initial_reactive_current_command_in_pu_based_on_mbase();
    double xsource = get_source_impedance_in_pu_based_on_mbase().imag();
    double eqcmd = iqcmd*(-xsource);

    double verror = 0.0;
    size_t vflag = get_voltage_flag();
    if(vflag == 0)
    {
        V_error_integrator.set_output(0.0);
        V_error_integrator.initialize();

        verror = eqcmd;
    }
    else
    {
        if(vflag == 1)
        {
            V_error_integrator.set_upper_limit(vterm+get_EQmax_in_pu());
            V_error_integrator.set_lower_limit(vterm+get_EQmin_in_pu());
        }
        else
        {
            V_error_integrator.set_upper_limit(get_EQmax_in_pu());
            V_error_integrator.set_lower_limit(get_EQmin_in_pu());
        }
        double vmax = V_error_integrator.get_upper_limit();
        double vmin = V_error_integrator.get_lower_limit();

        V_error_integrator.set_output(eqcmd);
        if(eqcmd>vmax)
        {
            osstream<<"Initialization error. Eqcmd (reactive voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
              <<"Eqcmd is "<<eqcmd<<", and Vmax is "<<vmax<<" for voltage flag = "<<vflag;
            show_information_with_leading_time_stamp(osstream);
        }
        if(eqcmd<vmin)
        {
            osstream<<"Initialization error. Eqcmd (reactive voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
              <<"Eqcmd is "<<eqcmd<<", and Vmin is "<<vmin<<" for voltage flag = "<<vflag;
            show_information_with_leading_time_stamp(osstream);
        }
        V_error_integrator.initialize();
        verror = 0.0;
    }

    double vcmd = verror+vterm;
    double vmax = get_Vmax_in_pu();
    double vmin = get_Vmin_in_pu();
    Q_error_integrator.set_output(vcmd);
    if(vcmd>vmax)
    {
        osstream<<"Initialization error. Vcmd (voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Vcmd is "<<vcmd<<", and Vmax is "<<vmax<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    if(vcmd<vmin)
    {
        osstream<<"Initialization error. Vcmd (voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Vcmd is "<<vcmd<<", and Vmin is "<<vmin<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    Q_error_integrator.initialize();

    double qcmd = qelec;
    double qmax = get_Qmax_in_pu();
    double qmin = get_Qmin_in_pu();
    if(qcmd>qmax)
    {
        osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                <<"Qcmd is "<<qcmd<<", and Qmax is "<<qmax<<".";
        show_information_with_leading_time_stamp(osstream);
    }
    if(qcmd<qmin)
    {
        osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                <<"Qcmd is "<<qcmd<<", and Qmin is "<<qmin<<".";
        show_information_with_leading_time_stamp(osstream);
    }

    set_reactive_power_reference_in_pu(qcmd);

    active_power_sensor.set_output(pelec);
    active_power_sensor.initialize();

    double pf = fabs(pelec)/abs(selec);
    pf = (qelec/pelec>0? pf:-pf);
    set_power_factor_reference_in_pu(pf);

    voltage_regulator_filter.set_output(qcmd);
    voltage_regulator_filter.initialize();

    voltage_regulator_integrator.set_output(qcmd);
    voltage_regulator_integrator.initialize();

    voltage_regulator_first_order_block.set_output(0.0);
    voltage_regulator_first_order_block.initialize();

    double xcomp = get_Xcomp_in_pu();
    double vref = vterm+iterm*xcomp;

    voltage_sensor.set_output(vref);
    voltage_sensor.initialize();

    set_voltage_reference_in_pu(vref);
    osstream<<"After initialized: voltage reference is "<<setw(20)<<setprecision(19)<<fixed<<vref<<endl
            <<"                 : voltage reference is "<<setw(20)<<setprecision(19)<<fixed<<get_voltage_reference_in_pu()<<endl
            <<"                 : sensor output     is "<<setw(20)<<setprecision(19)<<fixed<<voltage_sensor.get_output()<<endl;
    osstream<<"                 voltage regulator integrator input = "<<voltage_regulator_first_order_block.get_input()<<", output = "<<voltage_regulator_integrator.get_output()<<endl;
    osstream<<"                 voltage regulator first order block input = "<<voltage_regulator_first_order_block.get_input()<<", output = "<<voltage_regulator_first_order_block.get_output()<<endl;
    osstream<<"                 voltage_regulator_filter input = "<<voltage_regulator_filter.get_input()<<", output = "<<voltage_regulator_filter.get_output()<<endl;
    //show_information_with_leading_time_stamp(osstream);

    set_flag_model_initialized_as_true();
    osstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl;
    show_information_with_leading_time_stamp(osstream);
}

void WT3E0::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator==NULL)
        return;

    WT_GENERATOR_MODEL* wtgenmodel = wt_generator->get_wt_generator_model();
    if(wtgenmodel==NULL)
        return;

    double vterm = get_terminal_bus_voltage_in_pu();
    double iterm = get_wt_generator_terminal_current_in_pu();
    double freq = get_terminal_bus_frequency_deviation_in_pu();
    double mbase = get_mbase_in_MVA();
    complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
    double pelec = selec.real();
    double qelec = selec.imag();

    double speed     = get_wt_generator_speed_in_pu();
    double speed_ref = get_wt_generator_speed_referance_in_pu();
    //double speedref_bias = get_speed_reference_bias_in_pu();

    wind_turbine_speed_reference_sensor.set_input(speed_ref);
    wind_turbine_speed_reference_sensor.run(mode);
    osstream<<"wind_turbine_speed_reference_sensor input = "<<speed_ref<<", output = "<<wind_turbine_speed_reference_sensor.get_output()<<endl;

    //double input = speed + speedref_bias - wind_turbine_speed_reference_sensor.get_output();
    double input = speed - wind_turbine_speed_reference_sensor.get_output();

    torque_PI_regulator.set_input(input);
    torque_PI_regulator.run(mode);
    osstream<<"torque_PI_regulator input = "<<input<<", output = "<<torque_PI_regulator.get_output()<<endl;

    virtual_inertia_emulator.set_input(freq);
    virtual_inertia_emulator.run(mode);
    osstream<<"virtual_inertia_emulator input = "<<input<<", output = "<<virtual_inertia_emulator.get_output()<<endl;

    frequency_droop_controller.set_input(freq);
    frequency_droop_controller.run(mode);
    osstream<<"frequency_droop_controller input = "<<input<<", output = "<<frequency_droop_controller.get_output()<<endl;

    input = torque_PI_regulator.get_output()*(1.0+speed)
            +virtual_inertia_emulator.get_output()
            +frequency_droop_controller.get_output()
            -power_order_integrator.get_output();
    if(input>get_rPmax_in_pu())
        input = get_rPmax_in_pu();
    if(input<get_rPmin_in_pu())
        input = get_rPmin_in_pu();

    power_order_integrator.set_input(input);
    power_order_integrator.run(mode);
    osstream<<"power_order_integrator input = "<<input<<", output = "<<power_order_integrator.get_output()<<endl;

    active_power_sensor.set_input(pelec);
    active_power_sensor.run(mode);
    osstream<<"active_power_sensor input = "<<pelec<<", output = "<<active_power_sensor.get_output()<<endl;

    double xcomp = get_Xcomp_in_pu();
    input = vterm+iterm*xcomp;
    voltage_sensor.set_input(input);
    voltage_sensor.run(mode);
    osstream<<"At time "<<STEPS::TIME<<" s"<<endl;
    osstream<<"voltage_sensor input = "<<setw(20)<<setprecision(19)<<fixed<<input<<", output = "<<voltage_sensor.get_output()<<endl;

    input = get_voltage_reference_in_pu()-voltage_sensor.get_output();
    osstream<<"input = "<<input<<endl;
    input /= get_Fn();

    voltage_regulator_integrator.set_input(input);
    voltage_regulator_integrator.run(mode);
    osstream<<"voltage regulator integrator input = "<<input<<", output = "<<voltage_regulator_integrator.get_output()<<endl;

    voltage_regulator_first_order_block.set_input(input);
    voltage_regulator_first_order_block.run(mode);
    osstream<<"voltage regulator first order block input = "<<input<<", output = "<<voltage_regulator_first_order_block.get_output()<<endl;

    input = voltage_regulator_integrator.get_output() + voltage_regulator_first_order_block.get_output();
    osstream<<"voltage_regulator_filter input = "<<input<<", output = "<<voltage_regulator_filter.get_output()<<endl;
    voltage_regulator_filter.set_input(input);
    voltage_regulator_filter.run(mode);
    osstream<<"voltage_regulator_filter input = "<<input<<", output = "<<voltage_regulator_filter.get_output()<<endl;

    PE_VAR_CONTROL_MODE var_mode = get_var_control_mode();
    double qcmd = 0.0;
    if(var_mode==CONSTANT_VAR_MODE)
        qcmd = get_reactive_power_reference_in_pu();
    else
    {
        if(var_mode==CONSTANT_POWER_FACTOR_MODE)
        {
            double pf = get_power_factor_reference_in_pu();
            qcmd = sqrt(1.0-pf*pf)/pf*active_power_sensor.get_output();
        }
        else
            qcmd = voltage_regulator_filter.get_output();
    }
    if(qcmd>get_Qmax_in_pu())
        qcmd = get_Qmax_in_pu();
    if(qcmd<get_Qmin_in_pu())
        qcmd = get_Qmin_in_pu();
    osstream<<"Q command = "<<qcmd<<endl;

    input = qcmd - qelec;
    Q_error_integrator.set_input(input);
    Q_error_integrator.run(mode);
    osstream<<"Q_error_integrator input = "<<input<<", output = "<<Q_error_integrator.get_output()<<endl;

    size_t vflag = get_voltage_flag();
    if(vflag == 1 or vflag == 2)
    {
        if(vflag == 1)
        {
            V_error_integrator.set_upper_limit(vterm+get_EQmax_in_pu());
            V_error_integrator.set_lower_limit(vterm+get_EQmin_in_pu());
        }
        else
        {
            if(vflag == 2)
            {
                V_error_integrator.set_upper_limit(get_EQmax_in_pu());
                V_error_integrator.set_lower_limit(get_EQmin_in_pu());
            }
        }
        input = Q_error_integrator.get_output()-vterm;
        V_error_integrator.set_input(input);
        V_error_integrator.run(mode);
        osstream<<"V_error_integrator input = "<<input<<", output = "<<V_error_integrator.get_output()<<endl;
    }
    //show_information_with_leading_time_stamp(osstream);

    if(mode == UPDATE_MODE)
        set_flag_model_updated_as_true();
}


double WT3E0::get_active_current_command_in_pu_based_on_mbase() const
{
    double vterm = get_terminal_bus_voltage_in_pu();
    if(vterm==0.0)
        return 0.0;

    double pcmd = get_active_power_command_in_pu_based_on_mbase();
    double ipcmd = pcmd/vterm;
    double ipmax = get_IPmax_in_pu();
    if(ipcmd>ipmax)
        ipcmd = ipmax;
    return ipcmd;
}

double WT3E0::get_active_power_command_in_pu_based_on_mbase() const
{
    return power_order_integrator.get_output();
}

double WT3E0::get_reactive_current_command_in_pu_based_on_mbase() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    double xeq = gen->get_source_impedance_in_pu().imag();

    double eqcmd = get_reactive_voltage_command_in_pu_based_on_mbase();

    return eqcmd/(-xeq);
}

double WT3E0::get_reactive_power_command_in_pu_based_on_mbase() const
{
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
    switch(mode)
    {
        case CONSTANT_VAR_MODE:
        {
            return get_reactive_power_reference_in_pu();
        }
        case CONSTANT_POWER_FACTOR_MODE:
        {
            double pf = get_power_factor_reference_in_pu();
            double p = active_power_sensor.get_output();

            return p*sqrt(1.0-pf*pf)/pf;
        }
        case CONSTANT_VOLTAGE_MODE:
        {
            return voltage_regulator_filter.get_output();
        }
        default:
            return 0.0;
    }
}

double WT3E0::get_reactive_voltage_command_in_pu_based_on_mbase() const
{
    size_t voltage_flag = get_voltage_flag();
    if(voltage_flag==0)
    {
        double vterm = get_terminal_bus_voltage_in_pu();
        return Q_error_integrator.get_output()-vterm;
    }
    else
        return V_error_integrator.get_output();
}

void WT3E0::check()
{
    ;
}

void WT3E0::clear()
{
    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    power_order_integrator.set_limiter_type(NON_WINDUP_LIMITER);
}

void WT3E0::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(osstream);
}

void WT3E0::save()
{
    ;
}

string WT3E0::get_standard_model_string() const
{
    ostringstream osstream;
    WT_GENERATOR* source = get_wt_generator_pointer();
    size_t bus = source->get_source_bus();
    string identifier= source->get_identifier();

    size_t bus_reg = get_bus_to_regulate();
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
    int var_mode = (mode==CONSTANT_VAR_MODE)? 0: (mode==CONSTANT_POWER_FACTOR_MODE? -1 : 1);
    size_t voltage_flag = get_voltage_flag();
    double xc = get_Xcomp_in_pu();
    double trv = get_TRV_in_s();
    double fn = get_Fn();
    double kpv = get_KPV();
    double tv = get_TV_in_s();
    double kiv = get_KIV();
    double qmax = get_Qmax_in_pu();
    double qmin = get_Qmin_in_pu();
    double tfv = get_TFV_in_s();
    double tp = get_TP_in_s();
    double kqi = get_KQI();
    double vmin = get_Vmin_in_pu();
    double vmax = get_Vmax_in_pu();
    double kqv = get_KQV();
    double eqmin = get_EQmin_in_pu();
    double eqmax = get_EQmax_in_pu();
    double tspeed = get_Tspeed_in_s();
    double kpp = get_KPP();
    double kip = get_KIP();
    double kvi = get_Kvi();
    double tvi = get_Tvi_in_s();
    double kdroop = get_Kdroop();
    double tdroop = get_Tdroop_in_s();
    double rpmin = get_rPmin_in_pu();
    double rpmax = get_rPmax_in_pu();
    double tfp = get_TFP_in_s();
    double pmin = get_Pmin_in_pu();
    double pmax = get_Pmax_in_pu();
    double ipmax = get_IPmax_in_pu();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<bus_reg<<", "
      <<setw(8)<<var_mode<<", "
      <<setw(8)<<voltage_flag<<", "
      <<setw(8)<<setprecision(6)<<xc<<", "
      <<setw(8)<<setprecision(6)<<trv<<", "
      <<setw(8)<<setprecision(6)<<fn<<", "
      <<setw(8)<<setprecision(6)<<kpv<<", "
      <<setw(8)<<setprecision(6)<<tv<<", "
      <<setw(8)<<setprecision(6)<<kiv<<", "
      <<setw(8)<<setprecision(6)<<qmin<<", "
      <<setw(8)<<setprecision(6)<<qmax<<", "
      <<setw(8)<<setprecision(6)<<tfv<<", "
      <<setw(8)<<setprecision(6)<<tp<<", "
      <<setw(8)<<setprecision(6)<<kqi<<", "
      <<setw(8)<<setprecision(6)<<vmin<<", "
      <<setw(8)<<setprecision(6)<<vmax<<", "
      <<setw(8)<<setprecision(6)<<kqv<<", "
      <<setw(8)<<setprecision(6)<<eqmin<<", "
      <<setw(8)<<setprecision(6)<<eqmax<<", "
      <<setw(8)<<setprecision(6)<<tspeed<<", "
      <<setw(8)<<setprecision(6)<<kpp<<", "
      <<setw(8)<<setprecision(6)<<kip<<", "
      <<setw(8)<<setprecision(6)<<kvi<<", "
      <<setw(8)<<setprecision(6)<<tvi<<", "
      <<setw(8)<<setprecision(6)<<kdroop<<", "
      <<setw(8)<<setprecision(6)<<tdroop<<", "
      <<setw(8)<<setprecision(6)<<rpmin<<", "
      <<setw(8)<<setprecision(6)<<rpmax<<", "
      <<setw(8)<<setprecision(6)<<tfp<<", "
      <<setw(8)<<setprecision(6)<<pmin<<", "
      <<setw(8)<<setprecision(6)<<pmax<<", "
      <<setw(8)<<setprecision(6)<<ipmax<<" / ";

    return osstream.str();
}

size_t WT3E0::get_variable_index_from_variable_name(string var_name)
{
    return 0;
}

string WT3E0::get_variable_name_from_variable_index(size_t var_index)
{
    return "";
}

double WT3E0::get_variable_with_index(size_t var_index)
{
    return 0.0;
}

double WT3E0::get_variable_with_name(string var_name)
{
    return 0.0;
}

string WT3E0::get_dynamic_data_in_psse_format() const
{
    return get_standard_model_string();
}

string WT3E0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3E0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
