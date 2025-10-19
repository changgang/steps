#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_lcc_model/MTHVDCLCC1.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

MTHVDCLCC1::MTHVDCLCC1(STEPS& toolkit) : HLCC_MODEL(toolkit),
                                         udc_sensor(toolkit),
                                         idc_sensor(toolkit),
                                         pi_cc(toolkit),
                                         pi_cea(toolkit),
                                         pi_cv(toolkit)
 {
     clear();
 }

MTHVDCLCC1::~MTHVDCLCC1()
 {
     ;
 }

MTHVDCLCC1::MTHVDCLCC1(const MTHVDCLCC1& model) : HLCC_MODEL(model.get_toolkit()),
                                                   udc_sensor(model.get_toolkit()),
                                                   idc_sensor(model.get_toolkit()),
                                                   pi_cc(model.get_toolkit()),
                                                   pi_cea(model.get_toolkit()),
                                                   pi_cv(model.get_toolkit())
 {
     clear();
     copy_from_const_model(model);
 }

MTHVDCLCC1& MTHVDCLCC1::operator=(const MTHVDCLCC1& model)
 {
    if(this != &model)
        copy_from_const_model(model);
    return *this;;
 }

void MTHVDCLCC1::copy_from_const_model(const MTHVDCLCC1& model)
{
     STEPS& toolkit = model.get_toolkit();
     udc_sensor.set_toolkit(toolkit);
     idc_sensor.set_toolkit(toolkit);
     pi_cc.set_toolkit(toolkit);
     pi_cea.set_toolkit(toolkit);
     pi_cv.set_toolkit(toolkit);
     clear();

     udc_sensor.set_T_in_s(model.get_udc_sensor_block_Td_in_s());
     idc_sensor.set_T_in_s(model.get_idc_sensor_block_Td_in_s());
     pi_cc.set_Kp(model.get_cc_block_kp());
     pi_cc.set_Ki(model.get_cc_block_ki());
     pi_cea.set_Kp(model.get_cea_block_kp());
     pi_cea.set_Ki(model.get_cea_block_ki());
     pi_cv.set_Kp(model.get_cv_block_kp());
     pi_cv.set_Ki(model.get_cv_block_ki());
     set_vdcol(model.get_vdcol());
     set_dynamic_max_firing_angle(model.get_dynamic_max_firing_angle());
     set_dynamic_min_firing_angle(model.get_dynamic_min_firing_angle());
     set_dynamic_min_unbypass_gamma_in_deg(model.get_dynamic_min_unbypass_gamma_in_deg());
}

string MTHVDCLCC1::get_model_name() const
{
    return "MTHVDCLCC1";
}

void MTHVDCLCC1::set_udc_sensor_block_Td_in_s(const double T)
{
    if(T!=0)
        udc_sensor.set_T_in_s(T);
    else
        udc_sensor.set_T_in_s(0.01);
}

void MTHVDCLCC1::set_idc_sensor_block_Td_in_s(const double T)
{
    if(T!=0)
        idc_sensor.set_T_in_s(T);
    else
        idc_sensor.set_T_in_s(0.01);
}

void MTHVDCLCC1::set_cc_block_kp(const double kp)
{
    pi_cc.set_Kp(kp);
}

void MTHVDCLCC1::set_cc_block_ki(const double ki)
{
    pi_cc.set_Ki(ki);
}

void MTHVDCLCC1::set_cea_block_kp(const double kp)
{
    pi_cea.set_Kp(kp);
}

void MTHVDCLCC1::set_cea_block_ki(const double ki)
{
    pi_cea.set_Ki(ki);
}

void MTHVDCLCC1::set_cv_block_kp(const double kp)
{
    pi_cv.set_Kp(kp);
}
void MTHVDCLCC1::set_cv_block_ki(const double ki)
{
    pi_cv.set_Ki(ki);
}

void MTHVDCLCC1::set_vdcol(const VDCOL& vdcol_limiter)
{
    this->vdcol_limiter = vdcol_limiter;
}

void MTHVDCLCC1::set_dynamic_max_firing_angle(const double max_angle)
{
    this->dynamic_max_angle = max_angle;
}

