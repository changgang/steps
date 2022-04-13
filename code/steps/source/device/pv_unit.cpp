#include "header/device/pv_unit.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/model/pvu_models/pv_converter_model/pv_converter_models.h"
#include "header/model/pvu_models/pv_panel_model/pv_panel_models.h"
#include "header/model/pvu_models/pv_electrical_model/pv_electrical_models.h"
#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_models.h"

#include <istream>
#include <iostream>

using namespace std;

PV_UNIT::PV_UNIT(STEPS& toolkit) : SOURCE(toolkit)
{
    clear();
}

PV_UNIT::~PV_UNIT()
{
}

void PV_UNIT::clear()
{
    SOURCE::clear();
    set_number_of_lumped_pv_units(1);
    set_rated_power_per_pv_unit_in_MW(0.0);

    pv_converter_model = NULL;
    pv_panel_model = NULL;
    pv_electrical_model = NULL;
    pv_irradiance_model = NULL;

    set_positive_sequence_resistance_in_pu(0.0);
    set_positive_sequence_reactance_in_pu(0.0);
    set_negative_sequence_resistance_in_pu(0.0);
    set_negative_sequence_reactance_in_pu(0.0);
    set_zero_sequence_resistance_in_pu(0.0);
    set_zero_sequence_reactance_in_pu(0.0);
    set_grounding_resistance_in_pu(0.0);
    set_grounding_reactance_in_pu(0.0);
}

DEVICE_ID PV_UNIT::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_PV_UNIT);

    TERMINAL terminal;
    terminal.append_bus(get_unit_bus());
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}


void PV_UNIT::set_unit_bus(unsigned int bus)
{
    set_source_bus(bus);
}

unsigned int PV_UNIT::get_unit_bus() const
{
    return get_source_bus();
}

void PV_UNIT::set_number_of_lumped_pv_units(unsigned int n)
{
    if(n==0)
        n = 1;
    number_of_lumped_pv_units = n;
}

void PV_UNIT::set_rated_power_per_pv_unit_in_MW(double P)
{
    rated_power_per_pv_unit_in_MW = P;
}

unsigned int PV_UNIT::get_number_of_lumped_pv_units() const
{
    return number_of_lumped_pv_units;
}

double PV_UNIT::get_rated_power_per_pv_unit_in_MW() const
{
    return rated_power_per_pv_unit_in_MW;
}

void PV_UNIT::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    if(get_status()==true)
    {
        PV_CONVERTER_MODEL* conv = get_pv_converter_model();
        PV_ELECTRICAL_MODEL* elec = get_pv_electrical_model();
        PV_PANEL_MODEL* panel = get_pv_panel_model();
        PV_IRRADIANCE_MODEL* irrd = get_pv_irradiance_model();

        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                if(conv!=NULL)
                    conv->initialize();
                else
                {
                    osstream<<"Error. No PV CONVERTER model is provided for "<<get_compound_device_name()<<" for dynamic initialization.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }

                if(irrd!=NULL)
                    irrd->initialize();

                if(panel!=NULL)
                    panel->initialize();

                if(elec!=NULL)
                    elec->initialize();

                break;
            }
            default:
            {
                if(irrd!=NULL and irrd->is_model_active())
                    irrd->run(mode);

                if(elec!=NULL and elec->is_model_active())
                    elec->run(mode);

                if(panel!=NULL and panel->is_model_active())
                    panel->run(mode);

                if(conv!=NULL and conv->is_model_active())
                    conv->run(mode);
                break;
            }
        }
    }
}


void PV_UNIT::report() const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_compound_device_name()<<": "<<(get_status()==true?"in service":"out of service")<<", "
      <<"MBASE = "<<setw(6)<<setprecision(2)<<fixed<<get_mbase_in_MVA()<<" MVA"<<endl
      <<"P = "<<setw(8)<<setprecision(4)<<fixed<<get_p_generation_in_MW()<<" MW, "
      <<"Pmax = "<<setw(8)<<setprecision(4)<<fixed<<get_p_max_in_MW()<<" MW, "
      <<"Pmin = "<<setw(8)<<setprecision(4)<<fixed<<get_p_min_in_MW()<<" MW"<<endl
      <<"Q = "<<setw(8)<<setprecision(4)<<fixed<<get_q_generation_in_MVar()<<" MVar, "
      <<"Qmax = "<<setw(8)<<setprecision(4)<<fixed<<get_q_max_in_MVar()<<" MVar, "
      <<"Qmin = "<<setw(8)<<setprecision(4)<<fixed<<get_q_min_in_MVar()<<" MVar"<<endl
      <<"Zsource = "<<setw(8)<<setprecision(6)<<fixed<<get_source_impedance_in_pu();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PV_UNIT::save() const
{
    ;
}

