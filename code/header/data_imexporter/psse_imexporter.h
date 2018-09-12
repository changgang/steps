#ifndef PSSE_IMEXPORTER_H
#define PSSE_IMEXPORTER_H

#include "header/data_imexporter/data_imexporter.h"
#include "header/device/source.h"
#include "header/device/transformer.h"
#include "header/device/hvdc.h"
#include <vector>
class PSSE_IMEXPORTER : public DATA_IMEXPORTER
{
    public:
        PSSE_IMEXPORTER();
        ~PSSE_IMEXPORTER();

        virtual void load_powerflow_data(string file);
        virtual void load_dynamic_data(string dy_source);
        virtual void load_sequence_data(string sq_source);

        virtual void export_powerflow_data(string file);
        virtual void export_dynamic_data(string file);
        virtual void export_sequence_data(string file);

        size_t get_data_version() const;

    private:
        string trim_psse_comment(string str);
        void set_data_version(size_t version);

        void load_powerflow_data_into_ram(string file);

        void load_case_data();
        void load_bus_data();
        void load_load_data();
        void load_fixed_shunt_data();
        void load_source_data();
        void load_generator_data(vector<string>& data);
        void load_wt_generator_data(vector<string>& data);
        void load_pv_unit_data(vector<string>& data);
        void load_energy_storage_data(vector<string>& data);
        void load_source_common_data(vector<string>& data, SOURCE* source);
        void load_source_var_control_data(vector<string>& data, SOURCE* source);
        void load_line_data();
        void load_transformer_data();
        void add_transformer_with_data(vector<string> trans_data);
        void add_transformer_basic_data(TRANSFORMER& trans, vector<string> data);
        void add_transformer_winding_data(TRANSFORMER&trans, TRANSFORMER_WINDING_SIDE winding, vector<string> data, TRANSFORMER_WINDING_TAP_CODE winding_code);
        void add_transformer_impedance_admittance_data(TRANSFORMER& trans, vector<string> data, TRANSFORMER_IMPEDANCE_CODE impedance_code, TRANSFORMER_ADMITTANCE_CODE magnetizing_code);
        void load_area_data();
        void load_hvdc_data();
        void add_hvdc_with_data(vector<string> hvdc_data);
        void add_hvdc_basic_data(HVDC& hvdc, vector<string> data);
        void add_hvdc_converter_data(HVDC& hvdc, HVDC_CONVERTER_SIDE converter, vector<string> data);
        void load_vsc_hvdc_data();
        void load_transformer_impedance_correction_table_data();
        void load_multi_terminal_hvdc_data();
        void load_multi_section_line_data();
        void load_zone_data();
        void load_interarea_transfer_data();
        void load_owner_data();
        void load_facts_data();
        void load_switched_shunt_data();

        string export_case_data() const;
        string export_bus_data() const;
        string export_load_data() const;
        string export_fixed_shunt_data() const;
        string export_source_data() const;
        string export_generator_data() const;
        string export_wt_generator_data() const;
        string export_pv_unit_data() const;
        string export_energy_storage_data() const;
        string export_source_common_data(SOURCE* source) const;
        string export_source_var_control_data(SOURCE* source) const;
        string export_line_data() const;
        string export_transformer_data() const;
        string export_area_data() const;
        string export_hvdc_data() const;
        string export_vsc_hvdc_data() const;
        string export_transformer_impedance_correction_table_data() const;
        string export_multi_terminal_hvdc_data() const;
        string export_multi_section_line_data() const;
        string export_zone_data() const;
        string export_interarea_transfer_data() const;
        string export_owner_data() const;
        string export_facts_data() const;
        string export_switched_shunt_data() const;


        void load_dynamic_data_into_ram(string file);

        void load_all_models();
        void load_one_model(string data);
        string get_dynamic_model_name(string data);

        void add_GENCLS_model(string data);
        void add_GENROU_model(string data);
        void add_GENSAL_model(string data);

        void add_COMP_model(string data);

        void add_IEE2ST_model(string data);

        void add_SEXS_model(string data);
        void add_IEEET1_model(string data);
        void add_PSASPE1_model(string data);
        void add_PSASPE2_model(string data);
        void add_CSEET1_model(string data);
        void add_CSEET2_model(string data);
        void add_PSASPE13_model(string data);

        void add_TGOV1_model(string data);
        void add_IEEEG1_model(string data);
        void add_IEEEG2_model(string data);
        void add_IEEEG3_model(string data);
        void add_IEESGO_model(string data);


        vector<LOAD*> get_all_loads_of(string data);
        vector<LOAD*> remove_loads_with_different_identifier(vector<LOAD*> loads, string identifier);

        void add_IEEL_model(string data);

        void add_UFLS_model(string data);
        void add_PUFLS_model(string data);

        void add_CDC4T_model(string data);
        void add_CDC6T_model(string data);

        void add_WT3G2_model(string data);
        void add_AERD0_model(string data);
        void add_WT3T0_model(string data);
        void add_WT3E0_model(string data);
        void add_WT3P0_model(string data);
        void add_FILEWIND_model(string data);

        DEVICE_ID get_generator_device_id_from_string(string data);
        DEVICE_ID get_wt_generator_device_id_from_string(string data);
        DEVICE_ID get_load_device_id_from_string(string data);
        DEVICE_ID get_line_device_id_from_string(string data);
        DEVICE_ID get_hvdc_device_id_from_string(string data);
        DEVICE_ID get_transformer_device_id_from_string(string data);


        void load_source_dynamic_data();
        void load_exciter_dynamic_data();
        void load_stabilizer_dynamic_data();
        void load_turbine_governor_dynamic_data();
        void load_load_dynamic_data();
        void load_line_dynamic_data();

        size_t data_version;
        vector< vector<string> > raw_data_in_ram;
        vector<string> dyr_data_in_ram;


};

#endif // POWERFLOW_ASSEMBLER_H