void MTHVDCLCC1::set_dynamic_min_firing_angle(const double min_angle)
{
    this->dynamic_min_angle = min_angle;
}

void MTHVDCLCC1::set_min_current_command_value(const double c0)
{
    this->min_current_command_value = c0;
}

void MTHVDCLCC1::set_dynamic_idc_ref_in_kA(const double idc)
{
    this->idc_ref_in_kA = idc;
}

void MTHVDCLCC1::set_dynamic_angle_ref_in_deg(const double angle)
{
    this->angle_ref_in_deg = angle;
}

void MTHVDCLCC1::set_dynamic_udc_ref_in_kV(const double udc)
{
    this->udc_ref_in_kV = udc;
}

void MTHVDCLCC1::set_dynamic_min_unbypass_gamma_in_deg(const double gamma)
{
    this->dynamic_min_unbypass_gamma_in_deg = gamma;
}

double MTHVDCLCC1::get_udc_sensor_block_Td_in_s() const
{
    return udc_sensor.get_T_in_s();
}

double MTHVDCLCC1::get_idc_sensor_block_Td_in_s() const
{
    return idc_sensor.get_T_in_s();
}

double MTHVDCLCC1::get_cc_block_kp() const
{
    return pi_cc.get_Kp();
}

double MTHVDCLCC1::get_cc_block_ki() const
{
    return pi_cc.get_Ki();
}

double MTHVDCLCC1::get_cea_block_kp() const
{
    return pi_cea.get_Kp();
}

double MTHVDCLCC1::get_cea_block_ki() const
{
    return pi_cea.get_Ki();
}

double MTHVDCLCC1::get_cv_block_kp() const
{
    return pi_cv.get_Kp();
}

double MTHVDCLCC1::get_cv_block_ki() const
{
    return pi_cv.get_Ki();
}

VDCOL MTHVDCLCC1::get_vdcol() const
{
    return vdcol_limiter;
}

double MTHVDCLCC1::get_dynamic_max_firing_angle() const
{
    return dynamic_max_angle;
}

double MTHVDCLCC1::get_dynamic_min_firing_angle() const
{
    return dynamic_min_angle;
}

double MTHVDCLCC1::get_min_current_command_value() const
{
    return min_current_command_value;
}

double MTHVDCLCC1::get_idc_ref_in_kA() const
{
    return idc_ref_in_kA;
}

double MTHVDCLCC1::get_idc_ref_in_kA_of_VDCOL(double Udc)
{
    HLCC* lcc = get_hlcc_pointer();
    double Idc_ref = 0.0;
    HLCC_CONTROL_MODE cmode = lcc->get_control_mode();
    if(cmode==LCC_CONSTANT_CURRENT_MODE)
    {
        Idc_ref = lcc->get_command_idc_in_kA();
    }
    else if(cmode==LCC_CONSTANT_POWER_MODE)
    {
        double Pdc_ref = lcc->get_command_pdc_in_MW();
        Idc_ref = Pdc_ref/Udc;
    }
    else
    {
        Idc_ref = get_idc_ref_in_kA();
    }
    double Idc_max = vdcol_limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(Udc);
    double Idc_min = get_min_current_command_value();
    if(Idc_ref<Idc_min)
        Idc_ref = Idc_min;
    if(Idc_ref>Idc_max)
        Idc_ref = Idc_max;
    return Idc_ref;
}

double MTHVDCLCC1::get_dynamic_angle_ref_in_deg() const
{
    return angle_ref_in_deg;
}

double MTHVDCLCC1::get_dynamic_udc_ref_in_kV() const
{
    return udc_ref_in_kV;
}

double MTHVDCLCC1::get_dynamic_min_unbypass_gamma_in_deg() const
{
    return dynamic_min_unbypass_gamma_in_deg;
}

