#include "header/device/generator.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <iostream>

using namespace std;

GENERATOR::GENERATOR(POWER_SYSTEM_DATABASE* db) : SOURCE(db)
{
    char info[256];
    if(db==NULL)
    {
        sprintf(info, "Error. GENERATOR object cannot be constructed since NULL power system database is given.\n"
                      "Operations on the object is unpredictable.");
        show_information_with_leading_time_stamp(info);
    }
    clear();
    set_regulating_mode(REGULATING_PV);
    /*generator_model = NULL;
    exciter_model = NULL;
    stabilizer_model = NULL;
    turbine_governor_model = NULL;*/
}

GENERATOR::~GENERATOR()
{
    clear_all_models();
}
/*
void GENERATOR::update_model_power_system_databse()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(db!=NULL)
    {
        MODEL* model;

        model = get_generator_model();
        if(model!=NULL)
            model->set_power_system_database(db);

        model = get_exciter_model();
        if(model!=NULL)
            model->set_power_system_database(db);

        model = get_stabilizer_model();
        if(model!=NULL)
            model->set_power_system_database(db);

        model = get_turbine_governor_model();
        if(model!=NULL)
            model->set_power_system_database(db);
    }
}*/

void GENERATOR::set_generator_bus(size_t bus)
{
    set_source_bus(bus);
}
void GENERATOR::set_generator_impedance_in_pu(complex<double> z_pu)
{
    set_source_impedance_in_pu(z_pu);
}

size_t GENERATOR::get_generator_bus() const
{
    return get_source_bus();
}
complex<double> GENERATOR::get_generator_impedance_in_pu() const
{
    return get_source_impedance_in_pu();
}

void GENERATOR::clear()
{
    SOURCE::clear();

    sync_generator_model_index = INDEX_NOT_EXIST;
    exciter_model_index = INDEX_NOT_EXIST;
    stabilizer_model_index = INDEX_NOT_EXIST;
    turbine_governor_model_index = INDEX_NOT_EXIST;
}

