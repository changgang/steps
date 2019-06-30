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

        void load_one_model(string data);

        virtual void export_powerflow_data(string file, bool export_zero_impedance_line=true, bool export_out_of_service_bus=true);
        virtual void export_dynamic_data(string file);
        virtual void export_sequence_data(string file);
    private:
        string trim_psse_comment(string str);

        void load_powerflow_data_into_ram(string file);
        vector<vector<vector<string> > > convert_psse_raw_data2steps_vector() const;

        vector<vector<string> > convert_i_th_type_data2steps_vector(size_t i) const;
        vector<vector<string> > convert_case_data2steps_vector() const;
        vector<vector<string> > convert_bus_data2steps_vector() const;
        vector<vector<string> > convert_load_data2steps_vector() const;
        vector<vector<string> > convert_fixed_shunt_data2steps_vector() const;
        vector<vector<string> > convert_source_data2steps_vector() const;
        vector<vector<string> > convert_line_data2steps_vector() const;
        vector<vector<string> > convert_transformer_data2steps_vector() const;
        vector<vector<string> > convert_hvdc_data2steps_vector() const;
        vector<vector<string> > convert_area_data2steps_vector() const;
        vector<vector<string> > convert_vsc_hvdc_data2steps_vector() const;
        vector<vector<string> > convert_transformer_inpedance_correction_table_data2steps_vector() const;
        vector<vector<string> > convert_multi_terminal_hvdc_data2steps_vector() const;
        vector<vector<string> > convert_multi_section_line_data2steps_vector() const;
        vector<vector<string> > convert_zone_data2steps_vector() const;
        vector<vector<string> > convert_interarea_transfer_data2steps_vector() const;
        vector<vector<string> > convert_owner_data2steps_vector() const;
        vector<vector<string> > convert_facts_data2steps_vector() const;
        vector<vector<string> > convert_switched_shunt_data2steps_vector() const;

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

        size_t data_version;
        vector< vector<string> > raw_data_in_ram;
        vector<string> dyr_data_in_ram;
};

#endif // POWERFLOW_ASSEMBLER_H
