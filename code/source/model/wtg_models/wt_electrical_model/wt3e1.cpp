#include "header/model/wtg_models/wt_electrical_model/wt3e1.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"

WT3E1::WT3E1()
{
    clear();
}

WT3E1::WT3E1(const WT3E1& model)
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

void WT3E1::copy_from_const_model(const WT3E1& model)
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

void WT3E1::set_transformer_from_bus(size_t bus)
{
    transformer_from_bus = bus;
}

void WT3E1::set_transformer_to_bus(size_t bus)
{
    transformer_to_bus = bus;
}

void WT3E1::set_transformer_id(string id)
{
    transformer_id = id;
}

void WT3E1::set_Xcomp_in_pu(double Xc)
{
    Xcomp = Xc;
}

void WT3E1::set_TRV_in_s(double T)
{
    voltage_sensor.set_T_in_s(T);
}

void WT3E1::set_Fn(double Fn)
{
    this->Fn = Fn;
}

void WT3E1::set_KIV(double K)
{
    if(K!=0.0)
        voltage_regulator_integrator.set_T_in_s(1.0/K);
}

void WT3E1::set_Qmax_in_pu(double q)
{
    voltage_regulator_integrator.set_upper_limit(q);
}

void WT3E1::set_Qmin_in_pu(double q)
{
    voltage_regulator_integrator.set_lower_limit(q);
}

void WT3E1::set_KPV(double K)
{
    voltage_regulator_first_order_block.set_K(K);
}

void WT3E1::set_TV_in_s(double T)
{
    voltage_regulator_first_order_block.set_T_in_s(T);
}

void WT3E1::set_TFV_in_s(double T)
{
    voltage_regulator_filter.set_T_in_s(T);
}

void WT3E1::set_TP_in_s(double T)
{
    active_power_sensor.set_T_in_s(T);
}

void WT3E1::set_KQI(double K)
{
    if(K!=0.0)
        Q_error_integrator.set_T_in_s(1.0/K);
}

void WT3E1::set_Vmax_in_pu(double v)
{
    Q_error_integrator.set_upper_limit(v);
}

void WT3E1::set_Vmin_in_pu(double v)
{
    Q_error_integrator.set_lower_limit(v);
}

