#ifndef PV_PANEL_MODEL_H
#define PV_PANEL_MODEL_H

#include "header/model/pvu_models/pvu_model.h"
#include <cstdlib>

enum PV_MODE
{
    PV_UNDERSPEED_MODE = -1,
    PV_MPPT_MODE = 0,
    PV_OVERSPEED_MODE = 1
};


class PV_PANEL_MODEL : public PVU_MODEL
{
    /*  aero dynamic model for WTG
        inputs:
            vwind: wind speed, in pu or in mps
            beta: pitch angle, in deg
            speed: wt rotor speed, in pu, or rad/s. not speed deviation
    */
    public:
        PV_PANEL_MODEL();
        PV_PANEL_MODEL(const PV_PANEL_MODEL& model);
        virtual ~PV_PANEL_MODEL();
        virtual PV_PANEL_MODEL& operator=(const PV_PANEL_MODEL& model);
        void copy_from_const_model(const PV_PANEL_MODEL& model);
    public:
        virtual string get_model_type() const;
    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void set_block_toolkit() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_Cp(double lambda, double pitch_deg) const = 0;
        virtual double get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
};

#endif // PV_PANEL_MODEL_H
