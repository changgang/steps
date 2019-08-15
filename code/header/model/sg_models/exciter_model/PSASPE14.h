#ifndef PSASPE14_H
#define PSASPE14_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"

class PSASPE14: public EXCITER_MODEL
{
    public:
        PSASPE14();
        PSASPE14(const PSASPE14& model);
        virtual ~PSASPE14();
        virtual PSASPE14& operator=(const PSASPE14& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_Tr_in_s(double T);
        void set_Ka(double K);
        void set_Ta_in_s(double T);
        void set_Kp(double K);
        void set_Ki(double K);
        void set_Vrmax_in_pu(double vmax);
        void set_Vrmin_in_pu(double vmin);
        void set_Kifd(double K);
        void set_Tifd_in_s(double T);
        void set_IKp(double K);
        void set_IKi(double K);
        void set_Vfmax_in_pu(double vmax);
        void set_Vfmin_in_pu(double vmin);
        void set_Kt(double K);
        void set_Tt_in_s(double T);
        void set_Efdmax_in_pu(double emax);
        void set_Efdmin_in_pu(double emin);

        double get_Tr_in_s() const;
        double get_Ka() const;
        double get_Ta_in_s() const;
        double get_Kp() const;
        double get_Ki() const;
        double get_Vrmax_in_pu() const;
        double get_Vrmin_in_pu() const;
        double get_Kifd() const;
        double get_Tifd_in_s() const;
        double get_IKp() const;
        double get_IKi() const;
        double get_Vfmax_in_pu() const;
        double get_Vfmin_in_pu() const;
        double get_Kt() const;
        double get_Tt_in_s() const;
        double get_Efdmax_in_pu() const;
        double get_Efdmin_in_pu() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_excitation_voltage_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const PSASPE14& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK sensor, regulator, ifd_feedback, rectifier;
        PI_BLOCK regulator_pi, current_pi;
        double Efdmax, Efdmin;
};

#endif // EXCITER_MODEL_H
