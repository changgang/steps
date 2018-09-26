#ifndef BPA_IMEXPORTER_H
#define BPA_IMEXPORTER_H

#include "header/data_imexporter/data_imexporter.h"
#include "header/device/source.h"
#include "header/device/transformer.h"
#include "header/device/hvdc.h"
#include <vector>

class BPA_IMEXPORTER : public DATA_IMEXPORTER
{
    public:
        BPA_IMEXPORTER();
        ~BPA_IMEXPORTER();

        virtual void load_powerflow_data(string file);
        virtual void load_dynamic_data(string dy_source);
        virtual void load_sequence_data(string sq_source);


        virtual void export_powerflow_data(string file);
        virtual void export_dynamic_data(string file);
        virtual void export_sequence_data(string file);


        string convert_data_into_bpa_format(string data, string format) const;
        string convert_data_into_bpa_format(double data, string format) const;
        string convert_data_into_bpa_format(size_t data, string format) const;
        string convert_data_into_bpa_format(int data, string format) const;

        void set_data_version(size_t version);
        size_t get_data_version() const;

    private:
        bool is_comment_line(string str);

        void load_powerflow_data_into_ram(string file);
        void format_all_powerflow_cards();
        string get_powerflow_card_type(string data) const;

        void load_dynamic_data_into_ram(string file);
        void format_all_dynamic_cards();
        string get_dynamic_card_type(string data) const;


        void classify_dynamic_cards();

        void update_bus_number_with_bus_name_and_number_pair_file(string file);

        string format_bpa_data_to_readable_data(string original_data, string format);
        string grow_string_to_at_least_size(string data, size_t least_size);

        void load_case_data();
        void load_area_data();
        void load_zone_data();
        void load_owner_data();
        void load_bus_data();
        void set_bus_area();
        void set_area_swing_bus();
        void load_load_and_fixed_shunt_data();
        void load_generator_data();
        void load_line_data();
        void load_fixed_shunt_data();
        void load_source_data();
        void load_wt_generator_data();
        void load_source_common_data();
        void load_transformer_data();
        void load_hvdc_data();

        string export_case_data() const;
        string export_bus_data() const;
        string export_line_data() const;
        string export_transformer_data() const;
        string export_two_winding_transformer(const TRANSFORMER* trans) const;
        string export_three_winding_transformer(const TRANSFORMER* trans) const;
        string export_hvdc_data() const;
        string export_vsc_hvdc_data() const;
        string export_multi_terminal_hvdc_data() const;
        string export_switched_shunt_data() const;


        void load_all_models();
        void load_one_model(string data);
        string get_dynamic_model_name(string data);

        vector<LOAD*> get_all_loads_of(string data);
        vector<LOAD*> remove_loads_with_different_identifier(vector<LOAD*> loads, string identifier);

        DEVICE_ID get_generator_device_id_from_string(string data);
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
        vector<string> dat_data_in_ram;
        vector<string> swi_data_in_ram;

        vector<string> case_cards, BA_BB_plus_cards, BS_cards, BD_cards, X_cards, L_Lplus_E_cards, T_TP_R_cards, LD_cards;

        vector<string> MC_MF_MG_cards, M_cards, E_IEEE_1968_cards, F_IEEE_1981_cards, F_CSEE_cards, PSS_cards, Turbine_Governor_cards,
        Turbine_cards, Governor_cards, HVDC_dynamic_cards, Load_dynamic_cards, UFLS_UVLS_cards, splitting_cards;





};

#endif // POWERFLOW_ASSEMBLER_H