void PV_UNIT::set_model(MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type(STEPS_PV_UNIT))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="PV CONVERTER")
        {
            set_pv_converter_model((PV_CONVERTER_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="PV PANEL")
        {
            set_pv_panel_model((PV_PANEL_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="PV ELECTRICAL")
        {
            set_pv_electrical_model((PV_ELECTRICAL_MODEL*) model);
            return;
        }

        if(model->get_model_type()=="PV IRRADIANCE")
        {
            set_pv_irradiance_model((PV_IRRADIANCE_MODEL*) model);
            return;
        }

        ostringstream osstream;
        osstream<<"Warning. Unsupported model type '"<<model->get_model_type()<<"' when setting up pv unit-related model.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* PV_UNIT::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="PV CONVERTER")
        return get_pv_converter_model();
    if(model_type=="PV PANEL")
        return get_pv_panel_model();
    if(model_type=="PV ELECTRICAL")
        return get_pv_electrical_model();
    if(model_type=="PV IRRADIANCE")
        return get_pv_irradiance_model();
    return NULL;
}

void PV_UNIT::set_pv_converter_model(PV_CONVERTER_MODEL* model)
{
    if(model!=NULL)
        pv_converter_model = model;
}

void PV_UNIT::set_pv_panel_model(PV_PANEL_MODEL* model)
{
    if(model!=NULL)
        pv_panel_model = model;
}

void PV_UNIT::set_pv_electrical_model(PV_ELECTRICAL_MODEL* model)
{
    if(model!=NULL)
        pv_electrical_model = model;
}

void PV_UNIT::set_pv_irradiance_model(PV_IRRADIANCE_MODEL* model)
{
    if(model!=NULL)
        pv_irradiance_model = model;
}

PV_CONVERTER_MODEL* PV_UNIT::get_pv_converter_model() const
{
    return pv_converter_model;
}

PV_PANEL_MODEL* PV_UNIT::get_pv_panel_model() const
{
    return pv_panel_model;
}

PV_ELECTRICAL_MODEL* PV_UNIT::get_pv_electrical_model() const
{
    return pv_electrical_model;
}

PV_IRRADIANCE_MODEL* PV_UNIT::get_pv_irradiance_model() const
{
    return pv_irradiance_model;
}

PV_UNIT& PV_UNIT::operator=(const PV_UNIT& pvu)
{
    if(this==(&pvu)) return *this;

    set_toolkit(pvu.get_toolkit());
    clear();

    set_unit_bus(pvu.get_unit_bus());
    set_identifier(pvu.get_identifier());
    set_status(pvu.get_status());
    set_mbase_in_MVA(pvu.get_mbase_in_MVA());
    set_p_generation_in_MW(pvu.get_p_generation_in_MW());
    set_q_generation_in_MVar(pvu.get_q_generation_in_MVar());
    set_p_max_in_MW(pvu.get_p_max_in_MW());
    set_p_min_in_MW(pvu.get_p_min_in_MW());
    set_q_max_in_MVar(pvu.get_q_max_in_MVar());
    set_q_min_in_MVar(pvu.get_q_min_in_MVar());
    set_source_impedance_in_pu(pvu.get_source_impedance_in_pu());
    set_bus_to_regulate(pvu.get_bus_to_regulate());
    set_voltage_to_regulate_in_pu(pvu.get_voltage_to_regulate_in_pu());

    set_number_of_lumped_pv_units(pvu.get_number_of_lumped_pv_units());
    set_rated_power_per_pv_unit_in_MW(pvu.get_rated_power_per_pv_unit_in_MW());

    set_model(pvu.get_pv_converter_model());
    set_model(pvu.get_pv_panel_model());
    set_model(pvu.get_pv_electrical_model());
    set_model(pvu.get_pv_irradiance_model());

    set_positive_sequence_resistance_in_pu(pvu.get_positive_sequence_resistance_in_pu());
    set_positive_sequence_reactance_in_pu(pvu.get_positive_sequence_reactance_in_pu());
    set_negative_sequence_resistance_in_pu(pvu.get_negative_sequence_resistance_in_pu());
    set_negative_sequence_reactance_in_pu(pvu.get_negative_sequence_reactance_in_pu());
    set_zero_sequence_resistance_in_pu(pvu.get_zero_sequence_resistance_in_pu());
    set_zero_sequence_reactance_in_pu(pvu.get_zero_sequence_reactance_in_pu());
    set_grounding_resistance_in_pu(pvu.get_grounding_resistance_in_pu());
    set_grounding_reactance_in_pu(pvu.get_grounding_reactance_in_pu());

    return *this;
}

complex<double> PV_UNIT::get_complex_internal_voltage_in_pu_in_xy_axis() const
{
    return 0.0;
}

complex<double> PV_UNIT::get_source_dynamic_current_in_pu_based_on_system_base_power()
{
    return 0.0;
}

void PV_UNIT::set_positive_sequence_resistance_in_pu(double r)
{
    R1 = r;
}

void PV_UNIT::set_positive_sequence_reactance_in_pu(double x)
{
    X1  =x;
}

void PV_UNIT::set_negative_sequence_resistance_in_pu(double r)
{
    R2 = r;
}

void PV_UNIT::set_negative_sequence_reactance_in_pu(double x)
{
    X2 = x;
}

void PV_UNIT::set_zero_sequence_resistance_in_pu(double r)
{
    R0 = r;
}

void PV_UNIT::set_zero_sequence_reactance_in_pu(double x)
{
    X0 = x;
}

void PV_UNIT::set_grounding_resistance_in_pu(double r)
{
    Rground = r;
}

void PV_UNIT::set_grounding_reactance_in_pu(double x)
{
    Xground = x;
}

double PV_UNIT::get_positive_sequence_resistance_in_pu() const
{
    return R1;
}

double PV_UNIT::get_positive_sequence_reactance_in_pu() const
{
    return X1;
}

double PV_UNIT::get_negative_sequence_resistance_in_pu() const
{
    return R2;
}

double PV_UNIT::get_negative_sequence_reactance_in_pu() const
{
    return X2;
}

double PV_UNIT::get_zero_sequence_resistance_in_pu() const
{
    return R0;
}

double PV_UNIT::get_zero_sequence_reactance_in_pu() const
{
    return X0;
}

double PV_UNIT::get_grounding_resistance_in_pu() const
{
    return Rground;
}

double PV_UNIT::get_grounding_reactance_in_pu() const
{
    return Xground;
}

complex<double> PV_UNIT::get_positive_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R1 = get_positive_sequence_resistance_in_pu();
    double X1 = get_positive_sequence_reactance_in_pu();
    complex<double> Z1 = complex<double>(R1, X1)*one_over_mbase*sbase;
    complex<double> U1 = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = U1/Z1;

    return I;
}

complex<double> PV_UNIT::get_negative_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R2 = get_negative_sequence_resistance_in_pu();
    double X2 = get_negative_sequence_reactance_in_pu();
    complex<double> Z2 = complex<double>(R2, X2)*one_over_mbase*sbase;
    complex<double> U2 = psdb.get_bus_negative_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = U2/Z2;

    return I;
}

complex<double> PV_UNIT::get_zero_sequence_complex_current_in_pu()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    double R0 = get_zero_sequence_resistance_in_pu();
    double X0 = get_zero_sequence_reactance_in_pu();
    complex<double> Z0 = complex<double>(R0, X0)*one_over_mbase*sbase;
    complex<double> U0 = psdb.get_bus_zero_sequence_complex_voltage_in_pu(get_source_bus());
    complex<double> I = U0/Z0;

    return I;
}

complex<double> PV_UNIT::get_positive_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_positive_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> PV_UNIT::get_negative_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_negative_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}

complex<double> PV_UNIT::get_zero_sequence_complex_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    return get_zero_sequence_complex_current_in_pu()*sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_source_bus()));
}