void WT3E1::set_voltage_flag(size_t flag)
{
    if(flag<3)
        Voltage_Flag = flag;
    else
    {
        ostringstream osstream;
        osstream<<"Error. "<<flag<<" is not allowed to set up voltage flag for "<<get_model_name()<<" model. 0, 1, or 2 is allowed.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void WT3E1::set_KQV(double K)
{
    if(K!=0.0)
        V_error_integrator.set_T_in_s(1.0/K);
}

void WT3E1::set_EQmax_in_pu(double I)
{
    EQmax = I;
}

void WT3E1::set_EQmin_in_pu(double I)
{
    EQmin = I;
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
    max_torque_rate = r;
}

void WT3E1::set_rPmin_in_pu(double r)
{
    min_torque_rate = r;
}

void WT3E1::set_TFP_in_s(double T)
{
    power_order_integrator.set_T_in_s(T);
}

void WT3E1::set_Pmax_in_pu(double p)
{
    power_order_integrator.set_upper_limit(p);
}

void WT3E1::set_Pmin_in_pu(double p)
{
    power_order_integrator.set_lower_limit(p);
}

void WT3E1::set_IPmax_in_pu(double I)
{
    IPmax = I;
}

size_t WT3E1::get_transformer_from_bus() const
{
    return transformer_from_bus;
}

size_t WT3E1::get_transformer_to_bus() const
{
    return transformer_to_bus;
}

string WT3E1::get_transformer_id() const
{
    return transformer_id;
}

double WT3E1::get_Xcomp_in_pu() const
{
    return Xcomp;
}

double WT3E1::get_TRV_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double WT3E1::get_Fn() const
{
    return Fn;
}

double WT3E1::get_KIV() const
{
    return 1.0/voltage_regulator_integrator.get_T_in_s();
}

double WT3E1::get_Qmax_in_pu() const
{
    return voltage_regulator_integrator.get_upper_limit();
}

double WT3E1::get_Qmin_in_pu() const
{
    return voltage_regulator_integrator.get_lower_limit();
}

double WT3E1::get_KPV() const
{
    return voltage_regulator_first_order_block.get_K();
}

double WT3E1::get_TV_in_s() const
{
    return voltage_regulator_first_order_block.get_T_in_s();
}

double WT3E1::get_TFV_in_s() const
{
    return voltage_regulator_filter.get_T_in_s();
}

double WT3E1::get_TP_in_s() const
{
    return active_power_sensor.get_T_in_s();
}

double WT3E1::get_KQI() const
{
    return 1.0/Q_error_integrator.get_T_in_s();
}

double WT3E1::get_Vmax_in_pu() const
{
    return Q_error_integrator.get_upper_limit();
}

double WT3E1::get_Vmin_in_pu() const
{
    return Q_error_integrator.get_lower_limit();
}

size_t WT3E1::get_voltage_flag() const
{
    return Voltage_Flag;
}

double WT3E1::get_KQV() const
{
    return 1.0/V_error_integrator.get_T_in_s();
}

double WT3E1::get_EQmax_in_pu() const
{
    return EQmax;
}

double WT3E1::get_EQmin_in_pu() const
{
    return EQmin;
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
    return max_torque_rate;
}

double WT3E1::get_rPmin_in_pu() const
{
    return min_torque_rate;
}

double WT3E1::get_TFP_in_s() const
{
    return power_order_integrator.get_T_in_s();
}

double WT3E1::get_Pmax_in_pu() const
{
    return power_order_integrator.get_upper_limit();
}

double WT3E1::get_Pmin_in_pu() const
{
    return power_order_integrator.get_lower_limit();
}

double WT3E1::get_IPmax_in_pu() const
{
    return IPmax;
}

bool WT3E1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=40)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t bus, var_control_flag, voltage_flag, trans_from_bus, trans_to_bus;
            string trans_id;

            double tfv, kpv, kiv, xc, tfp, kpp, kip, pmax, pmin, qmax, qmin,
                   ipmax, trv, rpmax, rpmin, tspeed, kqi, vmax, vmin,
                   kqv, eqmax, eqmin, tv, tp, fn,
                   wmin, wp20, wp40, wp60, pmin_at_wmin, wp100;

            size_t i=3;
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

            WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE table;
            table.add_wind_turbine_power_and_speed_pair_in_pu(pmin_at_wmin, wmin);
            table.add_wind_turbine_power_and_speed_pair_in_pu(0.2, wp20);
            table.add_wind_turbine_power_and_speed_pair_in_pu(0.4, wp40);
            table.add_wind_turbine_power_and_speed_pair_in_pu(0.6, wp60);
            table.add_wind_turbine_power_and_speed_pair_in_pu(1.0, wp100);
            vector<double> p = table.get_wind_turbine_power_table();
            vector<double> s = table.get_wind_turbine_speed_table();

            set_wind_turbine_power_speed_lookup_table(table);

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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3E1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    voltage_sensor.set_toolkit(toolkit);
    voltage_regulator_first_order_block.set_toolkit(toolkit);
    voltage_regulator_integrator.set_toolkit(toolkit);
    voltage_regulator_filter.set_toolkit(toolkit);
    active_power_sensor.set_toolkit(toolkit);
    Q_error_integrator.set_toolkit(toolkit);
    V_error_integrator.set_toolkit(toolkit);
    wind_turbine_speed_reference_sensor.set_toolkit(toolkit);
    torque_PI_regulator.set_toolkit(toolkit);
    power_order_integrator.set_toolkit(toolkit);
}