DEVICE_ID GENERATOR::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");

    TERMINAL terminal;
    terminal.append_bus(get_generator_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

string GENERATOR::get_device_name() const
{
    return get_device_id().get_device_name();
}

void GENERATOR::set_sync_generator_model_index(size_t index)
{
    sync_generator_model_index = index;
}

void GENERATOR::set_exciter_model_index(size_t index)
{
    exciter_model_index = index;
}

void GENERATOR::set_stabilizer_model_index(size_t index)
{
    stabilizer_model_index = index;
}

void GENERATOR::set_turbine_governor_model_index(size_t index)
{
    turbine_governor_model_index = index;
}

size_t GENERATOR::get_sync_generator_model_index() const
{
    return sync_generator_model_index;
}

size_t GENERATOR::get_exciter_model_index() const
{
    return exciter_model_index;
}

size_t GENERATOR::get_stabilizer_model_index() const
{
    return stabilizer_model_index;
}

size_t GENERATOR::get_turbine_governor_model_index() const
{
    return turbine_governor_model_index;
}


/*void GENERATOR::set_generator_model(SYNC_GENERATOR_MODEL* model)
{
    if(model==NULL or model==generator_model)
        return;

    clear_generator_model();

    generator_model = model;
    generator_model->set_mbase_in_MVA(this->get_mbase_in_MVA());

    update_model_power_system_databse();
}

void GENERATOR::set_exciter_model(EXCITER_MODEL* model)
{
    if(model==NULL or model==exciter_model)
        return;

    clear_exciter_model();

    exciter_model = model;

    update_model_power_system_databse();
}

void GENERATOR::set_stabilizer_model(STABILIZER_MODEL* model)
{
    if(model==NULL or model==stabilizer_model)
        return;

    clear_stabilizer_model();

    stabilizer_model = model;

    update_model_power_system_databse();
}

void GENERATOR::set_turbine_governor_model(TURBINE_GOVERNOR_MODEL* model)
{
    if(model==NULL or model==turbine_governor_model)
        return;

    clear_turbine_governor_model();

    turbine_governor_model = model;

    update_model_power_system_databse();
}


SYNC_GENERATOR_MODEL* GENERATOR::get_generator_model()
{
    return generator_model;
}

EXCITER_MODEL* GENERATOR::get_exciter_model()
{
    return exciter_model;
}

STABILIZER_MODEL* GENERATOR::get_stabilizer_model()
{
    return stabilizer_model;
}

TURBINE_GOVERNOR_MODEL* GENERATOR::get_turbine_governor_model()
{
    return turbine_governor_model;
}

void GENERATOR::clear_all_models()
{
    clear_generator_model();
    clear_exciter_model();
    clear_stabilizer_model();
    clear_turbine_governor_model();
}

void GENERATOR::clear_generator_model()
{
    if(generator_model!=NULL)
    {
        delete generator_model;
        generator_model = NULL;
    }
}
void GENERATOR::clear_exciter_model()
{
    if(exciter_model!=NULL)
    {
        delete exciter_model;
        exciter_model = NULL;
    }
}

void GENERATOR::clear_stabilizer_model()
{
    if(stabilizer_model!=NULL)
    {
        delete stabilizer_model;
        stabilizer_model = NULL;
    }
}

void GENERATOR::clear_turbine_governor_model()
{
    if(turbine_governor_model!=NULL)
    {
        delete turbine_governor_model;
        turbine_governor_model = NULL;
    }
}


void GENERATOR::check_all_models()
{
    check_generator_model();
    check_exciter_model();
    check_stabilizer_model();
    check_turbine_governor_model();
}

void GENERATOR::check_generator_model()
{
    char info[256];
    SYNC_GENERATOR_MODEL* model = get_generator_model();
    if(model==NULL)
    {
        sprintf(info,"Error. Generator model is not set for %s.", get_device_name().c_str());
        show_information_with_leading_time_stamp(info);
        return;
    }
    else
    {
        model->check();
    }
}
void GENERATOR::check_exciter_model()
{
    EXCITER_MODEL* model = get_exciter_model();
    if(model!=NULL)
        model->check();
}

void GENERATOR::check_stabilizer_model()
{
    STABILIZER_MODEL* model = get_stabilizer_model();
    if(model!=NULL)
        model->check();
}

void GENERATOR::check_turbine_governor_model()
{
    TURBINE_GOVERNOR_MODEL* model = get_turbine_governor_model();
    if(model!=NULL)
        model->check();
}
*/

void GENERATOR::report() const
{
    char info[256];
    string regulating_mode;
    switch(get_regulating_mode())
    {
        case REGULATING_PQ:
            regulating_mode = "regulating P and Q";
            break;
        case REGULATING_PV:
            regulating_mode = "regulating P and V";
            break;
        case REGULATING_VA:
            regulating_mode = "regulating V and Angle";
            break;
        default:
            regulating_mode = "not set(ERROR)";
    }
    sprintf(info,"%s: %s, MBASE=%6.0fMVA\n"
                 "regulating mode: %s\n"
                 "P=%8.4fMW, Pmax=%8.4fMW, Pmin=%8.4fMW\n"
                 "Q=%8.4fMVar, Qmax=%8.4fMVar, Qmin=%8.4fMVar\n"
                 "Zsource=(%8.6f, %8.6f)\n",
                 get_device_name().c_str(),
                 get_status()==true?"in service":"out of service", get_mbase_in_MVA(),
                 regulating_mode.c_str(),
                 get_p_generation_in_MW(), get_p_max_in_MW(), get_p_min_in_MW(),
                 get_q_generation_in_MVar(),get_q_max_in_MVar(), get_q_min_in_MVar(),
                 get_generator_impedance_in_pu().real(), get_generator_impedance_in_pu().imag());
    show_information_with_leading_time_stamp(info);
}

void GENERATOR::save() const
{
    ;
}

GENERATOR& GENERATOR::operator=(const GENERATOR& gen)
{
    if(this==(&gen)) return *this;

    clear();
    set_power_system_database(gen.get_power_system_database());
    update_model_power_system_databse();
    set_generator_bus(gen.get_generator_bus());
    set_identifier(gen.get_identifier());
    set_status(gen.get_status());
    set_mbase_in_MVA(gen.get_mbase_in_MVA());
    set_p_generation_in_MW(gen.get_p_generation_in_MW());
    set_q_generation_in_MVar(gen.get_q_generation_in_MVar());
    set_p_max_in_MW(gen.get_p_max_in_MW());
    set_p_min_in_MW(gen.get_p_min_in_MW());
    set_q_max_in_MVar(gen.get_q_max_in_MVar());
    set_q_min_in_MVar(gen.get_q_min_in_MVar());
    set_generator_impedance_in_pu(gen.get_generator_impedance_in_pu());
    set_bus_to_regulate(gen.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(gen.get_voltage_to_regulate_in_pu());

    return *this;

}

complex<double> GENERATOR::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    size_t index = get_sync_generator_model_index();
    SYNC_GENERATOR_MODEL* generator_model = (SYNC_GENERATOR_MODEL*) STEPS::model_db.get_model_of_index(index);
    if(generator_model!=NULL)
        return generator_model->get_internal_voltage_in_pu_in_xy_axis();
    else
        return 0.0;
}

/*
void GENERATOR::initialize()
{
    char info[256];

    size_t index = get_sync_generator_model_index();
    SYNC_GENERATOR_MODEL* generator_model = (SYNC_GENERATOR_MODEL*) STEPS::model_db.get_model_of_index(index);
    index = get_exciter_model_index();
    EXCITER_MODEL* exciter_model = (EXCITER_MODEL*) STEPS::model_db.get_model_of_index(index);
    index = get_stabilizer_model_index();
    STABILIZER_MODEL* stabilizer_model = (STABILIZER_MODEL*) STEPS::model_db.get_model_of_index(index);
    index = get_turbine_governor_model_index();
    TURBINE_GOVERNOR_MODEL* turbine_governor_model = (TURBINE_GOVERNOR_MODEL*) STEPS::model_db.get_model_of_index(index);

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    if(db==NULL)
    {
        sprintf(info, "Warning. %s is not assigned to any power system database.\n"
                      "Generator will not be initialized for dynamics simulation.",
                      get_device_name().c_str());
        show_information_with_leading_time_stamp(info);
        return;
    }

    double P = get_p_generation_in_MW();
    double Q = get_q_generation_in_MVar();
    complex<double> S = complex<double>(P,Q);

    complex<double> V = db->get_bus_complex_voltage_in_pu(get_generator_bus());

    if(generator_model!=NULL)
    {
        check_and_initialize_generator_model_source_impedance();
        double mbase = get_mbase_in_MVA();
        S = S/mbase;
        generator_model->set_initial_terminal_complex_power_in_pu_based_on_mbase(S);
        generator_model->set_terminal_complex_voltage_in_pu(V);
        generator_model->initialize();

        if(exciter_model!=NULL)
        {
            double efd = generator_model->get_excitation_voltage_in_pu();
            exciter_model->set_excitation_voltage_in_pu(efd);
            exciter_model->set_generator_terminal_voltage_in_pu(abs(V));
            exciter_model->initialize();

            if(stabilizer_model!=NULL)
            {
                double vpss = exciter_model->get_stabilizing_signal_in_pu();
                stabilizer_model->set_stabilizing_signal_in_pu(vpss);
                set_stabilizer_input_signals();
                stabilizer_model->initialize();
            }
        }
        if(turbine_governor_model!=NULL)
        {
            double pmech = generator_model->get_mechanical_power_in_pu_base_on_mbase();
            turbine_governor_model->set_mechanical_power_in_pu(pmech);
            turbine_governor_model->initialize();
        }
    }
}
*/
void GENERATOR::check_and_initialize_generator_model_source_impedance()
{
    size_t index = get_sync_generator_model_index();
    SYNC_GENERATOR_MODEL* generator_model = (SYNC_GENERATOR_MODEL*) STEPS::model_db.get_model_of_index(index);

    if(generator_model==NULL)
        return;

    complex<double> dynamic_Zsource = generator_model->get_source_impedance_in_pu_based_on_mbase();
    complex<double> Zsource = get_source_impedance_in_pu();
    if(dynamic_Zsource==0.0)
    {
        generator_model->set_source_impedance_in_pu_based_on_mbase(Zsource);
    }
    else
    {
        if(dynamic_Zsource!=Zsource)
        {
            char info[256];
            sprintf(info,"ERROR. %s has different source impedance[%f+j%f] from its dynamic generator model[%f+j%f].",
                    get_device_name().c_str(), Zsource.real(), Zsource.imag(),
                    dynamic_Zsource.real(), dynamic_Zsource.imag()) ;
            show_information_with_leading_time_stamp(info);
        }
    }
}
void GENERATOR::set_stabilizer_input_signals()
{
    if(stabilizer_model==NULL)
        return;

    for(size_t i=0; i<MAX_STABILIZER_INPUT_SIGNAL; i++)
        set_stabilizer_input_signal_of_slot(i);
}

void GENERATOR::set_stabilizer_input_signal_of_slot(size_t i)
{
    char info[256];

    if(stabilizer_model==NULL)
        return;

    if(i>=MAX_STABILIZER_INPUT_SIGNAL)
        return;


    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(db==NULL)
    {
        sprintf(info, "%s is not assigned to any power system database.\n"
                      "Bus related input signals will not be set for its stabilizer.",
                      get_device_name().c_str());
        show_information_with_leading_time_stamp(info);
        return;
    }

    string signal_type = stabilizer_model->get_input_signal_type_of_slot(i);
    size_t signal_bus = stabilizer_model->get_input_signal_bus_of_slot(i);
    double signal_value = 0.0;
    bool signal_is_valid = false;

    if(signal_type=="ROTOR SPEED DEVIATION")
    {
        signal_value = generator_model->get_rotor_speed_deviation_in_pu();
        signal_is_valid = true;
    }
    if((not signal_is_valid) and signal_type=="TERMINAL ELECTRICAL POWER")
    {
        signal_value = generator_model->get_terminal_active_power_in_pu_based_on_mbase();
        signal_is_valid = true;
    }
    if((not signal_is_valid) and signal_type=="ACCELERATING POWER")
    {
        signal_value = generator_model->get_mechanical_power_in_pu_base_on_mbase();
        signal_value -=generator_model->get_air_gap_power_in_pu_based_on_mbase();
        signal_is_valid = true;
    }


    if((not signal_is_valid) and signal_type=="BUS FREQUENCY DEVIATION")
    {
        BUS* bus = db->get_bus(signal_bus);
        if(bus==NULL)
            return;

        signal_value = bus->get_frequency_deviation_in_pu();
        signal_is_valid = true;
    }
    if((not signal_is_valid) and signal_type=="BUS VOLTAGE")
    {
        BUS* bus = db->get_bus(signal_bus);
        if(bus==NULL)
            return;

        signal_value = bus->get_voltage_in_pu();
        signal_is_valid = true;
    }
    if(signal_is_valid)
        stabilizer_model->set_input_signal_bus_of_slot(i, signal_value);
}


void GENERATOR::run(DYNAMIC_MODE mode)
{
    char info[256];

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    if(db==NULL)
    {
        sprintf(info, "%s is not assigned to any power system database.\n"
                      "Dynamics simulation will not be performed for the generator.",
                      get_device_name().c_str());
        show_information_with_leading_time_stamp(info);
        return;
    }

    complex<double> V = db->get_bus_complex_voltage_in_pu(get_generator_bus());

    if(turbine_governor_model!=NULL)
    {
        double speed = generator_model->get_rotor_speed_deviation_in_pu();
        turbine_governor_model->set_generator_rotor_speed_deviation_in_pu(speed);
        turbine_governor_model->run(mode);
        double pmech = turbine_governor_model->get_mechanicla_power_in_pu();
        if(generator_model!=NULL)
            generator_model->set_mechanical_power_in_pu_base_on_mbase(pmech);
    }
    if(stabilizer_model!=NULL)
    {
        set_stabilizer_input_signals();
        stabilizer_model->run(mode);
        if(exciter_model!=NULL)
        {
            double vpss = stabilizer_model->get_stabilizeing_signal_in_pu();
            exciter_model->set_stabilizing_signal_in_pu(vpss);
        }
    }
    if(exciter_model!=NULL)
    {
        exciter_model->set_generator_terminal_voltage_in_pu(abs(V));
        exciter_model->run(mode);
        double efd = exciter_model->get_excitation_voltage_in_pu();
        if(generator_model!=NULL)
            generator_model->set_excitation_voltage_in_pu(efd);
    }
    if(generator_model!=NULL)
        generator_model->run(mode);
}


complex<double> GENERATOR::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    char info[256];

    if(get_status()==false or generator_model==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(db==NULL)
    {
        sprintf(info, "%s is not assigned to any power system database.\n"
                      "Source dynamic current in pu based on system base power be returned as 0.0.",
                      get_device_name().c_str());
        show_information_with_leading_time_stamp(info);
        return 0.0;
    }

    complex<double> I = generator_model->get_terminal_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();
    double sbase = db->get_system_base_power_in_MVA();
    I *= (mbase/sbase);
    return I;
}
