#ifndef VSCHVDCP0_H_INCLUDED
#define VSCHVDCP0_H_INCLUDED

#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/vsc_hvdc_network_model.h"
#include "header/basic/sparse_matrix_define.h"
#include "header/basic/inphno.h"

class VSCHVDCP0: public VSC_HVDC_NETWORK_MODEL
{
    public:
        VSCHVDCP0(STEPS& toolkit);
        VSCHVDCP0(const VSCHVDCP0& model);
        virtual ~VSCHVDCP0();
        virtual VSCHVDCP0& operator=(const VSCHVDCP0& model);
    public:
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        void set_model_data_with_index(unsigned int index, double value);
        double get_model_data_with_index(unsigned int index);

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);
        double get_model_internal_variable_with_index(unsigned int index);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

    public:
        //virtual void solve_vsc_hvdc_network();
        virtual void build_dynamic_dc_network_matrix();
    private:
        void initialize_converter_and_nonconverter_physical_internal_bus_pair();
        void build_initial_zero_matrix();
        void add_dc_lines_with_fault_to_dc_network();
        void split_dynamic_dc_network_matrix_to_4_sub_matrix();

        void solve_dynamic_network_with_quasi_steady_state_model();


    private:
        void copy_from_const_model(const VSCHVDCP0& model);
    private:
        STEPS_SPARSE_MATRIX dc_network_matrix;
        INPHNO inphno_all_buses;

        STEPS_SPARSE_MATRIX dc_network_submatrix_CC, dc_network_submatrix_CN, dc_network_submatrix_NC, dc_network_submatrix_NN;
        INPHNO inphno_converter_buses, inphno_nonconverter_buses;

        unsigned int iteration_count;
};
#endif // VSCHVDCP0_H_INCLUDED
