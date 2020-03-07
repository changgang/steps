#include "header/model/wtg_models/wt_turbine_model/wt3t0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"

WT3T0::WT3T0(STEPS& toolkit) : WT_TURBINE_MODEL(toolkit),
                               shaft_twist_block(toolkit),
                               turbine_inertia_block(toolkit),
                               generator_inertia_block(toolkit),
                               generator_rotor_angle_block(toolkit)
{
    clear();
}

WT3T0::~WT3T0()
{
}

void WT3T0::clear()
{
    set_model_float_parameter_count(5);
}

void WT3T0::copy_from_const_model(const WT3T0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    shaft_twist_block.set_toolkit(toolkit);
    turbine_inertia_block.set_toolkit(toolkit);
    generator_inertia_block.set_toolkit(toolkit);
    generator_rotor_angle_block.set_toolkit(toolkit);

    clear();

    set_damping_in_pu(model.get_damping_in_pu());
    set_Hturbine_in_s(model.get_Hturbine_in_s());
    set_Hgenerator_in_s(model.get_Hgenerator_in_s());
    set_Kshaft_in_pu(model.get_Kshaft_in_pu());
    set_Dshaft_in_pu(model.get_Dshaft_in_pu());
}

WT3T0::WT3T0(const WT3T0& model) : WT_TURBINE_MODEL(model.get_toolkit()),
                                   shaft_twist_block(model.get_toolkit()),
                                   turbine_inertia_block(model.get_toolkit()),
                                   generator_inertia_block(model.get_toolkit()),
                                   generator_rotor_angle_block(model.get_toolkit())
{
    copy_from_const_model(model);
}

WT3T0& WT3T0::operator=(const WT3T0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string WT3T0::get_model_name() const
{
    return "WT3T0";
}


void WT3T0::set_Hturbine_in_s(double H)
{
    turbine_inertia_block.set_T_in_s(2.0*H);
}

void WT3T0::set_Hgenerator_in_s(double H)
{
    generator_inertia_block.set_T_in_s(2.0*H);
}

void WT3T0::set_Kshaft_in_pu(double K)
{
    shaft_twist_block.set_T_in_s(1.0/K);
}

void WT3T0::set_Dshaft_in_pu(double D)
{
    Dshaft = D;
}

double WT3T0::get_Hturbine_in_s() const
{
    return 0.5*turbine_inertia_block.get_T_in_s();
}

double WT3T0::get_Hgenerator_in_s() const
{
    return 0.5*generator_inertia_block.get_T_in_s();
}

double WT3T0::get_Kshaft_in_pu() const
{
    return 1.0/shaft_twist_block.get_T_in_s();
}

double WT3T0::get_Dshaft_in_pu() const
{
    return Dshaft;
}

bool WT3T0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=8)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double ht, hg, damp, kshaft, dshaft;

            unsigned int i=3;
            ht = get_double_data(data[i],"0.0"); i++;
            hg = get_double_data(data[i],"0.0"); i++;
            kshaft = get_double_data(data[i],"0.0"); i++;
            dshaft = get_double_data(data[i],"0.0"); i++;
            damp = get_double_data(data[i],"0.0");

            set_Hturbine_in_s(ht);
            set_Hgenerator_in_s(hg);
            set_Kshaft_in_pu(kshaft);
            set_Dshaft_in_pu(dshaft);
            set_damping_in_pu(damp);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;

    }
    else
        return is_successful;
}

bool WT3T0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3T0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3T0::setup_block_toolkit_and_parameters()
{
}