double MTHVDCLCC1::solve_inverter_gamma_in_deg_with_beta_in_rad_and_idc_in_kA(double beta,double idc)
{
    ostringstream osstream;
    HLCC* lcc = get_hlcc_pointer();
    HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();
    if(omode==LCC_INVERTER)
    {
        idc = abs(idc);
        double vac = lcc->get_ac_bus_vlotage_in_kV();
        double vbase_grid = lcc->get_converter_transformer_grid_side_base_voltage_in_kV();
        double vbase_converter = lcc->get_converter_transformer_converter_side_base_voltage_in_kV();
        double k = vbase_grid/vbase_converter;
        double tap = lcc->get_converter_transformer_tap_in_pu();
        double Eac = vac/k/tap;
        double X = lcc->get_converter_transformer_X_in_ohm();

        double cos_gamma = SQRT2*idc*X/Eac+steps_cos(beta);
        if(cos_gamma>=0.999)
            cos_gamma = 0.999;
        double gamma_in_rad = steps_acos(cos_gamma);
        double gamma_in_deg = rad2deg(gamma_in_rad);
        //osstream<<"GAMMA DEG = "<<gamma_in_deg<<"WITH IDC= "<<idc<<" BETA = "<<beta<<"Uac = "<<vac;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return gamma_in_deg;
    }
    else
    {
        return 0.0;
    }
}



bool MTHVDCLCC1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=21)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            string converter_name = get_string_data(data[data_index],""); data_index++;
            set_model_converter_name(converter_name);

            double max_angle = get_double_data(data[data_index],"0.0"); data_index++;
            set_dynamic_max_firing_angle(max_angle);

            double min_angle = get_double_data(data[data_index],"0.0"); data_index++;
            set_dynamic_min_firing_angle(min_angle);

            double min_unbypass_angle = get_double_data(data[data_index],"0.0"); data_index++;
            set_dynamic_min_unbypass_gamma_in_deg(min_unbypass_angle);

            double t_udc = get_double_data(data[data_index],"0.0"); data_index++;
            set_udc_sensor_block_Td_in_s(t_udc);

            double t_idc = get_double_data(data[data_index],"0.0"); data_index++;
            set_idc_sensor_block_Td_in_s(t_idc);

            double kp_cc = get_double_data(data[data_index],"0.0"); data_index++;
            set_cc_block_kp(kp_cc);

            double ki_cc = get_double_data(data[data_index],"0.0"); data_index++;
            set_cc_block_ki(ki_cc);

            double kp_cea = get_double_data(data[data_index],"0.0"); data_index++;
            set_cea_block_kp(kp_cea);

            double ki_cea = get_double_data(data[data_index],"0.0"); data_index++;
            set_cea_block_ki(ki_cea);

            double kp_cv = get_double_data(data[data_index],"0.0"); data_index++;
            set_cv_block_kp(kp_cv);

            double ki_cv = get_double_data(data[data_index],"0.0"); data_index++;
            set_cv_block_ki(ki_cv);

            double c0 = get_double_data(data[data_index],"0.0"); data_index++;
            set_min_current_command_value(c0);

            double v1 = get_double_data(data[data_index],"0.0"); data_index++;
            double c1 = get_double_data(data[data_index],"0.0"); data_index++;
            double v2 = get_double_data(data[data_index],"0.0"); data_index++;
            double c2 = get_double_data(data[data_index],"0.0"); data_index++;
            double v3 = get_double_data(data[data_index],"0.0"); data_index++;
            double c3 = get_double_data(data[data_index],"0.0"); data_index++;

            VDCOL limiter;
            limiter.append_vdcol_point_in_kV_kA(v1, c1*0.001);
            limiter.append_vdcol_point_in_kV_kA(v2, c2*0.001);
            limiter.append_vdcol_point_in_kV_kA(v3, c3*0.001);
            set_vdcol(limiter);

            is_successful = true;
        }
        return is_successful;
    }
    return is_successful;
}
bool MTHVDCLCC1::setup_model_with_psse_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}
bool MTHVDCLCC1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void MTHVDCLCC1::setup_block_toolkit_and_parameters()
{
    udc_sensor.set_K(1.0);
    idc_sensor.set_K(1.0);

    double angmax_deg = get_dynamic_max_firing_angle();
    double angmax_rad = deg2rad(angmax_deg);

    double angmin_deg = get_dynamic_min_firing_angle();
    double angmin_rad = deg2rad(angmin_deg);

    pi_cc.set_limiter_type(WINDUP_LIMITER);
    pi_cc.set_upper_limit(angmax_rad);
    pi_cc.set_lower_limit(angmin_rad);

    pi_cea.set_limiter_type(WINDUP_LIMITER);
    pi_cea.set_upper_limit(angmax_rad);
    pi_cea.set_lower_limit(angmin_rad);

    pi_cv.set_limiter_type(WINDUP_LIMITER);
    pi_cv.set_upper_limit(angmax_rad);
    pi_cv.set_lower_limit(angmin_rad);

}