void WT3E1::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        WT_GENERATOR* wt_generator = get_wt_generator_pointer();
        if(wt_generator!=NULL)
        {
            WT_GENERATOR_MODEL* gen_model = wt_generator->get_wt_generator_model();
            if(gen_model!=NULL)
            {
                if(not gen_model->is_model_initialized())
                    gen_model->initialize();

                setup_block_toolkit_and_parameters();

                STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

                double vterm = get_terminal_bus_voltage_in_pu();
                double iterm = gen_model->get_terminal_current_in_pu_based_on_mbase();
                //double freq = get_terminal_bus_frequency_deviation_in_pu();
                //double mbase = wt_generator->get_mbase_in_MVA();
                complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
                double pelec = selec.real();
                double qelec = selec.imag();
                double speed = get_wind_turbine_reference_speed_with_power_in_pu(pelec);

                double ipcmd = gen_model->get_active_current_command_in_pu_based_on_mbase();
                double ipmax = get_IPmax_in_pu();
                if(ipcmd>ipmax)
                {
                    osstream<<"Initialization error. IPcmd (Active current command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"IPcmd is "<<ipcmd<<", and IPmax is "<<ipmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                double porder = ipcmd*vterm;
                double pmax = get_Pmax_in_pu();
                double pmin = get_Pmin_in_pu();
                power_order_integrator.set_output(porder);
                if(porder>pmax)
                {
                    osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Porder is "<<porder<<", and Pmax is "<<pmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(porder<pmin)
                {
                    osstream<<"Initialization error. Porder (Active power order) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Porder is "<<porder<<", and Pmin is "<<pmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                power_order_integrator.initialize();

                double torque =  porder/(1.0+speed);
                torque_PI_regulator.set_output(torque);
                torque_PI_regulator.initialize();

                wind_turbine_speed_reference_sensor.set_output(speed);
                wind_turbine_speed_reference_sensor.initialize();


                double iqcmd = gen_model->get_reactive_current_command_in_pu_based_on_mbase();
                double xsource = wt_generator->get_source_impedance_in_pu().imag();
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
                          <<"Eqcmd is "<<eqcmd<<", and Vmax is "<<vmax<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    if(eqcmd<vmin)
                    {
                        osstream<<"Initialization error. Eqcmd (reactive voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                          <<"Eqcmd is "<<eqcmd<<", and Vmin is "<<vmin<<".";
                        toolkit.show_information_with_leading_time_stamp(osstream);
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
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(vcmd<vmin)
                {
                    osstream<<"Initialization error. Vcmd (voltage command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Vcmd is "<<vcmd<<", and Vmin is "<<vmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                Q_error_integrator.initialize();

                double qcmd = qelec;
                double qmax = get_Qmax_in_pu();
                double qmin = get_Qmin_in_pu();
                if(qcmd>qmax)
                {
                    osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Qcmd is "<<qcmd<<", and Qmax is "<<qmax<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(qcmd<qmin)
                {
                    osstream<<"Initialization error. Qcmd (reactive power command) of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Qcmd is "<<qcmd<<", and Qmin is "<<qmin<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                set_reactive_power_reference_in_pu(qcmd);

                active_power_sensor.set_output(pelec);
                active_power_sensor.initialize();

                double smag = steps_fast_complex_abs(selec);
                double pf = fabs(pelec)/smag;
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

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void WT3E1::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator!=NULL)
    {
        WT_GENERATOR_MODEL* source_model = wt_generator->get_wt_generator_model();
        if(source_model!=NULL)
        {
            double vterm = get_terminal_bus_voltage_in_pu();
            double iterm = source_model->get_terminal_current_in_pu_based_on_mbase();
            //double freq = get_terminal_bus_frequency_deviation_in_pu();
            //double mbase = wt_generator->get_mbase_in_MVA();
            complex<double> selec = get_wt_generator_terminal_generation_in_pu_based_on_mbase();
            double pelec = selec.real();
            double qelec = selec.imag();

            double speed_ref = get_wind_turbine_reference_speed_with_power_in_pu(pelec);
            double speed = speed_ref;


            wind_turbine_speed_reference_sensor.set_input(speed_ref);
            wind_turbine_speed_reference_sensor.run(mode);

            double input = speed - wind_turbine_speed_reference_sensor.get_output();

            torque_PI_regulator.set_input(input);
            torque_PI_regulator.run(mode);

            input = torque_PI_regulator.get_output()*(1.0+speed)-power_order_integrator.get_output();
            if(input>get_rPmax_in_pu())
                input = get_rPmax_in_pu();
            if(input<get_rPmin_in_pu())
                input = get_rPmin_in_pu();

            power_order_integrator.set_input(input);
            power_order_integrator.run(mode);


            active_power_sensor.set_input(pelec);
            active_power_sensor.run(mode);

            double xcomp = get_Xcomp_in_pu();
            input = vterm+iterm*xcomp;
            voltage_sensor.set_input(input);
            voltage_sensor.run(mode);

            input = get_voltage_reference_in_pu()-voltage_sensor.get_output();
            input /= get_Fn();

            voltage_regulator_integrator.set_input(input);
            voltage_regulator_integrator.run(mode);

            voltage_regulator_first_order_block.set_input(input);
            voltage_regulator_first_order_block.run(mode);

            input = voltage_regulator_integrator.get_output() + voltage_regulator_first_order_block.get_output();
            voltage_regulator_filter.set_input(input);
            voltage_regulator_filter.run(mode);

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

            input = qcmd - qelec;
            Q_error_integrator.set_input(input);
            Q_error_integrator.run(mode);

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
            }

            if(mode == UPDATE_MODE)
                set_flag_model_updated_as_true();
        }
    }
}

double WT3E1::get_active_current_command_in_pu_based_on_mbase() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        size_t bus = source->get_source_bus();
        double vterm = psdb.get_bus_voltage_in_pu(bus);

        double porder = get_active_power_command_in_pu_based_on_mbase();
        double IPcmd = porder/vterm;

        double IPmax = get_IPmax_in_pu();
        if(IPcmd>IPmax)
            IPcmd = IPmax;
        return IPcmd;
    }
    else
        return 0.0;

}

double WT3E1::get_active_power_command_in_pu_based_on_mbase() const
{
    return power_order_integrator.get_output();
}

double WT3E1::get_reactive_current_command_in_pu_based_on_mbase() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        double EQcmd = 0.0;
        if(get_voltage_flag()==0)
        {
            EQcmd = get_reactive_power_command_in_pu_based_on_mbase();
        }
        else
        {
            EQcmd = get_reactive_voltage_command_in_pu();
        }

        double Xsource = source->get_source_impedance_in_pu().imag();

        double IQcmd = -EQcmd/Xsource;
        return IQcmd;
    }
    else
        return 0.0;
}

double WT3E1::get_reactive_power_command_in_pu_based_on_mbase() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(get_voltage_flag()==0)
        {
            size_t bus = source->get_source_bus();
            double vterm = psdb.get_bus_voltage_in_pu(bus);

            double EQcmd = Q_error_integrator.get_output()-vterm;
            return EQcmd;
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WT3E1::get_reactive_voltage_command_in_pu() const
{
    if(get_voltage_flag()==0)
    {
        return 0.0;
    }
    else
    {
        double EQcmd = V_error_integrator.get_output();
        return EQcmd;
    }
}

void WT3E1::check()
{
    ;
}

void WT3E1::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_transformer_from_bus(0);
    set_transformer_to_bus(0);
    set_transformer_id("");
    voltage_regulator_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    Q_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    V_error_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    power_order_integrator.set_limiter_type(NON_WINDUP_LIMITER);
}

void WT3E1::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3E1::save()
{
    ;
}

string WT3E1::get_standard_model_string() const
{
    ostringstream osstream;
    WT_GENERATOR* source = get_wt_generator_pointer();
    size_t bus = source->get_source_bus();
    string identifier= source->get_identifier();

    size_t bus_reg = get_bus_to_regulate();
    PE_VAR_CONTROL_MODE mode = get_var_control_mode();
    int var_mode = (mode==CONSTANT_VAR_MODE)? 0:(mode==CONSTANT_POWER_FACTOR_MODE? -1 : 1);
    size_t voltage_flag = get_voltage_flag();
    size_t trans_from_bus = get_transformer_from_bus();
    size_t trans_to_bus = get_transformer_to_bus();
    string trans_id = get_transformer_id();
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
    WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE table = get_wind_turbine_power_speed_lookup_table();
    vector<double> speed = table.get_wind_turbine_speed_table();
    vector<double> power = table.get_wind_turbine_power_table();
    double wmin = speed[0];
    double wp20 = table.get_reference_speed_with_power_in_pu(0.2);
    double wp40 = table.get_reference_speed_with_power_in_pu(0.4);
    double wp60 = table.get_reference_speed_with_power_in_pu(0.6);
    double pmin_at_wmin = power[0];
    double wp100 = table.get_reference_speed_with_power_in_pu(1.0);

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<bus_reg<<", "
      <<setw(8)<<var_mode<<", "
      <<setw(8)<<voltage_flag<<", "
      <<setw(8)<<trans_from_bus<<", "
      <<setw(8)<<trans_to_bus<<", "
      <<"'"<<trans_id<<"', "
      <<setw(8)<<setprecision(6)<<tfv<<", "
      <<setw(8)<<setprecision(6)<<kpv<<", "
      <<setw(8)<<setprecision(6)<<kiv<<", "
      <<setw(8)<<setprecision(6)<<xc<<", "
      <<setw(8)<<setprecision(6)<<tfp<<", "
      <<setw(8)<<setprecision(6)<<kpp<<", "
      <<setw(8)<<setprecision(6)<<kip<<", "
      <<setw(8)<<setprecision(6)<<pmax<<", "
      <<setw(8)<<setprecision(6)<<pmin<<", "
      <<setw(8)<<setprecision(6)<<qmax<<", "
      <<setw(8)<<setprecision(6)<<qmin<<", "
      <<setw(8)<<setprecision(6)<<ipmax<<", "
      <<setw(8)<<setprecision(6)<<trv<<", "
      <<setw(8)<<setprecision(6)<<rpmax<<", "
      <<setw(8)<<setprecision(6)<<rpmin<<", "
      <<setw(8)<<setprecision(6)<<tspeed<<", "
      <<setw(8)<<setprecision(6)<<kqi<<", "
      <<setw(8)<<setprecision(6)<<vmax<<", "
      <<setw(8)<<setprecision(6)<<vmin<<", "
      <<setw(8)<<setprecision(6)<<kqv<<", "
      <<setw(8)<<setprecision(6)<<eqmin<<", "
      <<setw(8)<<setprecision(6)<<eqmax<<", "
      <<setw(8)<<setprecision(6)<<tv<<", "
      <<setw(8)<<setprecision(6)<<tp<<", "
      <<setw(8)<<setprecision(6)<<fn<<", "
      <<setw(8)<<setprecision(6)<<wmin<<", "
      <<setw(8)<<setprecision(6)<<wp20<<", "
      <<setw(8)<<setprecision(6)<<wp40<<", "
      <<setw(8)<<setprecision(6)<<wp60<<", "
      <<setw(8)<<setprecision(6)<<pmin_at_wmin<<", "
      <<setw(8)<<setprecision(6)<<wp100<<"  /";

    return osstream.str();
}

void WT3E1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("A", i); i++;
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

void WT3E1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("PLL ANGLE IN DEG", i); i++;
}

double WT3E1::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
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
