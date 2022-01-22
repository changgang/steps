#ifndef VSC_HVDC_MODEL_H_INCLUDED
#define VSC_HVDC_MODEL_H_INCLUDED

#include "header/basic/timer.h"
#include "header/basic/steps_enum.h"
#include "header/model/model.h"
#include "header/block/integral_block.h"
#include <complex>

class VSC_HVDC;
class BUS;

class VSC_HVDC_MODEL : public MODEL
{
    public:
        VSC_HVDC_MODEL(STEPS& toolkit);
        virtual ~VSC_HVDC_MODEL();
        VSC_HVDC* get_multi_vsc_pointer() const;
    public: // hvdc common
        virtual string get_model_type() const;
        // get input for initialization

    public: // specific exciter

        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;
        void set_common_timer_toolkit();

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void check_blocking_logic() = 0;
        virtual void check_bypassing_logic() = 0;
        virtual void check_mode_switching_logic() = 0;
        virtual void solve_hvdc_model_without_integration() = 0;
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

};
#endif // VSC_HVDC_MODEL_H_INCLUDED