void MTHVDCLCC1::initialize()
{
    if(not is_model_initialized())
    {
        HLCC* lcc = get_hlcc_pointer();
        setup_block_toolkit_and_parameters();

        double udc = lcc->get_udc_in_kV();
        udc_sensor.set_output(udc);
        udc_sensor.initialize();

        double idc = lcc->get_idc_in_kA();
        idc_sensor.set_output(idc);
        idc_sensor.initialize();

        double angle_in_deg = lcc->get_firing_angle_in_deg();
        double angle_in_rad = deg2rad(angle_in_deg);

        set_dynamic_idc_ref_in_kA(abs(idc));
        set_dynamic_udc_ref_in_kV(udc);
        set_dynamic_angle_ref_in_deg(angle_in_deg);

        HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();
        if(omode==LCC_RECTIFIER)
        {
            pi_cc.set_output(angle_in_rad);
            pi_cc.initialize();

            double angmin_deg = get_dynamic_min_firing_angle();
            double angmin_rad = deg2rad(angmin_deg);

            pi_cea.set_output(angmin_rad);
            pi_cea.initialize();

            pi_cv.set_output(angmin_rad);
            pi_cv.initialize();

        }

        else//INVERTER
        {
            double gamma_in_deg = angle_in_deg;
            //double gamma_in_rad = deg2rad(gamma_in_deg);
            double gamma_min = get_dynamic_min_firing_angle();

            double beta_in_rad = lcc->solve_inverter_beta_in_rad_with_gamma_in_deg_and_idc_in_kA(gamma_in_deg,idc);
            double min_beta_in_rad = lcc->solve_inverter_beta_in_rad_with_gamma_in_deg_and_idc_in_kA(gamma_min,idc);

            HLCC_CONTROL_MODE cmode = lcc->get_control_mode();
            switch(cmode)
            {
                case LCC_CONSTANT_CURRENT_MODE:
                    break;
                case LCC_CONSTANT_POWER_MODE:
                    break;
                case LCC_CONSTANT_VOLTAGE_MODE:
                    pi_cc.set_output(min_beta_in_rad);
                    pi_cc.initialize();

                    pi_cea.set_output(min_beta_in_rad);
                    pi_cea.initialize();

                    pi_cv.set_output(beta_in_rad);
                    pi_cv.initialize();
                    break;
                case LCC_CONSTANR_FIRING_ANGLE_MODE:
                    pi_cc.set_output(min_beta_in_rad);
                    pi_cc.initialize();

                    pi_cea.set_output(beta_in_rad);
                    pi_cea.initialize();

                    pi_cv.set_output(min_beta_in_rad);
                    pi_cv.initialize();
                    break;
                default:
                    pi_cc.set_output(min_beta_in_rad);
                    pi_cc.initialize();

                    pi_cea.set_output(min_beta_in_rad);
                    pi_cea.initialize();

                    pi_cv.set_output(beta_in_rad);
                    pi_cv.initialize();
                    break;
            }
        }
        set_flag_model_initialized_as_true();
    }
}

void MTHVDCLCC1::check_limits_in_initialization()
{
    return;
}