void WT3T0::initialize()
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    WT_GENERATOR_MODEL* gen_model = gen->get_wt_generator_model();
    if(gen_model!=NULL)
    {
        if(not gen_model->is_model_initialized())
            gen_model->initialize();

        WT_AERODYNAMIC_MODEL* aero_model = gen->get_wt_aerodynamic_model();
        if(aero_model!=NULL)
        {
            if(not aero_model->is_model_initialized())
                aero_model->initialize();

            setup_block_toolkit_and_parameters();

            unsigned int bus = gen->get_generator_bus();

            double fbase = get_bus_base_frequency_in_Hz();
            double wbase = DOUBLE_PI*fbase;

            generator_rotor_angle_block.set_T_in_s(1.0/wbase);
            generator_rotor_angle_block.set_output(psdb.get_bus_positive_sequence_angle_in_rad(bus));
            generator_rotor_angle_block.initialize();

            double speed = get_initial_wind_turbine_speed_in_pu_from_wt_areodynamic_model();
            double dspeed = speed-1.0;

            generator_inertia_block.set_output(dspeed);
            generator_inertia_block.initialize();

            turbine_inertia_block.set_output(dspeed);
            double hturbine = get_Hturbine_in_s();
            if(fabs(hturbine)>DOUBLE_EPSILON)
                turbine_inertia_block.initialize();

            double pelec = gen_model->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
            double telec = pelec/speed;
            double tmech = get_damping_in_pu()*dspeed+telec;

            shaft_twist_block.set_output(tmech);
            shaft_twist_block.initialize();

            set_flag_model_initialized_as_true();

            ostringstream osstream;
            if(toolkit.is_detailed_log_enabled())
            {
                osstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl
                        <<"(1) Turbine speed is "<<get_turbine_speed_in_pu()<<" pu"<<endl
                        <<"(2) Generator speed is "<<get_generator_speed_in_pu()<<" pu"<<endl
                        <<"(3) Generator rotor angle is "<<get_rotor_angle_in_deg()<<" deg"<<endl
                        <<"(4) Shaft block state is "<<shaft_twist_block.get_state();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void WT3T0::run(DYNAMIC_MODE mode)
{
    WT_GENERATOR* wtgen = (WT_GENERATOR*) get_device_pointer();

    double dshaft = get_Dshaft_in_pu();
    double damp = get_damping_in_pu();

    double pmech = get_mechanical_power_in_pu_from_wt_aerodynamic_model();

    WT_GENERATOR_MODEL* wtgenmodel = wtgen->get_wt_generator_model();

    double pelec = wtgenmodel->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    double gdspeed = generator_inertia_block.get_output();
    double gspeed = gdspeed+1.0;
    double telec = pelec/gspeed;

    if(fabs(get_Hturbine_in_s())>DOUBLE_EPSILON)
    {
        double tdspeed = turbine_inertia_block.get_output();
        double tspeed = 1.0+tdspeed;
        double tmech = pmech/tspeed;

        double ttwist = shaft_twist_block.get_output();
        double input = tmech -(ttwist + dshaft*(tdspeed-gdspeed));
        turbine_inertia_block.set_input(input);
        turbine_inertia_block.run(mode);

        input = (dshaft*(tdspeed-gdspeed) + ttwist) - telec - damp*gdspeed;
        generator_inertia_block.set_input(input);
        generator_inertia_block.run(mode);

        double fbase = get_bus_base_frequency_in_Hz();
        double wbase = DOUBLE_PI*fbase;

        input = (turbine_inertia_block.get_output()-generator_inertia_block.get_output())*wbase;
        shaft_twist_block.set_input(input);
        shaft_twist_block.run(mode);
    }
    else
    {
        double tmech = pmech/(1.0+gdspeed);
        double input = tmech - telec - damp*gdspeed;
        generator_inertia_block.set_input(input);
        generator_inertia_block.run(mode);
    }

    generator_rotor_angle_block.set_input(generator_inertia_block.get_output()+1.0-get_initial_wind_turbine_speed_in_pu_from_wt_areodynamic_model());
    generator_rotor_angle_block.run(mode);

    if(mode==UPDATE_MODE)
    {
        set_flag_model_updated_as_true();
        /*WT_AERODYNAMIC_MODEL* aerd = wtgen->get_wt_aerodynamic_model();
        double wmin = aerd->get_min_steady_state_turbine_speed_in_pu();
        double wmax = aerd->get_max_steady_state_turbine_speed_in_pu();
        double w = get_generator_speed_in_pu();
        if(w<wmin or w>wmax*1.1)
        {
            ostringstream osstream;
            osstream<<get_device_name()<<" is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<"s due to ";
            if(w<wmin)
                osstream<<"rotor w<wmin: "<<w<<"<"<<wmin<<" pu";
            else
                osstream<<"rotor w>wmax*1.1: "<<w<<">"<<wmax*1.1<<" pu";
            toolkit.show_information_with_leading_time_stamp(osstream);
            wtgen->set_status(false);
        }*/
    }
}

double WT3T0::get_turbine_speed_in_pu() const
{
    if(fabs(get_Hturbine_in_s())>DOUBLE_EPSILON)
        return turbine_inertia_block.get_output()+1.0;
    else
        return get_generator_speed_in_pu();
}

double WT3T0::get_generator_speed_in_pu() const
{
    return generator_inertia_block.get_output()+1.0;
}

double WT3T0::get_rotor_angle_in_deg() const
{
    return rad2deg(get_rotor_angle_in_rad());
}

double WT3T0::get_rotor_angle_in_rad() const
{
    return generator_rotor_angle_block.get_output();
}

void WT3T0::check()
{
    ;
}

void WT3T0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void WT3T0::save()
{
    ;
}
string WT3T0::get_standard_psse_string() const
{
    ostringstream osstream;

    double ht = get_Hturbine_in_s();
    double hg = get_Hgenerator_in_s();
    double kshaft = get_Kshaft_in_pu();
    double damp = get_damping_in_pu();
    double dshaft = get_Dshaft_in_pu();

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<ht<<", "
            <<setw(8)<<setprecision(6)<<hg<<", "
            <<setw(8)<<setprecision(6)<<kshaft<<", "
            <<setw(8)<<setprecision(6)<<dshaft<<", "
            <<setw(8)<<setprecision(6)<<damp<<" /";
    return osstream.str();
}

void WT3T0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=1;
    add_model_data_name_and_index_pair("TURBINE INERTIA", i); i++; /*1*/
    add_model_data_name_and_index_pair("GENERATOR INERTIA", i); i++; /*2*/
    add_model_data_name_and_index_pair("SHAFT K", i); i++; /*3*/
    add_model_data_name_and_index_pair("SHAFT DAMPING", i); i++; /*4*/
    add_model_data_name_and_index_pair("GENERATOR DAMPING", i); i++; /*5*/
}

double WT3T0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="TURBINE INERTIA") return get_Hturbine_in_s();
    if(par_name=="GENERATOR INERTIA") return get_Hgenerator_in_s();
    if(par_name=="SHAFT K")           return get_Kshaft_in_pu();
    if(par_name=="SHAFT DAMPING")     return get_Dshaft_in_pu();
    if(par_name=="GENERATOR DAMPING") return get_damping_in_pu();
    return 0.0;
}

void WT3T0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="TURBINE INERTIA") return set_Hturbine_in_s(value);
    if(par_name=="GENERATOR INERTIA") return set_Hgenerator_in_s(value);
    if(par_name=="SHAFT K")           return set_Kshaft_in_pu(value);
    if(par_name=="SHAFT DAMPING")     return set_Dshaft_in_pu(value);
    if(par_name=="GENERATOR DAMPING") return set_damping_in_pu(value);
}

double WT3T0::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void WT3T0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=1;
    add_model_inernal_variable_name_and_index_pair("STATE@TURBINE SPEED BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@GENERATOR SPEED BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SHAFT TORSION BLOCK", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@ROTOR ANGLE BLOCK", i); i++;
}

double WT3T0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="STATE@TURBINE SPEED BLOCK") return turbine_inertia_block.get_state();
    if(var_name=="STATE@GENERATOR SPEED BLOCK") return generator_inertia_block.get_state();
    if(var_name=="STATE@SHAFT TORSION BLOCK") return shaft_twist_block.get_state();
    if(var_name=="STATE@ROTOR ANGLE BLOCK") return generator_rotor_angle_block.get_state();

    return 0.0;
}

string WT3T0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3T0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3T0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
