#ifndef PV_PANEL_MODEL_H
#define PV_PANEL_MODEL_H

#include "header/model/pvu_models/pvu_model.h"
#include <cstdlib>

class PV_PANEL_MODEL : public PVU_MODEL
{
    /*  aero dynamic model for WTG
        inputs:
            vwind: wind speed, in pu or in mps
            beta: pitch angle, in deg
            speed: wt rotor speed, in pu, or rad/s. not speed deviation
    */
    public:
        PV_PANEL_MODEL(STEPS& toolkit);
        PV_PANEL_MODEL(const PV_PANEL_MODEL& model);
        virtual ~PV_PANEL_MODEL();
        virtual PV_PANEL_MODEL& operator=(const PV_PANEL_MODEL& model);
        void copy_from_const_model(const PV_PANEL_MODEL& model);
    public:
        virtual string get_model_type() const;

        void set_S0_in_Wpm2(double S);
        void set_Sref_in_Wpm2(double S);
        void set_Tref_in_C_deg(double T);

        double get_S0_in_Wpm2() const;
        double get_Sref_in_Wpm2() const;
        double get_Tref_in_C_deg() const;
        double get_temperature_in_C_deg() const;

        double get_solar_irradiance_in_Wpm2();
        double get_solar_irradiance_in_pu_based_on_Sref();
        double get_solar_irradiance_in_pu_based_on_S0();
        double get_pv_unit_terminal_active_power_generation_in_pu_based_on_mbase() const;
        double get_active_power_generation_including_stator_loss_in_MW() const;

        virtual double get_Pref_in_pu_base_on_mbase() = 0;
        virtual double get_maximum_available_power_extracted_power_from_irradiance_in_pu_on_mbase() = 0;
        double get_maximum_available_power_extracted_power_from_irradiance_in_MW();
    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:

        double S0_Wpm2, Sref_Wpm2, Tref_in_C_deg;
};

#endif // PV_PANEL_MODEL_H