void MTHVDCLCC1::run(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit();
    HLCC* lcc = get_hlcc_pointer();
    ostringstream osstream;
    double input_udc_sensor = lcc->get_udc_in_kV();
    udc_sensor.set_input(input_udc_sensor);
    udc_sensor.run(mode);
    double Udcm = udc_sensor.get_output();
    //double Udcm = lcc->get_udc_in_kV();

    double input_idc_sensor = lcc->get_idc_in_kA();
    idc_sensor.set_input(input_idc_sensor);
    idc_sensor.run(mode);
    double Idcm = idc_sensor.get_output();
    //double Idcm = lcc->get_idc_in_kA();

    HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();
    HLCC_CONTROL_MODE cmode = lcc->get_control_mode();

    double Sbase = lcc->get_converter_rated_capacity_in_MVA();
    double Vbase = lcc->get_converter_base_voltage_in_kV();
    double Ibase = Sbase/Vbase;

    if(omode==LCC_RECTIFIER)
    {
        double Idcref = get_idc_ref_in_kA_of_VDCOL(Udcm);
        switch(cmode)
        {
            case LCC_CONSTANT_CURRENT_MODE:
                pi_cc.set_input((-Idcref+Idcm)/Ibase);
                pi_cc.run(mode);

                pi_cea.set_input(0.0);
                pi_cea.run(mode);

                pi_cv.set_input(0.0);
                pi_cea.run(mode);
                /*
                osstream<<"RECTIFEIR WITH CC CONTROL UPDATE WITH INPUT = IDCREF: "<<Idcref<<" -Idcm: "<<Idcm<<endl
                        <<"NEW OUTPUT = "<<pi_cc.get_output();
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                */

                break;
            case LCC_CONSTANT_POWER_MODE:
                pi_cc.set_input((-Idcref+Idcm)/Ibase);
                pi_cc.run(mode);

                pi_cea.set_input(0.0);
                pi_cea.run(mode);

                pi_cv.set_input(0.0);
                pi_cea.run(mode);
                break;
            case LCC_CONSTANT_VOLTAGE_MODE:
                break;
            case LCC_CONSTANR_FIRING_ANGLE_MODE:
                break;
            default:
                break;
        }
    }
    else//INVERTER
    {
        double Idc_ref = -get_idc_ref_in_kA_of_VDCOL(Udcm);
        double gamma_ref = get_dynamic_angle_ref_in_deg();//steady-state gamma as ref
        //double gamma_ref = 15;//steady-state min gamma
        double gamma_ref_in_rad = deg2rad(gamma_ref);

        double margin = lcc->get_converter_current_margin_in_pu();

        double gamma_in_deg = lcc->get_firing_angle_in_deg();
        double gamma_in_rad = deg2rad(gamma_in_deg);

        double Udc_ref = 0.0;

        switch(cmode)
        {

            case LCC_CONSTANT_CURRENT_MODE:
                break;
            case LCC_CONSTANT_POWER_MODE:
                break;
            case LCC_CONSTANT_VOLTAGE_MODE:
                pi_cc.set_input((-Idc_ref*(1-margin)+Idcm)/Ibase);
                pi_cc.run(mode);

                pi_cea.set_input(gamma_ref_in_rad-gamma_in_rad);
                pi_cea.run(mode);


                Udc_ref = lcc->get_command_udc_in_kV();
                pi_cv.set_input((-Udc_ref+Udcm)/Vbase);
                pi_cv.run(mode);
                /*

                osstream<<"INVERTER WITH CV CONTROL UPDATE WITH INPUT = UDCREF: "<<Udc_ref<<" -Udcm: "<<Udcm<<endl
                        <<"NEW OUTPUT = "<<pi_cc.get_output();
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                */
                break;
            case LCC_CONSTANR_FIRING_ANGLE_MODE:
                break;
            default:
                break;
        }
    }
    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void MTHVDCLCC1::check()
{
    ;
}

void MTHVDCLCC1::clear()
{
    set_udc_sensor_block_Td_in_s(0.01);
    udc_sensor.set_K(1.0);
    udc_sensor.set_limiter_type(NO_LIMITER);

    set_idc_sensor_block_Td_in_s(0.01);
    idc_sensor.set_K(1.0);
    idc_sensor.set_limiter_type(NO_LIMITER);

    set_cc_block_kp(1.0);
    set_cc_block_ki(1.0);
    pi_cc.set_limiter_type(NO_LIMITER);

    set_cea_block_kp(1.0);
    set_cea_block_ki(1.0);
    pi_cea.set_limiter_type(NO_LIMITER);

    set_cv_block_kp(1.0);
    set_cv_block_ki(1.0);
    pi_cv.set_limiter_type(NO_LIMITER);

    vdcol_limiter.clear();
    dynamic_max_angle = 90.0;
    dynamic_min_angle = 0.0;
    dynamic_min_unbypass_gamma_in_deg = 15;
    min_current_command_value = 0.0;
    idc_ref_in_kA= 0.0;
    angle_ref_in_deg = 0.0;
    udc_ref_in_kV = 0.0;


    set_bypass_flag(false);
    set_block_flag(false);

    time_of_last_bypass = 0.0;

}

void MTHVDCLCC1::report()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    toolkit.show_information_with_leading_time_stamp(osstream);;
}

