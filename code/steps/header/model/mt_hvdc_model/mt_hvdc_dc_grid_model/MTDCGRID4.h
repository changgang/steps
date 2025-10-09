#ifndef MTDCGRID4_H
#define MTDCGRID4_H


#include "header/model/mt_hvdc_model/mt_hvdc_dc_grid_model/mt_dc_grid_model.h"
#include "header/basic/steps_enum.h"
#include "header/basic/inphno.h"

class MTDCGRID4: public MT_DC_GRID_MODEL
{
    public:
        MTDCGRID4(STEPS& toolkit);
        ~MTDCGRID4();
        MTDCGRID4(const MTDCGRID4& model);
        virtual MTDCGRID4& operator=(const MTDCGRID4& model);
        void copy_from_const_model(const MTDCGRID4& model);

    public:

        virtual string get_model_name() const;
    public:
        void set_dc_network_simulation_time_step(double t_dc);
        void set_is_dc_network_dynamics_consider(bool is_dc_dynamic_consider);

        DC_LINE* get_dc_line_with_virtual_bus_number(unsigned int bus_number);
        DC_BUS* get_virtual_bus_with_virtual_bus_number(unsigned int bus_number);
        unsigned int get_number_of_dc_buses_with_dynamic_UR_type();
        bool is_internal_bus_in_U_type(unsigned int bus);
        bool check_is_dc_line_fault_exit();

        void initialize_dc_buses();
        void initialize_dc_lines();
        void initialize_dynamic_physical_internal_dc_bus_pair();
        void build_dc_network_matrix_with_dynamic();
        void build_dc_network_initial_zero_dynamic_matrix();
        void add_dc_line_to_dc_dynamic_network_matrix();
        void add_converter_to_dc_dynamic_network_matrix();
        void build_dc_network_matrix_without_dynamic();
        void split_dynamic_dc_network_matrix_to_4_sub_matrix();

        void initialize_dc_bus_injection_current();
        void add_dc_line_to_injection_current();
        void initialize_dc_bus_voltage_vector();
        void update_dc_network_matrix();
        void update_dynamic_physical_internal_dc_bus_pair();
        void update_dc_bus_injection_current();
        void update_dc_bus_voltage();


        void add_dc_fault_to_dc_network();

        virtual void solve_dc_network_without_line_dynamics();
        virtual void solve_dc_network_with_line_dynamics();
        void update_dc_bus_volatge_and_current();
        void update_dc_line_voltage_and_current();
        void run_a_step();

        double get_dc_network_simulation_time_step() const;
        bool get_is_dc_network_dynamics_consider() const;

    public:

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void check_limits_in_initialization();
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

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:

        //SHOW
        void show_dc_network_matrix();
        void show_sub_matrix_YCC();
        void show_sub_matrix_YCU();
        void show_sub_matrix_YUC();
        void show_sub_matrix_YUU();
        void show_dc_bus_voltage();
        void show_dc_bus_current();
        void show_bus_injection_current();




    public://input for initialize

    public://output


    private:
        double dc_network_simulation_time_step;
        double is_dc_network_dynamic_considered;

        bool is_dc_line_fault_exit;

        STEPS_SPARSE_MATRIX dynamic_dc_network_matrix;
        STEPS_SPARSE_MATRIX dynamic_submatrix_YCC,dynamic_submatrix_YCU,dynamic_submatrix_YUC,dynamic_submatrix_YUU ;
        INPHNO dynamic_inphno_dc_buses;
        vector<bool> is_internal_bus_virtual;
        vector<double> injection_current, bus_voltage;

};

#endif // HLCC_MODEL_H
