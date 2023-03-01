#ifndef DATA_IMEXPORTER_H
#define DATA_IMEXPORTER_H

#include "header/power_system_database.h"
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;
class DATA_IMEXPORTER
{
    public:
        DATA_IMEXPORTER(STEPS& toolkit);
        virtual ~DATA_IMEXPORTER();

        STEPS& get_toolkit() const;

        void set_base_frequency_in_Hz(double fbase);
        void set_export_zero_impedance_line_logic(bool logic);
        void set_export_out_of_service_bus_logic(bool logic);
        void set_export_internal_bus_number_logic(bool logic);
        void set_powerflow_data_save_mode(POWERFLOW_DATA_SAVE_MODE mode);
        void set_supplement_sequence_model_from_dynamic_model_logic(bool logic);
        double get_base_frequency_in_Hz() const;
        bool get_export_zero_impedance_line_logic() const;
        bool get_export_out_of_service_bus_logic() const;
        bool get_export_internal_bus_number_logic() const;
        POWERFLOW_DATA_SAVE_MODE get_powerflow_data_save_mode() const;
        bool get_supplement_sequence_model_from_dynamic_model_logic() const;


        void export_shadowed_bus_pair(string file) const;

        virtual void load_powerflow_data(string pf_source) = 0;
        virtual void load_powerflow_result(string pf_source) = 0;
        virtual void load_dynamic_data(string dy_source) = 0;
        virtual void load_sequence_data(string sq_source) = 0;

        virtual void export_powerflow_data(string file, bool export_zero_impedance_line=true, POWERFLOW_DATA_SAVE_MODE save_mode = SAVE_TO_KEEP_ORIGINAL_BUS_ORDER) = 0;
        virtual void export_dynamic_data(string file) = 0;
        virtual void export_sequence_data(string file) = 0;

        void setup_ordered_buses_with_powerflow_data_save_mode(POWERFLOW_DATA_SAVE_MODE mode);
        const vector<unsigned int>& get_ordered_buses() const;

        void check_and_supplement_sequence_model_from_dynamic_model();
    private:
        void setup_ordered_buses_with_mode_SAVE_TO_KEEP_ORIGINAL_BUS_ORDER();
        void setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NUMBER();
        void setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_BUS_NAME();
        void setup_ordered_buses_with_mode_SAVE_TO_ORDER_BUS_WITH_DYNAMIC_DEVICE_ORDER();
        vector<unsigned int> get_all_generator_buses();
        vector<unsigned int> get_all_wt_generator_buses();
        vector<unsigned int> get_all_pv_unit_buses();
        vector<unsigned int> get_all_energy_storage_buses();
        vector<unsigned int> get_all_load_buses();
        vector<unsigned int> get_all_hvdc_buses();
        vector<unsigned int> get_all_fixed_shunt_buses();
        void append_buses_to_ordered_buses(const vector<unsigned int>& buses, set<unsigned int>& existing_buses);

        void check_line_sequence_data();
        void check_transformer_sequence_data();
        void check_fixed_shunt_sequence_data();

        void check_and_supplement_load_sequence_data();
        void check_and_supplement_generator_sequence_data();
        void check_and_supplement_wt_generator_sequence_data();
        void check_and_supplement_pv_unit_sequence_data();
        void check_and_supplement_energy_storage_sequence_data();
        void check_and_supplement_vsc_hvdc_sequence_data();

        void supplement_load_sequence_model_from_dynamic_model(LOAD& load);
        void supplement_generator_sequence_model_from_dynamic_model(GENERATOR& gen);
        void supplement_wt_generator_sequence_model_from_dynamic_model(WT_GENERATOR& wt_gen);
        void supplement_vsc_hvdc_sequence_model_from_dynamic_model(VSC_HVDC& vsc_hvdc);

        STEPS* toolkit;

        double base_frequency_in_Hz;
        bool export_zero_impedance_line;
        bool export_out_of_service_bus;
        bool export_internal_bus_number;
        POWERFLOW_DATA_SAVE_MODE powerflow_data_save_mode;
        bool supplement_sequence_model_from_dynamic_model_logic;

        vector<unsigned int> ordered_buses_to_export;
};

#endif // DATA_IMEXPORTER_H