void MTHVDCLCC1::save()
{
    ;
}

string MTHVDCLCC1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    HLCC* lcc = get_hlcc_pointer();

    string mtdc_name = "'"+mtdc->get_name()+"'";
    string lcc_name = "'"+lcc->get_converter_name()+"'";
    string model_name = "'"+get_model_name()+"'";
    osstream<<setw(16)<<mtdc_name<<", "
            <<setw(10)<<lcc_name<<","
            <<setw(10)<<model_name<<", "
            <<setw(8)<<setprecision(4)<<get_dynamic_max_firing_angle()<<", "
            <<setw(8)<<setprecision(4)<<get_dynamic_min_firing_angle()<<", "
            <<setw(8)<<setprecision(4)<<get_dynamic_min_unbypass_gamma_in_deg()<<",\n "
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<get_udc_sensor_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_idc_sensor_block_Td_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_cc_block_kp()<<", "
            <<setw(8)<<setprecision(4)<<get_cc_block_ki()<<", "
            <<setw(8)<<setprecision(4)<<get_cea_block_kp()<<", "
            <<setw(8)<<setprecision(4)<<get_cea_block_ki()<<", "
            <<setw(8)<<setprecision(4)<<get_cv_block_kp()<<", "
            <<setw(8)<<setprecision(4)<<get_cv_block_ki()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_voltage_of_point_in_kV(0)<<", "
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_current_of_point_in_kA(0)<<", "
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_voltage_of_point_in_kV(1)<<", "
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_current_of_point_in_kA(1)<<", "
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_voltage_of_point_in_kV(2)<<", "
            <<setw(8)<<setprecision(4)<<vdcol_limiter.get_vdcol_current_of_point_in_kA(2)<<", "
            <<setw(10)<<"";
    return osstream.str();
}


void MTHVDCLCC1::update_converters()
{
    ostringstream osstream;
    HLCC* lcc = get_hlcc_pointer();
    HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();

    check_block_logic();
    check_bypass_logic();


    if(omode==LCC_RECTIFIER)
    {
        double angle_command_in_rad = get_dynamic_command_firing_angle_in_rad();
        double alpha_command_in_deg = rad2deg(angle_command_in_rad);
        lcc->set_firing_angle_in_deg(alpha_command_in_deg);
        //osstream<<" LCC"<<lcc->get_converter_name()<<" is update with alpha: "<<alpha_command_in_deg;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

void MTHVDCLCC1::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("MAX ALPHA DEG", i); i++;
    add_model_data_name_and_index_pair("MIN ALPHA DEG", i); i++;
    add_model_data_name_and_index_pair("MIN UNBYPASS ANGLE DEG", i); i++;
    add_model_data_name_and_index_pair("TUDC", i); i++;
    add_model_data_name_and_index_pair("TIDC", i); i++;
    add_model_data_name_and_index_pair("KP CC", i); i++;
    add_model_data_name_and_index_pair("KI CC", i); i++;
    add_model_data_name_and_index_pair("KP CEA", i); i++;
    add_model_data_name_and_index_pair("KI CEA", i); i++;
    add_model_data_name_and_index_pair("KP CV", i); i++;
    add_model_data_name_and_index_pair("KI CV", i); i++;
    add_model_data_name_and_index_pair("VDCOL V0", i); i++;
    add_model_data_name_and_index_pair("VDCOL I0", i); i++;
    add_model_data_name_and_index_pair("VDCOL V1", i); i++;
    add_model_data_name_and_index_pair("VDCOL I1", i); i++;
    add_model_data_name_and_index_pair("VDCOL V2", i); i++;
    add_model_data_name_and_index_pair("VDCOL I2", i); i++;
}

double MTHVDCLCC1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    VDCOL vdcol_limiter = get_vdcol();
    if(par_name=="MAX ALPHA DEG")
        return get_dynamic_max_firing_angle();
    if(par_name=="MIN ALPHA DEG")
        return get_dynamic_min_firing_angle();
    if(par_name=="MIN UNBYPASS ANGLE DEG")
        return get_dynamic_min_unbypass_gamma_in_deg();
    if(par_name=="TUDC")
        return get_udc_sensor_block_Td_in_s();
    if(par_name=="TIDC")
        return get_idc_sensor_block_Td_in_s();
    if(par_name=="KP CC")
        return get_cc_block_kp();
    if(par_name=="KI CC")
        return get_cc_block_ki();
    if(par_name=="KP CEA")
        return get_cea_block_kp();
    if(par_name=="KI CEA")
        return get_cea_block_ki();
    if(par_name=="KP CV")
        return get_cv_block_kp();
    if(par_name=="KI CV")
        return get_cv_block_ki();

    if(par_name=="VDCOL V0")
        return vdcol_limiter.get_vdcol_voltage_of_point_in_kV(0);
    if(par_name=="VDCOL I0")
        return vdcol_limiter.get_vdcol_current_of_point_in_kA(0)*1000.0;
    if(par_name=="VDCOL V1")
        return vdcol_limiter.get_vdcol_voltage_of_point_in_kV(1);
    if(par_name=="VDCOL I1")
        return vdcol_limiter.get_vdcol_current_of_point_in_kA(1)*1000.0;
    if(par_name=="VDCOL V2")
        return vdcol_limiter.get_vdcol_voltage_of_point_in_kV(2);
    if(par_name=="VDCOL I2")
        return vdcol_limiter.get_vdcol_current_of_point_in_kA(2)*1000.0;

    return 0.0;
}
void MTHVDCLCC1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    VDCOL vdcol_limiter = get_vdcol();
    if(par_name=="MAX ALPHA DEG")
        return set_dynamic_max_firing_angle(value);
    if(par_name=="MIN ALPHA DEG")
        return set_dynamic_min_firing_angle(value);
    if(par_name=="MIN UNBYPASS ANGLE DEG")
        return set_dynamic_min_unbypass_gamma_in_deg(value);
    if(par_name=="TUDC")
        return set_udc_sensor_block_Td_in_s(value);
    if(par_name=="TIDC")
        return set_idc_sensor_block_Td_in_s(value);
    if(par_name=="KP CC")
        return set_cc_block_kp(value);
    if(par_name=="KI CC")
        return set_cc_block_ki(value);
    if(par_name=="KP CEA")
        return set_cea_block_kp(value);
    if(par_name=="KI CEA")
        return set_cea_block_ki(value);
    if(par_name=="KP CV")
        return set_cv_block_kp(value);
    if(par_name=="KI CV")
        return set_cv_block_ki(value);

    if(par_name=="VDCOL V0")
        return;
    if(par_name=="VDCOL I0")
        return;
    if(par_name=="VDCOL V1")
        return;
    if(par_name=="VDCOL I1")
        return;
    if(par_name=="VDCOL V2")
        return;
    if(par_name=="VDCOL I2")
        return;

    return;
}

double MTHVDCLCC1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;;
}

void MTHVDCLCC1::prepare_model_internal_variable_table()
{
    ;
}

double MTHVDCLCC1::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;
}


string MTHVDCLCC1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string MTHVDCLCC1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}
string MTHVDCLCC1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

double MTHVDCLCC1::get_dynamic_command_firing_angle_in_rad()
{
    double angle_cc = pi_cc.get_output();
    double angle_cea = pi_cea.get_output();
    double angle_cv = pi_cv.get_output();
    double angle_command_in_rad = 0.0;
    if(angle_cc>angle_cea)
    {
        angle_command_in_rad = angle_cc;
    }
    else
    {
        angle_command_in_rad = angle_cea;
    }
    if(angle_command_in_rad<angle_cv)
    {
        angle_command_in_rad = angle_cv;;
    }
    return angle_command_in_rad;
}

void MTHVDCLCC1::check_bypass_logic()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    HLCC* lcc = get_hlcc_pointer();
    HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();

    double angle_command_in_rad = get_dynamic_command_firing_angle_in_rad();

    if(omode==LCC_INVERTER)
    {
        double idc = lcc->get_idc_in_kA();
        double beta_command_in_rad = angle_command_in_rad;
        double gamma_in_deg = solve_inverter_gamma_in_deg_with_beta_in_rad_and_idc_in_kA(beta_command_in_rad,idc);
        if(get_block_flag()==false)
        {
            if(get_bypass_flag()==false)
            {

                double gamma_min = get_dynamic_min_firing_angle();
                if(gamma_in_deg<=gamma_min)
                {
                    lcc->set_firing_angle_in_deg(0);
                    osstream<<"INVERTER LCC: "<<lcc->get_converter_name()<<" will be bypass due to insufficient gamma = "<<gamma_in_deg<<" with min gamma: "<<gamma_min;
                    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                    bypass_lcc();
                    double t= toolkit.get_dynamic_simulation_time_in_s();
                    time_of_last_bypass = t;
                }
                else
                {
                    lcc->set_firing_angle_in_deg(gamma_in_deg);
                }
            }
            else
            {

                double t= toolkit.get_dynamic_simulation_time_in_s();
                double t_duration = t-time_of_last_bypass;

                double min_unbypass_gamma_in_deg = get_dynamic_min_unbypass_gamma_in_deg();
                if(gamma_in_deg>min_unbypass_gamma_in_deg)
                {

                    lcc->set_firing_angle_in_deg(gamma_in_deg);
                    osstream<<"INVERTER LCC: "<<lcc->get_converter_name()<<" will be unbypass due to sufficient gamma = "<<gamma_in_deg<<" with min unbypass gamma: "<<min_unbypass_gamma_in_deg;
                    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                    unbypass_lcc();

                }
                else
                {
                    lcc->set_firing_angle_in_deg(0);
                }
            }
        }
    }
}

void MTHVDCLCC1::check_block_logic()
{
    HLCC* lcc = get_hlcc_pointer();
    double idc = lcc->get_idc_in_kA();
    if(get_block_flag()==false)
    {
        HLCC_OPERATION_MODE omode = lcc->get_converter_operating_mode();
        if(omode==LCC_RECTIFIER)
        {
            ;
        }
        else
        {
            ;
        }
    }
}

double MTHVDCLCC1::get_converter_angle_ref_in_deg_of_cc()
{
    double angle_in_rad = pi_cc.get_output();
    return rad2deg(angle_in_rad);
}

double MTHVDCLCC1::get_converter_angle_ref_in_deg_of_cea()
{
    double angle_in_rad = pi_cea.get_output();
    return rad2deg(angle_in_rad);
}

double MTHVDCLCC1::get_converter_angle_ref_in_deg_of_cv()
{
    double angle_in_rad = pi_cv.get_output();
    return rad2deg(angle_in_rad);;
}

double MTHVDCLCC1::get_converter_idc_ref_in_kA()
{
    HLCC* lcc = get_hlcc_pointer();
    double Udcm = udc_sensor.get_output();
    double idc_ref = get_idc_ref_in_kA_of_VDCOL(Udcm);
    if(lcc->get_converter_operating_mode()==LCC_INVERTER)
    {
        double margin = lcc->get_converter_current_margin_in_pu();
        idc_ref = -idc_ref*(1-margin);
    }
    return idc_ref;
}

